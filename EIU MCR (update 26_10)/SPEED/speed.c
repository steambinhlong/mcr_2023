/*====================================*/
/*  Include                           */
/*====================================*/
#include <avr/io.h>
#include "speed.h"

/*====================================*/
/*  Function name: speed_init         */
/*  Parameter: null                   */
/*  Description: configure hardware   */
/*               for PWM (speed)      */
/*               function             */
/*====================================*/
void speed_init(){
	DIR_DDR  |=  _BV(DIR_LEFT)|_BV(DIR_RIGHT)|_BV(DIR_STEERING);		// write the value of PE2(2), PE6(6), PE7(7) to 8bit-register(0x0D)
	PWM_DDR  |=  _BV(PWM_LEFT)|_BV(PWM_RIGHT)|_BV(PWM_STEERING);		// write the value of PE3(3), PE4(4), PE5(5) to 8bit-register(0x0D)
	BRAKE_DDR  |=  _BV(BRAKE_RIGHT)|_BV(BRAKE_LEFT)|_BV(BRAKE_STEERING);		// write the value of PB1(1), PB0(0), PB2(2) to 8bit-register(0x04)
	BRAKE_POUT  |=  _BV(BRAKE_RIGHT)|_BV(BRAKE_LEFT)|_BV(BRAKE_STEERING);		// write the value of PB1(1), PB0(0), PB2(2) to 8bit-register(0x05)
	// Timer/Counter 3 initialization
	// Clock source: System Clock
	// Clock value: 250.000 kHz
	// Mode: Ph. correct PWM top=0x00FF
	// OC3A output: Non-Inv.
	// OC3B output: Non-Inv.
	// OC3C output: Non-Inv.
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer3 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	// Compare C Match Interrupt: Off
	TCCR3A=(1<<COM3A1) | (0<<COM3A0) | (1<<COM3B1) | (0<<COM3B0) | (1<<COM3C1) | (0<<COM3C0) | (0<<WGM31) | (1<<WGM30);		// write 0xA9 to 8bit-register (0x90) 
	TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (0<<WGM32) | (0<<CS32) | (1<<CS31) | (0<<CS30);	// write 0x02 to 8bit-register (0x91) 
	TCNT3H=0x00;		// write 0x00 to 8bit-register(0x95)
	TCNT3L=0x00;		// write 0x00 to 8bit-register(0x94)
	ICR3H=0x00;			// write 0x00 to 8bit-register(0x97)
	ICR3L=0x00;			// write 0x00 to 8bit-register(0x96)
	OCR3AH=0x00;		// write 0x00 to 8bit-register(0x99)
	OCR3AL=0x00;		// write 0x00 to 8bit-register(0x98)
	OCR3BH=0x00;		// write 0x00 to 8bit-register(0x9B)
	OCR3BL=0x00;		// write 0x00 to 8bit-register(0x9A)
	OCR3CH=0x00;		// write 0x00 to 8bit-register(0x9D)
	OCR3CL=0x00;		// write 0x00 to 8bit-register(0x9C)

}

/*====================================*/
/*  Function name: set_left_dir       */
/*  Parameter: _dir                   */
/*  Description: setting direction for*/
/*             left motor             */
/*====================================*/
void set_left_dir(unsigned char _dir){
	// <_dir> is the parameter which is set when calling function
	if(_dir == FORWARD)
		DIR_POUT |= _BV(DIR_LEFT);		// write the value of PE2(2) to 8bit-register(0x0E)
	else
		DIR_POUT &= ~_BV(DIR_LEFT);		// write the opposite value of PE2(2) to 8bit-register(0x0E)
}

/*====================================*/
/*  Function name: set_right_dir      */
/*  Parameter: _dir                   */
/*  Description: setting direction for*/
/*             right motor            */
/*====================================*/
void set_right_dir(unsigned char _dir){
	if(_dir == FORWARD)
	// <_dir> is the parameter which is set when calling function
		DIR_POUT |= _BV(DIR_RIGHT);		// write the value of PE6(6) to 8bit-register(0x0E)
	else
		DIR_POUT &= ~_BV(DIR_RIGHT);	// write the opposite value of PE6(6) to 8bit-register(0x0E)
}

/*====================================*/
/*  Function name: speed              */
/*  Parameter:_left_speed,_right_speed*/
/*  Description: setting speed for    */
/*             left and right motor   */
/*====================================*/
void speed(int _right_speed , int _left_speed){

	if(_left_speed >= 0){
		set_left_dir(FORWARD);		// callback function and set 1 as parameter
	}else{
		_left_speed = -_left_speed;		// absolute the speed from negative to positive speed
		set_left_dir(BACKWARD);		// callback function and set 0 as parameter
	}
	if(_right_speed >= 0){
		set_right_dir(FORWARD);		// callback function and set 1 as parameter
	}else{
		_right_speed = -_right_speed;	// absolute the speed from negative to positive speed
		set_right_dir(BACKWARD);	// callback function and set 0 as parameter
	}

	if(_left_speed > MAX_SPEED)  _left_speed = MAX_SPEED;		// above limit speed is 255
	if(_left_speed < MIN_SPEED)  _left_speed = 0;		// below limit speed is 0
	
	if(_right_speed > MAX_SPEED)  _right_speed = MAX_SPEED;		// above limit speed is 255
	if(_right_speed < MIN_SPEED)  _right_speed = 0;		// below limit speed is 0

	LEFT_SPEED  = _left_speed;		// assign <_left_speed> to <LEFT_SPEED>
	RIGHT_SPEED = _right_speed;		// assign <_right_speed> to <RIGHT_SPEED>
}


/****************END FILE**************/

