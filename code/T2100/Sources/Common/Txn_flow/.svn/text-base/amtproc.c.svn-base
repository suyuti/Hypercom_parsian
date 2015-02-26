
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     amtproc.c
//  Functions used to implement the Amount processing functionality
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "utllib.h"
#include "tlib.h"

#include "struct.h"
#include "appdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "tables.h"
#include "amtentry.h"
#include "cvtamt.h"
#include "cstmamnt.h"
#include "aapallow.h"
#include "isrest.h"
#include "chkleng.h"
#include "cndmtamt.h"

#include "amtproc.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static Bool InputAmounts( void );
static Bool InputMultAmts( void );
static Bool InputSaleAmount_0( void );
static Bool InputCashAmount( void );
static Bool InputTenderAmount( void );
static char InputAmtConf( void );
static Bool CalcTrTotal( void );
static void PackAmount( enum msg_id Mid );



//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static Bool DBCashFlag;
static char EntryFlag;
static char AscAmount[14];
static Bool CndMultAmtFlag;
static const ENTRY_STR DefAmtProcEntry = {
	N_NullStr,
	N_NullStr,
	N_NullStr,
};

static ENTRY_STR AmtProcEntry;	// structure for AmountEntry() 



//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  AmtProc         Do Amount Entry
//
//  Parameters:     None
//
//  Global Inputs:  chRetval    0 - CANCEL key was pressed
//                              1 - amount entered successfully
//                              2 - Debit Cash Transaction needs to be performed.
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern UBYTE AmtProc( void )
{
	UBYTE chRetval;

#ifdef MAKE_EMV
// early amount giriþi buradan yapýlabilir .
#endif	// MAKE_EMV

        // No; get the Amounts from the user
	if ( InputAmounts(  ) )
	{
            // Okay; set the global status accordingly 
            chRetval = 1;
	}
	else
	{
            // Cancel or timeout; set the global status accordingly 
            chRetval = 0;
	}

	return chRetval;
}


//=============================================================================
// Private function definitions
//=============================================================================




//-----------------------------------------------------------------------------
//  InputAmounts        Get the required Amounts from the User
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool        True  - okay
//                                  False - Cancel or timeout.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InputAmounts( void )
{
	char Retval;
	UBYTE TempTotal[S_TRTOTAM];

	// See if Sale+Cash Transaction 
	if ( (SALCASH == TRINP.TRKEY) 
#ifdef MAKE_EMV
		 
#endif	// MAKE_EMV 
	   )
	{
		// Yes; loop till done
		while ( True )
		{
			DBCashFlag = False;
			// Yes; do we prompt for multiple Amounts ? 
			if ( CndMultAmtFlag )
			{
				// Yes; prompt for multiple amounts 
				Retval = InputMultAmts(  );
			}
			else
			{
				// No; prompt for Sale Amount, zero amount allowed 
				Retval = InputSaleAmount_0(  );
			}

			// See if Cancel or timeout 
			if ( !Retval )
			{
				// Yes; return false 
				return ( False );
			}

			// Prompt for Cash Amount 
			if ( !InputCashAmount(  ) )
			{
				// Yes; return false 
				return ( False );
			}

			// See if Debit Cash 
			if ( !DBCashFlag )
			{
				// No; prompt for Tender Amount 
				if ( !InputTenderAmount(  ) )
				{
					// Yes; return false 
					return ( False );
				}
			}

			// Get user confirmation of the Amount 
			Retval = InputAmtConf(  );

			// See if user accepted the Amount or Cancel/timeout 
			if ( 2 != Retval )
			{
				// Yes; return the status 
				return ( Retval );
			}
		}
	}
	else
	{
		// No; do we prompt for multiple Amounts ? 
		if ( CndMultAmtFlag )
		{
			// Yes; loop till done 
			while ( True )
			{
				// Prompt for multiple amounts
				if ( !InputMultAmts(  ) )
				{
					// Yes; return false 
					return ( False );
				}
								

				// Get user confirmation of the Amount 
				Retval = InputAmtConf(  );

				// See if user accepted the Amount or Cancel/timeout 
				if ( 2 != Retval )
				{
					// Yes; return the status 
					return ( Retval );
				}
			}
		}
		else
		{
			// No, See if Opening a TAB 
			if ( OPENTAB == TRINP.TRKEY )
			{
				// Yes; loop till done 
				while ( True )
				{
					// Get the amount 
					Retval = InputAnAmount( AmountEnt, 0 );

					// See if Cancel or timeout 
					if ( !Retval )
					{
						// Yes; return false 
						return ( False );
					}

					// Get user confirmation of the Amount 
					Retval = InputAmtConf(  );

					// See if user accepted the Amount or Cancel/timeout 
					if ( 2 != Retval )
					{
						// Yes; return the status 
						return ( Retval );
					}
				}
			}
			else
			{

				switch ( TRINP.TRKEY )
				{


					default:
						// No; zero amount not allowed 
						Retval = InputAnAmount( AmountEnt, 0 );
						// See if Cancel or timeout 
						if ( !Retval )
						{
							// Yes; return false 
							return ( False );
						}
				}


			}
		}
	}

	
	// Return okay 
	return ( True );
}



