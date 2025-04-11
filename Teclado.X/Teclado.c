#include <16F876.h>
#fuses XT,NOWDT,NOPROTECT,NOLVP
#use delay(clock= 4000000)     
#include <lcd.c>	//rutina del lcd
#include <kbd1.c>	//rutina del teclado



void main() {
   char k; 
          
   lcd_init(); 
   kbd_init();
   port_b_pullups(TRUE); 

   lcd_putc("\fListo...\n");			//imprime listo y va a la primer posicion segunda linea
      lcd_putc("HOLA");                               
   while (1) {
      k=kbd_getc(); 					//devuelve la tecla pulsada si no se apreta ninguna tecla devuelve el 0 en decimal
      if(k!=0){							//si k es distinto de cero
         lcd_putc('\f'); 				//limpia el display
           lcd_putc(k);             //Imprime carácter
           delay_ms(50);
         
		}
      }
   }



