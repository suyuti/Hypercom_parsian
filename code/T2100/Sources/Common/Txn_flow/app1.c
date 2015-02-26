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
#include "collect.h"
#include "prtdrv.h"

#ifdef MAKE_SNMP
#include "basnmp.h"
#endif // MAKE_SNMP
#include "comm_c.h"

#include "emv_l2.h"
#include "emvdata.h"


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
static Bool AcctCheck( void );
static enum transactions CvtEMVTranKey( enum transactions TranKey );


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
//  APP1SALE        The function that really handles the SALE transaction.
//                  There are checks to determine if this should be DEBIT
//                  , which if True will exit and begin processing
//                  in the respective routines.
//
//  Parameters:     None
//
//  Global Inputs:  Dspbuf -  Where the Amount string is stored
//
//  Returns:        None
//  Notes:
//
//-----------------------------------------------------------------------------

extern void APP1SALE( void )
{
        //char WK [] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
       //                0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 };
	static UBYTE retval;

	// If the terminal requires initialization, do not allow sale transactions 
	if ( 0x00 == TERM.TERMInit )
            return;
        
        //COM_ResetReq();

        TRINP.TRKEY = SALE;

        //memcpy ( TRINP.TRAQPTR->AQWORKKEY , WK , 16 ); 

        CurrentEvent.NEXTEVENT = 0; 

        if ( ForceSettlement () ) 
            return;
               
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
           ShowErrMsg( Msg_NoManEntry );
           CurrentEvent.NEXTEVENT = 0;	// Back to idle 
           return;	         
	}
        
#ifdef MAKE_EMV
        if ( ( ChipCardInserted ) ||
		 ( CheckServiceCode(  ) && ( TRINP.TRORIGKEY  != EMV_SALE ) ) )
	{
		CurrentEvent.NEXTEVENT = EMV_SALE;
		return;
	}
