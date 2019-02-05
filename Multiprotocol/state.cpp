#include <LiquidCrystal_I2C.h>
#include "state.h"
#include "Arduino.h"
#include "debug.h"

LiquidCrystal_I2C lcd(0x27,16,2);

State *curr_state = NULL;
State *new_state = NULL;

State *s_init = new LCD_state_init();
State *s_bind = new LCD_state_bind();


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
    snprintf(this->line1,sizeof(this->line2),"    wellcome    ");
    snprintf(this->line2,sizeof(this->line2),"    phschoen    ");
}
void LCD_state_init::enter(void) {
    lcd.setCursor(0,0);
    lcd.print(this->line1);
    lcd.setCursor(0,1);
    lcd.print(this->line2);
    time_enter = millis();
}
void LCD_state_init::update(void)
{
    uint32_t diff;
    diff = millis()-time_enter;
    if (diff > 5 * 1000) {
        new_state = s_bind;
    }
}
void LCD_state_init::leave(void)
{

}
//LCD_state_bind
LCD_state_bind::LCD_state_bind(void) {
    snprintf(this->line1,sizeof(this->line2),"bind mode       ");
    snprintf(this->line2,sizeof(this->line2),"                ");
    this->bind_time = 20;
}
void LCD_state_bind::enter(void) {
    lcd.setCursor(0,0);
    lcd.print(this->line1);
    lcd.setCursor(0,1);
    lcd.print(this->line2);
    this->time_enter = millis();
}

void LCD_state_bind::update(void)
{
  debugln("blubber\n");
  unsigned long remain = millis() - this->time_enter;
  remain = remain/1000; // to sec
  remain = this->bind_time - remain;
  snprintf(this->line2,sizeof(this->line2),"remaining sec %02d",remain);
  lcd.setCursor(0,1);
  lcd.print(this->line2);
  time_enter = millis();
}
void LCD_state_bind::leave(void)
{

}
