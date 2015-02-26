
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             isrest.c
//      Does this transaction require Base+Tip
//
//=============================================================================

#include "string.h"

#include "basictyp.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "findtran.h"

#include "isrest.h"

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
//!  \brief    Does this Transaction require Base+Tip
//!
//!  \param
//!     None
//!  
//!  \return
//!     Bool        True  - Yes (Restaurant Mode on & Transaction
//!                         requires Tip)
//!                 False - No.
//!
//!
//-----------------------------------------------------------------------------
extern Bool IsRest( void )
{
	// Get the transaction from the TRINP structure
	SecondEvent.TRNKEY = TRINP.TRKEY;

	// Look for the transaction
	FindTran ();

	// See if Tips allowed
	if ( !( SecondEvent.TRNOPT1 & T1_NOTIP ) )
	{
		// Yes; see if Restaurant Mode Selected
		if ( TCONF.TCOPT1 & TC1_REST )
		{
			// No; Transaction requires Base+Tip
			return ( True );
		}
	}

	// Transaction does not require Base+Tip
	return ( False );
}

//=============================================================================
// Private function definitions
//=============================================================================
