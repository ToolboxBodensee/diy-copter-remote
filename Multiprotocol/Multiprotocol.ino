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
#if 0
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#endif

//#define DEBUG_PIN     // Use pin TX for AVR and SPI_CS for STM32 => DEBUG_PIN_on, DEBUG_PIN_off, DEBUG_PIN_toggle
#define DEBUG_SERIAL    // Only for STM32_BOARD compiled with Upload method "Serial"->usart1, "STM32duino bootloader"->USB serial

#if defined (ARDUINO_AVR_XMEGA32D4) || defined (ARDUINO_MULTI_ORANGERX)
    #include "MultiOrange.h"
#endif

#include "config.h"
#include "tx_def.h"
#include "Multiprotocol.h"

//Multiprotocol module configuration file

#include "pins.h"
#include "Validate.h"
#include "common.h"
#include "state.h"
#include "input.h"
#include "cc2500_spi.h"

#include "FrSkyX_cc2500.h"
#include "FrSkyV_cc2500.h"
#include "FrSkyD_cc2500.h"

//Global constants/variables
uint32_t MProtocol_id;//tx id,
uint32_t MProtocol_id_master;
uint32_t blink=0,last_signal=0;
uint8_t protocol_flags=0,protocol_flags2=0;
//
uint16_t counter;
uint8_t  channel;
uint8_t  packet[40];

// Servo data
uint16_t Channel_data[NUM_CHN];
uint8_t  Channel_AUX;

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
uint8_t  hopping_frequency[50];
uint8_t  *hopping_frequency_ptr;
uint8_t  hopping_frequency_no=0;
uint8_t  rf_ch_num;
uint8_t  throttle, rudder, elevator, aileron;
uint8_t  flags;
uint16_t crc;
uint8_t  crc8;
uint16_t seed;
uint16_t failsafe_count;
uint16_t state;
uint8_t  len;

#if defined(FRSKYX_CC2500_INO) || defined(SFHSS_CC2500_INO)
    uint8_t calData[48];
#endif

//Channel mapping for protocols
const uint8_t CH_AETR[]={AILERON, ELEVATOR, THROTTLE, RUDDER, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12, CH13, CH14, CH15, CH16};
const uint8_t CH_TAER[]={THROTTLE, AILERON, ELEVATOR, RUDDER, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12, CH13, CH14, CH15, CH16};
const uint8_t CH_RETA[]={RUDDER, ELEVATOR, THROTTLE, AILERON, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12, CH13, CH14, CH15, CH16};
const uint8_t CH_EATR[]={ELEVATOR, AILERON, THROTTLE, RUDDER, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12, CH13, CH14, CH15, CH16};

// Mode_select variables
uint8_t mode_select;

#ifdef ENABLE_PPM
// PPM variable
volatile uint16_t PPM_data[NUM_CHN];
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
uint8_t prev_power=0xFD; // unused power value
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

