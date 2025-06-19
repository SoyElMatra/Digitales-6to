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
int pwm1=0;
int pwm2=0;
int pwm3=0;
int flag1=0;
int contador1=0, contador2=0, contador3=0;
int paquetes = 0;
// Declaración de Funciones ///////////////////////////////////////////////////

void inicbuff(void);                   // Borra buffer 
int  addcbuff(char c);
int resultado_proceso(int);

void inicbuff(void){                   // Inicia a  cbuff -------------------
   int i;

   for(i=0;i<lenbuff;i++){             // Bucle que pone a 0 todos los
      cbuff[ i ]=0x00;                   // caracteres en el buffer
   }
   xbuff=0x00;                         // Inicializo el indice de siguiente
                                       // caracter
}

#int_rda
void serial_isr() 
{                    // Interrupción recepción serie USART

    rcvchar=0x00;                       // Inicializo caracter recibido                    
    if(kbhit())
    {                        // Si hay algo pendiente de recibir ...
       rcvchar=getc();                  // lo descargo y ...
       if(rcvchar=='*'&&flag==0)
       {
           flag=1;
           return;
       }
       if(flag==1&&rcvchar!='*')
       {
       cbuff[xbuff++]=rcvchar;          // Añade caracter recibido al Buffer
       }
       if(flag==1&&rcvchar=='*')
       {
       flagcomando=1;
       flag=0;
       }// Añade caracter recibido al Buffer
     }

   }

#INT_TIMER1
void isr_int_tmr1(void)
{
    if(contador1!=0)
    {
        contador1--;
        if(contador1==1)
        {
            output_toggle(PIN_A0);
            contador1=pwm1;
        }
    }
    if(contador2!=0)
    {
        contador2--;
        if(contador2==1)
        {
            output_toggle(PIN_A1);
            contador2=pwm2;
        }
    }
    if(contador3!=0)
    {
        contador3--;
        if(contador3==1)
        {
            output_toggle(PIN_A2);
            contador3=pwm3;
        }
    }     

    set_timer1(10000);
}


void main() 
{
    inicbuff();
    enable_interrupts(global);
    enable_interrupts(int_rda);                   // Habilita Interrupción RDA
    enable_interrupts(INT_TIMER1);                    // Habilita interrupciones
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
    set_timer1(10000);
    
    while(1)
   {
       
      if(flagcomando==1)
      {
          
          
          /*for(int i=0; i<lenbuff; i++)
          {
              int indice=0;
              
              if(indice==0)
              {
                  if(cbuff[i]== '1')
                  {
                      //output_high(PIN_A0);
                      if(cbuff[i+1]=='1')
                      {
                          pwm1=50;
                          contador1=50;
                          i++;
                          continue;
                      }
                      else if(cbuff[i+1]=='2')
                      {
                          pwm1=100;
                          contador1=100;
                          i++;
                          continue;
                      }
                      else if(cbuff[i+1]=='3')
                      {
                          pwm1=150;
                          contador1=150;
                          i++;
                          continue;
                      }
                      else if(cbuff[i+1]=='4')
                      {
                          output_high(PIN_A0);
                          i++;
                          pwm1=0;
                          contador1=0;
                          continue;
                      }
                      else
                      {
                          output_low(PIN_A0);
                          i++;
                          pwm1=0;
                          contador1=0;
                          continue;
                      }
                  }
                  if(cbuff[i]== '2')
                  {
                      //output_high(PIN_A1);
                      if(cbuff[i+1]=='1')
                      {
                          pwm2=50;
                          contador2=50;
                          i++;
                          continue;
                      }
                      else if(cbuff[i+1]=='2')
                      {
                          pwm2=100;
                          contador2=100;
                          i++;
                          continue;
                      }
                      else if(cbuff[i+1]=='3')
                      {
                          pwm2=150;
                          contador2=150;
                          i++;
                          continue;
                      }
                      else if(cbuff[i+1]=='4')
                      {
                          output_high(PIN_A1);
                          i++;
                          pwm2=0;
                          contador2=0;
                          continue;
                      }
                      else
                      {
                          output_low(PIN_A1);
                          i++;
                          pwm2=0;
                          contador2=0;
                          continue;
                      }
                  }
                  if(cbuff[i]== '3')
                  {
                      //output_high(PIN_A2);
                      if(cbuff[i+1]=='1')
                      {
                          pwm3=50;
                          contador3=50;
                          i++;
                          continue;
                      }
                      else if(cbuff[i+1]=='2')
                      {
                          pwm3=100;
                          contador3=100;
                          i++;
                          continue;
                      }
                      else if(cbuff[i+1]=='3')
                      {
                          pwm3=150;
                          contador3=150;
                          i++;
                          continue;
                      }
                      else if(cbuff[i+1]=='4')
                      {
                          output_high(PIN_A2);
                          i++;
                          pwm3=0;
                          contador3=0;
                          continue;
                      }
                      else
                      {
                          output_low(PIN_A2);
                          i++;
                          pwm3=0;
                          contador3=0;
                          continue;
                      }
                  }
              }
              if(indice==0) indice++;
              else indice=0;
              
          }*/
          
        for(int i; i<32; i+=2) // RGM: FIJARSE QUE TAN LARGO ES CBUFF, EN EL DE PCA ES 54, ACA 32
        {
            if(cbuff[i]!= 0X00 || cbuff[i+1]!= 0X00) // RGM: EL NUMERO DE CONDICIONALES DEPENDE DE QUE TAN LARGO SERA EL PAQUETE
            {
                paquetes++; 
            } 
        }
          
        int char_a_int;
        
        for(i=0; i<(paquetes*2); i+=2)
        {
            if(cbuff[i] == '1')
            {
                char_a_int = cbuff[i+1]-48;
                pwm1 = resultado_proceso(char_a_int);
                contador1=pwm1;
                if(pwm1==200)
                {
                    output_high(PIN_A0);
                    pwm1=0;
                    contador1=0;
                }
                if(pwm1 == 250)
                {
                    output_low(PIN_A0);
                    pwm1=0;
                    contador1=0;
                }
            }
            else if(cbuff[i] == '2')
            {
                char_a_int = cbuff[i+1]-48;
                pwm2 = resultado_proceso(char_a_int);
                contador2=pwm2;
                if(pwm2==200)
                {
                    output_high(PIN_A1);
                    pwm2=0;
                    contador2=0;
                }
                if(pwm2 == 250)
                {
                    output_low(PIN_A1);
                    pwm2=0;
                    contador2=0;
                }
            }
            else if(cbuff[i] == '3')
            {
                char_a_int = cbuff[i+1]-48;
                pwm3 = resultado_proceso(char_a_int);
                contador3=pwm3;
                if(pwm3==200)
                {
                    output_high(PIN_A2);
                    pwm3=0;
                    contador3=0;
                }
                if(pwm3 == 250)
                {
                    output_low(PIN_A2);
                    pwm3=0;
                    contador3=0;
                }
            }
        }
          
          inicbuff();
          flagcomando=0;
          paquetes = 0;
      }         // Si hay comando pendiente
                                                 // de procesar ... lo procesa.

   } 
   
}

int resultado_proceso(int valor)
{
    switch(valor)
    {
        case 1: return 50;
            break;
        case 2: return 100;
            break;
        case 3: return 150;
            break;
        case 4: return 200;
            break;
        case 5: return 250;
            break;
        default: 
            break;
    }
}