
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             offline.c
//      Offline Processing
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdktime.h"
#include "tlib.h"

#include "defines.h"
#include "struct.h"
#include "fixdata.h"
#include "appdata.h"
#include "utllib.h"
#include "msg.h"
#include "tables.h"
#include "findtran.h"
#include "hstmsg.h"
#include "tables.h"
#include "settime.h"
#include "genauth.h"

#include "i8583utl.h"

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
//  Offline         If transaction is OFFLINE, Set TRREQ structure for
//                  an APPROVAL.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool    True = OFFLINE TRANSACTION, TRREQ fields updated 
//                          False = Not OFFLINE, No updates done.
//                          
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool Offline( void )
{
	int i;

#ifdef	MAKE_EMV
	if ( TRINP.TRORIGKEY )		// fallback
		return ( False );
#endif	// MAKE_EMV

	// Check forced online flag
	if ( ForcedOnline )
            return False;

	// Locate the transaction options
	HostEvent.TRNKEY = TRREQ.TRKEY;
	HostFindTran ();

	// See if a REFUND Transaction
	if ( REFUND == TRREQ.TRKEY )
	{
		// Yes; are we configured to do Refunds Offline?
		if ( !( TCONF.TCOPT2 & TC2_REF_OFF ) )
		{
			// No; are Refunds processed Offline?
			if ( !((TRREQ.TRISPTR->ISOPT3) & IS3_REF_OFF) )
			{
				// No; check if this is an OFFLINE transaction
				if ( !( HostEvent.TRNOPT2 & T2_OFFLINE ) )
				{
					// No; return false
					return ( False );
				}
			}
		}
	}
	else
	{
		// No; check if this is an OFFLINE transaction
		if ( !( HostEvent.TRNOPT2 & T2_OFFLINE ) )
		{
			// No; return false
			return ( False );
		}
	}

	// This is OFFLINE
	// Increment 'STAN' and move System Trace Number to 'TRREQ'
	IncTraceNum( TRREQ.TRSTAN );

	// Now set the time
	SetTranTime( TRREQ.TRYEAR );

	// Set  to APPROVED
	memcpy( TRREQ.TRRSPC, RSP_APP, S_TRRSPC );

	// Look up response in TRREQ
	RspLookUp( &TRREQ );

	// Generate AUTH Code for REFUND
	if ( REFUND == TRREQ.TRKEY )
    {
		GenAuth( &TRREQ );
    }

	//  Replace NULLS with Spaces in Auth Code
	for ( i = 0; i < S_TRAUTH; i++ )
	{
		if ( 0 == TRREQ.TRAUTH[i] )
			TRREQ.TRAUTH[i] = 0x20;
	}

	// Set Additional Response Data to Offline
	TRREQ.TRRSPA[0] = '0';
	TRREQ.TRRSPA[1] = '6';

	// Set Advice Bit
	TRREQ.TRSTATUS[0] |= TS1_ADV;

    // Imitate dial if required
    if ( AQ3_IMITDIAL & (TRREQ.TRAQPTR->AQOPT3) )
    {
        // See if dial-up
        if ( cl_DIALUP == TERM.TERMLine )
        {
            // Yes; display "Dialing Now" message
            ShowInfoMsg( DialNow, N_Comms );

            // For 3 seconds (Dial-Up Line)
            SDK_Wait( ONESECOND * 3 );
        }

        // Display "Processing Now" message
        ShowInfoMsg( ProcNow, N_Comms );

        // For 3 seconds
        SDK_Wait( ONESECOND * 3 );
    }

	// Increment TRAN Counter
	// FUTURE

	return ( True );
}

//=============================================================================
// Private function definitions
//=============================================================================
