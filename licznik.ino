#include <LiquidCrystal.h>
//VSS-  VDD+   VO-do potencjometru                          A+     K-
//             D12     D11      D5      D4      D3      D2
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
int popWartoscImpu  = 0;
char impuls = 1; //wartosc 0 lub 1 zeby po podaniu ciaglego napiecia nie naliczal kolejnych sztuk
double napImpulsu = 3.4; //minimalna wartość impulsu w voltach dla impulsu
double zeroNapiecia = 1; // wartosc napiecia ponirzej ktorego uznajemy za zanik impulsu
int opuznij = 100;

void setup() {
  lcd.begin(16, 2);
  lcd.print("0");
  pinMode(14, INPUT_PULLUP); //przycisk dodawania sztuki A0
  pinMode(15, INPUT_PULLUP); // przycisk odejmowania A1
  pinMode(16, INPUT_PULLUP); //przycisk wyboru A2
  Serial.begin(9600);
  wartoscImpulsu = analogRead(A3); //pin 17 czyli A3

}

void loop() {
  wartoscImpulsu = analogRead(A3);
  delay(opuznij);
  if (wartoscImpulsu < zeroNapiecia) //jak napiecie zaniknie to mozna znowu liczyc impuls
    impuls = 1;
  if ((wartoscImpulsu * (5.0 / 1024.0) > napImpulsu) && impuls == 1 ) { //warunek minimalnego napiecia dla impulsu zeby dodac
    dodaj(sztuka * poIle);
    impuls = 0;
  }
  //wyswietla napiecie na pinie A3
  Serial.println(wartoscImpulsu * (5.0 / 1024.0));

  if (digitalRead(16) == LOW)   { //przycisk wyboru A2
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
          dodaj(sztuka * poIle);
        }
        if (digitalRead(15) == LOW)   {
          odejmij(sztuka * poIle);
        }
        drugaLinia("", ilePacz, " sztuk w paczce           ", 0);
        break;
      }
    case 1:                     // bierzaca ilosc sztuk w kartonie
      {
        if (digitalRead(14) == LOW)   {
          dodaj(sztuka * poIle);
        }
        if (digitalRead(15) == LOW)   {
          odejmij(sztuka * poIle);
        }
        drugaLinia("", ileWOsta, " w ostatnim kartonie  ", ileKart);
        break;
      }
    case 2:                     // ilosc kartonow
      {
        if (digitalRead(14) == LOW)   {
          dodaj(sztuka * poIle);
        }
        if (digitalRead(15) == LOW)   {
          odejmij(sztuka * poIle);
        }
        drugaLinia("", ileKart, " pelne kartony      ", 0);
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
        drugaLinia("PACZKA to ", ustawPacz, " szt      ", 0);
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
          zmienEkrany();
        }
        drugaLinia("+wyjdz+ -skasuj- ", 0, "", 0);
        break;
      }



    case 5:                             //ustaw ile w kartonie
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
        drugaLinia("KARTON to ", ustawKart, " szt           ", 0);
        break;
      }
    case 6:                             //ustaw po ile ma sumowac 1 czy np 2 jak na dwa tory
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
        drugaLinia("LICZ PO ", poIle, " szt             ", 0);
        break;
      }


    case 7:                             //ustaw co ile ma dodawac 1 sztuke np co 2 uderzenia
      {
        if (digitalRead(14) == LOW)   {
          coIle++;
          delay(200);
        }
        if (digitalRead(15) == LOW)   {
          if (coIle > 1)
            coIle--;

          delay(200);
        }
        drugaLinia("LICZ ", coIle, " takt jak", 1);
        break;
      }

    case 8:                             //ustaw delay miedzy impulsami
      {
        if (digitalRead(14) == LOW)   {
          opuznij += 5;
          delay(200);
        }
        if (opuznij > 10)
          if (digitalRead(15) == LOW)   {
            opuznij -= 5;
            delay(200);
          }
        drugaLinia("DELAY ", opuznij, " takt    ", 0);
        break;
      }
    case 9:                             //ustaw napiecie wejsciowe impulsu
      {
        if (digitalRead(14) == LOW)   {
          napImpulsu += 0.1;
          delay(200);
        }
        if (digitalRead(15) == LOW)   {
          if (poIle > 0)
            napImpulsu -= 0.1;
          delay(200);
        }
        lcd.setCursor(0, 1);
        lcd.print("MIN NAP IMP ");
        lcd.print(napImpulsu);
        break;
      }

    case 10:                             // tu ustawiam napiecie ponizej ktorego traktujemy jak zero
      {
        if (digitalRead(14) == LOW)   {
          zeroNapiecia += 0.1;
          delay(200);
        }
        if (digitalRead(15) == LOW)   {
          if (poIle > 0)
            zeroNapiecia -= 0.1;
          delay(200);
        }
        lcd.setCursor(0, 1);
        lcd.print("MAX NAP ZERA ");
        lcd.print(zeroNapiecia);
        break;
      }


  }

}
void dodaj(int ile) {
  takty++;
  if (takty == coIle) {
    ileWszy += ile;
    takty = 0;

  }
  ilePacz = ileWszy % ustawPacz;
  delay(150);
}
void odejmij(int ile) {
  if (ileWszy > 0) {
    ileWszy -= ile;
    ilePacz = ileWszy % ustawPacz;
  }
  delay(150);
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
  delay(150);
  ekrany++;
  if (ekrany > 10)
    ekrany = 0;
}
void drugaLinia(String raz, int dwa, String trzy, int cztery) {
  lcd.setCursor(0, 1);
  lcd.print(raz);
  lcd.print(dwa);
  lcd.print(trzy);
  lcd.print(cztery);

}
