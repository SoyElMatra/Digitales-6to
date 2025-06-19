#include <stdio.h>

int main()
{
    float tiempo, f_osc, tmr1;
    int prescaler, i;

    printf("Cual es la frecuencia del oscilador? (Expresado en MHz)\n");
    scanf("%f", &f_osc);
    f_osc = f_osc * 1000000;

    printf("Cuanto tiempo desea interrumpir? (Expresado en milisegundos)\n");
    scanf("%f", &tiempo);
    tiempo = tiempo * 0.001;

    printf("PRESCALERS POSIBLES PARA TIMER1:\n");
    for (i = 1; i <= 8; i = i * 2)  // Timer1 generalmente permite 1, 2, 4, 8
    {
        if ((65536 - (tiempo * f_osc) / (4 * i)) > 0 && (65536 - (tiempo * f_osc) / (4 * i)) < 65535)
        {
            printf("%d ", i);
        }
    }

    printf("\nSeleccione un prescaler:\n");
    scanf("%d", &prescaler);

    tmr1 = 65536 - (tiempo * f_osc) / (4 * prescaler);

    printf("\nEl valor del Timer1 necesario seria de %.2f", tmr1);

    return 0;
}
 