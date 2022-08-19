#include <Wire.h>           //libreria memoriaexterna 
#include <EEPROM.h>         //libreria eeprom
#include <LiquidCrystal.h>  //libreria lcd
#include <Key.h>          
#include <Keypad.h>        //libreria teclado
#define DS1307_I2C_ADDRESS 0x68   



/*****************Convierte  Numeros DEC a BCD************/
byte decToBcd(byte val)
{
  return ((val/10*16)+(val%10)); 
}

//Convierte Numeros BCD a DEC ***********************
byte bcdToDec(byte val)
{
  return ((val/16*10)+(val%16)); 
}


LiquidCrystal lcd(  8, 9, 10, 11, 12, 13);
//LiquidCrystal lcd(RS,E,D4,D5,D6,D7); En el mismo orden se alambra
const byte FILAS=4;
const byte COLUMNAS=4;
byte pinesFilas[]   ={4,5,6,7};
byte pinesColumnas[]={17,16,15,14};
char keys [FILAS][COLUMNAS]={{'1','2','3','A'},
                             {'4','5','6','B'},
                             {'7','8','9','C'},
                             {'*','0','#','D'}
                             };
Keypad keypad=Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS );



void i2c_eeprom_write_byte(int deviceaddress, unsigned int eeaddress, byte data)
{//esta rutina escribe un BYTE
  int rdata=data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >>8)); //MSB
  Wire.write((int)(eeaddress & 0xFF));//LSB
  Wire.write(rdata);
  Wire.endTransmission();
}
        
byte i2c_eeprom_read_byte(int deviceaddress, unsigned int eeaddress)
{//esta rutina lee un BYTE
  byte rdata=0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >>8));//MSB
  Wire.write((int)(eeaddress & 0xFF));//LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  if(Wire.available()) rdata=Wire.read();
     return rdata;
}




//********************************************************
//*********VARIABLES**************************************
//********************************************************/

long segundos, auxsegundos;
int i, b, clave[4], address, datoi[4], dato1, dato2, datoe[2],datoe2[2], direccion, direccion2, contador;
int tiempo[8], ide, cantidad;
char datom;
short flag;
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
short espacio_vacio;
float valor;


void setup() {
    // se configuran los pines (INPUT/OUTPUT):
    //SE CONFIGURAN PINES Y SE INICIAN LAS LIBRERIAS
    //keypad.getKey();
    Wire.begin();
    lcd.begin(16,2);    //inicualizo la libreria de lcd
    Serial.begin(9600); //permita ver lo que pasa
}


void teclado()
{
  lcd.clear();
  auxsegundos=0;
  segundos=0;
  b=5;
   for(i=0;i<4;i++)
   {
     do{
         datom = keypad.getKey();
         delay(60);
          if(datom!='\0')
          {
            switch(i+1)
            {
              case 1:
              
               lcd.setCursor(5,0);
               lcd.print(datom);
               lcd.setCursor(5,1);
               lcd.print("*");
               clave[0]=datom-0x30;
               clave[0]=(clave[0]<<4); //igual al swap
               delay(50);
              break;

               case 2:
                lcd.setCursor(6,0);
                lcd.print(datom);
                lcd.setCursor(6,1);
                lcd.print("*");
                clave[1]=datom-0x30;
                clave[0]=clave[0]+clave[1];
                delay(50);
               break; 

              case 3:
              
               lcd.setCursor(7,0);
               lcd.print(datom);
               lcd.setCursor(7,1);
               lcd.print("*");
               clave[2]=datom-0x30;
               clave[2]=(clave[2]<<4); //igual al swap
               delay(50);
              break;

              case 4:
              
                lcd.setCursor(8,0);
                lcd.print(datom);
                lcd.setCursor(8,1);
                lcd.print("*");
                clave[3]=datom-0x30;
                clave[1]=clave[3]+clave[2];
                delay(50);
               break; 
               
            }i++;
             delay(500);
             auxsegundos=0;
             segundos=0;
          }
            auxsegundos++;
            if(auxsegundos>20)
            {
              segundos++;
              auxsegundos=0;
            }
      
     }while(i<4 && segundos<6);
      clave[i]='#';
      lcd.setCursor(b,0);
      b++;
      auxsegundos=0;
      segundos=0;
     
   }
}

