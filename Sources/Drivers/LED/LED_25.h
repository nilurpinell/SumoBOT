/*
 * LED_25.h
 *
 *  Created on: Apr 27, 2019
 *      Author: Dani
 */

#ifndef LED_25_H_
#define LED_25_H_

#include "derivative.h" /* include peripheral declarations */

void RLED_init(void);
void RLED_toggle(void);

void GLED_init(void);
void GLED_toggle(void);
void GLED_off(void);

void BLED_init(void);
void BLED_toggle(void);
void BLED_on(void);
void BLED_off(void);

#endif /* LED_25_H_ */
