/******************************************************************************
NOTE: this code has been edited from the below, original Sparkfun header starts from
 second set of asterisks!
******************************************************************************/

/******************************************************************************
SparkFun_MMA8452Q.cpp
SparkFun_MMA8452Q Library Source File
Jim Lindblom @ SparkFun Electronics
Original Creation Date: June 3, 2014
https://github.com/sparkfun/MMA8452_Accelerometer
This file implements all functions of the MMA8452Q class. Functions here range
from higher level stuff, like reading/writing MMA8452Q registers to low-level,
hardware I2C reads and writes.
Development environment specifics:
  IDE: Arduino 1.0.5
  Hardware Platform: Arduino Uno
  **Updated for Arduino 1.6.4 5/2015**
  
This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!
Distributed as-is; no warranty is given.
******************************************************************************/
#include <Wire.h>

#include "Arduino.h"
#include "MMA8452Q.hpp"
#include "BoardLayout.hpp"

/**
 * Instantiate class by opening wire connection and setting slave address
 */
MMA8452Q::MMA8452Q()
{
  this->_MMAHandler = I2CHandler();
  Wire1.begin();
  this->_MMAHandler.begin(Wire1, MMA8452Q_SLAVE_ADDR);
  this->_x = 0;
  this->_y = 0;
  this->_z = 0;
}

/** 
 * Sets the MMA8452 to standby mode. It must be in standby to change most register settings 
 */
void MMA8452Q::_MMA8452Standby(void)
{
  byte c = this->_MMAHandler.ReadSingleRegister(MMA8452Q_CTRL1_REG);
  this->_MMAHandler.WriteSingleRegister(MMA8452Q_CTRL1_REG, c & ~(0x01)); //Clear the active bit to go into standby
}

/**
 *  Sets the MMA8452 to active mode. Needs to be in this mode to output data
 */
void MMA8452Q::_MMA8452Active(void)
{
  byte c = this->_MMAHandler.ReadSingleRegister(MMA8452Q_CTRL1_REG);
  this->_MMAHandler.WriteSingleRegister(MMA8452Q_CTRL1_REG, c | 0x01); //Set the active bit to begin detection
}

/**
 * Test and set up accelerometer 
 * 
 * @return 0 == failure, else 1
 */
int8_t MMA8452Q::init(void)
{
  byte c = this->_MMAHandler.ReadSingleRegister(MMA8452Q_WHOAMI_REG);  // Read WHOAMI register
  if (c == MMA8452Q_WHOAMI_VAL) // WHOAMI should always be 0x2A
  {  
    DEBUG_PRINTLN("INFO: MMA8452Q is online...");
  }
  else
  {
    DEBUG_PRINT("WARNING: Could not connect to MMA8452Q: 0x");
    DEBUG_PRINTLN_HEX(c);
    DEBUG_PRINTLN("WARNING: Failed to init accelerometer!");  
    return 0;
  }
  
  this->_MMA8452Standby();  // Must be in standby to change registers

  // Set up the full scale range to 2, 4, or 8g.
  byte fsr = GSCALE;
  if(fsr > 8) fsr = 8; //Easy error check
  fsr >>= 2; // Neat trick, see page 22. 00 = 2G, 01 = 4A, 10 = 8G
  this->_MMAHandler.WriteSingleRegister(MMA8452Q_XYZ_DATA_CFG_REG, fsr);

  //The default data rate is 800Hz and we don't modify it in this example code

  this->_MMA8452Active();  // Set to active to start reading
  return 1;
}

/**
 * Update the accelerometer values, store in class vars x,y,z 
 */
void MMA8452Q::accel_update()
{
  byte rawData[6];  // x/y/z accel register data stored here

  this->_MMAHandler.ReadRegisters(MMA8452Q_OUT_X_MSB_REG, 6, rawData);  // Read the six raw data registers into data array
  
  this->_x = ((short)(rawData[0]<<8 | rawData[1])) >> 4;
  this->_y = ((short)(rawData[2]<<8 | rawData[3])) >> 4;
  this->_z = ((short)(rawData[4]<<8 | rawData[5])) >> 4;
}

/**
 * Print the Acceleration
 */
void MMA8452Q::print_accel(void)
{  
    DEBUG_PRINT("\rINFO: X_ACCEL: ");
    DEBUG_PRINT_DEC4(this->_x);

    DEBUG_PRINT(", Y_ACCEL: ");
    DEBUG_PRINT_DEC4(this->_y);

    DEBUG_PRINT(", Z_ACCEL: ");
    DEBUG_PRINT_DEC4(this->_z);
    DEBUG_PRINTLN("\n");
}

/**
 * accelerometer value of (y > 0) implies a left-handed person
 * is using the instrument
 *
 * NOTE: Of course the lefty-flipped value depends on orientation of the MMA8452Q chip
 * So final value of this is subject to the final install orientation of the project
 * 
 * @return bool True == The paddle is lefty flipped, else False
 */
bool MMA8452Q::is_lefty_flipped(void)
{
  return (this->_y > 0);
}
