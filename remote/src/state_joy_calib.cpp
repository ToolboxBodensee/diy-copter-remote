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

void lcd_show_dots(int row, int number) {
    char line[17] = "";
    int c = 0;
    lcd.setCursor(0,row);
    for(c = 0; c < 16;++c) {
        if (c < number)
            line[c]='.';
    }
    line[c]='\0';
    lcd_centerText(line);
}

void LCD_state_joy_calibration::update(void) {
    int8_t turns = 50;
    int8_t i;

    // init min/max
    input.calibration_reset();

    // min max calibration
    lcd.setCursor(0,0);
    lcd.print("min max Calib.  ");
    lcd_show_dots(1,16);
    i = turns;
    while(i > 0) {
        input.update();
        if (true == input.calibration_update()) {
            i = turns;
        }else {
            i -= 1;
            lcd_show_dots(1,(i *16)/turns);
        }
        delay(100);
    }

    // center
    lcd.setCursor(0,0);
    lcd.print("center stick     ");
    i = turns;
    lcd_show_dots(1,16);
    while(i > 0) {
        input.update();
        if (false == input.is_centered()) {
            i = turns;
        }else {
            i -= 1;
            lcd_show_dots(1,(i *16)/turns);
        }
        lcd.setCursor(12,0);
        if (input.is_centered(Input::CH_THROTTLE)) {
            lcd.write(' ');
        } else {
            lcd.write('T');
        }

        lcd.setCursor(13,0);
        if (input.is_centered(Input::CH_YAW)) {
            lcd.write(' ');
        } else {
            lcd.write('Y');
        }

        lcd.setCursor(14,0);
        if (input.is_centered(Input::CH_PITCH)) {
            lcd.write(' ');
        } else {
            lcd.write('P');
        }

        lcd.setCursor(15,0);
        if (input.is_centered(Input::CH_ROLL)) {
            lcd.write(' ');
        } else {
            lcd.write('R');
        }



        delay(100);
    }

    for (uint8_t _ch = 0; _ch < 4 ; ++_ch) {
        enum Input::input_channels ch = (enum Input::input_channels) _ch;
        char line [17]="";
        sprintf(line, "move max %s", ch_name[ch]);
        lcd.setCursor(0,0);
        lcd.print(line);

        i = turns;
        lcd_show_dots(1,16);
        while(i>0) {
            delay(50);
            input.update();

            input.print_ch(ch);
            if (input.is_high(ch)) {
                i -= 1;
                lcd.setCursor(0,1);
                lcd_show_dots(1,(i *16)/turns);
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
    eeprom_config.set_ch_config(ch_config);
    uint32_t id=0;
    #ifdef FORCE_GLOBAL_ID
        id = FORCE_GLOBAL_ID;
    #else
        // Generate a random ID from UUID
        #define STM32_UUID ((uint32_t *)0x1FFFF7E8)
        id = STM32_UUID[0] ^ STM32_UUID[1] ^ STM32_UUID[2];
    #endif
    debugln("Generated new master ID %lx", id);
    eeprom_config.set_master_id(id);
    eeprom_config.write();

    eeprom_config.read();
    if (eeprom_config.validate()) {
        debugln("ok calib\n");
    }else {
        debugln("failed calib\n");
    }


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
