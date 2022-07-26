/*
 * LED_46.c
 *
 *  Created on: Apr 7, 2019
 *      Author: PCO
 */
/*
#include "LED_46.h"

void RLED_init(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE_PCR29 = PORT_PCR_MUX(1);     
	GPIOE_PDDR |= (1 << 29);
	GPIOE_PSOR |= (1 << 29);
}

void GLED_init(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; // PORT D mask connected to Green LED
	PORTD_PCR5 = PORT_PCR_MUX(1);     
    GPIOD_PDDR |= (1 << 5);
	GPIOD_PSOR |= (1 << 5);
}

void RLED_toggle(void)
{
	GPIOE_PTOR |= (1 << 29);
}

void GLED_toggle(void)
{
	GPIOD_PTOR |= (1 << 5);
}
void GLED_off(void)
{
	GPIOD_PDOR |= (1 << 5);
}
*/
