#include "confg.h"
#include "lcd.h"
#include <LiquidCrystal.h>

//btn definitions
enum btns_mnu{
  menu_ini = 0,
  btnRIGHT,
  btnUP,
  btnDOWN,
  btnLEFT,
  btnSELECT,
  btnNONE
};

//working mode options (confg.cpp)
extern bool is_tunning; //calibration
extern bool is_standby; //no control
extern bool is_pressure_controled; //control by volume or pressure 

//reference values (confg.cpp)
extern uint8_t FiO2;
extern uint16_t in_presure;
extern uint16_t volume_tidal;
extern uint16_t breathing_rate;
extern uint16_t ie_ratio;

//warnings (confg.cpp)
extern uint16_t maximun_in_pressure;
extern uint16_t maximun_out_pressure;
extern uint16_t maximun_volume_tidal;
extern uint16_t manimun_in_pressure;
extern uint16_t manimun_out_pressure;
extern uint16_t manimun_volume_tidal;

extern bool breath;
extern bool ie_r;

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
      is_pressure_controled = 1;
      volume_tidal = confg_v();
      if (volume_tidal == 0){
        manimun_volume_tidal = volume_tidal;
      }
      else if (volume_tidal == 65000){
        maximun_volume_tidal = volume_tidal;
      }
      break;
    }

    
    case btnLEFT:{
      is_tunning = 1;
      FiO2 =confg_o2();
      break;
    }

    
    case btnUP:{
      is_pressure_controled = 1;
      in_presure = confg_p();
      if (in_presure == 0){
        manimun_in_pressure = in_presure;
      }
      else if (in_presure == 65000){
        maximun_in_pressure = in_presure;
      }
      break;
    }

    
    case btnDOWN:{
      breath = 1;
      breathing_rate = confg_b();
      break;
    }

    
    case btnSELECT:{
      ie_r = 1;
      ie_ratio = confg_ie();
      break;
    }

    
    case btnNONE:{
      is_standby = 1;
      lcd.print("Push a button");
      break;
    }
  }
}
