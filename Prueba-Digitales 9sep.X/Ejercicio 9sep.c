#include <18F2550.h>
#fuses XT,NOWDT,NOPROTECT,NOLVP,CPUDIV1
#use delay(clock= 4000000) 
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8, parity=N)
#define LCD_DATA_PORTB
#define use_portb_kbd TRUE
#include <lcd.c>	//rutina del lcd
#include <kbd.c>	//rutina del teclado


#define dht22 PIN_A2
#bit dht_io = 0x92.2


byte dht22_dat[5];
int i = 0, valor1;
short signo;
float dhthum, dthtemp, temp1, humid;
int GlobalErr1, GlobalErr2, GlobalErr3;
int temp, tmin, tmax;
int hum, hmin, hmax;
char K;
byte leer_datos_dht();

void cambiar_temp(void) {
    disable_interrupts(INT_TIMER0);

    do {
        for (i = 0; i < 4; i++) {
            if (i == 0) {
                lcd_putc("\fIngrese\n");
                printf(lcd_putc, "Temper.Min ");
            } else if (i == 1) {
                lcd_putc("\fIngrese\n");
                printf(lcd_putc, "Temper.Max ");
                temp = temp + (k - 48);
                tmin = temp;
            } else if (i == 2) {
                lcd_putc("\fIngrese\n");
                printf(lcd_putc, "Hum.Min ");
                temp = temp + (k - 48);
                tmax = temp;
            } else {
                lcd_putc("\fIngrese\n");
                printf(lcd_putc, "Hum.Max ");
                temp = temp + (k - 48);
                hmin = temp;
            }
            k = kbd_getc();
            while (K != '1' & k != '0') {
                k = kbd_getc();
            }

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
                while (k != '0') {
                    k = kbd_getc();
                }
            }
            printf(lcd_putc, "\%c", k);
            temp = temp + (k - 48)*10;
            delay_ms(400);
            k = kbd_getc();

            if (temp == 100) {
                while (k != '0') {
                    k = kbd_getc();
                }
            }
            if (temp < 100) {
                while (k == 0 | k == '*' | k == '#') {
                    k = kbd_getc();
                }
            }
            printf(lcd_putc, "\%c", k);

            delay_ms(500);
        }
        temp = temp + (k - 48);
        hmax = temp;
        if (tmin >= tmax || hmin >= hmax) {
            printf(lcd_putc, "\fError: Valores\nErroneos");
            delay_ms(2000);
        }

    } while (tmin >= tmax || hmin >= hmax);
    write_eeprom(0x01, tmin);
    write_eeprom(0x02, tmax);
    write_eeprom(0x03, hmin);
    write_eeprom(0x04, hmax);
    printf(lcd_putc, "\nTL:%2u", tmin);
    printf(lcd_putc, "\TH:%2u", temp);
    write_eeprom(0x00, 1);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(global); //Habilita interrupción general
    return;
}

byte leer_datos_dht() {
    byte i = 0;
    byte result = 0;
    for (i = 0; i < 8; i++) {
        while (input(dht22) == 0);
        delay_us(30);
        if (input(dht22) == 1) {
            result |= (1 << (7 - i));
        }
        while (input(dht22) == 1);
    }

    return result;
}

Void leer_dht22(void) {
    byte dht22_in, i, dht22_checksum;
    int16 temperatura, humedad;
    float temp1, hum;

    dht_io = 0;
    output_high(dht22);
    delay_us(20);
    output_low(dht22);
    delay_ms(18);

    output_high(dht22);
    delay_us(30);
    output_low(dht22);
    dht_io = 1; // configurar el pin como entrada
    delay_us(40);
    dht22_in = input(dht22);

    if (dht22_in) {
        GlobalErr1 = 1;
        // printf(lcd_putc,"\nSTART1 INCORRECTO");
        return;
    }
    GlobalErr1 = 0;
    delay_us(80);
    dht22_in = input(dht22);

    if (!dht22_in) {
        GlobalErr2 = 1;
        //printf(lcd_putc,"\nSTART2 INCORRECTO");
        return;
    }
    GlobalErr2 = 0;
    delay_us(35);

    for (i = 0; i < 5; i++) {
        dht22_dat[ i ] = leer_datos_dht(); // capturando datos
    }

    dht_io = 0;
    delay_us(10);
    output_high(dht22);

    dht22_checksum = dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3];

    if (dht22_dat[4] != dht22_checksum) {
        GlobalErr3 = 1;
        // printf("\r\nDHT checksum error");
        //printf(lcd_putc,"checksum error");
    }
    GlobalErr3 = 0;
    humedad = make16(dht22_dat[0], dht22_dat[1]);
    temperatura = make16(dht22_dat[2], dht22_dat[3]);

    if (bit_test(temperatura, 15) == 1) {
        signo = 1;
        BIT_CLEAR(temperatura, 15);
    }
    hum = humedad;
    temp1 = temperatura;
    dhthum = (hum) / 10;
    dthtemp = (temp1) / 10;
}

int contador;
#int_TIMER0

void TIMER0_isr(void) {
    contador = contador - 1;
    if (contador == 0) {
        leer_dht22();
        if (GlobalErr1 == 1 | GlobalErr2 == 1 | GlobalErr3 == 1) {
            printf(lcd_putc, "\fERROR DE LECTURA");
            set_timer0(0);
            contador = 10;
            return;
        }
        printf(lcd_putc, "\fH=%2.1f%%", dhthum);
        if (signo == 1) {
            printf(lcd_putc, "\ T=-%2.1f%cC", dthtemp, 158);
        }
        if (signo == 0) {
            printf(lcd_putc, "\ T=%2.1f%cC", dthtemp, 158);
        }
        if (hum == 0) {
            printf(lcd_putc, "\nTMIN:%2u", tmin);
            printf(lcd_putc, " TMAX:%2u", tmax);
            hum = 1;
        } else {
            printf(lcd_putc, "\nHMIN:%2u", hmin);
            printf(lcd_putc, " HMAX:%2u", hmax);
            hum = 0;
        }
        if (dthtemp < tmin) {
            output_high(PIN_C0);
            output_low(PIN_C1);
        }
        if (dthtemp >= tmin & dthtemp <= tmax) {
            output_low(PIN_C0);
            output_low(PIN_C1);
        }
        if (dthtemp > tmax) {
            output_low(PIN_C0);
            output_high(PIN_C1);
        }
        signo = 0;
        contador = 30;
    }
    set_timer0(0);
} //Se recarga el timer0

void main() {
    lcd_init();
    kbd_init();
    //PORT_B_PULLUPS(TRUE); 
    printf(lcd_putc, "\fSensor DHT22");
    delay_ms(500);
    setup_timer_0(T0_INTERNAL | T0_DIV_1); //Configuración timer
    contador = 30;
    enable_interrupts(INT_TIMER0); //Habilita interrupción timer0
    enable_interrupts(GLOBAL); //Habilita interrupción genera 
    set_timer0(0);
    valor1 = read_eeprom(0x00);
    if (valor1 != 0x01) {
        cambiar_temp();
    } else {
        temp = read_eeprom(0x01);
        tmin = temp;
        temp = read_eeprom(0x02);
        tmax = temp;
        temp = read_eeprom(0x03);
        hmin = temp;
        temp = read_eeprom(0x04);
        hmax = temp;
    }
    while (1) {

        k = kbd_getc();
        if (k == '#') {
            cambiar_temp();
        }
    }
}
