/*
 * _otp.h
 *
 *  Created on: Nov 17, 2023
 *      Author: KC
 */

#ifndef OTP__OTP_H_
#define OTP__OTP_H_


// OpenAuthentication Time-based One-time Password Algorithm (RFC 6238)
// Arduino Library
//
// Luca Dentella (http://www.lucadentella.it)

#include <inttypes.h>
#include "stdbool.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"


uint8_t* _hmacKey;
int _keyLength;
uint16_t _pump_serialNumber;
//uint8_t _byteArray[8];
//uint8_t value[32];
uint8_t* _hash;
//int _offset;
//long _truncatedHash;
char _code[7];

void otp(uint8_t* hmacKey, int keyLength, uint16_t pump_serialNumber);
uint32_t getCode(long otp_seed);
uint32_t getCodeFromSteps(long steps);


#endif /* OTP__OTP_H_ */
