#include "cc2500.h"
#include "config.h"
#include "pins.h"
#include "Arduino.h"

CC2500::CC2500(SPI *spi)
{
    this->spi = spi;
    this->bind_done = false;

    CC2500_CS_OUTPUT;
    CC2500_CS_ON;

    LOW_NOISE_AMP_OUTPUT;
    LOW_NOISE_AMP_OFF;
}

void CC2500::write_register(uint8_t address, uint8_t data)
{
    // select chip
    CC2500_CS_OFF;

    // first address then data
    spi->write(address);
    NOP;
    spi->write(data);

    // deselect chip
    CC2500_CS_ON;
}
void CC2500::write_register_multi(uint8_t address, const uint8_t data[], uint8_t length)
{
    CC2500_CS_OFF;

    spi->write(CC2500_WRITE_BURST | address);

    for (uint8_t i = 0; i < length; i++) {
        spi->write(data[i]);
    }

    CC2500_CS_ON;
}

void CC2500::write_data(uint8_t *dpbuffer, uint8_t len)
{
    this->strobe(CC2500_SFTX);
    this->write_register_multi(CC2500_3F_TXFIFO, dpbuffer, len);
    this->strobe(CC2500_STX);
}


void CC2500::read_register_multi(uint8_t address, uint8_t data[], uint8_t length)
{
    // select chip
    CC2500_CS_OFF;

    spi->write(CC2500_READ_BURST | address);

    for (uint8_t i = 0; i < length; i++) {
        data[i] = spi->read();
    }

    // deselect chip
    CC2500_CS_ON;
}

uint8_t CC2500::read_register(uint8_t address)
{
    uint8_t result;

    // select chip
    CC2500_CS_OFF;

    spi->write(CC2500_READ_SINGLE | address);
    result = spi->read();

    // deselect chip
    CC2500_CS_ON;

    return (result);
}

void CC2500::read_data(uint8_t *dpbuffer, uint8_t len)
{
    this->read_register_multi(CC2500_3F_RXFIFO, dpbuffer, len);
}

void CC2500::strobe(uint8_t state)
{
    CC2500_CS_OFF;
    spi->write(state);
    CC2500_CS_ON;
}

void CC2500::set_wireless_mode(enum wireless_mode mode)
{
    switch (mode) {
    case wireless_tx:
        this->write_register(CC2500_00_IOCFG2, 0x2F);
        this->write_register(CC2500_02_IOCFG0, 0x2F | 0x40);
        break;

    case wireless_rx:
        this->write_register(CC2500_02_IOCFG0, 0x2F);
        this->write_register(CC2500_00_IOCFG2, 0x2F | 0x40);
        break;

    case wireless_off:
        this->write_register(CC2500_02_IOCFG0, 0x2F);
        this->write_register(CC2500_00_IOCFG2, 0x2F);
        break;
    }
}

void CC2500::set_power(void)
{
    uint8_t power = CC2500_BIND_POWER;

    if (this->bind_done) {
#ifdef CC2500_ENABLE_LOW_POWER
        power = IS_POWER_FLAG_on ? CC2500_HIGH_POWER : CC2500_LOW_POWER;
#else
        power = CC2500_HIGH_POWER;
#endif
    }

    if (IS_RANGE_FLAG_on) {
        power = CC2500_RANGE_POWER;
    }

    if (this->prev_power != power) {
        this->write_register(CC2500_3E_PATABLE, power);
        this->prev_power = power;
    }
}
uint8_t CC2500::reset()
{
    this->strobe(CC2500_SRES);
    delay(1); //ms
    this->set_wireless_mode(wireless_off);
    return this->read_register(CC2500_0E_FREQ1) == 0xC4;//check if reset
}


void CC2500::resetChip(void)
{
    // Toggle chip select signal
    CC2500_CS_ON;
    delayMicroseconds(30);

    CC2500_CS_OFF;
    delayMicroseconds(30);

    CC2500_CS_ON;
    delayMicroseconds(45);

    this->strobe(CC2500_SRES);
    _delay_ms(100);

    this->prev_power = 0xfd;
}

void CC2500::set_power_value(uint8_t power)
{
    const unsigned char patable[8] = {
        0xC5,  // -12dbm
        0x97, // -10dbm
        0x6E, // -8dbm
        0x7F, // -6dbm
        0xA9, // -4dbm
        0xBB, // -2dbm
        0xFE, // 0dbm
        0xFF // 1.5dbm
    };

    if (power > 7)
        power = 7;

    this->write_register(CC2500_3E_PATABLE,  patable[power]);
}
