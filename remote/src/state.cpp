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
State *s_menu = NULL;


enum lcd_special_chars {
    battery_66   = 0,
    battery_33   = 1,
    battery_0    = 2,
    battery_100  = 3,
    rssiantenna  = 4,
    rssi_bars_1  = 5,
    rssi_bars_2  = 6,
    rssi_bars_3  = 7,
    MAX_SPECIAL_CHARS =8,
};
// 6 Byte-Arrays für 6 verschiedene Batteriesymbole
__extension__ struct lcd_special_chars_data { byte data[MAX_SPECIAL_CHARS]; }
   lcd_special_chars_data[MAX_SPECIAL_CHARS] =
{
    //[battery_0]   =
    { 0b01110, 0b11011, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b11111 },
    //[battery_33]  =
    { 0b01110, 0b11011, 0b10001, 0b10001, 0b10001, 0b11111, 0b11111, 0b11111 },
    //[battery_66]  =
    { 0b01110, 0b11011, 0b10001, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 },
    //[battery_100] =
    { 0b01110, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 },
    //[rssiantenna] =
    { 0b10101, 0b10101, 0b01110, 0b00100, 0b00100, 0b00101, 0b00101, 0b00101 },
    //[rssi_bars_1] =
    { 0b00001, 0b00001, 0b00001, 0b00001, 0b00101, 0b00101, 0b10101, 0b10101 },
    //[rssi_bars_2] =
    { 0b00001, 0b00001, 0b00001, 0b00001, 0b10101, 0b10101, 0b10101, 0b10101 },
    //[rssi_bars_3] =
    { 0b00001, 0b00001, 0b00101, 0b00101, 0b10101, 0b10101, 0b10101, 0b10101 },
};


void install_special_caracters(void)
{
    for(int i = 0; i < MAX_SPECIAL_CHARS; i ++) {
        lcd.createChar(i, lcd_special_chars_data[i].data);
    }
}

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
    s_menu = new LCD_state_menu();
    lcd.backlight();
    curr_state = NULL;
    new_state = s_init;

    install_special_caracters();
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

//LCD_state_init

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
//LCD_state_bind
LCD_state_bind::LCD_state_bind(void) {
    this->bind_time = 20;
}
void LCD_state_bind::enter(void) {
    lcd.setCursor(0,0);
    lcd.print("bind mode       ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    this->time_enter = millis();
}

void LCD_state_bind::update(void)
{
  debugln("blubber\n");
  char line[17];
  unsigned long time_in_ms = millis() - this->time_enter;
  unsigned long time_in_s = time_in_ms/1000; // to sec
  unsigned long remain_s = this->bind_time - time_in_s;

  snprintf(line,sizeof(line),"remaining sec %02lu",remain_s);
  lcd.setCursor(0,1);
  lcd.print(line);

    uint32_t end__ = micros();
    uint32_t start = micros();
    uint32_t next_callback_time;
    next_callback_time = initFrSky_2way();
    while(1) {
        start = end__;
        next_callback_time = ReadFrSky_2way();


        // update time
        time_in_ms = millis() - this->time_enter;
        time_in_s = time_in_ms/1000; // to sec
        remain_s = this->bind_time - time_in_s;

        // print on lcd
        snprintf(line,sizeof(line),"%02lu",remain_s);
        lcd.setCursor(14,1);
        lcd.print(line);

        uint32_t wait_until = start + next_callback_time;
        end__ = micros();

        if (end__ - start < next_callback_time) {
            uint32_t wait = next_callback_time;
            wait -= ((end__-start));
            delayMicroseconds(wait);
            end__ += wait;
        }
        end__ = micros();

        if (remain_s == 0)
            break;
    }

    // cange to menu when done
    new_state = s_menu;
}

void LCD_state_bind::leave(void)
{
    lcd.clear();
}


//
LCD_state_joy_calibration::LCD_state_joy_calibration(void) {

}
void LCD_state_joy_calibration::enter(void) {
    lcd.setCursor(0,0);
    lcd.print("calib. start    ");
    lcd.setCursor(0,1);
    lcd.print("move all sticks ");
    delay(500);
}
void show_dots(int row, int number) {
    lcd.setCursor(0,row);
    for(int c = 0; c < 16;++c) {
        if (c < number)
            lcd.print(".");
        else
            lcd.print(" ");
    }
}
void LCD_state_joy_calibration::update(void) {
    int8_t turns = 50;
    int8_t i;

    // init min/max
    input.calibration_reset();

    // min max calibration
    lcd.setCursor(0,0);
    lcd.print("min max Calib.  ");
    show_dots(1,16);
    i = turns;
    while(i > 0) {
        input.update();
        if (true == input.calibration_update()) {
            i = turns;
        }else {
            i -= 1;
            show_dots(1,(i *16)/turns);
        }
        delay(100);
    }

    // center
    lcd.setCursor(0,0);
    lcd.print("center sticks   ");
    i = turns;
    while(i > 0) {
        input.update();
        if (false == input.is_centered()) {
            i = turns;
        }else {
            i -= 1;
            show_dots(1,(i *16)/turns);
        }
        delay(100);
    }

    for (uint8_t _ch = 0; _ch < 4 ; ++_ch) {
        enum Input::input_channels ch = (enum Input::input_channels) _ch;
        lcd.setCursor(0,0);
        lcd.print("move to max:    ");
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print(ch_name[ch]);

        i = turns;
        while(i>0) {
            delay(50);
            input.update();

            input.print_ch(ch);
            if (input.is_high(ch)) {
                i--;
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

    struct ch_config ch_config[CH_COUNT];
    input.get_calibration(ch_config);


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

// #########################################################
// LCD_state_menu
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
        { "Joy calib     ", s_joy },
        { "HF calib      ", NULL },
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

    if (false == input.is_centered(Input::MENU_UP_DOWN)) {
        if (input.is_low(Input::MENU_UP_DOWN)){
            this->curr_selected +=1;
            if ( this->curr_selected > 3)
                this->curr_selected = 3;
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
        delay(500);
}

void LCD_state_menu::leave(void)
{
    lcd.clear();
}

// LCD_state_fly
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
