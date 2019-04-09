#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "FrSkyD_cc2500.h"
#include "state.h"
#include "input.h"
#include "eeprom.h"
#include "debug.h"
#include "config.h"
#include "pins.h"
#include "tx_def.h"
void centerText(char *text, int fieldWidth)
{
    int padlen = 0;
    padlen = (fieldWidth - strlen(text)) / 2 ;
    char line[17];
    sprintf(line, "%*s%s%*s\n", padlen, "", text, padlen, "");
    lcd.print(line);
}

LCD_state_init::LCD_state_init(void) {
}
void LCD_state_init::enter(void) {
    lcd.setCursor(0,0);
    lcd.print("    wellcome    ");
    lcd.setCursor(0,1);
    centerText(REMOE_OWNER,16);

    this->time_enter = millis();
}

void LCD_state_init::update(void)
{
    uint32_t diff;
    delay(100);
    diff = millis() - this->time_enter;
    if (diff > 3 * 1000) {
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

        uint32_t master_id = 0;
        eeprom_config.get_master_id(&master_id);
        set_rx_tx_addr(master_id);
    }
}

void LCD_state_init::leave(void)
{
    lcd.clear();
}
