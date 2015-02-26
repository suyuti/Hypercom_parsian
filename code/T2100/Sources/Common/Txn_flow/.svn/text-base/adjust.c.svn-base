
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     adjust.c
//      Functions used to implement Adjust Transaction
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdktime.h"
#include "utllib.h"

#include "struct.h"
#include "util.h"
#include "aapallow.h"
#include "appdata.h"
#include "amtentry.h"
#include "chkleng.h"
#include "cndmtamt.h"
#include "cstmamnt.h"
#include "findtran.h"
#include "inadjinv.h"
#include "inputpwd.h"
#include "isrest.h"
#include "journal.h"
#include "msg.h"
#include "pinutils.h"
#include "tabclose.h"
#include "tables.h"
#include "trinpclr.h"

#include "adjust.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static Bool InputPswdAdjust( void );
static Bool InputAdjFirstTotal( void );
static UBYTE InputAdjTipConf( void );
static Bool InputAdjFirstTip( void );
static UBYTE InputAdjTotal( void );
static Bool InputAdjBase( void );
static Bool InputAdjBar( void );
static Bool InputAdjTax( void );
static Bool InputAdjTip( void );
static Bool CompleteAdjust( UWORD AdjIdent );
static Bool CondFirstTip( void );
static void CmplAdjustAmts( void );



//=============================================================================
// Public data definitions
//=============================================================================
UBYTE NEWBAR[S_TRBASE];			// Adjust temporary Amount 
UBYTE NEWBASE[S_TRBASE];		// Adjust temporary Amount 
UBYTE NEWTAX[S_TRBASE];			// Adjust temporary Amount 
UBYTE NEWTIP[S_TRTIP];			// Adjust temporary Amount 
UBYTE NEWINPUTTIP[S_TRTIP];		// Adjust input Amount 
UBYTE NEWTOTAM[S_TRTOTAM];		// Adjust temporary Amount 
UBYTE NEWINPUTTOTAM[S_TRTOTAM];	// Adjust input Amount 
UBYTE TEMPAMT[S_TRTOTAM];		// Adjust working Amount
UBYTE NEWTRST1;					// Copy of first byte of TRSTATUS (for TS1_TIPENT)



//=============================================================================
// Private data definitions
//=============================================================================
static const ENTRY_STR YNEntry =	// structure for YesNoEntry() 
{
	N_NullStr,
	CustomMsg,
	CorrectYesNo,
};

static const ENTRY_STR AdjEntry =	// structure for AmountEntry() 
{
	N_NullStr,
	CustomMsg,
	AmountNew,
};

static const ENTRY_STR AdjFirstEntry =	// structure for AmountEntry()
{
	N_NullStr,
	CustomMsg,
	EnterTotal,
};

static const ENTRY_STR AdjFirstTipEntry =	// structure for AmountEntry() 
{
	N_NullStr,
	CustomMsg,
	EnterTip,
};

static UBYTE SAVAMT[S_TRTOTAM];	// Temp Storage for Amount 
static UBYTE TMPAMT[S_TRTOTAM];	// Temp Storage for Amounts 
static UBYTE InputAmount[S_TRBASE * 2];	// Temp Storage for entry 

//=============================================================================
// Public function definitions
//=============================================================================




//-----------------------------------------------------------------------------
//  Adjust          Adjust Transaction
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

void Adjust( void )
{
	// Set Transaction Type in TRINP 
	TRINP.TRKEY = ADJUST;

	// Save and verify the Input Password if needed 
	if ( InputPswdAdjust(  ) )
	{
		
	}
}



//-----------------------------------------------------------------------------
//  DoAdjust        Perform the Adjust
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

