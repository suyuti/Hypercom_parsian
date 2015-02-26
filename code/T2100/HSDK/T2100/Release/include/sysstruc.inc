/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef SYSSTRUC_INC
#define SYSSTRUC_INC

#include <types.h>

#ifdef DOXYGEN
#define __attribute__(A)
#endif

/**
	Cypher region information structure.
*/
typedef struct
{
	UBYTE CypPage;		// Page where Cypher Region Resides
	void *CypAddr;		// Address of the Cypher Region
	UWORD CypSize;		// Maximum Size of Cypher Region
} __attribute__((packed)) CYPDEF;

/* DEFINE STRUCTURE SIZE */
#define CYPDEFSZE sizeof( CYPDEF )   

//
//       TERMINAL LOCAL CONFIGURATION PARAMETERS
//
//       OPTION BYTE 1
//
#define TM1_TSEREDIT	0x01		// 00000001B  TERMINAL SERIAL NUMBER EDIT ALLOWED
#define TM1_NACDIAL	0x02		// 00000010B  TRANSMIT DIAL COMMAND TO NAC
#define TM1_TSER8	0x04		// 00000100B  8 DIGIT TERMINAL SERIAL NUMBER
#define TM1_THID12	0x08		// 00001000B  SEND 12 DIGIT HARDWARE ID
#define TM1_PWDOPT	0x10		// 00010000B  PROMPT FOR PASSWORD
#define TM1_MANIP       0x20            // 00100000B  ENABLE MANUAL IP CONFIGURATION
#define TM1_LAN_MASTER	0x40		// 01000000B  ENABLE MASTER LAN MODE WHEN 
					//            TERMLINE=TMLINE_LAN
#ifdef MAKE_VIKING
#define TM1_HIDE_LOGO	0x80 // Hide Optimum logo during startup.
#endif // MAKE_VIKING
//
//       OPTION BYTE 2
//
#define TM2_CLONE	0x01	// Enable Boot EPROM Clone Function (T7 Only)
#define TM2_NOLOAD      0x01    // Disable autoload (T6000 Only)
#define TM2_DIALTW	0x02	// Enable/disable blind dial
#define TM2_SPAIN  	0x04	// Spain dial procedure (T7 Only)
#define TM2_FRANCE	0x08	// France dial procedure (T7 Only)
#define TM2_THRM6	0x10	// Use threshold values set 6dB lower for modem
#define TM2_9600FB	0x20	// Use performance based fall back for 9600bps modes
#define TM2_9600NF	0x40	// Use no SW fix for 9600bps modes

#ifdef MAKE_VIKING
//
//       OPTION BYTE 3
//
#define TM3_HWTYPE_MASK 0x0f // Hardware type mask
#define TM3_V22 0x00 // V22 analog modem
#define TM3_GSM_OLD 0x01 // GSM modem with old hardware
#define TM3_V90 0x02 // V90 analog modem
#define TM3_ETHERNET 0x03 // Ethernet module
#define TM3_LAN 0x04 // POS LAN
#define TM3_BLUETOOTH 0x05 // Bluetooth
#define TM3_GSM 0x06 // GSM modem
#define TM3_ISDN 0x07 // ISDN modem
#define TM3_DIALBACKUP 0x08 // GSM/V90 dial backup
#define TM3_HEXLENGTH 0x10 // Insert 2 bytes hex length in header
#define TM3_HEMSTRANSP 0x20 // HEMS Transparent (set) or Reliable (reset) mode
#define TM3_SSL  0x40 // Perform SSL server authentication in application
#define TM3_SIMPIN  0x80 // GSM SIM card PIN indicator

#endif // MAKE_VIKING

//
//       OPTION BYTE 4
//
#define TM4_TRK123	0x01		// 00000001B  Track 1-2-3 Capable
#define TM4_ICC		0x02		// 00000010B  Smart Card Reader Present
#define TM4_CTLMOD	0x04		// 00000100B  CONTACTLESS MODULE ATTACHED
#define TM4_CHKRD	0x08		// 00001000B  Check Reader attached
#define TM4_BACKLIGHT	0x10		// 00010000B  Backlight is usable

#define TM4_KEYBACKUP	0x40		// 01000000B  Enable KEYTABLE Backup (New Format)
#define TM4_BATLIM	0x80		// 10000000B  Battery Voltage Limits Selection flag
					//    0 = Lithium-ION Battery Voltage Limits
					//    1 = Lead Acid Battery Voltage Limits

