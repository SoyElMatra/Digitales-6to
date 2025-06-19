#include<16f876a.h> 
#device adc=10
#fuses XT,NOWDT,NOPROTECT,NOLVP
#use delay(clock= 4000000)        
#use standard_io(a)

#include<lcd.c>

int16 q;
float p;

#int_TIMER0
void TIMER0_isr(void)
{
    delay_us(20);
    q = read_adc();
    p= q*500.0/1024.0;
    q= (int)p;
    printf(lcd_putc, "\fTemperatura:%3lu" ,q);
	set_timer0(0);
}

void main(void)
{
    SETUP_ADC_PORTS(AN0);
    SETUP_ADC(ADC_CLOCK_INTERNAL); 
    lcd_init();
    set_adc_channel(0);
    
    setup_timer_0(T0_INTERNAL|T0_DIV_256);
    set_timer0(0);
    
    enable_interrupts(INT_TIMER0);
    enable_interrupts(global);
    
    while(1)
    {

    }
}