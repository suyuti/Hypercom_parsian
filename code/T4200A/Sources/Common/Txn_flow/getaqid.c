
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             getaqid.c
//      Find acquirer routines
//
//=============================================================================

#include "stdlib.h"
#include "string.h"

#include "basictyp.h"
#include "sdkos.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "tables.h"

#include "getaqid.h"

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
//!     Get acquirer ID.
//!
//!  \param
//!     SelectAll   allow selection of all acquirers:
//!                     True  - Yes
//!                     False - No.
//!
//!  \return
//!     Bool        True  - Acquirer set
//!                 False - Acquirer not set.
//!
//!  \note
//!     Need to add support for MENU and Settle
//!
//-----------------------------------------------------------------------------
extern Bool GetAQID( Bool SelectAll )
{
	struct acquirer_rec *pAcquirerPtr;	// Pointer to AQTAB entry
	struct acquirer_rec *pHostPtr;	// Pointer to AQTAB entry
	UBYTE Entered;
	char EntHst;
	char InputHst;
	Bool LoopFlag;
	static ENTRY_STR GetAQIDEntry;

	GetAQIDEntry.TitleMid = N_NullStr;	// Title

	// See if all Acquirers are allowed to be selected
	if ( SelectAll )
	{
		// Yes; notify user that all hosts are selectable
		GetAQIDEntry.Line1Mid = HostNum;	// Prompt Line 1
		GetAQIDEntry.Line2Mid = N_ZeroForAllHost;	// Prompt Line 2
	}
	else
	{
		// No; do not notify the user about all hosts
		GetAQIDEntry.Line1Mid = N_NullStr;	// Prompt Line 1
		GetAQIDEntry.Line2Mid = HostNum;	// Prompt Line 2
	}


	LoopFlag = True;

	pHostPtr = FindFirstAcquirer(  );

	if ( NULL != pHostPtr )
	{
		// Have at least 1 Acquirer, See if any more
		pAcquirerPtr = FindNextAcquirer( pHostPtr );

		if ( NULL != pAcquirerPtr )
		{
			while ( LoopFlag )
			{
				// Have multiple Acquirers, so pick one
				// Prompt for Acquirer number
				Entered = NumEntry( 1, 2, 1, &GetAQIDEntry );

				// Check if user cancel
				if ( 0 == Entered )
				{
					// Yes; CANCEL, so Exit
					// Need to enter error code
					return ( False );
				}
				else
				{
					// Need to get entered value from Dspbuf
					// And convert to BCD and memcmpe to ACQ
					// Note : Can Enter 1 or 2 digits
					// InputHst has the binary value and 
					// EntHst has the BCD value entered.
					InputHst = atoi( Dspbuf );
					// Convert binary value to BCD value
					EntHst = InputHst / 10;
					EntHst = EntHst << 4;
					EntHst += InputHst % 10;

					if ( 0 != EntHst )
					{
						// Check all acquirers
						pAcquirerPtr = FindFirstAcquirer(  );

						// pAcquirerPtr is NOT NULL,
						// value checked in FindFirstAcquirer call above
						if ( (pAcquirerPtr->AQID) == EntHst )
						{
							// They are the same
							pHostPtr = pAcquirerPtr;
							LoopFlag = False;
						}
						else
						{
							while ( LoopFlag )
							{
								pAcquirerPtr =
									FindNextAcquirer( pAcquirerPtr );

								if ( NULL == pAcquirerPtr )
								{
									// No more to check, so prompt again
									SDK_Beeper( TENMS * 40 );

									// Exit the inner loop
									break;
								}
								else if ( (pAcquirerPtr->AQID) == EntHst )
								{
									// Found it, now go call
									pHostPtr = pAcquirerPtr;
									LoopFlag = False;
								}
							}	// while ( Loopflag )
						}
					}
					else
					{
						// All Acquirers selected; see if allowed
						if ( SelectAll )
						{
							// Yes; set the Acquirer ID to indicate all
							TRINP.TRAQID = 0;

							// Return okay
							return ( True );
						}
					}			// if ( EntHst != 0 )
				}				// if ( 0 == Entered )
			}					// while ( LoopFlag )
		}						// if ( NULL != pAcquirerPtr )
	}
	else
	{
		// No Acquirer set
		ShowErrMsg( NoAcq );

		// Need to return an error code here
		return ( False );
	}							// if ( NULL != pHostPtr )

	// Now it is time to set TRINP
	TRINP.TRAQID = pHostPtr->AQID;
	TRINP.TRAQPTR = pHostPtr;

	return ( True );
}

//=============================================================================
// Private function definitions
//=============================================================================
