
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             genauth.c
//      Generate AUTH Code
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "utllib.h"
#include "tlib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "tables.h"

#include "genauth.h"

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
//!     Generate AUTH Code (2 or 6 digits)
//!
//!  \param
//!     pExample    example decription
//!
//!  \return
//!     Bool        True  - okay to continue
//!                 False - reprompt for a different Amount
//!
//!  \note
//!     2 digit AUTH Code is calculated as follows:
//!         first digit = 4
//!         second digit is number between 0 and 7.
//!
//-----------------------------------------------------------------------------
void GenAuth( struct trandata_rec * pTranBuf )
{
    UBYTE TempBCD[3];
	UBYTE i;

	/* Space fill AUTH Code */
	memset( (char *)pTranBuf->TRAUTH, ' ', S_TRAUTH );

	/* Generate 6 digit AUTH ? */
	if ( (pTranBuf->TRISPTR->ISOPT3) & IS3_AUTH_FL6 )
	{
		/* Yes; get 3 random BCD numbers */
		for ( i = 0; i < sizeof(TempBCD); i++ )
		{
			/* Get a random number (UBYTE) and convert to BCD */
			TempBCD[i] = (UBYTE)CvtBcd( GetRandom( ) );	
		}

		/* Convert BCD to ASCII and put in buffer */
		BfAscii( (char *)pTranBuf->TRAUTH, TempBCD, sizeof(TempBCD) );
	}
	else
	{
		/* No; generate 2 digit AUTH */
		pTranBuf->TRAUTH[0] = '4';
		pTranBuf->TRAUTH[1] = ( GetRandom( ) & 0x07 ) | 0x30;
	}
}

//=============================================================================
// Private function definitions
//=============================================================================

