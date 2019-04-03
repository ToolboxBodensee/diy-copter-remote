#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "FrSkyD_cc2500.h"
#include "state.h"
#include "input.h"
#include "eeprom.h"
#include "debug.h"
#include "tx_def.h"

LCD_state_init::LCD_state_init(void) {
}
void LCD_state_init::enter(void) {
    lcd.setCursor(0,0);
    lcd.print("    wellcome    ");
    lcd.setCursor(0,1);
    lcd.print("    phschoen    ");
    this->time_enter = millis();
}
void LCD_state_init::update(void)
{
    uint32_t diff;
    delay(100);
    diff = millis() - this->time_enter;
    if (diff > 5 * 1000) {
        new_state = s_joy;
        debugln("read start");
        delay(1000);
        if ( 0  > eeprom_config.read()) {
            debugln("failed to read");
            return;
        }
        debugln("read fin");

        debugln("val start");
        if ( 0  > eeprom_config.validate()) {
            debugln("failed to validate");
            return;
        }
        debugln("val fin");

        new_state = s_menu;

        struct Input::ch_config ch_config[Input::CH_COUNT];
        eeprom_config.get_ch_config(ch_config);
        input.set_calibration(ch_config);
    }
}
void LCD_state_init::leave(void)
{
    lcd.clear();
}
