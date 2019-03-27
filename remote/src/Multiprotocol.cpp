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

#ifdef ENABLE_PPM
// PPM variable
volatile uint8_t  PPM_chan_max=0;
#endif

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
static void protocol_init();
uint8_t Update_All();

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

#ifdef ENABLE_PPM
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
  option            =   PPM_prot[line].option;  // Use radio-defined option value
  debug("freq offset: %d\n", option);
        if(PPM_prot[line].power)
          POWER_FLAG_on;
        if(PPM_prot[line].autobind) {
            AUTOBIND_FLAG_on;
            BIND_IN_PROGRESS;   // Force a bind at protocol startup
        }
        line++;

        protocol_init();

    }
#endif //ENABLE_PPM
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

    if(remote_callback==0 || IS_WAIT_BIND_on ) {
        do {
            Update_All();
        } while(remote_callback==0 || IS_WAIT_BIND_on);
    }

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

uint8_t Update_All() {

    #ifdef ENABLE_BIND_CH
        if(IS_AUTOBIND_FLAG_on &&
            IS_BIND_CH_PREV_off &&
            Channel_data[BIND_CH-1] > CHANNEL_MAX_COMMAND &&
            Channel_data[Input::CH_THROTTLE] < (CHANNEL_MIN_100+50)
        ) { // Autobind is on and BIND_CH went up and Throttle is low
            CHANGE_PROTOCOL_FLAG_on;                            //reload protocol
            BIND_IN_PROGRESS;                                   //enable bind

            debugln("%s:%d set bind prog",__func__, __LINE__);
            BIND_CH_PREV_on;
        }
        if(IS_AUTOBIND_FLAG_on && IS_BIND_CH_PREV_on && Channel_data[BIND_CH-1]<CHANNEL_MIN_COMMAND) {
            // Autobind is on and BIND_CH went down
            BIND_CH_PREV_off;
            //Request protocol to terminate bind
            #if defined(FRSKYD_CC2500_INO) || defined(FRSKYX_CC2500_INO) || defined(FRSKYV_CC2500_INO)
            if(protocol==PROTO_FRSKYD || protocol==PROTO_FRSKYX || protocol==PROTO_FRSKYV)
                BIND_DONE;
            else
            #endif
            if(bind_counter>2)
                bind_counter=2;
        }
    #endif //ENABLE_BIND_CH

    input.update();

    if(IS_CHANGE_PROTOCOL_FLAG_on) {
        debugln("%s:%d set bind prog",__func__, __LINE__);
        // Protocol needs to be changed or relaunched for bind
        protocol_init();                                    //init new protocol
        return 1;
    }
    return 0;
}

// Protocol start
static void protocol_init() {
    static uint16_t next_callback;
    if(IS_WAIT_BIND_off) {
        remote_callback = 0;            // No protocol
        next_callback=0;                // Default is immediate call back
        modules_reset();                // Reset all modules

        //Set global ID and rx_tx_addr
        MProtocol_id = RX_num + MProtocol_id_master;
        set_rx_tx_addr(MProtocol_id);

        blink=millis();

        debugln("Protocol selected: %d, sub proto %d, rxnum %d, option %d", protocol, sub_protocol, RX_num, option);

        switch(protocol)                // Init the requested protocol
        {
        case PROTO_FRSKYD:
            next_callback = initFrSky_2way();
            remote_callback = ReadFrSky_2way;
            break;
        }
    }

    #if defined(WAIT_FOR_BIND) && defined(ENABLE_BIND_CH)
        if( IS_AUTOBIND_FLAG_on && IS_BIND_CH_PREV_off && (cur_protocol[1]&0x80)==0 && mode_select == MODE_SERIAL) {
            // Autobind is active but no bind requested by either BIND_CH or BIND. But do not wait if in PPM mode...
            WAIT_BIND_on;
            return;
        }
    #endif
    WAIT_BIND_off;
    CHANGE_PROTOCOL_FLAG_off;

    delayMicroseconds(next_callback);
    debugln("%s BIND_BUTTON_FLAG_off",__func__);
}

