//#include <Nextion.h>
#include "screen_objetcs.h"
#include "screen_manager.h"
#include <SoftwareSerial.h>

SoftwareSerial SFrontEnd (SFRONT_END_TX,SFRONT_END_RX);
//buffer_screen buff;
CONFIGURATION config;
Alarm_state AS;
ALARMS alarms_struct;
DATA dataValue;
byte data[NUM_BYTES]; 
int ScreenStates;
bool update;

void serial_screen_init(void)
{
  SFrontEnd.begin(9600);
}

void init_screen_management(void)
{
  
  handlers();
  clear_buffer(NUM_BYTES1);
  ScreenStates=kpage0;
}



void handlers (void)
{
  b1.attachPush(b1PushCallback);  
  b2.attachPush(b2PushCallback);  
  b3.attachPush(b3PushCallback);  
  b4.attachPush(b4PushCallback);  
  b5.attachPush(b5PushCallback);  
  b6.attachPush(b6PushCallback);  
  b7.attachPush(b7PushCallback);  
  b8.attachPush(b8PushCallback);  
  b9.attachPush(b9PushCallback);  
  b10.attachPush(b10PushCallback);  
  b11.attachPush(b11PushCallback);  
  b12.attachPush(b12PushCallback);  
  b13.attachPush(b13PushCallback);  
  b14.attachPush(b14PushCallback);  
  b15.attachPush(b15PushCallback);  
  b16.attachPush(b16PushCallback);  
  b23.attachPush(b23PushCallback);  
  b24.attachPush(b24PushCallback);  
  b29.attachPush(b29PushCallback);  
  b30.attachPush(b30PushCallback);  
  b37.attachPush(b37PushCallback);  
  b38.attachPush(b38PushCallback);   
  b42.attachPush(b42PushCallback);  
  b43.attachPush(b43PushCallback);   
  b50.attachPush(b50PushCallback);  
  b51.attachPush(b51PushCallback);   
  b60.attachPush(b60PushCallback);  
  b61.attachPush(b61PushCallback);  
  b66.attachPush(b66PushCallback);  
  b67.attachPush(b67PushCallback);  
  b68.attachPush(b68PushCallback);
  b69.attachPush(b69PushCallback);  
  b70.attachPush(b70PushCallback);  
  b71.attachPush(b71PushCallback);    
  b73.attachPush(b73PushCallback); 
  bt0.attachPush(bt0PushCallback);
  bt0.attachPop(bt0PopCallback);
  bt1.attachPush(bt1PushCallback);
  bt1.attachPop(bt1PopCallback);
}


////////////////////////// events when a button is pressed or released ////////////////////////

void b1PushCallback(void *ptr)  
{
  ScreenStates=kpage1;
  
} 

void b2PushCallback(void *ptr)
{
  ScreenStates=kpage3;
}

void b3PushCallback(void *ptr)
{
  ScreenStates=kpage12;
}

void b4PushCallback(void *ptr)
{
  ScreenStates=kpage14;
}

void b5PushCallback(void *ptr)
{
  ScreenStates=kpage10;
}

void b6PushCallback(void *ptr)
{
  ScreenStates=kpage2;
}

void b7PushCallback(void *ptr)
{
  ScreenStates=kpage15;
}

void b8PushCallback(void *ptr)
{
  ScreenStates=kpage16;
}

void b9PushCallback(void *ptr)
{
  ScreenStates=kpage20;
  config.off=true;
}

void b10PushCallback(void *ptr)
{
  ScreenStates=kpage1;
}

void b11PushCallback(void *ptr)
{
  ScreenStates=kpage9;
  config.controlType = true;
}

void b12PushCallback(void *ptr)
{
  ScreenStates=kpage4;
  config.controlType = false;
}

void b13PushCallback(void *ptr)
{
  ScreenStates=kpage2;
}

void b14PushCallback(void *ptr)
{
  ScreenStates=kpage5;
  config.control = true;
}

void b15PushCallback(void *ptr)
{
  ScreenStates=kpage7;
  config.control = false;
}

void b16PushCallback(void *ptr)
{
  ScreenStates=kpage3;
}

void b23PushCallback(void *ptr)
{
  ScreenStates=kpage6;
}

void b24PushCallback(void *ptr)
{
  ScreenStates=kpage4;
}

void b29PushCallback(void *ptr)
{
  ScreenStates=kpage19;
  recieveData(NUM_BYTES1);
  update=true;

}

void b30PushCallback(void *ptr)
{
  ScreenStates=kpage5;
}

void b37PushCallback(void *ptr)
{
  ScreenStates=kpage8;
}

void b38PushCallback(void *ptr)
{
  ScreenStates=kpage4;
}

void b42PushCallback(void *ptr)
{
  ScreenStates=kpage19;
  recieveData(NUM_BYTES1);
  update=true;
}

void b43PushCallback(void *ptr)
{
  ScreenStates=kpage7;
}

void b50PushCallback(void *ptr)
{
  ScreenStates=kpage19;
  recieveData(NUM_BYTES1);
  update=true;
}

void b51PushCallback(void *ptr)
{
  ScreenStates=kpage3;
}

void b60PushCallback(void *ptr)
{
  ScreenStates=kpage1;
}

