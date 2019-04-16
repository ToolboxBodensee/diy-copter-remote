#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "state.h"
#include "input.h"
#include "debug.h"

#ifndef ENABLE_DBEUG
int16_t map16b( int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);


#include <USBComposite.h>
extern HIDJoystick Joystick;
LCD_state_joy_usb::LCD_state_joy_usb(void) {
}
void LCD_state_joy_usb::enter(void) {
    lcd.setCursor(0,0);
    lcd.print("joystick mode   ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    delay(500);
}
void LCD_state_joy_usb::update(void) {
    input.update();

    uint16_t *ch_data= input.get_channel_data();

    uint16_t x = map16b( ch_data[Input::CH_THROTTLE], CHANNEL_MIN_100, CHANNEL_MAX_100, 0, 1023);
    uint16_t y = map16b( ch_data[Input::CH_YAW], CHANNEL_MIN_100, CHANNEL_MAX_100, 0, 1023);

    uint16_t xr = map16b( ch_data[Input::CH_ROLL], CHANNEL_MIN_100, CHANNEL_MAX_100, 0, 1023);
    uint16_t yr = map16b( ch_data[Input::CH_PITCH], CHANNEL_MIN_100, CHANNEL_MAX_100, 0, 1023);

    bool bt0 =  ch_data[Input::CH_AUX1] == CHANNEL_MIN_100;
    bool bt1 =  ch_data[Input::CH_AUX2] == CHANNEL_MIN_100;
    bool bt2 =  ch_data[Input::CH_AUX3] == CHANNEL_MIN_100;
    bool bt3 =  ch_data[Input::CH_AUX4] == CHANNEL_MIN_100;
    bool bt4 =  ch_data[Input::CH_AUX5] == CHANNEL_MIN_100;
    bool bt5 =  ch_data[Input::CH_AUX6] == CHANNEL_MIN_100;
    delay(50);

    Joystick.X(x);
    Joystick.Y(y);
    Joystick.Xrotate(xr);
    Joystick.Yrotate(yr);
    Joystick.button(0, bt0);
    Joystick.button(1, bt1);
    Joystick.button(2, bt2);
    Joystick.button(3, bt3);
    Joystick.button(4, bt4);
    Joystick.button(5, bt5);

    if (input.is_menu_triggered()) {
        debug("%lu menu button trigger\n", millis);
        new_state = s_menu;
    }
}
void LCD_state_joy_usb::leave(void) {
    lcd.setCursor(0,0);
    lcd.print("finished ");
    lcd.setCursor(0,1);
    lcd.print("usb mode ");
}
#endif