//-----------------------------------------------------------------------------
//  InputMultAmts       Prompt user for multiple Amount entry
//                      (ie. Base, Bar, Tax, and Tip)
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool        True  - okay
//                                  False - Cancel or timeout.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InputMultAmts( void )
{

	// See if Sale+Cash Transaction
	if ( SALCASH == TRINP.TRKEY )
	{
		// Yes; zero amount allowed
		if ( !InputSaleAmount_0(  ) )
		{
			// Cancel or timeout; return false 
			return ( False );
		}

		// See if Cash only Transaction 
		if ( DBCashFlag )
		{
			// Yes; return okay 
			return ( True );
		}
	}
	else
	{
		// No; other type of Transaction, zero amount not allowed 
		if ( !InputAnAmount( AmountBase, 0 ) )
		{
			// Cancel or timeout; return false 
			return ( False );
		}
	}

	

	

	return ( True );
}



//-----------------------------------------------------------------------------
//  InputAnAmount   Prompt user to input an amount
//
//  Parameters:     enum msg_id Mid     message id
//
//                  char ZeroFlag       zero amount allowed flag
//
//  Global Inputs:
//
//  Returns:        Bool            True  - okay
//                                  False - Cancel or timeout.
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool InputAnAmount( enum msg_id Mid, char ZeroFlag )
{
	// Use Amount message that was passed in 
	memcpy( &AmtProcEntry, &DefAmtProcEntry, sizeof( ENTRY_STR ) );

	AmtProcEntry.Line2Mid = Mid;

	// Do until valid amount is entered 
	while ( True )
	{
		if ( !AmountEntry( &AmtProcEntry, ZeroFlag, 0 ) )	// zero amount not allowed 
			return ( False );

		// Save first character of entry 
		EntryFlag = Dspbuf[0];

		// Convert the amount to BCD 
		PackAmount( Mid );

		// Calculate and check the new Total Amount 
		if ( CalcTrTotal(  ) )
		{
			// Valid; exit the entry loop 
			break;
		}
	}

	// Return okay 
	return ( True );
}



//-----------------------------------------------------------------------------
//  InputSaleAmount_0   Prompt user for Sale Amount entry
//                      (zero amount allowed)
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool    True  - okay
//                              False - Cancel or timeout.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InputSaleAmount_0( void )
{
	// Do until valid amount is entered 
	while ( True )
	{
		// Use Sale Amount message; zero amount allowed 
		if ( !InputAnAmount( AmountSale, 1 ) )
		{
			// Cancel or timeout; return false
			return ( False );
		}

		// Valid; is Base Amount non-zero ? 
		if ( !NullComp( ( char * ) TRINP.TRBASE, S_TRBASE ) )
		{
			// Yes; exit the loop 
			break;
		}

		// Do we allow Cash only Transaction ? 
		if ( (TRINP.TRISPTR->ISOPT3) & IS3_CASH_TRN )
		{
			// Yes; do Debit Card Cash Transaction 
			DBCashFlag = True;

			// Exit the loop 
			break;
		}
	}

	// Return okay
	return ( True );
}



