#include <16F876A.h>
#FUSES XT,NOWDT,NOLVP
#use delay(clock=4000000)
#use standard_io(C)
#use standard_io(B)
#use standard_io(A)
#define LCD_DATA_PORTB
#define use_portb_kbd TRUE
#include <lcd.c>
#include <kbd.c>

int16 rpmcont;
int16 timcont;
int1 check = 1;

#INT_TIMER1

void isr_timer1() {
    rpmcont = ((timcont * 600.0) / 360) / 2.0;
    timcont = 0;
    check = 1;
    set_timer1(15535);
}

#INT_TIMER0

void isr_timer0() {
    timcont++;
    set_timer0(255);
}

void main() {
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_4);
    setup_timer_0(T0_EXT_L_TO_H | T0_DIV_1);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(INT_TIMER1);
    enable_interrupts(GLOBAL);
    set_timer1(15535);
    set_timer0(255);
    lcd_init();
    kbd_init();
    while (1) {
        if (check == 1) {
            check = 0;
            printf(lcd_putc, "\fRevoluciones: %lu", rpmcont);
        }
    }
}