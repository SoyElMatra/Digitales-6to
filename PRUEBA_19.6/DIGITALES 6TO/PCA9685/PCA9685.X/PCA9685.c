#include <16F876A.h>
#FUSES XT, NOWDT, NOLVP
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7)
#use I2C(MASTER, SDA=PIN_C4, SCL=PIN_C3, FAST=100000)
#use standard_io(A)
#use standard_io(B)

int i;

/***************************** Envío I2C **************************************/
void envio_I2C() {

   i2c_start();                // Comienzo de la comunicación I2C
   i2c_write(0x80);            // Dirección del PCA9685
   i2c_write(0x00);            // Dirección del registro MODE1
   i2c_write(0x30);            // Auto-incremento + sleep
   i2c_stop();

   i2c_start();
   i2c_write(0x80);            // Dirección del PCA9685
   i2c_write(0xFE);            // Dirección del registro PREESCALE
   i2c_write(0x79);            // Valor preescaler para 50 Hz
   i2c_stop();

   i2c_start();
   i2c_write(0x80);            // Dirección del PCA
   i2c_write(0x06);            // Dirección del LED 0
   i2c_write(0x00);            // led0_on_L
   i2c_write(0x00);            // led0_on_H
   i2c_write(0x00);            // led0_off_L
   i2c_write(0x01);            // led0_off_H
   i2c_stop();

   i2c_start();
   i2c_write(0x80);            // Dirección del PCA
   i2c_write(0x0A);            // Dirección del LED 1
   i2c_write(0x00);            // led1_on_L
   i2c_write(0x00);            // led1_on_H
   i2c_write(204);             // led1_off_L (?996 µs)
   i2c_write(0x00);            // led1_off_H
   i2c_stop();

   i2c_start();
   i2c_write(0x80);            // Dirección del PCA
   i2c_write(0x0E);            // Dirección del LED 2
   i2c_write(0x00);            // led2_on_L
   i2c_write(0x00);            // led2_on_H
   i2c_write(0x9A);            // led2_off_L (parte baja de 410)
   i2c_write(0x01);            // led2_off_H (parte alta de 410 ? 2001 µs)
   i2c_stop();

   i2c_start();
   i2c_write(0x80);            // Dirección del PCA
   i2c_write(0x12);            // Dirección del LED 3
   i2c_write(0x00);            // led3_on_L
   i2c_write(0x00);            // led3_on_H
   i2c_write(0x33);            // led3_off_L
   i2c_write(0x01);            // led3_off_H
   i2c_stop();
}
/******************************************************************************/

int const lenbuff = 54;             // Longitud del buffer

// VARIABLES EN RAM ///////////////////////////////////////////////////////////
int xbuff = 0x00;                   // Índice: siguiente char en cbuff
char cbuff[lenbuff];               // Buffer circular
char rcvchar = 0x00;               // Último caracter recibido
int1 flagcomando = 0;              // Flag para indicar comando disponible 
int1 flag;

// Declaración de Funciones ///////////////////////////////////////////////////
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

void main() {
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);

   while (TRUE) 
   {
      delay_ms(1000);
      envio_I2C();

      while (1) 
      {
         if (flagcomando == 1) 
         {
            i = 0;
            i2c_start();             // Comienzo de la comunicación I2C
            i2c_write(0x08);         // Dirección del PIC esclavo
            i2c_write('*');

            while (cbuff[i] != 0 && i < 54) 
            {
               i2c_write(cbuff[i]);
               i++;
            }

            i2c_write('*');
            i2c_stop();

            flagcomando = 0;
            inicbuff();
         }
      }
   }
}
