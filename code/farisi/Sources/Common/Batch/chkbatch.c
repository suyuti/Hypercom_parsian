//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             chkbatch.c
//      Check clear batch routines
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "journal.h"
#include "tables.h"
#include "findtran.h"
#include "incbatno.h"

#include "chkbatch.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static void ClearSettle( UBYTE aqid );


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
//! \brief Clears the batch if it has been settled.
//! 
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void ChkClrBatch( void )
{
	UBYTE aqid;

	// Don't check for time to clear batch if noclr option is on
	if ( !( CurrentEvent.TRNOPT4 & T4_NOCLRBAT ) )
	{
		if ( 0 != ( aqid = CheckSettle( TRINP.TRAQID ) ) )
		{
			// Get a lock on the journal before using JSREC.
			JournalLock ();

			// Set up journal selection record to clear all
			// transaction keys and types for acquirer
			SetDefaultSelection ();
			JSREC.JSAQID = aqid;
			JSREC.JSSTATOFF = 0;

			// If not in training mode then clear all settled
			// If in training clear all, settled or not.
			if ( !( TCONF.TCOPT4 & TC4_TRAINING ) )
			{
				JSREC.JSSTATON = TS1_SETL;
			}

			// clear this batch tranactions from the journal
			JournalClearBatch ();

			// Release the lock on the journal.
			JournalRelease ();

			// If not in training mode update BATTAB.
			if ( !( TCONF.TCOPT4 & TC4_TRAINING ) )
			{
				// Increment batch number in batch table for this acquirer
				IncBatNo ( aqid );
			}

			// Clear aquirer id from TFTAB
			TFTAB.TFLAQID = 0;
			SetTFTabLRC(  );

			// Clear the BT_SETL flag in the BATTAB entry.
			ClearSettle( aqid );
		}
	}
}

//-----------------------------------------------------------------------------
//! \brief Looks for a settled batch.
//! 
//! \param
//!     aqid        Acquirer id. 0 if all acquirers.
//! 
//! \return
//!     Acquirer id if acquirer settled otherwise False.
//-----------------------------------------------------------------------------
extern UBYTE CheckSettle( UBYTE aqid )
{
	struct batch_rec *pBatch_Table;	// Pointer to BATTAB entry
	struct acquirer_rec *pHostptr;	// Pointer to AQTAB entry

	// Check one acquirer or all?
	if ( 0 != aqid )
	{
		// Only one acquirer to check
		pBatch_Table = FindBatTab( aqid );

		// is settled?
		if ( pBatch_Table->BATFLAG & BT_SETL )
		{
			return ( aqid );
		}
	}
	else
	{
 		// Check all acquirers
		for ( pHostptr = AQTAB; pHostptr < &AQTAB[AQMAX]; pHostptr++ )
		{
			// Look at every entry - if ID then entry is used.
			if ( pHostptr->AQID )
			{
				pBatch_Table = FindBatTab( pHostptr->AQID );

				// is only financial settled and sigcap NOT enabled?
				if ( pBatch_Table->BATFLAG & BT_SETL )
				{
					return ( pHostptr->AQID );
				}
			}
		}
	}
	return ( False );
}

//-----------------------------------------------------------------------------
//! \brief Looks at Batch Pending.
//! 
//! \return
//!     True if Batch is Pending otherwise False
//-----------------------------------------------------------------------------
extern Bool ChkBatPend( void )
{
	Bool PendFlag;
	struct batch_rec *pBatchRec;

	// Reset pending flag
	PendFlag = False;

	// See if Acquirer selected yet
	if ( TRINP.TRAQID )
	{
		// Yes; locate the transaction options.
		SecondEvent.TRNKEY = TRINP.TRKEY;
		FindTran ();

		// See if we need to look at pending
		if ( SecondEvent.TRNOPT3 & T3_CHK_PEND )
		{
			// Yes; get Acquirer ID
			pBatchRec = FindBatTab( TRINP.TRAQID );

			// See if pending set
			if ( pBatchRec->BATFLAG & BT_PEND )
			{
				// Yes; set pending flag to true
				PendFlag = True;
			}
		}
	}

	// Return pending flag
	return ( PendFlag );
}

//-----------------------------------------------------------------------------
//! \brief Looks at Batch Settle.
//! 
//! \return
//!     0 - Batch is not settled,<br>
//!     1 - Batch both financial and sigcap settled,<br>
//!     2 - Batch is only financial settled,<br>
//!     3 - Batch is only sigcap settled
//-----------------------------------------------------------------------------
extern UBYTE ChkBatSettle( void )
{
	UBYTE retval;
	struct batch_rec *pBatchRec;

	// Reset pending flag
	retval = 0;

	// See if Acquirer selected yet
	if ( TRINP.TRAQID )
	{
		// Yes; get Acquirer ID
		pBatchRec = FindBatTab( TRINP.TRAQID );

		// are both financial and sigcap settled ?
		if ( ( pBatchRec->BATFLAG & BT_SETL ) &&
			 ( pBatchRec->BATFLAG & BT_SIGSETL ) )
		{
			retval = 1;			// Yes; return value
		}
		// is only financial batch settled ?
		else if ( pBatchRec->BATFLAG & BT_SETL )
		{
			retval = 2;			// Yes; return value
		}
		// is sigcap batch settled ?
		else if ( pBatchRec->BATFLAG & BT_SIGSETL )
		{
			retval = 3;			// Yes; return value
		}
	}

	// Return pending flag
	return ( retval );
}

//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief Clears BT_SETL flag in the batch table.
//! 
//! \param
//!     aqid        Clear BT_SETL flag in the batch table.
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
static void ClearSettle( UBYTE aqid )
{
	struct batch_rec *pBatch_Table;	// Pointer to BATTAB entry
	struct acquirer_rec *pHostptr;	// Pointer to AQTAB entry

	// Check one acquirer or all?
	if ( 0 != aqid )
	{
		// Only one acquirer to clear
		pBatch_Table = FindBatTab( aqid );
		pBatch_Table->BATFLAG &= ~( BT_SETL | BT_SIGSETL );
	}
	else
	{
		// Check all acquirers
		for ( pHostptr = AQTAB; pHostptr < &AQTAB[AQMAX]; pHostptr++ )
		{
			// Look at every entry - if ID then entry is used.
			if ( pHostptr->AQID )
			{
				pBatch_Table = FindBatTab( pHostptr->AQID );
				pBatch_Table->BATFLAG &= ~( BT_SETL | BT_SIGSETL );
			}
		}
	}

	// Set LRC on batch table
	SetBATTabLRC(  );
}

