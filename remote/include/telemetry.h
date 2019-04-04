#ifndef __TELE_METRY_H_
#define __TELE_METRY_H_
#include <Arduino.h>

#if ( defined(MULTI_TELEMETRY) || defined(MULTI_STATUS) )
static void multi_send_header(uint8_t type, uint8_t len);
#endif

#ifdef MULTI_TELEMETRY
static void multi_send_frskyhub();
#endif


void frsky_check_telemetry(uint8_t *pkt,uint8_t len);

void print_frskyd_telemetry();
void init_frskyd_link_telemetry();
#endif
