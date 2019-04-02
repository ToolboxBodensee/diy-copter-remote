
#ifndef _STATE_H_
#define _STATE_H_

#include <LiquidCrystal_I2C.h>
#include <stdint.h>


extern LiquidCrystal_I2C lcd;
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

class LCD_state_fly: public State {
private:
    unsigned long time_enter;

public:
    LCD_state_fly(void);
    void enter(void);
    void update(void);
    void leave(void);
};

class LCD_state_menu: public State {
private:
    int8_t curr_selected;
public:
    LCD_state_menu(void);
    void enter(void);
    void update(void);
    void leave(void);
};

class LCD_state_joy_calibration: public State {
public:
    LCD_state_joy_calibration(void);
    void enter(void);
    void update(void);
    void leave(void);
};
extern State *curr_state;
extern State *new_state;

extern State *s_init;
extern State *s_bind;
extern State *s_fly;
extern State *s_joy;
extern State *s_menu;
#endif  /*_STATE_H_*/
