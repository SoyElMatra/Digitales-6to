#include <16F876A.h>
#FUSES XT,NOWDT,NOLVP
#use delay(clock=4000000)
//#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7)
//#use I2C(MASTER, SDA=PIN_C4,SCL=PIN_C3,FAST=100000)
#use standard_io(A)
#use standard_io(B)

//#define LCD_DATA_PORT getenv("SFR:PORTC") 
#define LCD_ENABLE_PIN  PIN_C3
#define LCD_RS_PIN      PIN_C1
#define LCD_RW_PIN      PIN_C2
#define LCD_DATA4       PIN_C4
#define LCD_DATA5       PIN_C5
#define LCD_DATA6       PIN_C6
#define LCD_DATA7       PIN_C7
#include <LCD.c>

#define trigger_B1 PIN_B1
#define trigger_B2 PIN_B2
#define trigger_B3 PIN_B3
#define trigger_A0 PIN_A0
#define echo PIN_B4
int1 flag_B4 = 0;
int1 flag_B5 = 0;
int1 flag_B6 = 0;
int1 flag_B7 = 0;
int16 BW_echo;
int16 distancia = 0;

#INT_RB
void isr_RB(void)
{
    int8 dummy = input_b(); // Lectura obligatoria para limpiar el mismatch
    if(((input(PIN_B4)==1)&& flag_B4==0)||((input(PIN_B4)==0)&& flag_B4==1))
    {
        if(flag_B4==0)
        {
            set_timer1(0);
            flag_B4= 1;
        }
        else
        {
            BW_echo = get_timer1();
            flag_B4= 0;
        }
    }
    if(((input(PIN_B5)==1)&& flag_B5==0)||((input(PIN_B5)==0)&& flag_B5==1))
    {
        if(flag_B5==0)
        {
            set_timer1(0);
            flag_B5= 1;
        }
        else
        {
            BW_echo = get_timer1();
            flag_B5= 0;
        }
    }
    if(((input(PIN_B6)==1)&& flag_B6==0)||((input(PIN_B6)==0)&& flag_B6==1))
    {
        if(flag_B6==0)
        {
            set_timer1(0);
            flag_B6= 1;
        }
        else
        {
            BW_echo = get_timer1();
            flag_B6= 0;
        }
    }
    if(((input(PIN_B7)==1)&& flag_B7==0)||((input(PIN_B7)==0)&& flag_B7==1))
    {
        if(flag_B7==0)
        {
            set_timer1(0);
            flag_B7= 1;
        }
        else
        {
            BW_echo = get_timer1();
            flag_B7= 0;
        }
    }
    clear_interrupt(INT_RB);
}

void main(void)
{
    lcd_init();
    setup_timer_1 (T1_INTERNAL | T1_DIV_BY_1);
    enable_interrupts(GLOBAL);
    delay_ms(100);
    clear_interrupt(INT_RB);
    enable_interrupts(INT_RB);
    while(1)
    {
        output_high(trigger_B1);
        delay_us(10);
        output_low(trigger_B1);
        lcd_gotoxy(1,1);
        distancia = BW_echo/58;
        printf(lcd_putc,"B4:%3Lu",distancia);
        delay_ms(100);
        output_high(trigger_B2);
        delay_us(10);
        output_low(trigger_B2);
        lcd_gotoxy(8,1);
        distancia = BW_echo/58;
        printf(lcd_putc,"B5:%3Lu",distancia);
        delay_ms(100);
        output_high(trigger_B3);
        delay_us(10);
        output_low(trigger_B3);
        lcd_gotoxy(1,2);
        distancia = BW_echo/58;
        printf(lcd_putc,"B6:%3Lu",distancia);
        delay_ms(100);
        output_high(trigger_A0);
        delay_us(10);
        output_low(trigger_A0);
        lcd_gotoxy(8,2);
        distancia = BW_echo/58;
        printf(lcd_putc,"B7:%3Lu",distancia);
        delay_ms(100);
    }
}