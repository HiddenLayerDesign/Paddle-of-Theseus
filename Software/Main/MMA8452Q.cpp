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
#include <Wire.h>


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
    Serial.println("INFO: MMA8452Q is online...");
    return 1;
  }
  else
  {
    Serial.print("WARNING: Could not connect to MMA8452Q: 0x");
    Serial.println(c, HEX);
    return 0;
  }

}

void MMA8452Q::print_accel(void)
{
    Serial.print("INFO: X_ACCEL: ");
    Serial.print(x, 4);
    Serial.print(", Y_ACCEL: ");
    Serial.print(y, 4);
    Serial.print(", Z_ACCEL: ");
    Serial.print(z, 4);
    Serial.println();
}
