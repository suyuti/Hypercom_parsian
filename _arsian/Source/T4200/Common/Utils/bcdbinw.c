
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     bcdbinw.c
//  Functions used for the Conversion from BCD to Binary
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "utllib.h"

#include "bcdbinw.h"

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
//  BcdBinW         Converts  a 2 byte BCD number to a binary word
//
//  Parameters:     pBcd    pointer to 2 byte BCD number
//
//  Global Inputs:
//
//  Returns:        UWORD   Converted binary word value
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern UWORD BcdBinW( UBYTE * pBcd )
{
	return ( ( CvtBin( pBcd[0] ) * 100 ) + ( CvtBin( pBcd[1] ) ) );
}

//=============================================================================
// Private function definitions
//=============================================================================
