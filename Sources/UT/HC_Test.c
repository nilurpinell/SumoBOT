/*
 * HC_Test.c
 *
 *  Created on: Apr 29, 2019
 *      Author: PCO
 */
#include "HC_Test.h"

int HC_test(void)
{
	char buffer[20];
	UART0_send_string_ln("Start HC test:");
	int test_PTA5 = 0, test_PTC8 = 0, test_PTC9 = 0;
	while(!test_PTA5 || !test_PTC8 || !test_PTC9) {
		delayMs(60);
		if (!test_PTA5) {
			if (cm_PTA5>0)
			{
				sprintf(buffer,"cm_PTA5: %d", cm_PTA5);
				UART0_send_string_ln(buffer);
				if (cm_PTA5 == 5)
				{
					test_PTA5 = 1;
					UART0_send_string_ln("\033[32mTEST PTA5 OK!!\033[0m");
				}
			}
		} else UART0_send_string_ln("\033[32mTEST PTA5 OK!!\033[0m");
		if (!test_PTC8) {
			if (cm_PTC8>0)
			{
				sprintf(buffer,"cm_PTC8: %d", cm_PTC8);
				UART0_send_string_ln(buffer);
				if (cm_PTC8 == 5)
				{
					test_PTC8 = 1;
					UART0_send_string_ln("\033[32mTEST PTC8 OK!!\033[0m");
				}
			}
		} else UART0_send_string_ln("\033[32mTEST PTC8 OK!!\033[0m");
		if (!test_PTC9) {
			if (cm_PTC9>0)
			{
				sprintf(buffer,"cm_PTC9: %d", cm_PTC9);
				UART0_send_string_ln(buffer);
				if (cm_PTC9 == 5)
				{
					test_PTC9 = 1;
					UART0_send_string_ln("\033[32mTEST PTC9 OK!!\033[0m");
				}
			}
		} else UART0_send_string_ln("\033[32mTEST PTC9 OK!!\033[0m"); 
		if ((cm_PTA5 > 1 && cm_PTA5 < 10) || (cm_PTC8 > 1 && cm_PTC8 < 11) || (cm_PTC9 > 1 && cm_PTC9 < 11))
			BLED_on();
		else 
			BLED_off();
	}
	BLED_off();
	return 0;
}
