#include <16F873A.h>
#device adc=10
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock= 20000000)        
#use standard_io(a)
#define cont 124
#define T0 229
#define T1 15535

int contador1 = cont, contador2 = cont;
int1 flagrc0;
float h;
char K;

#INT_TIMER1

void isr_timer1(void) //Función
{
    output_high(PIN_C0);
    flagrc0 = 1;
    set_timer1(T1); //recarga del TMR1
    set_timer0(T0);
    enable_interrupts(INT_TIMER0);
}

#INT_TIMER0

void isr_timer0(void) {
    if (flagrc0 == 1) {
        contador1 = contador1 - 1;
        if (contador1 == 0) {
            output_low(PIN_C0);
            flagrc0 = 0;
            contador1 = contador2;

        }
    }
    if (flagrc0 == 0) {
        disable_interrupts(INT_TIMER0);
    }
    set_timer0(T0);
} //Se recarga el timer0

void main(){
     setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
    set_timer1(T1); //recarga del TMR1
    enable_interrupts(INT_TIMER1); //habilita interrupcion timer1
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_1); //Configuración timer0

    set_timer0(225); //Carga del timer0
    enable_interrupts(INT_TIMER0); //Habilita interrupción timer0
    enable_interrupts(GLOBAL); //Habilita interrupción general
    output_high(PIN_C0);
    flagrc0 = 1;
    while(1){
        
    }
}