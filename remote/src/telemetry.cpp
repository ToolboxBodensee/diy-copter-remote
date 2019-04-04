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
//**************************
// Telemetry serial code   *
//**************************
#include "telemetry.h"
#include "FrSkyD_cc2500.h"
#include "debug.h"
uint8_t RetrySequence ;

#if ( defined(MULTI_TELEMETRY) || defined(MULTI_STATUS) )
    #define MULTI_TIME              500 //in ms
    #define INPUT_SYNC_TIME         100 //in ms
    #define INPUT_ADDITIONAL_DELAY  100 // in 10µs, 100 => 1000 µs
    uint32_t lastMulti = 0;
#endif // MULTI_TELEMETRY/MULTI_STATUS

#if defined SPORT_TELEMETRY
    #define SPORT_TIME 12000    //12ms
    #define FRSKY_SPORT_PACKET_SIZE   8
    #define FX_BUFFERS  4
    uint32_t last = 0;
    uint8_t sport_counter=0;
    uint8_t RxBt = 0;
    uint8_t sport = 0;
    uint8_t pktx1[FRSKY_SPORT_PACKET_SIZE*FX_BUFFERS];

    // Store for out of sequence packet
    uint8_t FrskyxRxTelemetryValidSequence ;
    struct t_fx_rx_frame
    {
        uint8_t valid ;
        uint8_t count ;
        uint8_t payload[6] ;
    } ;

    // Store for FrskyX telemetry
    struct t_fx_rx_frame FrskyxRxFrames[4] ;
    uint8_t NextFxFrameToForward ;
    #ifdef SPORT_POLLING
        uint8_t sport_rx_index[28] ;
        uint8_t ukindex ;
        uint8_t kindex ;
        uint8_t TxData[2];
        uint8_t SportIndexPolling;
        uint8_t RxData[16] ;
        volatile uint8_t RxIndex=0 ;
        uint8_t sport_bytes=0;
        uint8_t skipped_id;
        uint8_t rx_counter=0;
    #endif
#endif // SPORT_TELEMETRY

#if defined HUB_TELEMETRY
    #define USER_MAX_BYTES 6
    uint8_t prev_index;
#endif // HUB_TELEMETRY

#define START_STOP  0x7e
#define BYTESTUFF   0x7d
#define STUFF_MASK  0x20
#define MAX_PKTX    10
uint8_t pktx[MAX_PKTX];
uint8_t indx;
uint8_t frame[18];

#if ( defined(MULTI_TELEMETRY) || defined(MULTI_STATUS) )
static void multi_send_header(uint8_t type, uint8_t len)
{
    Serial_write('M');
    #ifdef MULTI_TELEMETRY
        Serial_write('P');
        Serial_write(type);
    #else
        (void)type;
    #endif
    Serial_write(len);
}

#endif

#ifdef MULTI_TELEMETRY
static void multi_send_frskyhub()
{
    multi_send_header(MULTI_TELEMETRY_HUB, 9);
    for (uint8_t i = 0; i < 9; i++)
        Serial_write(frame[i]);
}
#endif


void frsky_check_telemetry(uint8_t *pkt,uint8_t len)
{
    uint8_t clen = pkt[0] + 3 ;
    if (len != clen)  // wrong length
        return;
    if(pkt[1] == rx_tx_addr[3] && pkt[2] == rx_tx_addr[2] ) {
        telemetry_link |= 1;                              // Telemetry data is available
        TX_RSSI = pkt[len-2];

        if(TX_RSSI >=128)
            TX_RSSI -= 128;
        else
            TX_RSSI += 128;

        TX_LQI = pkt[len-1]&0x7F;
        for (uint8_t i=3;i<len-2;i++)
            pktt[i]=pkt[i];                             // Buffer telemetry values to be sent

        if(pktt[6]>0 && pktt[6]<=10) {
            if ( ( pktt[7] & 0x1F ) == (telemetry_counter & 0x1F) ) {
                uint8_t topBit = 0 ;
                if ( telemetry_counter & 0x80 )
                    if ( ( telemetry_counter & 0x1F ) != RetrySequence )
                        topBit = 0x80 ;
                telemetry_counter = ( (telemetry_counter+1)%32 ) | topBit ; // Request next telemetry frame
            } else {
                // incorrect sequence
                RetrySequence = pktt[7] & 0x1F ;
                telemetry_counter |= 0x80 ;
                pktt[6]=0 ;                         // Discard current packet and wait for retransmit
            }
        } else
            pktt[6]=0;                                  // Discard packet
    }
}

