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

#include "cc2500_spi.h"
#include "Multiprotocol.h"
#include "input.h"
#include "FrSkyD_cc2500.h"
#include "telemetry.h"
uint8_t v_lipo1;
uint8_t v_lipo2;
uint8_t RX_RSSI;
uint8_t TX_RSSI;
uint8_t RX_LQI;
uint8_t TX_LQI;
uint8_t telemetry_link=0;
uint8_t telemetry_lost;
uint8_t telemetry_counter=0;
uint8_t  rx_tx_addr[5];
uint8_t  hopping_frequency[50];
uint8_t pkt[MAX_PKT];//telemetry receiving packets
uint8_t pktt[MAX_PKT];//telemetry receiving packets

uint16_t state;
uint16_t counter;

uint8_t freq_offset;
uint8_t freq_offset_prev;

const uint8_t FRSKY_common_startreg_cc2500_conf[] = {
  CC2500_02_IOCFG0 ,
  CC2500_00_IOCFG2 ,
  CC2500_17_MCSM1 ,
  CC2500_18_MCSM0 ,
  CC2500_06_PKTLEN ,
  CC2500_07_PKTCTRL1 ,
  CC2500_08_PKTCTRL0 ,
  CC2500_3E_PATABLE ,
  CC2500_0B_FSCTRL1 ,
  CC2500_0C_FSCTRL0 ,   // replaced by frequency offset value
  CC2500_0D_FREQ2 ,
  CC2500_0E_FREQ1 ,
  CC2500_0F_FREQ0 ,
  CC2500_10_MDMCFG4 ,
  CC2500_11_MDMCFG3 ,
  CC2500_12_MDMCFG2 ,
  CC2500_13_MDMCFG1 ,
  CC2500_14_MDMCFG0 ,
  CC2500_15_DEVIATN
};

const uint8_t FRSKY_common_end_cc2500_conf[][2] = {
  { CC2500_19_FOCCFG,   0x16 },
  { CC2500_1A_BSCFG,    0x6c },
  { CC2500_1B_AGCCTRL2, 0x43 },
  { CC2500_1C_AGCCTRL1, 0x40 },
  { CC2500_1D_AGCCTRL0, 0x91 },
  { CC2500_21_FREND1,   0x56 },
  { CC2500_22_FREND0,   0x10 },
  { CC2500_23_FSCAL3,   0xa9 },
  { CC2500_24_FSCAL2,   0x0A },
  { CC2500_25_FSCAL1,   0x00 },
  { CC2500_26_FSCAL0,   0x11 },
  { CC2500_29_FSTEST,   0x59 },
  { CC2500_2C_TEST2,    0x88 },
  { CC2500_2D_TEST1,    0x31 },
  { CC2500_2E_TEST0,    0x0B },
  { CC2500_03_FIFOTHR,  0x07 },
  { CC2500_09_ADDR,     0x00 }
};

const uint8_t FRSKYD_cc2500_conf[] = {
  /*02_IOCFG0*/      0x06 ,
  /*00_IOCFG2*/      0x06 ,
  /*17_MCSM1*/       0x0c ,
  /*18_MCSM0*/       0x18 ,
  /*06_PKTLEN*/      0x19 ,
  /*07_PKTCTRL1*/    0x04 ,
  /*08_PKTCTRL0*/    0x05 ,
  /*3E_PATABLE*/     0xff ,
  /*0B_FSCTRL1*/     0x08 ,
  /*0C_FSCTRL0*/     0x00 ,
  /*0D_FREQ2*/       0x5c ,
  /*0E_FREQ1*/       0x76 ,
  /*0F_FREQ0*/       0x27 ,
  /*10_MDMCFG4*/     0xAA ,
  /*11_MDMCFG3*/     0x39 ,
  /*12_MDMCFG2*/     0x11 ,
  /*13_MDMCFG1*/     0x23 ,
  /*14_MDMCFG0*/     0x7a ,
  /*15_DEVIATN*/     0x42
};

void frsky2way_init(uint8_t bind)
{
    //debugln("frsky2way_init");
    FRSKY_init_cc2500(FRSKYD_cc2500_conf);

    CC2500_WriteReg(CC2500_09_ADDR, bind ? 0x03 : rx_tx_addr[3]);
    CC2500_WriteReg(CC2500_07_PKTCTRL1, 0x05);
    CC2500_Strobe(CC2500_SIDLE);    // Go to idle...

    CC2500_WriteReg(CC2500_0A_CHANNR, 0x00);
    CC2500_WriteReg(CC2500_23_FSCAL3, 0x89);
    CC2500_Strobe(CC2500_SFRX);
    //#######END INIT########
}