// Init
void setup()
{
    // Setup diagnostic uart before anything else
    #ifdef DEBUG_SERIAL
        Serial.begin(115200,SERIAL_8N1);
        while (!Serial); // Wait for ever for the serial port to connect...
        debugln("Multiprotocol startup");
        debugln("time %s ", __TIME__);
    #endif

    // General pinout
        //ATMEGA328p
        // all inputs
        // outputs
        SDI_output;
        SCLK_output;
        #ifdef A7105_CSN_pin
            A7105_CSN_output;
        #endif
        #ifdef CC25_CSN_pin
            CC25_CSN_output;
        #endif
        #ifdef CYRF_CSN_pin
            CYRF_RST_output;
            CYRF_CSN_output;
        #endif
        #ifdef NRF_CSN_pin
            NRF_CSN_output;
        #endif

        // Timer1 config
    #ifdef TCCR1A
        TCCR1A = 0;
        TCCR1B = (1 << CS11);   //prescaler8, set timer1 to increment every 0.5us(16Mhz) and start timer
    #endif
        // Random
        //random_init();

    // Set Chip selects
    #ifdef A7105_CSN_pin
        A7105_CSN_on;
    #endif
    #ifdef CC25_CSN_pin
        CC25_CSN_on;
    #endif
    #ifdef CYRF_CSN_pin
        CYRF_CSN_on;
    #endif
    #ifdef NRF_CSN_pin
        NRF_CSN_on;
    #endif
    //  Set SPI lines
    #ifdef  STM32_BOARD
        initSPI2();
    #else
        SDI_on;
        SCLK_off;
    #endif

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

    #ifdef ENABLE_PPM
        uint8_t bank=bank_switch();
    #endif

    // Set default channels' value
    InitChannel();
    InitPPM();
    // Update LED
    LED_off;
    LED_output;

    //Init RF modules
    modules_reset();

    randomSeed(42);

    // Read or create protocol id
    MProtocol_id_master=random_id(10,false);

    debugln("Module Id: %lx", MProtocol_id_master);

#ifdef ENABLE_PPM
    //Protocol and interrupts initialization
    if(mode_select != MODE_SERIAL)
    { // PPM
        uint8_t line=bank*14+mode_select-1;
        protocol        =   PPM_prot[line].protocol;
        cur_protocol[1] = protocol;
        sub_protocol    =   PPM_prot[line].sub_proto;
        RX_num          =   PPM_prot[line].rx_num;

  debugln("protocol: %d", protocol);
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
  debugln("sub_protocol: %d", sub_protocol);
  option            =   PPM_prot[line].option;  // Use radio-defined option value
  debugln("freq offset: %d", option);
        if(PPM_prot[line].power)
          POWER_FLAG_on;
        if(PPM_prot[line].autobind)
        {
            AUTOBIND_FLAG_on;
            BIND_IN_PROGRESS;   // Force a bind at protocol startup
        }
        line++;

        protocol_init();

        #if defined(TELEMETRY)
            PPM_Telemetry_serial_init();// Configure serial for telemetry
        #endif
    }
    else
#endif //ENABLE_PPM
  debugln("Init complete");
  input.update();
  init_state();
}

