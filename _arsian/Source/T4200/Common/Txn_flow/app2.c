
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     app2.c
//  Functions used on the second generic application page
//
//=============================================================================

#include "string.h"

#ifdef MAKE_VIKING
#include "eftos.h"
#endif // MAKE_VIKING
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
#include "pinpad.h"
#include "tables.h"
#include "menu.h"
#include "hstprint.h"
#include "receipt.h"
#include "app1.h"
#include "amtproc.h"
#include "flaghost.h"
#include "cvv2proc.h"
#include "pinutils.h"
#include "respdsp.h"
#include "predial.h"
#include "acctproc.h"
#include "addprmpt.h"

#include "app2.h"

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

static const ENTRY_STR TillNumberEntry = {
	CustomMsg,					// Issuer Tran 
	TillNum,					// Prompt 
	N_NullStr,					// Default value 
};


static const ENTRY_STR LastRespEntry = {
	N_LastRsp,					// Title 
	CustomMsg,					// First line 
	CustomMsg2,					// Second line 
};

static const ENTRY_STR NoLastResp = {
	N_LastRsp,					// Title 
	N_NullStr,					// First line 
	N_NoData,					// Second line 
};



//=============================================================================
// Public function definitions
//=============================================================================





//-----------------------------------------------------------------------------
//!  \brief
//!     The function that handles the SAL+CASH transaction
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     We never come here directly.  Always redirected from
//!     App1SALE() or debit().  AcctProc() has already been called.
//!
//-----------------------------------------------------------------------------
extern void SalCash( void )
{
	CurrentEvent.NEXTEVENT = 0;

	// Set transaction to SALCASH 
	TRINP.TRKEY = SALCASH;

	// Test for journal full condition  
	if ( JournalFullTest(  ) )
		return;

	/* Card data may already exist. */
	if( NullComp( (char *)TRINP.TRPAN, S_TRPAN ) )
	{
		/* Use common routine to get account data from card or kbd. */
		if( !ReadData() )
			return;
	}

	/* validate account number - status stored in global chRetval*/
	if ( !AcctProc () )
		return;

#ifdef MAKE_EMV
	// check if readdata has set the flag for ICC inserted, in 
	// which case we can just jump into the EMV_SALCASH transaction
	// with the card inserted.
	if ( ChipCardInserted )
	{
		CurrentEvent.NEXTEVENT = EMV_SALCASH;
		return;
	}
#endif	// MAKE_EMV

	// See if card expired  
	if ( !CheckCardExp(  ) )
		return;

	/* Get card Issue Number  */
	if(!CheckIssueNum())       /* CANCEL_KY key pressed? */
    	return;

	// See if we display track 2 data 
	if ( !CheckDisplaySwipe( ) )
		return;

	// Get shift number if shift processing is enabled. 
	InShift(  );

	// Get Cashier Number; do not allow NULL entry 
	if ( !InputCashier( CustomMsg, False ) )
	{
		// User cancel; return 
		return;
	}

	// If possible, Prompt to get the CVV2 input data.  
	if ( !CVV2Proc () )
		return;

	// Do amount entry 
	if ( !AmtProc () )
		return;

	// Cash only? 
	if ( NullComp( ( char * ) TRINP.TRBASE, S_TRBASE ) )
	{
		CurrentEvent.NEXTEVENT = DBCASH;
		return;
	}

	// enter descriptor 
	if( !DoDesc() )
		return;

	// enter invoice number 
	if ( InInvoiceNum(  ) )
		return;

	// Additional prompts 
	if ( !AdditionalPrompt(  ) )
		return;

	// check for duplicate transactions 
	if ( InDupCheck(  ) )
		return;

	// Perform a Pre-Dial if allowed 
	PreDial ();

	// Try up to 3 times to enter the proper PIN 
	do
	{
		// Get PIN from PIN PAD, if required 
		if ( !GetPIN () )
		{
	 		/* Finish receipt printing, if preprint is enabled */
	 		if ( (AQ4_PREPRINT & (TRINP.TRAQPTR->AQOPT4)) && TRINP.TRPINRETRY )
	 			Receipt ();

			return;
		}

		ShowIssuer_Tran(  );

		// Set flags to print the receipt header and allow user cancel. 
		FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;

		// Flag the host process  
		FlagHost ();

		// Save host response text in terminal flag table.  
		SaveResponseText ();

		// Display Response 
		RespDisplay(  );

	}
	while ( ( TRINP.TRRSPOPT & R_BADPIN ) && ( TRINP.TRPINRETRY++ < 2 ) );

//***************************************************************************/
//      PRINT RECEIPT                                                       */
//***************************************************************************/

	Receipt ();

}



