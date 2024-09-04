


#include "FRAM.h"
#include "math.h"
#include "string.h"

#define FRAM_I2C_ADDRESS 0x54
//#define FRAM_I2C_ADDRESS 0x50

#define FRAMBYTESIZE 8192

// Define the I2C
extern I2C_HandleTypeDef hi2c1;
//#define FRAM_I2C &hi2c1
//
//// EEPROM ADDRESS (8bits)
//#define FRAM_ADDR 0x54
//
//uint8_t write_data = 201,
//		read_data = 0;
//
//FRAM_Write(FRAM_I2C, FRAM_ADDR, 0, &write_data, 1);
//FRAM_Read(FRAM_I2C, FRAM_ADDR, 0, &read_data, 1);


//uint8_t FRAM_Write(I2C_HandleTypeDef* hi2c, uint16_t chipAddress, uint16_t writeAddress, uint8_t* data, uint16_t dataLen)
//{
//	HAL_StatusTypeDef status;
//	uint8_t addr[3] = { writeAddress >> 8, writeAddress &0xFF, data[0]};
//	status = HAL_I2C_Master_Transmit(hi2c, chipAddress, addr, 3, 1000);
////	status = HAL_I2C_Master_Seq_Transmit_IT(hi2c, chipAddress, addr, 2, I2C_FIRST_AND_NEXT_FRAME);
//	if(status != HAL_OK){
//		return status;
//	}
////	while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY);
////	{
////	}
////	status = HAL_I2C_Master_Seq_Transmit_IT(hi2c, chipAddress, data, dataLen, I2C_LAST_FRAME);
////	if(status != HAL_OK){
////		return status;
////	}
////	while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
////	{
////	}
//	return status;
//}

// Function to write a byte to a specific address in FRAM
void FRAM_WriteByte(uint16_t memAddress, uint8_t data)
{
  uint8_t buffer[3];
  buffer[0] = (memAddress >> 8) & 0xFF;  // High byte of address
  buffer[1] = memAddress & 0xFF;         // Low byte of address
  buffer[2] = data;                      // Data byte

  HAL_I2C_Master_Transmit(&hi2c1, FRAM_I2C_ADDRESS << 1, buffer, 3, HAL_MAX_DELAY);
}

// Function to read a byte from a specific address in FRAM
uint8_t FRAM_ReadByte(uint16_t memAddress)
{
  uint8_t address[2];
  uint8_t data;

  address[0] = (memAddress >> 8) & 0xFF;  // High byte of address
  address[1] = memAddress & 0xFF;         // Low byte of address

  // Send the address from which we want to read
  HAL_I2C_Master_Transmit(&hi2c1, FRAM_I2C_ADDRESS << 1, address, 2, HAL_MAX_DELAY);

  // Request 1 byte from FRAM
  HAL_I2C_Master_Receive(&hi2c1, FRAM_I2C_ADDRESS << 1, &data, 1, HAL_MAX_DELAY);

  return data;
}


// Function to write multiple bytes (page write) to FRAM
void FRAM_Write(uint16_t memAddress, uint8_t *data, uint16_t size)
{
  uint8_t buffer[2 + size];  // Address bytes + data

  // Prepare the address and data buffer
  buffer[0] = (memAddress >> 8) & 0xFF;  // High byte of address
  buffer[1] = memAddress & 0xFF;         // Low byte of address

  // Copy the data into the buffer
  for (int i = 0; i < size; i++) {
    buffer[2 + i] = data[i];
  }

  // Transmit the address and data to the FRAM
  HAL_I2C_Master_Transmit(&hi2c1, FRAM_I2C_ADDRESS << 1, buffer, 2 + size, HAL_MAX_DELAY);
}

// Function to read multiple bytes (page read) from FRAM
void FRAM_Read(uint16_t memAddress, uint8_t *buffer, uint16_t size)
{
  uint8_t address[2];

  // Prepare the address bytes
  address[0] = (memAddress >> 8) & 0xFF;  // High byte of address
  address[1] = memAddress & 0xFF;         // Low byte of address

  // Send the starting address to the FRAM
  HAL_I2C_Master_Transmit(&hi2c1, FRAM_I2C_ADDRESS << 1, address, 2, HAL_MAX_DELAY);

  // Request multiple bytes from the FRAM
  HAL_I2C_Master_Receive(&hi2c1, FRAM_I2C_ADDRESS << 1, buffer, size, HAL_MAX_DELAY);
}

