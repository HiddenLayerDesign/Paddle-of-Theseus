/******************************************************* 
 *  File: MMA8452Q.cpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/

#include "Arduino.h"
#include "MMA8452Q.h"
#include "TeensyBSP.h"
#include <Wire.h>

#ifdef DEBUG
#define DEBUG_PRINTLN(x)  Serial.println(x)
#define DEBUG_PRINT(x)  Serial.print(x)
#define DEBUG_PRINTLN_HEX(x) Serial.println (x, HEX)
#define DEBUG_PRINT_DEC4(x) Serial.print(x, 4)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN_HEX(x) 
#define DEBUG_PRINT_DEC4 
#endif /* DEBUG */

// Read a single byte from addressToRead and return it as a byte
byte readRegister(byte addressToRead)
{
  Wire.beginTransmission(MMA8452Q_SLAVE_ADDR);
  Wire.write(addressToRead);
  Wire.endTransmission(false); //endTransmission but keep the connection active

  Wire.requestFrom(MMA8452Q_SLAVE_ADDR, 1); //Ask for 1 byte, once done, bus is released by default

  while(!Wire.available()) ; //Wait for the data to come back
  return Wire.read(); //Return this one byte
}

// Read bytesToRead sequentially, starting at addressToRead into the dest byte array
void readRegisters(byte addressToRead, int bytesToRead, byte * dest)
{
  Wire.beginTransmission(MMA8452Q_SLAVE_ADDR);
  Wire.write(addressToRead);
  Wire.endTransmission(false); //endTransmission but keep the connection active

  Wire.requestFrom(MMA8452Q_SLAVE_ADDR, bytesToRead); //Ask for bytes, once done, bus is released by default

  while(Wire.available() < bytesToRead); //Hang out until we get the # of bytes we expect

  for(int x = 0 ; x < bytesToRead ; x++)
    dest[x] = Wire.read();    
}

// Writes a single byte (dataToWrite) into addressToWrite
void writeRegister(byte addressToWrite, byte dataToWrite)
{
  Wire.beginTransmission(MMA8452Q_SLAVE_ADDR);
  Wire.write(addressToWrite);
  Wire.write(dataToWrite);
  Wire.endTransmission(); //Stop transmitting
}

// Sets the MMA8452 to standby mode. It must be in standby to change most register settings
void MMA8452Standby()
{
  byte c = readRegister(MMA8452Q_CTRL1_REG);
  writeRegister(MMA8452Q_CTRL1_REG, c & ~(0x01)); //Clear the active bit to go into standby
}

// Sets the MMA8452 to active mode. Needs to be in this mode to output data
void MMA8452Active()
{
  byte c = readRegister(MMA8452Q_CTRL1_REG);
  writeRegister(MMA8452Q_CTRL1_REG, c | 0x01); //Set the active bit to begin detection
}

MMA8452Q::MMA8452Q()
{
  Wire.begin();
  _slave_addr = MMA8452Q_SLAVE_ADDR;
  x = 0;
  y = 0;
  z = 0;
}

int8_t MMA8452Q::init(void)
{
  byte c = readRegister(MMA8452Q_WHOAMI_REG);  // Read WHOAMI register
  if (c == MMA8452Q_WHOAMI_VAL) // WHOAMI should always be 0x2A
  {  
    DEBUG_PRINTLN("INFO: MMA8452Q is online...");
  }
  else
  {
    DEBUG_PRINT("WARNING: Could not connect to MMA8452Q: 0x");
    DEBUG_PRINTLN_HEX(c);
    return 0;
  }
  
  MMA8452Standby();  // Must be in standby to change registers

  // Set up the full scale range to 2, 4, or 8g.
  byte fsr = GSCALE;
  if(fsr > 8) fsr = 8; //Easy error check
  fsr >>= 2; // Neat trick, see page 22. 00 = 2G, 01 = 4A, 10 = 8G
  writeRegister(MMA8452Q_XYZ_DATA_CFG_REG, fsr);

  //The default data rate is 800Hz and we don't modify it in this example code

  MMA8452Active();  // Set to active to start reading
}

void MMA8452Q::print_accel(void)
{
    DEBUG_PRINT("INFO: X_ACCEL: ");
    DEBUG_PRINT_DEC4(x);
    DEBUG_PRINT(", Y_ACCEL: ");
    DEBUG_PRINT_DEC4(y);
    DEBUG_PRINT(", Z_ACCEL: ");
    DEBUG_PRINT_DEC4(z);
    DEBUG_PRINTLN();
}
