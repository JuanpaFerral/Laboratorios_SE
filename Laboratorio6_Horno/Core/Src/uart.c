#include "uart.h"

extern UART_HandleTypeDef huart4;
extern volatile uint8_t indice;
extern uint8_t mensaje;
extern volatile uint8_t instruccion[10];
extern volatile bool fin_instruccion;
extern Statechart mi_maquina;

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

void ejecutar_instruccion()
{
	int8_t num_instrc = -1;
	uint8_t error[] = "ERROR\n";
	num_instrc = (strcmp((char*)instruccion, "startStop")==0) ? 0 :
				 (strcmp((char*)instruccion, "modo")==0) ? 1 :
				 (strcmp((char*)instruccion, "abrir_puerta")==0) ? 2 :
				 (strcmp((char*)instruccion, "cerrar_puerta")==0) ? 3 :  -1;
	switch (num_instrc)
	{
	case 0:
		statechart_raise_startStop(&mi_maquina);
		fin_instruccion = false;
		break;
	case 1:
		statechart_raise_modo(&mi_maquina);
		fin_instruccion = false;
		break;
	case 2:
		statechart_raise_abrir_puerta(&mi_maquina);
		fin_instruccion = false;
		break;
	case 3:
		statechart_raise_cerrar_puerta(&mi_maquina);
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



