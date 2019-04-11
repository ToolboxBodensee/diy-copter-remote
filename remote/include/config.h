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
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#define REMOE_OWNER "phschoen"

#define DEFAULT_BIND_TIME 13 /*seconds*/

/*****************/
/*** AUTO BIND ***/  // Also referred as "Bind on powerup"
/*****************/
//Bind from channel enables you to bind when a specified channel is going from low to high. This feature is only active
// if you specify AUTOBIND in PPM mode or set AutoBind to YES for serial mode. It also requires that the throttle channel is low.
//Comment to globaly disable the bind feature from a channel.
#define ENABLE_BIND_CH
//Set the channel number used for bind. Default is 16.
#define BIND_CH 16

//Comment to disable the wait for bind feature. If Autobind is enabled in the model config, this feature will not activate
// the selected protocol unless a bind is requested using bind from channel or the GUI "Bind" button.
//The goal is to prevent binding other people's model when powering up the TX, changing model or scanning through protocols.
#define WAIT_FOR_BIND


/****************/
/*** RF CHIPS ***/
/****************/
//There are 4 RF components supported. If one of them is not installed you must comment it using "//".
//If a chip is not installed all associated protocols are automatically disabled.
//4-in-1 modules have all RF chips installed
//!!!If a RF chip is present it MUST be marked as installed!!! or weird things will happen you have been warned.
//#define A7105_INSTALLED
//#define CYRF6936_INSTALLED
#define CC2500_INSTALLED
//#define NRF24L01_INSTALLED

/** CC2500 Fine Frequency Tuning **/
//For optimal performance the CC2500 RF module used by the FrSkyD, FrSkyV, FrSkyX, SFHSS, CORONA and Hitec protocols needs to be tuned for each protocol.
//Initial tuning should be done via the radio menu with a genuine FrSky/Futaba/CORONA/Hitec receiver.
//Once a good tuning value is found it can be set here and will override the radio's 'option' setting for all existing and new models which use that protocol.
//For more information: https://github.com/pascallanger/DIY-Multiprotocol-TX-Module/tree/master/docs/Frequency_Tuning.md
//Uncomment the lines below (remove the "//") and set an appropriate value (replace the "0") to enable. Valid range is -127 to +127.
//#define FORCE_FRSKYD_TUNING   0
//#define FORCE_FRSKYV_TUNING   0
//#define FORCE_FRSKYX_TUNING   0

/** Low Power **/
//Low power is reducing the transmit power of the multi module. This setting is configurable per model in PPM (table below) or Serial mode (radio GUI).
//It can be activated when flying indoor or small models since the distance is short or if a model is causing issues when flying closed to the TX.
//By default low power is completly disabled on all rf chips to prevent mistakes, but you can enable it by uncommenting the lines below:
//#define CC2500_ENABLE_LOW_POWER


/*****************/
/*** GLOBAL ID ***/
/*****************/
//A global ID is used by most protocols to bind and retain the bind to models. To prevent duplicate IDs, it is automatically
// generated using a random 32 bits number the first time the eeprom is initialized.
//If you have 2 Multi modules which you want to share the same ID so you can use either to control the same RC model
// then you can force the ID to a certain known value using the lines below.
//Default is commented, you should uncoment only for test purpose or if you know exactly what you are doing!!!
#define FORCE_GLOBAL_ID 0x12345678

//Protocols using the CYRF6936 (DSM, Devo, Walkera...) are using the CYRF ID instead which should prevent duplicated IDs.
//If you have 2 Multi modules which you want to share the same ID so you can use either to control the same RC model
// then you can force the ID to a certain known value using the lines below.
//Default is commented, you should uncoment only for test purpose or if you know exactly what you are doing!!!
//#define FORCE_CYRF_ID "\x12\x34\x56\x78\x9A\xBC"


/****************************/
/*** PROTOCOLS TO INCLUDE ***/
/****************************/
//In this section select the protocols you want to be accessible when using the module.
//All the protocols will not fit in the Atmega328p module so you need to pick and choose.
//Comment the protocols you are not using with "//" to save Flash space.

//The protocols below need a CC2500 to be installed
#define FRSKYD_CC2500_INO
#define FRSKYV_CC2500_INO
#define FRSKYX_CC2500_INO


/**************************/
/*** FAILSAFE SETTINGS  ***/
/**************************/
//The module is using the same default failsafe values for all protocols which currently supports it:
//  Devo, WK2x01, SFHSS, HISKY/HK310 and AFHDS2A
//All channels are centered except throttle which is forced low.
//If you want to diasble failsafe globally comment the line below using "//".
#define FAILSAFE_ENABLE

//Failsafe throttle low value in percentage.
//Value between -125% and +125%. Default -100.
#define FAILSAFE_THROTTLE_LOW -100

