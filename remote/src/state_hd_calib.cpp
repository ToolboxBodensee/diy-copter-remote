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


LCD_state_hf_cfg::LCD_state_hf_cfg(void) {
}
void LCD_state_hf_cfg::enter(void) {
    lcd.setCursor(0,0);
    lcd.print("enter hf config ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    this->time_enter = millis();
    this->changed = false;
}

void LCD_state_hf_cfg::update(void)
{
    char line[17];
    input.update();
    lcd.setCursor(0,0);
    lcd.print("freq offset:   ");
    // print on lcd
    lcd.setCursor(0,1);
    snprintf(line,sizeof(line),"%02lu",freq_offset);
    lcd.print(line);

    if (input.is_menu_triggered()) {
        debug("%lu menu button trigger\n", millis);
        new_state = s_menu;
    }

    bool wait = false;
    if (input.is_high(Input::MENU_UP_DOWN)) {
        freq_offset +=1;
        this->changed = true;
        wait = true;
    }
    if (input.is_low(Input::MENU_UP_DOWN)) {
        freq_offset -=1;
        this->changed = true;
        wait = true;
    }
    if(wait)
        delay(150);
}

void LCD_state_hf_cfg::leave(void)
{
    if (this->changed) {
        eeprom_config.set_freq_offset(freq_offset);
        eeprom_config.write();
        eeprom_config.read();
        if (eeprom_config.validate()) {
            debugln("ok calib\n");
        }else {
            debugln("failed calib\n");
        }
        this->changed = false;
    }
    lcd.clear();
}
