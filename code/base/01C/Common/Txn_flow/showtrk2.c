
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             showtrk2.c
//      Displaying Track 2 data on the screen
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdkgui.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"

#include "showtrk2.h"

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
//!     displaying Track 2 data on the screen
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Use CSTMSGand CSTMSG2 for TK 2 Data.
//!
//-----------------------------------------------------------------------------
extern void ShowTrk2( void )
{
	int i, j, limit, separator_count;

	CHARSIZE charsz;

	// Get Font size
	SDK_GuiGetCharSize( &charsz );

	// Maximum characters for the Dialogue line
	limit = ( ( SCR_PIXEL_WIDTH - ( TEXT_INDENT * 2 ) ) / ( charsz.Width ) );

	// Maximum characters for the line
	j = 0;
	separator_count = 0;
	memset( ( UBYTE * ) CSTMSG, 0,sizeof( CSTMSG ) );
	memset( ( UBYTE * ) CSTMSG2, 0,sizeof( CSTMSG ) );

	// Move first set of bytes to CSTMSG while looking for separator.
	for ( i = 0; i < limit; ++i )
	{
		// Copy TK2 data into global buffers in ASCII.
		CSTMSG[i] = TRINP.TRTRK2[i];

		// Allow 4 characters past '=' for EXP date.
		if ( j )
		{
			j--;
			if ( 0 == j )
			{
				// Set separator_count so 2nd buffer not filled.
				separator_count = 2;

				// If last char was also a separator do not
				// display it.
				if ( '=' == CSTMSG[i] )
					CSTMSG[i] = 0;

				break;
			}
		}

		// Found a separator.
		if ( '=' == CSTMSG[i] )
		{
			// When separator count gets to 2 the discretionary
			// data begins.  Don't display the discretionary data.
			separator_count++;
			if ( 2 == separator_count )
			{
				CSTMSG[i] = 0;
				break;
			}

			j = 4;
		}
	}

	// Copy to second display buffer is first was filled.
	if ( 2 > separator_count )
	{
		// Move rest of data to CSTMSG2 while looking for separator.
		for ( i = 0; i < limit; ++i )
		{
			CSTMSG2[i] = TRINP.TRTRK2[limit + i];

			// Allow 4 characters past '=' for EXP date.
			if ( j )
			{
				j--;
				if ( 0 == j )
				{
					// If last char was also a separator do not
					// display it.
					if ( '=' == CSTMSG2[i] )
						CSTMSG2[i] = 0;

					break;
				}
			}

			// Found a separator.
			if ( '=' == CSTMSG2[i] )
			{
				// When separator count gets to 2 the discretionary
				// data begins.  Don't display the discretionary data.
				separator_count++;
				if ( 2 == separator_count )
				{
					CSTMSG2[i] = 0;
					break;
				}

				j = 4;
			}
		}
	}
}



//=============================================================================
// Private function definitions
//=============================================================================
