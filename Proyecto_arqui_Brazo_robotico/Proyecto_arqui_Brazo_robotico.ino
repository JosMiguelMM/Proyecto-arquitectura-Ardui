#include <Servo.h>
#include <Wire.h>           //libreria memoriaexterna 
#include <EEPROM.h>         //libreria eeprom
#include <LiquidCrystal.h>  //libreria lcd
#include <Key.h>
#include <Keypad.h>        //libreria teclado
#define DS1307_I2C_ADDRESS 0x68
//DEFINO LOS SERVOMOTORES
Servo serbase;
Servo seraltura;
Servo seradeatras;
Servo serpinza;

LiquidCrystal lcd(  8, 9, 10, 11, 12, 13);
//LiquidCrystal lcd(RS,E,D4,D5,D6,D7); En el mismo orden se alambra


//TECLADO MATRICIAL
const byte FILAS = 4;
const byte COLUMNAS = 4;
byte pinesFilas[]   = {4, 5, 6, 7};
byte pinesColumnas[] = {17, 16, 15, 14};
char keys [FILAS][COLUMNAS] = {{'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}};
Keypad keypad = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS );
//*******************************************************************

//************************************************************
//PROYECTO DE FIN DE AULA
//************************************************************

/*****************Convierte  Numeros DEC a BCD************/
byte decToBcd(byte val)
{
  return ((val / 10 * 16) + (val % 10));
}

//Convierte Numeros BCD a DEC ***********************
byte bcdToDec(byte val)
{
  return ((val / 16 * 10) + (val % 16));
}



void i2c_eeprom_write_byte(int deviceaddress, unsigned int eeaddress, byte data)
{ //esta rutina escribe un BYTE
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); //MSB
  Wire.write((int)(eeaddress & 0xFF));//LSB
  Wire.write(rdata);
  Wire.endTransmission();
}

byte i2c_eeprom_read_byte(int deviceaddress, unsigned int eeaddress)
{ //esta rutina lee un BYTE
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); //MSB
  Wire.write((int)(eeaddress & 0xFF));//LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

//**********************************************************
//**************VARIABLES***********************************
//**********************************************************
int i, clave[4], datoe[2], b, movimiento, gradosbase=0,gradosdeatras=0,gradosserpinza=0, gradosseraltura=0, datoi[4], dato1,dato2;
int direccion2, tiempo[8], contador, secu, cantidad;
char datom, number_pressed;
long segundos, auxsegundos;
short flag, espacio_vacio;
int led1=53;
int led2=51;
int led3=49;
int led4=47;
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
float valor;






void setup() {
  Wire.begin();
  lcd.begin(16, 2);   //inicualizo la libreria de lcd
  Serial.begin(9600); //permita ver lo que pasa

  serbase.attach(28);
  serbase.write(90);
  
  seradeatras.attach(26);
  seradeatras.write(80);
  
  seraltura.attach(24);
  seraltura.write(90);
  
  serpinza.attach(22);
  serpinza.write(120);

  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
  digitalWrite(led4,LOW);

  pinMode(38, INPUT); // connect to Std pin
  pinMode(36, INPUT); // connect to Q4 pin
  pinMode(34, INPUT); // connect to Q3 pin
  pinMode(32, INPUT); // connect to Q2 pin
  pinMode(30, INPUT); // connect to Q1 pin
  
  
  }

//***************************************
//METODOS
//**************************************

void escribir_memoria()
{
  for(i=0;i<2;i++)
    {
      EEPROM.write(i, clave[i]);
      delay(5);
    }
}

void leer_memoria()
{
  for(i=0;i<4;i++)
    {
      datoi[i]=EEPROM.read(i);
      delay(5);
    }
}

void escribir_memoriaExterna()
{
  for (i = 0; i < 2; i++)
  {
    i2c_eeprom_write_byte(0x50, i, clave[i]);
    delay(5);
  }
}

void leer_memoriaExterna()
{
  for (i = 0; i < 2; i++)
  {
    datoe[i] = i2c_eeprom_read_byte(0x50, i);
    delay(5);
  }
}

void borrarMemoria()
{
  for (i = 0; i < 65535; i++)
  {
    i2c_eeprom_write_byte(0x50, i, 0xFF);
    delay(5);
  }
}

