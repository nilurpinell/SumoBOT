/*
 * PWM_Test.c
 *
 *  Created on: Apr 29, 2019
 *      Author: PCO
 */
#include "PWM_Test.h"

int PWM_test(void)
{
	char c;
		for(;;) {     

			UART0_send_string_ln("Select Mode:");
			UART0_send_string_ln("'w': Endavant \t 'a': esquerra \t 's': Enrrere\t 'd' dreta\t 'x': EXIT TESTS");
			
			
			while(!(UART0_S1 & UART0_S1_RDRF_MASK)) { } /* 0x20 wait for receive buffer full */
			c = UART0_D; /* read the char received */
			
			switch (c){
			case 'w':
				PWM_duty(-20000,-20000);//endavant
				break;
				
			case 'a':
				PWM_duty(20000,-20000);//esquerra
				break;
				
			case 's':
				PWM_duty(20000,20000);//endarrera
				break;
			case 'd':
				PWM_duty(-20000,20000);//dreta
				break;
							
			case 'x':
				PWM_duty(0,0);//parar
				return 0;
				break;
			
			default:
				UART0_send_ln();
				UART0_send_string("\t\033[37;1;5m");
				UART0_send_char(c);
				UART0_send_string_ln("\033[0m\033[31m IS NOT AN OPTION!\033[0m");
				UART0_send_ln();
				break;
			}
		}
	return 0;
}
