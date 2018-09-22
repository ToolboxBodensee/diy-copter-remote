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

	// SDIO MOSI
	#define SDI_pin	 2										//D5 = PD5
	#define SDI_port PORTB
	#define SDI_ipr  PINB
	#define SDI_ddr  DDRB
	#define SDI_on	SDI_port |= _BV(SDI_pin)
	#define SDI_off	SDI_port &= ~_BV(SDI_pin)
	#define SDI_1	(SDI_ipr & _BV(SDI_pin))
	#define SDI_0	(SDI_ipr & _BV(SDI_pin)) == 0x00
	#define SDI_input	SDI_ddr &= ~_BV(SDI_pin)
	#define SDI_output	SDI_ddr |=  _BV(SDI_pin)

	//SDO / MISO
	#define SDO_pin		3									//D6 = PD6
	#define SDO_port	PORTB
	#define SDO_ipr		PINB
	#define SDO_1 (SDO_ipr & _BV(SDO_pin))
	#define SDO_0 (SDO_ipr & _BV(SDO_pin)) == 0x00
  
	// SCLK 
	#define SCLK_port PORTB
	#define SCLK_ddr DDRB
	#define SCLK_pin	1								//D4 = PD4
	#define SCLK_output	SCLK_ddr  |=  _BV(SCLK_pin)
	#define SCLK_on		SCLK_port |=  _BV(SCLK_pin)
	#define SCLK_off	SCLK_port &= ~_BV(SCLK_pin)
 
	// Chip select CC2500
	#define CC25_CSN_pin	7								//D7 = PD7
	#define CC25_CSN_port	PORTE
	#define CC25_CSN_ddr	DDRE
	#define CC25_CSN_output	CC25_CSN_ddr  |=  _BV(CC25_CSN_pin)
	#define CC25_CSN_on		CC25_CSN_port |=  _BV(CC25_CSN_pin)
	#define CC25_CSN_off	CC25_CSN_port &= ~_BV(CC25_CSN_pin)

//*******************
//***    Timer    ***
//*******************
#define OCF1A_bm _BV(OCF1A)
#define OCF1B_bm _BV(OCF1B)
#define SET_TIMSK1_OCIE1B	TIMSK1 |= _BV(OCIE1B)
#define CLR_TIMSK1_OCIE1B	TIMSK1 &=~_BV(OCIE1B)

//*******************
//***    EEPROM   ***
//*******************
#define EE_ADDR uint8_t*
