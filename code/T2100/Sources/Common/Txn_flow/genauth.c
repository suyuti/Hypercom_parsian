
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
static void GenAuth_Cmn( struct trandata_rec *ptr );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================
extern void GenAuth_TRINP( void );

//-----------------------------------------------------------------------------
//  GenAuth         Generate AUTH Code (2 or 6 digits)
//
//  Parameters:     UBYTE * pExample    example decription
//
//  Global Inputs:
//
//  Returns:        Bool    True  - okay to continue
//                          False - reprompt for a different Amount
//
//  Notes:          2 digit AUTH Code is calculated as follows:
//
//                  first digit = 4
//                  second digit is number between 0 and 7.
//
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


//-----------------------------------------------------------------------------
//  GenAuth_TRINP   Generate AUTH Code (2 or 6 digits) on TRINP structure
//
//  Parameters:     none
//
//  Global Inputs:	TRINP contains actual transaction data
//
//  Returns:        TRINP.TRAUTH contains generated AUTH Code
//                          
//
//  Notes:          2 digit AUTH Code is calculated as follows:
//
//                  first digit = 4
//                  second digit is number between 0 and 7.
//
//-----------------------------------------------------------------------------
extern void GenAuth_TRINP( void )
{
	GenAuth_Cmn(&TRINP);
}


//-----------------------------------------------------------------------------
//  GenAuth_Cmn     Generate AUTH Code (2 or 6 digits) - common part
//  Parameters:     ptr - pointer to actual transaction data buffer
//
//  Global Inputs:	
//
//  Returns:        ptr->TRAUTH contains generated AUTH Code
//                          
//
//  Notes:          2 digit AUTH Code is calculated as follows:
//
//                  first digit = 4
//                  second digit is number between 0 and 7.
//
//-----------------------------------------------------------------------------
static void GenAuth_Cmn( struct trandata_rec *ptr )
{
	UBYTE TempBCD[3];
	UBYTE i;

	// Space fill AUTH Code
	//SpcIt( ( char * ) ptr->TRAUTH, S_TRAUTH );
        memset( ( char * ) ptr->TRAUTH, ' ', S_TRAUTH );

	// Generate 6 digit AUTH ?
	if ( ptr->TRISPTR->ISOPT3 & IS3_AUTH_FL6 )
	{
		// Yes; get 3 random BCD numbers
		for ( i = 0; i < 3; i++ )
		{
			// Get a random number (UBYTE) and convert to BCD
			TempBCD[i] = ( UBYTE ) CvtBcd( GetRandom(  ) );
		}

		// Convert BCD to ASCII and put in buffer
		BfAscii( ( char * ) ptr->TRAUTH, TempBCD, 3 );
	}
	else
	{
		// No; generate 2 digit AUTH
		ptr->TRAUTH[4] = '4';
		ptr->TRAUTH[5] = ( GetRandom(  ) & 0x07 ) | 0x30;
	}
}



//=============================================================================
// Private function definitions
//=============================================================================