void equivalencia()
{ byte signal ;
  signal = digitalRead(38);
  number_pressed = '\0';
  if (signal == HIGH)             /* If new pin pressed */
  {
    delay(10);
    number_pressed = ( 0x00 | (digitalRead(30) << 0) | (digitalRead(32) << 1) | (digitalRead(34) << 2) | (digitalRead(36) << 3) );
    switch (number_pressed)
    {
      case 0x01:
        number_pressed = '1';
        break;
      case 0x02:
        number_pressed = '2';
        break;
      case 0x03:
        number_pressed = '3';
        break;
      case 0x04:
        number_pressed = '4';
        break;
      case 0x05:
        number_pressed = '5';
        break;
      case 0x06:
        number_pressed = '6';
        break;
      case 7:
        number_pressed = '7';
        break;
      case 0x08:
        number_pressed = '8';
        break;
      case 0x09:
        number_pressed = '9';
        break;
      case 0x0A:
        number_pressed = '0';
        break;
      case 0x0B:
        number_pressed = '*';
        break;
      case 0x0C:
        number_pressed = '#';
        break;

      case 0x0D:
        number_pressed = 'A';
        break;

      case 0x0E:
        number_pressed = 'B';
        break;

      case 0x0F:
        number_pressed = 'C';
        break;

      case 0x00:
        number_pressed = 'D';
        break;
    }
  }
}

void teclado_dtmf()
{
  lcd.clear();
  auxsegundos = 0;
  segundos = 0;
  b = 5;

  for (i = 0; i < 4; i++)
  {
    do {
      equivalencia();
      datom = number_pressed;
      delay(60);
      if (datom != '\0')
      {
        switch (i + 1)
        {
          case 1:

            lcd.setCursor(5, 0);
            lcd.print(datom);
            lcd.setCursor(5, 1);
            lcd.print("*");
            clave[0] = datom - 0x30;
            clave[0] = (clave[0] << 4); //igual al swap
            delay(50);
            break;

          case 2:
            lcd.setCursor(6, 0);
            lcd.print(datom);
            lcd.setCursor(6, 1);
            lcd.print("*");
            clave[1] = datom - 0x30;
            clave[0] = clave[0] + clave[1];
            delay(50);
            break;

          case 3:

            lcd.setCursor(7, 0);
            lcd.print(datom);
            lcd.setCursor(7, 1);
            lcd.print("*");
            clave[2] = datom - 0x30;
            clave[2] = (clave[2] << 4); //igual al swap
            delay(50);
            break;

          case 4:

            lcd.setCursor(8, 0);
            lcd.print(datom);
            lcd.setCursor(8, 1);
            lcd.print("*");
            clave[3] = datom - 0x30;
            clave[1] = clave[3] + clave[2];
            delay(50);
            break;

        } i++;
        delay(600);
        auxsegundos = 0;
        segundos = 0;
      }
      auxsegundos++;
      if (auxsegundos > 20)
      {
        segundos++;
        auxsegundos = 0;
      }

    } while (i < 4 && segundos < 6);
    clave[i] = '#';
    lcd.setCursor(b, 0);
    b++;
    auxsegundos = 0;
    segundos = 0;

  }
}


void un_digito_dtmf()
{
  lcd.clear();
  segundos = 0;
  auxsegundos = 0;
  flag = false;
  do {

    equivalencia();
    datom = number_pressed;
    delay(60);
    if (datom != '\0')
    {
      lcd.setCursor(7, 0);
      lcd.print(datom);
      datom = datom - 0x30;
      flag = true;
      delay(100);
    } else
    {
      auxsegundos++;
      if (auxsegundos > 20)
      {
        segundos++;
        auxsegundos = 0;
      }
    }
  } while (segundos < 6 && flag == false);
}

void un_digito_dtmf2()
{
  lcd.clear();
  segundos = 0;
  auxsegundos = 0;
  flag = false;
  do {

    equivalencia();
    datom = number_pressed;
    delay(60);
    if (datom != '\0')
    {
      lcd.setCursor(7, 0);
      lcd.print(datom);
      datom = datom - 0x30;
      flag = true;
      delay(100);
    } else
    {
      auxsegundos++;
      if (auxsegundos > 20)
      {
        segundos++;
        auxsegundos = 0;
      }
    }
  } while (segundos < 2 && flag == false);
}