void DoAdjust( void )
{
	UBYTE Status = 1;
    UWORD AdjJrnIdent;

	// Do till done 
	while ( True )
	{
        // Get the Invoice Number 
        // and the Transaction Journal Identifier depended on Invoice Number
        AdjJrnIdent = InputAdjInv(  );

        // Check if cancelled or adjust unpossible
        if ( 0x0000 == AdjJrnIdent )
			break;

		// Do separate processing for OPENTAB.
		if ( OPENTAB != TRINP.TRKEY )
		{
			// Okay; get Total first if required 
			if ( !InputAdjFirstTotal(  ) )
				break;			// Cancel; exit the loop 

			// Get Calculated Tip Confirmation if required 
			Status = InputAdjTipConf(  );

			// Check if input steps to continue 
			if ( 0 == Status )
				break;	   // Cancel; exit the loop		
			else if ( 1 == Status )
			{
				// Yes; get Tip first if required 
				if ( !InputAdjFirstTip(  ) )
					break;

				// Process until the Total Amount is what user wants 
				while ( True )
				{
					// Get Total Amount 
					Status = InputAdjTotal(  );

					// Check if input steps to continue 
					if ( 0 == Status )
					{	// 0 --- Cancel; exit the loop
						return;
					}
					else if ( 1 == Status )
					{
						// Yes; get the Base Amount 
						if ( !InputAdjBase(  ) )
							return;	// Cancel; exit the loop 

						// Get the Bar Amount
						if ( !InputAdjBar(  ) )
							return;	// Cancel; exit the loop 

						// Get the Tax Amount 
						if ( !InputAdjTax(  ) )
							return;	// Cancel; exit the loop 

						// Get the Tip Amount 
						if ( !InputAdjTip(  ) )
							return;	// Cancel; exit the loop
					}
					else
					{
						// 2 --- Complete the transaction now. Exit the input loop
						break;
					}
				}
			}

			// Complete the Adjust 
			if ( !CompleteAdjust( AdjJrnIdent ) )
			{
				// Cancel; exit the loop 
				break;
			}
		}
		else
		{
			// Process Close Tab.   
			CloseTabCommon( AdjJrnIdent );
		}

		// See if we are to loop 
		if ( AdjAgain )
		{
			// Yes; be sure to clear out Transaction data 
			TrinpClear(  );
		}
		else
		{
			// No; exit the loop 
			break;
		}
	}
}




//-----------------------------------------------------------------------------
//  SaveAdjust      Adds up the Amounts and stores a new
//                  TRINP.TRTOTAM, that is if there is no overflow
//
//  Parameters:     UBYTE * pSaveAmount     pointer to saved amount.
//
//  Global Inputs:
//
//  Returns:        Bool        True  - okay to continue
//                              False - reprompt for a different Amount.
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool SaveAdjust( UBYTE * pSaveAmount )
{
	UBYTE Length;

	// Save the original Amount in case of overflow 
	memcpy( SAVAMT, pSaveAmount, S_TRTOTAM );

	// Get the length of the Amount entered 
	Length = StrLn( Dspbuf, sizeof( Dspbuf ) );

	// ASCII zero (0x30) fill the Temp Amount 
	memset( InputAmount, 0x30, sizeof( InputAmount ) );

	// Move the new Total Amount into the Temp Amount 
	// (Right-Justified) 
	memcpy( &InputAmount[sizeof( InputAmount ) - Length], Dspbuf, Length );

	// Save the new Amount in BCD 
	AscHex( pSaveAmount, ( char * ) InputAmount, S_TRBASE );

	// Save the Total Amount in case of overflow 
	memcpy( TMPAMT, NEWTOTAM, S_TRTOTAM );

	// Calculate new Total Amount: NEWTOTAM = NEWBASE + NEWTIP 
	AddAmt( NEWTOTAM, NEWBASE, NEWTIP );

	//                                                + NEWTAX 
	AddAmt( NEWTOTAM, NEWTOTAM, NEWTAX );

	//                                                + NEWBAR 
	AddAmt( NEWTOTAM, NEWTOTAM, NEWBAR );

	//                                                + TRCASHBK 
	AddAmt( NEWTOTAM, NEWTOTAM, TRINP.TRCASHBK );

	// Make sure not too long 
	if ( CheckTheLength( NEWTOTAM ) )
	{
		// Yes; return okay 
		return ( True );
	}
	else
	{
		// No; move the saved Amount back 
		memcpy( pSaveAmount, SAVAMT, S_TRTOTAM );

		// Restore the Total Amount also 
		memcpy( NEWTOTAM, TMPAMT, S_TRTOTAM );

		// Return to reprompt for another Amount 
		return ( False );
	}
}




