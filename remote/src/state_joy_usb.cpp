#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "state.h"
#include "input.h"
#include "debug.h"

//#include <USBComposite.h>

/* USBHID* HID; */
/* HIDJoystick* Joystick; */

LCD_state_joy_usb::LCD_state_joy_usb(void) {

}
void LCD_state_joy_usb::enter(void) {
    lcd.setCursor(0,0);
    lcd.print("joystick mode   ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    delay(500);

    /* HID = new USBHID(); */
    /* Joystick = new HIDJoystick(*HID); */
    /* HID->begin(HID_JOYSTICK); */
}
void LCD_state_joy_usb::update(void) {

    input.update();
    input.print();
    while(1) {
        /* Joystick->X(0); */
        /* delay(500); */
        /* Joystick->X(1023); */
        /* delay(500); */

        input.update();

        if (input.is_menu_triggered()) {
            debug("%lu menu button trigger\n", millis());
            input.print();
            break;
        }
    }
    new_state = s_menu;
}
void LCD_state_joy_usb::leave(void) {
    lcd.setCursor(0,0);
    lcd.print("finished ");
    lcd.setCursor(0,1);
    lcd.print("usb mode ");
}
