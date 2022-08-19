#include <LiquidCrystal.h>





/*Define input pins for DTMF Decoder pins connection*/

LiquidCrystal lcd( 14,15,16,17,18,19 );

//LiquidCrystal lcd( RS,E,D4,D5,D6,D7);




int IN1 = 8;      // pin digital 8 de Arduino a IN1 de modulo controlador
int IN2 = 9;      // pin digital 9 de Arduino a IN2 de modulo controlador
int IN3 = 10;     // pin digital 10 de Arduino a IN3 de modulo controlador
int IN4 = 11;     // pin digital 11 de Arduino a IN4 de modulo controlador
int demora = 20;      // demora entre pasos, no debe ser menor a 10 ms.


void setup() {

lcd.begin(16,2);
  Serial.begin(9600);

  
  pinMode(IN1, OUTPUT);   // todos los pines como salida
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("BIENVENIDOS AL") ;
         lcd.setCursor(3,1);
         lcd.print("SOY VERONI");
         delay(2000);

  for (int i = 0; i < 512; i++) // 512*4 = 2048 pasos
  {
    digitalWrite(IN1, HIGH);  // paso 1 
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    delay(demora);

    digitalWrite(IN1, LOW); // paso 2
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    delay(demora);

    digitalWrite(IN1, LOW); // paso 3
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(demora);

    digitalWrite(IN1, LOW); // paso 4
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(demora);
  }

  digitalWrite(IN1, LOW); // detiene por 5 seg.
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(5000);

}
