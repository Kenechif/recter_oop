/*
 * _otp.c
 *
 *  Created on: Nov 17, 2023
 *      Author: KC
 */



// OpenAuthentication Time-based One-time Password Algorithm (RFC 6238)
// For the complete description of the algorithm see
// http://tools.ietf.org/html/rfc4226#section-5.3
//
// Luca Dentella (http://www.lucadentella.it)



#include "_otp.h"
//#include "sha1__.h"


// Init the library with the private key, its length and the pump's serial number
void otp(uint8_t* hmacKey, int keyLength, uint16_t pump_serialNumber) {

	_hmacKey = hmacKey;
	_keyLength = keyLength;
	_pump_serialNumber = pump_serialNumber;
};


// Generate a code, using the otp seed provided
uint32_t getCode(long otp_seed)
{
//  long otp_preseed = 1234567890;

//  otp_seed |= otp_preseed;
  otp_seed += _pump_serialNumber;

  return getCodeFromSteps(otp_seed);
}


// Generate a code, using the number of steps provided
uint32_t getCodeFromSteps(long steps) {

  // STEP 0, map the number of steps in a 8-byte array (counter value)
//  _byteArray[0] = 0x00;
//  _byteArray[1] = 0x00;
//  _byteArray[2] = 0x00;
//  _byteArray[3] = 0x00;
//  _byteArray[4] = (uint8_t)((steps >> 24) & 0xFF);
//  _byteArray[5] = (uint8_t)((steps >> 16) & 0xFF);
//  _byteArray[6] = (uint8_t)((steps >> 8) & 0XFF);
//  _byteArray[7] = (uint8_t)((steps & 0XFF));

	uint8_t value[32] = {0};
	memcpy(value, _hmacKey, _keyLength);

	for (int i = 0; i < steps; i++)
	{
		for (int j = 0; j < sizeof(value); j++)
		{
			value[j] = (uint8_t)(value[j]* 27 + 1);
		}
	}

	int _offset;
	long _truncatedHash = 0;
	_offset = (uint8_t)(value[20 - 1] & 0xF);
	for (uint8_t j = 0; j < 4; ++j)
	{
		_truncatedHash <<= 8;
		_truncatedHash |= value[_offset + j];
	}

	//// STEP 3, compute the OTP value
	_truncatedHash &= 0x7FFFFFFF;
	_truncatedHash %= 1000000;

	return _truncatedHash;

//  // STEP 1, get the HMAC-SHA1 hash from counter and key
//  Sha1.initHmac(_hmacKey, _keyLength);
//  Sha1.write1_(_byteArray, 8);
//  _hash = Sha1.resultHmac();
//
//  // STEP 2, apply dynamic truncation to obtain a 4-bytes string
//  _offset = _hash[20 - 1] & 0xF;
//  _truncatedHash = 0;
//  for (int j = 0; j < 4; ++j) {
//    _truncatedHash <<= 8;
//    _truncatedHash  |= _hash[_offset + j];
//  }
//
//  // STEP 3, compute the OTP value
//  _truncatedHash &= 0x7FFFFFFF;
//  _truncatedHash %= 1000000;
//
//  return _truncatedHash;
}

