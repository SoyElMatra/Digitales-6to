#include <16F873A.h>
#device adc=10
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock= 20000000)        
#use standard_io(a)
#define LCD_DATA_PORTB
#define use_portb_kbd TRUE
#include <lcd.c>	//rutina del lcd
#include <kbd.c>	//rutina del teclado

int temp, tmin, tmax, diff, diffmax, valor1, contador1 = 97, contador2 = 97;
int16 qold, q;
int1 flagrc0;
float h;
char K;

void cambiar_temp(void) {
    disable_interrupts(INT_TIMER0);
    lcd_putc("\fIngrese\n");
    printf(lcd_putc, "Temper.Min (   )");
    k = kbd_getc(); //devuelve la tecla pulsada si no se apreta ninguna tecla devuelve el 0 en decimal
    while (K != '1' & k != '0') {
        k = kbd_getc();
    }

    lcd_gotoxy(13, 2);
    printf(lcd_putc, "\%c", k);
    temp = (k - 48)*100;
    delay_ms(400);
    k = kbd_getc();

    if (temp < 100) {
        while (k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    if (temp == 100) {
        while (k > 53 | k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(14, 2);
    printf(lcd_putc, "\%c", k);
    temp = temp + (k - 48)*10;
    delay_ms(400);
    k = kbd_getc();

    if (temp == 150) {
        while (k != '0') {
            k = kbd_getc();
        }
    }
    if (temp < 150) {
        while (k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(15, 2);
    printf(lcd_putc, "\%c", k);
    temp = temp + (k - 48);
    tmin = temp;
    //write_eeprom(0x01,temp);
    delay_ms(800);

    lcd_putc("\fIngrese\n");
    printf(lcd_putc, "Temper.Max (   )");
    k = kbd_getc(); //devuelve la tecla pulsada si no se apreta ninguna tecla devuelve el 0 en decimal

    while (K != '1' & k != '0') {
        k = kbd_getc();
    }

    lcd_gotoxy(13, 2);
    printf(lcd_putc, "\%c", k);
    temp = (k - 48)*100;
    delay_ms(400);
    k = kbd_getc();

    if (temp < 100) {
        while (k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    if (temp == 100) {
        while (k > 53 | k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(14, 2);
    printf(lcd_putc, "\%c", k);
    temp = temp + (k - 48)*10;
    delay_ms(400);
    k = kbd_getc();

    if (temp == 150) {
        while (k != '0') {
            k = kbd_getc();
        }
    }
    if (temp < 150) {
        while (k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(15, 2);
    printf(lcd_putc, "\%c", k);
    temp = temp + (k - 48);
    tmax = temp;
    //write_eeprom(0x02,temp);
    delay_ms(800);

    set_adc_channel(0); //Habilitación canal 0
    delay_us(20);
    q = read_adc();
    h = q * 250.0 / 1024.0;
    q = (int) h;
    printf(lcd_putc, "\fTemperatura:%3lu", q);
    valor1 = read_eeprom(0x00);
    if (tmin >= tmax && valor1 != 1) {
        lcd_putc("\fValores\nerroneos");
        delay_ms(2000);
    }
    if (tmin < tmax) {
        if (valor1 != 1) {
            write_eeprom(0x00, 1);
            valor1 = 1;
        }
        write_eeprom(0x01, tmin);
        write_eeprom(0x02, tmax);
        printf(lcd_putc, "\nTmin:%2u", tmin);
        printf(lcd_putc, "\Tmax:%2u", temp);
    }
    if (tmin >= tmax && valor1 == 1) {
        temp = read_eeprom(0x01);
        tmin = temp;
        printf(lcd_putc, "\nTmin:%2u", temp);
        temp = read_eeprom(0x02);
        tmax = temp;
        printf(lcd_putc, "\Tmax:%2u", temp);
    }
    diffmax = tmax - tmin;
    enable_interrupts(INT_TIMER0);
    enable_interrupts(global); //Habilita interrupción general
    return;
}

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
    kbd_init();
    port_b_pullups(TRUE);

    valor1 = read_eeprom(0x00);
    while (valor1 != 1) {
        cambiar_temp();
    }

    set_adc_channel(0); //Habilitación canal 0
    delay_us(200);
    q = read_adc();
    h = q * 250.0 / 1024.0;
    q = (int) h;
    qold = q;
    printf(lcd_putc, "\fTemperatura:%3lu%c", q, 223);
    temp = read_eeprom(0x01);
    tmin = temp;
    printf(lcd_putc, "\nTmin:%3u", temp);
    temp = read_eeprom(0x02);
    tmax = temp;
    lcd_gotoxy(9, 2);
    printf(lcd_putc, "\Tmax:%3u", temp);

    setup_timer_1(T1_INTERNAL | T1_DIV_BY_2);
    set_timer1(15540); //recarga del TMR1
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_1); //Configuración timer0
    set_timer0(230); //Carga del timer0

    enable_interrupts(INT_TIMER0); //Habilita interrupción timer0
    enable_interrupts(INT_TIMER1); //habilita interrupcion timer1
    enable_interrupts(GLOBAL); //Habilita interrupción general
    output_high(PIN_C0);
    flagrc0 = 1;
    diffmax = tmax - tmin;

    while (true) {
        k = kbd_getc();
        if (k == '#') {
            cambiar_temp();
        }
        q = read_adc();
        h = q * 250.0 / 1024.0;
        q = (int) h;
        if (qold != q) {
            qold = q;
            lcd_gotoxy(13, 1);
            printf(lcd_putc, "\%3lu", q);
        }
        if (q > tmin & q < tmax) {
            diff = q - tmin;
            if (contador2 <= 97) {
                contador2 = (int) (47.0 + (diff * (50.0 / diffmax)));
            }
        }
        if (q < tmin) {
            contador2 = 47;
        }
        if (q > tmax) {
            contador2 = 97;
        }
        delay_ms(10);
    }
}