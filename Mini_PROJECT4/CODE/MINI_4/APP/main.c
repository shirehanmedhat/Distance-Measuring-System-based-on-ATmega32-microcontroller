/******************************************************************************
 *
 * Module: Main file
 *
 * File Name: main.c
 *
 * Description: Source file for the project 4 application
 *
 * Author: Shirehan Medhat
 *
 *******************************************************************************/

#include "avr/io.h"
#include "../HAL/LCD/LCD.h"
#include "../HAL/ULTRASONIC/ULTRASONIC.h"
#include "util/delay.h"
void DisplayDistance (void);
int main()
{
	SREG|=(1<<7); // GIE
	Ultrasonic_init();
	LCD_init(); // initialization of LCD
	LCD_displayString("Distance= ");
	while(1)
	{
		DisplayDistance ();

	}
	return 0;

}
void DisplayDistance (void)
{
	uint16  CurrentDistance =0 ;
	/*Static to prevent Poping its value.*/
	static uint16 PrevDistance =0;
	/*Read the distance */
	CurrentDistance = Ultrasonic_readDistance();
	/*Move the cursor*/
	LCD_moveCursor(0, 10);
	if (CurrentDistance != PrevDistance)
	{
		/*Display the distance */
		if (CurrentDistance >= 100)
		{
			LCD_intgerToString(Ultrasonic_readDistance());
			LCD_displayString(" cm");
		}/*Display the distance less then 100 and clear the space */
		else if ((CurrentDistance >= 10) && (CurrentDistance < 100))
		{
			LCD_intgerToString(Ultrasonic_readDistance());
			LCD_displayString(" cm ");
		}

	}
	/*To avoid the LCD headache */
	PrevDistance = CurrentDistance;
}


