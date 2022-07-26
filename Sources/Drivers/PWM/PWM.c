
#include "PWM.h"

void PWM_init() {

	
	MCG_BASE_PTR->C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK; //  INTERNAL CLOCK|MCGIRCLK ACTIVE(SET)
	MCG_BASE_PTR->C2 = MCG_C2_IRCS_MASK;   // SELECT FAST INTERNAL REFERENCE CLOCK (1)
	SIM_BASE_PTR->SCGC6 |= SIM_SCGC6_TPM1_MASK;  // ENABLE TPM1 CLOCK GATE
	SIM_BASE_PTR->SCGC6 |= SIM_SCGC6_TPM2_MASK;  // ENABLE TPM2 CLOCK GATE
	
	SIM_BASE_PTR->SOPT2 |= SIM_SOPT2_TPMSRC(3);  // MCGIRCLK IS SELECTED FOR TPM CLOCK
	
	TPM1_BASE_PTR->SC = 0;
	TPM2_BASE_PTR->SC = 0;
	
	TPM1_BASE_PTR->SC |= TPM_SC_PS(1);  // Timer 1
	TPM1_BASE_PTR->SC |= TPM_SC_CMOD(1);
	TPM1_BASE_PTR->MOD = 20000; //Valor maxim del contador
	
	TPM2_BASE_PTR->SC |= TPM_SC_PS(1);  // Timer 2
	TPM2_BASE_PTR->SC |= TPM_SC_CMOD(1);
	TPM2_BASE_PTR->MOD = 20000; //Valor maxim del contador
	
	
	SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTE_MASK; //Activar el clock al port E
	
	PORTE_BASE_PTR->PCR[20] = PORT_PCR_MUX(3);  //Pin 20 del port E multiplexor opcio 3 (TPM1_CH0)
	PORTE_BASE_PTR->PCR[21] = PORT_PCR_MUX(3);  //Pin 21 del port E multiplexor opcio 3 (TPM1_CH1)
	PORTE_BASE_PTR->PCR[22] = PORT_PCR_MUX(3);  // Pin 22 del port E multiplexor opcio 3 (TPM2_CH0)
	PORTE_BASE_PTR->PCR[23] = PORT_PCR_MUX(3);  // Pin 23 del port E multiplexor opcio 3 (TPM2_CH1)
	
	TPM1_BASE_PTR->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // Canal seleccionat
	TPM1_BASE_PTR->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // Canal seleccionat
	TPM2_BASE_PTR->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // Canal seleccionat
	TPM2_BASE_PTR->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // Canal seleccionat
	
	TPM1_BASE_PTR->SC |= 0x08;
	TPM2_BASE_PTR->SC |= 0x08;
}

void PWM_duty(int motor1, int motor2){
	if(motor1>0){
		TPM1_BASE_PTR->CONTROLS[0].CnV = motor1; //Duty cycle en ms
		TPM1_BASE_PTR->CONTROLS[1].CnV = 0;
	}
	else if(motor1<0){
		TPM1_BASE_PTR->CONTROLS[1].CnV = -motor1;
		TPM1_BASE_PTR->CONTROLS[0].CnV = 0;
	}
	else{
		TPM1_BASE_PTR->CONTROLS[0].CnV = 0;
		TPM1_BASE_PTR->CONTROLS[1].CnV = 0;
	}
	
	if(motor2>0){
		TPM2_BASE_PTR->CONTROLS[0].CnV = 0;
		TPM2_BASE_PTR->CONTROLS[1].CnV = motor2; //Duty cycle en ms
	}
	else if(motor2<0) {
		TPM2_BASE_PTR->CONTROLS[0].CnV = -motor2;
		TPM2_BASE_PTR->CONTROLS[1].CnV = 0;
	}
	else{
		TPM2_BASE_PTR->CONTROLS[0].CnV = 0;
		TPM2_BASE_PTR->CONTROLS[1].CnV = 0;
	}
}
