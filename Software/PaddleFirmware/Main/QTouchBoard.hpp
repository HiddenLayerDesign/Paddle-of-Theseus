/*!
 * @file QTouchBoard.hpp
 * 
 * \brief Header for class to control pair of capacitive touch board for fretBoard and strumBoard
 *
 * Author: Chase E. Stewart for Hidden Layer Design
 *
 */
#ifndef __QTOUCHBOARD_HPP__
#define __QTOUCHBOARD_HPP__

#include <Wire.h>

#include  "I2CHandler.hpp"

#define QTOUCH2120_ADDR  0x1C  ///< Static I2C address for AT42QT2120 part
#define QTOUCH1070_ADDR  0x1B  ///< Static I2C address for AT42QT1070 part

#define REG_QT1070_CHIP_ID 0  ///< AT42QT1070 CHIP_ID register
#define VAL_QT1070_CHIP_ID 0x2E ///< AT42QT1070 Expected response for reading CHIP_ID register
#define REG_QT1070_VERSION 1  ///< AT42QT1070 VERSION register
#define REG_QT1070_KEY_STATUS_0 3  ///< AT42QT1070 KEY_STATUS register

#define REG_QT1070_INTEGRATION  46  ///< TODO figure out why this gets its own definition
#define REG_QT1070_AVE_AKS  39  ///< TODO figure out why this gets its own definition

#define REG_QT2120_CHIP_ID 0  ///< AT42QT2120 CHIP_ID register 
#define VAL_QT2120_CHIP_ID 0x3E  ///< AT42QT2120 Expected response for reading CHIP_ID register
#define REG_QT2120_VERSION 1  ///< AT42QT2120 VERSION register
#define REG_QT2120_KEY_STATUS_0 3  ///< AT42QT2120 KEY_STATUS register
#define REG_QT2120_KEY_STATUS_1 4  ///< AT42QT2120 KEY_STATUS register


/**************************************************************************/
/*!
    @brief  Class for interacting with QTouch I2C devices
    This class interfaces with the HiddenLayerDesign custom QTouch-powered I2C 
    boards on Wire and Wire1 used for the FretBoard and StrumBoard. 
    Despite their unique layouts, the schematics and partlists of the two boards 
    are identical, so making a common class saves a ton of code
*/
/**************************************************************************/
class QTouchBoard 
{
  private:
    int _intPin1070;  ///< GPIO interrupt pin for AT42QT1070
    int _intPin2120;  ///< GPIO interrupt pin for AT42QT2120
    I2CHandler QT2120Handler;
    I2CHandler QT1070Handler;
    
    void _InitQT1070(void);
    void _InitQT2120(void);

  public:
    QTouchBoard(int int1070, int int2120);
    void begin(TwoWire &inStream);
    ~QTouchBoard();
    void initQTouch(void);
    bool isValueUpdate(void);
    uint8_t GetMostSigMask(void);
    uint8_t GetMiddleMask(void);
    uint8_t GetLeastSigMask(void);
    uint8_t ReturnStrumKey(uint8_t ss0, uint8_t ss1, uint8_t ss2);
    uint8_t ReturnFret(uint8_t ks0, uint8_t ks1, uint8_t ks2);
};

#endif  // __QTOUCHBOARD_HPP__
