#include <16F887.h>
#device adc=10
#fuses HS,NOWDT,NOPROTECT,NOLVP,NOBROWNOUT
#use delay(clock=20000000)        
#use standard_io(a)
#use standard_io(b)
#use standard_io(c)
#use standard_io(d)

#define use_portb_kbd TRUE
#include <kbd.c>

#define LCD_ENABLE_PIN  PIN_B0    
#define LCD_RS_PIN      PIN_B1        
#define LCD_RW_PIN      PIN_B2                      
#define LCD_DATA4       PIN_B4    
#define LCD_DATA5       PIN_B5  
#define LCD_DATA6       PIN_B6                                  
#define LCD_DATA7       PIN_B7 
#include <lcd.c>

#define use_servo_1
#define servo_1 PIN_C0 


int16 q;
float p;
char k;
int i, contador=7, temp;

int tmax=80;
int tmin=40;

void mover_servo(int);
void cambiar_temp(void);

#INT_TIMER0 
void TIMER0(void)
{
    if(contador!=0)
    {
        contador--;
    }
    else
    {
        q = read_adc();
        p= q*500.0/1024.0;
        q= (int)p;
        printf(lcd_putc, "\fTemperatura:%3lu\nTmax:%d Tmin:%d" ,q, tmax, tmin);
        contador=7;
    }   
    set_timer0(0);
}

void main()
{
    lcd_init();
    kbd_init();
    
    setup_adc_ports(sAN0, VSS_VDD);
    setup_adc(ADC_CLOCK_INTERNAL);
    set_adc_channel(0);
    
    setup_timer_0(T0_INTERNAL|T0_DIV_256);
    set_timer0(0);
    
    enable_interrupts(INT_TIMER0);
    enable_interrupts(GLOBAL);
    
    while(1)
    {
        if(q>tmax)
        {
            mover_servo(1);
        }        
        else if(q<tmin)
        {
            mover_servo(0);
        }
    }
}

void mover_servo(int caso)
{
    if(caso==1)
    {
        for(i=0;i<100;i++)
        {
            output_high(PIN_C0);
            delay_ms(2);
            output_low(PIN_C0);
            delay_ms(18);
        }
    }
    else if(caso==0)
    {
        for(i=0;i<100;i++)
        {
            output_high(PIN_C0);
            delay_us(500);
            output_low(PIN_C0);
            delay_us(19500);
        } 
    }
    k=kbd_getc();
    if(k == '#') 
    {
        cambiar_temp();
    }
}

void cambiar_temp(void)
{
    disable_interrupts(INT_TIMER0);
    write_eeprom(0x00,1);
    lcd_putc("\fIngrese\n");
    printf(lcd_putc,"Temper.Min (   )");
    k = kbd_getc();
    while(K != '1' & k != '0') {
        k = kbd_getc();
    }
    lcd_gotoxy(13,2);
    printf(lcd_putc,"%c", k);
    temp = (k - 48) * 100;
    delay_ms(400);
    k = kbd_getc();

    if(temp < 100) {
        while(k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    if(temp == 100) {
        while(k > 53 | k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(14,2);
    printf(lcd_putc,"%c", k);
    temp += (k - 48) * 10;
    delay_ms(400);
    k = kbd_getc();

    if(temp == 150) {
        while(k != '0') {
            k = kbd_getc();
        }
    }
    if(temp < 150) {
        while(k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(15,2);
    printf(lcd_putc,"%c", k);
    temp += (k - 48);
    tmin = temp;
    delay_ms(800);

    lcd_putc("\fIngrese\n");
    printf(lcd_putc,"Temper.Max (   )");
    k = kbd_getc();
    while(K != '1' & k != '0') {
        k = kbd_getc();
    }
    lcd_gotoxy(13,2);
    printf(lcd_putc,"%c", k);
    temp = (k - 48) * 100;
    delay_ms(400);
    k = kbd_getc();

    if(temp < 100) {
        while(k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    if(temp == 100) {
        while(k > 53 | k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(14,2);
    printf(lcd_putc,"%c", k);
    temp += (k - 48) * 10;
    delay_ms(400);
    k = kbd_getc();

    if(temp == 150) {
        while(k != '0') {
            k = kbd_getc();
        }
    }
    if(temp < 150) {
        while(k == 0 | k == '*' | k == '#') {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(15,2);
    printf(lcd_putc,"%c", k);
    temp += (k - 48);
    tmax = temp;
    delay_ms(800);
}