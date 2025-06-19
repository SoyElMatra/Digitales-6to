#include <18f2550.h>       // Incluir el archivo de cabecera para el PIC18F2550
#fuses HS,NOWDT,NOPROTECT,NOLVP,NODEBUG,VREGEN,NOMCLR
#use delay(clock=20000000)   // Configurar la frecuencia del reloj (20 MHz)

#define LED_PIN PIN_B0    // Definir el pin donde conectamos el LED

#INT_TIMER0  // Interrupción para el Timer0
void TIMER0_isr() {
    output_toggle(LED_PIN);  // Cambiar el estado del LED (encender/apagar)
    set_timer0(0);  // Restablecer el Timer0 a 0
}

void main() {
    set_tris_b(0xFF);  // Configurar todos los pines de PORTB como salida
    output_b(0x00);  // Inicializar PORTB (apagado de todos los pines)

    // Configuración del Timer0
    setup_timer_0(T0_INTERNAL | T0_DIV_256);  // Usar el temporizador interno con prescaler 256
    set_timer0(0);  // Inicializar Timer0 a 0

    // Habilitar la interrupción de Timer0
    enable_interrupts(INT_TIMER0);  
    enable_interrupts(GLOBAL);  // Habilitar interrupciones globales

    while(TRUE) {
        // El programa principal no hace nada, ya que la interrupción se encarga de la tarea
    }
}
