#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "FrSkyD_cc2500.h"
#include "state.h"
#include "input.h"
#include "eeprom.h"
#include "debug.h"
#include "tx_def.h"
#include "config.h"


LCD_state_bind::LCD_state_bind(void) {
    this->bind_time = DEFAULT_BIND_TIME;
}
void LCD_state_bind::enter(void) {
    lcd.setCursor(0,0);
    lcd.print("bind mode       ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    this->time_enter = millis();
}

void LCD_state_bind::update(void)
{
    char line[17];
    unsigned long time_in_ms = millis() - this->time_enter;
    unsigned long time_in_s = time_in_ms/1000; // to sec
    unsigned long remain_s = this->bind_time - time_in_s;

    snprintf(line,sizeof(line),"remaining sec %02lu",remain_s);
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
        next_callback_time = ReadFrSky_2way_bind();


        // update time
        time_in_ms = millis() - this->time_enter;
        time_in_s = time_in_ms/1000; // to sec
        remain_s = this->bind_time - time_in_s;

        // print on lcd
        snprintf(line,sizeof(line),"%02lu",remain_s);
        lcd.setCursor(14,1);
        lcd.print(line);

        input.update();
        if (input.is_menu_triggered()) {
            debug("%lu menu button trigger\n", millis);
            new_state = s_menu;
            break;
        }

        end__ = micros();

        if (end__ - start < next_callback_time) {
            uint32_t wait = next_callback_time;
            wait -= ((end__-start));
            delayMicroseconds(wait);
            end__ += wait;
        }
        end__ = micros();

        if (remain_s == 0)
            break;
    }

    // cange to menu when done
    new_state = s_menu;
}

void LCD_state_bind::leave(void)
{
    lcd.clear();
}
