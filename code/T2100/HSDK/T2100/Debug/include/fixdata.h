/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef FIXDATA_H
#define FIXDATA_H

//=============================================================================
// Module overview:
//	This file contains the C declarations for the items in fixdata.s
//	All items are represented here, but some are simply commented out copies
//	of the assembly language.  Some of these items are not available in C,
//	other items just have not been declared yet
//
//	This file is in the same order as fixdata.s.
//=============================================================================
#include "types.h"
#include "drivdef.inc"
#include "sysstruc.inc"
#include "equates.inc"
#include "execdef.inc"

// The fixdata_app.h file can be specified in the application's directory
// to override the one in the OS.
#include <fixdata_app.h>

//=============================================================================
// Public defines and typedefs
//=============================================================================



//=============================================================================
// Public data declarations
//=============================================================================

//	COMDATA EQU     $       ; Start of Common Data
//	FUNCTION DS     3       ; process link to Function handler
//	LPAGE_ADJUST DS	3		; Number of Boot Pages
//	LK_IOCS DS      3       ; IOCS linkage address
//	LK_PTCS DS      3       ; PTCS linkage address
//	NMIDRIV DS	3		    ; Indirect Jump to NMI Driver on Page 0
//	GETPC   DS      2       ; (code lives here)
//	KERNP   DS      1       ; Page Mask of Kernel Page
//	I_PORTA DS      1       ; PORT A0H IMAGE
//	I_PORTB DS      1       ; PORT B
//	I_PORTC DS      1       ; Port C0H IMAGE
//	I_PORT20			    ; Port 20 Image for ICE
//	I_PORTD DS      1       ; Port D
//	I_PORT80			    ; Port 80 Image for ICE
//	I_PORTE DS      1       ; Port E0H IMAGE
//	I_PORTF DS      1       ; PORT F
//	I_PORTG DS      1       ; PORT G
//	I_PIOBC	DS	1		    ; Image of PIOB Control Register
//	I_PIOAC	DS	1		    ; Image of PIOA Control Register

// extern	unsigned short	CPUSPEED_D16;	// Z80 CPU Clock Speed / 16 (in 100 Hz Units)
// extern	unsigned long	MS1_66_CNT;		// 1.66 ms interrupt counter
// extern	unsigned char	C_PAGE;			// Current Page
// extern	unsigned char	PROCPG;			// Process's current page
// extern	unsigned char	BOARD;			// Board type (ICE only)
// extern	unsigned char	FSM_ID;			// FSM board ID (ICE only)
// extern	unsigned char	SOFTKEYS;		// Which soft keys are pressed (T7 only)