#ifdef MAKE_VIKING
//
//       Equates for TERMREMOTEMODULE
//
#define TMR_HWTYPE_MASK 0x0f // Hardware type mask
#define TMR_V22 0x00 // V22 analog modem
#define TMR_GSM_OLD 0x01 // GSM modem with old hardware
#define TMR_V90 0x02 // V90 analog modem
#define TMR_ETHERNET 0x03 // Ethernet module
#define TMR_LAN 0x04 // POS LAN
#define TMR_BLUETOOTH 0x05 // Bluetooth
#define TMR_GSM 0x06 // GSM modem
#define TMR_ISDN 0x07 // ISDN modem

#endif // MAKE_VIKING

//
//
//        EQUATES FOR TERMPRINT
//
#define TMP_NONE	0x00            // FTP7x with External Printer
#define TMP_T7P		0x01            // FTP10 with Internal Epson 192

// The following equates map directly onto the printer defintion table
// in struct.inc :
#define TMP_E190	0x05            // FTP10 with Internal Epson 190
#define TMP_P8F		0x06            // FTP20 with Internal P8F (Star 212)
#define TMP_P8S		0x07            // FTP20 with Internal P8S (Star 221)
#define TMP_P8GF	0x08            // FTP20 with Internal P8GF(Star 212G)
#define TMP_P8GS	0x09            // FTP20 with Internal P8GS(Star 221G)
#define TMP_MT101	0x0A            // FTP10 with Internal Epson MT101 Thermal
#define TMP_MT301	0x0B            // FTP20 with Internal Epson MT301 Thermal
#define TMP_EP190	0x0C		// ICE5000 Pansonic EPLZ190 Thermal &
					// ICE5000 Pansonic EPL1920 Thermal
					// with Cutter
#define TMP_EP205       0x0D            // ICE4000 Pansonic EPL205 Thermal
#define TMP_FT628	0x0E		// ICE5500 Plus with FTP-628 Thermal
					// without Cutter (NO Cutter)
//
//
//       EQUATES FOR TERMDMODE
//
#define TMDM_TONE	0x00		// TONE (DTMF) DIALING
#define TMDM_PULSE	0x01		// PULSE DIALING
#define TMDM_CELL	0x02		// AMPS CELL DIALING
#define TMDM_CDPD_TCP	0x03		// CDPD TCP/IP DIALING
#define TMDM_CDPD_UDP	0x04		// CDPD UDP DIALING
#define TMDM_GSM	0x05		// GSM NON TRANSPARENT
#define TMDM_GSM_TRANS	0x06		// GSM TRANSPARENT 
#define TMDM_RAD_MPAK	0x07		// MOBITEX RAP

#ifdef MAKE_VIKING

#define TMDM_V110_9600	0x08		// ISDN V.110 9600  bps
#define TMDM_V110_19200	0x09		// ISDN V.110 19200 bps
#define TMDM_V120	0x0A		// ISDN V.120
#define TMDM_X75	0x0B		// ISDN X.75
#define TMDM_X25	0x0C		// ISDN X.25
#define TMDM_USERATD	0x0D		// ISDN User mode (only ATD command)
#define TMDM_USERSM     0x0E		// ISDN User mode (used with Send & Match strings, has not any ATD commands) 

#endif // MAKE_VIKING

//
//
//       EQUATES FOR TERMDIALDEV
//
#define TMDD_NONE	0x00		// NO INTERNAL DEVICE (NORMAL LINE ONLY)
//
#define TMDD_CELL_CRFM	0x01		// STANDARD COMMUNICATIONS CELL BOARD
#define TMDD_CDPD_D200	0x02		// CMI MC-DART200 CDPD
#define TMDD_CDPD_D300	0x03		// CMI MC-DART300 CDPD
#define TMDD_RAD_RIM900	0x04		// RIM900 RADIO MODEM
#define TMDD_RAD_MOT505	0x05		// MOTOROLA 505SD
#define TMDD_RAD_RIM801	0x06		// RIM801
#define TMDD_NOVA_6812	0x07		// NOVATEL 6812 CDPD
#define TMDD_RAD_RIM902	0x08		// RIM902 RADIO MODEM
#define TMDD_NOVA_6832	0x09		// NOVATEL 6832 CDPD MODEM
#define TMDD_TR1000	0x0A		// 900 Mhz. RFM TR1000
#define TMDD_FALCOM	0x0B		// FALCOM GSM MODEM

