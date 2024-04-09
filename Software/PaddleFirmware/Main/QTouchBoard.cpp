/*!
 * @file QTouchBoard.cpp
 *
 * \brief Class to control pair of capacitive touch board for fretBoard and strumBoard
 * 
 * These two boards mimic the affordances of a guitar using special copper capacitive-touch
 * zones inscribed onto the PCBs and interpreted by AT42QT series QTouch sensors. Thus,
 * the StrumBoard and FretBoard are each a "QTouch Board". The boards even have the same
 * Bill of Materials between them, the only difference is size, count, and position of touch regions
 * 
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#include "QTouchBoard.hpp"

/**************************************************************************/
/*!
    @brief    Creates QTouchBoard connected to one of the two I2c instances and sets up chips
    @param    *inStream
              Reference to one of the I2C TwoWire streams, Wire or Wire1
    @param    initChips
              True == Initialize the AT42QT2120 and AT42QT1070 over I2C, else False
*/
/**************************************************************************/
QTouchBoard::QTouchBoard(int int1070, int int2120)
{
  // set _i2cStream later in setup()
  _intPin1070 = int1070;
  _intPin2120 = int2120;

  pinMode(_intPin1070, INPUT);
  pinMode(_intPin2120, INPUT);
}

/**************************************************************************/
/*!
    @brief    Take in initialized TwoWire object and add it to the QTouchBoard
*/
/**************************************************************************/
void QTouchBoard::begin(TwoWire &inStream)
{
  _i2cStream = &inStream;
  initQTouch();
}

QTouchBoard::~QTouchBoard() { }

/**************************************************************************/
/*!
    @brief    Creates QTouchBoard and initializes pins
*/
/**************************************************************************/
void QTouchBoard::initQTouch()
{
    _InitQT1070();
    _InitQT2120();  
}

/**************************************************************************/
/*!
    @brief    Initialize the register settings for the AT42QT1070
*/
/**************************************************************************/
void QTouchBoard::_InitQT1070()
{
  // Get Chip ID
  uint8_t chipId = _ReadSingleReg(false, REG_QT1070_CHIP_ID);
  Serial.print("QT1070 chipId = 0x"); Serial.print(chipId, HEX); Serial.print(", should be 0x"); Serial.println(VAL_QT1070_CHIP_ID, HEX);

  // Get FW version
  uint8_t versionByte = _ReadSingleReg(false, REG_QT1070_VERSION);
  uint8_t versionMajor = (versionByte & 0xF0) >> 4;
  uint8_t versionMinor = (versionByte & 0x0F);
  Serial.print("Firmware version = "); Serial.print(versionMajor); Serial.print("."); Serial.println(versionMinor);

  // Set touch integration
  for (int i=0; i<7; i++)
  {
    _WriteSingleReg(false, i + REG_QT1070_INTEGRATION, 4); // TODO revisit these settings
    _WriteSingleReg(false, i + REG_QT1070_AVE_AKS, 0x20); // TODO revisit these settings
  }

  // Set Low Power Mode
  _WriteSingleReg(false, 54, 1); // TODO revisit these settings
}

/**************************************************************************/
/*!
    @brief    Initialize the register settings for the AT42QT2120
*/
/**************************************************************************/
void QTouchBoard::_InitQT2120()
{
  // Get Chip ID
  uint8_t chipId = _ReadSingleReg(true, REG_QT2120_CHIP_ID);
  Serial.print("QT2120 chipId = 0x"); Serial.print(chipId, HEX); Serial.print(", should be 0x"); Serial.println(VAL_QT2120_CHIP_ID, HEX);

  // Get FW version
  uint8_t versionByte = _ReadSingleReg(true, REG_QT2120_VERSION);
  uint8_t versionMajor = (versionByte & 0xF0) >> 4;
  uint8_t versionMinor = (versionByte & 0x0F);
  Serial.print("Firmware version = "); Serial.print(versionMajor); Serial.print("."); Serial.println(versionMinor);

  // Set touch integration
  _WriteSingleReg(true, 11, 4); // TODO revisit these settings

  // Set drift hold time
  _WriteSingleReg(true, 13, 3); // TODO revisit these settings

  // Set detect threshold
  for (int i=0; i<12; i++)
  {
      _WriteSingleReg(true, 16+i, 19); // TODO revisit these settings
  } 
}

/**************************************************************************/
/*!
    @brief    Read a single AT42QTx register and return single byte of data
    @param    isQTouch2120
              True to communicate with QTOUCH2120_ADDR, else communicate with QTOUCH1070_ADDR 
    @param    reg
              Register address to read
    @return   Value returned from I2C device at register address reg    
*/
/**************************************************************************/
uint8_t QTouchBoard::_ReadSingleReg(bool isQTouch2120, uint8_t reg)
{
  uint8_t i2cAddr = (isQTouch2120) ? QTOUCH2120_ADDR : QTOUCH1070_ADDR;
  
  _i2cStream->beginTransmission(i2cAddr);
  _i2cStream->write(reg);
  _i2cStream->endTransmission(false); // endTransmission but keep the connection active

  _i2cStream->requestFrom((int) i2cAddr, 1); // Ask for 1 byte, once done, bus is released by default

  while(!_i2cStream->available()) ; // Wait for the data to come back
  return _i2cStream->read(); // Return this one byte
}