// NMILNK  DS	14		; Upper Memory NMI Code
// XCALL_SP  DS	2		; Active Process' XCall Stack Pointer
// USTACK  DS      2               ; User stack point save
//         DS      32
// CSTACK  EQU     $               ; common area stack
//
// SIOA_5IMG   DS	1		; Image of SIOA Write Register 5
// SIOA_TXFLG  DS	1		; SIOA Transmit Control flags
// SIOA_RXFLG  DS	1		; SIOA Receive Control flags
// SIOA_TXDATA DS	SIO_TXDATASZE	; SIOA Transmit Data Transfer Buffer
// SIOA_RXDATA DS	SIO_RXDATASZE	; SIOA Receive Data Transfer Buffer
// SIOA_RXSTAT DS	SIO_RXDATASZE	; SIOA Receive Status Transfer Buffer
//
// SETPG   DS      3               ; Hardware Mask
// SELPAGE DS      3               ; Indirect Jump to relocated routine
extern	unsigned char GETC_PAGE_MASK( void );	// Get the current page mask
// GET_PSTATUS  DS	3		; Indirect Jump to relocated routine
//
// SIOB_1IMG   DS	1		; Image of SIOB Write Register 1
// SIOB_5IMG   DS	1		; Image of SIOB Write Register 5
// SIOB_TXFLG  DS	1		; SIOB Transmit Control flags
// SIOB_RXFLG  DS	1		; SIOB Receive Control flags
// SIOB_TXDATA DS	SIO_TXDATASZE	; SIOB Transmit Data Transfer Buffer
// SIOB_RXDATA DS	SIO_RXDATASZE	; SIOB Receive Data Transfer Buffer
// SIOB_RXSTAT DS	SIO_RXDATASZE	; SIOB Receive Status Transfer Buffer
//
// VECTOR	DS	256		; Interrupt Vector Table
// VSR_SIZE EQU	10		; Size of Vector Service Routine
// VSR_ERR	DS	VSR_SIZE	; Error Vector Service Routine
// VSR_SIOB  EQU	$	; SIOB Vector Service Routines
// VSR_SIOB_TBE DS	VSR_SIZE	; SIOB TBE Vector Service Routine
// VSR_SIOB_ESC DS	VSR_SIZE	; SIOB ESC Vector Service Routine
// VSR_SIOB_RCA DS	VSR_SIZE	; SIOB RCA Vector Service Routine
// VSR_SIOB_SRC DS	VSR_SIZE	; SIOB SRC Vector Service Routine
// VSR_SIOA  EQU	$	; SIOA Vector Service Routines
// VSR_SIOA_TBE DS	VSR_SIZE	; SIOA TBE Vector Service Routine
// VSR_SIOA_ESC DS	VSR_SIZE	; SIOA ESC Vector Service Routine
// VSR_SIOA_RCA DS	VSR_SIZE	; SIOA RCA Vector Service Routine
// VSR_SIOA_SRC DS	VSR_SIZE	; SIOA SRC Vector Service Routine
// VSR_CTC0     DS	VSR_SIZE	; CTC0 Vector Service Routine
// VSR_CTC1     DS	VSR_SIZE	; CTC1 Vector Service Routine
// VSR_CTC2     DS	VSR_SIZE	; CTC2 Vector Service Routine
// VSR_CTC3     DS	VSR_SIZE	; CTC3 Vector Service Routine
// VSR_PIOA     DS	VSR_SIZE	; PIOA Vector Service Routine
// VSR_PIOB     DS	VSR_SIZE	; PIOB Vector Service Routine
//
// VSR_SIOD  EQU	$	; SIOD Vector Service Routines
// VSR_SIOD_TBE DS	VSR_SIZE	; SIOD TBE Vector Service Routine
// VSR_SIOD_ESC DS	VSR_SIZE	; SIOD ESC Vector Service Routine
// VSR_SIOD_RCA DS	VSR_SIZE	; SIOD RCA Vector Service Routine
// VSR_SIOD_SRC DS	VSR_SIZE	; SIOD SRC Vector Service Routine
//
// VSR_SIOC  EQU	$	; SIOC Vector Service Routines
// VSR_SIOC_TBE DS	VSR_SIZE	; SIOC TBE Vector Service Routine
// VSR_SIOC_ESC DS	VSR_SIZE	; SIOC ESC Vector Service Routine
// VSR_SIOC_RCA DS	VSR_SIZE	; SIOC RCA Vector Service Routine
// VSR_SIOC_SRC DS	VSR_SIZE	; SIOC SRC Vector Service Routine
//
// VSR_CTC4     DS	VSR_SIZE	; CTC4 Vector Service Routine
// VSR_CTC5     DS	VSR_SIZE	; CTC5 Vector Service Routine
// VSR_CTC6     DS	VSR_SIZE	; CTC6 Vector Service Routine
// VSR_CTC7     DS	VSR_SIZE	; CTC7 Vector Service Routine
//
// VSR_PIOC     DS	VSR_SIZE	; PIOC Vector Service Routine
// VSR_PIOD     DS	VSR_SIZE	; PIOD Vector Service Routine
//
// NUM_VSRS  EQU	($-VSR_ERR)/VSR_SIZE	; Number of Vector Service Routines
//
// SERVICE	DS	7		; Vector Interrupt Start Routine in RAM
// EIRETI	DS	11		; Vector Interrupt Return Routine in RAM
// SET_VECTOR  DS	17		; Set Vector Page & Address Routine in RAM
//
// ---------------------------------------------------------------------------
// DEV_DCBTBL  DS	2*MAXDEVNUM	; Pointers to the Active Device's DCBs in
// S_DEV_DCBTBL EQU $-DEV_DCBTBL	; Size of the Device's DCB Pointer Table
// DCBAREA	   DS	MAXDCB*DCBSZE	; area for DCB's (Device Control Blocks)
// DCB_END    EQU	$		; End Address of DCBAREA
// S_DCBAREA  EQU	$-DCBAREA	; Size of DCBAREA
// DCB_FREE   DS	2		; Pointer to the Next Free DCB
// ------------------------------
// DEV_FUNCTION DS	120		; Device Driver Function Call Routine in
// DEVFUNC_AF  DS	2		; Register Save Area for DEV_FUNCTION
// DEVFUNC_DE  DS	2		;
// DEVFUNC_HL  DS	2		;
// DFUNC_EI   DS	1		; DEV_FUNCTION Enable Interrupts flag
// ------------------------------
// CALLHL_PAGE DS	15		; Swap Pages & Call HL Routine in Common RAM
// ---------------------------------------------------------------------------
// XRS2_CMDFLG  DS	1		; XRS2 Command Status flags
// XRS2_TXFLG  DS	1		; XRS2 Transmit Control flags
// XRS2_RXFLG  DS	1		; XRS2 Receive Control flags
// XRS2_TXDATA DS	XIL_TXDATASZE	; XRS2 Transmit Data Byte Transfer Buffer
// XRS2_TXCTL  DS	XIL_TXDATASZE	; XRS2 Transmit Control Byte Transfer Buffer
// XRS2_RXDATA DS	XIL_RXDATASZE	; XRS2 Receive Data Byte Transfer Buffer
// XRS2_RXSTAT DS	XIL_RXDATASZE	; XRS2 Receive Status Byte Transfer Buffer
// ------------------------------
// IOCSDONE DS     2               ; pointer to IOCS done list
// LNKCOND DS      1		; Host Communication Link Condition
// LOOPFLG DS      1               ; L2 loopback flags
// ---------------------------------------------------------------------------
// PRGAREA DS      1024            ; common RAM area programs
// ESTACK  EQU     $               ; TOP OF EXECUTIVE STACK
// ---------------------------------------------------------------------------
// P_STATFMT DS    1               ; Format Type
// P_STATUS  DS    PST_SIZE        ; Page Status Table
// P_CHECK   DS    1               ; LRC for table
// ; ---------------------------------------------------------------------------
// K_RAM   EQU     $

