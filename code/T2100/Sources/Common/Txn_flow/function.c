
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
#include "comm_c.h"
#include "commmodule.h"
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
#include "sockets.h"
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

//=============================================================================
// External variables / declarations
//=============================================================================

// Public functions defined in this module

// Public from other modules
#ifdef MAKE_EMV
extern void PrintKeyTable( void );
#endif
#ifdef MAKE_CERTIFICATE
#include "crttab.h"
#endif // MAKE_CERTIFICATE

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
static void inet_ntoapp( void *pTermFld );
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
//static void ToggleBit (UBYTE * pByteValue, UBYTE BitToggler);
static BYTE InputTel( UBYTE * pPhoneData, const ENTRY_STR * pEntry ); // @AAMELIN

static void Fun_8( void );
static void Fun_21( void );
//static void Fun_37( void );
static void Fun_54 ( void );

static BYTE InputIPCONFIG( void );
static BYTE InputNMSIP( void );
static BYTE InputNMSPORT( void );
static BYTE InputINITIP( void );
static BYTE InputINITPORT( void );
static BYTE InputLOCALIP( void );
static BYTE InputLOCALSUBNET( void );
static BYTE InputGATEWAYIP( void );
static BYTE InputIP(IPADDRESS* ip, const ENTRY_STR * pEntry);
static BYTE InputPort(UWORD* port, const ENTRY_STR * pEntry);

#ifdef MAKE_SMARTINIT
static void Fun_55 ( void );
#endif // MAKE_SMARTINIT

static void Fun_76 ( void );
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
	MercFunction,				// Title
	N_NullStr,					// Default value
	N_EnterFunction,			// Prompt
};

static const ENTRY_STR Fun99PassEntry = {
	MaintFunction,				// Title
	SVPassword,					// Prompt
	N_NullStr,					// Default value
};

static const ENTRY_STR Fun90Entry = {
	N_PswdChange,				// Title
	OldPassword,				// Prompt
	N_NullStr,					// Default value
};

static const ENTRY_STR Fun90Entry1 = {
	N_PswdChange,				// Title
	NewPassword,				// Prompt
	N_NullStr,					// Default value
};

static const ENTRY_STR Fun90Entry2 = {
	N_PswdChange,				// Title
	NewPassAgn,					// Prompt
	N_NullStr,					// Default value
};

static const ENTRY_STR APTermSer = {
	EnterTSer,					// Prompt
    CustomMsg,                  // Line 1
	N_NullStr,					// Line 2
};


static const ENTRY_STR Fun99Entry = {
	ProgFunction,				// Title
	BlankMsg,					// Prompt
	N_NullStr,					// Default value
};

static const ENTRY_STR Fun99_9Entry = {
	Type,						// Title
	BlankMsg,					// Prompt
	N_NullStr,					// Default value
};



static const ENTRY_STR InpTelEntry = {
	EnterInitTel,				// Title
    CustomMsg,                  // 1-st line
	N_NullStr,					// 2-nd line
};


static const ENTRY_STR InpNMSEntry = {
	EnterNMSTel,				// Title
	CustomMsg,					// 1-st line
    N_NullStr,                  // 2-nd line
};

static const ENTRY_STR InpDBInitEntry = {
	EnterDBInitTel,				// Title
    CustomMsg,                  // 1-st line
	N_NullStr,					// 2-nd line
};


static const ENTRY_STR InpDBNMSEntry = {
	EnterDBNMSTel,				// Title
	CustomMsg,					// 1-st line
    N_NullStr,                  // 2-nd line
};

static const ENTRY_STR PABXEntry = {
	N_PABX,						// Title
    CustomMsg,                  // 1-st line
	N_NullStr,					// 2-nd line
};


static const ENTRY_STR NMSIPEntry = {
	EnterNMSIp,					// Title
    CustomMsg,                  // 1-st line
	N_NullStr,					// 2-nd line
};

static const ENTRY_STR NMSPortEntry = {
	EnterNMSPort,				// Title
    CustomMsg,                  // 1-st line
	N_NullStr,					// 2-nd line
};

static const ENTRY_STR InitIPEntry = {
	EnterInitIp,				// Title
    CustomMsg,                  // 1-st line
	N_NullStr,					// 2-nd line
};

static const ENTRY_STR InitPortEntry = {
	EnterInitPort,				// Title
    CustomMsg,                  // 1-st line
	N_NullStr,					// 2-nd line
};

static const ENTRY_STR LocalIPEntry = {
	EnterLocalIp,				// Title
    CustomMsg,                  // 1-st line
	N_NullStr,					// 2-nd line
};

static const ENTRY_STR LocalSubnetEntry = {
	EnterLocalSubnet,			// Title
    CustomMsg,                  // 1-st line
	N_NullStr,					// 2-nd line
};

static const ENTRY_STR GatewayIPEntry = {
	EnterLocalGateway,			// Title
    CustomMsg,                  // 1-st line
	N_NullStr,					// 2-nd line
};


static const ENTRY_STR VoidPwdEntry =	// Prompt for Void Password
{
	N_Passwords,				// Title
	VoidPassM,					// Prompt
	N_NullStr,					// Prompt line 2
};

static const ENTRY_STR RefundPwdEntry =	// Prompt for Refund Password
{
	N_Passwords,				// Title
	RefPassM,					// Prompt
	N_NullStr,					// Prompt line 2
};

static const ENTRY_STR AdjustPwdEntry =	// Prompt for Refund Password
{
	N_Passwords,				// Title
	AdjPassM,					// Prompt
	N_NullStr,					// Prompt line 2
};

static const ENTRY_STR ReportPwdEntry =	// Prompt for Refund Password
{
	N_Passwords,				// Title
	RepPassM,					// Prompt
	N_NullStr,					// Prompt line 2
};

static const ENTRY_STR KbdDialEntry =	// Prompt if Handset Dialing Allowed
{
	N_Dial,						// Title
	KbdDialM,					// Prompt
	N_NullStr,					// Prompt line 2
};

static const ENTRY_STR TollDialEntry =	// Prompt if Toll Dialing Allowed
{
	N_Dial,						// Title
	TollDialM,					// Prompt
	N_NullStr,					// Prompt line 2
};

static const ENTRY_STR MemDialEntry =	// Prompt if Memory (Fast) Dialing Allowed
{
	N_Dial,						// Title
	MemDialM,					// Prompt
	N_NullStr,					// Prompt line 2
};

static const ENTRY_STR RefDialEntry =	// Prompt if Referral Dialing Allowed
{
	N_Dial,						// Title
	RefDialM,					// Prompt
	N_NullStr,
};

static const ENTRY_STR ReceiptEntry = {
    CustomMsg7,                 // Title
	CustomMsg,					// Default Value (line 1)
	N_NullStr,					// Empty 
};

static const ENTRY_STR CU_AdR_Entry = {
	N_RS232,					// Title
	N_NullStr,					// Default Value (line 1)
	CustomMsg,					// Default Value overflow (line 2)
};

