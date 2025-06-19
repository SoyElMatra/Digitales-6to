#include <16F873a.h>
#device adc=10

#FUSES XT,NOWDT,NOLVP
#use delay(clock=4000000)
#use standard_io(c)
#use standard_io(b)

void main() {
   int16 j;
	int q_l;
	int q_h;
 	int i;

   setup_adc_ports(ALL_ANALOG);                                   //Canal 0 analógico
   setup_adc(ADC_CLOCK_INTERNAL);    //Fuente de reloj RC

 	i=0;
   
   for (;;) {
	for(i=0;i<4;i++)	{		
      set_adc_channel(i);           //Habilitación canal0
      delay_ms(1);
      j = read_adc();                  //Lectura canal 0

			if(j==0&j<=295)
		{	output_b(0b00000000);
			output_c(0);
		}

			if(j>295&j<=418)
		{	output_b(0b00000001);
			output_c(0);
		}

			if(j>418&j<=512)
		{	output_b(0b00000011);
			output_c(0);
		}

			if(j>512&j<=591)
		{	output_b(0b00000111);
			output_c(0);
		}

			if(j>591&j<=661)
		{	output_b(0b00001111);
			output_c(0);
		}

			if(j>661&j<=723)
		{	output_b(0b00011111);
			output_c(0);
		}

			if(j>723&j<=781)
		{	output_b(0b00111111);
			output_c(0);
		}

			if(j>781&j<=835)
		{	output_b(0b01111111);
			output_c(0);
		}

			if(j>835&j<=886)
		{	output_b(0b11111111);
			output_c(0b00000000);
		}

			if(j>886&j<=934)
		{	output_b(0b11111111);
			output_c(0b00000001);
		}

			if(j>934&j<=980)
		{	output_b(0b11111111);
			output_c(0b00000011);
		}

			if(j>980)
		{	output_b(0b11111111);
			output_c(0b00000111);
		}
            if(j==1023)
          {	output_b(0b11111111);
            output_c(0b00001111);
          }
			if(i==0)
				{output_high(pin_c4);
					delay_ms(100);
					output_low(pin_c4);
				}
			if(i==1)
				{output_high(pin_c5);
					delay_ms(100);
					output_low(pin_c5);
				}
			if(i==2)
				{output_high(pin_c6);
					delay_ms(100);
					output_low(pin_c6);
				}
			if(i==3)
				{output_high(pin_c7);
					delay_ms(100);
					output_low(pin_c7);
				}	
	}		
      }
}


