/*
 * bh1790glc.c
 *
 *  Created on: 21 janv. 2020
 *      Author: axel
 */

#include "bh1790glc.h"

static hrParam hr;
static ParamU8 prmU8;

static ParamF  s_maPrm_pre1;
static ParamF  s_maPrm_pre2;
static ParamF  s_maPrm_pre3;

static iirParam  iirPrm_hpf;  // Hi-Pass Filter
// fc=0.5Hz (fs=32.0Hz)
static iirParam  iirPrm_lpf;  // Low-Pass Filter
// fc=3.5Hz (fs=32.0Hz)

static uint8_t init = 0;

uint8_t heartrate(void){
	uint16_t dataOff = 0x0000;
	uint16_t dataOn = 0x0000;
	float32_t data = 0;

	getMeasureData(&dataOff, &dataOn);

	if(dataOn > 5000){
		PulseWaveCalc(&dataOff, &dataOn, &data);
		heartrateCalc(data);
		return TouchOn;
	}
	else
		return TouchOff;
}

void heartrateCalc(float32_t data){
	float32_t bpm_candidate = 0.0F;
	float32_t diff1         = 0.0F;
	float32_t diff2         = 0.0F;
	int32_t   tmp           = 0;

	hr.cnt++;

	if (hr.cnt > 1) {
		diff1 = data - hr.last1;
	}
	else {
		diff1 = 0.0F;
	}
	hr.last1 = data;

	if (hr.cnt > 2) {
		diff2 = diff1 - hr.last2;
	}
	else {
		diff2 = 0.0F;
	}
	hr.last2 = diff1;

	tmp = hr.cnt - hr.cnt_pkDet;
	if (tmp > 0) {
		bpm_candidate = 60.0F / (HR_SAMPLING_PERIOD * (float32_t)tmp);

	}
	else {
		bpm_candidate = 0.0F;
	}

	if (bpm_candidate < HR_BPM_MIN_TH) {
		hr.cnt_pkDet = 0;
	}

	if ((diff1 < 0.0F) && (diff2 >= 0.0F) && (hr.cnt_diff2 >= 3)) {
		if(hr.cnt_pkDet != 0) {
			if (bpm_candidate <= HR_BPM_MAX_TH) {
				bpm_candidate += 0.5F;
				hr.bpm = AverageU8((uint8_t)bpm_candidate, &prmU8);
			}
		}
		hr.cnt_pkDet = hr.cnt;
	}

	if (diff2 < 0.0F) {
		hr.cnt_diff2 = hr.cnt_diff2 + 1;
	}
	else {
		hr.cnt_diff2 = 0;
	}
}

void heartrateData(uint8_t *bpm){
	*bpm = hr.bpm;
}

void PulseWaveCalc(uint16_t *dataOff, uint16_t *dataOn, float32_t *dataOut){
	float32_t pre1Out    = 0.0F;
	float32_t pre2Out    = 0.0F;
	float32_t dataIn_ave = 0.0F;
	float32_t hpfOut     = 0.0F;

	pre1Out    = AverageF((float32_t)*dataOn,     &s_maPrm_pre1);
	pre2Out    = AverageF(pre1Out,               &s_maPrm_pre2);
	dataIn_ave = AverageF(pre2Out,               &s_maPrm_pre3);

	if (init == 0U) {
		init = 1U;
		iirSet(dataIn_ave, dataIn_ave, &iirPrm_hpf);
		iirSet(0.0F,       0.0F,       &iirPrm_lpf);
	}
	hpfOut   = iirFilter(dataIn_ave, &iirPrm_hpf);
	*dataOut = iirFilter(hpfOut,     &iirPrm_lpf);
}

void getMeasureData(uint16_t *dataOff, uint16_t *dataOn){
	uint8_t data[4];
	bh1790Read(data);
	*dataOff = (uint16_t) ((data[1] << 8) | data[0]);
	*dataOn = (uint16_t) ((data[3] << 8) | data[2]);
}

void heartrateInit(void){
	PulseWaveInit();
	hrInit();
}

void PulseWaveInit(void){

	const float32_t hpf_coeff[] = { 0.9329322F, -1.8658643F, 0.9329322F, 1.8613611F, -0.8703675F };
	const float32_t lpf_coeff[] = { 0.2635176F,  0.2635176F, 0.0F,       0.4729648F,  0.0F };

	// Move Average
	InitF(4U, &s_maPrm_pre1);
	InitF(8U, &s_maPrm_pre2);
	InitF(8U, &s_maPrm_pre3);

	// IIR Filter
	init = 0U;
	iirInit(hpf_coeff, &iirPrm_hpf);
	iirInit(lpf_coeff, &iirPrm_lpf);
}