static const ENTRY_STR ModemMode_Entry = {
	N_CommsModemMode,			// Title
	CustomMsg,					// Default Value
	N_NullStr,					// Empty
};

static const ENTRY_STR DBModemMode_Entry = {
	N_CommsDBModemMode,			// Title
	CustomMsg,					// Default Value
	N_NullStr,					// Empty
};

static const ENTRY_STR Sug_Tip_Entry = {
	Msg_Tips,					// Title
	Msg_Sug_Tip,				// Default Value (line 1)
	CustomMsg4,					// Default Value overflow (line 2)
};

static const ENTRY_STR InitTries_Entry =
{
    InitMaxTries,               // Title
    CustomMsg,                  // Default Value
	N_NullStr,                  // Empty
};

static const ENTRY_STR Fun_76_Entry =	// structure for Fun_76 entry
{
	N_Report,
	N_LastIPaddr,
	CustomMsg,
};

//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  MerchFunction    Handle Merchant Function Selection
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void MerchFunction( void )
{
	int FuncNum;

	memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );	
	        
	while ( True )
	{
		// Prompt for Merchant Function number
		if ( !NumEntry( 1, 2, 1, &MercFunctionEntry ) )
			break;				// User canceled or timed out

		// Convert number entered
		FuncNum = atoi( Dspbuf );

		// Check number entered
		switch ( FuncNum )
		{		
                        // batch tarama 
			case 1:
				CurrentEvent.NEXTEVENT = REVIEW;	// Batch Review
				return;

                        // yap�land�rma ayarlar� 
			case 2:
				TerminalSetup ();
				return;
                        // terminal reset 
			case 3:
				Fun_3(  );		// Program Restart
				return;			
			
                        // batch bilgilerini g�r�nt�leme
			case 8:
				Fun_8(  );		// Display Batch
				return;
			
                        // tarih saat ayarlama 
			case 10:                          
				Fun_10 ();
				return;

                        // Demo Moduna Ge�i� 
			case 12:
				CurrentEvent.NEXTEVENT = TRAINMODE;
				return;
			
                        // Ka��t �lerletme
			case 15:
				CurrentEvent.NEXTEVENT = PAPERFEED_FUN;
				return;

			// Ekran constrast�n� ayarlama
			case 16:
				CurrentEvent.NEXTEVENT = SETCONTRAST_FUN;
				return;
                        
                        // Dil Se�imi
			case 18:
				TextLanguageSelectMenu ();
				return;			
                        
                        // invoice numaras�n� girerek batchte bulunan bir kayd� g�rme 
			case 21:
				Fun_21(  );		
				return;			
                        
                        // sliplerin ve raporlar�n 4 sat�r�n� girme 
			case 26:
				Fun_26(  );		
				return;			
							
                        // Pin Pad Bilgisi 
			case 32:
				PinDisplayDiag ();
				return;

#ifdef MAKE_EMV
                        
                        // Ayr�nt�l� EMV Raporlar� ile i�lem ger�ekle�tirme
			case 37:
				Fun_37(  );		// Toggle EMV debug printing
				return;
                        
                        // RSA Key tablosunun bas�m� 
                        case 38:
				PrintKeyTable(  );    // Print RSA key table
				return;
#endif	// MAKE_EMV

                        // parametre y�kleme i�leminin maksimum deneme say�s�n� de�i�tirme 
                        case 54: 
                                Fun_54( );		// Set Max Tries for Init Recovery
				return;

#ifdef MAKE_SMARTINIT
			case 55: 
				Fun_55( );              // Toggle SmartInit
                                return;
#endif	// MAKE_SMARTINIT
                        
                        // Mail Order i�lemi 
			case 65:
				CurrentEvent.NEXTEVENT = MAIL_ORDER;	// Mail/Phone Order Transaction
				return;
			
                        // Yaz�c�n�n inaktif ve aktif edilmesi 
			case 71:
				Fun_71(  );		// Printer Enable/Disable
				return;

                        
                        // Son i�leme ait slibin tekrar bas�m� 
			case 72:
				JrnReprintLast ();
				return;
                        
                        // Herhangi bir i�lemin slibinin tekrar bas�m� 
			case 73:
				JrnReprintAny ();
				return;
                       
                        // IP adresi 
                        case 76:
				Fun_76 ( );                         
				return;
			
                        // Raporlar 
			case 79:
				Reports ();
				return;

                        // Yaz�c� Testi 
			case 82:
				TestPrinter ();
				return;
                        
                        // Ekran testi 
			case 83:
				Fun_83(  );		
				return;
                        
                        // Klavye Testi 
			case 84:
				Fun_84(  );		
				return;

#ifdef MAKE_SCREEN_CAPTURE
			case 88:
				Fun_88(  );		
				return;
#endif // MAKE_SCREEN_CAPTURE
                        
                        // Kart Okuyucu Testi 
			case 89:
				Fun_89(  );		
				return;
                        
                        // �ifre de�i�tirme (�ifre de�i�iklikleri bir sonraki parametre y�kleme 
                        // i�lemine kadar ge�erlidir.)
			case 90:
				Fun_90(  );		
				return;
                        
                        // Yaz�l�m bilgilerinin g�sterimi 
			case 91:
				Fun_91(  );		// Show Software Version
				return;
			
                        // PinPad Testi 
			case 94:
				Fun_94(  );		
				return;

                        // Yerel se�eneklerin de�i�imi ( �ifre ) 
			case 97:
				Fun_97(  );		
				return;
                        
                        // ba�lant� ayarlar�n�n de�i�tirilmesi 
			case 98:
				Fun_98(  );		
				return;
                        
                        // baz� �zel fonksiyonlar� i�erir (batch temizleme , reversal silme vb)                  
			case 99:
				Fun_99(  );		
				return;

			default:			// ge�ersiz giri� 
				ShowErrMsg( InvalidEntry );
				break;
		}
	}
}


//-----------------------------------------------------------------------------
//  Fun_0           Test Transaction
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void Fun_0( void )
{
	UBYTE status, num_hosts;

	num_hosts = AqidCount(  );	// count the number of acquirers

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

		do						// Call the selected option processor
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
				break;			// cancel or Time-Out
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
//  Fun_2           Merchant Programming
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
//  Fun_3           Restart Terminal
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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

				// restart comms to make sure we have a clean start 
				COM_ResetReq();
				while( !COM_ResetCompleted() )
				// Relinquish the CPU while we reset
					SDK_RelinqCPU(  );
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
//  Fun_4           Lock Keyboard
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
	if ( ENTER_KY == keyval )	// If YES Key
	{
		// Lock the Kyboard
		TCONF.TCLOCOP |= TCL_LOCKED;

		// Set TCONF's LRC
		SetTCTabLRC(  );
	}
}



//-----------------------------------------------------------------------------
//  Fun_21          Find an entry in batch and start reviewing batch.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

static void Fun_21( void )
{
	Find ();
}


