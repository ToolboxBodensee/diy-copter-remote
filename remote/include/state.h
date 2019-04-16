
#ifndef _STATE_H_
#define _STATE_H_

#include <LiquidCrystal_I2C.h>
#include <stdint.h>

extern LiquidCrystal_I2C lcd;
enum lcd_special_chars {
    battery_char= 0,

    rssiantenna= 3,
    rssi_bars  = 4,
    clock_char = 5,
    MAX_SPECIAL_CHARS =8,
};


void init_state(void);
void update_state(void);

class State {
  protected:

    public:
        virtual void enter(void) {

        }
        virtual void update(void) {

        }
        virtual void leave(void) {

        }

};

extern State *new_state;

class LCD_state_init: public State {
private:
    unsigned long time_enter;
public:
    LCD_state_init(void);
    void enter(void);
    void update(void);
    void leave(void);
};
extern State *s_init;

class LCD_state_bind: public State {
private:
    unsigned long time_enter;
    unsigned long bind_time;

public:
    LCD_state_bind(void);
    void enter(void);
    void update(void);
    void leave(void);
};
extern State *s_bind;

class LCD_state_fly: public State {
private:
    unsigned long time_enter;
    uint16_t last_time;

    void print_akku_quad(uint8_t akku_quad);
    void print_akku_remote(uint8_t akku_remote);
    void print_rssi(uint8_t rssi_percent);
    void print_time(uint16_t time);

public:
    LCD_state_fly(void);
    void enter(void);
    void update(void);
    void leave(void);
};
extern State *s_fly;

class LCD_state_menu: public State {
private:
    int8_t curr_selected;
public:
    LCD_state_menu(void);
    void enter(void);
    void update(void);
    void leave(void);
};
extern State *s_menu;

class LCD_state_joy_calibration: public State {
public:
    LCD_state_joy_calibration(void);
    void enter(void);
    void update(void);
    void leave(void);
};
extern State *s_joy;

#ifndef ENABLE_DBEUG
class LCD_state_joy_usb: public State {
private:
    unsigned long time_enter;
public:
    LCD_state_joy_usb(void);
    void enter(void);
    void update(void);
    void leave(void);
};
extern State *s_usb;
#endif

class LCD_state_hf_cfg: public State {
private:
    unsigned long time_enter;
    bool changed;
public:
    LCD_state_hf_cfg(void);
    void enter(void);
    void update(void);
    void leave(void);
};
extern State *s_hf_cfg;

extern State *curr_state;
extern State *new_state;

void lcd_centerText(const char *text);
#endif  /*_STATE_H_*/
