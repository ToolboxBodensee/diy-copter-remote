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

void InitChannel(void);

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

/******************************/
/**  FrSky V, D and X routines  **/
/******************************/
#if defined(FRSKYV_CC2500_INO) || defined(FRSKYD_CC2500_INO) || defined(FRSKYX_CC2500_INO)

    extern uint8_t FRSKY_common_startreg_cc2500_conf[];

    extern uint8_t FRSKYD_cc2500_conf[];


    extern uint8_t FRSKY_common_end_cc2500_conf[][2];
    void FRSKY_init_cc2500(const uint8_t *ptr);

#endif

#endif
