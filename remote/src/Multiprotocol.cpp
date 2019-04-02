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


#define DEBUG_SERIAL    // Only for STM32_BOARD compiled with Upload method "Serial"->usart1, "STM32duino bootloader"->USB serial

#include <cstdint> //adds types like unit_16_t

#include "config.h"
#include "tx_def.h"
#include "Multiprotocol.h"


#include "pins.h"
#include "Validate.h"
#include "common.h"
#include "state.h"
#include "input.h"
#include "cc2500_spi.h"

//#include "FrSkyX_cc2500.h"
//#include "FrSkyV_cc2500.h"
#include "FrSkyD_cc2500.h"

//Global constants/variables
uint32_t MProtocol_id;//tx id,
uint32_t MProtocol_id_master;
uint32_t blink=0,last_signal=0;
uint8_t protocol_flags=0,protocol_flags2=0;
//
uint8_t  channel;
uint8_t  packet[40];

static void protocol_init(void);

uint16_t seed;

// Protocol variables
uint8_t  cyrfmfg_id[6];//for dsm2 and devo
uint8_t  rx_tx_addr[5];
uint8_t  rx_id[4];
uint8_t  phase;
uint16_t bind_counter;
uint8_t  bind_phase;
uint8_t  binding_idx;
uint16_t packet_period;
uint8_t  packet_count;
uint8_t  packet_sent;
uint8_t  packet_length;
uint8_t  *hopping_frequency_ptr;
uint8_t  hopping_frequency_no=0;
uint8_t  rf_ch_num;
uint8_t  throttle, rudder, elevator, aileron;
uint8_t  flags;
uint16_t crc;
uint8_t  crc8;
uint16_t failsafe_count;
uint16_t state;
uint8_t  len;

#if defined(FRSKYX_CC2500_INO) || defined(SFHSS_CC2500_INO)
    uint8_t calData[48];
#endif


// Mode_select variables
uint8_t mode_select;

#if not defined (ORANGE_TX) && not defined (STM32_BOARD)
//Random variable
volatile uint32_t gWDT_entropy=0;
#endif

//Serial protocol
uint8_t sub_protocol;
uint8_t protocol;
uint8_t option;
uint8_t cur_protocol[3];
uint8_t prev_option;
uint8_t  RX_num;

//Serial RX variables
#define BAUD 100000
#define RXBUFFER_SIZE 26
volatile uint8_t rx_buff[RXBUFFER_SIZE];
volatile uint8_t rx_ok_buff[RXBUFFER_SIZE];
volatile uint8_t discard_frame = 0;

// Telemetry
uint8_t pkt[MAX_PKT];//telemetry receiving packets
float TIMER_PRESCALE = 5.82;
// Callback
typedef uint16_t (*void_function_t) (void);//pointer to a function with no parameters which return an uint16_t integer
void_function_t remote_callback = 0;

//forward declarations
void modules_reset();
uint32_t random_id(bool create_new);

// Init
void setup()
{
    // Setup diagnostic uart before anything else
    #ifdef ENABLE_DBEUG
        Serial.begin(115200,SERIAL_8N1);
        while (!Serial); // Wait for ever for the serial port to connect...
        debugln("Multiprotocol startup");
        debugln("time %s ", __TIME__);
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
    if( /*IS_BIND_BUTTON_on */ true)
    {
        BIND_BUTTON_FLAG_on;    // If bind button pressed save the status
        BIND_IN_PROGRESS;       // Request bind
    }
    else
        BIND_DONE;

    // Read status of mode select binary switch
    // after this mode_select will be one of {0000, 0001, ..., 1111}


    String str;
    debugln("select bank:");
    str="";
    while(true) {
      if (Serial.available() > 0) {
        char recieved = Serial.read();
        if (recieved == '\n') {
          int new_bank = atoi(str.c_str());
          curr_bank = new_bank;
          debugln("Bank selection %d", new_bank);
          break;
        }else {
          str += recieved;
        }
      }
    }

    debugln("select mode:");
    str="";
    while(true) {
      if (Serial.available() > 0) {
        char recieved = Serial.read();
        if (recieved == '\n') {
          int new_mode = atoi(str.c_str());
          debugln("Protocol selection switch reads as %d with \'%s\'", new_mode,str.c_str());
          mode_select = new_mode;
          break;
        }else {
          str += recieved;
        }
      }
    }
    debugln("Protocol selection switch reads as %d", mode_select);

    // Update LED
    LED_off;
    LED_output;

    //Init RF modules
    modules_reset();

    {
        seed = analogRead(PA0);
        randomSeed(seed);
    }

    // Read or create protocol id
    MProtocol_id_master=random_id(false);

    debugln("Module Id: %lx", MProtocol_id_master);

    //Protocol and interrupts initialization
    {
        uint8_t line=curr_bank*14+mode_select-1;
        protocol        =   PPM_prot[line].protocol;
        cur_protocol[1] = protocol;
        sub_protocol    =   PPM_prot[line].sub_proto;
        RX_num          =   PPM_prot[line].rx_num;

        debug("protocol: %d ", protocol);
        switch(protocol) {
            case PROTO_FRSKYD:
                debugln("PROTO_FRSKYD");
            break;
            case PROTO_FRSKYX:
                debugln("PROTO_FRSKYX");
            break;
            case PROTO_FRSKYV:
                debugln("PROTO_FRSKYV");
            break;
        }
        debug("sub_protocol: %d\n", sub_protocol);
        option = PPM_prot[line].option;  // Use radio-defined option value
        debug("freq offset: %d\n", option);
        line++;

        protocol_init();
    }

    debug("Init complete\n");
    input.init();
    input.update();
    init_state();
}

