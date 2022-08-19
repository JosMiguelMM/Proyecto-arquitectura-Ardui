// Declaracion de variables globales
float tempC; // Variable para almacenar el valor obtenido del sensor (0 a 1023)
int pinLM35 = A2; // Variable del pin de entrada del sensor (A0)
int i;
float suma;
void setup() {
  // Cambiamos referencia de las entradas analógicas
 
  //analogReference(INTERNAL1V1);
  // Configuramos el puerto serial a 9600 bps
  Serial.begin(9600);
}
 
void loop() {
 // tempC = analogRead(pinLM35); 
   
  // Calculamos la temperatura con la fórmula
   //tempCr = (5.0 * tempC * 100.0)/1024.0; 
  //tempCr=((tempC*5000.0)/1023)/10;
  //tempCr=tempC/10;
  suma=0;
  
  for(i=0;i<5;i++)
    {
      tempC = analogRead(A0);
      tempC= (5.0 * tempC * 100.0)/1024.0;
      suma=suma+tempC;
      delay(3);
    }
     float pro=suma/5.0;
     Serial.println(pro);
  
  // Esperamos un tiempo para repetir el loop
  delay(1000);
}
