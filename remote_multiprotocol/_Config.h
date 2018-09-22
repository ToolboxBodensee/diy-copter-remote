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

/**********************************************/
/** Multiprotocol module configuration file ***/
/**********************************************/

/*******************/
/*** TX SETTINGS ***/
/*******************/
//Modify the channel order based on your TX: AETR, TAER, RETA...
//Examples: Flysky & DEVO is AETR, JR/Spektrum radio is TAER, Multiplex is AERT...
//Default is AETR.
#define AETR

/*****************/
/*** AUTO BIND ***/  // Also referred as "Bind on powerup"
/*****************/
//Bind from channel enables you to bind when a specified channel is going from low to high. This feature is only active
// if you specify AUTOBIND in PPM mode or set AutoBind to YES for serial mode. It also requires that the throttle channel is low.
//Comment to globaly disable the bind feature from a channel.
#define ENABLE_BIND_CH
//Set the channel number used for bind. Default is 16.
#define BIND_CH	16

//Comment to disable the wait for bind feature. If Autobind is enabled in the model config, this feature will not activate
// the selected protocol unless a bind is requested using bind from channel or the GUI "Bind" button.
//The goal is to prevent binding other people's model when powering up the TX, changing model or scanning through protocols.
#define WAIT_FOR_BIND


/****************/
/*** RF CHIPS ***/
/****************/
//There are 4 RF components supported. If one of them is not installed you must comment it using "//".
//If a chip is not installed all associated protocols are automatically disabled.
//4-in-1 modules have all RF chips installed
//!!!If a RF chip is present it MUST be marked as installed!!! or weird things will happen you have been warned.
//#define A7105_INSTALLED
//#define CYRF6936_INSTALLED
#define CC2500_INSTALLED
//#define NRF24L01_INSTALLED

/** CC2500 Fine Frequency Tuning **/
//For optimal performance the CC2500 RF module used by the FrSkyD, FrSkyV, FrSkyX, SFHSS, CORONA and Hitec protocols needs to be tuned for each protocol.
//Initial tuning should be done via the radio menu with a genuine FrSky/Futaba/CORONA/Hitec receiver.
//Once a good tuning value is found it can be set here and will override the radio's 'option' setting for all existing and new models which use that protocol.
//For more information: https://github.com/pascallanger/DIY-Multiprotocol-TX-Module/tree/master/docs/Frequency_Tuning.md
//Uncomment the lines below (remove the "//") and set an appropriate value (replace the "0") to enable. Valid range is -127 to +127.
#define FORCE_FRSKYD_TUNING	0
#define FORCE_FRSKYV_TUNING	0
#define FORCE_FRSKYX_TUNING	0

/** Low Power **/
//Low power is reducing the transmit power of the multi module. This setting is configurable per model in PPM (table below) or Serial mode (radio GUI).
//It can be activated when flying indoor or small models since the distance is short or if a model is causing issues when flying closed to the TX.
//By default low power is completly disabled on all rf chips to prevent mistakes, but you can enable it by uncommenting the lines below:
//#define CC2500_ENABLE_LOW_POWER


/*****************/
/*** GLOBAL ID ***/
/*****************/
//A global ID is used by most protocols to bind and retain the bind to models. To prevent duplicate IDs, it is automatically
// generated using a random 32 bits number the first time the eeprom is initialized.
//If you have 2 Multi modules which you want to share the same ID so you can use either to control the same RC model
// then you can force the ID to a certain known value using the lines below.
//Default is commented, you should uncoment only for test purpose or if you know exactly what you are doing!!!
#define FORCE_GLOBAL_ID	0x12345678

//Protocols using the CYRF6936 (DSM, Devo, Walkera...) are using the CYRF ID instead which should prevent duplicated IDs.
//If you have 2 Multi modules which you want to share the same ID so you can use either to control the same RC model
// then you can force the ID to a certain known value using the lines below.
//Default is commented, you should uncoment only for test purpose or if you know exactly what you are doing!!!
//#define FORCE_CYRF_ID	"\x12\x34\x56\x78\x9A\xBC"


/****************************/
/*** PROTOCOLS TO INCLUDE ***/
/****************************/
//In this section select the protocols you want to be accessible when using the module.
//All the protocols will not fit in the Atmega328p module so you need to pick and choose.
//Comment the protocols you are not using with "//" to save Flash space.

//The protocols below need a CC2500 to be installed
#define	FRSKYD_CC2500_INO
#define	FRSKYV_CC2500_INO
#define	FRSKYX_CC2500_INO


/**************************/
/*** FAILSAFE SETTINGS  ***/
/**************************/
//The module is using the same default failsafe values for all protocols which currently supports it:
//  Devo, WK2x01, SFHSS, HISKY/HK310 and AFHDS2A
//All channels are centered except throttle which is forced low.
//If you want to diasble failsafe globally comment the line below using "//".
#define FAILSAFE_ENABLE

