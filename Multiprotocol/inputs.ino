void update_inputs(void) {
  
  INPUT_SIGNAL_on;
  
  if (state < FRSKY_BIND_DONE)
    return;

  #ifdef Throttle_pin
    uint16_t throttle = analogRead(Throttle_pin);
    uint16_t yaw = analogRead(Yaw_pin);
    uint16_t roll = analogRead(Roll_pin);
    uint16_t pitch = analogRead(Pitch_pin);
    uint16_t aux1 = 0;//analogRead(Aux1_pin);
    uint16_t aux2 = analogRead(Aux2_pin);
    
    uint16_t ch_min=CHANNEL_MIN_100;
    uint16_t ch_max=CHANNEL_MAX_100;

    
    Channel_data[THROTTLE] += map(throttle, 0, 3500, ch_min, ch_max);
    Channel_data[RUDDER] += map(yaw, 0, 3500, ch_min, ch_max);
    Channel_data[AILERON] += map(roll, 0, 3500, ch_min, ch_max);
    Channel_data[ELEVATOR] += map(pitch, 0, 3500, ch_min, ch_max);
    //Channel_data[CH5] += map(aux1, 0, 3500, ch_min, ch_max);
    Channel_data[CH6] += map(aux2, 0, 3500, ch_min, ch_max);

    
    Channel_data[THROTTLE] /= 2;
    Channel_data[RUDDER] /= 2;
    Channel_data[AILERON] /= 2;
    Channel_data[ELEVATOR] /= 2;
    Channel_data[CH5] /= 2;
    Channel_data[CH6] /= 2;
    
    //debugln("T %d y %d r %d p %d a1 %d a2 %d",throttle,yaw,roll,pitch,aux1,aux2);
    //debugln("T %d y %d r %d p %d a1 %d a2 %d",Channel_data[THROTTLE],Channel_data[RUDDER],Channel_data[AILERON],Channel_data[ELEVATOR],Channel_data[CH5],Channel_data[CH6]);
  #else
    //analogRead()
    if (Channel_data[THROTTLE] < CHANNEL_MAX_100) {
      Channel_data[THROTTLE] += 1;
    } else {
      Channel_data[THROTTLE] = CHANNEL_MIN_100;
    }
  #endif
}

