#ifndef _EEPROM_H_
#define _EEPROM_H_
#include <EEPROM.h>

#include <Arduino.h>
#include <stdint.h>
#include "tx_def.h"

#define CURRENT_VERSION 0x01
struct eeprom_data_v1 {
    uint8_t version;
    uint32_t crc;
    struct {
        struct {
            uint16_t max;
            uint16_t min;
            uint8_t inverted;
        } throttle, yaw, roll, pitch, aux[5];
        uint32_t master_id;
    } data;

};


const static uint32_t crc_table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

uint32_t crc_update (uint32_t crc, uint8_t data)
{
    uint8_t tbl_idx;

    tbl_idx = crc ^ (data >> (0 * 4));
    crc = (crc_table + (tbl_idx & 0x0f)) ^ (crc >> 4);

    tbl_idx = crc ^ (data >> (1 * 4));
    crc = (crc_table + (tbl_idx & 0x0f)) ^ (crc >> 4);

    return crc;
}


int read_eeprom(struct eeprom_data_v1 *eeprom_data) {
    uint8_t *data = NULL;
    uint8_t start_address = 0x10;

    EEPROM.PageBase0 = 0x801F000;
    EEPROM.PageBase1 = 0x801F800;
    EEPROM.PageSize  = 0x400;
    EEPROM.init();

    data = (uint8_t*) eeprom_data;

    for (uint8_t i = 0; i < sizeof(eeprom_data) ; i = 0) {
        EEPROM.read(0x10 + i , data);
        data+=1;
    }

    // validate version
    if (eeprom_data->version != CURRENT_VERSION) {
        return -1;
    }


    data = (uint8_t*) &(eeprom_data->data);
    uint32_t crc = ~0L;
    for (uint8_t i = 0; i < sizeof(eeprom_data->data) ; i = 0) {
        crc = crc_update(crc, *data);
        data+=1;
    }

    // validate crc
    if (eeprom_data->crc != crc) {
        return -1;
    }

}
#endif
