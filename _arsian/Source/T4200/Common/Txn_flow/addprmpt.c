
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     addprmpt.c
//      Functions used to implement Additional Prompt Transaction
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "util.h"
#include "transdef.h"
#include "tables.h"
#include "bit8583.h"

#include "addprmpt.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static char ChkPromptEnable( int index );
static void SaveAPdata( int index );



//=============================================================================
// Public data definitions
//=============================================================================



//=============================================================================
// Private data definitions
//=============================================================================
static const ENTRY_STR APNumEntry = {
	N_NullStr,					// Default title
	CustomMsg,					// Prompt in upper line 
	N_NullStr,					// No default value 
};

static const ENTRY_STR APAlphaNumEntry = {
	N_NullStr,					// Prompt in title area 
	N_NullStr,					// No default value 
	N_NullStr,					// Not used 
};



//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//!  \brief     Perform additional prompt processing
//!
//!  \param
//!     None
//!
//!  \return
//!     char    0 - bad entry
//!             1 - processed successfully
//-----------------------------------------------------------------------------
char AdditionalPrompt( void )
{
	char retval;
	char max, min;
	int idx;
	UBYTE APMap;

	retval = 1;
	APMap = TRINP.TRISPTR->ISAP;

	if ( !APMap )
	{
		return retval;
	}

	for ( idx = 0; idx < APMAXENT; idx++ )
	{
		// If the bit is set, we need to display the prompt & get input
		if ( APMap & 0x01 )
		{
			if ( ChkPromptEnable( idx ) )
			{
				// optimized. 
				memcpy( CSTMSG, ( UBYTE * ) APTAB[idx].APTEXT, S_APTEXT );
				CSTMSG[S_APTEXT] = '\0';

				max = ( char ) CvtBin( APTAB[idx].APMAX );
				min = ( char ) CvtBin( APTAB[idx].APMIN );
				if ( 1 == APTAB[idx].APTYPE )
					retval = NumEntry( 1, max, min, &APNumEntry );
				else if ( ( 0 == APTAB[idx].APTYPE ) ||
						  ( 2 == APTAB[idx].APTYPE ) )
					retval = AlphaNumEntry( 1, max, min, &APAlphaNumEntry );
				else
					retval = 0;	// bad entry type, exit

				// If user pressed CANCEL, bail out 
				if ( !retval )
					break;

				// valid entry, save the data into TRAP 
				SaveAPdata( idx );
			}
		}
		APMap >>= 1;
	}

	return retval;
}










//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief     Check if additional prompt is enabled
//!
//!  \param
//!     None
//!
//!  \return
//!     char        0 - additional prompt is disabled
//!                 1 - additional prompt is enabled
//-----------------------------------------------------------------------------
static char ChkPromptEnable( int index )
{
	char retval;

	retval = 0;

	switch ( CurrentEvent.TRNKEY )
	{
		case AUTH:
		case SALE:
			if ( APTAB[index].APOPT1 & AP1_SALE )
				retval = 1;
			break;
		case SALOFFL:
			if ( APTAB[index].APOPT1 & AP1_SALOFFL )
				retval = 1;
			break;
		case REFUND:
			if ( APTAB[index].APOPT1 & AP1_REFUND )
				retval = 1;
			break;

		case CRDVER:
			if ( APTAB[index].APOPT1 & AP1_CARDVER )
				retval = 1;
			break;

		case DBCASH:
			if ( APTAB[index].APOPT1 & AP1_CASH )
				retval = 1;
			break;

		case CLBATCH:
			if ( APTAB[index].APOPT2 & AP2_SETTLE )
				retval = 1;
			break;
		default:
			break;
	}
	return retval;
}


//-----------------------------------------------------------------------------
//!  \brief     Save the user entered additional data
//!
//!  \param
//!     None
//!
//!  \return
//!     int     Index of the additional prompt.
//!
//-----------------------------------------------------------------------------
static void SaveAPdata( int index )
{
	int len;
	int idx;
	int datamax;

	idx = 1;
	datamax = CvtBin( APTAB[index].APMAX );

	// Maximum length for TRAP is defined as 70 bytes. There is no need 
	// to check 2-byte BCD number(for now). 

	while ( 1 )
	{
		len = CvtBin( TRINP.TRAP[idx] );
		if ( !len )
		{

			// check for length byte, datamax and 2-byte APID
			if ( ( idx + datamax + 4 ) >= 70 )
				break;

			// empty field, add data here
			TRINP.TRAP[idx++] = BinBcdB( ( UBYTE ) ( datamax + 2 ) );
			memset( ( char * ) &( TRINP.TRAP[idx + 2] ), ' ',( UWORD ) datamax );
			len = StrLn( Dspbuf, sizeof( Dspbuf ) );

			memcpy( &( TRINP.TRAP[idx] ), ( UBYTE * ) APTAB[index].APID, 2 );
			memcpy( &( TRINP.TRAP[idx + 2] ), ( UBYTE * ) Dspbuf,
					( UWORD ) len );
			break;
		}
		else
		{
			idx += ( len + 2 );	// skipping data & length byte 
		}
	}

	return;
}
