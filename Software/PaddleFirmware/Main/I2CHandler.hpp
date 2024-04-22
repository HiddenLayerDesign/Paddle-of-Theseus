/*!
 * @file I2CHandler.hpp
 * 
 * \brief Header for class to control I2C interactions
 *
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#ifndef __I2C_HANDLER_HPP__
#define __I2C_HANDLER_HPP__

#include <Wire.h>

/**************************************************************************/
/*!
  @brief  Class to collect basic I2C functions to follow D.R.Y. principle
*/
/**************************************************************************/
class I2CHandler 
{
  private:
    TwoWire *_i2cStream;  ///< One of Wire/ Wire1 class for I2C
    uint8_t _slaveAddr;  ///< Provided slaveAddr will be constant for this class instance
    bool _isInitialized;  ///< Ensures Read/WriteReg are only used after TwoWire and slaveAddr are provided
  public:
    ~I2CHandler();
    I2CHandler();
    void begin(TwoWire &inStream, uint8_t slaveAddr);  
    uint8_t ReadSingleRegister(uint8_t reg);
    void WriteSingleRegister(uint8_t reg, uint8_t value);
    void ReadRegisters(uint8_t addressToRead, uint8_t bytesToRead, uint8_t * dest);
};

#endif /* __I2C_HANDLER_HPP__ */
