#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "FrSkyD_cc2500.h"
#include "state.h"
#include "input.h"
#include "eeprom.h"
#include "debug.h"
#include "tx_def.h"

LiquidCrystal_I2C lcd(0x27,16,2);

State *curr_state = NULL;
State *new_state = NULL;

State *s_init = NULL;
State *s_bind = NULL;
State *s_fly  = NULL;
State *s_joy  = NULL;
State *s_menu = NULL;


enum lcd_special_chars {
    battery_66   = 0,
    battery_33   = 1,
    battery_0    = 2,
    battery_100  = 3,
    rssiantenna  = 4,
    rssi_bars_1  = 5,
    rssi_bars_2  = 6,
    rssi_bars_3  = 7,
    MAX_SPECIAL_CHARS =8,
};
// 6 Byte-Arrays für 6 verschiedene Batteriesymbole
__extension__ struct lcd_special_chars_data { byte data[MAX_SPECIAL_CHARS]; }
   lcd_special_chars_data[MAX_SPECIAL_CHARS] =
{
    //[battery_0]   =
    { 0b01110, 0b11011, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11111 },
    //[battery_33]  =
    { 0b01110, 0b11011, 0b10001, 0b10001, 0b10001, 0b11111, 0b11111, 0b11111 },
    //[battery_66]  =
    { 0b01110, 0b11011, 0b10001, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 },
    //[battery_100] =
    { 0b01110, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 },
    //[rssiantenna] =
    { 0b10101, 0b10101, 0b01110, 0b00100, 0b00100, 0b00101, 0b00101, 0b00101 },
    //[rssi_bars_1] =
    { 0b00001, 0b00001, 0b00001, 0b00001, 0b00101, 0b00101, 0b10101, 0b10101 },
    //[rssi_bars_2] =
    { 0b00001, 0b00001, 0b00001, 0b00001, 0b10101, 0b10101, 0b10101, 0b10101 },
    //[rssi_bars_3] =
    { 0b00001, 0b00001, 0b00101, 0b00101, 0b10101, 0b10101, 0b10101, 0b10101 },
};


void install_special_caracters(void)
{
    for(int i = 0; i < MAX_SPECIAL_CHARS; i ++) {
        lcd.createChar(i, lcd_special_chars_data[i].data);
    }
}

void init_state(void) {
    #if 0
    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    #endif
    Wire.begin();
    lcd.init();

    s_init = new LCD_state_init();
    s_bind = new LCD_state_bind();
    s_fly  = new LCD_state_fly();
    s_joy  = new LCD_state_joy_calibration();
    s_menu = new LCD_state_menu();
    lcd.backlight();
    curr_state = NULL;
    new_state = s_init;

    install_special_caracters();
    update_state();
}

void update_state(void) {
    if(curr_state == new_state) {
        if(curr_state)
            curr_state->update();
    } else {
        if(curr_state)
            curr_state->leave();

        curr_state = new_state;

        if(curr_state)
            curr_state->enter();
    }
}
