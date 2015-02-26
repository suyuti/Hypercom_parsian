//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             journal.c
//      Journal functions
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdktime.h"
#include "fixdata.h"
#include "tlib.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "transdef.h"
#include "tables.h"
#include "findtran.h"
#include "hstmsg.h"

#include "journal.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static UBYTE SelMatch( void );
static void JournalClearOneAcq( void );
static void JournalCheckEmpty( void );
static Bool SetPtrs( struct trandata_rec * pTranBuf );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
const struct jrnarea_rec Jrn_Areas[] = {
	{
	 pn_PAGE31,					// page number
	 0x0080,					// area start address 
	 0x7f80						// size of area (ending addr - start addr + 1) 
	 },

	{	 //** LAST AREA -- DONT MOVE IT -------------**
	 0x00,						// page number 
	 0x0000,					// area start address     
	 0x0000						// size of area (ending addr - start addr + 1)
	 }
};


//=============================================================================
// Public function definitions
//=============================================================================
//-----------------------------------------------------------------------------
//! \brief Prepares journal by passing varaiable and pointers to journal areas.
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void JournalPrepare( void )
{
	UWORD transaction_size;
	UWORD jrn_fixed_size;
	UWORD jrn_compressed_size;
	UWORD max_jrn_size;
	const struct jrnarea_rec *pJnrAreaAddr;
	struct trandata_rec *pJrrecAddr;
	struct ll_def *pLLbufAddr;
	struct trandata_rec *pJhbufAddr;
	UBYTE *pThisAddr;
	UBYTE *pJrnCompressedAddr;
	int *pJrnRoomCount;

	// Size of trandata_rec.
	transaction_size = TRSZE;

	// Set the size of the journal uncompressed header
	jrn_fixed_size = TRFIXEDSZE;

	// Size of the compressed area
	jrn_compressed_size = TRCOMPSZE;

	// Size of the entire journal area, uncompressed + compressed +
	// 2 byte compressed size + 32 bytes for expansion.
	max_jrn_size = jrn_fixed_size + jrn_compressed_size + 2 + 32;

	// Set the pointer to the table defining pages used by the journal.
	pJnrAreaAddr = Jrn_Areas;

	// Set the pointer to the address of JRREC
	pJrrecAddr = &JRREC;

	// Set the pointer to the address of the linked portion of JRBUF
	pLLbufAddr = &JRBUF.LLBUF;

	// Set the pointer to the address of the fixed portion of JRBUF
	pJhbufAddr = &JRBUF.TRDATA;

	// Set the pointer to the address of the compressed portion of JRBUF
	pJrnCompressedAddr = &JRBUF.TRDATA.TRFIXEDEND + 1;

	// Set the pointer to the address of THIS - used in selection process
	pThisAddr = THIS;

	// Set the pointer to the count of remaining maximum sized records in journal
	pJrnRoomCount = &JrnRoomCount;

	// Send to assembly code using wrapper funtion
	JrnSetupW( transaction_size, jrn_fixed_size, jrn_compressed_size,
			   max_jrn_size, pJnrAreaAddr, ( UBYTE * ) pJrrecAddr, pLLbufAddr,
			   ( UBYTE * ) pJhbufAddr, pJrnCompressedAddr, pThisAddr,
			   pJrnRoomCount );
}


//-----------------------------------------------------------------------------
//! \brief Validates journal format flag. Also validates the linked list.
//!        Formats the journal if not valid.
//! 
//! \return
//!     False if the journal was reformated and empty else True.
//-----------------------------------------------------------------------------
Bool JournalValidate( void )
{
    UBYTE Status;

	// Lock the journal semaphore
	JournalLock(  );

	// Call wrapper to validate then set Status with condition code
	Status = JournalValidateW(  );

    // Determine "Journal not empty" flag for Application Manager
    JournalCheckEmpty ( );

    // Release the journal semaphore
	JournalRelease(  );

    // Return status
    if ( 0 != Status )
        return False;
    else
        return True;

}


//-----------------------------------------------------------------------------
//! \brief Unconditionally format the journal.
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void JournalFormat( void )
{
	// Lock the journal semaphore
	JournalLock(  );

	// Call wrapper to format the journal
	JournalFormatW(  );

    // Determine "Journal not empty" flag for Application Manager
    JournalCheckEmpty ( );

	// Release the journal semaphore
	JournalRelease(  );
}


