//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _APPDATA_H_
#define _APPDATA_H_
//=============================================================================
//
// Module overview:             appdata.h
//      Common application data
//

//=============================================================================
//=============================================================================
#include "defines.h"
#include "sdl.h"
#include "img.h"
#include "perfutil.h"
#include "prtdef.h"
#include "scard.h"
#include "struct.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================

//  Global constants used in print handling.
#define S_HSTPRDATA	40			// Length of Additional Host Print Data


//=============================================================================
// Public data declarations
//=============================================================================
extern UBYTE INJECTKEY;			// Store the injected key

// Signals between user process and host process
extern UBYTE TRREQBLD;			// REQUEST BUILD FLAG
extern UBYTE TRRSPREADY;		// REPSONSE READY

extern struct trandata_rec TRINP;	// TRANSACTION DATA ENTRY
extern struct trandata_rec TRREQ;	// CURRENT REQUEST MESSAGE
extern struct trandata_rec TRRSP;	// CURRENT RESPONSE MESSAGE

extern char ResponseText[S_RSPTXT + 1];	// Description of response from host

extern struct event_entry CurrentEvent;
extern struct event_entry SecondEvent;	// For FindTran().
extern struct event_entry HostEvent;	// For HostFindTran().

extern UWORD Trk1ReadLength, Trk2ReadLength;

extern short hdttext_table_id;	// default table id for dynamic ML support
extern short pintext_table_id;	// pin table id for dynamic ML support
extern short rsptext_table_id;	// response table id for dynamic ML support
extern short chktext_table_id;	// check table id for dynamic ML support
extern short chkmode_table_id;	// check mode table id for dynamic ML support
extern short evttext_table_id;	// Event table id for dynamic ML support
extern short pcltext_tableDef_id;	 	// PCL table id for dynamic ML support. Default txt table 
extern short pcltext_tableExt_id;	 	// PCL table id for dynamic ML support. Extended txt table
extern short TxtTableId;       			// Text Table to process

// enlarged the size of Dspbuf to handle 4th line text of receipt header

extern char Dspbuf[MAX_MSG_LEN + 1];	// Global prompt buffer
extern char Dspbuf2[MAX_MSG_LEN + 2];	// InputBox() display buffer
extern char GlobalDsp;			// when set to 1, display prompt is stored in CSTMSG

extern OS_RETURN_VALS HostOSRetval;	// Host OS return value
extern UBYTE RSPCODE[2];		// Host Response Code
extern UBYTE RSPOPT;			// Control Bits (Byte 1)
extern UBYTE RSPOPT2;			// Control Bits (Byte 2)

extern char CSTMSG[MAX_MSG_LEN + 1];	// Custom Message holder
extern char CSTMSG2[MAX_MSG_LEN + 1];	// Custom Message 2nd line holder
extern char CSTMSG3[MAX_MSG_LEN + 1];	// Custom Message 3rd line holder
extern char CSTMSG4[MAX_MSG_LEN + 1];	// Custom Message 4th line holder
extern char CSTMSG5[MAX_MSG_LEN + 1];	// Custom Message 5th line holder
extern char CSTMSG6[MAX_MSG_LEN + 1];	// Custom Message 6th line holder
extern char CSTMSG7[MAX_MSG_LEN + 1];	// Custom Message 7th line holder
extern char CSTMSG8[MAX_MSG_LEN + 1];	// Custom Message 8th line holder

extern UBYTE DelayIdleDisplay;	// Delay idle screen update.

extern UBYTE MsgComFlag;

extern UWORD SendMsgLen;
extern UWORD RecvMsgLen;

extern UBYTE *pSendBuffer;		// Pointer into buffer for sending message

extern struct jrnfind_rec JSREC;	// Journal select record

extern UBYTE THIS[];			//  Work area for the journal code

extern struct totals_rec TERMTOTALS;	// Terminal Totals
extern struct stdtot_rec STDTOTALS;	// Standard totals

extern UWORD JrnHostIdent;		// Set by journal cross page routines
extern int JrnRoomCount;		// Count of number of maximum sized
										// journal records that could still fit
extern UBYTE TRAININV[3];		// Training mode invoice number

extern UBYTE PRTSTAT;			// Printer logical status byte
extern UBYTE PROPT;				// Print Options

extern UBYTE VERSION[13];		// Highest Version
extern UBYTE RPTOPTS;			// Report Options


extern unsigned char *Pbufptr;			// Pointer to PRTBUF
extern UBYTE MaxCols;
extern Bool RollPrinter;
extern Bool ForceLF;
extern Bool GraphicsPrinter;
extern Bool GraphicsEnabled;
extern char HSTPRDATA[S_HSTPRDATA];	// Additional Host Print Data
extern UBYTE PRT_RTCbuff[RTCLEN];	// RTC Read Buffer

