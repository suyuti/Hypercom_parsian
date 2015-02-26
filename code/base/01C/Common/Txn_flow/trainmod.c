
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             trainmod.c
//      Training Mode routines.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "fixdata.h"

#include "utllib.h"
#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "util.h"
#include "journal.h"
#include "perfutil.h"
#include "tables.h"
#include "inputpwd.h"


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
//!  \brief
//!     Training Mode Transaction.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void TrainMode( void )
{
	static ENTRY_STR YNEntry;

	// Save and verify the Input Password 
	if ( InputPswd( N_NullStr ) )
	{
		// Okay; depending on what mode we are in currently (Training
		// or Normal) then prompt if the user wants to switch out of
		// the current mode 

		// Check if currently in Training Mode 
		if ( TCONF.TCOPT4 & TC4_TRAINING )
		{
			// Yes; switch out of Training Mode ? 
			YNEntry.Line1Mid = LeaveTrainingMode;
		}
		else
		{
			// No; switch to Training Mode ? 
			YNEntry.Line1Mid = GoToTrainingMode;
		}

		// Notify user 
		YNEntry.TitleMid = N_NullStr;
		YNEntry.Line2Mid = N_NullStr;

		// Get/check the User input 
		if ( ENTER_KY == YesNoEntry( &YNEntry ) )
		{
			// YES key pressed; toggle the Operation Mode 

			// Check if currently in Training Mode 
			if ( TCONF.TCOPT4 & TC4_TRAINING )
			{
				// Yes; clear all Training Transactions only 
				TrainJournalClear ();

				// Yes; clear the Performance Report Database
                InitRptTable ();

				// Set Training Mode to false 
				TCONF.TCOPT4 &= ~TC4_TRAINING;
			}
			else
			{
				// No; make a new Training Invoice Number 
				TRAININV[0] = 0x99;

				// Move in bottom four digits (2 bytes) 
				TRAININV[1] = TERM.TERMInv[1];
				TRAININV[2] = TERM.TERMInv[2];

				// Set Training Mode to true 
				TCONF.TCOPT4 |= TC4_TRAINING;
			}

			// Set the Table's LRC, in case it was changed 
			SetTCTabLRC(  );
		}
	}
}

//=============================================================================
// Private function definitions
//=============================================================================