//-----------------------------------------------------------------------------
//!  \brief
//!     The function that handles the CASH transaction
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     We never come here directly.  Always redirected from
//!     SAL+CSH.  AcctProc() has already been called. 
//!
//-----------------------------------------------------------------------------
extern void DbCash( void )
{
	CurrentEvent.NEXTEVENT = 0;

	// Set transaction to SALCASH 
	TRINP.TRKEY = DBCASH;

	/* Card data may already exist. */
	if( NullComp( (char *)TRINP.TRPAN, S_TRPAN ) )
	{
		/* Use common routine to get account data from card or kbd. */
		if ( !ReadData() )
			return;
	}	// if( NullComp( (char *)TRINP.TRPAN, S_TRPAN ) )

	/* validate account number - status stored in global chRetval*/
	if ( !AcctProc () )
		return;

#ifdef	MAKE_EMV
	// check if readdata has set the flag for ICC inserted, in 
	// which case we can just jump into the EMV_CASH transaction
	// with the card inserted.
	if ( ChipCardInserted )
	{
		CurrentEvent.NEXTEVENT = EMV_CASH;
		return;
	}
#endif	// MAKE_EMV

	/* This is the point when we know Acquirer/Issuer */
    // Cash only txn allowed by issuer?
	if ( !((TRINP.TRISPTR->ISOPT3) & IS3_CASH_TRN) )
	{
		/* No, show error and abort */
		ShowErrMsg( N_CashNotAllowed );
		return;
	}

	/* Put the Transaction Name into the Custom Message buffer */
	ShowIssuer_Tran( );

	/* See if card expired  */
	if ( !CheckCardExp(  ) )
		return;

	/* Get card Issue Number  */
	if(!CheckIssueNum())       /* CANCEL_KY key pressed? */
    	return;

	/* See if we display track 2 data */
	if ( !CheckDisplaySwipe( ) )
		return;

	/* Get shift number if shift processing is enabled. */
	InShift();

    /* Do amount entry */
	if ( !AmtProc () )
		return;

	// enter descriptor 
	if( !DoDesc() )
		return;

	// enter invoice number 
	if ( InInvoiceNum(  ) )
		return;

	// Additional prompts 
	if ( !AdditionalPrompt(  ) )
		return;

	// check for duplicate transactions 
	if ( InDupCheck(  ) )
		return;

	// Perform a Pre-Dial if allowed 
	PreDial ();

	// Try up to 3 times to enter the proper PIN 
	do
	{
		// Get PIN from PIN PAD, if required 
		if ( !GetPIN () )
		{
	 		/* Finish receipt printing, if preprint is enabled */
	 		if ( (AQ4_PREPRINT & (TRINP.TRAQPTR->AQOPT4)) && TRINP.TRPINRETRY )
	 			Receipt ();

			return;
		}

		ShowIssuer_Tran(  );

		// Set flags to print the receipt header and allow user cancel. 
		FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;

		// Flag the host process  
		FlagHost ();

		// Save host response text in terminal flag table.  
		SaveResponseText ();

		// Display Response 
		RespDisplay(  );

	}
	while ( ( TRINP.TRRSPOPT & R_BADPIN ) && ( TRINP.TRPINRETRY++ < 2 ) );

//***************************************************************************/
//      PRINT RECEIPT                                                       */
//***************************************************************************/

	Receipt ();

}



