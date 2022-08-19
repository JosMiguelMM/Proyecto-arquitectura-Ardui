#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(   4,5, 6, 7, 8, 9);
//LiquidCrystal lcd(RS,E,D4,D5,D6,D7); En el mismo orden se alambra
int i, dato;

//METODOS Y FUNCIONES PARA LIBRERIAS
void setup() 
{  //entradas o salidas
  //SE CONFIGURAN PINES Y SE INICIAN LAS LIBRERIAS
  //PIN13 COMO SALIDA
  pinMode(13,OUTPUT);
  pinMode(2,INPUT);
  lcd.begin(16,2);    //inicualizo la libreria de lcd
  Serial.begin(9600); //permita ver lo que pasa
}

//METODOS
void secuencia(){
  for(i=0;i<5; i++){
    digitalWrite(13,HIGH);
    Serial.print("LED ON");
    delay(1000);
    digitalWrite(13,LOW);
    delay(1000);
  }
}
void loop() {
  //Va el programa

  lcd.clear();
  lcd.setCursor(0,0);  //Empieza en 0
  lcd.print("Bienvenidos");
  lcd.setCursor(0,1);  //Empieza en 0
  lcd.print("ART.COM.TARDE.");
  delay(3000);
  //dato=digitalRead(2);  //leea
  
  //if(dato==LOW){
  //for(;;) for sin limite
  dato=analogRead(A0);
  if(dato<100){
    secuencia();
    delay(3000);
  
 }
 delay(100);
}

//METODOS
