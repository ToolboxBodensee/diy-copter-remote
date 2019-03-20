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
//*******************
//***   Pinouts   ***
//*******************
#ifndef _PINS_H_
#define _PINS_H_

#include "Arduino.h"

#define LED_off
#define LED_on
#define LED_output
#define IS_LED_on false
#define LED_toggle

//
#define SDI_pin       PA0 /* SDIO MOSI */
#define SCLK_pin      PA1 /* SCLK */
#define SDO_pin       PA2 /* SDO / MISO */
#define CC25_CSN_pin  PA3 /* Chip select CC2500 */

#define Throttle_pin  PA4
#define Yaw_pin       PA5
//#define Roll_pin      PA6
//#define Pitch_pin     PA7
#define Roll_pin      PA7
#define Pitch_pin     PA6

#define Aux1_pin      PB0
#define Aux2_pin      PB0
#define Aux3_pin      PB0
#define Aux4_pin      PB0
#define Aux5_pin      PB0
#define Aux6_pin      PB0
#define Menu_pin      PB0

#define cli()
#define sei()
#define NOP() __asm__ __volatile__("nop")

#define SDI_on      digitalWrite(SDI_pin, HIGH)
#define SDI_off     digitalWrite(SDI_pin, LOW)
#define SDI_1       (digitalRead(SDI_pin) == HIGH)
#define SDI_0       (digitalRead(SDI_pin) == LOW)
#define SDI_input   pinMode(SDI_pin, INPUT);
#define SDI_output  pinMode(SDI_pin, OUTPUT);

#define SDO_on      digitalWrite(SDO_pin, HIGH)
#define SDO_off     digitalWrite(SDO_pin, LOW)
#define SDO_1       (digitalRead(SDO_pin) == HIGH)
#define SDO_0       (digitalRead(SDO_pin) == LOW)
#define SDO_input   pinMode(SDO_pin, INPUT);
#define SDO_output  pinMode(SDO_pin, OUTPUT);

#define SCLK_on      digitalWrite(SCLK_pin, HIGH)
#define SCLK_off     digitalWrite(SCLK_pin, LOW)
#define SCLK_1       (digitalRead(SCLK_pin) == HIGH)
#define SCLK_0       (digitalRead(SCLK_pin) == LOW)
#define SCLK_input   pinMode(SCLK_pin, INPUT);
#define SCLK_output  pinMode(SCLK_pin, OUTPUT);

#define CC25_CSN_on      digitalWrite(CC25_CSN_pin, HIGH)
#define CC25_CSN_off     digitalWrite(CC25_CSN_pin, LOW)
#define CC25_CSN_1       (digitalRead(CC25_CSN_pin) == HIGH)
#define CC25_CSN_0       (digitalRead(CC25_CSN_pin) == LOW)
#define CC25_CSN_input   pinMode(CC25_CSN_pin, INPUT);
#define CC25_CSN_output  pinMode(CC25_CSN_pin, OUTPUT);

//*******************
//***    EEPROM   ***
//*******************
#define EE_ADDR uint8_t*

#endif
