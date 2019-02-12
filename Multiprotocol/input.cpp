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
void Input::init() {
  analogReadResolution(16);
  
  pinMode(Throttle_pin,INPUT);
  pinMode(Yaw_pin,INPUT);
  pinMode(Roll_pin,INPUT);
  pinMode(Aux1_pin,INPUT);
  pinMode(Aux2_pin,INPUT);
  pinMode(Aux3_pin,INPUT);
  pinMode(Aux4_pin,INPUT);
  pinMode(Aux5_pin,INPUT);
  pinMode(Aux6_pin,INPUT);
  pinMode(Menu_pin,INPUT);

  this->throttle.inverted = true;
  this->yaw.inverted      = false;
  this->roll.inverted     = false;
  this->pitch.inverted    = false;
  this->aux[0].inverted   = false;
  this->aux[1].inverted   = false;
  this->aux[2].inverted   = false;
  this->aux[3].inverted   = false;
  this->aux[4].inverted   = false;
  this->aux[5].inverted   = false;

  
  this->throttle.min  = 500;
  this->yaw.min       = 500;
  this->roll.min      = 500;
  this->pitch.min     = 500;
  this->throttle.max  = 500;
  this->yaw.max       = 500;
  this->roll.max      = 500;
  this->pitch.max     = 500;
}
void Input::do_calibration(void) { 
  int8_t turns = 50; 
  int8_t i = turns;
  
  while(i > 0) {
    this->update();
    if (true == this->save_calibration()) {
      i = turns;
      debugln("new values t %d-%d r %d-%d p %d-%d y %d-%d",
              this->throttle.min,this->throttle.max,
              this->roll.min,    this->roll.max,
              this->pitch.min,   this->pitch.max,
              this->yaw.min,     this->yaw.max);
    }else {
      i -= 1;
    }
            
    delay(100);
  }
  debugln("end values t %d-%d r %d-%d p %d-%d y %d-%d",
          this->throttle.min,this->throttle.max,
          this->roll.min,    this->roll.max,
          this->pitch.min,   this->pitch.max,
          this->yaw.min,     this->yaw.max);

}
  
bool Input::save_calibration(void) {
  bool changed = false;
  if (this->throttle.min > this->curr->throttle){
    changed = true;
    this->throttle.min = this->curr->throttle;
  } else if (this->throttle.max < this->curr->throttle) {
    changed = true;
    this->throttle.max = this->curr->throttle;
  }

  if (this->yaw.min > this->curr->yaw){
    changed = true;
    this->yaw.min = this->curr->yaw;
  } else if (this->yaw.max < this->curr->yaw) {
    changed = true;
    this->yaw.max = this->curr->yaw;
  }

  
  if (this->roll.min > this->curr->roll){
    changed = true;
    this->roll.min = this->curr->roll;
  } else if (this->roll.max < this->curr->roll) {
    changed = true;
    this->roll.max = this->curr->roll;
  }
  
  if (this->pitch.min > this->curr->pitch){
    changed = true;
    this->pitch.min = this->curr->pitch;
  } else if (this->roll.max < this->curr->pitch) {
    changed = true;
    this->pitch.max = this->curr->pitch;
  }
  return changed;
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

    
    /*debug_input("t%d y%d r%d p%d a1_%d a2_%d a3_%d a4_%d a5_%d m%d",
                this->curr->throttle,this->curr->yaw,this->curr->roll,this->curr->pitch,
                this->curr->aux[0],this->curr->aux[1],this->curr->aux[2],this->curr->aux[3],
                this->curr->aux[4],this->curr->aux[5],this->curr->menu
                );*/

    // only do channeloutpu if needed
    if (curr_state != s_fly)
        return;

    if(this->throttle.inverted)
      Channel_data[THROTTLE] = map(this->curr->throttle, this->throttle.min, this->throttle.max, CHANNEL_MAX_100, CHANNEL_MIN_100);
    else
      Channel_data[THROTTLE] = map(this->curr->throttle, this->throttle.min, this->throttle.max, CHANNEL_MIN_100, CHANNEL_MAX_100);

      
    if(this->yaw.inverted)
      Channel_data[RUDDER] = map(this->curr->yaw, this->yaw.min, this->yaw.max, CHANNEL_MAX_100, CHANNEL_MIN_100);
    else
      Channel_data[RUDDER] = map(this->curr->yaw, this->yaw.min, this->yaw.max, CHANNEL_MIN_100, CHANNEL_MAX_100);
      
    if(this->roll.inverted)
      Channel_data[AILERON] = map(this->curr->roll, this->roll.min, this->roll.max, CHANNEL_MAX_100, CHANNEL_MIN_100);
    else
      Channel_data[AILERON] = map(this->curr->roll, this->roll.min, this->roll.max, CHANNEL_MIN_100, CHANNEL_MAX_100);

      
    if(this->pitch.inverted)
      Channel_data[ELEVATOR] = map(this->curr->pitch, this->pitch.min, this->pitch.max, CHANNEL_MAX_100, CHANNEL_MIN_100);
    else
      Channel_data[ELEVATOR] = map(this->curr->pitch, this->pitch.min, this->pitch.max, CHANNEL_MIN_100, CHANNEL_MAX_100);

    for (uint8_t i = 0; i<6; ++i) {
      if(this->aux[i].inverted)
        Channel_data[CH5+i] = (this->curr->aux[i]) ? CHANNEL_MIN_100 : CHANNEL_MAX_100;
      else
        Channel_data[CH5+i] = (this->curr->aux[i]) ? CHANNEL_MAX_100 : CHANNEL_MIN_100;
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
