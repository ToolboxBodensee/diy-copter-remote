#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "FrSkyD_cc2500.h"
#include "telemetry.h"
#include "state.h"
#include "input.h"
#include "eeprom.h"
#include "debug.h"
#include "tx_def.h"
#include "pins.h"

int16_t map16b(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);

LCD_state_fly::LCD_state_fly(void) {
}

void LCD_state_fly::enter(void) {

    this->last_time = 0;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("fly mode        ");
    this->time_enter = millis();

    byte clock_char_data[] = {
        0b01110,
        0b10101,
        0b10101,
        0b10111,
        0b10001,
        0b10001,
        0b01110,
        0b00000
    };
    lcd.createChar(clock_char, clock_char_data);
    lcd.setCursor(0,1);
    lcd.write(clock_char);

    byte rssi_antenna_[] = {
        0b10101,
        0b10001,
        0b01110,
        0b00100,
        0b00100,
        0b00100,
        0b00101,
        0b00101, };
    lcd.createChar(rssiantenna, rssi_antenna_);


    byte rssi_bars_[8];
    rssi_bars_[0] =  0b00001;
    rssi_bars_[1] =  0b00001;
    rssi_bars_[2] =  0b00001;
    rssi_bars_[3] =  0b00001;
    rssi_bars_[4] =  0b00101;
    rssi_bars_[5] =  0b00101;
    rssi_bars_[6] =  0b10101;
    rssi_bars_[7] =  0b10101;

    lcd.createChar(rssi_bars, rssi_bars_);


    byte battery_char_data[8];
    battery_char_data[0] = 0b01110;
    battery_char_data[1] = 0b11011;
    battery_char_data[2] = 0b10001;
    battery_char_data[3] = 0b10001;
    battery_char_data[4] = 0b10001;
    battery_char_data[5] = 0b10001;
    battery_char_data[6] = 0b10001;
    battery_char_data[7] = 0b11111;
    lcd.createChar(battery_char, battery_char_data);

    lcd.setCursor(12,0);
    lcd.write(battery_char);

    //lcd.setCursor(12,1);
    //lcd.write(battery_char);

    lcd.setCursor(6,1);
    lcd.write(rssiantenna);

    lcd.setCursor(7,1);
    lcd.write(rssi_bars);
}

void LCD_state_fly::print_time(uint16_t time)
{
    char line[17];

    /**
     * 0123456789012345
     * fly mode    A PP
     * T SSS AA PP A PP
     **/
    if(this->last_time == time)
        return;

    this->last_time=time;

    lcd.setCursor(1,1);
    snprintf(line, sizeof(line), "%*u", 3, time);
    lcd.print(line);
}
void LCD_state_fly::print_akku_quad(uint8_t akku_quad)
{
    /**
     * 0123456789012345
     * fly mode    A PP
     * T SSS AA PP A PP
     **/
    char line[17];

    lcd.setCursor(13,1);
    snprintf(line, sizeof(line), "%*d", 3, akku_quad);
    lcd.print(line);
    return;
}

void LCD_state_fly::print_akku_remote(uint8_t akku_remote)
{
    /**
     * 0123456789012345
     * fly mode    A PP
     * T SSS AA PP A PP
     **/

    char line[17];

    lcd.setCursor(13,0);
    snprintf(line, sizeof(line), "%*d", 3, akku_remote);
    lcd.print(line);
    return;
}

void LCD_state_fly::print_rssi(uint8_t rssi_percent)
{
    char line[17];

    lcd.setCursor(8,1);
    snprintf(line, sizeof(line), "%*d", 3, rssi_percent);
    lcd.print(line);
}

void LCD_state_fly::update(void)
{
    uint8_t call=0;
    uint8_t rssi_percent = freq_offset;
    uint8_t akku_quad = freq_offset;
    uint32_t akku_remote = 2;
    uint32_t old_akku_remote = 2;
    unsigned long time_in_ms = millis() - this->time_enter;
    unsigned long time_in_s = time_in_ms/1000; // to sec

    uint32_t end__ = micros();
    uint32_t start = micros();
    uint32_t next_callback_time;
    next_callback_time = initFrSky_2way();

    input.update();
    // init for bind
    frsky2way_init(1);

    input.print();
    while(1) {
        start = end__;
        next_callback_time = ReadFrSky_2way();

        //if (next_callback_time > 9000) {
        if (next_callback_time == 9200) {
            input.update();

            if (input.is_menu_triggered()) {
                debug("%lu menu button trigger\n", millis());
                input.print();
                break;
            }
        }


        if (next_callback_time == 9200) {
            // print on lcd
            call +=2;
            if(call > 50)
                call= 0;

            switch(call)
            {


            case 10:
                // update time
                time_in_ms = millis() - this->time_enter;
                time_in_s = time_in_ms/1000; // to sec
                this->print_time(time_in_s);
                break;

            case 20:
                rssi_percent = freq_offset;//TX_RSSI;
                this->print_rssi(rssi_percent);
                break;

            case 30:
                // update akku
                //this->print_akku_quad(akku_quad);
                break;

            case 40:
                // update akku
                // 3.7 -> 1560
                // 4.22 -> 1440
                pinMode(Battery_pin, INPUT);
                for(int i = 0; i < 10; ++i)
                    akku_remote += analogRead(Battery_pin);
                akku_remote /=10;
                akku_remote = (old_akku_remote +akku_remote )/2;
                old_akku_remote = akku_remote;
                debug("akku %u \n", akku_remote);
                if (akku_remote > 1560)
                    akku_remote = 1560;
                if (akku_remote < 1440)
                    akku_remote = 1440;
                akku_remote = map16b(akku_remote, 1440, 1560, 100, 0);
                if (akku_remote > 100)
                    akku_remote = 100;
                this->print_akku_remote(akku_remote);
                break;

            default:
                break;
            }

        }


        end__ = micros();
        if (end__ - start < next_callback_time) {
            uint32_t wait = next_callback_time;
            wait -= (end__ - start);
            delayMicroseconds(wait);
            /* if((end__ - start) > 1000 ) { */
            /*     debug("call %d waited %lu timed %lu vs wait %lu \n", call,next_callback_time, wait, (end__ - start)); */
            /*     print_frskyd_telemetry(); */
            /* } */
        }
        end__ = micros();
    }

    // cange to menu when done
    new_state = s_menu;

}
void LCD_state_fly::leave(void)
{
    lcd.clear();
}
