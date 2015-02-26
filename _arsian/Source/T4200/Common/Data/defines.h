//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _DEFINES_H_
#define _DEFINES_H_
//=============================================================================
//
// Module overview:             defines.h
//      General application definitions
//

//=============================================================================
#include "sdkgui.h"

#ifndef DWORD
typedef unsigned int DWORD;
#endif

//=============================================================================
// Public defines and typedefs
//=============================================================================
//
//      Make sure NULL is defined without having
//      to include stdio.h or stddef.h
//
#ifndef NULL
  #ifdef __cplusplus
    #define NULL    0
  #else
    #define NULL    (void *)0
  #endif
#endif

#if defined(__GNUC__)
	#define PACKED	__attribute__ ((packed))
    #define ALIGN(N) __attribute__ ((aligned (N)))
#else
	#define PACKED
    #define ALIGN(N)
#endif

// Use These Defines For Compares to See if Bit is Set
#define BIT_0_SET 0x01
#define BIT_1_SET 0x02
#define BIT_2_SET 0x04
#define BIT_3_SET 0x08
#define BIT_4_SET 0x10
#define BIT_5_SET 0x20
#define BIT_6_SET 0x40
#define BIT_7_SET 0x80


// GlobalDsp Equates            
#define ZAPKEYON  	0x02		// ZAP key is turned on
#define CLRKEYON	0x04		// CLEAR key is turned on

// Message Communication Flag Equates
#define REQUEST_BUILT     0x01	// If bit 0 is set, REQ was built
#define REQUEST_SENT      0x02	// If bit 1 is set, REQ was sent
#define RESPONSE_RECEIVED 0x04	// If bit 2 is set, RSP was received
#define RESPONSE_UNPACKED 0x08	// If bit 3 is set, RSP was unpacked

//-----------------------------------------------------------------------------
//      General definitiions

#define S_LineBuffer  	41		// Length of Printer Line Buffer

#define MAX_MSG_LEN		60		// Maximum Input/Output Message Length

// ------------------------ DEVICES -------------------------
// These flags are used by Read_IO() for Parm_Map and DevFlag
#define DEV_PAD     0x0001
#define DEV_KBD     0x0002
#define DEV_CARDRDR 0x0004
//#define DEV_SIGCAP  0x0008
#define DEV_PINCARD 0x0010
//#define DEV_RS232   0x0020
//#define DEV_EXTKBD	0x0040
#ifdef MAKE_EMV
#define DEV_SCR     0x0080			// Smart card reader
#endif // MAKE_EMV
#define DEV_TOUCH   (0x0800 | DEV_PAD)

#define STAY_OPEN0  0x0100		// Wait for 1 input or timeout.
#define STAY_OPEN1  0x0200		// Stay open until enter, clear,
								// cancel key or timeout.
#define DEV_ABORT   0x8000		// Flag for internal use by Read_IO()

//-----------------------------------------------------------------------------
//

// ----------------------FILE SYSTEM PAGES ------------------------
//

#define LAST_FILE_PAGE 0x00

	// Dummy define to satisfy code. We don't have pages on Viking.
	#define FILE_SYSTEM_PAGES	\
			pn_PAGE41,			\
			LAST_FILE_PAGE

// Screen Size
	#define SCR_PIXEL_WIDTH		GUI_SCREEN_WIDTH
	#define SCR_PIXEL_HEIGHT	GUI_SCREEN_HEIGHT

// Max characters on a line - 6 is the smallest font
#define SCR_CHAR_WIDTH		SCR_PIXEL_WIDTH/6

#define BACKLIGHT_DELAY		20	// one count per second


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================


#endif // _DEFINES_H_