//-----------------------------------------------------------------------------
//  InputCashAmount     Prompt user for Cash Amount entry
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool        True  - okay
//                                  False - Cancel or timeout.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InputCashAmount( void )
{
	UBYTE TMPAMT[S_TRTOTAM];

	// See if Transaction is a cash only 
	if ( !DBCashFlag )
	{
		// No; see if Tender Amount is set 
		if ( TCONF.TCOPT4 & TC4_TENDER )
		{
			// Yes; do not prompt for Cash Amount 
			return ( True );
		}
	}

	// Use Cash Amount message 
	memcpy( &AmtProcEntry, &DefAmtProcEntry, sizeof( ENTRY_STR ) );

	AmtProcEntry.Line2Mid = AmountCash;

	// Do until valid amount is entered 
	while ( True )
	{
		// Zero amount allowed 
		if ( !AmountEntry( &AmtProcEntry, 1, 0 ) )
			return ( False );

		// Save Cash Amount on Debit Transaction in BCD 
		PackAmount( AmountCash );

		// Check if limit is zero 
		if ( !NullComp( ( char * ) TCONF.TCCSHBKLIMIT, S_TCCSHBKLIMIT ) )
		{
			// No; Clear Work Amount (Temp Amount) 
			memset( TMPAMT, 0, S_TRTOTAM );

			// Move Cash Back Limit to Temp Amount 
			memcpy( &TMPAMT[S_TRTOTAM - 1 - S_TCCSHBKLIMIT],
					TCONF.TCCSHBKLIMIT, S_TCCSHBKLIMIT );

			// Is Cash > Max ? 
			if ( memcmp( TMPAMT, TRINP.TRCASHBK, S_TRTOTAM ) > 0)
			{
				// Yes; notify user 
				SDK_Beeper( TENMS * 40 );

				// Have user re-enter the Amount 
				continue;
			}
		}

		// Calculate and check the new Total Amount 
		if ( CalcTrTotal(  ) )
		{
			// Valid; is Total Amount zero ?
			if ( NullComp( ( char * ) TRINP.TRTOTAM, S_TRTOTAM ) )
			{
				// Yes; notify user 
				SDK_Beeper( TENMS * 40 );
			}
			else
			{
				// No; exit the loop 
				break;
			}
		}
	}

	// Return okay 
	return ( True );
}



//-----------------------------------------------------------------------------
//  InputTenderAmount       Prompt user for Tender Amount entry
//
//  Parameters:             None
//
//  Global Inputs:
//
//  Returns:                Bool        True  - okay
//                                      False - Cancel or timeout.
//
//  Notes:      Some Merchants prompt for Cash Back Amount then
//              compute the Total Charge, Some prompt for the Total 
//              (Tender) Amount then compute the Cash Back.
//
//-----------------------------------------------------------------------------

