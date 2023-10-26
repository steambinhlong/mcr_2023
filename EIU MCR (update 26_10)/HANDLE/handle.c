
// File name	: handle.c
/*====================================*/
/*  Include                           */
/*====================================*/
#include <avr/io.h>
#include "handle.h"
#include "../eeprom/eeprom.h"


/*====================================*/
/*  Function name: handle_init        */
/*  Parameter: null                   */
/*  Description: 1.configure hardware */
/*               for handle function  */
/*               2.Used for handle    */
/*                  Servo motor       */
/*====================================*/
unsigned int handle_center = 1500;		// this is a intial for handle the servo -> It's used as timer counting from 1500 to 0
void handle_init(){

	DDRB  |=  _BV(PB5);		// write the value of PB5(5) to 8bit-register (0x04)

	// Timer/Counter 3 initialization
	// Clock source: System Clock
	// Clock value: 2000.000 kHz
	// Mode: Ph. & fr. cor. PWM top=ICR3
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
// 	TCCR3A=0xA8;
// 	TCCR3B=0x12;
// 	TCNT3H=0x00;
// 	TCNT3L=0x00;
// 	ICR3H=0x3E;
// 	ICR3L=0x80;
// 	OCR3AH=0x00;
// 	OCR3AL=0x00;
// 	OCR3BH=0x00;
// 	OCR3BL=0x00;
// 	OCR3CH=0x00;
// 	OCR3CL=0x00;
// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 2000.000 kHz
// Mode: Ph. & fr. cor. PWM top=ICR1
// OC1A output: Non-Inverted PWM
// OC1B output: Disconnected
// OC1C output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 5 ms
// Output Pulse(s):
// OC1A Period: 5 ms Width: 1.5 ms
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR1A=(1<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);		// write 0x80 to 8bit-register (0x80)
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (1<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);		// write 0x12 to 8bit-register (0x81)
TCNT1H=0x00;		// write 0x00 to 8bit-register(0x85)
TCNT1L=0x00;		// write 0x00 to 8bit-register(0x84)
ICR1H=0x13;			// write 0x13 to 8bit-register(0x87)
ICR1L=0x88;			// write 0x88 to 8bit-register(0x86)
OCR1AH=0x05;		// write 0x05 to 8bit-register(0x89)
OCR1AL=0xDC;		// write 0xDC to 8bit-register(0x88)
OCR1BH=0x00;		// write 0x00 to 8bit-register(0x8B)
OCR1BL=0x00;		// write 0x00 to 8bit-register(0x8A)
OCR1CH=0x00;		// write 0x00 to 8bit-register(0x8D)
OCR1CL=0x00;		// write 0x00 to 8bit-register(0x8C)
}

/*====================================*/
/*  Function name: handle             */
/*  Parameter: _angle                 */
/*  Description: used to control Servo*/
/*               to turn to "_angle"  */
/*====================================*/
void handle(int _angle){
	// <_angle> is the parameter of the function
	// max-servoSpeed take 2ms per 1 period (*)
	// time to servo rotating from 0 to 180 degrees about 1500ms, but 1 period that PWM oscillating take 10ms - 20ms -> 75-150 periods (**)
	// (*) + (**) -> in 1500ms, servo take 100-150ms to oscillating max PWM
	RC_ANGLE = handle_center - HANDLE_STEP * _angle;		// write the angle to 16bit-register(0x88)
}
void handleMicro(int _angle){
	// <_angle> is the parameter of the function
	RC_ANGLE = handle_center - _angle;			// write the angle to 16bit-register(0x88)
}
void save_handle_center ()
{
	eeprom_writebyte (HANDLE_CENTER_POS, 	handle_center);			// low byte, write <handle_center> to EEPROM (at position 40)
	eeprom_writebyte (HANDLE_CENTER_POS+1, 	handle_center>>8);		// high byte, write EEPROM (bit 8) to EEPROM (at position 41)
	/* Example: 1500 = [41] 0b0000 0101 
					   [40] 0b1101 1100
	*/
}
void read_handle_center ()
{
	handle_center = eeprom_readbyte (HANDLE_CENTER_POS+1);		// handle_center = 0b0000 0101 
	handle_center <<= 8;		/* handle_center = [40] 0b0000 0101 
												   [41] 0b0000 0000 
								*/
	handle_center = handle_center + eeprom_readbyte (HANDLE_CENTER_POS);
	/*
	handle_center = handle_center[40] 0b0000 0101 + (handle[41] 0b0000 0000 + EEPROM[40] 0b1101 1100) = 0b00000101 0b1101 1100 = 1500
	*/
	// if <handle_center> lower than 700
	if (handle_center < 700)
		handle_center = 1500;		// reset to intial value (1500)
	// if <handle_center> higher than 2300
	if (handle_center > 2300)
		handle_center = 1500;		// reset to intial value (1500)
}

/****************END FILE**************/

