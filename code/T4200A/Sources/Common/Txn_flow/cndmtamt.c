
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             cndmtamt.c
//      Multiple amounts entered check
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "tables.h"
#include "isrest.h"

#include "cndmtamt.h"

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
//!  \brief
//!     Are multiple Amounts entered ?
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - yes (if Tip, Bar, or Tax are enabled)
//!                 False - no (if only the Total Amount is entered).
//!
//-----------------------------------------------------------------------------
extern Bool CndMultAmt( void )
{
	struct issuer_rec *Issuer;

	// Get Pointer to Issuer Table for this ID
	Issuer = FindIssuer( TRINP.TRISID );

	// Do we prompt for Bar Amount?
	if ( (Issuer->ISOPT5) & IS5_BAR )
	{
		// Yes; return true
		return ( True );
	}

	// Do we prompt for the Tax Amount?
	if ( (Issuer->ISOPT4) & IS4_TAX )
	{
		// Yes; return true
		return ( True );
	}

	// Return True if Restaurant Mode and Tip enabled for this
	// Transaction
	return ( IsRest(  ) );

}
