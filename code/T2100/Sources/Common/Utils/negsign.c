
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             negsign.c
//      Negative Sign Routine 
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"

#include "struct.h"
#include "comdata.h"

#include "negsign.h"

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
//  NegSign         Put a Negative Sign before/over leading 
//                  character (ie. $)
//
//  Parameters:     char *  String      string to be updated
//
//                  UBYTE MaxLength     maximum length of the string.
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void NegSign( char *String, UBYTE MaxLength )
{
	UBYTE i;
	
	/* See if first character is the leading character */
	if ( String[0] == TCONF.TCLEADC[0] )
	{
		/* Yes; replace it with a '-' */
		String[0] = '-';
	}
	else
	{
		/* No; search for the leading character */
		for ( i = 1; i < MaxLength; i++ )
		{
			/* See if leading character found */
			if ( String[i] == TCONF.TCLEADC[0] )
			{
				/* Yes; prepend a '-' */
				String[i - 1] = '-';

				/* Exit the loop */
				break;
			}
		}
	}
}

//=============================================================================
// Private function definitions
//=============================================================================
