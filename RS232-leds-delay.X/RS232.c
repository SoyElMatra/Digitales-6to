#include <16F876A.h>
#FUSES XT,NOWDT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7)
#use standard_io(B)

int recvint[3] = {5, 5, 5}, i = 0, cont1 = 0, cont2 = 0, cont3 = 0;
int1 scan = 0;
char recv;

#INT_TIMER1
void isr_timer1() {
    if (recvint[0] < 4) {
        cont1--;
        if (cont1 <= 0) {
            output_toggle(PIN_B0);
            cont1 = recvint[0];
        }
    } else {
        if (recvint[0] == 4)
            output_high(PIN_B0);
        else
            output_low(PIN_B0);
    }

    if (recvint[1] < 4) {
        cont2--;
        if (cont2 <= 0) {
            output_toggle(PIN_B1);
            cont2 = recvint[1];
        }
    } else {
        if (recvint[1] == 4)
            output_high(PIN_B1);
        else
            output_low(PIN_B1);
    }

    if (recvint[2] < 4) {
        cont3--;
        if (cont3 <= 0) {
            output_toggle(PIN_B2);
            cont3 = recvint[2];
        }
    } else {
        if (recvint[2] == 4)
            output_high(PIN_B2);
        else
            output_low(PIN_B2);
    }

    set_timer1(3036);
}

#INT_RDA
void RDA_isr() {
    recv = getc();
    if (recv == '*') {
        scan = !scan;
        i = 0;
    } else if (scan) {
        if (i < 3) {
            recvint[i] = recv - 48;
            switch(i) {
                case 0: cont1 = recvint[i]; break;
                case 1: cont2 = recvint[i]; break;
                case 2: cont3 = recvint[i]; break;
            }
            i++;
        }
        if (i >= 3) {
            scan = 0;
            i = 0;
        }
    }
}

void main() {
    set_tris_b(0xF8);
    enable_interrupts(GLOBAL);
    enable_interrupts(INT_RDA);
    enable_interrupts(INT_TIMER1);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
    set_timer1(3036);

    while(TRUE) {
    }
}