#endif	// MAKE_EMV
        
        if ( !CheckCurrency() ) 
            return;

	if ( !AcctCheck(  ) )
            return;            

	// Test for journal full condition  
	if ( JournalFullTest(  ) )
		return;

	ShowIssuer_Tran(  );

	// See if card expired   @y
	if ( !CheckCardExp(  ) )
		return;

	// Get card Issue Number @y
	if ( !CheckIssueNum() )       // CANCEL_KY key pressed? 
		return;
        
        // service kod konrolu eklenecekse buraya eklenmeli 
        ForcedOnline = True;

	// If possible, Prompt to get the CVV2 input data.  
	if ( !CVV2Proc () )  // @y
		return;

	// Perform a Pre-Dial if not manually entered.
	if ( !INJECTKEY )
		PreDial ();

        if ( !AmtProc () )
             return;

        if ( !Discount() )
          return;

	if ( InInvoiceNum(  ) )
		return;
	
        // Settlement için gerekli @Aydeniz -- BC -- 
        // if ( ((TRINP.TRISPTR->ISOPT1) & IS1_PIN) && !((TRINP.TRISPTR->ISPINTRAN) & ISPIN_SALE) )
            // TRINP.TRKEY = DEBIT;

	// Try up to 3 times to enter the proper PIN 
	do
	{                
		// Get PIN from PIN PAD, if required 
                
		if ( !GetPIN () )
		{
	 		// Finish receipt printing, if preprint is enabled 
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
//  Dodesc          Accept descriptor selection
//
//  Parameters:     None
//
//  Global Inputs:  chRetval    0 - User cancelled the entry.
//                              1 - Descriptor(s) selected successfully 
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern UBYTE DoDesc( void )
{
	int i, cnt;
	UBYTE chRetval;

	// Enable
	if ( !((TRINP.TRISPTR->ISOPT1) & IS1_DESC) )
	{		
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
//  Refund          Handles the REFUND transaction
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

extern void Refund( void )
{        
/*
static const ENTRY_STR GetInvEntry = {
	N_NullStr,					// Title
	EnterApp,				// Prompt
	EnterApp,					// Default value
};

static const ENTRY_STR GetDateEntry = {
	YYYY_MM_DD,					// Title
	TarihGir,                                       // Prompt
	N_NullStr,					// Default value
};*/
        if (0x00 == TERM.TERMInit)
              return;
        
        if (!(TCONF.TCOPT1 & TC1_REFPERMIT)) 
        {
            ShowErrMsg( TerminalNotAllowed );         
            return;
        }

        //if ( ForceSettlement () ) 
        //    return;

	// Test for journal full condition  
	if ( JournalFullTest(  ) )
              return;	
       
         if ( !CheckCurrency() ) 
            return;

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
	        
	if ( INJECTKEY && !((TRINP.TRISPTR->ISOPT1) & IS1_MANPAN) )
	{
		ShowErrMsg( Msg_NoManEntry );
		CurrentEvent.NEXTEVENT = 0;	// Back to idle 
		return;	
	}

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

        ForcedOnline = True;
        // Perform a Pre-Dial if allowed 
	PreDial ();

	// Accept amount entry - status stored in global chRetval 
	if ( !AmtProc () )
		return;

        // Banka akýþýna göre belirlenecektir. @Aydeniz -- BC -- 
        /*
        retval = AlphaNumEntry(1, S_TRAUTH, 6, &GetInvEntry);
        if (0 == retval) 
           return;

        MoveIt( CSTMSG7, Dspbuf, S_TRAUTH);
        retval = AlphaNumEntry(1, S_TRAUTH, 6, &GetDateEntry);
        if (0 == retval) 
            return;

        MoveIt(CSTMSG8, Dspbuf, S_TRAUTH);
        */ 
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
//  Initialize      Initialize the Terminal
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//  Notes:
//
//-----------------------------------------------------------------------------

extern void Initialize( void )
{
	Bool bPinCardStatus;
        UBYTE RtcBuffer[RTCLEN];

	// Confirm the Initialization process 
	YNEntry.TitleMid = N_NullStr;

	YNEntry.Line1Mid = N_NullStr;
	YNEntry.Line2Mid = CorrectYesNo;
        
      // Batchte kayýt varsa , parametre yüklemek için önce günsonu yapýlmalýdýr mesajý verilir . 
      if ( TERM.TERMInit != 0x00 )
      {
        if ( HasBatchRecord() ) 
        {
            ShowInfoMsg(SettlementRequired, BlankMsg);
            SDK_Wait(ONESECOND * 4);     
            return;
        } 
      }

	// Don't ask for password if Auto Init
	//if ( ( AutoInit ) || ( 0xFE == TERM.TERMInit ) || EnterMPswd( ) )
	{
		// Okay; Initialiaze requested by scheduler 
		// or performing automatic initialize from host indicator 
		// or have user confirm 
		if ( ( AutoInit ) ||
			 ( 0x0FE == TERM.TERMInit ) || ( flagTag42 & FLG_INIT) || 
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
			
#ifdef MAKE_SNMP
			if ( (TCONF.TCSNMPOPT & TCSNMP_CLEAR_COUNTERS) && ( 0x0FF == TERM.TERMInit ) )
			{
				InitTable(&MIBTRANSFMT, (UBYTE *) &MIBTRANSTAB, sizeof (MIBTRANSTAB), &MIBTRANSLRC);

				// Clear counters clear flag
				TCONF.TCSNMPOPT &= ~TCSNMP_CLEAR_COUNTERS;
				// Set the Table's LRC
				SetTCTabLRC(  );
			} // @AAMELIN 

			if ( (TCONF.TCSNMPOPT & TCSNMP_CLEAR_WRITEBLE) && ( 0x0FF == TERM.TERMInit ) )
			{
				InitTable(&MIBWRITEOBJFMT, (UBYTE *) &MIBWRITEOBJ, sizeof (MIBWRITEOBJ), &MIBWRITEOBJLRC);

				// Clear writeble clear flag
				TCONF.TCSNMPOPT &= ~TCSNMP_CLEAR_WRITEBLE;
				// Set the Table's LRC
				SetTCTabLRC(  );
			} // @AAMELIN 
			if ( 0x0FF == TERM.TERMInit )
				config_community();

#endif // MAKE_SNMP

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
                        {
                          // If Batch number was changed in Initialization (Aq tab) change it in Batch tab
                          SetBatchNum();

                          // yükleme baþarýlý ise slip bas, tarihi sakla 
                          if ( TERM.TERMInit != 0x00 )
                          {
                              PrintInit(  );
                              PrintDriver( RP_INIT, &TRINP );	
                              SDK_RtcRead(RtcBuffer); 
                              AscHex(LastInitDate, (char *)&RtcBuffer[0], 5);
                               
                              // baþarý ile parametre yüklendi init flagi resetlenir 
                              if ( flagTag42 & FLG_INIT )
                                  flagTag42 &= ~FLG_INIT;

                              // TODO yorumu kaldýr
                              // Deneme amaçlý kaldýrýldý
                              //CurrentEvent.NEXTEVENT = LOGON;                               
                          }
                        }
			
		}
	}
}



//-----------------------------------------------------------------------------
//  APP1CARDVER     The function handles the Card Verification transaction
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

extern void APP1CARDVER( void )
{
	
}



//-----------------------------------------------------------------------------
//  SalOffl         The function handles the Offline transaction
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

extern void SalOffl( void )
{
}



//-----------------------------------------------------------------------------
//  FuncNotSupported        This function is used to indicate an unsupported
//                          function or transaction.
//
//  Parameters:             None
//
//  Global Inputs:
//
//  Returns:                None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void FuncNotSupported( void )
{
	ShowErrMsg( N_FuncNotSupported );
}



//-----------------------------------------------------------------------------
//  TstTran         The function handles the test transaction
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
//  Auth            The function handles the Auth transaction
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

extern void Auth( void )
{       
        
}



//-----------------------------------------------------------------------------
//  SaveResponseText        Save last response text from the host
//
//  Parameters:             None
//
//  Global Inputs:
//
//  Returns:                None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void SaveResponseText( void )
{
	// Move the host response to the terminal flag table if response exists. 
	if ( TRINP.TRHOSTTEXT[0] > 12) // Not readable character
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
//  CheckCardExp        Check for expired card
//
//  Parameters:         None
//
//  Global Inputs:      chRetval Is set to zero if CANCEL_KY or timed out
//
//  Returns:            None
//
//  Notes:
//
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
                
                // expired card
		if ( !chRetval )
		{
			ShowErrMsg( ExpiredCard );
			if ( INJECTKEY )
                            ShowIssuer_Tran(  );
			else
                            return chRetval;
		}				
	}
	while ( !chRetval );

	return chRetval;
}

//-----------------------------------------------------------------------------
//  MailOrder       Mail/Phone Order Transaction (Visanet Only)
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

extern void MailOrder( void )
{       
        if ( 0x00 == TERM.TERMInit )
            return;
        
         // mail order permitted
	if (!(TCONF.TCOPT4 & TC4_MOPERMIT)) 
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

	TRINP.TRKEY = MAIL_ORDER;
	CurrentEvent.NEXTEVENT = 0;
        

	// Put the Transaction Name into the Custom Message buffer 
	ShowIssuer_Tran(  );

	// Use common routine to get account data from card or kbd. 
	if ( !ReadData(  ) ) return;

	// validate account number - status stored in global chRetval 
	if ( !AcctProc () )
		return;

	ShowIssuer_Tran(  );
        
         if (TRINP.TRISPTR->ISOPT3 & IS3_NOMAILORDER) 
         {
            ShowErrMsg( Msg_MailOrderNotAllowed );
            return;
         }

	// See if card expired  
	if ( !CheckCardExp(  ) )
		return;
		
	// If possible, Prompt to get the CVV2 input data.  
	if ( !CVV2Proc () )
		return;

	// Accept amount entry - status stored in global chRetval 
	if ( !AmtProc () ) return;
        
        TRINP.TRPOCC = 0x08; 

        // herzaman online çýkar .. 
        ForcedOnline = True;

	// Perform a Pre-Dial if allowed 
	PreDial ();

        if ( InInvoiceNum(  ) ) return;

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





//-----------------------------------------------------------------------------
//  Installment       Installment Transaction 
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

extern void Installment( void )
{      
  // If the terminal requires initialization, do not allow sale transactions 
  if ( 0x00 == TERM.TERMInit )
      return;
  
  if (!(TCONF.TCOPT1 & TC1_INSPERMIT)) 
  {
    ShowInfoMsg(TerminalNotAllowed,N_Error);
    SDK_Wait(ONESECOND * 3);
    return;
  }

  //if ( ForceSettlement () ) 
  //          return;

  // Test for journal full condition  
  if ( JournalFullTest(  ) )
      return;    
   
    if ( !CheckCurrency() ) 
            return;

   TRINP.TRKEY = INSTALLMENT;
   CurrentEvent.NEXTEVENT = 0;

// Put the Transaction Name into the Custom Message buffer 
  ShowIssuer_Tran(  );
  
   // fallback deðilse ve defult olarak taksitliye yönlendirilmemiþse taksitli deðilse 
  if (  !(TRINP.TRSTATUS[1] & TS2_READIDLE) 
#ifdef MAKE_KIB
      && !(TRINP.TRLOYALTYSTATUS & TS_POINT) 
#endif
      )
    if ( !ReadData(  ) ) 
      return;
  
  #ifdef MAKE_EMV
    if ( ( ChipCardInserted ) ||
        ( CheckServiceCode(  ) && ( TRINP.TRORIGKEY  != EMV_INSTALLMENT ) ) )
    {
        CurrentEvent.NEXTEVENT = EMV_INSTALLMENT;
        return;
    }
  #endif	
   

  // validate account number - status stored in global chRetval 
   if ( !AcctProc () )
    return;
  
  if ( (INJECTKEY) && !((TRINP.TRISPTR->ISOPT1) & IS1_MANPAN)  )
  {
    ShowErrMsg( Msg_NoManEntry );
    CurrentEvent.NEXTEVENT = 0;	// Back to idle 
    return;	
  }
  
  if (TRINP.TRISPTR->ISOPT4 & IS4_INSBLOK) 
  {
    ShowErrMsg( Msg_InstallmentNotAllowed );
    return;
  }

  // See if card expired  
  if ( !CheckCardExp(  ) )
    return;

  // If possible, Prompt to get the CVV2 input data.  
  if ( !CVV2Proc () )
    return;    
  // herzaman online çýkar .. 
  ForcedOnline = True;

  // Perform a Pre-Dial if allowed 
  PreDial ();  

  // Accept amount entry - status stored in global chRetval 
  if ( !AmtProc () ) return;
  
  if( !Discount() ) return;

#ifdef MAKE_KIB
  if( !InstallmentProcessing() )
     return;
#else
  do {
    memset( tempBuf, 0x20, 30);
    memcpy( tempBuf, "02-", 3 );
    BfAscii( &tempBuf[3], &TCONF.TCINSNUMBER, 1);         
    memcpy(CSTMSG, tempBuf, 30);                  
    if (NumEntry(1, 2, 1, &InstNumEntry)) 
    {
      TRINP.TRINSTNUM = Dspbuf[0] - '0';
      if (Dspbuf[1] != 0)						
      {
         TRINP.TRINSTNUM <<= 4;
         TRINP.TRINSTNUM |= (Dspbuf[1] - '0');
      }

      if ( TRINP.TRINSTNUM > TCONF.TCINSNUMBER )
        ShowErrMsg( Msg_OverInsNum );
      else if ( TRINP.TRINSTNUM == 0x00 || TRINP.TRINSTNUM == 0x01 ) 
        ShowErrMsg( Msg_LowerInsNUm );
      else 
        break;                
    }
    else 
        return;
  } while ( 1 ); 

#endif  
    
  if ( InInvoiceNum(  ) ) return;

  ShowIssuer_Tran(  );  

  FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;

  FlagHost(  );
	
  SaveResponseText(  );

  RespDisplay ();

  Receipt (); 
}

//-----------------------------------------------------------------------------
//  AcctCheck       Check for MOD-10, manual entry and batch pending/settled
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          Delayed tests when default SALE from idle until the 
//                  corrrect issuer was determined, debit, etc.
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


/*-----------------------------------------------------------------------
 * Name:           CheckIssueNum
 * Description:    Check for Manual Issue Number entry
 * Parameters:     None.
 * Return Value:   None.
 * Notes:          chRetval is set to zero if CANCEL_KY or timed out.
 *-----------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------
 * Name:           CheckServiceCode
 * Description:    Check Service Code
 * Parameters:     None.
 * Return Value:   None.
 * Notes:          chRetval is set to zero if CANCEL_KY or timed out or error.
 *-----------------------------------------------------------------------*/
extern UBYTE CheckServiceCode( void )
{
    UBYTE *pSvcCode; // pointer for locating svc code    
    UBYTE emptyBuf[S_TRTRK2] = {0x00};
    // Set pointer to start of Track 2
    pSvcCode = TRINP.TRTRK2;
    
    // TRTRK2 boþ ise kontrol yapmaya gerek yok 
    if ( memcmp(TRINP.TRTRK2, emptyBuf, S_TRTRK2) == 0 )
        return False;

    // Find the track2 separator
    while( *pSvcCode++ != TRK2SEP );

    // jump over expiry date to first digit of svc code 
    pSvcCode += S_TREXPD*2;
  
    if ( *pSvcCode == '2' || *pSvcCode == '6' )  
      return True;

    return False;
}


#ifdef	MAKE_EMV
/*-----------------------------------------------------------------------
 * Name:            CvtEMVTranKey
 * Description:     This function will return the equivalent EMV
 *                  transaction key corresponding to the given 
 *                  transaction key.
 * Parameters:      None.
 * Return Value:    EMV transaction key.
 * Notes:           None.
 *-----------------------------------------------------------------------*/
static enum transactions CvtEMVTranKey( enum transactions TranKey )
{
	enum transactions EMVTranKey;

	/* Change transaction key to EMV transaction key */
	switch ( TranKey )
	{
		case SALE :
			EMVTranKey = EMV_SALE;
			break;

 		case INSTALLMENT :
                        EMVTranKey = EMV_INSTALLMENT;
			break;

		default : 
 			EMVTranKey = EMV_SALE;
			break;
	}

	CurrentEvent.NEXTEVENT = EMVTranKey;

	return ( EMVTranKey );
}
#endif	// MAKE_EMV


/*------------------------------------------------
 *  Name:           TerminalQuickReset
 *  Description:    Restart the terminal via booter
 *  Include files:  
 *  Parameters:     None.
 *  Return Value:   Nine
 *  Notes:          None.
 *-----------------------------------------------*/
extern void TerminalQuickReset( void )
{
    // Notify Application Manager to perform Fast Start
    // This setting must be cleared by Application Manager
    NMSIDLL[0] = 'F';
    NMSIDLL[1] = 'S';

	// restart comms to make sure we have a clean comm process
	COM_ResetReq();
	while( !COM_ResetCompleted() )
	// Relinquish the CPU while we reset
		SDK_RelinqCPU(  );
    // Do reset terminal
    SDK_TerminalReset ( );
}


//-----------------------------------------------------------------------------
//  Instal_Refund          Handles the Instal_Refund transaction
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



//-----------------------------------------------------------------------------
//  PreAuth          Handles the REFUND transaction
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

