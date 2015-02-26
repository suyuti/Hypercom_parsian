
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002, 2005
//=============================================================================
//
// Module overview:             void.c
//      Void Transaction.
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
#include "sdkgui.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "util2.h"
#include "tables.h"
#include "findtran.h"
#include "journal.h"
#include "getrninv.h"
#include "receipt.h"
#include "app1.h"
#include "flaghost.h"
#include "pinutils.h"
#include "respdsp.h"
#include "predial.h"
#include "inputpwd.h"
#include "cvtamt.h"
#include "chkbatch.h"

//=============================================================================
// Public function declarations
//=============================================================================

#include "void.h"


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static void VoidOnline( UWORD VoidOnlJrnIdent );
static void VoidOffline( UWORD VoidOfflJrnIdent );
static void VoidRspProcess( UWORD RspProcJrnIdent );
static Bool CardReadVerify( void );

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
//!     Void transaction processing.
//!
//!  \param
//!     None
//!
//!  \return
//!     Nothing.
//!
//-----------------------------------------------------------------------------
void Voidtran( void )
{
	// Reset next event upon entry 
	CurrentEvent.NEXTEVENT = 0;

    // Check if password entry required
    if ( !( TCONF.TCLOCOP & TCL_NO_VPWD ) )
	{
        // Yes, ask for password
		if ( False == InputPswd( N_NullStr ) )
			return;
	}

    // Check if Invoice Number specified
	if ( 0 == TRINP.TRJRNIDENT )
	{
        // No; Get Invoice Number
		if ( !GetTranInv ( ) )
			return; // Entry cancelled
	}

    // Void it now
	VoidIdent ( TRINP.TRJRNIDENT );

}


