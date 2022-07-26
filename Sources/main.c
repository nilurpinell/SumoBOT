/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "derivative.h" /* include peripheral declarations */

#include "Drivers/UART/UART.h"
#include "Drivers/ADC/ADC_I.h"
//#include "Drivers/LED/LED_46.h"
#include "Drivers/LED/LED_25.h"
#include "Drivers/PWM/PWM.h"

#include "UT/IR_Test.h"
#include "UT/HC_Test.h"
#include "UT/PWM_Test.h"


#include "common.h"


//Les necesito per combat, no se si es posible tenirles...
#include <math.h> //arrel cuadrada
#include <time.h> //random
#include <stdlib.h> //random

void ajustarangledreta(){
	PWM_duty(-20000,20000);
	orientacio = orientacio -120;
	delayMs(DELAY120DRETA);
	PWM_duty(-20000,-20000);
}
void ajustarangleesquerra(){
	PWM_duty(20000,-20000); //esquerra
	orientacio = orientacio + 120;
	delayMs(DELAY120ESQUERRA);
	PWM_duty(-20000,-20000);
}
void ajustarcercledreta(int distancia){
	
	PWM_duty(-20000,20000);//dreta
	delayMs(DELAY90DRETA);
	orientacio = orientacio -90;
	PWM_duty(-(((MSPERCM)*(distancia-10))-MSDEMENYS10),-20000);
	posicio(distancia, orientacio);
	orientacio = 170;
	delayMs(DELAYCERCLE);
	PWM_duty(100,100);
}

void crear_mapa(){ //Dibuja un circulo de 1's que es el ring. Fuera y el limite son 0's.
	int y=0;
	int x=0;
	for (y=-77/2;y <= 77/2;y++)  {
	        for (x=-77/2;x <= 77/2;x++)  {
	 
	            if ((int) sqrt (pow(x,2)+pow(y,2)) >= 77/2){ 
	            	mapa2.mapa[x+77/2][y+77/2] = 0;
	            }else {
	            	mapa2.mapa[x+77/2][y+77/2] = 1;
	            }
	         }
	     }

	mapa2.ultimaposicioENEX = 77/2;
	mapa2.ultimaposicioENEY = 77/2+5;
	mapa2.mapa[77/2][(77/2)+5]=2;
	mapa2.ultimaposicioX = 77/2;
	mapa2.ultimaposicioY = 77/2-5;
	mapa2.mapa[77/2][(77/2)-5]=3;
}


void posicio(double distancia, int orientacio){
	int x,y;
	x = (int)(distancia)*sin((3.1415/180)*(orientacio));
	y = (int)(distancia)*cos((3.1415/180)*(orientacio));
	mapa2.ultimaposicioX = x+mapa2.ultimaposicioX;
	mapa2.ultimaposicioY = y+mapa2.ultimaposicioY;
}


