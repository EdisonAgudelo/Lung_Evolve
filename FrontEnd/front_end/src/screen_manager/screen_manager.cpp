//#include <Nextion.h>
#include "screen_manager.h"
#include "screen_objetcs.h"
//#include "../nextion/Nextion.h"
//#include <SoftwareSerial.h>

//serial to backend pins
//#define SFRONT_END_TX 7
//#define SFRONT_END_RX 6



//buffer_screen buff;
CONFIGURATION config;
Alarm_state AS;
ALARMS alarms_struct;
DATA dataValue;
byte data[NUM_BYTES]; 
int ScreenStates;
bool update;
static ModeHeader MODE;
static TypeHeader TYPE;
static bool monitoring=false;


/////////////////////////////////////////////////////////
/*
NexNumber n1 = NexNumber(5, 13, "n1");
NexNumber n2 = NexNumber(5, 11, "n2");
NexNumber n3 = NexNumber(5, 12, "n3");
NexNumber n4 = NexNumber(6, 9, "n4");
NexNumber n5 = NexNumber(6, 11, "n5");


NexButton b1 = NexButton(0, 2, "b1");      // Main menu Button
NexButton b2 = NexButton(1, 5, "b2");      // breathing parameters Button
NexButton b23 = NexButton(5, 14, "b23");      //  next page Button
NexButton b29 = NexButton(6, 24, "b29"); 


NexCheckbox c0 = NexCheckbox(6, 31, "c0");    // 1:2 I:E checkbox option
NexCheckbox c1 = NexCheckbox(6, 31, "c1");    // 1:3 I:E checkbox option
NexCheckbox c2 = NexCheckbox(6, 33, "c2");    // 1:4 I:E checkbox option
NexCheckbox c3 = NexCheckbox(6, 34, "c3");    // Male gender checkbox option
NexCheckbox c4 = NexCheckbox(6, 35, "c4");    // Female gender checkbox option



//NexDSButton bt0 = NexDSButton(17, 3, "bt0");  // pause or not pause dual state Button




NexTouch *nex_listen_list[] = 
{
  &b1,
  &b2,
  &b29,
  &b23,
  &c0,
  &c1,
  &c2,
  &c3,
  &c4,
  NULL  
}; 
*/ 
/////////////////////////////////////////////////////////


void serial_screen_init(void)
{
  
  //SFrontEnd.begin(9600);
  Serial2.begin(9600);
  //Serial.print("init");
}

void init_screen_management(void)
{
  
  nexInit();
  handlers();
  //clear_buffer(NUM_BYTES1);
  
}



void handlers (void)
{
  //Serial.print("handler\n");

  b9.attachPush(b9PushCallback, &b9);
  b11.attachPush(b11PushCallback, &b11);
  b12.attachPush(b12PushCallback, &b12);
  b14.attachPush(b14PushCallback, &b14);
  b15.attachPush(b15PushCallback, &b15);
  b23.attachPush(b23PushCallback, &b23);
  b29.attachPush(b29PushCallback, &b29);
  b37.attachPush(b37PushCallback, &b37);
  b42.attachPush(b42PushCallback, &b42);
  b75.attachPush(b75PushCallback, &b75);
  b50.attachPush(b50PushCallback, &b50);
  b61.attachPush(b61PushCallback, &b61);
  b73.attachPush(b73PushCallback, &b73);
  bt0.attachPush(bt0PushCallback,&bt0);
  bt0.attachPop(bt0PopCallback,&bt0);

}


////////////////////////// events when a button is pressed or released ////////////////////////

void b9PushCallback(void *ptr)
{
  //shut down
  config.off=0x1;
}


void b11PushCallback(void *ptr)
{
  //assistive
  MODE=Kassistive;
  config.controlType=0x1;
}

void b12PushCallback(void *ptr)
{
  //controlled
  MODE=Kcontrolled;
  config.controlType=0x0;
}

void b14PushCallback(void *ptr)
{
  //volume control
  config.control=0x1;
}

void b15PushCallback(void *ptr)
{
  //pressure control
  config.control=0x0;
}

void b23PushCallback(void *ptr)
{

  TYPE=Kvolume1;
  screen_revieve_data();
  
}


