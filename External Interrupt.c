/*
 * Test.c
 * Created: 5/12/2019 12:19:41 PM
 * Author : Phantom
 */ 
#include "sam.h"
#define LED_PIN 0x08000000u		// external Ram				0000 1010 0000 0000 0000 0000 0000 
#define NLED_PIN 0x07000000u	// Vendor-Specific-Device	1111 0111 1111 1111 1111 1111 1111 

volatile long i = 0 ; 

void Setup_Peripheral(void)
{
	
	NVIC_DisableIRQ(PIOB_IRQn);
	NVIC_ClearPendingIRQ(PIOB_IRQn);
	NVIC_SetPriority(PIOB_IRQn, 0);
	NVIC_EnableIRQ(PIOB_IRQn);
	
	// configure external interrupt
	
	// Enable pin PIOB B_25 pin 2
	PIOB->PIO_IER|=PIO_IER_P25 ;
	
	// Edge/Level select Register
	PIOB->PIO_ESR|=PIO_ESR_P25 ;
	
	// RAISING EDGE select
	PIOB->PIO_REHLSR|=PIO_REHLSR_P25;
}





void PIOB_Handler()   { 
	
	
	if((PIOB->PIO_ISR) & (PIO_ISR_P25)){
		//PIOB->PIO_ODSR|=0x08000000u;   // OUTPUT DATA STATUS REGISTER (used to drive data on the line
		i++;
	}
	
	 
	// Disable pin { PIOB->PIO_ODSR&=0xf7ffffffu	;}
	//	PIOB->PIO_ODSR&=0xf7ffffffu	;
}



int main(void)
{
    /* Initialize the SAM system */
	// It is Used to initialize the Microcontroller to full (84 MHz)
    SystemInit();							// A device-specific system configuration function
	
	// PIO PARALLER INPUT/OUTPUT 
	 
	WDT->WDT_MR = WDT_MR_WDDIS;				// Disable the Watchdog Timer	
	
	// PIOB is PORT B			(See page 633 for more register) 
	
	// Set the pin as OUTPUT  // we need to do the bit mask  
	/*
	pPio->PIO_OER = dwMask ;
	pPio->PIO_PER = dwMask ;
	*/
	
	PIOB->PIO_PER |=0x0A000000u;		// Port I/O Enable Register 	 (write only) 	 Enable B_25 (pin 2) &  B_27 (pin 13) 
	
	// Clock is required for input reading
		PMC->PMC_PCER0 =(1<<12);				//Enable Peripheral Clock PMC_PCER1
		
	// Input Pin
		PIOB->PIO_ODR  |=0x02000000u;		// Output disable Register		 Disable B_25 (pin 2)
		PIOB->PIO_PUER |=0x02000000u;		// pull_up enable Register		 B_25 (pin 2)
		PIOB->PIO_OWDR |=0x02000000u;		// Output Disable Register		B_25 (pin 2)
		
		Setup_Peripheral(); 
		
	// Output Pin 
		PIOB->PIO_OER |=0x08000000u;		// OUTPUT  Enable Register		 (Write Only)   to set the direction as OUTPUT of B_27 (pin 13) 
		PIOB->PIO_PUDR |=0x08000000u;		// pull_up Disable resistor		 { pPio->PIO_PUDR = dwMask ; }  1== Diable   of B_27 (pin 13) 
		PIOB->PIO_OWER |=0x08000000u;		// Output Write Enable register to enable writing to ODSR directly  of B_27 (pin 13) 	
	
	
	//PIOB->PIO_ODSR|=LED_PIN;			// OUTPUT DATA STATUS REGISTER (used to drive data on the line )
	
	// Disable pin { PIOB->PIO_ODSR&=0xf7ffffffu	;}
	  PIOB->PIO_ODSR&=0xf7ffffffu	;

		
	
	
    /* Replace with your application code */
    while (1) 
    {
		// Disable pin { PIOB->PIO_ODSR&=0xf7ffffffu	;}
			
			
		if(i%12600==0)
		{
		   PIOB->PIO_ODSR|=0x08000000u;   
		}	
		else {
		   PIOB->PIO_ODSR&=0xf7ffffffu	;
		}
		
			
		/*if(i==0){
		   PIOB->PIO_ODSR&=0xf7ffffffu	;
		   delay(10000);
		}
		else 
		 {  PIOB->PIO_ODSR|=0x08000000u;   // OUTPUT DATA STATUS REGISTER (used to drive data on the line
		  delay(10000);}*/
		   
			
	}
	
	return 0 ; 
  }

void delay (unsigned long int n)
{
	unsigned long int i=0,j=0;
	for(i=0;i<=1000;i++)
	{
		for(j=0;j<=n;j++)
		{
			asm("nop");
		}
	}
	
}























// void PIOB_Handler ( void ); 
/*void PIOB_Handler() {

	// #define PIO_ISR_P27 (0x1u << 27) < \brief (PIO_ISR) Input Change Interrupt Status 
	
	if (PIOB -> PIO_ISR & PIO_ISR_P27) {
	}
*/

/* pmc_enable_periph_clk(ID_PIOA);
 pio_set_input(PIOA, PIO_PA29, PIO_PULLUP;
 pio_handler_set(PIOA, ID_PIOC, PIO_PA29, PIO_IT_EDGE, int_RINGMODULATOR);
 pio_enable_interrupt(PIOA, PIO_PA29);
 NVIC_EnableIRQ(PIOA_IRQn);*/
 

 /*asm("NOP") ; 
 SysTick_Config
 SystemCoreClock*/
 
 // sysclk_enable_peripheral_clock