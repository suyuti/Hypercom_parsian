/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef	EQUATES_INC
#define EQUATES_INC

#include "osplatform.inc" //; Exclusive OS platform definitions

// ---------------------------------------------------------------------------

//      USER EQUATES
#ifndef C_DEV
MARKER	EQU		-1              // TABLE END MARKER
#else
#define	MARKER		-1
#endif

#define	YES		-1
#define	NO		0

#define BACKLIGHTBTN  	0xE4		// BACKLIGHT Button
#define MENUBUTTON    	0xE5		// MENU Button
#define PAPERFEEDBTN  	0xE6		// PAPERFEED Button
#define	ABC_BUTTON 	0xE7		// ALPHA KEY VALUE
#define	NUM_BUTTON 	0xE8		// NUMERIC KEY VALUE
#define	PAGEUPKY 	0xE9		// PAGE UP KEY VALUE
#define	PAGEDNKY 	0xEA		// PAGE DOWN KEY VALUE
#define	HELPKY   	0xEB		// HELP KEY VALUE
#define	NEXTKY   	0xEC		// NEXT KEY VALUE
#define	PREVKY   	0xED		// PREVIOUS KEY VALUE
#define	LINEUPKY 	0xEE		// LINE UP KEY VALUE
#define	LINEDNKY 	0xEF		// LINE DOWN KEY VALUE
#define	CLEAR		0xF0            // CLEAR KEY VALUE
#define	ENTER		0xF1            // ENTER KEY VALUE
#define	RESET		0xF2            // RESET KEY VALUE
#define	DZERO		0xF3            // DOUBLE ZERO KEY
#define	BSP		0xF4            // BACKSPACE KEY
#define	ALPHA		0xF5            // ALPHA ROTATE
#define	HEXKY		0xF6            // HEX ROTATE
#define	YESKY		0xF7            // YES KEY
#define	NOKY		0xF8            // NO KEY
#define	SCROLL		0xF9            // SCROLL KEY
#define	FDIAL		0xFA            // FAST DIAL SELECT
#define	ZAPKY		0xFB            // ZAP DATA
#define	REDAIL		0xFC		// REDIAL KEY, SPELLED INCORRECTLY
#define	REDIAL		REDAIL		// REDIAL KEY, SPELLED CORRECTLY

#define	CRDLEN		160             // CARD BUFFER LENGTH

#define	MS100		10		// 100 ms  time [10 ms units]
#define	MS120		12		// 120 ms  time [10 ms units]
#define	MS200		20		// 200 ms  time [10 ms units]
#define	MS400		40		// 400 ms  time [10 ms units]
#define	MS500		50		// 500 ms  time [10 ms units]
#define	MS1000		100		// 1.0 sec time [10 ms units]
#define	MS1500		150		// 1.5 sec time [10 ms units]
#define	MS2500		250		// 2.5 sec time [10 ms units]
#define	SEC2		200		// 2   sec time [10 ms units]
#define	SEC5		500		// 5   sec time [10 ms units]
#define	SEC10		1000		// 10  sec time [10 ms units]
#define	SEC15		1500		// 15  sec time [10 ms units]
#define	SEC30		3000		// 30  sec time [10 ms units]
#define	SEC60		6000		// 60  sec time [10 ms units]
#define	MIN5		30000		// 5   min time [10 ms units]

#define MS10_166	6		// 10  ms  time [1.66 ms units]

#ifdef     ICE4000_OS
#define	DSPSZE		26		// WIDTH OF DISPLAY (for 5x7 Characters)
#define DSPSZE5X7 	DSPSZE		// WIDTH OF DISPLAY (for 5x7 Characters)
#define DSPSZE6X8 	21		// WIDTH OF DISPLAY (for 6x8 Characters)
#endif //; ICE4000_OS

#ifdef     ICE5000_OS
#define	DSPSZE		26		// WIDTH OF DISPLAY (for 5x7 Characters)
#endif //; ICE5000_OS

#ifdef     ICE6000_OS
#define	DSPSZE		53		// WIDTH OF DISPLAY (for 5x7 Characters)
#endif //; ICE6000_OS

#ifdef     ICE6500_OS
#define	DSPSZE		53		// WIDTH OF DISPLAY (for 5x7 Characters)
#endif //; ICE6500_OS

#define	MAX_CO		50		// MAXIMUM NUMBER OF COMPANIES
#define	MAX_AQ		8		// MAXIMUM # OF ACQUIRERS
#define	PANMAX		20		// MAXIMUM PAN LENGTH

#define	FT10CON		0x00		// PROGRAMMING CONFIGURATION
#define	REPRIN		0x01		// REPRINT FUNCTION
#define	RECONC		0x07		// RECONCILE FUNCTION
#define	MKTST		0x10		// ACQUIRER 1 MK TEST
#define	PINTST		0x11		// PIN PAD TEST
#define	PRTTST		0x12		// PRINTER TEST
#define	DSPTST		0x13		// DISPLAY TEST
#define	KBTST		0x14		// KEYPAD TEST
#define	CARRTST		0x15		// CARRIER TEST
#define	CRDTST		0x16		// CARD READER TEST
#define	RELTST		0x17		// RELEASE INFO
#define	PPNMS		0x18		// PIN PAD NMS

#define	LEFT		0               // PACK DATA DIRECTIONS
#define	RIGHT		1

#define	ATTN		0x00            // DATA TYPES - NUMERIC
#define	ATTAN		0x01            // ASCII NUMERIC
#define	ATTANS		0x02            // ASCII NUMERIC + SPECIALS
#define	ATTBIN		0x03            // BINARY
#define	ATTXN		0x04            // alpha + numeric
#define	FFIX		0x00            // FORMAT - FIXED
#define	FLLVAR		0x10            // LLVAR
#define	FLLLVAR		0x20            // LLLVAR

#define	H_TIME		3000		// DIALUP HANGUP DELAY
#define	TIME1		300		// DISPLAY REVIVE TIMER IN SECONDS (OFF)
#define	TIME2		4		// DISPLAY REVIVE TIMER IN SECONDS (ON)

//-----------------------------------------------------------------------------
//      General definitiions

#define TPDUID_APP 0x60		// APPLICATION MESSAGES
#define TPDUID_NMS 0x68		// NMS MESSAGES
#define TPDU_SZE   5		// LENGTH OF TPDU

// ---------------------------------------------------------------------------

#endif