void b29PushCallback(void *ptr)
{
  
  //volume2
  
  TYPE=Kvolume2;
  screen_revieve_data();
 
}

void b37PushCallback(void *ptr)
{
  //guarda numeros y checkbox de la pag7 pasa a la pag8
  //pressure1
  TYPE=kpressure1;
  screen_revieve_data();
}

void b42PushCallback(void *ptr)
{
  //guarda numeros y checkbox de la pag8 y pasa a la pag 20
  TYPE=Kpressure2;
  screen_revieve_data();
}

void b75PushCallback(void *ptr)
{
  //guarda numeros y checkbox de la pag9 pasa a la pag 10
  TYPE=Kassistive1;
  screen_revieve_data();
}

void b50PushCallback(void *ptr)
{
  //guarda numeros y checkbox de la pag10 pasa a la pag 20
  TYPE=Kassistive2;
  screen_revieve_data();
}

void b61PushCallback(void *ptr)
{
  //guarda numeros y checkbox de la pag11 pasa a la pag 12
  MODE=Kalarms;
  TYPE=kalarms1;
  screen_revieve_data();
}

void b73PushCallback(void *ptr)
{
  //guarda numeros y checkbox de la pag12 y pasa a la pag 1
  TYPE=Kalarms2;
  screen_revieve_data();
}

void bt0PushCallback(void *ptr)
{
  config.pause=0x1;
}

void bt0PopCallback(void *ptr)
{
  config.pause=0x0;
}
//***************************** End of events when a button is pressed or released********************************

void screen_revieve_data(void)
{
  uint32_t temp=0;
  switch(MODE)
  {
    case Kcontrolled:
      switch (TYPE)
      {
      case Kvolume1:
          n1.getValue(&config.fio2);
          delay(4);
          n2.getValue(&config.bpm);
          delay(4);
          n3.getValue(&config.peep);
          delay(4);
          n20.getValue(&config.tpause);
          delay(4);
          Serial2.print("page 6");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
        break;
      case Kvolume2:
              
          //n4.getValue(&heiht);
          //delay(4);
          //Serial.print("va a leer");
          n5.getValue(&config.apnea);
          delay(4);
          c0.getValue(&temp);
          if(temp)
          {
            config.ie=0x2;
          }
          else
          {
            delay(4);
            c1.getValue(&temp);
            if(temp)
            {
              config.ie=0x3;
            }
            else
            {
              config.ie=0x4;
            }
          }
          delay(4);
          Serial2.print("page 20");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          delay(4);
          update=true;
          monitoring=true;
      break;

      case kpressure1:
        n6.getValue(&config.fio2);
        delay(4);
        n7.getValue(&config.bpm);
        delay(4);
        n8.getValue(&config.peep);
        delay(4);
        n81.getValue(&config.tpause);
        delay(4);
        Serial2.print("page 8");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        delay(4);
      break;
      case Kpressure2:
      
      n9.getValue(&config.pressure);
      delay(4);
      n10.getValue(&config.apnea);
      delay(4);
      c5.getValue(&temp);
          if(temp)
          {
            config.ie=0x2;
          }
          else
          {
            delay(4);
            c6.getValue(&temp);
            if(temp)
            {
              config.ie=0x3;
            }
            else
            {
              config.ie=0x4;
            }
          }
          delay(4);
          Serial2.print("page 20");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          delay(4);
          update=true;
      break;

      default:
      break;
      }
    break;
    case Kassistive:
      switch(TYPE)
      {
        case Kassistive1:
          n11.getValue(&config.fio2);
          delay(4);
          n12.getValue(&config.peep);
          delay(4);
          n0.getValue(&config.trigger);
          delay(4);
          Serial2.print("page 10");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          delay(4);

        break;
        case Kassistive2:
          n82.getValue(&config.apnea);
          delay(4);
          Serial2.print("page 20");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          delay(4);
          update=true;
      break;
      }
    break;

    case Kalarms:
      switch(TYPE)
      {
        case kalarms1:
        n14.getValue(&config.maxInPressure);
        delay(4);
        n15.getValue(&config.minInPressure);
        delay(4);
        n16.getValue(&config.maxOutPressure);
        delay(4);
        n17.getValue(&config.minOutPressure);
        delay(4);
        Serial2.print("page 12");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        delay(4);
        break;
        case Kalarms2:
        n18.getValue(&config.maxTV);
        delay(4);
        n19.getValue(&config.minTV);
        delay(4);
        Serial2.print("page 1");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        delay(4);
        update=true;
        break;
      }
    break;

    default:
    break;
  }
}


