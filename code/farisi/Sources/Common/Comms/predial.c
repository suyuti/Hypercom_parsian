//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             predial.c
//      Perform pre-dialing if allowed
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "utllib.h"
#include "fixdata.h"

#include "struct.h"
#include "comdata.h"
#include "appdata.h"
#include "tables.h"
#include "rs232tab.h"
#include "findtran.h"
#include "comm.h"
#include "validexp.h"

#include "predial.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static void PerformPreDial( void );


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
//! \brief Performs a Pre-Dial if allowed
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void PreDial( void )
{
	static UBYTE FloorAmt[S_TRTOTAM];

	// See if in Training Mode
	if ( TCONF.TCOPT4 & TC4_TRAINING )
	{
		// Yes; no Pre-Dialing in Training Mode
		return;
	}

	// See if valid Acquirer ID exits
	if ( 0 == TRINP.TRAQID )
	{
		// No; exit the function
		return;
	}

	// Check Pre-Dial option
	if ( !(AQ4_PREDIAL & TRINP.TRAQPTR->AQOPT4) )
	{
		// No, exit the function
		return;
	}
	
	// Get the Transaction from the Request 
	SecondEvent.TRNKEY = TRINP.TRKEY;

	// Locate the Transaction - will be in SecondEvent 
	FindTran ();

	// Doing Floor Limit Processing ?
	if ( SecondEvent.TRNOPT2 & T2_FLOOR )
	{
		// Yes; see if card has valid date
		if ( ValidateEXP(  ) )
		{

			// Clear the Floor Amount
			memset( FloorAmt, 0, S_TRTOTAM );

			// Prepare Amount like TRTOTAM
			MoveItIss( &FloorAmt[3], TRINP.TRISPTR->ISFLOOR, S_ISFLOOR );

			// See if Floor Amount is zero
			if ( NullComp( ( char * ) FloorAmt, S_TRTOTAM ) )
			{
				// Yes; so dial
				PerformPreDial(  );
			}
			else
			{
				// No; Compare the Transaction Amount to the Floor 
				// Limit Amount
				if ( memcmp( TRINP.TRTOTAM, FloorAmt, S_TRTOTAM ) > 0 )
				{
					// Larger; so dial
					PerformPreDial(  );
				}
			}
		}
		else
		{
			// No; always dial on expired card
			PerformPreDial(  );
		}
	}
	else
	{
		// No floor limit; so dial
		PerformPreDial(  );
	}
}


//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief Performs a Pre-Dial
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
static void PerformPreDial( void )
{
	// Set the Acquirer ID for Dial
	DIALAQID = TRINP.TRAQID;

	// Set the Transaction type for Dial
	DialTranKey = TRINP.TRKEY;

	if (RSTAB.RSOPT & RSOPT_RSACTIVE) 
    {
        return;
    }

	// Set Up the Dialup Data
	InitiateDial ();

	// Set global indicating predial was started.  The flag must
	// be cleared before host is given control of the transaction.
	// If host is not given control then predial will be stoppeed
	// upon return to idle state.
	PreDialAbort = True;
}

