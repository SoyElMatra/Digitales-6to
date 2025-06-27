#include<16F876.h>
#fuses XT,NOWDT,NOLVP
#use delay(clock=4M)

#use standard_io(C)
#use standard_io(B) 
#use standard_io(A)

#define LCD_ENABLE_PIN  PIN_B3
#define LCD_RS_PIN      PIN_B1
#define LCD_RW_PIN      PIN_B2
#define LCD_DATA4       PIN_B4
#define LCD_DATA5       PIN_B5
#define LCD_DATA6       PIN_B6
#define LCD_DATA7       PIN_B7
#include<lcd.c>

#define TRIG PIN_A0

int16 duration, oldDuration, distance;
int1 flag = 1, check = 1;

#INT_EXT

void isr_ext() {
    duration = get_timer1();
    if (oldDuration != duration) {
        check = 1;
        oldDuration = duration;
    }
    distance = (duration / 58) - 5;
    flag = 1;
}

void sr04dist() {
    output_high(TRIG);
    delay_us(10);
    output_low(TRIG);
    set_timer1(0);
}

void main() {
    enable_interrupts(GLOBAL);
    enable_interrupts(INT_EXT_H2L);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
    lcd_init();
    while (1) {
        if (flag == 1 || get_timer1() > 25289) {
            flag = 0;
            sr04dist();
        }
        if (check == 1) {
            check = 0;
            printf(lcd_putc, "\fDistancia: %lu", distance);
        }
    }
}