#include <Servo.h>
Servo sermotor;
Servo sermotor2;
int i;
void setup() {
  pinMode(53, OUTPUT);
  sermotor.attach(22);
}

void loop() {
 /*for(int i=0;i<180;i++){ 
  sermotor.write(i);
  delay(100);
 }*/

 /*for(int i=360;i>=0;i--){ 
  sermotor.write(i);
  delay(10);
 }*/
  sermotor.write(180);
  delay(1000);

  

  
  
  
}