void un_digito()
{
  lcd.clear();
  segundos=0;
  auxsegundos=0;
  flag=false;
   do{
       datom=keypad.getKey();
       delay(60);
       if(datom!='\0')
       {
         lcd.setCursor(7,0);
         lcd.print(datom);
         datom=datom-0x30;
         flag=true;
       }else
            {
              auxsegundos++;
              if(auxsegundos>20)
                {
                  segundos++;
                  auxsegundos=0;
                }
            }
     }while(segundos<6 && flag==false);
  
}
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
  for(i=0;i<2;i++)
     {
       i2c_eeprom_write_byte(0x50, i, clave[i]);
       delay(5);
       }
}


void escribir_memoriaExternaD()
{direccion=0x08;
  for(i=0;i<2;i++)
     {
       i2c_eeprom_write_byte(0x50, direccion, clave[i]);
       delay(5);
       direccion=direccion+0x01;
       }
}

void leer_memoriaExterna()
{
  for(i=0; i<2;i++)
     {
       datoe[i]=i2c_eeprom_read_byte(0x50, i);
       delay(5);
     }
}

void leer_memoriaExternaD()
{direccion=0x08;
  for(i=0; i<2;i++)
     {
       datoe2[i]=i2c_eeprom_read_byte(0x50, direccion);
       delay(5);
       direccion=direccion+0x01;
     }
}

void borrar()
 {direccion=0x00;
   for(i=0;i< 220;i++)
     {
      i2c_eeprom_write_byte(0x50, direccion, 0xFF);
       delay(5);
       direccion++;
     }
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
  Wire.requestFrom(DS1307_I2C_ADDRESS,7);
  *second      =bcdToDec(Wire.read() & 0x7f);
  *minute      =bcdToDec(Wire.read());
  *hour        =bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek   =bcdToDec(Wire.read());
  *dayOfMonth  =bcdToDec(Wire.read());
  *month       =bcdToDec(Wire.read());
  *year        =bcdToDec(Wire.read());
}

void actualizar()
{
  second=0x00;
  minute=0x28;       
  hour=0x10;
  dayOfWeek=0x03; //dia de la semana 
  dayOfMonth=0x02; //22
  month=0x0B;
  year=0x15;   //21
  setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
}

void fecha_hora()
{
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  lcd.setCursor(0,0);
  lcd.print("Hora:");
  lcd.setCursor(5,0);
  lcd.print(hour, DEC);
  
  Serial.print(hour, DEC);
  
  lcd.print(":");
  lcd.setCursor(8,0);
  lcd.print(minute,DEC);
  
  Serial.print(minute, DEC);
  
  lcd.print(":");
  lcd.setCursor(11,0);
  lcd.print(second, DEC);
  
  lcd.setCursor(0,1);
  lcd.print("Fecha:");
  lcd.setCursor(6,1);
  lcd.print(dayOfMonth, DEC);
  lcd.print("/");
  
  Serial.print(dayOfMonth, DEC);
  
  lcd.setCursor(8,1);
  lcd.print(month,DEC);
  lcd.print("/");
  lcd.setCursor(11,1);
  lcd.print(year,DEC);
  delay(10000);
}

void buscar_pos()
{
     espacio_vacio=false;
     direccion2=0x10;
     contador=0;

     do{
         valor=i2c_eeprom_read_byte(0x50, direccion2);
         delay(3);

         if(valor==0xFF)
         {
          espacio_vacio=true;
          Serial.println(direccion2);
         }else{
          direccion2=direccion2+0x08;
         }
         contador=contador+0x01;
         
     }while((direccion2<=0xF0) && (espacio_vacio==false));
     
}

void hacer_reporte()
{
   getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

   tiempo[0]=contador;
   tiempo[1]=dayOfMonth;
   tiempo[2]=month;
   tiempo[3]=year;
   tiempo[4]=hour;
   tiempo[5]=minute;
   tiempo[6]=second;
   tiempo[7]=ide;
   Serial.println();
}

void mostrar_reporte()
{
  direccion2=0x10;
  cantidad=0;

   do{

       for(i=0;i<8;i++)
       {
        tiempo[i]=i2c_eeprom_read_byte(0x50, direccion2);
        direccion2++;
        delay(3);
         Serial.println(tiempo[i]);
       }

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("##DDMMAAHHMMSSID");
        lcd.setCursor(0,1);
        lcd.print(tiempo[0],DEC);
        lcd.setCursor(2,1);
        lcd.print(tiempo[1],DEC);
        lcd.setCursor(4,1);
        lcd.print(tiempo[2], DEC);
        lcd.setCursor(6,1);
        lcd.print(tiempo[3],DEC);
        lcd.setCursor(8,1);
        lcd.print(tiempo[4],DEC);
        lcd.setCursor(10,1);
        lcd.print(tiempo[5],DEC);
        lcd.setCursor(12,1);
        lcd.print(tiempo[6],DEC);
        lcd.setCursor(14,1);
        lcd.print(tiempo[7],DEC);
        delay(2000);
        cantidad++;
         
   }while(cantidad<50);
}


