/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef FIXDATA_APP_H
#define FIXDATA_APP_H

#ifdef __cplusplus
extern "C" {
#endif

//=============================================================================
// Module overview:
//	This file contains the C declarations for the items in fixdata.s
//	which belong to the application.
//	This file is "included" from fixdata.h.
//	The OS provides a default version of this file, but any application
//	can override by simply providing their own copy.

//=============================================================================

//=============================================================================
// Public defines and typedefs
//=============================================================================

#if !defined( __GNUC__ ) && !defined( __attribute__ )
#define __attribute__(A)
#endif

//=============================================================================
// Public data declarations
//=============================================================================

#if defined( _MSC_VER )
#pragma pack(1)
#endif

/**
	@brief Application initialized flags.
*/
extern volatile struct _ENAPPL
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned initialized     : 1;	//     0 - Main initialization is complete
} __attribute__((packed)) ENAPPL;

extern	IOCS_BLK KBDBLK;
extern	IOCS_BLK DSPBLK;
extern	IOCS_BLK CRDBLK;
extern	IOCS_BLK PINBLK;
extern	IOCS_BLK PINBLKW;
extern	IOCS_BLK PINBLKR;
extern	IOCS_BLK PINBLK;
extern	IOCS_BLK ERRBLK;
extern	IOCS_BLK BUZBLK;
extern	IOCS_BLK Z8IBLK;
extern	IOCS_BLK Z8IBLKR;
extern	IOCS_BLK COMBLK;
extern	IOCS_BLK COMBLKR;			// Comm read
extern	IOCS_BLK COMBLKS;			// Comm status
extern	IOCS_BLK HSETBLK;			// Handset monitor
extern	IOCS_BLK COMBLKW;
extern	IOCS_BLK HNGBLK;
extern	IOCS_BLK RTCBLK;
extern	IOCS_BLK H_KBDBLK;			// Host keyboard
extern	IOCS_BLK CRBBLK;			// Second card read channel
extern	IOCS_BLK LPTBLK;			// Parallel printer interface
extern	IOCS_BLK SCRBLK;			// Smart card
extern	IOCS_BLK SCRBLKW;			// Smart card
extern	IOCS_BLK PADBLK;			// Touch pad
extern	IOCS_BLK MCDBLK;			// Memory card
extern	IOCS_BLK SIGBLK;			// Signature capture
extern	IOCS_BLK PRTBLK;			// RS232 port
extern	IOCS_BLK IOCS_06;			// Spare
extern	IOCS_BLK IOCS_07;			// Spare
extern	IOCS_BLK IOCS_08;			// Spare
extern	IOCS_BLK IOCS_09;			// Spare
extern	IOCS_BLK IOCS_10;			// Spare

extern	PTCS_BLK	PPTBLK;  		//
extern	PTCS_BLK	DTIMER;  		// DIAL TIMER BLOCK
extern	PTCS_BLK	FSHBLK;  		// FLASH TIMER BLOCK
extern	PTCS_BLK	UPTBLK;  		// UP/DOWN TIME COUNTER
extern	PTCS_BLK	COMSLP;  		// Comm relinquish block
extern	PTCS_BLK	TIMBLK;  		// GP timer
extern	PTCS_BLK	RTIMER;  		// response timer
extern	PTCS_BLK	CLKBLK;  		// CLOCK UPDATE TIMER
extern	PTCS_BLK	BSYBLK;  		// TIMER FOR BUSY LINE PROCESSING
extern	PTCS_BLK	HTIMER;  		//
extern	PTCS_BLK	DIRTIMER;		//
extern	PTCS_BLK	PINTIM;  		//
extern	PTCS_BLK	TIM_01;  		// Spare
extern	PTCS_BLK	TIM_02;  		// Spare
extern	PTCS_BLK	TIM_03;  		// Spare
extern	PTCS_BLK	TIM_04;  		// Spare
extern	PTCS_BLK	TIM_05;  		// Spare
extern	PTCS_BLK	TIM_06;  		// Spare
extern	PTCS_BLK	TIM_07;  		// Spare
extern	PTCS_BLK	TIM_08;  		// Spare
extern	PTCS_BLK	TIM_09;  		// Spare
extern	PTCS_BLK	TIM_10;  		// Spare

