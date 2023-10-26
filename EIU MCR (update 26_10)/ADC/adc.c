
// File name	: adc.c
/*====================================*/
/*  Include                           */
/*====================================*/
#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"
#include "../eeprom/eeprom.h"

/*====================================*/
/*	Global Variable Declaration       */
/*====================================*/
unsigned int v_black[8];
unsigned int v_white[8];
unsigned int v_compare[8];
unsigned int sensor_value[8];
unsigned char sensor;
unsigned int compareValue;
unsigned int minOfMaxSensorValue[8];
unsigned int maxOfMinSensorValue[8];
unsigned char isUpdate = 0;
/*====================================*/
/*  Function name: adc_init           */
/*  Parameter: null                   */
/*  Description: configure hardware   */
/*               for adc function     */
/*====================================*/
void adc_init(){

	// ADC initialization
	// ADC Clock frequency: 1000.000 kHz
	// ADC Voltage Reference: AVCC pin
	// Use 8-bit mode
	ADMUX=ADC_VREF_TYPE & 0xff;		// compare the value of the comparision between 0b0100 0000 and 0b1111 1111 --> 0b0100 0000 (Mask effect) and 8bit-register(0x7C) value
	ADCSRA=0x84;					// write 0x84 to 8bit-register (0x7A)
	compareValue = 300;				// assign 300 for <compareValue>
}

/*====================================*/
/*  Function name: read_adc           */
/*  Parameter: adc_input              */
/*  Return value: value of voltage    */
/*                after converting    */
/*  Description:1.return voltage level*/
/*               of adc_input led from*/
/*               sensor circuit       */
/*              2. AD converter       */
/*                  function          */
/*====================================*/
// using read_adc

unsigned int read_adc(unsigned char adc_input)
{
	// Read the 8 most significant bits
	// of the AD conversion result

	ADMUX = adc_input | (ADC_VREF_TYPE & 0xff);		// <adc_input> is the parameter of the function, assign <ADMUX> the value of the comparision between <adc_input> and 0b01000000

	// Delay needed for the stabilization of the ADC input voltage
	//_delay_us(10); 

	// Start the AD conversion
	ADCSRA|=0x40;		// compare 0x40 and 8bit-register (0x7A) value

	// Wait for the AD conversion to complete
	while ((ADCSRA & 0x10)==0);		// when <ADCSRA> & 0x10 = 0 -> do not thing

	ADCSRA|=0x10;		// compare 0x10 and 8bit-register (0x7A) value
	return ADCW;		// return value of 16bit-register (0x78)
}
unsigned int read_bat(){
		// Read the 8 most significant bits
		// of the AD conversion result

		ADMUX = 0x00 | (ADC_VREF_TYPE & 0xff);		// assign <ADMUX> the value of the comparision | between 0x00 and 0b01000000 = 0b01000000
		 ADCSRB |= 0x08;	// compare 0x08 and 8bit-register (0x7B) value
		// Delay needed for the stabilization of the ADC input voltage
		//_delay_us(10);

		// Start the AD conversion
		ADCSRA|=0x40;		// compare 0x40 and 8bit-register (0x7A) value

		// Wait for the AD conversion to complete
		while ((ADCSRA & 0x10)==0);		// when <ADCSRA> & 0x10 = 0 -> do not thing

		ADCSRA|=0x10;			// compare 0x10 and 8bit-register (0x7A) value
		ADCSRB &= 0xf7;			// assign <ADCSRB> the value of the comparision & between 0xf7 and 
		return ADCW;
}
/*====================================*/
/*  Function name: read_volt          */
/*  Parameter: null                   */
/*  Description: save value of voltage*/
/*               for 8 sensor on      */
/*               sensor circuit       */
/*====================================*/
void read_volt()
{
	unsigned char i = 0;
	for (i = 0; i < 8; i++)			// for loop from 0 to 7
	sensor_value[7-i] = read_adc(i);		// write the value of 16bit-register (0x78) to <sensor_value> array from pointer at position 7 to pointer at position 0
}
/*====================================*/
/*  Function name: read_sensor        */
/*  Parameter: null                   */
/*  Description: update current value */
/*               on sensor circuit    */
/*               and output to led    */
/*====================================*/
void read_sensor(){
	unsigned char temp = 0;
	unsigned char i = 0;

	// <isUpdate> is the flag which used for notify updating sensor state
	if(isUpdate == 0)		// if sensor isn't updated
	{
		read_volt();		// get <sensor_value> array
		/*
		I'll simulate the value of sensor_value: 
		sensor_value[8] = {102, 105, 104, 103, 102, 95, 97, 100}
		It's is reversed with value which read from <ADCW>
		or: 100 	97 		95 		102 	103 	104		 105	 132
			[0]		[1] 	[2]		[3]		[4]		[5]		 [6]	 [7]

		and maxOfMinSensorValue[8] = {102, 105, 107, 105, 106, 103, 103, 104}
			minOfMaxSensorValue[8] = {98, 95, 93, 95, 94, 97, 97, 96}
		--> v_compare[8] = {100, 100, 100, 100, 100, 100, 100, 100}	
		(<v_compare> array is created by nice values)
		*/

		// read the example below the <for>
		for (i = 0; i < 8; i++)		// for loop from 0 to 7
		{
			temp = temp << 1;		// before processing, shift <temp> to the left 1 time
			if (sensor_value[i] > v_compare[i])
			{
				temp |= 0x01;      // <temp> | 0b00000001
			}
			else
			{
				temp &= 0xfe;      // <temp> & 0b11111110
			}
		}

		/*
		Note: 
		o: the value when sensor on the white line
		x: the value when sensor out of the white line
		*: the character is marked beside sensornum, It's used to mark (o) value

		Example: sensor is in case xxxx xoox
		or: case 0b0000 0110
		We conventing the left edge sensor as sensor0 -> the right edge sensor as sensor7
		[sensor0 - sensor1 - sensor2 - sensor3 - sensor4 - sensor5* - sensor6* - sensor7]
		When the sensor return this case, its mean sensor5 - sensor6 are on white line
		--> value of sensor5 - sensor6 maybe lower than other sensors's value

		And <temp> bring value is 0b0000 0000

		-> Processing:
		First, in for loop, <temp> is shifted to the left 1 time. Now, its value is also 0b0000 0000
		Second, comparing <sensor_value[i]> and <v_compare[i]>
					if sensor_value[8] = 			{102,  105,  104,  103,  102,     95,    97,  100}
					and v_compare[8] =   			{100,  100,  100,  100,  100,    100,   100,  100}	
		(sensor_value[i] > <v_compare[i]>)			true   true	 true  true  true  false  false  true

		if(true) temp |= 0x01
		else temp &= 0x254

		Loop 1 time:	temp = 0b0000 0001
		Loop 2 times:	temp = 0b0000 0011
		Loop 3 times:	temp = 0b0000 0111
		Loop 4 times:	temp = 0b0000 1111
		Loop 5 times:	temp = 0b0001 1111
		Loop 6 times:	temp = 0b0011 1110 (*)
		Loop 7 times:	temp = 0b0111 1100 (*)
		Loop 8 times:	temp = 0b1111 1001

		--> sensor = temp = 0b1111 1001 = 0xF9 (hex) = 249 (dec)
		*/
		sensor = temp;
	}
}
/*====================================*/
/*  Function name: save_sensor        */
/*  Parameter: color                  */
/*  Description: save value of voltage*/
/*              for 8 led on "color"  */
/*              line in eeprom of MCU.*/
/*				 color is BLACK       */
/*				or WHITE			  */
/*====================================*/
void save_sensor(unsigned char color)
{
	isUpdate = 0;		// assign the flag of updating sensor as 0
}
/*====================================*/
/*  Function name: update_vcompare    */
/*  Parameter: null                   */
/*  Description: update value of      */
/*               v_compare[8] and save*/
/*               in eeprom of MCU     */
/*====================================*/
void update_vcompare(){
	int i;
	for (i=0; i<8; i++)
	{
		v_compare[i] = (maxOfMinSensorValue[i] + minOfMaxSensorValue[i])/2;		// calibrate the average value between <maxOfMinSensorValue[i]> and <minOfMaxSensorValue[i]>
	}
	eeprom_writestr(V_COMPARE_POS, v_compare, 16);		// write <v_compare> array to eeprom memory at position 24 on register with size 16 bit ( [23] -> [38] )
}