void update_serial_data()
{
    if(rx_ok_buff[1]&0x20)                      //check range
        RANGE_FLAG_on;
    else
        RANGE_FLAG_off;

    if(rx_ok_buff[1]&0x40)                      //check autobind
        AUTOBIND_FLAG_on;
    else
        AUTOBIND_FLAG_off;

    if(rx_ok_buff[2]&0x80)                      //if rx_ok_buff[2] ==1,power is low ,0-power high
        POWER_FLAG_off;                         //power low
    else
        POWER_FLAG_on;                          //power high

    //Forced frequency tuning values for CC2500 protocols
    #if defined(FORCE_FRSKYD_TUNING)
        if(protocol==PROTO_FRSKYD)
            option=FORCE_FRSKYD_TUNING; // Use config-defined tuning value for FrSkyD
        else
    #endif
            option=rx_ok_buff[3];       // Use radio-defined option value

    #ifdef FAILSAFE_ENABLE
        bool failsafe=false;
        if(rx_ok_buff[0]&0x02) { // Packet contains failsafe instead of channels
            failsafe=true;
            rx_ok_buff[0]&=0xFD;                //remove the failsafe flag
        }
    #endif
    if( (rx_ok_buff[0] != cur_protocol[0]) || ((rx_ok_buff[1]&0x5F) != (cur_protocol[1]&0x5F)) || ( (rx_ok_buff[2]&0x7F) != (cur_protocol[2]&0x7F) ) )
    { // New model has been selected
        CHANGE_PROTOCOL_FLAG_on;                //change protocol
        WAIT_BIND_off;
        if((rx_ok_buff[1]&0x80)!=0 || IS_AUTOBIND_FLAG_on)
            BIND_IN_PROGRESS;                   //launch bind right away if in autobind mode or bind is set
        else
            BIND_DONE;
        protocol=(rx_ok_buff[0]==0x55?0:32) + (rx_ok_buff[1]&0x1F); //protocol no (0-63) bits 4-6 of buff[1] and bit 0 of buf[0]
        sub_protocol=(rx_ok_buff[2]>>4)& 0x07;  //subprotocol no (0-7) bits 4-6
        RX_num=rx_ok_buff[2]& 0x0F;             // rx_num bits 0---3
    } else if( ((rx_ok_buff[1]&0x80)!=0) && ((cur_protocol[1]&0x80)==0) )      // Bind flag has been set
        { // Restart protocol with bind
            CHANGE_PROTOCOL_FLAG_on;
            BIND_IN_PROGRESS;
    } else {
            if( ((rx_ok_buff[1]&0x80)==0) && ((cur_protocol[1]&0x80)!=0) )  // Bind flag has been reset
            { // Request protocol to end bind
                #if defined(FRSKYD_CC2500_INO) || defined(FRSKYX_CC2500_INO) || defined(FRSKYV_CC2500_INO)
                if(protocol==PROTO_FRSKYD || protocol==PROTO_FRSKYX || protocol==PROTO_FRSKYV)
                    BIND_DONE;
                else
                #endif
                if(bind_counter>2)
                    bind_counter=2;
            }
    }

    //store current protocol values
    for(uint8_t i=0;i<3;i++)
        cur_protocol[i] =  rx_ok_buff[i];

    // decode channel/failsafe values
    volatile uint8_t *p=rx_ok_buff+3;
    uint8_t dec=-3;
    for(uint8_t i=0;i<NUM_TX_CHN;i++) {
        dec+=3;
        if(dec>=8) {
            dec-=8;
            p++;
        }
        p++;
        uint16_t temp=((*((uint32_t *)p))>>dec)&0x7FF;
            if(failsafe)
                Failsafe_data[i]=temp;          //value range 0..2047, 0=no pulses, 2047=hold
            else
                Channel_data[i]=temp;           //value range 0..2047, 0=-125%, 2047=+125%
    }
        cli();
    if(IS_RX_MISSED_BUFF_on)                    // If the buffer is still valid
    {   memcpy((void*)rx_ok_buff,(const void*)rx_buff,RXBUFFER_SIZE);// Duplicate the buffer
        RX_MISSED_BUFF_off;
    }
        sei();
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
