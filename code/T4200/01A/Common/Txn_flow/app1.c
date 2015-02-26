//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     app1.c
//  Functions used on the first generic application page
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkgui.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "fixdata.h"
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
#include "showtrk2.h"
#include "journal.h"
#include "receipt.h"
#include "prttable.h"
#include "amtproc.h"
#include "flaghost.h"
#include "cvv2proc.h"
#include "pinutils.h"
#include "respdsp.h"
#include "predial.h"
#include "acctproc.h"
#include "rspinit.h"
#include "icepak.h"
#include "initcryp.h"
#include "inputpwd.h"
#include "validexp.h"
#include "inpmpswd.h"
#include "tabutil.h"
#include "chkbatch.h"
#include "addprmpt.h"
#include "manustd.h"
#include "manuissn.h"
#include "manuexp.h"
#include "validstd.h"

#ifdef	MAKE_EMV
#include "emv_l2.h"
#include "emvdata.h"
#endif	// MAKE_EMV

#include "forceSettlement.h"
#include "checkCurrency.h"
#include "loyalty.h"

#include "app1.h"


//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================



//=============================================================================
// Private function declarations
//=============================================================================
static void salofflCommon( void );
static Bool AcctCheck( void );
static Bool CheckPinEnabled( void );
static UBYTE CheckCardStartDate( void );
static UBYTE CheckServiceCode( void );
static void SetRejectStatus(void);
static Bool CheckDBCASH_SALCASH(void);

#ifdef	MAKE_EMV
static enum transactions CvtEMVTranKey( enum transactions TranKey );
#endif	// MAKE_EMV

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

static const ENTRY_STR PONumEntry = {
	EnterPONum,					// Prompt 
	N_NullStr,					// Default Value 
	N_NullStr,					// Not used 
};

static const ENTRY_STR ApprovalCodeEntry = {
	N_NullStr,					// Default Value 
	EnterApp,					// Prompt 
	N_NullStr,					// Not used 
};

static const ENTRY_STR CardPresentEntry = {
	N_NullStr,					// Default Title 
	N_NullStr,					// Blank Line (line 1) 
	Msg_CardPresent,			// Card Present? (line 2) 
};

static ENTRY_STR YNEntry;


//=============================================================================
// Public function definitions
//=============================================================================






//-----------------------------------------------------------------------------
//!  \brief
//!     The function that really handles the SALE transaction.
//!     There are checks to determine if this should be DEBIT
//!     , which if True will exit and begin processing
//!     in the respective routines.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Globals:  Dspbuf -  Where the Amount string is stored
//-----------------------------------------------------------------------------
extern void APP1SALE( void )
{
	static UBYTE retval;
	// If the terminal requires initialization, do not allow sale transactions 
	if ( 0x00 == TERM.TERMInit )
		return;
    TRINP.TRKEY = SALE;

    CurrentEvent.NEXTEVENT = 0;
    if (ForceSettlement())
    {
        return;
    }

	YNEntry.TitleMid = N_NullStr;
	YNEntry.Line1Mid = N_NullStr;
	YNEntry.Line2Mid = CorrectYesNo;

	ShowIssuer_Tran(  );
	// SALE initiated by a key press? 
	if ( INJECTKEY )
	{
		retval = NumEntry( INJECTKEY, 19, 6, &CardNumberEntry );
		if ( !retval )
		{
			CurrentEvent.NEXTEVENT = 0;	// Back to idle 
			return;				// Timed out or canceled 
		}
	}
	else if ( !CurrentEvent.NEXTEVENT )	// From Newtrans button selection 
	{
		// See if card swiped from idle     
		if ( ( DEV_CARDRDR != GetLastInputType( ) ) && ( DEV_PINCARD != GetLastInputType( ) ) )
		{
				// Use common routine to get account data from card 
				// or kbd. 
				if ( !ReadData(  ) )
					return;
		}
	}

	CurrentEvent.NEXTEVENT = 0;

	// validate account number - status stored in global chRetval 
	if ( !AcctProc () )
		return;

    // elle giriþ izni kontrolü 
	if ( INJECTKEY  && !((TRINP.TRISPTR->ISOPT1) & IS1_MANPAN)  ) 
	{           
    //       ShowErrMsg( Msg_NoManEntry );
           CurrentEvent.NEXTEVENT = 0;	// Back to idle 
           return;	         
	}

#ifdef MAKE_EMV
	// check if readdata has set the flag for ICC inserted, in 
	// which case we can just jump into the EMV_SALE (or EMV_CASH) transaction
	// with the card inserted.
	if ( ChipCardInserted )
	{
		CurrentEvent.NEXTEVENT = (TC2_CASHONLY & TCONF.TCOPT2) ? EMV_CASH : EMV_SALE;
		return;
	}
#endif	// MAKE_EMV


    if ( !CheckCurrency() ) 
        return;

	if ( !AcctCheck(  ) )
			return;

/*	if ( !((TRINP.TRISPTR->ISOPT2) & IS2_CAPTURE) )   
	{
		if ( !AcctCheck(  ) )
			return;
		CurrentEvent.NEXTEVENT = AUTH;
		return;
	}
*/
	// Test for journal full condition  
	if ( JournalFullTest(  ) )
		return;

	ShowIssuer_Tran(  );

	// See if card expired  
	if ( !CheckCardExp(  ) )
		return;

	/* See if card is valid  */
	if (!CheckCardStartDate())
		return;

	/* Get card Issue Number  */
	if(!CheckIssueNum())       /* CANCEL_KY key pressed? */
		return;

    // service kod konrolu eklenecekse buraya eklenmeli 
    ForcedOnline = True;

/*	
	if(!CheckServiceCode())       
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
*/
	// If possible, Prompt to get the CVV2 input data.  
	if ( !CVV2Proc () )
		return;

	// Perform a Pre-Dial if not manually entered.
	if ( !INJECTKEY )
		PreDial ();

	// Accept amount entry - status stored in global chRetval 
	if ( !AmtProc () )
		return;

    //if ( !Discount() )
    //      return;

	if ( InInvoiceNum(  ) )
		return;

    /*
	// Perform a Pre-Dial if allowed 
	PreDial ();

	if( !DoDesc() )
		return;

	if ( InInvoiceNum(  ) )
		return;


	// Additional prompts is handled here 
	if ( !AdditionalPrompt(  ) )
		return;

	if ( InDupCheck(  ) )
		return;
    */
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

		// Notify host packet is ready and wait for response.   
		FlagHost(  );

		// Save host response text in terminal flag table.  
		SaveResponseText(  );

		// Display Response 
		RespDisplay ();
	}
	while ( ( TRINP.TRRSPOPT & R_BADPIN ) && ( TRINP.TRPINRETRY++ < 2 ) );

