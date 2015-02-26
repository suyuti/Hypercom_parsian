//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             Hypertcf.c
//      main idle loop for Hypercom Transaction Control Framework
//
//=============================================================================

// System includes
#include "string.h"

// HSDK includes
#ifdef MAKE_VIKING
#include "eftos.h"
#endif // MAKE_VIKING
#include "osclib.h"
#include "hfs.h"
#include "stdlib.h"
#include "sdkgui.h"
#include "basictyp.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "tlib.h"
#include "fixdata.h"
#include "utllib.h"

// Application includes
#include "struct.h"
#include "appdata.h"
#include "autosetl.h"
#include "chkbatch.h"
#include "comdata.h"
#include "comm.h"
#include "disproc.h"
#include "eventbl.h"
#include "getkey.h"
#include "icepak.h"
#include "initcryp.h"
#include "iccdebug.h"
#include "journal.h"
#include "menu.h"
#include "msg.h"
#include "pinpad.h"
#include "prtdef.h"
#include "prtdef2.h"
#include "prttable.h"
#include "rs232tab.h"
#include "schdoverride.h"
#include "show.h"
#include "tables.h"
#include "text.h"
#include "textinit.h"
#include "trinpclr.h"
#include "util.h"
#include "util2.h"
#include "DbgTrace.h"

#ifdef MAKE_EMV
#include "emv2_pk.h"
#include "emv_l2.h"
#include "emvtabs2.h"
#include "emvutils.h"
#endif // MAKE_EMV

// This module external declaration
#include "hypertcf.h"
#ifdef MAKE_M2
#include "ColorScheme.h"
#endif // MAKE_M2

//=============================================================================
// External variables / declarations
//=============================================================================
extern int RegCardTab( void );

#ifdef MAKE_EMV
// EMV Functions
extern void EM4Prepare ( void );
extern void PrintCorruptKeys ( void );
#endif // MAKE_EMV

//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static void init_tcf_quick( void );
static void DispInitRequest( void );
static void IdleFunc( void );
static void MenuFunc( void );
static void IcePacFunc( void );
static void CardSwipeFunc( void );
static void ManualFunc( void );
static void TransFunc( void );
static void KBD_Unlock( void );
static Bool get_card( void );
void DecTimer( void );


//=============================================================================
// Public data definitions
//=============================================================================
UBYTE nextState;


//=============================================================================
// Private data definitions
//=============================================================================
static UWORD EventID;
static UBYTE timer_value, Step; 
static UBYTE Clean_up;
static Bool Once_a_Second;

static const ENTRY_STR CardNumberEntry = {
	N_NullStr,					// Issuer Tran
	EnterCard,					// Prompt
	N_NullStr,					// Default passed in INJECTKEY
};

