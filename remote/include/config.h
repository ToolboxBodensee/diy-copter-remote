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
//#define REMOE_OWNER "phschoen"
#define REMOE_OWNER "Marleen"
//#define REMOE_OWNER "Sina"
//#define REMOE_OWNER "Malaika"
//#define REMOE_OWNER "Pauline"
//#define REMOE_OWNER "Mara"

#define DEFAULT_BIND_TIME 13 /*seconds*/

/** Low Power **/
//Low power is reducing the transmit power of the multi module. This setting is configurable per model in PPM (table below) or Serial mode (radio GUI).
//It can be activated when flying indoor or small models since the distance is short or if a model is causing issues when flying closed to the TX.
//By default low power is completly disabled on all rf chips to prevent mistakes, but you can enable it by uncommenting the lines below:
//#define CC2500_ENABLE_LOW_POWER


/*****************/
/*** GLOBAL ID ***/
/*****************/
//#define FORCE_GLOBAL_ID 0x12345678

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
/*** TELEMETRY SETTINGS ***/
/**************************/
//In this section you can configure the telemetry.

//If you do not plan using the telemetry comment this global setting using "//" and skip to the next section.
#define TELEMETRY

//Comment if you don't want to send Multi status telemetry frames (Protocol available, Bind in progress, version...)
//Use with er9x/erksy9x, for OpenTX MULTI_TELEMETRY below is preferred instead
//#define MULTI_STATUS

//Uncomment to send Multi status and allow OpenTX to autodetect the telemetry format
//Supported by OpenTX version 2.2 RC9 and newer. NOT supported by er9x/ersky9x use MULTI_STATUS instead.
//#define MULTI_TELEMETRY

//Comment a line to disable a specific protocol telemetry
//#define SPORT_TELEMETRY               // Use FrSkyX SPORT format to send telemetry to TX
//#define HUB_TELEMETRY             // Use FrSkyD Hub format to send telemetry to TX

//SPORT_POLLING is an implementation of the same polling routine as XJT module for sport telemetry bidirectional communication.
//This is useful for passing sport control frames from TX to RX(ex: changing Betaflight PID or VTX channels on the fly using LUA scripts with OpentX).
//Using this feature requires to uncomment INVERT_TELEMETRY as this TX output on telemetry pin only inverted signal.
//!!!! This is a work in progress!!! Do not enable unless you want to test and report
//#define SPORT_POLLING

#endif
