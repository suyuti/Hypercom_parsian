
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             idlebutt.c
//      Function module
//
//=============================================================================

#include "string.h"
#include "stdlib.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkgui.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "util.h"
#include "menu.h"
#include "sdl.h"
#include "inputpwd.h"
#include "tabutil.h"
#include "inpmpswd.h"
#include "tables.h"

#include "idlebutt.h"


//=============================================================================
// External variables / declarations
//=============================================================================                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          


//=============================================================================
// Private function declarations
//=============================================================================
static Bool IdleButtonFunc( void );
static void InitIdleButton( void );
static void ChangeIdleButton( void );
static Bool GetButtonId( void );
static Bool SelectEventType( void );
static Bool SelectNewtrans( void );
static Bool SelectNewFunc( void );
static char DoConfirmEvent( const ENTRY_STR * screen );
static void UpdateIBTAB( void );
static void ConvertImgToAsc( IMAGE_ID valImgID );


//=============================================================================
// Private defines and typedefs
//=============================================================================

#define Max_Leng 10				// Lenght of Temp Buffer

#define Encode(out, input) \
	out[0] = (UBYTE)( (input >> 8) & 0xff); \
	out[1] = (UBYTE)( input & 0xff );

//=============================================================================
// Private data definitions
//=============================================================================

static UBYTE ButtonSelect;		// Button Selected 

static UBYTE TempEventLbl[S_BTNLBLLEN];	// Temporal Event Lbl- for Idle Button trans/function selected 
static UBYTE TempEventID[S_BTNTRNLEN];	// Temporal Event ID - for Idle Button Trans/function selected 

static UBYTE cancel;

static UBYTE TempImgBuff[Max_Leng];
static UBYTE tempbuf[2];
static WORD TempVal;			// Temporal Value 

ENTRY_STR YNEntry;


