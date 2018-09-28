void update_inputs(void) {
  //analogRead() 
  static int inc = 1;
  #if 1
        if (inc > 0) {
          if (Channel_data[THROTTLE] < CHANNEL_MIN_125 + 100) {
            Channel_data[THROTTLE] += 1;
          }else {
            inc = -1;
          }
        }else {
          if (Channel_data[THROTTLE] > CHANNEL_MIN_125) {
            Channel_data[THROTTLE] -= 1;
          }else {
            inc = +1;
          }
        }
#endif
}