//-----------------------------------------------------------------------------
//!  \brief
//!     Void transaction processing with jrn identifier set.
//!
//!  \param
//!     VoidJrnIdent    Transaction Journal Identifier
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
void VoidIdent( UWORD VoidJrnIdent )
{
    static ENTRY_STR YNEntry;
	UBYTE savekey;
	static char Lbuf[21];
	UBYTE isopt3;
	int	i;
	UBYTE chRetval;

	// Lock the journal semaphore  
	JournalLock ();

	// Read the record that was found  
	JournalRead ( VoidJrnIdent, &TRINP );

	// Release the journal semaphore  
	JournalRelease ();

	// See if batch is pending 
	if ( ChkBatPend(  ) )
	{
		ShowErrMsg( VoidNotAllowed );
		return;
	}

	if ( (isopt3 = (TRINP.TRISPTR->ISOPT3)) & IS3_NOVOID )
	{
		ShowErrMsg( VoidNotAllowed );
		return;
	}

	// If an AVS Error occurred ?    
	if ( AVSERRFLG & BIT_0_SET )
	{
		// Set TS2_AVSVOID to show voiding an AVS transaction 
		TRINP.TRSTATUS[1] |= TS2_AVSVOID;

		// Save the transaction key 
		savekey = TRINP.TRKEY;
	}
	else
	{
		//  If NO AVS Error; then prompt user for input 
		memset( Lbuf, ' ', sizeof( Lbuf ) );
		MoveTillZero( Lbuf, ( char * ) TRINP.TRINV, S_TRINV );

		// use the global var to save a local variable 
		CvtAmt( &Lbuf[20 - 13], TRINP.TRTOTAM );

		Lbuf[20] = '\0';		// null terminate string 

		SecondEvent.TRNKEY = TRINP.TRKEY;

		FindTran ();
		if ( SecondEvent.TRNOPT1 & T1_CREDIT )
		{
			for ( i = 20; i != 0 ; i-- )
			{
				if ( Lbuf[i] == ' ' ) 
				{
					Lbuf[i] = '-';
					break;
				}
			}
		}

        // Check if YES was entered 
    	YNEntry.TitleMid = N_Void;
		YNEntry.Line1Mid = CustomMsg;
		YNEntry.Line2Mid = CorrectYesNo;

		memcpy( CSTMSG, Lbuf, sizeof( Lbuf ) );

		if ( ENTER_KY != YesNoEntry( &YNEntry ) )
			return;				// NO or CANCEL_KY pressed 


		// Save the transaction key while checking pinpad required. 
		savekey = TRINP.TRKEY;
		TRINP.TRKEY = VOIDTRAN;

		// Get the pin from the pinpad if required. 
		chRetval = GetPIN ();

		if ( 0 == chRetval )
		{
			// Invalid Pin or other problem 
			return;
		}
		else
		{
			if ( 2 == chRetval )
			{
				// Read the card now. 
				if ( !CardReadVerify(  ) )
				{
					// Restore the key that was saved through pinpad testing. 
					// May be returning to review. 
					TRINP.TRKEY = savekey;
					return;
				}
			}
		}

		// Restore the key that was saved through pinpad testing. 
		TRINP.TRKEY = savekey;
	}							// END of AVS VOID split 

	{
		// Check if voiding offline transaction
		if ( SecondEvent.TRNOPT2 & T2_OFFLINE )
		{
			// Yes; don't go online
			VoidOffline( VoidJrnIdent );
		}
		else
		{
			// No; check if transaction is REFDEBIT or REFUND
			if ( REFUND == TRINP.TRKEY )
			{
				// Yes; check if refunds are processed offline
				if ( ( TCONF.TCOPT2 & TC2_REF_OFF ) ||
					 ( isopt3 & IS3_REF_OFF ) )
				{
					// Yes; process refund offline
					VoidOffline( VoidJrnIdent );
				}
				else
				{
					// No; check if voids are processed offline
					if ( ( TCONF.TCOPT2 & TC2_VOID_OFF ) ||
						 ( isopt3 & IS3_VOID_OFF ) )
					{
						// Yes; process void offline
						VoidOffline( VoidJrnIdent );
					}
					else
					{
						// No; process void online
						VoidOnline( VoidJrnIdent );
					}
				}
			}
			else
			{
				// No; check if voids are processed offline
				if ( ( TCONF.TCOPT2 & TC2_VOID_OFF ) ||
					 ( isopt3 & IS3_VOID_OFF ) )
				{
					// Yes; process void offline
					VoidOffline( VoidJrnIdent );
				}
				else
				{
					// No; process void online
					VoidOnline( VoidJrnIdent );
				}
			}
		}
	}

	return;
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Void transaction ONLINE processing.
//!
//!  \param
//!     VoidOnlJrnIdent - Transaction Journal Identifier
//!
//!  \return
//!     Nothing.
//!
//!  \note
//!     Journal is already locked when this function is called.
//!
//-----------------------------------------------------------------------------
static void VoidOnline( UWORD VoidOnlJrnIdent )
{

	/* Perform a Pre-Dial if allowed */
	PreDial ();

	TRINP.TRSTATUS[0] &= ~TS1_ADJ;
	TRINP.TRSTATUS[0] |= TS1_VOID;


	// If original amount field is empty, copy total amount into it 
	if ( NullComp( ( char * ) TRINP.TRORIGAMT, S_TRBASE ) )
		memcpy( TRINP.TRORIGAMT, TRINP.TRTOTAM, S_TRBASE );

	TRINP.TRORIGKEY = TRINP.TRKEY;

	TRINP.TRKEY = VOIDTRAN;

	// Set flags to print the receipt header and not allow user cancel. 
	FlagHostCntl = FLAG_PREPRINT;

	// Flag the host process  
	FlagHost ();

	// Save host response text in terminal flag table.  
	SaveResponseText ();

	// Display Response 
	RespDisplay ();

	// Only print the receipt if the transaction was approved  
	if ( TRINP.TRRSPOPT & R_APPROVED )
	{
		// Lock the journal semaphore  
		JournalLock ();

		JournalRead ( VoidOnlJrnIdent, &TRINP );

		// Release the journal semaphore  
		JournalRelease ();

		// Set VOID bit 
		TRINP.TRSTATUS[0] &= ~TS1_ADJ;
		TRINP.TRSTATUS[0] |= TS1_VOID;

		VoidRspProcess( VoidOnlJrnIdent );
	}
 	else
 	{
 		/* Finish receipt printing, if preprint is enabled */
		if ( AQ4_PREPRINT & (TRINP.TRAQPTR->AQOPT4) )
		{
 			Receipt ();
		}

		/* Display host response */
		RespDisplay ();
	}

}


//-----------------------------------------------------------------------------
//!  \brief
//!     Void transaction OFFLINE processing.
//!
//!  \param
//!     VoidOfflJrnIdent    Transaction Journal Identifier
//!
//!  \return
//!     None
//!
//!  \note
//!     Journal is already locked when this function is called.
//!
//-----------------------------------------------------------------------------
static void VoidOffline( UWORD VoidOfflJrnIdent )
{
	// Lock the journal semaphore  
	JournalLock ();

	// re-read the record that was found  
	JournalRead ( VoidOfflJrnIdent, &TRINP );

	// Release the journal semaphore  
	JournalRelease ();

	if ( TS1_ADV & TRINP.TRSTATUS[0] )
	{
		/* Offline Repeat is being voided - the host knows nothing about this txn.
		   Remove the TS1_ADV bit to prevent from sending to the host */
		/* Set the VOID bit first */
		TRINP.TRSTATUS[0] |= TS1_VOID;

		/* Now clear the advice bit  */
		TRINP.TRSTATUS[0] &= ~TS1_ADV;
	}
	else
	{
		/* Set the VOID & Adjust bits */
		TRINP.TRSTATUS[0] |= (TS1_VOID+TS1_ADJ);
	}

	VoidRspProcess( VoidOfflJrnIdent );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Void response processing - update the journal entry.
//!
//!  \param
//!     RspProcJrnIdent     Transaction Journal Identifier
//!
//!  \return
//!     None
//!
//!  \note
//!     Journal is already locked when this function is called.
//!
//-----------------------------------------------------------------------------
static void VoidRspProcess( UWORD RspProcJrnIdent )
{
	// If original amount field is empty, copy total amount into it 
	if ( NullComp( ( char * ) TRINP.TRORIGAMT, S_TRBASE ) )
	{
        memcpy( TRINP.TRORIGAMT, TRINP.TRTOTAM, S_TRBASE );
    }

	// Lock the journal semaphore  
	JournalLock ();

	// Update journal record
	RspProcJrnIdent = JournalUpdate ( &TRINP ,RspProcJrnIdent );

	// Release the journal semaphore  
	JournalRelease ();

	//  If AVS, do not print receipt here. 
	if ( ( AVSERRFLG & BIT_0_SET ) &&	// If AVS ERROR? 
		 ( AVSERRFLG & ~BIT_1_SET ) )	// If AVS TRANSACTION NOT CAPTURED? 
	{
		// Return to application input side and print remaining receipt 
		return;
	}

    // Print receipt
	Receipt ();

	// Display Transaction Accepted Prompt on Top Line 
	ShowInfoMsg( TransAccepted, N_Response );

}


//-----------------------------------------------------------------------------
//!  \brief
//!     Read card and verify same as TRTRK2.
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - data matched,
//!                 False - otherwise.
//!
//-----------------------------------------------------------------------------
static Bool CardReadVerify( void )
{
	int offset;
	UBYTE loopflag;
	Bool retval;
	UBYTE i;
	UBYTE InputType;
	Bool bPinCardStatus;

	retval = False;
	loopflag = True;

	do
	{
		// Display Swipe Card Message on text line 2. 
		User_Msgs[1] = Swipecard;
		DrawScreen( CARDSWIPE_SCR );

		// Read from card reader to get acc. number.                
		// User can cancel from pad or keybd or can be timed out.   
		InputType = read_io( DEV_PAD + DEV_KBD + DEV_CARDRDR, UserTimeout, &bPinCardStatus, &DevFlag );

		switch ( InputType )
		{
			case DEV_CARDRDR:
				if ( 0 != Trk2Status.CompleteFlag )
				{
					// Return if timed out. 
					if ( orvDEVICETIMEOUT == Trk2Status.Error )
					{
						SDK_Beeper( TENMS * 40 );	// Notify the user 
						return ( retval );
					}

					// Check if Track 2 data has been received 
					if ( orvOK == Trk2Status.Error )
					{
						// Yes; good read, format trk 2 Card Data 
						for ( i = 0; i < Trk2ReadLength; i++ )
						{
							// Convert to ASCII 
							TRINP.TRTRK2[i] += 0x30;
						}

						memset( &TRINP.TRTRK2[Trk2ReadLength], 0xFF,
							  ( UWORD ) ( S_TRTRK2 - Trk2ReadLength ) );
						loopflag = False;
					}
					else
					{
						ShowErrMsg( CardErr );
					}
				}
				break;

			case DEV_PAD:
				// Exit the program on any touch of the pad or T/O. 
				SDK_Beeper( TENMS * 40 );	// Notify the user 
				return ( retval );

			case DEV_KBD:
				// Return if timed out. 
				if ( orvDEVICETIMEOUT == KeyStatus.Error )
				{
					SDK_Beeper( TENMS * 40 );	// Notify the user 
					return ( retval );
				}
				if ( CLEAR_KY == KeyVal || CANCEL_KY == KeyVal )
				{
					// Yes; exit the loop 
					loopflag = False;
					return ( retval );
				}
				else
				{
					// Keep looping if button not CLEAR. 
					SDK_Beeper( TENMS * 40 );	// Notify the user 
				}
		}
	}
	while ( loopflag );

	// Check if Track 2 data has been received 
	if ( ( Trk2Status.Error == orvOK ) && Trk2Status.CompleteFlag )
	{
		// Yes; good read 
		memset( ( UBYTE * ) Dspbuf, 0,sizeof( Dspbuf ) );

		// Move track 2 data read to Dspbuf for memcmpe.    
		// Account data ends with a 0x3d, an equal sign 
		offset = 0;
		while ( ( TRINP.TRTRK2[offset] != 0x3d ) &&
				( offset < sizeof( Dspbuf ) ) )
		{
			Dspbuf[offset] = TRINP.TRTRK2[offset];
			offset++;
		}

		// Convert PAN to ascii for memcmpe. 
		MovPanAsc( CSTMSG, TRINP.TRPAN, ( UBYTE ) ( S_TRPAN * 2 ) );

		// memcmpe read data to TRINP TRTRK2 data.  
		if ( memcmp
			 ( ( UBYTE * ) Dspbuf, ( UBYTE * ) CSTMSG,
			   ( UWORD ) StrLn( Dspbuf, sizeof( Dspbuf ) ) ) )
		{
			// Track 2 data just read is different. 
			// Display CARD # DOESN'T MATCH message and exit. 
			ShowErrMsg( CardNoMatch );
		}
		else
		{
			// Track 2 data just read is same as in TRINP. 
			retval = True;
		}
	}
	return ( retval );
}

