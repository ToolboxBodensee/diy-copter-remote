#include <Arduino.h>

#ifndef FRSKYD_H
#define FRSKYD_H
#include <cc2500.h>

 #define BIND_IN_PROGRESS       protocol_flags &= ~_BV(7)
 #define BIND_DONE              protocol_flags |= _BV(7)
 #define IS_BIND_DONE           ( ( protocol_flags & _BV(7) ) !=0 )
 #define IS_BIND_IN_PROGRESS    ( ( protocol_flags & _BV(7) ) ==0 )

class Frsky_d
{
private:
    uint8_t hopping_frequency[50];
    uint8_t calibration;
    enum frsky_d_state {
        FRSKY_BIND      = 0,
        FRSKY_BIND_DONE = 1000,
        FRSKY_DATA1,
        FRSKY_DATA2,
        FRSKY_DATA3,
        FRSKY_DATA4,
        FRSKY_DATA5
    };
    int state;

    CC2500 * cc2500;

    uint16_t packet_count;
    uint8_t packet[40];
    uint8_t len;

    uint8_t  rx_tx_addr[5];
    uint8_t option;
    uint8_t prev_option;
    uint16_t counter;

    uint8_t pkt[29];

    #define NUM_CHN 16
    // Servo data
    uint8_t  Channel_AUX;

    uint16_t Channel_data[NUM_CHN];
    uint16_t failsafe_data[NUM_CHN];

    uint8_t protocol_flags=0,protocol_flags2=0;

    volatile uint32_t gWDT_entropy = 0;
    uint32_t MProtocol_id_master;
    uint32_t MProtocol_id;

    bool fail_save = false;

public:
    Frsky_d(CC2500 *cc2500);
    void init(bool bind);
    void build_bind_packet(void);
    uint16_t init_frsky_2way(void);

    uint16_t read_frsky_2way(void);
    void  frsky2way_init(bool bind);
    void frsky_init_hop(void);

    void init_cc2500(const uint8_t *ptr);
    void frsky2way_data_frame();

    uint16_t convert_channel_frsky(uint8_t num);

    void init_hop(void);

    void init_channel(void);
    void init_failsafe(void);


};



#endif /*FRSKYD_H*/
