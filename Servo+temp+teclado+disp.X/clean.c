#include <16F876a.h>
#device adc=10
#fuses XT,NOWDT,NOPROTECT,NOLVP
#use delay(clock= 4000000)        
#use standard_io(a)
#define LCD_DATA_PORTB
#define use_portb_kbd TRUE
#include <lcd.c>	//rutina del lcd
#include <kbd.c>	//rutina del teclado

void main(){
    for(int i = 0; i < 20; i++){
        write_eeprom(i, 0xFF);
    }
}