//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     batcntl.c
//  Functions used to implement the Batch control  functionality
//
//=============================================================================

// System includes
#include "string.h"
#include "ctype.h"
#include "stdlib.h"

// HSDK includes
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "utllib.h"

// Application includes
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "transdef.h"
#include "msg.h"
#include "util.h"
#include "tables.h"
#include "close.h"
#include "findtran.h"
#include "inputpwd.h"

// This module external declaration
#include "batcntl.h"

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
//! \brief Print batch totals or settle one acquirer
//! 
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void BatchControl( void )
{
	UBYTE status, num_hosts;

	CurrentEvent.NEXTEVENT = 0;

	// count the number of acquirers
	num_hosts = AqidCount(  );

	// we don't have any acquirer to settle, just return
	if ( num_hosts == 0 )
		return;

	// Verify the Input Password 
	if ( InputPswd( MercFunction ) )
	{

		// if we have only one acquirer in acquirer table, don't need to pop up 
		// display the host settle menu. Just set the proper data to TRINP      
		if ( num_hosts == 1 )
		{
    		TRINP.TRAQID  = AQTAB[0].AQID;
    		TRINP.TRAQPTR = &AQTAB[0];
		}
		else
		{
			DrawMenu( MENU_HOST_BATCH_CONTROL );

			do					// Call the selected option processor 
			{
				status = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

				if ( status == B_PREVIOUS )
				{
					DrawMenu( MENU_PREVIOUS );
					status = True;
				}
				// Go forward one menu level 
				else if ( status == B_NEXT )
				{
					DrawMenu( MENU_NEXT );
					status = True;
				}
				else if ( ( CANCEL_KY == status ) ||
						  ( CLEAR_KY == status ) || ( 0 == status ) )
				{
					break;		// cancel or Time-Out 
				}

				// settle all hosts 
				else if ( status == 'Z' )
				{
					// set to zero for multi host settle
					TRINP.TRAQID = 0;
					break;
				}
				else if ( ( status >= 'A' ) && ( status <= 'A' + 7 ) )
				{
        	    	TRINP.TRAQID  = AQTAB[status - 'A'].AQID;
            		TRINP.TRAQPTR = &AQTAB[status - 'A'];
					break;
				}


			}
			while ( status && ( CANCEL_KY != status ) );

			if ( ( CANCEL_KY == status ) || ( CLEAR_KY == status ) ||
				 ( 0 == status ) )
				return;
		}

					TRINP.TRKEY = CLBATCH;
					// Force current event to CLBATCH.   
					SecondEvent.TRNKEY = CLBATCH;
					FindTran ();
					memcpy( &CurrentEvent, &SecondEvent,
							sizeof( struct event_entry ) );

					CloseTerm ();
	}
}


//=============================================================================
// Private function definitions
//=============================================================================