void print_frskyd_telemetry() {
    debug("telemetry_link    %d\n", telemetry_link);
    debug("telemetry_counter %d\n", telemetry_counter);
    debug("v_lipo1           %d\n", v_lipo1 );
    debug("v_lipo2           %d\n", v_lipo2 );
    debug("RX_RSSI           %d\n", RX_RSSI );
    debug("TX_RSSI           %d\n", TX_RSSI );
    debug("RX_LQI            %d\n", RX_LQI  );
    debug("TX_LQI            %d\n", TX_LQI  );
}
void init_frskyd_link_telemetry()
{
    telemetry_link=0;
    telemetry_counter=0;
    v_lipo1=0;
    v_lipo2=0;
    RX_RSSI=0;
    TX_RSSI=0;
    RX_LQI=0;
    TX_LQI=0;
}

#if defined SPORT_TELEMETRY
/* SPORT details serial
            100K 8E2 normal-multiprotocol
            -every 12ms-or multiple of 12; %36
            1  2  3  4  5  6  7  8  9  CRC DESCR
            7E 98 10 05 F1 20 23 0F 00 A6 SWR_ID
            7E 98 10 01 F1 33 00 00 00 C9 RSSI_ID
            7E 98 10 04 F1 58 00 00 00 A1 BATT_ID
            7E BA 10 03 F1 E2 00 00 00 18 ADC2_ID
            7E BA 10 03 F1 E2 00 00 00 18 ADC2_ID
            7E BA 10 03 F1 E2 00 00 00 18 ADC2_ID
            7E BA 10 03 F1 E2 00 00 00 18 ADC2_ID
            7E BA 10 03 F1 E2 00 00 00 18 ADC2_ID
            7E BA 10 03 F1 E2 00 00 00 18 ADC2_ID


            Telemetry frames(RF) SPORT info
            15 bytes payload
            SPORT frame valid 6+3 bytes
            [00] PKLEN  0E 0E 0E 0E
            [01] TXID1  DD DD DD DD
            [02] TXID2  6D 6D 6D 6D
            [03] CONST  02 02 02 02
            [04] RS/RB  2C D0 2C CE //D0;CE=2*RSSI;....2C = RX battery voltage(5V from Bec)
            [05] HD-SK  03 10 21 32 //TX/RX telemetry hand-shake bytes
            [06] NO.BT  00 00 06 03 //No.of valid SPORT frame bytes in the frame
            [07] STRM1  00 00 7E 00
            [08] STRM2  00 00 1A 00
            [09] STRM3  00 00 10 00
            [10] STRM4  03 03 03 03
            [11] STRM5  F1 F1 F1 F1
            [12] STRM6  D1 D1 D0 D0
            [13] CHKSUM1 --|2 CRC bytes sent by RX (calculated on RX side crc16/table)
            [14] CHKSUM2 --|
            +2  appended bytes automatically  RSSI and LQI/CRC bytes(len=0x0E+3);

0x06    0x06    0x06    0x06    0x06

0x7E    0x00    0x03    0x7E    0x00
0x1A    0x00    0xF1    0x1A    0x00
0x10    0x00    0xD7    0x10    0x00
0x03    0x7E    0x00    0x03    0x7E
0xF1    0x1A    0x00    0xF1    0x1A
0xD7    0x10    0x00    0xD7    0x10

0xE1    0x1C    0xD0    0xEE    0x33
0x34    0x0A    0xC3    0x56    0xF3

        */
#if defined SPORT_POLLING || defined MULTI_TELEMETRY
const uint8_t PROGMEM Indices[] = { 0x00, 0xA1, 0x22, 0x83, 0xE4, 0x45,
                                    0xC6, 0x67, 0x48, 0xE9, 0x6A, 0xCB,
                                    0xAC, 0x0D, 0x8E, 0x2F, 0xD0, 0x71,
                                    0xF2, 0x53, 0x34, 0x95, 0x16, 0xB7,
                                    0x98, 0x39, 0xBA, 0x1B } ;
#endif



#endif

/**************************/
/**************************/
/**  Serial TX routines  **/
/**************************/
/**************************/