//-----------------------------------------------------------------------------
//!  \brief
//!     The function that handles the Balance Inquiry transaction
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void BalInq( void )
{
	TRINP.TRKEY = BALINQ;

	CurrentEvent.NEXTEVENT = 0;

	// Use common routine to get account data from card or kbd. 
	if ( !ReadData(  ) )
		return;

	// validate account number in Dspbuf - status stored in global chRetval 
	// Also moves account number to TRINP.TRPAN                            
	if ( !AcctProc () )
		return;

#ifdef	MAKE_EMV
	// check if readdata has set the flag for ICC inserted, in 
	// which case we can just jump into the EMV_REFUND transaction
	// with the card inserted.
	if ( ChipCardInserted )
	{
        ShowErrMsg( TranNotAllowed );
		return;
	}
#endif	//MAKE_EMV

    // See if card expired  
	if ( !CheckCardExp(  ) )
		return;

	// See if we display track 2 data 
	if ( !CheckDisplaySwipe( ) )
		return;

	// Perform a Pre-Dial if allowed 
	PreDial ();

	// Only 1 chance to enter the proper PIN 
	// Get PIN from PIN PAD, if required 
	if ( !GetPIN () )
		return;

	ShowIssuer_Tran(  );

	// Set flags to print the receipt header and allow user cancel. 
	FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;

	// Flag the host process  
	FlagHost ();

	// Save host response text in terminal flag table.  
	SaveResponseText ();

	// Display Response 
	BalanceRespDisplay(  );

//***************************************************************************/
//      PRINT RECEIPT                                                       */
//***************************************************************************/

	Receipt ();
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Print Till Report
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void DoPrtTill( void )
{
	// Set the transaction key in the TRINP structure 
	TRINP.TRKEY = CurrentEvent.TRNKEY;

	CurrentEvent.NEXTEVENT = 0;

	// See if Till feature is on 
	if ( TCONF.TCOPT1 & TC1_TILL )
	{
		// Set title 
		memcpy( CSTMSG, CurrentEvent.TRNTEXT,
				sizeof( CurrentEvent.TRNTEXT ) );

		// Prompt for Till Number 
		if ( NumEntry( 1, S_TRTILLNO, 1, &TillNumberEntry ) )
		{
			// Okay; fill holder for Till Number with spaces 
			memset( ( char * ) TRINP.TRTILLNO, ' ',S_TRTILLNO );

			// Save Till Number 
			MoveTillZero( ( char * ) TRINP.TRTILLNO, Dspbuf, S_TRTILLNO );

			// Notify user -- "PRINTING, WAIT..." 
			ShowInfoMsg( PrintingWait, PleaseWait );

			// Print Till Report 
			PrintTill ();
		}
	}
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Display last response data. Cross called when LASTRSP key pressed
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void LastRsp( void )
{
	// Verify there is data to display. Host text is 40 bytes. 
	if ( NullComp( ( char * ) TFTAB.TFHOSTTEXT, 20 ) )
	{
		// Clear the Custom messages 
		ClrCstMsgs(  );

		ContinueEntry( &NoLastResp );
	}
	else
	{							// There is data to display 

		// Display first 20 bytes on upper line.  
		memcpy( CSTMSG, TFTAB.TFHOSTTEXT, 20 );
		CSTMSG[20] = '\0';

		// Display second 20 bytes on lower line if it exists.  
		CSTMSG2[0] = '\0';
		if ( !SpaceComp( ( char * ) &TFTAB.TFHOSTTEXT[20], 20 ) )
		{
			memcpy( CSTMSG2, &TFTAB.TFHOSTTEXT[20], 20 );
			CSTMSG2[20] = '\0';
		}

		if ( 1 == ContinueEntry( &LastRespEntry ) )
			ButtonId = B_PREVIOUS;
		else
			ButtonId = CANCEL_KY;
	}
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Load DefaultTitle with acquired and transaction
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void SetDefaultTitle( void )
{
	UBYTE length;
	int title_limit;
	UBYTE	 tmpISNAME[S_ISNAME];

	// Get Font size 
	CHARSIZE charsz;
	SDK_GuiGetCharSize( &charsz );

	// Maximum characters for the line 
	title_limit = ( TITLE_F_W - ( TEXT_INDENT * 2 ) ) / ( charsz.Width );

	// Maximum characters for the line 

	// Fill the DefaultTitle with spaces 
	memset( DefaultTitle, ' ', ( UWORD ) title_limit );
	DefaultTitle[title_limit] = 0;	// Null terminate 

	/* Move the issuer name into local buffer */
	MoveItIss( tmpISNAME, (const char *)TRINP.TRISPTR->ISNAME, S_ISNAME );
	/* Put the issuer name in, left justified */
	MoveTillZero( DefaultTitle, (const char *)tmpISNAME, S_ISNAME );

	// Put the transaction in, right justified 
	length = StrLn( CurrentEvent.TRNTEXT, sizeof( CurrentEvent.TRNTEXT ) );

	// The text offset 
	memcpy( &DefaultTitle[title_limit - length], CurrentEvent.TRNTEXT,
			length );
}


extern void Logon(void)
{
  TRINP.TRKEY = LOGON;
  CurrentEvent.NEXTEVENT = 0;
  

   TRINP.TRAQID = AQTAB[0].AQID;
   TRINP.TRAQPTR = (struct acquirer_rec *)&AQTAB;

   //if ( InInvoiceNum(  ) )
   // return;

   FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;

   // Notify host packet is ready and wait for response.   
   FlagHost(  );

   // Save host response text in terminal flag table.  
   SaveResponseText(  );

   // Display Response 
   RespDisplay ();  
}




