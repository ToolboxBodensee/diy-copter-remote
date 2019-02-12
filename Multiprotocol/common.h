/*
  This project is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Multiprotocol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Multiprotocol.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef  _COMMON_H_
#define  _COMMON_H_
#include <stdint.h>
#include "config.h"
#include "tx_def.h"

void InitFailsafe(void);
void InitPPM(void);
void InitChannel(void);
void reverse_channel(uint8_t num);
uint16_t convert_channel_ppm(uint8_t num);
uint16_t convert_channel_10b(uint8_t num);
uint8_t convert_channel_8b(uint8_t num);
int16_t convert_channel_16b_limit(uint8_t num, int16_t min, int16_t max);
int16_t convert_channel_16b_nolimit(uint8_t num, int16_t min, int16_t max);
uint8_t convert_channel_s8b(uint8_t num);
uint16_t limit_channel_100(uint8_t num);
void convert_channel_HK310(uint8_t num, uint8_t *low, uint8_t *high);
void convert_failsafe_HK310(uint8_t num, uint8_t *low, uint8_t *high);
uint16_t convert_channel_frsky(uint8_t num);

/******************************/
/**  FrSky D and X routines  **/
/******************************/
enum {
  FRSKY_BIND        = 0,
  FRSKY_BIND_DONE   = 1000,
  FRSKY_DATA1,
  FRSKY_DATA2,
  FRSKY_DATA3,
  FRSKY_DATA4,
  FRSKY_DATA5
};

void Frsky_init_hop(void);
/******************************/
/**  FrSky V, D and X routines  **/
/******************************/
#if defined(FRSKYV_CC2500_INO) || defined(FRSKYD_CC2500_INO) || defined(FRSKYX_CC2500_INO)

    extern uint8_t FRSKY_common_startreg_cc2500_conf[];
    #if defined(FRSKYV_CC2500_INO)
    extern uint8_t FRSKYV_cc2500_conf[];
    #endif

    #if defined(FRSKYD_CC2500_INO)
    extern uint8_t FRSKYD_cc2500_conf[];
    #endif

    #if defined(FRSKYX_CC2500_INO)
    extern uint8_t FRSKYX_cc2500_conf[];
    extern uint8_t FRSKYXEU_cc2500_conf[];
    #endif

    extern uint8_t FRSKY_common_end_cc2500_conf[][2];
    void FRSKY_init_cc2500(const uint8_t *ptr);

    #ifdef FAILSAFE_ENABLE
    extern uint16_t Failsafe_data[NUM_CHN];
    #endif
#endif

#endif
