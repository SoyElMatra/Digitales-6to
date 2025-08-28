#include <16F873A.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)        
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8, parity=N)
#define LCD_DATA_PORTB
#define use_portb_kbd TRUE
#include <lcd.c>	
#include <kbd.c>	

#define dht22 PIN_A2
#bit dht_io = 0x85.2

byte dht22_dat[5];
short signo;
float dhthum, dthtemp;
int GlobalErr1, GlobalErr2, GlobalErr3;

int Tmin, Tmax;
int Hmin, Hmax;
char k;
int flag_init;

// ================== DHT22 ===================
byte leer_datos_dht() {
   byte i, result=0;
   for (i=0;i<8;i++) {
      while(input(dht22)==0);
      delay_us(30);
      if(input(dht22)==1) result |= (1<<(7-i));
      while(input(dht22)==1);
   }
   return result;
}

void leer_dht22(void) {
   byte i, dht22_in, dht22_checksum;
   int16 temperatura, humedad;
   float temp1, hum;

   dht_io=0; output_high(dht22); delay_us(20);
   output_low(dht22); delay_ms(18);
   output_high(dht22); delay_us(40);
   dht_io=1; delay_us(40);
   dht22_in=input(dht22);

   if(dht22_in){ GlobalErr1=1; return; }
   GlobalErr1=0; delay_us(60);
   dht22_in=input(dht22);
   if(!dht22_in){ GlobalErr2=1; return; }
   GlobalErr2=0; delay_us(80);

   for(i=0;i<5;i++){ dht22_dat[i]=leer_datos_dht(); }
   dht_io=0; delay_us(10); output_high(dht22);

   dht22_checksum=dht22_dat[0]+dht22_dat[1]+dht22_dat[2]+dht22_dat[3];
   if(dht22_dat[4]!=dht22_checksum){ GlobalErr3=1; return; }
   GlobalErr3=0;

   humedad=make16(dht22_dat[0],dht22_dat[1]);
   temperatura=make16(dht22_dat[2],dht22_dat[3]);
   if(bit_test(temperatura,15)){ signo=1; bit_clear(temperatura,15); }

   hum=humedad; temp1=temperatura;
   dhthum=hum/10; dthtemp=temp1/10;
}

// ================== CONFIGURACIÓN ===================
int leer_valor3d(int maxval, int tipo){
   int val=0;
   int d1,d2,d3;

   lcd_putc("\fIngrese\n");
   if(tipo==0) lcd_putc("Temp.Min (   )");
   if(tipo==1) lcd_putc("Temp.Max (   )");
   if(tipo==2) lcd_putc("Hum.Min (   )");
   if(tipo==3) lcd_putc("Hum.Max (   )");

   // centenas
   do{ k=kbd_getc(); }while(k==0);
   lcd_gotoxy(13,2); printf(lcd_putc,"%c",k);
   d1 = k - '0';
   val = d1*100;

   // decenas
   do{ k=kbd_getc(); }while(k==0);
   lcd_gotoxy(14,2); printf(lcd_putc,"%c",k);
   d2 = k - '0';
   val += d2*10;

   // unidades
   do{ k=kbd_getc(); }while(k==0);
   lcd_gotoxy(15,2); printf(lcd_putc,"%c",k);
   d3 = k - '0';
   val += d3;

   if(val>maxval){
      lcd_putc("\fValor invalido");
      delay_ms(1000);
      return -1; 
   }
   return val;
}

void configurar_temp(void){
   int t1,t2;
   do {
      t1 = leer_valor3d(125,0);
      t2 = leer_valor3d(125,1);
      if(t1<0 || t2<0 || t1>=t2){
         lcd_putc("\fError Tmin/Tmax");
         delay_ms(1500);
      }
   } while(t1<0 || t2<0 || t1>=t2);
   Tmin=t1; Tmax=t2;
   write_eeprom(0x01,Tmin);
   write_eeprom(0x02,Tmax);
}

void configurar_hum(void){
   int h1,h2;
   do {
      h1 = leer_valor3d(100,2);
      h2 = leer_valor3d(100,3);
      if(h1<0 || h2<0 || h1>=h2){
         lcd_putc("\fError Hmin/Hmax");
         delay_ms(1500);
      }
   } while(h1<0 || h2<0 || h1>=h2);
   Hmin=h1; Hmax=h2;
   write_eeprom(0x03,Hmin);
   write_eeprom(0x04,Hmax);
}

// ================== LECTOR DE TECLADO ===================
char leer_teclado(){
   char tecla;
   tecla = kbd_getc();
   if(tecla=='#'){ configurar_temp(); return '#'; }
   if(tecla=='*'){ configurar_hum(); return '*'; }
   return 0;
}

// ================== MAIN ===================
void main(){
   lcd_init();
   kbd_init();
   port_b_pullups(TRUE);

   output_low(PIN_A0); 
   output_low(PIN_A1); 

   printf(lcd_putc,"\fDHT22 + Teclado");
   delay_ms(1000);

   // Primer inicio
   flag_init=read_eeprom(0x00);
   if(flag_init!=1){
      configurar_temp();
      configurar_hum();
      write_eeprom(0x00,1);
   }

   Tmin=read_eeprom(0x01);
   Tmax=read_eeprom(0x02);
   Hmin=read_eeprom(0x03);
   Hmax=read_eeprom(0x04);

   while(TRUE){
      leer_dht22();
      while(GlobalErr1|GlobalErr2|GlobalErr3) leer_dht22();

      // Mostrar Temperatura 5s, pero chequeando teclado
      for(int i=0;i<50;i++){
         printf(lcd_putc,"\fT:%2.1fC",dthtemp);
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Min:%u Max:%u",Tmin,Tmax);

         if(dthtemp>Tmax){ output_high(PIN_A0); output_low(PIN_A1); }
         else if(dthtemp<Tmin){ output_high(PIN_A1); output_low(PIN_A0); }
         else { output_low(PIN_A0); output_low(PIN_A1); }

         if(leer_teclado()!=0) break; // <<< interrumpe al instante
         delay_ms(100);
      }

      // Mostrar Humedad 5s
      for(int i=0;i<50;i++){
         printf(lcd_putc,"\fH:%2.1f%%",dhthum);
         lcd_gotoxy(1,2);
         printf(lcd_putc,"Min:%u Max:%u",Hmin,Hmax);

         if(dhthum>Hmax){ output_high(PIN_A0); output_low(PIN_A1); }
         else if(dhthum<Hmin){ output_high(PIN_A1); output_low(PIN_A0); }
         else { output_low(PIN_A0); output_low(PIN_A1); }

         if(leer_teclado()!=0) break; // <<< interrumpe al instante
         delay_ms(100);
      }
   }
}
