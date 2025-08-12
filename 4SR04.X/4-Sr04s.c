#include <16F876A.h>
#fuses XT,NOWDT,NOPUT,NOWRT,NOLVP
#use delay(clock=4000000)
#use standard_io(B)
#use standard_io(C)

#define LCD_ENABLE_PIN  PIN_C0
#define LCD_RS_PIN      PIN_C1
#define LCD_RW_PIN      PIN_C2
#define LCD_DATA4       PIN_C4
#define LCD_DATA5       PIN_C5
#define LCD_DATA6       PIN_C6
#define LCD_DATA7       PIN_C7
#include <lcd.c>

#define TRIG1 PIN_B0
#define TRIG2 PIN_B1
#define TRIG3 PIN_B2
#define TRIG4 PIN_B3

#define ECHO1 4
#define ECHO2 5
#define ECHO3 6
#define ECHO4 7

int16 duration, oldDistance1, oldDistance2, oldDistance3, oldDistance4;
int16 distance1, distance2, distance3, distance4;
int1 flagdisp = 1, check = 1;
int i = 0;
int1 esperandoBajada = 0;
int8 prevb;

#INT_RB
void RB_isr() {
    int8 portb = input_b();
    int8 changed = portb ^ prevb;
    prevb = portb;

    int8 bitECHO;
    switch(i) {
        case 0: bitECHO = ECHO1; break;
        case 1: bitECHO = ECHO2; break;
        case 2: bitECHO = ECHO3; break;
        case 3: bitECHO = ECHO4; break;
    }

    if(changed & (1 << bitECHO)) {
        if(portb & (1 << bitECHO)) {
            set_timer1(0);
            esperandoBajada = 1;
        } else if(esperandoBajada) {
            duration = get_timer1();
            esperandoBajada = 0;
            disable_interrupts(INT_RB);
            check = 1;
            switch(i) {
                case 0:
                    distance1 = (duration / 58) - 5;
                    if(distance1 != oldDistance1) { flagdisp = 1; oldDistance1 = distance1; }
                    break;
                case 1:
                    distance2 = (duration / 58) - 5;
                    if(distance2 != oldDistance2) { flagdisp = 1; oldDistance2 = distance2; }
                    break;
                case 2:
                    distance3 = (duration / 58) - 5;
                    if(distance3 != oldDistance3) { flagdisp = 1; oldDistance3 = distance3; }
                    break;
                case 3:
                    distance4 = (duration / 58) - 5;
                    if(distance4 != oldDistance4) { flagdisp = 1; oldDistance4 = distance4; }
                    break;
            }
        }
    }
}

void sr04dist1() {
    i = 0;
    prevb = input_b();
    output_high(TRIG1); delay_us(10); output_low(TRIG1);
    set_timer1(0);
    enable_interrupts(INT_RB);
}

void sr04dist2() {
    i = 1;
    prevb = input_b();
    output_high(TRIG2); delay_us(10); output_low(TRIG2);
    set_timer1(0);
    enable_interrupts(INT_RB);
}

void sr04dist3() {
    i = 2;
    prevb = input_b();
    output_high(TRIG3); delay_us(10); output_low(TRIG3);
    set_timer1(0);
    enable_interrupts(INT_RB);
}

void sr04dist4() {
    i = 3;
    prevb = input_b();
    output_high(TRIG4); delay_us(10); output_low(TRIG4);
    set_timer1(0);
    enable_interrupts(INT_RB);
}

void main() {
    output_low(TRIG1);
    output_low(TRIG2);
    output_low(TRIG3);
    output_low(TRIG4);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
    enable_interrupts(GLOBAL);
    prevb = input_b();

    lcd_init();
    while(TRUE) {
        if(check == 1) {
            check = 0;
            switch(i) {
                case 0: sr04dist2(); break;
                case 1: sr04dist3(); break;
                case 2: sr04dist4(); break;
                case 3: sr04dist1(); break;
            }
        }
        if(flagdisp == 1) {
            flagdisp = 0;
            printf(lcd_putc, "\fSR1:%lu SR2:%lu\nSR3:%lu SR4:%lu", distance1, distance2, distance3, distance4);
        }
        delay_ms(50);
    }
}
