
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             function.c
//      Function module
//
//=============================================================================

// System includes
#include "string.h"
#include "stdlib.h"

// HSDK includes
#include "hw.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkgui.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "sdkmem.h"
#include "hfs.h"
#include "fixdata.h"
#include "tlib.h"

// Application includes
#include "utllib.h"
#include "defines.h"
#include "struct.h"
#include "app1.h"
#include "appdata.h"
#include "tables.h"
#include "batcntl.h"
#include "cashier.h"
#include "comdata.h"
#include "comm.h"
#include "disproc.h"
#include "fdwnld.h"
#include "flaghost.h"
#include "function.h"
#include "hstprint2.h"
#include "icepak.h"
#include "incbatno.h"
#include "initcryp.h"
#include "inpmpswd.h"
#include "inputpwd.h"
#include "istab.h"
#include "journal.h"
#include "jrnapps.h"
#include "menu.h"
#include "msg.h"
#include "pdntab.h"
#include "pinpad.h"
#include "prttable.h"
#include "reports.h"
#include "respdsp.h"
#include "review.h"
#include "rs232tab.h"
#include "settlesum.h"
#include "setup.h"
#include "shift.h"
#include "tables.h"
#include "tabproc.h"
#include "testprnt.h"
#include "totlook.h"
#include "util.h"

#ifdef MAKE_EMV
    #include "emv_l2.h"
    #include "emv2_pk.h"
    #include "emvtabs2.h"
    #include "iccdebug.h"
#endif

#include "setTMComParameter.h"

//=============================================================================
// External variables / declarations
//=============================================================================

// Public functions defined in this module
extern void SetUsrdTabLRC( void );

// Public from other modules
#ifdef MAKE_EMV
extern void PrintKeyTable( void );
#endif

//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static BYTE InputTermSer( void );
static Bool EnhDialM( void );
static BYTE InputInitTel( void );
static BYTE InputNMSTel( void );
static BYTE InputDialMode( void );
static BYTE InputPABX( void );
#ifndef MAKE_OPTIMUM
static void inet_ntoapp( void *pTermFld );
#endif // MAKE_OPTIMUM
static void ClearAcquirer( void );
static char DoPasswordEnable( const ENTRY_STR * screen, UBYTE opt_mask );
static Bool GetTipPercent( UBYTE key );
static Bool SetTipPercent( void );
static void Trk2TOCstMsg( void );
static Bool Enter99Pswd( void );
static Bool CUAddress( void );
static Bool InModemMode( void );
static Bool InDBModemMode( void );
static void TranAcc( void );
static void ToggleBit (UBYTE * pByteValue, UBYTE BitToggler);
static BYTE InputTel( UBYTE * pPhoneData, const ENTRY_STR * pEntry ); // @AAMELIN

static void Fun_8( void );
static void Fun_20( void );
static void Fun_21( void );
static void Fun_31( void );
static void Fun_34( void );
static void Fun_37( void );
static void Fun_54 ( void );

#ifdef MAKE_SMARTINIT
static void Fun_55 ( void );
#endif // MAKE_SMARTINIT

#ifndef MAKE_OPTIMUM
static void Fun_76 ( void );
#endif // MAKE_OPTIMUM

//static void Fun_95( void );
static void Fun_97 ( void );
static Bool Fun_97_1( void );
static void Fun_99( void );

#ifdef MAKE_SCREEN_CAPTURE
static void Fun_88( void );
#endif // MAKE_SCREEN_CAPTURE

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static ENTRY_STR YNEntry;

static const ENTRY_STR MercFunctionEntry = {
    MercFunction,               // Title
    N_NullStr,                  // Default value
    N_EnterFunction,            // Prompt
};

static const ENTRY_STR Fun99PassEntry = {
    MaintFunction,              // Title
    SVPassword,                 // Prompt
    N_NullStr,                  // Default value
};

static const ENTRY_STR Fun90Entry = {
    N_PswdChange,               // Title
    OldPassword,                // Prompt
    N_NullStr,                  // Default value
};

static const ENTRY_STR Fun90Entry1 = {
    N_PswdChange,               // Title
    NewPassword,                // Prompt
    N_NullStr,                  // Default value
};

static const ENTRY_STR Fun90Entry2 = {
    N_PswdChange,               // Title
    NewPassAgn,                 // Prompt
    N_NullStr,                  // Default value
};

static const ENTRY_STR APTermSer = {
    EnterTSer,                  // Prompt
    CustomMsg,                  // Line 1
    Currently,                  // Line 2
};


static const ENTRY_STR Fun99Entry = {
    ProgFunction,               // Title
    BlankMsg,                   // Prompt
    N_NullStr,                  // Default value
};

static const ENTRY_STR Fun99_9Entry = {
    Type,                       // Title
    BlankMsg,                   // Prompt
    N_NullStr,                  // Default value
};



static const ENTRY_STR InpTelEntry = {
    EnterInitTel,               // Title
    CustomMsg,                  // 1-st line
    Currently,                  // 2-nd line
};


static const ENTRY_STR InpNMSEntry = {
    EnterNMSTel,                // Title
    CustomMsg,                  // 1-st line
    Currently,                  // 2-nd line
};

static const ENTRY_STR InpDBInitEntry = {
    EnterDBInitTel,             // Title
    CustomMsg,                  // 1-st line
    N_NullStr,                  // 2-nd line
};


static const ENTRY_STR InpDBNMSEntry = {
    EnterDBNMSTel,              // Title
    CustomMsg,                  // 1-st line
    N_NullStr,                  // 2-nd line
};

static const ENTRY_STR PABXEntry = {
    N_PABX,                     // Title
    CustomMsg,                  // 1-st line
    Currently,                  // 2-nd line
};

static const ENTRY_STR VoidPwdEntry =   // Prompt for Void Password
{
    N_Passwords,                // Title
    VoidPassM,                  // Prompt
    N_NullStr,                  // Prompt line 2
};

static const ENTRY_STR RefundPwdEntry = // Prompt for Refund Password
{
    N_Passwords,                // Title
    RefPassM,                   // Prompt
    N_NullStr,                  // Prompt line 2
};

static const ENTRY_STR AdjustPwdEntry = // Prompt for Refund Password
{
    N_Passwords,                // Title
    AdjPassM,                   // Prompt
    N_NullStr,                  // Prompt line 2
};

static const ENTRY_STR ReportPwdEntry = // Prompt for Refund Password
{
    N_Passwords,                // Title
    RepPassM,                   // Prompt
    N_NullStr,                  // Prompt line 2
};

static const ENTRY_STR KbdDialEntry =   // Prompt if Handset Dialing Allowed
{
    N_Dial,                     // Title
    KbdDialM,                   // Prompt
    N_NullStr,                  // Prompt line 2
};

static const ENTRY_STR TollDialEntry =  // Prompt if Toll Dialing Allowed
{
    N_Dial,                     // Title
    TollDialM,                  // Prompt
    N_NullStr,                  // Prompt line 2
};

static const ENTRY_STR MemDialEntry =   // Prompt if Memory (Fast) Dialing Allowed
{
    N_Dial,                     // Title
    MemDialM,                   // Prompt
    N_NullStr,                  // Prompt line 2
};

static const ENTRY_STR RefDialEntry =   // Prompt if Referral Dialing Allowed
{
    N_Dial,                     // Title
    RefDialM,                   // Prompt
    N_NullStr,
};

static const ENTRY_STR ReceiptEntry = {
    CustomMsg7,                 // Title
    CustomMsg,                  // Default Value (line 1)
    N_NullStr,                  // Empty 
};

static const ENTRY_STR CU_AdR_Entry = {
    N_RS232,                    // Title
    N_NullStr,                  // Default Value (line 1)
    CustomMsg,                  // Default Value overflow (line 2)
};

static const ENTRY_STR ModemMode_Entry = {
    N_CommsModemMode,           // Title
    CustomMsg,                  // Default Value
    N_NullStr,                  // Empty
};

static const ENTRY_STR DBModemMode_Entry = {
    N_CommsDBModemMode,         // Title
    CustomMsg,                  // Default Value
    N_NullStr,                  // Empty
};

static const ENTRY_STR Sug_Tip_Entry = {
    Msg_Tips,                   // Title
    Msg_Sug_Tip,                // Default Value (line 1)
    CustomMsg4,                 // Default Value overflow (line 2)
};

static const ENTRY_STR InitTries_Entry =
{
    InitMaxTries,               // Title
    CustomMsg,                  // Default Value
    N_NullStr,                  // Empty
};

static const ENTRY_STR Fun_76_Entry =   // structure for Fun_76 entry
{
    N_Report,
    N_LastIPaddr,
    CustomMsg,
};