//! Leased line (TERMLine).
#define TMLINE_LEASE	0x00
//! Dial line (TERMLine).
#define TMLINE_DIAL	0x01
//! 4800 Baud POS LAN line (TERMLine).
#define TMLINE_LAN	0x02
//! 19200 Baud POS LAN line (TERMLine).
#define TMLINE_HSPDLAN	0x03
//! Ethernet line (TERMLine).
#define TMLINE_ETHERNET	0x04

#ifdef MAKE_VIKING

//! Journal not empty flag (TERMJrnStatus).
#define TMJRN_NOT_EMPTY 0x01

//! Autoerase master keys on application reload (TERMMKAutoErase).
#define TM_MK_AUTO_ERASE 0x77

#endif // MAKE_VIKING

/**
	Define the TERM table structure.
*/
typedef struct
{
        UBYTE TERMSer[8];               // TERMINAL SERIAL NUMBER
        UBYTE TERMChk;                  // SERIAL NUMBER CHECK DIGIT
	UBYTE TERMInitTel[12];  	// TERMINAL INIT TEL NO
	UBYTE TERMNmsTel[12];   	// TERMINAL NMS TELEPHONE NUMBER
        UBYTE TERMDMode;                // TERMINAL DIAL MODE 0-DTMF 1-PULSE
        UBYTE TERMPabx[4];              // TERMINAL PABX CODE
        UBYTE TERMTOut;                 // TIMEOUT FOR INITIALIZE MESSAGE
        UBYTE TERMPwd[4];               // DEFAULT PASSWORD
        UBYTE TERMLine;                 // LINE MODE
        UBYTE TERMCuAd;                 // CU ADDRESS
        UBYTE TERMOpt1;                 // OPTIONS 1
        UBYTE TERMOpt2;                 // OPTIONS 2
        UBYTE TERMOpt3;                 // OPTIONS 3
        UBYTE TERMOpt4;                 // OPTIONS 4
        UBYTE TERMNII[2];               // NII FOR INITIALIZE
        UBYTE TERMModem;                // 0-BELL 1-CCITT
        UBYTE TERMPDRatio;              // PULSE DIALING RATIO - 0-60/40 1-63/67
        UBYTE TERMPDMode;               // PULSE DIAL MODE - 0-NORMAL 1-NEW ZEALAND
        UBYTE TERMDTMFS;                // DTMF DIALING SPEED 0-SLOW 1-NORM 2-FAST
        UBYTE TERMDialTw;               // DIAL TONE  TIMER (BCD 100 MS)
        UBYTE TERMPauseW;               // DIAL PAUSE TIMER (BCD 100 MS)
        UBYTE TERMStan[3];              // SYSTEMS TRACE AUDIT NUMBER
        UBYTE TERMInv[3];               // TERMINAL INVOICE NUMBER
        UBYTE TERMHookW;                // DELAY AFTER LOOP BEFORE BUSY CHECK 100MS
        UBYTE TERMHold[2];              // HOLD LINE BEFORE DISONNECT 100MS
        UBYTE TERMCon;                  // CONNECTION TIME IN SECONDS
        UBYTE TERMDiscW;                // MINIMUM LINE DISCONNECT (100MS)
        UBYTE TERMMerch[23];            // MERCHANT NAME
        UBYTE TERMInit;                 // INIT DONE FLAG
        UBYTE TERMContrast;             // LED CONTRAST
        UBYTE TERMLineWait[2];          // WAIT FOR LINE BUSY 100MS
        UBYTE TERMLang;                 // LANGUAGE - SEE DEFINITIONS ABOVE
        UBYTE TERMPrint;                // PRINTER TYPE - SEE DEFINITIONS
        UBYTE TERMHID[8];               // HARDWARE ID NUMBER (S/N)
        UBYTE TERMTermPwd[8];           // TERMINAL VALIDATION PASSWORD
        UBYTE TERMTNmsPwd[8];           // TNMS VALIDATION PASSWORD
        UBYTE TERMTxLev;                // MODEM TRANSMIT LEVEL
        UBYTE TERMFTPxx;                // MAIN BOARD TYPE (FTP10,20,21,74,etc)
        UBYTE TERMFTPBuild;             // MAIN BOARD BUILD VERSION (-001,-002,etc)
        UBYTE TERMFTPRev;               // MAIN BOARD REVISION (A,B,C,etc)
        UBYTE TERMDialDev;              // INTERNAL DIAL DEVICE (CELL,CDPD,RIM,etc)
        UBYTE TERMHIDPre[4];            // HARDWARE ID NUMBER PREFIX
	UBYTE TERMPADXLo[2];		// PAD Device X-Axis Minimum
					// Calibration Value
	UBYTE TERMPADXHi[2];		// PAD Device X-Axis Maximum
					// Calibration Value
	UBYTE TERMPADYLo[2];		// PAD Device Y-Axis Minimum
					// Calibration Value
	UBYTE TERMPADYHi[2];		// PAD Device Y-Axis Maximum
					// Calibration Value
	UBYTE TERMDTMFIDW[1];		// INTERDIGIT PAUSE
	UBYTE TERMRINGMIN[2];		// MINIMUM RING DURATION (10MS UNITS)
	UBYTE TERMRINGMAX[2];		// MAXIMUM RING DURATION (10MS UNITS)
	UBYTE TERMPTRY[1];		// REDIAL ATTEMPTS
	UBYTE TERMSTRY[1];		// SECONDARY REDIAL ATTEMPTS
        UBYTE TERMDTMFLEV[1];		// MODEM DTMF LEVEL
	UBYTE TERMDTONEDRT[1];		// DIAL TONE DURATION TIME(100ms UNITS)
	UBYTE TERMDTONEINT[1];		// DIAL TONE INTERRUPTION TIME(1.66ms UNITS)
	UBYTE TERMDIALTSW[1];		// DIAL TONE START WAIT TIME
	CYPDEF TERMCypher;		// CYPHER REGION INFORMATION
	UBYTE TERMSECUREITOPT;		// Secure-It Application Option Bits
				  	// (WARNING: these Option Bits are reserved
					//	     for the exclusive use of the
					//	     Secure-It Application)



	// 32 Bytes Total
	IPADDRESS TERMIPAddress; 	// Terminal's Local IP Address
					// (4 bytes stored MSByte first)
	IPADDRESS TERMSubnetIP;  	// Terminal's Subnet IP Address
					// (4 bytes stored MSByte first)
	IPADDRESS TERMGatewayIP; 	// Terminal's Gateway IP Address
					// (4 bytes stored MSByte first)

	IPADDRESS TERMNmsIP;	 	// NMS Destination IP Address
					// (4 bytes stored MSByte first)
					// (Used for Program Loads)
	UWORD     TERMNmsPort;   	// NMS Destination Port Number
					// (2 bytes stored MSByte first)
					// (Used for Program Loads)

	IPADDRESS TERMInitIP;	 	// Init Destination IP Address
					// (4 bytes stored MSByte first)
	UWORD     TERMInitPort;  	// Init Destination Port Number
	IPADDRESS TERMDNS1IP;	 	// Init DNS1 IP Address
	IPADDRESS TERMDNS2IP;	 	// Init DNS2 IP Address

	CYPDEF TERMCypher_BUP;		// CYPHER BACKUP REGION INFORMATION

#if defined(MAKE_VIKING)

	UBYTE TERMJrnStatus;		// Jounal Status Notification from Application to AM

        UBYTE TERMRemoteModule;		// Remote module type

        UBYTE TERMCountry;		// Country code

	UBYTE TERMMKAutoErase;		// Master Key auto erase on loading application

        UBYTE TERMModemPatch;		// Modem patch ID

        UBYTE TERMSpare[58];		// PAD TO 100H-1

#else // if defined(MAKE_OPTIMUM)
        UBYTE TERMSpare[63];		// PAD TO 100H-1
#endif // MAKE_VIKING
} __attribute__((packed)) TERMREC;

