
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             trinpclr.c
//      Clear Transaction Input Fields.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "trinpclr.h"

#ifdef MAKE_EMV
#include "emv_l2.h"
#include "emvdata.h"
#endif

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
//!     Clear Transaction Input Fields.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void TrinpClear( void )
{
	// Clear Transaction Input Fields 
	memset( ( UBYTE * ) & TRINP, 0, TRSZE );

	// Default to Mag Stripe Read with no PIN capability 
	TRINP.TRPOSE[0] = 0x00;
	TRINP.TRPOSE[1] = 0x22;

	// Clear Transaction Buffer 
	memset( ( char * ) TRINP.TRTRANBUF, ' ', S_TRTRANBUF );

	// Clear Additional Host Print Data Buffer 
	memset( ( UBYTE * ) & HSTPRDATA, 0, S_HSTPRDATA );

#ifdef	MAKE_EMV
	SuppressSigLine = False;
#endif	// MAKE_EMV

	// Clear forced online flag
	ForcedOnline = False;
}


//=============================================================================
// Private function definitions
//=============================================================================
