#include <16F876A.h>
#FUSES XT,NOWDT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7)
#use I2C(MASTER, SDA=PIN_C4,SCL=PIN_C3,FAST=100000)
#use standard_io(A)
#use standard_io(B)

int i;

/***************************** Envío I2C **************************************/
void envio_I2C() {

    i2c_start(); //Comienzo de la comunicación I2C ...
    i2c_write(0x80); // dirección del PCA9685
    i2c_write(0x00); //dirección del registro MODE1
    i2c_write(0x30); //b'00110000' Autoincremento de registros y modo sleep para poder configurar la frecuencia
    i2c_stop();


    i2c_start(); //Comienzo de la comunicación I2C ...
    i2c_write(0x80); //dirección del PCA9685  
    i2c_write(0xFE); //DIRECCION DEL REGISTRO PREESCALE
    i2c_write(0x79); //Valor preescaler= 25.000000/(4096*50)-1=121=0x79
    i2c_stop(); //Finalización de la transmisión

    i2c_start(); //Comienzo de la comunicación I2C ...
    i2c_write(0x80); // dirección del PCA 
    i2c_write(0x06); //dirección del led 0
    i2c_write(0x00); //led0_on_L
    i2c_write(0x00); //led0_on_H
    i2c_write(0x66); //led0_off_L
    i2c_write(0x00); //led0_off_H
    i2c_stop(); //Finalización de la transmisión

    i2c_start(); //Comienzo de la comunicación I2C ...
    i2c_write(0x80); // dirección del PCA 
    i2c_write(0x0A); //dirección del led 1
    i2c_write(0x00); //led0_on_L
    i2c_write(0x00); //led0_on_H
    i2c_write(205); //led0_off_L 204 (204/4096)*20000=996useg
    i2c_write(0x00); //led0_off_H
    i2c_stop(); //Finalización de la transmisión

    i2c_start(); //Comienzo de la comunicación I2C ...
    i2c_write(0x80); // dirección del PCA 
    i2c_write(0x0E); //dirección del led 2
    i2c_write(0x00); //led0_on_L
    i2c_write(0x00); //led0_on_H
    i2c_write(0x9A); //led0_off_L Parte baja del nro 410
    i2c_write(0x01); //led0_off_H Parte alta del nro 410 t=410/4096*20000=2001useg
    i2c_stop(); //Finalización de la transmisión 

    i2c_start(); //Comienzo de la comunicación I2C ...
    i2c_write(0x80); // dirección del PCA 
    i2c_write(0x12); //dirección del led 3
    i2c_write(0x00); //led0_on_L
    i2c_write(0x00); //led0_on_H
    i2c_write(0x33); //led0_off_L
    i2c_write(0x01); //led0_off_H
    i2c_stop(); //Finalización de la transmisión
}
/******************************************************************************/
int const lenbuff = 54; // Longitud de buffer, Ajustar
// a lo que desees (o te sea posible)

// VARIABLES EN RAM ///////////////////////////////////////////////////////////

int xbuff = 0x00; // Índice: siguiente char en cbuff
char cbuff[lenbuff]; // Buffer
char rcvchar = 0x00; // último caracter recibido
int1 flagcomando = 0; // Flag para indicar comando disponible 
int1 flag;
// Declaración de Funciones ///////////////////////////////////////////////////

void inicbuff(void); // Borra buffer 
int addcbuff(char c);
void procesa_comando(void); // Procesa comando

void inicbuff(void) { // Inicia a  cbuff -------------------
    int i;

    for (i = 0; i < lenbuff; i++) { // Bucle que pone a 0 todos los
        cbuff[ i ] = 0x00; // caracteres en el buffer
    }
    xbuff = 0x00; // Inicializo el indice de siguiente
    // caracter
}

#int_rda

void serial_isr() { // Interrupción recepción serie USART

    rcvchar = 0x00; // Inicializo caracter recibido                    
    //if(kbhit()){                        // Si hay algo pendiente de recibir ...
    rcvchar = getc(); // lo descargo y ...
    if (rcvchar == '*' && flag == 0) {
        flag = 1;
        return;
    }
    if (flag == 1 && rcvchar != '*') {
        cbuff[xbuff++] = rcvchar; // Añade caracter recibido al Buffer
    }
    if (flag == 1 && rcvchar == '*') {
        flagcomando = 1;
        flag = 0;
    }// Añade caracter recibido al Buffer
}
//addcbuff(rcvchar);               // lo añado al buffer y ...
// echos(rcvchar);                  // hago eco (si procede).
// }

void main() {

    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);

    while (TRUE) {
        delay_ms(1000);
        envio_I2C();
        while (1) {
            if (flagcomando == 1) {
                i = 0;
                i2c_start(); //Comienzo de la comunicación I2C ...
                i2c_write(0x08); //...con la dirección del PIC esclavo... 
                i2c_write('*');
                while (cbuff[i] != 0 & i < 54) {
                    i2c_write(cbuff[i]);
                    i = i + 1;
                }
                i2c_write('*');
                i2c_stop();
                flagcomando = 0;
                inicbuff();
            }
        }
    }
}
