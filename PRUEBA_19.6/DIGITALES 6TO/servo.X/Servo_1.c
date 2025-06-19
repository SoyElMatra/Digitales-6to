#include <16f873A.h>
#fuses HS,nowdt,noprotect,nolvp
#use delay(clock=20000000)
#use standard_io(b) 

int contador=131, contador1=131, contador2=25, contador3=25, contador4=100, contador5=100; 
int1 flagrb0, flagrb3, flagrc2;

#int_TIMER0
void TIMER0_isr(void) 
{
    if(flagrb0==1)
    {    
        contador=contador-1;
        if(contador==0) 
        {
            output_low(PIN_B0);
            flagrb0=0;
            contador=contador1;
        }
    }
    if(flagrb3==1)
    {    
        contador2=contador2-1;
        if(contador2==0) 
        {
            output_low(PIN_B3);
            flagrb3=0;
            contador2=contador3;
        }
    }
    if(flagrc2==1)
    {    
        contador4=contador4-1;
        if(contador4==0) 
        {
            output_low(PIN_C2);
            flagrc2=0;
            contador4=contador5;
        }
    }
    if(flagrb0==0 && flagrb3==0 && flagrc2==0)
    {
        disable_interrupts(INT_TIMER0);
    }    
    set_timer0(225); 
} // Se recarga el timer0

#int_TIMER1 // Interrupción Timer1
void temp1s(void) // Función
{ 
    output_high(PIN_B0); 
    output_high(PIN_B3);
    output_high(PIN_C2);
    flagrb0=1;
    flagrb3=1;
    flagrc2=1;
    set_timer1(15536); // recarga del TMR1
    set_timer0(225);
    enable_interrupts(INT_TIMER0); 
}

main() 
{
    // port_b_pullups(TRUE); 
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
    set_timer1(15536); // recarga del TMR1
    enable_interrupts(INT_TIMER1); // habilita interrupcion timer1
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1); // Configuración timer0

    set_timer0(225); // Carga del timer0
    enable_interrupts(INT_TIMER0); // Habilita interrupción timer0
    enable_interrupts(global); // Habilita interrupción general

    // contador=25;
    // contador2=25;
    output_high(PIN_B0);
    output_high(PIN_B3);
    flagrb0=1;
    flagrb3=1;
    // port_b_pullups(TRUE); // Pull-up para RB1

    while(1) 
    {
        if(input(PIN_B1)==0) 
        {
            delay_ms(10);
            if(input(PIN_B1)==0) 
            {
                while(input(PIN_B1)==0);    
                if(contador1<131)
                    contador1=contador1+1;
            }
        }

        if(input(PIN_B2)==0) 
        {
            delay_ms(10);
            if(input(PIN_B2)==0) 
            {
                while(!input(PIN_B2));    
                if(contador1>25)
                    contador1=contador1-1;
            }
        }

        if(input(PIN_B4)==0) 
        {
            delay_ms(10);
            if(input(PIN_B4)==0) 
            {
                while(!input(PIN_B4));    
                if(contador3<131)
                    contador3=contador3+1;
            }
        }

        if(input(PIN_B5)==0) 
        {
            delay_ms(10);
            if(input(PIN_B5)==0) 
            {
                while(!input(PIN_B5));    
                if(contador3>25)
                    contador3=contador3-1;
            }
        }
        
        if(input(PIN_C0)==0) 
        {
            delay_ms(10);
            if(input(PIN_C0)==0) 
            {
                while(input(PIN_C0)==0);    
                if(contador5<131)
                {
                    contador5=contador5+1;
                }    
            }
        }
        if(input(PIN_C1)==0) 
        {
            delay_ms(10);
            if(input(PIN_C1)==0) 
            {
                while(input(PIN_C1)==0);    
                if(contador5<131)
                {
                    contador5=contador5-1;
                }    
            }
        }
        
    }    
}