//-----------------------------------------------------------------------------
//! \brief Frees journal entry pointed to by identifier.
//! 
//! \param
//!     JrnIdent    the identifier to be free
//! 
//! \return
//!     status of JournalFreeW: False is bad, True is good.
//! 
//! \note
//!     Lock/release of journal is responsibility of caller.
//-----------------------------------------------------------------------------
UBYTE JournalFree( UWORD JrnIdent )
{
    UBYTE Status;

	// Call wrapper to free the journal entry
	Status = JournalFreeW( JrnIdent );

    // Determine "Journal not empty" flag for Application Manager
    JournalCheckEmpty ( );

    return Status;
}



//-----------------------------------------------------------------------------
//! \brief Updates journal entry.
//! 
//! \param
//!     pTranBuf    pointer to transaction data
//!     JrnIdent    the identifier to be updated
//! 
//! \return
//!     New identifier or 0000 if error occured.
//! 
//! \note
//!     Lock/release of journal is responsibility of caller.
//-----------------------------------------------------------------------------
UWORD JournalUpdate( struct trandata_rec * pTranBuf, UWORD JrnIdent )
{
	int total_bytes;
    UWORD NewIdent;

	// Pack the data in JRBUF for writing to journal
	total_bytes = JournalPackW( ( UBYTE * ) pTranBuf );	// Data goes to  JRBUF struct

	// Call wrapper to update the journal entry
	// JRBUF has the packed data
	NewIdent = JournalUpdateW( JrnIdent, ( UWORD ) total_bytes );

    // Determine "Journal not empty" flag for Application Manager
    JournalCheckEmpty ( );

    // Return ident updated
    return NewIdent;
}

//-----------------------------------------------------------------------------
//! \brief Update journal entry pointed to by JrnIdentifier.
//!        Differs from JournalUpdate in that the entry must exist already
//!        in the entry and will be located first if necessary.
//!
//! \param
//!     pTranBuf    pointer to transaction data
//! 
//! \return
//!     True if successful update/rewrite.
//-----------------------------------------------------------------------------
extern Bool JournalReWrite( struct trandata_rec * pTranBuf )
{
	// Get control of the Journal.
	JournalLock();

	// See if we need to locate the journal entry.
	if( !pTranBuf->TRJRNIDENT )
	{
		// Set up selection data in common record JSREC 
		SetDefaultSelection();

		// Select only this host.  
		JSREC.JSAQID = pTranBuf->TRAQID;

		// Move Invoice Number into Journal search 
		MoveIt( JSREC.JSINV, pTranBuf->TRINV, S_TRINV );

		// 
		JSREC.JSSTATOFF = 0x00;

		// Look for it in the journal.
		pTranBuf->TRJRNIDENT = JournalSelFirst();
	}

	// Make sure we have located the journal entry.
	if( pTranBuf->TRJRNIDENT )
	{
		// Update the Transaction.
		JournalUpdate( pTranBuf,pTranBuf->TRJRNIDENT );

		// Release the Journal
		JournalRelease();

		return True;
	}

	// Release the Journal
	JournalRelease();
	return False;
}


//-----------------------------------------------------------------------------
//! \brief Allocates a new journal record for reversal or for non-reversal.
//! 
//! \param
//!     pTranBuf    pointer to transaction data
//! 
//! \return
//!     New identifier or 0000 if not processed.
//-----------------------------------------------------------------------------
UWORD JournalNew( struct trandata_rec * pTranBuf )
{
	int total_bytes;
	UWORD NewIdent;

	// Get the transaction key options to SecondEvent.
	SecondEvent.TRNKEY = pTranBuf->TRKEY;
	FindTran ();

	// Is reversal required for this transaction ?
    if ( ( pTranBuf -> TRSTATUS[0] & TS1_REV) && 
        !( SecondEvent.TRNOPT3 & T3_REV ) )
	{
        // Requested reversal does not required for this transaction
		return 0x0000;
	}
    
    // Check if capture required for this transaction
	if ( SecondEvent.TRNOPT1 & T1_NOCAPTURE )
	{
        // Not required
		return 0x0000;
	}

	// Lock the journal semaphore
	JournalLock ();
    
	// Pack the data in JRBUF for writing to journal
	total_bytes = JournalPackW( ( UBYTE * ) pTranBuf );	// Data goes to  JRBUF struct

	// Test the training bit in the host config.  If set then set
	// the status byte in the journal.
	if ( TCONF.TCOPT4 & TC4_TRAINING )
	{
		JRBUF.TRDATA.TRSTATUS[0] |= TS1_TRAIN;
	}

	// Allocate and store data - allow for growth of compressed area
	// Compressed area size will depend on RLE compression routine.
	NewIdent = JournalAllocateW( total_bytes + 32 );

    // Determine "Journal not empty" flag for Application Manager
    JournalCheckEmpty ( );

	// Release the journal semaphore
	JournalRelease ();

    // Return ident
    return NewIdent;
}



