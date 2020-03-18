/******************************************************* 
 *  File: ADXL345.h
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  This design is built off of Teensy MIDI example code 
 *     and Seeed studio code for the Ultrasonic Rangefinder
 *     
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)    
 *  
 *******************************************************/
#ifndef __ADXL345_H__
#define __ADXL345_H__
 
#include "Arduino.h"

/* ADXL345 Slave Addr and simple WHOAMI response */
#define ADXL345_SLAVE_ADDR     (0xA6 >> 1)
#define ADXL345_WHOAMI_VAL     0xE5

/* ADXL345 Registers */
#define ADXL345_WHOAMI_REG      0x00
#define ADXL345_TAP_THRESH_REG  0x1D
#define ADXL345_OFST_X_REG      0x1E
#define ADXL345_OFST_Y_REG      0x1F
#define ADXL345_OFST_Z_REG      0x20
#define ADXL345_TAP_DUR_REG     0x21
#define ADXL345_POWER_CTL_REG   0x2D
#define ADXL345_DATA_FORMAT_REG 0x31
#define ADXL345_DATAX0_REG      0x32
//#define ADXL345_DATAX1_REG      0x33
#define ADXL345_DATAY0_REG      0x34
//#define ADXL345_DATAY1_REG      0x35
#define ADXL345_DATAZ0_REG      0x36
//#define ADXL345_DATAZ1_REG      0x37

/* ADXL345_POWER_CTL register values */
#define ADXL345_POWER_CTL_WU0  (1<<0)
#define ADXL345_POWER_CTL_WU1  (1<<1)
#define ADXL345_POWER_CTL_SLP  (1<<2)
#define ADXL345_POWER_CTL_MEAS (1<<3)

/* ADXL345_DATA_FORMAT values */
#define ADXL345_RANGE_0     (1<<0)
#define ADXL345_RANGE_1     (1<<1)
#define ADXL345_JUSTIFY     (1<<2)
#define ADXL345_FULL_RES    (1<<3)

class ADXL345
{
  public:
    ADXL345(void);
    int8_t init(void);
    void accel_update(void);
    uint16_t accel_x;
    uint16_t accel_y;
    uint16_t accel_z;
  private:
    int8_t ADXL345_set_registers(); 
    uint8_t _slave_addr;
};

#endif /* __ADXL345_H__ */
