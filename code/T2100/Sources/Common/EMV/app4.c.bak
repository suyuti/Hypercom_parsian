
#ifdef MAKE_EMV
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     app2.4
//  EMV application processing
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "osclib.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "sdl.h"
#include "fixdata.h"
#include "utllib.h"
#include "emv2_pk.h"

#include "defines.h"
#include "util.h"
#include "util2.h"
#include "emv_l2.h"
#include "scard.h"
#include "emvtabs2.h"
#include "emvaptag.h"
#include "emvutils.h"
#include "emvdata.h"
#include "tables.h"
#include "findtran.h"
#include "receipt.h"
#include "amtproc.h"
#include "respdsp.h"
#include "acctproc.h"
#include "hstmsg.h"
#include "util.h"
#include "settime.h"
#include "sdktime.h"
#include "app4.h"
#include "pinpad.h"
#include "crdrange.h"
#include "checkCurrency.h"


extern Bool IsTrack2EquivDataConsistent( void );
extern Bool MerchantForceAccept( UBYTE );
void PerformFallback( void );
Bool IsInitCard( void );
extern UBYTE HandleCertOfflineReferral( struct iccrec_rec * pIccEntry, UBYTE *TACDef );
extern Bool ForceSettlement( void ); 

//=============================================================================
// Private function declarations
//=============================================================================
static Bool MerchantForcedOnline( void );
static UBYTE TrainingModeProcessing( struct iccrec_rec *pIccEntry );
static Bool FindCardInHotlist( UBYTE * Pan, UBYTE Length, UBYTE Seq );
static Bool IsCardHot( UBYTE * Pan, UBYTE Length, UBYTE Seq );
extern void AddZeroAdvToJournal( void );

static ENTRY_STR YNEntry;		/* structure for YesNoEntry() */
/* --- Local Variables/Consts ---- */
static const ENTRY_STR ApprovalCodeEntry = {
	N_NullStr,					// Default Value 
	EnterApp,					// Prompt 
	N_NullStr,					// Not used 
};
static struct taidrec_rec TAIDEntry;

const UBYTE BadPAN[] = {0x47, 0x61, 0x73, 0x90, 0x01, 0x01, 0x00, 0x10};
const UBYTE BadPANKasys[] = {0x47, 0x61, 0x73, 0x00, 0x00, 0x00, 0x00, 0x29};

const UBYTE BadSEQ = 0x01;
const UBYTE BadSEQKasys = 0x00;
/*------------------------------------------------------------------
 *  Name:           EMVPROC
 *  Description:    cross callable EMV transaction handler
 *  Parameters:     none
 *  Return Value:   
 *  Notes:          None.
 *-----------------------------------------------------------------*/

 // This function uses the EMV Level 2 library
 // When using this library, if status EMVL2_FAIL is returned this indicates
 // that a fatal error has happened and that the transaction must terminate 
 // without performing any further smart card comms.
 