//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//!  \brief
//!     Handle Merchant Function Selection
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void MerchFunction( void )
{
    int FuncNum;

    memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );  // Clear default value holder
    // Loop till valid function entered or cancel
    while ( True )
    {
        // Prompt for Merchant Function number

        if ( !NumEntry( 1, 2, 1, &MercFunctionEntry ) )
            break;              // User canceled or timed out

        // Convert number entered
        FuncNum = atoi( Dspbuf );

        // Check number entered
        switch ( FuncNum )
        {
        case 0:
            Fun_0(  );      // Test transaction
            return;

        case 1:
            CurrentEvent.NEXTEVENT = REVIEW;    // Batch Review
            return;

        case 2:
            TerminalSetup ();
            return;

        case 3:
            Fun_3(  );      // Program Restart
            return;

        case 4:
            Fun_4(  );      // Unlock Keyboard
            return;

        case 5:
            TotalLook(  );  // Transactions Count
            return;

        case 8:
            Fun_8(  );      // Display Batch
            return;

        case 9:
            CurrentEvent.NEXTEVENT = AUTH;  // Auth Transaction
            return;

        case 10:
            Fun_10 ();
            return;
            
        case 11 :
            SetTMComParam();
            return;

            // Training Mode Transaction
        case 12:
            CurrentEvent.NEXTEVENT = TRAINMODE;
            return;

        case 14:
            CurrentEvent.NEXTEVENT = ICEPAK_DOWNLOAD;   // Download IcePak graphic files
            return;

            // Paper Feed Transaction
        case 15:
            CurrentEvent.NEXTEVENT = PAPERFEED_FUN;
            return;

            // Set Contrast Transaction
        case 16:
            CurrentEvent.NEXTEVENT = SETCONTRAST_FUN;
            return;

        case 18:
            TextLanguageSelectMenu ();
            return;

        case 20:
            Fun_20(  );     // Open/Close/Report Tabs
            return;

        case 21:
            Fun_21(  );     // Find - Batch Review starting
            return;         // with an invoice number.

            // Change shift number.
        case 22:
            ChangeShift ();
            return;

        case 26:
            Fun_26(  );     // Entering 4th line for receipt
            return;         // and report header

        case 29:
            SettleStatus(  );   // Settlement Status Report
            return;

            // Display PIN PAD information
        case 31:
            Fun_31(  );     //Toggle Card Read
            return;

            // Display PIN PAD information
        case 32:
            PinDisplayDiag ();
            return;

#ifdef MAKE_EMV
        case 34:
            Fun_34(  );
            return;

        case 37:
            Fun_37(  );     // Toggle EMV debug printing
            return;

        case 38:
            PrintKeyTable(  );    // Print RSA key table
            return;
#endif	// MAKE_EMV

        case 54: Fun_54( );     // Set Max Tries for Init Recovery
            return;
#ifdef MAKE_SMARTINIT
        case 55: 
            Fun_55( );              // Toggle SmartInit
            return;
#endif	// MAKE_SMARTINIT

        case 65:
            CurrentEvent.NEXTEVENT = MAIL_ORDER;    // Mail/Phone Order Transaction
            return;

        case 68:
            FileDownLoad(  );
            return;

        case 71:
            Fun_71(  );     // Printer Enable/Disable
            return;

            // Reprint last receipt
        case 72:
            JrnReprintLast ();
            return;
            // Reprint any receipt
        case 73:
            JrnReprintAny ();
            return;

        case 74:
            CurrentEvent.NEXTEVENT = CRDVER;    // Card Verification
            return;

        case 75:
            CurrentEvent.NEXTEVENT = BALINQ;    // Set the transaction to BALINQ
            return;

#ifndef MAKE_OPTIMUM
        case 76:
            Fun_76 ( );                         // Show captured IP address
            return;
#endif // MAKE_OPTIMUM

        case 77:
            Fun_77 ();      // Cashier Sign-On/Off
            return;

        case 78:
            Fun_78 ();      // Cashier report
            return;
            // Report Selection
        case 79:
            Reports ();
            return;

            // Printer Test
        case 82:
            TestPrinter ();
            return;

        case 83:
            Fun_83(  );     // Display Test
            return;

        case 84:
            Fun_84(  );     // Keyboard Test
            return;

#ifdef MAKE_SCREEN_CAPTURE
        case 88:
            Fun_88(  );     // Screen Capture toggle
            return;
#endif // MAKE_SCREEN_CAPTURE

        case 89:
            Fun_89(  );     // Cardreader Test
            return;

        case 90:
            Fun_90(  );     // Password Change
            return;

        case 91:
            Fun_91(  );     // Show Software Version
            return;

        case 92:
            Fun_92 ();  // Program Load
            return;

        case 93:
            CurrentEvent.NEXTEVENT = LOGON; // Logon to get working key
            return;

        case 94:
            Fun_94(  );     // Pinpad test
            return;

// temporary does not supported
//			case 95:
//				Fun_95(  );		// Network Statistics
//				return;

        case 97:
            Fun_97(  );     // Local Options
            return;

        case 98:
            Fun_98(  );     // Local Configuration
            return;

        case 99:
            Fun_99(  );     // Local Functions
            return;

        default:            // Unknown, notify user
            ShowErrMsg( InvalidEntry );
            break;
        }
    }
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Test Transaction
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_0( void )
{
    UBYTE status, num_hosts;

    num_hosts = AqidCount(  );  // count the number of acquirers

    // we don't have any acquirer, just return
    if ( num_hosts == 0 )
        return;

    // if we have only one acquirer in acquirer table, don't need to pop up
    // display the host menu. Just set the proper data to TRINP
    if ( num_hosts == 1 )
    {
        TRINP.TRAQID  = AQTAB[0].AQID;
        TRINP.TRAQPTR = &AQTAB[0];
    }
    else
    {
        DrawMenu( MENU_HOST_TESTTRAN );

        do                      // Call the selected option processor
        {

            status = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

            if ( status == B_PREVIOUS )
            {
                DrawMenu( MENU_PREVIOUS );
            }
            // Go forward one menu level
            else if ( status == B_NEXT )
            {
                DrawMenu( MENU_NEXT );
            }
            else if ( ( status == CANCEL_KY ) || ( status == 0 ) ||
                      ( status == CLEAR_KY ) )
            {
                break;          // cancel or Time-Out
            }
            else if ( ( status >= 'A' ) && ( status <= 'A' + 7 ) )
            {
                TRINP.TRAQID  = AQTAB[status - 'A'].AQID;
                TRINP.TRAQPTR = &AQTAB[status - 'A'];
                break;
            }
            else if ( status == B_PREVIOUS_NEW_MENU )
            {

                ButtonId = B_PREVIOUS_NEW_MENU;
                status = 0;
                break;
            }

        }
        while ( status && ( status != CANCEL_KY ) &&
                ( status != CLEAR_KY ) && !( CurrentEvent.NEXTEVENT ) );

        if ( ( status == CANCEL_KY ) || ( status == CLEAR_KY ) ||
             ( status == 0 ) )
        {
            CurrentEvent.NEXTEVENT = 0;
            return;
        }
    }

    // Sucessfully got AQID
    // Set the transaction to Test Transaction
    TRINP.TRKEY = TESTTRAN;
    CurrentEvent.NEXTEVENT = TESTTRAN;
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Merchant Programming
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_2( void )
{
    UBYTE Status;

    // Get Maintenance Password
    GetMsg( MerchProg, DefaultTitle );
    Status = EnterMPswd(  );
    if ( Status )
    {
        // Okay; see if Maintenance Password disabled
        if ( 2 == Status )
        {
            // Yes; save and verify the Input Password
            if ( !InputPswd( MerchProg ) )
            {
                // Bad; return to caller
                return;
            }
        }

        TermAll(  );
    }
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Restart Terminal
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_3( void )
{
    Bool loopflag;

    SDK_GuiClearScr(  );

    SetUserMessage( MSG_USER3, N_Reset );
    SetUserMessage( MSG_USER4, N_Quick );

    SetUserMessage( MSG_USER7, RestartTerminal );

    SetUserMessage( MSG_USER1, N_NullStr );
    SetUserMessage( MSG_USER2, N_NullStr );

    SetUserEvent( EVENT_USER3, CLEAR_KY );
    SetUserEvent( EVENT_USER4, ENTER_KY );

    DrawScreen( TEMPLATE_2BTNQUERY_SCR );
    SDK_GuiDispAll();

    // Wait for a key press or timeout
    do
    {
        loopflag = False;

        switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
        {
        case CLEAR_KY:
            // Wait for 1/2 second
            SDK_Wait( TENMS * 50 );

            // Restart Terminal
            SDK_TerminalReset(  );
            break;

        case ENTER_KY:
            // Quick reset
            TerminalQuickReset( );
            break;

        case CANCEL_KY:
        case 0:
            break;

        default:
            // Bad key, error beep
            SDK_Beeper( TENMS * 40 );
            loopflag = True;
            break;
        }
    }
    while ( loopflag );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Lock Keyboard
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_4( void )
{
    UBYTE keyval;

    // Prompt to Lock Keyboard
    YNEntry.TitleMid = N_NullStr;
    YNEntry.Line1Mid = KeyLock;
    YNEntry.Line2Mid = N_NullStr;

    // Prompt for YES or NO Keys
    keyval = YesNoEntry( &YNEntry );
    if ( ENTER_KY == keyval )   // If YES Key
    {
        // Lock the Kyboard
        TCONF.TCLOCOP |= TCL_LOCKED;

        // Set TCONF's LRC
        SetTCTabLRC(  );
    }
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Open/Close/Report Tab.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void Fun_20( void )
{
    TabProcessing ();
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Find an entry in batch and start reviewing batch.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void Fun_21( void )
{
    Find ();
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Accept input for the receipt/report header/footer
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_26( void )
{

    UBYTE keyval;
    UBYTE Length;
    UBYTE *pTextLineField;
    UBYTE TextSize;

    // Calculate maximum number of characters to display on a line, in screen_limit
    MaxCharDisp ();

    // Loop till they are done entering text data
    while ( 1 )
    {

        // Set up the Footer Buttons
        GetMsg( Msg_FooterLine, CSTMSG );
        GetMsg( Msg_FooterLine, CSTMSG2 );
        GetMsg( Msg_FooterLine, CSTMSG3 );
        GetMsg( Msg_FooterLine, CSTMSG4 );
        Length = StrLn( CSTMSG, sizeof( CSTMSG ) );
        CSTMSG[Length] = '1';
        CSTMSG[Length + 1] = '\0';
        CSTMSG2[Length] = '2';
        CSTMSG2[Length + 1] = '\0';
        CSTMSG3[Length] = '3';
        CSTMSG3[Length + 1] = '\0';
        CSTMSG4[Length] = '4';
        CSTMSG4[Length + 1] = '\0';

// Prompt user to select header or footer line
        DrawScreen( HEADER_FOOTER_SCR );

        keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );
        switch ( keyval )
        {
        case 0:         // User Timeout
        case CANCEL_KY: // User Aborted Transaction
            return;
        case '0':
        case 'H':           // User Selected Header Line 4
            // Set first line of display to show Header text
            GetMsg( Msg_HeaderLine, CSTMSG7 );

            // Set Pointer to Point to Header Table
            pTextLineField = USRDTAB;

            // Set Size of the Field
            TextSize = USRDSZE;
            break;

        default:                
            keyval -= 0x30;
            // See if User selected a Footer Text Line
            if ( ( 1 <= keyval ) && ( 4 >= keyval ) )
            {
                // Set first line of display to show footer text
                GetMsg( Msg_FooterLine, CSTMSG7 );
                Length = StrLn( CSTMSG7, sizeof( CSTMSG7 ) );
                CSTMSG7[Length++] = ( keyval + 0x30 );
                CSTMSG7[Length] = '\0';

                // Set Pointer to Point to Header Table
                pTextLineField =
                ( UBYTE * ) ( FTTAB.FTFOOTER1 +
                              ( ( keyval - 1 ) * S_FOOTER ) );
                // Set Size of the Field
                TextSize = S_FOOTER;
                break;
            }
            else
            {
                // Invalid Value, beep them
                SDK_Beeper( TENMS * 40 );
                continue;
            }
        }

        // If text exists, Prepare to Display it
        if ( !NullComp( ( char * ) pTextLineField, TextSize ) &&
             !SpaceComp( ( char * ) pTextLineField, TextSize ) )
        {
            // Yes; check if too big for a single line
            if ( TextSize > screen_limit )
            {
                // Yes; move first part of text into CSTMSG
                memcpy( CSTMSG, pTextLineField, ( UWORD ) screen_limit );

                // Null terminate the first display line
                CSTMSG[screen_limit] = '\0';

                // Move the remainder into CSTMSG3
                memcpy( CSTMSG3, &pTextLineField[screen_limit],
                        ( UWORD ) ( TextSize - screen_limit ) );

                // Null terminate the second display line
                CSTMSG3[TextSize - screen_limit] = '\0';
            }
            else
            {
                // No; move text into CSTMSG
                memcpy( CSTMSG, pTextLineField, TextSize );

                // Null terminate the first display line
                CSTMSG[TextSize] = '\0';

                // Null terminate the second display line
                CSTMSG3[0] = '\0';
            }
        }
        else
        {
            // Field Empty, Nothing to display
            GetMsg( BlankMsg, CSTMSG );
            GetMsg( BlankMsg, CSTMSG3 );
        }

        // Set zap key
        GlobalDsp = ZAPKEYON;

        keyval = AlphaNumEntry( 1, TextSize, 0, &ReceiptEntry );

        // See if time to abort Transaction
        if ( !keyval )
            return;

        // Valid data has been entered. Save it & update the LRC
        // If Zap is returned, it means the user wants to clear
        // the line of text. Update the USRDTAB as such.
        // If no data entered, it means the user wants to leave
        // the line of text as it currently is.

        // Get the length of the text entered
        Length = StrLn( Dspbuf, sizeof( Dspbuf ) );

        // See if user accepted the current entry
        if ( ( !Length ) && ( 1 == keyval ) )
        {
            // Yes; Get Next Value
            continue;
        }

        // If ZAPKEY (BACKSPACE ) Then Clear the field out, else set it
        if ( 1 == keyval )
        {
            // See if any text entered
            if ( Length )
            {
                // Yes; save the entered text
                memcpy( pTextLineField, Dspbuf, Length );
            }

            // See if need to space fill the text
            if ( Length != TextSize )
            {
                // Yes; space fill the text
                memset( ( char * ) ( pTextLineField + Length ), ' ',
                        ( UWORD ) ( TextSize - Length ) );
            }
        }
        else
        {
            // Was ZAPPED, Clear out text field
            memset( ( char * ) ( pTextLineField ),' ', ( UWORD ) TextSize );
        }

        // Need to set LRC for the table that is being updated
        if ( pTextLineField == USRDTAB )
        {
            SetUsrdTabLRC();
        }
        else
        {
            // Set the Footer Table LRC
            SetFTTabLRC(  );
        }
    }                       // End of while loop
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Toggle the card read between the S8 and T7
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void Fun_31( void )
{
    UBYTE keyval;

    // Check if terminal has is configured for swipe from pinpad.
    if ( (TCONF.TCPINPAD & TCP_MSR) && (0xFF == TERM.TERMInit) )
    {
        YNEntry.TitleMid = N_Pinpad;
        YNEntry.Line2Mid = N_NullStr;

        // Check current status of pinpad card read
        if ( PINSTAT & PinCard_Read )
        {
            YNEntry.Line1Mid = ChangeToTermRead;
            keyval = YesNoEntry( &YNEntry );
            if ( ENTER_KY == keyval )   // If YES
            {
                // Card read at pinpad ON, so turn it OFF
                PINSTAT &= ~PinCard_Read;
            }
        }
        else
        {
            YNEntry.Line1Mid = ChangeToPinRead;
            keyval = YesNoEntry( &YNEntry );
            if ( ENTER_KY == keyval )   // If YES
            {
                // If not external, force an auto-detect.
                if ( PIN_TYPE_INTERNAL == PIN_TYPE )
                {
                    // Verify external pinpad before allowing change.
                    // Send message to pinpad to display
                    PinMessage ( Msg_ClrScrn );
                }

                // If external pin pad found then allow switch.
                if ( PIN_TYPE_INTERNAL != PIN_TYPE )
                {
                    // Card read at pinpad OFF so turn it ON
                    PINSTAT |= PinCard_Read;
                }
                else
                {
                    ShowErrMsg( ExtPinNot );
                }
            }
        }
    }
    else
    {
        FuncNotSupported ();
    }
}


#ifdef MAKE_EMV
//-----------------------------------------------------------------------------
//!  \brief
//!     Enable EMV exception file flag
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
void Fun_34( void )
{
    UBYTE keyval;

    YNEntry.TitleMid = CustomMsg;
    YNEntry.Line1Mid = CustomMsg;
    YNEntry.Line2Mid = CustomMsg2;

    // AppCapTxnLog: Whether we use prev amount from journal
    memcpy( CSTMSG,  "Txn Log supported", 18 );
    if (LocalAppCap[1] & APP_CAP2_TXN_LOG)
    {
        memcpy( CSTMSG2, "Disable ?", 10 );
    }
    else
    {
        memcpy( CSTMSG2, "Enable ?",  9 );
    }

    // Prompt for YES or NO Keys 
    keyval = YesNoEntry( &YNEntry );

    if ( ENTER_KY == keyval )   // If YES Key toggle bit
    {
        ToggleBit (&LocalAppCap[1], APP_CAP2_TXN_LOG);
    }

    // Exception File ?
    memcpy( CSTMSG,  "Exception File", 15 );
    if (LocalAppCap[1] & APP_CAP2_EXC_FILE_TEST)
    {
        memcpy( CSTMSG2, "Disable ?", 10 );
    }
    else
    {
        memcpy( CSTMSG2, "Enable ?",  9 );
    }
    // Prompt for YES or NO Keys 
    keyval = YesNoEntry( &YNEntry );

    if ( ENTER_KY == keyval )   // If YES Key
    {
        ToggleBit (&LocalAppCap[1], APP_CAP2_EXC_FILE_TEST);
    }

    memcpy( CSTMSG,  "Force Online Prompt", 20 );
    if (LocalAppCap[1] & APP_CAP2_MERCH_SUSP)
    {
        memcpy( CSTMSG2, "Disable ?", 10 );
    }
    else
    {
        memcpy( CSTMSG2, "Enable ?",  9 );
    }
    // Prompt for YES or NO Keys 
    keyval = YesNoEntry( &YNEntry );

    if ( ENTER_KY == keyval )   // If YES Key
    {
        ToggleBit (&LocalAppCap[1], APP_CAP2_MERCH_SUSP);
    }

    memcpy( CSTMSG,  "Forced Sale Prompt", 19 );
    if (LocalAppCap[1] & APP_CAP2_EMV_FORCED)
    {
        memcpy( CSTMSG2, "Disable ?", 10 );
    }
    else
    {
        memcpy( CSTMSG2, "Enable ?",  9 );
    }
    // Prompt for YES or NO Keys 
    keyval = YesNoEntry( &YNEntry );

    if ( ENTER_KY == keyval )   // If YES Key
    {
        ToggleBit (&LocalAppCap[1], APP_CAP2_EMV_FORCED);
    }

    PrintAppCap();
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Toggle one bit of a byte
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//! 
//!  \note
//!     Created to take care of the bitmap flags
//!
//-----------------------------------------------------------------------------
static void ToggleBit (UBYTE * pByteValue, UBYTE BitToggler)
{

    if (*pByteValue & BitToggler)
    {
        *pByteValue &= ~BitToggler;
    }
    else
    {
        // turn bit on
        *pByteValue |= BitToggler;
    }

}

//-----------------------------------------------------------------------------
//!  \brief
//!     Enable EMV debug printing
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void Fun_37( void )
{

    UBYTE keyval;

    switch ( PrintEMVDebug )
    {
    case True:
        GetMsg( EmvInfoEna, CSTMSG );
        GetMsg( DisEmvInfo, CSTMSG2 );

        YNEntry.TitleMid = CustomMsg;
        YNEntry.Line1Mid = CustomMsg2;
        break;

    case False:
        GetMsg( EmvInfoDis, CSTMSG );
        GetMsg( EnaEmvInfo, CSTMSG2 );

        YNEntry.TitleMid = CustomMsg;
        YNEntry.Line1Mid = CustomMsg2;
        break;
    }
    YNEntry.Line2Mid = N_NullStr;


    // Prompt for YES or NO Keys 
    keyval = YesNoEntry( &YNEntry );

    if ( ENTER_KY == keyval )   // If YES Key
    {
        switch ( PrintEMVDebug )
        {
        case True:
            PrintEMVDebug = False;
            break;

        case False:
            PrintEMVDebug = True;
            break;
        }
    }
}
#endif //MAKE_EMV


//-----------------------------------------------------------------------------
//!  \brief
//!     Set Max Tries for Init Recovery mechanism
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
void Fun_54 ( void )
{
    UBYTE retval;
    UBYTE length;

    /* Get the 'Currently' Message */
    GetMsg( Currently, CSTMSG );

    /* Get the length of the prompt */
    length = strlen( CSTMSG );

    /* Blank out the Nul terminator */
    CSTMSG[length++] = ' ';

    /* We need to show the counter as BCD */
    /* Show 'FF' as '00' (Recovery Disabled) */
    retval = (0xFF != TFTAB.TFMAXTRY) ? (UBYTE)CvtBcd(TFTAB.TFMAXTRY) : 0x00;

    /* Convert from BCD to ASCII */
    BfAscii( &CSTMSG[length++], &retval, 1 );

    /* Null terminate the result for display */
    CSTMSG[++length] = '\0';

    retval = NumEntry( 1, 2, 0, &InitTries_Entry);

    if ( 1 == retval )
    {
        /* Check if anything was entered or just 'Enter' was pressed */
        if ( Dspbuf[0] )
        {
            /* Data entered, convert ascii to bcd */
            retval = Dspbuf[0] - '0';

            if ( Dspbuf[1] )
            {
                retval = retval*10 + (Dspbuf[1]-'0');
            }

            if ( 0 == retval )
            {
                /* 0xFF means 'Disable Recovery mechanism' */
                retval = 0xFF;
            }

            TFTAB.TFMAXTRY = retval;

            /* Set the TFTAB Table LRC to save change */
            SetTFTabLRC();
        }
    }
}

#ifdef	MAKE_SMARTINIT               
//-----------------------------------------------------------------------------
//!  \brief
//!     Toggle SmartInit
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void Fun_55( void )
{
    UBYTE keyval;

    switch ( SmartInitEnabled )
    {
    case True:
        GetMsg( SmartInitEna, CSTMSG );
        GetMsg( DisSmartInit, CSTMSG2 );

        YNEntry.TitleMid = CustomMsg;
        YNEntry.Line1Mid = CustomMsg2;
        break;

    case False:
        GetMsg( SmartInitDis, CSTMSG );
        GetMsg( EnaSmartInit, CSTMSG2 );

        YNEntry.TitleMid = CustomMsg;
        YNEntry.Line1Mid = CustomMsg2;
        break;
    }
    YNEntry.Line2Mid = N_NullStr;

    // Prompt for YES or NO Keys
    keyval = YesNoEntry( &YNEntry );

    if ( ENTER_KY == keyval ) // If YES Key
    {
        switch ( SmartInitEnabled )
        {
        case True:
            SmartInitEnabled = False;
            break;

        case False:
            SmartInitEnabled = True;
            break;
        }
    }
}
#endif // MAKE_SMARTINIT

//-----------------------------------------------------------------------------
//!  \brief
//!     Printer Enable/Disable
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_71( void )
{
    UBYTE keyval;

    // Check if Printer Enabled or Disabled
    if ( TCONF.TCOPT2 & TC2_PRINT )
    {
        // Test for printer logically enabled
        if ( PRTSTAT & 0x01 )
        {
            // Prompt Printer Enabled


            YNEntry.TitleMid = N_Printer;
            YNEntry.Line1Mid = PrinterEP;
            YNEntry.Line2Mid = DisablePP;

            keyval = YesNoEntry( &YNEntry );
            if ( ENTER_KY == keyval )   // If YES       
            {
                // Disable Printer
                PRTSTAT = 0x00;
            }
        }
        else
        {
            // Prompt Printer Disabled
            YNEntry.TitleMid = N_Printer;
            YNEntry.Line1Mid = PrinterDP;
            YNEntry.Line2Mid = EnablePP;

            keyval = YesNoEntry( &YNEntry );

            if ( ENTER_KY == keyval )   // If YES       
            {
                // Enable Printer
                PRTSTAT = 0x01;
            }
        }
    }
    else
    {
        // No Printer detected
        ShowErrMsg( NoPrinter );
        SDK_Beeper( TENMS * 40 );
    }
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Display Test
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_83( void )
{
    UBYTE m, i;
    UBYTE Step = 1;
    UBYTE retval;

    // Start with Normal Screen
    Enable_Flags[3] = 0;

    // repeat until EXIT button pressed
    while ( 1 )
    {
        if ( 1 == Step )
        {
            // Fill 1-st line by ASCII chars 0x20..0x2F
            memcpy ( CSTMSG, (UBYTE*) "20| ", 4 );
            m = 0x20;
            for (i=0; i<16; i++)
                CSTMSG[i+4] = i + m; 
            CSTMSG[20] = 0;

            // Fill 2-nd line by ASCII chars 0x30..0x3F
            memcpy ( CSTMSG2, (UBYTE*) "30| ", 4 );
            m = 0x30;
            for (i=0; i<16; i++)
                CSTMSG2[i+4] = i + m; 
            CSTMSG2[20] = 0;

            // Fill 3-rd line by ASCII chars 0x40..0x4F
            memcpy ( CSTMSG3, (UBYTE*) "40| ", 4 );
            m = 0x40;
            for (i=0; i<16; i++)
                CSTMSG3[i+4] = i + m; 
            CSTMSG3[20] = 0;

            // Fill 4-th line by ASCII chars 0x50..0x5F
            memcpy ( CSTMSG4, (UBYTE*) "50| ", 4 );
            m = 0x50;
            for (i=0; i<16; i++)
                CSTMSG4[i+4] = i + m; 
            CSTMSG4[20] = 0;

            // Fill 5-th line by ASCII chars 0x60..0x6F
            memcpy ( CSTMSG5, (UBYTE*) "60| ", 4 );
            m = 0x60;
            for (i=0; i<16; i++)
                CSTMSG5[i+4] = i + m; 
            CSTMSG5[20] = 0;
        }
        else if ( 2 == Step )
        {
            // Fill 1-st line by ASCII chars 0x70..0x7F
            memcpy ( CSTMSG, (UBYTE*) "70| ", 4 );
            m = 0x70;
            for (i=0; i<16; i++)
                CSTMSG[i+4] = i + m; 
            CSTMSG[20] = 0;

            // Fill 2-nd line by ASCII chars 0x80..0x8F
            memcpy ( CSTMSG2, (UBYTE*) "80| ", 4 );
            m = 0x80;
            for (i=0; i<16; i++)
                CSTMSG2[i+4] = i + m; 
            CSTMSG2[20] = 0;

            // Fill 3-rd line by ASCII chars 0x90..0x9F
            memcpy ( CSTMSG3, (UBYTE*) "90| ", 4 );
            m = 0x90;
            for (i=0; i<16; i++)
                CSTMSG3[i+4] = i + m; 
            CSTMSG3[20] = 0;

            // Fill 4-th line by ASCII chars 0xA0..0xAF
            memcpy ( CSTMSG4, (UBYTE*) "A0| ", 4 );
            m = 0xA0;
            for (i=0; i<16; i++)
                CSTMSG4[i+4] = i + m; 
            CSTMSG4[20] = 0;

            // Fill 5-th line by ASCII chars 0xB0..0xBF
            memcpy ( CSTMSG5, (UBYTE*) "B0| ", 4 );
            m = 0xB0;
            for (i=0; i<16; i++)
                CSTMSG5[i+4] = i + m; 
            CSTMSG5[20] = 0;
        }
        else if ( 3 == Step )
        {
            // Fill 1-st line by ASCII chars 0xC0..0xCF
            memcpy ( CSTMSG, (UBYTE*) "C0| ", 4 );
            m = 0xC0;
            for (i=0; i<16; i++)
                CSTMSG[i+4] = i + m; 
            CSTMSG[20] = 0;

            // Fill 2-nd line by ASCII chars 0xD0..0xDF
            memcpy ( CSTMSG2, (UBYTE*) "D0| ", 4 );
            m = 0xD0;
            for (i=0; i<16; i++)
                CSTMSG2[i+4] = i + m; 
            CSTMSG2[20] = 0;

            // Fill 3-rd line by ASCII chars 0xE0..0xEF
            memcpy ( CSTMSG3, (UBYTE*) "E0| ", 4 );
            m = 0xE0;
            for (i=0; i<16; i++)
                CSTMSG3[i+4] = i + m; 
            CSTMSG3[20] = 0;

            // Fill 4-th line by ASCII chars 0xF0..0xFF
            memcpy ( CSTMSG4, (UBYTE*) "F0| ", 4 );
            m = 0xF0;
            for (i=0; i<16; i++)
                CSTMSG4[i+4] = i + m; 
            CSTMSG4[20] = 0;

            // Fill 5-th line by ASCII chars 0x60..0x6F
            CSTMSG5[0] = 0;
        }

        // Draw the base input screen
        DrawScreen( TST_DSPLY_SCR );

        // Get Keyboard Entry
        retval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

        // Check if User pressed the TEST Button
        if ( 'T' == retval )
        {
            // Find the status ob Enable Flag, and invert it
            if ( 0 == Enable_Flags[3] )
            {
                Enable_Flags[3] = 1;
            }
            else
                Enable_Flags[3] = 0;
        }

        // NEXT button pressed - show next part of character set
        if ( 'N' == retval )
        {
            Step++;

            if ( Step == 4)
                Step = 1;
        }
        // Exit if Timeout or CANCEL Key
        if ( ( CANCEL_KY == retval ) || ( !retval ) )
            return;
    }
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Keyboard Test
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_84( void )
{
    UBYTE retval, count = 0;
    char LoopFlag = True;
    char *v;

    // Set the title
    GetMsg( KeyBrdTest, CSTMSG );

    // Get the COL/ROW Message
    GetMsg( DispKeyP, CSTMSG3 );

    // Enable first line of text
    Enable_Flags[3] = 1;
    GetMsg( MsgPressCancelToExit, CSTMSG2 );

    // Disable QUIT button
    Enable_Flags[4] = 0;

    // Point to the end of the string
    v = CSTMSG3 + StrLn( CSTMSG3, sizeof( CSTMSG3 ) );

    // repeat until EXIT button pressed
    while ( LoopFlag )
    {
        // Draw the base input screen
        DrawScreen( TEST_SCR );

        // Get Keyboard Entry
        retval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

        // Prepare to exit if "CONTINUE" button was pressed

        // Exit if timed out
        if ( 0 == retval )
            LoopFlag = False;

        switch ( retval )
        {
        case ENTER_KY:
            GetMsg( N_Enter, v );
            count = 0;
            break;

        case CLEAR_KY:
            GetMsg( N_Clear, v );
            count = 0;
            break;

        case CANCEL_KY:
            SDK_BeepIt( 3 );
            GetMsg( N_Cancel, v );
            DrawScreen( TEST_SCR );
            SDK_Wait( ONESECOND * 2 );
            LoopFlag = False;
            SDK_BeepIt( 1 );
            break;

        case DOWN_KY:
            GetMsg( N_Down, v );
            count = 0;
            break;

        case UP_KY:
            GetMsg( N_Up, v );
            count = 0;
            break;

        case SELECT_KY:
            GetMsg( N_Select, v );
            count++;
            // On Viking and T7PLUS exit upon 2 cancels in a row.
            if ( 2 == count ) LoopFlag = False;
            break;

        default:
            *v = retval;
            *( v + 1 ) = 0;
            count = 0;
            break;
        }
    }

    // Reset the title
    DefaultTitle[0] = 0;
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Cardreader Test
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_89( void )
{
    UBYTE i, draw;
    UBYTE InputType;
    Bool bPinCardStatus;

    draw = 0;
    Enable_Flags[3] = 0;        // Turn off second line of display

    // Loop till done
    do
    {
        if ( !draw )
        {
            draw = 1;
            Enable_Flags[3] = 0;    // Turn off second line of display

            // See if card read is on Pinpad
            // PinCard_Read is set at startup based on TermMaster settings.
            if ( PINSTAT & PinCard_Read )
            {
                // First force an autodetect on the pinpad so autodetect
                // message will not interfere with display of swipe msg.
                // Send message to pinpad to display
                PinMessage ( Msg_ClrScrn );

                // Card read is on the pinpad.
                GetMsg( SwipeCardPin, Dspbuf );;
            }
            else
            {
                // Card read is on the terminal.
                GetMsg( Swipecard, CSTMSG );
            }
            DrawScreen( CARDTEST_SCR );


        }

        // Relinquish the CPU between card reads.
        SDK_RelinqCPU(  );

        if ( PINSTAT & PinCard_Read )
        {
            // Card read is on the pinpad, reset the Pinpad
            PinReset ();

            // Start card read on Pinpad
            // See if good request
            if ( PinCardRead () )
            {
                // Request good: Loop until card read or key pressed.
                do
                {
                    // See if keypad or keyboard pressed.
                    InputType = read_io( DEV_PAD + DEV_KBD + STAY_OPEN0, 0, &bPinCardStatus, &DevFlag );

                    switch ( InputType )
                    {
                    case DEV_PAD:
                        KeyVal = CANCEL_KY; // Yes; exit the loop
                        break;
                    case DEV_KBD:
                        // Yes; check for CANCEL_KY Key
                        if ( CLEAR_KY == KeyVal )
                            KeyVal = CANCEL_KY; // Yes; exit the loop

                        if ( CANCEL_KY != KeyVal )
                        {
                            SDK_Beeper( TENMS * 40 );   // No; notify the user
                        }
                        break;
                    }

                }
                while ( ( KeyVal != CANCEL_KY ) &&
                        !PinCardReadStatus.CompleteFlag );
            }
        }

        // See if card read is on Terminal
        if ( !( PINSTAT & PinCard_Read ) )
        {
            InputType = read_io( DEV_PAD + DEV_KBD + DEV_CARDRDR + STAY_OPEN0, 0, &bPinCardStatus, &DevFlag );
            switch ( InputType )
            {
            case DEV_CARDRDR:
                if ( Trk2Status.CompleteFlag != 0 )
                {
                    // Check if Track 2 data has been received
                    if ( ( Trk2Status.Error == orvOK ) &&
                         ( Trk2Status.CompleteFlag ) && 
                         ( 2 < Trk2ReadLength ) )
                    {
                        // Yes; good read, so display TK 2 Card Data
                        for ( i = 0; i < Trk2ReadLength; i++ )
                        {
                            // Convert to ASCII
                            TRINP.TRTRK2[i] += 0x30;
                        }

                        Trk2TOCstMsg(  );
                        DrawScreen( CARDTEST_SCR );

                        memset( TRINP.TRTRK2, 0, S_TRTRK2 );
                    }
                    else
                    {
                        ShowErrMsg( CardErr );
                        draw = 0;
                    }
                }
                break;
            case DEV_PAD:
                KeyVal = CANCEL_KY; // Yes; exit the loop
                break;
            case DEV_KBD:
                // Yes; check for CANCEL_KY Key
                if ( CLEAR_KY == KeyVal )
                    KeyVal = CANCEL_KY; // Yes; exit the loop

                if ( CANCEL_KY != KeyVal )
                    SDK_Beeper( TENMS * 40 );   // No; notify the user

                break;
            }
        }

        // Display the card data if the card was read on the Pinpad
        if ( ( PINSTAT & PinCardReadReqPend ) && ( KeyVal != CANCEL_KY )
             && ( PinCardReadStatus.CompleteFlag ) )
        {
            // Yes; check if card read was okay
            // Yes; get card read response from Pinpad
            // Check if Track 2 data has been received
            if ( ( PinCardReadStatus.Error == orvOK ) && PinCardReadResp () )
            {
                // Yes; good read, so display TK 2 Card Data
                Trk2TOCstMsg(  );
                DrawScreen( CARDTEST_SCR );

                memset( TRINP.TRTRK2, 0, S_TRTRK2 );

                // Notify user
                SDK_BeepIt( 1 );
            }
            else
            {
                ShowErrMsg( CardErr );
                draw = 0;
            }
        }
    }
    while ( KeyVal != CANCEL_KY );

    // Close the pinpad if card reads from pinpad.
    if ( PINSTAT & PinCard_Read )
    {
        // Card read is on the pinpad, reset the Pinpad
        PinReset ();
    }
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Password Change
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_90( void )
{
    UBYTE status;
    UBYTE TempPbuf[4];          // Temp Password buffer

    // Save and verify the Input Password

    // Do until correct Password entered or cancel
    do
    {
        // Prompt for Input Password (4 Digits)
        status = NumEntry( 1, 4, 4, &Fun90Entry );

        // Check if Input Password has been entered
        if ( status )
        {
            // Yes; save the Input Password entered
            memcpy( TRINP.TRPWD, Dspbuf, 4 );

            // Verify Input Password entered
            if ( 0 != memcmp( TRINP.TRPWD, ( UBYTE * ) TERM.TERMPwd, 4 ) )
            {
                // incorrect; stay in the loop
                SDK_Beeper( TENMS * 40 );
                status = 2;
            }
        }
        else
        {
            // No; cancel, exit the loop
            return;
        }
    }
    while ( status == 2 );

    // Do until correct Password entered or cancel
    do
    {
        // Prompt for Input Password (4 Digits)
        status = NumEntry( 1, 4, 4, &Fun90Entry1 );

        // Check if Input Password has been entered
        if ( status )
        {
            // Yes; save the Password entered
            memcpy( TempPbuf, Dspbuf, 4 );

            // Prompt for Input Password Again (4 Digits)
            status = NumEntry( 1, 4, 4, &Fun90Entry2 );

            // Check if Input Password has been entered
            if ( 1 == status )
            {
                // Yes; save the Input Password entered
                memcpy( TRINP.TRPWD, Dspbuf, 4 );

                // Verify Input Password entered
                if ( 0 == memcmp( TRINP.TRPWD, TempPbuf, 4 ) )
                {
                    // Copy new password into right area
                    memcpy( ( UBYTE * ) TERM.TERMPwd, ( UBYTE * ) TRINP.TRPWD,
                            4 );

                    // Recalculate term table Checksum
                    SetTERMTabLRC(  );
                }
                else
                {
                    // Passwords do not match, notify user
                    ShowErrMsg( PswdNotMatch );
                    status = 2;
                }
            }
        }
    }
    while ( status == 2 );

    // Display TRANSACTION ACCEPTED and Beep Twice
    if ( status )
    {
//FIXME?		Parm2 = N_PswdChange;	// title for transaction accepted
        TranAcc(  );
    }
    return;
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Show Software Version
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_91( void )
{
    VERSION_INFO sdkversion;
    UBYTE retval;
#ifdef MAKE_EMV
    UBYTE ver[25]; // temp array for storing version strings
    UBYTE Z8VersionID; // version ID not used in Fun 91 but required for ScrdVersion
#endif // MAKE_EMV

    // Display Cancel Box
    Enable_Flags[0] = 1;
    // Display the software version
    ShowSWVersion();

#ifdef MAKE_EMV
    /* repeat until EXIT button pressed from first screen */
    do
    {
        retval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout);
        switch (retval)
        {
        case 0:
        case CLEAR_KY:
        case CANCEL_KY:
            return;
        case ENTER_KY:
            break;
        default:
            // Bad key, error beep
            SDK_Beeper( TENMS * 40 );
        }
    } while (retval != ENTER_KY);

    // Add EMV L1 and L2 version strings
    strcpy ( CSTMSG, "EMVL1" );

    memset( ver, 0, sizeof(ver) );
    ScrdVersion ( (char *) ver, &Z8VersionID );
    strcpy( CSTMSG2, (char*)ver );

    // Read the library version string
    strcpy ( CSTMSG3, "EMVL2 " );

    memset( ver, 0, sizeof(ver) );
    EMVL2_GetVersionString( ver );
    strcpy( CSTMSG4, (char*)ver );

    /* Display Screen */
    DrawScreen( EMV_VER_SCR );
#endif // MAKE_EMV

    // repeat until EXIT button pressed
    do
    {
        retval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout);
        switch (retval)
        {
        case 0:
        case CLEAR_KY:
        case CANCEL_KY:
            return;
        case ENTER_KY:
            break;
        default:
            // Bad key, error beep
            SDK_Beeper( TENMS * 40 );
        }
    } while (retval != ENTER_KY);

    SDK_Version( &sdkversion );

    strcpy( CSTMSG, sdkversion.SDKplatform );
    strcat( CSTMSG, "_" );
    strcat( CSTMSG, sdkversion.SDKversion );
//    strcat( CSTMSG, "_" );
//    strcat( CSTMSG, sdkversion.SDKvariant );

    strcpy( CSTMSG2, sdkversion.HFCversion );

    strcpy( CSTMSG3, sdkversion.OSrelease );

    strcpy( CSTMSG4, sdkversion.OSversion );

    DrawScreen( LIBS_VER_SCR );

    // repeat until EXIT button pressed
    do
    {
        retval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout);
        switch (retval)
        {
        case 0:
        case CLEAR_KY:
        case CANCEL_KY:
            return;
        case ENTER_KY:
            break;
        default:
            // Bad key, error beep
            SDK_Beeper( TENMS * 40 );
        }
    } while (retval != ENTER_KY);

}



//-----------------------------------------------------------------------------
//!  \brief
//!     PinPad TEST Message
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_94( void )
{
    UBYTE retval;
    char LoopFlag = True;
    Bool bPinRetVal;

    // Reset PINPAD	variables
    PIN_TYPE = PIN_TYPE_INVALID;
    PINSTAT = 0;
    PIN_DEVICE = 0;

    // Store Pinpad auto-detect flag
    bPinRetVal= TCONF.TCOPT1 & TC1_PINDETECT;
    // Enables Pinpad auto-detect flag for PinInit
    TCONF.TCOPT1|= TC1_PINDETECT;
    // First, try to autodetect the pinpad
    PinInit( );
    // Reset Pinpad auto-detect flag if it originally was clear
    if ( !bPinRetVal )
        TCONF.TCOPT1&= ~TC1_PINDETECT;

    // Select * PINPAD TEST * message to display on pinpad
    // Send message to pinpad to display
    bPinRetVal = PinMessage ( Msg_PinTest );

    // Set the title
    GetMsg( PinPadTest, CSTMSG );

    // Disable First line of text
    Enable_Flags[3] = 0;

    // Display results of test.
    if ( bPinRetVal )
    {
        GetMsg( N_Passed, CSTMSG3 );
        SDK_BeepIt( 2 );
    }
    else
    {
        GetMsg( N_Failed, CSTMSG3 );
        SDK_Beeper( TENMS * 40 );
    }

    // Enable QUIT button
    Enable_Flags[4] = 1;

    // Display Screen
    DrawScreen( TEST_SCR );

    // Display until EXIT button pressed
    while ( LoopFlag )
    {
        // Get Keyboard Entry
        retval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

        // Exit if timeout, 'x' or Exit pressed
        if ( ( CLEAR_KY == retval ) || 
             ( SELECT_KY == retval ) ||
             ( 'C' == retval ) || 
             ( 0 == retval ) || 
             ( CANCEL_KY == retval ) || 
             ( ENTER_KY == retval ) )
            LoopFlag = False;
    }
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Network Statistics
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
/*
static void Fun_95( void )
{
    UBYTE retval;
    char LoopFlag = True;

    // Convert the Transmit Error count to ASCII
    BfAscii( CSTMSG, (const UBYTE*)NMSDAT.NMReTxC, 2 );

    // Null terminate the Transmit Error line
    CSTMSG[4] = '\0';

    // Convert the Receive Error count to ASCII
    BfAscii( CSTMSG2, (const UBYTE*)NMSDAT.NMRxErr, 2 );

    // Null terminate the Receive Error line
    CSTMSG2[4] = '\0';

    // Display Screen
    DrawScreen( STATISTICS_SCR );

    // Display until EXIT button pressed
    while ( LoopFlag )
    {
        // Get Keyboard Entry
        retval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

        // Exit if timeout, 'x' or Exit pressed
        if ( ( CLEAR_KY == retval ) || ( 'C' == retval ) || ( 0 == retval )
             || ( CANCEL_KY == retval ) || ( ENTER_KY == retval ) )
            LoopFlag = False;
    }
}
*/


//-----------------------------------------------------------------------------
//!  \brief
//!     Local Options Select
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void Fun_97( void )
{
    Bool loop;

    // Save and verify the Input Password
    if ( InputPswd( LocalOptSel ) )
    {
        DrawMenu( MENU_LOCALOPTS );

        loop = True;
        while ( loop )
        {
            // Get Keypad button entry
            switch ( GetOneKey( DEV_PAD | DEV_KBD, UserTimeout ) )
            {
            case 'P':       // Password options
                if ( !Fun_97_1(  ) )
                    loop = False;
                else
                {
                    // Draw the Local Options Menu
                    DrawMenu( MENU_PREVIOUS );
                }
                break;

            case 'T':       // Tip Percentages
                if ( !SetTipPercent(  ) )
                    loop = False;
                else
                {
                    // Draw the Local Options Menu
                    DrawMenu( MENU_REDRAW );
                }
                break;

            case B_PREVIOUS:    // Previous
                DrawMenu( MENU_PREVIOUS );
                break;

            case B_NEXT:    // Next
                DrawMenu( MENU_NEXT );
                break;

            case B_PREVIOUS_NEW_MENU:   // Previous - new menu
                return;

            case CANCEL_KY: // Timed out or cancelled
            case CLEAR_KY:
            case 0:
                loop = False;
                break;

            default:
                break;
            }

        }

        // Update the LRC in case any changes were made
        SetTCTabLRC(  );
    }
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Local Configuration
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_98( void )
{
    UBYTE MPStatus;
    UBYTE tempTERMLine;
    UBYTE tempTERMCuAd;
    UBYTE tempTERMModem;
    struct rs232_rec tmpRSTAB;

    // Get Maintenance Password
    GetMsg( N_Comms, DefaultTitle );
    MPStatus = EnterMPswd(  );
    if ( MPStatus )
    {
        // Password entered, or Password disabled
        if ( 2 == MPStatus )
        {
            // Verify the Input Password
            if ( !InputPswd( LocalConf ) )
            {
                // Bad; return to caller
                return;
            }
        }
    }
    else
    {
        return;
    }

    // Save the Line Type, CU Address and Modem Mode settings
    tempTERMLine = TERM.TERMLine;
    tempTERMCuAd = TERM.TERMCuAd;
    tempTERMModem = TERM.TERMModem;

    // Save RSTAB to temporary byffer
    memcpy (&tmpRSTAB, &RSTAB, RSSZE);

    // Choose the communication
    if ( 0 == RS232CFG() )

        // Display/Set Modem Mode
        if ( InModemMode() )

            // Display/Set DB Modem Mode
            if ( InDBModemMode() )
                // Enhance Dial Messages?, Exit if user presses Cancel
                if ( EnhDialM() )

                    // Display/Set CU Address
                    CUAddress();
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Local Functions
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void Fun_99( void )
{

    UBYTE Status;

    // Get Maintenance Password
    Status = EnterMPswd(  );

    if ( Status )
    {
        // Okay; see if Maintenance Password is disabled
        if ( 2 == Status )
        {
            // Yes; get Function 99 Password
            if ( !Enter99Pswd(  ) )
            {
                // Bad; so exit
                return;
            }
        }

        CurrentEvent.NEXTEVENT = MENU_MAINT_FUNC;
    }
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Maintenance - Clear tables and journal.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_99_9_1( void )
{

    YNEntry.TitleMid = N_Clear;
    YNEntry.Line1Mid = Msg_Everything;
    YNEntry.Line2Mid = CorrectYesNo;

    // Prompt to confirm selection of clear everything.
    if ( ENTER_KY != YesNoEntry( &YNEntry ) )
        return;

    // Format the journal
    JournalFormat ();

    // Clear the fomat flags on each table and then do a CheckTables()
    // call to force a reinitialization of the tables.

    // NMS Table
    NMSFMT = 0;

    // Terminal Table
    TERMFMT = 0;

    // Text on 4th line of  receipt/report header
    USRDFMT = 0;

    // Batch Control Tables
    BATFMT = 0;

    // Terminal Flag Indicator Table
    TFFMT = 0;

    // Local terminal parameters Table
    LOCFMT = 0;

    // RS232 table
    RSFMT = 0;
    // Terminal Configuration
    TCFMT = 0;

    // Card Range Tables
    CRFMT = 0;

    // Card Issuer Tables
    ISFMT = 0;

    // Acquirer Tables
    AQFMT = 0;

    // Visa 2 Tables
    V2FMT = 0;

    // Descriptor Tables
    DSFMT = 0;

    // Idle Screen Button Table
    IBFMT = 0;

    // ICEPAK confiiguration table
    IPFMT = 0;

    // Cashier table
    CASHFMT = 0;

    // Additional prompt Tables
    APFMT = 0;

    // Receipt footers Table
    FTFMT = 0;

    // IP Connection Table
    IPCFMT = 0;

    // PDN Table
    PDNFMT = 0;

#ifdef MAKE_EMV
    // CA Public Key table
    PKFMT = 0;

    // Revocation key table
    RKFMT = 0;

    // Key expiry table
    KEFMT = 0;

    // ICC Table 
    ICCFMT = 0;

    // TAID Table
    TAIDFMT = 0;

    // Additional Tags Table
    ADTFMT = 0;
#endif // MAKE_EMV

    // Call CheckTables() to format tables.
    CheckTables ();

    // Validate the Printer Type
    CheckPrinterType ();

    // Show "transaction complete"
    ShowInfoMsg( TransComplete, N_Clear );
    SDK_BeepIt( 2 );

    // Delay for two seconds
    SDK_Wait( ONESECOND * 2 );

    return;

}



//-----------------------------------------------------------------------------
//!  \brief
//!     Maintenance - Clear acquirer
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_99_9_2( void )
{
    UBYTE status;
    struct acquirer_rec *pAcquirerPtr;  // Pointer to AQTAB entry
    struct batch_rec *pBatch_Table; // Pointer to BATTAB entry
    char num_hosts;

    num_hosts = AqidCount(  );  // count the number of acquirers

    // we don't have any acquirer to clear, just return
    if ( 0 == num_hosts )
        return;

    // if we have only one acquirer in acquirer table, don't need to pop up
    // display the host clear menu. Just set the proper data to TRINP
    if ( 1 == num_hosts )
    {
        TRINP.TRAQID  = AQTAB[0].AQID;
        TRINP.TRAQPTR = &AQTAB[0];

        // Clear one acquirer.
        ClearAcquirer(  );

        // After the single acquirer is cleared
        // reformat the journal.
        JournalFormat ();

        // If not in training mode update BATTAB.
        if ( !( TCONF.TCOPT4 & TC4_TRAINING ) )
        {
            // Increment batch number in batch table for this acquirer
            IncBatNo ( TRINP.TRAQID );

            // Clear Batch pending flag
            pBatch_Table = FindBatTab( TRINP.TRAQID );
            pBatch_Table->BATFLAG &= ~BT_PEND;
            pBatch_Table->BATXFER = 0;

            // Set LRC on batch table 
            SetBATTabLRC(  );
        }

        CheckTables ();

        // Validate the Printer Type
        CheckPrinterType ();
    }
    else
    {
        DrawMenu( MENU_HOST_CLEAR_ACQUIRER );
        do                      // Call the selected option processor
        {
            status = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

            // Go back one menu level
            if ( B_PREVIOUS == status )
            {
                DrawMenu( MENU_PREVIOUS );
            }
            // Go forward one menu level
            else if ( B_NEXT == status )
            {
                DrawMenu( MENU_NEXT );

            }
            else if ( B_PREVIOUS_NEW_MENU == status )
            {

                ButtonId = B_PREVIOUS_NEW_MENU;
                break;          // exit

            }
            else if ( ( CANCEL_KY == status ) ||
                      ( CLEAR_KY == status ) || ( 0 == status ) )
            {
                break;          // Cancel or Time-Out
            }

            // clear all hosts
            else if ( 'Z' == status )
            {
                break;
            }
            else if ( ( status >= 'A' ) && ( status <= 'A' + 7 ) )
            {
                TRINP.TRAQID  = AQTAB[status - 'A'].AQID;
                TRINP.TRAQPTR = &AQTAB[status - 'A'];

                // Specific host entered.
                ClearAcquirer(  );

                // If not in training mode update BATTAB.
                if ( !( TCONF.TCOPT4 & TC4_TRAINING ) )
                {
                    // Increment batch number in batch table for this acquirer
                    IncBatNo ( TRINP.TRAQID );

                    // Clear Batch pending flag
                    pBatch_Table = FindBatTab( TRINP.TRAQID );
                    pBatch_Table->BATFLAG &= ~BT_PEND;
                    pBatch_Table->BATXFER = 0;

                    // Set LRC on batch table 
                    SetBATTabLRC(  );
                }

                CheckTables ();

                // Validate the Printer Type
                CheckPrinterType ();

                ShowInfoMsg( TransComplete, N_Host );
                SDK_BeepIt( 2 );

                DrawMenu( MENU_HOST_CLEAR_ACQUIRER );
            }




        }
        while ( status && ( status != CANCEL_KY ) );

        if ( ( CANCEL_KY == status ) || ( CLEAR_KY == status ) ||
             ( 0 == status ) || ( B_PREVIOUS_NEW_MENU == status ) )
            return;

        // clear all hosts
        if ( 'Z' == status )
        {
            // All hosts entered.
            pAcquirerPtr = FindFirstAcquirer(  );

            while ( NULL != pAcquirerPtr )
            {
                // Clear the found acquirer.
                TRINP.TRAQID = pAcquirerPtr->AQID;
                TRINP.TRAQPTR = pAcquirerPtr;

                // Clear this acquirer.
                ClearAcquirer(  );

                // If not in training mode update BATTAB.
                if ( !( TCONF.TCOPT4 & TC4_TRAINING ) )
                {
                    // Increment batch number in batch table for this acquirer
                    IncBatNo ( TRINP.TRAQID );

                    // Clear Batch pending flag
                    pBatch_Table = FindBatTab( TRINP.TRAQID );
                    pBatch_Table->BATFLAG &= ~BT_PEND;
                    pBatch_Table->BATXFER = 0;

                    // Set LRC on batch table 
                    SetBATTabLRC(  );
                }

                // Clear next acquirer if it exists
                pAcquirerPtr = FindNextAcquirer( pAcquirerPtr );
            }

            // After all acquirers are cleared
            // reformat the journal.
            JournalFormat ();

            CheckTables ();

            // Validate the Printer Type
            CheckPrinterType ();
        }
    }

    ShowInfoMsg( TransComplete, N_Host );
    SDK_BeepIt( 2 );

    // Delay for two seconds
    SDK_Wait( ONESECOND * 2 );

    return;
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Maintenance - Clear all reversals from the journal.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_99_9_6( void )
{
    static UWORD JrnIdent;

    // Get a lock on the journal before using JSREC.
    JournalLock ();

    // Set up selection data in common record JSREC
    SetDefaultSelection ();

    // Select only reversals from the journal.
    JSREC.JSSTATON = TS1_REV;

    // Don't select a reversal advice or settled transaction.
    JSREC.JSSTATOFF = TS1_ADV + TS1_SETL;

    do
    {
        // JrnIdent will be set with first reversal ident, if one exists.
        JrnIdent = JournalSelFirst ();

        if ( 0x0000 != JrnIdent )
        {
            // Reversal was found; free it.
            JournalFree ( JrnIdent );
        }

    }
    while ( JrnIdent != 0x0000 );


    // Release the lock on the journal.
    JournalRelease ();

    ShowInfoMsg( TransComplete, N_Clear );
    SDK_BeepIt( 2 );

    // Delay for two seconds
    SDK_Wait( ONESECOND * 2 );

    return;
}





//-----------------------------------------------------------------------------
//!  \brief
//!     Print Host List
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_99_96( void )
{
    // Notify user -- "PRINTING, WAIT..."
    ShowInfoMsg( PrintingWait, PleaseWait );

    PrintHostList ();
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Enter Function 99 Password
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - if password entered
//!                 False - if cancel key hit.
//!
//-----------------------------------------------------------------------------
static Bool Enter99Pswd( void )
{
    UBYTE Status;
    UBYTE   RtcBuf[RTCLEN];     /* RTC Read Buffer */
    UBYTE   t;
    UBYTE   *ptr;

    // Loop till valid password entered or cancel
    while ( True )
    {
        // Display Maintenance Password message
        // Prompt for Function 99 Password (6 Digits)
        Status = NumEntry( 2, 6, 6, &Fun99PassEntry );

        // Check if user cancel
        if ( Status == 0 )
        {
            // Yes; return False
            return( False );
        }

        /* Read System Time */
        SDK_RtcRead( RtcBuf );
        /* Shuffle the digits */
        for ( ptr = RtcBuf; ptr < RtcBuf+6; ptr += 2 )
        {
            t = *ptr;
            *ptr = (*(ptr+1) < '9') ? *(ptr+1)+1 : '0';
            *(ptr+1) = (t < '9') ? t+1 : '0';
        }

        /* Check if password is correct */
        if ( 0 == memcmp( (UBYTE *)Dspbuf, RtcBuf, 6 ) )
        {
            /* Password is correct, so break out of loop */
            break;
        }
        else
        {
            // Password incorrect, notify user
            SDK_Beeper( TENMS * 40 );
        }
    }

    // Password has been verified
    return( True );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Show Current Line Type       
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True:  new entry enteredNone
//!                 False: current value accepted
//-----------------------------------------------------------------------------
static Bool CUAddress( void )
{
    Bool retval;
    UBYTE length;

    retval = True;

    // Get the Current CU Address Message
    GetMsg( Currently, CSTMSG );

    // Get the length of the prompt
    length = strlen( CSTMSG );

    // Blank out the Nul terminator
    CSTMSG[length++] = ' ';

    // Convert from BCD to ASCII
    BfAscii( &CSTMSG[length++], ( UBYTE * ) & TERM.TERMCuAd,
             sizeof( TERM.TERMCuAd ) );

    // Null terminate the result for display
    CSTMSG[++length] = '\0';

    GlobalDsp = ZAPKEYON;

    retval = NumEntry( 1, 2, 0, &CU_AdR_Entry );

    if ( 0 == retval )
    {
        retval = False;
    }
    else
    {
        if ( 2 == retval )
        {
            // ZAP Key Pressed, Clear field
            TERM.TERMCuAd = 0;
        }
        else
        {
            if ( Dspbuf[0] )
            {
                // Convert Input to BCD and load into field
                AscHex( ( UBYTE * ) & TERM.TERMCuAd, Dspbuf, 1 );
            }
        }
        // Set the TERM Table LRC to save change
        SetTERMTabLRC(  );
        retval = True;
    }

    // Current value accepted
    return( retval );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Show and adjust Modem Mode       
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True:  Enter was pressed
//!                 False: Cancel or Clear was pressed
//-----------------------------------------------------------------------------
static Bool InModemMode( void )
{
    Bool retval;
    UBYTE length;

    // Get the Current Modem Mode
    GetMsg( Currently, CSTMSG );

    // Get the length of the prompt
    length = strlen( CSTMSG );

    // Blank out the Nul terminator
    CSTMSG[length++] = ' ';

    // Convert from BCD to ASCII
    BfAscii( &CSTMSG[length++], ( UBYTE * ) & TERM.TERMModem,
             sizeof( TERM.TERMModem ) );

    // Null terminate the result for display
    CSTMSG[++length] = '\0';

    GlobalDsp = ZAPKEYON;

    retval = AlphaNumEntry( 1, 2, 0, &ModemMode_Entry );

    if ( 0 == retval )
    {
        return( False );
    }
    else
    {
        if ( 2 == retval )
        {
            // ZAP Key Pressed, Clear field
            TERM.TERMModem = 0;
        }
        else
        {
            // Check if single digit was entered
            if ( 0 == Dspbuf[1] )
            {
                Dspbuf[1] = Dspbuf[0];
                Dspbuf[0] = '0';
            }

            if ( (0 != Dspbuf[0]) && (0 != Dspbuf[1]) )
            {
                // Convert Input to BCD and load into field
                AscHex( ( UBYTE * ) & TERM.TERMModem, Dspbuf, 1 );
            }
        }
        // Set the TERM Table LRC to save change
        SetTERMTabLRC(  );
        retval = True;
    }

    // Current value accepted
    return( True );
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Show and adjust DB Modem Mode       
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True:  Enter was pressed
//!                 False: Cancel or Clear was pressed
//-----------------------------------------------------------------------------
static Bool InDBModemMode( void )
{
    // Current value accepted
    return( True );
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Diplay Transaction Accecpted and beep
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void TranAcc( void )
{
    ShowInfoMsg( TransAccepted, N_Response );

    SDK_BeepIt( 2 );
    SDK_Wait( ONESECOND * 2 );
    return;
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Input Terminal Serial Number
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void XInputTermSer( void )
{
    InputTermSer(  );
    if ( B_PREVIOUS == ButtonId )
        CurrentEvent.NEXTEVENT = MENU_REDRAW;   // redraw the terminal setup menu
}



//-----------------------------------------------------------------------------
//!  \brief
//!     input Initialization Telephone Number
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void XInputInitTel( void )
{
    InputInitTel(  );
    if ( B_PREVIOUS == ButtonId )
        CurrentEvent.NEXTEVENT = MENU_REDRAW;   // redraw the terminal setup menu

}

//-----------------------------------------------------------------------------
//!  \brief
//!     Input NMS Telephone Number
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void XInputNMSTel( void )
{
    InputNMSTel(  );
    if ( B_PREVIOUS == ButtonId )
        CurrentEvent.NEXTEVENT = MENU_REDRAW;   // redraw the terminal setup menu

}


//-----------------------------------------------------------------------------
//!  \brief
//!     Input Dial Mode
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void XInputDialMode( void )
{
    InputDialMode(  );
    if ( B_PREVIOUS == ButtonId )
        CurrentEvent.NEXTEVENT = MENU_REDRAW;   // redraw the terminal setup menu

}



//-----------------------------------------------------------------------------
//!  \brief
//!     input PABX Access Code
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void XInputPABX( void )
{
    InputPABX(  );
    if ( B_PREVIOUS == ButtonId )
        CurrentEvent.NEXTEVENT = MENU_REDRAW;   // redraw the terminal setup menu
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Do all terminal setup steps
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void TermAll( void )
{
    UBYTE Step;
    BYTE Status;

    // Initialize input step
    Step = 1;

    // Okay; loop till done
    do
    {
        memset( ( UBYTE * ) Dspbuf, 0, 8 ); // Leave title box alone
        memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );  // Init default value display buffer
        // Case statement based on input step
        switch ( Step )
        {
        case 1:         // Input Terminal Serial Number
            Status = InputTermSer(  );
            break;

        case 2:         // Input Dial Backup Initialization Telephone Number
            Step++;
        case 3:         // Input Dial Backup Initialization Telephone Number
            Step++;
        case 4:         // Input Initialization Telephone Number
            Status = InputInitTel(  );
            break;

        case 5:         // Input Dial Backup Initialization Telephone Number
            Step++;

        case 6:         // Input NMS Telephone Number
            Status = InputNMSTel(  );
            break;

        case 7:         // Input Dial Backup NMS Telephone Number
            Step++;

        case 8:         // Input Dial Mode
            Status = InputDialMode(  );
            break;

        case 9:         // Input PABX Access Code
            Status = InputPABX(  );
            break;
        }

        // Case statement based on status
        switch ( Status )
        {
        case -1:            // Go to previous step
            Step--;
            break;

        case 0:         // Cancel
            Step = 0;
            break;

        case 1:         // Go to next step
            Step++;
            // See if need to go back to input step 1
            if ( Step > 9 )
            {
                // Yes; set input step to 1
                Step = 1;
            }
            break;
        }
    }
    while ( Step > 0 );

}



//-----------------------------------------------------------------------------
//!  \brief
//!     Local Options 1 Select - password options
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        False for cancel or timout, else True
//!
//-----------------------------------------------------------------------------
static Bool Fun_97_1( void )
{
    char retval;

    // Display the password selection menu
    DrawMenu( MENU_PASSWORDS_ONOFF );

    do
    {
        // Get Keyboard Entry
        switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
        {
        case 'V':           // Void
            retval = DoPasswordEnable( &VoidPwdEntry, TCL_NO_VPWD );
            break;

        case 'R':           // Refund
            retval = DoPasswordEnable( &RefundPwdEntry, TCL_NO_RPWD );
            break;

        case 'A':           // Adjust
            retval = DoPasswordEnable( &AdjustPwdEntry, TCL_NO_APWD );
            break;

        case 'P':           // Report
            retval = DoPasswordEnable( &ReportPwdEntry, TCL_NO_RPPW );
            break;

        case 'L':           // All 4
            retval = DoPasswordEnable( &VoidPwdEntry, TCL_NO_VPWD );
            if ( retval != 'N' )    // anything but Next?
                break;
            retval = DoPasswordEnable( &RefundPwdEntry, TCL_NO_RPWD );
            if ( retval != 'N' )    // anything but Next?
                break;
            retval = DoPasswordEnable( &AdjustPwdEntry, TCL_NO_APWD );
            if ( retval != 'N' )    // anything but Next?
                break;
            retval = DoPasswordEnable( &ReportPwdEntry, TCL_NO_RPPW );
            break;

        case B_PREVIOUS:    // Previous
            DrawMenu( MENU_PREVIOUS );
            break;

        case B_NEXT:        // Next
            DrawMenu( MENU_NEXT );
            break;

        case B_PREVIOUS_NEW_MENU:   // Previous - new menu
            return True;

        case CANCEL_KY: // Timed out or cancelled
        case CLEAR_KY:
        case 0:
            retval = 'C';
            break;

        default:
            break;

        }

        if ( retval == 'N' )
            DrawMenu( MENU_REDRAW );    // redraw password menu


    }
    while ( retval != 'C' );

    return False;
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Local Options Select for passwords
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void PasswordOptions( void )
{
    char retval = False;

    // Save and verify the Input Password
    retval = InputPswd( LocalOptSel );

    if ( retval )
    {
        Fun_97_1(  );

        // Set TCONF's LRC
        SetTCTabLRC(  );
    }
}


//-----------------------------------------------------------------------------
//!  \brief
//!     ICEPAK graphics download
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_14( void )
{
    UBYTE retval;

    UBYTE oldlrc;
    UBYTE newlrc;

    // If IcePak is not configured in TermMaster DB, just return.
    // Check if Icewpak Table has been formatted 
    if ( IPFMT == 0x55 )
    {
        // Yes; get present LRC 
        oldlrc = IPLRC;

        // Calculate LRC 
        newlrc = CalcLrc( (UBYTE *)&IPTAB, IPSZE );

        // Check if LRCs are the same 
        if ( newlrc != oldlrc )
        {
            ShowErrMsg( InvalidEntry );
            return;
        }
        else if ( (UBYTE)0xFF == NullComp( (char *) IPTAB.IPSID, S_IPSID ) )
        {
            ShowErrMsg( InvalidEntry );
            return;
        }
    }
    else
    {
        ShowErrMsg( InvalidEntry );
        return;
    }

    // Prompt for YES or NO Keys
    YNEntry.TitleMid = CustomMsg;
    YNEntry.Line1Mid = N_NullStr;
    YNEntry.Line2Mid = CorrectYesNo;
    memcpy( CSTMSG, CurrentEvent.TRNTEXT, sizeof( CurrentEvent.TRNTEXT ) );

    retval = YesNoEntry( &YNEntry );
    if ( ENTER_KY == retval )   // If YES Key
    {
        TRINP.TRKEY = ICEPAK_DOWNLOAD;

        // Set flags to not print the receipt header and not allow user cancel.
        FlagHostCntl = 0;

        // Flag the host process
        FlagHost ();

        // Save host response text in terminal flag table.
        SaveResponseText ();

        // Display Response
        RespDisplay ();


        IcePak_Init ();
    }

    CurrentEvent.NEXTEVENT = 0;

}


//-----------------------------------------------------------------------------
//!  \brief
//!     Reprint function
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Reprint( void )
{
    UBYTE status;

    CurrentEvent.NEXTEVENT = 0;

    DrawMenu( MENU_REPRINT );

    // Call the selected Option Processor
    do
    {

        status = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

        switch ( status )
        {
        case 'C':           // reprint last receipt
            JrnReprintLast ();
            DrawMenu( MENU_REDRAW );
            break;

        case 'D':           // reprint any receipt
            JrnReprintAny ();
            if ( CANCEL_KY == ButtonId )
                status = CANCEL_KY;
            DrawMenu( MENU_REDRAW );
            break;

        default:
            break;
        }

    }
    while ( status && ( status != CANCEL_KY ) && ( status != CLEAR_KY )
            && ( status != B_PREVIOUS_NEW_MENU ) );

    if ( ( 0 == status ) || ( CANCEL_KY == status ) ||
         ( CLEAR_KY == status ) )
        ButtonId = CANCEL_KY;
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Local Options Select for Tip Percentages
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void TipPercentages( void )
{
    // Save and verify the Input Password
    if ( InputPswd( Msg_Tips ) )
    {
        SetTipPercent(  );

    }
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Prompt the user to either view or Print the Settlement Status.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void SettleStatus( void )
{
    while ( 1 )
    {
        // Display a PRINT and VIEW button screen
        DrawMenu( MENU_SETTLE_STATUS );

        // Get Keypad button entry
        switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
        {
        case 'P':           // Print Settlement Summary
            // Do Issuer Receipt
            SettleSumRpt ();
            break;

        case 'V':           // View Settlement Summary
            ViewSettleSum ();
            break;

        case CANCEL_KY: // Timed out or cancelled
        case 0:
            return;

        default:
            SDK_Beeper( TENMS * 40 );
            break;
        }
    }
}


//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//!  \brief
//!     Display Current Batch
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void Fun_8( void )
{
    struct acquirer_rec *pAcquirerPtr;  // Pointer to AQTAB entry
    struct acquirer_rec *pHostPtr;  // Pointer to AQTAB entry
    UBYTE keyval;
    struct batch_rec *pBatch_Table; // Pointer to BATTAB entry
    UBYTE length;
    UBYTE tmpaqid;

    pHostPtr = FindFirstAcquirer(  );


    Enable_Flags[0] = 1;        // Default to Next Arrow ON
    Enable_Flags[7] = 0;        // Previous Arrow OFF

    while ( NULL != pHostPtr )
    {
        // Look for Previous Batch
        if ( NULL == FindPrevAcquirer( pHostPtr ) )
            Enable_Flags[7] = 0;    // Turn Previous Arrow OFF

        // Look for Next Batch
        if ( NULL == FindNextAcquirer( pHostPtr ) )
            Enable_Flags[0] = 0;    // Turn Next Arrow OFF

        pBatch_Table = FindBatTab( pHostPtr->AQID );

        // Get Host Id header
        GetMsg( N_HostId, CSTMSG );
        length = StrLn( CSTMSG, sizeof( CSTMSG ) );

        // Acquirer ID is one byte BCD, convert to two ASCII characters
        tmpaqid = pHostPtr->AQID;
        BufAsc( ( char * ) &CSTMSG[length], &tmpaqid );
        length += 2;
        CSTMSG[length] = '\0';

        // Get Host Name header
        GetMsg( N_HostName, CSTMSG2 );
        length = StrLn( CSTMSG2, sizeof( CSTMSG2 ) );

        // Get the Acquirer Name
        MoveItAq( &CSTMSG2[length], ( pHostPtr->AQNAME ), 10 );
        CSTMSG2[length + 10] = '\0';

        // Get Batch Number header
        GetMsg( N_BatchNumber, CSTMSG3 );
        length = StrLn( CSTMSG3, sizeof( CSTMSG3 ) );

        // Get Current Batch Number, convert to ASCII
        BfAscii( &CSTMSG3[length], pBatch_Table->BATCUR, S_BATCUR );
        CSTMSG3[length + ( S_BATCUR * 2 )] = '\0';

        // Display Batch Info Screen (see disproc.c)
        DrawScreen( BATCHINFO_SCR );

        // Get a key from the user
        keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

        // Check if cancel or Time-Out
        if ( ( CANCEL_KY == keyval ) || ( 0 == keyval ) )
        {
            // Yes; exit the function
            return;
        }

        // Check for Previous Batch wanted
        if ( ( B_PREVIOUS == keyval ) )
        {
            // Look for Previous Batch
            pAcquirerPtr = FindPrevAcquirer( pHostPtr );
            if ( NULL != pAcquirerPtr )
            {
                // Set pointer if Acquirer found
                pHostPtr = pAcquirerPtr;
                Enable_Flags[0] = 1;    // Turn Next Arrow ON
            }
            else
                Enable_Flags[7] = 0;    // Turn Previous Arrow OFF

        }
        else
        {
            // Check for Next Batch wanted
            if ( ( B_NEXT == keyval ) )
            {
                // Look for Next Batch
                pAcquirerPtr = FindNextAcquirer( pHostPtr );
                if ( NULL != pAcquirerPtr )
                {
                    pHostPtr = pAcquirerPtr;
                    Enable_Flags[7] = 1;    // Turn Previous Arrow ON
                }
                else
                    Enable_Flags[0] = 0;    // Turn Next Arrow OFF
            }
        }
    }

    // No Acquirer set
    ShowErrMsg( NoAcq );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Copy Track 2 data without Discretionary data.
//!     The data is returned in CSTMSG, with no wrap.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Original function ShowTrk2, without wrap.
//!
//-----------------------------------------------------------------------------
static void Trk2TOCstMsg( void )
{
    int idx, ExpDateCnt, separator_count;

    // Clean CSTMSG
    separator_count = 0;
    memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );

    // Move first set of bytes to CSTMSG while looking for separator.
    for ( idx = 0, ExpDateCnt = 0; ExpDateCnt <= 4 && idx < S_TRTRK2; idx++ )
    {
        // Copy TK2 data into global buffers in ASCII.
        CSTMSG[idx] = TRINP.TRTRK2[idx];

        // Only increments counter if was started before (when '=' found)
        if ( ExpDateCnt )
            ExpDateCnt++;

        // Found a separator
        if ( '=' == CSTMSG[idx] )
        {
            // Start ExpDate counter
            ExpDateCnt++;

            // When separator count gets to 2 the discretionary data begins.
            // Don't display the discretionary data.
            separator_count++;
            if ( 2 == separator_count )
            {
                break;
            }
        }
    }

    CSTMSG[idx] = 0;
}




//-----------------------------------------------------------------------------
//!  \brief
//!     Maintenance - Part of clear batch
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void ClearAcquirer( void )
{
    // Get a lock on the journal before using JSREC.
    JournalLock ();

    // Set up selection data in common record JSREC
    SetDefaultSelection ();

    // Clear all flags.
    JSREC.JSSTATOFF = 0;

    // Clear selected acquirer.
    JSREC.JSAQID = TRINP.TRAQID;

    // Clear this acquirers batch tranactions from the journal
    JournalClearBatch ();

    // Release the lock on the journal.
    JournalRelease ();

    SetBATTabLRC(  );

    // Clear last AQID from TFTAB.
    TFTAB.TFLAQID = 0;
    SetTFTabLRC(  );
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Input Terminal Serial Number
//!
//!  \param
//!     None
//!
//!  \return
//!     BYTE        1 - current Terminal Serial Number accepted
//!                     or new Terminal Serial Number entered
//!                 0 - input cancelled or timed-out.
//!
//-----------------------------------------------------------------------------
static BYTE InputTermSer( void )
{
    Bool UpdateSer;
    UBYTE length;

    // See if Serial Number Edit allowed
    if ( TERM.TERMOpt1 & TM1_TSEREDIT )
    {
        // Yes; set update flag to True
        UpdateSer = True;

        // Get current Serial Number
        memcpy( CSTMSG, ( UBYTE * ) TERM.TERMSer, 8 );

        // See if 8 digit Serial Number required
        if ( TERM.TERMOpt1 & TM1_TSER8 )
        {
            // Yes; null terminate Serial Number for display
            CSTMSG[8] = '\0';

            // Enter 8 digit Serial Number
            while ( True )
            {
                // Check the input
                if ( AlphaNumEntry( 1, 8, 0, &APTermSer ) == 1 )
                {
                    // Okay; get length of input
                    length = StrLn( Dspbuf, sizeof( Dspbuf ) );

                    // Check for maximum length of input
                    if ( length == 8 )
                    {
                        ButtonId = B_PREVIOUS;
                        // Yes; break out of the loop
                        break;
                    }
                    else
                    {
                        // No; check for value acknowledgement
                        if ( length == 0 )
                        {
                            ButtonId = B_PREVIOUS;
                            // Yes; return okay
                            return( 1 );
                        }
                    }
                }
                else
                {
                    // Cancel; set update flag to false
                    UpdateSer = False;

                    // Break out of the loop
                    break;
                }
            }

            // Append a blank at the end
            Dspbuf[8] = ' ';
        }
        else
        {
            // No; add Terminal Serial Number Check Digit to display
            CSTMSG[8] = TERM.TERMChk;

            // Null terminate Serial Number for display
            CSTMSG[9] = '\0';

            // Enter 9 digit Serial Number (includes ck-digit)
            while ( True )
            {
                // Check the input
                if ( AlphaNumEntry( 1, 9, 0, &APTermSer ) == 1 )
                {
                    // Okay; get length of input
                    length = StrLn( Dspbuf, sizeof( Dspbuf ) );

                    // Check for maximum length of input
                    if ( length == 9 )
                    {
                        // Okay; check input with check digit
                        if ( ChkTermChk( Dspbuf ) )
                        {
                            ButtonId = B_PREVIOUS;
                            // Yes; break out of the loop
                            break;
                        }
                        else
                        {
                            // Incorrect check digit, notify user
                            SDK_Beeper( TENMS * 40 );
                        }
                    }
                    else
                    {
                        // No; check for value acknowledgement
                        if ( length == 0 )
                        {
                            ButtonId = B_PREVIOUS;
                            // Yes; return okay
                            return( 1 );
                        }
                        else
                        {
                            // Input too short, notify user
                            SDK_Beeper( TENMS * 40 );
                        }
                    }
                }
                else
                {
                    // Cancel; set update flag to false
                    UpdateSer = False;

                    // Break out of the loop
                    break;
                }
            }
        }

        // Check if Serial Number is to be updated
        if ( UpdateSer )
        {
            // Yes; save the new Terminal Serial Number
            memcpy( ( UBYTE * ) TERM.TERMSer, Dspbuf, 8 );

            // Check if 9 digit Terminal Serial Number
            if ( !( TERM.TERMOpt1 & TM1_TSER8 ) )
            {
                // Yes; update Terminal Serial Number Check Digit
                TERM.TERMChk = Dspbuf[8];
            }

            // Recalculate Checksum
            SetTERMTabLRC(  );
        }
        else
        {
            // No; return Cancel/Time-Out
            return( 0 );
        }
    }

    // Return okay
    return( 1 );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Check if Enhanced Dial Messages set
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True:  Set to ENHANCED MESSAGESNone
//!                 False: Not set to ENHANCED MESSAGES
//-----------------------------------------------------------------------------
static Bool EnhDialM( void )
{
    UBYTE retval;

    // Prompt for Enhanced Dialing Messages
    YNEntry.TitleMid = N_Comms;
    YNEntry.Line1Mid = EnhDial;
    YNEntry.Line2Mid = N_DialMsgs;

    retval = YesNoEntry( &YNEntry );

    if ( ENTER_KY == retval )   // If YES       
    {

        // Set to ENHANCED MESSAGES
        TCONF.TCDOPT |= TCD_ENH;
        retval = True;
    }
    else
    {
        if ( CLEAR_KY == retval )   // If NO        
        {
            // CLEAR to NO ENHANCED MESSAGES
            TCONF.TCDOPT &= ~TCD_ENH;
            retval = True;

        }
        else
        {
            // Did NOT get valid input
            retval = False;
        }
    }

    SDK_BeepIt( 1 );

    // Set TCONF's LRC
    SetTCTabLRC(  );

    return( retval );
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Input Initialization Telephone Number
//!
//!  \param
//!     None
//!              
//!                      
//!  \return
//!     BYTE        1 - Current Init Telephone Number accepted
//!                     or new Init Telephone Number entered 
//!                 0 - input cancelled or timed-out
//!                 -1 - go back to previous entry.
//!                      
//-----------------------------------------------------------------------------
static BYTE InputInitTel( void )
{
    return( InputTel((UBYTE *)TERM.TERMInitTel,&InpTelEntry) );
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Input NMS Telephone Number
//!
//!  \param
//!     None.
//!              
//!  \return
//!     BYTE        1 - Current NMS Telephone Number accepted
//!                     or new NMSt Telephone Number entered 
//!                 0 - input cancelled or timed-out
//!                 -1 - go back to previous entry.
//!                      
//-----------------------------------------------------------------------------
static BYTE InputNMSTel( void )
{
    return(  InputTel((UBYTE *)TERM.TERMNmsTel,&InpNMSEntry) );
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Input Dial Backup Initialization Telephone Number
//!
//!  \param
//!     None.
//!              
//!  \return
//!     BYTE        1 - Current Init Telephone Number accepted
//!                     or new Init Telephone Number entered 
//!                 0 - input cancelled or timed-out
//!                 -1 - go back to previous entry.
//!                      
//-----------------------------------------------------------------------------
extern BYTE InputDBInitTel( void )
{
    return 1;
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Input Dial Backup NMS Telephone Number
//!
//!  \param
//!     None
//!              
//!  \return
//!     BYTE        1 - Current NMS Telephone Number accepted
//!                     or new NMSt Telephone Number entered 
//!                 0 - input cancelled or timed-out
//!                 -1 - go back to previous entry.
//!                      
//-----------------------------------------------------------------------------
extern BYTE InputDBNMSTel( void )
{
    return 1;
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Input Telephone Number
//!
//!  \param
//!     PhoneData   pointer to phone data source and storage
//!
//!  \param
//!     pEntry      pointer to screen data
//!              
//!  \return
//!     BYTE        1 - Current NMS Telephone Number accepted
//!                     or new NMSt Telephone Number entered 
//!                 0 - input cancelled or timed-out
//!                 -1 - go back to previous entry.
//!                      
//-----------------------------------------------------------------------------
static BYTE InputTel( UBYTE * pPhoneData, const ENTRY_STR * pEntry )
{
    char Tel[25];
    UBYTE i;
    UBYTE length;
    BYTE Status;

    // Loop till done
    do
    {
        // Convert from BCD to ASCII 
        BfAscii( Tel, pPhoneData, 12  );

        // Null terminate the result to fit the display 
        Tel[24] = '\0';

        // Put Nul character at Terminator position if exists 
        for ( i = 0; i < 25; i++ )
        {
            // Check if Terminator 
            if ( Tel[i] == 'F' )
            {
                // Yes; replace with Nul character 
                Tel[i] = '\0';

                // Break out of the loop 
                break;
            }
        }

        // Move NMS Telephone Number to global display buffer 
        memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );
        MoveTillZero( CSTMSG, Tel, sizeof( CSTMSG ) );

        // Set clear key 
        GlobalDsp = CLRKEYON;

        // Check the input 
        switch ( AlphaNumEntry( 1, 24, 0, pEntry ) )
        {
        case 1:         // Okay; get length of input 
            length = StrLn( Dspbuf, sizeof( Dspbuf ) );
            memcpy( Tel, Dspbuf, length );

            // Check if an entry was input 
            if ( length )
            {
                // Yes; fill out remainder with 'F's 
                memset( ( UBYTE * ) ( Tel + length ),
                        'F',( UWORD ) ( 24 - length ) );

                // Preset status to 1
                Status = 1;

                // Test for legal ASCII values.  Repeat entry if not 0-9, A-F. 
                for ( i = 0; i < length; i++ )
                {
                    // Only allow values 0-9 and A-F
                    if ( !( ( Tel[i] >= '0' ) &&
                            ( Tel[i] <= '9' ) ) )
                    {
                        // Not 0-9, check for A-F
                        if ( !
                             ( ( Tel[i] >= 'A' ) &&
                               ( Tel[i] <= 'F' ) ) )
                        {
                            // Illegal character was input.  Error msg and reprompt.
                            ShowErrMsg( N_Only09AFvalid );
                            Status = 99;
                            break;
                        }
                    }
                }

                // Don't save if illegal char was input.
                if ( 99 == Status )
                    break;

                // Convert input from ASCII to BCD and save 
                AscHex( pPhoneData, Tel, 12 );

                SetTERMTabLRC();
            }

            ButtonId = B_PREVIOUS;
            // Set status to okay 
            Status = 1;

            break;

        case 3:         // Go back to previous entry 
            Status = -1;
            ButtonId = B_PREVIOUS;

            break;

        default:            // Entry cancelled or timed-out 
            Status = 0;

            break;
        }
    }
    while ( 99 == Status );

    // Return the status 
    return( Status );
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Input Dial Mode
//!
//!  \param
//!     None
//!              
//!  \return
//!     BYTE        1 - current Dial Mode accepted or new Dial Mode entered
//!                     or new NMSt Telephone Number entered 
//!                 0 - input cancelled or timed-out
//!                      
//-----------------------------------------------------------------------------
static BYTE InputDialMode( void )
{
    ClearMenus ();

    // Draw Pulse/Tone menu
    DrawMenu( MENU_DIAL_MODE );

    // Determine which button should be displayed inverted 
    switch ( TERM.TERMDMode )
    {
    case TMDM_PULSE:
        MenuItemCounter = 1; 
        break;

    case TMDM_TONE: 
        MenuItemCounter = 0; 
        break;
    }

    while ( True )
    {
        switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
        {
        
        case 'A':           // TONE 
            TERM.TERMDMode = TMDM_TONE;
            SetTERMTabLRC(  );
            return 1;

        case 'B':           // PULSE 
            TERM.TERMDMode = TMDM_PULSE;
            SetTERMTabLRC(  );
            return 1;

        case ENTER_KY:      // Just confirm currnt setting
            return 1;

        case 0:             // Timeout or cancel
        case CLEAR_KY:
        case CANCEL_KY:
            return 0;

        default:
            // Bad key, error beep 
            SDK_Beeper( TENMS * 40 );
            break;
        }

    }
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Input Init Preferred Service
//!
//!  \param
//!     None
//!              
//!  \return
//!     BYTE        1 - current Dial Mode accepted or new Dial Mode entered
//!                     or new NMSt Telephone Number entered 
//!                 0 - input cancelled or timed-out
//!                      
//-----------------------------------------------------------------------------
extern BYTE InputDBInitPref( void )
{
    return 1;
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Input NMS Preferred Service
//!
//!  \param
//!     None
//!              
//!  \return
//!     BYTE        1 - current Dial Mode accepted or new Dial Mode entered
//!                     or new NMSt Telephone Number entered 
//!                 0 - input cancelled or timed-out
//!                      
//-----------------------------------------------------------------------------
extern BYTE InputDBNMSPref( void )
{
    return 1;
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Input PABX Telephone Number
//!
//!  \param
//!     None
//!              
//!  \return
//!     BYTE        1 - Current NMS Telephone Number accepted
//!                     or new NMSt Telephone Number entered 
//!                 0 - input cancelled or timed-out
//!                 -1 - go back to previous entry.
//!                      
//-----------------------------------------------------------------------------
static BYTE InputPABX( void )
{
    char PABX[9];
    UBYTE i;
    UBYTE length;
    BYTE Status;

    // Loop till done
    do
    {
        // Convert from BCD to ASCII
        BfAscii( PABX, ( UBYTE * ) TERM.TERMPabx, sizeof( TERM.TERMPabx ) );

        // Null terminate the result
        PABX[8] = '\0';

        // Put Nul character at Terminator position if exists
        for ( i = 0; i < 8; i++ )
        {
            // Check if Terminator
            if ( PABX[i] == 'F' )
            {
                // Yes; replace with Nul character
                PABX[i] = '\0';

                // Break out of the loop
                break;
            }
        }

        // Clear CSTMSG in case no PABX characters to move.
        memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );

        // Move PABX Access Code to global display buffer
        MoveTillZero( CSTMSG, PABX, sizeof( CSTMSG ) );

        // Set zap and clear keys
        GlobalDsp = ZAPKEYON + CLRKEYON;

        // Check the input
        switch ( AlphaNumEntry( 1, 8, 0, &PABXEntry ) )
        {
        case 1:         // Success; copy the input into local buffer
            strcpy( PABX, Dspbuf );

            // Get length of input
            length = strlen( PABX );

            // Check if an entry inputted
            if ( length )
            {
                // Yes; check if non-maximum length 
                // inputted
                if ( length != 8 )
                {
                    // Yes; fill out remainder with 'F's
                    memset( ( UBYTE * ) ( PABX + length ),'F',
                            ( UWORD ) ( 8 - length )  );
                }

                // Preset status to 1
                Status = 1;

                // Test for legal ASCII values.  Repeat entry if not 0-9, A-F. 
                for ( i = 0; i < length; i++ )
                {
                    // Only allow values 0-9 and A-F
                    if ( !( ( PABX[i] >= '0' ) && ( PABX[i] <= '9' ) ) )
                    {
                        // Not 0-9, check for A-F
                        if ( !( ( PABX[i] >= 'A' ) &&
                                ( PABX[i] <= 'F' ) ) )
                        {
                            // Illegal character was input.  Error msg and reprompt.
                            ShowErrMsg( N_Only09AFvalid );
                            Status = 99;
                            break;
                        }
                    }
                }

                // Don't save if illegal char was input.
                if ( 99 == Status )
                    break;

                // Convert input from ASCII to BCD and save
                AscHex( ( UBYTE * ) TERM.TERMPabx, PABX,
                        sizeof( TERM.TERMPabx ) );

                // Recalculate Checksum
                SetTERMTabLRC(  );
            }
            // The length of entry is zero, clear PABX
            else
            {
                memset( ( UBYTE * ) PABX,'F',  8  );

                // Convert input from ASCII to BCD and save
                AscHex( ( UBYTE * ) TERM.TERMPabx, PABX,
                        sizeof( TERM.TERMPabx ) );

                // Recalculate Checksum
                SetTERMTabLRC(  );
            }

            // Current value accepted or new entry entered
            Status = 1;
            ButtonId = B_PREVIOUS;

            break;

        case 2:         // Zap key pressed; reset value to all F's
            memset( ( UBYTE * ) PABX, 'F', 8 );

            // Convert input from ASCII to BCD and save
            AscHex( ( UBYTE * ) TERM.TERMPabx, PABX,
                    sizeof( TERM.TERMPabx ) );

            // Recalculate Checksum
            SetTERMTabLRC(  );

            // Need to redo input
            Status = 99;

            break;

        case 3:         // Go back to previous entry
            Status = -1;
            ButtonId = B_PREVIOUS;

            break;

        default:            // Entry cancelled or timed-out
            Status = 0;

            break;
        }
    }
    while ( 99 == Status );

    // Return the status
    return( Status );
}

#ifndef MAKE_OPTIMUM
//-----------------------------------------------------------------------------
//!  \brief
//!     Covert the input hex IP address into ASCII in the network format.
//!
//!  \param
//!     pTermFld    Pointer to the IP address to convert
//!
//!  \return
//!     None
//!
//!  \note
//!     NMSBUF contains the converted IP address.
//-----------------------------------------------------------------------------
static void inet_ntoapp( void *pTermFld )
{
    struct in_addr input;

    memcpy( &input, pTermFld, sizeof( UDWORD ) );
    inet_ntoa( ( char * ) NMSBUF, input );
}
#endif // MAKE_OPTIMUM

//-----------------------------------------------------------------------------
//!  \brief
//!     Prompt to enable or disable a password
//!
//!  \param
//!     screen      Pointer to Yes/No entry screen descriptor
//!
//!  \param
//!     opt_mask    Mask containing bit to be set or reset
//!
//!  \return
//!     char        'N' - yes or no selected, go to next step
//!                 'C' - cancel or timeout, abort function
//!                 'B' - Back to previous menu
//-----------------------------------------------------------------------------
static char DoPasswordEnable( const ENTRY_STR * screen, UBYTE opt_mask )
{
    switch ( YesNoEntry( screen ) )
    {
    case CLEAR_KY:
        // Do not require passord
        TCONF.TCLOCOP |= opt_mask;
        return 'N';

    case ENTER_KY:
        // Require password
        TCONF.TCLOCOP &= ~opt_mask;
        return 'N';

    case B_PREVIOUS:        // Previous
        return 'B';

    default:                // Timeout or cancel
        return 'C';
    }
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Set up Tip Percentage Screen To display current tip
//!     percentages, then display them and allow user to change them.
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        False - time out or cancel.
//!                 True - Successful
//-----------------------------------------------------------------------------
static Bool SetTipPercent( void )
{
    UBYTE length;
    UBYTE keyval;

    while ( 1 )
    {
        // Prepare each tip field value
        GetMsg( Msg_TipPct, CSTMSG );
        GetMsg( Msg_TipPct, CSTMSG2 );
        GetMsg( Msg_TipPct, CSTMSG3 );
        length = StrLn( CSTMSG, sizeof( CSTMSG ) );

        // This first tip percentage field
        BufAsc( ( char * ) &CSTMSG[length - 3], &TCONF.TCTIPPCT_1 );

        // This second tip percentage field
        BufAsc( ( char * ) &CSTMSG2[length - 3], &TCONF.TCTIPPCT_2 );

        // This third tip percentage field
        BufAsc( ( char * ) &CSTMSG3[length - 3], &TCONF.TCTIPPCT_3 );


        // Now draw the Tip Percentage Screen
        DrawScreen( SET_TIP_SCR );

        // Wait for a Keypress or timeout
        keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );
        switch ( keyval )
        {
        case '1':           // User selected the First button
        case '2':           // User selected the Second button
        case '3':           // User selected the Third button
            // Get new percentage from user.
            if ( !GetTipPercent( keyval ) )
            {
                // Timeout or CANCEL
// FIXME?
//@ASSA at 04.07.2005 					chRetval = 0;
                return False;
            }
            break;

        case CANCEL_KY: // Timed out or cancelled
        case 0:
            return False;

        case 'd':           // User Done changing tips
        case B_PREVIOUS:    // Previous button selected

        case ENTER_KY:      // User accepts percentage values
            return True;

        default:
            // bad key, error beep
            SDK_Beeper( TENMS * 40 );
            break;
        }
    }
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Prompt user for Tip Percentage  tip percentages, 
//!
//!  \param
//!     key         Key/Button Selected
//!
//!  \return
//!     Bool        False - Time Out or Cancel.
//!                 True - Successfull
//!
//-----------------------------------------------------------------------------
static Bool GetTipPercent( UBYTE key )
{
    UBYTE retval, length;
    UBYTE *pTip;

    // Set clear key
    GlobalDsp = ZAPKEYON;

    // Load current percentage for key selected
    switch ( key )
    {
    case '1':               // First percentage
        memcpy( CSTMSG4, CSTMSG, sizeof( CSTMSG ) );
        break;

    case '2':               // Second percentage
        memcpy( CSTMSG4, CSTMSG2, sizeof( CSTMSG ) );
        break;

    case '3':               // Third percentage
        memcpy( CSTMSG4, CSTMSG3, sizeof( CSTMSG ) );
        break;
    }

    // Check the input
    retval = NumEntry( 1, 2, 0, &Sug_Tip_Entry );

    // Check for Timeout, CANCEL or two CLEARS
    if ( !retval )
        return False;

    // If ZAP Key Pressed, force '00' entry
    if ( 2 == retval )
    {
        Dspbuf[0] = '0';
        Dspbuf[1] = '0';
    }

    // Get length of string entered
    length = StrLn( Dspbuf, sizeof( Dspbuf ) );

    // Don't change anything if user just pressed ENTER
    if ( length )
    {

        // Need to pad if only one digit entered
        if ( 1 == length )
        {
            Dspbuf[1] = Dspbuf[0];
            Dspbuf[0] = '0';
        }

        // Set pointer to point to right field
        pTip = ( &TCONF.TCTIPPCT_1 + ( key - 0x31 ) );

        // COnvert to BCD, Load and set LCR
        AscHex( pTip, Dspbuf, 1 );
        SetTCTabLRC(  );
    }
    return True;
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Maintenance - Clear file system.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_99_9_8( void )
{
#ifndef MAKE_VIKING
    // Reformat the file system
    fformat( HFS_FORMAT_QUICK );

    ShowInfoMsg( TransComplete, N_HFS );
    SDK_BeepIt( 2 );

    // Delay for two seconds
    SDK_Wait( ONESECOND * 2 );
#endif // not MAKE_VIKING
}


#ifdef MAKE_SCREEN_CAPTURE
//-----------------------------------------------------------------------------
//!  \brief
//!     Screen Capture toggle.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
void Fun_88( void )
{
    static int grab = 0;
    if (!grab)
    {
        HW_StartCaptureServer(2);
    }
    else
    {
        HW_StopCaptureServer();
    }
    grab = !grab;
}
#endif // MAKE_SCREEN_CAPTURE

#ifndef MAKE_OPTIMUM
//-----------------------------------------------------------------------------
//!  \brief
//!     Show captured IP address
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Globas:  Captured_IPaddr
//!
//-----------------------------------------------------------------------------
void Fun_76( void )
{
    //  Convert the IP address to ASCII 
    inet_ntoapp( &Captured_IPaddr );

    // Move the IP address to global display buffer 
    memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );
    MoveTillZero( CSTMSG, ( char * ) NMSBUF, sizeof( CSTMSG ) );

    // Show it till key pressed
    GetMsg( N_Report, DefaultTitle );
    ContinueEntry( &Fun_76_Entry );
}
#endif // MAKE_OPTIMUM

//-----------------------------------------------------------------------------
//!  \brief
//!     Set the Checksum (LRC) for the User Data Table.
//!
//!  \param
//!     None.
//!
//!  \return
//!     None
//!
//!  \note
//!     Updates USRDLRC.
//!
//-----------------------------------------------------------------------------
extern void SetUsrdTabLRC( void )
{
    USRDLRC = CalcLrc( ( UBYTE * ) &USRDTAB, USRDSZE );
#ifdef MAKE_OPTIMUM
    StoreTable( (UBYTE *)&USRDTAB );
#endif
}

