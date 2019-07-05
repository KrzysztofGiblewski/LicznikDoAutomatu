// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
   int ilePacz=0;
   int ileKart=0;
   int ileWszy=0;
   
void setup() {
  lcd.begin(16, 2);
  lcd.print("0");
  pinMode(14, INPUT_PULLUP); 
  pinMode(15, INPUT_PULLUP); 
   pinMode(16, INPUT_PULLUP); 
 
}

void loop() {
  if (digitalRead(14) == LOW)
   {
      delay(300);
      ilePacz++;
      ileKart++;
      ileWszy++;
   }
     if (digitalRead(15) == LOW)
   {
      delay(300);
      ilePacz--;
      ileKart--;
      ileWszy--;
   }
  lcd.setCursor(0,0);
    lcd.print(ileWszy);
         lcd.setCursor(0, 1);
         lcd.print(ilePacz);
         
       
  
  
}
