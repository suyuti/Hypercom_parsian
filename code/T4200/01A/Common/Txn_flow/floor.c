
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             floor.c
//      Floor limit processing
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "tlib.h"
#include "utllib.h"
#include "fixdata.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "util.h"
#include "tables.h"
#include "findtran.h"
#include "hstmsg.h"
#include "validexp.h"
#include "settime.h"
#include "genauth.h"

#include "floor.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================
#define RSPA_FLOOR  "03"		// Additional Resp Data for Floor Limit

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
//!  \brief
//!     See if Transaction can be approved as below Floor Limit
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - approved by Floor Limit
//!                 False - not approved by Floor Limit.
//!
//-----------------------------------------------------------------------------
extern Bool Floor( void )
{
	Bool Approved;
	UBYTE FloorAmt[S_TRTOTAM];	// Temp storage for Amount

#ifdef	MAKE_EMV
	if ( TRINP.TRORIGKEY )		// fallback
		return ( False );
#endif	// MAKE_EMV

	// Check forced online flag
	if ( ForcedOnline )
		return False;

	// Reset Approved flag
	Approved = False;

	// Locate the transaction options
	HostEvent.TRNKEY = TRREQ.TRKEY;
	HostFindTran ();

	// See if this can be approved by Floor Limit Processing
	if ( HostEvent.TRNOPT2 & T2_FLOOR )
	{
		// Yes; see if expired
		if ( ValidateEXP(  ) )
		{
			// No; clear the Floor Amount
			memset( FloorAmt, 0, S_TRTOTAM );

			// Prepare Amount like TRTOTAM
			MoveItIss( &FloorAmt[3], TRREQ.TRISPTR->ISFLOOR, S_ISFLOOR );

			// memcmpe the Transaction Amount to the Floor Limit Amount
			if ( memcmp( TRREQ.TRTOTAM, FloorAmt, S_TRTOTAM ) <= 0 )
			{
				// Smaller; locally Approved

				// Increment 'STAN' and move System Trace Number to 
				//'TRREQ'
				IncTraceNum( TRREQ.TRSTAN );

				// Set the Time for this Transaction
				SetTranTime( TRREQ.TRYEAR );

				// Set Approved
				memcpy( TRREQ.TRRSPC, RSP_APP, S_TRRSPC );

				// Set POS Condition Code
				TRREQ.TRPOCC = 0x06;

				// Are we in Training Mode?
				if ( TCONF.TCOPT4 & TC4_TRAINING )
				{
					// Yes; fill the Auth Number with X's
					memset( TRREQ.TRAUTH, 'X', S_TRAUTH );
				}
				else
				{
					// No; generate AUTH Code in TRREQ
					GenAuth( &TRREQ );
				}

				// Look up Response in TRREQ
				RspLookUp( &TRREQ );

				// Set the Advice bit
				TRREQ.TRSTATUS[0] |= TS1_ADV;

				// Set Additional Response Data
				memcpy( TRREQ.TRRSPA, RSPA_FLOOR, S_TRRSPA );

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

				// Increment Transaction Counter - FUTURE

				// Set Approved flag
				Approved = True;
			}
		}
	}

	// Return Approved flag
	return ( Approved );
}

//=============================================================================
// Private function definitions
//=============================================================================
