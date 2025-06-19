#include <16F876.h>
#fuses XT,NOWDT,NOPROTECT,NOLVP
#use delay(clock= 4000000)        
#define use_portc_kbd TRUE
#include <kbd1.c>

#define LCD_ENABLE_PIN  PIN_B6      
#define LCD_RS_PIN      PIN_B5        
#define LCD_RW_PIN      PIN_B4                      
#define LCD_DATA4       PIN_B0    
#define LCD_DATA5       PIN_B1   
#define LCD_DATA6       PIN_B2                                  
#define LCD_DATA7       PIN_B3 

#include <lcd.c>

void main()
{
    char k;
    
    port_b_pullups(TRUE);
    lcd_init();
    kbd_init();
    printf(lcd_putc, "hola");
    while(1)
    {
        k=kbd_getc(); 	
        if(k!=0)
        {			
            lcd_putc('\f'); 
            lcd_putc(k);            
            delay_ms(50);
		}
    }
}