//-----------------------------------------------------------------------------
//  CopyAmounts     Copy Amounts to temporary buffers
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

extern void CopyAmounts( void )
{
	// Set up temporary variables for the Adjust Transaction 

	// Copy Tip Amount
	memcpy( NEWTIP, TRINP.TRTIP, S_TRTIP );

	// Copy Bar Amount 
	memcpy( NEWBAR, TRINP.TRBAR, S_TRBASE );

	// Copy Tax Amount 
	memcpy( NEWTAX, TRINP.TRTAX, S_TRBASE );

	// Copy Total Amount 
	memcpy( NEWTOTAM, TRINP.TRTOTAM, S_TRTOTAM );

	// Copy Base Amount 
	memcpy( NEWBASE, TRINP.TRBASE, S_TRBASE );

	// Save status (for TS1_TIPENT) 
	NEWTRST1 = TRINP.TRSTATUS[0];

	// If we are coming from Batch Review TRKEY is Adjust, change it
	// to the Transaction key 
	TRINP.TRKEY = TRINP.TRORIGKEY;
}




//=============================================================================
// Private function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  InputPswdAdjust     Input Adjust Password
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool    True  - Password entered okay or may skip it
//                              False - Password entry canceled.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InputPswdAdjust( void )
{
	Bool Status;

	// See if called from Batch Review 
	if ( TRINP.TRINV[0] )
	{
		// Yes; Batch Review handles the Password 
		Status = True;
	}
	else
	{
		// See if Password is needed 
		if ( TCONF.TCLOCOP & TCL_NO_APWD )
		{
			// No; password is not needed 
			Status = True;
		}
		else
		{
			// Yes; save and verify the Input Password 
			Status = InputPswd( N_NullStr );
		}
	}

	// Return the status 
	return ( Status );
}



//-----------------------------------------------------------------------------
//  InputAdjFirstTotal      If a Tip Amount does not exist, and we have Tip
//                          Processing turned on, then the user probably
//                          entered Adjust to add a Tip, so prompt for Total
//                          first if so configured.
//                                                      
//  Parameters:             None
//
//  Global Inputs:
//
//  Returns:                Bool    True  - okay to continue
//                                  False - cancel/abort the transaction.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InputAdjFirstTotal( void )
{
	Bool status;
	UBYTE retval;
	UBYTE Length;

	// Calculate Tip from Total ? 
	if ( !( TCONF.TCOPT5 & TC5_CALCTIP ) )
	{
		// No; return okay 
		return ( True );
	}

	// Ask for Tip first in Adjust ? 
	if ( !CondFirstTip(  ) )
	{
		// No; return okay 
		return ( True );
	}

	// Copy Amounts to temporary buffers 
	CopyAmounts(  );

	// Move the Base Amount into the global custom message 
	CustomAmount( AmountBase, NEWBASE );

	// Loop till done 
	while( True )
	{
		// Set clear key 
		GlobalDsp = CLRKEYON;

		// Get new Base Amount 
		retval = AmountEntry( &AdjFirstEntry, 1, 0 );
		if( 1 == retval )
		{	// Save new Base Amount 

			// Get the length of the Base Amount entered 
			Length = StrLn( Dspbuf, sizeof( Dspbuf ) );

			// ASCII zero (0x30) fill the Temp Amount 
			memset( InputAmount, 0x30, sizeof( InputAmount ) );

			// Move the new Total Amount into the Temp Amount 
			// (Right-Justified) 
			memcpy( &InputAmount[sizeof( InputAmount ) - Length],
					Dspbuf, Length );

			// Save the input Total Amount in BCD 
			AscHex( NEWINPUTTOTAM, ( char * ) InputAmount, S_TRTOTAM );

			status = True;

			// Exit loop
			break;
		}
		else if( 3 != retval )
		{	// Entry canceled 
			status =  False ;

			// Exit loop
			break;
		}			

		// Redo entry
	} 

	// Return the status
	return( status );
}



