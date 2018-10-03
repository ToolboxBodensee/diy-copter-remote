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
#define LED_off
#define LED_on
#define LED_output
#define IS_LED_on false
#define LED_toggle
#if 1

  // SDIO MOSI
  #define SDI_pin     16
  #define SDI_on      digitalWrite(SDI_pin, HIGH)
  #define SDI_off     digitalWrite(SDI_pin, LOW)
  #define SDI_1       (digitalRead(SDI_pin) == HIGH)
  #define SDI_0       (digitalRead(SDI_pin) == LOW)
  #define SDI_input   pinMode(SDI_pin, INPUT);
  #define SDI_output  pinMode(SDI_pin, OUTPUT);

  //SDO / MISO
  #define SDO_pin     14
  #define SDO_on      digitalWrite(SDO_pin, HIGH)
  #define SDO_off     digitalWrite(SDO_pin, LOW)
  #define SDO_1       (digitalRead(SDO_pin) == HIGH)
  #define SDO_0       (digitalRead(SDO_pin) == LOW)
  #define SDO_input   pinMode(SDO_pin, INPUT);
  #define SDO_output  pinMode(SDO_pin, OUTPUT);
  
  // SCLK  
  #define SCLK_pin     15
  #define SCLK_on      digitalWrite(SCLK_pin, HIGH)
  #define SCLK_off     digitalWrite(SCLK_pin, LOW)
  #define SCLK_1       (digitalRead(SCLK_pin) == HIGH)
  #define SCLK_0       (digitalRead(SCLK_pin) == LOW)
  #define SCLK_input   pinMode(SCLK_pin, INPUT);
  #define SCLK_output  pinMode(SCLK_pin, OUTPUT);
  
  // Chip select CC2500
  #define CC25_CSN_pin     7
  #define CC25_CSN_on      digitalWrite(CC25_CSN_pin, HIGH)
  #define CC25_CSN_off     digitalWrite(CC25_CSN_pin, LOW)
  #define CC25_CSN_1       (digitalRead(CC25_CSN_pin) == HIGH)
  #define CC25_CSN_0       (digitalRead(CC25_CSN_pin) == LOW)
  #define CC25_CSN_input   pinMode(CC25_CSN_pin, INPUT);
  #define CC25_CSN_output  pinMode(CC25_CSN_pin, OUTPUT);

//*******************
//***    Timer    ***
//*******************
#define OCF1A_bm _BV(OCF1A)
#define OCF1B_bm _BV(OCF1B)
#define SET_TIMSK1_OCIE1B	TIMSK1 |= _BV(OCIE1B)
#define CLR_TIMSK1_OCIE1B	TIMSK1 &=~_BV(OCIE1B)
#else
  // SDIO MOSI
  #define SDI_pin     26
  #define SDI_on      digitalWrite(SDI_pin, HIGH)
  #define SDI_off     digitalWrite(SDI_pin, LOW)
  #define SDI_1       (digitalRead(SDI_pin) == HIGH)
  #define SDI_0       (digitalRead(SDI_pin) == LOW)
  #define SDI_input   pinMode(SDI_pin, INPUT);
  #define SDI_output  pinMode(SDI_pin, OUTPUT);

  //SDO / MISO
  #define SDO_pin     25
  #define SDO_on      digitalWrite(SDO_pin, HIGH)
  #define SDO_off     digitalWrite(SDO_pin, LOW)
  #define SDO_1       (digitalRead(SDO_pin) == HIGH)
  #define SDO_0       (digitalRead(SDO_pin) == LOW)
  #define SDO_input   pinMode(SDO_pin, INPUT);
  #define SDO_output  pinMode(SDO_pin, OUTPUT);
  
  // SCLK  
  #define SCLK_pin     33
  #define SCLK_on      digitalWrite(SCLK_pin, HIGH)
  #define SCLK_off     digitalWrite(SCLK_pin, LOW)
  #define SCLK_1       (digitalRead(SCLK_pin) == HIGH)
  #define SCLK_0       (digitalRead(SCLK_pin) == LOW)
  #define SCLK_input   pinMode(SCLK_pin, INPUT);
  #define SCLK_output  pinMode(SCLK_pin, OUTPUT);
  
  // Chip select CC2500
  #define CC25_CSN_pin     32
  #define CC25_CSN_on      digitalWrite(CC25_CSN_pin, HIGH)
  #define CC25_CSN_off     digitalWrite(CC25_CSN_pin, LOW)
  #define CC25_CSN_1       (digitalRead(CC25_CSN_pin) == HIGH)
  #define CC25_CSN_0       (digitalRead(CC25_CSN_pin) == LOW)
  #define CC25_CSN_input   pinMode(CC25_CSN_pin, INPUT);
  #define CC25_CSN_output  pinMode(CC25_CSN_pin, OUTPUT);
#endif 
//*******************
//***    EEPROM   ***
//*******************
#define EE_ADDR uint8_t*