void FRAM_ChipErase(void)
{
	for (uint16_t i = 0; i < FRAMBYTESIZE; i++)
	{
		FRAM_WriteByte(i, 0xFF);
		HAL_Delay(5);  // write cycle delay
	}
}


//uint8_t FRAM_Write(I2C_HandleTypeDef* hi2c, uint16_t chipAddress, uint16_t writeAddress, uint8_t* data, uint16_t dataLen){
//	HAL_StatusTypeDef status;
//	uint8_t addr[2] = { writeAddress >> 8, writeAddress &0xFF};
//	status = HAL_I2C_Master_Seq_Transmit_IT(hi2c, chipAddress, addr, 2, I2C_FIRST_AND_NEXT_FRAME);
//	if(status != HAL_OK){
//		return status;
//	}
//	while (HAL_I2C_GetState(&hi2c) != HAL_I2C_STATE_READY) {
//	}
//	status = HAL_I2C_Master_Seq_Transmit_IT(hi2c, chipAddress, data, dataLen, I2C_LAST_FRAME);
//	if(status != HAL_OK){
//		return status;
//	}
//	while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY) {
//	}
//	return status;
//}
//
//uint8_t FRAM_Read(I2C_HandleTypeDef* hi2c, uint16_t chipAddress, uint16_t readAddress, uint8_t* data, uint16_t dataLen)
//{
//	HAL_StatusTypeDef status;
//	uint8_t addr[2] = { readAddress >> 8, readAddress &0xFF};
//	status = HAL_I2C_Master_Seq_Transmit_IT(hi2c, chipAddress, addr, 2, I2C_LAST_FRAME_NO_STOP);
//	if(status != HAL_OK){
//		return status;
//	}
////	while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY) {
////	}
//	status = HAL_I2C_Master_Seq_Receive_IT(hi2c, chipAddress, data, dataLen, I2C_LAST_FRAME);
//	if(status != HAL_OK){
//		return status;
//	}
////	while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY) {
////	}
//	return status;
//}

