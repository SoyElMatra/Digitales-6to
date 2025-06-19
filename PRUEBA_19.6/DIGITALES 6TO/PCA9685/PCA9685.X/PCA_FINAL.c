#include <16F876A.h>
#FUSES XT, NOWDT, NOLVP
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7)
#use I2C(MASTER, SDA=PIN_C4, SCL=PIN_C3, FAST=100000)
#use standard_io(A)
#use standard_io(B)

int i, paquetes=0;
int const lenbuff = 54;             // Longitud del buffer

// VARIABLES EN RAM
int xbuff = 0x00;                   // Índice: siguiente char en cbuff
char cbuff[lenbuff];               // Buffer circular
char rcvchar = 0x00;               // Último caracter recibido
int1 flagcomando = 0;              // Flag para indicar comando disponible 
int1 flag;

// Declaración de Funciones
void inicbuff(void);               // Borra buffer 
int  addcbuff(char c);
void procesa_comando(void);        // Procesa comando

void inicbuff(void) 
{
   int i;
   for (i = 0; i < lenbuff; i++) 
   {
      cbuff[i] = 0x00;
   }
   xbuff = 0x00;
}

#int_rda
void serial_isr() 
{
   rcvchar = 0x00;
   rcvchar = getc();

   if (rcvchar == '*' && flag == 0) 
   {
      flag = 1;
      return;
   }

   if (flag == 1 && rcvchar != '*') 
   {
      cbuff[xbuff++] = rcvchar;
   }

   if (flag == 1 && rcvchar == '*') 
   {
      flagcomando = 1;
      flag = 0;
   }
}

void envio_de_pwm(int canal, int16 off)
{
   int16 adress= 0x06 + 4*canal;
   int16 off_alto= make8(off, 1);
   int16 off_bajo= make8(off, 0);
   
   i2c_start();
   i2c_write(0x80);            // Dirección del PCA
   i2c_write(adress);            // Dirección del LED 1
   i2c_write(0x00);            // led1_on_L
   i2c_write(0x00);            // led1_on_H
   i2c_write(off_bajo);             // led1_off_L (?996 µs)
   i2c_write(off_alto);            // led1_off_H
   i2c_stop();
}

void main() 
{
    inicbuff();
    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);
    delay_ms(1000);
    
    i2c_start();
    i2c_write(0x80);
    i2c_write(0x00);
    i2c_write(0x30);
    i2c_stop();
   
      while (1) 
      {
         if (flagcomando == 1) 
         {
             for(i=0; i<54; i+=3)
             {
                if(cbuff[i]!= 0x00 || cbuff[i+1]!= 0x00 || cbuff[i+2]!= 0x00) 
                {
                    paquetes++; 
                } 
             }
             for(i=0; i<paquetes; i+=3)
             {
                 int16 off_funcion = make16(cbuff[i+1], cbuff[i+2]);
                 envio_de_pwm(cbuff[i], off_funcion);
             }
             
             flagcomando = 0;
             inicbuff();
         }
       }
}


   
