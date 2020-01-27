/*
 * MQ-3.c
 *
 *  Created on: 26 janv. 2020
 *      Author: axel
 */

#include "MQ-3.h"
#include <math.h>

void alcohol(void){
	uint8_t value;
	MQ3_ADC(&value);
	alcoholCalc(&value);
}

void alcoholCalc(uint8_t *value){
	uint16_t R2 = 2000;
	uint16_t R0 = 16000;
	float32_t RS = 0;
	float32_t ratio = 0;
	float64_t x = 0;
	float32_t rate = 0;
	float32_t volt = 0;

	volt = (float32_t)*value/1024 * 5;
	RS = ((5 * R2) / volt) - R2;
	ratio = RS/R0;
	x = 0.4 * ratio;
	rate = pow(x, -1.431);
	rate = rate * 0.001;
}


