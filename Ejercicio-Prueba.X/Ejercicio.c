#include <16F876A.h>
#device adc=10
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock= 20000000)        
#use standard_io(a)
#define LCD_DATA_PORTB
#include <lcd.c>	//rutina del lcd


int tmin, tmax, diff, diffmax, contador1 = 47, contador2 = 47;
int16 qold, q;
int1 flagrc0;
float h;

#INT_TIMER1

void isr_timer1(void) //Función
{
    output_high(PIN_C0);
    flagrc0 = 1;
    set_timer1(15600); //recarga del TMR1
    set_timer0(230);
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
    set_timer0(230);
} //Se recarga el timer0

void main() {

    setup_adc_ports(AN0_VREF_VREF); //Canal 0 analógico
    setup_adc(ADC_CLOCK_INTERNAL); //Fuente de reloj RC
    lcd_init();
    set_adc_channel(0); //Habilitación canal 0
    delay_us(200);
    q = read_adc();
    h = q * 250.0 / 1024.0;
    q = (int) h;
    qold = q;
    tmin = 0;
    tmax = 100;
    printf(lcd_putc, "\fTemperatura:%3lu%c", q, 223);
    printf(lcd_putc, "\nTmin:%3u", tmin);
    lcd_gotoxy(9, 2);
    printf(lcd_putc, "\Tmax:%3u", tmax);

    setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
    set_timer1(15600); //recarga del TMR1
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_1); //Configuración timer0
    set_timer0(230); //Carga del timer0

    enable_interrupts(INT_TIMER0); //Habilita interrupción timer0
    enable_interrupts(INT_TIMER1); //habilita interrupcion timer1
    enable_interrupts(GLOBAL); //Habilita interrupción general
    output_high(PIN_C0);
    flagrc0 = 1;
    diffmax = tmax - tmin;

    while (true) {
        q = read_adc();
        h = q * 250.0 / 1024.0;
        q = (int) h;
        if (qold != q) {
            qold = q;
            lcd_gotoxy(13, 1);
            printf(lcd_putc, "\%3lu", q);
        }
        if (q > tmin && q < tmax) {
            diff = q - tmin;
            if (contador2 <= 98) {
                contador2 = (int) (48.0 + (diff * (50.0 / diffmax)));
            }
        }
        if (q == tmin) {
            contador2 = 48;
        }
        if (q == tmax) {
            contador2 = 98;
        }
        delay_ms(10);
    }
}