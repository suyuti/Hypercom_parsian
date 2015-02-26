//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             visa1.c
//      Entry point for Visa1 host processing.
//
//=============================================================================
#include <string.h>
#include "basictyp.h"
#include "i8583utl.h"
#include "sdk.h"
#include "sdkos.h"
#include "struct.h"
#include "appdata.h"
#include "util.h"
#include "utllib.h"
#include "comm.h"
#include "hstcopy.h"
#include "reqvisa1.h"
#include "rspvisa1.h"


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
//!     Entry point for Visa1 host processing
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void Visa1( void )
{
	OS_RETURN_VALS CommStatus;

	// Copy the input transaction to the request buffer 
	memcpy( &TRREQ, &TRINP, TRSZE );

	// See if this should be processed offline 
	if ( !Offline(  ) )
	{
		// No; see if floor limit approved 
		if ( !Floor(  ) )
		{
			// Calculate total + reauth margin 
			CalcReauth( TRREQ.TRTOTAMI, TRREQ.TRTOTAM );

			// Reset the Message Communication Flags 
			MsgComFlag = 0x00;

			// Send the request to the host  
			CommStatus = ReqVisa1(  );

			// Wait for the response    
			RspVisa1( CommStatus );
		}						// end of !Floor 
	}							// end of !Offline 

	// Perform Referral processing. 
	// FUTURE  

	// Copy transaction back to TRINP and mark it ready 
	CopyFromReqReady(  );

	// Hang Up 
	Hang_Up_Pin();
}


//=============================================================================
// Private function definitions
//=============================================================================