// ENAPPL declared in fixdata_app.h

// PDT     DS      MAXPCB*SIZEPDT  ; Process descriptor table
// NEWPAGE DS      1               ; new page for process
// P_SCHED DS      25              ; process switch routine
// PCBAREA DS      MAXPCB*SIZEPCB  ; PCB must be in common RAM !
// K_RAML  EQU     $-K_RAM

// ---------------------------------------------------------------------------
extern	char	NMSIDLL[2]; 
// "IL" - set by App to direct AppManager to perform program load
// "QS" - set by AppMngr to direct App to perform quik start
// "FS" - set by App to direct AppManager to perform quik (fast) start
// "AS" - set by AppMngr to direct App to perform Autosettlement 
// ---------------------------------------------------------------------------

// XRS3_CMDFLG  DS	1		; XRS3 Command Status flags
// XRS3_TXFLG  DS	1		; XRS3 Transmit Control flags
// XRS3_RXFLG  DS	1		; XRS3 Receive Control flags
// XRS3_TXDATA DS	XIL_TXDATASZE	; XRS3 Transmit Data Byte Transfer Buffer
// XRS3_TXCTL  DS	XIL_TXDATASZE	; XRS3 Transmit Control Byte Transfer Buffer
// XRS3_RXDATA DS	XIL_RXDATASZE	; XRS3 Receive Data Byte Transfer Buffer
// XRS3_RXSTAT DS	XIL_RXDATASZE	; XRS3 Receive Status Byte Transfer Buffer
// ---------------------------------------------------------------------------
extern	char	EPROM_VER[10];	// EPROM verion
// ; ---------------------------------------------------------------------------
extern	volatile unsigned char	TERMFMT;	// Formatted indicator
extern	volatile TERMREC 		TERM;
extern	volatile unsigned char	TERMLRC;	// LRC of data
//
extern	volatile unsigned char	NMSFMT;		// NMS data area formatted
extern	volatile NMSREC			NMSDAT;		// NMS data
// ---------------------------------------------------------------------------
// NBOOT_PAGES DS	1		; Number of Boot Pages
// NUMAPP_PAGES DS	1		; Number of Physical Pages of Lower Memory
// UPMEM_PAGE  DS	1		; Physical Page Number of Upper Memory Page
extern	unsigned char	LOAD_UPDATED;	// The Current Load has been Updated Flag
// ---------------------------------------------------------------------------
// SELPAGE_BC DS   2               ; REGISTER SAVE FOR SELPAGE
// SELPAGE_DE DS   2               ;
// SELPAGE_HL DS   2               ;
// ---------------------------------------------------------------------------
// USERRAM EQU     $

