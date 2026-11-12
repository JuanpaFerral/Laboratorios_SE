#include "estados.h"

extern tiempo cronometro_regresivo;
extern int segundos_recibidos;
extern osThreadId_t tarea_cronometro;
extern TIM_HandleTypeDef htim2;

void start_pause(uint8_t opcion)
{
	switch (opcion)
	{
		case 1:
			osThreadResume(tarea_cronometro);
			break;
		case 2:
			osThreadSuspend(tarea_cronometro);
			break;
	}
}
void restart()
{
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
	cronometro_regresivo.minutos = segundos_recibidos/60;
	cronometro_regresivo.segundos = segundos_recibidos%60;
	cronometro_regresivo.display[0] = cronometro_regresivo.segundos % 10;
	cronometro_regresivo.display[1] = cronometro_regresivo.segundos /10;
	cronometro_regresivo.display[2] = cronometro_regresivo.minutos % 10;
	cronometro_regresivo.display[3] = cronometro_regresivo.minutos /10;
}



