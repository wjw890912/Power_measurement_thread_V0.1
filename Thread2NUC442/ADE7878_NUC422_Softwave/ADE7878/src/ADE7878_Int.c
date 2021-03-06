


#include <stdio.h>
#include "NUC472_442.h"
#include "gpio.h"
#include "ade7878_Int.h" 
#include "ADE7878.h"
#include "ADE7878_SPI.h"
extern unsigned char IRQFlag ;
void Config_ADE7878_IRQ(void)
{
 /*  Configure PB4-5 as INPUT mode and enable interrupt by falling edge trigger */
    GPIO_SetMode(PB, BIT5,  GPIO_MODE_INPUT);
    GPIO_EnableInt(PB, 4, GPIO_INT_FALLING); //IRQ1
	GPIO_EnableInt(PB, 5, GPIO_INT_FALLING); //IRQ0
	NVIC_SetPriority(GPB_IRQn, 5 );
    NVIC_EnableIRQ(GPB_IRQn);


}

void GPB_IRQHandler(void)
{

    /* To check if PB4 interrupt occurred */
    if (PB->INTSRC & BIT4) 
	{
        PB->INTSRC = BIT4;
    
		 IRQFlag=1;//IRQ1 is occrued .
									  

    } else 
	/* To check if PB5 interrupt occurred */
	if(PB->INTSRC & BIT5)
	{	
		PB->INTSRC = BIT5;
	   	IRQFlag=1;//IRQ0 is occrued .
	}
	else
	{  
      PB->INTSRC = PB->INTSRC ;  
    }


}