// Pinpad status byte.  
extern UBYTE PINSTAT;
extern UBYTE PIN_TYPE;

// Iostatus structure for pin pad reads.
extern IOSTATUS PinCardReadStatus;

extern struct trandata_rec JRREC;	// journal record
extern UWORD ButtonId;			// Button id pressed
extern UWORD SDLScreenId;		// common parmeter used by DrawScreen

extern char Titlebuf[SCR_CHAR_WIDTH + 1];	// Buffer for screen title
extern char DefaultTitle[SCR_CHAR_WIDTH + 1];	// Usually contains "ISSUER   TRANS"

extern UBYTE backlight_timer;	// timer for backlight on-time delay

// INPUT DEVICE GLOBALS 
extern IOSTATUS KeyStatus, Trk1Status, Trk2Status, PadStatus;
extern UWORD DevFlag;			// see DEV_xxx in defines.h
extern UBYTE KeyVal;
extern UBYTE KeyBuffer[2];

// Global paramater for ChkPrinter
#define NoErrDisp 0x00
#define ErrNotEnabled 0x01
#define ErrNotReady 0x02

// PRFLAG is global so it can be tested in tcf loop.    
extern UBYTE PRFLAG;			// Print Flags

// Global paramater for Printer Errors 
extern UBYTE PrinterErrorFlags;
#define PrinterOK 0x00
#define PaperOutError 0x01
#define PrinterOverheatError 0x02
#define OtherPrintError 0x03

// Global IcePak parameters - IcePak_Init must be called
// to properly initialize these
extern UBYTE IcePak_StartDelay;
extern UBYTE IcePak_IntervalDelay;
extern UBYTE IcePak_Active;

// Global parameter to indicate Issuer supported Transactions,
// Debit, i.e.at least one Issuer supports the Transaction

extern UBYTE IssuerTrans;
#define TRANS_DEBIT		0x01	// Debit Transaction
#define TRANS_DEBIT_REF 0x02	// Debit Issuer allows refunds?
#define TRANS_CHKSEL    0x10	// Check
#define TRANS_ACHCHECK  0x20	// ACH_CHECK, ACH_CHECK_REF, PRIOR_ACHCHECK
#define TRANS_ACHPRIOR  0x40	// PRIOR_ACHCHECK
#define TRANS_ACHREF    0x80	// ACH_CHECK_REF

// Set when predial is started. Reset on flag to host.  If still set    
// when back at transaction idle then must hang up the line.            
extern Bool PreDialAbort;

// CText Pointers for buttons custom labels
extern char *CtxtPtr[8];

// Used to pass parameters to EntryScreen
struct ESParms_str
{
	char DspMode;
	char MaxLen;
	char MinLen;
	UBYTE Alpha;
	enum msg_id TitleMid;
	enum msg_id Line1Mid;
	enum msg_id Line2Mid;
};

// Pointer to Field Definition
extern struct FLDDEF FieldDef;

// Typedef enum - Extended print capability
extern CURRENTELEM currElement; // Current field processed
extern UBYTE vFieldNum;         //  curent Field
 
// Application Level Auto Answer Flag 0-OFF 1-Initilized 2-Processing
extern UBYTE AutoAnswerFlg;

// AVS Response Support
extern UBYTE AVSERRFLG;

// HDT  Screen Definition Language Data
extern UBYTE *pTableStart;
extern PAGE_NUMBERS TablePage;
extern UWORD TableItemCount;
extern enum transactions User_Events[NUM_OF_USER_EVENTS];
extern enum msg_id User_Msgs[NUM_OF_USER_MESSAGES];
extern IMAGE_ID User_Imgs[NUM_OF_USER_IMAGES];
extern UBYTE Enable_Flags[NUM_OF_ENABLE_FLAGS];
extern SDL_PARMS SDLparms;		// used to pass data for SDL functions

extern UBYTE FlagHostCntl;		// set as required to preprint and allow cancel.
#define FLAG_ALLOW_CANCEL 	0x01	// Allow user to cancel.
#define FLAG_PREPRINT		0x02	// Preprint the receipt.

extern int screen_limit;		// Max number of characters to display on a line

extern OS_TIMES UserTimeout;	// Timeout for user input

extern Bool bFileInit;			// Flag to indicate performing initialization from database
extern Bool AutoInit;			// Flag to indicate performing automatic initialization

extern Bool ShowDialStatus;		// Flag to show dialing connection status

extern UBYTE MLSemaphore;		// Semaphore to control access to MultiLingual table.

extern char MLBuffer[MAX_MSG_LEN + 1];	// Global buffer to return message from MultiLingual table.

