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

//#include "iface_cc2500.h"
#include "frskyd.h"
#include "config.h"
#include "Arduino.h"


Frsky_d::Frsky_d(CC2500 *cc2500)
{
    this->calibration = 0x89;
    this->state = FRSKY_BIND;
    this->cc2500 = cc2500;
    this->packet_count = 0;

    // TODO  real random randomSeed(analogRead(0));
    randomSeed(42);

    this->MProtocol_id_master = random(10);
    this->MProtocol_id = this->MProtocol_id_master;

    this->init_channel();
    this->init_failsafe();

    Serial.print(" multiprotocl id : ");
    Serial.print(this->MProtocol_id);
    Serial.print("\n");
}
void Frsky_d::init(bool bind)
{
    this->init_cc2500(FRSKYD_cc2500_conf);

    cc2500->write_register(CC2500_09_ADDR, bind ? 0x03 : rx_tx_addr[3]);
    cc2500->write_register(CC2500_07_PKTCTRL1, 0x05);
    cc2500->strobe(CC2500_SIDLE);    // Go to idle...
    //
    cc2500->write_register(CC2500_0A_CHANNR, 0x00);
    cc2500->write_register(CC2500_23_FSCAL3, 0x89);
    cc2500->strobe(CC2500_SFRX);
    //#######END INIT########
}
void Frsky_d::init_cc2500(const uint8_t *ptr)
{
    for (uint8_t i = 0; i < 19; i++) {
        uint8_t reg = FRSKY_common_startreg_cc2500_conf[i];
        uint8_t val = ptr[i];

        if (reg == CC2500_0C_FSCTRL0)
            val = option;

        cc2500->write_register(reg, val);
    }

    prev_option = option ;      // Save option to monitor FSCTRL0 change

    for (uint8_t i = 0; i < 17; i++) {
        uint8_t reg = FRSKY_common_end_cc2500_conf[i][0];
        uint8_t val = FRSKY_common_end_cc2500_conf[i][1];
        cc2500->write_register(reg, val);
    }

    cc2500->set_wireless_mode(CC2500::wireless_tx);
    cc2500->set_power();
    cc2500->strobe(CC2500_SIDLE);    // Go to idle...
}

void Frsky_d::build_bind_packet(void)
{
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

void Frsky_d::frsky2way_data_frame()
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

    for (uint8_t i = 0; i < 8; i++) {
        uint16_t value;
        value = convert_channel_frsky(i);

        if (i < 4) {
            packet[6 + i] = value & 0xff;
            packet[10 + (i >> 1)] |= ((value >> 8) & 0x0f) << (4 * (i & 0x01));
        } else {
            packet[8 + i] = value & 0xff;
            packet[16 + ((i - 4) >> 1)] |= ((value >> 8) & 0x0f) << (4 * ((i - 4) & 0x01));
        }
    }
}

uint16_t Frsky_d::init_frsky_2way(void)
{
    Serial.print("init hop");
    init_hop();
    packet_count = 0;
#if defined TELEMETRY
    init_frskyd_link_telemetry();
#endif

    if (IS_BIND_IN_PROGRESS) {
        init(true);
        state = FRSKY_BIND;
    } else {
        state = FRSKY_BIND_DONE;
    }

    return 10000;
}

