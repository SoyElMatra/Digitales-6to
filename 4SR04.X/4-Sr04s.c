#include <16F876A.h>
#fuses XT,NOWDT,PUT,NOWRT,NOLVP
#use delay(clock= 4000000)
#use standard_io(B)

#include<lcd4201.c>

#define TRIG1 PIN_B0
#define TRIG2 PIN_B1
#define TRIG3 PIN_B2
#define TRIG4 PIN_B3

int16 duration, oldDistance1, oldDistance2, oldDistance3, oldDistance4, distance1, distance2, distance3, distance4;
int1 flag1 = 1,flag2 = 1,flag3 = 1,flag4 = 1, check = 1, checkdisp = 1;
int i = 0;
#INT_RB

RB_isr() {
    duration = get_timer1();
    switch (i) {
        case 0:
            distance1 = (duration / 58) - 5;
            if(distance1 != oldDistance1){
                check = 1;
                oldDistance1 = distance1
            }
            break;
        case 1:
            distance2 = (duration / 58) - 5;
            if(distance2 != oldDistance2){
                check = 1;
                oldDistance2 = distance2
            }
            break;
        case 2:
            distance3 = (duration / 58) - 5;
            if(distance3 != oldDistance3){
                check = 1;
                oldDistance3 = distance3
            }
            break;
        case 3:
            distance4 = (duration / 58) - 5;
            if(distance4 != oldDistance4){
                check = 1;
                oldDistance4 = distance4
            }
            break;
    }
}

void sr04dist1() {
    i = 0;
    output_high(TRIG1);
    delay_us(10);
    output_low(TRIG1);
    set_timer1(0);
}

void sr04dist2() {
    i = 1;
    output_high(TRIG2);
    delay_us(10);
    output_low(TRIG2);
    set_timer1(0);
}

void sr04dist3() {
    i = 2;
    output_high(TRIG3);
    delay_us(10);
    output_low(TRIG3);
    set_timer1(0);
}

void sr04dist4() {
    i = 3;
    output_high(TRIG4);
    delay_us(10);
    output_low(TRIG4);
    set_timer1(0);
}

void main() {
    set_tris_B(0xF0);
    output_low(PIN_B1);
    enable_interrupts(GLOBAL);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
    delay_ms(500);
    clear_interrupt(INT_RB);
    enable_interrupts(INT_RB);
    
    lcd_init();
    while (1) {
        if(flag1 == 1 || distance1 > 25289 || distance2 > 25289 || distance3 > 25289 || distance4 > 25289){
            
        }
        if(check == 1){
            printf(lcd_putc, "Sensor1: %lu\nSensor2: %lu\nSensor3: %lu\nSensor4: %lu\n", distance1, distance2, distance3, distance4);
            check = 0;
        }
    }
}