//-----------------------------------------------------------------------------
//  Fun_26          Accept input for the receipt/report header/footer
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
			case 0:			// User Timeout
			case CANCEL_KY:	// User Aborted Transaction
				return;
			case '0':
			case 'H':			// User Selected Header Line 4
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
			USRDLRC = CalcLrc( ( UBYTE * ) & USRDTAB, USRDSZE );
		}
		else
		{
			// Set the Footer Table LRC
			SetFTTabLRC(  );
		}
	}						// End of while loop
}


#ifdef MAKE_EMV
//-----------------------------------------------------------------------------
//  Fun_34          Enable EMV exception file flag
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------
/*
void Fun_34( void )
{
    UBYTE keyval;

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

    if ( ENTER_KY == keyval )	// If YES Key toggle bit
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

    if ( ENTER_KY == keyval )	// If YES Key
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

    if ( ENTER_KY == keyval )	// If YES Key
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

    if ( ENTER_KY == keyval )	// If YES Key
    {
            ToggleBit (&LocalAppCap[1], APP_CAP2_EMV_FORCED);
    }

    PrintAppCap();
}
*/
//-----------------------------------------------------------------------------
//  ToggleBit: Toggle one bit of a byte
//  Parameters:     None
//  Global Inputs: None
//  Returns:        None
//  Notes: Created to take care of the bitmap flags
//
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
//  Fun_37          Enable EMV debug printing
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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

	if ( ENTER_KY == keyval )	// If YES Key
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

/*------------------------------------------------------------------
 *  Name:           Fun_54
 *  Description:    Set Max Tries for Init Recovery mechanism
 *  Include files:  
 *  Parameters:     None.
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/
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
//  Fun_55          Toggle SmartInit
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
//  Fun_71          Printer Enable/Disable
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
			if ( ENTER_KY == keyval )	// If YES       
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

			if ( ENTER_KY == keyval )	// If YES       
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
//  Fun_83          Display Test
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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

           if ( 2 == Step )
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
                    
           if ( 3 == Step )
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
//  Fun_84          Keyboard Test
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
//  Fun_89          Cardreader Test
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void Fun_89( void )
{
	UBYTE i, draw;
	UBYTE InputType;
	Bool bPinCardStatus;

	draw = 0;
	Enable_Flags[3] = 0;		// Turn off second line of display

	// Loop till done
	do
	{
		if ( !draw )
		{
			draw = 1;
			Enable_Flags[3] = 0;	// Turn off second line of display

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
							KeyVal = CANCEL_KY;	// Yes; exit the loop
							break;
						case DEV_KBD:
							// Yes; check for CANCEL_KY Key
							if ( CLEAR_KY == KeyVal )
								KeyVal = CANCEL_KY;	// Yes; exit the loop

							if ( CANCEL_KY != KeyVal )
							{
								SDK_Beeper( TENMS * 40 );	// No; notify the user
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
					KeyVal = CANCEL_KY;	// Yes; exit the loop
					break;
				case DEV_KBD:
					// Yes; check for CANCEL_KY Key
					if ( CLEAR_KY == KeyVal )
						KeyVal = CANCEL_KY;	// Yes; exit the loop

					if ( CANCEL_KY != KeyVal )
						SDK_Beeper( TENMS * 40 );	// No; notify the user

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
//  Fun_90          Password Change
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void Fun_90( void )
{
	UBYTE status;
	UBYTE TempPbuf[4];			// Temp Password buffer

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
//  Fun_91          Show Software Version
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void Fun_91( void )
{
	VERSION_INFO sdkversion;
	UBYTE retval;
#ifdef MAKE_EMV
	UBYTE ver[25]     ; // temp array for storing version strings
	UBYTE Z8VersionID ; // version ID not used in Fun 91 but required for ScrdVersion
#endif // MAKE_EMV

	// Display the software version
	memcpy( CSTMSG, VERSION, 10 );
	CSTMSG[10] = '\0';

	// Display the downline load revision
	BufAsc( ( char * ) CSTMSG2, &TCONF.TCDLL );
	CSTMSG2[2] = '\0';

    // Display the BOOTER version 
    // "BOOTER FU065A20" string expected from EFTOS
    // skip "BOOTER" word
    const char *ptr;
    for ( ptr = HW_Booter_Version(); *ptr != ' '; ptr++ );

    // Copy the rest
    strcpy (CSTMSG3, ptr);

	// Display the Terminal ID
	memcpy( CSTMSG4, ( UBYTE * ) TERM.TERMSer, 8 );

	// See if 8 digit Serial Number required
	if ( TERM.TERMOpt1 & TM1_TSER8 )
	{
		// Yes; 8 digit Serial Number for display
		CSTMSG4[8] = '\0';
	}
	else
	{
		// No, 9 digit Serial Number

		// Add Terminal Serial Number Check Digit to display
		CSTMSG4[8] = TERM.TERMChk;
		CSTMSG4[9] = '\0';
	}

	// Display Cancel Box
	Enable_Flags[0] = 1;

	// Display Screen
	DrawScreen( SOFT_VER_SCR );

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
	strcpy ( CSTMSG, "EMVL1" ) ;

	ScrdVersion ( (char *) ver, &Z8VersionID ) ;
	memcpy ( &CSTMSG2[0], &ver[7], 4 ) ;
	CSTMSG2[4] = '\0';

	// Read the library version string
	strcpy ( CSTMSG3, "EMVL2 " ) ;

	EMVL2_GetVersionString( ver );
	memcpy ( &CSTMSG4[0], ver, 5 ) ;
	CSTMSG4[5] = '\0';

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
	strcat( CSTMSG, sdkversion.SDKvariant );
	strcat( CSTMSG, "_" );
	strcat( CSTMSG, sdkversion.SDKversion );

	strcpy( CSTMSG2, sdkversion.OSrelease );

	strcpy( CSTMSG3, sdkversion.OSversion );

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
//  Fun_94          PinPad TEST Message
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void Fun_94( void )
{
	UBYTE retval;
	char LoopFlag = True;
	Bool bPinRetVal;

	// Reset PINPAD	variables
	PIN_TYPE = PIN_TYPE_INVALID;
	PINSTAT = 0;

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
//  Fun_95          Network Statistics
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
//  Fun_97          Local Options Select
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
				case 'P':		// Password options
					if ( !Fun_97_1(  ) )
						loop = False;
					else
					{
						// Draw the Local Options Menu
						DrawMenu( MENU_PREVIOUS );
					}
					break;

				case 'T':		// Tip Percentages
					if ( !SetTipPercent(  ) )
						loop = False;
					else
					{
						// Draw the Local Options Menu
						DrawMenu( MENU_REDRAW );
					}
					break;

				case B_PREVIOUS:	// Previous
					DrawMenu( MENU_PREVIOUS );
					break;

				case B_NEXT:	// Next
					DrawMenu( MENU_NEXT );
					break;

				case B_PREVIOUS_NEW_MENU:	// Previous - new menu
					return;

				case CANCEL_KY:	// Timed out or cancelled
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
//  Fun_98          Local Configuration
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
    memcpy ( &tmpRSTAB, &RSTAB, RSSZE);

	// Choose the communication
	if ( 0 == RS232CFG() )

    	// Display/Set Modem Mode
        if ( InModemMode(  ) )

			// Display/Set DB Modem Mode
			if ( InDBModemMode(  ) )
				// Enhance Dial Messages?, Exit if user presses Cancel
				if ( EnhDialM(  ) )

					// Display/Set CU Address
					CUAddress(  );

	// restart comms to make sure we have a clean start
	COM_ResetReq();
	while( !COM_ResetCompleted() )
		// Relinquish the CPU while we reset
		SDK_RelinqCPU(  );

	// Reset POS LAN if CU addr was changed
    if ( ( TM3_LAN == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) &&
         ( tempTERMCuAd != TERM.TERMCuAd ) 
        )
    {
        ResetCommModule ( );
    }

    // Reassign COM1/COM2 if RSTAB was changed
    if ( memcmp ( &tmpRSTAB, &RSTAB, RSSZE) )
    {
        Assign_COM1_COM2( );
    }

    // Return
	return;

}



//-----------------------------------------------------------------------------
//  Fun_99          Local Functions
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
//  Fun_99_9_1      Maintenance - Clear tables and journal.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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

#ifdef MAKE_CERTIFICATE
	// Delete certificate file if exist
	remove( CERTIFICATE_FILE );
#endif // MAKE_CERTIFICATE

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
//  Fun_99_9_2      Maintenance - Clear acquirer
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void Fun_99_9_2( void )
{
	UBYTE status;
	struct acquirer_rec *pAcquirerPtr;	// Pointer to AQTAB entry
	struct batch_rec *pBatch_Table; // Pointer to BATTAB entry
	char num_hosts;

	num_hosts = AqidCount(  );	// count the number of acquirers

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
		do						// Call the selected option processor
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
				break;			// exit

			}
			else if ( ( CANCEL_KY == status ) ||
					  ( CLEAR_KY == status ) || ( 0 == status ) )
			{
				break;			// Cancel or Time-Out
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
//  Fun_99_9_6      Maintenance - Clear all reversals from the journal.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
//  Fun_99_96       Print Host List
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void Fun_99_96( void )
{
	// Notify user -- "PRINTING, WAIT..."
	ShowInfoMsg( PrintingWait, PleaseWait );

	PrintHostList ();
}


//-----------------------------------------------------------------------------
//  Enter99Pswd     Enter Function 99 Password
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool    True  - if password entered
//                          False - if cancel key hit.
//
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool Enter99Pswd( void )
{
	UBYTE Status;
        UBYTE	RtcBuf[RTCLEN];   	/* RTC Read Buffer */
	UBYTE	t;
	UBYTE	*ptr;
        
        
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
			return ( False );
		}

		// Read System Time 
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
	return ( True );
}


