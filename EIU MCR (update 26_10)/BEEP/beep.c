// File name	: beep.c
/*====================================*/
/*  Include                           */
/*====================================*/
#include <avr/io.h>
#include "beep.h"
#include "../TIMER/timer.h"

/*====================================*/
/*	Global Variable Declaration       */
/*====================================*/
unsigned int beep_cn;			// init the beep duration
unsigned char beep_flag;		// init the flag of buzzer

/*====================================*/
/*  Function name: beep_init          */
/*  Parameter: null                   */
/*  Description: configure hardware   */
/*               for beep speaker     */
/*====================================*/
void beep_init(){
	BEEP_DDR |= _BV(BEEP_PIN);		// write the value from PB4(4) of 8bit-register (0x05)
	beep_cn = 0;					// beep duration will assigned as 0
	beep_flag = 0;					// assign 0 for flag (false)
}

/*====================================*/
/*  Function name: beep_mod           */
/*  Parameter: null                   */
/*  Description: control beep from	  */
/* 				speaker by using	  */
/*				beep_flag and beep_cn.*/
/*				 This function is	  */
/*				called by interrupt	  */
/* 				timer				  */
/*====================================*/
void beep_mod(){
	// when the flag of beep is available (different 0)
	if(beep_flag){
		// if true
		beep_cn--;	// <beep_cn> will decreasing, it's used as timer counting down to 0 with 1ms delay
		// when <beep_cn> = 0
		if(beep_cn==0){
			beep_flag = 0;		// assign <beep_flag> as 0 (false)
			beep_off();			// turn off the buzzer
		}
	}
}

/*====================================*/
/*  Function name: beep_on            */
/*  Parameter: null                   */
/*  Description: speaker on          */
/*====================================*/
void beep_on(){
	BEEP_PORT |= _BV(BEEP_PIN);			// compare PB4(4) from 8bit-register (0x05) and the BEEP_PORT value
}

/*====================================*/
/*  Function name: beep_off           */
/*  Parameter: null                   */
/*  Description: speaker off          */
/*====================================*/
void beep_off(){
	BEEP_PORT &= ~_BV(BEEP_PIN);		// reset the BEEP_PORT at PB4(4) from 8bit-register (0x05)
}

/*====================================*/
/*  Function name: beep_long          */
/*  Parameter: timer                  */
/*  Description: speaker on during    */
/*               "_timer"             */
/*====================================*/
void beep_long(unsigned int _timer){
	beep_cn = _timer/TIMER_DIV;			// TIMER_DIV (counter period) is 1ms, <_timer> is the parameter that user setting
	beep_flag = 1;						// assign <beep_flag> as 1 (true)
	beep_on();							// turn on the buzzer
}


/****************END FILE**************/