/**
	@brief Semaphore flags
*/
extern volatile struct _SEMFLG
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) SEMFLG;

/**
	@brief Timer flags
*/
extern volatile struct _TIMFLG
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned expired         : 1;	//     0 - Timer has expired
} __attribute__((packed)) TIMFLG;

/**
	@brief Card reader flags
*/
extern volatile struct _CRDFLG
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) CRDFLG;

/**
	@brief Dial flags
*/
extern volatile struct _HANGFLG
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) HANGFLG;

/**
	@brief Dial flags
*/
extern volatile struct _DIALFLG
{
    unsigned dialCmdComplete : 1; 	// BIT 7 - DIAL COMMAND COMPLETE (INTERNAL)
    unsigned resetComms      : 1; 	//     6 - RESET COMMS
    unsigned dialCompleteFun7: 1; 	//     5 - DIAL COMPLETE FOR FUN_7
    unsigned autoAnswer      : 1; 	//     4 - AUTOANSWER
    unsigned hangUpNow       : 1; 	//     3 - HANG UP NOW
    unsigned hangUpSoon      : 1; 	//     2 - HANG UP SOON
    unsigned dialComplete    : 1; 	//     1 - DIAL COMPLETE STATUS IN LNKERR
    unsigned dialRequested   : 1; 	//     0 - DIAL COMMAND REQUESTED
} __attribute__((packed)) DIALFLG;

/**
	@brief Idle flags
*/
extern volatile struct _IDLEFLG
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) IDLEFLG;

/**
	@brief Key pad flags
*/
extern volatile struct _KEYFLG
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) KEYFLG;

/**
	@brief Serial printer port flags
*/
extern volatile struct _PRTFLG
{
    unsigned deviceOpen      : 1;	// BIT 7 - Device is open
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) PRTFLG;

/**
	@brief Communication device flags
*/
extern volatile struct _COMFLG
{
    unsigned readCmdComplete : 1; 	// BIT 7 - READ COMPLETE (INTERNAL ONLY)
    unsigned linkRestart     : 1; 	//     6 - LINK RESTART REQUIRED
    unsigned dialComplete    : 1; 	//     5 - DIAL COMPLETE - STATUS IN LNKERR
    unsigned nmsMsgRequest   : 1; 	//     4 - NMS MESSAGE REQUEST - STATUS IN LNKERR (NMSSTART & NMSEND)
    unsigned readComplete    : 1; 	//     3 - READ COMPLETE
    unsigned readPending     : 1; 	//     2 - READ PENDING  (INTERNAL ONLY)
    unsigned appMsgRequest   : 1; 	//     1 - APPLICATION MESSAGE REQUEST (TXSTART & TXEND)
    unsigned waitingForResp  : 1; 	//     0 - WAITING FOR RESPONSE, DO NOT HANG UP
} __attribute__((packed)) COMFLG;

/**
	@brief PIN pad flag flags
*/
extern volatile struct _PINFLG
{
    unsigned deviceOpen      : 1;	// BIT 7 - Device is open
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned cardReadPending : 1;	//     4 - Card read is requested
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned available       : 1;	//     1 - OK to use
    unsigned readComplete    : 1;	//     0 - Read completed
} __attribute__((packed)) PINFLG;

/**
	@brief Second card reader flag.
*/
extern volatile struct _CRBFLG
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) CRBFLG;

/**
	@brief Parallel line printer flag.
*/
extern volatile struct _LPTFLG
{
    unsigned deviceOpen      : 1;	// BIT 7 - Device is open
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) LPTFLG;

/**
	@brief Semaphore for PIOB.
*/
extern volatile struct _SEMPIO
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) SEMPIO;

/**
	@brief Smart card interface.
*/
extern volatile struct _SCRFLG
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned cardIn          : 1;	//     0 - Card is logically in
} __attribute__((packed)) SCRFLG;

/**
	@brief SAM smart card.
*/
extern volatile struct _SC2FLG
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) SC2FLG;