void combat(){
	orientacio=90;
	char buffer[20];
	int gir = GIRINICIAL;
	srand(gir);
	int random = rand() %6; 
	double distancia = -1;
	crear_mapa();
	state='s', nextstate='s';
	WD_touch(5);
	
	
	ADC0_compare_i(13, 400, GT);
	
	
	for(;;) {
		if (alertstatus==0){
			switch (state){
				case 's': //Sensing
					//RLED_toggle();
					if (cm_PTA5>0||cm_PTC8>0||cm_PTC9>0){
						if (cm_PTA5<5){ //Davant
							//sprintf(buffer,"HC davant menor 5: %d", cm_PTA5);
							//UART0_send_string_ln(buffer);
							PWM_duty(-20000,-20000); //Carga cap endavant
							WD_touch(5);
						}else{
							if (cm_PTC9<5){ //dreta
								//GLED_toggle();
								//sprintf(buffer,"HC dreta menor 5: %d", cm_PTC9);
								//UART0_send_string_ln(buffer);
								PWM_duty(-20000,20000);
								WD_touch(5);
								orientacio = orientacio -120;
								delayMs(DELAY120DRETA);
								PWM_duty(-20000,-20000); //Carga cap endavant
								WD_touch(5);
							}else{
								if (cm_PTC8<5){ //esquerra
									//GLED_toggle();
									//sprintf(buffer,"HC esquerra menor 5: %d", cm_PTC8);
									//UART0_send_string_ln(buffer);
									PWM_duty(20000,-20000); //esquerra
									WD_touch(5);
									orientacio = orientacio + 120;
									delayMs(DELAY120ESQUERRA);
									PWM_duty(-20000,-20000); //Carga cap endavant
									WD_touch(5);
								}else{
									//BLED_toggle();	
									
									if (cm_PTA5<cm_PTC8) {
										if (cm_PTA5<cm_PTC9) {
											//	sprintf(buffer,"HC davant: %d", cm_PTA5);
											//UART0_send_string_ln(buffer);
											distancia = cm_PTA5;
											mapa2.mapa[mapa2.ultimaposicioENEX][mapa2.ultimaposicioENEY]=1;
											int eneX = ((int) (distancia)*sin((3.1415/180)*orientacio))+mapa2.ultimaposicioX;
											int eneY = ((int) (distancia)*cos((3.1415/180)*orientacio))+mapa2.ultimaposicioY;
											if((eneX<77) && (eneY<77) && (mapa2.mapa[eneX][eneY]>0))
												{
												//	sprintf(buffer,"HC davant dins mapa: %d", cm_PTA5);
												//UART0_send_string_ln(buffer);
													mapa2.ultimaposicioENEX = eneX;
													mapa2.ultimaposicioENEY = eneY;
													mapa2.mapa[mapa2.ultimaposicioENEX][mapa2.ultimaposicioENEY]=2;
												}else{
													distancia = -1;
												} 
										}else{
											distancia = cm_PTC9;
											//sprintf(buffer,"HC dreta: %d", cm_PTC9);
											//UART0_send_string_ln(buffer);
											mapa2.mapa[mapa2.ultimaposicioENEX][mapa2.ultimaposicioENEY]=1;
											int eneX = ((int) (distancia)*sin((3.1415/180)*orientacio))+mapa2.ultimaposicioX;
											int eneY = ((int) (distancia)*cos((3.1415/180)*orientacio))+mapa2.ultimaposicioY;
											if((eneX<77) && (eneY<77) && (mapa2.mapa[eneX][eneY]>0))
												{
													sprintf(buffer,"HC dreta dins mapa: %d", cm_PTC9);
													UART0_send_string_ln(buffer);
													mapa2.ultimaposicioENEX = eneX;
													mapa2.ultimaposicioENEY = eneY;
													mapa2.mapa[mapa2.ultimaposicioENEX][mapa2.ultimaposicioENEY]=2;
													PWM_duty(-20000,20000);
													WD_touch(5);
													orientacio = orientacio -120;
													delayMs(DELAY120DRETA);
												}else{
													distancia = -1;
												} 
										}
									}else{
										if (cm_PTC8<cm_PTC9) {
											distancia = cm_PTC8;
											//sprintf(buffer,"HC esquerra: %d", cm_PTC8);
											//UART0_send_string_ln(buffer);
											mapa2.mapa[mapa2.ultimaposicioENEX][mapa2.ultimaposicioENEY]=1;
											int eneX = ((int) (distancia)*sin((3.1415/180)*orientacio))+mapa2.ultimaposicioX;
											int eneY = ((int) (distancia)*cos((3.1415/180)*orientacio))+mapa2.ultimaposicioY;
											if((eneX<77) && (eneY<77) && (mapa2.mapa[eneX][eneY]>0))
												{
												//sprintf(buffer,"HC esquerra dins mapa: %d", cm_PTC8);
													//	UART0_send_string_ln(buffer);
													mapa2.ultimaposicioENEX = eneX;
													mapa2.ultimaposicioENEY = eneY;
													mapa2.mapa[mapa2.ultimaposicioENEX][mapa2.ultimaposicioENEY]=2;
													PWM_duty(20000,-20000); //esquerra
													WD_touch(5);
													orientacio = orientacio + 120;
													delayMs(DELAY120ESQUERRA);
												}else{
													distancia = -1;
												} 
										}else{
											distancia = cm_PTC9;
											//sprintf(buffer,"HC dreta: %d", cm_PTC9);
											//UART0_send_string_ln(buffer);
											mapa2.mapa[mapa2.ultimaposicioENEX][mapa2.ultimaposicioENEY]=1;
											int eneX = ((int) (distancia)*sin((3.1415/180)*orientacio))+mapa2.ultimaposicioX;
											int eneY = ((int) (distancia)*cos((3.1415/180)*orientacio))+mapa2.ultimaposicioY;
											if((eneX<77) && (eneY<77) && (mapa2.mapa[eneX][eneY]>0))
												{
												//	sprintf(buffer,"HC dreta dins mapa: %d", cm_PTC9);
												//UART0_send_string_ln(buffer);
													mapa2.ultimaposicioENEX = eneX;
													mapa2.ultimaposicioENEY = eneY;
													mapa2.mapa[mapa2.ultimaposicioENEX][mapa2.ultimaposicioENEY]=2;
													PWM_duty(-20000,20000);
													WD_touch(5);
													orientacio = orientacio -120;
													delayMs(DELAY120DRETA);
												}else{
													distancia = -1;
												} 
										}
									}
									if (distancia>5){
										//sprintf(buffer,"Distancia fora del mapa: %d", distancia);
										//UART0_send_string_ln(buffer);
										nextstate = 'c';
										break;
									}else{
										nextstate = 's';
									//BLED_toggle();	
										break;
									}
							
								}
								
							}
						}
						
						
						//girar per encarar depenen del UC que salti, fer un if de 3
						//distancia = cm_PT que salti
						//orientacio= orientacio +- angle!!! 
						
					}else{
						if (gir%2==0){ //dreta
							
							PWM_duty(-20000,20000); //dreta
							WD_touch(5);
							orientacio = orientacio - ANGLEGIR; 
							delayMs(MSGIR);
						}else{
							PWM_duty(20000,-20000); //esquerra
							WD_touch(5);
							orientacio = orientacio + ANGLEGIR;
							delayMs(MSGIR);
						}
						break;
						if (gir <=MAXGIR){
							gir= gir + 1;	
						}else{
							gir= GIRINICIAL+1;
							PWM_duty(-20000,-17500);
							delayMs(MOVIMENT);
							posicio(MOVIMENT*VELOCITAT, orientacio);
						}
						break;
						nextstate='s';
					}
					break;
					nextstate='s';
				case 'c':
					//GLED_toggle();
					PWM_duty(-20000,-17500);//endavant
					WD_touch(5);
					delayMs((distancia-3)/VELOCITAT);
					posicio(distancia-3, orientacio);
					nextstate= 's';
					random = rand() %6; 
					break;
					
					/*
					if (random>2){//endavant
						PWM_duty(-20000,-17500);//endavant
						WD_touch(5);
						delayMs((distancia-3)/VELOCITAT);
						posicio(distancia-3, orientacio);
						nextstate= 's';
						random = rand() %6; 
						break;
					}else{

						double x = (distancia/2)*sin((3.1415/180)*orientacio);
						double y = (distancia/2)*cos((3.1415/180)*orientacio);
						if (random>4){// gir per la dreta
							
							x = (distancia/2)*sin((3.1415/180)*(orientacio-90));
							y = (distancia/2)*cos((3.1415/180)*(orientacio-90));
							if((x+mapa2.ultimaposicioX<77) && (y+mapa2.ultimaposicioY<77) && (mapa2.mapa[(int)(x+mapa2.ultimaposicioX)][(int)(y+mapa2.ultimaposicioY)]>0))
							{
								PWM_duty(-20000,20000);//dreta
								WD_touch(5);
								delayMs(DELAY90DRETA);
								orientacio = orientacio -90;
								PWM_duty(-(((MSPERCM)*(distancia-10))-MSDEMENYS10),-20000);
								posicio(distancia, orientacio);
								orientacio = 170;
								WD_touch(5);
								delayMs(DELAYCERCLE);
								WD_touch(5);
								PWM_duty(100,100);
								
								
								nextstate= 's';
								random = rand() %6; 
								break;
							}else{
								PWM_duty(-20000,-17500);//endavant
								WD_touch(5);
								delayMs((distancia-3)/VELOCITAT);
								posicio(distancia-3, orientacio);
								nextstate= 's';
								random = rand() %6; 
								break;
							}
						} else{// gir per la esquerra
							x = (distancia/2)*sin((3.1415/180)*(orientacio+90));
							y = (distancia/2)*cos((3.1415/180)*(orientacio+90));
							if((x+mapa2.ultimaposicioX<77) && (y+mapa2.ultimaposicioY<77) && (mapa2.mapa[(int)(x+mapa2.ultimaposicioX)][(int)(y+mapa2.ultimaposicioY)]>0))
							{
								PWM_duty(20000,-20000);//esquerra
								WD_touch(5);
								delayMs(90);
								orientacio = orientacio +90;
								PWM_duty(-20000,-(((MSPERCM)*(distancia-10))-MSDEMENYS10));
								posicio(distancia, orientacio);
								orientacio = 10;
								WD_touch(5);
								delayMs(DELAYCERCLE);
								WD_touch(5);
								PWM_duty(100,100);
								
								
								nextstate= 's';
								random = rand() %6; 
								break;
							} else{
								PWM_duty(-20000,-17500);//endavant
								WD_touch(5);
								delayMs((distancia-3)/VELOCITAT);
								posicio(distancia-3, orientacio);
								nextstate= 's';
								random = rand() %6; 
								break;
							}
						}
						
					}
					
					*/
					
					 
				default: //per si de cas, torna al estat de sensing si hi ha estat desconegut
					state='s', nextstate='s';
					break;
			}
		state = nextstate;
		}
	}
}

