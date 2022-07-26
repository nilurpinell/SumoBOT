/*
 * ADC_P.h
 *
 *  Created on: Apr 7, 2019
 *      Author: PCO
 */

#ifndef ADC_P_H_
#define ADC_P_H_

#include "derivative.h" /* include peripheral declarations */

void ADC0_init_p(void);
unsigned short ADC0_read_p(unsigned char ch);

#endif /* ADC_P_H_ */
