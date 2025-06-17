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

int16 rpmcont, rpmDeseada = 30, i;
int16 timcont, cuenta = 0;
int1 check = 1;
char c;

void cambiarDeseada() {
    printf(lcd_putc, "\fIngrese\nRPM(   )");
    lcd_gotoxy(5, 2);
    for (i = 0; i < 3; i++) {
        c = kbd_getc();
        while (c == "\0") {
            c = kbd_getc();
        }
        switch (i) {
            case 0:
                rpmDeseada = (c - 48) + 100;
                break;
            case 1:
                rpmDeseada += (c - 48) + 10;
                break;
            case 2: rpmDeseada += (c - 48);
        }
    }
    write_eeprom(0x00, rpmDeseada);
}

#INT_TIMER1

void isr_timer1() {
    rpmcont = ((timcont * 600.0) / 180) / 2.0;
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
    port_b_pullups(TRUE);

    setup_timer_2(T2_DIV_BY_16, 255, 1);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_4);
    setup_timer_0(T0_EXT_L_TO_H | T0_DIV_1);

    enable_interrupts(INT_TIMER0);
    enable_interrupts(INT_TIMER1);
    enable_interrupts(GLOBAL);

    set_timer1(15535);
    set_timer0(255);

    kbd_init();
    lcd_init();

    /*rpmDeseada = read_eeprom(0x00);
    if (rpmDeseada == 0xFF) {
        cambiarDeseada();
    }*/

    setup_ccp1(CCP_PWM);
    set_pwm1_duty(cuenta);
    while (1) {
        if (check == 1) {
            check = 0;
            printf(lcd_putc, "\fRevs: %lu\nDeseada: %lu: ", rpmcont, rpmDeseada);
        }
        if (rpmcont < rpmDeseada && cuenta < 1023) {
            cuenta++;
        } else if (rpmcont > rpmDeseada && cuenta > 0) {
            cuenta--;
        }
        set_pwm1_duty(cuenta);
        c = kbd_getc();
        while(c == "\0" && rpmcont == rpmDeseada){
            c = kbd_getc();
        }
        if (c == "#") {
            cambiarDeseada();
        }
        delay_ms(50);
    }
}