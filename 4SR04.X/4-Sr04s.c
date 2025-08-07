#include <16F876A.h>
#fuses XT,NOWDT,PUT,NOWRT,NOLVP
#use delay(clock= 4000000)
#use standard_io(B)
#use standard_io(C)

#define LCD_DATA_PORT getenv("SFR:PORTC")
#include<lcd.c>

#define TRIG1 PIN_B0
#define TRIG2 PIN_B1
#define TRIG3 PIN_B2
#define TRIG4 PIN_B3

int16 duration, oldDistance1, oldDistance2, oldDistance3, oldDistance4, distance1, distance2, distance3, distance4;
int1 flagdisp = 1, check = 1;
int i = 0;

#INT_RB

RB_isr() {
    duration = get_timer1();
    disable_interrupts(INT_RB);
    check = 1;
    switch (i) {
        case 0:
            distance1 = (duration / 58) - 5;
            if (distance1 != oldDistance1) {
                flagdisp = 1;
                oldDistance1 = distance1;
            }
            break;
        case 1:
            distance2 = (duration / 58) - 5;
            if (distance2 != oldDistance2) {
                flagdisp = 1;
                oldDistance2 = distance2;
            }
            break;
        case 2:
            distance3 = (duration / 58) - 5;
            if (distance3 != oldDistance3) {
                flagdisp = 1;
                oldDistance3 = distance3;
            }
            break;
        case 3:
            distance4 = (duration / 58) - 5;
            if (distance4 != oldDistance4) {
                flagdisp = 1;
                oldDistance4 = distance4;
            }
            break;
    }
}

void sr04dist1() {
    i = 0;
    enable_interrupts(INT_RB);
    output_high(TRIG1);
    delay_us(10);
    output_low(TRIG1);
    set_timer1(0);
}

void sr04dist2() {
    i = 1;
    enable_interrupts(INT_RB);
    output_high(TRIG2);
    delay_us(10);
    output_low(TRIG2);
    set_timer1(0);
}

void sr04dist3() {
    i = 2;
    enable_interrupts(INT_RB);
    output_high(TRIG3);
    delay_us(10);
    output_low(TRIG3);
    set_timer1(0);
}

void sr04dist4() {
    i = 3;
    enable_interrupts(INT_RB);
    output_high(TRIG4);
    delay_us(10);
    output_low(TRIG4);
    set_timer1(0);
}

void main() {
    enable_interrupts(GLOBAL);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
    clear_interrupt(INT_RB);
    enable_interrupts(INT_RB);

    lcd_init();
    while (1) {
        delay_ms(10);
        if (check == 1 || duration >= 25289) {
            check = 0;
            i++;
            if(i >= 4){
                i = 0;
            }
            switch(i){
                case 0:
                    sr04dist2();
                    break;
                case 1: 
                    sr04dist3();
                    break;
                case 2: 
                    sr04dist4();
                    break;
                case 3: 
                    sr04dist1();
                    break;
            }
        }
        if (flagdisp == 1) {
            flagdisp = 0;
            printf(lcd_putc, "\fSR1: %lu SR2: %lu\nSR3: %lu SR4: %lu\n", distance1, distance2, distance3, distance4);
        }
    }
}

