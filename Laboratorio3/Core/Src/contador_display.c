#include "contador_display.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart4;
extern volatile bool cambio;
extern volatile bool runtime;
extern volatile bool fin_cronometro;
extern uint8_t cronometro;
extern uint8_t centesima;
extern uint8_t segundos;
extern uint8_t minutos;
extern uint8_t alarma[];
extern uint8_t display[4];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == htim1.Instance)
	{
		cambio = true;
		Display_Refrescar(display);
		if(runtime)
		{
			cronometrar();
		}
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
void Display_Refrescar(uint8_t* numeros)
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
	cambio = false;
	contador++;
	if (contador >= 4)
	{
		contador = 0;
	}
}


void cronometrar()
{
	cronometro++;
	if(cronometro == 5)
	{
		centesima++;
		cronometro = 0;
	}
	if(centesima == 100)
	{
		centesima = 0;
		if(segundos > 0)
		{
			segundos--;
		}
		else
		{
			if(minutos > 0)
			{
				minutos--;
				segundos = 59;
			}
			else
			{
				runtime = false;
				fin_cronometro = true;
				HAL_UART_Transmit(&huart4, alarma, strlen((char*)alarma), 100);
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
			}
		}
	}
	display[0] = segundos % 10;
	display[1] = segundos /10;
	display[2] = minutos % 10;
	display[3] = minutos /10;
}


