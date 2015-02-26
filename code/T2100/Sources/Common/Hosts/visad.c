
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             visad.c
//      VISA D Host Module.

//=============================================================================
//=============================================================================

#include "string.h"
#include <stdlib.h>
#include <ctype.h>
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "struct.h"
#include "appdata.h"

#include "defines.h"
#include "transdef.h"
#include "termtab.h"
#include "isdebit.h"
#include "findtran.h"
#include "batpend.h"
#include "msg.h"
#include "util.h"
#include "journal.h"
#include "utllib.h"
#include "i8583utl.h"
#include "hstcopy.h"
#include "comm.h"
#include "reqvisad.h"
#include "rspvisad.h"


//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static Bool UploadVisaD( void );
static void DebitConfirm( void );


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
//  VisaD           Entry point for VisaD host processing.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

extern void VisaD( void )
{
	OS_RETURN_VALS CommStatus;

	// Special test to be sure not settling batch that
	// has only voids.  If there is no amount for debit
	// or credit then do not send anything to VisaD host.
	if ( ( ( CLBATCH == TRREQ.TRKEY )
		   || ( CLBATCHA == TRREQ.TRKEY ) )
		 &&
		 ( ( NullComp( ( char * ) TERMTOTALS.TOTBASIC.TOTDBCNT, S_TOTDBCNT ) )
		   &&
		   ( NullComp
			 ( ( char * ) TERMTOTALS.TOTBASIC.TOTCRCNT, S_TOTCRCNT ) ) ) )
	{
		// Nothing to settle. 
		AllVoids(  );
	}
	else
	{
		// Copy the input transaction to the request buffer 
		memcpy( &TRREQ, &TRINP, TRSZE );

		// See if this should be processed offline 
		if ( !Offline(  ) )
		{
			// No; see if floor limit approved 
			if ( !Floor(  ) )
			{
				// No; set batch pending if required 
				SetBatchPending(  );

				// Increment 'STAN' and move System Trace Number to 'TRREQ' 
				IncTraceNum( TRREQ.TRSTAN );

				// Calculate total + reauth margin 
				CalcReauth( TRREQ.TRTOTAMI, TRREQ.TRTOTAM );

				if ( !UploadVisaD(  ) )
				{
					// Reset the Message Communication Flags 
					MsgComFlag = 0x00;

					// Send the request to the host  
					CommStatus = ReqVisaD(  );

					// Wait for the response    
					RspVisaD( CommStatus );
				}

				// See if a Debit Type Transaction 
				if ( IsDebitTrans( TRREQ.TRKEY ) )
				{
					// Yes; send a Debit confirmation 
					DebitConfirm(  );
				}
			}
			else				// Then is a floor limit 
			{
				// POS device generated authorization code 
				TRREQ.TRAUTHSOURCE = '6';
			}					// End of FloorLimit
		}						// Else of Offline
		else
		{
			// Get the Transaction from the Request 
			HostEvent.TRNKEY = TRREQ.TRKEY;

			// Locate the Transaction - will be in HostEvent 
			HostFindTran();

			// See what type of Adjust 
			if ( ( HostEvent.TRNOPT1 & T1_CREDIT ) ||
				 ( 0 == TRREQ.TRAUTH[0] ) )
			{
				// Refund or Not Authorized 
				TRREQ.TRAUTHSOURCE = '9';
			}
			else
			{
				// offline voice approved 
				TRREQ.TRAUTHSOURCE = 'E';
			}
		}						// End of Offline

		// Perform Referral processing. 
		// FUTURE  

		// Add approved transaction to the journal  
		if ( TRREQ.TRRSPOPT & R_APPROVED )
		{
			// ****** Future CALL  CHG_AQID*******

			// Store Transaction in journal 
			// Lock the journal semaphore  
			JournalLock();

			// Must set up globals pJrnTransBuf and JrnRevFlag              
			JournalNew( &TRREQ );


			// Release the journal semaphore  
			JournalRelease();

		}

		// Perform special processing   
		if ( ( CLBATCH == TRREQ.TRKEY ) || ( CLBATCHA == TRREQ.TRKEY )
			 || ( CLBATCHE == TRREQ.TRKEY ) )
		{
			// Clear batch pending flag depending on response option. 
			if ( !( TRREQ.TRRSPOPT & R_NOCLRPEND ) )
			{
				ClearBatchPending(  );
			}
		}
	}

	// Copy transaction back to TRINP and mark it ready 
	CopyFromReqReady(  );

	// Hang Up 
	Hang_Up_Pin();
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  UploadVisaD     Upload the batch to Visa D host.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Bool    True  - one was sent, Response in TRREQ,
//                          False - no more to send.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static Bool UploadVisaD( void )
{
	OS_RETURN_VALS CommStatus;
	UWORD Ident;
	Bool Status;

	if ( ( CLBATCH == TRREQ.TRKEY ) || ( CLBATCHA == TRREQ.TRKEY ) )
	{
		// Reset the Message Communication Flags 
		MsgComFlag = 0x00;

		// Send the request to the host  
		CommStatus = ReqVisaD(  );

		// Wait for .1 second 
		SDK_Wait( TENMS * 10 );

		// See if a Response Received and request was sent successfully. 
		if ( ( !( MsgComFlag & RESPONSE_RECEIVED ) )
			 && ( orvOK == CommStatus ) )
		{
			// Set transaction   
			TRREQ.TRKEY = CLTERM;

			// Increment TRREQ.TRSTAN
			IncTraceNum( TRREQ.TRSTAN );

			// Reset the Message Communication Flags 
			MsgComFlag = 0x00;

			// Send the request to the host  
			CommStatus = ReqVisaD(  );

			// Wait for .1 second 
			SDK_Wait( TENMS * 10 );

			// See if a Response Received and request was sent successfully. 
			if ( ( !( MsgComFlag & RESPONSE_RECEIVED ) )
				 && ( orvOK == CommStatus ) )
			{

				// Get a lock on the journal before using JSREC.    
				JournalLock();

				// Set up selection data in common record JSREC         
				SetDefaultSelection();

				// Select only this host.  
				JSREC.JSAQID = TRREQ.TRAQID;

				// No Voids in the search. 
				JSREC.JSSTATOFF = TS1_VOID;

				// JrnIdentifier will be set with first ident, if one exists.   
				Ident= JournalSelFirst();

				// While any entries in the Journal 
				while ( 0x0000 != Ident )
				{
					// Read data into TRREQ 
					JournalRead( Ident,&TRREQ );

					// Set transaction   
					TRREQ.TRKEY = RUPLOAD;

					// Increment TRREQ.TRSTAN 
					IncTraceNum( TRREQ.TRSTAN );

					// Reset the Message Communication Flags 
					MsgComFlag = 0x00;

					// Send the request to the host  
					CommStatus = ReqVisaD(  );

					// Wait for .1 second 
					SDK_Wait( TENMS * 10 );

					// See if a Response Received 
					if ( MsgComFlag & RESPONSE_RECEIVED )
					{
						break;
					}

					Ident= JournalSelNext( Ident );
				}

				// Copy the input transaction to the request buffer  
				memcpy( &TRREQ, &TRINP, TRSZE );

				// Set transaction   
				TRREQ.TRKEY = CLBATCHE;

				// Increment TRREQ.TRSTAN
				IncTraceNum( TRREQ.TRSTAN );

				// Send the request to the host  
				CommStatus = ReqVisaD(  );

				// Release the lock on the journal. 
				JournalRelease();
			}
		}

		// Wait for the response    
		RspVisaD( CommStatus );

		// Restore transaction   
		TRREQ.TRKEY = CLBATCH;

		// Status status to True 
		Status = True;
	}
	else
	{
		// Status status to false 
		Status = False;
	}

	// Return the status 
	return ( Status );
}

//-----------------------------------------------------------------------------
//  DebitConfirm    Send the debit confirmation message.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void DebitConfirm( void )
{
	OS_RETURN_VALS CommStatus;
	UBYTE trans_save;

	// If debit wasn't approved, then don't send confirmation 
	if ( TRREQ.TRRSPOPT & R_APPROVED )
	{
		// Temporarily save the original transaction key.  
		trans_save = TRREQ.TRKEY;

		// Set the transaction to debit confirm.  
		TRREQ.TRKEY = DEBITCONF;

		// Reset the Message Communication Flags 
		MsgComFlag = 0x00;

		// Send the request to the host  
		CommStatus = ReqVisaD(  );

		// Wait for .1 second 
		SDK_Wait( TENMS * 10 );

		if ( CommStatus == orvOK )
		{
			// Mark the transaction as received without error 
			TRREQ.TRDEBIT_STATUS = 0;
		}

		// Set the transaction to original.  
		TRREQ.TRKEY = trans_save;

		if ( TRREQ.TRDEBIT_NETWORKID == 'V' )
		{
			// Set the transaction to SALE.  
			TRREQ.TRKEY = SALE;

			// Clear the Cashback Amount 
			memset( TRREQ.TRCASHBK, 0,S_TRBASE );

			// Future
			// Need set in TRREQ the Card Acq and Iss pointers
			// as well as the transaction pointers
		}
	}
}
