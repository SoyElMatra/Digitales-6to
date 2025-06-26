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

#define TRIG PIN_C0
#define ECHO PIN_C1

int16 duration, distance;


void sr04dist() {
    output_high(TRIG);
    delay_us(10);
    output_low(TRIG);
    while (!input(ECHO));
    set_timer1(0);
    while (input(ECHO));
    duration = get_timer1();
    distance = (duration / 58);
}

void main() {
    enable_interrupts(INT_TIMER1);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
    set_timer1(0);
    lcd_init();
    while (1) {
        sr04dist();
        printf(lcd_putc, "\fDistancia: %lu", distance);
        delay_ms(100);
    }
}