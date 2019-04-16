#include <EEPROM.h>

#include <Arduino.h>
#include <stdint.h>
#include <debug.h>

#include "tx_def.h"
#include "eeprom.h"
#include "crc.h"

class Eeprom_config eeprom_config;

Eeprom_config::Eeprom_config() {
    EEPROM.PageBase0 = 0x801F000;
    EEPROM.PageBase1 = 0x801F800;
    EEPROM.PageSize  = 0x400;
    EEPROM.init();
    memset(&this->current_config, 0, sizeof(this->current_config));
    this->sucessfull_read = false;
}

Eeprom_config::~Eeprom_config() {
    /* nope */
}

int Eeprom_config::validate() {
    uint32_t crc_calc = 0;

    if (this->sucessfull_read == false) {
        debugln("no read\n");
        return -1;
    }

    if (this->current_config.version != CURRENT_VERSION) {
        debugln("wrong version %lu vs %lu \n", this->current_config.version, CURRENT_VERSION);
        return -1;
    }

    crc_calc = tiny_crc32(&(this->current_config.data), sizeof(this->current_config.data));

    if (crc_calc != this->current_config.data_crc) {
        debugln("wrong crc %lu vs %lu \n", crc_calc, this->current_config.data_crc);
        return -1;
    }
    debugln("valid config\n");
    return 1;
}

int Eeprom_config::read(void) {
    uint8_t *data = NULL;
    data = (uint8_t *) &this->current_config;

    for (uint8_t i = 0; i < sizeof(struct eeprom_data_v1) ; i++) {
        data[i] = EEPROM.read(0x10 + i);
    }

    this->sucessfull_read = true;
    return 0;
}

int Eeprom_config::write(void) {
    uint8_t *data = NULL;
    data = (uint8_t *) &this->current_config;

    this->current_config.version = CURRENT_VERSION;
    this->current_config.data_crc = tiny_crc32(&(this->current_config.data), sizeof(this->current_config.data));

    for (uint8_t i = 0; i < sizeof(struct eeprom_data_v1) ; i++) {
        EEPROM.write(0x10 + i , data[i]);
    }

    return 0;
}

int Eeprom_config::get_ch_config(struct Input::ch_config* config) {
    if (this->sucessfull_read == false && config != NULL) {
        return -1;
    }
    memcpy(config, &this->current_config.data.ch, sizeof(struct Input::ch_config) * Input::CH_COUNT);
    return 0;
}

int Eeprom_config::set_ch_config(struct Input::ch_config* config) {
    memcpy(this->current_config.data.ch, config, sizeof(struct Input::ch_config) * Input::CH_COUNT);
    return 0;
}

int Eeprom_config::get_master_id(uint32_t* master_id)
{
    if (this->sucessfull_read == false && master_id != NULL) {
        return -1;
    }
    *master_id = this->current_config.data.master_id;
    return 0;
}

int Eeprom_config::set_master_id(uint32_t master_id)
{
    this->current_config.data.master_id = master_id;
    return 0;
}

int Eeprom_config::get_freq_offset(uint8_t* freq_offset) {
    if (this->sucessfull_read == false && freq_offset != NULL) {
        return -1;
    }
    *freq_offset = this->current_config.data.freq_offset;
    return 0;
}
int Eeprom_config::set_freq_offset(uint8_t freq_offset) {
    this->current_config.data.freq_offset = freq_offset;
    return 0;
}
