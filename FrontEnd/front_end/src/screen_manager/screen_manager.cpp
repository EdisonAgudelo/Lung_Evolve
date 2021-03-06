
/*
    Lung Evolve Mechanical Ventilator
    Copyright (C) 2020  Edison Agudelo, Mateo Garcia, Alejandra Londoño
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see https://www.gnu.org/licenses/gpl-3.0.html.
    
    */


#include "screen_manager.h"
#include "screen_objetcs.h"
#include "time_s.h"



CONFIGURATION config;
volatile Alarm_state AS;
ALARMS alarms_struct;
ALARMS mask_alarms_struct;
DATA dataValue;
byte data[NUM_BYTES]; 
int ScreenStates;
bool update;
static ModeHeader MODE;
static TypeHeader TYPE;
static bool monitoring=false;
bool alarmPage=false;
bool update_alarm_screen;
bool off;
bool new_conf_screen;


#define CH0_OFFSET 10
#define CH1_OFFSET 50
#define CH2_OFFSET 100




/////////////////////////////////////////////////////////


void serial_screen_init(void)
{
  
  //SFrontEnd.begin(9600);
  Serial2.begin(9600);
  //Serial.println("screen serial communcation begin");
}

void init_screen_management(void)
{
  
  nexInit();
  handlers();
  init_mask();
  //Serial.println("screen management initialization done");
  //clear_buffer(NUM_BYTES1);
  //bt1.setValue(0);
  
}

void init_mask (void)
{
  for(int i=0;i<sizeof(mask_alarms_struct.bits);i++)
  {
    mask_alarms_struct.bits[i]=false;
  }
  //mask_alarms_struct.all=0;
}


void handlers (void)
{
  //Serial.print("handler\n");
  b2.attachPush(b2PushCallback, &b2);
  b3.attachPush(b3PushCallback, &b3);
  b5.attachPush(b5PushCallback, &b5);
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
  b69.attachPush(b69PushCallback,&b69);
  b70.attachPush(b70PushCallback,&b70);
  b67.attachPush(b67PushCallback,&b67);
  bt0.attachPush(bt0PushCallback,&bt0);
  bt0.attachPop(bt0PopCallback,&bt0);
  bt1.attachPush(bt1PushCallback,&bt1);
  

}


////////////////////////// events when a button is pressed or released ////////////////////////

void b2PushCallback(void *ptr)
{

  //Serial.println("breathing parameters menu");
  config.tunning=false;
  monitoring=false;
  new_conf_screen=true; 
  
}

void virtualCallbackISR(void)
{
  static bool prev=false;
  //b2PushCallback(nullptr);
  nexLoop(nex_listen_list);
  TimeVirtualISRAdd(!prev,virtualCallbackISR,2000);
    prev=!prev;

}
void b3PushCallback(void *ptr)
{
  config.tunning=true;
  //Serial.println("calibration menu");
}

void b5PushCallback(void *ptr)
{
  config.tunning=false;
  //Serial.println("alarm configuration menu");
}

void b9PushCallback(void *ptr)
{
  //shut down
  //Serial.println("shut down");
  off=true;
  //Serial.print("off");
  delay(5);
}


void b11PushCallback(void *ptr)
{
  //assistive
  MODE=Kassistive;
  //Serial.println("assistive mode");
  
  
  
}

void b12PushCallback(void *ptr)
{
  //controlled
  MODE=Kcontrolled;
  //Serial.println("controlled mode");
  
 
}

void b14PushCallback(void *ptr)
{
  //volume control
  //Serial.println("volume control mode");
  
}

void b15PushCallback(void *ptr)
{
  //pressure control
  
  //Serial.print("pression control");
  //Serial.println("pressure control mode");
 
}

void b23PushCallback(void *ptr)
{

  TYPE=Kvolume1;
  screen_revieve_data();
  
}


