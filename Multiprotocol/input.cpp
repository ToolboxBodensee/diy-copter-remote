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
  int8_t i;

  // min max calibration
  i = turns;
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

  // center
  debugln("now center all sticks");
  i = turns;
  while(i > 0) {
    delay(100);
    this->update();
    if (false == this->is_centered()) {
      i = turns;
    }else {
      i -= 1;
    }

  }

  debugln("now Move throttle to max");
  i = turns;
  while(i>0) {
    delay(100);
    this->update();

    if (true == this->is_throttle_up()) {
        i--;
        continue;
    }
    if (true == this->is_throttle_down()) {
        this->throttle.inverted = ! this->throttle.inverted;
        i = turns;
        continue;
    }
  }
  debugln("throttle invert is %d", this->throttle.inverted);


  debugln("now Move yaw to max");
  i = turns;
  while(i>0) {
    delay(100);
    this->update();

    if (true == this->is_yaw_up()) {
        i--;
        continue;
    }
    if (true == this->is_yaw_down()) {
        this->yaw.inverted = ! this->yaw.inverted;
        i = turns;
        continue;
    }
  }
  debugln("yaw invert is %d", this->yaw.inverted);

  debugln("now Move pitch to max");
  i = turns;
  while(i>0) {
    delay(100);
    this->update();

    if (true == this->is_pitch_up()) {
        i--;
        continue;
    }
    if (true == this->is_pitch_down()) {
        this->pitch.inverted = ! this->pitch.inverted;
        i = turns;
        continue;
    }
  }
  debugln("pitch invert is %d", this->pitch.inverted);

  debugln("now Move roll to max");
  i = turns;
  while(i>0) {
    delay(100);
    this->update();

    if (true == this->is_roll_up()) {
        i--;
        continue;
    }
    if (true == this->is_roll_down()) {
        this->roll.inverted = ! this->roll.inverted;
        i = turns;
        continue;
    }
  }
  debugln("roll invert is %d", this->roll.inverted);

}
bool Input::is_centered(void) {
    return this->is_centered_left() && this->is_centered_right();
}

bool Input::is_centered_left(void) {
    uint16_t range;
    uint16_t delta;

    range = this->throttle.max - this->throttle.min;
    delta = range >>4;
    if ( this->curr->throttle < this->throttle.min + range/2 - delta ||
         this->curr->throttle > this->throttle.min + range/2 + delta
    ) {
        // throttle is not centered
        return false;
    }


    range = this->yaw.max - this->yaw.min;
    delta = range >>4;
    if ( this->curr->yaw < this->yaw.min + range/2 - delta ||
         this->curr->yaw > this->yaw.min + range/2 + delta
    ) {
        // yaw is not centered
        return false;
    }
    return true;
}
bool Input::is_centered_right(void) {
    uint16_t range;
    uint16_t delta;

    range = this->pitch.max - this->pitch.min;
    delta = range >>4;
    if ( this->curr->pitch < this->pitch.min + range/2 - delta ||
         this->curr->pitch > this->pitch.min + range/2 + delta
    ) {
        // pitch is not centered
        return false;
    }

    range = this->roll.max - this->roll.min;
    delta = range >>4;
    if ( this->curr->roll < this->roll.min + range/2 - delta ||
         this->curr->roll > this->roll.min + range/2 + delta
    ) {
        // roll is not centered
        return false;
    }
    return true;
}

bool Input::is_menu_left(void) {
    return this->is_roll_down();
}
bool Input::is_menu_right(void) {
    return this->is_roll_up();
}
bool Input::is_menu_down(void) {
    return this->is_pitch_down();
}
bool Input::is_menu_up(void) {
    return this->is_pitch_up();
}
bool Input::is_throttle_down(void) {
    uint16_t delta = (this->throttle.max - this->throttle.min)/3;
    if ( this->curr->throttle < this->throttle.min + delta) {
        return true;
    }
    return false;

}
bool Input::is_throttle_up(void) {
    uint16_t delta = (this->throttle.max - this->throttle.min)/3;
    if ( this->curr->throttle > this->throttle.max - delta) {
        return true;
    }
    return false;
}
bool Input::is_yaw_up(void) {
    uint16_t delta = (this->yaw.max - this->yaw.min)/3;
    if ( this->curr->yaw > this->yaw.max - delta) {
        return true;
    }
    return false;
}
bool Input::is_yaw_down(void) {
    uint16_t delta = (this->yaw.max - this->yaw.min)/3;
    if ( this->curr->yaw > this->yaw.min + delta) {
        return true;
    }
    return false;
}

bool Input::is_roll_down(void) {
    uint16_t delta = (this->roll.max - this->roll.min)/3;
    if ( this->curr->roll < this->roll.min + delta) {
        return true;
    }
    return false;

}
bool Input::is_roll_up(void) {
    uint16_t delta = (this->roll.max - this->roll.min)/3;
    if ( this->curr->roll > this->roll.max - delta) {
        return true;
    }
    return false;
}
bool Input::is_pitch_up(void) {
    uint16_t delta = (this->pitch.max - this->pitch.min)/3;
    if ( this->curr->pitch > this->pitch.max - delta) {
        return true;
    }
    return false;
}
bool Input::is_pitch_down(void) {
    uint16_t delta = (this->pitch.max - this->pitch.min)/3;
    if ( this->curr->pitch > this->pitch.min + delta) {
        return true;
    }
    return false;
}

bool Input::is_menu_triggered(void) {
    return this->curr->menu;
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
