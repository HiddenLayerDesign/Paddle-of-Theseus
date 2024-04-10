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
#include "I2CHandler.hpp"
#include "BoardLayout.hpp"

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
  this->_intPin1070 = int1070;
  this->_intPin2120 = int2120;
  pinMode(this->_intPin1070, INPUT);
  pinMode(this->_intPin2120, INPUT);

  this->QT1070Handler = I2CHandler();
  this->QT2120Handler = I2CHandler();
}

/**************************************************************************/
/*!
    @brief    Take in initialized TwoWire object and add it to the QTouchBoard
*/
/**************************************************************************/
void QTouchBoard::begin(TwoWire &inStream)
{
  this->QT1070Handler.begin(inStream, QTOUCH1070_ADDR);
  this->QT2120Handler.begin(inStream, QTOUCH2120_ADDR);
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
  uint8_t chipId = QT1070Handler.ReadSingleRegister(REG_QT1070_CHIP_ID);
  if (chipId != VAL_QT2120_CHIP_ID)
  {
    DEBUG_PRINT("QT1070 chipId = 0x"); DEBUG_PRINT_HEX(chipId); DEBUG_PRINT(", should be 0x"); DEBUG_PRINTLN_HEX(VAL_QT1070_CHIP_ID);
  }

  // Get FW version
  uint8_t versionByte = QT1070Handler.ReadSingleRegister(REG_QT1070_VERSION);
  uint8_t versionMajor = (versionByte & 0xF0) >> 4;
  uint8_t versionMinor = (versionByte & 0x0F);

#ifndef DEBUG
  (void) versionMajor;
  (void) versionMinor;
#endif

  DEBUG_PRINT("Firmware version = "); DEBUG_PRINT(versionMajor); DEBUG_PRINT("."); DEBUG_PRINTLN(versionMinor);

  // Set touch integration
  for (int i=0; i<7; i++)
  {
    QT1070Handler.WriteSingleRegister(i + REG_QT1070_INTEGRATION, 4); // TODO revisit these settings
    QT1070Handler.WriteSingleRegister(i + REG_QT1070_AVE_AKS, 0x20); // TODO revisit these settings
  }

  // Set Low Power Mode
  QT1070Handler.WriteSingleRegister(54, 1); // TODO revisit these settings
}

/**************************************************************************/
/*!
    @brief    Initialize the register settings for the AT42QT2120
*/
/**************************************************************************/
void QTouchBoard::_InitQT2120()
{
  // Get Chip ID
  uint8_t chipId = QT2120Handler.ReadSingleRegister(REG_QT2120_CHIP_ID);
  if (chipId != VAL_QT2120_CHIP_ID)
  {
    DEBUG_PRINT("QT2120 chipId = 0x"); DEBUG_PRINT_HEX(chipId); DEBUG_PRINT(", should be 0x"); DEBUG_PRINTLN_HEX(VAL_QT2120_CHIP_ID);
  }

  // Get FW version
  uint8_t versionByte = QT2120Handler.ReadSingleRegister(REG_QT2120_VERSION);
  uint8_t versionMajor = (versionByte & 0xF0) >> 4;
  uint8_t versionMinor = (versionByte & 0x0F);

#ifndef DEBUG
  (void) versionMajor;
  (void) versionMinor;
#endif

  DEBUG_PRINT("Firmware version = "); DEBUG_PRINT(versionMajor); DEBUG_PRINT("."); DEBUG_PRINTLN(versionMinor);

  // Set touch integration
  QT2120Handler.WriteSingleRegister(11, 4); // TODO revisit these settings

  // Set drift hold time
  QT2120Handler.WriteSingleRegister(13, 3); // TODO revisit these settings

  // Set detect threshold
  for (int i=0; i<12; i++)
  {
      QT2120Handler.WriteSingleRegister(16+i, 19); // TODO revisit these settings
  } 
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

uint8_t QTouchBoard::GetMostSigMask(void)
{
  return QT1070Handler.ReadSingleRegister(REG_QT1070_KEY_STATUS_0);
}

uint8_t QTouchBoard::GetMiddleMask(void)
{
  return QT2120Handler.ReadSingleRegister(REG_QT2120_KEY_STATUS_1);
}

uint8_t QTouchBoard::GetLeastSigMask(void)
{
  return QT2120Handler.ReadSingleRegister(REG_QT2120_KEY_STATUS_0);
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
