//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             appdacc2.c
//      Appends first two characters of the Acceptor ID to the send data
//
//=============================================================================
//=============================================================================
#include "basictyp.h"

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
//!     Appends first two characters of the Acceptor ID to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendAqAccID2( void )
{
	UBYTE i;
	char Char;

	// Append the first two characters of the Acceptor ID from the
	// Check Authorizer Acquirer Table to the send data
	for ( i = 0; i < 2; i++ )
	{
		// Get a character from the Acceptor ID
		Char = TRREQ.TRAQPTR->AQACCID[i];

		// Check if a space or a null
		if ( ( ' ' == Char ) || ( 0x00 == Char ) )
		{
			// Yes; quit moving the data
			break;
		}

		// Append the character to the send data
		*pSendBuffer = Char;

		// Increment the send data pointer
		pSendBuffer++;
	}
}


//=============================================================================
// Private function definitions
//=============================================================================

