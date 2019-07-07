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
int poIle = 1; //po ile dodawać kolejne sztuki jeśli robi dwa na raz to niech dodaje po 2 sztuki
int coIle = 1;
int takty = 0;
int sztuka = 1;
int wartoscImpulsu = 100;

void setup() {
  lcd.begin(16, 2);
  lcd.print("0");
  pinMode(14, INPUT_PULLUP); //przycisk dodawania sztuki
  pinMode(15, INPUT_PULLUP); // przycisk odejmowania
  pinMode(16, INPUT_PULLUP); //przycisk wyboru
  // pinMode(17,analogRead);
  Serial.begin(9600);
  wartoscImpulsu = analogRead(A3);

}

void loop() {
  wartoscImpulsu = analogRead(A3);
  delay(150);
  if (wartoscImpulsu * (5.0 / 1024.0) > 2.5) {
    dodaj(sztuka*poIle);
  }
  Serial.println(wartoscImpulsu * (5.0 / 1024.0));

  if (digitalRead(16) == LOW)   { //przycisk wyboru
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
          dodaj(sztuka*poIle);
        }
        if (digitalRead(15) == LOW)   {
          odejmij(sztuka*poIle);
        }
        lcd.setCursor(0, 1);
        lcd.print(ilePacz);
        lcd.print(" szt           ");
        break;
      }
    case 1:                     // bierzaca ilosc sztuk w kartonie
      {
        if (digitalRead(14) == LOW)   {
          dodaj(sztuka*poIle);
        }
        if (digitalRead(15) == LOW)   {
          odejmij(sztuka*poIle);
        }
        lcd.setCursor(0, 1);
        lcd.print(ileWOsta);
        lcd.print(" w ostatnim kartonie  ");
        lcd.print(ileKart);
        break;
      }
    case 2:                     // ilosc kartonow
      {
        if (digitalRead(14) == LOW)   {
          dodaj(sztuka*poIle);
        }
        if (digitalRead(15) == LOW)   {
          odejmij(sztuka*poIle);
        }
        lcd.setCursor(0, 1);
        lcd.print(ileKart);
        lcd.print(" pelne kartony  ");

        break;
      }
    case 3:                             //ustaw ile w paczce
      {
        if (digitalRead(14) == LOW)   {
          ustawPacz += 5;
          delay(250);
        }
        if (digitalRead(15) == LOW)   {
          if (ustawPacz > 4)
            ustawPacz -= 5;
          delay(250);
        }
        lcd.setCursor(0, 1);
        lcd.print("PACZKA to ");
        lcd.print(ustawPacz);
        lcd.print(" szt    ");
        break;
      }
    case 4:                             //ustaw ile w kartonie
      {
        if (digitalRead(14) == LOW)   {
          ustawKart += ustawPacz;
          delay(250);
        }
        if (digitalRead(15) == LOW)   {
          if (ustawKart >= ustawPacz)
            ustawKart -= ustawPacz;
          delay(250);
        }
        lcd.setCursor(0, 1);
        lcd.print("KARTON to ");
        lcd.print(ustawKart);
        lcd.print(" szt        ");
        break;
      }
    case 5:                             //ustaw po ile ma sumowac 1 czy np 2 jak na dwa tory
      {
        if (digitalRead(14) == LOW)   {
          poIle++;
          delay(250);
        }
        if (digitalRead(15) == LOW)   {
          if (poIle > 0)
            poIle--;
          delay(250);
        }
        lcd.setCursor(0, 1);
        lcd.print("LICZ PO ");
        lcd.print(poIle);
        lcd.print(" szt        ");
        break;
      }
    case 6:                             //ustaw co ile ma dodawac 1 sztuke np co 2 uderzenia
      {
        if (digitalRead(14) == LOW)   {
          coIle++;
          delay(250);
        }
        if (digitalRead(15) == LOW)   {
          if (coIle > 0)
            coIle--;
          delay(250);
        }
        lcd.setCursor(0, 1);
        lcd.print("LICZ ");
        lcd.print(coIle);
        lcd.print(" takt jak1 ");
        break;
      }
    case 7:                             //Zeruj liczniki
      {
        if (digitalRead(14) == LOW)   {     //jak wcisne + to wychodzimy ekran wyrzej
          zmienEkrany();
        }
        if (digitalRead(15) == LOW)   {     //jak wcisne - to kasuje liczniki
          ileWszy = 0;
          ilePacz = 0;
          zmienEkrany();
        }
        lcd.setCursor(0, 1);
        lcd.print("-skasuj- +wyjdz+");
        break;
      }


  }

}
void dodaj(int ile) {
  takty++;
  if(takty==coIle){
    ileWszy += ile;
    takty=0;
  }
  
  ilePacz = ileWszy % ustawPacz;
  delay(250);
}
void odejmij(int ile) {
  if (ileWszy > 0) {
    ileWszy -= ile;
    ilePacz = ileWszy % ustawPacz;
  }
  delay(250);
}
void liczPacz() {
  if (ilePacz >= ustawPacz)
    ilePacz = 1;
}
void liczKart() {
  if (ustawKart > 0) {
    ileKart = ileWszy / ustawKart;
    ileWOsta = ileWszy % ustawKart;
  } else {
    ileKart = 0 ;
    ileWOsta = ileWszy;
  }

}
void zmienEkrany() {
  delay(250);
  ekrany++;
  if (ekrany > 7)
    ekrany = 0;
}
