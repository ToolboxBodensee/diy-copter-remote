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
/********************/
/**  SPI routines  **/
/********************/
#include "spi.h"
#include "pins.h"
SPI::SPI()
{
}
void SPI::init(void)
{
    SPI_SCLK_OUTPUT;
    SPI_MOSI_OUTPUT;
    SPI_MISO_INPUT;
}

void SPI::write(const uint8_t command)
{
    uint8_t tmp = command;

    //SCK start low
    SPI_SCLK_OFF;
    SPI_MOSI_OFF;

    for (uint8_t i = 0; i< 8; ++i) {
        // check if bit is set
        if(tmp & 0x80) {
            SPI_MOSI_ON;
        } else {
            SPI_MOSI_OFF;
        }

        // trigger clock
        SPI_SCLK_ON;
        // go to next bit
        tmp = tmp << 1;
        // reset clock
        SPI_SCLK_OFF;
    }
    SPI_MOSI_ON;
}

uint8_t SPI::read(void)
{
    uint8_t result = 0;

    for(uint8_t i = 0; i < 8; i++) {

        result = result << 1;
        if(SPI_MISO_IS_HIGH) {
            result |= 0x01;
        }

        // trigger clock
        SPI_SCLK_ON;
        // give chip some time to react
        NOP;
        // reset clock
        SPI_SCLK_OFF;
    }
    return result;
}
