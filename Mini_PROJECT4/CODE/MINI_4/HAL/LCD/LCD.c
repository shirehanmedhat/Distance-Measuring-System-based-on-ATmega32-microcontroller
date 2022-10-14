/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the LCD driver
 *
 * Author: Shirehan Medhat
 *
 *******************************************************************************/
/* For the delay functions */
#include <util/delay.h>
#include "../../STD_LIBRARIES/common_macros.h"
/* For GET_BIT Macro */
#include "lcd.h"
#include "../../MCAL/GPIO/gpio.h"




void LCD_init(void)
{
	/*1- DDR FOR CONTROL PINS :Common Step For Both 4-Bit And 8-Bit Mode*/
	/* Configure the direction for RS and E pins as output pins */
		GPIO_setupPinDirection(LCD_RS_PORT_ID,LCD_RS_PIN_ID,PIN_OUTPUT);
		GPIO_setupPinDirection(LCD_E_PORT_ID,LCD_E_PIN_ID,PIN_OUTPUT);
		GPIO_setupPinDirection(LCD_RW_PORT_ID,LCD_RW_PIN_ID,PIN_OUTPUT);
		GPIO_writePin (LCD_RW_PORT_ID , LCD_RW_PIN_ID ,LOGIC_LOW );

		_delay_ms(20);		/* LCD Power ON delay always > 15ms */

	/*2- DDR FOR DATA PINS & INITIALIZATION COMMANDS:*/
	/*- FOR 4 BIT MODE*/
#if(LCD_DATA_BITS_MODE == 4)
	/* Configure 4 pins in the data port as output pins */
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,PIN_OUTPUT);

	/* Send for 4 bit INITIALIZATION COMMANDS of LCD  */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);

	/* use 2-lines LCD + 4-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);
	/*- FOR 8 BIT MODE*/
#elif(LCD_DATA_BITS_MODE == 8)
	/* Configure the data port as output port */
	GPIO_setupPortDirection(LCD_DATA_PORT_ID,PORT_OUTPUT);

	/* use 2-lines LCD + 8-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);

#endif

	LCD_sendCommand(LCD_CURSOR_OFF); /* cursor off */
	LCD_sendCommand(LCD_CLEAR_COMMAND); /* clear LCD at the beginning */
}

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 command)

{
	/* Instruction Mode RS=0 */
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN_ID,LOGIC_LOW);
	 /* delay for processing Tas = 50ns */
	_delay_ms(1);
	/* Enable LCD E=1 */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_HIGH);
	/* delay for processing Tpw - Tdws = 190ns */
	_delay_ms(1);

#if(LCD_DATA_BITS_MODE == 4)
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(command,4));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(command,5));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(command,6));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(command,7));
	/* delay for processing Tdsw = 100ns */
	_delay_ms(1);
	/* Disable LCD E=0 */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);
	 /* delay for processing Th = 13ns */
	_delay_ms(1);
	/* Enable LCD E=1 */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_HIGH);
	/* delay for processing Tpw - Tdws = 190ns */
	_delay_ms(1);

	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(command,0));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(command,1));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(command,2));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(command,3));
	/* delay for processing Tdsw = 100ns */
	_delay_ms(1);
	/* Disable LCD E=0 */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);
	/* delay for processing Th = 13ns */
	_delay_ms(1);

#elif(LCD_DATA_BITS_MODE == 8)
	/* out the required command to the data bus D0 --> D7 */
	GPIO_writePort(LCD_DATA_PORT_ID,command);
	/* delay for processing Tdsw = 100ns */
	_delay_ms(1);
	/* Disable LCD E=0 */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);
	/* delay for processing Th = 13ns */
	_delay_ms(1);
#endif
}

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 data)
{
	//SAME AS SEND COMMAND BUT THE RS VALUE
	/* Data Mode RS=1 */
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN_ID,LOGIC_HIGH);
	/* delay for processing Tas = 50ns */
	_delay_ms(1);
	/* Enable LCD E=1 */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_HIGH);
	/* delay for processing Tpw - Tdws = 190ns */
	_delay_ms(1);

#if(LCD_DATA_BITS_MODE == 4)
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(data,4));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(data,5));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(data,6));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(data,7));
	/* delay for processing Tdsw = 100ns */
	_delay_ms(1);
	/* Disable LCD E=0 */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);
	/* delay for processing Th = 13ns */
	_delay_ms(1);
	/* Enable LCD E=1 */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_HIGH);
	/* delay for processing Tpw - Tdws = 190ns */
	_delay_ms(1);

	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID,GET_BIT(data,0));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID,GET_BIT(data,1));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID,GET_BIT(data,2));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID,GET_BIT(data,3));
	/* delay for processing Tdsw = 100ns */
	_delay_ms(1);
	/* Disable LCD E=0 */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);
	/* delay for processing Th = 13ns */
	_delay_ms(1);

#elif(LCD_DATA_BITS_MODE == 8)
	/* out the required command to the data bus D0 --> D7 */
	GPIO_writePort(LCD_DATA_PORT_ID,data);
	/* delay for processing Tdsw = 100ns */
	_delay_ms(1);
	/* Disable LCD E=0 */
	GPIO_writePin(LCD_E_PORT_ID,LCD_E_PIN_ID,LOGIC_LOW);
	/* delay for processing Th = 13ns */
	_delay_ms(1);
#endif
}

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const char *Str)
{
	uint8 counter = 0;
	while(Str[counter] != '\0')
	{
		LCD_displayCharacter(Str[counter]);
		counter++;
	}

}

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row,uint8 col)
{
	uint8 LCD_DDRAM_ADRESS_ROW_BASED;

	/* Calculate the required address in the LCD DDRAM */
	switch(row)
	{
		case 0:
			LCD_DDRAM_ADRESS_ROW_BASED=col;
				break;
		case 1:
			LCD_DDRAM_ADRESS_ROW_BASED=col+0x40;
				break;
		case 2:
			LCD_DDRAM_ADRESS_ROW_BASED=col+0x10;
				break;
		case 3:
			LCD_DDRAM_ADRESS_ROW_BASED=col+0x50;
				break;
	}
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(LCD_DDRAM_ADRESS_ROW_BASED | LCD_SET_CURSOR_LOCATION);
}

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	/* go to to the required LCD position */
	LCD_moveCursor(row,col);
	/* display the string */
	LCD_displayString(Str);
}

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data)
{
	/* String to hold the ascii result */
   char CONVERTED_TO_STRING[16];
   /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
   itoa(data,CONVERTED_TO_STRING,10);
   /* Display the string */
   LCD_displayString(CONVERTED_TO_STRING);
}

/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_COMMAND); /* Send clear display command */
}




