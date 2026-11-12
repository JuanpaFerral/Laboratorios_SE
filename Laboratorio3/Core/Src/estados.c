#include "estados.h"

extern TIM_HandleTypeDef htim2;
extern volatile bool runtime;
extern uint8_t segundos;
extern uint8_t minutos;
extern int segundos_recibidos;
extern uint8_t display[4];
extern volatile bool fin_cronometro;

void start_pause(uint8_t opcion)
{
	switch (opcion)
	{
		case 1:
			runtime = true;
			break;
		case 2:
			runtime = false;
			break;
	}
}
void restart()
{
	minutos = segundos_recibidos/60;
	segundos = segundos_recibidos%60;
	display[0] = segundos % 10;
	display[1] = segundos /10;
	display[2] = minutos % 10;
	display[3] = minutos /10;
	if(fin_cronometro)
	{
		fin_cronometro = false;
		HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
	}
}


