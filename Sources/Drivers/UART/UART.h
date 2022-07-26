/*
 * UART.h
 *
 *  Created on: Apr 7, 2019
 *      Author: PCO
 */

#ifndef UART_H_
#define UART_H_

#include "derivative.h" /* include peripheral declarations */
#include <string.h>

void UART0_init(void);
void UART0_send_string_ln(char *buff);
void UART0_send_string(char *buff);
void UART0_send_char(char c);
void UART0_send_ln(void);
void itoa(int n, char s[]);

#endif /* UART_H_ */
