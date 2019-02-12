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

        // menu inputs
        bool menu_triggered(void);

        bool left_triggered(void);
        bool right_triggered(void);
        bool down_triggered(void);
        bool up_triggered(void);
};

extern uint16_t Channel_data[NUM_CHN];
extern Input input;
#endif
