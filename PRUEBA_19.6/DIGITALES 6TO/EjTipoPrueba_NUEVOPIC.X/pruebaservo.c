#include <18f2550.h>
#device adc=10
#fuses HS,NOWDT,NOPROTECT,NOLVP,CPUDIV1
#use delay(clock=20M)        
#use standard_io(a)
#use standard_io(b)
#use standard_io(c)

#define REP 30

void main()
{
    int i;
    port_b_pullups(0XFF);
    while(1)
    {
        for(i=0;i<REP;i++)
        {
            output_high(PIN_C0);
            delay_ms(2);
            output_low(PIN_C0);
            delay_ms(18);
        }
       
        for(i=0;i<REP;i++)
        {
            output_high(PIN_C0);
            delay_us(500);
            output_low(PIN_C0);
            delay_us(19500);
        }
       
        
    }
}
