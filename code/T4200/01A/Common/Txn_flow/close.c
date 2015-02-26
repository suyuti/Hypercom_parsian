
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             close.c
//      Routines for closing the batch
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "tables.h"
#include "chkprinter.h"
#include "findtran.h"
#include "totals.h"
#include "journal.h"
#include "hstprint.h"
#include "flaghost.h"
#include "respdsp.h"
#include "comm.h"
#include "hstprint2.h"
#include "hstmsg.h"

#include "close.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static void CloseAcq( void );
static char BatchSingle( void );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static char mysoc[S_TRSOC];
static UBYTE bat_typ;
static UBYTE acqid;
static UBYTE trinpaqid;
static struct acquirer_rec *pAqptr;	// Pointer to Acquirer entry
static UBYTE trinpkey;
static struct batch_rec *pBatch_Ptr;	// Pointer to BATTAB entry
static enum transactions origtrkey;
static UBYTE tmpAQPNAME[S_AQPNAME];


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Close Term
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void CloseTerm( void )
{
	struct acquirer_rec *pacquirer;	// Pointer to AQTAB entry

	origtrkey = TRINP.TRKEY;	// Save original TRKEY

	if ( True )
	{
		// Make a local copy of some information.
		memcpy( mysoc, TRINP.TRSOC, S_TRSOC );
		bat_typ = TRINP.TRBATTYP;
		pAqptr = TRINP.TRAQPTR;
		trinpkey = TRINP.TRKEY;
		acqid = trinpaqid = TRINP.TRAQID;

		if ( trinpaqid )		// settle a single acquirer
		{
			// Settling one acquirer.
			CloseAcq(  );
		}
		else					// settle multiple acquirers.
		{
			// Settling all acquirers.
			pacquirer = FindFirstAcquirer(  );

			while ( NULL != pacquirer )
			{
				// Settle the found acquirer. Indicate terminal settlement.
				trinpkey = SETTLE;
				acqid = pacquirer -> AQID;
				pAqptr = pacquirer;

				// Settle this acquirer.
				CloseAcq(  );

				// Settle next acquirer if it exists
				pacquirer = FindNextAcquirer( pacquirer );

				// Don't leave SCANNING BATCH message on the display
				// if there is not another acquirer.  If another
				// acquirer then make response stay visible longer.
				if ( NULL == pacquirer )
				{
					DelayIdleDisplay = False;
				}
				else
				{
					SDK_Wait( 1 * ONESECOND );
				}
			}
		}
	}
}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Close Acq
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void CloseAcq( void )
{
	UBYTE RtcBuffer[RTCLEN];	// RTC Read Buffer
	Bool flag_closeacq;			// local flag for settle an acquirer

	// Close the selected acquirer. Rebuild TRINP.
	memset( ( UBYTE * ) & TRINP, 0, TRSZE );
	TRINP.TRAQPTR = pAqptr;
	TRINP.TRAQID = acqid;

	pBatch_Ptr = FindBatTab( acqid );
	flag_closeacq = False;	// close financial host, try again later

	// Read the real time clock.
	SDK_RtcRead( RtcBuffer );

	if ( True )			// not settle sigcap settlement?
	{
		CalcTotals ();
		StdCalcTotals ();

		// If there are no void sales or void refunds and no sales
		// or refunds then this settle attempt is done.
		if ( ( NullComp
			   ( ( char * ) TERMTOTALS.TOTBASIC.TOTDBVCNT, S_TOTDBVCNT ) ) &&
			 ( NullComp
			   ( ( char * ) TERMTOTALS.TOTBASIC.TOTCRVCNT, S_TOTCRVCNT ) ) &&
			 ( NullComp
			   ( ( char * ) TERMTOTALS.TOTBASIC.TOTDBCNT, S_TOTDBCNT ) ) &&
			 ( NullComp
			   ( ( char * ) TERMTOTALS.TOTBASIC.TOTCRCNT, S_TOTCRCNT ) ) )
		{
			// Nothing to settle.  If this was autosettle attempt clean up.
			if ( CLBATCHA == trinpkey )
			{
				// Don't update batch table if in training mode.
				if ( !( TCONF.TCOPT4 & TC4_TRAINING ) )
				{
					// Clear settle attempts counter.
					pBatch_Ptr->BATSETLTRY = 0;

					// Clear idle timer count.
					pBatch_Ptr->BATSETLIDL = 0;

					// Set auto initiation.
					pBatch_Ptr->BATSETLINI = trinpkey;

					// Save terminal totals in battab.
					memcpy( &pBatch_Ptr->BATSETLTOT, &TERMTOTALS,
							TOTBASESZE );

					// Move date and time to battab.
					AscHex( pBatch_Ptr->BATSETLDAT, ( char * ) RtcBuffer,
							S_BATSETLDAT + S_BATSETLTIM );

					// Recalculate the LRC on the batch table
					SetBATTabLRC(  );
				}
			}
		}
		else
		{
			flag_closeacq = True;
		}
	}

	if ( flag_closeacq )		// do we need to close this acquirer?
	{
		// There are transactions to settle. Move date and time to battab.
		AscHex( pBatch_Ptr->BATATTMDAT, ( char * ) RtcBuffer,
				S_BATATTMDAT + S_BATATTMTIM );

		// Increment batch table settle attempts counter.
		IncBcdLen( ( UBYTE * ) & pBatch_Ptr->BATSETLTRY, 1 );

		// Set auto initiation.
		pBatch_Ptr->BATSETLINI = trinpkey;

		// Recalculate the LRC on the batch table
		SetBATTabLRC(  );

		// Copy sales and refund totals to TRINP.
		memcpy( TRINP.TRSTOT, TERMTOTALS.TOTBASIC.TOTDBAMT, S_TRSTOT );
		memcpy( TRINP.TRCTOT, TERMTOTALS.TOTBASIC.TOTCRAMT, S_TRCTOT );

		// Set transaction key.
		TRINP.TRKEY = CLBATCH;

		// Force current event to CLBATCH.
		SecondEvent.TRNKEY = CLBATCH;
		FindTran ();
		memcpy( &CurrentEvent, &SecondEvent, sizeof( struct event_entry ) );

		// Restore the batch type.
		TRINP.TRBATTYP = bat_typ;

		// Restore the SOC.
		memcpy( TRINP.TRSOC, mysoc, S_TRSOC );

		// Move current batch number to TRINP.
		BfAscii( ( char * ) TRINP.TRBATNO, pBatch_Ptr->BATCUR, S_BATCUR );

		// Move close batch message to TRINP and also display it.
		memset( ( char * ) TRINP.TRTRANBUF, ' ', S_TRTRANBUF );
		GetMsg( SettleTran, CSTMSG );
		MoveTillZero( ( char * ) &TRINP.TRTRANBUF[S_TRTRANBUF - StrLn( CSTMSG, sizeof( CSTMSG ) )], 
					CSTMSG, sizeof( CSTMSG ) );

		if ( BatchSingle(  ) )
		{
			// If not approved set retry timer to 5 minutes..
			if ( !( TRINP.TRRSPOPT & R_APPROVED ) )
			{
				// Don't update batch table if in training mode.
				if ( !( TCONF.TCOPT4 & TC4_TRAINING ) )
				{
					pBatch_Ptr->BATSETLIDL = 5;
					memcpy( pBatch_Ptr->BATSETLTEXT, TRINP.TRHOSTTEXT, S_BATSETLTEXT );
					memcpy( pBatch_Ptr->BATSETLRSPC, TRINP.TRRSPC, S_BATSETLRSPC );

					// Recalculate the LRC on the batch table
					SetBATTabLRC(  );
				}

				return;
			}
			else
			{
				// Approved - save terminal totals in battab.
				memcpy( &pBatch_Ptr->BATSETLTOT, &TERMTOTALS, TOTBASESZE );

				// Move current batch number to settle in battab.
				memcpy( pBatch_Ptr->BATSETLNO, pBatch_Ptr->BATCUR, S_BATSETLNO );

				// Move host response text and code to battab.
				memcpy( pBatch_Ptr->BATSETLTEXT, TRINP.TRHOSTTEXT, S_BATSETLTEXT );
				memcpy( pBatch_Ptr->BATSETLRSPC, TRINP.TRRSPC, S_BATSETLRSPC );

				// Clear the settle attempts counter.
				pBatch_Ptr->BATSETLTRY = 0;
				pBatch_Ptr->BATSETLIDL = 0;

				// Set settled date and time in battab.
				SDK_RtcRead( RtcBuffer );
				AscHex( pBatch_Ptr->BATSETLDAT, ( char * ) RtcBuffer,
						S_BATSETLDAT + S_BATSETLTIM );

				// Indicate host settlement done for this acquirer.
				// Verify acquirer id exists.
				if ( TRINP.TRAQID )
				{
					pBatch_Ptr->BATFLAG |= BT_SETL;

					// Set the signature captured settled bit also if
					// in training mode.  Sig cap is not done for training
					// but must set BT_SIGSETL or will get retry batch after
					// exit from training mode.
					if ( TCONF.TCOPT4 & TC4_TRAINING )
						pBatch_Ptr->BATFLAG |= BT_SIGSETL;
				}

				// Recalculate the LRC on the batch table
				SetBATTabLRC(  );

				// Acquire a lock on the Journal
				JournalLock ();

				// Set up default selection data in common record JSREC
				// Acquired ID is all that JournalSetSettled will need.
				SetDefaultSelection ();
				JSREC.JSSTATOFF = 0;
				JSREC.JSAQID = acqid;

				// Tag all settle records in the journal.
				JournalSetSettle ();

				// Release Journal
				JournalRelease ();
			}
		}
	}
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Batch Single
//!
//!  \param
//!     None
//!
//!  \return
//!     char       True
//!
//-----------------------------------------------------------------------------
static char BatchSingle( void )
{
	MoveItAq( tmpAQPNAME, TRINP.TRAQPTR->AQPNAME, S_AQPNAME );	// Move acquirer host name into tmp buffer

    // Set flags to not print the receipt header and not allow user cancel.
	FlagHostCntl = 0;

	// Notify host packet is ready and wait for response.
	FlagHost ();

	RespDisplay(  );

	// If not approved or not clear batch transaction then return now.
	if ( !( TRINP.TRRSPOPT & R_APPROVED ) || ( CLBATCH != TRINP.TRKEY ) )
	{
		return ( True );
	}

	// Hang up the line.
	Hang_Up ();

	// Print the settle and issuer receipt if the printer is available.
	if ( ChkPrinter( ErrNotEnabled | ErrNotReady ) )
	{
		SettleReceipt ();
		IssuerReceipt ();
	}
	// If business day processing print host settle receipt.
	if ( !memcmp( TRINP.TRRSPC, ( UBYTE * ) RSP_BUS, S_TRRSPC ) )
	{
		// Hosttl_Receipt (); -- To be added 
		// when a host that sends totals to terminal is required

		memcpy( TRINP.TRRSPC, RSP_APP, S_TRRSPC );

		RspLookUp( &TRINP );
	}

	// If batch balances print the batch receipt.
	if ( !memcmp( TRINP.TRRSPC, ( UBYTE * ) RSP_BAT, S_TRRSPC ) )
	{
		// Indicate to clear batch at host and print batch receipt.
		TRINP.TRBATTYP = 2;

		// BatchReceipt (); -- To be added
		// when a host that sends totals to terminal is required
	}

	// Flag acquirer data as cleared plus other TF flag information.
	TFTAB.TFLAQID = TRINP.TRAQID;
	TFTAB.TFKEY = TRINP.TRKEY;
	memcpy( TFTAB.TFYEAR, TRINP.TRYEAR, S_TFYEAR );
	memcpy( TFTAB.TFHOSTTEXT, TRINP.TRHOSTTEXT, S_TRHOSTTEXT );

	// Set LRC on modified tables.
	SetAQTabLRC(  );
	SetBATTabLRC(  );
	SetTFTabLRC(  );

	DefaultTitle[0] = 0;		// Use transaction as title
	RespDisplay(  );

	DelayIdleDisplay = True;

	return ( True );
}
