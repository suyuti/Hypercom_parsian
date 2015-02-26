//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             appdata.c
//      declaration of common application data
//
//=============================================================================

#include "basictyp.h"

#include "transdef.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "journal.h"
#include "pinpad.h"
#include "prtdef.h"


//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================


UBYTE INJECTKEY;				// Store the injected key
UWORD Trk1ReadLength, Trk2ReadLength;

// Signals between user process and host process
UBYTE TRREQBLD;					// REQUEST BUILD FLAG
UBYTE TRRSPREADY;				// REPSONSE READY

struct trandata_rec TRINP;		// TRANSACTION DATA ENTRY
struct trandata_rec TRREQ;		// CURRENT REQUEST MESSAGE
struct trandata_rec TRRSP;		// CURRENT RESPONSE MESSAGE

char ResponseText[S_RSPTXT + 1];	// Description of response from host

struct event_entry CurrentEvent;
struct event_entry SecondEvent;	// For FindTran().
struct event_entry HostEvent;	// For HostFindTran().

short hdttext_table_id;			// Default table id for dynamic ML support
short pintext_table_id;			// Pin table id for dynamic ML support
short rsptext_table_id;			// Response table id for dynamic ML support
short chktext_table_id;			// Check table id for dynamic ML support
short chkmode_table_id;			// Check mode table id for dynamic ML support
short evttext_table_id;			// Event table id for dynamic ML support
short pcltext_tableDef_id;	 	// PCL table id for dynamic ML support. Default txt table 
short pcltext_tableExt_id;	 	// PCL table id for dynamic ML support. Extended txt table

short TxtTableId;    			// Text Table to process

// enlarged the size of Dspbuf to handle 4th line text of receipt header

char Dspbuf[MAX_MSG_LEN + 1];	// Global prompt buffer
char Dspbuf2[MAX_MSG_LEN + 2];	// InputBox() display buffer
char GlobalDsp;					// when set to 2, ZAP key is turned on

OS_RETURN_VALS HostOSRetval;	// Host OS return value
UBYTE RSPCODE[2];				// Host Response Code
UBYTE RSPOPT;					// Control Bits (Byte 1)
UBYTE RSPOPT2;					// Control Bits (Byte 2)

char CSTMSG[MAX_MSG_LEN + 1];	// Custom Message holder
char CSTMSG2[MAX_MSG_LEN + 1];	// Custom Message 2nd line holder
char CSTMSG3[MAX_MSG_LEN + 1];	// Custom Message 3rd line holder
char CSTMSG4[MAX_MSG_LEN + 1];	// Custom Message 4th line holder
char CSTMSG5[MAX_MSG_LEN + 1];	// Custom Message 5th line holder
char CSTMSG6[MAX_MSG_LEN + 1];	// Custom Message 6th line holder
char CSTMSG7[MAX_MSG_LEN + 1];	// Custom Message 7th line holder
char CSTMSG8[MAX_MSG_LEN + 1];	// Custom Message 8th line holder

UBYTE DelayIdleDisplay;			// Delay idle screen update.

UWORD SendMsgLen;				// Length of Message to send
UWORD RecvMsgLen;				// Length of Message received
UBYTE *pSendBuffer;				// Pointer into buffer for sending message


UBYTE MsgComFlag;				// If bit 0 is set, REQ was built
							// If bit 1 is set, REQ was sent
							// If bit 2 is set, RSP was received
							// If bit 3 is set, RSP was unpacked

struct journal_rec JRBUF;		// For Header Record Building

// NOTE TO APPLICATION PROGRAMMER:  JRBUF is a structure consisting of the size of the link
// list controls and the size of trandata_rec. If trandata_rec is modified drastically so 
// there is no additional data after the part of trandata_rec that is written/read
// to/from the journal there could be data overruns.  
// If a developer modifies trandata_rec so there is not at least 34 additional bytes
// following "TRJRNEND" then additional area must be added to the journal_rec structure.  
// Since the amount of expansion area is controlled by the application, one possible fix is
// to add the following array at the end of the journal_rec structure with an adjustment to
// 34 if some area already existed:    UBYTE EXPDATA[34];
//
// The 34 bytes were not just added to JRBUF because in almost all cases there is sufficient
// data after TRJRNEND.  It is only if drastic changes are made to trandata_rec that the
// additional space would be necessary.