#ifdef MAKE_VIKING

extern	unsigned char	PADLOGONFMT;			// Formatted indicator
extern	BYTE			PADLOGON[200];			// Pad Logon Script
extern	unsigned char	PADLOGONLRC;			// LRC of data

extern	unsigned char	PPPModeFMT;				// Formatted indicator
extern	BYTE			PPPMode;				// PPP Mode
extern	unsigned char	PPPModeLRC;				// LRC of data

extern	unsigned char	OLD_LogonUsernameFMT;		// Formatted indicator
extern	BYTE			OLD_LogonUsername[16];		// Logon User Name
extern	unsigned char	OLD_LogonUsernameLRC;		// LRC of data

extern	unsigned char	OLD_LogonPasswordFMT;		// Formatted indicator
extern	BYTE			OLD_LogonPassword[16];		// Logon Password
extern	unsigned char	OLD_LogonPasswordLRC;		// LRC of data

extern	unsigned char	ModemATCommandFMT;	// Formatted indicator
extern	BYTE			ModemATCommand[64];	// Modem AT Command
extern	unsigned char	ModemATCommandLRC;		// LRC of data

extern	unsigned char	PIN_CODEFMT;			// Formatted indicator
extern	BYTE			PIN_CODE[3+8+1];		// Sim Card PIN code
extern	unsigned char	PIN_CODELRC;			// LRC of data

extern	unsigned char	LogonUsernameFMT;		// Formatted indicator
extern	BYTE			LogonUsername[32];		// Logon User Name
extern	unsigned char	LogonUsernameLRC;		// LRC of data

extern	unsigned char	LogonPasswordFMT;		// Formatted indicator
extern	BYTE			LogonPassword[32];		// Logon Password
extern	unsigned char	LogonPasswordLRC;		// LRC of data

extern	unsigned char	DialBackupFMT;		// Formatted indicator
extern	BYTE			DBInitPrefferedService;	// Init preffered service
extern	BYTE			DBNmsPrefferedService;	// NMS preffered service
extern	UBYTE			DBInitTel[12];			// DB Init phone number
extern	UBYTE			DBNmsTel[12];		// DB NMS phone number
extern	UBYTE			DBModem;			// DB modem mode
extern	UBYTE			DBCon;				// DB connection time in seconds
extern	BYTE			DBPPPMode;			// DB PPP mode
extern	BYTE			DBModemATCommand[64];	// DB modem AT command
extern	BYTE			DBLogonUsername[32];	// DB logon username
extern	BYTE			DBLogonPassword[32];	// DB logon password
extern	unsigned char	DialBackupLRC;		// LRC of data

#endif // MAKE_VIKING

// The rest of the parameters are declared in fixdata_app.h

#endif	// _FIXDATA_H_