//-----------------------------------------------------------------------------
//! \brief Finds and reads journal record identified into transaction buffer.
//! 
//! \param
//!     Ident       transaction journal identifier
//!     pTranBuf    pointer to transaction data
//! 
//! \return
//!     True on success else False.
//-----------------------------------------------------------------------------
UBYTE JournalRead( UWORD Ident, struct trandata_rec * pTranBuf )
{
    UBYTE Status;

	Status = JournalReadW( Ident, ( UBYTE * ) pTranBuf, True );

	if ( Status )
	{
		// Save the original key & the original STAN
		pTranBuf->TRORIGKEY = pTranBuf->TRKEY;
		memcpy( pTranBuf->TROSTAN, pTranBuf->TRSTAN, S_TRSTAN );

		// Now must recalculate base amount from tip and total
		SubAmt( pTranBuf->TRBASE, pTranBuf->TRTOTAM,
				pTranBuf->TRTIP );

		// Recalculate base + bar amount
		SubAmt( pTranBuf->TRBASE, pTranBuf->TRBASE,
				pTranBuf->TRTAX );

		// Recalculate base + bar - bar
		SubAmt( pTranBuf->TRBASE, pTranBuf->TRBASE,
				pTranBuf->TRBAR );

		// Recalculate base - cashback = base + tax
		SubAmt( pTranBuf->TRBASE, pTranBuf->TRBASE,
				pTranBuf->TRCASHBK );

		// Recalculate total + fee
		AddAmt( pTranBuf->TRTOTF, pTranBuf->TRTOTAM,
				pTranBuf->TRSURC );

		// Set transaction pointers for card range, issuer and acquirer.
		if ( !SetPtrs( pTranBuf ) )
			Status = False;

		// Set up response text
		RspLookUp( pTranBuf );
	}

    return Status;
}


//-----------------------------------------------------------------------------
//! \brief Finds first journal record per JSREC selection record.
//! 
//! \return
//!     Transaction journal identifier or 0x0000 if not found.
//! 
//! \note
//!     Global variable JSREC selection record must be defined before calling.
//!     JRBUF will have LL and FIXED data. Journal Lock must be acquired
//!     before calling.
//-----------------------------------------------------------------------------
UWORD JournalSelFirst( void )
{
    UWORD Ident = 0x0000;

	Ident = JournalFirstW(  );

	// If a record was found see if the criteria matches
	// SelMatch will return True if journal matches criteria
	while ( Ident != 0x0000 )
	{
		if ( SelMatch(  ) == True )
			break;

        Ident = JournalNextW( Ident );
	}

    return Ident;
}


//-----------------------------------------------------------------------------
//! \brief Finds next journal record per JSREC selection record.
//! 
//! \param
//!     PrevIdent   current transaction journal identifier
//! 
//! \return
//!     Transaction journal identifier or 0x0000 if not found.
//! 
//! \note
//!     Global variable JSREC selection record must be defined before calling.
//!     JRBUF will have LL and FIXED data. Journal Lock must be acquired
//!     before calling.
//-----------------------------------------------------------------------------
UWORD JournalSelNext( UWORD PrevIdent )
{
    UWORD NextIdent;

	NextIdent = JournalNextW( PrevIdent );
	
    while ( NextIdent != 0x0000 )
	{
		if ( SelMatch(  ) == True )
			break;

		NextIdent = JournalNextW( NextIdent );
	}

    return NextIdent;
}


//-----------------------------------------------------------------------------
//! \brief Finds previous journal record per JSREC selection record.
//! 
//! \param
//!     CurrIdent   current transaction journal identifier
//! 
//! \return
//!     Transaction journal identifier or 0x0000 if not found.
//! 
//! \note
//!     Global variable JSREC selection record must be defined before calling.
//!     JRBUF will have LL and FIXED data. Journal Lock must be acquired
//!     before calling.
//-----------------------------------------------------------------------------
UWORD JournalSelPrevious( UWORD CurrIdent )
{
    UWORD PrevIdent;

	PrevIdent = JournalPreviousW( CurrIdent );
	
    while ( PrevIdent != 0x0000 )
	{
		if ( SelMatch(  ) == True )
			break;

		PrevIdent = JournalPreviousW( PrevIdent );
	}

    return PrevIdent;
}