void EMVPROC( void )
{
	// app selection vars
	UBYTE index = 0;
	CandTextType *pCurrentCand = 0;
    UBYTE TargetPerCent;
    UBYTE MaxPerCent;
    UBYTE Threshold[8];
    UBYTE PrevAmt[6] = {0,0,0,0,0,0};
    UBYTE AppVersions[8];        
    UBYTE status;
	Bool  HotCard;
	Bool  MerchOnline = False;
	UBYTE AppAction;
	UBYTE TACs[15];
	struct iccrec_rec *iccptr;
	struct iccrec_rec IccEntry;
#ifdef MAKE_ICCDEBUG
        static UBYTE FloorAmt1[6];
#endif  
        unsigned char tempBuf[30];
        UBYTE value;
static const ENTRY_STR InstNumEntry = {
	InstNum,					// Default Title
	InstNum,					// Blank Line (line 1) 
	CustomMsg,					// Card Present? (line 2)
};

	ForcedAccept = False;
	
	// Test critical conditions
	if ( ( FALSE == TERM.TERMInit ) || ( JournalFullTest( ) ) )
	{
		return;
	}
        
        TRINP.TRKEY = CurrentEvent.TRNKEY;
         // return to idle when finished
	CurrentEvent.NEXTEVENT = 0;

#ifdef MAKE_KIB 
            
        if ( IsSaleAnInstalment() )
              return;

#endif

        if ( ForceSettlement () ) 
            return;
        
        if ( !CheckCurrency() ) 
            return;

	// If ICC not inserted, get it now or get out
	if (( DEV_SCR != GetLastInputType( )) && ( DEV_PINCARD != GetLastInputType( )) && !( TRINP.TRLOYALTYSTATUS & TS_POINT ) )
	{
        if ( !ForceICCInsertion( ) ) return;
	}

	ShowInfoMsg( ProcCard, SmartCard );

	// Setup the transaction key
	
	
       

	TRINP.TRPOSE[0] = 0x00;		// pos entry ICC
	TRINP.TRPOSE[1] = 0x51;

	// set transaction time up
	SetTranTime( TRINP.TRYEAR );

    if ( isCardInserted( ) )
	{
        // Initialise user card, including re-entry if card read fail
		if ( InitUserCard( ) )
		{
			if ( EMVL2_Initialise( EMVVersion, CryptSlot ) )	// 2nd param is Atmel slot (0 = not present)
			{				              			
                            status = PerformAppSelection( &pCurrentCand, &index );
                      if (status & 0x80)
		{
                        GetMsg( APP_BLOCKED, CSTMSG );
                        GetMsg( LIST_EMPTY, CSTMSG2 );
                        ShowInfoMsg( CustomMsg2, CustomMsg );
                        SDK_Wait(ONESECOND * 2);
		}
		else
				// switch on possible PerformAppSelection outcomes          
				switch ( status )
				{
					case EMVL2_SELECTED:	// Smart card processing can continue
					{
							// Retrieve selected TAID entry for further use
							TAIDEntry = TAIDTAB[index];
                                                        TAIDTABIndex = index;

							// Update EMV fields in library and message structure
							if (( UpdFieldsPostAppSel( ) != EMVL2_OK ) || ( WriteAIDData( index ) != EMVL2_OK ) )
							{
								// pack up and get out
								// display a modal error message?
								ShowErrMsg( AppSelection );
								break;
							}
                                                                                 
							/* validate account number */
							if ( !AcctProc () )
							{
								/* Error checking the Card Range, Issuer/Acquirer */
								break;
							}

							/* This is the point when we know the Acquirer/Issuer */
							if ( !(AQ2_ICCACQ & TRINP.TRAQPTR->AQOPT2) )
							{
								/* Non-ICC Acquirer, exit */
								ShowErrMsg(ChipNotSupported);
								break;
							}

							// Check if problem with ICCTAB entry
							if ( (NULL == ( iccptr = FindIcc( TRINP.TRISID ) ) ))
							{
								ShowErrMsg( UnsuppCard );
								break;
							}
                                                        
                                                        if(EMV_INSTALLMENT == TRINP.TRKEY) 
                                                        {
                                                           if (TRINP.TRISPTR->ISOPT4 & IS4_INSBLOK) 
                                                           {
                                                                ShowErrMsg( Msg_InstallmentNotAllowed );
                                                                return;
                                                            }
                                                         }

	                        // Retrieve last transaction for this PAN from journal if present
	                        if (TCONF.TCAPPCAP[1] & APP_CAP2_TXN_LOG)
		                    {
                                GetPreviousAmount(PrevAmt); 
                            }
                            
							

                            // write ACQ TERM ID and Merch Id to tagstore
                            WriteSchemeData( );
                            
							// Copy the selected entry to upper memory
							IccEntry = *iccptr;
	                        
	                        
							// Copy Appl. Text into TRINP (for printing)
							// if the issuer is configured for this:
							if ( IS4_APPTEXT & (TRINP.TRISPTR->ISOPT4) )
							{
								memcpy( TRINP.TRAPPTEXT, pCurrentCand->text, S_TRAPPTEXT );
							}

							/* Increment Invoice Number */
		                    InInvoiceNum( );

							// Redisplay Issuer and Transaction Name 
							ShowIssuer_Tran( );
                            strcpy( CSTMSG, CurrentEvent.TRNTEXT);
				            GetMsg( CustomMsg, DefaultTitle );
                                                
                                                // puanlý iþlem ise 
                                                if ( TRINP.TRLOYALTYSTATUS & TS_POINT )
                                                {
                                                    if( !LoyaltyProcessing() )
                                                        break;
                                                }
                                                else
                                                {
						    if ( !AmtProc () )                                                    								
							 break;  
                                                    if( !Discount() ) return;
                                                }
  
                                // Send amount(s) to the tag store
							WriteAmountData(  );
							WriteFloorInvData( );
                           	ShowInfoMsg( ProcCard, SmartCard );

							




							// Data Authentication Process
							if ( EMVL2_FAIL == EMVL2_AuthenticateData( ) )
							{
								// catastrophic failure - we're out of here
								ShowErrMsg( CardFail );
								break;
							}

#ifdef MAKE_KIB
        if( !InstallmentProcessing() )
              return;
#endif


#ifndef MAKE_KIB
              if (EMV_INSTALLMENT == TRINP.TRKEY) 
              {
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
            }
#endif


                                memset( Threshold, 0, sizeof( Threshold ) );
				memcpy( &Threshold[3], &IccEntry.ICCTHRESH, 2 );
	                        Bcd2Bin( &TargetPerCent, &IccEntry.ICCTARGPCT,1 );
	                        Bcd2Bin( &MaxPerCent, &IccEntry.ICCMAXTPCT, 1);
								
							BuildVerBuff( AppVersions, TAIDEntry );

							// check the exception file
							HotCard = IsCardHot((UBYTE*) &TRINP.TRPAN, 8, TRINP.TRPANSEQ);
							MerchOnline =  MerchantForcedOnline();

							status = PerformCVMProcessing( );
							// Check for status on exit from CVM
							// currently only 2 handled (FAIL/DONE)
							// check for signature or online PIN requirement
							// Check also for card still inserted if no error
							switch ( status )
							{
								case EMVL2_CVM_SIG:	// still need signature
									// flag for receipt is already set
									SuppressSigLine = False;
                                     TRINP.TROPTEMV &= ~TROPTEMV_NOSIG;
									// lack of break intentional;

								case EMVL2_CVM_DONE:	// nothing more to do
									  status = EMVL2_OK; // reset status for subsequent GenAC
								      break;			
                            }
                            
                             if ( status != EMVL2_FAIL )	
							 {
							 	//unless merch thinks otherwise, we plan to approve offline 
								AppAction = MerchOnline ? EMVL2_ONLINE : EMVL2_OK;
	                    
                                // Show "Processing Card" 
							    ShowInfoMsg( ProcCard, SmartCard );

								// Initialise flags for advices before 1st GenAC
								FirstGenACAdviceRequired = False;
								SecondGenACAdviceRequired = False;

								// Read TACs from TermMaster
								memcpy(TACs, &IccEntry.ICCTACDEN, 5);
	                            memcpy(TACs + 5, &IccEntry.ICCTACONL, 5);
	                            memcpy(TACs + 10, &IccEntry.ICCTACDEF, 5);
#ifdef MAKE_ICCDEBUG
MoveIt ( TACLAR, TACs, 15);
#endif
							 	
								          status = EMVL2_ValidateTransaction( HotCard, MerchOnline,
															        PrevAmt,
														        AppVersions, TargetPerCent,
															        MaxPerCent,
															        ( UBYTE * ) &Threshold );

									 if (status == EMVL2_OK)
									{
									     {

#ifdef MAKE_ICCDEBUG
                                 if (TrackErr == 0x01)
                                 {
                                     AppAction = EMVL2_DECLINE;
                                     TrackErr = 0x00;
                                  }
#endif

			 							// status is result of GenAC passed upwards
									    // first param is application decision passed into library
									    // NB: Currently called with EMVL2_OK hardcoded. It is also possible 
									    // to pass in EMVL2_DECLINE or EMVL2_ONLINE as a parameter which modifies
									    // the minimum security level of cryptogram. EMVL2_DECLINE will force the
									    // library to ask for an AAC, while _ONLINE will seek an ARQC.
									    // These two options are not EMV compliant
                                       status = EMVL2_ActionAnalysis( AppAction, TACs, TACs + 5, TACs + 10);
	                                  // Informational advice messages are an optional EMV feature and the EMVL2
	                                  // library will indicate when the creation of such a message has bee requested
	                                  // Add code here to handle the creation of such informational advice messages.
	                                  if (status & BITMAP_RETVAL) // indicates response contains card decision bitmap
                                      {

	            						   if (status & ADVICE_REQUESTED)
										   {
											   FirstGenACAdviceRequired = True;
										   }

										   // mask off the advice request bits before decoding the rest
	                                       status &= BITMAP_RETVAL | ~(ADVICE_REQUESTED|SCRIPT_PERFORMED);

                                      }

									  // Modify the return value to reflect force transaction types
	                                  if ( MerchOnline )
                                      {
										   status = EMVL2_ONLINE;
                                      }							
									  else
	                                  {
	                                      // offer override if declined 
	                                      if ((EMVL2_DECLINE == status) || (EMVL2_REFERRAL == status))
	                                      {
	                                          // Check to see if we want to force acceptance now
	                                          ForcedAccept = MerchantForceAccept( status );

	                                          if (( ForcedAccept ) && (AppAction == EMVL2_OK))
										      {
										           status = EMVL2_ACCEPT;
										      }
	                                       }
	                                   }
// { visa testleri icin @@A 	
/*
#ifdef MAKE_ICCDEBUG
                  if (EMVL2_DECLINE != status )    
                  {
                       ClrIt( FloorAmt1, S_TRTOTAM );
                        
                       if ( (CmpBuf( TRINP.TRTOTAM, TCONF.TCFLOOR1, S_TRTOTAM ) <= 0) )
                       {
                          status = EMVL2_ACCEPT;
                        }
                  }
#endif
*/
// } visa testleri icin @@A 

// { @@Aydeniz- Mastercard testleri

   memset(AppLabel, 0x00, 34);
   EMVL2_ReadTagData( AppLabel, 34, TAG_50 ); 

// }

                                       switch ( status ) // action analysis result
                                       {
		                                    case EMVL2_ACCEPT:
                                                // Update the fields post GenAC 
	                                            value = UpdFieldsPostGenAC();
                                                    UpdFinalResponseCode( );
                                                    if ( value == EMVL2_OK)
			                                    {
		                                           // Set the time
		                                           SetTranTime( TRINP.TRYEAR );

				                                   // Process offline transaction 
			                                        EMV_Offline(  );
                                                }
                                                else
                                                {
                                                    // catastrophic failure - we're out of here
//@pl 				                                memcpy( CSTMSG, "PostGenAC fail", 13 );
//@pl 				                                ShowErrMsg( CustomMsg );
                                                    ShowErrMsg( POSTGENACFAIL_MSG );
                                                  }
                                                  break;

                                            case EMVL2_ONLINE:
                                                  // Go online 
                                                  status = GoOnline( &IccEntry, TACs + 10 );
                                                  // handling for 2nd GenAC
                                                  switch ( status )
                                                  {
                                                      case EMVL2_ONLINE_CANCELLED:
                                                          // Show decline msg
                                                          ShowErrMsg ( N_UserCancelled );
                                                          break;
                                                      case EMVL2_DECLINE:
                                                          // Show decline msg
//@pl 	                                                memcpy (CSTMSG, "Declined", 9);
//@pl 	                                                memcpy (CSTMSG2, "Result", 7);
//@pl 													ShowInfoMsg (CustomMsg, CustomMsg2 );
                                                          ShowInfoMsg (DECLINED_MSG, RESULT_MSG );
                                                          SDK_Wait(ONESECOND * 2);
                                                          break;    
                                                      case EMVL2_MAG_STRIPE:
                                                          // Fallback to a better place
                                                          PerformFallback();
                                                          return;
                                                          break;

                                                    } // end switch GoOnline result
                                                    break;

                                            case EMVL2_REFERRAL:
                                                  // AAR received
                                                  // Add code here to handle voice referral processing
                                                  // or allow to drop through to become a decline
                                                  // set TRINP.TRRSPC (Tag_8A) for either decline or accept
                                                  HandleCertOfflineReferral(&IccEntry, TACs + 10);
                                                  //@pl SuppressSigLine = True;
                                                  //@pl set 'SIGNATURE IS NOT REQUIRED' bit
                                                  TRINP.TROPTEMV |= TROPTEMV_NOSIG;
                                                    break;

                                            case EMVL2_DECLINE:
                                                  // check if advice requested
                                                  // Update the fields post GenAC 
                                                  UpdFieldsPostGenAC();
                                                  // We should not trust the Response Code here, we will force it to
                                                  // 'Z1' (declined) to show the correct msg on the screen & receipt
                                                  // UpdFinalResponseCode();
                                                  memcpy( TRINP.TRRSPC, "Z1", S_TRRSPC);

                                                  if (FirstGenACAdviceRequired)
                                                  {
                                                      // add zero amount advice if requested and not txn stored
                                                      // no trace num stored yet, so get here
                                                      IncTraceNum( TRINP.TRSTAN );
                                                      AddZeroAdvToJournal();
                                                  }

                                                  // Show decline msg
//@pl                                              memcpy (CSTMSG, "Declined", 9);
//@pl                                              memcpy (CSTMSG2, "Result", 7);
//@pl                                              ShowInfoMsg (CustomMsg, CustomMsg2 );
                                                  ShowInfoMsg (DECLINED_MSG, RESULT_MSG );
                                                  SDK_Wait(ONESECOND * 2);
                                                  break;

                                            case EMVL2_MAG_STRIPE:
                                                  // Fallback to a better place
                                                  PerformFallback();
                                                  return;
                                                  break;

                                            default:
                                                  // Show "Processing Card"
                                                  ShowInfoMsg( ProcCard, SmartCard );
  
                                       }	// end switch ActionAnalysis

                                  }

                                  /* Display final response from TRINP */
                                  RspLookUp( &TRINP );

                                  /* Display Response */
                                  RespDisplay();

                                  Receipt ();

							}
							else	// validate transaction = EMVL2_FAIL
                            {
							    ShowErrMsg( CardFail );
							}

                        } // end of case EMVL2_SELECTED
						break;

						case EMVL2_NO_MORE_APPS:	// Candidate list exhausted (mag stripe not permitted)
						    ShowErrMsg( NoMoreApps );
							break;

						case EMVL2_EASY_ENTRY:	  // Easy Entry card found, continue as per mag stripe
//@pl       	                    memcpy( CSTMSG, "Easy Entry", 11 ); 
//@pl                             ShowErrMsg( CustomMsg );
                            ShowErrMsg( EASYENTRY_MSG );
	                        break;
                                                
                                                  
						case EMVL2_MAG_STRIPE:   // Fallback to be performed if library say it is ok
                                                case EMVL2_NON_EMV:
						    PerformFallback();
					        return; // return early - txn changing horses
	                        break;

	                    case EMVL2_MNDTY_DATA:	  // Mandatory data missing
//@pl                             memcpy( CSTMSG, "Mndty Data Missing", 19 ); 
//@pl                             ShowErrMsg( CustomMsg );
                            ShowErrMsg( MNDTTDATAMISSING_MSG );
	                        break;
							
						case EMVL2_ICC_BLOCKED:   // Card blocked
//@pl 						    memcpy( CSTMSG, "Card Blocked", 13 ); 
//@pl                             ShowErrMsg( CustomMsg );
                            ShowErrMsg( CardBlocked );
	                        break;


						default: // critical failure includes EMVL2_FAIL
						    ShowErrMsg( CardFail );
							break;

					}				// end app selection result switch 
				}

				// print ICC debug information
				if ( PrintEMVDebug )
				{
					PrintICCDebug( EMVVersion, pCurrentCand, PrevAmt );
				}

			}
			else				// EMVL2_Initialise failed
			{
				ShowErrMsg( CardFail );
			}
		}
	
		// End the session with the Smart Card and force removal
        // set up message for remove card
        GetMsg( TransComplete, CSTMSG );		
        PerformCardClose(  );

	}  // smart card insert check failed

        PinMessage( Msg_ClrScrn);
	return;
}

















