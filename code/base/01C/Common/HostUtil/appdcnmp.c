//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             appdcnmp.c
//      Appends check number with leading character '#' to send data
//
//=============================================================================
#include "basictyp.h"

#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"


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
//!     Appends check number with leading character '#' to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendCheckNumP( void )
{
	// See if check number exists
	if ( TRREQ.TRTCNO[0] )
	{
		// Yes; append specified leading character to send data
		*pSendBuffer = '#';

		// Increment the send data pointer
		pSendBuffer++;

		// Append the check number to the send data
		pSendBuffer += MoveTillZero(
				( char * ) pSendBuffer,
				( char * ) TRREQ.TRTCNO, 6 );
	}
}


//=============================================================================
// Private function definitions
//=============================================================================