//Failsafe throttle low value in percentage.
//Value between -125% and +125%. Default -100.
#define FAILSAFE_THROTTLE_LOW -100

//The radio using serial protocol can set failsafe data.
// Two options are available:
//  a. replace the default failsafe data with serial failsafe data when they are received.
//  b. wait for the radio to provide failsafe before sending it. Enable advanced settings like "FAILSAFE NOT SET" or "FAILSAFE RX".
// Option a. is the default since you have a protection even if no failsafe has been set on the radio.
// You can force option b. by uncommenting the line below (remove the "//").
//#define FAILSAFE_SERIAL_ONLY


/**************************/
/*** TELEMETRY SETTINGS ***/
/**************************/
//In this section you can configure the telemetry.

//If you do not plan using the telemetry comment this global setting using "//" and skip to the next section.
//#define TELEMETRY

//Comment to invert the polarity of the output telemetry serial signal.
//This function takes quite some flash space and processor power on an atmega.
//For OpenTX it must be uncommented.
//On a 9XR_PRO running ersky9x both commented and uncommented will work depending on the radio setting Invert COM1 under the Telemetry menu.
//On other addon/replacement boards like the 9xtreme board or the Ar9x board running ersky9x, you need to uncomment the line below.
//For er9x it depends if you have an inveter mod or not on the telemetry pin. If you don't have an inverter comment this line.
//#define INVERT_TELEMETRY

//Comment if you don't want to send Multi status telemetry frames (Protocol available, Bind in progress, version...)
//Use with er9x/erksy9x, for OpenTX MULTI_TELEMETRY below is preferred instead
//#define MULTI_STATUS

//Uncomment to send Multi status and allow OpenTX to autodetect the telemetry format
//Supported by OpenTX version 2.2 RC9 and newer. NOT supported by er9x/ersky9x use MULTI_STATUS instead.
//#define MULTI_TELEMETRY

//Comment a line to disable a specific protocol telemetry
//#define DSM_TELEMETRY				// Forward received telemetry packet directly to TX to be decoded by er9x, ersky9x and OpenTX
//#define SPORT_TELEMETRY				// Use FrSkyX SPORT format to send telemetry to TX
//#define AFHDS2A_FW_TELEMETRY		// Forward received telemetry packet directly to TX to be decoded by ersky9x and OpenTX
//#define AFHDS2A_HUB_TELEMETRY		// Use FrSkyD Hub format to send basic telemetry to TX like er9x
//#define HUB_TELEMETRY				// Use FrSkyD Hub format to send telemetry to TX
//#define BAYANG_HUB_TELEMETRY		// Use FrSkyD Hub format to send telemetry to TX
//#define BUGS_HUB_TELEMETRY			// Use FrSkyD Hub format to send telemetry to TX
//#define HUBSAN_HUB_TELEMETRY		// Use FrSkyD Hub format to send telemetry to TX
//#define CABELL_HUB_TELEMETRY		// Use FrSkyD Hub format to send telemetry to TX
//#define HITEC_HUB_TELEMETRY			// Use FrSkyD Hub format to send basic telemetry to the radios which can decode it like er9x, ersky9x and OpenTX
//#define HITEC_FW_TELEMETRY			// Under development: Forward received telemetry packets to be decoded by ersky9x and OpenTX

//SPORT_POLLING is an implementation of the same polling routine as XJT module for sport telemetry bidirectional communication.
//This is useful for passing sport control frames from TX to RX(ex: changing Betaflight PID or VTX channels on the fly using LUA scripts with OpentX).
//Using this feature requires to uncomment INVERT_TELEMETRY as this TX output on telemetry pin only inverted signal.
//!!!! This is a work in progress!!! Do not enable unless you want to test and report
//#define SPORT_POLLING


/****************************/
/*** SERIAL MODE SETTINGS ***/
/****************************/
//In this section you can configure the serial mode.
//The serial mode enables full editing of all the parameters in the GUI of the radio. It is enabled by placing the rotary switch on position 0.
//This is available natively for ER9X, ERSKY9X and OpenTX.

//If you do not plan to use the Serial mode comment this line using "//" to save Flash space
//#define ENABLE_SERIAL


/*************************/
/*** PPM MODE SETTINGS ***/
/*************************/
//In this section you can configure all details about PPM.
//If you do not plan to use the PPM mode comment this line using "//" to save Flash space, you don't need to configure anything below in this case
#define ENABLE_PPM