void InitF(uint8_t len, ParamF *prm){
	uint8_t i    = 0U;

	prm->len = len;
	prm->sum = 0.0F;
	for (i = 0U; i < 8; i++) {
		prm->buffer[i] = 0.0F;
	}
	prm->pos = 0U;
	prm->num = 0U;
}

float32_t AverageF(float32_t data, ParamF *prm){
	float64_t ans = 0.0F;

	if (prm->num == prm->len) {
		prm->sum -= prm->buffer[prm->pos];
	}
	prm->sum += data;
	prm->buffer[prm->pos] = data;

	(prm->pos)++;
	if (prm->pos >= prm->len) {
		prm->pos = 0U;
	}
	if (prm->num < prm->len) {
		(prm->num)++;
	}

	if (prm->num > 0U) {
		ans = prm->sum / (float64_t)prm->num;
	}

	return ((float32_t)ans);
}

void InitU8(uint8_t len, ParamU8 *prm){
	uint8_t i = 0;

	prm->len = len;
	prm->sum = 0;
	for (i = 0U; i < 8; i++) {
		prm->buffer[i] = 0U;
	}
	prm->pos = 0U;
	prm->num = 0U;
}

uint8_t AverageU8(uint8_t bpm, ParamU8 *prm){
	int16_t ans = 0;

	if (prm->num == prm->len) {
		prm->sum -= (int16_t)prm->buffer[prm->pos];
	}
	prm->sum += (int16_t)bpm;
	prm->buffer[prm->pos] = bpm;

	(prm->pos)++;
	if (prm->pos >= prm->len) {
		prm->pos = 0U;
	}
	if (prm->num < prm->len) {
		(prm->num)++;
	}

	if(prm->num > 0U) {
		ans = prm->sum / (int16_t)prm->num;
	}

	return ((uint8_t)ans);
}

void iirInit(const float32_t coeff[], iirParam *prm){
	if (coeff == 0) {
		prm->coeff.a0 = 1.0F;
		prm->coeff.a1 = 0.0F;
		prm->coeff.a2 = 0.0F;
		prm->coeff.b1 = 0.0F;
		prm->coeff.b2 = 0.0F;
	}
	else {
		prm->coeff.a0 = coeff[0];
		prm->coeff.a1 = coeff[1];
		prm->coeff.a2 = coeff[2];
		prm->coeff.b1 = coeff[3];
		prm->coeff.b2 = coeff[4];
	}

	prm->reg.x1 = 0.0F;
	prm->reg.x2 = 0.0F;
	prm->reg.y1 = 0.0F;
	prm->reg.y2 = 0.0F;
}

void iirSet(float32_t x1, float32_t x2, iirParam * iir){
	iir->reg.x1 = x1;
	iir->reg.x2 = x2;

	iir->reg.y1 = 0.0F;
	iir->reg.y2 = 0.0F;
}

float32_t iirFilter(float32_t data, iirParam *iir){
	float32_t work1 = 0.0F;
	float32_t work2 = 0.0F;
	float32_t work  = 0.0F;

	work1 = (iir->coeff.a0 * data) + (iir->coeff.a1 * iir->reg.x1) + (iir->coeff.a2 * iir->reg.x2);
	work2 = (iir->coeff.b1 * iir->reg.y1) + (iir->coeff.b2 * iir->reg.y2);
	work  = work1 + work2;

	iir->reg.x2 = iir->reg.x1;
	iir->reg.x1 = data;

	iir->reg.y2 = iir->reg.y1;
	iir->reg.y1 = work;

	return (work);
}

void hrInit(void){

	hr.bpm       = 0U;
	hr.cnt       = 0;
	hr.cnt_diff2 = 0;
	hr.cnt_pkDet = 0;
	hr.last1     = 0.0F;
	hr.last2     = 0.0F;

	InitU8(4U, &prmU8);
}

void PulsewaveClear(void){
	init = 0;
	ClearF(&s_maPrm_pre1);
	ClearF(&s_maPrm_pre1);
	ClearF(&s_maPrm_pre1);
}

void ClearF(ParamF *prm){
	prm->sum = 0.0F;
	prm->pos = 0U;
	prm->num = 0U;
}

void hrClear(void){
	hr.cnt       = 0;
	hr.cnt_diff2 = 0;
	hr.cnt_pkDet = 0;
	hr.bpm = 0U;
	ClearU8(&prmU8);
}

void ClearU8(ParamU8 *prm){
	prm->sum = 0;
	prm->pos = 0U;
	prm->num = 0U;
}

void clear(void){
	PulsewaveClear();
	hrClear();
}