//-----------------------------------------------------------------------------
//  InputAdjustTipConf  Confirm the Tip as computed from the entered Total
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            UBYTE       0 - cancel/abort the Transaction
//                                  1 - okay to continue
//                                  2 - complete the Transaction now.
//  Notes:
//
//-----------------------------------------------------------------------------

static UBYTE InputAdjTipConf( void )
{
	// Calculate Tip from Total ? 
	if ( !( TCONF.TCOPT5 & TC5_CALCTIP ) )
	{
		// No; return okay 
		return ( 1 );
	}

	// Ask for Tip first in Adjust ? 
	if ( !CondFirstTip(  ) )
	{
		// No; return okay 
		return ( 1 );
	}

	// Calculate new Tip Amount: (Total - Base - Tax)
	// NEWINPUTTIP = NEWINPUTTOTAM - NEWBASE - NEWTAX
	if ( SubAmt( TEMPAMT, NEWINPUTTOTAM, NEWBASE ) )
	{
		// Negative means they typed in a smaller number; return okay 
		return ( 1 );
	}
	else if ( SubAmt( NEWINPUTTIP, TEMPAMT, NEWTAX ) )
	{
		// Negative means they typed in a smaller number; return okay 
		return ( 1 );
	}

	// Move the Tip Amount into the global custom message 
	CustomAmount( AmountTip, NEWINPUTTIP );

	// Notify the user 

	// See if this is the correct Amount 
	switch ( YesNoEntry( &YNEntry ) )
	{
		case 0:				// Timed out 
		case CANCEL_KY:		// Cancel key is pressed 
			// Return cancel/abort the Transaction 
			return ( 0 );
		case CLEAR_KY:			// NO - return continue 
			return ( 1 );
		case ENTER_KY:			// Yes - accept tip amount. 
			memcpy( NEWTIP, NEWINPUTTIP, S_TRTIP );
			break;
	}


	// Tip amount was accepted.  Put the new Total in its place 
	memcpy( NEWTOTAM, NEWINPUTTOTAM, S_TRTOTAM );

	// A non-blank Tip was entered 
	NEWTRST1 |= TS1_TIPENT;

	// Return complete the Transaction now 
	return ( 2 );
}



//-----------------------------------------------------------------------------
//  InputAdjFirstTip        If a Tip Amount does not exist, and we have Tip
//                          Processing turned on, then the user probably
//                          entered Adjust to add a Tip, so prompt for it first
//
//  Parameters:             None
//
//  Global Inputs:
//
//  Returns:                Bool        True  - okay to continue
//                                      False - cancel/abort the transaction.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InputAdjFirstTip( void )
{
	Bool status;
	UBYTE retval;

	// Copy Amounts to temporary buffers 
	CopyAmounts(  );

	// Ask for Tip first in Adjust ? 
	if ( !CondFirstTip(  ) )
	{
		// No; return okay 
		return ( True );
	}

	// Move the Base Amount into the global custom message 
	CustomAmount( AmountBase, NEWBASE );

	// Loop till done 
	while( True )
	{
		// Set clear key 
		GlobalDsp = CLRKEYON;

		// Get new Tip Amount 
		retval = AmountEntry( &AdjFirstTipEntry, 1, 0 );
		if( 1 == retval )
		{	// See if a non-blank Tip was entered 
			if ( Dspbuf[0] )
			{
				// Yes; set new status accordingly 
				NEWTRST1 |= TS1_TIPENT;
			}

			// See if new Tip Amount is valid 
			if ( SaveAdjust( NEWTIP ) )
			{
				// Yes; set status to indicate okay 
				status = True;

				// Exit loop
				break;
			}
			
			// Redo input 
		}
		else if( 3 != retval )
		{	// Entry canceled
			status = False;

			// Exit loop
			break;
		}

		// Redo input 
	}

	// Return the status
	return( status );
}



