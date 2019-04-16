#include "Arduino.h"
#include <string.h>
#include "config.h"
#include "tx_def.h"
#include "Multiprotocol.h"
#include "input.h"
#include "pins.h"
#include "state.h"
Input input;


const char* ch_name[Input::CH_COUNT] = {
    "ROLL",
    "PITCH",
    "THROTTLE",
    "YAW",

    "AUX1",
    "AUX2",
    "AUX3",
    "AUX4",
    "AUX5",
};

Input::Input(void) {
    uint8_t i;
    this->curr = &(this->input[0]);
    this->old  = &(this->input[1]);
    memset(this->input,0, sizeof(this->input));

    for (i = 0; i < NUM_TX_CHN; i++) {
        //InitFailsafe
        this->failsafe_data[i] = (CHANNEL_MAX_100 - CHANNEL_MIN_100) / 2 + CHANNEL_MIN_100;
        // init channel
        this->channel_data[i] = 1024;
    }

    this->failsafe_data[CH_THROTTLE] = CHANNEL_MIN_100;  //1=-125%, 204=-100%
    this->channel_data[CH_THROTTLE] = CHANNEL_MIN_100;
}

uint16_t* Input::get_channel_data(void) {
    return this->channel_data;
}
void Input::mark_processed(void) {
    struct data* temp = this->old;
    this->old = this->curr;
    this->curr = temp;
}

void Input::init() {
    this->pins[CH_THROTTLE] = Throttle_pin;
    this->ch_config[CH_THROTTLE].is_analog = true;

    this->pins[CH_YAW] = Yaw_pin;
    this->ch_config[CH_YAW].is_analog = true;

    this->pins[CH_PITCH] = Pitch_pin;
    this->ch_config[CH_PITCH].is_analog = true;

    this->pins[CH_ROLL] = Roll_pin;
    this->ch_config[CH_ROLL].is_analog = true;

    this->pins[CH_AUX1] = Aux1_pin;
    this->ch_config[CH_AUX1].is_analog = false;

    this->pins[CH_AUX2] = Aux2_pin;
    this->ch_config[CH_AUX2].is_analog = false;

    this->pins[CH_AUX3] = Aux3_pin;
    this->ch_config[CH_AUX3].is_analog = false;

    this->pins[CH_AUX4] = Aux4_pin;
    this->ch_config[CH_AUX4].is_analog = false;

    this->pins[CH_AUX5] = Aux5_pin;
    this->ch_config[CH_AUX5].is_analog = false;

    this->pins[CH_AUX6] = Aux6_pin;
    this->ch_config[CH_AUX6].is_analog = false;

    for (uint8_t i = 0; i < CH_COUNT; ++i) {
        pinMode(this->pins[i], INPUT);
    }
    pinMode(Menu_pin,INPUT);

    //analogReadResolution(16);

    // move this to eeprom later
    this->ch_config[CH_THROTTLE].inverted = false;
    this->ch_config[CH_YAW].inverted      = false;
    this->ch_config[CH_ROLL].inverted     = false;
    this->ch_config[CH_PITCH].inverted    = false;
    this->ch_config[CH_AUX1].inverted     = true;
    this->ch_config[CH_AUX2].inverted     = true;
    this->ch_config[CH_AUX3].inverted     = true;
    this->ch_config[CH_AUX4].inverted     = true;
    this->ch_config[CH_AUX5].inverted     = true;
    this->ch_config[CH_AUX6].inverted     = true;
}

bool Input::is_centered(void) {
    return  this->is_centered(CH_ROLL) &&
            this->is_centered(CH_PITCH) &&
            this->is_centered(CH_THROTTLE) &&
            this->is_centered(CH_YAW);
}

bool Input::is_centered(enum Input::input_channels ch) {
    uint16_t range = this->ch_config[ch].max - this->ch_config[ch].min;
    uint16_t delta = range / 5;

    if ( this->curr->ch_data[ch] < this->ch_config[ch].min + range / 2 - delta ||
       this->curr->ch_data[ch] > this->ch_config[ch].min + range / 2 + delta
    ) {
        // pitch is not centered
        return false;
    }
    return true;
}

bool Input::is_high(enum Input::input_channels ch) {
    uint16_t range = this->ch_config[ch].max - this->ch_config[ch].min;
    uint16_t delta = range / 5;
    if ( this->curr->ch_data[ch] > this->ch_config[ch].max - delta) {
        return true;
    }
    return false;

}
bool Input::is_low(enum Input::input_channels ch) {
    uint16_t range = this->ch_config[ch].max - this->ch_config[ch].min;
    uint16_t delta = range / 5;
    if ( this->curr->ch_data[ch] < this->ch_config[ch].min + delta) {
        return true;
    }
    return false;
}

bool Input::is_menu_triggered(void) {
    return this->curr->menu != this->old->menu;
}