/**************************************************************************/
/*!
    @brief    Write a single byte value to selected AT42QTx register reg
    @param    isQTouch2120
              True to communicate with QTOUCH2120_ADDR, else communicate with QTOUCH1070_ADDR 
    @param    reg
              Register address to write to
    @param    value
              Value to be written    
*/
/**************************************************************************/
void QTouchBoard::_WriteSingleReg(bool isQTouch2120, uint8_t reg, uint8_t value)
{
  uint8_t i2cAddr = (isQTouch2120) ? QTOUCH2120_ADDR : QTOUCH1070_ADDR;
  
  _i2cStream->beginTransmission(i2cAddr);
  _i2cStream->write(reg);
  _i2cStream->write(value);
  _i2cStream->endTransmission();
}

/**************************************************************************/
/*!
    @brief    Check whether this QTouchBoard has any new values
    @return   True if at least one of the update GPIO interrupt pins fired
*/
/**************************************************************************/
bool QTouchBoard::isValueUpdate()
{
  // check whether either of the QTouch pins have pulled their onChange GPIO down
  return (!digitalRead(_intPin1070) || !digitalRead(_intPin2120));
}

/**************************************************************************/
/*!
    @brief    Alias for reading register of QT2120
*/
/**************************************************************************/
uint8_t QTouchBoard::QT2120ReadSingleReg(uint8_t reg)
{
  return _ReadSingleReg(true, reg); 
}

/**************************************************************************/
/*!
    @brief    Alias for reading register of QT1070
*/
/**************************************************************************/
uint8_t QTouchBoard::QT1070ReadSingleReg(uint8_t reg)
{
  return _ReadSingleReg(false, reg); 
}

/**************************************************************************/
/*!
    @brief    Alias for writing register of QT2120
*/
/**************************************************************************/
void QTouchBoard::QT2120WriteSingleReg(uint8_t reg, uint8_t value)
{
  _WriteSingleReg(true, reg, value);  
}

/**************************************************************************/
/*!
    @brief    Alias for writing register of QT1070
*/
/**************************************************************************/
void QTouchBoard::QT1070WriteSingleReg(uint8_t reg, uint8_t value)
{
  _WriteSingleReg(false, reg, value);  
}

 /**************************************************************************/
/*!
    @brief    Update _fret member with the highest fret currently pressed
    @param    ks0
              Output of AT42QT2120 status register LSB
    @param    ks1
              Output of AT42QT2120 status register MSB 
    @param    ks2
              Output of AT42QT1070 status register 
*/
/**************************************************************************/
uint8_t QTouchBoard::ReturnFret(uint8_t ks0, uint8_t ks1, uint8_t ks2)
{
  // Efficiently check FretBoard status registers for highest pressed fret
  uint8_t _fret = 0;

  if (ks2)
  {
    if (ks2 & 0x40) _fret = 19;
    else if (ks2 & 0x20) _fret = 18;
    else if (ks2 & 0x10) _fret = 17;
    else if (ks2 & 0x08) _fret = 16;
    else if (ks2 & 0x04) _fret = 15;
    else if (ks2 & 0x02) _fret = 14;
    else if (ks2 & 0x01) _fret = 13;
  }
  else if (ks1)
  {
    if      (ks1 & 0x08) _fret = 12;
    else if (ks1 & 0x04) _fret = 11;
    else if (ks1 & 0x02) _fret = 10;
    else if (ks1 & 0x01) _fret = 9;
  }
  else if (ks0)
  {
    if (ks0 & 0x80) _fret = 8;
    else if (ks0 & 0x40) _fret = 7;
    else if (ks0 & 0x20) _fret = 6;
    else if (ks0 & 0x10) _fret = 5;
    else if (ks0 & 0x08) _fret = 4;
    else if (ks0 & 0x04) _fret = 3;
    else if (ks0 & 0x02) _fret = 2;
    else if (ks0 & 0x01) _fret = 1;
    else _fret = 0;
  }
  return _fret;
}

/**************************************************************************/
/*!
    @brief    Update _key member with the set of keys currently pressed, if any
    @param    ss0
              Output of AT42QT2120 status register LSB
    @param    ss1
              Output of AT42QT2120 status register MSB 
    @param    ss2
              Output of AT42QT1070 status register 
*/
/**************************************************************************/
uint8_t QTouchBoard::ReturnStrumKey(uint8_t ss0, uint8_t ss1, uint8_t ss2)
{
  uint8_t result = 0;

  if (ss0 & 0x0F)
  {
    result |= 0x1;
  }
  if ((ss0 >> 4) & 0x0F)
  {
    result |= 0x2;
  } 
  if (ss1 & 0x0F)
  {
    result |= 0x4;
  } 
  if (ss2 & 0x0F)
  {
    result |= 0x8;
  } 
  return result;
}