//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     Entry point for Transaction Control Framwork process.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void INITTCFW( void )
{

	init_tcf_quick();			// Initialization
	MenuItemCounter = 0;

    Clean_up = CANCEL_KY;		// Cancel put screen back in idle mode
	nextState = ST_IDLE;		// Preset to start at IdleFunc.

	while( 1 )					// Loop here forever
	{
		switch ( nextState )
		{
			case ST_IDLE:		// Always the first state
#ifdef	MAKE_EMV
				/* Set Idle flag */
				ScrChkStat |= ScrIdleState;
#endif	// MAKE_EMV
				IdleFunc();
				break;
			case ST_MENUFUNC:	// Triggered if ButtonID is a Screen (only from Idle)
				MenuFunc();
				break;
			case ST_ICEPAC:
#ifdef	MAKE_EMV
				/* Set Idle flag */
				ScrChkStat |= ScrIdleState;
#endif	// MAKE_EMV
				IcePacFunc();
				break;
			case ST_CARDSWIPE:
				CardSwipeFunc();
				break;
			case ST_MANUAL:
				ManualFunc();
				break;
			case ST_TRANSACTION:
				TransFunc();
				break;
			default:
				nextState = ST_IDLE;	// Should never get here unless
				break;			// something bad is wrong.
		}

#ifdef	MAKE_EMV
		/* Check Idle state flag */
		if ( ScrChkStat & ScrIdleState )
		{
			/* Clear Idle state flag */
			ScrChkStat &= ~ScrIdleState;

			if ( ScrChkStat )
			{
				/* Cancel response timeout timer */
				OS_CancelTimer ( &TIM_01 );
				/* Close Pin Pad Port */
				Rewind_Pin();
				/* Reset all flags */
				ScrChkStat = 0;
			}
		}
#endif	// MAKE_EMV
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Format PAN and expiration into BCD and store in TRINP
//!
//! \return
//!     0 if error, else set to 1
//-----------------------------------------------------------------------------
extern UBYTE BuildPAN_EXP( void )
{
	UBYTE offset, idx;
	UBYTE chRetval;

	offset = 0;					// offset in source buffer
	idx = 0;					// index to destination buffer
	memset( TRINP.TRPAN, 0xFF, S_TRPAN );

	while ( ( TRINP.TRTRK2[offset] != 0x3d ) && ( offset != S_TRPAN * 2 ) )
	{
		TRINP.TRPAN[idx] = ( TRINP.TRTRK2[offset++] );
		if ( TRINP.TRTRK2[offset] != 0x3d )
		{
			TRINP.TRPAN[idx] <<= 4;
			TRINP.TRPAN[idx++] |= ( TRINP.TRTRK2[offset++] & 0x0f );
		}
	}

	// If offset is odd then must shift last nibble of last byte
	// 4 to the left and then set the last nibble to f.
	if ( offset % 2 )
	{
		TRINP.TRPAN[idx] <<= 4;
		TRINP.TRPAN[idx] |= 0x0f;
	}


	// If offset equals (S_TRPAN * 2) then a there was no 0x3d - bad card.
	if ( offset != S_TRPAN * 2 )
	{
		offset++;				// skip the separator 0x3d, '='

		TRINP.TREXPD[0] = TRINP.TRTRK2[offset++];
		TRINP.TREXPD[0] <<= 4;
		TRINP.TREXPD[0] |= ( TRINP.TRTRK2[offset++] & 0x0f );
		TRINP.TREXPD[1] = TRINP.TRTRK2[offset++];
		TRINP.TREXPD[1] <<= 4;
		TRINP.TREXPD[1] |= ( TRINP.TRTRK2[offset++] & 0x0f );
		chRetval = 1;
	}
	else
	{
		chRetval = 0;
	}
	return chRetval;
}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Decrement timer_value till 0 and set refresh every second.
//!
//! \return
//!     None
//!
//! \note
//!     Scheduled every second.
//-----------------------------------------------------------------------------
void DecTimer( void )
{
	if ( timer_value )
	{
		// Decrement timer_value since it is non-zero.
		timer_value--;
		if ( !timer_value )
		{
			Clean_up = 10;		// Flag loop that timer expired.
		}
	}

	if ( backlight_timer )
	{
		backlight_timer--;
	}

	Once_a_Second = True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Initializes for Transaction Control Framwork process.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void init_tcf_quick( void )
{
#ifdef MAKE_OPTIMUM
    Bool            bInitOk;
#endif

    // First initialize the IO data buffers (this must be done on every page
	// that uses IO functions), then test the various sections of the SDK
	// interface.
	SDK_InitIO();

	// Do GUI initializations
	SDK_GuiInit();

    // Open Crypto service
    if(!InitSecurityProvider( ))
    {
        // FIXME!!! Fatal Error Message here
        RunTimeError( 0x0091 );
    }

#ifdef MAKE_M2
    SchemeSetBackground( (unsigned char *) "background.bin" );
#endif // MAKE_M2

    // Prepare journal
	JournalPrepare();

#ifdef MAKE_EMV					
	// moved this section down to ensure that pin and tables etc are 
	// initialised before calling the SmartCard/Pin	 

    // Initialise the smart card
    ScrdInitIO( );

	// Set slot for Atmel cryptography chip - 0 if not present
//	InitCryptoProc();

    // Initialise the EMV kernel
    EMVL2_Initialise( EMVL2_v4, 0 );

    ChipCardInserted = False; // checked at txn start
    PrintEMVDebug = False;  // Disable debug printing

    // move termconf value into changeable global
    memcpy ( (void *) LocalAppCap, (void *) TCONF.TCAPPCAP, 3);

	/* Reset all flags */
	ScrChkStat = 0;
#endif // MAKE_EMV

	// Check Terminal Tables Consistency
	CheckTables();
#ifdef MAKE_OPTIMUM
    // Update tables if CRC saved is different from that
    // of the TM init file or current tables are corrupt
    bInitOk= UpdateTables();
    // Check Terminal Tables Consistency
    if( bInitOk == True )
    {
        CheckTables();
    }
#endif

    // Validate the Printer Type
	CheckPrinterType();

	// Initialize Dynamic Multilingual Text
	TextTables_Init();

	// Set default font
	FontSize = DEFAULT_FONT_SIZE;
	SetCurrentLanguageFont();

	InitSDL();

	/* Make sure AQTAB semaphore is released */
	RelSem( &AqSem );

	/* Make sure CRTAB semaphore is released */
	RelSem( &CRSem );

	/* Make sure ISTAB semaphore is released */
	RelSem( &IssSem );

	// Delay auto settle for 5 minutes, avoid conflicts.
	DelayAutoSettle();

    // Format the Software Version
	VersionFormat();

	// Check the Quick Start setting
	if ( (NMSIDLL[0] == 'Q') && (NMSIDLL[1] == 'S') )
	{
        // Quick start done, clear flags
        NMSIDLL[0] = 0;
        NMSIDLL[1] = 0;
	}
    else
    {
		// Set the Display Contrast.
		SDK_DisplayContrast( TERM.TERMContrast );
		VerifyEventTableSize();

		// ******** STANDARD BOOTUP SCREEN **************

        // Show Software Version
        ShowSWVersion();
        SDK_Wait( ONESECOND * 3 );
    }

	// Set user timeout
	if ( TCONF.TCOPT6 & TC6_LONGTO ) 
        UserTimeout = 255;
	else							 
        UserTimeout = 30;

	timer_value = 0;

	// clear TRINP before allowing other processes to proceed.
	TrinpClear();

	// ENAPPL is set to zero by the kernel at start up.
	// Other processes wait until ENAPPL is turned on.  Do it now.
	ENAPPL.initialized = 1;

	// Make sure all processes run once
	SDK_RelinqCPU();

	// initialize ICEPAK , fverify() must be called before this
	IcePak_Init();

    //  Test the journal validity
	if ( !JournalValidate() )
	{
		// Notify user "Transaction Memory" 
        ShowErrMsg( TransMemErr );
	}

	// Set Training Mode on
	TCONF.TCOPT4 |= TC4_TRAINING;

	// Clear the Journal of all Training Transactions
	TrainJournalClear();

	// Set Training Mode off
	TCONF.TCOPT4 &= ~TC4_TRAINING;

	// Set the Table's LRC
	SetTCTabLRC();

	// Clear initialization in progress flag
	TFTAB.TFIND1 &= ~TF1_DOING_INIT;
	SetTFTabLRC();

	// Initialize Pointer to the printer custom routines
	PrtDefInit();

	// Set printer logical flag to available if configured.
	if ( TCONF.TCOPT2 & TC2_PRINT ) PRTSTAT = 0x01;

	DelayIdleDisplay = False;
	Once_a_Second = False;
	Clean_up = 0;

	SDK_RegisterTimerRoutine( rt_ONE_SEC, SDK_ThisPage(  ), DecTimer );

	OpenKeyPad();	// Open keypad
	SDK_Trk1Close();
	SDK_Trk2Close();
	DevFlag = 0;				// Reset flag indicating closed all input devices

#ifdef MAKE_OPTIMUM
#ifdef MAKE_M2
    // See if internal RFID
    if ( TCONF.TCOPT4 & TC4_INTRRFID )
    {
        HW_ReadCard( HW_READ_RFID | HW_READ_MSR );
    }
    else
    {
        HW_ReadCard( HW_READ_MSR );
    }
#endif //MAKE_M2
    // Register card ranges to AAE.
	RegCardTab();
#endif

// Initialize the pinpad.
	PinInit ();

	// Check the Quick Start setting
	if ( (NMSIDLL[0] != 'Q') || (NMSIDLL[1] != 'S') )
	{
		// Beep once when terminal is actually starting.
		SDK_BeepIt( 1 );
	}

	// Check settlement required by Application Manager
   	if ( (NMSIDLL[0] == 'A') && (NMSIDLL[1] == 'S') )
    {
        NMSIDLL[0] = 0x00;
        NMSIDLL[1] = 0x00;
		ShowErrMsg( SettlementRequired );
    }
}

//-----------------------------------------------------------------------------
//! \brief
//!     Set Display if Initialize required
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void DispInitRequest( void )
{
	// Default to Initialize message
	GetMsg( PleaseInit, CSTMSG );

	// See if NAC Dialing selected or Dial Up only
	if ( ( TERM.TERMOpt1 & TM1_NACDIAL ) || ( cl_DIALUP == TERM.TERMLine ) )
	{
		// See if INIT Tel No Entered
		if ( 0xff == TERM.TERMInitTel[0] )
		{
			// "CONFIGURE TERMINAL/OR CALL HELP-CO"
			GetMsg( ConfigTerm, CSTMSG );
		}
	}

	// "PLEASE INITIALIZE"
	DrawScreen( INIT_SCR );
    ButtonCnt = 4;
}

//-----------------------------------------------------------------------------
//! \brief
//!     This the very first and idle function for the application.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void IdleFunc( void )
{
	static UBYTE icepak_delay;
	static UWORD device_map;
	static Bool ReWriteScreen;
	Bool bPinCardStatus;
	UBYTE inputtype;

	// always clear menu stack when back at idle
	ClearMenus();

	// Display Idle Screen and wait for Input
	// If input is another screen, then stay in this function 
	// Else, setup next step and return to control loop
	SDK_DisplayBackLight( 1 );	// turn on backlight
	backlight_timer = BACKLIGHT_DELAY;	// Start backlight timer

#ifdef MAKE_EMV
    // remove card if still in the slot
    PerformCardClose();
#endif // MAKE_EMV

    // Close all entering devices (reported by @TUM)
	read_io( 0, 0, &bPinCardStatus, &DevFlag );

#ifdef MAKE_EMV
	// Print ICC Debug Report
	if (PrintEMVDebug)
	{
		PrintIccDebugReport();
	}
#endif // MAKE_EMV

	timer_value = 0;			// Reset screen timer

	Clean_up = EventID = Step = 0;
	ReWriteScreen = True;
	ShowDialStatus = True;

	do
	{
#ifdef MAKE_EMV
		// print corrupt key if necessary
		if ((BadKeyInKeystore) && (!BadKeyReported))
		{
		    PrintCorruptKeys();
		    BadKeyReported = True;
		}
#endif // MAKE_EMV


		// clear TRINP before checking event
		TrinpClear();

		// Set ice pac start delay time locally.
		icepak_delay = IcePak_StartDelay;

		// If global DelayIdleDisplay is true then set timer_value
		// so swipe/time message will not occur till timer_value  expires.
		if ( DelayIdleDisplay )
		{
			timer_value = ( UBYTE ) UserTimeout;
			Clean_up = 0;		// Don't clean up if delay this display
		}
		else
		{
			timer_value = 0;
		}
		DelayIdleDisplay = False;

		// check keyboard locked condition
		if ( TCONF.TCLOCOP & TCL_LOCKED )
		{
			KBD_Unlock();
			// Clear timer_value in case just completed an
			// initialization that has the keyboard locked
			// option and were expecting to leave the
			// response  on the display.
			timer_value = 0;
		}

		if ( 0x00 == TERM.TERMInit )
		{
			if ( ReWriteScreen )
				DispInitRequest();	// Need to initialize
		}
		else
		{
			if ( ( 0 == timer_value ) && ( ReWriteScreen ) )
			{
				if ( PINSTAT & PinCard_Read )
					User_Msgs[7] = SwipeCardPin;
				else
					User_Msgs[7] = Swipecard;

				Enable_Flags[0] = 1;	// turn on alternating title bar

				// Don't turn on sliding card graphic if
				// reading the card from the pinpad.
				if ( !( PINSTAT & PinCard_Read ) )
					Enable_Flags[1] = 1;	// turn on card image
				else
					Enable_Flags[1] = 0;	// do not turn on card image
               	
				DrawScreen( IDLE_SCR );
			}
			// Always set ReWriteScreen to true. Only False immediately
			// after a background touch.
			ReWriteScreen = True;
		}

		// Wait for input. In case of ICEPAC, use timer to exit after expired
		INJECTKEY = 0;			// Pass In injkey Key
		inputtype = 0;			// Reset input flag

		do						// Now wait for input
		{
			device_map = DEV_PAD + DEV_KBD + STAY_OPEN1;
			if ( PINSTAT & PinCard_Read )
				device_map |= DEV_PINCARD;
			else
				device_map |= DEV_CARDRDR;

#ifdef MAKE_EMV
			// Add Smart card reader if present
//###			if ( TERM.TERMOpt4 & TM4_ICC )
			{
				device_map |= DEV_SCR;
			}
#endif // MAKE_EMV

			inputtype = read_io( device_map, 0, &bPinCardStatus, &DevFlag );	// Wait for input
			if ( !inputtype && !backlight_timer )
			{
				SDK_DisplayBackLight( 0 );	// turn off backlight
                backlight_timer = BACKLIGHT_DELAY;	// Start backlight timer
			}

			// Check for autosettle and icepac at idle if there was no input.
			if ( !inputtype && Once_a_Second )
			{
				Once_a_Second = False;

				// If time to auto settle then do CLBATCHA transaction.
				if ( AutoSettle() )
				{
					EventID = CLBATCHA;	// Next state autosettle
					nextState = ST_TRANSACTION;
					break;
				}
				else
				{
					// See if initialiaze requested by scheduler
					// or performing automatic initialize from host indicator
					if ( ( 0x0FE == TERM.TERMInit ) || AutoInit )
					{
						// Yes, Perform terminal initialize
						//INITDIAL 
						ShowDialStatus = False;
						EventID = INITIALIZE;
						nextState = ST_TRANSACTION;
						break;
					}
				}

				// If icepak delay start timer is enabled then count it
				// down unless there is a delay from DelayIdleDisplay.
				if ( icepak_delay && ( 0 == timer_value ) )
				{
					icepak_delay--;
					if ( !icepak_delay )
					{
						nextState = ST_ICEPAC;
						break;
					}
				}
			}

			// If any input has occurred terminal scheduled idle minutes prior
			// to a scheduled event, clear that event from the schedule table,
			// terminal is too busy to perform the scheduled event
			if ( inputtype )
			{
				SchdOverride();
			}

			SDK_RelinqCPU();
		}
		while ( !inputtype && !Clean_up );

		switch ( inputtype )	// Act according to inputtype
		{
#ifdef MAKE_EMV
			case DEV_SCR:		// Smart card inserted
                // Smart card processing...
				EventID = (TC2_CASHONLY & TCONF.TCOPT2) ? EMV_CASH : EMV_SALE; // Prepare for EVENT_TABLE index call
				nextState = ST_TRANSACTION;
 				break;
#endif // MAKE_EMV

			case DEV_PAD:		// Button Pressed
				switch ( ButtonId )
				{
					case CLEAR_KY:
					case CANCEL_KY:	// User pressed Cancel
					case ENTER_KY:
					case 0:	// Background touched
						break;


					default:
						if ( ButtonId < END_OF_EVENTS )	// Transaction or function
						{
							EventID = ButtonId;
							nextState = ST_TRANSACTION;
							break;
						}
						else if ( ButtonId < EVENT_MAX )	// Special action or invalid
							break;
						else if ( ButtonId < MENU_MAX )	// Menu screen
							nextState = ST_MENUFUNC;	// inputtype stays zero (loop back)
						// else input screen or invalid, ignore
						break;
				}

				// If still in idle and timer_value is not set, do not redraw idle screen 
				// to prevent sprite from flashing 
				if(ST_IDLE == nextState && 0 == timer_value)
				{
					ReWriteScreen = False;
				}

				break;
			case DEV_KBD:		// Key press input
				if ( ( KeyVal >= '0' ) && ( KeyVal <= '9' ) )
				{
					INJECTKEY = KeyVal;	// Pass In injectkey Key
					TRINP.TRPOSE[1] = 0x12;	// Set Manual entry mode
					EventID = SALE;	// Default Sale Transaction
					nextState = ST_TRANSACTION;
				}

				// If still in idle and timer_value is not set, do not redraw idle screen 
				// to prevent sprite from flashing 
				if(ST_IDLE == nextState && 0 == timer_value)
				{
					ReWriteScreen = False;
				}

				break;

			case DEV_CARDRDR:	// Card swiped. Next functions is X_IdleCard
				nextState = ST_CARDSWIPE;
				break;

			case DEV_PINCARD:	// Card swiped on the pinpad
				if ( 0 == BuildPAN_EXP() )
				{
					ShowErrMsg( CardErr );
					timer_value = 10;	// Error msg display timer
					nextState = ST_IDLE;
					break;
				}

				EventID = SALE;	// Prepare for EVENT_TABLE index call
				nextState = ST_TRANSACTION;
				break;

			default:			// Autosettle and icepac will go through here.
				break;
		}
	}
	while ( ( ST_IDLE == nextState ) && !Clean_up );

	// turn on backlight whenever we leave idle, later turn off after 25 seconds of inactivity
	SDK_DisplayBackLight( 1 );	// turn on backlight
	backlight_timer = BACKLIGHT_DELAY;	// Start backlight timer
}


//-----------------------------------------------------------------------------
//! \brief
//!     The menu function loops on screen displays until hit an event or
//!     another state.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void MenuFunc( void )
{
	static UBYTE inputtype;		// Local DEV_xxx
	static UWORD device_map;
        Bool bPinCardStatus;

	// Display Screen and wait for Input If input is another screen,
	// then stay in this function Else, setup next step and return
	// to control loop

	inputtype = 0;				// Reset input flag

	do
	{
		if ( !Clean_up )
		{
			if ( DEV_KBD != inputtype )
			{
				DrawMenu( ButtonId );	// Display Screen
			}
		}
		else
		{
			if ( B_PREVIOUS == Clean_up )
			{
				DrawMenu( MENU_PREVIOUS );	// Display Screen
				Clean_up = 0;
			}
			else if ( B_NEXT == Clean_up )
			{
				DrawMenu( MENU_NEXT );	// Display Screen
				Clean_up = 0;
			}
		}

		timer_value = ( UBYTE ) UserTimeout;	// Timeout value to get out of this screen

		// Wait for input. In case of ICEPAC, use timer to exit after expired
		INJECTKEY = 0;			// Pass In injkey Key
		inputtype = 0;			// Reset input flag

		do						// Now wait for input
		{
			device_map = DEV_PAD + DEV_KBD + STAY_OPEN1;

			inputtype = read_io( device_map, 0, &bPinCardStatus, &DevFlag );	// Wait for input

		}
		while ( !inputtype && !Clean_up );

		switch ( inputtype )	// Act according to inputtype
		{
			case DEV_PAD:		// Button Pressed
				switch ( ButtonId )
				{
					case CLEAR_KY:
					case CANCEL_KY:	// User pressed Cancel
						Clean_up = CANCEL_KY;
						nextState = ST_IDLE;	// Back to idle
						break;

					case ENTER_KY:
						Clean_up = CANCEL_KY;
						nextState = ST_IDLE;	// Back to idle
						break;

					case B_PREVIOUS:
					case B_PREVIOUS_NEW_MENU:
						Clean_up = B_PREVIOUS;	// Back to previous step
						break;

					case B_NEXT:
						Clean_up = B_NEXT;	// next menu
						break;

					case 0:	// ignore
						break;

					default:
						if ( ButtonId < END_OF_EVENTS )	// Transaction or function
						{
							EventID = ButtonId;
							nextState = ST_TRANSACTION;
						}
						else if ( ButtonId < EVENT_MAX )	// Special action or invalid
						{
							Clean_up = ( UBYTE ) ButtonId;
						}
						// assume another menu and continue
						break;
				}
				break;

			case DEV_KBD:		// Key press input. Allow beep but no action unless CLEAR.
				if ( CLEAR_KY == KeyVal )
				{
					Clean_up = CANCEL_KY;
					nextState = ST_IDLE;	// Back to idle
				}

				if ( CANCEL_KY == KeyVal )
				{
					Clean_up = CANCEL_KY;
					nextState = ST_IDLE;	// Back to idle
				}

				break;

			case 0:			// Timed out
				Clean_up = CANCEL_KY;
				nextState = ST_IDLE;	// Back to idle
				SDK_BeepIt( 4 );
				break;
		}
	}
	while ( ( ST_MENUFUNC == nextState ) && ( CANCEL_KY != Clean_up ) );
}


//-----------------------------------------------------------------------------
//! \brief
//!     ICEPAC screen per HDS setup. IDLE must time out before start
//!     on ICEPAC screen
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void IcePacFunc( void )
{
	static UBYTE inputtype;		// Local DEV_xxx
	static UWORD device_map;
	static UBYTE icepak_interval;
    Bool bPinCardStatus;

	SDK_DisplayBackLight( 1 );	// turn on backlight
	backlight_timer = BACKLIGHT_DELAY;	// Start backlight timer
    
#if defined( MAKE_OPTIMUM ) && defined( MAKE_M2 )
    //  Switch off RFID if on
    HW_ReadCard( HW_READ_MSR );
#endif

	do
	{
		icepak_interval = IcePak_IntervalDelay;
		DrawScreen( ICEPAK_IDLE_SCR );
		IcePak_NextIdleScreen();	// put icepak idle screen in screen buffer

		// Wait for input. Use timer to exit after expired
		do
		{
			// This device map must exactly match the one in IdleFunc().
			device_map = DEV_TOUCH | DEV_PAD | DEV_KBD | STAY_OPEN1;
			if ( PINSTAT & PinCard_Read )
				device_map |= DEV_PINCARD;
			else
				device_map |= DEV_CARDRDR;

#ifdef MAKE_EMV
			// Add Smart card reader if present
//###		 	if(TERM.TERMOpt4 & TM4_ICC)
		 	{
				device_map |= DEV_SCR;
		 	}
#endif // MAKE_EMV

			inputtype = read_io( device_map, 0, &bPinCardStatus, &DevFlag );	// Wait for input
			if ( !inputtype && !backlight_timer )
			{
				SDK_DisplayBackLight( 0 );	// turn off backlight
				backlight_timer = BACKLIGHT_DELAY;	// Start backlight timer
			}

			// Check for autosettle and icepac at idle if there was no input.
			if ( !inputtype && Once_a_Second )
			{
				Once_a_Second = False;

                // If time to auto settle then do CLBATCHA transaction.
				if ( AutoSettle() )
				{
					EventID = CLBATCHA;	// Next state autosettle
					nextState = ST_TRANSACTION;
					break;
				}
				else
				{
					// See if initialiaze requested by scheduler
					// or performing automatic initialize from host indicator
					if ( ( 0x0FE == TERM.TERMInit ) || AutoInit )
					{
						// Yes, Perform terminal initialize
						//INITDIAL 
						ShowDialStatus = False;
						EventID = INITIALIZE;
						nextState = ST_TRANSACTION;
						break;
					}
				}
								
				// If icepak idle display timer is enabled then count it down.
				if ( icepak_interval )
				{
					icepak_interval--;
					if ( !icepak_interval )
					{
						break;
					}
				}
			}
			SDK_RelinqCPU();
		}
		while ( !inputtype );

		switch ( inputtype )				// Act according to inputtype
		{
#ifdef MAKE_EMV
			case DEV_SCR:			// Smart card inserted
				// Smart card processing...
				EventID = (TC2_CASHONLY & TCONF.TCOPT2) ? EMV_CASH : EMV_SALE;	// Prepare for EVENT_TABLE index call
				nextState  = ST_TRANSACTION;
 				break;
#endif // MAKE_EMV

			case DEV_PAD:					// Pad Pressed
				nextState = ST_IDLE;		// Go back to idle
				SDK_BeepIt( 1 );			// after a short beep.
				break;

			case DEV_KBD:					// Key press input
				if ( ( KeyVal >= '0' ) && ( KeyVal <= '9' ) )
				{
					INJECTKEY = KeyVal;		// Pass In injkey Key
					TRINP.TRPOSE[1] = 0x12;	// Set Manual entry mode
					EventID = SALE;			// Default Sale Transaction
					nextState = ST_TRANSACTION;
				}
				else						// ENTER or CLEAR key pressed
					nextState = ST_IDLE;	// Go back to idle
				break;

			case DEV_CARDRDR:				// Card swiped. Next functions is X_IdleCard
				nextState = ST_CARDSWIPE;
				break;

			case DEV_PINCARD:				// Card swiped on the pinpad
				EventID = SALE;				// Prepare for EVENT_TABLE index call
				nextState = ST_TRANSACTION;
				break;

			default:
				break;
		}
	} while ( ST_ICEPAC == nextState );

#if defined( MAKE_OPTIMUM ) && defined( MAKE_M2 )
    // See if internal RFID and switch it on
	if ( TCONF.TCOPT4 & TC4_INTRRFID )
    {
        HW_ReadCard( HW_READ_RFID | HW_READ_MSR );
    }
    else
	{
        HW_ReadCard( HW_READ_MSR );
	}
#endif

	// turn on backlight whenever we leave idle, later turn off after 25 seconds of inactivity
	SDK_DisplayBackLight( 1 );	// turn on backlight
	backlight_timer = BACKLIGHT_DELAY;	// Start backlight timer
}


//-----------------------------------------------------------------------------
//! \brief
//!     Triggers from card swipe at idle
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void CardSwipeFunc( void )
{
	if ( get_card() )
	{
		nextState = ST_TRANSACTION;
		EventID = SALE;
	}
	else
	{
		timer_value = 10;		// Error msg display timer
		nextState = ST_IDLE;
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Keypress from IDLE triggers this function
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ManualFunc( void )
{
	if ( NumEntry( INJECTKEY, 19, 6, &CardNumberEntry ) )
	{
		nextState = ST_TRANSACTION;
		EventID = SALE;
	}
	else
	{
		timer_value = 10;		// Error msg display timer
		nextState = ST_IDLE;
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Action from IDLE triggers this function
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void TransFunc( void )
{
	static const struct event_entry *pEvent;

	do
	{
		pEvent = &EVENT_TABLE[EventID];	// Setup function call
		memcpy( ( struct event_entry * ) &CurrentEvent, pEvent,
				sizeof( struct event_entry ) );

		// Updates Event message from Multilingual table
		TextGetString( ( char * ) &CurrentEvent.TRNTEXT, evttext_table_id,
					   EventID );

		DefaultTitle[0] = 0;	// use new trans for default title

		// See if time to clear a batch that was settled.
		ChkClrBatch();

// check if init required
		if ( pEvent->TRNOPT3 & T3_NO_INIT )
		{
	        // Call action handler anyway, special case
	        pEvent->X_func();
		}
		else // must look at whether we are initialized
		{
			if ( 0x00 == TERM.TERMInit ) // init required?
	        {
		        ShowErrMsg( PleaseInit );
				CurrentEvent.NEXTEVENT = 0;
	        }
			else
			{
		        // Call action handler
		        pEvent->X_func();
			}
		}

		if ( CurrentEvent.NEXTEVENT )
		{

			if ( CurrentEvent.NEXTEVENT < END_OF_EVENTS )	// Another crosscall
			{
				EventID = CurrentEvent.NEXTEVENT;
			}
			else if ( CurrentEvent.NEXTEVENT < MENU_MAX )	// Menu screen
			{
				ButtonId = CurrentEvent.NEXTEVENT;
				nextState = ST_MENUFUNC;
			}
		}
		else if ( ButtonId == B_PREVIOUS || ButtonId == B_PREVIOUS_NEW_MENU )
		{
			Clean_up = B_PREVIOUS;	// Back to previous step
			nextState = ST_MENUFUNC;
		}
		else if ( ButtonId == B_NEXT )
		{
			Clean_up = B_NEXT;	// Back to previous step
			nextState = ST_MENUFUNC;
		}
		else
		{
			nextState = ST_IDLE;
		}
	}
	while ( ST_TRANSACTION == nextState );

	// If predialed but aborted transaction then must hangup now.
	if ( PreDialAbort )
	{
		// Request Hang Up of the line if on-line
		Hang_Up();
		PreDialAbort = False;
	}

	// If bad PIN returned, drop line here. The reason is that neither Hang_Up_Pin()s in HOST modules
	// nor transaction functions trying up to 3 times to enter the proper PIN, drop line.
	if ( TRINP.TRRSPOPT & R_BADPIN )
	{
		// Request Hang Up of the line if on-line
		Hang_Up();
	}	
}


//-----------------------------------------------------------------------------
//! \brief
//!     Terminal is locked, waiting for password to unlock
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void KBD_Unlock( void )
{
	UBYTE Status;
	ENTRY_STR PswdEntry;

	// NumEntry parameters
	PswdEntry.TitleMid = Closed;
	PswdEntry.Line1Mid = N_NullStr;
	PswdEntry.Line2Mid = EnterPswd;
	do
	{
		memset( ( UBYTE * ) Dspbuf, 0, 5 );	// Initialize input buffer
		Enable_Flags[0] = 1;	// Disable CANCEL Key
		// Prompt for Input Password (4 Digits)
		Status = NumEntry( 4, 4, 4, &PswdEntry );

		// Check if Input Password has been entered
		if ( 1 == Status )
		{
			// Yes; save the Input Password entered
			memcpy( TRINP.TRPWD, Dspbuf, 4 );
			// Verify Input Password entered
			if ( 0 == memcmp( TRINP.TRPWD, (UBYTE *)TERM.TERMPwd, 4 ) )
			{
				TCONF.TCLOCOP &= 0xFE;	// clear the lock bit
				// Set TCONF's LRC
				SetTCTabLRC();
				Clean_up = CANCEL_KY;	// Cancel put screen back in idle mode
			}
			else
			{
				// Incorrect password, notify user
				SDK_Beeper( TENMS * 40 );
			}
		}
	}
	while ( TCONF.TCLOCOP & TCL_LOCKED );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Track information has been read, now format
//!
//! \return
//!     True on success else False
//-----------------------------------------------------------------------------
static Bool get_card( void )
{
	UBYTE i;

	// Check if Track 2 data has been received without error.
	if ( Trk2Status.Error != orvOK )
	{
        DBGTRACE (LVL_WARNING, TYPE_SPOS32, "Read card error", __FUNCTION__ );
		ShowErrMsg( CardErr );
		return False;
	}
	// Yes; good read, format trk 2 Card Data
	for ( i = 0; i < Trk2ReadLength; i++ )
	{
		// Convert to ASCII
		TRINP.TRTRK2[i] += 0x30;
	}
	memset( &TRINP.TRTRK2[Trk2ReadLength], 0xFF,
		  ( UWORD ) ( S_TRTRK2 - Trk2ReadLength ) );

	if ( 0 == BuildPAN_EXP() )
	{
        DBGTRACE (LVL_WARNING, TYPE_SPOS32, "Read card error", __FUNCTION__ );
		ShowErrMsg( CardErr );
		return False;
	}

	// Check if Track 1 data has completed and has been received
	//  without error since Track 2 data is OK.
	if ( ( Trk1Status.CompleteFlag ) && ( orvOK == Trk1Status.Error ) )
	{
		// Yes; good read, format trk 1 Card Data
		for ( i = 0; i < Trk1ReadLength; i++ )
		{
			// Convert to ASCII
			TRINP.TRTRK1[i] += 0x20;
		}
		memset( &TRINP.TRTRK1[Trk1ReadLength], 0xFF,
			  ( UWORD ) ( S_TRTRK1 - Trk1ReadLength ) );
	}
	else
	{
		// Be sure TRK1 data is cleared of any trash data.
		// This field will be tested later if issurer option
		// indicates TRK1 is required.
		memset( TRINP.TRTRK1, 0, S_TRTRK1 );
	}
	return True;
}