void teclado()
{
  lcd.clear();
  auxsegundos = 0;
  segundos = 0;
  b = 5;
  for (i = 0; i < 4; i++)
  {
    do {
      datom = keypad.getKey();
      delay(60);
      if (datom != '\0')
      {
        switch (i + 1)
        {
          case 1:

            lcd.setCursor(5, 0);
            lcd.print(datom);
            lcd.setCursor(5, 1);
            lcd.print("*");
            clave[0] = datom - 0x30;
            clave[0] = (clave[0] << 4); //igual al swap
            delay(50);
            break;

          case 2:
            lcd.setCursor(6, 0);
            lcd.print(datom);
            lcd.setCursor(6, 1);
            lcd.print("*");
            clave[1] = datom - 0x30;
            clave[0] = clave[0] + clave[1];
            delay(50);
            break;

          case 3:

            lcd.setCursor(7, 0);
            lcd.print(datom);
            lcd.setCursor(7, 1);
            lcd.print("*");
            clave[2] = datom - 0x30;
            clave[2] = (clave[2] << 4); //igual al swap
            delay(50);
            break;

          case 4:

            lcd.setCursor(8, 0);
            lcd.print(datom);
            lcd.setCursor(8, 1);
            lcd.print("*");
            clave[3] = datom - 0x30;
            clave[1] = clave[3] + clave[2];
            delay(50);
            break;

        } i++;
        delay(500);
        auxsegundos = 0;
        segundos = 0;
      }
      auxsegundos++;
      if (auxsegundos > 20)
      {
        segundos++;
        auxsegundos = 0;
      }

    } while (i < 4 && segundos < 6);
    clave[i] = '#';
    lcd.setCursor(b, 0);
    b++;
    auxsegundos = 0;
    segundos = 0;

  }
}

void un_digito()
{
  lcd.clear();
  segundos = 0;
  auxsegundos = 0;
  flag = false;
  do {
    datom = keypad.getKey();
    delay(60);
    if (datom != '\0')
    {
      lcd.setCursor(7, 0);
      lcd.print(datom);
      datom = datom - 0x30;
      flag = true;
    } else
    {
      auxsegundos++;
      if (auxsegundos > 20)
      {
        segundos++;
        auxsegundos = 0;
      }
    }
  } while (segundos < 6 && flag == false);

}
void r()
{
          movimiento = 90;
          gradosbase = gradosbase + movimiento;
          
          serbase.write(gradosbase);
          Serial.println(gradosbase);
          delay(1000);
}

//*******************Funcion para Escribir en el RTC********************
void setDateDs1307(byte second,
                   byte minute,
                   byte hour,
                   byte dayOfWeek,
                   byte dayOfMonth,
                   byte month,
                   byte year)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}
//********Leer RTC
void getDateDs1307(byte *second,
                   byte *minute,
                   byte *hour,
                   byte *dayOfWeek,
                   byte *dayOfMonth,
                   byte *month,
                   byte *year)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  *second      = bcdToDec(Wire.read() & 0x7f);
  *minute      = bcdToDec(Wire.read());
  *hour        = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek   = bcdToDec(Wire.read());
  *dayOfMonth  = bcdToDec(Wire.read());
  *month       = bcdToDec(Wire.read());
  *year        = bcdToDec(Wire.read());
}

void actualizar()
{
  second = 0x00;
  minute = 0x28;
  hour = 0x0B;
  dayOfWeek = 0x07; //dia de la semana
  dayOfMonth = 0x1B; //22
  month = 0x0B;
  year = 0x15; //21
  setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
}