/** TX END POINTS **/
//It is important for the module to know the endpoints of your radio.
//Below are some standard transmitters already preconfigured.
//Uncomment only the one which matches your transmitter.
#define TX_ER9X			//ER9X/ERSKY9X/OpenTX	( 988<->2012 microseconds)
//#define TX_DEVO7		//DEVO					(1120<->1920 microseconds)
//#define TX_SPEKTRUM	//Spektrum				(1100<->1900 microseconds)
//#define TX_HISKY		//HISKY					(1120<->1920 microseconds)
//#define TX_MPX		//Multiplex MC2020		(1250<->1950 microseconds)
//#define TX_WALKERA	//Walkera PL0811-01H	(1000<->1800 microseconds)
//#define TX_CUSTOM		//Custom

// The lines below are used to set the end points in microseconds if you have selected TX_CUSTOM.
// A few things to consider:
//  - If you put too big values compared to your TX you won't be able to reach the extremes which is bad for throttle as an example
//  - If you put too low values you won't be able to use your full stick range, it will be maxed out before reaching the ends
//  - Centered stick value is usually 1500. It should match the middle between MIN and MAX, ie Center=(MAX+MIN)/2. If your TX is not centered you can adjust the value MIN or MAX.
//  - 100% is referred as the value when the TX is set to default with no trims
#if defined(TX_CUSTOM)
	#define PPM_MAX_100	1900	//	100%
	#define PPM_MIN_100	1100	//	100%
#endif

/** Number of PPM Channels **/
// The line below is used to set the minimum number of channels which the module should receive to consider a PPM frame valid.
// The default value is 4 to receive at least AETR for flying models but you could also connect the PPM from a car radio which has only 3 channels by changing this number to 3.
#define MIN_PPM_CHANNELS 4
// The line below is used to set the maximum number of channels which the module should work with. Any channels received above this number are discarded.
// The default value is 16 to receive all possible channels but you might want to filter some "bad" channels from the PPM frame like the ones above 6 on the Walkera PL0811.
#define MAX_PPM_CHANNELS 16

/** Rotary Switch Protocol Selector Settings **/
//The table below indicates which protocol to run when a specific position on the rotary switch has been selected.
//All fields and values are explained below. Everything is configurable from here like in the Serial mode.
//Tip: You can associate multiple times the same protocol to different rotary switch positions to take advantage of the model match based on RX_Num

//A system of banks enable the access to more protocols than positions on the rotary switch. Banks can be selected by placing the rotary switch on position 15, power up the module and
// short press the bind button multiple times until you reach the desired one. The bank number currently selected is indicated by the number of LED flash.
// Full procedure is located here: https://github.com/pascallanger/DIY-Multiprotocol-TX-Module/blob/master/Protocols_Details.md#protocol-selection-in-ppm-mode

//The parameter below indicates the number of desired banks between 1 and 5. Default is 5.
#define NBR_BANKS 3

const PPM_Parameters PPM_prot[14*NBR_BANKS]=	{
#if NBR_BANKS > 0
//******************************       BANK 1       ******************************
//	Switch	Protocol 	    	Sub protocol	RX_Num	Power		Auto Bind		Option
/*	1	*/	  {PROTO_FRSKYD,	0		        ,	0	,	P_LOW	,	AUTOBIND	,	-120		},
/*  2 */    {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , -100   },
/*	3	*/	  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , -80   },
/*	4	*/	  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , -60   },
/*	5	*/	  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , -40   },
/*	6	*/	  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  , -20   },
/*	7	*/	  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  ,   0   },
/*	8	*/	  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  ,  10   },
/*	9	*/	  {PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  ,  20   },
/*	10	*/	{PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  ,  40   },
/*	11	*/	{PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  ,  60   },
/*	12	*/	{PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  ,  80   }, // option=fine freq tuning
/*	13	*/	{PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  ,  100   },
/*	14	*/	{PROTO_FRSKYD,  0           , 0 , P_LOW , AUTOBIND  ,  120   },
#endif
#if NBR_BANKS > 1
//******************************       BANK 2       ******************************
//	Switch	Protocol 		Sub protocol	RX_Num	Power		Auto Bind		Option
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
#endif
#if NBR_BANKS > 2
//******************************       BANK 3       ******************************
//	Switch	Protocol 		Sub protocol	RX_Num	Power		Auto Bind		Option
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
#endif
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

// Auto Bind	AUTOBIND or NO_AUTOBIND
// For protocols which does not require binding at each power up (like Flysky, FrSky...), you might still want a bind to be initiated each time you power up the TX.
// As an example, it's usefull for the WLTOYS F929/F939/F949/F959 (all using the Flysky protocol) which requires a bind at each power up.
// It also enables the Bind from channel feature, allowing to execute a bind by toggling a designated channel.

// Option: the value is between -128 and +127.
// The option value is only valid for some protocols, read this page for more information: https://github.com/pascallanger/DIY-Multiprotocol-TX-Module/blob/master/Protocols_Details.md
