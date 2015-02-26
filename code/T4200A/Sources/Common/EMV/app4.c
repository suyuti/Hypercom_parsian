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
#include "autotest.h"

extern Bool IsTrack2EquivDataConsistent( void );

//=============================================================================
// Private function declarations
//=============================================================================
static Bool MerchantForcedOnline( void );
static UBYTE TrainingModeProcessing( struct iccrec_rec *pIccEntry );
static Bool FindCardInHotlist( UBYTE * Pan, UBYTE Length, UBYTE Seq );
static Bool IsCardHot( UBYTE * Pan, UBYTE Length, UBYTE Seq );


//! Local Variables/Consts
static const ENTRY_STR ApprovalCodeEntry = {
	N_NullStr,					// Default Value 
	EnterApp,					// Prompt 
	N_NullStr,					// Not used 
};


//-----------------------------------------------------------------------------
//! \brief
//!     EMV transaction handler.
//!
//! This function uses the EMV Level 2 library.
//! When using this library, if status EMVL2_FAIL is returned this indicates
//! that a fatal error has happened and that the transaction must terminate
//! without performing any further smart card comms.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void EMVPROC( void )
{
	// app selection vars
	UBYTE index = 0;
	CandTextType *pCurrentCand = 0;

    UBYTE TargetPerCent;
    UBYTE MaxPerCent;
    UBYTE Threshold[8];

    UBYTE PrevAmount[6];

    UBYTE AppVersions[8];
	UBYTE status;
	struct iccrec_rec *iccptr;
	struct iccrec_rec IccEntry;
	struct taidrec_rec TAIDEntry;
	UBYTE AppAction;
    UBYTE TACs[15];
	Bool train_retval = False;
    Bool bHotCard = False;
    UBYTE chRetval;
    UBYTE chRetval1;

    // Test critical conditions
	if ( ( FALSE == TERM.TERMInit ) || ( JournalFullTest( ) ) )
	{
		return;
	}

	// If ICC not inserted, get it now or get out
	if (( DEV_SCR != GetLastInputType( )) && ( DEV_PINCARD != GetLastInputType( )) )
	{
        if ( !ForceICCInsertion( ) ) return;
	}

	// Get Cashier Number; do not allow NULL entry 
	if ( !InputCashier( CustomMsg, False ) )
	{
		// User cancel; return 
		return;
	}

	ShowInfoMsg( ProcCard, SmartCard );

	// Setup the transaction key
	TRINP.TRKEY = CurrentEvent.TRNKEY;
	
    // return to idle when finished
	CurrentEvent.NEXTEVENT = 0;

	TRINP.TRPOSE[0] = 0x00;		// pos entry ICC
	TRINP.TRPOSE[1] = 0x50;

	// set transaction time up
	SetTranTime( TRINP.TRYEAR );

    if ( isCardInserted() )
	{
        // Initialise user card, including re-entry if card read fail
		if ( InitUserCard() )
		{
			if ( EMVL2_Initialise( EMVVersion, CryptSlot ) )	// 2nd param is Atmel slot (0 = not present)
			{
                if ( TC6_ICCEARLYAMT & TCONF.TCOPT6 )
                {
					// 'Early Amount Entry for ICC Txns' 
					// Get Event message for title bar
					strcpy( CSTMSG, CurrentEvent.TRNTEXT );
					GetMsg( CustomMsg, DefaultTitle );	/* set title for screen  */
					chRetval = AmtProc();                 

				    if ( chRetval )
                    {
                        /* Show "Processing Card" */
						ShowInfoMsg( ProcCard, SmartCard );

                        // write amounts into tag store
                        WriteAmountData();
                    }
                    else
                    {
						// cancelled/timed out
                        PerformCardClose();
					    return;
                    }
                }

				/* Perform Application Selection */
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

						// Update EMV fields in library and message structure
                        chRetval = UpdFieldsPostAppSel();
                        chRetval1 = WriteAIDData( index );
                       
						if (chRetval != EMVL2_OK || chRetval1 != EMVL2_OK )
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
						if ( NULL == ( iccptr = FindIcc( TRINP.TRISID ) ) )
						{
							ShowErrMsg( UnsuppCard );
							break;
						}

						// Cash only or Cash Back txn allowed by issuer?
						if (  ( (EMV_CASH == TRINP.TRKEY) && !((TRINP.TRISPTR->ISOPT3) & IS3_CASH_TRN) )
							||( (EMV_SALCASH == TRINP.TRKEY) && !((TRINP.TRISPTR->ISOPT3) & IS3_CASHAMT) )
						)
						{
							/* No, show error and abort */
							ShowErrMsg( N_CashNotAllowed );
							break;
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
		                InInvoiceNum();

						// Redisplay Issuer and Transaction Name 
						ShowIssuer_Tran();

                        if ( !(TC6_ICCEARLYAMT & TCONF.TCOPT6) )
                        {
							// Do amount entry in parallel with data authentication
							if ( !AmtProc() )
							{
								/* Cancel/timeout during amount entry */
								break;
							}

                            // Send amount(s) to the tag store
							WriteAmountData();
                        }

						// See if in Training Mode
						if ( TCONF.TCOPT4 & TC4_TRAINING )
						{
							/* TRAINING MODE */
							// Processing depends on the amount,
							// train_retval == False -> continue with the txn
							// train_retval == True  -> abort the txn
							train_retval = TrainingModeProcessing( &IccEntry );
						}

						//@TUM DEBUG Test only
						if (train_retval)
						{
							break;
						}

						// Remove amount entry stuff from the display
						ShowInfoMsg( ProcCard, SmartCard );

						// Send floor limit / invoice number to the tag store
                        WriteFloorInvData( );

						// Wait for data authentication to complete
						if( TCONF.TCDOPT & TCD_ENH )
                            ShowInfoMsg( Authentication, SmartCard );
						else
                            ShowInfoMsg( ProcCard, SmartCard );

						// Data Authentication Process
						if ( EMVL2_FAIL == EMVL2_AuthenticateData( ) )
						{
							// catastrophic failure - we're out of here
							ShowErrMsg( CardFail );
							break;
						}

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

								// lack of break intentional;

							case EMVL2_CVM_DONE:	// nothing more to do
								/* Show "Processing Card" */
								ShowInfoMsg( ProcCard, SmartCard );

								// Perform Validate Transaction
								memset( Threshold, 0, sizeof( Threshold ) );
								memcpy( &Threshold[3], &IccEntry.ICCTHRESH, 2 );
                                Bcd2Bin( &TargetPerCent, &IccEntry.ICCTARGPCT,1 );
                                Bcd2Bin( &MaxPerCent, &IccEntry.ICCMAXTPCT, 1);
								BuildVerBuff( AppVersions, TAIDEntry );

								/* 26/06/2003 Calculate the 'Previous Amount' if the feature is supported */
								memset( PrevAmount, 0, sizeof(PrevAmount) );

		                        if ( LocalAppCap[1] & APP_CAP2_TXN_LOG )
//@ASSA at 22.08.2005 									if ( TRINP.TRAQPTR->AQOPT1 & AQ1_PREVAMT )
								{
									GetPreviousAmount( PrevAmount );
								}
		                        // check the exception file
		                        bHotCard = IsCardHot( (UBYTE *)&TRINP.TRPAN, 8, TRINP.TRPANSEQ );

                            	// Read TACs from TermMaster
                            	memcpy(TACs, &IccEntry.ICCTACDEN, 5);
                            	memcpy(TACs + 5, &IccEntry.ICCTACONL, 5);
                            	memcpy(TACs + 10, &IccEntry.ICCTACDEF, 5);

								status = EMVL2_ValidateTransaction( bHotCard,
                                                                   MerchantForcedOnline(),
																   PrevAmount,
																   AppVersions, TargetPerCent,
																   MaxPerCent,
																   ( UBYTE * ) &Threshold );

								if ( status != EMVL2_FAIL )	// status only = _OK or _FAIL
								{
									// status is result of GenAC passed upwards
									if ( (IccEntry.ICCOPT1 & ICC1_TRK2CMP) && !IsTrack2EquivDataConsistent( ) )
									{
										// Detected inconsistency between Tag '57' & Tags '5A', '5F24'
										// Ask for AAC (ETEC 4.0.C test requirement)
										AppAction = EMVL2_DECLINE;
									}	// if ( !IsTrack2EquivDataConsistent( ) )

									AppAction = MerchantForcedOnline() ? EMVL2_ONLINE : EMVL2_OK;

									status = EMVL2_ActionAnalysis( AppAction,
                                                                  ( UBYTE * ) &IccEntry.ICCTACDEN,
																  ( UBYTE * ) &IccEntry.ICCTACONL,
																  ( UBYTE * ) &IccEntry.ICCTACDEF );

                                    // Informational advice messages are an optional EMV feature and the EMVL2
                                    // library will indicate when the creation of such a message has bee requested
                                    // Add code here to handle the creation of such informational advice messages.
                                    if (status & BITMAP_RETVAL) // top bit indicates this response contains a card decision bitmap
                                    {
                                        // mask off the advice request bits before decoding the rest
                                        status &= BITMAP_RETVAL | ~(ADVICE_REQUESTED|SCRIPT_PERFORMED);
                                    }

		                            switch ( status ) // action analysis result
		                            {
                                        case EMVL2_ACCEPT:
											UpdFinalResponseCode( );
				                            // Update the fields post GenAC 
				                            if ( EMVL2_OK == UpdFieldsPostGenAC( ) )
				                            {
                                                // Set the time
	                                            SetTranTime( TRINP.TRYEAR );

					                            // Process offline transaction 
					                            EMV_Offline( );
				                            }
				                            else
				                            {
					                            // catastrophic failure - we're out of here
												TransactionVoid( );
												SuppressSigLine = True;
				                            }
				                            break;

			                            case EMVL2_ONLINE:
											// Update fields after GenAC
                                           	UpdFieldsPostGenAC( );

				                            // Go online 
				                            status = GoOnline( &IccEntry );

											// handling for 2nd GenAC
											switch ( status )
											{
												case EMVL2_ONLINE_CANCELLED:                                            
													// Show decline msg
													ShowErrMsg ( N_UserCancelled );
													SuppressSigLine = True;                                                  
													break;

												case EMVL2_DECLINE:
													
													// Show decline msg
													ShowInfoMsg( CardDeclined, SmartCard );
													SuppressSigLine = True;
                                                    memset ( TRINP.TRAUTH, 0, S_TRAUTH ) ; // BD cleared this here for 2CC.122.01
													break; 
//@TUM 13/11/2003 Europay/Mastercard requirement: If the card returns SW1,SW2 == '6985'
// in response to Generate AC or Ext. Authenticate, the terminal must allow the Fallback.
// Start of patch
												case EMVL2_MAG_STRIPE:
													// Switch to FALLBACK
													TransactionVoid();

									                // store original txn type (e.g. EMV_SALE)
									                TRINP.TRORIGKEY = TRINP.TRKEY;

									                // set nextevent = fallback, and change TRKEY
									                TRINP.TRKEY = EMV_FALLBACK;
// End of patch
                                            } // end switch GoOnline result
											break;

         
                                        case EMVL2_REFERRAL:
								             // Add code here to handle voice referral processing
                                             // or allow to drop through to become a decline
//											ShowErrMsg( CardDeclined );
//											SuppressSigLine = True;
//											break;

                                        case EMVL2_DECLINE:
	                                        // Show decline message
						                    // Update the fields post GenAC 
                        					UpdFieldsPostGenAC();
											// We should not trust the Response Code here, we will force it to
											// 'Z1' (declined) to show the correct msg on the screen & receipt
											// UpdFinalResponseCode();
											memcpy( TRINP.TRRSPC, "Z1", S_TRRSPC);
                                            ShowErrMsg( CardDeclined );
											SuppressSigLine = True;                                            
	                                        break;

                                        default:
	                    					TransactionVoid();
						                    // Show "Processing Card"
	                                        ShowInfoMsg( ProcCard, SmartCard );
                                    }	// end switch ActionAnalysis
										
									// End the session with the Smart Card and force removal
									PerformCardClose();

									/* Display final response from TRINP */
									RspLookUp( &TRINP );

									/* Replace the Response Message for Approved Refund type transaction */
									if ( R_APPROVED & TRINP.TRRSPOPT && CurrentEvent.TRNOPT1 & T1_CREDIT )
									{
										GetMsg( RefundTran, (char *)&TRINP.TRHOSTTEXT[0] );	// 1st line 'REFUND'
										GetMsg( Accepted, (char *)&TRINP.TRHOSTTEXT[20] );	// 2nd line 'ACCEPTED'
									}

									/* Display Response */
									RespDisplay();					
									Receipt();                                   
								}
								else	// validate transaction = EMVL2_FAIL
								{
									TransactionVoid( );
									ShowErrMsg( CardFail );
								}
							break;

                            case EMVL2_ONLINE_CANCELLED:
                            {
                                // Cancelled by user while doing CVM processing
                                ShowErrMsg ( N_UserCancelled );
                                SuppressSigLine = True;                                                  
                                break;
                            }
                            // case EMVL2_FAIL:
							default:
								TransactionVoid( );
								if ( OnlineCancelled )
								{
									ShowErrMsg( N_UserCancelled );
								}
								else
								{
									ShowErrMsg( CardFail );
								}
								break;

						}	// end CVM Processing switch
                        break;
					}

					case EMVL2_NO_MORE_APPS:	// Candidate list exhausted (mag stripe not permitted)
						ShowErrMsg( NoMoreApps );
						break;

					case EMVL2_EASY_ENTRY:	  // Easy Entry card found, continue as per mag stripe
                        ShowErrMsg( EASY_ENTRY );
                        break;

                    case EMVL2_NON_EMV:		// Non EMV card found, perform a fallback mag stripe txn 
                    case EMVL2_MAG_STRIPE:	// No supported applications, a mag stripe txn may be performed
						// store original txn type (e.g. EMV_SALE)
						TRINP.TRORIGKEY = TRINP.TRKEY;
						// change TRKEY
						TRINP.TRKEY = EMV_FALLBACK;
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
				}// end app selection result switch 

				// We get here if train_retval == True (Training Mode only)
				if ( train_retval )
				{
					/* Display final response from TRINP */
					RspLookUp( &TRINP );

					/* Display Response */
					RespDisplay ();

					Receipt ();
				}	// if ( train_retval )

				// print ICC debug information
				if ( PrintEMVDebug )
				{
					PrintICCDebug( EMVVersion, pCurrentCand, PrevAmount );
				}

			}
			else				// EMVL2_Initialise failed
			{
				ShowErrMsg( CardFail );
			}
		}
		else					// init user card failed
		{
			// InitUserCard failed
			ShowErrMsg( CardFail );
		}

	}							// smart card insert check failed


	if( isCardInserted() )
	{
		// End the session with the Smart Card and force removal
		PerformCardClose();
	}

	// Check if FALLBACK is to be initiated
	if ( EMV_FALLBACK == TRINP.TRKEY )
	{

		// store original txn type (e.g. EMV_SALE)
		SecondEvent.TRNKEY = TRINP.TRORIGKEY;

		// Get Transaction information
		FindTran ();

		// Set transaction name in TRTRANBUF
		memcpy( TRINP.TRTRANBUF, SecondEvent.TRNTEXT, sizeof( SecondEvent.TRNTEXT ) );

		// set nextevent = fallback
		CurrentEvent.NEXTEVENT = EMV_FALLBACK;

		// Switch to Fallback transaction
		return;
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Handles the FALLBACK transaction after chip failure
//!
//! \return
//!     None
//!
//! \note
//!		Amount string is stored in global buffer Dspbuf. Journal full already
//!     tested in EMVSALE
//-----------------------------------------------------------------------------
void FALLBACK( void )
{
	TRINP.TRKEY = EMV_FALLBACK;
	TRINP.TRPOSE[0] = 0x00;		// pos entry fallback
	TRINP.TRPOSE[1] = 0x92;

	// Set flag, it will prevent ReadData() from allowing 'Insert Card'
	TRINP.TRSTATUS[1] |= TS2_FALLBACK;

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
		case EMV_CASH:			/* switch to DBCASH */
			CurrentEvent.NEXTEVENT = DBCASH;
			break;

		case EMV_SALCASH:		/* switch to SALE+CASH */
			CurrentEvent.NEXTEVENT = SALCASH;
			break;

		case EMV_SALE:			/* switch to SALE */
			CurrentEvent.NEXTEVENT = SALE;
			break;

		case EMV_REFUND:			/* switch to REFUND */
			CurrentEvent.NEXTEVENT = REFUND;
			break;
	}

	TRINP.TRORIGKEY = 0;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Gets Forced Online capability for current transaction
//!
//! \return
//!     True if it is allowed else False
//-----------------------------------------------------------------------------
static Bool MerchantForcedOnline( void )
{
    // don't allow if capability bit not set
    if (!(LocalAppCap[1] & APP_CAP2_MERCH_SUSP)) 
        return False;

	switch( TRINP.TRKEY )
	{
	case EMV_SALE:
		return False;
	case EMV_CASH:
	case EMV_SALCASH:
	case EMV_REFUND:
		return True;
	}
	return False;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Special processing for training mode
//!
//! \param
//!		[out]pIccEntry		Pointer to chip card entry
//!
//! \return
//!     False - continue with the transaction, True - Abort
//-----------------------------------------------------------------------------
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
//! \brief
//!     Checks against the hot card list of one card for cert purposes only
//!
//! \param
//!		[in]Pan				Pointer to PAN
//!		[in]Length			Length of PAN
//!		[in]Seq				unknown
//!
//! \return
//!     True if card is in hot card list else False
//-----------------------------------------------------------------------------
static Bool IsCardHot( UBYTE * Pan, UBYTE Length, UBYTE Seq )
{
    if (LocalAppCap[1] & APP_CAP2_EXC_FILE_TEST)
    {
        if ( FindCardInHotlist( Pan, Length, Seq ) )
            return True;
    }

    return False;

}

//-----------------------------------------------------------------------------
//! \brief
//!     Finds card in hot card list
//!
//! \param
//!		[in]Pan				Pointer to PAN
//!		[in]Length			Length of PAN
//!		[in]Seq				unknown
//!
//! \return
//!     True if card is in hot card list else False
//-----------------------------------------------------------------------------
static Bool FindCardInHotlist( UBYTE * Pan, UBYTE Length, UBYTE Seq )
{
// hotlist implementaion pending
    return False;
}
#endif