void b29PushCallback(void *ptr)
{
  
  //volume2
  //Serial.print("volume");
  TYPE=Kvolume2;
  new_conf_screen=false;
  screen_revieve_data();
  //Serial.println("configuration recieved");
 
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
  new_conf_screen=false;
  screen_revieve_data();
  //Serial.println("configuration recieved");
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
  new_conf_screen=false;
  //Serial.println("configuration recieved");
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
  //Serial.println("configuration recieved");
}

void bt0PushCallback(void *ptr)
{
  uint32_t temp=0;
  
  Serial.println("pause  detected ");
  update=1;
  bt0.getValue(&temp);
  if(temp==0x1)
  {
    config.pause=0x1;
    new_conf_screen=false;
    //Serial.println("pause init detected");
  }
  else
  {
    config.pause=0x0;
    //Serial.println("pause end detected");
  }
  delay(6);
}

void bt0PopCallback(void *ptr)
{
  //config.pause=0x0;
}

void bt1PushCallback(void *ptr)
{
  uint32_t temp=0;
  bt1.getValue(&temp);
  if(temp==0x1)
  {
    AS.ScreenSoundOff=true;
    //Serial.println("sound OFF detected");
  }
  else
  {
    AS.ScreenSoundOff=false;
    //Serial.println("sound ON detected");
  }

  //Serial.print("silence");
  
  
}

void b69PushCallback(void *ptr)
{
  alarmPage=true;//si se pone bloqueante, quite los nextloop de send_screen_alarm y descomente
  //screen_alarms();
  send_screen_alarm();
  //Serial.println("alarm state page");
}

void b70PushCallback(void *prt)
{
  alarmPage=false;
  //Serial.println("return to other option menu detected");
  //Serial.print("false");
  Serial2.print("page 18");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  delay(4);
}


void b67PushCallback(void *ptr)
{
  monitoring=true;
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
          config.control=true;
          config.controlType=false;
          //Serial.write(config.control);
          n1.getValue(&config.fio2);
          delay(5);
          n2.getValue(&config.bpm);
          delay(5);
          n3.getValue(&config.peep);
          delay(5);
          n20.getValue(&config.tpause);
          delay(5);
          Serial2.print("page 6");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
        break;
      case Kvolume2:
              
          n4.getValue(&config.tidal);
          delay(5);
          n5.getValue(&config.apnea);
          delay(5);
          c0.getValue(&temp);
          if(temp)
          {
            config.ie=0x2;
          }
          else
          {
            delay(5);
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
          delay(5);
          Serial2.print("page 20");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          delay(5);
          update=true;
          monitoring=true;
      break;

      case kpressure1:
        config.control=false;
        config.controlType=false;
        n6.getValue(&config.fio2);
        delay(5);
        n7.getValue(&config.bpm);
        delay(5);
        n8.getValue(&config.peep);
        delay(5);
        n21.getValue(&config.tpause);
        delay(5);
        Serial2.print("page 8");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        delay(5);
      break;
      case Kpressure2:
      
      n9.getValue(&config.pressure);
      delay(5);
      n10.getValue(&config.apnea);
      delay(5);
      n26.getValue(&config.tidal);
      delay(5);
      c5.getValue(&temp);
          if(temp)
          {
            config.ie=0x2;
          }
          else
          {
            delay(5);
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
          delay(5);
          Serial2.print("page 20");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          delay(5);
          update=true;
          monitoring=true;
      break;

      default:
      break;
      }
    break;
    case Kassistive:
      config.controlType=true;
      config.control=true;
      switch(TYPE)
      {
        case Kassistive1:
          n11.getValue(&config.fio2);
          delay(5);
          n12.getValue(&config.peep);
          delay(5);
          n23.getValue(&config.trigger);
          delay(5);
          if((int32_t)config.trigger>0)
          {
            config.triggerSource=false;
          }
          else
          {
            config.triggerSource=true;
          }
          
          Serial2.print("page 10");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          delay(5);

        break;
        case Kassistive2:
          n13.getValue(&config.tpause);
          delay(5);
          n24.getValue(&config.apnea);
          delay(5);
          n25.getValue(&config.tidal);
          delay(5);
          Serial2.print("page 20");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          delay(5);
          update=true;
          monitoring=true;
      break;
      }
    break;

    case Kalarms:
      switch(TYPE)
      {
        case kalarms1:
        n14.getValue(&config.maxInPressure);
        delay(5);
        n15.getValue(&config.minInPressure);
        delay(5);
        n16.getValue(&config.maxOutPressure);
        delay(5);
        n17.getValue(&config.minOutPressure);
        delay(5);
        Serial2.print("page 12");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        delay(5);
        break;
        case Kalarms2:
        n18.getValue(&config.maxTV);
        delay(5);
        n19.getValue(&config.minTV);
        delay(5);
        Serial2.print("page 1");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);
        delay(5);
        update=true;
        break;
      }
    break;

    default:
    break;
  }
}


