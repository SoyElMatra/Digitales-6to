#include <16F877.h>
#fuses XT,NOWDT,NOPROTECT,NOLVP
#use delay(clock=4000000)
#use i2c(Master,sda=PIN_C4,scl=PIN_C3)          //Configuración I2C
#include <lcd.c>

#define EEPROM_ADDRESS long int  

void write_ext_eeprom(long int address, BYTE data)
{
 
   i2c_start();              //Inicializa la transmisión
  i2c_write(0xA0);          //Escribe la palabra de control (dirección 0h + 0 para escritura)
  i2c_write(address>>8);   //Parte alta de la dirección a escribir en la eeprom.
   i2c_write(address);      //Parte baja de la dirección a escribir en la eeprom.
   i2c_write(data);         //Dato a escribir
   i2c_stop();              //Finalización de la transmisión.
}


BYTE read_ext_eeprom(long int address) {
   BYTE data;
   i2c_start();                        //Inicializa la transmisión
   i2c_write(0xA0);           //Escribe la palabra de control (dirección 0h + 0 para escritura)
   i2c_write(address>>8);   //Parte alta de la dirección a escribir en la eeprom.
   i2c_write(address);         //Parte baja de la dirección a escribir en la eeprom.
   i2c_start();                      //Reinicio
   i2c_write(0xa1);             //Escribe la palabra de control (dirección 0h + 1 para lectura)
   data=i2c_read(0);           //lectura del dato
   i2c_stop();                      //Finalización de la transmisión.
   return(data);
}


void main() {
 
   int8 valor=0, dato;
   EEPROM_ADDRESS address;
   lcd_init();
      
    for (address=0;address<=9;address++) {
      WRITE_EXT_EEPROM(address, valor);
      lcd_gotoxy(1,1);
      printf(lcd_putc,"enviando=%1D",valor);
      delay_ms(500);
      valor=valor+2 ;
   }
   for (address=0;address<=9;address++) {
      dato=READ_EXT_EEPROM( address);
      lcd_gotoxy(1,2);
      printf(lcd_putc,"recibiendo=%1D",dato);
      delay_ms(500);
   }
 }


