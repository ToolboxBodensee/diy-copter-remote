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
    diff = millis() - this->time_enter;
    if (diff > 5 * 1000) {
        new_state = s_joy;
    }
}
void LCD_state_init::leave(void)
{
    lcd.clear();
}
