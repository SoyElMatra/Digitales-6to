#include <stdio.h>

int main()
{
	float tiempo, f_osc, tmr0;
	int prescaler, i;
	
	printf("Cual es la frecuencia del oscilador\?\t(Expresado en megas)\n");
	scanf("%f", &f_osc);
	f_osc=f_osc*1000000;
	
	printf("Cuanto tiempo desea interrumpir\?(Expresado en milisegundos)\n");
	scanf("%f", &tiempo);
	tiempo=tiempo*0.001;
	
	printf("PRESCALERS POSIBLES:\n");
	for(i=1;i<=256;i=i*2)
	{
		if((256-(tiempo*f_osc)/(4*i))>0 && (256-(tiempo*f_osc)/(4*i))<255)
		{
			printf("%d ", i);
		}
	}
	printf("\n");
	scanf("%d", &prescaler);
	
	tmr0=(256-(tiempo*f_osc)/(4*prescaler));

	printf("\nEl valor del timer 0 necesario seria de %.2f", tmr0);	
}