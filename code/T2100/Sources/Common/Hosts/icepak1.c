
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             icepak1.c
//      Functions for IcePak
//
//=============================================================================

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkgui.h"

#include "struct.h"
#include "defines.h"
#include "icepak.h"
#include "comdata.h"
#include "appdata.h"
#include "utllib.h"


//=============================================================================
// External variables / declarations
//=============================================================================

//=============================================================================
// Private defines and typedefs
//=============================================================================
#define MAX_IDLE_SCREENS 4

//=============================================================================
// Private function declarations
//=============================================================================
static void IcePak_ShowScreen( UBYTE image_index );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static int IdleCount;
static UBYTE IdleImageList[MAX_IDLE_SCREENS + 1];	// list of available idle screens


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//  IcePak_Init         Initializes IcePak.
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool        True  - okay to continue
//                                  False - reprompt for a different Amount
//
//  Notes:              Must be called before using other IcePak
//                      functions
//-----------------------------------------------------------------------------

extern void IcePak_Init( void )
{
	FILE *handle;
	int i;
	int j;
	UBYTE tmpb[2];


	IdleCount = 0;
	IcePak_Active = 0;
	IcePak_StartDelay = 0;
	IcePak_IntervalDelay = 0;

	j = 0;
	if ( IPLRC != CalcLrc( ( UBYTE * ) & IPTAB, IPSZE ) )	// if ICEPAK initialization table bad
		return;					// don't activate IcePak

	for ( i = 0; i < MAX_IDLE_SCREENS; i++ )
	{
		if ( ( handle = fopen( IcePak[i].pFileName, "r" ) ) != NULL )
		{
			IdleImageList[j++] = i;
			fclose( handle );
		}
		else
		{
			if ( j == 0 )
				break;			// If first screen not available, ignore the rest
		}

	}

	IdleImageList[j] = 0xff;	// mark end of list

	if ( j > 0 )				// activate IcePak only if some files are available
	{
		IcePak_Active = 1;

		tmpb[0] = 0;
		tmpb[1] = ( int ) IPTAB.IPDELAY1;
		IcePak_StartDelay = ( UBYTE ) BcdBin( tmpb );	// get BCD start delay (seconds)

		tmpb[1] = ( int ) IPTAB.IPDELAY2;
		IcePak_IntervalDelay = ( UBYTE ) BcdBin( tmpb );	// get BCD interval delay (seconds)
		/* Set default start delay  */
		if ( !IcePak_StartDelay ) IcePak_StartDelay = 30;
		/* Set default interval delay */
		if ( !IcePak_IntervalDelay ) IcePak_IntervalDelay = 9;
	}

}


//-----------------------------------------------------------------------------
//  IcePak_NextIdleScreen       Draws next available idle screen. If no idle
//                              screens exist or IcePak is not enabled
//                              this function just returns
//
//  Parameters:                 None
//                  
//
//  Global Inputs:
//
//  Returns:                    None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void IcePak_NextIdleScreen( void )
{
	if ( !IcePak_Active )
		return;

	IcePak_ShowScreen( IdleImageList[IdleCount] );

	IdleCount++;
	if ( IdleImageList[IdleCount] == 0xff )
		IdleCount = 0;			// start over

}


//-----------------------------------------------------------------------------
//  IcePak_SaverScreen      Draws the IcePak saver image if available
//
//  Parameters:             None
//
//  Global Inputs:
//
//  Returns:                None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void IcePak_SaverScreen( void )
{
	IcePak_ShowScreen( IMAGE_SAVER );
}




//-----------------------------------------------------------------------------
//  IcePak_RemoveAl     Remove all Icepak image files.
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void IcePak_RemoveAll( void )
{
	int i;

	for ( i = 0; i < NUM_OF_IP_IMAGES; i++ )
	{
		remove( IcePak[i].pFileName );
	}

	IcePak_Init(  );			// re-initialize with no image files
}



//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//  IcePak_ShowScreen   Draws the selected IcePak screen
//
//  Parameters:         None
//
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:
//
//-----------------------------------------------------------------------------

static void IcePak_ShowScreen( UBYTE image_index )
{
	FILE *handle;
	char *pFName;

	pFName = IcePak[image_index].pFileName;
	if ( ( handle = fopen( pFName, "r" ) ) != NULL )
	{
		SDK_GuiClearScr(  );	// clear screen buffer
		SDK_GuiFileImage( 0, 0, handle );	// put image in screen buffer
		fclose( handle );
		SDK_GuiDispAll(  );

	}
}