void Test()
{	  
	char c;
	for(;;) {     

		UART0_send_string_ln("Select Test:");
		UART0_send_string_ln("'i': IR \t 'm': MOTORS \t 'u': ULTRASOUND\t 'x': EXIT TESTS");
		
		
		while(!(UART0_S1 & UART0_S1_RDRF_MASK)) { } /* 0x20 wait for receive buffer full */
		c = UART0_D; /* read the char received */
		
		switch (c){
		case 'i':
			RLED_toggle();
			if(IR_test())GLED_toggle();
			delayMs(50);
			GLED_off();
			RLED_toggle();
			break;
			
		case 'm':
			RLED_toggle();
			PWM_test();
			RLED_toggle();
			
			break;
			
		case 'u':
			HC_test();
			break;
			
		case 'x':
			return;
			break;
			
		case 'w':
			UART0_send_string_ln("Pulsar de nuevo para tocar al perrito.");
			WD_touch(5);
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
}

int main(void)
{

	
	RLED_init();
	GLED_init();
	BLED_init();
	UART0_init();
	PWM_init();
	HC_init();
	ADC0_init_i();
	
	
	//WD_touch(5); // Wathdog configurable a 5 s (ejemplo de uso)
	alertstatus = 0;
	RLED_toggle();
	//GLED_toggle();
	//WD5S_touch();
	//Test();
	//WD_touch(5);
	
	
	
	
	//BLED_on();
	/*
								//PWM   = distancia(cm)	= temps (s)
								//-3000 = 66 maxim.		= 4.97
								//-2500 = 57			= 2.64
								//-2000 = 48			= 2.88
								//-1500 = 39            = 2.57
								//-1000 = 30			= 2.51
								//-500  = 19			= 2.50
								//0		= 10 minim.
								PWM_duty(20000,-20000);//esquerra
								WD5S_touch();
								delayMs(90);
								PWM_duty(-20000,-1000);
								delayMs(1200);
								PWM_duty(0,0);*/
	
	
	
	
	//ajustar 5s
	delayMs(CINCSEC);
	BLED_toggle();	
	
	//ajustarangledreta();
	//ajustarangleesquerra();	
	//ajustarcercledreta(30);
	//ADC0_compare_i(13, 400, GT);
	
	for(;;) {     
		combat();
	}
	return 0;
}


