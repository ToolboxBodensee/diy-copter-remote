#ifndef _VALIDATE_H_
#define _VALIDATE_H_
#include "config.h"
#include "tx_def.h"

// Check forced tuning values are valid
#ifdef FORCE_FRSKYD_TUNING
    #if ( FORCE_FRSKYD_TUNING < -127 ) || ( FORCE_FRSKYD_TUNING > 127 )
        #error "The FrSkyD forced frequency tuning value is outside of the range -127..127."
    #endif
#endif
#ifdef FORCE_FRSKYV_TUNING
    #if ( FORCE_FRSKYV_TUNING < -127 ) || ( FORCE_FRSKYV_TUNING > 127 )
        #error "The FrSkyV forced frequency tuning value is outside of the range -127..127."
    #endif
#endif
#ifdef FORCE_FRSKYX_TUNING
    #if ( FORCE_FRSKYX_TUNING < -127 ) || ( FORCE_FRSKYX_TUNING > 127 )
        #error "The FrSkyX forced frequency tuning value is outside of the range -127..127."
    #endif
#endif

#endif