//-----------------------------------------------------------------------------
//  InputAdjTotal       Input Adjust Total
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:    //      UBYTE       0 - cancel/abort the Transaction
//                                  1 - okay to continue
//                                  2 - complete the Transaction now.
//  Notes:
//
//-----------------------------------------------------------------------------

static UBYTE InputAdjTotal( void )
{
	UBYTE status;
	UBYTE retval;

	// Loop till done 
	do
	{
		// Move the Total Amount into the global custom message 
		CustomAmount( AmountTotals, NEWTOTAM );

		// Notify the user 

		// See if this is the correct Amount 
		switch ( YesNoEntry( &YNEntry ) )
		{
			case 0:			// Timed out 
			case CANCEL_KY:	// Cancel key is pressed 
				// Return cancel/abort the Transaction 
				return ( 0 );

			case ENTER_KY:		// YES key was pressed 

				// Return complete the Transaction now 
				return ( 2 );
		}

		// See if additional Amount prompts allowed 
		if ( AAPAllowed(  ) )
		{
			// Yes; do we prompt for multiple Amounts ? 
			if ( CndMultAmt(  ) )
			{
				// Yes; so do not prompt for the Total Amount 
				return ( 1 );
			}

			// Add check for Nova Host here 
		}

		// Do until done 
		while( True )
		{
			// Set clear key 
			GlobalDsp = CLRKEYON;

			// Get new Total Amount 
			retval = AmountEntry( &AdjEntry, 0, 0 );
			if( 1 == retval)
			{	// See if new Total Amount is valid 
				if ( SaveAdjust( NEWBASE ) )
				{
					// Yes; set status to indicate okay 
					status = 1;

					// Exit to return
					break;
				}
				
				// Redo input 
			}
			else if( 3 != retval )
			{	// Entry canceled
				status = 0;

				// Exit to return
				break;
			}
			else
			{	// Go back to previous entry 
				break;
			}
		}
	} while ( 3 == retval );

	// return the status
	return( status );
}



//-----------------------------------------------------------------------------
//  InputAdjBase        Get and save the Adjust Base Amount
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:    //      Bool    True  - okay to continue
//                              False - cancel/abort the transaction.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InputAdjBase( void )
{
	Bool status;
	UBYTE retval;

	// Add check for Nova Host here 

	// See if additional Amount prompts allowed 
	if ( !AAPAllowed(  ) )
	{
		// No; return okay 
		return ( True );
	}

	// Do we prompt for multiple Amounts ? 
	if ( !CndMultAmt(  ) )
	{
		// No; return okay 
		return ( True );
	}

	// Loop till done 
	do
	{
		// Move the Base Amount into the global custom message 
		CustomAmount( AmountBase, NEWBASE );

		// Notify the user 

		// See if this is the correct Amount 
		switch ( YesNoEntry( &YNEntry ) )
		{
			case 0:			// Timed out 
			case CANCEL_KY:	// CANCEL key pressed 
				// Return cancel/abort the Transaction 
				return ( False );
			case ENTER_KY:		// YES key was pressed 
				// Return okay 
				return ( True );
		}

		// Do until done 
		while( True )
		{
			// Set clear key 
			GlobalDsp = CLRKEYON;

			// Get new Base Amount 
			retval = AmountEntry( &AdjEntry, 0, 0 );
			if( 1 == retval )
			{	// See if new Base Amount is valid 
				if ( SaveAdjust( NEWBASE ) )
				{
					// Yes; set status to indicate okay 
					status = True;

					// Exit to return
					break;
				}
						
				// Redo input 
			}
			else if( 3 != retval )
			{	// Entry canceled 
				status = False;

				// Exit to return;
				break;
			}
			else
			{	// Go back to previous entry
				break;
			}
		}
	} while ( 3 == retval );

	// Return the status
	return( status );
}