// Main
// Protocol scheduler
void loop()
{
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
            input.update();
            update_state();
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

uint8_t Update_All()
{
    #ifdef ENABLE_SERIAL
        if(mode_select==MODE_SERIAL && IS_RX_FLAG_on)       // Serial mode and something has been received
        {
            update_serial_data();                           // Update protocol and data
            update_channels_aux();
            last_signal=millis();
        }
    #endif //ENABLE_SERIAL
    #ifdef ENABLE_PPM
        if(mode_select!=MODE_SERIAL && IS_PPM_FLAG_on)      // PPM mode and a full frame has been received
        {
            debugln("%s:%d",__func__, __LINE__);
            for(uint8_t i=0;i<PPM_chan_max;i++) {
                // update servo data without interrupts to prevent bad read
                uint16_t val;
                cli();                                      // disable global int
                val = PPM_data[i];
                sei();                                      // enable global int
                val=map16b(val,PPM_MIN_100*2,PPM_MAX_100*2,CHANNEL_MIN_100,CHANNEL_MAX_100);
                if(val&0x8000)                  val=CHANNEL_MIN_125;
                else if(val>CHANNEL_MAX_125)    val=CHANNEL_MAX_125;
                //Channel_data[i]=val;
            }
            PPM_FLAG_off;                                   // wait for next frame before update
            update_channels_aux();
            last_signal=millis();
        }
    #endif //ENABLE_PPM


    #ifdef ENABLE_BIND_CH
        if(IS_AUTOBIND_FLAG_on && IS_BIND_CH_PREV_off && Channel_data[BIND_CH-1]>CHANNEL_MAX_COMMAND && Channel_data[THROTTLE]<(CHANNEL_MIN_100+50))
        { // Autobind is on and BIND_CH went up and Throttle is low
            CHANGE_PROTOCOL_FLAG_on;                            //reload protocol
            BIND_IN_PROGRESS;                                   //enable bind

      debugln("%s:%d set bind prog",__func__, __LINE__);
            BIND_CH_PREV_on;
        }
        if(IS_AUTOBIND_FLAG_on && IS_BIND_CH_PREV_on && Channel_data[BIND_CH-1]<CHANNEL_MIN_COMMAND)
        { // Autobind is on and BIND_CH went down
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

    if(IS_CHANGE_PROTOCOL_FLAG_on)
    {
        debugln("%s:%d set bind prog",__func__, __LINE__);
        // Protocol needs to be changed or relaunched for bind
        protocol_init();                                    //init new protocol
        return 1;
    }
    return 0;
}

// Update channels direction and Channel_AUX flags based on servo AUX positions
static void update_channels_aux(void)
{
    //Reverse channels direction
    #ifdef REVERSE_AILERON
        reverse_channel(AILREON);
    #endif
    #ifdef REVERSE_ELEVATOR
        reverse_channel(ELEVATOR);
    #endif
    #ifdef REVERSE_THROTTLE
        reverse_channel(THROTTLE);
    #endif
    #ifdef REVERSE_RUDDER
        reverse_channel(RUDDER);
    #endif

    //Calc AUX flags
    Channel_AUX=0;
    for(uint8_t i=0;i<8;i++)
        if(Channel_data[CH5+i]>CHANNEL_SWITCH)
            Channel_AUX|=1<<i;
}

#ifdef ENABLE_PPM
uint8_t bank_switch(void)
{
    uint8_t bank= curr_bank;
    if(bank>=NBR_BANKS)
    { // Wrong number of bank
        curr_bank = 0;  // set bank to 0
        bank=0;
    }
    debugln("Using bank %d", bank);

    phase=3;
    uint32_t check=millis();
    blink=millis();
    while(mode_select==15)
    { //loop here if the dial is on position 15 for user to select the bank
        if(blink<millis())
        {
            switch(phase & 0x03)
            { // Flash bank number of times
                case 0:
                    blink+=BLINK_BANK_TIME_HIGH;
                    phase++;
                    break;
                case 1:
                    blink+=BLINK_BANK_TIME_LOW;
                    phase++;
                    break;
                case 2:
                    if( (phase>>2) >= bank)
                    {
                        phase=0;
                        blink+=BLINK_BANK_REPEAT;
                    }
                    else
                        phase+=2;
                    break;
                case 3:
                    blink+=BLINK_BANK_TIME_LOW;
                    phase=0;
                    break;
            }
        }
        if(check<millis())
        {
            bool test_bind=true/*IS_BIND_BUTTON_on*/;
            if( test_bind )
            {   // Increase bank
                bank++;
                if(bank>=NBR_BANKS)
                    bank=0;
                curr_bank = bank;
                debugln("Using bank %d", bank);
                phase=3;
                blink+=BLINK_BANK_REPEAT;
                check+=2*BLINK_BANK_REPEAT;
            }
            check+=1;
        }
    }
    return bank;
}
#endif

inline void tx_pause()
{
    #ifdef TELEMETRY
    // Pause telemetry by disabling transmitter interrupt
        #ifdef ORANGE_TX
            USARTC0.CTRLA &= ~0x03 ;
        #else
            #ifndef BASH_SERIAL
                #ifdef STM32_BOARD
                    USART3_BASE->CR1 &= ~ USART_CR1_TXEIE;
                #else
                    UCSR0B &= ~_BV(UDRIE0);
                #endif
            #endif
        #endif
    #endif
}

inline void tx_resume()
{
    #ifdef TELEMETRY
    // Resume telemetry by enabling transmitter interrupt
        #ifndef SPORT_POLLING
        if(!IS_TX_PAUSE_on)
        #endif
        {
            #ifdef ORANGE_TX
                cli() ;
                USARTC0.CTRLA = (USARTC0.CTRLA & 0xFC) | 0x01 ;
                sei() ;
            #else
                #ifndef BASH_SERIAL
                    #ifdef STM32_BOARD
                        USART3_BASE->CR1 |= USART_CR1_TXEIE;
                    #else
                        UCSR0B |= _BV(UDRIE0);
                    #endif
                #else
                    resumeBashSerial();
                #endif
            #endif
        }
    #endif
}

// Protocol start
static void protocol_init()
{
    static uint16_t next_callback;
    if(IS_WAIT_BIND_off)
    {
        remote_callback = 0;            // No protocol
        next_callback=0;                // Default is immediate call back
        modules_reset();                // Reset all modules

        //Set global ID and rx_tx_addr
        MProtocol_id = RX_num + MProtocol_id_master;
        set_rx_tx_addr(MProtocol_id);

        #ifdef FAILSAFE_ENABLE
            InitFailsafe();
        #endif

        blink=millis();

        debugln("Protocol selected: %d, sub proto %d, rxnum %d, option %d", protocol, sub_protocol, RX_num, option);

        switch(protocol)                // Init the requested protocol
        {
        case PROTO_FRSKYD:
            next_callback = initFrSky_2way();
            remote_callback = ReadFrSky_2way;
            break;
        case PROTO_FRSKYV:
            next_callback = initFRSKYV();
            remote_callback = ReadFRSKYV;
            break;
        case PROTO_FRSKYX:
            next_callback = initFrSkyX();
            remote_callback = ReadFrSkyX;
            break;
        }
    }

    #if defined(WAIT_FOR_BIND) && defined(ENABLE_BIND_CH)
        if( IS_AUTOBIND_FLAG_on && IS_BIND_CH_PREV_off && (cur_protocol[1]&0x80)==0 && mode_select == MODE_SERIAL)
        { // Autobind is active but no bind requested by either BIND_CH or BIND. But do not wait if in PPM mode...
            WAIT_BIND_on;
            return;
        }
    #endif
    WAIT_BIND_off;
    CHANGE_PROTOCOL_FLAG_off;

#if 0
    if(next_callback>32000)
    { // next_callback should not be more than 32767 so we will wait here...
        uint16_t temp=(next_callback>>10)-2;
        delayMilliseconds(temp);
        next_callback-=temp<<10;                // between 2-3ms left at this stage
    }
    cli();                                      // disable global int
    OCR1A = TCNT1 + next_callback*2;            // set compare A for callback
    #ifndef STM32_BOARD
        TIFR1 = OCF1A_bm ;                      // clear compare A flag
    #else
        TIMER2_BASE->SR = 0x1E5F & ~TIMER_SR_CC1IF; // Clear Timer2/Comp1 interrupt flag
    #endif
    sei();                                      // enable global int
    BIND_BUTTON_FLAG_off;                       // do not bind/reset id anymore even if protocol change
#else
  delayMicroseconds(next_callback);
#endif
  debugln("%s BIND_BUTTON_FLAG_off",__func__);
}

void update_serial_data()
{
    RX_DONOTUPDATE_on;
    RX_FLAG_off;                                //data is being processed
    #ifdef SAMSON   // Extremely dangerous, do not enable this unless you know what you are doing...
        if( rx_ok_buff[0]==0x55 && (rx_ok_buff[1]&0x1F)==PROTO_FRSKYD && rx_ok_buff[2]==0x7F && rx_ok_buff[24]==217 && rx_ok_buff[25]==202 )
        {//proto==FRSKYD+sub==7+rx_num==7+CH15==73%+CH16==73%
            rx_ok_buff[1]=(rx_ok_buff[1]&0xE0) | PROTO_FLYSKY;          // change the protocol to Flysky
            memcpy((void*)(rx_ok_buff+4),(void*)(rx_ok_buff+4+11),11);  // reassign channels 9-16 to 1-8
        }
    #endif
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
    #if defined(FORCE_FRSKYD_TUNING) && defined(FRSKYD_CC2500_INO)
        if(protocol==PROTO_FRSKYD)
            option=FORCE_FRSKYD_TUNING; // Use config-defined tuning value for FrSkyD
        else
    #endif
    #if defined(FORCE_FRSKYV_TUNING) && defined(FRSKYV_CC2500_INO)
        if(protocol==PROTO_FRSKYV)
            option=FORCE_FRSKYV_TUNING; // Use config-defined tuning value for FrSkyV
        else
    #endif
    #if defined(FORCE_FRSKYX_TUNING) && defined(FRSKYX_CC2500_INO)
        if(protocol==PROTO_FRSKYX)
            option=FORCE_FRSKYX_TUNING; // Use config-defined tuning value for FrSkyX
        else
    #endif
    #if defined(FORCE_SFHSS_TUNING) && defined(SFHSS_CC2500_INO)
        if (protocol==PROTO_SFHSS)
            option=FORCE_SFHSS_TUNING;  // Use config-defined tuning value for SFHSS
        else
    #endif
    #if defined(FORCE_CORONA_TUNING) && defined(CORONA_CC2500_INO)
        if (protocol==PROTO_CORONA)
            option=FORCE_CORONA_TUNING; // Use config-defined tuning value for CORONA
        else
    #endif
    #if defined(FORCE_HITEC_TUNING) && defined(HITEC_CC2500_INO)
        if (protocol==PROTO_HITEC)
            option=FORCE_HITEC_TUNING;  // Use config-defined tuning value for HITEC
        else
    #endif
            option=rx_ok_buff[3];       // Use radio-defined option value

    #ifdef FAILSAFE_ENABLE
        bool failsafe=false;
        if(rx_ok_buff[0]&0x02)
        { // Packet contains failsafe instead of channels
            failsafe=true;
            rx_ok_buff[0]&=0xFD;                //remove the failsafe flag
            FAILSAFE_VALUES_on;                 //failsafe data has been received
        }
    #endif
    #ifdef BONI
        if(CH14_SW)
            rx_ok_buff[2]=(rx_ok_buff[2]&0xF0)|((rx_ok_buff[2]+1)&0x0F);    // Extremely dangerous, do not enable this!!! This is really for a special case...
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
    }
    else
        if( ((rx_ok_buff[1]&0x80)!=0) && ((cur_protocol[1]&0x80)==0) )      // Bind flag has been set
        { // Restart protocol with bind
            CHANGE_PROTOCOL_FLAG_on;
            BIND_IN_PROGRESS;
        }
        else
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

    //store current protocol values
    for(uint8_t i=0;i<3;i++)
        cur_protocol[i] =  rx_ok_buff[i];

    // decode channel/failsafe values
    volatile uint8_t *p=rx_ok_buff+3;
    uint8_t dec=-3;
    for(uint8_t i=0;i<NUM_CHN;i++)
    {
        dec+=3;
        if(dec>=8)
        {
            dec-=8;
            p++;
        }
        p++;
        uint16_t temp=((*((uint32_t *)p))>>dec)&0x7FF;
        #ifdef FAILSAFE_ENABLE
            if(failsafe)
                Failsafe_data[i]=temp;          //value range 0..2047, 0=no pulses, 2047=hold
            else
        #endif
                Channel_data[i]=temp;           //value range 0..2047, 0=-125%, 2047=+125%
    }
    RX_DONOTUPDATE_off;
        cli();
    if(IS_RX_MISSED_BUFF_on)                    // If the buffer is still valid
    {   memcpy((void*)rx_ok_buff,(const void*)rx_buff,RXBUFFER_SIZE);// Duplicate the buffer
        RX_FLAG_on;                             // data to be processed next time...
        RX_MISSED_BUFF_off;
    }
        sei();
    #ifdef FAILSAFE_ENABLE
        if(failsafe)
            debugln("RX_FS:%d,%d,%d,%d",Failsafe_data[0],Failsafe_data[1],Failsafe_data[2],Failsafe_data[3]);
    #endif
}

void modules_reset()
{
    #ifdef  CC2500_INSTALLED
        CC2500_Reset();
    #endif
    #ifdef  A7105_INSTALLED
        A7105_Reset();
    #endif
    #ifdef  CYRF6936_INSTALLED
        CYRF_Reset();
    #endif
    #ifdef  NRF24L01_INSTALLED
        NRF24L01_Reset();
    #endif

    //Wait for every component to reset
    delay(100);
    prev_power=0xFD;        // unused power value
}

#ifdef STM32_BOARD
    void usart2_begin(uint32_t baud,uint32_t config )
    {
        usart_init(USART2);
        usart_config_gpios_async(USART2,GPIOA,PIN_MAP[PA3].gpio_bit,GPIOA,PIN_MAP[PA2].gpio_bit,config);
        LED2_output;
        usart_set_baud_rate(USART2, STM32_PCLK1, baud);
        usart_enable(USART2);
    }
    void usart3_begin(uint32_t baud,uint32_t config )
    {
        usart_init(USART3);
        usart_config_gpios_async(USART3,GPIOB,PIN_MAP[PB11].gpio_bit,GPIOB,PIN_MAP[PB10].gpio_bit,config);
        usart_set_baud_rate(USART3, STM32_PCLK1, baud);
        usart_enable(USART3);
    }
    void init_HWTimer()
    {
        HWTimer2.pause();                                   // Pause the timer2 while we're configuring it

        TIMER2_BASE->PSC = 35;                              // 36-1;for 72 MHZ /0.5sec/(35+1)
        TIMER2_BASE->ARR = 0xFFFF;                          // Count until 0xFFFF

        HWTimer2.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);  // Main scheduler
        HWTimer2.setMode(TIMER_CH2, TIMER_OUTPUT_COMPARE);  // Serial check

        TIMER2_BASE->SR = 0x1E5F & ~TIMER_SR_CC2IF;         // Clear Timer2/Comp2 interrupt flag
        HWTimer2.attachInterrupt(TIMER_CH2,ISR_COMPB);      // Assign function to Timer2/Comp2 interrupt
        TIMER2_BASE->DIER &= ~TIMER_DIER_CC2IE;             // Disable Timer2/Comp2 interrupt

        HWTimer2.refresh();                                 // Refresh the timer's count, prescale, and overflow
        HWTimer2.resume();
    }
