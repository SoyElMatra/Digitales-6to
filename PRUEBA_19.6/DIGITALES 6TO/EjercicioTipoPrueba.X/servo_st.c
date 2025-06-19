/*
 
#define use_servo_1                                   // Habilita el servo 1
#define use_servo_2                                   // Habilita el servo 2
#define use_servo_3                                   // Habilita el servo 3

#define servo_1 PIN_B0                                // Asigna el pin de control del servo 1 al pin B0
#define servo_2 PIN_B1                                // Asigna el pin de control del servo 2 al pin B1
#define servo_3 PIN_B2                                // Asigna el pin de control del servo 3 al pin B2

#include <servo_st.c>                                 // Libreria para controlar servomotores
 
 servo_init()
 
 servo_1_write(ANGULO);
 */



/* #include <servo_st.c>
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 26/05/2020
* Electronica y Circuitos
*
*/

#define v_min 11
#define v_max 48

int16 cont_st = 0;
int8 pwm_st[10];

#INT_TIMER0
void timer_0()
{
   cont_st = cont_st + 1;
   if(cont_st > 385)
   {
      cont_st = 0;
   }
   #ifdef use_servo_1
   if(cont_st < pwm_st[0]){output_high(servo_1);} else{output_low(servo_1);}
   #endif
   
   #ifdef use_servo_2
   if(cont_st < pwm_st[1]){output_high(servo_2);} else{output_low(servo_2);}
   #endif
   
   #ifdef use_servo_3
   if(cont_st < pwm_st[2]){output_high(servo_3);} else{output_low(servo_3);}
   #endif
   
   #ifdef use_servo_4
   if(cont_st < pwm_st[3]){output_high(servo_4);} else{output_low(servo_4);}
   #endif
   
   #ifdef use_servo_5
   if(cont_st < pwm_st[4]){output_high(servo_5);} else{output_low(servo_5);}
   #endif
   
   #ifdef use_servo_6
   if(cont_st < pwm_st[5]){output_high(servo_6);} else{output_low(servo_6);}
   #endif
   
   #ifdef use_servo_7
   if(cont_st < pwm_st[6]){output_high(servo_7);} else{output_low(servo_7);}
   #endif
   
   #ifdef use_servo_8
   if(cont_st < pwm_st[7]){output_high(servo_8);} else{output_low(servo_8);}
   #endif
   
   #ifdef use_servo_9
   if(cont_st < pwm_st[8]){output_high(servo_9);} else{output_low(servo_9);}
   #endif
   
   #ifdef use_servo_10
   if(cont_st < pwm_st[9]){output_high(servo_10);} else{output_low(servo_10);}
   #endif
}

void servo_init()
{
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_1);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);
   set_timer0(230);
}

long map_int(int16 x, int16 eMin, int16 eMax, int16 sMin, int16 sMax)
{
   return(x -eMin)*(sMax-sMin)/(eMax-eMin)+sMin;
}

#ifdef use_servo_1
void servo_1_write(int angle1)
{
   pwm_st[0] = map_int(angle1, 0, 180, v_min, v_max);
}
#endif

#ifdef use_servo_2
void servo_2_write(int angle2)
{
   pwm_st[1] = map_int(angle2, 0, 180, v_min, v_max);
}
#endif

#ifdef use_servo_3
void servo_3_write(int angle3)
{
   pwm_st[2] = map_int(angle3, 0, 180, v_min, v_max);
}
#endif

#ifdef use_servo_4
void servo_4_write(int angle4)
{
   pwm_st[3] = map_int(angle4, 0, 180, v_min, v_max);
}
#endif

#ifdef use_servo_5
void servo_5_write(int angle5)
{
   pwm_st[4] = map_int(angle5, 0, 180, v_min, v_max);
}
#endif

#ifdef use_servo_6
void servo_6_write(int angle6)
{
   pwm_st[5] = map_int(angle6, 0, 180, v_min, v_max);
}
#endif

#ifdef use_servo_7
void servo_7_write(int angle7)
{
   pwm_st[6] = map_int(angle7, 0, 180, v_min, v_max);
}
#endif

#ifdef use_servo_8
void servo_8_write(int angle8)
{
   pwm_st[7] = map_int(angle8, 0, 180, v_min, v_max);
}
#endif

#ifdef use_servo_9
void servo_9_write(int angle9)
{
   pwm_st[8] = map_int(angle9, 0, 180, v_min, v_max);
}
#endif

#ifdef use_servo_10
void servo_10_write(int angle10)
{
   pwm_st[9] = map_int(angle10, 0, 180, v_min, v_max);
}
#endif
