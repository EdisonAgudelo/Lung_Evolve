
//definitions for sofware I2C
#define I2C_TIMEOUT 10
#define I2C_PULLUP 1
#define SDA_PORT PORTC //review
#define SDA_PIN 7      // //review
#define SCL_PORT PORTC //review
#define SCL_PIN 5      //review

#include "SoftI2CMaster.h"

#include "hardware_interface.h"
#include <Wire.h>

//library version for arduino mega

/*
  Timer 5 woriking for millis and micros aplication
  Timer 1 , 3, 4 is for custom pwm genarator
  Timer 0 is used by flowmeter
*/

void (*timer1m_callback)(void)=nullptr;
void (*uart_callback)(void)=nullptr;

volatile bool g_pending_interrupt[6] = {false};

volatile uint8_t g_overflow_count[6] = {0};

uint32_t g_hard_counter = 0;
//I2C com

//read data from a I2C slave device
bool I2CRead(int id, uint8_t addres, uint8_t *buffer, uint8_t lenght)
{
  int i = 0;
  uint16_t time_out = 0;

  switch (id)
  {
  case kSoftI2C:
    
    if (!i2c_start_wait((addres << 1) | I2C_READ))
    {
      i2c_stop();
      return false;
    }
    
    while (0 != lenght--)
    {
    
      buffer[i++] = i2c_read(0 == lenght);
    }
    i2c_stop();
    break;

  case kHardI2C:

    // request data to a slave
    Wire.requestFrom(addres, lenght);

    do
    {
      time_out++;
      while (Wire.available())
      {
        time_out = 0;
        //for protection purpose
        if (i >= lenght)
          break;
        buffer[i++] = Wire.read();
      }
    } while (i < lenght && time_out < I2C_TIMEOUT*10);

    if (time_out >= I2C_TIMEOUT*10)
      return false;

    break;

  default:
    return false;
    break;
  }

  return true;
}

//write data from I2C slave divice.
bool I2CWrite(int id, uint8_t addres, uint8_t *buffer, uint8_t lenght)
{
  int i = 0;
  switch (id)
  {
  case kSoftI2C:
    

    if (!i2c_start_wait((addres << 1) | I2C_WRITE))
    {
      i2c_stop();
      return false;
    }
    while (0 != lenght--)
    {
      if (!i2c_write(buffer[i++]))
        break;
    }
    i2c_stop();

    //if it is a no complete transaction
    if (lenght != 0xff)
      return false;
    break;

  case kHardI2C:

    Wire.beginTransmission(addres);
    i = Wire.write(buffer, lenght);
    Wire.endTransmission();

    //if it is a no complete transaction
    if (i != lenght)
      return false;
    break;

  default:
    return false;
    break;
  }
  return true;
}

bool I2CBegin(int id)
{
  switch (id)
  {
  case kHardI2C:
    Wire.begin();
    break;
  case kSoftI2C:
    break;
  default:
    return false;
    break;
  }
  return true;
}


////////// Serial interface ////////
void UartConfigCallback(void (*callback)(void))
{
  uart_callback = callback;
}

void serialEvent() {
  if(uart_callback!=nullptr)
    uart_callback();
}

void UartBegin(uint32_t baudrate)
{
  Serial.begin(baudrate);
}

void UartWrite(uint8_t data)
{
  Serial.write(data);
}

uint8_t UartRead(void)
{
  return Serial.read();
}

bool UartAvailable(void){
  return Serial.available();
}

////////// GPIO interface //////////

void PinConfigRisingIRS(int pin, void (*callback)(void))
{
  attachInterrupt(digitalPinToInterrupt(pin), callback, RISING);
}

void PinConfigDigital(int pin, bool dir)
{
  if (kInput == dir)
    pinMode(pin, INPUT);
  else
    pinMode(pin, OUTPUT);
}

void PinConfigAnalog(int pin)
{
  pinMode(pin, INPUT);
}

bool PinReadDigital(int pin)
{
  return digitalRead(pin);
}

uint16_t PinReadAnalog(int pin)
{
  return analogRead(pin);
}

void PinSetDigital(int pin, bool level)
{
  if (kHigh == level)
    digitalWrite(pin, HIGH);
  else
    digitalWrite(pin, LOW);
}

//////////// Timers Interfacer /////////

uint16_t Timer1msCount(void)
{
  //return (uint16_t)TCNT2;
  return (((uint16_t)TCNT5H) << 8) + (uint16_t)TCNT5L;
}

void Timer1msISR(void (*callback)(void))
{
  timer1m_callback = callback;
  uint16_t compare_value = 16000;

  TCCR5A = 0x00; // mode 4, no outputs pin
  TCCR5B = 0x09; //preescaler value = 1. Ignore Input compare

  //this register define PWM frecuency
  OCR5AH = (compare_value >> 8) & 0x00ff; //config calculated compare value
  OCR5AL = (compare_value)&0x00ff;        //config calculated compare value

  TIMSK5 = 0x2; // intterrupt in compare match A
}

