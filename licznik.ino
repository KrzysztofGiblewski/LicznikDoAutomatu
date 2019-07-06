// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int ilePacz = 0;   //ile sztuk w bierzacej paczce
int ileKart = 0;   //ile pelnych kartonow
long ileWszy = 0;   //ile wszystkich produktow
int ustawPacz = 100; // zadaje ilosc w paczce np 100szt
int ustawKart = 0; //zadaje ilosc w kartonie np 1000sz
int ileWOsta = 0; //ile w ostatnim nie pelnym kartonie
int ekrany = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.print("0");
  pinMode(14, INPUT_PULLUP); //przycisk dodawania sztuki
  pinMode(15, INPUT_PULLUP); // przycisk odejmowania
  pinMode(16, INPUT_PULLUP); //przycisk wyboru

}

void loop() {
  if (digitalRead(16) == LOW)   {
    zmienEkrany();
  }
  liczKart(); //licze kartony
  liczPacz(); //licze zeby było tyle ile ma mieć paczka

  lcd.setCursor(0, 0);
  lcd.print(ileWszy);
  lcd.print(" szt wszystkich ");
  switch (ekrany)
  {
    case 0:             // bierzacza ilosc w paczce wlasnie robionej
      {
        if (digitalRead(14) == LOW)   {
          dodaj(1);
        }
        if (digitalRead(15) == LOW)   {
          odejmij(1);
        }
        lcd.setCursor(0, 1);
        lcd.print(ilePacz);
        lcd.print(" szt           ");
        break;
      }
    case 1:                     // bierzaca ilosc sztuk w kartonie
      {
        if (digitalRead(14) == LOW)   {
          dodaj(1);
        }
        if (digitalRead(15) == LOW)   {
          odejmij(1);
        }
        lcd.setCursor(0, 1);
        lcd.print(ileWOsta);
        lcd.print(" szt w kartonie  ");
        lcd.print(ileKart);
        break;
      }
    case 2:                             //ustaw ile w paczce
      {
        if (digitalRead(14) == LOW)   {
          ustawPacz += 5;
          delay(300);
        }
        if (digitalRead(15) == LOW)   {
          ustawPacz -= 5;
          delay(300);
        }
        lcd.setCursor(0, 1);
        lcd.print("PACZKA to ");
        lcd.print(ustawPacz);
        lcd.print("szt    ");
        break;
      }
    case 3:                             //ustaw ile w kartonie
      {
        if (digitalRead(14) == LOW)   {
          ustawKart += ustawPacz;
          delay(300);
        }
        if (digitalRead(15) == LOW)   {
          if(ustawKart>0)
          ustawKart -= ustawPacz;
          delay(300);
        }
        lcd.setCursor(0, 1);
        lcd.print("KARTON to ");
        lcd.print(ustawKart);
        lcd.print("szt        ");
        break;
      }
    case 4:                             //Zeruj liczniki
      {
        if (digitalRead(14) == LOW)   {     //jak wcisne + to wychodzimy ekran wyrzej
          zmienEkrany();
        }
        if (digitalRead(15) == LOW)   {     //jak wcisne - to kasuje liczniki
          ileWszy = 0;
          ilePacz = 0;
        }
        lcd.setCursor(0, 1);
        lcd.print("-skasuj- +wyjdz+");
        break;
      }

  }

}
void dodaj(int ile) {
  delay(300);
  ilePacz += ile;
  ileWszy += ile;

}
void odejmij(int ile) {
  delay(300);
  if (ilePacz >= 0) {
    ilePacz -= ile;
    ileWszy -= ile;
  }
  if (ilePacz < 0)
    ilePacz = ustawPacz - ile;

}
void liczPacz() {
  if (ilePacz > ustawPacz)
    ilePacz = 1;
}
void liczKart() {
  if (ustawKart > 0) {
    ileKart = ileWszy / ustawKart;
    ileWOsta = ileWszy % ustawKart;
  } else {
    ileKart = 1 ;
    ileWOsta = ileWszy;
  }

}
void zmienEkrany() {
  delay(300);
  ekrany++;
  if (ekrany > 4)
    ekrany = 0;
}
