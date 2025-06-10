#include <16F876A.h>
#FUSES XT,NOWDT,NOLVP
#use delay(clock=4000000)
#use standard_io(B)
#use standard_io(A)
#define LCD_DATA_PORTB
#define use_portb_kbd TRUE
#include <lcd.c>
#include <kbd.c>

int timcont, rpmcont;


#INT_TIMER1
void isr_timer1(){
    rpmcont = ((timcont * 2) / 360) * 600; 
    set_timer0(0);
    set_timer1(15535);
}

void main(){
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_4);
    setup_timer_0(T0_EXT_H_TO_L | T0_DIV_2);
    enable_interrupts(INT_TIMER1);
    enable_interrupts(GLOBAL);
    set_timer1(15535);
    lcd_init();
    kbd_init();
    while(1){
        printf(lcd_putc,"Revoluciones: \n%u", rpmcont);
        timcont = get_timer0;
        delay_ms(100);
    }
}