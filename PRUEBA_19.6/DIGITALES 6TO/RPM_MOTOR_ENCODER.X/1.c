#include <16F876A.h>
#fuses XT,NOWDT,NOPROTECT,NOLVP,NOBROWNOUT
#use delay(clock=4000000)
#use standard_io(A)
#use standard_io(B)

#define LCD_DATA_PORTB
#define use_portb_kbd TRUE

#include <lcd.c>
#include <kbd.c>

char k;
int temp;

int contador=0;
int flag=0; 
int16 pwm = 0;
int16 rpm_ideal;
int16 rpm_ideal_eprom;
int16 rpm;

#INT_TIMER1 
void TIMER1(void)
{
    contador = get_timer0();
    flag=1;
    set_timer1(53036);
    set_timer0(0);
}

void cambiar_temp(void) 
{
    
    disable_interrupts(INT_TIMER1);
    //write_eeprom(0x00,1);
    lcd_putc("\fIngrese\n");
    printf(lcd_putc,"RPM (   )");
    k = kbd_getc();
    while(K != '1' & k != '0') 
    {
        k = kbd_getc();
    }
    lcd_gotoxy(6,2);
    printf(lcd_putc,"%c", k);
    temp = (k - 48) * 100;
    delay_ms(400);
    k = kbd_getc();

    if(temp < 100) {
        while(k == 0 | k == '*' | k == '#') 
        {
            k = kbd_getc();
        }
    }
    if(temp == 100) {
        while(k > 56 | k == 0 | k == '*' | k == '#') 
        {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(7,2);
    printf(lcd_putc,"%c", k);
    temp += (k - 48) * 10;
    delay_ms(400);
    k = kbd_getc();

    if(temp == 180) 
    {
        while(k != '0') 
        {
            k = kbd_getc();
        }
    }
    if(temp < 180) 
    {
        while(k == 0 | k == '*' | k == '#') 
        {
            k = kbd_getc();
        }
    }
    lcd_gotoxy(8,2);
    printf(lcd_putc,"%c", k);
    temp += (k - 48);
    rpm_ideal = temp;
    write_eeprom(0x00, rpm_ideal);
    delay_ms(800);
    printf(lcd_putc,"\f");
    enable_interrupts(INT_TIMER1);
}

void main(void)
{
    setup_timer_2(T2_DIV_BY_16,255,1);
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
    setup_timer_0(T0_EXT_L_TO_H|T0_DIV_1);
    set_timer0(0);
    set_timer1(53036);
    setup_ccp1(CCP_PWM);
    enable_interrupts(INT_TIMER1);
    enable_interrupts(GLOBAL);
    

    
    //port_b_pullups(0b11111111);
    
    lcd_init();
    kbd_init();
    
    PORT_B_pullups(TRUE); 
    set_pwm1_duty(0);
   
    while(1)
    {
        if(flag==1)
        {
            rpm = (600*contador) / 360;
            rpm_ideal_eprom = read_eeprom(0x00);
            if(rpm_ideal_eprom > rpm)
            {
                if(pwm<1023)
                {
                   pwm++; 
                }
                set_pwm1_duty(pwm);
            }
            if(rpm_ideal_eprom < rpm)
            {
                if(pwm>0)
                {
                   pwm--; 
                }
                set_pwm1_duty(pwm);
            }
            /*if(rpm_ideal == rpm)
            {
                set_pwm1_duty(pwm);
            }*/
            printf(lcd_putc, "\f");
            printf(lcd_putc, "RPM REAL = %lu\nRPM = %lu", rpm, rpm_ideal_eprom);
            flag=0;
        }
        
      k=kbd_getc();
        if(k == '#') 
        {
            cambiar_temp();
        }
        
    }
}