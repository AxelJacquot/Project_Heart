/*
 * bh1790glc.h
 *
 *  Created on: 25 janv. 2020
 *      Author: axel
 */

#ifndef BH1790GLC_H_
#define BH1790GLC_H_

#include "TypeDef.h"

#define HR_BPM_MIN_TH       (40.0F)
#define HR_BPM_MAX_TH       (240.0F)

#define HR_SAMPLING_PERIOD  (0.03125F)

#define TouchOn 0xff
#define TouchOff 0x00

typedef struct {
    uint8_t      bpm;
    int32_t      cnt;
    int32_t      cnt_diff2;
    int32_t      cnt_pkDet;
    float32_t    last1;
    float32_t    last2;
} hrParam;

// Moving Average Parameter of uint8_t data
typedef struct {
    uint8_t   buffer[8]; // Buffer of uint8_t data
    int16_t   sum;                // Sum of values within the buffer
    uint8_t   pos;                // Index of the oldest data
    uint8_t   len;                // Moving Average Length
    uint8_t   num;                // Number of valid data
} ParamU8;

// Moving Average Parameter of float32_t data
typedef struct {
    float32_t buffer[8]; // Buffer of float32_t data
    float64_t sum;                // Sum of values within the buffer
    uint8_t   pos;                // Index of the oldest data
    uint8_t   len;                // Moving Average Length
    uint8_t   num;                // Number of valid data
} ParamF;

// IIR Filter Parameter:  Coefficient
typedef struct {
    float32_t   a0; // Coeff of Filter Input Data
    float32_t   a1; // Coeff of x1
    float32_t   a2; // Coeff of x2
    float32_t   b1; // Coeff of y1
    float32_t   b2; // Coeff of y2
} iirCoeff;

// IIR Filter Parameter:  Internal register
typedef struct {
    float32_t   x1; // Last data of Filter Input
    float32_t   x2; // Second last data of Filter Input
    float32_t   y1; // Last data of Filter Output
    float32_t   y2; // Second last data of Filter Output
} iirRegister;

// IIR Filter Parameter
typedef struct {
    iirCoeff    coeff;
    iirRegister reg;
} iirParam;


uint8_t heartrate(void);
void heartrateCalc(float32_t data);
void heartrateData(uint8_t *bpm);
void PulseWaveCalc(uint16_t *dataOff, uint16_t *dataOn, float32_t *dataOut);
void getMeasureData(uint16_t *dataOff, uint16_t *dataOn);
void heartrateInit(void);

void PulseWaveInit(void);
void InitF(uint8_t len, ParamF *prm);
float32_t AverageF(float32_t data, ParamF *prm);
void InitU8(uint8_t len, ParamU8 *prm);
uint8_t AverageU8(uint8_t bpm, ParamU8 *prm);
void iirInit(const float32_t coeff[], iirParam *prm);
void iirSet(float32_t x1, float32_t x2, iirParam * iir);
float32_t iirFilter(float32_t data, iirParam *iir);
void hrInit(void);
void PulsewaveClear(void);
void ClearF(ParamF *prm);
void hrClear(void);
void ClearU8(ParamU8 *prm);
void clear(void);

void bh1790Read(uint8_t dataOut[4]);


#endif /* BH1790GLC_H_ */
