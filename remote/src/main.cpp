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
#include <Arduino.h>


#include "spi.h"
#include "cc2500.h"
#include "frskyd.h"
#include "io_data.h"

SPI *spi = NULL;
IO_DATA *io_data = NULL;
CC2500 *cc2500 = NULL;
Frsky_d *frskyd = NULL;

void setup()
{
    while (!Serial) {
        delay(500);
    }

    Serial.begin(115200);

    spi = new SPI();
    spi->init();

    cc2500 = new CC2500(spi);

    frskyd = new Frsky_d(cc2500);

    io_data = new IO_DATA();
    io_data->init();

    Serial.println("Remote ON!");

    delay (500);
    cc2500->reset();
    Serial.print(" init\n");
    frskyd->init(false);

    delay (500);
    Serial.print(" init 2way\n");
    frskyd->init_frsky_2way();

}

void loop()
{
    Serial.println("loop!");

    io_data->update();

    Serial.print(" akku= ");
    Serial.print(io_data->akku_power);
    Serial.print(" arm= ");
    Serial.print(io_data->arm_sw);
    Serial.print(" mode= ");
    Serial.print(io_data->flight_mode_sw);

    Serial.print("l x= ");
    Serial.print(io_data->r_x);
    Serial.print(" y= ");
    Serial.print(io_data->r_y);
    Serial.print(" sw= ");
    Serial.print(io_data->r_sw);

    Serial.print("r x= ");
    Serial.print(io_data->l_x);
    Serial.print(" y= ");
    Serial.print(io_data->l_y);
    Serial.print(" sw= ");
    Serial.print(io_data->l_sw);
    Serial.print("\n");

    //TODO needed ? PE1_off;
    //TODO needed ? PE2_on;

    Serial.print(" read 2 way\n");
    int rt = frskyd->read_frsky_2way();

    Serial.print(" read :");
    Serial.print(rt);
    Serial.print("\n");

    delayMicroseconds(rt);
}
