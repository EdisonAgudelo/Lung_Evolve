
#include confg.h
#include <LiquidCrystal.h>
uint8_t FiO2 = 0;
uint8_t i= 21;
uint16_t in_presure = 0;
uint16_t volume_tidal = 0;
uint16_t breathing_rate = 6;
uint16_t ie_ratio;
int adc_key_in = 0;


uint8_t confg_o2(void){
  lcd.clear(0,0);
  lcd.clear(0,1);
  lcd.clear(0,2);
  lcd.clear(0,3);
  lcd.clear(0,4);
  lcd.setCursor(0,0);
  lcd.print("FiO2 = %x [%]",i);
  if ((i > 21)&&(i < 100)){
    adc_key_in = analogRead(0);
    lcd.clear(0,0);
    lcd.setCursor(0,0);
    lcd.print("FiO2 = %x [%]",i);
    if ((adc_key_in > 50)&&(adc_key_in < 250)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      FiO2 = i + 1;
      lcd.print("FiO2 = %x [%]",FiO2);
    }
    else if ((adc_key_in > 250)&&(ad_key_in <= 450)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      FiO2 = i - 1;
      lcd.print("FiO2 = %x [%]",FiO2);  
    }
  }
  return FiO2;  
}

uint16_t confg_p(void){
  lcd.clear(0,0);
  lcd.clear(0,1);
  lcd.clear(0,2);
  lcd.clear(0,3);
  lcd.clear(0,4);
  lcd.setCursor(0,0); 
  lcd.print("Inlet pressure/Presión de entrada = %x [cmH2O]",in_presure); 
  if (in_presure < 65)){
    adc_key_in = analogRead(0);
    lcd.clear(0,0);
    lcd.setCursor(0,0);
    lcd.print("Inlet pressure/Presión de entrada = %x [cmH2O]",in_presure);
    if ((adc_key_in > 50)&&(adc_key_in < 250)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      in_presure = (in_presure + 1)*1000;
      lcd.print("Inlet pressure/Presión de entrada = %x [cmH2O]",in_presure);
    }
    else if ((adc_key_in > 250)&&(ad_key_in <= 450)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      in_presure = (in_presure - 1)*1000;
      lcd.print("Inlet pressure/Presión de entrada = %x [cmH2O]",in_presure);  
    }
  }
  return in_presure;
}

uint16_t confg_v(void){
  lcd.clear(0,0);
  lcd.clear(0,1);
  lcd.clear(0,2);
  lcd.clear(0,3);
  lcd.clear(0,4);
  lcd.setCursor(0,0); 
  lcd.print("Tidal volume/Volumen tidal = %x [mL]",volume_tidal); 
  if (volumen_tidal < 6500)){
    adc_key_in = analogRead(0);
    lcd.clear(0,0);
    lcd.setCursor(0,0);
    lcd.print("Tidal volume/Volumen tidal = %x [mL]",volume_tidal);
    if ((adc_key_in > 50)&&(adc_key_in < 250)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      volume_tidal = (volume_tidal + 1)*10;
      lcd.print("Tidal volume/Volumen tidal = %x [mL]",volume_tidal);
    }
    else if ((adc_key_in > 250)&&(ad_key_in <= 450)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      volume_tidal = (volume_tidal - 1)*10;
      lcd.print("Tidal volume/Volumen tidal = %x [mL]",volume_tidal);  
    }
  }
  return volume_tidal;

uint16_t confg_b(void){
  lcd.clear(0,0);
  lcd.clear(0,1);
  lcd.clear(0,2);
  lcd.clear(0,3);
  lcd.clear(0,4);
  lcd.setCursor(0,0); 
  lcd.print("Breathing rate/Frecuencia respiratoria = %x [breaths/min]",breathing_rate); 
  if ((breathing_rate > 6)&&(breathing_rate < 40)){
    adc_key_in = analogRead(0);
    lcd.clear(0,0);
    lcd.setCursor(0,0);
    lcd.print("Breathing rate/Frecuencia respiratoria = %x [breaths/min]",breathing_rate);
    if ((adc_key_in > 50)&&(adc_key_in < 250)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      breathing_rate = (breathing_rate + 1)*1000;
      lcd.print("Breathing rate/Frecuencia respiratoria = %x [breaths/min]",breathing_rate);
    }
    else if ((adc_key_in > 250)&&(ad_key_in <= 450)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      breathing_rate = (breathing_rate - 1)*1000;
      lcd.print("Breathing rate/Frecuencia respiratoria = %x [breaths/min]",breathing_rate);  
    }
  }
  return breathing_rate;
}

//ENTENDER BIEN LOS VALORES DE REFERENCIA 
uint16_t confg_ie(void){
  lcd.clear(0,0);
  lcd.clear(0,1);
  lcd.clear(0,2);
  lcd.clear(0,3);
  lcd.clear(0,4);
  lcd.setCursor(0,0); 
  lcd.print("I:E ratio/Tasa I:E = %x ",ie_ratio); 
  /*if ((breathing_rate > 6)&&(breathing_ratez < 40)){
    adc_key_in = analogRead(0);
    lcd.clear(0,0);
    lcd.setCursor(0,0);
    lcd.print("Breathing rate = %x [breaths/min]",breathing_rate);
    if ((adc_key_in > 50)&&(adc_key_in < 250)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      breathing_rate = (breathing_rate + 1)*1000;
      lcd.print("Breathing rate = %x [breaths/min]",breathing_rate);
    }
    else if ((adc_key_in > 250)&&(ad_key_in <= 450)){
      lcd.clear(0,0);
      lcd.setCursor(0,0);
      breathing_rate = (breathing_rate - 1)*1000;
      lcd.print("Breathing rate = %x [breaths/min]",breathing_rate);  
    }
  }*/
  return ie_ratio;  
}
