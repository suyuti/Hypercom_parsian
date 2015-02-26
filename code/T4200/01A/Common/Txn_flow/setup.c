
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             setup.c
//      Screen/Printer setup functions
//
//=============================================================================

#include "string.h"
#include "stdlib.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkgui.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "sdkmem.h"

#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "util.h"
#include "comdata.h"
#include "fixdata.h"
#include "chkprinter.h"
#include "setup.h"
#include "receipt.h"
#include "hstprint.h"
#include "inputpwd.h"
#include "inpmpswd.h"
#include "tables.h"

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


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!	\brief
//!     Entry point for setting up terminal.
//!
//!	\param
//!     None
//!
//!	\return
//!	    None.
//!
//-----------------------------------------------------------------------------
extern void TerminalSetup( void )
{
	UBYTE Status;

	GetMsg( MaintPswd, DefaultTitle );

	// Get Maintenance Password 
	Status = EnterMPswd(  );

	if ( Status )
	{
		// Okay; see if Maintenance Password disabled 
		if ( 2 == Status )
		{
			// Yes; save and verify the Input Password 
			if ( InputPswd( N_Terminal ) )
			{
				CurrentEvent.NEXTEVENT = MENU_TERM_SETUP;
			}
		}
		else
		{
			CurrentEvent.NEXTEVENT = MENU_TERM_SETUP;
		}
	}

	memset( ( UBYTE * ) DefaultTitle, 0,sizeof( DefaultTitle ) );
}


//-----------------------------------------------------------------------------
//!	\brief
//!     Performs line feeds as required.
//!
//!	\param
//!     None
//!
//!	\return
//! 	None
//!
//-----------------------------------------------------------------------------
extern void PaperFeed( void )
{
	UBYTE key;

	DrawMenu( MENU_PAPERFEED );

	while ( True )
	{
		if ( PAPERFEED_FUN == 
			 ( key = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) ) )
		{
            if ( ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
			{
				PrintLineFeed ();
			}
			else
			{
				// Need to redraw the Screen after the error message 
				DrawMenu( MENU_PAPERFEED );
			}

		}
		else if (   CANCEL_KY == key || CLEAR_KY == key || 0 == key 
				 || B_PREVIOUS_NEW_MENU == key || ENTER_KY == key )
		{
			break;				// exit loop 
		}
	}							// end while(True) 

}


//-----------------------------------------------------------------------------
//!	\brief
//!     Screen to set display contrast, also sets global 
//!		variable TERM.TERMContrast.
//!
//!	\param
//!     None
//!
//!	\return
//!	    None
//!
//-----------------------------------------------------------------------------
extern void SetContrast( void )
{
	UBYTE key;

	DrawScreen( SETCONTRAST_SCR );

	while ( True )
	{

		key = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

		if ( key == CANCEL_KY || key == CLEAR_KY || key == 0 )
		{
			break;				// exit loop 
		}

		if ( key == 'H' )		// higher 
		{
			if ( TERM.TERMContrast < dc_LEVEL7 )
				TERM.TERMContrast++;
		}
		else if ( key == 'L' )	// lower 
		{
			if ( TERM.TERMContrast > dc_LEVEL0 )
				TERM.TERMContrast--;
		}

		SetTERMTabLRC(  );
		SDK_DisplayContrast( TERM.TERMContrast );
	}							// end while(True) 
}


//=============================================================================
// Private function definitions
//=============================================================================
