#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "input.h"
extern class Eeprom_config eeprom_config;

class Eeprom_config {
public:

    Eeprom_config(void);
    ~Eeprom_config(void);

    int validate(void);
    int read(void);
    int write(void);

    int get_ch_config(struct Input::ch_config* config);
    int set_ch_config(struct Input::ch_config* config);

    int get_master_id(uint32_t* master_id);
    int set_master_id(uint32_t master_id);

private:
    #define CURRENT_VERSION 0x01
    struct eeprom_data_v1 {
        uint8_t version;
        uint32_t data_crc;
        struct {
            uint32_t master_id;
            struct Input::ch_config ch[Input::CH_COUNT];
        } data;
    } current_config;
    bool sucessfull_read;
};


#endif
