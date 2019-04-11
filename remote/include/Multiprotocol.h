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

#ifndef _MULTIPROTOCOL_H_
#define _MULTIPROTOCOL_H_
//******************
// Version
//******************
#define NONE        0
#include "tx_def.h"

#include "stdint.h"

void set_rx_tx_addr(uint32_t id);
extern uint8_t protocol_flags;
extern uint8_t protocol_flags2;
extern uint8_t  crc8;
extern uint8_t  packet_count;
extern uint8_t  packet[40];
extern uint8_t option;
extern uint16_t state;
extern uint8_t  len;

// Telemetry

enum MultiPacketTypes
{
    MULTI_TELEMETRY_STATUS          = 1,
    MULTI_TELEMETRY_SPORT           = 2,
    MULTI_TELEMETRY_HUB             = 3,
    MULTI_TELEMETRY_DSM             = 4,
    MULTI_TELEMETRY_DSMBIND         = 5,
    MULTI_TELEMETRY_AFHDS2A         = 6,
    MULTI_TELEMETRY_CONFIG          = 7,
    MULTI_TELEMETRY_SYNC            = 8,
    MULTI_TELEMETRY_SPORT_POLLING   = 9,
    MULTI_TELEMETRY_HITEC           = 10,
};

// Macros
#ifndef _BV
#define _BV(i)  (1<<i)
#endif

//***************
//***  Flags  ***
//***************
//
#define CHANGE_PROTOCOL_FLAG_on     protocol_flags |= _BV(1)
#define CHANGE_PROTOCOL_FLAG_off    protocol_flags &= ~_BV(1)
#define IS_CHANGE_PROTOCOL_FLAG_on  ( ( protocol_flags & _BV(1) ) !=0 )
//
#define IS_POWER_FLAG_on    (true)
//
#define RANGE_FLAG_on       protocol_flags |= _BV(3)
#define RANGE_FLAG_off      protocol_flags &= ~_BV(3)
#define IS_RANGE_FLAG_on    ( ( protocol_flags & _BV(3) ) !=0 )
//
#define AUTOBIND_FLAG_on    protocol_flags |= _BV(4)
#define AUTOBIND_FLAG_off   protocol_flags &= ~_BV(4)
#define IS_AUTOBIND_FLAG_on ( ( protocol_flags & _BV(4) ) !=0 )
//
#define BIND_BUTTON_FLAG_on     protocol_flags |= _BV(5)
#define BIND_BUTTON_FLAG_off    protocol_flags &= ~_BV(5)
//Bind flag
#define BIND_IN_PROGRESS    protocol_flags &= ~_BV(7)
#define BIND_DONE           protocol_flags |= _BV(7)
#define IS_BIND_DONE        ( ( protocol_flags & _BV(7) ) !=0 )
#define IS_BIND_IN_PROGRESS ( ( protocol_flags & _BV(7) ) ==0 )
//
#define RX_MISSED_BUFF_off  protocol_flags2 &= ~_BV(2)
#define RX_MISSED_BUFF_on   protocol_flags2 |= _BV(2)
#define IS_RX_MISSED_BUFF_on    ( ( protocol_flags2 & _BV(2) ) !=0 )
//Bind from channel
#define BIND_CH_PREV_off    protocol_flags2 &= ~_BV(6)
#define BIND_CH_PREV_on     protocol_flags2 |= _BV(6)
#define IS_BIND_CH_PREV_on  ( ( protocol_flags2 & _BV(6) ) !=0 )
#define IS_BIND_CH_PREV_off ( ( protocol_flags2 & _BV(6) ) ==0 )
//Wait for bind
#define WAIT_BIND_off       protocol_flags2 &= ~_BV(7)
#define WAIT_BIND_on        protocol_flags2 |= _BV(7)
#define IS_WAIT_BIND_on     ( ( protocol_flags2 & _BV(7) ) !=0 )
#define IS_WAIT_BIND_off    ( ( protocol_flags2 & _BV(7) ) ==0 )


#include "debug.h"

#endif
