/*
 Lo mismo que el anterior pero el rpm se envia por rs232
 */

#include <16F876A.h>
#fuses XT,NOWDT,NOPROTECT,NOLVP,NOBROWNOUT
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7)
#use standard_io(A)
#use standard_io(B)

#define LCD_DATA_PORTB
#include <lcd.c>

/******************************************************************************/

int const lenbuff=32;                  // Longitud de buffer, Ajustar
                                       // a lo que desees (o te sea posible)

int  xbuff=0x00;                       // Índice: siguiente char en cbuff
char cbuff[lenbuff];                   // Buffer
char rcvchar=0x00;                     // último caracter recibido
int1 flagcomando=0;                    // Flag para indicar comando disponible 
int1 flag;

/******************************************************************************/

int temp;

int contador=0;
int flag1=0; 
int16 pwm = 0;
int16 rpm_ideal;
int16 rpm_ideal_eprom;
int16 rpm;

void inicbuff(void);                   // Borra buffer 
int  addcbuff(char c);

void inicbuff(void){                   // Inicia a  cbuff -------------------
   int i;

   for(i=0;i<lenbuff;i++){             // Bucle que pone a 0 todos los
      cbuff[ i ]=0x00;                 // caracteres en el buffer
   }
   xbuff=0x00;                         // Inicializo el indice de siguiente
                                       // caracter
}

/******************************************************************************/
#int_rda
void serial_isr() 
{                                          // Interrupción recepción serie USART

    rcvchar=0x00;                          // Inicializo caracter recibido                    
    if(kbhit())
    {                                     // Si hay algo pendiente de recibir ...
       rcvchar=getc();                    // lo descargo y ...
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
       }                                // Añade caracter recibido al Buffer
     }

 }
/******************************************************************************/

#INT_TIMER1 
void TIMER1(void)
{
    contador = get_timer0();
    flag1=1;
    set_timer1(53036);
    set_timer0(0);
}

void main(void)
{
    setup_timer_2(T2_DIV_BY_16,255,1);
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
    setup_timer_0(T0_EXT_L_TO_H|T0_DIV_1);
    set_timer0(0);
    set_timer1(53036);
    setup_ccp1(CCP_PWM);
    enable_interrupts(INT_TIMER1);
    enable_interrupts(GLOBAL);
    
    inicbuff();
    enable_interrupts(int_rda);
    
    lcd_init();
    
    set_pwm1_duty(0);
   
    while(1)
    {
        if(flag1==1)
        {
            rpm = (600*contador) / 360;
            rpm_ideal_eprom = read_eeprom(0x00);
            if(rpm_ideal_eprom > rpm)
            {
                if(pwm<1023)
                {
                   pwm++; 
                }
                set_pwm1_duty(pwm);
            }
            if(rpm_ideal_eprom < rpm)
            {
                if(pwm>0)
                {
                   pwm--; 
                }
                set_pwm1_duty(pwm);
            }
            printf(lcd_putc, "\f");
            printf(lcd_putc, "RPM REAL = %lu\nRPM = %lu", rpm, rpm_ideal_eprom);
            flag1=0;
        }
        
        if(flagcomando==1)
        {
            int16 centenas = (cbuff[0]-48) * 100;
            int16 decenas = (cbuff[1]-48) * 10;
            int16 unidades = (cbuff[2]-48);
            
            int16 total = centenas + decenas + unidades;
            if(total>180) total = 180;
            write_eeprom(0x00, total);
            
            inicbuff();
            total=0;
            flagcomando=0;
        }
        
        
    }
}