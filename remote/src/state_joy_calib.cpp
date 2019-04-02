#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "FrSkyD_cc2500.h"
#include "state.h"
#include "input.h"
#include "eeprom.h"
#include "debug.h"
#include "tx_def.h"

LCD_state_joy_calibration::LCD_state_joy_calibration(void) {

}
void LCD_state_joy_calibration::enter(void) {
    lcd.setCursor(0,0);
    lcd.print("calib. start    ");
    lcd.setCursor(0,1);
    lcd.print("move all sticks ");
    delay(500);
}
void show_dots(int row, int number) {
    lcd.setCursor(0,row);
    for(int c = 0; c < 16;++c) {
        if (c < number)
            lcd.print(".");
        else
            lcd.print(" ");
    }
}
void LCD_state_joy_calibration::update(void) {
    int8_t turns = 50;
    int8_t i;

    // init min/max
    input.calibration_reset();

    // min max calibration
    lcd.setCursor(0,0);
    lcd.print("min max Calib.  ");
    show_dots(1,16);
    i = turns;
    while(i > 0) {
        input.update();
        if (true == input.calibration_update()) {
            i = turns;
        }else {
            i -= 1;
            show_dots(1,(i *16)/turns);
        }
        delay(100);
    }

    // center
    lcd.setCursor(0,0);
    lcd.print("center sticks   ");
    i = turns;
    while(i > 0) {
        input.update();
        if (false == input.is_centered()) {
            i = turns;
        }else {
            i -= 1;
            show_dots(1,(i *16)/turns);
        }
        delay(100);
    }

    for (uint8_t _ch = 0; _ch < 4 ; ++_ch) {
        enum Input::input_channels ch = (enum Input::input_channels) _ch;
        lcd.setCursor(0,0);
        lcd.print("move to max:    ");
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(ch_name[ch]);

        i = turns;
        while(i>0) {
            delay(50);
            input.update();

            input.print_ch(ch);
            if (input.is_high(ch)) {
                i--;
                continue;
            }

            if (input.is_low(ch)) {
                input.invert_ch(ch);
                debug("invert");
                i = turns;
                continue;
            }
        }
    }

    struct Input::ch_config ch_config[Input::CH_COUNT];
    input.get_calibration(ch_config);


    lcd.setCursor(0,0);
    lcd.print("center again    ");
    lcd.setCursor(0,1);
    lcd.print("all sticks      ");
    while (false == input.is_centered()) {
        input.update();
    }
    new_state = s_menu;
}
void LCD_state_joy_calibration::leave(void) {
    lcd.setCursor(0,0);
    lcd.print("finished ");
    lcd.setCursor(0,1);
    lcd.print("calibration");
}
