/*
 * TypeDef.h
 *
 *  Created on: 25 janv. 2020
 *      Author: axel
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_


#include <stdint.h>


//===============================================================================
//  Constants
//===============================================================================


//===============================================================================
//  Type Definition
//===============================================================================
//  Basic Type
typedef unsigned char       uint8_t;     // 8bit  unsigned integer type
typedef signed char         int8_t;      // 8bit    signed integer type
typedef unsigned long long  uint64_t;    // 64bit unsigned integer type
typedef signed long long    int64_t;     // 64bit   signed integer type
typedef float               float32_t;   // 32bit floating point number type
typedef double              float64_t;   // 64bit floating point number type

// A data structure that store a pair of uint16_t data
typedef struct {
    uint16_t on;
    uint16_t off;
} u16_pair_t;


//===============================================================================
//  Extern Global Variables
//===============================================================================


//===============================================================================
//  Public Function Prototypes
//===============================================================================


//===============================================================================
//  Macro
//===============================================================================





#endif /* TYPEDEF_H_ */