//-----------------------------------------------------------------------------
//  InputAdjBar         Get and save the Adjust Bar Amount
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool        True  - okay to continue
//                                  False - cancel/abort the transaction.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InputAdjBar( void )
{
	Bool status;
	UBYTE retval;

	// Add check for Nova Host here 

	// See if additional Amount prompts allowed 
	if ( !AAPAllowed(  ) )
	{
		// No; return okay 
		return ( True );
	}

	// Loop till done 
	do
	{
		// Move the Bar Amount into the global custom message 
		CustomAmount( AmountBar, NEWBAR );

		// Notify the user 


		// See if this is the correct Amount 
		switch ( YesNoEntry( &YNEntry ) )
		{
			case 0:			// Timed out 
			case CANCEL_KY:	// CANCEL key pressed 
				// Return cancel/abort the Transaction 
				return ( False );
			case ENTER_KY:		// YES key was pressed 
				// Return okay 
				return ( True );
		}

		// Do until done 
		while( True )
		{
			// Set clear key 
			GlobalDsp = CLRKEYON;

			// Get new Bar Amount 
			retval = AmountEntry( &AdjEntry, 0, 0 );
			if( 1 == retval )
			{	// See if new Bar Amount is valid 
				if ( SaveAdjust( NEWBAR ) )
				{
					// Yes; set status to indicate okay 
					status = True;

					// Exit to return
					break;
				}
				
				// Redo input 
			}
			else if( 3 != retval )
			{	// Entry canceled
				status = False;

				// Exit to return
				break;
			}
			else
			{	// Go back to previous entry
				break;
			}
		}
	} while( 3 == retval );

	// Return the status
	return( status );
}



//-----------------------------------------------------------------------------
//  InputAdjTax         Get and save the Adjust Tax Amount
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool        True  - okay to continue
//                                  False - cancel/abort the transaction.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool InputAdjTax( void )
{
	Bool status;
	UBYTE retval;

	// Add check for Nova Host here 

	// See if additional Amount prompts allowed 
	if ( !AAPAllowed(  ) )
	{
		// No; return okay 
		return ( True );
	}

	
	// Loop till done 
	do
	{
		// Move the Tax Amount into the global custom message 
		CustomAmount( AmountTax, NEWTAX );

		// See if this is the correct Amount 
		switch ( YesNoEntry( &YNEntry ) )
		{
			case 0:			// Timed out 
			case CANCEL_KY:	// CANCEL key pressed 
				// Return cancel/abort the Transaction 
				return ( False );
			case ENTER_KY:		// YES key was pressed 
				// Return okay 
				return ( True );
		}

		// Do until done 
		while( True )
		{
			// Set clear key 
			GlobalDsp = CLRKEYON;

			// Get new Tax Amount 
			retval = AmountEntry( &AdjEntry, 1, 0 );
			if( 1 == retval )
			{	// See if new Tax Amount is valid 
				if ( SaveAdjust( NEWTAX ) )
				{
					// Yes; set status to indicate okay 
					status =  True;

					// Exit to return
					break;
				}
				
				// Redo input 
			}
			else if( 3 != retval )
			{	// Entry canceled 
				status = False;

				// Exit to return
				break;
			}
			else
			{	// Go back to previous entry
				break;
			}
		}
	} while( 3 == retval );

	// Return the stauts
	return( status );
}