void screen_alarms(void)
{
 static int J=0;
    //Serial.print("entra");
    if(J<sizeof(alarms_struct.bits))
    {
      if(alarms_struct.bits[J]==true)
      {
        if(J<=9)
        {
          
         // send_screen_alarm(J,1); // 1 indicates medium priority
        }
        else
        {
          
//          send_screen_alarm(J,2); //2 indicates high priority
        }
        
          
      }
      else
      {
  //      send_screen_alarm(J,0); 
      }
      
      
      
    }
    else
    {
      J=0;
    }
    
    
}

void send_screen_alarm(void)
{
  uint32_t red=63488,yellow=65504,white=65535;
  if(mask_alarms_struct.bits!=alarms_struct.bits)
  {
    //Serial.println("sending alarm state");
        if(alarms_struct.bits[0]==true)
        {
          t30.Set_background_color_bco(yellow);
        }
        else
        {
          t30.Set_background_color_bco(white);
        }

        if(alarms_struct.bits[1]==true)
        {
          t31.Set_background_color_bco(yellow);
        }
        else
        {
          t31.Set_background_color_bco(white);
        }
      

        if(alarms_struct.bits[2]==true)
        {
          t32.Set_background_color_bco(yellow);
        }
        else
        {
          t32.Set_background_color_bco(white);
        }

        if(alarms_struct.bits[3]==true)
        {
          t33.Set_background_color_bco(yellow);
        }
        else
        {
          t33.Set_background_color_bco(white);
        }
        
 
        if(alarms_struct.bits[4]==true)
        {
          t34.Set_background_color_bco(yellow);
        }
        else
        {
          t34.Set_background_color_bco(white);
        }

        if(alarms_struct.bits[5]==true)
        {
          t35.Set_background_color_bco(yellow);
        }
        else
        {
          t35.Set_background_color_bco(white);
        }
        
   
        if(alarms_struct.bits[6]==true)
        {
          t36.Set_background_color_bco(yellow);
        }
        else
        {
          t36.Set_background_color_bco(white);
        }
        

        if(alarms_struct.bits[7]==true)
        {
          t37.Set_background_color_bco(yellow);
        }
        else
        {
          t37.Set_background_color_bco(white);
        }
        

        if(alarms_struct.bits[8]==true)
        {
          t38.Set_background_color_bco(yellow);
        }
        else
        {
          t38.Set_background_color_bco(white);
        }
       

        if(alarms_struct.bits[9]==true)
        {
          t39.Set_background_color_bco(yellow);
        }
        else
        {
          t39.Set_background_color_bco(white);
        }
        

        if(alarms_struct.bits[10]==true)
        {
          t40.Set_background_color_bco(red);
        }
        else
        {
          t40.Set_background_color_bco(white);
        }
        

        if(alarms_struct.bits[11]==true)
        {
          t41.Set_background_color_bco(red);
        }
        else
        {
          t41.Set_background_color_bco(white);
        }

        if(alarms_struct.bits[12]==true)
        {
          t42.Set_background_color_bco(red);
        }
        else
        {
          t42.Set_background_color_bco(white);
        }
        
        if(alarms_struct.bits[13]==true)
        {
          t43.Set_background_color_bco(red);
        }
        else
        {
          t43.Set_background_color_bco(white);
        }
    
        if(alarms_struct.bits[14]==true)
        {
          t44.Set_background_color_bco(red);
        }
        else
        {
          t44.Set_background_color_bco(white);
        }

        if(alarms_struct.bits[15]==true)
        {
          t45.Set_background_color_bco(red);
        }
        else
        {
          t45.Set_background_color_bco(white);
        }
       

        if(alarms_struct.bits[16]==true)
        {
          t46.Set_background_color_bco(red);
        }
        else
        {
          t46.Set_background_color_bco(white);
        }

        if(alarms_struct.bits[17]==true)
        {
          t47.Set_background_color_bco(red);
        }
        else
        {
          t47.Set_background_color_bco(white);
        }

        if(alarms_struct.bits[18]==true)
        {
          t48.Set_background_color_bco(red);
        }
        else
        {
          t48.Set_background_color_bco(white);
        }

        if(alarms_struct.bits[19]==true)
        {
          t49.Set_background_color_bco(red);
        }
        else
        {
          t49.Set_background_color_bco(white);
        }
     
        if(alarms_struct.bits[20]==true)
        {
          t50.Set_background_color_bco(red);
        }
        else
        {
          t50.Set_background_color_bco(white);
        }
        update_alarm_screen=true;
  }
  else
  {
    update_alarm_screen=false;
  }
  //Serial.println("end of sending alarm state");
  for(int j=0;j<21;j++)
  {
    mask_alarms_struct.bits[j]=alarms_struct.bits[j];
  }
  
    
}





