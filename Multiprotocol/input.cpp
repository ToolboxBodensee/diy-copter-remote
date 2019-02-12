#include "Arduino.h"
#include <string.h>
#include "config.h"
#include "tx_def.h"
#include "Multiprotocol.h"
#include "input.h"
#include "pins.h"
#include "state.h"
Input input;

Input::Input(void) {
    this->curr = &(this->input[0]);
    this->old  = &(this->input[1]);
    memset(this->input,0, sizeof(this->input));
}
void Input::mark_processed(void) {
    struct data* temp = this->old;
    this->old = this->curr;
    this->curr = temp;
}

struct Input::data* Input::get_curr_input(void) {
    return this->curr;
}

struct Input::data* Input::get_old_input(void) {
    return this->old;
}

void Input::update(void) {
    this->curr->throttle = analogRead(Throttle_pin);
    this->curr->yaw = analogRead(Yaw_pin);
    this->curr->roll = analogRead(Roll_pin);
    this->curr->pitch = analogRead(Pitch_pin);

    this->curr->aux[0] = digitalRead(Aux1_pin);
    this->curr->aux[1] = digitalRead(Aux2_pin);
    this->curr->aux[2] = digitalRead(Aux3_pin);
    this->curr->aux[3] = digitalRead(Aux4_pin);
    this->curr->aux[4] = digitalRead(Aux5_pin);
    this->curr->aux[5] = digitalRead(Aux6_pin);

    this->curr->menu = digitalRead(Menu_pin);

    // only do channeloutpu if needed
    if (curr_state != s_fly)
        return;

    Channel_data[THROTTLE] = map(this->curr->throttle, 0, 3500, CHANNEL_MIN_100, CHANNEL_MAX_100);
    Channel_data[RUDDER]   = map(this->curr->yaw,      0, 3500, CHANNEL_MIN_100, CHANNEL_MAX_100);
    Channel_data[AILERON]  = map(this->curr->roll,     0, 3500, CHANNEL_MIN_100, CHANNEL_MAX_100);
    Channel_data[ELEVATOR] = map(this->curr->pitch,    0, 3500, CHANNEL_MIN_100, CHANNEL_MAX_100);

    for (uint8_t i = 0; i<6; ++i) {
        if(this->curr->aux[i])
            Channel_data[CH5+i] = CHANNEL_MAX_100;
        else
            Channel_data[CH5+i] = CHANNEL_MIN_100;
    }
}

bool Input::menu_triggered(void) {
    return false;
}

bool Input::left_triggered(void) {
    return false;
}
bool Input::right_triggered(void) {
    return false;
}
bool Input::down_triggered(void) {
    return false;
}
bool Input::up_triggered(void) {
    return false;
}
