/******************************************************* 
 *  File: MMA8452Q.h
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)    
 *  
 *******************************************************/
#ifndef __MMA8452Q_H__
#define __MMA8452Q_H__
 
#include "Arduino.h"

/* MMA8452Q Slave Addr and simple WHOAMI response */
#define MMA8452Q_SLAVE_ADDR     0x1D
#define MMA8452Q_WHOAMI_REG     0x0D
#define MMA8452Q_WHOAMI_VAL     0x2A

/* MMA8452Q Registers */
#define MMA8452Q_CTRL1_REG        0x2A
#define MMA8452Q_XYZ_DATA_CFG_REG 0x0E
#define MMA8452Q_OUT_X_MSB_REG    0x01

#define GSCALE 2 // Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.

class MMA8452Q
{
  public:
    MMA8452Q(void);
    int8_t init(void);
    void accel_update(void);
    void print_accel(void);
    bool is_lefty_flipped(void);
    float x;
    float y;
    float z;
  private:
    int8_t MMA8452Q_set_registers(); 
    uint8_t _slave_addr;
};

#endif /* __MMA8452Q_H__ */
