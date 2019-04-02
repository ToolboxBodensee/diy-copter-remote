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
    lcd.setCursor(0,0);
    lcd.print("fly mode        ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    this->time_enter = millis();
}

void LCD_state_fly::update(void)
{

}
void LCD_state_fly::leave(void)
{
    lcd.clear();
}
