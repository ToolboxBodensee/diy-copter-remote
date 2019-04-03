#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "FrSkyD_cc2500.h"
#include "state.h"
#include "input.h"
#include "eeprom.h"
#include "debug.h"
#include "tx_def.h"
#include "common.h"

LCD_state_fly::LCD_state_fly(void) {
}

void LCD_state_fly::enter(void) {
    lcd.setCursor(0,0);
    lcd.print("fly mode        ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    this->time_enter = millis();
}

void LCD_state_fly::print_time(uint16_t time)
{
    /**
     * 0123456789012345
     * fly mode    A PP
     * T SSS AA PP A PP
     **/

    char line[17];
    lcd.setCursor(0,1);
    byte clock_char_data[8] = {
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
    lcd.write(clock_char);

    snprintf(line, sizeof(line), "%*d", 3, time);
    lcd.print(line);
}
void LCD_state_fly::print_akku(uint8_t akku_quad, uint8_t akku_remote)
{
    /**
     * 0123456789012345
     * fly mode    A PP
     * T SSS AA PP A PP
     **/
    byte battery_char_data[8];

    char line[17];
    uint8_t akku[] = {akku_remote, akku_quad};
    for (uint8_t i = 0; i < 2; ++i) {
        lcd.setCursor(12,i);
        battery_char_data[0] = 0b01110;
        battery_char_data[1] = ( akku[i] > 90) ? 0b11111 : 0b11011;
        battery_char_data[2] = ( akku[i] > 75) ? 0b11111 : 0b10001;
        battery_char_data[3] = ( akku[i] > 60) ? 0b11111 : 0b10001;
        battery_char_data[4] = ( akku[i] > 45) ? 0b11111 : 0b10001;
        battery_char_data[5] = ( akku[i] > 30) ? 0b11111 : 0b10001;
        battery_char_data[6] = ( akku[i] > 15) ? 0b11111 : 0b10001;
        battery_char_data[7] = 0b11111;
        lcd.createChar(battery_char, battery_char_data);
        lcd.write(battery_char);
        snprintf(line, sizeof(line), " %*d", 2, akku[i]);
        lcd.print(line);
    }
}
void LCD_state_fly::print_rssi(uint8_t rssi_percent)
{
    char line[17];
    byte rssi_antenna[8] = { 0b10101, 0b10101, 0b01110, 0b00100, 0b00100, 0b00101, 0b00101, 0b00101 };

    byte rssi_antenna_1[8] = { 0b00001, 0b00001, 0b00001, 0b00001, 0b00101, 0b00101, 0b10101, 0b10101 };
    byte rssi_antenna_2[8] = { 0b00001, 0b00001, 0b00001, 0b00001, 0b10101, 0b10101, 0b10101, 0b10101 };
    byte rssi_antenna_3[8] = { 0b00001, 0b00001, 0b00101, 0b00101, 0b10101, 0b10101, 0b10101, 0b10101 };

    lcd.setCursor(6,1);
    lcd.createChar(rssiantenna, rssi_antenna);

    lcd.write(rssiantenna);
    if (rssi_percent > 75) {/* 100-75 -> 3 */
        lcd.createChar(rssi_bars, rssi_antenna_3);
        lcd.write(rssi_bars);
    } else if (rssi_percent > 50) { /* 74-50 -> 2 */
        lcd.createChar(rssi_bars, rssi_antenna_2);
        lcd.write(rssi_bars);
    } else if (rssi_percent > 25) { /* 49-25 -> 1 */
        lcd.createChar(rssi_bars, rssi_antenna_1);
        lcd.write(rssi_bars);
    } else { /* 25-0 -> 0 */
        lcd.write(' ');
    }

    snprintf(line, sizeof(line), " %*d", 2, rssi_percent);
    lcd.print(line);
}

void LCD_state_fly::update(void)
{
    uint8_t rssi_percent = 100;
    uint8_t akku_quad = 1;
    uint8_t akku_remote = 2;
    char line[17];
    unsigned long time_in_ms = millis() - this->time_enter;
    unsigned long time_in_s = time_in_ms/1000; // to sec

    snprintf(line, sizeof(line), "fly mode %02lu sec", time_in_s);
    lcd.setCursor(0,1);
    lcd.print(line);

    uint32_t end__ = micros();
    uint32_t start = micros();
    uint32_t next_callback_time;
    next_callback_time = initFrSky_2way();

    // init for bind
    frsky2way_init(1);
    while(1) {
        start = end__;
        next_callback_time = ReadFrSky_2way();

        if (state == FRSKY_DATA1) {
            // update time
            time_in_ms = millis() - this->time_enter;
            time_in_s = time_in_ms/1000; // to sec

            // print on lcd
            this->print_time(time_in_s);

            rssi_percent += 1;
            this->print_time(rssi_percent);

            akku_quad += 1;
            akku_remote += 2;
            this->print_akku(akku_quad, akku_remote);

            input.update();
        }


        end__ = micros();
        if (end__ - start < next_callback_time) {
            uint32_t wait = next_callback_time;
            wait -= (end__ - start);
            delayMicroseconds(wait);
        }
        end__ = micros();

        if (input.is_menu_triggered() == 0)
            break;

    }

    // cange to menu when done
    new_state = s_menu;

}
void LCD_state_fly::leave(void)
{
    lcd.clear();
}
