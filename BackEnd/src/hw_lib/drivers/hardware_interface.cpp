

#include "hardware_interface.h"

//library version for arduino mega

/*
  timer 0 and 5 is for flow sensor count
  timer 2 woriking for millis and micros aplication
  Timer 1 , 3, 4 is for custom pwm genarator
*/

void (* timer1m_callback)(void);
bool g_pending_interrupt[6] = {false};

void PinConfigRisingIRS(int pin, void (*callback)(void))
{
  attachInterrupt(digitalPinToInterrupt(pin), callback, RISING);
}

void Timer1msISR(void (*callback)(void))
{
  timer1m_callback = callback;
  
  TCCR2A = 0x2; // mode 2, all pins in norma operation
  TCCR2B = 0x4; //preescaler value 64  // mode 2
  
  //this register define PWM frecuency
  OCR2A = 250;  //config calculated compare value for 1ms interrupt
  
  TIMSK2 = 0x2; //generate interrupt on  Output Compare Match A
}

ISR(TIMER2_COMPA_vect)
{
  timer1m_callback();
}

uint16_t Timer1msCount(void)
{
  return (uint16_t)TCNT2;
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

void PWMConfigFrecuency(int frecuency, int pwm_id)
{
  uint8_t i;

  const uint16_t preescaler[] = {1, 8, 64, 256, 1024};
  uint16_t compare_value;

  cli();

    compare_value = 0x00ff;

    for (i = 0; i < sizeof(preescaler); i++)
    {
      if (F_CPU / (preescaler[i] * 0xffff) <= frecuency)
      {
        compare_value = F_CPU / (preescaler[i] * frecuency);
        break; //break for
      }
    }

  switch (pwm_id)
  {
  case 1:
    
    TCCR1A = 0x33; // mode 15, com3b as inverted PWM
    TCCR1B = (i+1) | 0x18; //preescaler value + mode 15. Ignore Input compare 
    
    //this register define PWM frecuency
    OCR1AH = (compare_value >> 8) & 0x00ff;  //config calculated compare value
    OCR1AL = (compare_value) & 0x00ff; //config calculated compare value

    compare_value-=compare_value/4; //on period is equal to 1/5 of total time
    OCR1BH = (compare_value >> 8) & 0x00ff;  //config calculated compare value
    OCR1BL = (compare_value) & 0x00ff; //config calculated compare value

    TIMSK1 = 0x0; //no generate any interrupt

    break;

  case 3:

    TCCR3A = 0x33; // mode 15, com3b as inverted PWM
    TCCR3B = (i+1) | 0x18; //preescaler value + mode 15. Ignore Input compare 
    
    //this register define PWM frecuency
    OCR3AH = (compare_value >> 8) & 0x00ff;  //config calculated compare value
    OCR3AL = (compare_value) & 0x00ff; //config calculated compare value

    compare_value-=compare_value/4; //on period is equal to 1/5 of total time
    OCR3BH = (compare_value >> 8) & 0x00ff;  //config calculated compare value
    OCR3BL = (compare_value) & 0x00ff; //config calculated compare value

    TIMSK3 = 0x0; //no generate any interrupt

    break;
  
  case 4:

    TCCR4A = 0x33; // mode 15, com3b as inverted PWM
    TCCR4B = (i+1) | 0x18; //preescaler value + mode 15. Ignore Input compare 
    
    //this register define PWM frecuency
    OCR4AH = (compare_value >> 8) & 0x00ff;  //config calculated compare value
    OCR4AL = (compare_value) & 0x00ff; //config calculated compare value

    compare_value-=compare_value/4; //on period is equal to 1/5 of total time
    OCR4BH = (compare_value >> 8) & 0x00ff;  //config calculated compare value
    OCR4BL = (compare_value) & 0x00ff; //config calculated compare value

    TIMSK4 = 0x0; //no generate any interrupt

    break;

  default:
    //no able to work with this timer as pwm
    break;
  }
  sei();
}

bool PWMRequestInterrupt(int pwm_id)
{
  g_pending_interrupt[pwm_id] = true;
  switch (pwm_id)
  {
  case 1:
      TIMSK1 |= 0x1;  //turn on interrupt
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

ISR(TIMER1_OVF_vect)
{
    TIMSK1 &= ~(0x1); //turn off interrupt
    g_pending_interrupt[1] = false;
}

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

bool PWMIsPendingInterrupt(int pwm_id)
{
  return g_pending_interrupt[pwm_id];
}