/*-----------------------------------------------------------------------
 * Name:           FALLBACK
 * Description:    Handles the FALLBACK transaction after chip failure
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          Amount string is stored in global buffer Dspbuf.
 *                 Journal full already tested in EMVSALE
 *-----------------------------------------------------------------------*/
void FALLBACK( void )
{
	// Set flag, it will prevent ReadData() from allowing 'Insert Card'
	TRINP.TRSTATUS[1] |= TS2_FALLBACK;
	TRINP.TRKEY = EMV_FALLBACK;
	TRINP.TRPOSE[0] = 0x08;		// pos entry fallback
	TRINP.TRPOSE[1] = 0x01;

	ShowIssuer_Tran(  );

	CurrentEvent.NEXTEVENT = 0;

	// get card details from user
	if ( !ReadData(  ) )
	{
            return;
	}

	// cue up real transaction type
	/* Switch to corresponding transaction */
	switch ( TRINP.TRORIGKEY )
	{
		
		case EMV_SALE:			/* switch to SALE */
			CurrentEvent.NEXTEVENT = SALE;
			break;
                case EMV_INSTALLMENT:			/* switch to SALE */
			CurrentEvent.NEXTEVENT = INSTALLMENT;
			break;
                case INSTALLMENT:
                        TRINP.TRORIGKEY = EMV_INSTALLMENT;
                        CurrentEvent.NEXTEVENT = INSTALLMENT;
			break;

	}

}


