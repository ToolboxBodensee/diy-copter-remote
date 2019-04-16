#ifndef _INPUT_H_
#define _INPUT_H_
#include <stdint.h>
#include "tx_def.h"

#define NUM_TX_CHN 16

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
            CH_AUX6     = 9,

            CH_MAX      = 9,
            CH_COUNT    = 10,

            MENU_UP_DOWN = CH_PITCH,
            MENU_LEFT_RIGHT = CH_ROLL,
        };
        struct ch_config {
            uint16_t max;
            uint16_t min;
            bool inverted;
            bool is_analog;
        };

        Input(void);
        void init(void);
        void update(void);

        void update_inputs(void);
        void mark_processed(void);

        bool is_centered(void);
        bool is_centered(enum input_channels ch);
        bool is_low(enum input_channels ch);
        bool is_high(enum input_channels ch);
        // menu inputs
        bool is_menu_triggered(void);

        void invert_ch(enum input_channels ch);
        void print_ch(enum input_channels ch);
        void print();

        void calibration_reset(void);
        bool calibration_update(void);

        void set_calibration(struct ch_config *new_config);
        void get_calibration(struct ch_config *curr_config);

        uint16_t *get_channel_data(void);

        uint16_t ch_raw[CH_COUNT];
    private:

        // raw sticks input

        // calculated inputs (my be inverted
        struct data {
            uint16_t ch_data[CH_COUNT];
            bool menu;
        };

        // actual tx channel data
        uint16_t channel_data[NUM_TX_CHN];
        uint16_t failsafe_data[NUM_TX_CHN];

        struct data input[2];
        struct data* curr;
        struct data* old;

        // config of each channel
        struct ch_config ch_config[CH_COUNT];

        // pin setttings
        int pins[CH_COUNT];
};
extern const char* ch_name[Input::CH_COUNT];
extern Input input;

#endif