void fecha_hora()
{
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  lcd.setCursor(0, 0);
  lcd.print("Hora:");
  lcd.setCursor(5, 0);
  lcd.print(hour, DEC);

  Serial.print(hour, DEC);

  lcd.print(":");
  lcd.setCursor(8, 0);
  lcd.print(minute, DEC);

  Serial.print(minute, DEC);
  lcd.setCursor(10, 0);
  lcd.print(":");
  lcd.setCursor(11, 0);
  lcd.print(second, DEC);

  lcd.setCursor(0, 1);
  lcd.print("Fecha:");
  lcd.setCursor(6, 1);
  lcd.print(dayOfMonth, DEC);
  lcd.setCursor(8, 1);
  lcd.print("/");

  Serial.print(dayOfMonth, DEC);

  lcd.setCursor(9, 1);
  lcd.print(month, DEC);
  lcd.print("/");
  lcd.setCursor(12, 1);
  lcd.print(year, DEC);
  delay(10000);
}

void buscar_pos()
{
  espacio_vacio = false;
  direccion2 = 0x10;
  contador = 0;

  do {
    valor = i2c_eeprom_read_byte(0x50, direccion2);
    delay(3);

    if (valor == 0xFF)
    {
      espacio_vacio = true;
      Serial.println(direccion2);
    } else {
      direccion2 = direccion2 + 0x08;
    }
    contador = contador + 0x01;

  } while ((direccion2 <= 0xF0) && (espacio_vacio == false));

}

void hacer_reporte()
{
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  tiempo[0] = contador;
  tiempo[1] = dayOfMonth;
  tiempo[2] = month;
  tiempo[3] = year;
  tiempo[4] = hour;
  tiempo[5] = minute;
  tiempo[6] = second;
  tiempo[7] = secu;
  Serial.println();
}

