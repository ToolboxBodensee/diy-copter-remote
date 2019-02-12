#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "Arduino.h"
#include "state.h"
#include "debug.h"
#include "tx_def.h"

LiquidCrystal_I2C lcd(0x27,16,2);

State *curr_state = NULL;
State *new_state = NULL;

State *s_init = new LCD_state_init();
State *s_bind = new LCD_state_bind();
State *s_fly  = new LCD_state_fly();
State *s_menu = new LCD_state_menu();


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
    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();
    lcd.init();

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
    snprintf(this->line[0],sizeof(this->line[0]),"    wellcome    ");
    snprintf(this->line[1],sizeof(this->line[1]),"    phschoen    ");
}
void LCD_state_init::enter(void) {
    lcd.setCursor(0,0);
    lcd.print(this->line[0]);
    lcd.setCursor(0,1);
    lcd.print(this->line[1]);
    this->time_enter = millis();
}
void LCD_state_init::update(void)
{
    uint32_t diff;
    diff = millis() - this->time_enter;
    if (diff > 5 * 1000) {
        new_state = s_bind;
    }
}
void LCD_state_init::leave(void)
{
    lcd.clear();
}
//LCD_state_bind
LCD_state_bind::LCD_state_bind(void) {
    snprintf(this->line[0],sizeof(this->line[0]),"bind mode       ");
    snprintf(this->line[1],sizeof(this->line[1]),"                ");
    this->bind_time = 20;
}
void LCD_state_bind::enter(void) {
    lcd.setCursor(0,0);
    lcd.print(this->line[0]);
    lcd.setCursor(0,1);
    lcd.print(this->line[1]);
    this->time_enter = millis();
}

void LCD_state_bind::update(void)
{
  debugln("blubber\n");
  unsigned long time_in_ms = millis() - this->time_enter;
  unsigned long time_in_s = time_in_ms/1000; // to sec
  unsigned long remain_s = this->bind_time - time_in_s;

  snprintf(this->line[0],sizeof(this->line[1]),"remaining sec %02d",remain_s);
  lcd.setCursor(0,1);
  lcd.print(this->line[0]);
  if (time_in_s >= this->bind_time)
      new_state = s_menu;
}

void LCD_state_bind::leave(void)
{
    lcd.clear();
}

// LCD_state_menu
LCD_state_menu::LCD_state_menu(void) {
    snprintf(this->line[0],sizeof(this->line[0]),"Menubind mode       ");
    snprintf(this->line[1],sizeof(this->line[1]),"                ");
    this->curr_selected = 0;
}
void LCD_state_menu::enter(void) {
    lcd.setCursor(0,0);
    lcd.print(this->line[0]);
    lcd.setCursor(0,1);
    lcd.print(this->line[1]);
    this->time_enter = millis();
}

void LCD_state_menu::update(void)
{

}
void LCD_state_menu::leave(void)
{
    lcd.clear();
}

// LCD_state_fly
LCD_state_fly::LCD_state_fly(void) {
    snprintf(this->line[0],sizeof(this->line[0]),"fly mode       ");
    snprintf(this->line[1],sizeof(this->line[1]),"                ");
}

void LCD_state_fly::enter(void) {
    lcd.setCursor(0,0);
    lcd.print(this->line[0]);
    lcd.setCursor(0,1);
    lcd.print(this->line[1]);
    this->time_enter = millis();
}

void LCD_state_fly::update(void)
{

}
void LCD_state_fly::leave(void)
{
    lcd.clear();
}
