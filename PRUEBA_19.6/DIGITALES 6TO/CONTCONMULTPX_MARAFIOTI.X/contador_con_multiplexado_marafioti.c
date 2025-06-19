#include <16f877a.h>
#fuses XT, NOWDT, NOLVP
#use delay(clock=4000000)
#use standard_io(a)
#use standard_io(b)

int8 num=00,minutos=00,horas=00, cont=0;

int8 multiplex=0;

int8 display[10] = {    0b00111111, //0
                        0b00000110, //1
                        0b01011011, //2
                        0b01001111, //3
                        0b01100110, //4
                        0b01101101, //5
                        0b01111101, //6
                        0b00000111, //7
                        0b01111111, //8
                        0b01100111  //9
                   };

#INT_TIMER1
void isr_int_tmr1(void)
{
    if(cont==0)
    {
        cont++;
    }
    else if(cont==1)
    {
        num++;
        cont=0;
    }
    
    if(num>=60)
    {
        num=0;
        minutos++;
    }
    if(minutos>=60)
    {
        minutos=0;
        horas++;
    }
    if(horas>=24)
    {
        horas=0;
    }
    set_timer1(3036);
}

#INT_TIMER0
void isr_int_tmr0(void)
{
    if(multiplex==0)
    {
        output_high(PIN_C5);
        output_b(display[horas/10]);
        output_low(PIN_C0);
        
        multiplex++;
    }
    else if(multiplex==1)
    {
        output_high(PIN_C0);
        output_b(display[(horas)%10]);
        output_high(PIN_B7);
        output_low(PIN_C1);
        
        multiplex++;
    }
    else if(multiplex==2)
    {
        output_high(PIN_C1);
        output_b(display[(minutos)/10]);
        output_low(PIN_C2);
        
        multiplex++;
    }
    else if(multiplex==3)
    {
        output_high(PIN_C2);
        output_b(display[(minutos)%10]);
        output_high(PIN_B7);
        output_low(PIN_C3);
        
        multiplex++;
    }
    else if(multiplex==4)
    {
        output_high(PIN_C3);
        output_b(display[(num)/10]);
        output_low(PIN_C4);
        
        multiplex++;
    }
    else if(multiplex==5)
    {
        output_high(PIN_C4);
        output_b(display[(num)%10]);
        output_high(PIN_B7);
        output_low(PIN_C5);
        
        multiplex=0;
    }
    set_timer0(178);
}

void main(void)
{
    int toggle=0;
    
    enable_interrupts(GLOBAL);
    enable_interrupts(INT_TIMER1);
    enable_interrupts(INT_TIMER0);
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
    set_timer1(3036);
    
    setup_timer_0(T0_INTERNAL|T0_DIV_64);
    set_timer0(178);
    
    while(1)
    {
        if(input(PIN_A0)==0)
        {
            while(input(PIN_A0)==0);
            if(toggle==0) 
            {
                disable_interrupts(INT_TIMER1);
                toggle=1;
            }
            else
            {
                enable_interrupts(INT_TIMER1);
                toggle=0;
            }    
        }
    }
}