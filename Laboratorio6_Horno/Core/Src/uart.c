#include "uart.h"

extern UART_HandleTypeDef huart4;
extern volatile uint8_t indice;
extern uint8_t mensaje;
extern volatile uint8_t instruccion[20];
extern volatile bool fin_instruccion;
extern Statechart mi_maquina;

static volatile bool overflow_rx = false;

void uart_enviar(const char *txt)
{
    HAL_UART_Transmit(&huart4, (uint8_t *)txt, strlen(txt), 100);
}

void enviar_estado_horno(void)
{
    char buffer[96];
    const char *op = statechart_is_state_active(&mi_maquina, Statechart_main_region_Hornomicroondas_Operacion_Funcionando)
                     ? "FUNCIONANDO" : "DETENIDO";
    const char *puerta = statechart_is_state_active(&mi_maquina, Statechart_main_region_Hornomicroondas_Puerta_Abierta)
                         ? "ABIERTA" : "CERRADA";
    const char *modo = statechart_is_state_active(&mi_maquina, Statechart_main_region_Hornomicroondas_Modo_Grill)
                       ? "GRILL" : "MICROONDAS";
    const char *aviso = statechart_get_aviso_activo(&mi_maquina) ? "ACTIVO" : "INACTIVO";

    snprintf(buffer, sizeof(buffer),
             "ESTADO -> Op: %s | Puerta: %s | Modo: %s | Aviso: %s\r\n",
             op, puerta, modo, aviso);
    uart_enviar(buffer);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart4)
	{
		if(mensaje == '\0')
		{
			if(indice > 0 || overflow_rx)
			{
				instruccion[indice] = mensaje;
				fin_instruccion = true;
				indice = 0;
			}

		}
		else if(indice < (sizeof(instruccion)-1))
		{
			instruccion[indice++] = mensaje;
		}
		else
		{
				overflow_rx = true;
			}
		HAL_UART_Receive_IT(&huart4, &mensaje, sizeof(uint8_t));
	}
}

void ejecutar_instruccion()
{
	int8_t num_instrc = -1;
	static bool op_antes = false;
	static bool puerta_antes = false;
	static bool modo_antes = false;
	static bool op_despues = false;
	static bool puerta_despues = false;
	static bool modo_despues = false;
	num_instrc = (strcmp((char*)instruccion, "startStop")==0) ? 0 :
				 (strcmp((char*)instruccion, "modo")==0) ? 1 :
				 (strcmp((char*)instruccion, "abrir_puerta")==0) ? 2 :
				 (strcmp((char*)instruccion, "cerrar_puerta")==0) ? 3 :  -1;
	if(num_instrc != -1)
	{
		op_antes = statechart_is_state_active(&mi_maquina, Statechart_main_region_Hornomicroondas_Operacion_Funcionando);
		puerta_antes = statechart_is_state_active(&mi_maquina, Statechart_main_region_Hornomicroondas_Puerta_Abierta);
		modo_antes = statechart_is_state_active(&mi_maquina, Statechart_main_region_Hornomicroondas_Modo_Grill);
	}
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
		uart_enviar("ERROR: Comando no reconocido\n");
		break;
	default:
		break;
	}
	op_despues = statechart_is_state_active(&mi_maquina, Statechart_main_region_Hornomicroondas_Operacion_Funcionando);
	puerta_despues = statechart_is_state_active(&mi_maquina, Statechart_main_region_Hornomicroondas_Puerta_Abierta);
	modo_despues = statechart_is_state_active(&mi_maquina, Statechart_main_region_Hornomicroondas_Modo_Grill);
	if (op_antes != op_despues || puerta_antes != puerta_despues || modo_antes != modo_despues)
	{
		uart_enviar("OK: Comando ejecutado\n");
	}
	else
	{
		uart_enviar("ERROR_ESTADO: Orden no permitida en el estado actual\n");
	}
}



