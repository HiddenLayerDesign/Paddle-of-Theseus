/******************************************************* 
 *  File: ITG3200.h
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
#ifndef __ITG3200_H__
#define __ITG3200_H__
 
#include "Arduino.h"

/* ITG3200 Slave Addr and simple WHOAMI response */
#define ITG3200_SLAVE_ADDR      (0xD0 >> 1)
#define ITG3200_WHOMAI_VAL      0x68

/* ITG3200 Registers */
#define ITG3200_WHOAMI_REG      0x00
#define ITG3200_SMPLRT_DIV_REG  0x15
#define ITG3200_DLPF_FS_REG     0x16
#define ITG3200_INT_CFG_REG     0x17
#define ITG3200_INT_STATUS_REG  0x1A
#define ITG3200_TEMP_OUT_H_REG  0x1B
#define ITG3200_TEMP_OUT_L_REG  0x1C
#define ITG3200_GYRO_XOUT_H_REG 0x1D
#define ITG3200_GYRO_XOUT_L_REG 0x1E
#define ITG3200_GYRO_YOUT_H_REG 0x1F
#define ITG3200_GYRO_YOUT_L_REG 0x20
#define ITG3200_GYRO_ZOUT_H_REG 0x21
#define ITG3200_GYRO_ZOUT_L_REG 0x22
#define ITG3200_PWR_MGM_REG     0x3E



#endif // __ITG3200_H__
