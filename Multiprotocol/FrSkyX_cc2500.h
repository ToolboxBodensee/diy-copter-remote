/*  **************************
    * By Midelic on RCGroups *
    **************************
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

#ifndef _FRSKYX_CC2500_H_
#define _FRSKYX_CC2500_H_
#include <stdint.h>

#define FRX_FAILSAFE_TIMEOUT 1032
#define FRX_FAILSAFE_TIME 1032

void frskyX_set_start(uint8_t ch);
void frskyX_init(void);
void frskyX_initialize_data(uint8_t adr);
uint16_t frskyX_CRCTable(uint8_t val);
uint16_t frskyX_crc_x(uint8_t *data, uint8_t len);
void frskyX_build_bind_packet(void);
uint16_t frskyX_scaleForPXX(uint8_t i);
uint16_t frskyX_scaleForPXX_FS(uint8_t i);

void frskyX_data_frame(void);
uint16_t ReadFrSkyX(void);
uint16_t initFrSkyX(void);
#endif