/////*
//// * FRAM.c
//// *
//// *  Created on: 23 Apr 2024
//// *      Author: Fidel
//// */
////
////
////
////#include "FRAM.h"
////
////
//////  DENSITY CODES
////
////#define FRAM_MB85RC64                 0x03
////#define FRAM_MB85RC256                0x05
////#define FRAM_MB85RC512                0x06
////#define FRAM_MB85RC1M                 0x07
////
////
//////  used for metadata and sleep
////const uint8_t FRAM_SLAVE_ID_ = 0x7C;  //  == 0xF8
////const uint8_t FRAM_SLEEP_CMD = 0x86;  //
////
////
/////////////////////////////////////////////////////////////////////////////////
//////
////// FRAM PUBLIC
//////
////FRAM(TwoWire *wire)
////{
////  _wire            = wire;
////  _address         = 0x50;
////  _writeProtectPin = -1;
////  _sizeBytes       = 0;
////}
////
////
////void TwoWire::beginTransmission(uint8_t address)
////{
////  // indicate that we are transmitting
////  transmitting = 1;
////  // set address of targeted slave
////  txAddress = address;
////  // reset tx buffer iterator vars
////  txBufferIndex = 0;
////  txBufferLength = 0;
////}
////
////
////int begin(const uint8_t address)
////{
////  if ((address < 0x50) || (address > 0x57)) return FRAM_ERROR_ADDR;
////
////  _address = address;
////  if (! isConnected()) return FRAM_ERROR_CONNECT;
////  getSize();
////  return FRAM_OK;
////}
////
////
////bool isConnected()
////{
////  _wire->beginTransmission(_address);
////  return (_wire->endTransmission() == 0);
////}
////
////
////void write8(uint16_t memAddr, uint8_t value)
////{
////  uint8_t val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(uint8_t));
////}
////
////
////void write16(uint16_t memAddr, uint16_t value)
////{
////  uint16_t val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(uint16_t));
////}
////
////
////void write32(uint16_t memAddr, uint32_t value)
////{
////  uint32_t val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(uint32_t));
////}
////
////
////void write64(uint16_t memAddr, uint64_t value)
////{
////  uint64_t val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(uint64_t));
////}
////
////
////void writeFloat(uint16_t memAddr, float value)
////{
////  float val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(float));
////}
////
////
////void _writeBlock(uint16_t memAddr, uint8_t * obj, uint8_t size)
////{
////  _wire->beginTransmission(_address);
////  _wire->write((uint8_t) (memAddr >> 8));
////  _wire->write((uint8_t) (memAddr & 0xFF));
////  uint8_t * p = obj;
////  for (uint8_t i = size; i > 0; i--)
////  {
////    _wire->write(*p++);
////  }
////  _wire->endTransmission();
////}
////
////
////void writeDouble(uint16_t memAddr, double value)
////{
////  double val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(double));
////}
////
////
////void write(uint16_t memAddr, uint8_t * obj, uint16_t size)
////{
////  const int blocksize = 24;
////  uint8_t * p = obj;
////  while (size >= blocksize)
////  {
////    _writeBlock(memAddr, p, blocksize);
////    memAddr += blocksize;
////    p += blocksize;
////    size -= blocksize;
////  }
////  //  remaining
////  if (size > 0)
////  {
////    _writeBlock(memAddr, p, size);
////  }
////}
////
////
////uint8_t read8(uint16_t memAddr)
////{
////  uint8_t val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(uint8_t));
////  return val;
////}
////
////
////uint16_t read16(uint16_t memAddr)
////{
////  uint16_t val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(uint16_t));
////  return val;
////}
////
////
////uint32_t read32(uint16_t memAddr)
////{
////  uint32_t val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(uint32_t));
////  return val;
////}
////
////
////uint64_t read64(uint16_t memAddr)
////{
////  uint64_t val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(uint64_t));
////  return val;
////}
////
////
////float readFloat(uint16_t memAddr)
////{
////  float val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(float));
////  return val;
////}
////
////
////double readDouble(uint16_t memAddr)
////{
////  double val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(double));
////  return val;
////}
////
////
////void read(uint16_t memAddr, uint8_t * obj, uint16_t size)
////{
////  const uint8_t blocksize = 24;
////  uint8_t * p = obj;
////  while (size >= blocksize)
////  {
////    _readBlock(memAddr, p, blocksize);
////    memAddr += blocksize;
////    p += blocksize;
////    size -= blocksize;
////  }
////  //  remainder
////  if (size > 0)
////  {
////    _readBlock(memAddr, p, size);
////  }
////}
////
////
////////////////////////////////////////////////////////////////////////////
////
////
////int32_t readUntil(uint16_t memAddr, char * buffer, uint16_t bufferLength, char separator)
////{
////  //  read and fill the buffer at once.
////  read(memAddr, (uint8_t *)buffer, bufferLength);
////  for (uint16_t length = 0; length < bufferLength; length++)
////  {
////    if (buffer[length] == separator)
////    {
////      //  replace separator => \0 EndChar
////      buffer[length] = 0;
////      return length;
////    }
////  }
////  //  entry does not fit in given buffer.
////  return (int32_t)-1;
////}
////
////
////int32_t readLine(uint16_t memAddr, char * buffer, uint16_t bufferLength)
////{
////  //  read and fill the buffer at once.
////  read(memAddr, (uint8_t *)buffer, bufferLength);
////  for (uint16_t length = 0; length < bufferLength-1; length++)
////  {
////    if (buffer[length] == '\n')
////    {
////      //  add \0 EndChar after '\n'
////      buffer[length + 1] = 0;
////      return length + 1;
////    }
////  }
////  //  entry does not fit in given buffer.
////  return (int32_t)-1;
////}
////
////
////////////////////////////////////////////////////////////////////////////
////
////
////bool setWriteProtect(bool b)
////{
////  if (_writeProtectPin < 0) return false;
////  digitalWrite(_writeProtectPin, b ? HIGH : LOW);
////  return true;
////}
////
////
////bool getWriteProtect()
////{
////  if (_writeProtectPin < 0) return false;
////  return (digitalRead(_writeProtectPin) == HIGH);
////}
////
////
////////////////////////////////////////////////////////////////////
//////
//////  MANUFACTURER  PRODUCTID  SIZE
//////
////uint16_t getManufacturerID()
////{
////  uint32_t value = _getMetaData();
////  return (value >> 12) & 0x0FFF;
////}
////
////
////uint16_t getProductID()
////{
////  uint32_t value = _getMetaData();
////  return value & 0x0FFF;
////}
////
////
////#define FRAM_MANU_FUJITSU     0x0A
////#define FRAM_MANU_CYPRESS     0x04
////
////
//////  DENSITY  Fujitsu data sheet
//////  3 =>     MB85RC64 = 64 Kbit.
//////  5 =>     MB85RC256
//////  6 =>     MB85RC512
//////  7 =>     MB85RC1M
//////
//////  DENSITY  Cypress / Infineon data sheet
//////  3 =>     FM24V05 = 64 KByte.
//////  4 =>     FM24V10 = 128 KByte.
//////
//////  NOTE: returns the size in kiloBYTE (0 is read error)
////uint16_t getSize()
////{
////  uint32_t value = _getMetaData();
////  if (value == 0xFFFFFFFF) return 0;
////
////  uint16_t manufacturer = (value >> 12) & 0x0FFF;
////
////  if (manufacturer == FRAM_MANU_CYPRESS)
////  {
////    uint16_t density = (value >> 8) & 0x0F;
////    uint16_t size = (1UL << density) * 8;  //  KB
////    _sizeBytes = size * 1024UL;
////    return size;
////  }
////  //  default FRAM_MANU_FUJITSU
////  uint16_t density = (value >> 8) & 0x0F;
////  uint16_t size = (1UL << density) * 1;  //  KB
////  _sizeBytes = size * 1024UL;
////  return size;
////}
////
////
////uint32_t getSizeBytes()
////{
////  return _sizeBytes;
////}
////
////
//////  override to be used when getSize() fails == 0
////void setSizeBytes(uint32_t value)
////{
////  _sizeBytes = value;
////}
////
////
////uint32_t clear(uint8_t value)
////{
////  uint8_t buffer[16];
////  for (uint8_t i = 0; i < 16; i++) buffer[i] = value;
////  uint32_t start = 0;
////  uint32_t end = _sizeBytes;
////  for (uint32_t address = start; address < end; address += 16)
////  {
////    _writeBlock(address, buffer, 16);
////  }
////  return end - start;
////}
////
////
//////  EXPERIMENTAL - to be confirmed
//////  page 12 datasheet
//////  command = S 0xF8 A address A S 86 A P  (A = Ack from slave )
////void sleep()
////{
////  _wire->beginTransmission(FRAM_SLAVE_ID_);       //  S 0xF8
////  _wire->write(_address << 1);                    //  address << 1
////  _wire->endTransmission(false);                  //  no stoP
////  _wire->beginTransmission(FRAM_SLEEP_CMD >> 1);  //  S 0x86
////  _wire->endTransmission(true);                   //  stoP
////}
////
////
//////  page 12 datasheet   trec <= 400us
////bool wakeup(uint32_t timeRecover)
////{
////  //  wakeup
////  bool b = isConnected();
////  if (timeRecover == 0) return b;
////  //  wait recovery time
////  delayMicroseconds(timeRecover);
////  //  check recovery OK
////  return isConnected();
////}
////
////
/////////////////////////////////////////////////////////////////////////////////
//////
////// FRAM PROTECTED
//////
////
//////  metadata is packed as  [MMMMMMMM][MMMMDDDD][PPPPPPPP]
//////  M = manufacturerID
//////  D = density => memory size = 2^D KB
//////  P = product ID (together with D)
//////  P part might be proprietary
////uint32_t _getMetaData()
////{
////  _wire->beginTransmission(FRAM_SLAVE_ID_);
////  _wire->write(_address << 1);
////  _wire->endTransmission(false);
////  int x = _wire->requestFrom(FRAM_SLAVE_ID_, (uint8_t)3);
////  if (x != 3) return 0xFFFFFFFF;
////
////  uint32_t value = 0;
////  value = _wire->read();
////  value = value << 8;
////  value |= _wire->read();
////  value = value << 8;
////  value |= _wire->read();
////
////  return value;
////}
////
////
////void _writeBlock(uint16_t memAddr, uint8_t * obj, uint8_t size)
////{
////  _wire->beginTransmission(_address);
////  _wire->write((uint8_t) (memAddr >> 8));
////  _wire->write((uint8_t) (memAddr & 0xFF));
////  uint8_t * p = obj;
////  for (uint8_t i = size; i > 0; i--)
////  {
////    _wire->write(*p++);
////  }
////  _wire->endTransmission();
////}
////
////
////void _readBlock(uint16_t memAddr, uint8_t * obj, uint8_t size)
////{
////  _wire->beginTransmission(_address);
////  _wire->write((uint8_t) (memAddr >> 8));
////  _wire->write((uint8_t) (memAddr & 0xFF));
////  _wire->endTransmission();
////  _wire->requestFrom(_address, size);
////  uint8_t * p = obj;
////  for (uint8_t i = size; i > 0; i--)
////  {
////    *p++ = _wire->read();
////  }
////}
////
////
/////////////////////////////////////////////////////////////////////////////////
//////
//////  FRAM32  PUBLIC
//////
////FRAM32(TwoWire *wire) : FRAM(wire)
////{
////}
////
////
////void write8(uint32_t memAddr, uint8_t value)
////{
////  uint8_t val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(uint8_t));
////}
////
////
////void write16(uint32_t memAddr, uint16_t value)
////{
////  uint16_t val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(uint16_t));
////}
////
////
////void write32(uint32_t memAddr, uint32_t value)
////{
////  uint32_t val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(uint32_t));
////}
////
////
////void write64(uint32_t memAddr, uint64_t value)
////{
////  uint64_t val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(uint64_t));
////}
////
////
////void writeFloat(uint32_t memAddr, float value)
////{
////  float val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(float));
////}
////
////
////void writeDouble(uint32_t memAddr, double value)
////{
////  double val = value;
////  _writeBlock(memAddr, (uint8_t *)&val, sizeof(double));
////}
////
////
////void write(uint32_t memAddr, uint8_t * obj, uint16_t size)
////{
////  const int blocksize = 24;
////  uint8_t * p = obj;
////  while (size >= blocksize)
////  {
////    _writeBlock(memAddr, p, blocksize);
////    memAddr += blocksize;
////    p += blocksize;
////    size -= blocksize;
////  }
////  //  remaining
////  if (size > 0)
////  {
////    _writeBlock(memAddr, p, size);
////  }
////}
////
////
////uint8_t read8(uint32_t memAddr)
////{
////  uint8_t val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(uint8_t));
////  return val;
////}
////
////
////uint16_t read16(uint32_t memAddr)
////{
////  uint16_t val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(uint16_t));
////  return val;
////}
////
////
////uint32_t read32(uint32_t memAddr)
////{
////  uint32_t val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(uint32_t));
////  return val;
////}
////
////
////uint64_t read64(uint32_t memAddr)
////{
////  uint64_t val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(uint64_t));
////  return val;
////}
////
////
////float readFloat(uint32_t memAddr)
////{
////  float val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(float));
////  return val;
////}
////
////
////double readDouble(uint32_t memAddr)
////{
////  double val;
////  _readBlock(memAddr, (uint8_t *)&val, sizeof(double));
////  return val;
////}
////
////
////void read(uint32_t memAddr, uint8_t * obj, uint16_t size)
////{
////  const uint8_t blocksize = 24;
////  uint8_t * p = obj;
////  while (size >= blocksize)
////  {
////    _readBlock(memAddr, p, blocksize);
////    memAddr += blocksize;
////    p += blocksize;
////    size -= blocksize;
////  }
////  // remainder
////  if (size > 0)
////  {
////    _readBlock(memAddr, p, size);
////  }
////}
////
////
////int32_t readUntil(uint32_t memAddr, char * buffer, uint16_t bufferLength, char separator)
////{
////  //  read and fill the buffer at once.
////  read(memAddr, (uint8_t *)buffer, bufferLength);
////  for (uint16_t length = 0; length < bufferLength; length++)
////  {
////    if (buffer[length] == separator)
////    {
////      //  replace separator => \0 EndChar
////      buffer[length] = 0;
////      return length;
////    }
////  }
////  //  entry does not fit in given buffer.
////  return (int32_t)-1;
////}
////
////
////int32_t readLine(uint32_t memAddr, char * buffer, uint16_t bufferLength)
////{
////  //  read and fill the buffer at once.
////  read(memAddr, (uint8_t *)buffer, bufferLength);
////  for (uint16_t length = 0; length < bufferLength-1; length++)
////  {
////    if (buffer[length] == '\n')
////    {
////      //  add \0 EndChar after '\n'
////      buffer[length + 1] = 0;
////      return length + 1;
////    }
////  }
////  //  entry does not fit in given buffer.
////  return (int32_t)-1;
////}
////
////
////uint32_t clear(uint8_t value)
////{
////  uint8_t buffer[16];
////  for (uint8_t i = 0; i < 16; i++) buffer[i] = value;
////  uint32_t start = 0;
////  uint32_t end = _sizeBytes;
////  for (uint32_t addr = start; addr < end; addr += 16)
////  {
////    _writeBlock(addr, buffer, 16);
////  }
////  return end - start;
////}
////
////
/////////////////////////////////////////////////////////////////////////////////
//////
//////  FRAM32  PROTECTED
//////
////void writeBlock(uint32_t memAddr, uint8_t * obj, uint8_t size)
////{
////  uint8_t _addr = _address;
////  if (memAddr & 0xFFFE0000) return;  //  ignore invalid memory addresses
////  if ((memAddr & 0x00010000) == 0x00010000) _addr += 0x01;
////
////  _wire->beginTransmission(_addr);
////  _wire->write((uint8_t) (memAddr >> 8));
////  _wire->write((uint8_t) (memAddr & 0xFF));
////  uint8_t * p = obj;
////  for (uint8_t i = size; i > 0; i--)
////  {
////    _wire->write(*p++);
////  }
////  _wire->endTransmission();
////}
////
////
////void _readBlock(uint32_t memAddr, uint8_t * obj, uint8_t size)
////{
////  uint8_t _addr = _address;
////  if (memAddr & 0xFFFE0000) return;  //  ignore invalid memory addresses
////  if ((memAddr & 0x00010000) == 0x00010000) _addr += 0x01;
////
////  _wire->beginTransmission(_addr);
////  _wire->write((uint8_t) (memAddr >> 8));
////  _wire->write((uint8_t) (memAddr & 0xFF));
////  _wire->endTransmission();
////  _wire->requestFrom(_addr, size);
////  uint8_t * p = obj;
////  for (uint8_t i = size; i > 0; i--)
////  {
////    *p++ = _wire->read();
////  }
////}
////
////
/////////////////////////////////////////////////////////////////////////////////
//////
//////  FRAM11
//////
////FRAM11(TwoWire *wire) : FRAM(wire)
////{
////  _sizeBytes = 2048;
////}
////
////
////int begin(const uint8_t address, const int8_t writeProtectPin)
////{
////  int rv = begin(address, writeProtectPin);
////  _sizeBytes = 2048;
////  return rv;
////}
////
////
////uint16_t getSize()
////{
////  return _sizeBytes / 1024;
////}
////
////
/////////////////////////////////////////////////////////////////////////////////
//////
//////  FRAM11  PROTECTED
//////
////void _writeBlock(uint16_t memAddr, uint8_t * obj, uint8_t size)
////{
////  //  Device uses Address Pages
////  uint8_t  DeviceAddrWithPageBits = _address | ((memAddr & 0x0700) >> 8);
////  _wire->beginTransmission(DeviceAddrWithPageBits);
////  _wire->write((uint8_t) (memAddr & 0xFF));
////
////  uint8_t * p = obj;
////  for (uint8_t i = size; i > 0; i--)
////  {
////    _wire->write(*p++);
////  }
////  _wire->endTransmission();
////}
////
////
////void _readBlock(uint16_t memAddr, uint8_t * obj, uint8_t size)
////{
////  //  Device uses Address Pages
////  uint8_t DeviceAddrWithPageBits = _address | ((memAddr & 0x0700) >> 8);
////  _wire->beginTransmission(DeviceAddrWithPageBits);
////  _wire->write((uint8_t) (memAddr & 0xFF));
////  _wire->endTransmission();
////  _wire->requestFrom(DeviceAddrWithPageBits, size);
////
////  uint8_t * p = obj;
////  for (uint8_t i = size; i > 0; i--)
////  {
////    *p++ = _wire->read();
////  }
////}
////
////
/////////////////////////////////////////////////////////////////////////////////
//////
//////  FRAM9
//////
////FRAM9(TwoWire *wire) : FRAM(wire)
////{
////  _sizeBytes = 512;
////}
////
////
////int begin(const uint8_t address, const int8_t writeProtectPin)
////{
////  int rv = begin(address, writeProtectPin);
////  _sizeBytes = 512;
////  return rv;
////}
////
////
////uint16_t getSize()
////{
////  return _sizeBytes / 1024;  //  == 0.
////}
////
////
/////////////////////////////////////////////////////////////////////////////////
//////
//////  FRAM9  PROTECTED
//////
////void _writeBlock(uint16_t memAddr, uint8_t * obj, uint8_t size)
////{
////  //  Device uses Address Pages
////  uint8_t DeviceAddrWithPageBits = _address | ((memAddr & 0x0100) >> 8);
////  _wire->beginTransmission(DeviceAddrWithPageBits);
////  _wire->write((uint8_t) (memAddr & 0xFF));
////
////  uint8_t * p = obj;
////  for (uint8_t i = size; i > 0; i--)
////  {
////    _wire->write(*p++);
////  }
////  _wire->endTransmission();
////}
////
////
////void _readBlock(uint16_t memAddr, uint8_t * obj, uint8_t size)
////{
////  //  Device uses Address Pages
////  uint8_t DeviceAddrWithPageBits = _address | ((memAddr & 0x0100) >> 8);
////  _wire->beginTransmission(DeviceAddrWithPageBits);
////  _wire->write((uint8_t) (memAddr & 0xFF));
////  _wire->endTransmission();
////  _wire->requestFrom(DeviceAddrWithPageBits, size);
////
////  uint8_t * p = obj;
////  for (uint8_t i = size; i > 0; i--)
////  {
////    *p++ = _wire->read();
////  }
////}
////
////
//////  -- END OF FILE --
////
////
////
////
//
//
//#include "FRAM.h"
//#include "math.h"
//#include "string.h"
//
//// Define the I2C
//extern I2C_HandleTypeDef hi2c1;
//#define FRAM_I2C &hi2c1
//
//// FRAM ADDRESS (8bits)
//#define FRAM_ADDR 0x54
//
//// Define the Page Size and number of pages
//#define PAGE_SIZE 32     //64     // in Bytes
//#define PAGE_NUM  256    //512    // number of pages
//
//extern int t;
//
//extern uint32_t transaction_period,
//				transaction_period2;
//
//
//
///*****************************************************************************************************************************************/
//uint8_t bytes_temp[4];
//
//// function to determine the remaining bytes
//uint16_t bytestowrite_FRAM (uint16_t size, uint16_t offset)
//{
//	if ((size+offset)<PAGE_SIZE) return size;
//	else return PAGE_SIZE-offset;
//}
//
///* write the data to the FRAM
// * @page is the number of the start page. Range from 0 to PAGE_NUM-1
// * @offset is the start byte offset in the page. Range from 0 to PAGE_SIZE-1
// * @data is the pointer to the data to write in bytes
// * @size is the size of the data
// */
//void FRAM_Write (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
//{
//   //float fl = data->price_;
//	// Find out the number of bit, where the page addressing starts
//	int paddrposition = log(PAGE_SIZE)/log(2);
//
//	// calculate the start page and the end page
//	uint16_t startPage = page;
//	uint16_t endPage = page + ((size+offset)/PAGE_SIZE);
//
//	// number of pages to be written
//	uint16_t numofpages = (endPage-startPage) + 1;
//	uint16_t pos=0;
//
//	// write the data
//	for (int i=0; i<numofpages; i++)
//	{
//		/* calculate the address of the memory location
//		 * Here we add the page address with the byte address
//		 */
//		uint16_t MemAddress = startPage<<paddrposition | offset;
//		uint16_t bytesremaining = bytestowrite_FRAM(size, offset);  // calculate the remaining bytes to be written
//
//		HAL_I2C_Mem_Write(FRAM_I2C, FRAM_ADDR, MemAddress, 2, &data[pos], bytesremaining, 1000);  // write the data to the FRAM
//
//		startPage += 1;  // increment the page, so that a new page address can be selected for further write
//		offset=0;   // since we will be writing to a new page, so offset will be 0
//		size = size-bytesremaining;  // reduce the size of the bytes
//		pos += bytesremaining;  // update the position for the data buffer
//
//		HAL_Delay (5);  // Write cycle delay (5ms)
//	}
//}
//
//
////void save_settings()
////{
////	extern const int save_settings1_loc;
////	extern const int save_settings2_loc;
////    int sz = sizeof(copy[0]);
////
////  // for (int i = 0 ; i < sz;i++)
////  // {
////	   FRAM_Write(save_settings1_loc, 0, &settings[0], sz);
////	   FRAM_Write(save_settings2_loc, 0, &settings[1], sz);
////  // }
////}
//
//
//void float2Bytes_FRAM(uint8_t * ftoa_bytes_temp,float float_variable)
//{
//    union {
//      float a;
//      uint8_t bytes[4];
//    } thing;
//
//    thing.a = float_variable;
//
//    for (uint8_t i = 0; i < 4; i++) {
//      ftoa_bytes_temp[i] = thing.bytes[i];
//    }
//
//}
//
//float Bytes2float_FRAM(uint8_t * ftoa_bytes_temp)
//{
//    union {
//      float a;
//      uint8_t bytes[4];
//    } thing;
//
//    for (uint8_t i = 0; i < 4; i++) {
//    	thing.bytes[i] = ftoa_bytes_temp[i];
//    }
//
//   float float_variable =  thing.a;
//   return float_variable;
//}
//
//
///*Write the Float/Integer values to the FRAM
// * @page is the number of the start page. Range from 0 to PAGE_NUM-1
// * @offset is the start byte offset in the page. Range from 0 to PAGE_SIZE-1
// * @data is the float/integer value that you want to write
// */
//
//void FRAM_Write_NUM (uint16_t page, uint16_t offset, float data)
//{
//
//	float2Bytes_FRAM(bytes_temp, data);
//
//	FRAM_Write(page, offset, bytes_temp, 4);
//}
//
///* Reads the single Float/Integer values from the FRAM
// * @page is the number of the start page. Range from 0 to PAGE_NUM-1
// * @offset is the start byte offset in the page. Range from 0 to PAGE_SIZE-1
// * @returns the float/integer value
// */
//
//float FRAM_Read_NUM (uint16_t page, uint16_t offset)
//{
//	uint8_t buffer[4];
//
//	FRAM_Read(page, offset, buffer, 4);
//
//	return (Bytes2float_FRAM(buffer));
//}
//
///* READ the data from the FRAM
// * @page is the number of the start page. Range from 0 to PAGE_NUM-1
// * @offset is the start byte offset in the page. Range from 0 to PAGE_SIZE-1
// * @data is the pointer to the data to write in bytes
// * @size is the size of the data
// */
//int8_t FRAM_Read (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
//{
//	int paddrposition = log(PAGE_SIZE)/log(2);
//
//	uint16_t startPage = page;
//	uint16_t endPage = page + ((size+offset)/PAGE_SIZE);
//
//	uint16_t numofpages = (endPage-startPage) + 1;
//	uint16_t pos=0;
//    t = 0;
//	for (int i=0; i<numofpages; i++)
//	{
//		uint16_t MemAddress = startPage<<paddrposition | offset;
//		uint16_t bytesremaining = bytestowrite_FRAM(size, offset);
//		HAL_I2C_Mem_Read(FRAM_I2C, FRAM_ADDR, MemAddress, 2, &data[pos], bytesremaining, 1000);
//		startPage += 1;
//		offset=0;
//		size = size-bytesremaining;
//		pos += bytesremaining;
//
//		if(t > 500)
//			return 0;
//	}
//	return 1;
//}
//
////
////void retrieve_settings()
////{
////	extern const int save_settings1_loc;
////	extern const int save_settings2_loc;
////
////   int sz = sizeof(copy[0]);
////
////   FRAM_Read(save_settings1_loc, 0, &settings[0], sz);
////   FRAM_Read(save_settings2_loc, 0, &settings[1], sz);
////   //read totaliser settings..
////}
//
//
///* Erase a page in the FRAM Memory
// * @page is the number of page to erase
// * In order to erase multiple pages, just use this function in the for loop
// */
//void FRAM_PageErase (uint16_t page)
//{
//	// calculate the memory address based on the page number
//	int paddrposition = log(PAGE_SIZE)/log(2);
//	uint16_t MemAddress = page<<paddrposition;
//
//	// create a buffer to store the reset values
//	uint8_t data[PAGE_SIZE];
//	memset(data,0xff,PAGE_SIZE);
//
//	// write the data to the FRAM
//	HAL_I2C_Mem_Write(FRAM_I2C, FRAM_ADDR, MemAddress, 2, data, PAGE_SIZE, 1000);
//
//	HAL_Delay (5);  // write cycle delay
//}
//
//
