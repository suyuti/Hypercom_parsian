
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             chkleng.c
//      Validate the length of a total amount
//
//=============================================================================

#include "string.h"

#include "basictyp.h"

#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"

#include "chkleng.h"

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
//!     Make sure new Total does not exceed Transaction Digits
//!
//!  \param
//!     pAmount     amount
//!
//!  \return
//!     Bool        True  - okay
//!                 False - too large
//!
//-----------------------------------------------------------------------------
extern Bool CheckTheLength( UBYTE * pAmount )
{
	UBYTE Length;
	UBYTE i;
	UBYTE Digits;

	// Get maximum number of digits
	Length = S_TRTOTAM << 1;

	// Loop through the Amount bytes
	for ( i = 0; i < S_TRTOTAM; i++ )
	{
		// See if top digit non-zero
		if ( pAmount[i] & 0xf0 )
		{
			// Yes; exit the loop
			break;
		}

		// Downcount
		Length--;

		// See if low digit non-zero
		if ( pAmount[i] & 0x0f )
		{
			// Yes; exit the loop
			break;
		}

		// Downcount
		Length--;
	}

	// Get total digits
	Digits = CvtBin( TCONF.TCDLEN );

	// Determine if length is okay
	if ( Length <= Digits )
	{
		// Yes; return okay
		return ( True );
	}
	else
	{
		// No; return too large
		return ( False );
	}
}

//=============================================================================
// Private function definitions
//=============================================================================
