/*
 * common.h
 *
 *  Created on: Apr 29, 2019
 *      Author: PCO
 */

#ifndef COMMON_H_
#define COMMON_H_

#include "MKL25Z4.h"
#include "Drivers/LED/LED_25.h"

#define HC_TPM0_MOD 60	// ms
#define WD(ms)	((int)(((int)(ms))/HC_TPM0_MOD))
#define WD_5S	5000 	// 5 seg


#define DELAY90ESQUERRA 135
#define DELAY120ESQUERRA 180
#define VELOCITAT 0.16
#define DELAY90DRETA 158
#define DELAY120DRETA 210
#define MOVIMENT 100
#define DELAYDEMESDRETA 1.16666
#define MSPERCM 500/9
#define MSDEMENYS10 111.11111
#define MAXGIR 10
#define GIRINICIAL 1
#define MSINTER 7000




//AQUESTS VAN JUNTS, SI MODIFIQUES UN, MODIFICA ELS ALTRES INVERSAMENT (SI INCREMENTES MS EN *3, DIVIDIR CINCSEC EN 3 Y DELAYCERCLE EN 3)
#define MS 3000 //PER ELS DELAYS DE 90 I 120 TOCAR AQUEST
#define CINCSEC 1500 
#define DELAYCERCLE 833
//AQUESTES DUES VAN JUNTES IGUAL QUE LES ANTERIORS, RELACIONADES AMB MS
#define ANGLEGIR 10
#define MSGIR 15


void delayMs(int);
void WD_touch(int);
void WD5S_touch(void);
void SysTick_Handler(void);


volatile int alertstatus;
volatile int orientacio;
volatile char state, nextstate;

struct mapa{
	char mapa[77][77];
	int ultimaposicioX;
	int ultimaposicioY;
	int ultimaposicioENEX;
	int ultimaposicioENEY;
};
typedef struct mapa mapa1;
mapa1 mapa2;
/** TODO
 * Nil necesita una función que se ejecute cada 5 segundos, que vaya con otra que cuando se llame actue como watchdog reiniciando los 5 segundos
 * Y otra función que al llamarla se le pase como argumento un tiempo y 
 * sin bloquear te interrumpa cuando transcurra ese tiempo. Si se llama otra vez antes de la interrupción programada debería reestablecer el contandor anulando la primera llamada.
 */



#endif /* COMMON_H_ */
