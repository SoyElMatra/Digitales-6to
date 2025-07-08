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

#define PCA9685_ADDR 0x80  // Dirección I2C 

int16 contador=0, ancho=0;
int valor, rpm;
int1 flagmedida=0;
char k;

// --- Funciones para escribir en el PCA9685 ---
void pca9685_write_byte(int8 reg, int8 data) {
   i2c_start();
   i2c_write(PCA9685_ADDR);
   i2c_write(reg);
   i2c_write(data);
   i2c_stop();
}

void pca9685_set_pwm(int8 channel, int16 on, int16 off) {
   int8 base_reg = 0x06 + 4 * channel;
   i2c_start();
   i2c_write(PCA9685_ADDR);
   i2c_write(base_reg);
   i2c_write(on & 0xFF);
   i2c_write(on >> 8);
   i2c_write(off & 0xFF);
   i2c_write(off >> 8);
   i2c_stop();
}

void pca9685_init() {
   pca9685_write_byte(0x00, 0x30);//b'00110000' Autoincremento de registros y modo sleep para poder configurar la frecuencia
   delay_ms(10);
   pca9685_write_byte(0xFE, 0x17);   // Prescaler para 50Hz approx 25.000000/(4096*200)-1=29=0x17
   delay_ms(10);
   pca9685_write_byte(0x00, 0xA1);   // Auto-incremento activado
}

void cambiar_veloc (void){
    disable_interrupts(INT_TIMER1);
      pca9685_set_pwm(0, 0, 0);   // Canal 0:  
  lcd_putc("\fIngrese\n");
		  printf(lcd_putc,"Valor.RPM (   )");
           k=kbd_getc(); 				      //devuelve la tecla pulsada si no se apreta ninguna tecla devuelve el 0 en decimal
          while(K!='1'&k!='0')
		{
			k=kbd_getc();
		}
			
		    lcd_gotoxy(12,2);
            printf(lcd_putc,"%c",k);
            rpm=(k-48)*100;
            delay_ms(400);
		    k=kbd_getc();
            if(rpm<100)
			{
           while(k==0|k=='*'|k=='#')
		{
			k=kbd_getc();
		}
			}
			if(rpm==100)
			{
			while(k>'8'|k==0|k=='*'|k=='#')
		{
			k=kbd_getc();
		}
			}		
			lcd_gotoxy(13,2);
            printf(lcd_putc,"%c",k);
			rpm=rpm+(k-48)*10;
		    delay_ms(400);
            k=kbd_getc();
            if(rpm==180)
			{
 while(k!='0')
		{
			k=kbd_getc();
		}
			}
			if(rpm<180)
			{
 while(k==0|k=='*'|k=='#')
		{
			k=kbd_getc();
		}
			}
			lcd_gotoxy(14,2);
            printf(lcd_putc,"%c",k);
			rpm=rpm+(k-48);
			 write_eeprom(0x01,rpm);	
             delay_ms(500);
             enable_interrupts(INT_TIMER1);
}


//*************** INT TIMER1*********************
#int_TIMER1
void TIMER1_isr(void) {
contador=get_timer0();         //Lectura contador TMR0
contador=(contador*1200)/360;     //Conversión a rpm
  printf(lcd_putc,"\f%5lu rpm\n",contador);
  printf(lcd_putc,"%3u rpm",rpm);
  //lcd_gotoxy(1,1);
  flagmedida=1;
set_timer0(0);                      //Reinicia cuenta
set_timer1(59286);                // Recarga a 0.05s 
 }




void main() {
   delay_ms(100);      // Espera inicial
   lcd_init();
   kbd_init();
    PORT_B_pullups(TRUE); 
   setup_timer_0(RTCC_EXT_L_TO_H|RTCC_DIV_1);   //Configuración TMR0
     setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);     //Configuración TMR1
   set_timer0(0);                               //Borrado contador
   set_timer1(59286);                         //Carga a 0.05s
   pca9685_init();     // Inicializar PCA9685
     pca9685_set_pwm(0, 0, ancho);   // Canal 0: PWM Duty 0
     enable_interrupts(int_timer1);
   enable_interrupts(global);           //Habilitación interrupci
   valor=read_eeprom(0x00);
   if(valor!=0x03)
   {
       write_eeprom(0x00,3);
       rpm=180;
       write_eeprom(0x01,rpm);
   }
     
   while(TRUE) {
      if(k=='*'){
         cambiar_veloc();
     }
     if(flagmedida==1){
         flagmedida=0;
     rpm=read_eeprom(0x01);
   if(contador<rpm&&ancho<4095)  {
       ancho=ancho+1;
       pca9685_set_pwm(0, 0, ancho);   // Canal 0:  
   }
   if(contador>rpm&&ancho>0){
       ancho=ancho-1;
       pca9685_set_pwm(0, 0, ancho);   // Canal 0:
   }
     }
     k=kbd_getc();
 }
}
