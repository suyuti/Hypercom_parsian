//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             hstmsg.c
//      Message tables
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "msg.h"

#include "hstmsg.h"

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
//! \brief
//!     Loops up Host Response for a Transaction Data structure
//!
//! \param
//!		[in]ptr					Pointer where to get response
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void RspLookUp( struct trandata_rec *ptr )
{
	// RSPCODE is a global
	memcpy( RSPCODE, ptr->TRRSPC, S_TRRSPC );

	// Get data for this message
	GetRspTxtOpts(  );

	// Save the Translated Text
	memcpy( ptr->TRRSPTXT, ResponseText, S_RSPTXT );

	// Save the Option Bits (Byte 1)
	ptr->TRRSPOPT = RSPOPT;

	// Save the Option Bits (Byte 2)
	ptr->TRRSPOPT2 = RSPOPT2;

	// Substitute fields in the response text

	// Check if "authno" exits
	if ( !memcmp( ( UBYTE * ) & ptr->TRRSPTXT[14], ( UBYTE * ) "authno", 6 ) )
	{
		// Yes; subsitute Authorization Code in message
		{
			// Other hosts
			memcpy( &ptr->TRRSPTXT[14], ptr->TRAUTH, S_TRAUTH );
		}
	}
	else
	{
		// Check if "rr" exits
		if ( !memcmp( ( UBYTE * ) & ptr->TRRSPTXT[18], ( UBYTE * ) "rr", 2 ) )
		{
			// Yes; substitute Response Code in message
			memcpy( &ptr->TRRSPTXT[18], ptr->TRRSPC, S_TRRSPC );
		}
	}
}


//=============================================================================
// Private function definitions
//=============================================================================

