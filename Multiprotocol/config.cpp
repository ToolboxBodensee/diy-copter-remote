/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Multiprotocol is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Multiprotocol.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "config.h"
uint8_t curr_bank = 0;
const PPM_Parameters PPM_prot[14*NBR_BANKS]=    {
//******************************       BANK 1       ******************************
//  Switch  Protocol            Sub protocol    RX_Num  Power       Auto Bind       Option
/*  1 */    {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 0   },
/*  2 */    {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 20   },
/*  3 */    {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 40   },
/*  4 */    {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 60  },
/*  5 */    {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 80  },
/*  6 */    {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 100  },
/*  7 */    {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 120  },
/*  8 */    {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 140  },
/*  9 */    {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 160  },
/*  10  */  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 180  },
/*  11  */  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 200  },
/*  12  */  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 220  }, // option=fine freq tuning
/*  13  */  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 240   },
/*  14  */  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , 255   },
//******************************       BANK 2       ******************************
//  Switch  Protocol        Sub protocol    RX_Num  Power       Auto Bind       Option
/*  1 */    {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  , -120    },
/*  2 */    {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  , -100   },
/*  3 */    {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  , -80   },
/*  4 */    {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  , -60   },
/*  5 */    {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  , -40   },
/*  6 */    {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  , -20   },
/*  7 */    {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  ,   0   },
/*  8 */    {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  ,  10   },
/*  9 */    {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  ,  20   },
/*  10  */  {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  ,  40   },
/*  11  */  {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  ,  60   },
/*  12  */  {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  ,  80   }, // option=fine freq tuning
/*  13  */  {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  ,  100   },
/*  14  */  {PROTO_FRSKYX,  EU_16       , 0 , P_LOW , AUTOBIND  ,  120   },
//******************************       BANK 3       ******************************
//  Switch  Protocol        Sub protocol    RX_Num  Power       Auto Bind       Option
/*  1 */    {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  , -120    },
/*  2 */    {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  , -100   },
/*  3 */    {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  , -80   },
/*  4 */    {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  , -60   },
/*  5 */    {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  , -40   },
/*  6 */    {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  , -20   },
/*  7 */    {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  ,   0   },
/*  8 */    {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  ,  10   },
/*  9 */    {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  ,  20   },
/*  10  */  {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  ,  40   },
/*  11  */  {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  ,  60   },
/*  12  */  {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  ,  80   }, // option=fine freq tuning
/*  13  */  {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  ,  100   },
/*  14  */  {PROTO_FRSKYX,  CH_16       , 0 , P_LOW , AUTOBIND  ,  120   },
};
/* Available protocols and associated sub protocols to pick and choose from
    PROTO_FLYSKY
        Flysky
        V9X9
        V6X6
        V912
        CX20
    PROTO_HUBSAN
        H107
        H301
        H501
    PROTO_FRSKYV
        NONE
    PROTO_FRSKYD
        NONE
    PROTO_FRSKYX
        CH_16
        CH_8
        EU_16
        EU_8
    PROTO_HISKY
        Hisky
        HK310
    PROTO_V2X2
        V2X2
        JXD506
    PROTO_DSM
        DSM2_22
        DSM2_11
        DSMX_22
        DSMX_11
    PROTO_DEVO
        NONE
    PROTO_YD717
        YD717
        SKYWLKRP_LOW
        SYMAX4
        XINXUN
        NIHUI
    PROTO_KN
        WLTOYS
        FEILUN
    PROTO_SYMAX
        SYMAX
        SYMAX5C
    PROTO_SLT
        NONE
    PROTO_CX10
        CX10_GREEN
        CX10_BLUE
        DM007
        JC3015_1
        JC3015_2
        MK33041
    PROTO_Q2X2
        Q222
        Q242
        Q282
    PROTO_SLT
        SLT
        VISTA
    PROTO_CG023
        CG023
        YD829
    PROTO_BAYANG
        BAYANG
        H8S3D
        X16_AH
        IRDRONE
    PROTO_ESKY
        NONE
    PROTO_MT99XX
        MT99
        H7
        YZ
        LS
        FY805
    PROTO_MJXQ
        WLH08
        X600
        X800
        H26D
        E010
        H26WH
    PROTO_SHENQI
        NONE
    PROTO_FY326
        FY326
        FY319
    PROTO_SFHSS
        NONE
    PROTO_J6PRO
        NONE
    PROTO_FQ777
        NONE
    PROTO_ASSAN
        NONE
    PROTO_HONTAI
        HONTAI
        JJRCX1
        X5C1
        FQ777_951
    PROTO_AFHDS2A
        PWM_IBUS
        PPM_IBUS
        PWM_SBUS
        PPM_SBUS
    PROTO_WK2x01
        WK2801
        WK2401
        W6_5_1
        W6_6_1
        W6_HEL
        W6_HEL_I
    PROTO_Q303
        Q303
        CX35
        CX10D
        CX10WD
    PROTO_GW008
        NONE
    PROTO_DM002
        NONE
    PROTO_CABELL
        CABELL_V3
        CABELL_V3_TELEMETRY
        CABELL_SET_FAIL_SAFE
        CABELL_UNBIND
    PROTO_ESKY150
    PROTO_H8_3D
        H8_3D
        H20H
        H20MINI
        H30MINI
    PROTO_CORONA
        COR_V1
        COR_V2
        FD_V3
    PROTO_CFLIE
        NONE
    PROTO_HITEC
        OPT_FW
        OPT_HUB
        MINIMA
    PROTO_WFLY
        NONE
    PROTO_BUGS
        NONE
    PROTO_SLT
        SLT_V1
        SLT_V2
        Q100
        Q200
        MR100
*/

// RX_Num is used for TX & RX match. Using different RX_Num values for each receiver will prevent starting a model with the false config loaded...
// RX_Num value is between 0 and 15.

// Power P_HIGH or P_LOW: High or low power setting for the transmission.
// For indoor P_LOW is more than enough.

// Auto Bind    AUTOBIND or NO_AUTOBIND
// For protocols which does not require binding at each power up (like Flysky, FrSky...), you might still want a bind to be initiated each time you power up the TX.
// As an example, it's usefull for the WLTOYS F929/F939/F949/F959 (all using the Flysky protocol) which requires a bind at each power up.
// It also enables the Bind from channel feature, allowing to execute a bind by toggling a designated channel.

// Option: the value is between -128 and +127.
// The option value is only valid for some protocols, read this page for more information: https://github.com/pascallanger/DIY-Multiprotocol-TX-Module/blob/master/Protocols_Details.md