static Bool MerchantForcedOnline( void )
{
	UBYTE keyval;

    // don't allow if capability bit not set, or if EMV_FORCED txn type
    // EMV_FORCED transaction disabled - ASH 30-03-05
	//if ((!(TCONF.TCAPPCAP[1] & APP_CAP2_MERCH_SUSP)) || ( TRINP.TRORIGKEY == EMV_FORCED) )
	if (!(TCONF.TCAPPCAP[1] & APP_CAP2_MERCH_SUSP)) 
		return False;

//@pl 	memcpy (CSTMSG, "FORCE ONLINE", 13);
//@pl 	memcpy (CSTMSG2, "FORCE ONLINE?", 14);
	GetMsg( FORCEONLINE_MSG, CSTMSG );
	GetMsg( ISFORCEONLINE_MSG, CSTMSG2 );
	YNEntry.TitleMid = CustomMsg;
	YNEntry.Line1Mid = CustomMsg2;	

	YNEntry.Line2Mid = N_NullStr;

	// Prompt for YES or NO Keys 
	keyval = YesNoEntry( &YNEntry );

	if ( ENTER_KY == keyval )	// If YES Key
	{
		return True;
	}

	return False;
}

// TrainingModeProcessing - Special processing for training mode
// Returns: False - continue with the transaction, True - Abort.
static UBYTE TrainingModeProcessing( struct iccrec_rec *pIccEntry )
{
	UBYTE retval = True;	// default

	/* Set Terminal Action Codes to zeroes */
	memset( (UBYTE *)pIccEntry->ICCTACDEN, 0x00, S_ICCTACDEN );
	memset( (UBYTE *)pIccEntry->ICCTACONL, 0x00, S_ICCTACONL );
	memset( (UBYTE *)pIccEntry->ICCTACDEF, 0x00, S_ICCTACDEF );

	return( retval );
}

