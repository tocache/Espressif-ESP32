#include <LiquidCrystal.h>

LiquidCrystal lcd(16, 17, 5, 18, 19, 21);

int Potenciometro = 0;

void setup(){
  lcd.begin(16, 2);
  lcd.print("hello, world!");
}

void loop() {
  lcd.setCursor(0, 1);
  Potenciometro = analogRead(34);
  lcd.print("CH0:");
  lcd.print(Potenciometro, DEC);
  lcd.print("    ");  
  //lcd.print(millis() / 1000);
}
