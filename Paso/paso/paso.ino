#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

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

//********************************************************
//*********VARIABLES**************************************
//********************************************************

long segundos, auxsegundos;
int i, b, clave[4], clave2[2], address, datoi[4], dato1, dato2;
char datom;
short flag;
void setup() {
    // se configuran los pines (INPUT/OUTPUT):
    //SE CONFIGURAN PINES Y SE INICIAN LAS LIBRERIAS
    keypad.getKey();
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
void escribir_memoriaD()
{address=0x02;
  for(i=0;i<2;i++)
    {
      EEPROM.write(address, clave[i]);
      delay(5);
      address++;
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
      
      leer_memoria();
      teclado();

      if(clave[0]==0x12 && clave[1]==0x34)
      {
admin:
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("BIENVENIDOS AL") ;
         lcd.setCursor(3,1);
         lcd.print("MENU");
         delay(2000);

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
      }else if(clave[0]==datoi[0] && clave[1]==datoi[1])
        {
Miguel:  
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
            lcd.print("MEN CAMBIAR CLAV") ;
            lcd.setCursor(0,1);
            lcd.print("DIGITE NUE.CLAVE");
            delay(2000);
            teclado();

              if(clave[0]==dato1 && clave[1]==dato2)
                 {
                     escribir_memoria();
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
            lcd.print("MENU2") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto Miguel;
         }else if(datom==0x03)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU3") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto Miguel;
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
          
         
         
        }else if (clave[0]==datoi[2] && clave[i]==datoi[3])
         {
David:   
        // address=0x03; 
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
            lcd.print("MEN CAMBIAR CLAV") ;
            lcd.setCursor(0,1);
            lcd.print("DIGITE NUE.CLAVE");
            delay(2000);
            teclado();

              if(clave[0]==dato1 && clave[1]==dato2)
                 {
                     escribir_memoriaD();
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
            lcd.print("MENU2") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto David;
         }else if(datom==0x03)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MENU3") ;
            lcd.setCursor(0,1);
            lcd.print("EN CONSTRUCCION");
            delay(2000);
            goto David;
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
             }
}