#endif

#if defined(TELEMETRY)
void PPM_Telemetry_serial_init()
{
    if( (protocol==PROTO_FRSKYD))
        initTXSerial( SPEED_9600 ) ;
    if(protocol==PROTO_FRSKYX)
        initTXSerial( SPEED_57600 ) ;
    if(protocol==PROTO_DSM)
        initTXSerial( SPEED_125K ) ;
}
#endif

// Convert 32b id to rx_tx_addr
void set_rx_tx_addr(uint32_t id)
{ // Used by almost all protocols
    rx_tx_addr[0] = (id >> 24) & 0xFF;
    rx_tx_addr[1] = (id >> 16) & 0xFF;
    rx_tx_addr[2] = (id >>  8) & 0xFF;
    rx_tx_addr[3] = (id >>  0) & 0xFF;
    rx_tx_addr[4] = (rx_tx_addr[2]&0xF0)|(rx_tx_addr[3]&0x0F);
}

uint32_t random_id(uint16_t address, uint8_t create_new)
{
    #ifndef FORCE_GLOBAL_ID
        uint32_t id=0;

        #if 0
        //(eeprom_read_byte((EE_ADDR)(address+10))==0xf0 && !create_new)
        {  // TXID exists in EEPROM
            for(uint8_t i=4;i>0;i--)
            {
                id<<=8;
                id|=eeprom_read_byte((EE_ADDR)address+i-1);
            }
            if(id!=0x2AD141A7)  //ID with seed=0
            {
                debugln("Read ID from EEPROM");
                return id;
            }
        }
        // Generate a random ID
        #if defined STM32_BOARD
            #define STM32_UUID ((uint32_t *)0x1FFFF7E8)
            if (!create_new)
            {
                id = STM32_UUID[0] ^ STM32_UUID[1] ^ STM32_UUID[2];
                debugln("Generated ID from STM32 UUID");
            }
            else
        #endif
    #endif
                id = random(0xfefefefe) + ((uint32_t)random(0xfefefefe) << 16);

    #if 0
        for(uint8_t i=0;i<4;i++)
            eeprom_write_byte((EE_ADDR)address+i,id >> (i*8));
        eeprom_write_byte((EE_ADDR)(address+10),0xf0);//write bind flag in eeprom.
    #endif
        return id;
    #else
        (void)address;
        (void)create_new;
        return FORCE_GLOBAL_ID;
    #endif
}