/* DEFINE STRUCTURE SIZE */
#define SZTERM sizeof( TERMREC )   

/**
	EEPROM DATA STRUCTURE.
*/
typedef struct
{
   UBYTE EEFmt;                 // FORMATTED INDICATOR
   UBYTE EEData[128-2];         // REST OF DATA
   UBYTE EELrc;                 // LRC OF DATA
} EESTRUCT;

// Size of EEPROM data
#define EESZE sizeof( EESTRUCT )  

/*
;
;------------------------------------------------------------------
;------------------------------------------------------------------
;
;	The following section defines Symbols to be used for the
;	2 Byte NMSIDLL Variable.
;	The NMSIDLL Variable is used by the Terminal Applications
;	to tell the Boot Application to perform a Program Load,
;	FsatLoad, etc...
;
;	NMSIDLL+0 & NMSIDLL+1 are used in Combination to issue commands
;	to the Boot Application.
;	    "IL" - Initialize Load (Perform Program Load)
;	    "IB" - Initialize Boot (Perform Secure FastLoad Authentication)
;
;	    "E0" - Perform PinPad FastLoad @   9600 Baud
;	    "E1" - Perform PinPad FastLoad @  19200 Baud
;	    "E2" - Perform PinPad FastLoad @  38400 Baud
;	    "E3" - Perform PinPad FastLoad @  57600 Baud
;	    "E4" - Perform PinPad FastLoad @ 115200 Baud
;	    "E5" - Perform PinPad FastLoad @ 153600 Baud
;
;	    "F0" - Perform RS232-1 FastLoad @   9600 Baud
;	    "F1" - Perform RS232-1 FastLoad @  19200 Baud
;	    "F2" - Perform RS232-1 FastLoad @  38400 Baud
;	    "F3" - Perform RS232-1 FastLoad @  57600 Baud
;	    "F4" - Perform RS232-1 FastLoad @ 115200 Baud
;	    "F5" - Perform RS232-1 FastLoad @ 153600 Baud
;
;           "G0" - Perform RS232-2 FastLoad @   9600 Baud
;           "G1" - Perform RS232-2 FastLoad @  19200 Baud
;           "G2" - Perform RS232-2 FastLoad @  38400 Baud
;           "G3" - Perform RS232-2 FastLoad @  57600 Baud
;           "G4" - Perform RS232-2 FastLoad @ 115200 Baud
;
;           "H0" - Perform RS232-3 FastLoad @  9600 Baud
;           "H1" - Perform RS232-3 FastLoad @ 19200 Baud
;           "H2" - Perform RS232-3 FastLoad @  38400 Baud
;           "H3" - Perform RS232-3 FastLoad @  57600 Baud
;           "H4" - Perform RS232-3 FastLoad @ 115200 Baud
;
;       Symbols for NMSIDLL+0
;
;
*/

