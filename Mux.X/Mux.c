#include <16F877.h>
#fuses XT,NOWDT,NOPROTECT,NOLVP
#use delay(clock= 4000000)     

int disp[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111};
int hor = 23, min = 59, sec = 55, count = 0, hormod = 0, minmod = 0, secmod = 0, select = 1, valor;
int16 fetta;
int1 chusco = 0;

#INT_EXT

void isr_ext() {
    if (chusco == 0) {
        chusco = 1;
    } else {
        chusco = 0;
    }
}

#INT_TIMER0

void isr_Timer0() {
    if (chusco == 0) {
        if (!input_state(PIN_C0)) {
            output_high(PIN_C0);
            output_d(disp[(hor % 10)] + 128);
            output_low(PIN_C1);
        } else if (!input_state(PIN_C1)) {
            output_high(PIN_C1);
            output_d(disp[min / 10]);
            output_low(PIN_C2);
        } else if (!input_state(PIN_C2)) {
            output_high(PIN_C2);
            output_d(disp[min % 10] + 128);
            output_low(PIN_C3);
        } else if (!input_state(PIN_C3)) {
            output_high(PIN_C3);
            output_d(disp[sec / 10]);
            output_low(PIN_C4);
        } else if (!input_state(PIN_C4)) {
            output_high(PIN_C4);
            output_d(disp[sec % 10]);
            output_low(PIN_C5);
        } else if (!input_state(PIN_C5)) {
            output_high(PIN_C5);
            output_d(disp[hor / 10]);
            output_low(PIN_C0);
        }
    } else {
        if (!input_state(PIN_C0)) {
            output_high(PIN_C0);
            output_d(disp[(hormod % 10)] + 128);
            output_low(PIN_C1);
        } else if (!input_state(PIN_C1)) {
            output_high(PIN_C1);
            output_d(disp[minmod / 10]);
            output_low(PIN_C2);
        } else if (!input_state(PIN_C2)) {
            output_high(PIN_C2);
            output_d(disp[minmod % 10] + 128);
            output_low(PIN_C3);
        } else if (!input_state(PIN_C3)) {
            output_high(PIN_C3);
            output_d(disp[secmod / 10]);
            output_low(PIN_C4);
        } else if (!input_state(PIN_C4)) {
            output_high(PIN_C4);
            output_d(disp[secmod % 10]);
            output_low(PIN_C5);
        } else if (!input_state(PIN_C5)) {
            output_high(PIN_C5);
            output_d(disp[hormod / 10]);
            output_low(PIN_C0);
        }
    }
    set_timer0(99);
}

#INT_TIMER1

void isr_timer1() {
    if (chusco == 0) {
        if (count == 2) {
            count = 0;
            sec++;
            if (sec > 59) {
                sec = 0;
                min++;
            }
            if (min > 59) {
                min = 0;
                hor++;
            }
            if (hor > 23) {
                sec = 0;
                min = 0;
                hor = 0;
            }
        }
    } else {
        if (select == 1) {
            if (count == 2) {
                count = 0;
                disable_interrupts(INT_TIMER0);
                output_d(0);
            } else {
                enable_interrupts(INT_TIMER0);
            }
        }
        if (select == 2) {
            if (count == 2) {
                count = 0;
                disable_interrupts(INT_TIMER0);
                output_d(0);
            } else {
                enable_interrupts(INT_TIMER0);
            }
        }
        if (select == 3) {
            if (count == 2) {
                count = 0;
                disable_interrupts(INT_TIMER0);
                output_d(0);
            } else {
                enable_interrupts(INT_TIMER0);
            }
        }
    }
    count++;
    set_timer1(3035);
}

void main() {
    output_low(PIN_C0);
    output_high(PIN_C1);
    output_high(PIN_C2);
    output_high(PIN_C3);
    output_high(PIN_C4);
    output_high(PIN_C5);
    port_b_pullups(TRUE);
    enable_interrupts(GLOBAL);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(INT_EXT_H2L);
    setup_timer_0(T0_INTERNAL | T0_DIV_16);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
    set_timer0(99);
    set_timer1(65535);

    while (TRUE) {
        if (chusco == 0) {
            enable_interrupts(INT_TIMER1);
            while (chusco == 0);
        } else {
            hormod = hor;
            minmod = min;
            secmod = sec;
            fetta = get_timer1();
            disable_interrupts(INT_TIMER1);
            while (chusco == 1) {
                while (select == 1 && chusco == 1) {
                    if (!input(PIN_B1)) {
                        while (!input(PIN_B1));
                        select++;
                    }
                    if (!input(PIN_B5)) {
                        while (!input(PIN_B5));
                        secmod++;
                    }
                    if (!input(PIN_B2)) {
                        chusco = 0;
                        write_eeprom(0x00, hormod);
                        write_eeprom(0x01, minmod);
                        write_eeprom(0x02, secmod);
                        write_eeprom(0x03, 1);
                        set_timer1(fetta);
                    }
                }
                while (select == 2 && chusco == 1) {
                    if (!input(PIN_B1)) {
                        while (!input(PIN_B1));
                        select++;
                    }
                    if (!input(PIN_B4)) {
                        while (!input(PIN_B4));
                        minmod++;
                    }
                    if (!input(PIN_B2)) {
                        chusco = 0;
                        write_eeprom(0x00, hormod);
                        write_eeprom(0x01, minmod);
                        write_eeprom(0x02, secmod);
                        write_eeprom(0x03, 1);
                        set_timer1(fetta);
                    }
                }
                while (select == 3 && chusco == 1) {
                    if (!input(PIN_B1)) {
                        while (!input(PIN_B1));
                        select = 1;
                    }
                    if (!input(PIN_B3)) {
                        while (!input(PIN_B3));
                        hormod++;
                    }
                    if (!input(PIN_B2)) {
                        chusco = 0;
                        write_eeprom(0x00, hormod);
                        write_eeprom(0x01, minmod);
                        write_eeprom(0x02, secmod);
                        write_eeprom(0x03, 1);
                        set_timer1(fetta);
                    }
                }
            }
            valor = read_eeprom(0x03);
            if (valor != 1) {
                write_eeprom(0x03, 0xFF);
                hor = hormod;
                min = minmod;
                sec = secmod;
                set_timer1(3035);
            }
        }
    }
}
