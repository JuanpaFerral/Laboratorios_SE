#include "contador_display.h"

extern TIM_HandleTypeDef htim2;
extern osThreadId_t tarea_cronometro;
extern uint8_t alarma[];
extern int segundos_recibidos;

tiempo tiempo_iniciador()
{
	tiempo result = {0};
	result.minutos = segundos_recibidos/60;
	result.segundos = segundos_recibidos%60;
	result.display[0] = result.segundos % 10;
	result.display[1] = result.segundos /10;
	result.display[2] = result.minutos % 10;
	result.display[3] = result.minutos /10;
	return result;
}

void manejar_Display(void *tiempo_cronometro)
{
	uint32_t tick_actual = osKernelGetTickCount();
 	uint32_t ticks = pdMS_TO_TICKS(2);
	for(;;)
	{
		tick_actual += ticks;
		Display_Refrescar(((tiempo *)tiempo_cronometro)->display);
		osDelayUntil(tick_actual);
	}
}

void tarea_cronometro_(void *tiempo_cronometro)
{
	uint32_t tick_actual = osKernelGetTickCount();
	uint32_t ticks = pdMS_TO_TICKS(1000);
	osThreadSuspend(tarea_cronometro);
	for(;;)
	{
		tick_actual += ticks;
		cronometrar((tiempo *)tiempo_cronometro);
		osDelayUntil(tick_actual);
		tick_actual = osKernelGetTickCount();
	}
}

void Display_EscribirPatron(uint8_t patron)
{
	uint8_t valores[10]={
			0x3f, // 0
			0x06, // 1
			0x5b, // 2
			0x4f, // 3
			0x66, // 4
			0x6d, // 5
			0x7d, // 6
			0x07, // 7
			0x7f, // 8
			0x6f  // 9
	};

	if(patron < 10)
	{
		HAL_GPIO_WritePin(Seg_a_GPIO_Port,Seg_a_Pin, (valores[patron] & (1 << 0) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento A
		HAL_GPIO_WritePin(Seg_b_GPIO_Port,Seg_b_Pin, (valores[patron] & (1 << 1) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento B
		HAL_GPIO_WritePin(Seg_c_GPIO_Port,Seg_c_Pin, (valores[patron] & (1 << 2) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento C
		HAL_GPIO_WritePin(Seg_d_GPIO_Port,Seg_d_Pin, (valores[patron] & (1 << 3) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento D
		HAL_GPIO_WritePin(Seg_e_GPIO_Port,Seg_e_Pin, (valores[patron] & (1 << 4) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento E
		HAL_GPIO_WritePin(Seg_f_GPIO_Port,Seg_f_Pin, (valores[patron] & (1 << 5) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento F
		HAL_GPIO_WritePin(Seg_g_GPIO_Port,Seg_g_Pin, (valores[patron] & (1 << 6) ? GPIO_PIN_SET : GPIO_PIN_RESET)); // segmento G
	}
}
void Display_Refrescar(uint8_t *numeros)
{
	static uint8_t contador = 0;
	Display_EscribirPatron(numeros[contador]);
	if (contador == 2)
	{
		HAL_GPIO_WritePin(P_decimal_GPIO_Port,P_decimal_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(P_decimal_GPIO_Port,P_decimal_Pin,GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(Bit_0_GPIO_Port,Bit_0_Pin, (contador & (1 << 0) ? GPIO_PIN_SET : GPIO_PIN_RESET));
	HAL_GPIO_WritePin(Bit_1_GPIO_Port,Bit_1_Pin, (contador & (1 << 1) ? GPIO_PIN_SET : GPIO_PIN_RESET));
	contador++;
	if (contador >= 4)
	{
		contador = 0;
	}
}


void cronometrar(tiempo *tAux)
{
	if((tAux->segundos) > 0)
	{
		tAux->segundos--;
	}
	else
	{
		if((tAux->minutos) > 0)
		{
			tAux->minutos--;
			tAux->segundos = 59;
		}
	}
	tAux->display[0] = tAux->segundos % 10;
	tAux->display[1] = tAux->segundos /10;
	tAux->display[2] = tAux->minutos % 10;
	tAux->display[3] = tAux->minutos /10;
	if (tAux->minutos == 0 && tAux->segundos == 0)
		{
			HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
			responder_uart("ALARM\n");
			osThreadSuspend(tarea_cronometro);
		}
}
