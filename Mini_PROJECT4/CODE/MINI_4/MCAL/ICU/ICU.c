/******************************************************************************
 *
 * Module: ICU
 *
 * File Name: icu.c
 *
 * Description: Source file for the AVR ICU driver
 *
 * Author: Shirehan Medaht
 *
 *******************************************************************************/
#include "ICU.h"
#include "../../STD_LIBRARIES/common_macros.h" /* To use the macros like SET_BIT */
#include "../GPIO/GPIO.h"
#include <avr/io.h>        /* To use the IO Ports Registers */
#include <avr/interrupt.h> /* For ICU ISR */
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*PTR_TO_CALL_ISR_BACK) (void) = NULL_PTR;
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/



ISR ( TIMER1_CAPT_vect){

	if (PTR_TO_CALL_ISR_BACK != NULL_PTR)
	{
		(*PTR_TO_CALL_ISR_BACK)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */

void Icu_init(const Icu_ConfigType * Config_Ptr)
{
	   /* Configure ICP1/PD6 as i/p pin */
		GPIO_setupPinDirection (PORTD_ID ,PIN6_ID ,PIN_INPUT);
		/* Timer1 always operates in Normal Mode */
		TCCR1A = (1<<FOC1A) | (1<<FOC1B);
		/*INSERTION "DYNAMIC CONFIG"*/
		/*CLOCK SELECTION*/
		TCCR1B = (TCCR1B & 0xF8)|((Config_Ptr->CLOCK) & 0x07);
		/*EDGE SELECTION*/
		/*NO need for & 0x40 but I put it to make it safe  */
		TCCR1B = (TCCR1B & 0xBF)|(((Config_Ptr->EDGE_SELECT)<<6) & 0x40 );
		/* Initial Value for Timer1 */
		TCNT1=0;
		/* Initial Value for the input capture register */
		ICR1 =0;
		/* Enable the Input Capture interrupt to generate an interrupt when edge is detected on ICP1/PD6 pin */
		//SET_BIT (TIMSK , TICIE1);
		TIMSK |= (1<<TICIE1);

}
/*
 * Description: Function to set the Call Back function address.
 */
void Icu_setCallBack(void(*a_ptr)(void))
{
	PTR_TO_CALL_ISR_BACK = a_ptr;
}
/*
 * Description: Function to set the required edge detection.
 */
void Icu_setEdgeDetectionType(const Icu_EdgeType edgeType)
{
	TCCR1B = (TCCR1B & 0xBF)|((edgeType<<6) & 0x40 );
}
/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
uint16 Icu_getInputCaptureValue(void)
{
	return ICR1 ;
}
/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */

void Icu_clearTimerValue(void)
{
	TCNT1 =0;
}
/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void Icu_DeInit(void)
{
	TCCR1A = 0;
	TCCR1B =0;
	TCNT1 =0;
	ICR1=0;
	//CLEAR_BIT (TIMSK , TICIE1);
	TIMSK &= ~(1<<TICIE1);

}

