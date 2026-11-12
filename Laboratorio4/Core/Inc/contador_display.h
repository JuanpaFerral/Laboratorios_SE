#ifndef INC_CONTADOR_DISPLAY_H_
#define INC_CONTADOR_DISPLAY_H_

#include "main.h"
#include "cmsis_os.h"
#include "uart.h"

typedef struct
{
	uint8_t display[4];
	uint8_t segundos;
	uint8_t minutos;
}tiempo;

tiempo tiempo_iniciador(void);
void Display_EscribirPatron(uint8_t);
void Display_Refrescar(uint8_t*);
void cronometrar(tiempo*);
void manejar_Display(void*);
void tarea_cronometro_(void *tiempo_cronometro);

#endif /* INC_CONTADOR_DISPLAY_H_ */
