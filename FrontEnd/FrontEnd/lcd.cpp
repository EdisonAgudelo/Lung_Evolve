#include "confg.h"
#include "lcd.h"
#include <LiquidCrystal.h>

enum btns_mnu{
  menu_ini = 0,
  btnRIGHT,
  btnUP,
  btnDOWN,
  btnLEFT,
  btnSELECT,
  btnNONE
};

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
int lcd_key = 0;
int adc_key_in = 0;

void init_lcd(void){
 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd_key = 0;
}

void lcd_menu(void){
  adc_key_in = analogRead(0);
  
  switch (lcd_key){
    case menu_ini:{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set FiO2/Introduzca FiO2: left  ");
      lcd.setCursor(0,1);
      lcd.print("Set inlet Pressure/Introduzca presión de entrada : up");
      lcd.setCursor(0,2);
      lcd.print("Set tidal Volume/Introduzca Volumen tidal: right  ");
      lcd.setCursor(0,3);
      lcd.print("Set breathing rate/Introduzca frecuencia respiratoria : down  ");
      lcd.setCursor(0,4);
      lcd.print("Set I:E ratio/Introduzca tasa I:E : select  ");
      if (adc_key_in < 50){
        lcd_key = btnRIGHT;
      }
      else if (adc_key_in < 250){
        lcd_key = btnUP;
      }
      else if (adc_key_in < 450){
        lcd_key = btnDOWN;
      }
      else if (adc_key_in < 650){
        lcd_key = btnLEFT;
      }
      else if (adc_key_in < 850){
        lcd_key = btnSELECT;
      }
      else {
        lcd_key = btnNONE;
      }
    }

    case btnRIGHT:{
      //int=confg_O2();
      break;
    }

    
    case btnLEFT:{
     // int=confg_p();
      break;
    }

    
    case btnUP:{
      
      break;
    }

    
    case btnDOWN:{
      
      break;
    }

    
    case btnSELECT:{
      
      break;
    }

    
    case btnNONE:{
      
      break;
    }
  }
}
