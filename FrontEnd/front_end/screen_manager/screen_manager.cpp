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
   
  b1.attachPush(b1PushCallback);  // Button press
  b1.attachPop(b1PopCallback);  // Button release
  b0.attachPush(b0PushCallback);  // Button press
  b4.attachPush(b4PushCallback);  // Button press
  b21.attachPush(b21PushCallback);  // Button press
  b22.attachPush(b22PushCallback);  // Button press
  b23.attachPush(b23PushCallback);  // Button press
  b24.attachPush(b24PushCallback);  // Button press
  bt0.attachPop(bt0PopCallback);  // Dual state button bt0 release
  h0.attachPop(h0PopCallback);  // Slider release
  r0.attachPush(r0PushCallback);  // Radio checkbox press
  r1.attachPush(r1PushCallback);  // Radio checkbox press
  r2.attachPush(r2PushCallback);  // Radio checkbox press
  c0.attachPush(c0PushCallback);  // Radio checkbox press
  j0.attachPush(j0PushCallback);  // Progress bar as a button press
  page0.attachPush(page0PushCallback);  // Page press event
  page1.attachPush(page1PushCallback);  // Page press event
  page2.attachPush(page2PushCallback);  // Page press event

  
}


////////////////////////// events when a button is pressed ////////////////////////

void b1PushCallback(void *ptr)  // Press event for button b1
{
  ScreenStates=kpage1;
} 

void b2PushCallback(void *ptr)
{
  ScreenStates=kpage2;
}

void b1PopCallback(void *ptr)  // Release event for button b1
{
 
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





////////////////////////////////////////window change event/////////////////////////////////////////
void window1PushCallback(void *ptr)  
{
  CurrentPage = 1;  
}  

void window2PushCallback(void *ptr)  
{
  CurrentPage = 2;  
}  

void window3PushCallback(void *ptr)  
{
  CurrentPage = 3;  
}  

void window4PushCallback(void *ptr)  
{
  CurrentPage = 4;  
}  

//***************************** End of events when a button is pressed ********************************


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


   default:
    ScreenStates=kpage0;
     break;
   }
}




