struct jrnfind_rec JSREC;		// Journal select record


UBYTE THIS[S_THIS];				// Work area for the journal code

struct totals_rec TERMTOTALS;	// Terminal Totals
struct stdtot_rec STDTOTALS;	// Standard totals
struct settletotals_rec SETTLETOTALS;	// Settle totals @ydeniz -- BC 

UWORD JrnHostIdent;				// Set by journal cross page routines
int JrnRoomCount;				// Count of number of maximum sized
							// journal records that could still fit

UBYTE TRAININV[3];				// Training mode invoice number

UBYTE PRTSTAT;					// Printer logical status byte
UBYTE PROPT;					// Print Options

UBYTE VERSION[13];				// Highest Version
UBYTE RPTOPTS;					// Report Options

//
// Global constants used in print handling. 


unsigned char *Pbufptr;
UBYTE MaxCols;
Bool RollPrinter;
Bool ForceLF;
Bool GraphicsPrinter;
Bool GraphicsEnabled;
char HSTPRDATA[S_HSTPRDATA];	// Additional Host Print Data
UBYTE PRT_RTCbuff[RTCLEN];		// RTC Read Buffer

struct trandata_rec JRREC;		// journal record

// Pinpad status byte.  
UBYTE PINSTAT;
UBYTE PIN_TYPE;

// Iostatus structure for pin pad reads.
IOSTATUS PinCardReadStatus;

UWORD ButtonId;					// Button id pressed
UWORD SDLScreenId;				// common parmeter used by DrawScreen

char Titlebuf[SCR_CHAR_WIDTH + 1];	// Buffer for screen title
char DefaultTitle[SCR_CHAR_WIDTH + 1];	// Usually contains "ISSUER   TRANS"

UBYTE backlight_timer;			// timer for back light, one count per second

//************ INPUT DEVICE GLOBALS ********************
IOSTATUS KeyStatus, Trk1Status, Trk2Status, PadStatus;
UWORD DevFlag;					// see DEV_xxx in defines.h
UBYTE KeyVal;
UBYTE KeyBuffer[2];

// PRFLAG is global so it can be tested in tcf loop.    
UBYTE PRFLAG;					// Print Flags

// Global paramater for Printer Errors 
UBYTE PrinterErrorFlags;

// Global IcePak parameters - IcePak_Init() must be called
// to properly initialize these
UBYTE IcePak_StartDelay;
UBYTE IcePak_IntervalDelay;
UBYTE IcePak_Active;

// Global parameter to indicate Issuer supported Transactions, 
// Debit and EBT (i.e. at least one Issuer supports the Transaction)
UBYTE IssuerTrans;

//*******************************************************

// Set when predial is started. Reset on flag to host.  If still set    
// when back at transaction idle then must hang up the line.            
Bool PreDialAbort;

// CText Pointers for buttons custom labels
char *CtxtPtr[8];

// Pointer to Field Definition
struct FLDDEF FieldDef;

// Typedef enum - Extended print capability
CURRENTELEM currElement;   // Current element processed
UBYTE vFieldNum;           // current Field


// Application Level Auto Answer Flag 0-OFF 1-Initilized 2-Processing
UBYTE AutoAnswerFlg;

// AVS Response Support
UBYTE AVSERRFLG;

// HDT  Screen Language Data
UBYTE *pTableStart;
PAGE_NUMBERS TablePage;
UWORD TableItemCount;
enum transactions User_Events[NUM_OF_USER_EVENTS];
enum msg_id User_Msgs[NUM_OF_USER_MESSAGES];
IMAGE_ID User_Imgs[NUM_OF_USER_IMAGES];
UBYTE Enable_Flags[NUM_OF_ENABLE_FLAGS];
SDL_PARMS SDLparms;				// used to pass data for input box


