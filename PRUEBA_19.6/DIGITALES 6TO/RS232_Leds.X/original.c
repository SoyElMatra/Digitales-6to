#include <16f873a.h>                          // Definiciones del PIC 16F873A
#fuses HS,NOWDT,NOPROTECT,NOLVP,PUT,BROWNOUT  // Los Fuses de siempre
#use delay(clock=20000000)                     // Oscilador a 4 Mhz
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)// RS232 Estándar
#include <LCD.c>    

// CONSTANTES /////////////////////////////////////////////////////////////////

int const lenbuff=32;                  // Longitud de buffer, Ajustar
                                       // a lo que desees (o te sea posible)

// VARIABLES EN RAM ///////////////////////////////////////////////////////////

int  xbuff=0x00;                       // Índice: siguiente char en cbuff
char cbuff[lenbuff];                   // Buffer
char rcvchar=0x00;                     // último caracter recibido
int1 flagcomando=0;                    // Flag para indicar comando disponible 
int1 flag;
// Declaración de Funciones ///////////////////////////////////////////////////

void inicbuff(void);                   // Borra buffer 
int  addcbuff(char c);
void procesa_comando(void);            // Procesa comando

void inicbuff(void){                   // Inicia a  cbuff -------------------
   int i;

   for(i=0;i<lenbuff;i++){             // Bucle que pone a 0 todos los
      cbuff[ i ]=0x00;                   // caracteres en el buffer
   }
   xbuff=0x00;                         // Inicializo el indice de siguiente
                                       // caracter
}

#int_rda
void serial_isr() {                    // Interrupción recepción serie USART

   rcvchar=0x00;                       // Inicializo caracter recibido                    
   if(kbhit()){                        // Si hay algo pendiente de recibir ...
      rcvchar=getc();                  // lo descargo y ...
      if(rcvchar=='*'&&flag==0){
          flag=1;
          return;
      }
      if(flag==1&&rcvchar!='*'){
      cbuff[xbuff++]=rcvchar;          // Añade caracter recibido al Buffer
      }
      if(flag==1&&rcvchar=='*'){
      flagcomando=1;
      flag=0;
      }// Añade caracter recibido al Buffer
      }
      //addcbuff(rcvchar);               // lo añado al buffer y ...
     // echos(rcvchar);                  // hago eco (si procede).
   }



void main() {
lcd_init();
   inicbuff();                                   // Borra buffer al inicio
   
    enable_interrupts(int_rda);                   // Habilita Interrupción RDA
   enable_interrupts(global);                    // Habilita interrupciones
 lcd_putc("\fPrueba String");
   do {
   
      if(flagcomando==1){
          printf(lcd_putc,"\f%s", cbuff);
          flagcomando=0;
          inicbuff();
          
      }         // Si hay comando pendiente
                                                 // de procesar ... lo procesa.

   } while (TRUE);

}