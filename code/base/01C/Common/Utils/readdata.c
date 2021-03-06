
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             readdata.c
//      read card data
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
#include "util2.h"
#include "pinpad.h"
#include "hypertcf.h"

#include "readdata.h"

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
static const ENTRY_STR CardNumberEntry = {
	N_NullStr,					// Issuer Tran
	EnterCard,					// Prompt
	N_NullStr,					// Default passed in INJECTKEY
};


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//!  \brief     Read Card Data
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     On input:   Mid    = message ID of prompt
//!     On output:  False if canceled, timed out,
//!                 or invalid input
//-----------------------------------------------------------------------------

extern Bool ReadCardData( enum msg_id Mid )
{
	UBYTE loopflag, i;
	UWORD device_map;
	Bool bRetval, bPinCardStatus;
	UBYTE InputType;

#ifdef	MAKE_EMV
	// initialise, then check
	ChipCardInserted = False;
#endif	// MAKE_EMV

	loopflag = True;
	bPinCardStatus = False;

	do
	{
		// Display Swipe Card Message on text line 2
		// If PINPad enabled, substitute "SWIPE CARD ON PINPAD" for "SWIPE CUSTOMER CARD"
		if ( ( Swipecard == Mid ) && ( PINSTAT & PinCard_Read ) )
		{
			Mid = SwipeCardPin;
		}
		else
		{
			Mid = Swipecard;
		}

#ifdef	MAKE_EMV
		if ( TS2_FALLBACK & TRINP.TRSTATUS[1] )
		{
			Mid = Swipecard;   
		}
		else
		{
 			Mid = SwipeInsert; 
		}
#endif	// MAKE_EMV

		if ( MAIL_ORDER == TRINP.TRKEY )
		{
			Mid = EnterCard;
		}

		User_Msgs[1] = Mid;

		DrawScreen( STATUS_SCR );

		// Read from pad, keyboard and either terminal card reader
		// or pinpad card reader to get the account number.
		device_map = DEV_PAD | DEV_KBD | STAY_OPEN0;

#ifdef	MAKE_EMV
		if ( !( TS2_FALLBACK & TRINP.TRSTATUS[1] ) )	// if not fallback
		{
			device_map |= DEV_SCR;
		}
#endif	// MAKE_EMV

		if ( PINSTAT & PinCard_Read )
			device_map |= DEV_PINCARD;
		else
			device_map |= DEV_CARDRDR;

		InputType = read_io( device_map, UserTimeout, &bPinCardStatus, &DevFlag );

		switch ( InputType )
		{
#ifdef	MAKE_EMV
			case DEV_SCR:
				// we have a winner
				ChipCardInserted = True;
				loopflag = False;
				break;
#endif	// MAKE_EMV

			case DEV_CARDRDR:
				if ( 0 != Trk2Status.CompleteFlag )
				{
					// Return if timed out.
					if ( orvDEVICETIMEOUT == Trk2Status.Error )
					{
						SDK_Beeper( TENMS * 40 );	//Notify the user
						bRetval = False;
						return bRetval;
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

						if ( 0 == BuildPAN_EXP () )
						{
							ShowErrMsg( CardErr );
							// Be sure TRK1 and TRK2 data are cleared of any trash data.
							memset( TRINP.TRTRK1, 0, S_TRTRK1 );
							memset( TRINP.TRTRK2, 0, S_TRTRK2 );
							break;
						}

						loopflag = False;
						// Check if Track 1 data has completed and has been received
						//  without error since Track 2 data is OK.
						if ( ( Trk1Status.CompleteFlag ) &&
							 ( orvOK == Trk1Status.Error ) )
						{
							// Yes; good read, format trk 1 Card Data
							for ( i = 0; i < Trk1ReadLength; i++ )
							{
								// Convert to ASCII
								TRINP.TRTRK1[i] += 0x20;
							}

							memset( &TRINP.TRTRK1[Trk1ReadLength], 0xFF,
								  ( UWORD ) ( S_TRTRK1 - Trk1ReadLength ) );
						}
						else
						{
							// Be sure TRK1 data is cleared of any trash data.
							// This field will be tested later if issurer option
							// indicates TRK1 is required.
							memset( TRINP.TRTRK1, 0, S_TRTRK1 );
						}
					}
					else
					{
						ShowErrMsg( CardErr );
						// Be sure TRK1 and TRK2 data are cleared of any trash data.
						// This field will be tested later if issurer option indicates
						// TRK1 is required.
						memset( TRINP.TRTRK1, 0, S_TRTRK1 );
						memset( TRINP.TRTRK2, 0, S_TRTRK2 );
					}
				}
				break;

			case DEV_PINCARD:	// Card swiped on the pinpad
				if ( bPinCardStatus )
				{
					if ( 0 == BuildPAN_EXP () )
					{
						ShowErrMsg( CardErr );
						// Be sure TRK1 and TRK2 data are cleared of any trash data.
						memset( TRINP.TRTRK1, 0, S_TRTRK1 );
						memset( TRINP.TRTRK2, 0, S_TRTRK2 );
						break;
					}

					loopflag = False;
				}
				else
				{
					// Display ** CARD ERROR ** on the pinpad
					PinMessage ( Msg_PinCardErr );
				}
				break;

			case DEV_KBD:
				// Return if timed out
				if ( orvDEVICETIMEOUT == KeyStatus.Error )
				{
					SDK_Beeper( TENMS * 40 );	// Notify the user
					bRetval = False;
					return bRetval;
				}

				if ( ( KeyVal >= '0' ) && ( KeyVal <= '9' ) )
				{
					// Pass in injected Key
					INJECTKEY = KeyVal;

					// Set Manual entry mode and clear loop flag.
					TRINP.TRPOSE[1] = 0x12;
					loopflag = False;
					break;
				}
				else
				{
					// Return on clear or enter when expecting digits
					if ( KeyVal != CLEAR_KY )
					{
						SDK_Beeper( TENMS * 40 );	// Notify the user
					}
					bRetval = False;
					return bRetval;
				}

				// No keyboard for ICE4000, all input from PAD
			case DEV_PAD:
				if ( orvDEVICETIMEOUT == PadStatus.Error )
				{
					SDK_Beeper( TENMS * 40 );	// Notify the user
					bRetval = False;
					return bRetval;
				}

				if ( CANCEL_KY == ButtonId )
				{
					SDK_Beeper( TENMS * 40 );	// Notify the user
					bRetval = False;
				}
				return bRetval;

			default:
				// Exit the program on time out
				SDK_Beeper( TENMS * 40 );	//Notify the user
				bRetval = False;
				return bRetval;
		}
	}
	while ( loopflag );

	// If we got here because a number key was pressed,
	// get the rest of the account number from the keypad
	// and store it in Dspbuf.
	if ( INJECTKEY )
	{
		if ( !NumEntry( INJECTKEY, 19, 6, &CardNumberEntry ) )
		{
			CurrentEvent.NEXTEVENT = 0;	// Back to idle
			bRetval = False;
			return bRetval;
		}
	}

	bRetval = True;
	return bRetval;
}

//=============================================================================
// Private function definitions
//=============================================================================
