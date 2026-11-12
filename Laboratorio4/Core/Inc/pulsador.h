#ifndef INC_PULSADOR_H_
#define INC_PULSADOR_H_

#include "main.h"
#include "cmsis_os.h"
#include "contador_display.h"
#include "estados.h"

typedef struct
{
	GPIO_TypeDef *puerto;
	uint16_t pin;
	IRQn_Type cod_interrupcion;
	osSemaphoreId_t semaforo;
	GPIO_PinState estado_anterior;
}pulsador;

void debounce(void*);
pulsador pulsador_iniciar(GPIO_TypeDef *, uint16_t, IRQn_Type, osSemaphoreId_t);

#endif /* INC_PULSADOR_H_ */
