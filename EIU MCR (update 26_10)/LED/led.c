// File name	: led.c
/*====================================*/
/*  Include                           */
/*====================================*/
#include <avr/io.h>
#include "led.h"



/*====================================*/
/*  Function name: led_init           */
/*  Parameter: null                   */
/*  Description: configure hardware   */
/*               for led function     */
/*====================================*/
void led_init(){
	LED_DATA_DIR = 0xFF;		// write 0xFF to 8bit-register (0x01)
	LED_DATA_PORT = 0xff;		// write 0xff to 8bit-register (0x02)
	GRB_DATA_DIR |= 0x38;		// compare 0x38 and 8bit-register (0x10A) value
	GRB_DATA_PORT &= 0xc7;		// compare 0xc7 and 8bit-register (0x10B) value
}

/*====================================*/
/*  Function name: led_put            */
/*  Parameter: _val                   */
/*  Description: output _val into led */
/*             module on MCU circuit  */
/*====================================*/
void led_put(unsigned char _val){
	LED_DATA_PORT = ~_val;		// write the opposite value of <sensor> to 8bit-register (0x02)
}


void RGB(unsigned char redColor,unsigned char greenColor,unsigned char blueColor){
	if (redColor > 0 )
	GRB_DATA_PORT |= 0b00010000;
	else GRB_DATA_PORT &= 0b11101111;
	if (greenColor > 0 )
	GRB_DATA_PORT |= 0b00100000;
	else GRB_DATA_PORT &= 0b11011111;
	if (blueColor > 0 )
	GRB_DATA_PORT |= 0b00001000;
	else GRB_DATA_PORT &= 0b11110111;
}

/****************END FILE**************/

