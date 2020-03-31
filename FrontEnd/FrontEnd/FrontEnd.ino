
#include "lcd.h"
#include <LiquidCrystal.h>

extern LiquidCrystal lcd;

void setup() {
    init_lcd();
    lcd.print("Lung_Envolve");
    lcd.setCursor(0,1);
    lcd.print("firmware version 1.0 ");
    delay(1000);
    lcd.clear();
}

void loop() {
  lcd.setCursor(9,1);
  lcd.print(millis()/1000);
  lcd_menu();
}