//-----------------------------------------------------------------------------
//  CUAddress       Show Current Line Type       
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool    True:  new entry enteredNone
//                          False: current value accepted
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool CUAddress( void )
{
	Bool retval;
	UBYTE length;

	retval = True;

    // Skip this function if GSM modem or Ethernet or Bluetooth or COM1/COM2
    // used for communication
    if( ( TM3_GSM == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
        ( TM3_GSM_OLD == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
        ( TM3_ETHERNET == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
        ( TM3_BLUETOOTH == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
        ( TM3_ISDN == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
        ( RSTAB.RSOPT & RSOPT_RSACTIVE ) )
        return True;

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
	return ( retval );
}


//-----------------------------------------------------------------------------
//  InModemMode       Show and adjust Modem Mode       
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool    True:  Enter was pressed
//                          False: Cancel or Clear was pressed
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InModemMode( void )
{
	Bool retval;
	UBYTE length;

    // Skip this function if Ethernet or POS LAN used
    if ( ( TM3_LAN == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
          ( TM3_ETHERNET == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) )
        return True;

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
		return ( False );
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
	return ( True );
}

//-----------------------------------------------------------------------------
//  InDBModemMode       Show and adjust DB Modem Mode       
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool    True:  Enter was pressed
//                          False: Cancel or Clear was pressed
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InDBModemMode( void )
{
	Bool retval;
	UBYTE length;

    // Skip this function if Ethernet or POS LAN used
    if (  TM3_DIALBACKUP != (TERM.TERMOpt3 & TM3_HWTYPE_MASK)  )
        return True;

	// Get the Current Modem Mode
	GetMsg( Currently, CSTMSG );

	// Get the length of the prompt
	length = strlen( CSTMSG );

	// Blank out the Nul terminator
	CSTMSG[length++] = ' ';

	// Convert from BCD to ASCII
	BfAscii( &CSTMSG[length++], ( UBYTE * ) & DBModem,
			 sizeof( TERM.TERMModem ) );

	// Null terminate the result for display
	CSTMSG[++length] = '\0';

	GlobalDsp = ZAPKEYON;

	retval = AlphaNumEntry( 1, 2, 0, &DBModemMode_Entry );

	if ( 0 == retval )
	{
		return ( False );
	}
	else
	{
		if ( 2 == retval )
		{
			// ZAP Key Pressed, Clear field
			DBModem = 0;
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
				AscHex( ( UBYTE * ) & DBModem, Dspbuf, 1 );
			}
		}
		// Set the TERM Table LRC to save change
		SetDialBackLRC(  );
		retval = True;
	}

	// Current value accepted
	return ( True );
}

//-----------------------------------------------------------------------------
//  TranAcc         Diplay Transaction Accecpted and beep
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

static void TranAcc( void )
{
	ShowInfoMsg( TransAccepted, N_Response );

	SDK_BeepIt( 2 );
	SDK_Wait( ONESECOND * 2 );
	return;
}


//-----------------------------------------------------------------------------
//  XInputTermSer   Input Terminal Serial Number
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void XInputTermSer( void )
{
	InputTermSer(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
}



//-----------------------------------------------------------------------------
//  XInputInitTel   input Initialization Telephone Number
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void XInputInitTel( void )
{
	InputInitTel(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu

}

//-----------------------------------------------------------------------------
//  XInputNMSTel    Input NMS Telephone Number
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void XInputNMSTel( void )
{
	InputNMSTel(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu

}


//-----------------------------------------------------------------------------
//  XInputDialMode  Input Dial Mode
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void XInputDialMode( void )
{
	InputDialMode(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu

}



//-----------------------------------------------------------------------------
//  XInputPABX      input PABX Access Code
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------
extern void XInputPABX( void )
{
	InputPABX(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
}


//                               Added by Ufuk Varol 14.03.2007
//BEGIN
//-----------------------------------------------------------------------------
//  XInputIPCONFIG  input IP Configuration Mode
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------
extern void XInputIPCONFIG( void )
{
	InputIPCONFIG(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
}




//-----------------------------------------------------------------------------
//  XInputNMSIP		input NMS Dest. IP Address
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------
extern void XInputNMSIP( void )
{
	InputNMSIP(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
}

//-----------------------------------------------------------------------------
//  XInputNMSPORT	input NMS Port Number
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------
extern void XInputNMSPORT( void )
{
	InputNMSPORT(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
}

//-----------------------------------------------------------------------------
//  XInputINITIP    input Init IP Address
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------
extern void XInputINITIP( void )
{
	InputINITIP(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
}

//-----------------------------------------------------------------------------
//  XInputINITPORT  input Init Port Number
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------
extern void XInputINITPORT( void )
{
	InputINITPORT(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
}

//-----------------------------------------------------------------------------
//  XInputLOCALIP   input Local IP Address
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------
extern void XInputLOCALIP( void )
{
	InputLOCALIP(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
}

//-----------------------------------------------------------------------------
//  XInputLOCALSUBNET  input Local Subnet Mask
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------
extern void XInputLOCALSUBNET( void )
{
	InputLOCALSUBNET(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
}

//-----------------------------------------------------------------------------
//  XInputGATEWAYIP  input GateWay IP Address
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------
extern void XInputGATEWAYIP( void )
{
	InputGATEWAYIP(  );
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
}


//END
//-----------------------------------------------------------------------------
//  TermAll         Do all terminal setup steps
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
		memset( ( UBYTE * ) Dspbuf, 0, 8 );	// Leave title box alone
		memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );	// Init default value display buffer
		// Case statement based on input step
		switch ( Step )
		{
			case 1:			// Input Terminal Serial Number
				Status = InputTermSer(  );
				break;

			case 2:			// Input Dial Backup Initialization Telephone Number
				if (TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK))
				{
					Status = InputDBInitPref(  );
					break;
				}
				else if((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET)
				{
					Status = InputIPCONFIG();
					break;
				}
				else
				  Step++;
			case 3:			// Input Dial Backup Initialization Telephone Number
				if (TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK))
				{
					Status = InputDBNMSPref(  );
					break;
				}
				else if((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET)
				{
					Status = InputNMSIP();
					break;
				}
				else
				  Step++;
			case 4:			// Input Initialization Telephone Number
				if((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET)
				{
					Status = InputNMSPORT();					
				}
				else
				{
					Status = InputInitTel(  );					
				}
				break;
			case 5:			// Input Dial Backup Initialization Telephone Number
				if (TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK))
				{
					Status = InputDBInitTel(  );
					break;
				}
				else if((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET)
				{
					Status = InputINITIP();
					break;
				}
				else
				  Step++;

			case 6:			// Input NMS Telephone Number
				if((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET)
				{
					Status = InputINITPORT();					
				}
				else
				{
					Status = InputNMSTel(  );					
				}
				break;
			case 7:			// Input Dial Backup NMS Telephone Number
				if (TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK))
				{
					Status = InputDBNMSTel(  );
					break;
				}
				else if((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET)
				{
					Status = InputLOCALIP();
					break;
				}
				else
				  Step++;

			case 8:			// Input Dial Mode
				if((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET)
				{
					Status = InputLOCALSUBNET();
				}
				else
				{
					Status = InputDialMode(  );
				}
				break;
			case 9:			// Input PABX Access Code
				if((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET)
				{
					Status = InputGATEWAYIP();
				}
				else
				{
					Status = InputPABX(  );
				}
				break;

		}

		// Case statement based on status
		switch ( Status )
		{
			case -1:			// Go to previous step
				Step--;
				break;

			case 0:			// Cancel
				Step = 0;
				break;

			case 1:			// Go to next step
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
//  Fun_97_1        Local Options 1 Select - password options
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool    False for cancel or timout, else True
//
//  Notes:
//
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
			case 'V':			// Void
				retval = DoPasswordEnable( &VoidPwdEntry, TCL_NO_VPWD );
				break;

			case 'R':			// Refund
				retval = DoPasswordEnable( &RefundPwdEntry, TCL_NO_RPWD );
				break;

			case 'A':			// Adjust
				retval = DoPasswordEnable( &AdjustPwdEntry, TCL_NO_APWD );
				break;

			case 'P':			// Report
				retval = DoPasswordEnable( &ReportPwdEntry, TCL_NO_RPPW );
				break;

			case 'L':			// All 4
				retval = DoPasswordEnable( &VoidPwdEntry, TCL_NO_VPWD );
				if ( retval != 'N' )	// anything but Next?
					break;
				retval = DoPasswordEnable( &RefundPwdEntry, TCL_NO_RPWD );
				if ( retval != 'N' )	// anything but Next?
					break;
				retval = DoPasswordEnable( &AdjustPwdEntry, TCL_NO_APWD );
				if ( retval != 'N' )	// anything but Next?
					break;
				retval = DoPasswordEnable( &ReportPwdEntry, TCL_NO_RPPW );
				break;

			case B_PREVIOUS:	// Previous
				DrawMenu( MENU_PREVIOUS );
				break;

			case B_NEXT:		// Next
				DrawMenu( MENU_NEXT );
				break;

			case B_PREVIOUS_NEW_MENU:	// Previous - new menu
				return True;

			case CANCEL_KY:	// Timed out or cancelled
			case CLEAR_KY:
			case 0:
				retval = 'C';
				break;

			default:
				break;

		}

		if ( retval == 'N' )
			DrawMenu( MENU_REDRAW );	// redraw password menu


	}
	while ( retval != 'C' );

	return False;
}


//-----------------------------------------------------------------------------
//  PasswordOptions     Local Options Select for passwords
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:
//
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
//  Fun_14          ICEPAK graphics download
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
		else if( (UBYTE)0xFF == NullComp( (char *) IPTAB.IPSID, S_IPSID ) )
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
	if ( ENTER_KY == retval )	// If YES Key
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
//  Reprint         Reprint function
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
			case 'C':			// reprint last receipt
				JrnReprintLast ();
				DrawMenu( MENU_REDRAW );
				break;

			case 'D':			// reprint any receipt
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
//  TipPercentages  Local Options Select for Tip Percentages
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
//  SettleStatus    Prompt the user to either view or Print the
//                  Settlement Status.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void SettleStatus( void )
{
	while ( 1 )
	{
		// Display a PRINT and VIEW button screen
		DrawScreen( SETTLE_STAT_SCR );

		// Get Keypad button entry
		switch ( GetOneKey( DEV_PAD, UserTimeout ) )
		{
			case 'P':			// Print Settlement Summary
				// Do Issuer Receipt
				SettleSumRpt ();
				break;

			case 'V':			// View Settlement Summary
				ViewSettleSum ();
				break;

			case CANCEL_KY:	// Timed out or cancelled
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
//  Fun_8           Display Current Batch
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

static void Fun_8( void )
{
	struct acquirer_rec *pAcquirerPtr;	// Pointer to AQTAB entry
	struct acquirer_rec *pHostPtr;	// Pointer to AQTAB entry
	UBYTE keyval;
	struct batch_rec *pBatch_Table;	// Pointer to BATTAB entry
	UBYTE length;
	UBYTE tmpaqid;

	pHostPtr = FindFirstAcquirer(  );


	Enable_Flags[0] = 1;		// Default to Next Arrow ON
	Enable_Flags[7] = 0;		// Previous Arrow OFF

	while ( NULL != pHostPtr )
	{
		// Look for Previous Batch
		if ( NULL == FindPrevAcquirer( pHostPtr ) )
			Enable_Flags[7] = 0;	// Turn Previous Arrow OFF

		// Look for Next Batch
		if ( NULL == FindNextAcquirer( pHostPtr ) )
			Enable_Flags[0] = 0;	// Turn Next Arrow OFF

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
				Enable_Flags[0] = 1;	// Turn Next Arrow ON
			}
			else
				Enable_Flags[7] = 0;	// Turn Previous Arrow OFF

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
					Enable_Flags[7] = 1;	// Turn Previous Arrow ON
				}
				else
					Enable_Flags[0] = 0;	// Turn Next Arrow OFF
			}
		}
	}

	// No Acquirer set
	ShowErrMsg( NoAcq );
}


//-----------------------------------------------------------------------------
//  Trk2TOCstMsg    Copy Track 2 data without Discretionary data.
//                  The data is returned in CSTMSG, with no wrap.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          Original function ShowTrk2, without wrap.
//
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
//  ClearAcquirer   Maintenance - Part of clear batch
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
//  InputTermSer    Input Terminal Serial Number
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        BYTE    1 - current Terminal Serial Number accepted
//                              or new Terminal Serial Number entered
//                          0 - input cancelled or timed-out.
//
//  Notes:
//
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
							return ( 1 );
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
							return ( 1 );
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
			return ( 0 );
		}
	}

	// Return okay
	return ( 1 );
}


//-----------------------------------------------------------------------------
//  EnhDialM        Check if Enhanced Dial Messages set
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool    True:  Set to ENHANCED MESSAGESNone
//                          False: Not set to ENHANCED MESSAGES
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool EnhDialM( void )
{
	UBYTE retval;

	// Prompt for Enhanced Dialing Messages
	YNEntry.TitleMid = N_Comms;
	YNEntry.Line1Mid = EnhDial;
	YNEntry.Line2Mid = N_DialMsgs;

	retval = YesNoEntry( &YNEntry );

	if ( ENTER_KY == retval )	// If YES       
	{

		// Set to ENHANCED MESSAGES
		TCONF.TCDOPT |= TCD_ENH;
		retval = True;
	}
	else
	{
		if ( CLEAR_KY == retval )	// If NO        
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

	return ( retval );
}



//-----------------------------------------------------------------------------
//  InputInitTel        Input Initialization Telephone Number
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current Init Telephone Number accepted
//                                 or new Init Telephone Number entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

static BYTE InputInitTel( void )
{
       return(	InputTel((UBYTE *)TERM.TERMInitTel,&InpTelEntry) );
}

//-----------------------------------------------------------------------------
//  InputNMSTel         Input NMS Telephone Number
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current NMS Telephone Number accepted
//                                 or new NMSt Telephone Number entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

static BYTE InputNMSTel( void )
{
       return(	InputTel((UBYTE *)TERM.TERMNmsTel,&InpNMSEntry) );
}

//-----------------------------------------------------------------------------
//  InputInitTel        Input Dial Backup Initialization Telephone Number
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current Init Telephone Number accepted
//                                 or new Init Telephone Number entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

extern BYTE InputDBInitTel( void )
{
	BYTE state;
       	state = InputTel((UBYTE *)DBInitTel,&InpDBInitEntry);
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
	return(state);
}

//-----------------------------------------------------------------------------
//  InputNMSTel         Input Dial Backup NMS Telephone Number
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current NMS Telephone Number accepted
//                                 or new NMSt Telephone Number entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

extern BYTE InputDBNMSTel( void )
{
	BYTE state;
       	state = InputTel((UBYTE *)DBNmsTel,&InpDBNMSEntry);
	if ( B_PREVIOUS == ButtonId )
		CurrentEvent.NEXTEVENT = MENU_REDRAW;	// redraw the terminal setup menu
	return(state);
}


//-----------------------------------------------------------------------------
//  InputTel         Input Telephone Number
//
//  Parameters:         PhoneData - pointer to phone data source and storage
//			pEntry  - pointer to screen data
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current NMS Telephone Number accepted
//                                 or new NMSt Telephone Number entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
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
			case 1:			// Okay; get length of input 
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

					// Recalculate Checksum 
					if (((UBYTE *)DBNmsTel == pPhoneData) || ((UBYTE *)DBNmsTel == pPhoneData))
                                                SetDialBackLRC ();
					else
                                        	SetTERMTabLRC();
				}

				ButtonId = B_PREVIOUS;
				// Set status to okay 
				Status = 1;

				break;

			case 3:			// Go back to previous entry 
				Status = -1;
				ButtonId = B_PREVIOUS;

				break;

			default:			// Entry cancelled or timed-out 
				Status = 0;

				break;
		}
	}
	while ( 99 == Status );

	// Return the status 
	return ( Status );
}

//-----------------------------------------------------------------------------
//  InputDialMode       Input Dial Mode
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - current Dial Mode accepted or new Dial Mode entered
//                                 or new NMSt Telephone Number entered 
//                             0 - input cancelled or timed-out
//                      
//  Notes:
//
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

			case 'A':			// TONE 
				TERM.TERMDMode = TMDM_TONE;
                SetTERMTabLRC(  );
                return 1;

			case 'B':			// PULSE 
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
//  InputDBInitPref       Input Init Preferred Service
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - current Dial Mode accepted or new Dial Mode entered
//                                 or new NMSt Telephone Number entered 
//                             0 - input cancelled or timed-out
//                      
//  Notes:
//
//-----------------------------------------------------------------------------
extern BYTE InputDBInitPref( void )
{
	ClearMenus ();

	// Draw Pulse/Tone menu
	DrawMenu( MENU_INIT_PREFERRED );

	// Determine which button should be displayed inverted 
	switch ( DBInitPrefferedService )
	{
	case 0x00:
            MenuItemCounter = 0; 
            break;

	case 0x01: 
            MenuItemCounter = 1; 
            break;
	case 0x02:
            MenuItemCounter = 2; 
            break;

	case 0x03: 
            MenuItemCounter = 3; 
            break;
	}
    
	while ( True )
	{
		switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
		{

		case 'A':			// GSM / GPRS with Dial back up 
			DBInitPrefferedService = 0x00;
                        SetDialBackLRC();
                	return 1;
		case 'B':			// Dial back up with GSM / GPRS
			DBInitPrefferedService = 0x01;
                        SetDialBackLRC();
                	return 1;
		case 'C':			// GSM / GPRS only
			DBInitPrefferedService = 0x02;
                        SetDialBackLRC();
                	return 1;
		case 'D':			//  Dial back up only
			DBInitPrefferedService = 0x03;
                        SetDialBackLRC();
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
//  InputDBNMSPref       Input NMS Preferred Service
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - current Dial Mode accepted or new Dial Mode entered
//                                 or new NMSt Telephone Number entered 
//                             0 - input cancelled or timed-out
//                      
//  Notes:
//
//-----------------------------------------------------------------------------
extern BYTE InputDBNMSPref( void )
{
	ClearMenus ();

	// Draw Pulse/Tone menu
	DrawMenu( MENU_NMS_PREFERRED );

	// Determine which button should be displayed inverted 
	switch ( DBNmsPrefferedService )
	{
	case 0x00:
            MenuItemCounter = 0; 
            break;

	case 0x01: 
            MenuItemCounter = 1; 
            break;
	case 0x02:
            MenuItemCounter = 2; 
            break;

	case 0x03: 
            MenuItemCounter = 3; 
            break;
	}
    
	while ( True )
	{
		switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
		{

		case 'A':			// GSM / GPRS with Dial back up 
			DBNmsPrefferedService = 0x00;
                        SetDialBackLRC();
                	return 1;
		case 'B':			// Dial back up with GSM / GPRS
			DBNmsPrefferedService = 0x01;
                        SetDialBackLRC();
                	return 1;
		case 'C':			// GSM / GPRS only
			DBNmsPrefferedService = 0x02;
                        SetDialBackLRC();
                	return 1;
		case 'D':			//  Dial back up only
			DBNmsPrefferedService = 0x03;
                        SetDialBackLRC();
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
//  InputPABX           Input PABX Telephone Number
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current NMS Telephone Number accepted
//                                 or new NMSt Telephone Number entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
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
			case 1:			// Success; copy the input into local buffer
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

			case 2:			// Zap key pressed; reset value to all F's
				memset( ( UBYTE * ) PABX, 'F', 8 );

				// Convert input from ASCII to BCD and save
				AscHex( ( UBYTE * ) TERM.TERMPabx, PABX,
						sizeof( TERM.TERMPabx ) );

				// Recalculate Checksum
				SetTERMTabLRC(  );

				// Need to redo input
				Status = 99;

				break;

			case 3:			// Go back to previous entry
				Status = -1;
				ButtonId = B_PREVIOUS;

				break;

			default:			// Entry cancelled or timed-out
				Status = 0;

				break;
		}
	}
	while ( 99 == Status );

	// Return the status
	return ( Status );
}


//                                  Added by Ufuk Varol 14.03.2007
//BEGIN
//-----------------------------------------------------------------------------
//  InputIPCONFIG       Input IP Configuration
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - current IP Configuration accepted or new Dial Mode entered
//                                 or new IP Configuration selected 
//                             0 - input cancelled or timed-out
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

static BYTE InputIPCONFIG( void )
{
	ClearMenus ();

        // Draw Pulse/Tone menu
	DrawMenu( MENU_IP_CONFIG );

        // Determine which button should be displayed inverted 
	if((TERM.TERMOpt1 & TM1_MANIP) == TM1_MANIP)
	{
		MenuItemCounter = 1; 
	}
	else
	{	
		MenuItemCounter = 0; 
	}
    
	while ( True )
	{
		switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
		{

			case 'A':			// Auto
				TERM.TERMOpt1 &= ~TM1_MANIP;
                                SetTERMTabLRC(  );
                                return 1;

			case 'B':			// Manual
				TERM.TERMOpt1 |= TM1_MANIP;
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
//  InputNMSIP	        Input NMS Dest. IP Address
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current NMS Dest. IP Address accepted
//                                 or new NMS Dest. IP Address entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

static BYTE InputNMSIP( void )
{
	return(	InputIP(&TERM.TERMNmsIP,&NMSIPEntry) );
}

//-----------------------------------------------------------------------------
//  InputNMSPORT        Input NMS Dest. Port Number
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current NMS Dest. Port Number accepted
//                                 or new NMS Dest. Port Number entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

static BYTE InputNMSPORT( void )
{
	return(	InputPort(&(TERM.TERMNmsPort),&NMSPortEntry) );
}
//-----------------------------------------------------------------------------
//  InputINITIP	        Input Init Dest. IP Address
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current Init Dest. IP Address accepted
//                                 or new Init Dest. IP Address entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

static BYTE InputINITIP( void )
{
	return(	InputIP(&(TERM.TERMInitIP),&InitIPEntry) );
}

//-----------------------------------------------------------------------------
//  InputINITPORT       Input Init Dest. Port Number
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current Init Dest. Port Number accepted
//                                 or new Init Dest. Port Number entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

static BYTE InputINITPORT( void )
{
	return(	InputPort(&(TERM.TERMInitPort),&InitPortEntry) );
}
//-----------------------------------------------------------------------------
//  InputLOCALIP	    Input Local IP Address
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current Local IP Address accepted
//                                 or new Local IP Address entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

static BYTE InputLOCALIP( void )
{
	return(	InputIP(&(TERM.TERMIPAddress),&LocalIPEntry) );
}

//-----------------------------------------------------------------------------
//  InputLOCALSUBNET	Input Local Subnet Mask
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current Local Subnet Mask accepted
//                                 or new Local Subnet Mask entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

static BYTE InputLOCALSUBNET( void )
{
	return(	InputIP(&(TERM.TERMSubnetIP),&LocalSubnetEntry) );
}
//-----------------------------------------------------------------------------
//  InputGATEWAYIP		Input Gateway IP
//
//  Parameters:         None.
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current Gateway IP accepted
//                                 or new Gateway IP entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------

static BYTE InputGATEWAYIP( void )
{
	return(	InputIP(&(TERM.TERMGatewayIP),&GatewayIPEntry) );
}

//-----------------------------------------------------------------------------
//  InputIP	            Input IP Address
//
//  Parameters:         ip - ip data source and storage
//						pEntry  - pointer to screen data
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current IP accepted
//                                 or new IP entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------
static BYTE InputIP(IPADDRESS* ip, const ENTRY_STR * pEntry)
{
	char Ip[20];
	struct in_addr input;	
	UBYTE length;
	BYTE Status;
        DWORD tmp;
	UBYTE tmpIpBytes[4];
	UDWORD tmpIp = 0;
	

	// Loop till done
	do
	{
		// Convert from UDWORD to ASCII
		memcpy(tmpIpBytes, ip, 4);
		tmpIp |= tmpIpBytes[0];
		tmpIp = tmpIp << 8;
		tmpIp |= tmpIpBytes[1];
		tmpIp = tmpIp << 8;
		tmpIp |= tmpIpBytes[2];
		tmpIp = tmpIp << 8;
		tmpIp |= tmpIpBytes[3];		
		memset(Ip, 0, 20);
		memcpy(&input, &tmpIp, sizeof(IPADDRESS));



		strcpy(Ip,inet_ntoa(input));	

		// Move IP Address to global display buffer 
		memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );
		MoveTillZero( CSTMSG, Ip, sizeof( CSTMSG ) );

		// Set clear key 
		GlobalDsp = CLRKEYON;

		// Check the input 
		switch ( IpNumEntry( 1, 15, 0, pEntry ) )
		{
			case 1:			// Okay; get length of input 
				length = StrLn( Dspbuf, sizeof( Dspbuf ) );
				memcpy( Ip, Dspbuf, length );

				// Check if an entry was input 
				if ( length )
				{
					// Yes; fill out remainder with 'F's 
					memset( ( UBYTE * ) ( Ip + length ),
							 0,( UWORD ) ( 20 - length ) );

					// Preset status to 1
					Status = 1;

					// Test for legal IP address
					if(!isaddr(Ip))
						break;

					// Convert input from ASCII to UDWORD and save 
                    tmp = aton(Ip);
					memcpy(ip,&tmp,4);					
					SetTERMTabLRC();
				}

				ButtonId = B_PREVIOUS;
				// Set status to okay 
				Status = 1;

				break;

			case 3:			// Go back to previous entry 
				Status = -1;
				ButtonId = B_PREVIOUS;

				break;

			default:			// Entry cancelled or timed-out 
				Status = 0;

				break;
		}
	}
	while ( 99 == Status );

	// Return the status 
	return ( Status );
}

//-----------------------------------------------------------------------------
//  InputPort           Input Port Number
//
//  Parameters:         port - port data source and storage
//						pEntry  - pointer to screen data
//              
//                      
//  Global Inputs:
//
//  Returns:            BYTE - 1 - Current port number accepted
//                                 or new port number entered 
//                             0 - input cancelled or timed-out
//                            -1 - go back to previous entry.
//                      
//  Notes:
//
//-----------------------------------------------------------------------------
static BYTE InputPort(UWORD* port, const ENTRY_STR * pEntry)
{
	char Port[5];
	UBYTE i;
	UBYTE length;
	BYTE Status;
	int tmp;

	// Loop till done
	do
	{
		// Convert from UWORD to ASCII
                memset(Port, 0, 5);
		itoa((int)(*port),Port,10);		

		// Move NMS Telephone Number to global display buffer 
		memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );
		MoveTillZero( CSTMSG, Port, sizeof( CSTMSG ) );

		// Set clear key 
		GlobalDsp = CLRKEYON;

		// Check the input 
		switch ( NumEntry( 1, 5, 0, pEntry ) )
		{
			case 1:			// Okay; get length of input 
				length = StrLn( Dspbuf, sizeof( Dspbuf ) );
				memcpy( Port, Dspbuf, length );
				Port[length] = '\0';

				// Check if an entry was input 
				if ( length )
				{
					// Preset status to 1
					Status = 1;

					// Test for legal ASCII values.  Repeat entry if not 0-9.
					for ( i = 0; i < length; i++ )
					{
						// Only allow values 0-9 and .
						if ( !( ( Port[i] >= '0' ) &&
								( Port[i] <= '9' ) ) )
						{
							ShowErrMsg( N_Only09AFvalid );
							Status = 99;
							break;
						}
					}

					// Don't save if illegal char was input.
					if ( 99 == Status )
						break;

					// Convert input from ASCII to UWORD and save 
					tmp = atoi(Port);
					memcpy((UBYTE*)port,&tmp,2);
					//AscHex((UBYTE*)port,Port,2);
					SetTERMTabLRC();
				}

				ButtonId = B_PREVIOUS;
				// Set status to okay 
				Status = 1;

				break;

			case 3:			// Go back to previous entry 
				Status = -1;
				ButtonId = B_PREVIOUS;

				break;

			default:			// Entry cancelled or timed-out 
				Status = 0;

				break;
		}
	}
	while ( 99 == Status );

	// Return the status 
	return ( Status );
}




//END
//-----------------------------------------------------------------------------
//  inet_ntoapp     Covert the input hex IP address into ASCII in the network
//                  format.
//
//  Parameters:
//      pTermFld    Pointer to the IP address to convert
//  Global Inputs:
//
//  Returns:
//      Nothing
//  Notes:
//      NMSBUF contains the converted IP address.
//-----------------------------------------------------------------------------
static void inet_ntoapp( void *pTermFld )
{
	struct in_addr input;

	memcpy( &input, pTermFld, sizeof( UDWORD ) );
	strcpy(( char * ) NMSBUF, inet_ntoa( input ));
}

//-----------------------------------------------------------------------------
//  DoPasswordEnable    Prompt to enable or disable a password
//
//  Parameters:         const ENTRY_STR * screen    Pointer to Yes/No entry
//                                                  screen descriptor
//
//                      UBYTE opt_mask              Mask containing bit to be set
//                                                  or reset
//  Global Inputs:
//
//  Returns:            char        'N' - yes or no selected, go to next step
//                                  'C' - cancel or timeout, abort function
//                                  'B' - Back to previous menu
//  Notes:
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

		case B_PREVIOUS:		// Previous
			return 'B';

		default:				// Timeout or cancel
			return 'C';
	}
}



//-----------------------------------------------------------------------------
//  SetTipPercent   Set up Tip Percentage Screen To display current tip
//                  percentages, then display them and allow user to change
//                  them.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        False - time out or cancel.
//                              True - Successful
//  Notes:
//
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
		keyval = GetOneKey( DEV_PAD, UserTimeout );
		switch ( keyval )
		{
			case '1':			// User selected the First button
			case '2':			// User selected the Second button
			case '3':			// User selected the Third button
				// Get new percentage from user.
				if ( !GetTipPercent( keyval ) )
				{
					// Timeout or CANCEL
// FIXME?
//@ASSA at 04.07.2005 					chRetval = 0;
					return False;
				}
				break;

			case CANCEL_KY:	// Timed out or cancelled
			case 0:
				return False;

			case 'd':			// User Done changing tips
			case B_PREVIOUS:	// Previous button selected

			case ENTER_KY:		// User accepts percentage values
				return True;

			default:
				// bad key, error beep
				SDK_Beeper( TENMS * 40 );
				break;
		}
	}
}



//-----------------------------------------------------------------------------
//  GetTipPercent   Prompt user for Tip Percentage  tip percentages, 
//
//  Parameters:     UBYTE key   Key/Button Selected
//
//  Global Inputs:
//
//  Returns:        Bool        False - Time Out or Cancel.
//                              True - Successfull
//
//  Notes:
//
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
		case '1':				// First percentage
			memcpy( CSTMSG4, CSTMSG, sizeof( CSTMSG ) );
			break;

		case '2':				// Second percentage
			memcpy( CSTMSG4, CSTMSG2, sizeof( CSTMSG ) );
			break;

		case '3':				// Third percentage
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
//  Fun_99_9_8      Maintenance - Clear file system.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
//  Fun_88          Screen Capture toggle.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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


//-----------------------------------------------------------------------------
//  Fun_76          Show captured IP address
//
//  Parameters:     None
//
//  Global Inputs:  Captured_IPaddr
//
//  Returns:        None
//
//  Notes:
//
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
