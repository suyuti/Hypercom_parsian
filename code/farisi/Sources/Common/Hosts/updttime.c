//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             updttime.c
//      Update System Time/Date from Host Response.
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"

#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "tables.h"
#include "findtran.h"

#include "updttime.h"
#include "settime.h"

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
//!     Updates System Time/Date from Host Response
//!
//! \return
//!     None.
//!
//! \note
//!		TRREQ.TRDATE as global input
//-----------------------------------------------------------------------------
extern void UpdateTime( void )
{
	UBYTE RtcBuf[RTCLEN];

	// Check if System Time/Date exists in Host Response 
	if ( !NullComp( ( char * ) TRREQ.TRDATE, S_TRDATE + S_TRTIME ) )
	{
		// Yes; see if ignore Time from Host 
		if ( (TRREQ.TRAQPTR->AQOPT1) & AQ1_NOTIME )
		{
			// Yes; ignore the Host Time as it is probably wrong.
			// Use the System Time in the Transaction 
			SetTranTime( TRREQ.TRYEAR );
		}
		else
		{
			// No; get the Transaction from the Request 
			HostEvent.TRNKEY = TRREQ.TRKEY;

			// Locate the Transaction - will be in HostEvent 
			HostFindTran ();

			// See if allowed to use Time from Host 
			if ( !( HostEvent.TRNOPT1 & T1_NOTIME ) )
			{
				// Yes; get Response Code Options 
				if ( TRREQ.TRRSPOPT & R_APPROVED )
				{
					// Approved; update System Time from Host Response Time 

					// Get the current Time to set Year 
					SDK_RtcRead( RtcBuf );

					// Update Date/Time in buffer 
					BfAscii( ( char * ) ( RtcBuf + 2 ), TRREQ.TRDATE,
							 S_TRDATE + S_TRTIME );

					// Update the Time 
					SDK_RtcWrite( RtcBuf, 12 );
				}
			}
		}
	}
}


//=============================================================================
// Private function definitions
//=============================================================================

