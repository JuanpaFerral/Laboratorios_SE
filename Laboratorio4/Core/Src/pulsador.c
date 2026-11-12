#include "pulsador.h"

extern osThreadId_t tarea_cronometro;
extern tiempo cronometro_regresivo;

pulsador pulsador_iniciar(GPIO_TypeDef *puerto_aux, uint16_t pin_aux, IRQn_Type interrup_aux, osSemaphoreId_t semaforo_aux)
{
	pulsador result;
	result.puerto = puerto_aux;
	result.pin = pin_aux;
	result.cod_interrupcion = interrup_aux;
	result.semaforo = semaforo_aux;
	result.estado_anterior = HAL_GPIO_ReadPin(result.puerto, result.pin);
	return result;
}

void debounce(void* argumentos)
{
	pulsador* aux = (pulsador*)argumentos;
	GPIO_PinState estado_actual;
	for(;;)
	{
		if((osSemaphoreAcquire(aux->semaforo, osWaitForever) == osOK))
		{
			osDelay(50);
			estado_actual = HAL_GPIO_ReadPin(aux->puerto, aux->pin);
			if(estado_actual == GPIO_PIN_RESET && aux->estado_anterior == GPIO_PIN_RESET)
			{
				if(aux->pin == SW1_Pin)
				{
					if(eTaskGetState((TaskHandle_t)tarea_cronometro) == eSuspended)
					{
						start_pause(1);
					}
					else
					{
						start_pause(2);
					}
				}
				else if(eTaskGetState((TaskHandle_t)tarea_cronometro) == eSuspended)
				{
					restart();
				}
			}
			__HAL_GPIO_EXTI_CLEAR_IT(aux->pin);
			HAL_NVIC_ClearPendingIRQ(aux->cod_interrupcion);
			HAL_NVIC_EnableIRQ(aux->cod_interrupcion);
		}
	}
}
