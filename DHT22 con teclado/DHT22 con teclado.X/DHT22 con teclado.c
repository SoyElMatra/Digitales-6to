//funciona ok

#include <16F876A.h>
#fuses XT,NOWDT,NOPROTECT,NOLVP
#use delay(clock= 4000000) 
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8, parity=N)
#define LCD_DATA_PORTB
#define use_portb_kbd TRUE
#include <lcd.c>	//rutina del lcd
#include <kbd.c>	//rutina del teclado


#define dht22 PIN_A2
#bit dht_io = 0x85.2


byte dht22_dat[5];
short signo;
float dhthum, dthtemp, temp1, humid, tmin, tmax, tminold, tmaxold;
int GlobalErr1, GlobalErr2, GlobalErr3, k;
float humin, humax;
byte leer_datos_dht();
int1 neg = 0;

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

void cambiar_temp() {
    disable_interrupts(GLOBAL);
    tminold = tmin;
    tmaxold = tmax;
    
    //======TEMP MAX======\\

    lcd_putc("Ingrese:(* = -)\n");
    printf(lcd_putc, "Temper.Min (   )");
    k = kbd_getc(); //devuelve la tecla pulsada si no se apreta ninguna tecla devuelve el 0 en decimal
    while (K != '*' & k != '0') {
        k = kbd_getc();
    }

    if (k != '*') {
        lcd_gotoxy(13, 2);
        printf(lcd_putc, "\%c", k);
        neg = 0;
    } else {
        lcd_gotoxy(13, 2);
        printf(lcd_putc, "\%c", '-');
        tmin = (k - 48)*100;
        neg = 1;
    }

    k = kbd_getc(); //devuelve la tecla pulsada si no se apreta ninguna tecla devuelve el 0 en decimal
    while (k != '*' & k > (8 + 48) | (k > 4 + 48 & temp < 0)) {
        k = kbd_getc();
    }

    lcd_gotoxy(13, 2);
    printf(lcd_putc, "\%c", k);
    if (neg == 1) {
        tmin = -(k - 48)*10;
    } else {
        tmin = (k - 48)*10;
    }

    k = kbd_getc(); //devuelve la tecla pulsada si no se apreta ninguna tecla devuelve el 0 en decimal
    while (k == 0) {
        k = kbd_getc();
    }

    lcd_gotoxy(13, 2);
    printf(lcd_putc, "\%c", k);
    if (neg == 1) {
        tmin = -(k - 48);
    } else {
        tmin = (k - 48);
    }
    delay_ms(500);
    
    //======TEMP MAX======\\

    lcd_putc("Ingrese:(* = -)\n");
    printf(lcd_putc, "Temper.Max (   )");
    k = kbd_getc(); //devuelve la tecla pulsada si no se apreta ninguna tecla devuelve el 0 en decimal
    while (K != '*' & k != '0') {
        k = kbd_getc();
    }

    if (k != '*') {
        lcd_gotoxy(13, 2);
        printf(lcd_putc, "\%c", k);
        neg = 0;
    } else {
        lcd_gotoxy(13, 2);
        printf(lcd_putc, "\%c", '-');
        tmax = (k - 48)*100;
        neg = 1;
    }

    k = kbd_getc(); //devuelve la tecla pulsada si no se apreta ninguna tecla devuelve el 0 en decimal
    while (k != '*' & k > (8 + 48) | (k > 4 + 48 & neg = 1)) {
        k = kbd_getc();
    }

    lcd_gotoxy(13, 2);
    printf(lcd_putc, "\%c", k);
    if (neg == 1) {
        tmax = -(k - 48)*10;
    } else {
        tmax = (k - 48)*10;
    }

    k = kbd_getc(); //devuelve la tecla pulsada si no se apreta ninguna tecla devuelve el 0 en decimal
    while (k == 0) {
        k = kbd_getc();
    }

    lcd_gotoxy(13, 2);
    printf(lcd_putc, "\%c", k);
    if (neg == 1) {
        tmax = -(k - 48);
    } else {
        tmax = (k - 48);
    }
    delay_ms(500);
    
    if(tmin >= tmax){
        lcd_putc("ERROR:\nValores erroneos");
        tmin = tminold;
        tmax = tmaxold;
    }else{
        
    }
}

void cambiar_hum() {

}

int contador;
#int_TIMER0

void TIMER0_isr(void) {
    contador = contador - 1;
    if (contador == 0) {
        leer_dht22();
        if (GlobalErr1 == 1 | GlobalErr2 == 1 | GlobalErr3 == 1) {
            printf(lcd_putc, "\fERROR DE LECTURA");
            return;
        }
        printf(lcd_putc, "\fH=%2.1f%%", dhthum);
        if (signo == 1) {
            printf(lcd_putc, "\ T=-%2.1f%cC", dthtemp, 158);
        }
        if (signo == 0) {
            printf(lcd_putc, "\ T=%2.1f%cC", dthtemp, 158);
        }
        signo = 0;
        contador = 30;
    }
    set_timer0(0);
} //Se recarga el timer0

void main() {


    lcd_init();
    kbd_init();
    PORT_B_PULLUPS(TRUE);
    printf(lcd_putc, "\fSensor DHT22");
    delay_ms(500);
    setup_timer_0(T0_INTERNAL | T0_DIV_256); //Configuración timer
    contador = 30;
    enable_interrupts(INT_TIMER0); //Habilita interrupción timer0
    enable_interrupts(GLOBAL); //Habilita interrupción genera 



    while (1) {

        k = kbd_getc();
        if (k == '#') {
            output_toggle(PIN_C0);
        }
    }
}