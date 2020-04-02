/*
 * MQ-3.h
 *
 *  Created on: 26 janv. 2020
 *      Author: axel
 */

#ifndef MQ_3_H_
#define MQ_3_H_

#include "TypeDef.h"



void MQ3_ADC(uint16_t *value);

uint8_t alcohol(void);

uint8_t alcoholCalc(uint16_t *value);

void R0Calc(uint16_t *value);





#endif /* MQ_3_H_ */
