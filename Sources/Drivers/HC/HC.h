/*
 * HC.h
 *
 *  Created on: Apr 27, 2019
 *      Author: Dani
 */

#ifndef HC_H_
#define HC_H_

#include "derivative.h" /* include peripheral declarations */
#include "../../common.h"

extern volatile int cm_PTA5, cm_PTC8, cm_PTC9;

void HC_init(void);
void FTM0_IRQHandler() __attribute__((interrupt("TPM0_IRQ")));

#endif /* HC_H_ */
