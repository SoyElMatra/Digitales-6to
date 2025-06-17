#include <16F876A.h>
#FUSES XT,NOWDT,NOLVP
#use delay(clock=4000000)
#use standard_io(C)
#use standard_io(B)
#use standard_io(A)
#define LCD_DATA_PORTB
#define use_portb_kbd TRUE

void main(){
    write_eeprom(0x00, 0xFF);
}