/**************************/
/**************************/
/**  Interrupt routines  **/
/**************************/
/**************************/

//PPM
#ifdef blubber // ENABLE_PPM
    #ifdef ORANGE_TX
        #if PPM_pin == 2
            ISR(PORTD_INT0_vect)
        #else
            ISR(PORTD_INT1_vect)
        #endif
    #elif defined STM32_BOARD
        void PPM_decode()
    #else
        #if PPM_pin == 2
            ISR(INT0_vect, ISR_NOBLOCK)
        #else
            ISR(INT1_vect, ISR_NOBLOCK)
        #endif
    #endif
    {   // Interrupt on PPM pin
        static int8_t chan=0,bad_frame=1;
        static uint16_t Prev_TCNT1=0;
        uint16_t Cur_TCNT1;

        Cur_TCNT1 = TCNT1 - Prev_TCNT1 ;    // Capture current Timer1 value
        if(Cur_TCNT1<1600)
            bad_frame=1;                    // bad frame
        else
            if(Cur_TCNT1>4400)
            {  //start of frame
                if(chan>=MIN_PPM_CHANNELS)
                {
                    PPM_FLAG_on;            // good frame received if at least 4 channels have been seen
                    if(chan>PPM_chan_max) PPM_chan_max=chan;    // Saving the number of channels received
                }
                chan=0;                     // reset channel counter
                bad_frame=0;
            }
            else
                if(bad_frame==0)            // need to wait for start of frame
                {  //servo values between 800us and 2200us will end up here
                    PPM_data[chan]=Cur_TCNT1;
                    if(chan++>=MAX_PPM_CHANNELS)
                        bad_frame=1;        // don't accept any new channels
                }
        Prev_TCNT1+=Cur_TCNT1;
    }
