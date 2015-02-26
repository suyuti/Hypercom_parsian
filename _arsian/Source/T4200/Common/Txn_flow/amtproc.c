
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
static Bool InputAnAmount( enum msg_id Mid, char ZeroFlag );
static Bool InputSaleAmount_0( void );
static Bool InputCashAmount( void );
static Bool InputTenderAmount( void );
static char InputAmtConf( void );
static Bool CalcTrTotal( void );
static void PackAmount( enum msg_id Mid );
static Bool InputTipAmount( void );
static Bool InTipPercent( void );

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
static UBYTE SugTipPct;			// Save user Selected Suggest Tip Percentage

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
//!  \brief
//!     Do Amount Entry
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Globas:  chRetval
//!                 0 - CANCEL key was pressed
//!                 1 - amount entered successfully
//!                 2 - Debit Cash Transaction needs to be performed.

//-----------------------------------------------------------------------------
extern UBYTE AmtProc( void )
{
	UBYTE chRetval;

#ifdef MAKE_EMV
    // if 'Early Amount Entry for ICC Txns' never mind the rest - we don't have an issuer!
    // just get amounts based on txn type
    if (TC6_ICCEARLYAMT & TCONF.TCOPT6)
    {
        chRetval = 1; // initialise flag as OK

        // Check whether amounts have already been entered 
        // (maybe for fallback txn with early amount entry)
        if ( !NullComp( ( char * ) TRINP.TRTOTAM, S_TRTOTAM ) ) 
        {
            return chRetval; // we already have the amounts
        }
        else
        {

		    // Get the Amounts from the user
            switch ( TRINP.TRKEY )
            {
                case EMV_SALE:
                case EMV_REFUND:
		            if (!InputSaleAmount_0())
                    {
			            chRetval = 0; // Sale amount cancelled/timed out
                    }
                    break;
        
                case EMV_SALCASH:
		            if (!InputSaleAmount_0())
                    {
			            chRetval = 0; // Sale amount cancelled/timed out
                        return chRetval;
                    }
                    // lack of break intentional

                case EMV_CASH:
                    if (!InputCashAmount())
                    {
                        chRetval = 0; // Cash amount timed out
                    }
                    break;

                default:
                    // i don't know - get a sale amount
		            if (!InputSaleAmount_0())
                    {
			            chRetval = 0; // Sale amount cancelled/timed out
                    }
                    break;
            }

        }

        return chRetval;
    }
#endif	// MAKE_EMV


	// Reset Debit Cash Transaction flag 
	DBCashFlag = False;

	// See if we need to copy the Merchant Surcharge 
	if ( SALCASH == TRINP.TRKEY )
	{
		// Yes; copy Merchant Surcharge from Terminal Conf (right 
		// justified) 
		memcpy( &TRINP.TRMSURC[S_TRBASE - S_TCMERDBS], TCONF.TCMERDBS, S_TCMERDBS );
	}

	// See if we prompt for multiple Amounts 
	CndMultAmtFlag = CndMultAmt(  );

	// See if any Transactions disallow prompt for multiple amounts 
	if ( ( CRDVER == TRINP.TRKEY ) || ( OPENTAB == TRINP.TRKEY ) )
	{
		// Yes; only prompt for single Amount 
		CndMultAmtFlag = False;
	}
	else
	{
		// No; are Additional Amount Prompts disallowed ? 
		if ( !AAPAllowed(  ) )
		{
			// Yes; only prompt for single Amount
			CndMultAmtFlag = False;
		}
	}

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
//!  \brief
//!     Get the required Amounts from the User
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - okay
//!                 False - Cancel or timeout.
//-----------------------------------------------------------------------------

static Bool InputAmounts( void )
{
	char Retval;
	UBYTE TempTotal[S_TRTOTAM];

	// See if Sale+Cash Transaction 
	if ( (SALCASH == TRINP.TRKEY) 
#ifdef MAKE_EMV
		 || ( EMV_SALCASH == TRINP.TRKEY )
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

				// See if Dual Amount Entry is required 
				if ( TCONF.TCOPT1 & TC1_DUALAMT )
				{
					// Yes; exit the loop 
					break;
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
#ifdef MAKE_EMV
					case EMV_CASH:
						// Get the cash amount 
						if ( !InputCashAmount(  ) )
							return ( False );
						break;

                    case DBCASH:
                        // Get the cash amount 
                        if (!InputCashAmount())
                            return ( False );
                        break;
#endif	// MAKE_EMV

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

	// See if Dual Amount Entry is required 
	if ( TCONF.TCOPT1 & TC1_DUALAMT )
	{
		// Yes; save Total for memcmpision with user entry 
		memcpy( TempTotal, TRINP.TRTOTAM, S_TRTOTAM );

		// Loop till done
		while ( True )
		{
			// Do we prompt for multiple Amounts ? 
			if ( CndMultAmtFlag )
			{
				// Yes; notify user about Total entry 
				memcpy( &AmtProcEntry, &DefAmtProcEntry,
						sizeof( ENTRY_STR ) );

				AmtProcEntry.Line2Mid = AmountTotal;
			}
			else
			{
				// No; notify user about re-entering Amount 
				memcpy( &AmtProcEntry, &DefAmtProcEntry,
						sizeof( ENTRY_STR ) );

				AmtProcEntry.Line2Mid = AmountAgain;
			}

			if ( !AmountEntry( &AmtProcEntry, 0, 0 ) )	// zero amount not allowed 
				return ( False );

			// Pack the Total Amount 
			PackAmount( AmountTotal );

			// See if user entry matches calculated Total 
			if ( memcmp( TRINP.TRTOTAM, TempTotal, S_TRTOTAM ) )
			{
				// No; notify user 
				SDK_Beeper( TENMS * 40 );
			}
			else
			{
				// Yes; exit the loop 
				break;
			}
		}
	}

	// Return okay 
	return ( True );
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Prompt user for multiple Amount entry (ie. Base, Bar, Tax, and Tip)
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - okay
//!                 False - Cancel or timeout.
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

	// See if Bar Amount is required 
	if ( ( (TRINP.TRISPTR->ISOPT5) & IS5_BAR ) &&
		 ( AUTH != CurrentEvent.TRNKEY ) &&
		 ( OPENTAB != CurrentEvent.TRNKEY ) )
	{
		// Yes; zero amount is allowed 
		if ( !InputAnAmount( AmountBar, 1 ) )
		{
			// Cancel or timeout; return false 
			return ( False );
		}
	}

	// See if Tax Amount is required 
	if ( ((TRINP.TRISPTR->ISOPT4) & IS4_TAX ) &&
		 ( AUTH != CurrentEvent.TRNKEY ) &&
		 ( OPENTAB != CurrentEvent.TRNKEY ) )
	{
		// Yes; zero amount is allowed 
		if ( !InputAnAmount( AmountTax, 1 ) )
		{
			// Cancel or timeout; return false 
			return ( False );
		}
	}

	// See if Tip Amount is required 
	if ( ( !( TCONF.TCOPT5 & TC5_NOTIP ) ) && IsRest(  ) &&
		 ( OPENTAB != CurrentEvent.TRNKEY ) )
	{
		if ( !InputTipAmount(  ) )
		{
			// Cancel or timeout; return false 
			return ( False );
		}

	}

	return ( True );
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Prompt user to input an amount
//!
//!  \param
//!     Mid         message id
//!  \param
//!     ZeroFlag    zero amount allowed flag
//!
//!  \return
//!       Bool      True  - okay
//!                 False - Cancel or timeout.
//-----------------------------------------------------------------------------
static Bool InputAnAmount( enum msg_id Mid, char ZeroFlag )
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
//!  \brief
//!     Prompt user for Sale Amount entry (zero amount allowed)
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - okay
//!                 False - Cancel or timeout.
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
//!  \brief
//!     Prompt user for Cash Amount entry
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - okay
//!                 False - Cancel or timeout.
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
//!  \brief
//!     Prompt user for Tender Amount entry
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - okay
//!                 False - Cancel or timeout.
//!
//!  \note
//!     Some Merchants prompt for Cash Back Amount then
//!     compute the Total Charge, Some prompt for the Total 
//!     (Tender) Amount then compute the Cash Back.
//!
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
//!  \brief
//!     Prompt user for amount Confirmation
//!
//!  \param
//!     None
//!
//!  \return
//!     char        0 - Cancel or timeout
//!                 1 - Amount confirmed
//!                 2 - Amount denied.
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
		 || ( EMV_SALCASH == TRINP.TRKEY )
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
//!  \brief
//!     Calculate the Total Amount from the parts and make
//!     sure the Total does not exceed Transaction Digits
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - okay
//!                 False - too large.
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
//!  \brief
//!     Convert amount from ASCII into BCD and padded with leading
//!     0s. It also saves it into proper destination buffer in TRINP.
//!
//!  \param
//!     Mid         ID to identify which amount should be saved. 
//!
//!  \return
//!     None
//!
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

		default:
			break;
	}

}



//-----------------------------------------------------------------------------
//!  \brief
//!     Process Tip Input
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True - Okay
//!                 False - Timeout or user cancel
//-----------------------------------------------------------------------------
static Bool InputTipAmount( void )
{
	UBYTE keyVal;

	// Loop till a tip amount is accepted 
	while ( True )
	{
		// Default selected tip percentage to 0 
		SugTipPct = 0;

		// Processtip input 
		if ( InTipPercent(  ) )
		{
			// Calculate the total amount and suggested tip amount  
			CReauth( TRINP.TRTIP, TRINP.TRBASE, SugTipPct );

			// Get tip amount 
			SubAmt( TRINP.TRTIP, TRINP.TRTIP, TRINP.TRBASE );

		}
		else
		{
			// Cancel or timeout; return false 
			return ( False );
		}

		// See if user selected the tip percentage 
		if ( 0 != SugTipPct )
		{
			// Yes; move the Tip Amount into the global custom message 
			CustomAmount( AmountTip, TRINP.TRTIP );
			GetMsg( CorrectYN, CSTMSG2 );
			CSTMSG3[0] = '\0';

			// Draw the Cash/Tender Screen 
			DrawScreen( CASH_TEND_SCR );

			// Prompt for Tip Confirmation Yes/No Input 
			do
			{
				keyVal = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );
				switch ( keyVal )
				{
					case 0:	// Timeout 
					case CANCEL_KY:
						return ( False );

					case ENTER_KY:	// YES 
						return ( CalcTrTotal(  ) );
				}
			}
			while ( CLEAR_KY != keyVal );	// if NO, go to Tip amount input screen again
		}
		else
		{
			// No; zero amount is allowed 
			if ( !InputAnAmount( AmountTip, 1 ) )
			{
				// Cancel or timeout; return false 
				return ( False );
			}

			// See if a non-blank Tip was entered 
			if ( EntryFlag )
			{
				// Yes; set the flag in TRINP 
				TRINP.TRSTATUS[0] |= TS1_TIPENT;
			}

			// Return okay 
			return ( True );
		}
	}
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Set up Tip Percentage Screen and get tip input
//!
//!  \param
//!     None
//!
//!  \return
//!     Bool        True  - Tip input
//!                 False - Time out or cancel
//-----------------------------------------------------------------------------
static Bool InTipPercent( void )
{
	static UBYTE TipPercent[3];	// Suggest Tip Percentage 
	static int TipCounter;

	TipCounter = 0;
	Enable_Flags[3] = 0;
	Enable_Flags[4] = 0;

	// Clear output meaasges
	*CSTMSG  = 0;
	*CSTMSG2 = 0;
	*CSTMSG3 = 0;
	*CSTMSG4 = 0;

	// Check Each of the Tip Percentage fields to see if set in TermMaster 
	if ( 0x00 != TCONF.TCTIPPCT_1 )
	{
		// This tip percentage field was set, get percentage 
		TipPercent[TipCounter++] = TCONF.TCTIPPCT_1;

		// Calculate the total amount and suggested tip amount  
		CReauth( TRINP.TRTOTAM, TRINP.TRBASE, TCONF.TCTIPPCT_1 );

		// Get tip amount 
		SubAmt( TRINP.TRSUGTIP1, TRINP.TRTOTAM, TRINP.TRBASE );
	}
	else
		memset( TRINP.TRSUGTIP1, 0, S_SUGTIP );

	if ( 0x00 != TCONF.TCTIPPCT_2 )
	{
		// This tip percentage field was set, get percentage 
		TipPercent[TipCounter++] = TCONF.TCTIPPCT_2;

		// Calculate the total amount and suggested tip amount  
		CReauth( TRINP.TRTOTAM, TRINP.TRBASE, TCONF.TCTIPPCT_2 );

		// Get tip amount 
		SubAmt( TRINP.TRSUGTIP2, TRINP.TRTOTAM, TRINP.TRBASE );
	}
	else
		memset( TRINP.TRSUGTIP2, 0, S_SUGTIP );

	if ( 0x00 != TCONF.TCTIPPCT_3 )
	{
		// This tip percentage field was set, get percentage 
		TipPercent[TipCounter++] = TCONF.TCTIPPCT_3;

		// Calculate the total amount and suggested tip amount  
		CReauth( TRINP.TRTOTAM, TRINP.TRBASE, TCONF.TCTIPPCT_3 );

		// Get tip amount 
		SubAmt( TRINP.TRSUGTIP3, TRINP.TRTOTAM, TRINP.TRBASE );
	}
	else
		memset( TRINP.TRSUGTIP3, 0, S_SUGTIP );

	// Check Tip Percentage counter and see if any fields were set 
	if ( 0 != TipCounter )
	{
		// Need to build screen to display default percentages 
		// Will always put TipPercent[0] into CSTMSG Buffer 
		// Convert TipPercent[0]to ASCII and place in CSTMSG[1]/[2] 
		BufAsc( ( char * ) &CSTMSG[0], &TipPercent[0] );

		// Convert 0X % to dispaly  X % 
		if ( '0' == CSTMSG[0] )
		{
			CSTMSG[0] = ' ';
		}

		CSTMSG[2] = ' ';
		CSTMSG[3] = '%';
		CSTMSG[4] = '\0';

		// Now build the rest of the buttons 
		switch ( TipCounter )
		{
			case 1:			// A two Button Screen 
				// Set up Second Button 
				GetMsg( Msg_Other, CSTMSG2 );
				break;

			case 2:			// A three Button Screen 
				// Set up Second Button 
				// Convert TipPercent[1]to ASCII and place in CSTMSG2[1]/[2] 
				BufAsc( ( char * ) &CSTMSG2[0], &TipPercent[1] );

				// Convert 0X % to dispaly  X % 
				if ( '0' == CSTMSG2[0] )
				{
					CSTMSG2[0] = ' ';
				}

				CSTMSG2[2] = ' ';
				CSTMSG2[3] = '%';
				CSTMSG2[4] = '\0';

				// Set up Third Button 
				GetMsg( Msg_Other, CSTMSG3 );
				Enable_Flags[3] = 1;
				break;

			case 3:			// A four Button Screen 
				// Set up Second Button 
				// Convert TipPercent[1]to ASCII and place in CSTMSG2[1]/[2] 
				BufAsc( ( char * ) &CSTMSG2[0], &TipPercent[1] );

				// Convert 0X % to dispaly  X % 
				if ( '0' == CSTMSG2[0] )
				{
					CSTMSG2[0] = ' ';
				}

				CSTMSG2[2] = ' ';
				CSTMSG2[3] = '%';
				CSTMSG2[4] = '\0';

				// Set up Third Button 
				// Convert TipPercent[2]to ASCII and place in CSTMSG3[1]/[2] 
				BufAsc( ( char * ) &CSTMSG3[0], &TipPercent[2] );

				// Convert 0X % to dispaly  X % 
				if ( '0' == CSTMSG3[0] )
				{
					CSTMSG3[0] = ' ';
				}

				CSTMSG3[2] = ' ';
				CSTMSG3[3] = '%';
				CSTMSG3[4] = '\0';
				Enable_Flags[3] = 1;

				// Set up Second Button 
				GetMsg( Msg_Other, CSTMSG4 );
				Enable_Flags[4] = 1;
				break;
		}


		// Now draw the Tip Percentage Screen 
		DrawScreen( TIP_PERCENT_SCR );

		// Do until done 
		while ( True )
		{
			// Wait for a Keypress or timeout 
			switch ( GetOneKey( DEV_PAD | DEV_KBD, UserTimeout ) )
			{
				case '1':		// User selected the First button 	
					// Save value set in TipPercent[0] 
					SugTipPct = TipPercent[0];
					return True;

				case '2':		// User selected the Second button 	
					// Check if second tip percentage, or OTHER 
					if ( 1 != TipCounter )
					{
						// Save value set in TipPercent[1] 
						SugTipPct = TipPercent[1];
					}
					return True;

				case '3':		// User selected the Third button 	
					// Check if second tip percentage, or OTHER 
					if ( 2 != TipCounter )
					{
						// Save value set in TipPercent[2 
						SugTipPct = TipPercent[2];
					}
					return True;

				case '4':		// User selected the Fourth "OTHER" button 
					return True;

				case CANCEL_KY:	// Timed out or cancelled 
				case CLEAR_KY:
				case 0:
					return False;

				default:
					// bad key, error beep 
					SDK_Beeper( TENMS * 40 );
					break;
			}
		}
	}

	return True;
}

