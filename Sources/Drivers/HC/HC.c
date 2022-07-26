#include "MKL25Z4.h"
#include "HC.h"


volatile int then_PTA5 = 0, then_PTC8 = 0, then_PTC9 = 0;
volatile int now_PTA5 = 0, now_PTC8= 0, now_PTC9 = 0;
volatile int cm_PTA5 = 0 , cm_PTC8 = 0 , cm_PTC9 = 0 ;

void HC_init()
{
	// Configurar reloj interno desde 2 MHz a 25 Hz -> 60 ms con un duty de 11 us para el trigger
	MCG_BASE_PTR->C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK; //  INTERNAL CLOCK|MCGIRCLK ACTIVE(SET)
	MCG_BASE_PTR->C2 = MCG_C2_IRCS_MASK;   // SELECT FAST INTERNAL REFERENCE CLOCK (1)
	SIM_BASE_PTR->SCGC6 |= SIM_SCGC6_TPM0_MASK;  // ENABLE TPM0 CLOCK GATE
	SIM_BASE_PTR->SOPT2 |= SIM_SOPT2_TPMSRC(3);  // MCGIRCLK IS SELECTED FOR TPM CLOCK
	TPM0_BASE_PTR->SC = 0;
	TPM0_BASE_PTR->SC |= TPM_SC_PS(1);  // * especificar frequencia
	TPM0_BASE_PTR->SC |= TPM_SC_CMOD(1);  // COUNTER INC. ON EVERY CLOCK
	TPM0_BASE_PTR->MOD = HC_TPM0_MOD*1000;  // * especificar periodo 60 ms
	
	// Clock to PortA and PortC
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK;
	
	// PTA4 salida a trigger conectado con TPM0_CH1
	TPM0_C1SC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // SELECT CHANNEL MODE
	TPM0_C1V = 11;  // * especificar duty cycle 11 us
	PORTA_PCR4 = PORT_PCR_MUX(3);  // * especificar multiplexacio del TPM0_CH1 Alternativa 3
	
	// Configuracion de la recepcion de echo
	// PTA5 TPM0 CHANNEL 2
	TPM0_C2SC =  TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK; // Capture on both edges
	TPM0_C2SC &= ~(TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);// Capture on both edges
	PORTA_PCR5 = PORT_PCR_MUX(3);  // * especificar multiplexacio del TPM0_CH2 Alternativa 3
	TPM0_C2SC |= TPM_CnSC_CHIE_MASK; // Enable int on channel
	
	// PTC8 TPM0 CHANNEL 4
	TPM0_C4SC =  TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK; // Capture on both edges
	TPM0_C4SC &= ~(TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);// Capture on both edges
	PORTC_PCR8 = PORT_PCR_MUX(3);  // * especificar multiplexacio del TPM0_CH4 Alternativa 3
	TPM0_C4SC |= TPM_CnSC_CHIE_MASK; // Enable int on channel
	
	// PTC9 TPM0 CHANNEL 5
	TPM0_C5SC =  TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK; // Capture on both edges
	TPM0_C5SC &= ~(TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK);// Capture on both edges
	PORTC_PCR9 = PORT_PCR_MUX(3);  // * especificar multiplexacio del TPM0_CH5 Alternativa 3
	TPM0_C5SC |= TPM_CnSC_CHIE_MASK; // Enable int on channel

	NVIC_BASE_PTR -> ICPR |= 1 << (INT_TPM0 - 16); //Borrar peticiones de interrupción anteriores:
	NVIC_BASE_PTR -> ISER |= 1 << (INT_TPM0 - 16); //Habilitar la interrupción (activación global, en el NVIC)
	TPM0_BASE_PTR->SC |= 0x08;
}


void FTM0_IRQHandler(void) {
	if (TPM0_STATUS & (1<<2))
	{ // INT PTA5 TPM0 CHANNEL 2
		then_PTA5 = now_PTA5;
		now_PTA5 = TPM0_C2V;
		cm_PTA5 = (now_PTA5 - then_PTA5) / 58;
		TPM0_C2SC |= 0x80; // clear CHF
	}
	else if (TPM0_STATUS & (1<<4))
	{ // INT PTC8 TPM0 CHANNEL 4
		then_PTC8 = now_PTC8;
		now_PTC8 = TPM0_C4V;
		cm_PTC8 = (now_PTC8 - then_PTC8) / 58;
		TPM0_C4SC |= 0x80; // clear CHF
	}
	else if (TPM0_STATUS & (1<<5))
	{ // INT PTC9 TPM0 CHANNEL 5
		then_PTC9 = now_PTC9;
		now_PTC9 = TPM0_C5V;
		cm_PTC9 = (now_PTC9 - then_PTC9) / 58;
		TPM0_C5SC |= 0x80; // clear CHF
	}
}


