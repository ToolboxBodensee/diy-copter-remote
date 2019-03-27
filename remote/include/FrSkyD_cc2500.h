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

#ifndef _FRSKYD_CC2500_H_
#define _FRSKYD_CC2500_H_

#include <stdint.h>
void Frsky_init_hop(void);
void frsky2way_init(uint8_t bind);

void frsky2way_build_bind_packet();

void frsky2way_data_frame(void);

uint16_t initFrSky_2way(void);
uint16_t ReadFrSky_2way(void);

uint16_t convert_channel_frsky(uint8_t num);
#endif
