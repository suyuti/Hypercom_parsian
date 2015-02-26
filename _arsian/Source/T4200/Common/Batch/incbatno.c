//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             incbatno.c
//      Increment batch number function
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "tables.h"

#include "incbatno.h"

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
//! \brief Increments Batch Number.
//! 
//! \param
//!     AcquirerID  acquirer ID to increment the batch number for.
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void IncBatNo( UBYTE AcquirerID )
{
	struct batch_rec *pBatch_Table;

	// Locate the address of the batch table to be updated
	pBatch_Table = FindBatTab( AcquirerID );

	// Move next batch number to current batch number
	memcpy( pBatch_Table->BATCUR, pBatch_Table->BATNXT, 3 );

	// Increment next batch number
	while ( True )
	{
		IncBcdLen( pBatch_Table->BATNXT, 3 );

		// Make sure it is not 999999
		if ( memcmp( ( UBYTE * ) pBatch_Table->BATNXT,
					 ( UBYTE * ) "\x99\x99\x99", 3 ) )
		{
			// Yes; make sure it is not 000000
			if ( memcmp( ( UBYTE * ) pBatch_Table->BATNXT,
						 ( UBYTE * ) "\x00\x00\x00", 3 ) )
			{
				SetBATTabLRC(  );
				break;
			}
		}
	}
}

//=============================================================================
// Private function definitions
//=============================================================================