//-----------------------------------------------------------------------------
//  Name:                       IsCardHot        
//
//      Purpose:                Checks against the hot card list of one card for cert purposes only
//
//  Parameters:     None
//
//  Global Inputs:      None
//
//  Returns:        Bool, card is hot or not
//
//  Notes:                      
//
//-----------------------------------------------------------------------------
static Bool IsCardHot( UBYTE * Pan, UBYTE Length, UBYTE Seq )
{
    if (TCONF.TCAPPCAP[1] & APP_CAP2_EXC_FILE_TEST)
	{
		if   (((memcmp(Pan, BadPAN, Length) == 0) && (Seq == BadSEQ))
			 ||
		     ((memcmp(Pan, BadPANKasys, Length) == 0) && (Seq == BadSEQKasys)))
			return True;
	}

	return False;
}

static Bool FindCardInHotlist( UBYTE * Pan, UBYTE Length, UBYTE Seq )
{
// hotlist implementaion pending
    return False;
}



//-----------------------------------------------------------------------------
// Name:           ICCGETPAN
// Description:    Perform application selection to retrieve card details from chip
//
// Parameters:     None
// Return Value:   chRetval != 0	PAN is got OK,
//                           = 0 	PAN is not got. In this case: 
//									if TRINP.TRKEY == EMV_FALLBACK then magnetic stripe could 
//									be used to get the PAN.	Original transaction name is saved on TRINP.TRORIGKEY.
//									
//
// Notes:          TRINP structure will be updated with chip card details for the
//                 selected application.
//-----------------------------------------------------------------------------
UBYTE ICCGETPAN( void )
{
	UBYTE	index = 0;
	CandTextType *pCurrentCand = 0;
	UBYTE	status;
	UBYTE	retval;

	// count PAN is not got as default
	retval = 0;

	ShowInfoMsg( ProcCard, SmartCard );
	
	// kart takýlý mý ? 
	if ( isCardInserted( ) )
	{
		// kartýn çip datasý okunabiliyor mu 
		if ( IsInitCard( ) )
		{
			if ( EMVL2_Initialise( EMVVersion, CryptSlot ) )	// 2nd param is Atmel slot (0 = not present)
			{
				// Perform Application Selection
				status = PerformAppSelection( &pCurrentCand, &index );

				// switch on possible PerformAppSelection outcomes          
				switch ( status )
				{
					case EMVL2_SELECTED:	// Smart card processing can continue
						// Retrieve selected TAID entry for further use
//						TAIDEntry = TAIDTAB[index];
//						TAIDTABIndex = index;

						// Update EMV fields in library and message structure
						UpdFieldsPostAppSel( );

						// count PAN is got OK,
						retval = 1;
						break;

					case EMVL2_NO_MORE_APPS:	// Candidate list exhausted (mag stripe not permitted)
						ShowErrMsg( NoMoreApps );
						break;

					case EMVL2_EASY_ENTRY:		// Easy Entry card found, continue as per mag stripe
                                                ShowErrMsg( EASYENTRY_MSG );
						break;

					case EMVL2_NON_EMV:		// Non EMV card found, perform a fallback mag stripe txn 
					case EMVL2_MAG_STRIPE:		// No supported applications, a mag stripe txn may be performed
						ShowErrMsg( CardFail );
						break;

					case EMVL2_MNDTY_DATA:	  // Mandatory data missing 
						// Mandatory data missing
						ShowErrMsg( AppBlocked );
						break;                                           

					case EMVL2_ICC_BLOCKED:    // Card Blocked
						ShowErrMsg( CardBlocked );
						break;   

					default:
						// critical failure includes EMVL2_FAIL
						ShowErrMsg( CardFail );
						break;
				}	// end app selection result switch 
			}
			else	// EMVL2_Initialise failed
			{
				ShowErrMsg( CardFail );
			}
		}
		else	// init user card failed
		{
			// InitUserCard failed
            // @@AA void icin duzenleme ,  
			// ShowErrMsg( CardFail );
            ;
		}

	}	// smart card insert check failed

	// End the session with the Smart Card and force removal

        CurrentEvent.NEXTEVENT = 0;
        PerformCardClose();

	return retval;	
}

