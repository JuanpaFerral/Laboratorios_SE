#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include "cmsis_os.h"
#include "contador_display.h"
#include "estados.h"

typedef struct
{
	uint8_t buffer[10];
	uint8_t indice;
	bool fin_instruccion;
	uint8_t mensaje;
}instruccion;

typedef struct
{
	char texto[10];
} tx_mensaje;

instruccion uart_iniciar(void);
void leer_instruccion(void*);
void display_estado(uint8_t*);
void responder_uart(const char*);
void ejecutar_instruccion(void*);
void enviar_instruccion(void*);

#endif /* INC_UART_H_ */
