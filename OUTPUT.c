/*
 * Test.c
 *
 * Created: 5/12/2019 12:19:41 PM
 * Author : Phantom
 */ 


#include "sam.h"
#define LED_PIN 0x08000000u		// external Ram				0000 1000 0000 0000 0000 0000 0000 
#define NLED_PIN 0xf7ffffffu	// Vendor-Specific-Device	1111 0111 1111 1111 1111 1111 1111 


int main(void)
{
    /* Initialize the SAM system */
	// It is Used to initialize the Microcontroller to full (84 MHz)
    SystemInit();							// A device-specific system configuration function
	
	/* 
	extern void WDT_Disable( Wdt* pWDT )
	{
		pWDT->WDT_MR = WDT_MR_WDDIS;
	}
	
	*/
	
	
	WDT->WDT_MR = WDT_MR_WDDIS;				// Disable the Watchdog Timer	 
	
	// PIOB is PORT B			(See page 633 for more register) 
	
	
	// Set the pin as OUTPUT  // we need to do the bit mask  
	/*
	pPio->PIO_OER = dwMask ;
	pPio->PIO_PER = dwMask ;
	*/
	
	PIOB->PIO_PER |=0x0A000000;		// Port I/O Enable Register 	 (write only)   WPEN is cleared 
	
	PIOB->PIO_OER |=LED_PIN;		// OUTPUT  Enable Register		 (Write Only)   to set the direction as OUTPUT 
	
	// 		
	PIOB->PIO_PUDR |=LED_PIN;		// pull_up Disable resistor  { pPio->PIO_PUDR = dwMask ; }  1== Diable 
		
	PIOB->PIO_OWER |=LED_PIN;		// Output Write Enable register to enable writing to ODSR directly 

    /* Replace with your application code */
    while (1) 
    {
		PIOB->PIO_ODSR|=LED_PIN;			// OUTPUT DATA STATUS REGISTER (used to drive data on the line ) 	
	    delay(1000);
		PIOB->PIO_ODSR&=NLED_PIN;  
		delay(1000);
		
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