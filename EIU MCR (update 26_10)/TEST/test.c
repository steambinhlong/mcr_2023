

/*====================================*/
/*  Include                           */
/*====================================*/
#include <util/delay.h>
#include "test.h"
#include "../ADC/adc.h"
#include "../BEEP/beep.h"
#include "../LED/led.h"
#include "../HANDLE//handle.h"
#include "../SPEED/speed.h"
#include "../INPUT/input.h"
#include "../TIMER/timer.h"
#include "../ENCODER/encoder.h"
//#include "../UART/uart.h"
#include "../RUN/run.h"
#include "../LCD/lcd4bit.h"

volatile unsigned char testing_flag = 1;		// volatile is the data type which synchronizing the value of the variable in every void
/*====================================*/
/*  Function name: test               */
/*  Parameter: null                   */
/*  Description: to implement cases   */
/*              to test all modules   */
/*              on the car       */
/*====================================*/

void test(){
	while(testing_flag == 1){
		// if true
		switch(get_dipsw()){
			// DIPSW: 0 0 0 0
			case 0:
			RGB(1,0,0); 	//the RGB light turns red
			/*************line sensor calibration************/
			if (SW1){	// press SW1 to update black value
				update_black();		// get <maxOfMinSensorValue[8]> array
				beep_long(20);		// turn speaker on 20ms
			}
			if (SW2){	// press SW2 to update white value
				update_white();		// get <minOfMaxSensorValue[8]> array
				beep_long(100);		// turn speaker on 100ms
			}
			if (SW3){	// press SW3 to save sensor value
				update_vcompare();	// calibrate <v_compare[8]> array and write it into EEPROM Memory
				beep_long(300);		// turn speaker on 300ms
			}
			if (SW4){	// press SW4 to save sensor value
				update_vcompare();	// calibrate <v_compare[8]> array and write it into EEPROM Memory
				beep_long(300);		// turn speaker on 300ms
			}
			break;
			// DIPSW: 0 0 0 1
			case 1:
			RGB(0,1,0);		//the RGB light turns green
			/************test DC motor***********/
			if (SW2){			// press KEY1 to run left motor  backward
				speed(-250,0);		// Right motor is reverse moving
				_delay_ms(10);
				}else if(SW1){		// press KEY0 to run left motor forward
				speed(250,0);		// Right motor is clockwise moving
				_delay_ms(10);
				}else if(SW4){		// press KEY3 to run right motor backward
				speed(0,-250);		// Left motor is reverse moving
				_delay_ms(10);
				}else if(SW3){		// press KEY2 to run right motor forward
				speed(0, 250);		// Left motor is clockwise moving
				_delay_ms(10);
				}else{				// stop
				speed(0,0);			// set all the motors stop
			}
			
			break;
			// DIPSW: 0 0 1 0
			case 2:
			RGB(0,0,1);//the RGB light turns blue
			/************test RC servo***********/
			if (SW3){			// press SW3 to turn right
				handle_center -= 2;		// <handle_center> will be decreased
				handle(0);				// set 0 to the new postion
				_delay_ms(50);
				}else if(SW2){	// press SW2 to turn left
				handle_center += 2;		// <handle_center> will be increased
				handle(0);				// set 0 to the new postion
				_delay_ms(50);
			}
			else if (SW1||SW4)		// press SW1 or SW4 to save the angle to EEPROM
			{
				save_handle_center();		// save the <handle_center> value
				_delay_ms(1000);
				beep_long(200);				// turn speaker on 200ms
				_delay_ms(200);
			}
			break;
			// case 3://test encoder
			// RGB(1,1,0);//the RGB light turns yellow
			// LCD_Gotoxy(0,0);
			// LCD_PutDec(distance1);
			// /************test Encoder***********/
			// break;
			// case 4://the RGB light turns blue
			// RGB(0,1,1);
			// // LCD_Clr();
			// // LCD_Gotoxy(0,0);
			// // LCD_PutDec(sensor_value[7]);
			// // LCD_Gotoxy(4,0);
			// // LCD_PutDec(sensor_value[6]);
			// // LCD_Gotoxy(8,0);
			// // LCD_PutDec(sensor_value[5]);
			// // LCD_Gotoxy(12,0);
			// // LCD_PutDec(sensor_value[4]);
			// // LCD_Gotoxy(0,1);
			// // LCD_PutDec(sensor_value[3]);
			// // LCD_Gotoxy(4,1);
			// // LCD_PutDec(sensor_value[2]);
			// // LCD_Gotoxy(8,1);
			// // LCD_PutDec(sensor_value[1]);
			// // LCD_Gotoxy(12,1);
			// // LCD_PutDec(sensor_value[0]);
			// // _delay_ms(100);
			// RYGB(0,100,0,0);
			// break;
			// DIPSW: 0 0 1 1
			case 5:
			RGB(1,0,1);		//the RGB light turns purple
			if (SW3){			// press SW3 to turn right
				// if true
				handle(35);		// limit left angle 
				_delay_ms(50);
				}else if(SW2){	// press SW2 to turn left
				
				handle(-35);	// limit right angle 
				_delay_ms(45);
			}
			
			break;
			default:		// go to run.c
			RGB(0,0,0);		// turn off the led
			if (SW1)		// press SW1
			{
				// if true
				testing_flag = 0;	// disable flag of testing functions
				beep_long(200);		// turn speaker on 200ms
			}
			else if(SW4)	// press SW1
			{
				testing_flag = 0;	// disable flag of testing functions
				beep_long(200);		// turn speaker on 200ms
			}
			else if (SW2)	// press SW2
			{
				
				testing_flag = 0;	// disable flag of testing functions
				beep_long(200);		// turn speaker on 200ms
			}
			else if(SW3)	// press SW3
			{
				testing_flag = 0;	// disable flag of testing functions
				beep_long(200);		// turn speaker on 200ms
			}
			
			break;
			
		}
	}
}

/****************END FILE**************/

