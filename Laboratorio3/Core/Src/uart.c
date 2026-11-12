#include "uart.h"     // Si creas un header para tus propias funciones

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart4;
extern volatile bool runtime;
extern volatile uint8_t indice;
extern uint8_t mensaje;
extern volatile uint8_t instruccion[10];
extern volatile bool fin_instruccion;
extern volatile bool fin_cronometro;
extern uint8_t segundos;
extern uint8_t minutos;
extern int segundos_recibidos;
extern uint8_t alarma[];
extern uint8_t display[4];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart4)
	{
		if(mensaje == '\0')
		{
			instruccion[indice] = mensaje;
			fin_instruccion = true;
			indice = 0;
		}
		else if(indice < (sizeof(instruccion)-1))
		{
			instruccion[indice++] = mensaje;
		}
		HAL_UART_Receive_IT(&huart4, &mensaje, sizeof(uint8_t));
	}
}

void display_estado(uint8_t* estado)
{
	estado[0] = display[3] + '0';
	estado[1] = display[2] + '0';
	estado[2] = '.';
	estado[3] = display[1] + '0';
	estado[4] = display[0] + '0';
	estado[5] = '\n';
	estado[6] = '\0';
}

void ejecutar_instruccion()
{
	int8_t num_instrc = -1;
	uint8_t error[] = "ERROR\n";
	uint8_t ok[] = "OK\n";
	uint8_t run[] = "RUNNING\n";
	uint8_t pause[] = "PAUSE\n";
	uint8_t estado[7];
	if(sscanf((char*)instruccion, "SET %d", &segundos_recibidos)==1)
	{
		if(segundos_recibidos >= 0 && segundos_recibidos<= 5999)
		{
			num_instrc = 0;
		}
	}
	else
	{
		num_instrc = (strcmp((char*)instruccion, "START")==0) ? 1 :
				     (strcmp((char*)instruccion, "PAUSE")==0) ? 2 :
				     (strcmp((char*)instruccion, "RESET")==0) ? 3 :
				     (strcmp((char*)instruccion, "STATUS")==0) ? 4 : -1;
	}
	switch (num_instrc) {
		case 0:
			if(!runtime)
			{
				HAL_UART_Transmit(&huart4, ok, strlen((char*)ok), 100);
				minutos = segundos_recibidos/60;
				segundos = segundos_recibidos%60;
				display[0] = segundos % 10;
				display[1] = segundos /10;
				display[2] = minutos % 10;
				display[3] = minutos /10;
			}
			else
			{
				HAL_UART_Transmit(&huart4, error, strlen((char*)error), 100);
			}
			fin_instruccion = false;
			break;
		case 1:
			HAL_UART_Transmit(&huart4, ok, strlen((char*)ok), 100);
			fin_instruccion = false;
			start_pause(num_instrc);
			break;
		case 2:
			HAL_UART_Transmit(&huart4, ok, strlen((char*)ok), 100);
			fin_instruccion = false;
			start_pause(num_instrc);
			break;
		case 3:
			if(runtime)
			{
				HAL_UART_Transmit(&huart4, error, strlen((char*)error), 100);
			}
			else
			{
				HAL_UART_Transmit(&huart4, ok, strlen((char*)ok), 100);
				restart();
			}
			fin_instruccion = false;
			break;
		case 4:
			display_estado(estado);
			if(runtime)
			{
				HAL_UART_Transmit(&huart4, run, strlen((char*)run), 100);
				HAL_UART_Transmit(&huart4, estado, strlen((char*)estado), 100);
			}
			else
			{
				HAL_UART_Transmit(&huart4, pause, strlen((char*)pause), 100);
				HAL_UART_Transmit(&huart4, estado, strlen((char*)estado), 100);
			}
			fin_instruccion = false;
			break;
		case -1:
			HAL_UART_Transmit(&huart4, error, strlen((char*)error), 100);
			fin_instruccion = false;
			break;
		default:
			break;
	}
}














/*
 * uart.c
 *
 *  Created on: 17 sept 2026
 *      Author: juanp
 */