double PWMConfigFrecuency(uint32_t frecuency, int pwm_id)
{
  uint8_t i;

  const uint16_t preescaler[] = {1, 8, 64, 256, 1024};
  uint16_t compare_value;
  double pwm_period;
  double A;
  double B;

  cli();

  compare_value = 0x00ff;

  for (i = 0; i < (sizeof(preescaler) / sizeof(uint16_t)); i++)
  {
    if (((uint32_t)F_CPU / ((uint32_t)preescaler[i] * (uint32_t)0xffff)) <= frecuency)
    {
      compare_value = F_CPU / (preescaler[i] * frecuency);
      break; //break for
    }
  }

  A = (double)(compare_value+1);
  B = (double)(F_CPU / preescaler[i]);
  pwm_period = 1000000.0*A/B;

  g_pending_interrupt[pwm_id] = false;

  switch (pwm_id)
  {
  case 1:

    TCCR1A = 0x23;           // mode 15, com3b as normal PWM
    TCCR1B = (i + 1) | 0x18; //preescaler value + mode 15. Ignore Input compare

    //this register define PWM frecuency
    OCR1AH = (compare_value >> 8) & 0x00ff; //config calculated compare value
    OCR1AL = (compare_value)&0x00ff;        //config calculated compare value

    compare_value /= 2;                     //on period is equal to 1/2 of total time
    OCR1BH = (compare_value >> 8) & 0x00ff; //config calculated compare value
    OCR1BL = (compare_value)&0x00ff;        //config calculated compare value

    TIMSK1 = 0x0; //no generate any interrupt
    //TIMSK1 = 0x1; //no generate any interrupt
    break;

  case 3:

    TCCR3A = 0x23;           // mode 15, com3b as normal PWM
    TCCR3B = (i + 1) | 0x18; //preescaler value + mode 15. Ignore Input compare

    //this register define PWM frecuency
    OCR3AH = (compare_value >> 8) & 0x00ff; //config calculated compare value
    OCR3AL = (compare_value)&0x00ff;        //config calculated compare value

    compare_value /= 2;                     //on period is equal to 1/2 of total time
    OCR3BH = (compare_value >> 8) & 0x00ff; //config calculated compare value
    OCR3BL = (compare_value)&0x00ff;        //config calculated compare value

    TIMSK3 = 0x0; //no generate any interrupt

    break;

  case 4:

    TCCR4A = 0x23;           // mode 15, com3b as normal PWM
    TCCR4B = (i + 1) | 0x18; //preescaler value + mode 15. Ignore Input compare

    //this register define PWM frecuency
    OCR4AH = (compare_value >> 8) & 0x00ff; //config calculated compare value
    OCR4AL = (compare_value)&0x00ff;        //config calculated compare value

    compare_value /= 2;                     //on period is equal to 1/2 of total time
    OCR4BH = (compare_value >> 8) & 0x00ff; //config calculated compare value
    OCR4BL = (compare_value)&0x00ff;        //config calculated compare value

    TIMSK4 = 0x0; //no generate any interrupt

    break;

  default:
    //no able to work with this timer as pwm
    break;
  }

  sei();

  return pwm_period;
}

bool PWMRequestInterrupt(int pwm_id)
{
  g_pending_interrupt[pwm_id] = true;

  switch (pwm_id)
  {
  case 1:
    TIMSK1 |= 0x1; //turn on interrupt
    break;

  case 3:
    TIMSK3 |= 0x1; //turn on interrupt
    break;

  case 4:
    TIMSK4 |= 0x1; //turn on interrupt
    break;
  default:
    g_pending_interrupt[pwm_id] = false;
    //not enable to to this thing
    break;
  }
}

bool PWMIsPendingInterrupt(int pwm_id)
{
  return g_pending_interrupt[pwm_id];
}


void CounterBegin(int counter_id)
{
  switch (counter_id)
  {
  case 0:
    TCCR0A = 0x2; // mode 2, all pins in normal operation
    TCCR0B = 0x7; // External clock source in rising edge

    OCR0A = 0xfe;

    TIMSK0 = 0x2; //generate interrupt on compare match
    break;

  default:
    //not able to work 
    break;
  }
}

uint32_t CounterGetValue(int counter_id)
{
  uint32_t temp=0;
  switch (counter_id)
  {
  case 0:
    temp = TCNT0;
    TCNT0 = 0;
    temp +=g_overflow_count[counter_id]*0xfe;
    g_overflow_count[counter_id] = 0;
    break;

  case 1:
    temp = g_overflow_count[counter_id];
    g_overflow_count[counter_id] = 0;
  break;

  default:
    break;
  }
      
  return temp;
}

/////////////////// ISR /////////////////

ISR(TIMER0_COMPA_vect)
{
  TCNT0 = 0;
  g_overflow_count[0]++;
}


ISR(TIMER1_OVF_vect)
{
  TIMSK1 &= ~(0x1); //turn off interrupt
  g_pending_interrupt[1] = false;
}
/*
ISR(TIMER1_OVF_vect)
{
  g_overflow_count[1]++;
}*/

ISR(TIMER3_OVF_vect)
{
  TIMSK3 &= ~(0x1); //turn off interrupt
  g_pending_interrupt[3] = false;
}

ISR(TIMER4_OVF_vect)
{
  TIMSK4 &= ~(0x1); //turn off interrupt
  g_pending_interrupt[4] = false;
}

ISR(TIMER5_COMPA_vect)
{
  timer1m_callback();
}
