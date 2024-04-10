/*!
 * @file I2CHandler.cpp
 *
 * \brief Class to collect common I2C functions to follow D.R.Y. principle
 * 
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#include "I2CHandler.hpp"
#include "BoardLayout.hpp"

/**************************************************************************/
/*!
    @brief    Doesn't do much, just set isInitialized to false for now
*/
/**************************************************************************/
I2CHandler::I2CHandler()
{
  this->_isInitialized = false;
  return;
}

I2CHandler::~I2CHandler() 
{
  this->_isInitialized = false;
  return;
}

/**************************************************************************/
/*!
    @brief    Take in initialized TwoWire object and add it to the QTouchBoard
*/
/**************************************************************************/
void I2CHandler::begin(TwoWire &inStream, uint8_t slaveAddr)
{
  this->_i2cStream = &inStream;
  this->_slaveAddr = slaveAddr; 
  this->_isInitialized = true;
}

/**
 * Read a single byte from addressToRead and return it as a byte
 * 
 * @param addressToRead [in] The address of the register to read
 * @return byte Value returned from register
 */
uint8_t I2CHandler::ReadSingleRegister(uint8_t addressToRead)
{
  if (!_isInitialized)
  {
    DEBUG_PRINTLN("Need to initialize by calling `begin()` first! ");
    return 0;
  }
  this->_i2cStream->beginTransmission(this->_slaveAddr);
  this->_i2cStream->write(addressToRead);
  this->_i2cStream->endTransmission(false); //endTransmission but keep the connection active

  this->_i2cStream->requestFrom(this->_slaveAddr, (uint8_t) 1); //Ask for 1 byte, once done, bus is released by default

  while(!this->_i2cStream->available()) ; //Wait for the data to come back
  return this->_i2cStream->read(); //Return this one byte
}

/** 
 * Read bytesToRead sequentially, starting at addressToRead into the dest byte array 
 * 
 * @param addressToRead [in] register address to read in the slave
 * @param bytesToRead [in] count of bytes to read 
 * @param dest [in/out] pointer to the destination for data
 */
void I2CHandler::ReadRegisters(uint8_t addressToRead, uint8_t bytesToRead, uint8_t * dest)
{
  if (!_isInitialized)
  {
    DEBUG_PRINTLN("Need to initialize by calling `begin()` first! ");
    return;
  }

  this->_i2cStream->beginTransmission(this->_slaveAddr);
  this->_i2cStream->write(addressToRead);
  this->_i2cStream->endTransmission(false); //endTransmission but keep the connection active

  this->_i2cStream->requestFrom(this->_slaveAddr, bytesToRead); //Ask for bytes, once done, bus is released by default

  while(this->_i2cStream->available() < bytesToRead); //Hang out until we get the # of bytes we expect

  for(int i = 0 ; i < bytesToRead ; i++)
  {
    dest[i] = this->_i2cStream->read();
  }    
}

/** 
 * Writes a single byte (dataToWrite) into addressToWrite
 * 
 * @param addressToWrite [in] Register address for write destination
 * @param dataToWrite [in] Byte of data to write
 */
void I2CHandler::WriteSingleRegister(uint8_t addressToWrite, uint8_t dataToWrite)
{
  if (!_isInitialized)
  {
    DEBUG_PRINTLN("Need to initialize by calling `begin()` first! ");
    return;
  }

  this->_i2cStream->beginTransmission(this->_slaveAddr);
  this->_i2cStream->write(addressToWrite);
  this->_i2cStream->write(dataToWrite);
  this->_i2cStream->endTransmission();
}