void mostrar_reporte()
{
  direccion2 = 0x10;

  do {

    for (i = 0; i < 8; i++)
    {
      tiempo[i] = i2c_eeprom_read_byte(0x50, direccion2);
      direccion2++;
      delay(3);
      Serial.println(tiempo[i]);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("##DDMMAAHHMMSSID");
    lcd.setCursor(0, 1);
    lcd.print(tiempo[0], DEC);
    lcd.setCursor(2, 1);
    lcd.print(tiempo[1], DEC);
    lcd.setCursor(4, 1);
    lcd.print(tiempo[2], DEC);
    lcd.setCursor(6, 1);
    lcd.print(tiempo[3], DEC);
    lcd.setCursor(8, 1);
    lcd.print(tiempo[4], DEC);
    lcd.setCursor(10, 1);
    lcd.print(tiempo[5], DEC);
    lcd.setCursor(12, 1);
    lcd.print(tiempo[6], DEC);
    lcd.setCursor(14, 1);
    lcd.print(tiempo[7], DEC);
    delay(2000);
    cantidad++;

  } while ( tiempo[0]!=0xFF);
}

void loop() {

principal:

  actualizar();
 //borrarMemoria();
  //fecha_hora();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BIENVENIDOS AL");
  lcd.setCursor(0, 1);
  lcd.print("BRAZO ROBOTICO");
  delay(2000);//
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INTEGRANTES");
  lcd.setCursor(0, 1);
  lcd.print("DAVID MARTINEZ");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MIGUEL MENDEZ");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("POR FAVOR DIGITE");
  lcd.setCursor(0, 1);
  lcd.print("LA CONTRASEÑA");
  delay(2000);
  
  leer_memoriaExterna();
  leer_memoria();
  Serial.println(datoi[0]);
  Serial.println(datoi[1]);
  teclado_dtmf();
  
  
  //teclado()
  Serial.println("teclado");
  Serial.println(clave[0]);
  Serial.println(clave[1]);
  
 if ((clave[0] == datoe[0]) && (clave[1] == datoe[1]))
  {
inter:    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PRESIONE");
    lcd.setCursor(0, 1);
    lcd.print("1)CAMBIO CONTRA.");
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PRESIONE");
    lcd.setCursor(0, 1);
    lcd.print("2)MODO OP MANUAL");
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PRESIONE");
    lcd.setCursor(0, 1);
    lcd.print("3)OP. SECUENCIAS");
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PRESIONE");
    lcd.setCursor(0, 1);
    lcd.print("4)REGISTROS");
    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DE INGRESO");
    lcd.setCursor(0, 1);
    lcd.print("5)SALIR");
    delay(2000);
         
    un_digito_dtmf();
    if(datom==0x01)
     {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MEN CAMBIAR CLAV") ;
            lcd.setCursor(0,1);
            lcd.print("DIGITE NU. CLAVE");
            delay(2000);
            teclado_dtmf();
            
            dato1=clave[0];
            dato2=clave[1];

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MEN CAMBIAR CLAV") ;
            lcd.setCursor(0,1);
            lcd.print("DIGITE NUE.CLAVE");
            delay(2000);
            teclado_dtmf();
            //teclado();

              if(clave[0]==dato1 && clave[1]==dato2)
                 {
                     //escribir_memoriaExterna();
                     escribir_memoria();
                     
                     lcd.clear();
                     lcd.setCursor(0,0);
                     lcd.print("CLAVE MODIFICADA") ;
                     lcd.setCursor(0,1);
                     lcd.print("CON EXITO");
                     delay(2000);
                     goto inter;
                 }else
                      {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("NO SE PUEDE CAM.") ;
                        lcd.setCursor(0,1);
                        lcd.print("CONTRASEÑA");
                        delay(2000);
                        goto inter;
                      }
            
     }
    
    else if (datom == 0x02)
    { secu=0x00;    
      buscar_pos();
          if(espacio_vacio==true)
                {
                  hacer_reporte();
                    for(i=0;i<8;i++)
                          {
                            i2c_eeprom_write_byte(0x50, direccion2, tiempo[i]);
                            delay(3);
                            float le=i2c_eeprom_read_byte(0x50, direccion2);
                            delay(3);
                            Serial.println(le);
                            direccion2=direccion2+0x01;
                            
                          }
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("REGISTRO DE INGR");
                            lcd.setCursor(2,1);
                            lcd.print("GUARDADO");
                            delay(2000);
                }else {
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("MEMORIA LLENA");
                            delay(2000);
                         }
    
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Modo Manual");
          lcd.setCursor(0,0);
          lcd.print("PULSE");
          delay(2000);

          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("1)IZQUIERDA BASE");
          lcd.setCursor(0,0);
          lcd.print("2)DERECHA BASE");
          delay(2000);

          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("3)");
          lcd.setCursor(0,0);
          lcd.print("4)");
          delay(2000);
          
          
      do {
        
        un_digito_dtmf();
        //un_digito();
        if (datom == 0x01)
        {
          if(gradosbase<0)
          {
            gradosbase=0;
          }else if(gradosbase>180)
            {
              gradosbase=180;
            }
          
          movimiento = 10;
          gradosbase = gradosbase + movimiento;
          digitalWrite(led1,HIGH);
          digitalWrite(led2,LOW);
          digitalWrite(led3,LOW);
          digitalWrite(led4,LOW);
          
          serbase.write(gradosbase);
          delay(10);

          
        } else if (datom == 0x02)
        {
          if(gradosbase<0)
          {
            gradosbase=0;
          }else if(gradosbase>180)
            {
              gradosbase=180;
            }
          movimiento = -10;
          gradosbase = gradosbase + (movimiento);
          digitalWrite(led1,HIGH);
          digitalWrite(led2,LOW);
          digitalWrite(led3,LOW);
          digitalWrite(led4,LOW);
          serbase.write(gradosbase);
          delay(10);
          
        }else if(datom==0x03)
        {
          if(gradosdeatras<0)
          {
            gradosdeatras=0;
          }else if(gradosdeatras>180)
            {
              gradosdeatras=180;
            }
          movimiento=10;
          gradosdeatras=gradosdeatras+movimiento;
          digitalWrite(led1,LOW);
          digitalWrite(led2,HIGH);
          digitalWrite(led3,LOW);
          digitalWrite(led4,LOW);
          seradeatras.write(gradosdeatras);
          delay(10);
          
        }
        else if(datom==0x04)
        {
          if(gradosdeatras<0)
          {
            gradosdeatras=0;
          }else if(gradosdeatras>180)
            {
              gradosdeatras=180;
            }
          movimiento=-10;
          gradosdeatras=gradosdeatras+(movimiento);
          digitalWrite(led1,LOW);
          digitalWrite(led2,HIGH);
          digitalWrite(led3,LOW);
          digitalWrite(led4,LOW);
          seradeatras.write(gradosdeatras);
          delay(10);
        }else if(datom==0x05)
        {
          if(gradosseraltura<0)
          {
            gradosseraltura=0;
          }else if(gradosseraltura>180)
            {
              gradosseraltura=180;
            }
          movimiento=10;
          gradosseraltura=gradosseraltura+movimiento;
          digitalWrite(led1,LOW);
          digitalWrite(led2,LOW);
          digitalWrite(led3,HIGH);
          digitalWrite(led4,LOW);
          seraltura.write(gradosseraltura);
          delay(10);
          
        }else if(datom==0x06)
        {
          if(gradosseraltura<0)
          {
            gradosseraltura=0;
          }else if(gradosseraltura>180)
            {
              gradosseraltura=180;
            }
          movimiento=-10;
          gradosseraltura=gradosseraltura+(movimiento);
          digitalWrite(led1,LOW);
          digitalWrite(led2,LOW);
          digitalWrite(led3,HIGH);
          digitalWrite(led4,LOW);
          seraltura.write(gradosseraltura);
          delay(10);
          
        }else if(datom==0x07)
        {
          if(gradosserpinza<90)
          {
            gradosserpinza=90;
          }else if(gradosserpinza>180)
            {
              gradosserpinza=180;
            }
          movimiento=10;
          gradosserpinza=gradosserpinza+movimiento;
          digitalWrite(led1,LOW);
          digitalWrite(led2,LOW);
          digitalWrite(led3,LOW);
          digitalWrite(led4,HIGH);
          serpinza.write(gradosserpinza);
          delay(10); 
          
        }else if(datom==0x08)
        {
          if(gradosserpinza<90)
          {
            gradosserpinza=90;
          }else if(gradosserpinza>180)
            {
              gradosserpinza=180;
            }
          movimiento=-10;
          gradosserpinza=gradosserpinza+movimiento;
          digitalWrite(led1,LOW);
          digitalWrite(led2,LOW);
          digitalWrite(led3,LOW);
          digitalWrite(led4,HIGH);
          serpinza.write(gradosserpinza);
          delay(10); 
        }

      } while (datom != 0x09);
      goto inter;
      
    }else if(datom==0x03)
       {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("SECUENCIAS");
          lcd.setCursor(0, 1);
          lcd.print("PRECIONE PARA");
          delay(2000);
          
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ELEGIR");
          lcd.setCursor(0, 1);
          lcd.print("SEGUN SU UTILID.");
          delay(2000);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("1)CENTRO DERECHA");
          lcd.setCursor(0, 1);
          lcd.print("2)IZQUIER DERECH");
          delay(2000);
          
          un_digito_dtmf();
          //un_digito();
          
          if(datom==0x01) //secuencia 1
            { secu=0x01;    
             buscar_pos();
          if(espacio_vacio==true)
                {
                  hacer_reporte();
                    for(i=0;i<8;i++)
                          {
                            i2c_eeprom_write_byte(0x50, direccion2, tiempo[i]);
                            delay(3);
                            float le=i2c_eeprom_read_byte(0x50, direccion2);
                            delay(3);
                            Serial.println(le);
                            direccion2=direccion2+0x01;
                            
                          }
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("REGISTRO DE INGR");
                            lcd.setCursor(2,1);
                            lcd.print("GUARDADO");
                            delay(2000);
                }else {
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("MEMORIA LLENA");
                            delay(2000);
                         }
              
              fecha_hora();
              do{
                  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
                
                 
                 
                 digitalWrite(led1,LOW);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,LOW);
                 serbase.write(80);
                 seradeatras.write(80);
                 seraltura.write(110);
                 serpinza.write(180);
                 delay(2000);
                
                 digitalWrite(led1,LOW);
                 digitalWrite(led2,HIGH);
                 digitalWrite(led3,HIGH);
                 digitalWrite(led4,LOW);
                 serbase.write(80);
                 seradeatras.write(130);
                 seraltura.write(30);
                 serpinza.write(180);
                 delay(2000);

                 
                 digitalWrite(led1,LOW);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,HIGH);
                 serbase.write(80);
                 seradeatras.write(130);
                 seraltura.write(30);
                 serpinza.write(90);
                 delay(2000);

                 
                 digitalWrite(led1,LOW);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,HIGH);
                 serbase.write(80);
                 seradeatras.write(130);
                 seraltura.write(30);
                 serpinza.write(180);
                 delay(3000);

                 
                 digitalWrite(led1,LOW);
                 digitalWrite(led2,HIGH);
                 digitalWrite(led3,HIGH);
                 digitalWrite(led4,LOW);
                 serbase.write(80);
                 seradeatras.write(80);
                 seraltura.write(110);
                 serpinza.write(180);
                 delay(3000);

                 
                 digitalWrite(led1,HIGH);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,LOW);
                 serbase.write(0);
                 seradeatras.write(80);
                 seraltura.write(110);
                 serpinza.write(180);
                 delay(2000);

                 
                 digitalWrite(led1,LOW);
                 digitalWrite(led2,HIGH);
                 digitalWrite(led3,HIGH);
                 digitalWrite(led4,LOW);
                 serbase.write(0);
                 seradeatras.write(130);
                 seraltura.write(30);
                 serpinza.write(180);
                 delay(2000);

                 
                 digitalWrite(led1,LOW);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,HIGH);
                 serbase.write(0);
                 seradeatras.write(130);
                 seraltura.write(30);
                 serpinza.write(90);
                 delay(2000);

                 
                 digitalWrite(led1,LOW);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,HIGH);
                 serbase.write(0);
                 seradeatras.write(130);
                 seraltura.write(30);
                 serpinza.write(180);
                 delay(2000);

                 
                 digitalWrite(led1,LOW);
                 digitalWrite(led2,HIGH);
                 digitalWrite(led3,HIGH);
                 digitalWrite(led4,LOW);
                 serbase.write(0);
                 seradeatras.write(80);
                 seraltura.write(110);
                 serpinza.write(180);
                 delay(2000);

                 
                 digitalWrite(led1,HIGH);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,LOW);
                 serbase.write(80);
                 seradeatras.write(80);
                 seraltura.write(110);
                 serpinza.write(180);
                 delay(1000);
                 
              }while((minute!=5) );
            }
        else  if(datom==0x02) //secuencia 2
            {secu=0x02;    
             buscar_pos();
          if(espacio_vacio==true)
                {
                  hacer_reporte();
                    for(i=0;i<8;i++)
                          {
                            i2c_eeprom_write_byte(0x50, direccion2, tiempo[i]);
                            delay(3);
                            float le=i2c_eeprom_read_byte(0x50, direccion2);
                            delay(3);
                            Serial.println(le);
                            direccion2=direccion2+0x01;
                            
                          }
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("REGISTRO DE INGR");
                            lcd.setCursor(2,1);
                            lcd.print("GUARDADO");
                            delay(2000);
                }else {
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("MEMORIA LLENA");
                            delay(2000);
                         }
              
              fecha_hora();
              do{
                 getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
                 
                 digitalWrite(led1,LOW);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,LOW);     
                 serbase.write(170);
                 seradeatras.write(80);
                 seraltura.write(120);
                 serpinza.write(180);
                 delay(2000);

                 digitalWrite(led1,LOW);
                 digitalWrite(led2,HIGH);
                 digitalWrite(led3,HIGH);
                 digitalWrite(led4,LOW);
                 serbase.write(170);
                 seradeatras.write(160);
                 seraltura.write(50);
                 serpinza.write(180);
                 delay(2000);

                 digitalWrite(led1,LOW);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,HIGH);
                 serbase.write(170);
                 seradeatras.write(160);
                 seraltura.write(50);
                 serpinza.write(90);
                 delay(2000);

                 digitalWrite(led1,LOW);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,HIGH);
                 serbase.write(170);
                 seradeatras.write(160);
                 seraltura.write(50);
                 serpinza.write(180);
                 delay(3000);

                 digitalWrite(led1,LOW);
                 digitalWrite(led2,HIGH);
                 digitalWrite(led3,HIGH);
                 digitalWrite(led4,LOW);
                 serbase.write(170);
                 seradeatras.write(80);
                 seraltura.write(120);
                 serpinza.write(180);
                 delay(3000);

                 digitalWrite(led1,HIGH);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,LOW);
                 serbase.write(0);
                 seradeatras.write(80);
                 seraltura.write(120);
                 serpinza.write(180);
                 delay(2000);

                 digitalWrite(led1,LOW);
                 digitalWrite(led2,HIGH);
                 digitalWrite(led3,HIGH);
                 digitalWrite(led4,LOW);
                 serbase.write(0);
                 seradeatras.write(160);
                 seraltura.write(60);
                 serpinza.write(180);
                 delay(2000);

                 digitalWrite(led1,LOW);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,HIGH);
                 serbase.write(0);
                 seradeatras.write(160);
                 seraltura.write(60);
                 serpinza.write(90);
                 delay(2000);

                 digitalWrite(led1,LOW);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,HIGH);
                 serbase.write(0);
                 seradeatras.write(160);
                 seraltura.write(60);
                 serpinza.write(180);
                 delay(2000);

                 digitalWrite(led1,LOW);
                 digitalWrite(led2,HIGH);
                 digitalWrite(led3,HIGH);
                 digitalWrite(led4,LOW);
                 serbase.write(0);
                 seradeatras.write(80);
                 seraltura.write(120);
                 serpinza.write(180);
                 delay(2000);

                 digitalWrite(led1,HIGH);
                 digitalWrite(led2,LOW);
                 digitalWrite(led3,LOW);
                 digitalWrite(led4,LOW);
                 serbase.write(170);
                 seradeatras.write(80);
                 seraltura.write(120);
                 serpinza.write(180);
                 delay(1000);

              }while(minute!=5);
            }
         else if(datom==0x03)
            {  secu=0x03;    
            buscar_pos();
          if(espacio_vacio==true)
                {
                  hacer_reporte();
                    for(i=0;i<8;i++)
                          {
                            i2c_eeprom_write_byte(0x50, direccion2, tiempo[i]);
                            delay(3);
                            float le=i2c_eeprom_read_byte(0x50, direccion2);
                            delay(3);
                            Serial.println(le);
                            direccion2=direccion2+0x01;
                            
                          }
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("REGISTRO DE INGR");
                            lcd.setCursor(2,1);
                            lcd.print("GUARDADO");
                            delay(2000);
                }else {
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("MEMORIA LLENA");
                            delay(2000);
                         }
              
              fecha_hora();
              do{
                 getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
               
                 serbase.write(170);
                 seradeatras.write(80);
                 seraltura.write(120);
                 serpinza.write(180);
                 delay(2000);

                 serbase.write(170);
                 seradeatras.write(160);
                 seraltura.write(50);
                 serpinza.write(180);
                 delay(2000);

                 serbase.write(170);
                 seradeatras.write(160);
                 seraltura.write(50);
                 serpinza.write(90);
                 delay(2000);

                 serbase.write(170);
                 seradeatras.write(160);
                 seraltura.write(50);
                 serpinza.write(180);
                 delay(3000);

                 serbase.write(170);
                 seradeatras.write(80);
                 seraltura.write(120);
                 serpinza.write(180);
                 delay(3000);
                 
                 serbase.write(0);
                 seradeatras.write(80);
                 seraltura.write(120);
                 serpinza.write(180);
                 delay(2000);

                 serbase.write(0);
                 seradeatras.write(160);
                 seraltura.write(60);
                 serpinza.write(180);
                 delay(2000);

                 serbase.write(0);
                 seradeatras.write(160);
                 seraltura.write(60);
                 serpinza.write(90);
                 delay(2000);

                 serbase.write(0);
                 seradeatras.write(160);
                 seraltura.write(60);
                 serpinza.write(180);
                 delay(2000);

                 serbase.write(0);
                 seradeatras.write(80);
                 seraltura.write(120);
                 serpinza.write(180);
                 delay(2000);

                 serbase.write(170);
                 seradeatras.write(80);
                 seraltura.write(120);
                 serpinza.write(180);
                 delay(1000);

              }while(minute!=3);
            }
          goto inter;
       }else if(datom==0x04)
       {
        mostrar_reporte();
        
       }else if (datom==0x05)
          {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("HA SALIDO ");
            lcd.setCursor(0, 1);
            lcd.print("DEL MENU ");
            delay(2000);
            goto inter;
          }else {
          goto inter;
          }
    goto inter;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LA CONTRASEÑA");
    lcd.setCursor(0, 1);
    lcd.print("ES ERRONEA");
    delay(2000);
    goto principal;
  }
 }
