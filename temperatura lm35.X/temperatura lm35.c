#include<16f876a.h> 
#device adc=10
#fuses XT,NOWDT,NOPROTECT,NOLVP
#use delay(clock= 4000000)        
#use standard_io(a)
#include<lcd.c>	//rutina del lcd

 
 int16 q;
 float p;


void main() {
	SETUP_ADC_PORTS(AN0);              //Canal 0 analógico
    SETUP_ADC(ADC_CLOCK_INTERNAL);    //Fuente de reloj RC
	lcd_init();
   set_adc_channel(0);           //Habilitación canal 0 
    while(1){
		delay_us(20);
        q = read_adc(); 
        p= q*500.0/1024.0;
        q= (int)p;
		printf(lcd_putc, "\fTemperatura:%3lu" ,q);
		delay_ms(500);
}
}