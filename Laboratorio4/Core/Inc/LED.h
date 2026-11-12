/*
 * LED.h
 *
 *  Created on: Sep 22, 2026
 *      Author: Yo
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"
#include "cmsis_os.h"

typedef struct
{
	GPIO_TypeDef *puerto;
	uint16_t pin;
	uint8_t frecuencia;
}LED;

LED led_iniciador(GPIO_TypeDef *, uint16_t, uint8_t);

void prenderLED(void*);


#endif /* INC_LED_H_ */