// Main
// Protocol scheduler
void loop()
{
    uint32_t s;
    s =micros();
    input.update();
    debug("input took %lu", (micros()-s));

    s =micros();
    update_state();
    debugln("state took %lu", (micros()-s));
    return;
    uint32_t next_callback;

    uint32_t end__ = micros();
    uint32_t start = micros();

    while(1) {
        start = end__;
        next_callback = remote_callback();

        if (next_callback > 4000) {
            uint32_t s;

            s =micros();
            input.update();
            debug("input took %lu", (micros()-s));

            s =micros();
            update_state();
            debugln("state took %lu", (micros()-s));
        }

        uint32_t wait_until = start + next_callback;
        end__ = micros();

        if (end__-start < next_callback) {
            uint32_t wait = next_callback;
            wait -= ((end__-start));
            delayMicroseconds(wait);
            end__ += wait;
        }
        end__ = micros();
    }
}

// Protocol start
static void protocol_init() {
    modules_reset();                // Reset all modules
    uint32_t next_callback = 0;

    //Set global ID and rx_tx_addr
    MProtocol_id = RX_num + MProtocol_id_master;
    set_rx_tx_addr(MProtocol_id);
    debugln("Protocol selected: %d, sub proto %d, rxnum %d, option %d", protocol, sub_protocol, RX_num, option);

    switch(protocol)                // Init the requested protocol
    {
        case PROTO_FRSKYD:
            next_callback = initFrSky_2way();
            remote_callback = ReadFrSky_2way;
            break;
    }
    delayMicroseconds(next_callback);
}

void modules_reset()
{
    CC2500_Reset();

    //Wait for every component to reset
    delay(100);
}

// Convert 32b id to rx_tx_addr
void set_rx_tx_addr(uint32_t id)
{ // Used by almost all protocols
    rx_tx_addr[0] = (id >> 24) & 0xFF;
    rx_tx_addr[1] = (id >> 16) & 0xFF;
    rx_tx_addr[2] = (id >>  8) & 0xFF;
    rx_tx_addr[3] = (id >>  0) & 0xFF;
    rx_tx_addr[4] = (rx_tx_addr[2]&0xF0)|(rx_tx_addr[3]&0x0F);
}

uint32_t random_id(bool create_new)
{
    #ifdef FORCE_GLOBAL_ID
        (void)create_new;
        return FORCE_GLOBAL_ID;
    #else
        uint32_t id=0;

        if (eeprom_read_byte((EE_ADDR)(10))==0xf0 && !create_new) {
            // TXID exists in EEPROM
            for(uint8_t i=4;i>0;i--) {
                id<<=8;
                id|=eeprom_read_byte((EE_ADDR)i-1);
            }
            if(id!=0x2AD141A7)  //ID with seed=0
            {
                debugln("Read ID from EEPROM");
                return id;
            }
        }
        // Generate a random ID from UUID
            #define STM32_UUID ((uint32_t *)0x1FFFF7E8)
        if (!create_new) {
                id = STM32_UUID[0] ^ STM32_UUID[1] ^ STM32_UUID[2];
            debugln("Generated ID from STM32 UUID %x", id);
        } else {
            id = random(0xfefefefe) + ((uint32_t)random(0xfefefefe) << 16);
            }

    #if 0
        for(uint8_t i=0;i<4;i++)
            eeprom_write_byte((EE_ADDR)address+i,id >> (i*8));
        eeprom_write_byte((EE_ADDR)(address+10),0xf0);//write bind flag in eeprom.
    #endif
        return id;
    #endif

}
