/******************************************************* 
 *  File: ADXL345.cpp
 *    
 *  Author: Chase E Stewart 
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/

#include "Arduino.h"
#include "ADXL345.h"
#include "Wire.h"

const float accel_factor = 0.0078;

ADXL345::ADXL345()
{
  Wire.begin();

  _slave_addr = ADXL345_SLAVE_ADDR;
  x = 0;
  y = 0;
  z = 0;
}

int8_t ADXL345::init(void)
{
  /* Get WHOAMI value and compare to expected value */
  Wire.beginTransmission(_slave_addr);
  Wire.write(ADXL345_WHOAMI_REG);
  Wire.endTransmission();
  Wire.requestFrom(_slave_addr, 1);
  byte rx_byte = Wire.read();
  if (rx_byte != ADXL345_WHOAMI_VAL)
  {
    Serial.print("ERROR: Unexpected ADXL345 WHOAMI value: 0x");
    Serial.println(rx_byte, HEX);
    return -1;  
  }
  Wire.endTransmission();
  Serial.print("DEBUG: ADXL345 WHOAMI value matches expected value: 0x");
  Serial.println(rx_byte, HEX);

  /* Set ADXL345 Registers */
  Wire.beginTransmission(_slave_addr);
  Wire.write(ADXL345_POWER_CTL_REG);
  Wire.write(ADXL345_POWER_CTL_MEAS);
  Wire.endTransmission();
  Serial.println("DEBUG: ADXL345 POW_CTRL mode MEASURE");

  Wire.beginTransmission(_slave_addr);
  Wire.write(ADXL345_DATA_FORMAT_REG);
  Wire.write(ADXL345_RANGE_0);
  Wire.endTransmission();  
  Serial.println("DEBUG: ADXL345 DATA_FMT setting MODE_0");
  Serial.println("INFO: Accelerometer initialization complete");
  return 0;
}
   
void ADXL345::accel_update(void)
{
  uint16_t accel_x_lsb, accel_x_msb, combined_x;
  uint16_t accel_y_lsb, accel_y_msb, combined_y;
  uint16_t accel_z_lsb, accel_z_msb, combined_z;

  /* Get X-axis Acceleration */
  Wire.beginTransmission(_slave_addr);
  Wire.write(ADXL345_DATAX0_REG);
  Wire.endTransmission();
  Wire.requestFrom(_slave_addr, 2);
  accel_x_msb = (uint16_t) Wire.read();
  accel_x_lsb = (uint16_t) Wire.read();
  Wire.endTransmission();
  combined_x = (accel_x_msb << 8) | (accel_x_lsb);
  x = (float) (combined_x * accel_factor);

  /* Get Y-axis Acceleration */
  Wire.beginTransmission(_slave_addr);
  Wire.write(ADXL345_DATAY0_REG);
  Wire.endTransmission();
  Wire.requestFrom(_slave_addr, 2);
  accel_y_msb = (uint16_t) Wire.read();
  accel_y_lsb = (uint16_t) Wire.read();
  Wire.endTransmission();
  combined_y = (accel_y_msb << 8) | (accel_y_lsb);
  y = (float) (combined_y * accel_factor);

  /* Get Z-axis Acceleration */
  Wire.beginTransmission(_slave_addr);
  Wire.write(ADXL345_DATAZ0_REG);
  Wire.endTransmission();
  Wire.requestFrom(_slave_addr, 2);
  accel_z_msb = (uint16_t) Wire.read();
  accel_z_lsb = (uint16_t) Wire.read();
  Wire.endTransmission();
  combined_z = (accel_z_msb << 8) | (accel_z_lsb);
  z = (float) (combined_z * accel_factor);
}

void ADXL345::print_accel(void)
{
    Serial.print("INFO: X_ACCEL: ");
    Serial.print(x, 4);
    Serial.print(", Y_ACCEL: ");
    Serial.print(y, 4);
    Serial.print(", Z_ACCEL: ");
    Serial.print(z, 4);
    Serial.println();
}