static Bool InputTenderAmount( void )
{
	// Is Tender Amount enabled ? 
	if ( !( TCONF.TCOPT4 & TC4_TENDER ) )
	{
		// No; so don't prompt for Tender Amount 
		return ( True );
	}

	// Use Tender Amount message 
	memcpy( &AmtProcEntry, &DefAmtProcEntry, sizeof( ENTRY_STR ) );

	AmtProcEntry.Line2Mid = AmountTender;

	// Do until valid amount is entered 
	while ( True )
	{
		// Zero amount not allowed 
		if ( !AmountEntry( &AmtProcEntry, 0, 0 ) )
			return ( False );

		// Save Tender (instead of Cash) Amount on Debit Transaction 
		PackAmount( AmountTender );

		// Subtract Base from Total giving Cash Amount 
		if ( !SubAmt( TRINP.TRCASHBK, TRINP.TRTOTAM, TRINP.TRBASE ) )
		{
			// Subtract Tip from Cash Amount to really get Cash Amount 
			if ( !SubAmt( TRINP.TRCASHBK, TRINP.TRCASHBK, TRINP.TRTIP ) )
			{
				// Subtract Bar from Cash Amount to really get Cash Amount 
				if ( !SubAmt( TRINP.TRCASHBK, TRINP.TRCASHBK, TRINP.TRBAR ) )
				{
					// Subtract Tax from Cash Amount to really get Cash Amount */
					if ( !SubAmt
						 ( TRINP.TRCASHBK, TRINP.TRCASHBK, TRINP.TRTAX ) )
					{
						// Subtract Merch Surcharge from Cash Amount to
						// really get Cash Amount 
						if ( !SubAmt
							 ( TRINP.TRCASHBK, TRINP.TRCASHBK,
							   TRINP.TRMSURC ) )
						{
							// Amount Tender big enough; exit the loop 
							break;
						}
					}
				}
			}
		}
	}

	// Return okay 
	return ( True );
}



//-----------------------------------------------------------------------------
//  InputAmtConf        Prompt user for amount Confirmation
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            char    0 - Cancel or timeout
//                              1 - Amount confirmed
//                              2 - Amount denied.
//  Notes:
//
//-----------------------------------------------------------------------------

static char InputAmtConf( void )
{
	char Retval;
	UBYTE i;
	UBYTE length;

	// Be sure AscAmount is null terminated.    
	AscAmount[13] = '\0';

	// See if Sale+Cash Transaction (not Cash only) 
	if ( (
		  (SALCASH == TRINP.TRKEY)
#ifdef	MAKE_EMV
		 
#endif	// MAKE_EMV
	     )
		&& (!DBCashFlag) &&
		   ((TRINP.TRISPTR->ISOPT3) & IS3_CASHAMT) &&
		   (TCONF.TCOPT4 & TC4_TENDER) )
	{

		// Add "Cash = $ " at front of Custom Message 
		GetMsg( Msg_Cash, CSTMSG );
		length = StrLn( CSTMSG, sizeof( CSTMSG ) );

		// Convert Cash Amount into ASCII 
		CvtAmt( AscAmount, TRINP.TRCASHBK );

		// Loop through the Cash Amount 
		for ( i = 0; i < sizeof( AscAmount ); i++ )
		{
			// See if non-blank character found 
			if ( ' ' != AscAmount[i] )
			{
				// Yes; exit the loop 
				break;
			}
		}

		// Move the Cash Amount into the Custom Message 
		memcpy( &CSTMSG[length], &AscAmount[i],
				( UWORD ) StrLn( &AscAmount[i], sizeof( AscAmount ) ) );
		length += StrLn( &AscAmount[i], sizeof( AscAmount ) );
		CSTMSG[length] = '\0';

		// Add "T=" at middle of Custom Message 
		GetMsg( Msg_Tender, CSTMSG2 );
		length = StrLn( CSTMSG2, sizeof( CSTMSG2 ) );

		// Convert Tender Amount into ASCII 
		CvtAmt( AscAmount, TRINP.TRTOTAM );

		// Loop through the Tender Amount 
		for ( i = 0; i < sizeof( AscAmount ); i++ )
		{
			// See if non-blank character found 
			if ( ' ' != AscAmount[i] )
			{
				// Yes; exit the loop 
				break;
			}
		}

		// Move the Tender Amount into the Custom Message 
		memcpy( &CSTMSG2[length], &AscAmount[i],
				( UWORD ) StrLn( &AscAmount[i], sizeof( AscAmount ) ) );
		length += StrLn( &AscAmount[i], sizeof( AscAmount ) );
		CSTMSG2[length] = '\0';
		GetMsg( CorrectYN, CSTMSG3 );

	}
	else
	{
		// No; move the Total Amount into the global custom message 
		CustomAmount( AmountTotals, TRINP.TRTOTAM );
		GetMsg( CorrectYN, CSTMSG2 );
		CSTMSG3[0] = '\0';
	}

	// Draw the Cash/Tender Screen 
	DrawScreen( CASH_TEND_SCR );

	// See if this is the correct Total Amount 
	for ( Retval = (char)0xFF; Retval == (char)0xFF; )
	{
		
		switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
		{
			case 0:			// Timed out 
			case CANCEL_KY:	// CANCEL key pressed; return error 
				Retval = 0;
				break;

			case ENTER_KY:		// YES key was pressed; done, return 
				Retval = 1;
				break;

			case CLEAR_KY:
				Retval = 2;
				break;
		}
	}
	// Return the status 
	return ( Retval );
}