/**
	@brief Flag for expansion.
*/
extern volatile struct _FLG_07
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) FLG_07;

/**
	@brief Flag for expansion.
*/
extern volatile struct _FLG_08
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) FLG_08;

/**
	@brief Flag for expansion.
*/
extern volatile struct _FLG_09
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) FLG_09;

/**
	@brief Flag for expansion.
*/
extern volatile struct _FLG_10
{
    unsigned bit7            : 1;	// BIT 7 - UNUSED
    unsigned bit6            : 1;	//     6 - UNUSED
    unsigned bit5            : 1;	//     5 - UNUSED
    unsigned bit4            : 1;	//     4 - UNUSED
    unsigned bit3            : 1;	//     3 - UNUSED
    unsigned bit2            : 1;	//     2 - UNUSED
    unsigned bit1            : 1;	//     1 - UNUSED
    unsigned bit0            : 1;	//     0 - UNUSED
} __attribute__((packed)) FLG_10;


extern volatile unsigned char LNKERR;	///< Link error on comms
extern volatile unsigned char ONLINE;	///< True if online

/**
	@brief Timeout flags.
*/
extern volatile struct _TIMEOUT
{
    unsigned bit7            : 1;		// BIT 7 - UNUSED
    unsigned bit6            : 1;		//     6 - UNUSED
    unsigned bit5            : 1;		//     5 - UNUSED
    unsigned bit4            : 1;		//     4 - UNUSED
    unsigned bit3            : 1;		//     3 - UNUSED
    unsigned unknownResp     : 1;		//     2 - Unknown Response
    unsigned commsError      : 1;		//     1 - Comms Error
    unsigned expired         : 1;		//     0 - Response timed out
} __attribute__((packed)) TIMEOUT;

extern	unsigned char	NEWCLK;  		// New clock update required
extern	unsigned char	DIALAQID;		// AQID for this dial connection
extern	unsigned char	JRNSEM;  		// Journal semaphore
extern	unsigned char	LOCKEY;  		// Local key press for keyalt
extern	unsigned char	COMSEM;  		// Comms ownership semaphore
extern	unsigned char	PKDIR;   		// Pack direction

extern	char			INPBUF[128];	// Input buffer
extern	DIALDATA 		D_INFO;			// Dial config information block
extern	char			INPKEY;			// Key input buffer
extern	char			INJKEY;			// Injected key
extern	unsigned char	BUZDAT;			// Beeper time data
extern	unsigned char	RTCBUF[14];		// RTC buffer

extern	unsigned char	CRDBUF1[CRDLEN];// Card read buffer 1
extern	unsigned short	CRDLEN1;		// Card read length 1
extern	unsigned char	CRDBUF2[CRDLEN];// Card read buffer 2
extern	unsigned short	CRDLEN2;		// Card read length 2

extern	unsigned char * TXSTART;		// Start of appl data to send
extern	unsigned char * TXEND;			// End of appl data to send

extern	unsigned char * NMSEND;		// Start of nms data to send
extern	unsigned char * NMSSTART;		// End of nms data to send

#define	S_SENDBUF	1024
extern	unsigned char	SENDBUF[S_SENDBUF];// Transmit buffer
extern	unsigned short	SENDLEN;		// Number of bytes to send

#define	S_RECVBUF	1024
extern	unsigned char	RECVBUF[S_RECVBUF];// Receive buffer
extern	unsigned short	RECVLEN;		// Number of bytes received

#define	S_NMSBUF	1024
extern	unsigned char	NMSBUF[S_NMSBUF];// Transmit buffer
extern	unsigned short	NMSLEN;			// Number of bytes received

extern	unsigned char	RXCOMM[1024];	// Receive buffer (comms module)
#define	RXCOME ( RXCOMM + sizeof RXCOMM )

extern  unsigned char	PRTBUF[23*33];	// Charaters/line * line/page (build buffer)
extern  unsigned char	PRINTB[23*33];	// Charaters/line * line/page (driver buffer)

#if defined( _MSC_VER )
#pragma pack()
#endif

extern  void SIM_FixdataInit( void );

#ifdef __cplusplus
}
#endif

#endif	// _FIXDATA_APP_H_
