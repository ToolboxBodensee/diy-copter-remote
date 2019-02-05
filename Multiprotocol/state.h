
#ifndef _STATE_H_
#define _STATE_H_

#include <LiquidCrystal_I2C.h>


void init_state(void);
void update_state(void);

class State {
  protected:
  
    char line1[17];
    char line2[17];
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

class LCD_state_flight: public State {
private:
    unsigned long time_enter;

public:
    void enter(void);
    void update(void);
    void leave(void);
};
#endif  /*_STATE_H_*/
