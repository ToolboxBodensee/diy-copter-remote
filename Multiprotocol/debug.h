#ifndef DEBUG_H
#define DEBUG_H

//********************
//** Debug messages **
//********************
  #define debug(msg, ...)  {  char buf[128]; sprintf(buf, msg, ##__VA_ARGS__); Serial.println(buf);}
  #define debugln(msg, ...)  {  char buf[128];   sprintf(buf, msg "\r\n", ##__VA_ARGS__);  Serial.println(buf);}
  #define debug_time(msg)  { uint16_t debug_time_TCNT1=TCNT1; debug_time=debug_time_TCNT1-debug_time; debugln(msg "%u", debug_time); debug_time=debug_time_TCNT1; }
#endif