//-----------------------------------------------------------------------------
//  InputAdjTip         Get and savt the Adjust Tip Amount
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool        True  - okay to continue
//                                  False - cancel/abort the transaction.
//  Notes:
//
//-----------------------------------------------------------------------------
// kaldýr @a 
static Bool InputAdjTip( void )
{
	Bool status;
	UBYTE retval;

	// See if additional Amount prompts allowed 
	if ( !AAPAllowed(  ) )
	{
		// No; return okay 
		return ( True );
	}

	// Loop till done 
	do
	{
		// Move the Tip Amount into the global custom message 
		CustomAmount( AmountTip, NEWTIP );


		// See if this is the correct Amount 
		switch ( YesNoEntry( &YNEntry ) )
		{
			case 0:			// Timed out 
			case CANCEL_KY:	// CANCEL key pressed 
				// Return cancel/abort the Transaction 
				return ( False );
			case ENTER_KY:		// YES key was pressed 
				// Return okay 
				return ( True );
		}

		// Do until done 
		while( True )
		{
			// Set clear key 
			GlobalDsp = CLRKEYON;

			// Get new Tip Amount 
			retval = AmountEntry( &AdjEntry, 1, 0 );
			if( 1 == retval )
			{	// See if a non-blank Tip was entered 
				if ( Dspbuf[0] )
				{
					// Yes; set new status accordingly 
					NEWTRST1 |= TS1_TIPENT;
				}

				// See if new Tip Amount is valid 
				if ( SaveAdjust( NEWTIP ) )
				{
					// Yes; set status to indicate okay 
					status =  True ;

					// Exit to return
					break;
				}
	
				// Redo input 
			}
			else if( 3 != retval )
			{	// Entry canceled 
				status = False;

				// Exit to return
				break;
			}
			else
			{	// Go back to previous entry 
				break;
			}
		}
	} while( 3 == retval );

	// Return the status
	return( status );
}



//-----------------------------------------------------------------------------
//  CompleteAdjust          All the new Totals have been entered, now it is
//                          time to store the changes in the Journal
//
//  Parameters:             AdjIdent - Transaction Journal Identifier
//
//  Global Inputs:          
//
//  Returns:                Bool    True  - okay to continue
//                                  False - cancel/abort the transaction.
//  Notes:
//
//-----------------------------------------------------------------------------

