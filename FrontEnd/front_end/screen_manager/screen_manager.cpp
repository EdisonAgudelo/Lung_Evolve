//funcion para guardar en el bus de datos en las 3 estructuras
//detección de eventos de la pantalla
//lee de la pantalla las entradas
//envia datos a la pantalla
//armar el protocolo o usar el protocolo
//menus y logica de menus, manejo en general de pantalla



//



//hardware comunicación serial
//




#include "screen_manager.h"



char textBuffer[100] = {0};  //buffer to recieve text from the screen

void init_screen_management(void)
{
  handlers();
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
}

void b10PushCallback(void *ptr)
{
  ScreenStates=kpage1;
}

void b11PushCallback(void *ptr)
{
  ScreenStates=kpage9;
}

void b12PushCallback(void *ptr)
{
  ScreenStates=kpage4;
}

void b13PushCallback(void *ptr)
{
  ScreenStates=kpage2;
}

void b14PushCallback(void *ptr)
{
  ScreenStates=kpage5;
}

void b15PushCallback(void *ptr)
{
  ScreenStates=kpage7;
}

void b16PushCallback(void *ptr)
{
  ScreenStates=kpage3;
}

void 23PushCallback(void *ptr)
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
}

void b43PushCallback(void *ptr)
{
  ScreenStates=kpage7;
}

void b50PushCallback(void *ptr)
{
  ScreenStates=kpage19;
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
  //modificar struct de estado de funcionamiento pausado 
}

void bt0PopCallback(void *ptr)
{
  //modificar struct de estado de funcionamiento no pausado 
}

void bt1PushCallback(void *ptr)
{
  //modificar la struct de estado de funcionamiento, alarma sonora no activa 
}

void bt1PopCallback(void *ptr)
{
  //modificar la struct de estado de funcionamiento, alarma sonora no activa
}



void b2PushCallback(void *ptr)  // Press event for button b0
{
  //counter = counter - 1;  // Subtract 1 to the current value of the counter
  Serial.print("n3.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  Serial.print(counter);  // This is the value you want to send to that object and atribute mentioned before.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);
}  // End of press event

void b3PushCallback(void *ptr)  // Press event for button b0
{
  //counter = counter - 1;  // Subtract 1 to the current value of the counter
  Serial.print("n3.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  Serial.print(counter);  // This is the value you want to send to that object and atribute mentioned before.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);
}  // End of press event

void b4PushCallback(void *ptr)  // Press event for button b4
{
  counter = counter + 1;  // Add 1 to the current value of the counter

  Serial.print("n3.val=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  Serial.print(counter);  // This is the value you want to send to that object and atribute mentioned before.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);
}  // End of press event



void j0PushCallback(void *ptr)  // Press event for progress bar
{
  Serial.print("j0.pco=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  Serial.print(63488);  // Code for the color red
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);
}  // End of press event



void b7PushCallback(void *ptr)  // Press event for "regresar" button on windows #
{
  Serial.print("page 0");  // Sending this it's going to tell the nextion display to go to page 0.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);
}  // End of press event



void b22PushCallback(void *ptr)  // Press event for "Send" button on page 2
{
  memset(textBuffer, 0, sizeof(textBuffer));  // Clear the buffer, so we can start using it
  t5.getText(textBuffer, sizeof(textBuffer));  // Read the text on the object t5 and store it on the buffer

  // Now is going to send the text we received to object t23:
  Serial.print("t23.txt=");  // This is sent to the nextion display to set what object name (before the dot) and what atribute (after the dot) are you going to change.
  Serial.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
  Serial.print(textBuffer);  // This is the text you want to send to that object and atribute mentioned before.
  Serial.print("\"");  // Since we are sending text, and not a number, we need to send double quote before and after the actual text.
  Serial.write(0xff);  // We always have to send this three lines after each command sent to the nextion display.
  Serial.write(0xff);
  Serial.write(0xff);
}  // End of press event


//***************************** End of events when a button is pressed or released********************************


void screen_management(void)
{
    /*
    *si estoy en pagina tal que hago
    *si estoy en pagina de monitor que envío
    */

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
}




















