//protocolo backend
//funcion para actualizar el bus de datos
//recibir datos de backend


//serial
#include "backend_manager.h"
#include <SoftwareSerial.h>
#include "data_bus/data_bus.h" /////////revisar
#include "hardware/hardware.h"
#include "alarm_manager/alarm_manager.h"
#include <stdlib.h>
#include <string.h>




void serial_conf(void)
{
    SoftwareSerial Sback_end (SBACK_END_TX,SBACK_END_RX);
}

void serial_init(void)
{
    serial_conf();
    sback_end.begin(9600);
    serial.begin(9600);
}


void backend_management(void)
{

}

uint32_t backend_protocol_send(comando,payload)
{
    //sacar longitud al payload
    //aplicar polinomio
    //concatenar comando+tamaño payload+payload+crc
    /*
        mensaje
        divisor
        error


    */

}

uint8_t CRC8Calculate(uint8_t *data, uint8_t length)
{
    
}




uint32_t backend_protocol_recieve(void)
{
    //separa el primer byte y escoge cual comando es
    //separa el segundo byte y segun eso ecoge el payload
    //separa el ultimo byte  y le aplica comando+tamaño+payload, si correcto, almacena, si no, envia NACK

}

void data_bus_manager(tipo de dato,variable, valor)
{
    
        
    //va a la estructura, segun la variable, almacena el valor

    /*si guarda alarma
    if(valor de la alarma a guardar == true) //si la alarma es prioritaria
    {
        if(prev_state==0) //revisa si había una alarma activada antes, si no:
        {
            start_counter=millis(); //toma dato del tiempo
            prev_state=1; //dice que ya hay una alarma activada
        }
        else
        {
            prev_state=1; //si ya había una alarma activada, dejela activada
        }
    }
    */

}
/*



 
void loop() {
  while (Serial_2.available()) {    // Mientras que lleguen caracteres por el puerto serial_2
     delay(5);
     char c = Serial_2.read();     // Lee los caracteres uno a uno en la variable c
     Mensaje += c;                 // Almacena la suma de caracteres en el mensaje
  }     
  if (Mensaje.length()>0){       
     Serial.println(Mensaje);     // envia mensaje al PC 
     if (Mensaje == "encender"){
            digitalWrite(13,1);   // Enciende LED en Pin 13
     }
     if (Mensaje == "apagar"){   
            digitalWrite(13,0);   // Apaga LED en Pin 13 
     } 
  }  
  Mensaje="";                     // Borramos el mensaje ya enviado al PC
}

*/
