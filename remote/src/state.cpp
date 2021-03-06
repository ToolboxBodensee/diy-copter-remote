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
State *s_hf_cfg = NULL;
#ifndef ENABLE_DBEUG
State *s_usb  = NULL;
#endif
State *s_menu = NULL;


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
#ifndef ENABLE_DBEUG
    s_usb  = new LCD_state_joy_usb();
#endif
    s_menu = new LCD_state_menu();
    s_hf_cfg = new LCD_state_hf_cfg();

    lcd.backlight();
    curr_state = NULL;
    new_state = s_init;

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
void lcd_centerText(const char *text)
{
    int fieldWidth = 16;
    int padlen = 0;
    padlen = (fieldWidth - strlen(text)) / 2 ;
    char line[17] = "                ";
    sprintf(line, "%*s%s%*s", padlen, "", text, padlen, "");
    lcd.print(line);
}
