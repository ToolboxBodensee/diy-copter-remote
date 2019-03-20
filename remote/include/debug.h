#ifndef _DEBUG_H_
#define _DEBUG_H_
#include "Arduino.h"
#include <stdio.h>

//********************
//** Debug messages **
//********************
#define ENABLE_DBEUG // ~1k
#ifdef ENABLE_DBEUG
  #define debug(msg, ...)  {  char buf[256]; sprintf(buf, msg, ##__VA_ARGS__); Serial.print(buf);}
  #define debugln(msg, ...)  {  char buf[256];   sprintf(buf, msg "\r\n", ##__VA_ARGS__);  Serial.println(buf);}
  #define debug_time(msg)  { uint16_t debug_time_TCNT1=TCNT1; debug_time=debug_time_TCNT1-debug_time; debugln(msg "%u", debug_time); debug_time=debug_time_TCNT1; }

  #define debugln_input(msg, ...)  {  char buf[256];   sprintf(buf, "input:" msg "\r\n", ##__VA_ARGS__);  Serial.println(buf);}
  #define debug_input(msg, ...)  {  char buf[256];   sprintf(buf, "input:" msg , ##__VA_ARGS__);  Serial.println(buf);}
#else
  #define debug(msg, ...)
  #define debugln(msg, ...)
  #define debug_time(msg)

  #define debugln_input(msg, ...)
  #define debug_input(msg, ...)

#endif

#endif