void frsky2way_build_bind_packet(void)
{
  //debugln("build bind");
    //11 03 01 d7 2d 00 00 1e 3c 5b 78 00 00 00 00 00 00 01
    //11 03 01 19 3e 00 02 8e 2f bb 5c 00 00 00 00 00 00 01
    packet[0] = 0x11;
    packet[1] = 0x03;
    packet[2] = 0x01;
    packet[3] = rx_tx_addr[3];
    packet[4] = rx_tx_addr[2];
    uint16_t idx = ((state - FRSKY_BIND) % 10) * 5;
    packet[5] = idx;
    packet[6] = hopping_frequency[idx++];
    packet[7] = hopping_frequency[idx++];
    packet[8] = hopping_frequency[idx++];
    packet[9] = hopping_frequency[idx++];
    packet[10] = hopping_frequency[idx++];
    packet[11] = 0x00;
    packet[12] = 0x00;
    packet[13] = 0x00;
    packet[14] = 0x00;
    packet[15] = 0x00;
    packet[16] = 0x00;
    packet[17] = 0x01;
}

void frsky2way_data_frame()
{
  //pachet[4] is telemetry user frame counter(hub)
    //11 d7 2d 22 00 01 c9 c9 ca ca 88 88 ca ca c9 ca 88 88
    //11 57 12 00 00 01 f2 f2 f2 f2 06 06 ca ca ca ca 18 18
    packet[0] = 0x11;             //Length
    packet[1] = rx_tx_addr[3];
    packet[2] = rx_tx_addr[2];
    packet[3] = counter;//
    #if defined TELEMETRY
        packet[4] = telemetry_counter;
    #else
        packet[4] = 0x00;
    #endif

    packet[5] = 0x01;
    //
    packet[10] = 0;
    packet[11] = 0;
    packet[16] = 0;
    packet[17] = 0;
    for(uint8_t i = 0; i < 8; i++) {
        uint16_t value;
        value = convert_channel_frsky(i);
        if(i < 4) {
            packet[6+i] = value & 0xff;
            packet[10+(i>>1)] |= ((value >> 8) & 0x0f) << (4 *(i & 0x01));
        } else {
            packet[8+i] = value & 0xff;
            packet[16+((i-4)>>1)] |= ((value >> 8) & 0x0f) << (4 * ((i-4) & 0x01));
        }
    }
}

uint16_t initFrSky_2way()
{
    Frsky_init_hop();
    packet_count=0;

    if(IS_BIND_IN_PROGRESS)
    {
        state = FRSKY_BIND;
        debugln("initFrSky_2way bind");
    } else {
        state = FRSKY_BIND_DONE;
        debugln("initFrSky_2way bind done");
    }
    return 10000;
}
uint16_t ReadFrSky_2way_bind(void)
{
    frsky2way_build_bind_packet();
    CC2500_Strobe(CC2500_SIDLE);
    CC2500_WriteReg(CC2500_0A_CHANNR, 0x00);
    CC2500_WriteReg(CC2500_23_FSCAL3, 0x89);
    CC2500_Strobe(CC2500_SFRX);//0x3A
    CC2500_WriteData(packet, packet[0]+1);

    if(IS_BIND_DONE) {
        state = FRSKY_BIND;
        debugln("%s bind done",__func__);
    } else {
        state++;
    }

    if(state == FRSKY_BIND_DONE) {
        /* reset state again */
        state = 0;
        debugln("%s bind done fr",__func__);
    }
    return 9000;
}

