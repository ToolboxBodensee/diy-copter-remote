#ifndef _INPUT_H_
#define _INPUT_H_
#include <stdint.h>
#include "tx_def.h"

class Input {
    private:
        struct data {
            uint16_t throttle;
            uint16_t yaw;
            uint16_t roll;
            uint16_t pitch;

            bool aux[6];

            bool menu;

        };
        struct data input[2];
        struct data* curr;
        struct data* old;

        struct {
          uint16_t max;
          uint16_t min;
          uint8_t inverted;
        } throttle, yaw, roll, pitch, aux[5];

        bool save_calibration(void);
    public:

        Input(void);
        void init(void);
        void do_calibration(void);
        void update(void);

        struct data* get_curr_input(void);
        struct data* get_old_input(void);
        void update_inputs(void);
        void mark_processed(void);

        bool is_centered(void);
        bool is_centered_left(void);
        bool is_centered_right(void);
        // menu inputs
        bool is_menu_triggered(void);

        bool is_menu_left(void);
        bool is_menu_right(void);
        bool is_menu_down(void);
        bool is_menu_up(void);

        bool is_throttle_up(void);
        bool is_throttle_down(void);
        bool is_yaw_up(void);
        bool is_yaw_down(void);

        bool is_roll_up(void);
        bool is_roll_down(void);
        bool is_pitch_up(void);
        bool is_pitch_down(void);
};

extern uint16_t Channel_data[NUM_CHN];
extern Input input;
#endif
