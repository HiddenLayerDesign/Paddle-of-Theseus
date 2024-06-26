/******************************************************* 
 *  File: Version.hpp
 *    
 *  Author: Chase E. Stewart
 *  For Hidden Layer Design
 *  
 *  License: Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
 *
 *******************************************************/
#ifndef __VERSION_HPP__
#define __VERSION_HPP__

/**
 * Set these to set the SW version
 */
#define VERSION_MAJOR  (uint8_t) 2
#define VERSION_MINOR  (uint8_t) 0
#define VERSION_BUGFIX (uint8_t) 0

typedef struct {
  uint8_t version_major;
  uint8_t version_minor;
  uint8_t version_bugfix;
  } version_t;

#endif /* __VERSION_HPP__ */
