#include "LED.h"

LED led_iniciador(GPIO_TypeDef *puerto, uint16_t pin, uint8_t frec)
{
	LED result;
	result.puerto = puerto;
	result.pin = pin;
	result.frecuencia = frec;
	return result;
}


void prenderLED(void* argumentos)
{
	uint8_t frecuencia_led = ((LED*)argumentos)->frecuencia;
	uint16_t pin_led = ((LED*)argumentos)->pin;
	GPIO_TypeDef *puerto_led = ((LED*)argumentos)->puerto;
	uint32_t periodo = pdMS_TO_TICKS(1000/(2*frecuencia_led));
	uint32_t tick_actual = osKernelGetTickCount();
	for(;;)
	{
		HAL_GPIO_TogglePin(puerto_led, pin_led);
		tick_actual += periodo;
		osDelayUntil(tick_actual);
	}
}