// kartýn çipi okunabiliyor mu ? 
Bool IsInitCard( void )
{
	UBYTE SCBytes[33]; // BD no 33 is enough
	UWORD SCLength;
	UBYTE SCStatus;
	Bool EMV = 1;
	UBYTE HistBytesOffset;
	UBYTE HistLen;
	UBYTE status;
	Bool RetVal;
	

	// fear the worst
	RetVal = False;

	status = ScrdGetATR( scrd_SCR, EMV, SCBytes,
							 &SCLength, &HistBytesOffset,
							 &HistLen, &SCStatus );

	if ( EMVL1_OK == status && ATR_GOOD == SCStatus )
	{
		RetVal = True;
	}
	else
	{	 		
		memcpy( CSTMSG, "Card Fail", 10 );
		PerformCardClose(  );			
			
	}

	ScrdEndSession( scrd_SCR );

	return RetVal;
}


void PerformFallback(void)
{

    GetMsg( Swipecard, CSTMSG );

    PerformCardClose();

	// store original txn type (e.g. EMV_SALE)
	SecondEvent.TRNKEY = TRINP.TRORIGKEY = TRINP.TRKEY;

	// Get Transaction information
	FindTran( );

	// Set transaction name in TRTRANBUF
	memcpy( TRINP.TRTRANBUF, SecondEvent.TRNTEXT, sizeof( SecondEvent.TRNTEXT ) );

	// set nextevent = fallback, and change TRKEY
	CurrentEvent.NEXTEVENT = TRINP.TRKEY = EMV_FALLBACK;
}