UBYTE FlagHostCntl;				// set as required to preprint and allow cancel.

int screen_limit;				// Max number of characters to display on a line

OS_TIMES UserTimeout;			// Timeout for user input

Bool bFileInit;			        // Flag to indicate performing initialization from database
Bool AutoInit;					// Flag to indicate performing automatic initialization

Bool ShowDialStatus;			// Flag to show dialing connection status

UBYTE MLSemaphore;				// Semaphore to control access to MultiLingual table.

char MLBuffer[MAX_MSG_LEN + 1];	// Global buffer to return message from MultiLingual table.

UBYTE DialTranKey;				// Transaction being dialed for

PERFORMRPT Data_kind;          // Performance Report. Data Kind to increment (variable type enum).

UBYTE AqSem;					/* AQTAB access semaphore*/
void * gpAqPtr;					/* Global pointer used by AQTAB access functions */
UBYTE aqByte;					/* Global byte value used by AQTAB access functions */

UBYTE CRSem;					/* CRTAB access semaphore*/
void * gpCRPtr;					/* Global pointer used by CRTAB access functions */
UBYTE CRByte;					/* Global byte value used by CRTAB access functions */

UBYTE IssSem;					/* ISTAB access semaphore*/
void * gpIssPtr;				/* Global pointer used by ISTAB access functions */
UBYTE issByte;					/* Global byte value used by ISTAB access functions */

UBYTE *pSFData;				// Subfield data start pointer
UBYTE *pB60DAddress;			// Pointer to destination buffer
UWORD MoveSize;				// Number of bytes to move
UWORD B60DBytesLeft;			// Bytes left in destination buffer

Bool OnlineCancelled;			/* online transaction cancelled by user */


UBYTE	PIN_DEVICE;				/* PIN device type */
void*	PIN_CONFIG;				/* Pointer to PIN device configuration parameters */

UBYTE  MenuItemCounter;
UBYTE  Number_Of_Applications;

#ifdef MAKE_EMV
UBYTE ScriptLen;			// Length of received issuer script
UBYTE IssScriptBuf[128];	// Temporary storage for Issuer Scripts
UBYTE EMVVersion;           // Operating mode of EMV Library
Bool  PrintEMVDebug;        // EMV debug printing control flag;
Bool  ChipCardInserted;     // Indicates card inserted when txn started by button press
UBYTE Atmel_ATR[12];        // Atmel version data
SCRDDEVICE CryptSlot;       // used for locating the Atmel crypto processing chip

//Application Capabilities
UBYTE LocalAppCap[3];
// //first byte
// #define APP_CAP1_CARDHOLDER_CONF 0x80    // Enable Transaction Log
// #define APP_CAP1_MULTI_LANG      0x40    // Disable Exception file flag
// #define APP_CAP1_PED_PRESENT     0x20    // Disable merch force online capability
// #define APP_CAP1_PIN_BYPASS      0x10    // Disable merch force accept after fail capability
// #define APP_CAP1_AMT_PIN_SAME    0x08    // Enable Transaction Log
// //second byte
// #define APP_CAP2_TXN_LOG         0x80    // Enable Transaction Log
// #define APP_CAP2_EXC_FILE_TEST   0x40    // Disable Exception file flag
// #define APP_CAP2_MERCH_SUSP      0x20    // Disable merch force online capability
// #define APP_CAP2_EMV_FORCED      0x10    // Disable merch force accept after fail capability
// //third byte
// #define APP_CAP3_EMV_ADVICES     0x80    // Enable Transaction Log
// #define APP_CAP3_ISS_REFER       0x40    // Disable Exception file flag
// #define APP_CAP3_CARD_REFER      0x20    // Disable merch force online capability

UBYTE	ScrChkStat;	// Smart Card Reader Check State flags
#endif // MAKE_EMV


Bool ForcedOnline;	// Force online flag

//FIXME not used?
UBYTE  FontSize;	// Display font size
UBYTE RefundDate[8];      // @ydeniz -- BC -- 

UBYTE SetTMParameterVariables;

//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================


//=============================================================================
// Private function definitions
//=============================================================================