//-----------------------------------------------------------------------------
//  CalcTrTotal     Calculate the Total Amount from the parts and make
//                  sure the Total does not exceed Transaction Digits
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - okay
//                              False - too large.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool CalcTrTotal( void )
{
	Bool Status;

	// Add Base to Tip giving Total 
	AddAmt( TRINP.TRTOTAM, TRINP.TRBASE, TRINP.TRTIP );

	// Add Bar to Total giving Total 
	AddAmt( TRINP.TRTOTAM, TRINP.TRTOTAM, TRINP.TRBAR );

	// Add Tax to Total giving Total 
	AddAmt( TRINP.TRTOTAM, TRINP.TRTOTAM, TRINP.TRTAX );

	// Add Cash Back to Total giving Total 
	AddAmt( TRINP.TRTOTAM, TRINP.TRTOTAM, TRINP.TRCASHBK );

	// Add Merchant Surcharge to Total giving Total 
	AddAmt( TRINP.TRTOTAM, TRINP.TRTOTAM, TRINP.TRMSURC );

	// Check the Total length 
	Status = CheckTheLength( TRINP.TRTOTAM );

	/* See if error */
	if ( !Status )
	{
		/* Yes; notify user */
		ShowErrMsg( InvalidAmount );
	}

	// Return the status 
	return ( Status );
}




//-----------------------------------------------------------------------------
//  PackAmount      Convert amount from ASCII into BCD and padded with leading
//                  0s. It also saves it into proper destination buffer 
//                  in TRINP.
//
//  Parameters:     enum msg_id Mid     ID to identify which amount should 
//                                      be saved. 
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

static void PackAmount( enum msg_id Mid )
{
	static char local[13];
	static int cnt;

	memset( ( UBYTE * ) local, '0', 12 );
	local[12] = 0;
	cnt = StrLn( Dspbuf, 12 );

	// added the zero count check.  
	if ( cnt )
		memcpy( &local[12 - cnt], ( UBYTE * ) Dspbuf, ( UWORD ) cnt );

	switch ( Mid )
	{
		case AmountEnt:
		case AmountSale:
		case AmountBase:
			AscHex( TRINP.TRBASE, local, 6 );
			break;

		case AmountBar:
			AscHex( TRINP.TRBAR, local, 6 );
			break;

		case AmountOpt:
			AscHex( TRINP.TROPTAMT, local, 6 );
			break;

		case AmountTax:
			AscHex( TRINP.TRTAX, local, 6 );
			break;

		case AmountTip:
			AscHex( TRINP.TRTIP, local, 6 );
			break;

		case AmountCash:
			AscHex( TRINP.TRCASHBK, local, 6 );
			break;

		case AmountTender:
		case AmountTotal:
			AscHex( TRINP.TRTOTAM, local, 6 );
			break;
#ifdef MAKE_KIB
               case LoyaltyAmountEnt:
                      AscHex( TRINP.TRUSEDLOYALTYAMOUNT, local, 6 );
                      break;
#endif                
		default:
			break;
	}

}




