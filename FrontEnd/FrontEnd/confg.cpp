#include <stdint.h>
#include <stdio.h>
#include  "confg.h"
#include <LiquidCrystal.h>


uint8_t FiO2 = 0;
uint8_t i= 21;
uint16_t in_presure = 0;
uint16_t volume_tidal = 0;
uint16_t breathing_rate = 6;
uint16_t ie_ratio;
char text_out[40];

extern int adc_key_in;
extern LiquidCrystal lcd;

uint8_t confg_o2(void){
  lcd.clear();
  lcd.setCursor(0,0);
  sprintf(text_out,"FiO2 = %u [%]",i);
  lcd.print(text_out);
  if ((i > 21)&&(i < 100)){
    adc_key_in = analogRead(0);
    lcd.clear();
    lcd.setCursor(0,0);
    sprintf(text_out,"FiO2 = %u [%]",i);
    lcd.print(text_out);
    if ((adc_key_in > 50)&&(adc_key_in < 250)){
      lcd.clear();
      lcd.setCursor(0,0);
      FiO2 = i + 1;
      sprintf(text_out,"FiO2 = %u [%]",i);
      lcd.print(text_out);
    }
    else if ((adc_key_in > 250)&&(adc_key_in <= 450)){
      lcd.clear();
      lcd.setCursor(0,0);
      FiO2 = i - 1;
      sprintf(text_out,"FiO2 = %u [%]",i);
      lcd.print(text_out);  
    }
  }
  return FiO2;  
}

uint16_t confg_p(void){
  lcd.clear();
  lcd.setCursor(0,0); 
  sprintf(text_out,"Inlet pressure/Presión de entrada = %u [cmH2O]",in_presure);
  lcd.print(text_out);
  if (in_presure < 65){
    adc_key_in = analogRead(0);
    lcd.clear();
    lcd.setCursor(0,0);
    sprintf(text_out,"Inlet pressure/Presión de entrada = %u [cmH2O]",in_presure);
    lcd.print(text_out);
    if ((adc_key_in > 50)&&(adc_key_in < 250)){
      lcd.clear();
      lcd.setCursor(0,0);
      in_presure = (in_presure + 1)*1000;
      sprintf(text_out,"Inlet pressure/Presión de entrada = %u [cmH2O]",in_presure);
      lcd.print(text_out);
    }
    else if ((adc_key_in > 250)&&(adc_key_in <= 450)){
      lcd.clear();
      lcd.setCursor(0,0);
      in_presure = (in_presure - 1)*1000;
      sprintf(text_out,"Inlet pressure/Presión de entrada = %u [cmH2O]",in_presure);
      lcd.print(text_out);  
    }
  }
  return in_presure;
}

uint16_t confg_v(void){
  lcd.clear();
  lcd.setCursor(0,0); 
  sprintf(text_out,"Tidal volume/Volumen tidal = %u [mL]",volume_tidal);
  lcd.print(text_out);
  if (volume_tidal < 6500){
    adc_key_in = analogRead(0);
    lcd.clear();
    lcd.setCursor(0,0);
    sprintf(text_out,"Tidal volume/Volumen tidal = %u [mL]",volume_tidal);
    lcd.print(text_out);
    if ((adc_key_in > 50)&&(adc_key_in < 250)){
      lcd.clear();
      lcd.setCursor(0,0);
      volume_tidal = (volume_tidal + 1)*10;
      sprintf(text_out,"Tidal volume/Volumen tidal = %u [mL]",volume_tidal);
      lcd.print(text_out);
    }
    else if ((adc_key_in > 250)&&(adc_key_in <= 450)){
      lcd.clear();
      lcd.setCursor(0,0);
      volume_tidal = (volume_tidal - 1)*10;
      sprintf(text_out,"Tidal volume/Volumen tidal = %u [mL]",volume_tidal);
      lcd.print(text_out);  
    }
  }
  return volume_tidal;
}

uint16_t confg_b(void){
  lcd.clear();
  lcd.setCursor(0,0);
  sprintf(text_out,"Breathing rate/Frecuencia respiratoria = %u [breaths/min]",breathing_rate);
  lcd.print(text_out);  
  if ((breathing_rate > 6)&&(breathing_rate < 40)){
    adc_key_in = analogRead(0);
    lcd.clear();
    lcd.setCursor(0,0);
    sprintf(text_out,"Breathing rate/Frecuencia respiratoria = %u [breaths/min]",breathing_rate);
    lcd.print(text_out); 
    if ((adc_key_in > 50)&&(adc_key_in < 250)){
      lcd.clear();
      lcd.setCursor(0,0);
      breathing_rate = (breathing_rate + 1)*1000;
      sprintf(text_out,"Breathing rate/Frecuencia respiratoria = %u [breaths/min]",breathing_rate);
      lcd.print(text_out); 
    }
    else if ((adc_key_in > 250)&&(adc_key_in <= 450)){
      lcd.clear();
      lcd.setCursor(0,0);
      breathing_rate = (breathing_rate - 1)*1000;
      sprintf(text_out,"Breathing rate/Frecuencia respiratoria = %u [breaths/min]",breathing_rate);
      lcd.print(text_out);   
    }
  }
  return breathing_rate;
}

//ENTENDER BIEN LOS VALORES DE REFERENCIA 
uint16_t confg_ie(void){
  lcd.clear();
  lcd.setCursor(0,0);
  sprintf(text_out,"I:E ratio/Tasa I:E = %u ",ie_ratio);
  lcd.print(text_out);   
  /*if ((breathing_rate > 6)&&(breathing_ratez < 40)){
    adc_key_in = analogRead(0);
    lcd.clear(0,0);
    lcd.setCursor(0,0);
    sprintf(text_out,"I:E ratio/Tasa I:E = %u ",ie_ratio);
    lcd.print(text_out);
    if ((adc_key_in > 50)&&(adc_key_in < 250)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      breathing_rate = (breathing_rate + 1)*1000;
      sprintf(text_out,"I:E ratio/Tasa I:E = %u ",ie_ratio);
      lcd.print(text_out);
    }
    else if ((adc_key_in > 250)&&(ad_key_in <= 450)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      breathing_rate = (breathing_rate - 1)*1000;
      sprintf(text_out,"I:E ratio/Tasa I:E = %u ",ie_ratio);
      lcd.print(text_out);  
    }
  }*/
  return ie_ratio;  
}
