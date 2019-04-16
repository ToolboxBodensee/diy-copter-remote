#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "FrSkyD_cc2500.h"
#include "state.h"
#include "input.h"
#include "eeprom.h"
#include "debug.h"
#include "tx_def.h"

LCD_state_menu::LCD_state_menu(void) {
}
void LCD_state_menu::enter(void) {
    lcd.setCursor(0,0);
    lcd.print("menu mode       ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    this->curr_selected = 0;
}

void LCD_state_menu::update(void)
{
    struct {
        char name[15];
        State * state;
    } menus[] = {
        { "Flight        ", s_fly },
        { "Bind          ", s_bind },
#ifdef ENABLE_DBEUG
        { "Joy usb       ", s_usb},
#endif
        { "Joy calib     ", s_joy },
        { "HF config     ", s_hf_cfg },
        { "              ", NULL },
    };

    bool wait = false;
    char curr[2][16];
    snprintf(curr[0], sizeof(curr[0]), "> %s", menus[this->curr_selected].name);
    snprintf(curr[1], sizeof(curr[1]), "  %s", menus[this->curr_selected+1].name);

    lcd.setCursor(0,0);
    lcd.print(curr[0]);
    lcd.setCursor(0,1);
    lcd.print(curr[1]);

    input.update();
    if (false == input.is_centered(Input::MENU_UP_DOWN)) {
        if (input.is_low(Input::MENU_UP_DOWN)){
            this->curr_selected +=1;
            if ( this->curr_selected > 4)
                this->curr_selected = 4;
            else
                wait = true;
        }
        if (input.is_high(Input::MENU_UP_DOWN)){
            this->curr_selected -=1;
            if ( this->curr_selected < 0)
                this->curr_selected = 0;
            else
                wait = true;
        }
    }

    if (input.is_high(Input::MENU_LEFT_RIGHT)) {
        lcd.setCursor(0,0);
        lcd.print("entering         ");
        lcd.setCursor(0,1);
        lcd.print(menus[this->curr_selected].name);

        // do wait until its centered
        while(false == input.is_centered(Input::MENU_LEFT_RIGHT)) {
            input.update();
        }

        if (menus[this->curr_selected].state != NULL) {
            new_state = menus[this->curr_selected].state;
        }
        wait = true;
    }

    if(wait)
        delay(1100);
}

void LCD_state_menu::leave(void)
{
    lcd.clear();
}