uint16_t Frsky_d::read_frsky_2way(void)
{
    if (state < FRSKY_BIND_DONE) {
        Serial.print("bind mode\n");
        build_bind_packet();
        cc2500->strobe(CC2500_SIDLE);
        cc2500->write_register(CC2500_0A_CHANNR, 0x00);
        cc2500->write_register(CC2500_23_FSCAL3, 0x89);
        cc2500->strobe(CC2500_SFRX);//0x3A
        cc2500->write_data(packet, packet[0] + 1);

        if (IS_BIND_DONE) {
            Serial.print("bind finished \n");
            state = FRSKY_BIND_DONE;
        } else {
            //state += 1;
            Serial.print("bind not done\n");
            Serial.print("state :");
            Serial.print(state);
            Serial.print("\n");
        }

        return 9000;
    }

    if (state == FRSKY_BIND_DONE) {
        Serial.print("bind done \n");
        state = FRSKY_DATA2;
        init(false);
        counter = 0;
        BIND_DONE;
    } else if (state == FRSKY_DATA5) {
        Serial.print("data 5 \n");
        cc2500->strobe(CC2500_SRX);//0x34 RX enable
        state = FRSKY_DATA1;
        return 9200;
    }

    counter = (counter + 1) % 188;

    if (state == FRSKY_DATA4) {
        Serial.print("data 4 \n");
        //telemetry receive
        cc2500->set_wireless_mode(CC2500::wireless_rx);
        cc2500->strobe(CC2500_SIDLE);
        cc2500->write_register(CC2500_0A_CHANNR, hopping_frequency[counter % 47]);
        cc2500->write_register(CC2500_23_FSCAL3, 0x89);
        state++;
        return 1300;
    } else {
        if (state == FRSKY_DATA1) {
            Serial.print("data 1 \n");
            len = cc2500->read_register(CC2500_3B_RXBYTES | CC2500_READ_BURST) & 0x7F;

            if (len && len <= (0x11 + 3)) { // 20bytes
                cc2500->read_data(pkt, len);              //received telemetry packets
#if defined(TELEMETRY)

                if (pkt[len - 1] & 0x80) {
                    //with valid crc
                    packet_count = 0;
                    frsky_check_telemetry(pkt, len); //check if valid telemetry packets and buffer them.
                }

#endif
            } else {
                packet_count++;

                // restart sequence on missed packet - might need count or timeout instead of one missed
                if (packet_count > 100) {
                    //~1sec
                    packet_count = 0;
#if defined TELEMETRY
                    telemetry_link = 0; //no link frames
                    pkt[6] = 0; //no user frames.
#endif
                }
            }

            cc2500->set_wireless_mode(CC2500::wireless_tx);
            cc2500->set_power();  // Set tx_power
        }

        cc2500->strobe(CC2500_SIDLE);
        cc2500->write_register(CC2500_0A_CHANNR, hopping_frequency[counter % 47]);

        if (prev_option != option) {
            cc2500->write_register(CC2500_0C_FSCTRL0, option); // Frequency offset hack
            prev_option = option ;
        }

        cc2500->write_register(CC2500_23_FSCAL3, 0x89);
        cc2500->strobe(CC2500_SFRX);
        frsky2way_data_frame();
        cc2500->write_data(packet, packet[0] + 1);
        state++;
    }

    return state == FRSKY_DATA4 ? 7500 : 9000;
}

uint16_t Frsky_d::convert_channel_frsky(uint8_t num)
{
    // Channel value for FrSky (PPM is multiplied by 1.5)
    uint16_t val = Channel_data[num];
    return ((val*15)>>4)+1290;
}

void Frsky_d::init_hop(void)
{
    uint8_t val;
    uint8_t channel = rx_tx_addr[0]&0x07;
    uint8_t channel_spacing = rx_tx_addr[1];
    //Filter bad tables
    if(channel_spacing<0x02)
        channel_spacing+=0x02;
    if(channel_spacing>0xE9)
        channel_spacing-=0xE7;
    if(channel_spacing%0x2F==0)
        channel_spacing++;

    hopping_frequency[0]=channel;
    for(uint8_t i=1;i<50;i++) {
        channel=(channel+channel_spacing) % 0xEB;
        val=channel;
        if((val==0x00) || (val==0x5A) || (val==0xDC))
            val++;
        hopping_frequency[i]=i>46?0:val;
    }
}

void Frsky_d::init_channel(void)
{
    for(uint8_t i=0;i<NUM_CHN;i++)
        Channel_data[i]=1024;

    Channel_data[THROTTLE]=204;
}

void Frsky_d::init_failsafe()
{
    for(uint8_t i=0;i<NUM_CHN;i++)
        failsafe_data[i]=1024;

    failsafe_data[THROTTLE]=204;
    fail_save = true;
}
