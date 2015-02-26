//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             appdmta.c
//      Appends Transit and Account Numbers with a prepend "T" & "A" 
//      to the send data
//
//=============================================================================
#include "basictyp.h"

#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "hostutil.h"

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
//!     Appends Transit and Account Numbers with a prepended "T" and "A" to
//!     send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendMICRTA( void )
{
	// Append Routing Transit Indicator to the send data
	AppendChar( 'T' );

	// Append the Transit Number to the send data
	pSendBuffer += MoveTillZero(
			( char * ) pSendBuffer,
			( char * ) TRREQ.micr.f2.TRTRANSIT, 9 );

	// Append Account Number Indicator to the send data
	AppendChar( 'A' );

	// Append the Account Number to the send data
	pSendBuffer += MoveTillZero(
			( char * ) pSendBuffer,
			( char * ) TRREQ.micr.f2.TRChkAcct, 18 );
}

//=============================================================================
// Private function definitions
//=============================================================================

