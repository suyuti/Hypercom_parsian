
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             hyper.c
//      Hyper host module
//
//=============================================================================

#include "string.h"

#include "i8583utl.h"
#include "bit8583.h"
#include "findtran.h"
#include "comm.h"
#include "refproc.h"
#include "batpend.h"
#include "hstjs.h"
#include "hstmsg.h"
#include "reqhyp.h"
#include "rsphyp.h"
#include "hstcopy.h"
#include "tables.h"

#include "hyper.h"
#include "autoinit.h"
#include "comdata.h"
#include "struct.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static Bool Rev( void );
static Bool EMVInfAdv( void );
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
//  Hyper           Entry point for Hyper host processing.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void Hyper( void )
{
	OS_RETURN_VALS CommStatus;
	UBYTE offset, idx;

        offset = 0;
        idx = 0;

	// Copy the input transaction to the request buffer
	memcpy( &TRREQ, &TRINP, TRSZE );

	Move_Table = ( struct move_rec * ) &Move_Tab[0];	// Set Hyper Bit table


	
	// See if this should be processed offline
	if ( !EMVInfAdv() )
	{
                // floor limit kontrolüne gerek yok 
		if ( 1 )
		{
			// No; process any outstanding Reversals
			if ( Rev(  ) )
			{
				// Okay; process offline/adjust transfer
				if ( !Xfer(  ) )
				{
					// Error; mark transaction as ready and move data 
					// back to input record
					CopyFromReqReady(  );

					// Hang Up
					Hang_Up_Pin ();

					// Exit function
					return;
				}

                // Set batch pending if required
				SetBatchPending(  );

				// Increment 'STAN' and move System Trace Number to 
				// 'TRREQ'
				IncTraceNum( TRREQ.TRSTAN );

				
				
				// Set the reversal bit on
				TRREQ.TRSTATUS[0] |= TS1_REV;

                // Add Reversal to the journal
				JrnHostIdent = JournalNew ( &TRREQ );

				// Clear the reversal bit
				TRREQ.TRSTATUS[0] &= ~TS1_REV;

				// Reset the Message Communication Flags
				MsgComFlag = 0x00;

				// Send the request to the host
				CommStatus = ReqHyper(  );

				// Wait for the response
				RspHyper( CommStatus );
				

				// Test response options to see if reversal is cleared.
				// Free the reversal if response option is set to clear.
				if ( !( TRREQ.TRRSPOPT & R_NOCLRREV ) )
				{
					// Free the reversal if response option is set to clear.
					if ( JrnHostIdent != 0 )
					{
						// Free the reversal - get a lock first.
						JournalLock ();

						JournalFree ( JrnHostIdent );

						// Release the journal semaphore
						JournalRelease ();
					}
				}
			}					// End of Rev
		}						// End of Floor
	}							// End of Offline

    if (1)         
    {

	// Add approved transaction to the journal
	if ( TRREQ.TRRSPOPT & R_APPROVED )
	{				 
        // ICC txns processed in emvproc.
        if ( 0x50 != (TRREQ.TRPOSE[1] & 0xF0) )
        {
            // Add transaction to the journal
            TRREQ.TRJRNIDENT = JournalNew ( &TRREQ );
        }
    }
   }

	// Perform special processing
	while ( ( CLBATCH == TRREQ.TRKEY ) || ( CLBATCHA == TRREQ.TRKEY )
			|| ( CLBATCHE == TRREQ.TRKEY ) )
	{
		// Clear batch pending flag depending on response option.
		if ( !( TRREQ.TRRSPOPT & R_NOCLRPEND ) )
		{
			ClearBatchPending(  );
		}

		// Process upload per host response.
		if ( TRREQ.TRRSPOPT & R_UPLOAD )
		{
			// Move data back to input record for response processing
			memcpy( &TRINP, &TRREQ, TRSZE );

			// Transfer host response text to custom message
			memcpy( CSTMSG, TRINP.TRRSPTXT, S_TRRSPTXT );

			// Null terminate the custom message
			CSTMSG[S_TRRSPTXT] = '\0';

			// Display host response text
			ShowInfoMsg( CustomMsg, N_Comms );

			Upload(  );
		}
		else
		{
			// Not upload request - Send stats if approved.
			// Or just break out if not approved.
			if ( TRREQ.TRRSPOPT & R_APPROVED )
			{
				// Send statistics to the host.
                                ReadyInitialize();
                                revTryCount = 0;

                                // Eklendi 
                                Schedule();
                                
                                // TODO !!!!
                                // Host logon trxn destekleyince
                                // Yorumu kaldýrýlacak 
                                //CurrentEvent.NEXTEVENT = LOGON;                               
			}

			break;
		}
	}


	// Move data back to input record
	memcpy( &TRINP, &TRREQ, TRSZE );
	
	// Trickle feed Transactions to Host
	Trickle(  );

    // Mark the Transaction as ready in TRINP.
	TRRSPREADY |= BIT_1_SET;

	// Hang Up
	
        Hang_Up_Pin ();
}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  Rev             Make sure the Reversal is sent to Host first
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - processed okay
//                              False - processing error.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool Rev( void )
{
	Bool Status;
	OS_RETURN_VALS CommStatus;

	// Set status to true
	Status = True;

	// Get the Transaction from the Request 
	HostEvent.TRNKEY = TRREQ.TRKEY;

	// Locate the Transaction - will be in HostEvent 
	HostFindTran ();
        
	// See if we send Reversals before this Transaction
	if ( !( HostEvent.TRNOPT2 & T2_NOPREREV ) )
	{       
                
                // reversal problemi çözümü 
                // reversal gönderilmez
                if ( TCAP.TCPOPP2 & TCP_DONTSENDREVERSAL ) 
                {                    
                    TCAP.TCPREVTRYCOUNT = 0;
                    SetTCAPTabLRC();
                    return Status;
                }
                    
                if ( TCAP.TCPOPP2 & TCP_LIMITTRYREVERSAL ) 
                {
                    if ( revTryCount >= TCAP.TCPREVTRYCOUNT )
                      return Status;
                    
                    revTryCount += 1;
                }

		// Yes; get control of the Journal
		JournalLock ();

		// Set up selection record
		DefHostJSRec(  );

		// Look for a Reversal that is not an Advice or Settled
		JSREC.JSSTATON = TS1_REV;
		JSREC.JSSTATOFF = TS1_ADV + TS1_SETL;

		// See if a Reversal in the Journal
		if ( HostJSFirst(  ) )
		{
			// Get the data
			HostJRead(  );

			// Release the Journal
			JournalRelease ();

			// Calculate total + reauth margin.
			CalcReauth( TRREQ.TRTOTAMI, TRREQ.TRTOTAM );

			// Reset the Message Communication Flags
			MsgComFlag = 0x00;


			// Send the Request message
			CommStatus = ReqHyper(  );

			// Clear Additional Response Data so we can look at
			// Response
			memset( TRREQ.TRRSPA, 0, S_TRRSPA );

			// Wait for a Response
			RspHyper( CommStatus );

			// See if Approved
			if ( TRREQ.TRRSPOPT & R_APPROVED )
			{

				// Yes; get control of the Journal
				JournalLock ();

				// Delete from the Journal
				JournalFree ( JrnHostIdent );

				// Release the Journal
				JournalRelease ();

				// Copy original Transaction to Request buffer
				memcpy( &TRREQ, &TRINP, TRSZE );

                                // reversal baþarý ile gönderildi 
                                revTryCount = 0;

			}
			else
			{
				// Copy original Transaction to Request buffer
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

				// Status status to false
				Status = False;
			}
			
		}
		else
		{
			// No; release the Journal
			JournalRelease ();
		}
	}
	// Return the status
	return ( Status );
}

//-----------------------------------------------------------------------------
//  EMVInfAdv       Process Informative EMV Advice
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - is processed
//                              False - is not processed
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool EMVInfAdv( void )
{
        return False;
}