//-----------------------------------------------------------------------------
//! \brief Clears batch per JSREC selection record.
//! 
//! \return
//!     None.
//! 
//! \note
//!     Global variable JSREC selection record must be defined before calling.
//!     Journal Lock must be acquired before calling.
//-----------------------------------------------------------------------------
extern void JournalClearBatch( void )
{
	struct acquirer_rec *pAcquirerPtr;	// Pointer to AQTAB entry

	// Clear all acquirers if JSREC acquirer id is 0
	if ( JSREC.JSAQID )
	{
		// Clear batch for one acquirer.  ID is at JSREC.JSAQID
		JournalClearOneAcq(  );
	}
	else
	{
		// Clear batch for all acquirers.
		pAcquirerPtr = FindFirstAcquirer(  );
		while ( NULL != pAcquirerPtr )
		{
			JSREC.JSAQID = pAcquirerPtr->AQID;
			JournalClearOneAcq(  );
			pAcquirerPtr = FindNextAcquirer( pAcquirerPtr );
		}
	}

    // Determine "Journal not empty" flag for Application Manager
    JournalCheckEmpty ( );

}


//-----------------------------------------------------------------------------
//! \brief Clears the Journal of all Training Transactions.
//! 
//! \return
//!     None.
//! 
//! \note
//!     Global variable JSREC selection record must be defined before calling.
//-----------------------------------------------------------------------------
extern void TrainJournalClear( void )
{
	// Make sure we are in Training Mode
	if ( TCONF.TCOPT4 & TC4_TRAINING )
	{
		// Yes; lock the journal semaphore
		JournalLock(  );

		// Set default selection information into JSREC
		SetDefaultSelection(  );

		// Voided or not
		JSREC.JSSTATOFF = 0x00;

		// Clear all Transactions (Journal makes sure to only clear
		// Training)
		JournalClearBatch(  );

		// Release the journal semaphore
		JournalRelease(  );
	}
}



//-----------------------------------------------------------------------------
//! \brief Sets the settle bit for one acquirer.
//! 
//! \return
//!     None.
//! 
//! \note
//!     Global variable JSREC has acquired id.
//!     Journal Lock must be acquired before calling.
//-----------------------------------------------------------------------------
extern void JournalSetSettle( void )
{
    UWORD SettleJrnIdent;

	SettleJrnIdent = JournalFirstW(  );

	// If a record was found see if for this acquirer.
	while ( SettleJrnIdent != 0x0000 )
	{
		// If record for this acquirer check training mode.
		if ( JSREC.JSAQID == JRBUF.TRDATA.TRAQID )
		{
			// If this is a training transaction then set settled.
			// or not a training transaction and not training mode
			if ( ( JRBUF.TRDATA.TRSTATUS[0] & TS1_TRAIN ) ||
				 ( !( JRBUF.TRDATA.TRSTATUS[0] & TS1_TRAIN ) &&
				   !( TCONF.TCOPT4 & TC4_TRAINING ) ) )
			{
				// Set the settled bit on this journal entry.
				JRBUF.TRDATA.TRSTATUS[0] |= TS1_SETL;

				// Write fixed portion of record back into journal.
				JournalPutFixedW(  );
			}
		}
		SettleJrnIdent = JournalNextW( SettleJrnIdent );
	}

    // Determine "Journal not empty" flag for Application Manager
    JournalCheckEmpty ( );
}


//-----------------------------------------------------------------------------
//! \brief Gets a lock on the journal.
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void JournalLock( void )
{
	// Don't return to user until lock is acquired.
	while ( !JournalLockWrapper(  ) )
	{
		// Give up rest of time slice until lock is acquired.
		SDK_RelinqCPU(  );
	}
}



//-----------------------------------------------------------------------------
//! \brief Releases the lock on the journal.
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void JournalRelease( void )
{
	JournalReleaseWrapper(  );
}

//-----------------------------------------------------------------------------
//! \brief Formats the global journal selection record JSREC with defaults.
//! 
//! \return
//!     None.
//! 
//! \note
//!     JSSTATOFF defaults to TS1_VOID + TS1_REV + TS1_SETL. Caller can
//!     override defaults with own criteria.
//-----------------------------------------------------------------------------
extern void SetDefaultSelection( void )
{
	// Set default selection information into global JSREC
	memset( ( UBYTE * ) & JSREC, 0, JSSZE );

	// Search for any transaction key
	JSREC.JSKEY = 0xff;

	// Search for any type records
	JSREC.JSTYPE = 0xff;

	// STATUS BITS OFF
	JSREC.JSSTATOFF = TS1_VOID + TS1_REV + TS1_SETL;
}

