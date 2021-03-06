//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             i8583utl.c
//      iso8583 utility routines
//
//=============================================================================
#include "string.h"
#include "fixdata.h"

#include "i8583utl.h"
#include "i_host.h"
#include "tables.h"
#include "comdata.h"
#include "struct.h"
#include "findtran.h"
#include "comm.h"
#include "host.h"
#include "hstjs.h"
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
static struct ipcontab *FindIPCById( UBYTE id );


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
//!     Transfers Offlines/Adjusts to Host
//!
//! \return
//!     True on success else False
//-----------------------------------------------------------------------------
extern Bool Xfer( void )
{
    struct ipcontab* pIPC1;
    struct ipcontab* pIPC2;
	Bool Status;
    Bool AdviceProcessed = False;
    UBYTE module;

	// Set status to true
	Status = True;

	// Get the Transaction from the Request 
	HostEvent.TRNKEY = TRREQ.TRKEY;

	// Locate the Transaction - will be in HostEvent 
	HostFindTran ();

	// See if we need to presend Advices
	if ( HostEvent.TRNOPT1 & T1_PREADV )
	{
		// No; send Advices till no more to send
		while ( True )
		{
			// Send a single Advice
			if ( SendAdvice(  ) )
			{
                // Indicate Advice has been processed
                AdviceProcessed = True;

				// See if Approved
				if ( !( TRREQ.TRRSPOPT & R_APPROVED ) )
				{
					// No; copy original Transaction to Request 
					// Buffer
					memcpy( &TRREQ, &TRINP, TRSZE );

                    // Check if cancelled by user
                    if ( OnlineCancelled )
                    {
                        // Yes; set Cancelled by user response
                        memcpy( TRREQ.TRRSPC, RSP_UC, S_TRRSPC );
                    }
                    else
                    {
                        // No; set in a No Data Response Code
                        memcpy( TRREQ.TRRSPC, RSP_ND, S_TRRSPC );
                    }

					// Look up Response in TRREQ
					RspLookUp( &TRREQ );

					// Set status to false
					Status = False;

					// Exit the loop
					break;
				}
			}
			else
			{
                // Perform disconnect if required
                // Check if original transaction is a settlement
                HostEvent.TRNKEY = TRINP.TRKEY;
                HostFindTran();
                if ( HostEvent.TRNOPT2 & T2_SETTLE )
                {
                }

				// No more to send; copy original Transaction to
				// Request Buffer
				memcpy( &TRREQ, &TRINP, TRSZE );

				// Exit the loop
				break;
			}
		}
	}

	// Return the status
	return ( Status );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Trickles feed an Advice after a Transaction
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void Trickle( void )
{
	// Get the Transaction from the Request 
	HostEvent.TRNKEY = TRREQ.TRKEY;

	// Locate the Transaction - will be in HostEvent 
	HostFindTran ();

	// See if we need to hold Advices until Settle
	if ( !( ( TRREQ.TRAQPTR->AQOPT3 ) & AQ3_HOLDADV ) )
	{
		// No; see if this Transaction allows Trickle feed of Advices
		if ( HostEvent.TRNOPT1 & T1_POSTADV )
		{
			// Yes; see if Approved
			if ( TRREQ.TRRSPOPT & R_APPROVED )
			{
				// Yes; see if AVS response
				if ( !TRREQ.TRAVSRSP[0] )
				{
					// No; see if Floor Limit Response
					if ( memcmp
						 ( TRREQ.TRRSPA, ( UBYTE * ) RSPA_FLOOR, S_TRRSPA ) )
					{
						// No; send a single Advice
						SendAdvice(  );
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Sends the next Advice to the Host
//!
//! \return
//!     True if one was sent, Response in TRREQ else False
//-----------------------------------------------------------------------------
Bool SendAdvice( void )
{
	Bool Status;
	struct batch_rec *pBatch_Table;
	OS_RETURN_VALS CommStatus;
    UWORD JrnIdent;

	// Set status to true
	Status = True;

	// Locate the address of the batch table
	pBatch_Table = FindBatTab( CurrentAqid );

	// Loop till one sent or no more to send
	while ( True )
	{
		// Get Ident to Transfer
		JrnIdent = pBatch_Table->BATXFER;

		// Get control of the Journal
		JournalLock ();

		// See if zero
		if ( 0x0000 == JrnIdent )
		{
            // Set up default selection data in common record JSREC
            SetDefaultSelection ();

            // Status Bits off
            JSREC.JSSTATOFF = TS1_SETL;

            // Search for an Advice
			JSREC.JSSTATON = TS1_ADV;
            
            // Select this Acquirer
            JSREC.JSAQID = TRREQ.TRAQID;

            // Find first one to send ( oldest )
            JrnIdent = JournalSelFirst ();
			
            // Find first one to send
			if ( 0x0000 == JrnIdent )
			{
				// None; now look for an Adjust
				JSREC.JSSTATON = TS1_ADJ;

                // Find first one to send ( oldest )
                JrnIdent = JournalSelFirst ();

                if ( 0x0000 == JrnIdent )
				{
					// None; release the Journal
					JournalRelease ();

					// Indicate no more to send
					return False;
				}
			}

			// Save the Ident in the Batch Table
			pBatch_Table->BATXFER = JrnIdent;

			// Update the Batch Table LRC
			SetBATTabLRC(  );
		}

		// Ready to send Advice for Ident

		// Read the Transaction
		if ( JournalRead ( JrnIdent, &TRREQ ) )
		{
			// Okay; release the Journal
			JournalRelease ();

			// See if Voided
			if ( TRREQ.TRSTATUS[0] & TS1_VOID )
			{
				// Yes; clear the Total
				memset( TRREQ.TRTOTAM, 0,S_TRTOTAM );

				// Clear the Base Amount
				memset( TRREQ.TRBASE, 0,S_TRBASE );

				// Clear the Tip Amount
				memset( TRREQ.TRTIP, 0,S_TRTIP );
			}

			// See if an Advice; Advice takes precedence over Adjust
			if ( !( TRREQ.TRSTATUS[0] & TS1_ADV ) )
			{
				// No; must be an Adjust; set the Adjust Type
				SetAdjustType(  );

				// Increment 'STAN' and move System Trace Number to 
				// 'TRREQ'
				IncTraceNum( TRREQ.TRSTAN );
			}

			// Reset the Message Communication Flags
			MsgComFlag = 0x00;

			// Build/send the request to the host
			CommStatus = Host->pHostReq(  );

			// Clear Additional Response Data so we can look at Response
			memset( TRREQ.TRRSPA, 0,S_TRRSPA );

			// Wait for a response
			Host->pHostRsp( CommStatus );

			// See if Approved
			if ( !( TRREQ.TRRSPOPT & R_APPROVED ) )
			{
				// No; so leave as is

				// Exit the loop
				break;
			}


			// Clear Xfer ID in Batch Table
			pBatch_Table->BATXFER = 0x0000;

			// Update the Batch Table LRC
			SetBATTabLRC(  );

			// Get control of the Journal
			JournalLock ();

			// Read the Transaction
			JournalRead ( JrnIdent, &TRREQ );

			// See if a Reversal
			if ( TRREQ.TRSTATUS[0] & TS1_REV )
			{
				// Yes; we have sent an Advice of a Reversal

				// Delete the Transaction from Batch
				JournalFree ( JrnIdent );
			}
			else
			{
				// No; clear Advice and Adjust bits
				TRREQ.TRSTATUS[0] &= ~( TS1_ADV | TS1_ADJ );

				// Update the RRN in the Batch with the RRN from the
				// Response
				memcpy( TRREQ.TRRRN, TRRSP.TRRRN, S_TRRRN );

				// Clear the Original Amount
				memset( TRREQ.TRORIGAMT, 0,S_TRBASE );

				// Update the Transaction
                JrnIdent = JournalUpdate ( &TRREQ, JrnIdent );
			}

			// Release the Journal
			JournalRelease ();

			// Get back the Additional Response Data
			memcpy( TRREQ.TRRSPA, TRRSP.TRRSPA, S_TRRSPA );

			// Get back the Response Code
			memcpy( TRREQ.TRRSPC, TRRSP.TRRSPC, S_TRRSPC );

			// Look up Response in TRREQ
			RspLookUp( &TRREQ );

			// Exit the loop
			break;
		}
		else
		{
			// Not found; release the Journal
			JournalRelease ();

			// Clear Xfer ID in Batch Table
			pBatch_Table->BATXFER = 0x0000;

			// Update the Batch Table LRC
			SetBATTabLRC(  );
		}
	}

	// Return the status
	return ( Status );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sets Adjust Type of the Transaction
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void SetAdjustType( void )
{
	enum transactions AdjType;

	// Get the Transaction from the Request
	HostEvent.TRNKEY = TRREQ.TRKEY;

	// Locate the Transaction - will be in HostEvent
	HostFindTran ();

	// See what type of Adjust
	if ( HostEvent.TRNOPT1 & T1_CREDIT )
	{
		// Refund
		AdjType = ADJREFUND;
	}
	else
	{
		// Sale
		AdjType = ADJSALE;
	}

	// Save Adjust Type
	TRREQ.TRKEY = AdjType;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Uploads batch records to the host
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void Upload( void )
{
	OS_RETURN_VALS CommStatus;
	UWORD JrnIdent;
	UBYTE trans_save, upload_end;

	// Get a lock on the journal before using JSREC.
	JournalLock ();

	// Set up selection data in common record JSREC
	SetDefaultSelection ();

	// Select only this host.
	JSREC.JSAQID = TRREQ.TRAQID;

	// Include voids and reversals in the search, but not settled.
	JSREC.JSSTATOFF = TS1_SETL;

	// JrnIdent will be set with first ident, if one exists.
	JrnIdent = JournalSelFirst ();

	upload_end = True;

	while ( 0x0000 != JrnIdent )
	{
		// Unpack the complete record to TRREQ.
		JournalRead ( JrnIdent, &TRREQ );

		// Set transaction
		TRREQ.TRKEY = RUPLOAD;

		// Increment System Trace Audit No.
		IncTraceNum( TRREQ.TRSTAN );

		// Preset again flag.
		TRREQ.TRAGAIN = False;

		// Set again flag if there is another record to be sent.
		if ( 0x0000 != JournalSelNext ( JrnIdent ) )
		{
			TRREQ.TRAGAIN = True;
		}

		// Reset the Message Communication Flags
		MsgComFlag = 0x00;

		// Send the request to the host
		CommStatus = Host->pHostReq(  );

		// Wait for the response
		Host->pHostRsp( CommStatus );

		if ( !( TRREQ.TRRSPOPT & R_APPROVED ) )
		{
			upload_end = False;
			break;
		}

		JrnIdent = JournalSelNext ( JrnIdent );
	}

	if ( upload_end )
	{
		// Copy saved request back from TRINP.
		memcpy( &TRREQ, &TRINP, TRSZE );

		// Temporarily save the original transaction key.
		trans_save = TRREQ.TRKEY;

		// Set the transaction to clear batch end.
		TRREQ.TRKEY = CLBATCHE;

		// Increment System Trace Audit No.
		IncTraceNum( TRREQ.TRSTAN );

		// Reset the Message Communication Flags
		MsgComFlag = 0x00;

		// Send the request to the host
		CommStatus = Host->pHostReq(  );

		// Wait for the response
		Host->pHostRsp( CommStatus );

		// Restore the saved transaction key.
		TRREQ.TRKEY = trans_save;
	}

	// Look up response in TRREQ.
	RspLookUp( &TRREQ );

	// Release the lock on the journal.
	JournalRelease ();
}

//-----------------------------------------------------------------------------
//! \brief
//!     All voids in batch so do not settle
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void AllVoids( void )
{
	// Allow exit from visak without actually sending to VISAK host.
	// Set response code to APPROVED
	memcpy( TRREQ.TRRSPC, RSP_APP, S_TRRSPC );

	// Set host response text.
	memset( ( char * ) TRREQ.TRHOSTTEXT, ' ',S_TRHOSTTEXT );
	GetMsg( NoBatchTotals, CSTMSG );

	memcpy( TRREQ.TRHOSTTEXT, CSTMSG, StrLn( CSTMSG, sizeof( CSTMSG ) ) );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Finds the IP Connect Entry given the Id
//!
//! \param
//!		[in]id				IP Connect Number
//!
//! \return
//!     Pointer to IP Connect Entry or NULL if not found
//-----------------------------------------------------------------------------
struct ipcontab *FindIPCById( UBYTE id )
{
	UBYTE i;

	for( i = 0; i < IPCMAX; i++ )
	{
		if( IPCTAB[i].id == id )
		{
			return &IPCTAB[i];
		}
	}
	return NULL;
}