void screen_management(void)
{
  
  int id= 5, ch=1;
  String data_monitoring = "add";
  uint32_t i;
    /*
    *si estoy en pagina tal que hago
    *si estoy en pagina de monitor que envío
    */
   
    nexLoop(nex_listen_list);
    delay(20);
    if(monitoring)
    {
      for(i=0;i<100;i++)
      {
        data_monitoring += id;
        data_monitoring +=",";
        data_monitoring +=ch;
        data_monitoring +=i;
        data_monitoring +="\xFF\xFF\xFF";
        Serial2.print(data_monitoring);
        
        Serial.write(i);
      }
      
      
    }
   /* 
   if(ScreenStates==kpage19)
   {
     //revisar si hay alarmas prioritarias y aun hay vacante, envíe alarma para mostrar
     if(config.controlType==true)//assistive
     {
      Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
      Serial.print(dataValue.peep);  // This is the value you want to send to that object and atribute mentioned before.
      Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
      Serial.print(dataValue.tv);  // This is the value you want to send to that object and atribute mentioned before.
      Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
      Serial.print(dataValue.bpm);  // This is the value you want to send to that object and atribute mentioned before.
      Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
      Serial.print(dataValue.ie);  // This is the value you want to send to that object and atribute mentioned before.
      Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
      Serial.write(0xff);
      Serial.write(0xff);

      Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
      Serial.print(dataValue.pressure);  // This is the value you want to send to that object and atribute mentioned before.
      Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
      Serial.write(0xff);
      Serial.write(0xff);
     }
     else//controlled
     {
       if(config.control==true)//volume
       {
        Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
        Serial.print(dataValue.peep);  // This is the value you want to send to that object and atribute mentioned before.
        Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
        Serial.write(0xff);
        Serial.write(0xff);

        Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
        Serial.print(dataValue.tv);  // This is the value you want to send to that object and atribute mentioned before.
        Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
        Serial.write(0xff);
        Serial.write(0xff);

        Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
        Serial.print(dataValue.bpm);  // This is the value you want to send to that object and atribute mentioned before.
        Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
        Serial.write(0xff);
        Serial.write(0xff);

        Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
        Serial.print(dataValue.ie);  // This is the value you want to send to that object and atribute mentioned before.
        Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
        Serial.write(0xff);
        Serial.write(0xff);

        Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
        Serial.print(dataValue.pressure);  // This is the value you want to send to that object and atribute mentioned before.
        Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
        Serial.write(0xff);
        Serial.write(0xff);
       }else//pressure
       {
        Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
        Serial.print(dataValue.peep);  // This is the value you want to send to that object and atribute mentioned before.
        Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
        Serial.write(0xff);
        Serial.write(0xff);

        Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
        Serial.print(dataValue.tv);  // This is the value you want to send to that object and atribute mentioned before.
        Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
        Serial.write(0xff);
        Serial.write(0xff);

        Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
        Serial.print(dataValue.bpm);  // This is the value you want to send to that object and atribute mentioned before.
        Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
        Serial.write(0xff);
        Serial.write(0xff);

        Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
        Serial.print(dataValue.ie);  // This is the value you want to send to that object and atribute mentioned before.
        Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
        Serial.write(0xff);
        Serial.write(0xff);

        Serial.print("");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
        Serial.print(dataValue.pressure);  // This is the value you want to send to that object and atribute mentioned before.
        Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
        Serial.write(0xff);
        Serial.write(0xff); 
       }
     }
   }
     else
     {
       
     }
     */
    //Serial.print("n3.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    //Serial.print(counter);  // This is the value you want to send to that object and atribute mentioned before.
    //Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    //Serial.write(0xff);
    //Serial.write(0xff);
}
  












