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

#ifndef CC2500_H
#define CC2500_H
#include "Arduino.h"
#include "spi.h"
#include "iface_cc2500.h"


class CC2500
{
private:
    SPI *spi;
    bool bind_done;
    uint8_t prev_power;
public:
    //TODO IS_RANGE_FLAG_on
    #define IS_RANGE_FLAG_on false
    enum wireless_mode {
        wireless_off,
        wireless_tx,
        wireless_rx,
    };
    // CC2500 power output from the chip itself
    // The numbers do not take into account any outside amplifier
    enum CC2500_POWER {
        CC2500_POWER_0  = 0x00, // -55dbm or less
        CC2500_POWER_1  = 0x50, // -30dbm
        CC2500_POWER_2  = 0x44, // -28dbm
        CC2500_POWER_3  = 0xC0, // -26dbm
        CC2500_POWER_4  = 0x84, // -24dbm
        CC2500_POWER_5  = 0x81, // -22dbm
        CC2500_POWER_6  = 0x46, // -20dbm
        CC2500_POWER_7  = 0x93, // -18dbm
        CC2500_POWER_8  = 0x55, // -16dbm
        CC2500_POWER_9  = 0x8D, // -14dbm
        CC2500_POWER_10 = 0xC6, // -12dbm
        CC2500_POWER_11 = 0x97, // -10dbm
        CC2500_POWER_12 = 0x6E, //  -8dbm
        CC2500_POWER_13 = 0x7F, //  -6dbm
        CC2500_POWER_14 = 0xA9, //  -4dbm
        CC2500_POWER_15 = 0xBB, //  -2dbm
        CC2500_POWER_16 = 0xFE, //   0dbm
        CC2500_POWER_17 = 0xFF, //  +1dbm

        CC2500_HIGH_POWER  = CC2500_POWER_17,
        CC2500_LOW_POWER   = CC2500_POWER_13,
        CC2500_RANGE_POWER = CC2500_POWER_1,
        CC2500_BIND_POWER  = CC2500_POWER_1,
    };


    CC2500(SPI *spi);
    void write_register(uint8_t address, uint8_t data);
    void write_register_multi(uint8_t address, const uint8_t data[], uint8_t length);
    void write_data(uint8_t *dpbuffer, uint8_t len);

    void    read_register_multi(uint8_t address, uint8_t data[], uint8_t length);
    uint8_t read_register(uint8_t address);
    void    read_data(uint8_t *dpbuffer, uint8_t len);

    void strobe(uint8_t state);

    void set_wireless_mode(enum wireless_mode mode);
    void set_power(void);

    uint8_t reset(void);
    void set_power_value(uint8_t power);
    void resetChip(void);
};

#endif /* CC2500_H */
