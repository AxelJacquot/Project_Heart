/*
 * MQ-3.c
 *
 *  Created on: 26 janv. 2020
 *      Author: axel
 */

#include "MQ-3.h"
#include <math.h>

uint8_t alcohol(void){
	uint16_t value;
	uint8_t rate;
	MQ3_ADC(&value);
	R0Calc(&value);
	rate = alcoholCalc(&value);
	return rate;
}

uint8_t alcoholCalc(uint16_t *value){
	uint16_t R2 = 9970;
	uint16_t R0 = 264;
	float32_t RS = 0;
	float32_t ratio = 0;
	float64_t x = 0;
	uint8_t rate = 0;
	float32_t volt = 0;

	volt = ((float32_t)*value/1024) * 3.3;
	RS = ((3.3 * R2) / volt) - R2;
	ratio = RS/R0 ;

	x = pow(ratio, -1.431);
	x = 0.4 * x;
	rate = x * 1000;
	return rate;
}

void R0Calc(uint16_t *value){
	uint16_t R2 = 9970;
	uint16_t R;
	float32_t Rs;
	float32_t volt = (float32_t)*value/1024 * 3.3;
	Rs = ((3.3 * R2) / volt) - R2;
	R = Rs / 188;
}
