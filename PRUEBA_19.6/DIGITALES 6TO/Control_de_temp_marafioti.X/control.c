#include <16F876a.h>
#device adc=10
#fuses XT,NOWDT,NOPROTECT,NOLVP
#use delay(clock=4000000)        
#use standard_io(a)

#define LCD_DATA_PORTB
#define use_portb_kbd TRUE

#include <lcd.c>    // rutina del lcd
#include <kbd.c>    // rutina del teclado

int valor1;
char K;
int temp, tmin, tmax;
int16 q;
int16 cont;
float h;
int1 bandera=0;


void cambiar_temp(void) {
    disable_interrupts(INT_TIMER0);
    write_eeprom(0x00,1);
    lcd_putc("\fIngrese\n");
    printf(lcd_putc,"Temper.Min (   )");
    k = kbd_getc();
    while(K != '1' & k != '0') {
        k = kbd_getc();
    }
    lcd_gotoxy(13,2);
    printf(lcd_putc,"%c", k);
    temp = (k - 48) * 100;
    delay_ms(400);
    k = kbd_getc();

    if(temp < 100) {
        while(k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    if(temp == 100) {
        while(k > 53 | k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(14,2);
    printf(lcd_putc,"%c", k);
    temp += (k - 48) * 10;
    delay_ms(400);
    k = kbd_getc();

    if(temp == 150) {
        while(k != '0') {
            k = kbd_getc();
        }
    }
    if(temp < 150) {
        while(k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(15,2);
    printf(lcd_putc,"%c", k);
    temp += (k - 48);
    tmin = temp;
    delay_ms(800);

    lcd_putc("\fIngrese\n");
    printf(lcd_putc,"Temper.Max (   )");
    k = kbd_getc();
    while(K != '1' & k != '0') {
        k = kbd_getc();
    }
    lcd_gotoxy(13,2);
    printf(lcd_putc,"%c", k);
    temp = (k - 48) * 100;
    delay_ms(400);
    k = kbd_getc();

    if(temp < 100) {
        while(k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    if(temp == 100) {
        while(k > 53 | k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(14,2);
    printf(lcd_putc,"%c", k);
    temp += (k - 48) * 10;
    delay_ms(400);
    k = kbd_getc();

    if(temp == 150) {
        while(k != '0') {
            k = kbd_getc();
        }
    }
    if(temp < 150) {
        while(k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(15,2);
    printf(lcd_putc,"%c", k);
    temp += (k - 48);
    tmax = temp;
    delay_ms(800);

    set_adc_channel(0);
    delay_us(20);
    q = read_adc(); 
    h = q * 250.0 / 1024.0;
    q = (int)h;
    printf(lcd_putc, "\fTemperatura:%3lu", q);
    if(tmin >= tmax) {
        /*
        temp = read_eeprom(0x01);
        tmin = temp;
        printf(lcd_putc, "\nTmin:%2u", temp);
        temp = read_eeprom(0x02);
        tmax = temp;
        printf(lcd_putc,"\Tmax:%2u", temp);
         */
        if(bandera==0)
        {
            printf(lcd_putc, "\fError al ingresar\ntemperatura");
            write_eeprom(0x05,1);
            delay_ms(1000);
            break;
        }
        else
        {
            temp=read_eeprom(0x01);
            tmin=temp;
            printf(lcd_putc, "\nTmin:%2u",temp);
            temp=read_eeprom(0x02);
            tmax=temp;
            printf(lcd_putc,"\Tmax:%2u",temp);
        }

    }
    if(tmin < tmax) {
        write_eeprom(0x01, tmin);
        write_eeprom(0x02, tmax);
        printf(lcd_putc, "\nTmin:%2u", tmin);
        printf(lcd_putc,"\Tmax:%2u", tmax);
        write_eeprom(0x05,0);
        bandera=1;
        
    }
    enable_interrupts(INT_TIMER0);
    enable_interrupts(global);
    return;
}

int contador;
#int_TIMER0
void TIMER0_isr(void) {
    contador -= 1;
    if(contador == 0) {  
        q = read_adc();
        h = q * 250.0 / 1024.0;
        q = (int)h;
        lcd_gotoxy(13,1);
        printf(lcd_putc,"%3lu", q);
        contador = 7;
    }
    set_timer0(0);
}

void main() {
    setup_adc_ports(AN0_VREF_VREF);
    setup_adc(ADC_CLOCK_INTERNAL);
    lcd_init();
    kbd_init();
    port_b_pullups(TRUE);
    setup_timer_0(T0_INTERNAL|T0_DIV_256);

    contador = 7;
    write_eeprom(0x00, 2);
    valor1 = read_eeprom(0x00);
    if(valor1 != 0x01) {
        cambiar_temp();
    }

    set_adc_channel(0);
    delay_us(200);
    q = read_adc();
    h = q * 250.0 / 1024.0;
    q = (int)h;
    printf(lcd_putc, "\fTemperatura:%3lu%c", q, 223);
    temp = read_eeprom(0x01);
    tmin = temp;
    printf(lcd_putc, "\nTmin:%3u", temp);
    temp = read_eeprom(0x02);
    tmax = temp;
    lcd_gotoxy(9,2);
    printf(lcd_putc,"\Tmax:%3u", temp);

    set_timer0(0);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(global);

    while(true) {
        if(k == '#') {
            cambiar_temp();
        }
        if(q < tmin) {
            output_high(PIN_A1);
            output_low(PIN_A5);
        }
        if(q >= tmin & q <= tmax) {
            output_low(PIN_A1);
            output_low(PIN_A5);
        }
        if(q > tmax) {
            output_high(PIN_A5);
            output_low(PIN_A1);
        }
        k = kbd_getc();
        if(read_eeprom(0x05)!=0)
        {
            cambiar_temp();
        }
    }
}