extern UBYTE DialTranKey;		// Transaction being dialed for

extern PERFORMRPT Data_kind;          // Performance Report. Data Kind to increment (variable type enum).

extern UBYTE AqSem;						/* AQTAB access semaphore*/
extern void * gpAqPtr;					/* Global pointer used by AQTAB access functions */
extern UBYTE aqByte;					/* Global byte value used by AQTAB access functions */

extern UBYTE CRSem;						/* CRTAB access semaphore*/
extern void * gpCRPtr;					/* Global pointer used by CRTAB access functions */
extern UBYTE CRByte;					/* Global byte value used by CRTAB access functions */

extern UBYTE IssSem;					/* ISTAB access semaphore*/
extern void * gpIssPtr;					/* Global pointer used by ISTAB access functions */
extern UBYTE issByte;					/* Global byte value used by ISTAB access functions */

extern UBYTE *pSFData;				// Subfield data start pointer
extern UBYTE *pB60DAddress;			// Pointer to destination buffer
extern UWORD MoveSize;				// Number of bytes to move
extern UWORD B60DBytesLeft;			// Bytes left in destination buffer

extern Bool OnlineCancelled;			/* online transaction cancelled by user */



extern UBYTE	PIN_DEVICE;				/* PIN device type */
extern void*	PIN_CONFIG;				/* Pointer to PIN device configuration parameters */

#ifdef MAKE_EMV
extern UBYTE ScriptLen;			// Length of received issuer script
extern UBYTE IssScriptBuf[128];	// Temporary storage for Issuer Scripts
extern UBYTE EMVVersion;		// Operating mode of EMV Library
extern Bool PrintEMVDebug; // EMV debug printing control flag;
extern Bool ChipCardInserted;	// Indicates card inserted when txn started by button press
extern UBYTE Atmel_ATR[12];		// Atmel version data
extern SCRDDEVICE CryptSlot;	// Atmel crypto processor's slot

//Application Capabilities
extern UBYTE LocalAppCap[3];
//first byte
#define APP_CAP1_CARDHOLDER_CONF 0x80    // Enable Transaction Log
#define APP_CAP1_MULTI_LANG      0x40    // Disable Exception file flag
#define APP_CAP1_PED_PRESENT     0x20    // Disable merch force online capability
#define APP_CAP1_PIN_BYPASS      0x10    // Disable merch force accept after fail capability
#define APP_CAP1_AMT_PIN_SAME    0x08    // Enable Transaction Log
//second byte
#define APP_CAP2_TXN_LOG         0x80    // Enable Transaction Log
#define APP_CAP2_EXC_FILE_TEST   0x40    // Disable Exception file flag
#define APP_CAP2_MERCH_SUSP      0x20    // Disable merch force online capability
#define APP_CAP2_EMV_FORCED      0x10    // Disable merch force accept after fail capability
//third byte
#define APP_CAP3_EMV_ADVICES     0x80    // Enable Transaction Log
#define APP_CAP3_ISS_REFER       0x40    // Disable Exception file flag
#define APP_CAP3_CARD_REFER      0x20    // Disable merch force online capability

// Bad Key flag used to report to user
extern Bool BadKeyInKeystore;
extern Bool BadKeyReported;

extern UBYTE	ScrChkStat;	// Smart Card Reader Check State flags
#define	ScrIdleState	0x80
#define ScrSendState	0x40
#define ScrRecvState	0x20
#define ScrTimerState	0x10
#define ScrTimeOutState	0x01

#endif // MAKE_EMV

extern UBYTE  MenuItemCounter;
extern UBYTE  Number_Of_Applications;
extern Bool ForcedOnline;	// Force online flag

extern UBYTE  FontSize;	// Display font size

//!  Device ID's used in this application
enum DeviceIds
{
	DEVICE_ID_USER_INTERFACE = 15,  // start with 15, just do not use zero
	DEVICE_ID_CARD_READER,
	DEVICE_ID_PIN_PAD,
	DEVICE_ID_PRINTER,
	DEVICE_ID_MODEM,
	DEVICE_ID_HOST_COMM,
	DEVICE_ID_HOST_INTERFACE,
	DEVICE_ID_SERIAL_PORT,
	DEVICE_ID_CHECK_READER,
	DEVICE_ID_SMART_CARD_READER,
	DEVICE_ID_NOT_USED
}; 


#define SELECT_TRX_HOST_INFO        0x01
#define SELECT_SETTLE_HOST_INFO     0x02
#define SELECT_PRIMARY_INFO         0x04
#define SELECT_SECONDARY_INFO       0x08

extern UBYTE SetTMParameterVariables;

//=============================================================================
// Public function declarations
//=============================================================================


#endif // _APPDATA_H_
