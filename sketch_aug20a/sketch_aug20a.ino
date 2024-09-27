#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,10,9,8,7);

void setup() {
  // put your setup code here, to run once:
lcd.begin(16,2);
}

void loop() {
  // put your main code here, to run repeatedly:
lcd.print('Hello world');

}
