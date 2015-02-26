
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             cvtamt.c
//      Convert BCD amount to ASCII
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "comdata.h"

#include "cvtamt.h"

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


//------------------------------------------------------------------
//!  \brief
//!     Convert BCD Amount to ASCII
//!  \param
//!     pAscAmt     pointer to ASCII Amount (13 bytes)
//   \param
//!		pBcdAmt     pointer to BCD Amount (6 bytes).
//!  \return
//!     None
//!-----------------------------------------------------------------
void CvtAmt( char *pAscAmt, UBYTE *pBcdAmt )
{
    static char *ptr;
    static char *ptrX;
	static char *ptrend;
	UBYTE  offset;

	ptr = pAscAmt;
    ptrX = ptr;
	
	/* Blank first position */
	*ptr = ' ';

	/* Convert BCD to ASCII */
	BfAscii( ptr+1, pBcdAmt, S_TRBASE);

	offset = 13-TCONF.TCDPP;

    /* See if decimal point to place */
	if ( TCONF.TCDPP )
	{
		/* Yes; shift the Amount */
		memcpy( ptr, ptr+1, offset );
		/* Place the decimal point */
		*(ptr+offset) = '.';
	}

	/* Now we need to replace leading 0's with spaces for leading 0
	   plus decimal point */
	
	ptrend = ptr + offset - 1;	// offset-1 = 12-TCONF.TCDPP

	for ( ; ptr < ptrend; ptr++ )
	{
		/* See if character is not a space */
		if ( ' ' != *ptr )
		{
			/* Yes; see if a zero */
			if ( '0' == *ptr )
			{
				/* Yes; overwrite with a space */
				*ptr = ' ';
			}
			else
			{
				/* No; exit the loop */
				break;
			}
		}
	}

    // Currency Symbolden sonra araya bir bosluk eklemek gerekir
    --ptr;
    *ptr = ' ';

	/* Set Currency char(s) */
	for ( offset = S_TCLEADC; offset; offset-- )
	{
		if ( TCONF.TCLEADC[offset - 1] > ' ' )
		{
			if ( (--ptr) >= pAscAmt )
			{
				*ptr = TCONF.TCLEADC[offset - 1];
			}
			else
			{
				break;
			}
		}
	}
} 
