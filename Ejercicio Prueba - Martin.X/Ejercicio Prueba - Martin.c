#include <16F876A.h>
#FUSES XT,NOWDT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7)
#use I2C(MASTER, SDA=PIN_C4,SCL=PIN_C3,FAST=1000000)
#define use_portb_kbd TRUE
#include <lcd.c>
#include <kbd.c>	//rutina del teclado
#use standard_io(A)
#use standard_io(B)

#define addr1 0x80
#define addr2 0x82

// --- Funciones para escribir en el PCA9685 ---

void pca9685_write_byte(int8 reg, int8 data, int8 addr) {
    i2c_start();
    i2c_write(addr);
    i2c_write(reg);
    i2c_write(data);
    i2c_stop();
}

void pca9685_set_pwm(int8 channel, int8 addr, int16 on, int16 off) {
    int8 base_reg = 0x06 + 4 * channel;
    i2c_start();
    i2c_write(addr);
    i2c_write(base_reg);
    i2c_write(on & 0xFF);
    i2c_write(on >> 8);
    i2c_write(off & 0xFF);
    i2c_write(off >> 8);
    i2c_stop();
}

void pca9685_init(int8 addr, int8 freq) {
    pca9685_write_byte(0x00, 0x30, addr); //b'00110000' Autoincremento de registros y modo sleep para poder configurar la frecuencia
    delay_ms(10);
    pca9685_write_byte(0xFE, freq, addr); // Prescaler para 250Hz approx 25.000.000/(4096*250)-1=29=0x17
    delay_ms(10);
    pca9685_write_byte(0x00, 0xA1, addr); // Auto-incremento activado
}

void main() {
    pca9685_init(addr1, 0x79);//aca mandamos la frecuencia de operacion de cada PCA
    pca9685_init(addr2, 0x3C);//y los inicializamos
                              //calculo para 10ms/100Hz: 25.000.000/(4096*100)-1=60=0x3C   

    pca9685_set_pwm(2, addr1, 0, 307);//1500          //el orden de los argumentos es: Canal, Adress,
    pca9685_set_pwm(4, addr1, 0, 511);//2500          //on (Siempre en 0 porque es el desfase) y
    pca9685_set_pwm(6, addr1, 0, 256);//1000          //off, el cual mide la duracion de la señal y
    pca9685_set_pwm(1, addr2, 0, 615);//1500          //se calcula con la siguiente cuenta:
    pca9685_set_pwm(3, addr2, 0, 1023);//2500          //(4095 * Tiempo de duracion deseado) / periodo de la frecuencia elegida
    pca9685_set_pwm(5, addr2, 0, 512);//1000


    while (1) {

    }
}