void b61PushCallback(void *ptr)
{
  ScreenStates=kpage11;
}

void b66PushCallback(void *ptr)
{
  ScreenStates=kpage10;
}

void b73PushCallback(void *ptr)
{
  ScreenStates=kpage1;
  recieveData(NUM_BYTES1);
  update=true;
}

void b67PushCallback(void *ptr)
{
  ScreenStates=kpage19;
}

void b68PushCallback(void *ptr)
{
  ScreenStates=kpage1;
}

void b69PushCallback(void *ptr)
{
  ScreenStates=kpage18;
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

void b70PushCallback(void *ptr)
{
  ScreenStates=kpage17;
}

void b71PushCallback(void *ptr)
{
  ScreenStates=kpage17;
}

void bt0PushCallback(void *ptr)
{
  config.pause=true;
}

void bt0PopCallback(void *ptr)
{
  config.pause=true; 
}

void bt1PushCallback(void *ptr)
{
  AS.ScreenSoundOff=true;  
}

void bt1PopCallback(void *ptr)
{
  AS.ScreenSoundOff=false; 
}




//***************************** End of events when a button is pressed or released********************************


void screen_management(void)
{
    /*
    *si estoy en pagina tal que hago
    *si estoy en pagina de monitor que envío
    */
    nexLoop(nex_listen_list);
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
     
    Serial.print("n3.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
    //Serial.print(counter);  // This is the value you want to send to that object and atribute mentioned before.
    Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
    Serial.write(0xff);
    Serial.write(0xff);
   }

/*
   switch (ScreenStates)
   {
      case kpage0://pagina inicial, foto y boton para ir al menu ppal
      
      break;
      case kpage1://pagina del menu ppal primera parte
      
      break;
      case kpage2://pagina del menu ppal segunda parte
      
      break;
      case kpage3://pagina para escoger si es asistido o controlado 
      
      break;
      case kpage4://pagina para escoger tipo de control v o p 
      
      break;
      case kpage5://pagina para configurar los parametros cuando es control por v primera parte 
      
      break;
      case kpage6: //pagina para configurar los parámetros cuando es control por v segunda parte 
      break;
      case kpage7://pagina para configurar los parámetros cuando es control por p primera parte
      break;
      case kpage8: //pagina para configurar los parámetros cuando es control por p segunda parte
      break;
      case kpage9: //pagina para configurar los parametros cuando es asistido
      break;
      case kpage10: //pagina para configurar las alarmas primera parte
      break;
      case kpage11: //pagina para configurar las alarmas segunda parte
      break;
      case kpage12: //pagina para llevar a cabo las pruebas primera parte
      break;
      case kpage13: //pagina para llevar a cabo las pruebas segunda parte
      break;
      case kpage14: //pagina para realizar las conf del sistema
      break;
      case kpage15: //pagina acerca de lung evolve
      break;
      case kpage16: //pagina de ayuda
      break;
      case kpage17: //pagina donde se escoge si se quiere pausar el sistema, regresar a menu ppal o regresar a la pagina actual
      break;
      case kpage18: //pagina que muestra todas las alarmas
      break;
      case kpage19: //pagina de monitoreo
      break;
      case kpage20: //pagina de apagado
      break;


   default:
    ScreenStates=kpage0;
     break;
   }
*/
  




void recieveData(int dataLength)
{
  //Serial.write(buff->fio2);
 if (Serial.available())
 {
  Serial.readBytes(data,dataLength);
   for(int i = 0;i < dataLength;i++)
   {
      if(data[i]==kfio2)
      {
        config.fio2=data[i+1];
      }else if(data[i]==kbpm)
      {
        config.bpm=data[i+1];
      }else if(data[i]==kpeep)
      {
        config.peep=data[i+1];
      }else if(data[i]==kheigh)
      {
        config.heigh=data[i+1];
      }else if(data[i]==kapnea)
      {
        config.apnea=data[i+1];
      }else if(data[i]==kie)
      {
        config.ie=data[i+1];
      }else if(data[i]==kgender)
      {
       config.gender=data[i+1]; 
      }else if(data[i]==kpressure)
      {
        config.pressure=data[i+1];
      }else if(data[i]==kmaxInPressure)
      {
        config.maxInPressure=data[i+1];
      }else if(data[i]==kminInPressure)
      {
        config.minInPressure=data[i+1];
      }else if(data[i]==kmaxOutPressure)
      {
        config.maxOutPressure=data[i+1];
      }else if(data[i]==kminOutPressure)
      {
        config.minOutPressure=data[i+1];
      }else if(data[i]==kmaxVT)
      {
        config.maxTV=data[i+1];
      }else if(data[i]==kminTV)
      {
        config.minTV=data[i+1];
      }

    }
    
  }
  clear_buffer(dataLength);
}

void clear_buffer(int nbytes)
{
  for(int i = 0;i < nbytes;i++){
        data[i] = 0x00;
    }
}

/*
void init_struct(buffer_screen *buff)
{
  buff->fio2=0x0;
  buff->bpm=0x0;
  buff->peep=0x0;
  buff->heigh=0x0;
  buff->apnea=0x0;
  buff->ie=0x0;
  buff->gender=0x0;
  buff->pressure=0x0;
}
*/











