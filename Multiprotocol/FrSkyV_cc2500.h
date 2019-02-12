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

#ifndef _FRSKYV_CC2500_H_
#define _FRSKYV_CC2500_H_

#define FRSKYV_BIND_COUNT 200
#include <stdint.h>
#include "Multiprotocol.h"

enum frskyv_data_e {
    FRSKYV_DATA1=0,
    FRSKYV_DATA2,
    FRSKYV_DATA3,
    FRSKYV_DATA4,
    FRSKYV_DATA5
};

uint8_t FRSKYV_crc8(uint8_t result, uint8_t *data, uint8_t len);

uint8_t FRSKYV_crc8_le(uint8_t *data, uint8_t len);
void FRSKYV_build_bind_packet(void);
uint8_t FRSKYV_calc_channel(void);
void FRSKYV_build_data_packet(void);
uint16_t ReadFRSKYV(void);
uint16_t initFRSKYV(void);
#endif
