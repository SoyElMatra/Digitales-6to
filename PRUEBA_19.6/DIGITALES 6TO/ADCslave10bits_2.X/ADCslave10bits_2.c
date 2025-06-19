#include <16F877a.h>
#FUSES XT,NOWDT,nolvp
#use delay(clock=4000000)
#use I2C(SLAVE, SDA=PIN_C4 ,SLOW, SCL=PIN_C3, ADDRESS=0x0a,NOFORCE_SW)
#use standard_io(b)
#use standard_io(d)

int16 q;
int q_l;
int q_h;
int estado=0;
int valor;

#int_SSP
SSP_isr()
{
	valor=i2c_read();
    estado++;
    if(estado==2)
    {
        output_b(valor);
    }
    if(estado==3)
    {
        output_d(valor);
    }
    if(estado>=4 && estado <6)
    {}
    else if(estado == 6) estado=0;
    
}
void main() 
{
    enable_interrupts(INT_SSP);
  	enable_interrupts(GLOBAL);
    for(;;)	
    { 

    }
             
}

