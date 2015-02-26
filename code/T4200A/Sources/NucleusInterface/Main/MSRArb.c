//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hyperware Base Application
//      (c) Copyright 2006
//
// File Name:
//      MSRArb.c
//
// File Contents:
//  Routines to uses arbitrators 
//
//=============================================================================

#include "basictyp.h"
#include "sdk.h"
#include "utllib.h"
#include "struct.h"
#include "comdata.h"
#include "msg.h"
#include "appdata.h"
#include "DbgTrace.h"

#include <string.h>
#include <malloc.h>
#include "hw.h"


//=============================================================================
// External variables / declarations
//=============================================================================

//=============================================================================
// Public functions defined in this module
//=============================================================================
extern int RegCardTab ( void );

//=============================================================================
// Public from other modules
//=============================================================================

//=============================================================================
// Private defines and typedefs
//=============================================================================

//=============================================================================
// Private function declarations
//=============================================================================
static int RegCardTabEntry( struct card_rec *pCard );
	
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
//!
//!      Register the card table to MSR arbitrator
//!
//! \param
//!
//! \return
//!      int	0 - Fail, otherwise - Succeed
//!
//! \note
//!
int RegCardTab( void )
{
#if defined(USING_AAE) && !defined(APPMGR_NO_AAE)
	int i;

	DBGTRACE( LVL_NORMAL, TYPE_SPOS32, "RegCardTab: called. CRMAX=%d\n", CRMAX );

	for ( i = 0; i < CRMAX; i ++ )
	{
		if ( CRTAB[i].CRNUM )
		{
			RegCardTabEntry( &CRTAB[i] );
		}
	}

	RegCardRangeDone(  );
	DBGTRACE( LVL_NORMAL, TYPE_SPOS32, "RegCardTab: Done.\n" );
#endif // APPMGR_NO_AAE

	return ( 1 );
}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Register the card table to MSR arbitrator
//!
//! \param
//!
//! \return
//!      int	0 - Fail, otherwise - Succeed
//!
//! \note
//!
static int RegCardTabEntry( struct card_rec *pCard )
{
#if defined(USING_AAE) && !defined(APPMGR_NO_AAE)
	unsigned char PANLen;
	char *pPANLow, *pPANHigh;
	int status;
	
	status = 0;
	if ( pCard )
	{
		// Convert BCD to binary
		PANLen = CvtBin( pCard->CRPLEN );

		if ( PANLen )
		{
			// Alocate memory for the card range buffer
			pPANLow = ( char * ) malloc( sizeof( char ) * ( PANLen + 1 ) );
			if ( !pPANLow )
			{
				return 0;
			}

			pPANHigh = ( char * ) malloc( sizeof( char ) * ( PANLen + 1 ) );
			if ( !pPANHigh )
			{
				free( pPANLow );
				return 0;
			}

			// Move card range from the card table entry
			BfAscii( pPANLow, pCard->CRPANL, S_CRPANL );
			BfAscii( pPANHigh, pCard->CRPANH, S_CRPANH );

			// Add '0's at the end of the low range if needed
			if ( PANLen > S_CRPANL * 2 )
			{
				memset( pPANLow + S_CRPANL * 2, '0', PANLen -  S_CRPANL * 2  );
			}
			pPANLow[PANLen] = '\0';

			// Add '9's at the end of the high range if needed
			if ( PANLen > S_CRPANH * 2 )
			{
				memset( pPANHigh + S_CRPANH * 2, '9', PANLen -  S_CRPANH * 2 );
			}
			pPANHigh[PANLen] = '\0';

			DBGTRACE( LVL_NORMAL, TYPE_SPOS32, "RegCardTabEntry: Len=%d, Low=%s, High=%s.\n", PANLen, pPANLow, pPANHigh );

			// Register the card range to AAE
			status = RegCardRange( ( const char * )pPANLow, 
				                   ( const char * )pPANHigh, 
								   ( unsigned int ) PANLen );

			// Free memory 
			free( pPANLow );
			free( pPANHigh );
		}
	}

	return ( status );
#else // APPMGR_NO_AAE
	return ( 1 );
#endif // APPMGR_NO_AAE
}



