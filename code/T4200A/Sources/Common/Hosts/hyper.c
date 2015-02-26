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
#include "events.h"

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
//!     Entry point for Hyper host processing
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void Hyper( void )
{
    OS_RETURN_VALS CommStatus;

    // Copy the input transaction to the request buffer
    memcpy( &TRREQ, &TRINP, TRSZE );

    Move_Table = ( struct move_rec * ) &Move_Tab[0];    // Set Hyper Bit table


    // Set point of service condition code
    if ( OPENTAB == TRREQ.TRKEY )
    {
        TRREQ.TRPOCC = 0x51;
    }
    else if ( MAIL_ORDER == TRREQ.TRKEY )
    {
        TRREQ.TRPOCC = 0x08;
    }

    // See if this should be processed offline
    if ( !Offline(  ) )
    {
        // No; see if floor limit approved
        if ( !Floor(  ) )
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
                memcpy(TRREQ.TRLTSTAN, TRREQ.TRSTAN, S_TRSTAN);

                // Calculate total + reauth margin.
                CalcReauth( TRREQ.TRTOTAMI, TRREQ.TRTOTAM );

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

                if ((TRREQ.TRSTATUS[0] & TS1_REV)  && (TRREQ.TRRSPOPT & R_REVAPPROVED)) {
                    TRREQ.TRRSPOPT |= R_APPROVED;
                }

                // Check the response host indicators
                if ( 0x00 != TRRSP.TRPROC[2] )
                {
                    // See if automatic initialization bit set
                    if ( TRREQ.TRPROC[2] & BIT_2_SET )
                    {
                        // Set automatic initliaze flag
                        AutoInit = True;
                    }
                }

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
            }                   // End of Rev
        }                       // End of Floor
    }                           // End of Offline

    // Perform Referral processing for non ICC txns.
    //(ICC txns processed in emvproc).
    if ( 0x50 != (TRREQ.TRPOSE[1] & 0xF0) )
    {
        // Not ICC Transaction, process Voice Referral
        Referral( &TRREQ );
    }

    // Add approved transaction to the journal
    if (TRREQ.TRRSPOPT & R_APPROVED) 
    {                
        // ICC txns processed in emvproc.
        if ( 0x50 != (TRREQ.TRPOSE[1] & 0xF0) )
        {
            // Add transaction to the journal
            TRREQ.TRJRNIDENT = JournalNew ( &TRREQ );
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
#if 0
                // Send statistics to the host.
                //  IMPLEMENT STATISTICS IN THE FUTURE
                memcpy( &TRINP, &TRREQ, TRSZE );
                SendStatistics(  );
                memcpy( &TRREQ, &TRINP, TRSZE );
#endif
            }

            break;
        }
    }

    // Move data back to input record
    memcpy( &TRINP, &TRREQ, TRSZE );

    // Trickle feed Transactions to Host
    Trickle();

    // Mark the Transaction as ready in TRINP.
    TRRSPREADY |= BIT_1_SET;
    SDK_SetEvents( EVENT_HOSTRSP );

    // Hang Up
    Hang_Up_Pin ();
}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Makes sure the Reversal is sent to Host first
//!
//! \return
//!     True on success else False
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
            if ((TRREQ.TRSTATUS[0] & TS1_REV)  && (TRREQ.TRRSPOPT & R_REVAPPROVED)) {
                TRREQ.TRRSPOPT |= R_APPROVED;
            }
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
            }
            else
            {
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