extern Bool ReadICCTrack( void )
{
  UBYTE status;
  UBYTE	index = 0;
  CandTextType *pCurrentCand = 0;
  UBYTE	retval;

  retval = False;
  TRINP.TRPOSE[0] = 0x09;		// pos entry ICC
  TRINP.TRPOSE[1] = 0x02;

  
  if ( isCardInserted() )
  { 
    if ( InitUserCard( ) )
    {
      if ( EMVL2_Initialise( EMVVersion, CryptSlot ) )	// 2nd param is Atmel slot (0 = not present)
      {
          status = PerformAppSelection( &pCurrentCand, &index );
          switch ( status )
          {
            case EMVL2_SELECTED:
                 UpdFieldsPostAppSel( );
                 retval = True;
                 break;
            case EMVL2_NO_MORE_APPS:	                 
                 ShowErrMsg( NoMoreApps );
                 PerformCardClose();
                 break;
            case EMVL2_EASY_ENTRY:	
                 ShowErrMsg( EASYENTRY_MSG );
                 PerformCardClose();
                 break;
            case EMVL2_NON_EMV:
            case EMVL2_MAG_STRIPE:
                 ShowErrMsg( Msg_UseMagneticStripe );
                 PerformCardClose();                                                                    
                 break;
            case EMVL2_MNDTY_DATA:	
                 ShowErrMsg( AppBlocked );
                 PerformCardClose();
                 break;        
            case EMVL2_ICC_BLOCKED: 
                 ShowErrMsg( CardBlocked );
                 PerformCardClose();
                 break;   

            default:
                 // critical failure includes EMVL2_FAIL                
                 ShowErrMsg( CardFail );
                 PerformCardClose();
                 break;
          }                                          
          
      }
      else	// EMVL2_Initialise failed
      {
          ShowErrMsg( CardFail );
      }
    }
    else 
    {
      if ( TRINP.TRORIGKEY != INSTALLMENT )
      {
          CurrentEvent.NEXTEVENT = 0;
          SDK_BeepIt( 2 );
          retval = False;
      }
    }
  }

  return retval;
}


#endif























































































































































































































