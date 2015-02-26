
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     aapallow.c
//      Functions used to check if Additional Amount Prompts 
//      are Allowed
//
//=============================================================================

#include "string.h"

#include "basictyp.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "tables.h"

#include "aapallow.h"

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
//  APPAllowed      Additional Amount Prompts allowed ?
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool    True  - yes (non-REFUND Transaction or REFUND
//                                  Transaction with additional Amount
//                                  prompts enabled)
//
//                          False - no (REFUND Transaction with additional
//                                  Amount prompts disabled).
//  Notes:
//
//-----------------------------------------------------------------------------
Bool AAPAllowed( void )
{
	struct issuer_rec *Issuer;

	// Is this a REFUND Transaction ? 
	if ( REFUND == TRINP.TRKEY )
	{
		// Yes; get Pointer to Issuer Table for this ID
		Issuer = FindIssuer( TRINP.TRISID );

		// Additional Amount prompts disabled ? 
		if ( !( (Issuer->ISOPT5) & IS5_REFAMTPR ) )
		{
			// Yes; return false 
			return ( False );
		}
	}

	// Return true 
	return ( True );
}



//=============================================================================
// Private function definitions
//=============================================================================
