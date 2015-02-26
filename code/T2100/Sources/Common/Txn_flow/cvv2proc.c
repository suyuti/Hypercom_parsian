//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             cvv2proc.c
//      CVV2 processing routines
//
//=============================================================================

#include "string.h"
#include "stdlib.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "tables.h"

#include "cvv2proc.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static Bool InCVV2( void );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
// Setup screen entry prompt structures

static const ENTRY_STR CVV2Entry =	// Prompt for CVV2 Value
{
	N_NullStr,					// Prompt
	N_CVV2Entry,				// Default
	N_NullStr,					// Default
};


//=============================================================================
// Public function definitions
//=============================================================================


//=============================================================================
// Private function definitions
//=============================================================================
//-----------------------------------------------------------------------------
//  CVV2Proc        Process CVV2 input.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern UBYTE CVV2Proc( void )
{

	// Initialize return value
	UBYTE chRetval = False;

	// Prompt to get CVV2 Presence Indicator
        // @A BC - Türkiyede kullanýlmýyor . 
	// if ( InCVV2Ind(  ) )
	{
		// Prompt to get CVV2 Value
		if ( InCVV2(  ) )
			chRetval = True;
	}
	return chRetval;
}

//-----------------------------------------------------------------------------
//  InCVV2          Get CVV2 Value
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool    True  => if successful;
//                          False => if timed out or user cancelled.
//
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InCVV2( void )
{
	UBYTE length;

	// Set cvv2 value to spaces
	memset( ( char * ) TRINP.TRCVV2, ' ',S_TRCVV2 );

	// elle girilmiþse issuer cvv2 sorulmasýný istiyorsa cvv2 bilgisi alýnýr
	if ( ( TRINP.TRPOSE[1] == 0x12 ) &&
		 ( (TRINP.TRISPTR->ISOPT5) & IS5_CVV2 ) )
	{
		
          if ( !AlphaNumEntry( 1, S_TRCVV2, 1, ( ENTRY_STR * ) & CVV2Entry ) )
          {
              // User cancelled or timed out
	      return False;
          }

          memset( ( char * ) TRINP.TRCVV2, ' ', S_TRCVV2 );
          length = StrLn( Dspbuf, sizeof( Dspbuf ) );

          // Move CVV2 on the format " XXX" into TRINP structure.
          memcpy( &TRINP.TRCVV2[S_TRCVV2 - length], Dspbuf, length );
        }      

	return True;
}