#endif //ENABLE_PPM

//Serial RX
#ifdef ENABLE_SERIAL
    #ifdef ORANGE_TX
        ISR(USARTC0_RXC_vect)
    #elif defined STM32_BOARD
        void __irq_usart2()
    #else
        ISR(USART_RX_vect)
    #endif
    {   // RX interrupt
        static uint8_t idx=0;
        #ifdef ORANGE_TX
            if((USARTC0.STATUS & 0x1C)==0)      // Check frame error, data overrun and parity error
        #elif defined STM32_BOARD
            if((USART2_BASE->SR & USART_SR_RXNE) && (USART2_BASE->SR &0x0F)==0)
        #else
            UCSR0B &= ~_BV(RXCIE0) ;            // RX interrupt disable
            sei() ;
            if((UCSR0A&0x1C)==0)                // Check frame error, data overrun and parity error
        #endif
        { // received byte is ok to process
            if(idx==0||discard_frame==1)
            {   // Let's try to sync at this point
                idx=0;discard_frame=0;
                RX_MISSED_BUFF_off;         // If rx_buff was good it's not anymore...
                rx_buff[0]=UDR0;
                #ifdef FAILSAFE_ENABLE
                    if((rx_buff[0]&0xFC)==0x54) // If 1st byte is 0x54, 0x55, 0x56 or 0x57 it looks ok
                #else
                    if((rx_buff[0]&0xFE)==0x54) // If 1st byte is 0x54 or 0x55 it looks ok
                #endif
                {
                    TX_RX_PAUSE_on;
                    tx_pause();
                    #if defined STM32_BOARD
                        TIMER2_BASE->CCR2=TIMER2_BASE->CNT+(6500L); // Full message should be received within timer of 3250us
                        TIMER2_BASE->SR = 0x1E5F & ~TIMER_SR_CC2IF; // Clear Timer2/Comp2 interrupt flag
                        TIMER2_BASE->DIER |= TIMER_DIER_CC2IE;      // Enable Timer2/Comp2 interrupt
                    #else
                        OCR1B = TCNT1+(6500L) ; // Full message should be received within timer of 3250us
                        TIFR1 = OCF1B_bm ;      // clear OCR1B match flag
                        SET_TIMSK1_OCIE1B ;     // enable interrupt on compare B match
                    #endif
                    idx++;
                }
            }
            else
            {
                rx_buff[idx++]=UDR0;        // Store received byte
                if(idx>=RXBUFFER_SIZE)
                {   // A full frame has been received
                    if(!IS_RX_DONOTUPDATE_on)
                    { //Good frame received and main is not working on the buffer
                        memcpy((void*)rx_ok_buff,(const void*)rx_buff,RXBUFFER_SIZE);// Duplicate the buffer
                        RX_FLAG_on;         // flag for main to process servo data
                    }
                    else
                        RX_MISSED_BUFF_on;  // notify that rx_buff is good
                    discard_frame=1;        // start again
                }
            }
        }
        else
        {
            idx=UDR0;                       // Dummy read
            discard_frame=1;                // Error encountered discard full frame...
            debugln("Bad frame RX");
        }
        if(discard_frame==1)
        {
            #ifdef STM32_BOARD
                TIMER2_BASE->DIER &= ~TIMER_DIER_CC2IE; // Disable Timer2/Comp2 interrupt
            #else
                CLR_TIMSK1_OCIE1B;          // Disable interrupt on compare B match
            #endif
            TX_RX_PAUSE_off;
            tx_resume();
        }
        #if not defined (ORANGE_TX) && not defined (STM32_BOARD)
            cli() ;
            UCSR0B |= _BV(RXCIE0) ;         // RX interrupt enable
        #endif
    }

    //Serial timer
    #ifdef ORANGE_TX
        ISR(TCC1_CCB_vect)
    #elif defined STM32_BOARD
        void ISR_COMPB()
    #else
        ISR(TIMER1_COMPB_vect, ISR_NOBLOCK )
    #endif
    {   // Timer1 compare B interrupt
        discard_frame=1;
        #ifdef STM32_BOARD
            TIMER2_BASE->DIER &= ~TIMER_DIER_CC2IE; // Disable Timer2/Comp2 interrupt
            debugln("Bad frame timer");
        #else
            CLR_TIMSK1_OCIE1B;                      // Disable interrupt on compare B match
        #endif
        tx_resume();
    }
#endif //ENABLE_SERIAL