void loop() {
    // put your main code here, to run repeatedly:
menu:  
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("BIENVENIDOS");
      lcd.setCursor(0,1);
      lcd.print("ARQ. COMP. TARDE");
      delay(2000);

      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("MIGUEL MENDEZ");
      lcd.setCursor(0,1);
      lcd.print("DAVID MARTINEZ");
      delay(2000);

      //borrar();
       
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("GRUPO");
      lcd.setCursor(4,1);
      lcd.print("NUMERO");
      delay(2000);

      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("POR FAVOR");
      lcd.setCursor(0,1);
      lcd.print("INGRESE CLAVE");
      delay(2000);

      
      //actualizar(); 
      leer_memoria();
      leer_memoriaExterna();
      leer_memoriaExternaD();
      teclado();

      if(clave[0]==0x12 && clave[1]==0x34)
      {
admin:
         

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("MARQUE 1 PARA EL") ;
         lcd.setCursor(0,1);
         lcd.print("MENU DE MIGUEL");
         delay(2000);

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("MARQUE 2 PARA EL ") ;
         lcd.setCursor(3,1);
         lcd.print("MENU DE DAVID");
         delay(2000);

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("MARQUE 3 PARA") ;
         lcd.setCursor(3,1);
         lcd.print("SALIR");
         delay(2000);

         un_digito();
         if(datom==0x01)
           {
             goto Miguel;
             
           }else if(datom==0x02)
             {
              goto David;
              
             }else if(datom==0x03)
               {
                 goto menu;
                 
               }else 
                   {
                     lcd.clear();
                     lcd.setCursor(0,0);
                     lcd.print("ERROR DE NUMERO");
                     delay(2000); 
                     goto admin;    
                   }
      }else if(clave[0]==datoe[0] && clave[1]==datoe[1])
        {
Miguel:  ide = 0x13;
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("MENU USUARIO") ;
         lcd.setCursor(2,1);
         lcd.print("MIGUEl");
         delay(2000);

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("MARQUE NUM PARA") ;
         lcd.setCursor(0,1);
         lcd.print("1 CAMBIAR CLAVE");
         delay(2000);

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("2 FECHA Y HORA") ;
         lcd.setCursor(0,1);
         lcd.print("3 REPORTES");
         delay(2000);

         
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("4 MOTORPAS.A.PAS") ;
         lcd.setCursor(0,1);
         lcd.print("5 SERVOMOTOR");
         delay(2000);

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("6 CAD") ;
         lcd.setCursor(0,1);
         lcd.print("7 DTMF");
         delay(2000);

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("8 SENSORES") ;
         lcd.setCursor(0,1);
         lcd.print("9 SALIR");
         delay(2000);

         un_digito();
         if(datom==0x01)
         {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MEN CAMBIAR CLAV") ;
            lcd.setCursor(0,1);
            lcd.print("DIGITE NU. CLAVE");
            delay(2000);
            teclado();
            
            dato1=clave[0];
            dato2=clave[1];

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("POR FAVOR DIGITE");
            lcd.setCursor(0,1);
            lcd.print("DIGITE NUE.CLAVE");
            delay(2000);
            teclado();

              if(clave[0]==dato1 && clave[1]==dato2)
                 {
                     escribir_memoriaExterna();
                     lcd.clear();
                     lcd.setCursor(0,0);
                     lcd.print("CLAVE MODIFICADA") ;
                     lcd.setCursor(0,1);
                     lcd.print("CON EXITO");
                     delay(2000);
                     goto Miguel;
                 }else
                      {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("NO SE PUEDE CAM.") ;
                        lcd.setCursor(0,1);
                        lcd.print("CONTRASEÑA");
                        delay(2000);
                        goto Miguel;
                      }
            
            
         }else if(datom==0x02)
         {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU") ;
            lcd.setCursor(0,1);
            lcd.print("FECHA Y HORA");
            delay(2000);
            fecha_hora();
            delay(1000);
            
            goto Miguel;
         }else if(datom==0x03)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU REPORTES");
            delay(2000);

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("1 GENERAR.REPORT");
            lcd.setCursor(0,1);
            lcd.print("2 MOSTRAR.REPORT");
            delay(2000);

            un_digito();
             if(datom==0x01)
               {
                  buscar_pos();
                  if(espacio_vacio==true)
                    {hacer_reporte();
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
                            lcd.print("REPORTE GUARDADO");
                            lcd.setCursor(2,1);
                            lcd.print("CON EXITO");
                            delay(2000);
                            goto Miguel;
                    }else {
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("MEMORIA LLENA");
                            delay(2000);
                            goto Miguel;
                    }
               }else if(datom==0x02)
                 {
                   lcd.clear();
                   lcd.setCursor(0,0);
                   lcd.print("REPORTES GUARDA.");
                   delay(2000);
                   mostrar_reporte();
                   goto Miguel;
                  }
            
            
          }else if(datom==0x04)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU4") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto Miguel;
          }else if(datom==0x05)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU5") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto Miguel;
          }else if(datom==0x06)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU6") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto Miguel;
          }else if(datom==0x07)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU7") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto Miguel;
          }else if(datom==0x08)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU8") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto Miguel;
          }else if(datom==0x09)
          {
            goto admin;
          }
          
         
         
        }else if (clave[0]==datoe2[0] && clave[1]==datoe2[1])
         {
David:   
         ide=0x10;
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("MENU USUARIO") ;
         lcd.setCursor(3,1);
         lcd.print("DAVID");
         delay(2000);

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("MARQUE NUM PARA") ;
         lcd.setCursor(0,1);
         lcd.print("1 CAMBIAR CLAVE");
         delay(2000);

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("2 FECHA Y HORA") ;
         lcd.setCursor(0,1);
         lcd.print("3 REPORTES");
         delay(2000);

         
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("4 MOTORPAS.A.PAS") ;
         lcd.setCursor(0,1);
         lcd.print("5 SERVOMOTOR");
         delay(2000);

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("6 CAD") ;
         lcd.setCursor(0,1);
         lcd.print("7 DTMF");
         delay(2000);

         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("8 SENSORES") ;
         lcd.setCursor(0,1);
         lcd.print("9 SALIR");
         delay(2000);

         un_digito();
         if(datom==0x01)
         {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MEN CAMBIAR CLAV") ;
            lcd.setCursor(0,1);
            lcd.print("DIGITE NU. CLAVE");
            delay(2000);
            teclado();
            
            dato1=clave[0];
            dato2=clave[1];

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("DIGITE NUE.CLAVE");
            delay(2000);
            teclado();

              if(clave[0]==dato1 && clave[1]==dato2)
                 {
                     escribir_memoriaExternaD();
                     lcd.clear();
                     lcd.setCursor(0,0);
                     lcd.print("CLAVE MODIFICADA") ;
                     lcd.setCursor(0,1);
                     lcd.print("CON EXITO");
                     delay(2000);
                     goto David;
                 }else
                      {
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("NO SE PUEDE CAM.") ;
                        lcd.setCursor(0,1);
                        lcd.print("CONTRASEÑA");
                        delay(2000);
                        goto David;
                      }
            
            
         }else if(datom==0x02)
         {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU") ;
            lcd.setCursor(0,1);
            lcd.print("FECHA Y HORA");
            delay(2000);
            fecha_hora();
            delay(1000);
            goto David;
         }else if(datom==0x03)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU REPORTES");
            delay(2000);

            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("1 GENERAR.REPORT");
            lcd.setCursor(0,1);
            lcd.print("2 MOSTRAR.REPORT");
            delay(2000);

            un_digito();
             if(datom==0x01)
               {
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
                            lcd.print("REPORTE GUARDADO");
                            lcd.setCursor(2,1);
                            lcd.print("CON EXITO");
                            goto David;
                    }else {
                            lcd.clear();
                            lcd.setCursor(0,0);
                            lcd.print("MEMORIA LLENA");
                            delay(2000);
                            goto David;
                    }
               }else if(datom==0x02)
                 {
                   lcd.clear();
                   lcd.setCursor(0,0);
                   lcd.print("REPORTES GUARDA.");
                   delay(2000);
                   mostrar_reporte();
                   goto David;
                  }
            
          }else if(datom==0x04)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU4") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto David;
          }else if(datom==0x05)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU5") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto David;
          }else if(datom==0x06)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU6") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto David;
          }else if(datom==0x07)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU7") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto David;
          }else if(datom==0x08)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU8") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto David;
          }else if(datom==0x09)
          {
            goto admin;
          }
          
         }else 
             {
               lcd.clear();
               lcd.setCursor(0,0);
               lcd.print("CONTRASE.") ;
               lcd.setCursor(0,1);
               lcd.print("ERRONEA");
               delay(2000);
               goto menu;
             }
}