//-----------------------------------------------------------------------------
//! \brief Gets the number of maxsize records that could still fit in the
//!        journal.
//! 
//! \return
//!     Number of transaction available to allocate in the journal.
//-----------------------------------------------------------------------------
int JournalRoom( void )
{
    int NumOfRec;

    // Lock the journal
    JournalLock( );

    // Get the number of records available to allocate
    NumOfRec = JrnRoomCountW(  );

    // Release journal
    JournalRelease( );

    // Return 
    return NumOfRec;
}


//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief Check match between selection record, JSREC, and the fixed part
//!        of the record read from the journal, JRBUF.TRDATA.
//! 
//! \return
//!     True means there is a match otherwise False.
//-----------------------------------------------------------------------------
static UBYTE SelMatch(  )
{
	// If in training mode select only training transactions
	if ( TCONF.TCOPT4 & TC4_TRAINING )
	{
		if ( !( JRBUF.TRDATA.TRSTATUS[0] & TS1_TRAIN ) )
			return ( False );
	}

	// Match journal record identifier.
	if ( JSREC.JSIDENT )
	{
		if ( JSREC.JSIDENT != JRBUF.LLBUF.LLIdent )
			return ( False );
	}

	// Match transaction key.  JSKEY of 0xff means match any.
	if ( 0xff != JSREC.JSKEY )
	{
		if ( JSREC.JSKEY != JRBUF.TRDATA.TRKEY )
			return ( False );
	}

	// Match acquirer ID.
	if ( JSREC.JSAQID )
	{
		if ( JSREC.JSAQID != JRBUF.TRDATA.TRAQID )
			return ( False );
	}

	// Match acquirer ID to be ignored.
	if ( JSREC.JSAQIGNORE )
	{
		if ( JSREC.JSAQIGNORE == JRBUF.TRDATA.TRAQID )
			return ( False );
	}

	// Match issuer ID.
	if ( JSREC.JSISID )
	{
		if ( JSREC.JSISID != JRBUF.TRDATA.TRISID )
			return ( False );
	}

	// Match invoice number.
	if ( !NullComp( ( char * ) JSREC.JSINV, S_TRINV ) )
	{
		if ( memcmp( JSREC.JSINV, JRBUF.TRDATA.TRINV, S_TRINV ) )
			return ( False );
	}

	// Match cashier.
	if ( !NullComp( ( char * ) JSREC.JSCASHIER, S_TRCASHIER ) )
	{
		if ( memcmp( JSREC.JSCASHIER, JRBUF.TRDATA.TRCASHIER, S_TRCASHIER ) )
			return ( False );
	}

	// Is shift processing match required?
	if ( JSREC.JSSHIFT )
	{
		// Only match shift number if configured.
		if ( TCONF.TCOPT5 & TC5_SHIFT )
		{
			if ( JSREC.JSSHIFT != JRBUF.TRDATA.TRSHIFT )
			{
				return ( False );
			}
		}
	}

	// Match total amount.
	if ( !NullComp( ( char * ) JSREC.JSTOTAM, S_TRTOTAM ) )
	{
		if ( memcmp( JSREC.JSTOTAM, JRBUF.TRDATA.TRTOTAM, S_TRTOTAM ) )
		{
			return ( False );
		}
	}

	// Match status bits that must be on.
	if ( ( JSREC.JSSTATON & JRBUF.TRDATA.TRSTATUS[0] ) != JSREC.JSSTATON )
	{
		return ( False );
	}

	// Match status bits 2 that must be on.
	if ( ( JSREC.JSSTATON2 & JRBUF.TRDATA.TRSTATUS[1] ) != JSREC.JSSTATON2 )
	{
		return ( False );
	}

	// Match status bits that must be off.
	if ( JSREC.JSSTATOFF & JRBUF.TRDATA.TRSTATUS[0] )
	{
		return ( False );
	}

	// Match status bits 2 that must be off.
	if ( JSREC.JSSTATOFF2 & JRBUF.TRDATA.TRSTATUS[1] )
	{
		return ( False );
	}

	// Match transaction type.  JSTYPE of 0xff means match any.
	if ( 0xff != JSREC.JSTYPE )
	{
		// Get the transaction from the journal header read in
		SecondEvent.TRNKEY = JRBUF.TRDATA.TRKEY;

		// Locate the transaction - will be in SecondEvent
		FindTran ();

		// memcmpe T1_CREDIT from transaction definition with
		// JSTYPE.  If not the same then no match.
		if ( JSREC.JSTYPE != ( SecondEvent.TRNOPT1 & T1_CREDIT ) )
		{
			return ( False );
		}
	}

	return ( True );
}



