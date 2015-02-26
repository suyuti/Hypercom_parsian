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
//! idle screen count
static int IdleCount;
//! list of available idle screens
static UBYTE IdleImageList[MAX_IDLE_SCREENS + 1];


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     Initializes IcePak
//!
//! \return
//!     None
//!
//! \note
//!     Must be called before using other IcePak functions.
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
//! \brief
//!     Draws next available idle screen
//!
//! Draws next available idle screen. If no idle screens exist or IcePak is
//! not enabled this function just returns.
//!
//! \return
//!     None
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
//! \brief
//!     Draws the IcePak saver image if available
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void IcePak_SaverScreen( void )
{
	IcePak_ShowScreen( IMAGE_SAVER );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Removes all Icepak image files
//!
//! \return
//!     None
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
//! \brief
//!     Draws the selected IcePak screen
//!
//! \param
//!		[in]image_index			Image filename suffix
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void IcePak_ShowScreen( UBYTE image_index )
{
	FILE            *handle;
	char            *pFName;
    UWORD           x,y;
    IMAGE_PARMS     Iparms;

	pFName = IcePak[image_index].pFileName;
	if ( ( handle = fopen( pFName, "r" ) ) != NULL )
	{
        SDK_GuiClearScr();	// clear screen buffer
        x = y = 0;
        if( 0 == SDK_GetFileImageParms( handle,&Iparms ) )
        {
            // make an image centred
            if( Iparms.Width <= FW_W )
                x = (FW_W-Iparms.Width)/2;
            if( Iparms.Height <= FW_H )
                y = (FW_H-Iparms.Height)/2;
        }
        // rewind to begin position
        fseek( handle,0L,SEEK_SET );
        SDK_GuiFileImage( x, y, handle );	// put image in screen buffer
		fclose( handle );
		SDK_GuiDispAll();

	}
}

