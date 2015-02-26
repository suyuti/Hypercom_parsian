
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             dstab.c
//      Descriptor Table functionality
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

#include "dstab.h"

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
//  SetDSTabLRC     Set the Checksum (LRC) for the Descriptor Table
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

extern void SetDSTabLRC( void )
{
	DSLRC = CalcLrc( ( UBYTE * ) & DSTAB, DSSZE * DSMAX );
}

//-----------------------------------------------------------------------------
//  FindDesc        Find the description in descriptor table from key.
//
//  Parameters:     UBYTE DescNum   Descriptor number
//
//  Global Inputs:
//
//  Returns:        struct desc_rec * pointer to descriptor table
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern struct desc_rec *FindDesc( UBYTE DescNum )
{
	UBYTE i;

	// Don't search if requested description is null
	if ( DescNum != 0x00 )
	{
		// Search table for DSNUM that matches requested DescNum
		for ( i = 0; i < DSMAX; i++ )
		{
			// Compare requested DescNum to DSKEY in the table
			if ( DSTAB[i].DSKEY == DescNum )
			{
				return ( ( struct desc_rec * ) &DSTAB[i] );
			}
		}
	}

	// DescNum was not found in the DSTAB table or was null
	return ( ( struct desc_rec * ) 0x0000 );
}


//=============================================================================
// Private function definitions
//=============================================================================
