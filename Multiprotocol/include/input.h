#ifndef _INPUT_H_
#define _INPUT_H_
#include <stdint.h>
#include "tx_def.h"

#define NUM_TX_CHN 16

extern uint16_t Channel_data[NUM_TX_CHN];
extern uint16_t Failsafe_data[NUM_TX_CHN];
class Input {
    public:
        enum input_channels {
            CH_ROLL     = 0,
            CH_PITCH    = 1,
            CH_THROTTLE = 2,
            CH_YAW      = 3,

            CH_AUX1     = 4,
            CH_AUX2     = 5,
            CH_AUX3     = 6,
            CH_AUX4     = 7,
            CH_AUX5     = 8,
            CH_AUX6     = 8,

            CH_MAX      = 8,
            CH_COUNT    = 9,

            MENU_UP_DOWN = CH_PITCH,
            MENU_LEFT_RIGHT = CH_ROLL,
        };

        struct data {
            uint16_t ch_data[CH_COUNT];
            bool menu;
        };


        Input(void);
        void init(void);
        void do_calibration(void);
        void update(void);

        struct data* get_curr_input(void);
        struct data* get_old_input(void);
        void update_inputs(void);
        void mark_processed(void);

        bool is_centered(void);
        bool is_centered(enum input_channels ch);
        bool is_low(enum input_channels ch);
        bool is_high(enum input_channels ch);
        // menu inputs
        bool is_menu_triggered(void);

    private:
        struct data input[2];
        struct data* curr;
        struct data* old;

        uint16_t ch_raw[CH_COUNT];

        struct {
          uint16_t max;
          uint16_t min;
          bool inverted;
          bool is_analog;
        } ch_config[CH_COUNT];

        uint32_t pins[CH_COUNT];

        bool calibration_update(void);
};

extern uint16_t Channel_data[NUM_TX_CHN];
extern Input input;
#endif