//The radio using serial protocol can set failsafe data.
// Two options are available:
//  a. replace the default failsafe data with serial failsafe data when they are received.
//  b. wait for the radio to provide failsafe before sending it. Enable advanced settings like "FAILSAFE NOT SET" or "FAILSAFE RX".
// Option a. is the default since you have a protection even if no failsafe has been set on the radio.
// You can force option b. by uncommenting the line below (remove the "//").
//#define FAILSAFE_SERIAL_ONLY


/**************************/
/*** TELEMETRY SETTINGS ***/
/**************************/
//In this section you can configure the telemetry.

//If you do not plan using the telemetry comment this global setting using "//" and skip to the next section.
#define TELEMETRY

//Comment to invert the polarity of the output telemetry serial signal.
//This function takes quite some flash space and processor power on an atmega.
//For OpenTX it must be uncommented.
//On a 9XR_PRO running ersky9x both commented and uncommented will work depending on the radio setting Invert COM1 under the Telemetry menu.
//On other addon/replacement boards like the 9xtreme board or the Ar9x board running ersky9x, you need to uncomment the line below.
//For er9x it depends if you have an inveter mod or not on the telemetry pin. If you don't have an inverter comment this line.
//#define INVERT_TELEMETRY

//Comment if you don't want to send Multi status telemetry frames (Protocol available, Bind in progress, version...)
//Use with er9x/erksy9x, for OpenTX MULTI_TELEMETRY below is preferred instead
//#define MULTI_STATUS

//Uncomment to send Multi status and allow OpenTX to autodetect the telemetry format
//Supported by OpenTX version 2.2 RC9 and newer. NOT supported by er9x/ersky9x use MULTI_STATUS instead.
//#define MULTI_TELEMETRY

//Comment a line to disable a specific protocol telemetry
//#define DSM_TELEMETRY             // Forward received telemetry packet directly to TX to be decoded by er9x, ersky9x and OpenTX
//#define SPORT_TELEMETRY               // Use FrSkyX SPORT format to send telemetry to TX
//#define AFHDS2A_FW_TELEMETRY      // Forward received telemetry packet directly to TX to be decoded by ersky9x and OpenTX
//#define AFHDS2A_HUB_TELEMETRY     // Use FrSkyD Hub format to send basic telemetry to TX like er9x
//#define HUB_TELEMETRY             // Use FrSkyD Hub format to send telemetry to TX
//#define BAYANG_HUB_TELEMETRY      // Use FrSkyD Hub format to send telemetry to TX
//#define BUGS_HUB_TELEMETRY            // Use FrSkyD Hub format to send telemetry to TX
//#define HUBSAN_HUB_TELEMETRY      // Use FrSkyD Hub format to send telemetry to TX
//#define CABELL_HUB_TELEMETRY      // Use FrSkyD Hub format to send telemetry to TX
//#define HITEC_HUB_TELEMETRY           // Use FrSkyD Hub format to send basic telemetry to the radios which can decode it like er9x, ersky9x and OpenTX
//#define HITEC_FW_TELEMETRY            // Under development: Forward received telemetry packets to be decoded by ersky9x and OpenTX

//SPORT_POLLING is an implementation of the same polling routine as XJT module for sport telemetry bidirectional communication.
//This is useful for passing sport control frames from TX to RX(ex: changing Betaflight PID or VTX channels on the fly using LUA scripts with OpentX).
//Using this feature requires to uncomment INVERT_TELEMETRY as this TX output on telemetry pin only inverted signal.
//!!!! This is a work in progress!!! Do not enable unless you want to test and report
//#define SPORT_POLLING

/*************************/
/*** PPM MODE SETTINGS ***/
/*************************/
#define NBR_BANKS 1

struct PPM_Parameters
{
    uint8_t protocol : 6;
    uint8_t sub_proto : 3;
    uint8_t rx_num : 4;

#define P_HIGH      1
#define P_LOW       0
    uint8_t power : 1;

#define AUTOBIND    1
#define NO_AUTOBIND 0
    uint8_t autobind : 1;

    uint8_t option;
};

//******************
// Protocols
//******************
enum PROTOCOLS
{
    MODE_SERIAL     = 0,    // Serial commands
    PROTO_FRSKYD    = 3,    // =>CC2500
    PROTO_FRSKYX    = 15,   // =>CC2500
    PROTO_SFHSS     = 21,   // =>CC2500
    PROTO_FRSKYV    = 25,   // =>CC2500
    PROTO_CORONA    = 37,   // =>CC2500
    PROTO_HITEC     = 39,   // =>CC2500
};

enum FRSKYX_SUP_PROTOCOL
{
    CH_16   = 0,
    CH_8    = 1,
    EU_16   = 2,
    EU_8    = 3,
};

extern const PPM_Parameters PPM_prot[14*NBR_BANKS];
#endif
