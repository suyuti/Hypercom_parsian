//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             doprtlist.c
//      Print list of transactions
//
//=============================================================================
#include "basictyp.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "tables.h"
#include "msg.h"
#include "util.h"
#include "hstprint.h"

#include "doprtlst.h"

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
//! \brief
//!     Prints a list of Transactions
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void DoPrtList( void )
{
	UBYTE status, num_hosts;

	//  Set the transaction key in the TRINP structure
	TRINP.TRKEY = CurrentEvent.TRNKEY;

	CurrentEvent.NEXTEVENT = 0;

	num_hosts = AqidCount(  );	//  count the number of acquirers

	//  we don't have any acquirer to settle, just return
	if ( num_hosts == 0 )
		return;

	//  if we have only one acquirer in acquirer table, don't need to pop up
	//  display the host settle menu. Just set the proper data to TRINP
	if ( num_hosts == 1 )
	{
    	TRINP.TRAQID  = AQTAB[0].AQID;
    	TRINP.TRAQPTR = &AQTAB[0];

		//  Notify user -- "PRINTING, WAIT..."
		ShowInfoMsg( PrintingWait, PleaseWait );

		//  Print Detail Listing
		PrintList ();
	}
	else
	{
		DrawMenu( MENU_HOST_AUDIT_REPORT );

		do						//  Call the selected option processor
		{
			status = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

			if ( status == 'Z' )
			{
				//  set to zero for multi host clear
				TRINP.TRAQID = 0;

				//  Selected okay; notify user -- "PRINTING, WAIT..."
				ShowInfoMsg( PrintingWait, PleaseWait );

				//  Print Detail Listing
				PrintList ();
				DrawMenu( MENU_REDRAW );
			}
			else if ( ( status >= 'A' ) && ( status <= 'A' + 7 ) )
			{
        		TRINP.TRAQID  = AQTAB[status - 'A'].AQID;
        		TRINP.TRAQPTR = &AQTAB[status - 'A'];

				//  Selected okay; notify user -- "PRINTING, WAIT..."
				ShowInfoMsg( PrintingWait, PleaseWait );

				//  Print Detail Listing
				PrintList ();
				DrawMenu( MENU_REDRAW );
			}
			else if ( status == B_PREVIOUS )
			{
				DrawMenu( MENU_PREVIOUS );
			}
			else if ( status == B_NEXT )
			{
				DrawMenu( MENU_NEXT );
			}

		}
		while ( status && ( status != CANCEL_KY ) && ( status != CLEAR_KY ) &&
				( status != B_PREVIOUS_NEW_MENU ) );
	}
}

//=============================================================================
// Private function definitions
//=============================================================================