uint16_t ReadFrSky_2way()
{
    if (state <= FRSKY_BIND_DONE) {

        frsky2way_init(0);
        //debugln("%s bind done",__func__);

        state = FRSKY_DATA2;
        counter = 0;
    } else if (state == FRSKY_DATA5) {
        CC2500_Strobe(CC2500_SRX);//0x34 RX enable
        state = FRSKY_DATA1;
        return 9200;
    }

    counter = (counter + 1) % 188;
    if (state == FRSKY_DATA4) {   //telemetry receive
        CC2500_SetTxRxMode(RX_EN);
        CC2500_Strobe(CC2500_SIDLE);
        CC2500_WriteReg(CC2500_0A_CHANNR, hopping_frequency[counter % 47]);
        CC2500_WriteReg(CC2500_23_FSCAL3, 0x89);
        state++;
        return 1300;
    } else {
        if (state == FRSKY_DATA1) {
            len = CC2500_ReadReg(CC2500_3B_RXBYTES | CC2500_READ_BURST) & 0x7F;

            //debugln("%d len",len);
            if (len && len<=(0x11+3)) { // 20bytes
                debug("rx tel %d\n", len);
                CC2500_ReadData(pkt, len);              //received telemetry packets
                #if defined(TELEMETRY)
                    if(pkt[len-1] & 0x80)
                    {//with valid crc
                        debug("rx invalid crc\n");
                        packet_count=0;
                        frsky_check_telemetry(pkt,len); //check if valid telemetry packets and buffer them.
                    }
                    debug("rx end crc\n");
                #endif
            } else {
                if (len != 0)
                    debug("rx len %d\n", len);
                packet_count++;
                // restart sequence on missed packet - might need count or timeout instead of one missed
                if(packet_count > 100) {//~1sec
                    packet_count=0;
                    #if defined TELEMETRY
                        telemetry_link=0;//no link frames
                        pkt[6]=0;//no user frames.
                    #endif
                }
            }
            CC2500_SetTxRxMode(TX_EN);
            CC2500_SetPower();  // Set tx_power
        }
        CC2500_Strobe(CC2500_SIDLE);
        CC2500_WriteReg(CC2500_0A_CHANNR, hopping_frequency[counter % 47]);

        // Frequency offset update
        if ( freq_offset_prev != freq_offset) {
            CC2500_WriteReg(CC2500_0C_FSCTRL0, freq_offset);
            freq_offset_prev = freq_offset;
        }

        CC2500_WriteReg(CC2500_23_FSCAL3, 0x89);
        CC2500_Strobe(CC2500_SFRX);
        frsky2way_data_frame();
        CC2500_WriteData(packet, packet[0]+1);
        state++;
    }
    return state == FRSKY_DATA4 ? 7500 : 9000;
}

uint16_t convert_channel_frsky(uint8_t num)
{
  uint16_t val = input.get_channel_data()[num];
  return ((val * 15) >> 4) + 1290;
}

void Frsky_init_hop(void)
{
    uint8_t val;
    uint8_t channel = rx_tx_addr[0] & 0x07;
    uint8_t channel_spacing = rx_tx_addr[1];
    //Filter bad tables
    if (channel_spacing < 0x02)
        channel_spacing += 0x02;
    if (channel_spacing > 0xE9)
        channel_spacing -= 0xE7;
    if (channel_spacing % 0x2F == 0)
        channel_spacing++;

    hopping_frequency[0] = channel;

    for (uint8_t i = 1; i < 50; i++) {
        channel = (channel + channel_spacing) % 0xEB;
        val = channel;

        if ((val == 0x00) || (val == 0x5A) || (val == 0xDC))
            val++;

        hopping_frequency[i] = i > 46 ? 0 : val;
    }
}

void FRSKY_init_cc2500(const uint8_t *ptr)
{
  for (uint8_t i = 0; i < 19; i++)
  {
    uint8_t reg = FRSKY_common_startreg_cc2500_conf[i];
    uint8_t val = ptr[i];
    if (reg == CC2500_0C_FSCTRL0)
      val = freq_offset;
    CC2500_WriteReg(reg, val);
  }
  freq_offset_prev = freq_offset;
  for (uint8_t i = 0; i < 17; i++)
  {
    uint8_t reg = FRSKY_common_end_cc2500_conf[i][0];
    uint8_t val = FRSKY_common_end_cc2500_conf[i][1];
    CC2500_WriteReg(reg, val);
  }
  CC2500_SetTxRxMode(TX_EN);
  CC2500_SetPower();
  CC2500_Strobe(CC2500_SIDLE);    // Go to idle...
}

void set_rx_tx_addr(uint32_t id)
{ // Used by almost all protocols
    rx_tx_addr[0] = (id >> 24) & 0xFF;
    rx_tx_addr[1] = (id >> 16) & 0xFF;
    rx_tx_addr[2] = (id >>  8) & 0xFF;
    rx_tx_addr[3] = (id >>  0) & 0xFF;
    rx_tx_addr[4] = (rx_tx_addr[2]&0xF0)|(rx_tx_addr[3]&0x0F);
}
