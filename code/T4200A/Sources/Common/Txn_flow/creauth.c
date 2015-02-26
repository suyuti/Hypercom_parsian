
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             creauth.c
//      Calculate amount+reauth percentage
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

#include "i8583utl.h"

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
//!     Calculate Amount+Reauth Percentage
//!
//!  \param
//!     pResult     pointer to 6-byte BCD Result Amount
//!  \param
//!     pAmount     pointer to 6-byte BCD Initial Amount.
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void CalcReauth( UBYTE * pResult, UBYTE * pAmount )
{
	// See if the Tip Amount is zero
	if ( NullComp( ( char * ) TRREQ.TRTIP, S_TRTIP ) )
	{
		if ( TRREQ.TRISPTR != NULL )
		{
			// Yes; perform calculation
			CReauth( pResult, pAmount, TRREQ.TRISPTR->ISREAUTH );
		}
	}
	else
	{
		// No; Tip exists, no need for calculation
		memcpy( pResult, pAmount, S_TRTOTAM );
	}
}

//=============================================================================
// Private function definitions
//=============================================================================