//-----------------------------------------------------------------------------
//! \brief Clears a batch per JSREC selection record.
//! 
//! \return
//!     None.
//! 
//! \note
//!     Global JSREC is selection record. Journal Lock was acquired before calling.
//-----------------------------------------------------------------------------
static void JournalClearOneAcq( void )
{
    UWORD Ident;

	// Locate and delete all journal records for the acquirer in JSREC
	Ident = JournalSelFirst(  );

	// Loop through the rest of the batch freeing as we go.
	while ( 0x0000 != Ident )
	{
		JournalFreeW( Ident );

		// Locate the next record in the batch
		// Keep searching for first, not next, because are freeing.
		Ident = JournalSelFirst(  );
	}
}

//-----------------------------------------------------------------------------
//! \brief Determines flag "Journal not empty" for Application Manager.
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
static void JournalCheckEmpty( void )
{
    struct jrnfind_rec tmpJSREC;

    // Do nothing in trainig mode
	if ( TCONF.TCOPT4 & TC4_TRAINING )
        return;

    // Save journal selection record
    memcpy (&tmpJSREC, &JSREC, sizeof (struct jrnfind_rec));

    // Set up selection data in common record JSREC
	SetDefaultSelection(  );

#ifdef MAKE_VIKING
    // Set "Journal not empty" if any record found
	if ( 0x0000 == JournalSelFirst(  ) )
	{
        TERM.TERMJrnStatus &= ~TMJRN_NOT_EMPTY;
	}
    else
    {
        TERM.TERMJrnStatus |= TMJRN_NOT_EMPTY;
    }
#endif // MAKE_VIKING

    // Recalculate LRC
	SetTERMTabLRC(  );

    // Restore identifier and selection record
    memcpy (&JSREC, &tmpJSREC, sizeof (struct jrnfind_rec));

}

//-----------------------------------------------------------------------------
//! \brief Sets pointers to card range, issuer and acquirer.
//! 
//! \return
//!     True on success else False.
//-----------------------------------------------------------------------------
Bool SetPtrs( struct trandata_rec * pTranBuf )
{
	// Check, if it is a reconciliation transaction
	if ( CLBATCH == pTranBuf -> TRKEY )
	{
		// Set the acquirer pointer
		pTranBuf -> TRAQPTR = (struct acquirer_rec *) FindAcquirer(pTranBuf -> TRAQID);

		return(True);
	}

   	// Locate card range table and do some verification     
	if ( NULL == (pTranBuf -> TRCRPTR = FindCR_ById(pTranBuf -> TRCRNUM)) )
	{
		// Card Range Not Found
		return( False );
	}

	// Set the issuer pointer  
	pTranBuf->TRISPTR =	( struct issuer_rec * ) FindIssuer( pTranBuf->TRISID );

	// Set the acquirer pointer  
	pTranBuf->TRAQPTR =	( struct acquirer_rec * ) FindAcquirer( pTranBuf->TRAQID );

	// Verify all pointers were set  
	if (   ( pTranBuf->TRCRPTR == NULL ) 
		|| ( pTranBuf->TRISPTR == NULL ) 
		|| ( pTranBuf->TRAQPTR == NULL ) )
	{
		return ( False );
	}

	// Look for the transaction 
	SecondEvent.TRNKEY = pTranBuf->TRKEY;
	FindTran ();

	// See if we need to set up issuer name 
	if ( SecondEvent.TRNOPT3 & T3_NOISNAME )
	{
		// Clear, in case issuer name had previously been set 
		memset( ( char * ) pTranBuf->TRTRANBUF, ' ', 10 );
	}
	else
	{
		// Set the issuer name 
		// TTRANSBUF uses the first 10 positions for the issuer 
		// and the second 10 for the transaction name.          
		MoveItIss( pTranBuf->TRTRANBUF, ( pTranBuf->TRISPTR )->ISNAME, 10 );
	}
	return ( True );
}

