/*
 * ADC_P.c
 *
 *  Created on: Apr 7, 2019
 *      Author: PCO
 */

#include "ADC_P.h"

void ADC0_init_p(void)
{
	
	// Enable clocks
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;	// ADC 0 clock
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;	// PTB clock
	//SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;	// PTB0 clock
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;	// PTC clock
	
	//Set Inputs (not necessary)
	PORTB_PCR0 = 0; // PTB0 analog input */
	PORTB_PCR1 = 0; // PTB1 analog input */
	PORTB_PCR2 = 0; // PTB2 analog input */
	PORTB_PCR3 = 0; // PTB3 analog input */
	//PORTE_PCR20 = 0; // PTB3 analog input */
	PORTC_PCR2 = 0; // PTB3 analog input */

	// Configure ADC
	ADC0_CFG1 = 0; // Reset register
	ADC0_CFG1 |= (ADC_CFG1_MODE(2)  |  	// 10 bits mode
				  ADC_CFG1_ADICLK(0)|	// Input Bus Clock (20-25 MHz out of reset (FEI mode))
				  ADC_CFG1_ADIV(2)) ;	// Clock divide by 2 (10-12.5 MHz)
		
		
	ADC0_SC2 &= ~0x40; /* software trigger */
}
 
unsigned short ADC0_read_p(unsigned char ch)
{
	
	ADC0_SC1A = (ch & ADC_SC1_ADCH_MASK);     // Write to SC1A to start conversion
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); 	 // Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Run until the conversion is complete
	return ADC0_RA;
}
