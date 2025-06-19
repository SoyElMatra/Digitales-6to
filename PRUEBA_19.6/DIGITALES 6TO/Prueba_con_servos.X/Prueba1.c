#include <16f628A.h>
#fuses HS,NOWDT,NOLVP
#use delay(clock=20M)

#use standard_io(B) 
#use standard_io(A)

#define use_servo_1  
#define use_servo_2
#define use_servo_3
#define servo_1 PIN_B0 
#define servo_2 PIN_B7
#define servo_3 PIN_B6

#include <servo_st.c>

int cont=0;
int cambio=0;

#INT_TIMER1
void tmr1(void)
{
    if(cont<50) 
    {
        cont++;
    }
    else
    {
        cambio=!cambio;
        cont=0;
    }
    set_timer1(62411);
}

void main()
{
    servo_init();
    int cond=1;
    
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
    enable_interrupts(GLOBAL);
    enable_interrupts(INT_TIMER1);
    set_timer1(62411);
    
    while(1)
    {  
        if(input(PIN_A0)==0)
        {
            while(input(PIN_A0)==0)
            {
                if(cambio == 0)
                {
                    servo_3_write(90); 
                }
                else
                {
                    servo_3_write(0); 
                }
            }
            cond = !cond;     
        }
        if(cond==0)
        {
            servo_1_write(0);    
            servo_2_write(180);
        }
        else
        {
            servo_1_write(180);
            servo_2_write(0);
        }
    }
}