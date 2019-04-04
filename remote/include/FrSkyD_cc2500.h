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

enum {
  FRSKY_BIND        = 0,
  FRSKY_BIND_DONE   = 1000,
  FRSKY_DATA1,
  FRSKY_DATA2,
  FRSKY_DATA3,
  FRSKY_DATA4,
  FRSKY_DATA5
};

#define MAX_PKT 29
extern uint8_t  rx_tx_addr[5];
extern uint8_t pkt[MAX_PKT];//telemetry receiving packets
extern  uint8_t pktt[MAX_PKT];//telemetry receiving packets
extern uint8_t v_lipo1;
extern uint8_t v_lipo2;
extern uint8_t RX_RSSI;
extern uint8_t TX_RSSI;
extern uint8_t RX_LQI;
extern uint8_t TX_LQI;
extern uint8_t telemetry_link;
extern uint8_t telemetry_lost;
extern uint8_t telemetry_counter;

extern uint8_t freq_offset;
extern uint16_t state;

void Frsky_init_hop(void);
void FRSKY_init_cc2500(const uint8_t *ptr);

void frsky2way_init(uint8_t bind);

void frsky2way_build_bind_packet();

void frsky2way_data_frame(void);

uint16_t initFrSky_2way(void);
uint16_t ReadFrSky_2way(void);
uint16_t ReadFrSky_2way_bind(void);

uint16_t convert_channel_frsky(uint8_t num);
void set_rx_tx_addr(uint32_t id);
#endif