void screen_management(void)
{
  
  static int i=0;
  const uint8_t OFFSET=0x40;
  const uint32_t factor_pressure =0xff/0x3c,factor_tidal=0x64/0x1b58,factor_mix=0x64/0x78;
  uint32_t conth,contm;
    /*
    *si estoy en pagina tal que hago
    *si estoy en pagina de monitor que envío
    */
   
    nexLoop(nex_listen_list);
    


  if(monitoring)
  {
    //Serial.println("monitoring page data sending");
    if(data_change)
    {
      //s0.addValue(0,((dataValue.patient_volume*factor_tidal)+CH0_OFFSET));//dataValue.battery_level
      //delay(2);
      s0.addValue(1,((dataValue.in_pressure*factor_pressure)+CH1_OFFSET));
      //delay(2);
      //s0.addValue(2,((dataValue.mixture_flow*factor_mix)+CH2_OFFSET));
      //delay(2);
      //j1.setValue(dataValue.battery_level);
      //delay(2);
     /* n32.setValue(dataValue.in_pressure);
      delay(2);
      n33.setValue(dataValue.tidal);
      delay(2);
      n34.setValue(dataValue.breathing_rate);
      delay(2);
      n35.setValue(dataValue.ie_ratio);
      delay(2);
      data_change = false;
      nexLoop(nex_listen_list);*/
    }
    
  }
  else
  {

  }

  if(off)
  {
    off=false;
    //Serial.println("the device is going to turn off");
    RELE(true);

  }

    /*
    if(monitoring && update_alarm_screen)
    {
      //envia data
      for(i=0;i<21;i++)
      {
        if((i<10) && (AS.MediumAlarmState==true))
        {
          if(alarms_struct.bits[i]==true)
          {
            contm++;
          }
        }
        if((10<i<21) && (AS.HighAlarmState==true))
        {
           if(alarms_struct.bits[i]==true)
          {
            conth++;
          }
        }
        
      }
      n20.setValue(conth);
      n21.setValue(contm);    
      conth=0;
      contm=0;  

    }
    else
    {
      conth=0;
      contm=0;
    }
    */
    
  
}
  












