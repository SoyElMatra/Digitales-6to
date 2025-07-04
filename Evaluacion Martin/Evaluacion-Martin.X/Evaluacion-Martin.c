#include <16F876A.h>
#fuses XT,NOWDT,NOPROTECT,NOLVP,NOBROWNOUT
#use delay(clock=4000000)
#use I2C(MASTER, SDA=PIN_C4, SCL=PIN_C3, FAST=100000)
#use standard_io(A)
#use standard_io(B)

#define LCD_DATA_PORTB
#define use_portb_kbd TRUE

#include <lcd.c>
#include <kbd.c>



char k;
int temp;

int contador = 0;
int1 flag = 0;
int16 rpm_ideal;
int16 rpm_ideal_eprom;
int16 rpm;
int16 off_funcion = 4;

#INT_TIMER1 

void TIMER1(void) {
    contador = get_timer0();
    flag = 1;
    set_timer1(23500);
    set_timer0(0);
}

void envio_de_pwm(int canal, int16 off) {
    int16 adress = 0x06 + 4 * canal;
    int16 off_alto = make8(off, 1);
    int16 off_bajo = make8(off, 0);

    i2c_start(); //Comienzo de la comunicación I2C ...
    i2c_write(0x80); // dirección del PCA9685
    i2c_write(0x00); //dirección del registro MODE1
    i2c_write(0x30); //b'00110000' Autoincremento de registros y modo sleep para poder configurar la frecuencia
    i2c_stop();


    i2c_start(); //Comienzo de la comunicación I2C ...
    i2c_write(0x80); //dirección del PCA9685  
    i2c_write(0xFE); //DIRECCION DEL REGISTRO PREESCALE
    i2c_write(0x17); //Valor preescaler= 25.000.000/(4096*250)-1=23=0x17
    i2c_stop(); //Finalización de la transmisión

    i2c_start();
    i2c_write(0x80);
    i2c_write(adress);
    i2c_write(0x00);
    i2c_write(0x00);
    i2c_write(off_bajo);
    i2c_write(off_alto);
    i2c_stop();
}

void cambiar_rpm(void) {

    disable_interrupts(INT_TIMER1);
    lcd_putc("\fIngrese\n");
    printf(lcd_putc, "RPM (   )");
    k = kbd_getc();
    while (K != '1' && k != '0') {
        k = kbd_getc();
    }
    lcd_gotoxy(6, 2);
    printf(lcd_putc, "%c", k);
    temp = (k - 48) * 100;
    delay_ms(400);
    k = kbd_getc();

    if (temp < 100) {
        while (k == 0 || k == '*' || k == '#') {
            k = kbd_getc();
        }
    }
    if (temp == 100) {
        while (k > 56 || k == 0 || k == '*' || k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(7, 2);
    printf(lcd_putc, "%c", k);
    temp += (k - 48) * 10;
    delay_ms(400);
    k = kbd_getc();

    if (temp == 180) {
        while (k != '0') {
            k = kbd_getc();
        }
    }
    if (temp < 180) {
        while (k == 0 || k == '*' || k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(8, 2);
    printf(lcd_putc, "%c", k);
    temp += (k - 48);
    rpm_ideal = temp;
    write_eeprom(0x00, rpm_ideal);
    rpm_ideal_eprom = read_eeprom(0x00);
    delay_ms(800);
    printf(lcd_putc, "\f");
    enable_interrupts(INT_TIMER1);
}

void main(void) {
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_4);
    setup_timer_0(T0_EXT_L_TO_H | T0_DIV_1);
    set_timer0(0);
    set_timer1(23500);
    enable_interrupts(INT_TIMER1);
    enable_interrupts(GLOBAL);

    lcd_init();
    kbd_init();

    PORT_B_pullups(TRUE);

    rpm_ideal_eprom = read_eeprom(0x00);
    if (rpm_ideal_eprom == 0xFF) {
        cambiar_rpm();
    }

    while (1) {
        if (flag == 1) {
            rpm = contador;
            printf(lcd_putc, "\f");
            printf(lcd_putc, "RPM REAL = %lu\nRPM = %lu", rpm, rpm_ideal_eprom);
            flag = 0;
        }

        if (rpm_ideal_eprom > rpm && off_funcion <= 4095) {
            if ((rpm_ideal_eprom - rpm) > 20 && off_funcion <= 3095) {
                off_funcion += 10;
            } else if ((rpm_ideal_eprom - rpm) > 5 && off_funcion <= 4085) {
                off_funcion += 10;
            } else {
                off_funcion++;
            }
        } else

               if (rpm_ideal_eprom < rpm && off_funcion >= 0) {
                if ((rpm - rpm_ideal_eprom) > 20 && off_funcion > 100) {
                    off_funcion -= 100;
                } else if ((rpm - rpm_ideal_eprom) > 5 && off_funcion > 10) {
                    off_funcion -= 10;
                } else {
                    off_funcion--;
                }
            }
        envio_de_pwm(0, off_funcion);

        k = kbd_getc();
        if (k == '#') {
            cambiar_rpm();
        }

    }
}
