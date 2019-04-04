#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "FrSkyD_cc2500.h"
#include "state.h"
#include "input.h"
#include "eeprom.h"
#include "debug.h"
#include "tx_def.h"

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
    lcd.setCursor(6,1);
    lcd.write(rssiantenna);

    byte rssi_bars_[] = {
        0b00001,
        0b00001,
        0b00001,
        0b00001,
        0b00101,
        0b00101,
        0b10101,
        0b10101 };
    lcd.createChar(rssi_bars, rssi_bars_);
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
    if(this->last_time==time)
        return;

    this->last_time=time;

    lcd.setCursor(1,1);
    snprintf(line, sizeof(line), "%*u", 3, time);
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
    for (int i = 0; i < 2; ++i) {
        battery_char_data[0] = 0b01110;
        battery_char_data[1] = ( akku[i] > 90) ? 0b11111 : 0b11011;
        battery_char_data[2] = ( akku[i] > 75) ? 0b11111 : 0b10001;
        battery_char_data[3] = ( akku[i] > 60) ? 0b11111 : 0b10001;
        battery_char_data[4] = ( akku[i] > 45) ? 0b11111 : 0b10001;
        battery_char_data[5] = ( akku[i] > 30) ? 0b11111 : 0b10001;
        battery_char_data[6] = ( akku[i] > 15) ? 0b11111 : 0b10001;
        battery_char_data[7] = 0b11111;
        lcd.createChar(battery_char+i, battery_char_data);

        lcd.setCursor(12,i);
        lcd.write(battery_char+i);

        lcd.setCursor(13,i);
        snprintf(line, sizeof(line), "%*d", 3, akku[i]);
        lcd.print(line);
    }
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
    uint8_t rssi_percent = 100;
    uint8_t akku_quad = 1;
    uint8_t akku_remote = 2;
    char line[17];
    unsigned long time_in_ms = millis() - this->time_enter;
    unsigned long time_in_s = time_in_ms/1000; // to sec

    this->print_time(time_in_s);
    this->print_akku(akku_quad, akku_remote);
    this->print_rssi(rssi_percent);

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
        if (next_callback_time > 9000) {
            input.update();

            if (input.is_menu_triggered()) {
                debug("%lu menu button trigger\n", millis());
                input.print();
                break;
            }


            // print on lcd
            call +=1;
            if(call > 5)
                call= 0;

            switch(call)
            {
            case 1:
                rssi_percent += 1;
                if(rssi_percent > 100)
                    rssi_percent = 0;
                this->print_rssi(rssi_percent);
                break;

            case 2:
                // update time
                time_in_ms = millis() - this->time_enter;
                time_in_s = time_in_ms/1000; // to sec
                this->print_time(time_in_s);
                break;

            case 3:
                // update akku
                akku_quad += 1;
                akku_remote += 2;
                if(akku_quad > 100)
                    akku_quad = 0;
                if(akku_remote > 100)
                    akku_remote = 0;
                this->print_akku(akku_quad, akku_remote);
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