void Input::invert_ch(enum Input::input_channels ch) {
    this->ch_config[ch].inverted = !this->ch_config[ch].inverted;
}
void Input::print_ch(enum Input::input_channels ch) {
    debug("ch%d: %04d %04d %04d min %d max %d high %d mid %d low %d\n",
            ch,
            this->ch_raw[ch],
            this->curr->ch_data[ch],
            this->old->ch_data[ch],
            this->ch_config[ch].min,
            this->ch_config[ch].max,
            this->is_high((enum Input::input_channels)ch),
            this->is_centered((enum Input::input_channels)ch),
            this->is_low((enum Input::input_channels)ch)
            );
}
void Input::print() {
    debug("menu %d (old %d)\n", this->curr->menu, this->old->menu);

    for (uint8_t i = 0; i < CH_COUNT; ++i) {
        print_ch((enum Input::input_channels) i);
    }
    for (uint8_t i = 0; i < NUM_TX_CHN; ++i) {
        debug("chdata %d \n", this->channel_data[i]);
    }
}


bool Input::calibration_update(void) {
    bool changed = false;

    for (uint8_t ch = 0; ch < CH_COUNT; ch++) {
        if (this->ch_config[ch].min > this->ch_raw[ch]) {
            this->ch_config[ch].min = this->ch_raw[ch];
            changed = true;
        } else if (this->ch_config[ch].max < this->ch_raw[ch]) {
            changed = true;
            this->ch_config[ch].max = this->ch_raw[ch];
        }
    }

    if (changed) {
        debugln("new calib values t %d-%d r %d-%d p %d-%d y %d-%d",
            this->ch_config[CH_THROTTLE].min, this->ch_config[CH_THROTTLE].max,
            this->ch_config[CH_ROLL].min,    this->ch_config[CH_ROLL].max,
            this->ch_config[CH_PITCH].min,   this->ch_config[CH_PITCH].max,
            this->ch_config[CH_YAW].min,     this->ch_config[CH_YAW].max);
        // TODO save in eeprom
    }

    return changed;
}
void Input::calibration_reset(void) {
    for (uint8_t ch = 0; ch < CH_COUNT; ch++) {
        if (this->ch_config[ch].is_analog) {
            this->ch_config[ch].min = this->ch_raw[ch];
            this->ch_config[ch].max = this->ch_raw[ch];
        }else {
            this->ch_config[ch].min = CHANNEL_MIN_100;
            this->ch_config[ch].max = CHANNEL_MAX_100;
        }
    }
}
void Input::set_calibration(struct ch_config *new_config)
{
    if (new_config != NULL) {
        memcpy(this->ch_config, new_config, sizeof(ch_config));
    }
}
void Input::get_calibration(struct ch_config *curr_config)
{
    if (curr_config != NULL) {
        memcpy(curr_config, this->ch_config, sizeof(ch_config));
    }
}

void Input::update(void) {
    this->mark_processed();
    for (uint8_t ch = 0; ch < CH_MAX; ch ++) {

        if (this->ch_config[ch].is_analog) {
            this->ch_raw[ch] = analogRead(this->pins[ch]);
        } else {
            this->ch_raw[ch] = digitalRead(this->pins[ch]) == HIGH ? CHANNEL_MIN_100 : CHANNEL_MAX_100;
        }

        // do inverting
        if (this->ch_config[ch].inverted)
            this->curr->ch_data[ch] = this->ch_config[ch].max - this->ch_raw[ch];
        else
            this->curr->ch_data[ch] = this->ch_raw[ch];

        // cap on max
        if (this->ch_config[ch].min > this->curr->ch_data[ch]) {
            this->curr->ch_data[ch] = this->ch_config[ch].min;
        } else if (this->ch_config[ch].max < this->curr->ch_data[ch]) {
            this->curr->ch_data[ch] = this->ch_config[ch].max;
        }
    }
    this->curr->menu = digitalRead(Menu_pin) == HIGH;


    for (uint8_t ch = 0; ch < CH_COUNT; ++ch) {
        this->channel_data[ch] = map(this->curr->ch_data[ch], this->ch_config[ch].min, this->ch_config[ch].max, CHANNEL_MIN_100, CHANNEL_MAX_100);
    }

    #define CHANNEL_DEAD 70
    uint8_t mid = (CHANNEL_MAX_100 - CHANNEL_MIN_100)/2 + CHANNEL_MIN_100;
    uint8_t min_dead = mid - CHANNEL_DEAD;
    uint8_t max_dead = mid + CHANNEL_DEAD;
    for (uint8_t ch = 0; ch < 4; ++ch) {
        if (min_dead < this->channel_data[ch] &&
            this->channel_data[ch] < max_dead) {
            // dead range
            this->channel_data[ch] = mid;
        }
        /* else { */
        /*     if (this->channel_data[ch] > mid) { */
        /*         this->channel_data[ch] -= CHANNEL_DEAD; */
        /*     } else { */
        /*         this->channel_data[ch] += CHANNEL_DEAD; */
        /*     } */
        /*     this->channel_data[ch] = map(this->curr->ch_data[ch], CHANNEL_MIN_100+CHANNEL_DEAD, CHANNEL_MAX_100-CHANNEL_DEAD, CHANNEL_MIN_100, CHANNEL_MAX_100); */
        /* } */
    }

    /*debug_input("t%d y%d r%d p%d a1_%d a2_%d a3_%d a4_%d a5_%d m%d",
                this->curr->throttle,this->curr->yaw,this->curr->roll,this->curr->pitch,
                this->curr->aux[0],this->curr->aux[1],this->curr->aux[2],this->curr->aux[3],
                this->curr->aux[4],this->curr->aux[5],this->curr->menu
                );*/
}