void update_white(){
	int i;
	for (i=0; i<8; i++)
	{
		minOfMaxSensorValue[i] = sensor_value[i];		// assign <minOfMaxSensorValue[i]> as <sensor_value[i]>
	}
	for (i=0; i<8; i++)
	{
		v_compare[i] = (maxOfMinSensorValue[i] + minOfMaxSensorValue[i])/2;		// calibrate the average value between <maxOfMinSensorValue[i]> and <minOfMaxSensorValue[i]>
	}
	
}
void update_black(){
	int i;
	for (i=0; i<8; i++)
	{
		maxOfMinSensorValue[i] = sensor_value[i];		// assign <maxOfMinSensorValue[i]> as <sensor_value[i]>
	}
	for (i=0; i<8; i++)
	{
		v_compare[i] = (maxOfMinSensorValue[i] + minOfMaxSensorValue[i])/2;		// calibrate the average value between <maxOfMinSensorValue[i]> and <minOfMaxSensorValue[i]>
	}
	
}
/*====================================*/
/*  Function name: read_vcompare      */
/*  Parameter: null                   */
/*  Description: read v_compare[8]    */
/*               from eeprom          */
/*====================================*/
void read_vcompare(){
	eeprom_readstr(V_COMPARE_POS, v_compare, 16);		// assign value from eeprom at position 24 with size 16 bit to <v_compare[8]> array
}

/*====================================*/
/*  Function name: read_vcompare      */
/*  Parameter: null                   */
/*  Description: write v_compare[8]   */
/*               into eeprom          */
/*====================================*/
void write_vcompare(){
	/*eeprom_writestr(V_COMPARE_POS, v_compare, 8);*/
}


/****************END FILE**************/