static const ENTRY_STR SaleEntry =	// Prompt for Sale Transaction 
{
	N_Sale,						// Title 
	ConfirmTrans,				// prompt
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR RefundEntry =	// Prompt for Refund Transaction 
{
	RefundTran,					// Title 
	ConfirmTrans,				// prompt
	N_NullStr,					// Prompt line 2
};

static const ENTRY_STR VoidEntry =	// Prompt for Void Transaction
{
	N_Void,						// Title 
	ConfirmTrans,				// prompt
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR DebitEntry =	// Prompt for Debit Transaction 
{
	N_Debit,					// Title 
	ConfirmTrans,				// prompt 
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR SettleEntry =	// Prompt for Settlement Transaction 
{
	SettleTran,					// Title 
	ConfirmTrans,				// prompt
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR CardVerEntry =	// Prompt for CardVerEntry Transaction
{
	N_CardVer,					// Title 
	ConfirmTrans,				// prompt
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR SALOFFLEntry =	// Prompt for SALOFFLEntry Transaction 
{
	N_Force,					// Title 
	ConfirmTrans,				// prompt 
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR TABEntry =	// Prompt for TABEntry Transaction 
{
	N_Tab,						// Title 
	ConfirmTrans,				// prompt
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR TRAINMODEEntry =	// Prompt for TRAINMODEEntry Transaction
{
	N_TrainMode,				// Title 
	ConfirmTrans,				// prompt 
	N_NullStr,					// Prompt line 2 
};


//--------------------- Event Funtions----------------------------------
static const ENTRY_STR ReviewEntry =	// Prompt for Review Funtion 
{
	N_Review,					// Title 
	ConfirmFunc,				// prompt 
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR FindEntry =	// Prompt for Find Function 
{
	N_Find,						// Title 
	ConfirmFunc,				// prompt 
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR MfunctionEntry =	// Prompt for Merchant Function 
{
	N_Function,					// Title 
	ConfirmFunc,				// prompt 
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR ReprintEntry =	// Prompt for Reprint Function 
{
	N_Reprint,					// Title 
	ConfirmFunc,				// prompt
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR ReportEntry =	// Prompt for Report Function 
{
	N_Report,					// Title 
	ConfirmFunc,				//  prompt 
	N_NullStr,					// Prompt line 2
};

static const ENTRY_STR ShiftEntry =	// Prompt for Shift Function 
{
	N_Shift,					// Title 
	ConfirmFunc,				// prompt 
	N_NullStr,					// Prompt line 2 
};

static const ENTRY_STR LanguageEntry =	// Prompt for Language Function 
{
	Msg_Lang,					// Title 
	ConfirmFunc,				// prompt 
	N_NullStr,					// Prompt line 2
};

static const ENTRY_STR ChangeAppEntry =	// Prompt for Change Application function 
{
	N_ChangeApp,				// Title 
	ConfirmFunc,				// prompt 
	N_NullStr,					// Prompt line 2
};

static const UWORD RIGHTSIDE = 0x00FF;
static const UBYTE LEFTSIDE = 8;

//-----------------------------------------------------------------------------
//  SetIdleButton               Set Idle Button Screen
//                               
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

extern void SetIdleButton( void )
{
	UBYTE MPStatus;

	GetMsg( N_IdleButton, DefaultTitle );

	// Get Maintenance Password 
	MPStatus = EnterMPswd(  );
	if ( MPStatus )
	{
		// Okay; see if Maintenance Password disabled 
		if ( 2 == MPStatus )
		{
			// Yes; save and verify the Input Password 
			if ( !InputPswd( N_IdleButton ) )
			{
				// Bad; return to caller 
				return;
			}
		}

		IdleButtonFunc(  );
	}

	return;
}

//-----------------------------------------------------------------------------
//  IdleButtonFunc      Select Idle Button Function (2 functions)
//                                _ Init Idle Button Table. 
//                                _ Change Idle Button Screen 
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool IdleButtonFunc( void )
{
	static UBYTE keyfunc;
	UBYTE chRetval;

	Bool loop;
	chRetval = 1;				// To validate GetbuttonId function 

	loop = True;

	while ( loop )
	{

		// Now draw the Idle Button Function Screen 
		DrawMenu( MENU_IDLE_INITCHG );


		// Wait for a Keypress or timeout 
		keyfunc = GetOneKey( DEV_PAD, UserTimeout );
		switch ( keyfunc )
		{
			case 'A':			// Selectd -Init. Idle Button- Fun 
				InitIdleButton(  );
				loop = True;
				break;

			case 'B':			// Selected -Change Idle Button -Func 
				ChangeIdleButton(  );
				loop = True;
				break;


			case CANCEL_KY:	// Timed out, cancelled or clear
			case 0:
				loop = False;
				SDK_Beeper( 1 );
				chRetval = 0;
				break;


			default:
				// bad key, error beep 
				SDK_Beeper( 1 );
				loop = True;
				break;
		}
	}
	return ( chRetval );

}

//-----------------------------------------------------------------------------
//  InitIdleButton    Enab/Dis Idle Button Table Init.
//                               
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

static void InitIdleButton( void )
{
	UBYTE keyval;

	// Test for Init Idle Button Table enabled 
	if ( True == LOCTAB.IBTAB_INIT )
	{
		// Prompt Init. IBTAB Enabled 
		YNEntry.TitleMid = N_InitButTab;
		YNEntry.Line1Mid = Init_Ena;
		YNEntry.Line2Mid = DisablePP;

		keyval = YesNoEntry( &YNEntry );

		if ( ENTER_KY == keyval )	// If YES       
		{
			// Disable IBTAB Init 
			LOCTAB.IBTAB_INIT = False;
			SetLOCTabLRC(  );
		}
	}
	else
	{
		// Prompt Init. IBTAB Disabled 
		YNEntry.TitleMid = N_InitButTab;
		YNEntry.Line1Mid = Init_Dis;
		YNEntry.Line2Mid = EnablePP;

		keyval = YesNoEntry( &YNEntry );

		if ( ENTER_KY == keyval )	// If YES       
		{
			// Enable Init 
			LOCTAB.IBTAB_INIT = True;
			SetLOCTabLRC(  );
		}
	}
}

//-----------------------------------------------------------------------------
//  ChangeIdleButton    Start the process to Select button and Event to display
//                               
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

static void ChangeIdleButton( void )
{

	// Step 1- To select Idle Button screen to change, max. 4 button 
	if ( !GetButtonId(  ) )
		return;

	// Step 2- To select the new Event Type to copy into the Idle Button- Event [Transaction] or [Function] -  
	if ( !SelectEventType(  ) )
		return;


	// Step 3- to Update IBTAB -Idle Button Screen Tab- 
	UpdateIBTAB(  );
	return;
}

//-----------------------------------------------------------------------------
//  GetButtonId    Get the ID of the button selected
//                               
//                               
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        1.-True - o False
//
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool GetButtonId( void )
{
	UBYTE Status;
	UBYTE currbutton;			// Current Button 
	UBYTE chRetval;



	chRetval = 1;				// To validate GetbuttonId function 
	ButtonSelect = 0;			// Initial ButtonID. Button to be change in the idle 
	currbutton = 0;				// Initialize current Button ID 
	Status = 99;

	GetMsg( SelectButton, DefaultTitle );


	while ( 99 == Status )
	{


		// Draw the Screen 
		DrawMenu( MENU_SELECT_BTN );


		switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )	// ONTE, PULSE, CANCEL_KY, or ENTER ? 
		{
			case '1':			// Button1
				currbutton = 1;
				Status = 1;
				chRetval = 1;


				break;

			case '2':			// Button2 
				currbutton = 2;
				Status = 1;
				chRetval = 1;

				break;

			case '3':			// Button3 
				currbutton = 3;
				Status = 1;
				chRetval = 1;

				break;

			case '4':			// Button4 
				currbutton = 4;
				Status = 1;
				chRetval = 1;

				break;

			case 0:			// Timed out 
				SDK_Beeper( 1 );
			case CANCEL_KY:
				currbutton = 0;
				chRetval = 0;	// Abort Process 
				Status = 1;
				break;

			default:
				SDK_Beeper( 1 );
				Status = 99;

				break;
		}
		ButtonSelect = currbutton;

	}

	ClearMenus ();

	// Return the chRetval 
	return ( chRetval );

}

//-----------------------------------------------------------------------------
//  SelectEventType    Select new Transaction or function to copy into the idle Button Scr
//                               
//                               
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        1.-True - o False
//
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool SelectEventType( void )
{
	static Bool loop;
	UBYTE chRetval = 1;				// To validate GetbuttonId function

	// display button selected 

	switch ( ButtonSelect )
	{
		case 1:
			GetMsg( Chang_Btn1, DefaultTitle );	// set title for screen "Chg Event Button 1"
			break;

		case 2:
			GetMsg( Chang_Btn2, DefaultTitle );	// set title for screen "Chg Event Button 2" 
			break;

		case 3:
			GetMsg( Chang_Btn3, DefaultTitle );	// set title for screen "Chg Event Button 3" 
			break;

		case 4:
			GetMsg( Chang_Btn4, DefaultTitle );	// set title for screen "Chg Event Button 4" 
			break;
	}


	DrawMenu( MENU_EVENT_TYPE );


	loop = True;

	while ( loop )
	{

		// Get Keypad button entry 
		switch ( GetOneKey( DEV_PAD, UserTimeout ) )
		{
			case 'T':			// SelectNewtrans for Idle Button options
				if ( SelectNewtrans(  ) )
					loop = False;

				else
				{
					if ( cancel == 1 )
					{
						chRetval = 0;
						loop = False;
					}
					else
						// Draw the Local Options Menu 
						DrawMenu( MENU_PREVIOUS );
				}

				break;

			case 'F':			// Select New Function for Idle Button options 
				if ( SelectNewFunc(  ) )
					loop = False;

				else
				{
					if ( cancel == 1 )
					{
						chRetval = 0;
						loop = False;
					}
					else
						// Draw the Local Options Menu 
						DrawMenu( MENU_PREVIOUS );
				}
				break;


			case CANCEL_KY:	// Timed out or cancelled 
			case 0:
				SDK_Beeper( 1 );
				loop = False;
				chRetval = 0;
				break;

		}

	}
	return ( chRetval );
}


//-----------------------------------------------------------------------------
//  SelectNewtrans    Select new Transaction to copy into the idle Button Scr
//                               
//                               
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        1.-True - o False
//
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool SelectNewtrans( void )
{
	static char retval;

	retval = 0;

	// Display the "Transaction" menu 
	DrawMenu( MENU_IDLE_NEWTRANS );

	do
	{


		// Get Keyboard Entry 
		switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
		{
			case 'A':			// SALE 
				retval = DoConfirmEvent( &SaleEntry );

				if ( retval == 'Y' )
				{

					// New Event ID for idle button
					TempEventID[0] = SALE >> LEFTSIDE;
					TempEventID[1] = SALE & RIGHTSIDE;

					// Icon associated to this Event Selected
					ConvertImgToAsc( IMAGE_SALE );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'B':			// Refund 
				retval = DoConfirmEvent( &RefundEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button
					TempEventID[0] = REFUND >> LEFTSIDE;
					TempEventID[1] = REFUND & RIGHTSIDE;

					// Icon associated to this Event Selected 
					ConvertImgToAsc( IMAGE_REFUND );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'C':			// VOID 
				retval = DoConfirmEvent( &VoidEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = VOIDTRAN >> LEFTSIDE;
					TempEventID[1] = VOIDTRAN & RIGHTSIDE;

					// Icon associated to this Event selected 
					ConvertImgToAsc( IMAGE_VOID );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'D':			// Debit 
				retval = DoConfirmEvent( &DebitEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = DEBIT >> LEFTSIDE;
					TempEventID[1] = DEBIT & RIGHTSIDE;

					// Icon associated to the Event selected 
					ConvertImgToAsc( IMAGE_ATM );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'E':			// Settlement 
				retval = DoConfirmEvent( &SettleEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = SETTLE >> LEFTSIDE;
					TempEventID[1] = SETTLE & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_BANK );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'F':			// Card Ver 
				retval = DoConfirmEvent( &CardVerEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = CRDVER >> LEFTSIDE;
					TempEventID[1] = CRDVER & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_CARDS );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'G':			// SALOFFLINE 
				retval = DoConfirmEvent( &SALOFFLEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = SALOFFL >> LEFTSIDE;
					TempEventID[1] = SALOFFL & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_OFFLINE );
					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;


			case 'H':			// TAB 
				retval = DoConfirmEvent( &TABEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = TABPROC_FUN >> LEFTSIDE;
					TempEventID[1] = TABPROC_FUN & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_TAB );
					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'J':			// TRAINMODE 
				retval = DoConfirmEvent( &TRAINMODEEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = TRAINMODE >> LEFTSIDE;
					TempEventID[1] = TRAINMODE & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_TRAIN );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case B_PREVIOUS:	// Previous 
				DrawMenu( MENU_PREVIOUS );
				break;

			case B_NEXT:		// Next 
				DrawMenu( MENU_NEXT );
				break;

			case B_PREVIOUS_NEW_MENU:	// Previous - new menu 
				return False;

			case CANCEL_KY:	// Timed out or cancelled 
				cancel = 1;
				return False;

			case 0:
				retval = 'C';

			default:
				SDK_Beeper( 1 );
				break;


		}
		if ( retval == 'N' )
			DrawMenu( MENU_REDRAW );	// redraw menu 


	}
	while ( retval != 'C' );

	return False;
}

//-----------------------------------------------------------------------------
//  SelectNewFun    Select new function to copy into the idle Button Scr
//                               
//                               
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        1. Selection succesfull 0.Selection Canceled .
//
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool SelectNewFunc( void )
{

	static char retval;

	retval = 0;

	// Display the Function selection menu 
	DrawMenu( MENU_IDLE_NEWFUNC );

	do
	{


		// Get Keyboard Entry 
		switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
		{
			case 'A':			// Review Function allowed? 
				retval = DoConfirmEvent( &ReviewEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = REVIEW >> LEFTSIDE;
					TempEventID[1] = REVIEW & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_REVIEW );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'B':			// Find Function  
				retval = DoConfirmEvent( &FindEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = FIND >> LEFTSIDE;
					TempEventID[1] = FIND & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_FIND );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'C':			// Merchant Function   
				retval = DoConfirmEvent( &MfunctionEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = FUNCTION >> LEFTSIDE;
					TempEventID[1] = FUNCTION & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_FUNCTION );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'D':			// Reprint Function  

				retval = DoConfirmEvent( &ReprintEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = REPRINT >> LEFTSIDE;
					TempEventID[1] = REPRINT & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_PRINT );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'E':			// Report Function 
				retval = DoConfirmEvent( &ReportEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = REPORTS >> LEFTSIDE;
					TempEventID[1] = REPORTS & RIGHTSIDE;

					// Icon associated to the Event Selected
					ConvertImgToAsc( IMAGE_REPORT );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;


			case 'F':			// Shift Function 
				retval = DoConfirmEvent( &ShiftEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = CHANGESHIFT_FUN >> LEFTSIDE;
					TempEventID[1] = CHANGESHIFT_FUN & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_SHIFT );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'G':			// Set Language Function 
				retval = DoConfirmEvent( &LanguageEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = SET_LANGUAGE >> LEFTSIDE;
					TempEventID[1] = SET_LANGUAGE & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_LANGUAGE );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;

			case 'H':			// Change Application 
				retval = DoConfirmEvent( &ChangeAppEntry );

				if ( retval == 'Y' )
				{
					// New Event ID for idle button 
					TempEventID[0] = CHG_APP >> LEFTSIDE;
					TempEventID[1] = CHG_APP & RIGHTSIDE;

					// Icon associated to the Event Selected 
					ConvertImgToAsc( IMAGE_EMPTY );

					return True;
				}
				else if ( retval == 'C' )

					return False;


				break;


			case B_PREVIOUS:	// Previous opts menu 
				DrawMenu( MENU_PREVIOUS );	// put previou menu back up 
				break;

			case B_NEXT:		// Next opts menu 
				DrawMenu( MENU_NEXT );
				break;

			case B_PREVIOUS_NEW_MENU:	// need to go to new menu 
				return False;

			case CANCEL_KY:	// Timed out or cancelled 
				cancel = 1;
				return False;

			case 0:

				SDK_Beeper( 1 );
				retval = 'C';
				break;

			default:
				SDK_Beeper( 1 );
				break;

		}

		if ( retval == 'N' )
			DrawMenu( MENU_REDRAW );	// stay in Select Functions 


	}
	while ( retval != 'C' );

	return False;
}

//-----------------------------------------------------------------------------
//  DoConfirmEvent    Prompt to confirm an event selection
//                               
//                               
//  Parameters:     Screen Entry
//
//  Global Inputs:
//
//  Returns:        'Y' - yes selected, go to next step
//                  'C' - cancel or timeout, abort function
//                  'N' - No selected go to next step
//  Notes:
//
//-----------------------------------------------------------------------------

static char DoConfirmEvent( const ENTRY_STR * screen )
{

	cancel = 0;					// 0 represent option 'N'- 1 represent option Cancel 'x' 

	switch ( YesNoEntry( screen ) )
	{

		case CLEAR_KY:
			// Do not Confirm 
			return 'N';

		case ENTER_KY:
			// Confirmed then search the Event 
			TextGetString( ( char * ) TempEventLbl, hdttext_table_id,
						   ( text_msg_id ) screen->TitleMid );

			return 'Y';

		case 0:
			SDK_Beeper( 1 );	// Timeout 

		case CANCEL_KY:		// Cancel 
			cancel = 1;

	}

	return 'C';

}

//-----------------------------------------------------------------------------
//  UpdateIBTab      Update IBTAB with Button and Event ID previusly selected
//                               
//                               
//  Parameters:     
//
//  Global Inputs:
//
//  Returns:        
//                  
//                  
//  Notes:
//
//-----------------------------------------------------------------------------

static void UpdateIBTAB( void )
{

	// Load current Button selected 
	switch ( ButtonSelect )
	{
		case 1:				// For Idle Button 1 - Left up 

			// Updating Button Label and Event ID  
			memcpy( IBTAB.IBBTN1LABEL1, TempImgBuff, sizeof( TempImgBuff ) );
			memcpy( IBTAB.IBBTN1LABEL2, TempEventLbl,
					sizeof( TempEventLbl ) );
			memcpy( IBTAB.IBTRANBTN1, TempEventID, sizeof( TempEventID ) );

			break;

		case 2:				// For Idle Button 2 - Rigth up 

			// Updating Button Label and Event ID  
			memcpy( IBTAB.IBBTN2LABEL1, TempImgBuff, sizeof( TempImgBuff ) );
			memcpy( IBTAB.IBBTN2LABEL2, TempEventLbl,
					sizeof( TempEventLbl ) );
			memcpy( IBTAB.IBTRANBTN2, TempEventID, sizeof( TempEventID ) );

			break;

		case 3:				// For Idle Button 3 - Left down 

			// Updating Button Label and Event ID  
			memcpy( IBTAB.IBBTN3LABEL1, TempImgBuff, sizeof( TempImgBuff ) );
			memcpy( IBTAB.IBBTN3LABEL2, TempEventLbl,
					sizeof( TempEventLbl ) );
			memcpy( IBTAB.IBTRANBTN3, TempEventID, sizeof( TempEventID ) );

			break;

		case 4:				// For Idle Button 4 - Right down 

			// Updating Button Label and Event ID  
			memcpy( IBTAB.IBBTN4LABEL1, TempImgBuff, sizeof( TempImgBuff ) );
			memcpy( IBTAB.IBBTN4LABEL2, TempEventLbl,
					sizeof( TempEventLbl ) );
			memcpy( IBTAB.IBTRANBTN4, TempEventID, sizeof( TempEventID ) );

			break;

	}

	// Set Idle Button screen table LRC 
	SetIBTabLRC(  );


	return;
}

//-----------------------------------------------------------------------------
// ConvertImgToAsc      Converting Img ID to ASCII
//
//  Parameters:         Image ID
//
//  Global Inputs:
//  Returns:        Nothing
//
//  Notes:
//
//-----------------------------------------------------------------------------

static void ConvertImgToAsc( IMAGE_ID valImgID )
{
	// Convert BCD to ASCII 
	TempVal = CvtBcd( ( UWORD ) valImgID );

	// Establishing the right order 
	Encode( tempbuf, TempVal )
		// Convertion hex to Asc 
		BfAscii( ( char * ) &TempImgBuff, ( const UBYTE * ) &tempbuf,
				 sizeof( tempbuf ) );
}
