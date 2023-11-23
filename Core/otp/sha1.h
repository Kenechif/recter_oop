///*
// * sha1.h
// *
// *  Created on: 20 Oct 2023
// *      Author: Fidel
// */
//
//#ifndef OTP_SHA1_H_
//#define OTP_SHA1_H_
//
//#include <inttypes.h>
//#include "stdbool.h"
//#include "string.h"
//#include "stddef.h"
//#include "stdint.h"
//#include "stdlib.h"
//#include "stdio.h"
//#include "math.h"
//
//#define __WRITE_RESULT size_t
//#define __WRITE_RETURN(x) return x;
//
//
//#define HASH_LENGTH 20
//#define BLOCK_LENGTH 64
//
//union _buffer
//{
//  uint8_t b[BLOCK_LENGTH];
//  uint32_t w[BLOCK_LENGTH/4];
//};
//
//
//union _state
//{
//  uint8_t b[HASH_LENGTH];
//  uint32_t w[HASH_LENGTH/4];
//};
//
//    void init_innerHash(void);
//    void initHmac(const uint8_t* secret, int secretLength);
//    uint8_t* result(void);
//    uint8_t* resultHmac(void);
//    __WRITE_RESULT write1(uint8_t);
//    size_t write1_(const uint8_t *buffer, size_t size);
//
//    void pad();
//    void addUncounted(uint8_t data);
//    void hashBlock();
//    uint32_t rol32(uint32_t number, uint8_t bits);
//    union _buffer bufferr;
//    uint8_t bufferOffset;
//    union _state statee;
//    uint32_t byteCount;
//    uint8_t keyBuffer[BLOCK_LENGTH];
//    uint8_t innerHash[HASH_LENGTH];
//
//
//
////#include <inttypes.h>
////
////#define HASH_LENGTH 20
////#define BLOCK_LENGTH 64
////
////union _buffer {
////  uint8_t b[BLOCK_LENGTH];
////  uint32_t w[BLOCK_LENGTH/4];
////} buffer;
////union _state {
////  uint8_t b[HASH_LENGTH];
////  uint32_t w[HASH_LENGTH/4];
////} state;
////
////uint8_t bufferOffset;
////uint32_t byteCount;
////uint8_t keyBuffer[BLOCK_LENGTH];
////uint8_t innerHash[HASH_LENGTH];
////
////void init(void);
////void initHmac(const uint8_t* secret, uint8_t secretLength);
////uint8_t* result(void);
////uint8_t* resultHmac(void);
////void write(uint8_t);
////void writeArray(uint8_t *buffer, uint8_t size);
//
//
//#endif /* OTP_SHA1_H_ */