//***************************************************************************/
//      PRINT RECEIPT                                                       */
//***************************************************************************/

	Receipt ();
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Accept descriptor selection
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Globas:  chRetval    0 - User cancelled the entry.
//!                          1 - Descriptor(s) selected successfully 
//-----------------------------------------------------------------------------
extern UBYTE DoDesc( void )
{
	int i, cnt;
	UBYTE chRetval;

	/* Enable */
	if ( !((TRINP.TRISPTR->ISOPT1) & IS1_DESC) )
	{
		/* No, exit */
		chRetval = 1;
		return chRetval;
	}

	// Preset return value for case where no descriptors exist. 
	chRetval = 1;

	// Added the following two sections to handle zero descriptor or one
	// descriptor situation. 

	cnt = 0;
	for ( i = 0; i < DSMAX; i++ )
	{
		if ( DSTAB[i].DSKEY )
			cnt++;
	}

	if ( !cnt )
		return chRetval;
	else
	{
		if ( 1 == cnt )
		{
			for ( i = 0; i < DSMAX; i++ )
			{
				if ( DSTAB[i].DSKEY )
				{
					TRINP.TRDESC[0] = DSTAB[i].DSKEY;
					return chRetval;
				}
			}
		}
	}

	// Get message to display on first line 
	GetMsg( EntDescrip, CSTMSG );

	// Do NOT display Second line of text 
	Enable_Flags[3] = 0;

	DrawScreen( DESCRIPTOR_SCR );

	chRetval = 2;
	cnt = 0;
	while ( 2 == chRetval )
	{
		chRetval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

		// check for cancel or time-out 
		if ( ( CANCEL_KY == chRetval ) || ( 0 == chRetval ) )
			chRetval = 0;
		else
		{
			if ( ( ENTER_KY == chRetval ) || ( CLEAR_KY == chRetval ) ||
				 ( BSP_KY == chRetval ) )
			{
				// If no descriptor selected yet, ENTER and CLEAR are not
				// allowed. The user can select up to 4 descriptors. 

				if ( !cnt )
				{
					chRetval = 2;
					SDK_Beeper( TENMS * 40 );
				}
				else
				{
					if ( ENTER_KY == chRetval )
						chRetval = 1;
					else
					{
						// User rejected the last selection, DEC the counter,
						// clear the descriptor key in TRINP and restart. 
						chRetval = 2;
						TRINP.TRDESC[--cnt] = 0;
						if ( !cnt )	// No descriptor, then blank out 2nd line 
						{
							Enable_Flags[3] = 0;
						}
						else
						{
							for ( i = 0; i < DSMAX; i++ )
							{
								// Display the previous descriptor 
								if ( DSTAB[i].DSKEY == TRINP.TRDESC[cnt - 1] )
								{
									memset( ( UBYTE * ) CSTMSG2, 0, sizeof( CSTMSG2 ) );
									memcpy( CSTMSG2, DSTAB[i].DSTXT,
											S_DSTXT );

									// Do display Second line of text 
									Enable_Flags[3] = 1;

									break;
								}
							}
						}

						// Draw the screen with/without Descriptor 
						DrawScreen( DESCRIPTOR_SCR );
					}
				}
			}
			else
			{
				// Maximum of 4 descriptors can be selected 

				if ( cnt < 4 )
				{
					for ( i = 0; i < DSMAX; i++ )
					{
						if ( DSTAB[i].DSKEY == chRetval )
						{
							memset( ( UBYTE * ) CSTMSG2, 0, sizeof( CSTMSG2 ) );
							memcpy( CSTMSG2, DSTAB[i].DSTXT, S_DSTXT );
							// Do display Second line of text 
							Enable_Flags[3] = 1;
							// Draw the screen with Descriptor 
							DrawScreen( DESCRIPTOR_SCR );
							TRINP.TRDESC[cnt++] = DSTAB[i].DSKEY;
							chRetval = 2;
							break;
						}
					}
				}

				if ( chRetval != 2 )
				{
					chRetval = 2;
					SDK_Beeper( TENMS * 40 );
				}
			}
		}
	}

	return chRetval;
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Handles the REFUND transaction
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Refund( void )
{

	// Test for journal full condition  
	if ( JournalFullTest(  ) )
		return;

	// Get shift number if shift processing is enabled. 
	InShift(  );

	TRINP.TRKEY = REFUND;

	CurrentEvent.NEXTEVENT = 0;

	// Put the Transaction Name into the Custom Message buffer 
	ShowIssuer_Tran(  );

	// Use common routine to get account data from card or kbd. 
	if ( !ReadData(  ) )
		return;

	// validate account number - status stored in global chRetval 
	if ( !AcctProc () )
		return;
	   
#ifdef	MAKE_EMV
	// check if readdata has set the flag for ICC inserted, in 
	// which case we can just jump into the EMV_REFUND transaction
	// with the card inserted.
	if ( ChipCardInserted )
	{
		CurrentEvent.NEXTEVENT = EMV_REFUND;
		return;
	}
#endif	//MAKE_EMV

	// Moved password prompt inside the NEXTEVENT CHECK.  
	if ( !( TCONF.TCLOCOP & TCL_NO_RPWD ) )
	{
		if ( False == InputPswd( N_NullStr ) )
			return;
	}

	if ( (TRINP.TRISPTR->ISOPT2) & IS2_NOREF )
	{
		ShowErrMsg( RefundNotAllowed );
		return;
	}

	// VOID AUTH is not supported at this stage due to the fact that I cannot
	// get a approval from the host simulator. I've talked to PAM in PHX office
	// and she is getting the same error. Will implement this feature when 
	// requested by someone.  

	ShowIssuer_Tran(  );

	// See if card expired  
	if ( !CheckCardExp(  ) )
		return;

	/* Get card Issue Number  */
	if(!CheckIssueNum())       /* CANCEL_KY key pressed? */
		return;

	/* Check Service Code  */
	if(!CheckServiceCode())       /* CANCEL_KY key pressed or error? */
		return;

	// See if we display track 2 data 
	if ( !CheckDisplaySwipe( ) )
		return;

	// Get Cashier Number; do not allow NULL entry 
	if ( !InputCashier( CustomMsg, False ) )
	{
		// User cancel; return 
		return;
	}

	// Accept amount entry - status stored in global chRetval 
	if ( !AmtProc () )
		return;

	if( !DoDesc() )
		return;

	if ( InInvoiceNum(  ) )
		return;

	// Additional prompts is handled here. 
	if ( !AdditionalPrompt(  ) )
		return;

	if ( InDupCheck(  ) )
		return;

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

		// Set flags to print the receipt header and not allow user cancel. 
		FlagHostCntl = FLAG_PREPRINT;

		// Notify host packet is ready and wait for response.   
		FlagHost(  );

		// Save host response text in terminal flag table.  
		SaveResponseText(  );

		// Display Response 
		RespDisplay ();

	}
	while ( ( TRINP.TRRSPOPT & R_BADPIN ) && ( TRINP.TRPINRETRY++ < 2 ) );

//***************************************************************************/
//      PRINT RECEIPT                                                       */
//***************************************************************************/

	Receipt ();
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Initialize the Terminal
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//-----------------------------------------------------------------------------
extern void Initialize( void )
{
	Bool bPinCardStatus;

	// Confirm the Initialization process 
	YNEntry.TitleMid = N_NullStr;

	YNEntry.Line1Mid = N_NullStr;
	YNEntry.Line2Mid = CorrectYesNo;

	// Don't ask for password if Auto Init
	if ( ( AutoInit ) || ( 0xFE == TERM.TERMInit ) || EnterMPswd( ) )
	{
		// Okay; Initialiaze requested by scheduler 
		// or performing automatic initialize from host indicator 
		// or have user confirm 
		if ( ( AutoInit ) ||
			 ( 0x0FE == TERM.TERMInit ) ||
			 ( ENTER_KY == YesNoEntry( &YNEntry ) ) )
		{
			AutoInit = False;

			TRINP.TRKEY = INITIALIZE;

			// Set initialization in progress flag 
			TFTAB.TFIND1 |= TF1_DOING_INIT;

			SetTFTabLRC(  );

			/* @TUM 22/06/2003 Allow user to cancel the initialisation if we have Recovery mechanism */
			if ( 0xFF != TFTAB.TFMAXTRY )
			{
				FlagHostCntl = FLAG_ALLOW_CANCEL;
			}
			else
			{
				/* Recovery disabled, don't allow cancelling */
				FlagHostCntl = 0;
			}

			// Notify host packet is ready and wait for response.   
			FlagHost(  );

			// Clear the batch if directed by init data. 
			if ( TCONF.TCINIT )
			{
				// Clear the init flag so only clear batch once.    
				TCONF.TCINIT = 0;
				SetTCTabLRC(  );

				// Reformat the journal - unconditionally.  
				JournalFormat ();

				// Initialize the batch table.  
				InitTable( &BATFMT, ( UBYTE * ) BATTAB, AQMAX * BATSZE,
						   &BATLRC );
			}

			// Set printer logical flag to available if configured.   
			if ( TCONF.TCOPT2 & TC2_PRINT )
				PRTSTAT = 0x01;

#ifdef	MAKE_EMV
			ScrdInitIO( );
	
			// Set slot for Atmel cryptography chip - 0 if not present
			InitCryptoProc ();

			EMVVersion = EMVL2_v4;		// 1 = EMV311, 2 = EMV4
			EMVL2_Initialise( EMVVersion, CryptSlot );
		    ChipCardInserted = False;	// checked at txn start
		    PrintEMVDebug = False;		// Disable debug printing
#endif	//MAKE_EMV

			// Check Terminal Tables Consistency 
			CheckTables ();

			// Validate the Printer Type 
			CheckPrinterType ();

			// Set user timeout 
			if ( TCONF.TCOPT6 & TC6_LONGTO )
				UserTimeout = 255;
			else
				UserTimeout = 30;

			// Clear initialization in progress flag 
			TFTAB.TFIND1 &= ~TF1_DOING_INIT;

			SetTFTabLRC(  );

			if ( (TCONF.TCOPT2 & TC2_FONTUPD) && ( 0x0FF == TERM.TERMInit ) )
			{
				// Update the printer font      
				FontInit ();
			}

			if ( ShowDialStatus )
			{
				// Display Response 
				RespDisplay ();

				if ( !( TRINP.TRRSPOPT & R_APPROVED ) )
				{   
					read_io( 0, 0, &bPinCardStatus, &DevFlag );
					read_io( DEV_PAD + DEV_KBD + STAY_OPEN1, UserTimeout, &bPinCardStatus, &DevFlag );
				}
			}
			else
			{
				ShowDialStatus = True;
			}

			// Initialize the pin pad.      
			PinInit ();

			// initialize ICEPAK , fverify() must be called before this
			IcePak_Init ();

			if (TRINP.TRRSPOPT & R_APPROVED)
				// If Batch number was changed in Initialization (Aq tab) change it in Batch tab
				SetBatchNum();
			
		}
	}
}



//-----------------------------------------------------------------------------
//!  \brief
//!     The function handles the Card Verification transaction
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void APP1CARDVER( void )
{
	TRINP.TRKEY = CRDVER;

	CurrentEvent.NEXTEVENT = 0;

	// Use common routine to get account data from card or kbd. 
	if ( !ReadData(  ) )
		return;

	// validate account number - status stored in global chRetval 
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

    if ( ((TRINP.TRISPTR->ISOPT2) & IS2_NOCARDVER ) )
	{
		ShowErrMsg( TranNotAllowed );
		return;
	}

	ShowIssuer_Tran(  );

	// See if card expired  
	if ( !CheckCardExp(  ) )
		return;

	/* Get card Issue Number  */
	if(!CheckIssueNum())       /* CANCEL_KY key pressed? */
		return;

	/* Check Service Code  */
	if(!CheckServiceCode())       /* CANCEL_KY key pressed or error? */
		return;

	// See if we display track 2 data 
	if ( !CheckDisplaySwipe( ) )
		return;

	// Perform a Pre-Dial if allowed 
	PreDial ();

	// validate account number - status stored in global chRetval 
	if ( !AmtProc () )
		return;

	ShowIssuer_Tran(  );

	// Only print the receipt if the configuration option is on. 
	if ( TCONF.TCOPT3 & TC3_PRTVER )
	{
		// Set flags to preprint and don't allow cancel.    
		FlagHostCntl = FLAG_PREPRINT;
	}
	else
	{
		// Don't set the flag to preprint and don't allow cancel.   
		FlagHostCntl = 0;
	}

	// Notify host packet is ready and wait for response.   
	FlagHost(  );

	// Save host response text in terminal flag table.  
	SaveResponseText(  );

	// Display Response 
	RespDisplay ();

//***************************************************************************/
//      PRINT RECEIPT                                                       */
//***************************************************************************/

	// Only print the receipt if the configuration option is on.  
	if ( ( TCONF.TCOPT3 & TC3_PRTVER ) )
	{
		Receipt ();
	}
}



//-----------------------------------------------------------------------------
//!  \brief
//!     The function handles the Offline transaction
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void SalOffl( void )
{
	TRINP.TRKEY = SALOFFL;

	CurrentEvent.NEXTEVENT = 0;

	// Use common routine to get account data from card or kbd. 
	if ( !ReadData(  ) )
		return;

	// validate account number - status stored in global chRetval 
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

	ShowIssuer_Tran(  );

	// See if card expired  
	if ( !CheckCardExp(  ) )
		return;

	/* Get card Issue Number  */
	if(!CheckIssueNum())       /* CANCEL_KY key pressed? */
		return;

	/* Check Service Code  */
	if(!CheckServiceCode())       /* CANCEL_KY key pressed or error? */
		return;

	// See if we display track 2 data 
	if ( !CheckDisplaySwipe( ) )
		return;

	// Test for journal full condition  
	if ( JournalFullTest(  ) )
		return;

	// Get shift number if shift processing is enabled. 
	InShift(  );

	// Get Cashier Number; do not allow NULL entry 
	if ( !InputCashier( CustomMsg, False ) )
	{
		// User cancel; return 
		return;
	}

	// Accept amount entry - status stored in global chRetval 
	if ( !AmtProc () )
		return;

	if( !DoDesc() )
		return;

	if ( InInvoiceNum(  ) )
		return;

	// Do common part of Sale Offline Transaction 
	salofflCommon(  );
}



//-----------------------------------------------------------------------------
//!  \brief
//!     This function is used to indicate an unsupported
//!     function or transaction.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void FuncNotSupported( void )
{
	ShowErrMsg( N_FuncNotSupported );
}



//-----------------------------------------------------------------------------
//!  \brief
//!     The function handles the test transaction
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void TstTran( void )
{
	// Set flags to not print the receipt header and not allow user cancel. 
	FlagHostCntl = 0;

	// Notify host packet is ready and wait for response.   
	FlagHost(  );

	// Display Response 
	RespDisplay ();

}



//-----------------------------------------------------------------------------
//!  \brief
//!     The function handles the Auth transaction
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Auth( void )
{
	TRINP.TRKEY = AUTH;

	CurrentEvent.NEXTEVENT = 0;

	// Card data may already exist if switching because of IS2_CAPTURE. 
	if ( NullComp( ( char * ) TRINP.TRPAN, S_TRPAN ) )
	{
		// Use common routine to get account data from card or kbd. 
		if ( !ReadData(  ) )
			return;
	}

	// validate account number - status stored in global chRetval 
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

	ShowIssuer_Tran(  );

	// See if card expired  
	if ( !CheckCardExp(  ) )
		return;

	/* Get card Issue Number  */
	if(!CheckIssueNum())       /* CANCEL_KY key pressed? */
		return;

	/* Check Service Code  */
	if(!CheckServiceCode())       /* CANCEL_KY key pressed or error? */
		return;

	// See if we display track 2 data 
	if ( !CheckDisplaySwipe( ) )
		return;

	// Get Cashier Number; do not allow NULL entry 
	if ( !InputCashier( CustomMsg, False ) )
	{
		// User cancel; return 
		return;
	}

	// If possible, Prompt to get the CVV2 input data.  
	if ( !CVV2Proc () )
		return;

	// Perform a Pre-Dial if allowed 
	PreDial ();

	// Accept amount entry - status stored in global chRetval 
	if ( !AmtProc () )
		return;

	if( !DoDesc() )
		return;

	if ( InInvoiceNum(  ) )
		return;

	// Additional prompts is handled here. 
	if ( !AdditionalPrompt(  ) )
		return;

	ShowIssuer_Tran(  );

	// Set flags to print the receipt header and allow user cancel. 
	FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;

	// Notify host packet is ready and wait for response.   
	FlagHost(  );

	// Save host response text in terminal flag table.  
	SaveResponseText(  );

	// Display Response 
	RespDisplay ();

//***************************************************************************/
//      PRINT RECEIPT                                                       */
//***************************************************************************/

	Receipt ();
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Save last response text from the host
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void SaveResponseText( void )
{
	// Move the host response to the terminal flag table if response exists. 
	if ( TRINP.TRHOSTTEXT[0] )
	{
		memcpy( TFTAB.TFHOSTTEXT, TRINP.TRHOSTTEXT, S_TRHOSTTEXT );
	}
	else
	{
		memset( ( char * ) TFTAB.TFHOSTTEXT, ' ',S_TRHOSTTEXT );
		if ( TRINP.TRRSPTXT[0] )
		{
			memcpy( TFTAB.TFHOSTTEXT, TRINP.TRRSPTXT, S_TRRSPTXT );
		}
	}

	// Set LRC on TFTAB.  
	SetTFTabLRC(  );
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Check for expired card
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Globas: chRetval Is set to zero if CANCEL_KY or timed out
//!
//-----------------------------------------------------------------------------
extern UBYTE CheckCardExp( void )
{
	UBYTE chRetval;

	do
	{
		// SALE initiated by a key press? If so, get EXP date 
		if ( (INJECTKEY) && ((TRINP.TRISPTR->ISOPT1) & IS1_EXPD) )
		{
			chRetval = ManualExp(  );
			if ( !chRetval )	// CANCEL_KY key pressed? 
				return chRetval;
		}

		if ( ((TRINP.TRISPTR->ISOPT2) & IS2_CH_EXPD) )
			chRetval = ValidateEXP(  );
		else
			chRetval = 1;

		if ( !chRetval )
		{
			ShowErrMsg( ExpiredCard );

			if ( INJECTKEY )
				ShowIssuer_Tran(  );
			else
				return chRetval;
		}
		else					// For Manual entry, we need to check the IMPRINT OPTION 
		if ( INJECTKEY )
		{
			if ( SALE == TRINP.TRKEY || SALOFFL == TRINP.TRKEY )
			{
				if ( TCONF.TCOPT3 & TC3_IMPRINT )
				{

					YNEntry.Line1Mid = TakeImprint;

					chRetval = YesNoEntry( &YNEntry );
					if ( chRetval != ENTER_KY )
					{
						chRetval = 0;
						return chRetval;
					}
				}
			}
		}
	}
	while ( !chRetval );

	return chRetval;
}

//-----------------------------------------------------------------------
//! \brief
//!     Check for prevalid Card
//! \param
//!     None
//! \return
//!     None
//! \note
//!     chRetval is set to zero if CANCEL_KY or timed out.
//-----------------------------------------------------------------------
UBYTE CheckCardStartDate( void )
{
	UBYTE chRetval;

	do
	{
		/* SALE initiated by a key press? If so, get EXP date */
		if ( (INJECTKEY) && ((TRINP.TRISPTR->ISOPT6) & IS6_STARTD) )
		{
			chRetval = ManualStartDate();
			if ( !chRetval )       /* CANCEL_KY key pressed? */
				return chRetval;
		}

		if ( ((TRINP.TRISPTR->ISOPT6) & IS6_CH_STARTD) )
		{
			chRetval = ValidateStartDate();
		}
		else
		{
			chRetval = 1;
		}

		if ( !chRetval )
		{
			ShowErrMsg(PrevalidCard);

			if ( INJECTKEY )
			{
				ShowIssuer_Tran( );
			}
			else
			{
				return chRetval;
			}
		}
	} while( !chRetval );

	return chRetval;
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Check for display of track 2 data
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Globals: chRetval  Is set to zero if CANCEL_KY or timed out
//!
//-----------------------------------------------------------------------------
extern UBYTE CheckDisplaySwipe( void )
{
	UBYTE chRetval;

	// Preset chRetval with a good return.  
	chRetval = 1;

	// If Card Swipe, determine whether we should display track 2 data 
	if ( !INJECTKEY )
	{
		if ( TCONF.TCOPT1 & TC1_SHOWMSR )
		{
			ShowTrk2 ();
			chRetval = YesNoEntry( &YNEntryTrk2 );

			// Clear the Custom messages 
			ClrCstMsgs(  );

			if ( chRetval != ENTER_KY )
			{
				chRetval = 0;
				return chRetval;
			}
		}

		if((TRINP.TRISPTR->ISOPT4) & IS4_PANLAST4)
		{
			chRetval = GetPAN4(  );
			if ( !chRetval )
			{
				ShowErrMsg( InvalidCard );
				return chRetval;
			}
			else if ( CANCEL_KY == chRetval )
			{
				chRetval = 0;
				return chRetval;
			}
			else
				ShowIssuer_Tran(  );
		}
	}
	return chRetval;
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Mail/Phone Order Transaction (Visanet Only)
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void MailOrder( void )
{

	// Test for journal full condition  
	if ( JournalFullTest(  ) )
		return;

	// Get shift number if shift processing is enabled. 
	InShift(  );

	TRINP.TRKEY = MAIL_ORDER;

	CurrentEvent.NEXTEVENT = 0;

	// Put the Transaction Name into the Custom Message buffer 
	ShowIssuer_Tran(  );

	// Use common routine to get account data from card or kbd. 
	if ( !ReadData(  ) ) return;

	// validate account number - status stored in global chRetval 
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

    // Check is the card swiped
	if (0x20 == (TRINP.TRPOSE[1] & 0xf0))
	{
		ShowErrMsg( SwipeNotAllowed );
		return;
	}

	ShowIssuer_Tran(  );

	// See if card expired  
	if ( !CheckCardExp(  ) )
		return;

	TRINP.TRPOSE[1] &= 0x0F;
	TRINP.TRPOSE[1] |= 0x30; // Set Manual Entry without Cardholder

	// See if we display track 2 data 
	if ( !CheckDisplaySwipe( ) )
		return;

	// Get Cashier Number; do not allow NULL entry 
	if ( !InputCashier( CustomMsg, False ) )
	{
		// User cancel; return 
		return;
	}

	// If possible, Prompt to get the CVV2 input data.  
	if ( !CVV2Proc () )
		return;

	// Accept amount entry - status stored in global chRetval 
	if ( !AmtProc () ) return;

	// Perform a Pre-Dial if allowed 
	PreDial ();

	if( !DoDesc() )
		return;

	if ( InInvoiceNum(  ) ) return;

	// Additional prompts is handled here 
	if ( !AdditionalPrompt(  ) ) return;

	if ( InDupCheck(  ) ) return;

	ShowIssuer_Tran(  );

	// Set flags to print the receipt header and allow user cancel. 
	FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;

	// Flag the host process  
	FlagHost(  );

	// Save host response text in terminal flag table.  
	SaveResponseText(  );

	// Display Response 
	RespDisplay ();

	// print the receipt   
	Receipt ();

}




//=============================================================================
// Private function definitions
//=============================================================================




//-----------------------------------------------------------------------------
//!  \brief
//!     The function handles the the Common portion of the
//!     Offline Transaction which is also used by the Sale
//!     Transaction for over Floor Limit processing
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void salofflCommon( void )
{
	UBYTE retval;
	// Additional prompts is handled here. 
	if ( !AdditionalPrompt(  ) )
		return;

	if ( InDupCheck(  ) )
		return;

	ShowIssuer_Tran(  );
	retval = AlphaNumEntry( 1, S_TRAUTH, 2, &ApprovalCodeEntry );

	if ( !retval )
	{
		return;
	}

	memcpy( TRINP.TRAUTH, Dspbuf, S_TRAUTH );

	// Set flag to keep message on the display.  
	DelayIdleDisplay = True;

	// Set flags to print the receipt header and not allow user cancel. 
	FlagHostCntl = FLAG_PREPRINT;

	// Notify host packet is ready and wait for response.   
	FlagHost(  );

	// Save host response text in terminal flag table.  
	SaveResponseText(  );

	// Display Response 
	RespDisplay ();

//***************************************************************************/
//      PRINT RECEIPT                                                       */
//***************************************************************************/

	Receipt ();
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Check for MOD-10, manual entry and batch pending/settled
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Delayed tests when default SALE from idle until the 
//!     correct issuer was determined, debit, etc.
//-----------------------------------------------------------------------------
static Bool AcctCheck( void )
{
	// See if Batch Pending for this Acquirer 
	if ( ChkBatPend(  ) )
	{
		// Yes; notify the user of Batch Pending 
		ShowErrMsg( RetryBatchTrans );
		return ( False );
	}

	// Check if we need to perform MOD-10 verification 
	if ( (TRINP.TRISPTR->ISOPT2) & IS2_MOD10 )
	{
		// Yes; validate the PAN using MOD-10 check 
		if ( !ValidateAcct () )
		{
			ShowErrMsg( InvalidCard );
			CurrentEvent.NEXTEVENT = CurrentEvent.TRNKEY = 0;
			return ( False );
		}
	}

	// If Acct# was entered manually, we should check ISSUER 
	// option byte #1 to see if it is allowed                
	if( (INJECTKEY) && !((TRINP.TRISPTR->ISOPT1) & IS1_MANPAN) )
	{
		ShowErrMsg( NoManEntry );
		return ( False );
	}

	return ( True );
}


//-----------------------------------------------------------------------
//! \brief
//!     Check for Manual Issue Number entry
//! \param
//!     None
//! \return
//!     None
//! \note
//!     chRetval is set to zero if CANCEL_KY or timed out.
//-----------------------------------------------------------------------
UBYTE CheckIssueNum( void )
{
	UBYTE chRetval = 1;

	do
	{
		/* SALE initiated by a key press? If so, get Issue Number */
		if ( (INJECTKEY) && (!(TRINP.TRISSUENUM)) )
		{
			chRetval = ManualIssueNum( );
			if ( !chRetval )       /* CANCEL_KY key pressed? */
				return chRetval;
		}

	} while( !chRetval );

	return chRetval;
}

//-----------------------------------------------------------------------
//! \brief
//!     Check Service Code
//! \param
//!     None
//! \return
//!     None
//! \note
//!     chRetval is set to zero if CANCEL_KY or timed out or error.
//-----------------------------------------------------------------------
UBYTE CheckServiceCode( void )
{
    return 1;
    /* MSD
	UBYTE	CheckDigit;
	Bool	OnlineFlag = False;
	UBYTE chRetval;

	chRetval = 1;

	if (CurrentEvent.TRNOPT2 & T2_OFFLINE)
	{
		return chRetval;
	}

	/* If Service Code check enabled and if not Manual Entry? 
	/* @UVA NEED TO ADD Checking For Not Force transactions		
	/* @TUM 04/08/2003 Added test for Amex ANSI card (where there's no Service Code) 
	if ( (!INJECTKEY) && ((TRINP.TRISPTR->ISOPT6) & IS6_ENASERV) && !(TS2_AMEXANSI & TRINP.TRSTATUS[1]) )
	{
#ifdef	MAKE_EMV
		/* Check if acquirer supports ICC 
		if ( (TRINP.TRAQPTR->AQOPT2) & AQ2_ICCACQ )
 		{
			/* Check if performing ICC Fallback 
 			if ( !(TS2_FALLBACK & TRINP.TRSTATUS[1]) )
 			{
				if( (TRINP.TRSERVCD[0] == 0x02) || (TRINP.TRSERVCD[0] == 0x06) )
				{
					/* Change transaction Key into EMV type 
					TRINP.TRKEY = CvtEMVTranKey(TRINP.TRKEY);

					/* Set chRetval to 0 to switch to a new event 
					chRetval = 0;

					return chRetval;
				}
			}	// if ( !(TS2_FALLBACK & TRINP.TRSTATUS[1]) )
		}
#endif	// MAKE_EMV

		CheckDigit = TRINP.TRSERVCD[1] & 0x0f;

		/*  Check Digit 3 first 
		switch(CheckDigit)
		{
			case 0x05:	/* Reject if no PIN verification available 
					/* Reject if Cash or Purchase with Cashback requested 
				if ( CheckDBCASH_SALCASH() )
				{
					SetRejectStatus();
					/* Abotran 
					chRetval = 0;
				   	break;
				}
			case 0x00:	/* Reject if no PIN verification available 
				/* Accept if PIN pad present 
				if ( CheckPinEnabled( ) ) 
                    break;

			case 0x03:  /* Reject 
				SetRejectStatus();
				/* Abotran 
				chRetval = 0;
				break;

			case 0x06:	/* Accept. Prompt for PIN if PIN supported 
				break;
			case 0x02:	/* Accept. Reject if Cash or Purchase with Cashback requested 
			case 0x07:	/* Reject if Cash or Purchase with Cashback requested 
				if ( CheckDBCASH_SALCASH( ) )
				{
					SetRejectStatus();
					/* Abotran 
					chRetval = 0;
				}
				break;
			case 0x04:	/* Accept only if Cash Advance requested 
				if ( !(TRINP.TRKEY == DBCASH) )
				{
					SetRejectStatus();
					/* Abotran 
					chRetval = 0;
				}
			   	break;
			case 0x08:	/* Online authorisation required 
			case 0x09:	/* Online authorisation required 
				/* Online 
				/* Set Online Flag 
				OnlineFlag = True;
				break;
			case 0x01:	/* Accept 
				break;
		} /* End of switch 3rd digit 

		/* Exit if Reject 
		if ( TRINP.TRSTATUS[1] & TS2_REJECT )
		{
			return chRetval;
		}

		CheckDigit = ( TRINP.TRSERVCD[1] & 0x0f0 ) >> 4;

		/*  Check Digit 2 second 
		switch(CheckDigit)
		{
			case 0x00:	/* Accept 
				break;
			default:	/* Online authorisation required 
				/* Set Online Flag 
				OnlineFlag = True;
		}

		CheckDigit = TRINP.TRSERVCD[0] & 0x0f;

		/*  Check Digit 1 third 
		switch(CheckDigit)
		{
			case 0x01:	/* Accept 
				break;
			default:	/* Online authorisation required 
				/* Set Online Flag 
				OnlineFlag = True;
		}

		if ( OnlineFlag )
		{
			// Force transaction online
			ForcedOnline = True;
		}
	}

	return chRetval;
    */
}

//-----------------------------------------------------------------------
//! \brief
//!     Check if Cash or Purchase with Cashback requested
//! \param
//!     None
//! \return
//!     Bool        True if cash
//!                 False if not
//-----------------------------------------------------------------------
Bool CheckDBCASH_SALCASH( void )
{
	if ( (TRINP.TRKEY == DBCASH) || (TRINP.TRKEY == SALCASH) )
	{
		TRINP.TRSTATUS[1] |= TS2_NOSALCASH;
		return True;
	}
	else
	{
		return False;
	}
}

//-----------------------------------------------------------------------
//! \brief
//!     Set Reject Code
//! \param
//!     None
//! \return
//!     None
//-----------------------------------------------------------------------
void SetRejectStatus(void)
{
    /* MSD
	/* Set Reject Code
	TRINP.TRSTATUS[1] |= TS2_REJECT;
	
	 /*	Msg "Unsupported Card" 
	ShowErrMsg( UnsuppCard );

	/* Beep
	SDK_Beeper( TENMS * 40 );
    */
}

#ifdef	MAKE_EMV

//-----------------------------------------------------------------------
//! \brief
//!     This function will return the equivalent EMV
//!     transaction key corresponding to the given 
//!     transaction key.
//! \param
//!     None
//! \return
//!     transactions    EMV transaction key.
//-----------------------------------------------------------------------
static enum transactions CvtEMVTranKey( enum transactions TranKey )
{
	enum transactions EMVTranKey;

	/* Change transaction key to EMV transaction key */
	switch ( TranKey )
	{
		case SALE :
			EMVTranKey = EMV_SALE;
			break;

 		case REFUND :
 			EMVTranKey = EMV_REFUND;
 			break;

		case SALCASH :
			EMVTranKey = EMV_SALCASH;
			break;

 		case DBCASH :
 			EMVTranKey = EMV_CASH;
 			break;

		default : 
 			EMVTranKey = EMV_SALE;
			break;
	}

	CurrentEvent.NEXTEVENT = EMVTranKey;

	return ( EMVTranKey );
}
#endif	// MAKE_EMV

//------------------------------------------------
//!  \brief
//!     Check PIn verification available
//!  \param
//!     None
//!  \return
//!     Bool        True  - PIN verification available.
//!                 False - PIN verification not available.
//-----------------------------------------------
Bool CheckPinEnabled( void )
{
	/* Does the issuer want PIN entry? */
	if ( !((TRINP.TRISPTR->ISOPT1) & IS1_PIN) )
		return False;									/* No */

	/* Check, if PINPAD enabled */
	if ( !PinTranEnabled () ) return False;						/* No */

	return True;
}


//------------------------------------------------
//!  \brief
//!     Restart the terminal via booter
//!  \param
//!     None
//!  \return
//!     None
//-----------------------------------------------
extern void TerminalQuickReset( void )
{
    // Notify Application Manager to perform Fast Start
    // This setting must be cleared by Application Manager
    NMSIDLL[0] = 'F';
    NMSIDLL[1] = 'S';

    // Do reset terminal
    SDK_TerminalReset ( );
}
extern void PreAuth( void )
{
     if (0x00 == TERM.TERMInit)
          return;
     
     if (!(TCONF.TCOPT3 & TC3_PAUTHPERMIT)) 
     {
	ShowInfoMsg(TerminalNotAllowed,N_Error);
	SDK_Wait(ONESECOND * 3);
	return;
     }

     //if ( ForceSettlement () ) 
     //       return;        

    // Test for journal full condition  
    if ( JournalFullTest(  ) )
          return;
        

    TRINP.TRKEY = PREAUTH;        
    CurrentEvent.NEXTEVENT = 0;

    TRINP.TRPOCC = 0x06;
    CommonTran();
    
}

extern void ClosedPreAuth( void )
{              
     if (0x00 == TERM.TERMInit)
          return;

     if (!(TCONF.TCOPT1 & TC1_CLPREPERMIT)) 
     {
	ShowInfoMsg(TerminalNotAllowed,N_Error);
	SDK_Wait(ONESECOND * 3);
	return;
     }

     //if ( ForceSettlement () ) 
     //       return;
    
    // Test for journal full condition  
    if ( JournalFullTest(  ) )
          return;
    

    TRINP.TRKEY = CLOSED_PREAUTH;        
    CurrentEvent.NEXTEVENT = 0;

    CommonTran();
}


extern void Instal_Refund( void )
{
        UBYTE retval;
static const ENTRY_STR GetInvEntry = {
	N_NullStr,					// Title
	EnterApp,				// Prompt
	EnterApp,					// Default value
};

static const ENTRY_STR GetDateEntry = {
	YYYY_MM_DD,					// Title
	TarihGir,                                       // Prompt
	N_NullStr,					// Default value
};
        if (0x00 == TERM.TERMInit)
              return;
        
        if (!(TCONF.TCOPT1 & TC1_REFPERMIT)) 
        {
            ShowInfoMsg(TerminalNotAllowed,N_Error);
            SDK_Wait(ONESECOND * 3);
            return;
        }

        //if ( ForceSettlement () ) 
        //    return;

	// Test for journal full condition  
	if ( JournalFullTest(  ) )
              return;
        
         if ( !CheckCurrency() ) 
            return;

	TRINP.TRKEY = INSTAL_REFUND;
	CurrentEvent.NEXTEVENT = 0;

	// Put the Transaction Name into the Custom Message buffer 
	ShowIssuer_Tran(  );

	// Use common routine to get account data from card or kbd. 
	if ( !ReadData(  ) )
		return;

	// validate account number - status stored in global chRetval 
	if ( !AcctProc () )
		return;

        // Moved password prompt inside the NEXTEVENT CHECK.  
	if ( !( TCONF.TCLOCOP & TCL_NO_RPWD ) )
	{
          if ( False == InputPswd( N_NullStr ) )
              return;
	}

	if ( (TRINP.TRISPTR->ISOPT2) & IS2_NOREF )
	{
		ShowErrMsg( RefundNotAllowed );
		return;
	}

	ShowIssuer_Tran(  );

	// See if card expired  
	if ( !CheckCardExp(  ) )
		return;

	/* Get card Issue Number  */
	if(!CheckIssueNum())       /* CANCEL_KY key pressed? */
		return;    
                   
        // herzaman online çýkar .. 
        ForcedOnline = True;
        // Perform a Pre-Dial if allowed 
	PreDial ();

	// Accept amount entry - status stored in global chRetval 
	if ( !AmtProc () )
		return;
       
        retval = AlphaNumEntry(1, S_TRAUTH, 6, &GetInvEntry);
        if (0 == retval) 
           return;
        MoveTillZero( ( char * ) TRINP.TRAUTH, Dspbuf, S_TRAUTH );
        
        retval = NumEntry(1, 8, 8, &GetDateEntry);
        if (0 == retval) 
            return;
        memcpy(RefundDate, Dspbuf, 8);

        ShowIssuer_Tran(  );
        if ( InInvoiceNum(  ) )
		return;
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

		// Set flags to print the receipt header and not allow user cancel. 
		FlagHostCntl = FLAG_PREPRINT;

		// Notify host packet is ready and wait for response.   
		FlagHost(  );

		// Save host response text in terminal flag table.  
		SaveResponseText(  );

		// Display Response 
		RespDisplay ();

	}
	while ( ( TRINP.TRRSPOPT & R_BADPIN ) && ( TRINP.TRPINRETRY++ < 2 ) );

//***************************************************************************/
//      PRINT RECEIPT                                                       */
//***************************************************************************/

	Receipt ();
}


extern void CommonTran( void )  
{
    ENTRY_STR AuthCodeEntry = {
		N_NullStr,				// Prompt
		N_NullStr,				// Default Value
		N_NullStr,				// Not used
    };
    UBYTE retval;
    UBYTE RtcBuf[RTCLEN];
        
         //if ( !CheckCurrency() ) 
         //   return;

        if ( !ReadData(  ) )
            return;

	// validate account number - status stored in global chRetval 
	if ( !AcctProc () )
            return;
        
        switch( TRINP.TRKEY ) 
        {
          case PREAUTH :
                if ( (TRINP.TRISPTR->ISOPT9) & IS9_PREBLOK )
                {
                    ShowErrMsg( Msg_PreAuthNotAllowed );
                    return;
                }
                break;

          case CLOSED_PREAUTH:
                if ( (TRINP.TRISPTR->ISOPT9) & IS9_CLPREBLOK )
                {
                    ShowErrMsg( Msg_ClosedPreAuthNotAllowed );
                    return;
                }
                break;        
        }
        

        if ( (INJECTKEY) && !((TRINP.TRISPTR->ISOPT1) & IS1_MANPAN)  )
        {
          ShowErrMsg( Msg_NoManEntry );
          CurrentEvent.NEXTEVENT = 0;	// Back to idle 
          return;	
        }

      	ShowIssuer_Tran(  );
               
        // Date - Time 
        SDK_RtcRead( RtcBuf );	
        AscHex( TRINP.TRTIME, ( char * ) &RtcBuf[6], S_TRTIME );
        AscHex( TRINP.TRDATE, ( char * ) &RtcBuf[2], S_TRDATE );
        AscHex( TRINP.TRYEAR, ( char * ) &RtcBuf[0], S_TRYEAR );

	// See if card expired  
	if ( !CheckCardExp(  ) )
		return;

	/* Get card Issue Number  */
	if(!CheckIssueNum())       /* CANCEL_KY key pressed? */
		return;

	// Accept amount entry - status stored in global chRetval 
	if ( !AmtProc () )
		return;
        
          // herzaman online çýkar .. 
        ForcedOnline = True;
        // Perform a Pre-Dial if allowed 
	PreDial ();

	if ( InInvoiceNum(  ) )
		return;
  
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
                
                if ( TRINP.TRKEY == CLOSED_PREAUTH ) 
                {
                   AuthCodeEntry.TitleMid = N_EnterAuth;
                   retval = AlphaNumEntry( 1, S_TRAUTH, 2, &AuthCodeEntry );
	           if ( retval )
                   {
	              retval = MoveTillZero( ( char * ) TRINP.TRAUTH, Dspbuf, S_TRAUTH );
                   }    
                   else 
                       return;
                }

		// Set flags to print the receipt header and not allow user cancel. 
		FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;

		// Notify host packet is ready and wait for response.   
		FlagHost(  );

		// Save host response text in terminal flag table.  
		SaveResponseText(  );

		// Display Response 
		RespDisplay ();

	}
	while ( ( TRINP.TRRSPOPT & R_BADPIN ) && ( TRINP.TRPINRETRY++ < 2 ) );

//***************************************************************************/
//      PRINT RECEIPT                                                       */
//***************************************************************************/

	Receipt ();
}



