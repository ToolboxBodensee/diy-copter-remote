/*********************************************************
                    Multiprotocol Tx code
               by Midelic and Pascal Langer(hpnuts)
    http://www.rcgroups.com/forums/showthread.php?t=2165676
    https://github.com/pascallanger/DIY-Multiprotocol-TX-Module/edit/master/README.md

    Thanks to PhracturedBlue, Hexfet, Goebish, Victzh and all protocol developers
                Ported  from deviation firmware

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

#include <cstdint> //adds types like unit_16_t

#include "config.h"
#include "tx_def.h"
#include "Multiprotocol.h"


#include "pins.h"
#include "Validate.h"
#include "state.h"
#include "input.h"
#include "cc2500_spi.h"

//#include "FrSkyX_cc2500.h"
//#include "FrSkyV_cc2500.h"
#include "FrSkyD_cc2500.h"

//Global constants/variables
uint8_t protocol_flags=0,protocol_flags2=0;
//
uint8_t  channel;
uint8_t  packet[40];

// Protocol variables
uint8_t  rx_id[4];
uint16_t packet_period;
uint8_t  packet_count;
uint8_t  *hopping_frequency_ptr;
uint16_t crc;
uint8_t  crc8;
uint16_t failsafe_count;
uint8_t  len;
#ifndef ENABLE_DBEUG
#include <USBComposite.h>
USBHID HID;
HIDJoystick Joystick(HID);
#endif

// Telemetry
void setup()
{
    // Setup diagnostic uart before anything else
    #ifdef ENABLE_DBEUG
        Serial.begin(9600,SERIAL_8N1);
        delay(1000);
        debugln("Multiprotocol startup");
        delay(1000);
        debugln("time %s ", __TIME__);
        delay(1000);
    #else
        HID.begin(HID_JOYSTICK);
    #endif

    // all inputs
    // outputs
    SDI_output;
    SCLK_output;
    CC25_CSN_output;

    // Random
    //random_init();
    CC25_CSN_on;
    SDI_on;
    SCLK_off;

    //Wait for every component to start
    delay(100);

    // Read status of bind button
    BIND_IN_PROGRESS;       // Request bind

    // Update LED
    LED_off;
    LED_output;

    //frquency offset initialization
    {
        freq_offset = 0;
        debug("freq offset: %d\n", freq_offset);
        CC2500_Reset();

        //Wait for cc2500 to reset
        delay(100);
    }

    input.init();
    input.update();

    init_state();

    debug("Init complete\n");
}

void loop()
{
    update_state();
}