static Bool CompleteAdjust( UWORD AdjIdent )
{
	UBYTE Update;
	Bool Status;
    UWORD LocalJrnIdent;

	// Set the transaction key to adjust while checking if  
	// pinpad is required.  TRINP will be updated later     
	// so it is not necessary to restore the TRKEY.         
	TRINP.TRKEY = ADJUST;

	// Set the new total for use by the pin pad code.   
	// TRINP will be updated later anyway.              
	memcpy( TRINP.TRTOTAM, NEWTOTAM, S_TRTOTAM );

	// Get the pin from the pinpad if required. 
	if ( !GetPIN( ) )
		return ( False );

	// Get control of the Journal 
	JournalLock ();

    // Get the ident
    LocalJrnIdent = AdjIdent;

	// Read data into TRINP 
	// Journal read okay ? 
	if ( JournalRead ( LocalJrnIdent, &TRINP ) )
	{
		// Yes; set update flag to include Amounts and flags 
		Update = 1;

		// New Tip Amount ? 
		if ( memcmp( TRINP.TRTIP, NEWTIP, S_TRTIP ) )
		{
			// Yes; add check for Concord Host here 

			// A non-blank Tip was entered 
			NEWTRST1 |= TS1_TIPENT;

			// Update the Transaction status 
			TRINP.TRSTATUS[0] = NEWTRST1;
		}
		else
		{
			// No; new Total Amount ? 
			if ( !memcmp( TRINP.TRTOTAM, NEWTOTAM, S_TRTOTAM ) )
			{
				// No; new Base Amount ? 
				if ( !memcmp( TRINP.TRBASE, NEWBASE, S_TRBASE ) )
				{
					// No; new Tax Amount ? 
					if ( !memcmp( TRINP.TRTAX, NEWTAX, S_TRBASE ) )
					{
						// No; (Note NEWBAR does not need to be checked) 

						// If TRSTATUS changes it is because of
						// TS1_TIPENT 
						if ( TRINP.TRSTATUS[0] == NEWTRST1 )
						{
							// No change, don't update Journal 
							Update = 0;
						}
						else
						{
							// Save the new status in the Journal 
							TRINP.TRSTATUS[0] = NEWTRST1;

							// Set update flag for status only 
							Update = 2;
						}
					}
				}
			}
		}

		// See if update needed 
		if ( Update )
		{
			// Yes; see if update Amounts and flags 
			if ( 1 == Update )
			{
				// Yes; update Amounts and flags 
				CmplAdjustAmts(  );
			}

			// Update the Journal 
			LocalJrnIdent = JournalUpdate ( &TRINP, LocalJrnIdent );
		}
	}

	// Get the status 
	if ( 0x0000 == LocalJrnIdent )
	{
		Status = False;
	}
	else
	{
		Status = True;

		// Update AdjIdent with possible new value.  
		AdjIdent = LocalJrnIdent;
	}

	// Release the Journal 
	JournalRelease ();

	// See if status okay 
	if ( Status )
	{
		// Yes; notify the user 
		// Display "TRANSACTION ACCEPTED" on upper line 
		ShowInfoMsg( TransAccepted, N_Response );

		// Notify the user 
		SDK_BeepIt( 2 );

		// Delay for two seconds 
		SDK_Wait( ONESECOND * 2 );
	}
	else
	{
		// No; notify the user 
		ShowErrMsg( InvNumberMissing );
	}

	// Return the status 
	return ( Status );
}


//-----------------------------------------------------------------------------
//  CondFirstTip        Should the Tip be prompted for first in Adjust ?
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool        True  - Yes
//                                  False - No.
//
//  Notes:      If a Tip Amount does not exist, and we have Tip
//              Processing turned on, then the user probably
//              entered Adjust to add a Tip.
//-----------------------------------------------------------------------------
// @kaldýr @a
static Bool CondFirstTip( void )
{
	
	
}

//-----------------------------------------------------------------------------
//  CmplAdjustAmts      Complete Adjust Amounts
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:
//
//-----------------------------------------------------------------------------

static void CmplAdjustAmts( void )
{
	// See if original Amount set already 
	if ( NullComp( ( char * ) TRINP.TRORIGAMT, S_TRBASE ) )
	{
		// No; see if Advice outstanding already 
		if ( !( TRINP.TRSTATUS[0] & ( TS1_ADV + TS1_ADJ ) ) )
		{
			// No; save old Total 
			memcpy( TRINP.TRORIGAMT, TRINP.TRTOTAM, S_TRBASE );
		}
	}

	// Move from work area back to Journal - Total Amount 
	memcpy( TRINP.TRTOTAM, NEWTOTAM, S_TRTOTAM );

	// Copy the new Base Amount into the Transaction's Base Amount
	// Field so it can be printed on the Receipt (it is not stored in
	// the Journal) 
	memcpy( TRINP.TRBASE, NEWBASE, S_TRBASE );

	// Move from work area back to Journal - Tax Amount 
	memcpy( TRINP.TRTAX, NEWTAX, S_TRBASE );

	// Move from work area back to Journal - Tip Amount 
	memcpy( TRINP.TRTIP, NEWTIP, S_TRTIP );

	// Move from work area back to Journal - Bar Amount 
	memcpy( TRINP.TRBAR, NEWBAR, S_TRBASE );

	// Mark as Adjusted 
	TRINP.TRSTATUS[0] |= TS1_ADJ;

}
