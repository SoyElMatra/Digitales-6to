//funciona ok

#include <16F873A.h>
#fuses XT,NOWDT,NOPROTECT,NOLVP
#use delay(clock= 4000000) 
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8, parity=N)
#include <lcd.c>	//rutina del lcd


#define dht22 PIN_A2
#bit dht_io = 0x85.2


byte dht22_dat[5];
short signo;
float dhthum, dthtemp, temp1, humid;
int GlobalErr1, GlobalErr2, GlobalErr3;
byte leer_datos_dht();

byte leer_datos_dht() {
    byte i = 0;
    byte result = 0;
    for (i = 0; i < 8; i++) {
        //We enter this during the first start bit (low for 50uS) of the byte
        //Next: wait until pin goes high
        while (input(dht22) == 0);
        delay_us(30);
        if (input(dht22) == 1) {//Was: if(PINC & _BV(dht_PIN))
            result |= (1 << (7 - i));
        }
        while (input(dht22) == 1);
        //Was: while((PINC & _BV(dht_PIN)));
    }//end of "for.."

    return result;
}

Void leer_dht22(void) {
    //byte GlobalErr=0;
    byte dht22_in, i, dht22_checksum;
    int16 temperatura, humedad;
    float temp1, hum;

    dht_io = 0; // configurar el pin como salida
    output_high(dht22);
    delay_us(20);
    output_low(dht22);
    delay_ms(18);

    output_high(dht22);
    delay_us(40);
    dht_io = 1; // configurar el pin como entrada
    delay_us(40);
    dht22_in = input(dht22);

    if (dht22_in) {
        GlobalErr1 = 1;
        // printf(lcd_putc,"\nSTART1 INCORRECT");
        return;
    }
    GlobalErr1 = 0;
    delay_us(60);
    dht22_in = input(dht22);

    if (!dht22_in) {
        GlobalErr2 = 1;
        //printf(lcd_putc,"\nSTART2 INCORRECT");
        return;
    }
    GlobalErr2 = 0;
    delay_us(80);

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

    if (bit_test(temperatura, 15)) {
        signo = 1;
        BIT_CLEAR(temperatura, 15);
    }


    hum = humedad;
    temp1 = temperatura;

    dhthum = (hum) / 10;
    dthtemp = (temp1) / 10;
}

void main() {


    lcd_init();
    printf(lcd_putc, "\fSensor DHT22");
    delay_ms(500);

    //dht_init(); 
    // delay_ms(300);


    while (true) {

        leer_dht22();
        delay_ms(10);

        while (GlobalErr1 == 1 | GlobalErr2 == 1 | GlobalErr3 == 1) {
            leer_dht22();
            //delay_ms(1000);	
        }

        // printf(lcd_putc,"\fHumedad=%f %%",dhthum);
        printf(lcd_putc, "\fH=%2.1f%%", dhthum);
        printf("\H=%2.1f%%", dhthum);
        if (signo == 1) {

            //printf(lcd_putc,"\nTemp.=-%f %cC", dthtemp,158);
            printf(lcd_putc, "\ T=-%2.1f%cC", dthtemp, 158);
            printf("\T=-%2.1fc", dthtemp);
        }
        if (signo == 0) {
            //printf(lcd_putc,"\nTemp.=%f %cC", dthtemp,158);
            printf(lcd_putc, "\ T=%2.1f%cC", dthtemp, 158);
            printf("\T=%2.1fc", dthtemp);
        }
        signo = 0;
        delay_ms(2000);
    }
}


