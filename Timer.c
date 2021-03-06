/* Includes -------------------------------------------------------------------*/
#include "sam.h"

/* Variables ------------------------------------------------------------------*/

#define LED_PIN 1u<<23		// external Ram				0000 1000 0000 0000 0000 0000 0000

// #define NLED_PIN 0xf7ffffffu	// Vendor-Specific-Device	1111 0111 1111 1111 1111 1111 1111
volatile int h =0      ;
volatile long dummyA   ;
volatile long dummyB   ;
volatile long value_cv ;
/* Prototypes -----------------------------------------------------------------*/
static void intialize_Timer (void) ;

// static void delay_ms(uint32_t time_ms);

int main(void) {


	/* Initialize the SAM system */
	// It is Used to initialize the Microcontroller to full (84 MHz)
	SystemInit();							// A device-specific system configuration function
	
	// Disable watchdog timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	intialize_Timer();
	
	//TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_WAVSEL_UP_RC | TC_CMR_ACPA_TOGGLE;

	
	//PIOB->PIO_PER  |=0x0A000000;		// Port I/O Enable Register 	 (write only)   WPEN is cleared
	//PIOB->PIO_OER  |=LED_PIN;		// OUTPUT  Enable Register		 (Write Only)   to set the direction as OUTPUT
	//PIOB->PIO_PUDR |=LED_PIN;		// pull_up Disable resistor  { pPio->PIO_PUDR = dwMask ; }  1== Diable
	//PIOB->PIO_OWER |=LED_PIN;		// Output Write Enable register to enable writing to ODSR directly
	
	PIOC->PIO_PER  |=LED_PIN;		// Port I/O Enable Register 	 (write only)   WPEN is cleared
	PIOC->PIO_OER  |=LED_PIN;		// OUTPUT  Enable Register		 (Write Only)   to set the direction as OUTPUT
	PIOC->PIO_PUDR |=LED_PIN;		// pull_up Disable resistor  { pPio->PIO_PUDR = dwMask ; }  1== Diable
	PIOC->PIO_OWER |=LED_PIN;		// Output Write Enable register to enable writing to ODSR directly
	
	
	// PIOB->PIO_ODSR|=LED_PIN;
	PIOC->PIO_ODSR&=0xff7fffff;

	
	
	
	while (1) {
		
	}

}




void TC0_Handler()
{
	dummyA = TC_SR_CPAS   ;
	value_cv = TC0->TC_CHANNEL[0].TC_CV ;
	dummyB = TC0->TC_CHANNEL[0].TC_SR;
	if(h==0) PIOC->PIO_ODSR|=LED_PIN ;
	else     PIOC->PIO_ODSR&=0xff7fffff;
	h=!h ; 
	TC0->TC_CHANNEL[0].TC_CCR|= TC_CCR_SWTRG ;
}

void intialize_Timer (void)
{
	// Enable the PMC for Timer 0
	//	PMC->PMC_PCER0 |= ID_TC0 ;
	REG_PMC_PCER0 |= PMC_PCER0_PID27;
	//		pmc_enable_periph_clk(ID_TC0);
	
	//Disable PIO Control on PB25 and set up for Peripheral B TIOA0
	//   TC0   TIOA0    PB25    B
	PIOB->PIO_PDR  |= PIO_PB25 ;    // Checked
	PIOB->PIO_ODR  |= PIO_PB25 ;	// Checked	     // OUTPUT  Enable Register		 (Write Only)   to set the direction  as OUTPUT
	PIOB->PIO_ABSR |= PIO_PB25 ;  	// Checked 	// Set PWM pin peripheral type A or B, in this case B ( Timer 0 )
	PIOB->PIO_PUDR |= PIO_PB25;		// Checked // pull_up Disable resistor  { pPio->PIO_PUDR = dwMask ; }  1== Diable
	
	// PAGE 634 PDR register
	// PDR is 0 (no effect) & PDR is 1 (enable the peripheral and disable the PIO controller)
	//NVIC_DisableIRQ(TC0_IRQn);
	//NVIC_ClearPendingIRQ(TC0_IRQn);
	//NVIC_SetPriority(TC0_IRQn, 1);
	NVIC_EnableIRQ(TC0_IRQn); // enable TC0 interrupts
	
	//		TC0->TC_WPMR|= 0x54494D00 ;  // enable write to registers
	
	//REG_TC0_WPMR = 0x54494D00;  // enable write to registers
	TC0->TC_CHANNEL[0].TC_IER|= TC_IER_COVFS|TC_IER_CPCS ;
	TC0->TC_CHANNEL[0].TC_CCR|= TC_CCR_CLKEN |TC_CCR_SWTRG ;
	TC0->TC_CHANNEL[0].TC_CMR|= TC_CMR_TCCLKS_TIMER_CLOCK1|TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC|TC_CMR_EEVTEDG_RISING ;
	// TC0->TC_CHANNEL[0].TC_RA |= 0X00000001;
	// TC0->TC_CHANNEL[0].TC_RB |= 0X00000001;
	TC0->TC_CHANNEL[0].TC_RC |= 420000;
	
	
}
