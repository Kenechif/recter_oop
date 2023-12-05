/*
 * otp.h
 *
 *  Created on: 20 Oct 2023
 *      Author: Fidel
 */

#ifndef OTP_OTP_H_
#define OTP_OTP_H_


// OpenAuthentication Time-based One-time Password Algorithm (RFC 6238)

#include <inttypes.h>
#include "stdbool.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

void otp(uint8_t* hmacKey, uint16_t keyLength, uint16_t pump_serialNumber);

uint32_t getCode(unsigned long otp_seed);
uint32_t getCodeFromSteps(unsigned long steps);

uint8_t* _hmacKey;
uint16_t _keyLength;

uint8_t _byteArray[8];
uint8_t* _hash;
int _offset;
long _truncatedHash;
char _code[7];

extern uint16_t pump_SN;


#endif /* OTP_OTP_H_ */
