/*
 * UART.c
 *
 *  Created on: Apr 7, 2019
 *      Author: PCO
 */

#include "UART.h"

/* initialize UART0 to transmit at 115200 Baud */
void UART0_init(void) {
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK; /* enable clock for UART0 */
	SIM_SOPT2 |= SIM_SOPT4_TPM2CLKSEL_MASK; /* 0x04000000 use FLL output for UART Baud rate generator
	*/
	UART0_C2 = 0; /* turn off UART0 while changing configurations */
	UART0_BDH = 0x00;
	UART0_BDL = UART0_BDL_SBR(12); /* 115200 Baud */
	UART0_C4 = 0x0F; /* Over Sampling Ratio 16 */
	UART0_C1 = 0x00; /* 8-bit data */
	UART0_C2 = UART0_C2_TE_MASK | UART0_C2_RE_MASK; /*0x08 enable transmit and receive 0x04 */
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; /* enable clock for PORTA */
	
	PORTA_PCR2 = 0x0200; /* make PTA2 UART0_Tx pin */
	PORTA_PCR1 = 0x0200;  /* make PTA1 UART0_Rx pin */
}
 
void UART0_send_ln(void){
	while(!(UART0_S1 & UART0_S1_TDRE_MASK)) { }
	UART0_D = '\n'; /* send a char */
	while(!(UART0_S1 & UART0_S1_TDRE_MASK)) { }
	UART0_D = '\r'; /* send a char */
}

void UART0_send_string_ln(char *buff){
	int i = 0;
	while(buff[i]!='\0'){
		while(!(UART0_S1 & UART0_S1_TDRE_MASK)) { } /* 0x80 wait for transmit buffer empty */
		UART0_D = buff[i]; /* send a char */
		i++;
	}
	UART0_send_ln();
}

void UART0_send_string(char *buff){
	int i = 0;
	while(buff[i]!='\0'){
		while(!(UART0_S1 & UART0_S1_TDRE_MASK)) { } /* 0x80 wait for transmit buffer empty */
		UART0_D = buff[i]; /* send a char */
		i++;
	}
}

void UART0_send_char(char c){
	while(!(UART0_S1 & UART0_S1_TDRE_MASK)) { } /* 0x80 wait for transmit buffer empty */
	UART0_D = c; /* send a char */
}

/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert n to characters in s */
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}
