#include "uart.h"

extern UART_HandleTypeDef huart4;
extern int segundos_recibidos;
extern tiempo cronometro_regresivo;
extern osThreadId_t tarea_cronometro;
extern osSemaphoreId_t sem_Uart_ejecutar;
extern osMessageQueueId_t cola_mensajes;
extern osMessageQueueId_t cola_caracter;
extern instruccion UART;

instruccion uart_iniciar()
{
	instruccion result;
	result.fin_instruccion = false;
	result.indice = 0;
	result.mensaje = 0;
	return result;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart4)
	{
		HAL_UART_Receive_IT(&huart4, &(UART.mensaje), sizeof(uint8_t));
		if (cola_caracter != NULL)
		{
			osMessageQueuePut(cola_caracter, &(UART.mensaje), 0, 0);
		}
	}
}

void leer_instruccion(void* argumentos)
{
	instruccion* aux = (instruccion*)argumentos;
	uint8_t caracter_recibido;
	for(;;)
	{
		if((osMessageQueueGet(cola_caracter, &caracter_recibido, NULL, osWaitForever) == osOK))
		{
			if(caracter_recibido == '\0')
			{
				aux->buffer[aux->indice] = caracter_recibido;
				aux->fin_instruccion = true;
				aux->indice = 0;
				osSemaphoreRelease(sem_Uart_ejecutar);
			}
			else if(aux->indice < (sizeof(aux->buffer)-1))
			{
				aux->buffer[(aux->indice)++] = caracter_recibido;
			}
		}
	}
}

void display_estado(uint8_t* estado)
{
	estado[0] = cronometro_regresivo.display[3] + '0';
	estado[1] = cronometro_regresivo.display[2] + '0';
	estado[2] = '.';
	estado[3] = cronometro_regresivo.display[1] + '0';
	estado[4] = cronometro_regresivo.display[0] + '0';
	estado[5] = '\n';
	estado[6] = '\0';
}

void responder_uart(const char* respuesta)
{
	tx_mensaje aux;
	strncpy(aux.texto, respuesta,  sizeof(aux.texto)-1);
	aux.texto[sizeof(aux.texto) - 1] = '\0';
	osMessageQueuePut(cola_mensajes, &aux, 0, 0);
}

void ejecutar_instruccion(void* argumentos)
{
	instruccion* aux = (instruccion*)argumentos;
	bool estado_tarea = false;
	int8_t num_instrc;
	uint8_t *ptr = NULL;
	uint8_t error[] = "ERROR\n";
	uint8_t ok[] = "OK\n";
	uint8_t run[] = "RUNNING\n";
	uint8_t pause[] = "PAUSE\n";
	uint8_t estado[7];
	for(;;)
	{
		if((osSemaphoreAcquire(sem_Uart_ejecutar, osWaitForever) == osOK))
		{
			num_instrc = -1;
			if(strncmp((char*)(aux->buffer), "SET",3) == 0)
			{
				ptr = (aux->buffer) + 3;
				segundos_recibidos = atoi((char*)ptr);
				if(segundos_recibidos >= 0 && segundos_recibidos<= 5999)
				{
					num_instrc = 0;
				}
			}
			else
			{
				num_instrc = (strcmp((char*)(aux->buffer), "START")==0) ? 1 :
						     (strcmp((char*)(aux->buffer), "PAUSE")==0) ? 2 :
							 (strcmp((char*)(aux->buffer), "RESET")==0) ? 3 :
							 (strcmp((char*)(aux->buffer), "STATUS")==0) ? 4 : -1;
			}
			estado_tarea = (eTaskGetState((TaskHandle_t)tarea_cronometro) == eSuspended);
			switch (num_instrc) {
			case 0:
				if(estado_tarea)
				{
					responder_uart((char*)ok);
					cronometro_regresivo.minutos = segundos_recibidos/60;
					cronometro_regresivo.segundos = segundos_recibidos%60;
					cronometro_regresivo.display[0] = cronometro_regresivo.segundos % 10;
					cronometro_regresivo.display[1] = cronometro_regresivo.segundos /10;
					cronometro_regresivo.display[2] = cronometro_regresivo.minutos % 10;
					cronometro_regresivo.display[3] = cronometro_regresivo.minutos /10;
				}
				else
				{
					responder_uart((char*)error);
				}
				aux->fin_instruccion = false;
				break;
			case 1:
				responder_uart((char*)ok);
				aux->fin_instruccion = false;
				start_pause(num_instrc);
				break;
			case 2:
				responder_uart((char*)ok);
				aux->fin_instruccion = false;
				start_pause(num_instrc);
				break;
			case 3:
				if(!estado_tarea)
				{
					responder_uart((char*)error);
				}
				else
				{
					responder_uart((char*)ok);
					restart();
				}
				aux->fin_instruccion = false;
				break;
			case 4:
				display_estado(estado);
				if(!estado_tarea)
				{
					responder_uart((char*)run);
					responder_uart((char*)estado);
				}
				else
				{
					responder_uart((char*)pause);
					responder_uart((char*)estado);
				}
				aux->fin_instruccion = false;
				break;
			case -1:
				responder_uart((char*)error);
				aux->fin_instruccion = false;
				break;
			default:
				break;
			}
		}
	}
}

void enviar_instruccion(void* argumentos)
{
	tx_mensaje aux;
	for(;;)
	{
		if(osMessageQueueGet(cola_mensajes, &aux, NULL, osWaitForever) == osOK)
		{
			HAL_UART_Transmit(&huart4, (uint8_t*)aux.texto, strlen(aux.texto), osWaitForever);
		}
	}
}


