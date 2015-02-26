
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     batpend.c
//  Functions used to Set and clear batch pending
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "cashier.h"
#include "host.h"
#include "findtran.h"
#include "batpend.h"
#include "tables.h"

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
//  SetBatchPending     Set the batch pending flag if required
//
//  Parameters:
//      None
//  Global Inputs:
//
//  Returns:
//      Nothing
//  Notes:
//
//-----------------------------------------------------------------------------

extern void SetBatchPending( void )
{
	struct batch_rec *pBatch_Table;	// Pointer to BATTAB entry 

	SecondEvent.TRNKEY = TRREQ.TRKEY;

	// Look for the transaction 
	FindTran ();

	// See if need to set batch pending for this transaction 
	if ( SecondEvent.TRNOPT3 & T3_BATPEND )
	{
		// Must set batch pending now.  
		pBatch_Table = FindBatTab( CurrentAqid );
		pBatch_Table->BATFLAG |= BT_PEND;

		// Set LRC on batch table 
		SetBATTabLRC(  );

	}
}



//-----------------------------------------------------------------------------
//  CleartBatchPending      Clear the batch pending flag if required
//
//  Parameters:
//      None
//  Global Inputs:
//
//  Returns:
//      Nothing
//  Notes:
//
//-----------------------------------------------------------------------------

extern void ClearBatchPending( void )
{
	struct batch_rec *pBatch_Table;	// Pointer to BATTAB entry 

	// Locate the batch table and clear the pending bit.  
	pBatch_Table = FindBatTab( CurrentAqid );
	pBatch_Table->BATFLAG &= ~BT_PEND;

	// Set LRC on batch table 
	SetBATTabLRC(  );

}


//=============================================================================
// Private function definitions
//=============================================================================