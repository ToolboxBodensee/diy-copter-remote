#ifndef _TX_DEV_H_
#define _TX_DEV_H_

// Turnigy PPM and channels
#if defined(TX_ER9X)
    #define PPM_MAX_100 2012    //  100%
    #define PPM_MIN_100 988     //  100%
#endif

// Devo PPM and channels
#if defined(TX_DEVO7)
    #define PPM_MAX_100 1920    //  100%
    #define PPM_MIN_100 1120    //  100%
#endif

// SPEKTRUM PPM and channels
#if defined(TX_SPEKTRUM)
    #define PPM_MAX_100 1900    //  100%
    #define PPM_MIN_100 1100    //  100%
#endif

// HISKY
#if defined(TX_HISKY)
    #define PPM_MAX_100 1920    //  100%
    #define PPM_MIN_100 1120    //  100%
#endif

// Multiplex MC2020
#if defined(TX_MPX)
    #define PPM_MAX_100 1950 // 100%
    #define PPM_MIN_100 1250 // 100%
#endif

// Walkera PL0811-01H
#if defined(TX_WALKERA)
    #define PPM_MAX_100 1800 // 100%
    #define PPM_MIN_100 1000 // 100%
#endif

//Channel MIN MAX values
#define CHANNEL_MAX_100 1844    //  100%
#define CHANNEL_MIN_100 204     //  100%
#define CHANNEL_MAX_125 2047    //  125%
#define CHANNEL_MIN_125 0       //  125%

#define CHANNEL_MIN_COMMAND 784     // 1350us
#define CHANNEL_SWITCH      1104    // 1550us
#define CHANNEL_MAX_COMMAND 1424    // 1750us

//Channel definitions


#endif /* _TX_DEV_H_ */
