//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             demo.c
//      Demo mode host module
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"

#include "utllib.h"
#include "struct.h"
#include "appdata.h"
#include "defines.h"
#include "msg.h"
#include "journal.h"
#include "comm.h"
#include "hstmsg.h"
#include "demo.h"
#include "hstcopy.h"
#include "tables.h"
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


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Entry point for Demo Host processing
//!
//! \return
//!     None
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern void DemoHost( void )
{
	// Hang up now
	Hang_Up ();

	// Copy transaction to Request Buffer
	memcpy( &TRREQ, &TRINP, TRSZE );

	// See if this should be processed offline
	if ( !Offline(  ) )
	{
		// No; see if floor limit approved
		if ( !Floor(  ) )
		{
			// No; increment 'STAN' and move System Trace Number to 
			//'TRREQ'
			IncTraceNum( TRREQ.TRSTAN );

			// Fake sending the Request
			ReqDemo(  );

			// Fill Response as needed
			RspDemo( orvOK );
		}
	}

	// Add transaction to the journal
	JournalNew ( &TRREQ );

	// Mark transaction as ready and move data back to input record
	CopyFromReqReady(  );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Fakes the Request
//!
//! \return
//!     (OS_RETURN_VALS)0 - Not used, only follow i_host rule
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS ReqDemo( void )
{
	// Display "Dialing Now" message
	ShowInfoMsg( DialNow, N_Comms );

	// For 3 seconds (Dial-Up Line)
	SDK_Wait( ONESECOND * 3 );

	// Display "Processing Now" message
	ShowInfoMsg( ProcNow, N_Comms );

	// For 2 seconds (Dial-Up Line)
	SDK_Wait( ONESECOND * 2 );
	return ( ( OS_RETURN_VALS ) 0 );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Fakes the Response
//!
//! \param[in] ret              Not used, only follow i_host rule
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void RspDemo( OS_RETURN_VALS ret )
{
	UBYTE RtcBuf[RTCLEN];

	// Fill the Auth Number with X's
	memset( TRREQ.TRAUTH, 'X', S_TRAUTH );

	// Fill the Response Code with ASCII 0's
	memset( TRREQ.TRRSPC, '0', S_TRRSPC );

	// Get the current Time
	SDK_RtcRead( RtcBuf );

	// Fill the Transaction Date/Time with current RTC Date/Time
	AscHex( TRREQ.TRYEAR, ( char * ) RtcBuf, 6 );

	// Look up Response in TRREQ
	RspLookUp( &TRREQ );
}

