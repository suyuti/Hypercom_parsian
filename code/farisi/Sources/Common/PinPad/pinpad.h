//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _PINPAD_H_
#define _PINPAD_H_

//=============================================================================
//
// Module overview:             pinpad.h
//      pinpad definitions, data, and functions
//
//=============================================================================

#include "pinwrap.h"
//=============================================================================
// Public defines and typedefs
//=============================================================================
//====================================================
//===  PINSTAT bit values                          ===
//====================================================

//! Card read from pin.
#define PinCard_Read		0x01
//! Card Read Request Pending Bit
#define PinCardReadReqPend	0x02
//! Waiting for User Action
#define PinWaitUserAction	0x04
//! Wait for Reset Response
#define PinWaitResetResp	0x08
//! Card Read Error
#define PinCardReadError	0x10
//! Smart Card read from pinpad
#define PinSmartCard_Read	0x40
//! Cancel Card Swipe
#define PinCancelCardSwipe	0x80

//====================================================
//===  PIN_TYPE values                             ===
//====================================================

//! INVALID PIN PAD
#define PIN_TYPE_INVALID	0
//! EXTERNAL PIN PAD (UNDEFINED TYPE)
#define PIN_TYPE_EXTERNAL	1
//! INTEGRATED ICE5000
#define PIN_TYPE_INTERNAL	2
//! S9 PINPAD
#define PIN_TYPE_S9		3
//! S9SC PINPAD
#define PIN_TYPE_S9SC		4
//! HFT 106 PINPAD
#define PIN_TYPE_HFT106		5
//! P1100 PINPAD (HFT PROTOCOL)
#define PIN_TYPE_P1100		6

//====================================================
//===  PIN Buffer size                             ===
//====================================================

#define S_PINBUF 1024


#define KEKMAX 16
#define S_KEKKEY 16

typedef struct kek_rec
{
    //! Key Encrypting Key
	UBYTE KEKKEY[S_KEKKEY];
    //! Tag for Key Encrypting Key
	UBYTE KEKTAG;
}
KEKREC;

#define KEKSIZE sizeof(struct kek_rec)

#define PSWDSIZE 8

typedef struct password_rec
{
	UBYTE PASSWORD[PSWDSIZE];
}
PASSWORDREC;

// Structure that holds the PED device attributes (version, etc)
typedef struct pedver_rec
{
	UBYTE   PEDTYPE[7];     // PED TYPE STRING
	UBYTE   LDRVER[8];	    // SEC Loader Version (Swedish PED only)
    UBYTE   KEYAREA[9];     // Key Area check value (Swedish PED only)
    UBYTE   SERIALNO[17];   // PED Serial Number
    UBYTE   SECVER[17];     // SEC application version
    UBYTE   EMVL2VER[17];     // EMVL2 partial kernel version
    UBYTE   EMVL2CKSUM[8];  // EMVL2 partial kernel checksum
    UBYTE   EMVL1VER[17];   // EMVL1 version
    UBYTE   DISPFMT[3];     // Format of the display row:col
    UBYTE   EMVL1CKSUM[8];  // EMVL2 partial kernel checksum
    UBYTE   SWITCH;         // K1100/1200 Switch status
} __attribute__((packed)) PEDVERREC;

// Pinpad Operation Mode definitions
typedef enum {
    PIN_INVALID_MODE,
    PIN_CAT33_2400,
    PIN_CAT33_9600,
    PIN_CAT33_19200,
    PIN_POSMINI_2400,
    PIN_POSMINI_9600,
    PIN_POSMINI_19200,
}PIN_MODE;

// Pin type definition for EMV Offline PIN verification
typedef enum {
    PINTYPE_CLEAR = '1',
    PINTYPE_ENCR  = '2',
}PINOFFLTYPE;

// Pin type definition for Online PIN verification
typedef enum {
    PINTYPE_MASTER = 0,
    PINTYPE_DUKPT  = 1,
}PINONLTYPE;



//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================
extern void PinInit( void );
extern void Rewind_Pin( void );
extern Bool PinDisplayDiag( void );
extern Bool PinGetPin( void );
extern Bool PinCardRead( void );
extern Bool PinCardReadResp( void );
extern void PinReset( void );
extern Bool PinMessage( enum msg_id PinMsgNumber );
extern Bool PinGetVersion( PEDVERREC *version );
extern Bool PinSetMode( PIN_MODE mode );
extern Bool PinAutoDetect( void );

#ifdef MAKE_EMV
extern Bool EMV_PINVerifyOffline( UBYTE CVMstatus, UBYTE tries, UBYTE* PINdata, UBYTE* retStatus );
extern Bool EMV_PINVerifyOnline( UBYTE* retStatus );
#endif	// MAKE_EMV

#if defined( MAKE_ISOMAC ) 
extern Bool PinGenMac( UBYTE PinMacId, UBYTE *pMacKeyPtr, UBYTE *pMacDataPtr, UWORD MacDataLength, UBYTE *PinData );
#endif // MAKE_ISOMAC


#endif // _PINPAD_H_

