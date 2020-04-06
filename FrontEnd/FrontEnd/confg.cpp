#include <stdint.h>
#include <stdio.h>
#include  "confg.h"
#include <LiquidCrystal.h>

//working mode options 
bool is_tunning = 0; //calibration
bool is_standby = 0; //no control
bool is_pressure_controled = 0; //control by volume or pressure 

//reference values 
uint8_t FiO2 = 0;
uint8_t i= 21;
uint16_t in_presure = 0;
uint16_t volume_tidal = 0;
uint16_t breathing_rate = 6;
uint16_t ie_ratio = 0;
char text_out[40];

//warnings
uint16_t maximun_in_pressure = 0;
uint16_t maximun_out_pressure = 0;
uint16_t maximun_volume_tidal = 0;
uint16_t manimun_in_pressure = 0;
uint16_t manimun_out_pressure = 0;
uint16_t manimun_volume_tidal = 0;

extern int adc_key_in;
extern LiquidCrystal lcd;
bool breath = 0;
bool ie_r = 0;

uint8_t confg_o2(void){
  lcd.clear();
  lcd.setCursor(0,0);
  sprintf(text_out,"FiO2 = %u [%]",i);
  lcd.print(text_out);
  while (is_tunning == 1){
    if ((i > 21)&&(i < 100)){
      adc_key_in = analogRead(0);
      lcd.clear();
      lcd.setCursor(0,0);
      if (adc_key_in < 250){ //up button
        lcd.clear();
        lcd.setCursor(0,0);
        FiO2 = (i + 1) * 10; //step 10 by 10
      }
      else if (adc_key_in < 50){ //right button
        lcd.clear();
        lcd.setCursor(0,0);
        FiO2 = i + 1; //step 1 by 1
      }
      else if (adc_key_in < 450){ //down button
        lcd.clear();
        lcd.setCursor(0,0);
        FiO2 = (i - 1) * 10; //step 10 by 10 
      }    
      else if (adc_key_in < 650){ //left button
        lcd.clear();
        lcd.setCursor(0,0);
        FiO2 = (i - 1); //step 1 by 1 
      }
      else if (adc_key_in < 850){
        is_tunning = 0;
      }
    } 
    sprintf(text_out,"FiO2 = %u [%]",i);
    lcd.print(text_out);       
  }
  return FiO2;  
}

uint16_t confg_p(void){
  lcd.clear();
  lcd.setCursor(0,0); 
  sprintf(text_out,"Inlet pressure/Presión de entrada = %u*1000 [cmH2O]",in_presure);
  lcd.print(text_out);
  while (is_pressure_controled == 1){
    adc_key_in = analogRead(0);
    lcd.clear();
    lcd.setCursor(0,0);
    if (adc_key_in < 250){ //up button
      lcd.clear();
      lcd.setCursor(0,0);
      in_presure = (in_presure + 1) * 10; //step 10 by 10
    }
    else if (adc_key_in < 50){ //right button
      lcd.clear();
      lcd.setCursor(0,0);
      in_presure = in_presure + 1; //step 1 by 1
    }
    else if (adc_key_in < 450){ //down button
      lcd.clear();
      lcd.setCursor(0,0);
      in_presure = (in_presure - 1)*10; //step 10 by 10
    }
    else if (adc_key_in < 650){ //left button
      lcd.clear();
      lcd.setCursor(0,0);
      in_presure = in_presure - 1; //step 1 by 1  
    }
    else if (adc_key_in < 850 ){ //select button
      is_pressure_controled = 0;
    }
  sprintf(text_out,"Inlet pressure/Presión de entrada = %u*1000 [cmH2O]",in_presure);
  lcd.print(text_out);
  }
  in_presure = in_presure * 1000;
  return in_presure;
}

uint16_t confg_v(void){
  lcd.clear();
  lcd.setCursor(0,0); 
  sprintf(text_out,"Tidal volume/Volumen tidal = %u*10 [mL]",volume_tidal);
  lcd.print(text_out);
  while (is_pressure_controled == 1){
    adc_key_in = analogRead(0);
    lcd.clear();
    lcd.setCursor(0,0);
    if (adc_key_in < 250){ //up button
      lcd.clear();
      lcd.setCursor(0,0);
      volume_tidal = (volume_tidal + 1) * 100; // step 100 by 100
    }
    else if (adc_key_in < 50){ //right button
      lcd.clear();
      lcd.setCursor(0,0);
      volume_tidal = volume_tidal + 1; //step 1 by 1  
    }
    else if (adc_key_in < 450){ //down button
      lcd.clear();
      lcd.setCursor(0,0);
      volume_tidal = (volume_tidal - 1)*100; //step 100 by 100
    }
    else if (adc_key_in < 650){ //left button
      lcd.clear();
      lcd.setCursor(0,0);
      volume_tidal = volume_tidal - 1; //step 1 by 1
    }
    else if (adc_key_in < 850){ //select button
      is_pressure_controled = 0;  
    }
    sprintf(text_out,"Tidal volume/Volumen tidal = %u*10 [mL]",volume_tidal);
    lcd.print(text_out);
  }
  volume_tidal = volume_tidal *10;
  return volume_tidal;
}

uint16_t confg_b(void){
  lcd.clear();
  lcd.setCursor(0,0);
  sprintf(text_out,"Breathing rate/Frecuencia respiratoria = %u*1000 [breaths/min]",breathing_rate);
  lcd.print(text_out);  
  while (breath == 1){
    if ((breathing_rate > 6)&&(breathing_rate < 40)){
      adc_key_in = analogRead(0);
      lcd.clear();
      lcd.setCursor(0,0); 
      if (adc_key_in < 250){ //up button
        lcd.clear();
        lcd.setCursor(0,0);
        breathing_rate = breathing_rate + 1;
      }
      else if (adc_key_in < 450){ //down button
        lcd.clear();
        lcd.setCursor(0,0);
        breathing_rate = breathing_rate - 1;
      }
      else if (adc_key_in < 850){ //select button
        breath = 0;  
      }  
    }
    sprintf(text_out,"Breathing rate/Frecuencia respiratoria = %u*1000 [breaths/min]",breathing_rate);
    lcd.print(text_out); 
  }
  breathing_rate = breathing_rate * 1000;
  return breathing_rate;
}
 
uint16_t confg_ie(void){
  lcd.clear();
  lcd.setCursor(0,0);
  sprintf(text_out,"I:E ratio/Tasa 1:E = 1:%u*1000 ",ie_ratio);
  lcd.print(text_out);  
  while (ie_r == 1){
    if ((ie_ratio > 1)&&(ie_ratio < 3)){
      adc_key_in = analogRead(0);
      lcd.clear();
      lcd.setCursor(0,0); 
      if (adc_key_in < 250){ //up button
        lcd.clear();
        lcd.setCursor(0,0);
        ie_ratio = ie_ratio + 1;
      }
      else if (adc_key_in < 450){ //down button
        lcd.clear();
        lcd.setCursor(0,0);
        ie_ratio = ie_ratio - 1;
      }
      else if (adc_key_in < 850){ //select button
        ie_r = 0;  
      }  
    }
    sprintf(text_out,"I:E ratio/Tasa 1:E = 1:%u*1000 ",ie_ratio);
    lcd.print(text_out); 
  }
  ie_ratio = ie_ratio * 1000; 
  return ie_ratio;  
}
