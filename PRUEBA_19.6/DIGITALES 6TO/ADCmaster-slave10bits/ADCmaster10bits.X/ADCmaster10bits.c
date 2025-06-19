#include <16F877A.h>
#device adc=10
#FUSES XT,NOWDT,NOLVP
#use delay(clock=4000000)
#use I2C(MASTER, SDA=PIN_C4, SLOW, SCL=PIN_C3)
#use standard_io(B) 
#use standard_io(d)

int valor;
int16 q;
int q_l_1, q_h_1;
int q_l_2, q_h_2;

/***************************** Envío I2C **************************************/
void envio_I2C ()
{
   i2c_start();
   i2c_write(0x0a);                    
   i2c_write(q_l_1);                
   i2c_write(q_h_1); 
   i2c_stop(); 
   i2c_start();
   i2c_write(0x0c);
   i2c_write(q_l_2);
   i2c_write(q_h_2);
   
   i2c_stop();                       
}
/******************************************************************************/
     


void main() 
{
	setup_adc_ports(AN0_AN1_AN3);            //Canal 0 analógico
 	setup_adc(ADC_CLOCK_INTERNAL);    //Fuente de reloj RC
    for (;;) 
    {
        set_adc_channel(0);    //Habilitación canal0
        delay_us(20);
        
        q = read_adc();  
        q_l_1=make8(q,0);
        //output_b(q_l_1);
        q_h_1=make8(q,1);
        //output_d(q_h_1);
        
        set_adc_channel(3);    
        delay_us(20);
        
        q = read_adc();  
        q_l_2=make8(q,0);
        //output_b(q_l_2);
        q_h_2=make8(q,1);
        //output_d(q_h_2); 
        
        envio_I2C();
        delay_ms(100);
   }
}