#define	NMS_INITIALIZE	'I'		// Initialize Command
#define	NMS_FASTPINPAD	'E'		// PinPad  FastLoad Command
#define	NMS_FASTRS232_1	'F'		// RS232-1 FastLoad Command
#define NMS_FASTRS232_2 'G'             // RS232-2 FastLoad Command
#define NMS_FASTRS232_3 'H'             // RS232-3 FastLoad Command
//
//	Symbols for NMSIDLL+1
//
#define	NMS_PROGLOAD	'L'		// Initialize Program Load SubCommand
#define	NMS_FAST_AUTH	'B'		// Initialize Secure FastLoad Authentication
					// SubCommand
#define	NMS_9600	'0'		// FastLoad @   9600 Baud SubCommand
#define	NMS_19200	'1'		// FastLoad @  19200 Baud SubCommand
#define	NMS_38400	'2'		// FastLoad @  38400 Baud SubCommand
#define	NMS_57600	'3'		// FastLoad @  57600 Baud SubCommand
#define	NMS_115200	'4'		// FastLoad @ 115200 Baud SubCommand
#define	NMS_153600	'5'		// FastLoad @ 153600 Baud SubCommand
//
/*------------------------------------------------------------------         */

/**
	NMS data area used by the system.
*/
typedef struct
{
        UBYTE NMSwName[7];              /* SOFTWARE NAME*/
        UBYTE NMSwRev[3];               /* SOFTWARE REVISION*/
        UBYTE NMStatus;                 /* TERMINAL STATUS*/
/*                                                                           */
/*       GENERAL STATISTICS                                                  */
/*                                                                           */
        UBYTE NMMin[2];                 /* MESSAGES IN*/
        UBYTE NMMOut[2];                /* MESSAGES OUT*/
        UBYTE NMTrans[2];               /* USER TRANSACTION COUNT*/
        UBYTE NMRedials[2];             /* REDIAL COUNTS*/
        UBYTE NMCErr[2];                /* COMMS ERRORS*/
        UBYTE NMTOut[2];                /* TRANSACTION TIMEOUTS*/
        UBYTE NMRTOut[2];               /* TIMEOUTS DUE TO REVERSAL PENDING*/
/*                                                                           */
/*       SDLC LEVEL 2 STATISTICS                                             */
/*                                                                           */
        UBYTE NMReTxC[2];               /* RETRANSMITS COUNT*/
        UBYTE NMRxErr[2];               /* RECEIVE FRAMING ERRORS*/
        UBYTE NMSnrIn[2];               /* SNRMS IN*/
        UBYTE NMSnrOut[2];              /* SNRMS OUT*/
        UBYTE NMRnrIn[2];               /* RNRS IN*/
        UBYTE NMRnrOut[2];              /* RNRS OUT*/
        UBYTE NMTstIn[2];               /* TESTS IN*/
        UBYTE NMTstOut[2];              /* TESTS OUT*/
        UBYTE NMDmIn[2];                /* DMS IN*/
        UBYTE NMDmOut[2];               /* DMS OUT*/
        UBYTE NMUaIn[2];                /* UAS IN*/
        UBYTE NMUaOut[2];               /* UAS OUT*/
        UBYTE NMFrmIn[2];               /* FRMR IN*/
        UBYTE NMFrmOut[2];              /* FRMR OUT*/
/*                                                                           */
/*       RESPONSE TIME MEASUREMENTS                                          */
/*                                                                           */
        UBYTE NMRspCnt;                 /* RESPONSE TIME TABLE COUNT (44)*/
/*                                                                           */
        UBYTE NMRspB1[2];               /* RESPONSE TIME BOUNDARY 1  0080*100 MS*/
        UBYTE NMRspB2[2];               /*               BOUNDARY 2  0160*100 MS*/
        UBYTE NMRspB3[2];               /*               BOUNDARY 3  0450*100 MS*/
        UBYTE NMRspB4[2];               /*               BOUNDARY 4  0900*100 MS*/
/*                                                                           */
        UBYTE NMRspC1[2];               /* RESPONSE TIME COUNT 1*/
        UBYTE NMRspC2[2];               /*               COUNT 2*/
        UBYTE NMRspC3[2];               /*               COUNT 3*/
        UBYTE NMRspC4[2];               /*               COUNT 4*/
/*                                                                           */
/*       UPTIME/DOWNTIME                                                     */
/*                                                                           */
        UBYTE NMUpTime[3];              /* UPTIME IN MINUTES*/
        UBYTE NMDnTime[3];              /* DOWNTIME IN MINUTES*/
/*                                                                           */
/*       MORE GENERAL STATISTICS                                             */
/*                                                                           */
        UBYTE NMRestarts[2];            /* ERROR RECOVERY RESTARTS*/
        UBYTE NMDelErr[2];              /* DELIVERY ERRORS*/
        UBYTE NMMode;                   /* MODE OF OPERATION (E-EPROM, R-RAM)*/
        UBYTE NMAlarms;                 /* ALARM TYPE - MINOR ALARMS (N-NO, Y-YES)*/
/*                                                                           */
/*       APPLICATION COUNTS                                                  */
/*                                                                           */
        UBYTE NMPTran[2];               /* PRIMARY TRANSACTION COUNT*/
        UBYTE NMSTran[2];               /* SECONDARY TRANSACTION COUNT*/
        UBYTE NMPRedial[2];             /* PRIMARY REDIAL COUNT*/
        UBYTE NMSRedial[2];             /* SECONDARY REDIAL COUNT*/
        UBYTE NMCrdErr[2];              /* CARD READER ERRORS*/
        UBYTE NMHostNA[2];              /* HOST NOT AVAILABLE COUNT*/
        UBYTE NMOffl[3];                /* TIME IN OFFLINE MODE (MINUTES)*/
        UBYTE NMCrdReads[2];            /* TOTAL MSR READS*/
        UBYTE NMSpare1[2];              /* SPARE COUNTER*/
        UBYTE NMSpare2[2];              /* SPARE COUNTER*/
        UBYTE NMSpare3[2];              /* SPARE COUNTER*/
/*                                                                           */
/*       TERMINAL STATUS                                                     */
/*                                                                           */
        UBYTE NMEcrBR;                  /* ECR BAUD RATE*/
        UBYTE NMDOpt;                   /* TELEPHONE DIAL OPTIONS*/
        UBYTE NMSepIn[2];               /* MERCHANT PASSWORD*/
        UBYTE NMDualAmt;                /* AMOUNT DUAL ENTRY*/
        UBYTE NMOpOp;                   /* OPERATING OPTIONS*/
        UBYTE NMLocOp;                  /* LOCAL OPTIONS (NOT APPLICABLE TO PIP)*/
        UBYTE NMResv[2];                /* RESERVED*/
        UBYTE NMTSer[8];                /* TERMINAL SERIAL NUMBER*/
/*                                                                           */
/*       LAST NMS TRANSMIT                                                   */
/*                                                                           */
        UBYTE NMLast[6];                /* LAST NMS TRANSMIT (YYMMDDHHMMSS)*/
/*                                                                           */
} __attribute__((packed)) NMSREC;

/* DEFINE STRUCTURE SIZE */
#define NMSZE sizeof( NMSREC )    

#endif
