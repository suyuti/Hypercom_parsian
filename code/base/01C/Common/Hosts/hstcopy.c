//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             hstcopy.c
//      Copy transaction to/from request buffer
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"

#include "hstcopy.h"

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
//!     Copies the Transaction from the TRREQ Buffer after flagging the
//!     Transaction as Complete
//!
//! \param
//!		[in]Amount			Pointer to Destination Buffer
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void CopyFromReqReady( void )
{
	// Move data back to input record
	memcpy( &TRINP, &TRREQ, TRSZE );

	// Mark the Transaction as ready in TRINP.
	TRRSPREADY |= BIT_1_SET;
}

//=============================================================================
// Private function definitions
//=============================================================================

