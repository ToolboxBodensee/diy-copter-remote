#ifndef CONFIG_H
#define CONFIG_H

const uint8_t FRSKYD_cc2500_conf[] = {
    /*02_IOCFG0*/    0x06,
    /*00_IOCFG2*/    0x06,
    /*17_MCSM1*/     0x0c,
    /*18_MCSM0*/     0x18,
    /*06_PKTLEN*/    0x19,
    /*07_PKTCTRL1*/  0x04,
    /*08_PKTCTRL0*/  0x05,
    /*3E_PATABLE*/   0xff,
    /*0B_FSCTRL1*/   0x08,
    /*0C_FSCTRL0*/   0x00,
    /*0D_FREQ2*/     0x5c,
    /*0E_FREQ1*/     0x76,
    /*0F_FREQ0*/     0x27,
    /*10_MDMCFG4*/   0xAA,
    /*11_MDMCFG3*/   0x39,
    /*12_MDMCFG2*/   0x11,
    /*13_MDMCFG1*/   0x23,
    /*14_MDMCFG0*/   0x7a,
    /*15_DEVIATN*/   0x42
};

const uint8_t FRSKY_common_end_cc2500_conf[][2] = {
    { CC2500_19_FOCCFG,   0x16 },
    { CC2500_1A_BSCFG,    0x6c },
    { CC2500_1B_AGCCTRL2, 0x43 },
    { CC2500_1C_AGCCTRL1, 0x40 },
    { CC2500_1D_AGCCTRL0, 0x91 },
    { CC2500_21_FREND1,   0x56 },
    { CC2500_22_FREND0,   0x10 },
    { CC2500_23_FSCAL3,   0xa9 },
    { CC2500_24_FSCAL2,   0x0A },
    { CC2500_25_FSCAL1,   0x00 },
    { CC2500_26_FSCAL0,   0x11 },
    { CC2500_29_FSTEST,   0x59 },
    { CC2500_2C_TEST2,    0x88 },
    { CC2500_2D_TEST1,    0x31 },
    { CC2500_2E_TEST0,    0x0B },
    { CC2500_03_FIFOTHR,  0x07 },
    { CC2500_09_ADDR,     0x00 }
};

const uint8_t FRSKY_common_startreg_cc2500_conf[] = {
    CC2500_02_IOCFG0,
    CC2500_00_IOCFG2,
    CC2500_17_MCSM1,
    CC2500_18_MCSM0,
    CC2500_06_PKTLEN,
    CC2500_07_PKTCTRL1,
    CC2500_08_PKTCTRL0,
    CC2500_3E_PATABLE,
    CC2500_0B_FSCTRL1,
    CC2500_0C_FSCTRL0,     // replaced by option value
    CC2500_0D_FREQ2,
    CC2500_0E_FREQ1,
    CC2500_0F_FREQ0,
    CC2500_10_MDMCFG4,
    CC2500_11_MDMCFG3,
    CC2500_12_MDMCFG2,
    CC2500_13_MDMCFG1,
    CC2500_14_MDMCFG0,
    CC2500_15_DEVIATN
};
//AETR
    #define AILERON  0
    #define ELEVATOR 1
    #define THROTTLE 2
    #define RUDDER   3

#endif
