
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     amtentry.c
//  Functions used to implement the Amount Entry functionality
//
//=============================================================================

#include "basictyp.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "comdata.h"
#include "transdef.h"
#include "util.h"
#include "utilfunc.h"

#include "amtentry.h"

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
//!     Accept user input for amount entry
//!
//!  \param
//!     pEntry      Pointer to ENTRY _STR user buffer to store, 
//!                     display line and flag indicating
//!                     whether zero amount is allowed plus a
//!                     flag for amount entry size.
//!  \param
//!       Zero_OK   If ZERO_OK is set to 1, zero amount is
//!                     allowed.Otherwise zero amount is not
//!                     allowed.
//!  \param
//!       IO_Settle_Amt     If IO_Settle_Amt is one then use settle
//!                             sized amount.Otherwise us standard sized
//!                             amount. 
//!
//!  \return
//!     UBYTE       0 - CANCEL key was pressed
//!                 1 - amount was entered successfully
//!                 3 - CLEAR key was pressed
//!
//-----------------------------------------------------------------------------
UBYTE AmountEntry( const ENTRY_STR * pEntry, UBYTE Zero_OK,
						  UBYTE IO_Settle_Amt )
{
	struct ESParms_str ESParms;

	ESParms.DspMode = Zero_OK;
	ESParms.MaxLen = IO_Settle_Amt;
	ESParms.TitleMid = pEntry->TitleMid;
	ESParms.Line1Mid = pEntry->Line1Mid;
	ESParms.Line2Mid = pEntry->Line2Mid;

	XAmountEntry ( &ESParms );

	return ESParms.Alpha;		// Alpha contains return value
}


//=============================================================================
// Private function definitions
//=============================================================================





