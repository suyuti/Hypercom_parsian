
#ifdef MAKE_EMV
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     emvutils.c
//  EMV application processing
//
//=============================================================================

#include "basictyp.h"
#include "osclib.h"
#include "ddtm.h"
#include "fixdata.h"
#include "utllib.h"
#include "emv_l2.h"
#include "emv2_pk.h"
#include "emvdata.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "scard.h"
#include "string.h"
#include "emvtabs2.h"
#include "emvaptag.h"
#include "pinpad.h"
#include "tables.h"
#include "findtran.h"
#include "journal.h"
#include "flaghost.h"
#include "predial.h"
#include "pinutils.h"
#include "refproc.h"
#include "hstjs.h"
#include "hstmsg.h"
#include "cstmamnt.h"
#include "genauth.h"

#include "emvutils.h"
#include "sdktime.h"
#include "respdsp.h"
#include "comm.h"
#include "pinpad.h"


extern void GenAuth_TRINP( void );

//=============================================================================
// Private function declarations
//=============================================================================
UBYTE PerformCVMProcessing( void );
static UBYTE EMVPinEntry( UBYTE * pindata, Bool Retry );
UBYTE PerformAppSelection( CandTextType **, UBYTE * );
UBYTE HandleCertOfflineReferral( struct iccrec_rec * pIccEntry, UBYTE *TACDef );
Bool ForceICCInsertion( void );
Bool isCardInserted( void );
void GetKeyPress ( void );
Bool InitUserCard( void );

void BuildVerBuff( UBYTE * buffptr, struct taidrec_rec entry );
UBYTE GoOnline( struct iccrec_rec * pIccEntry, UBYTE *TACDef );
Bool IsTrack2EquivDataConsistent( void );
static void AddTxnToJournal( void );
void AddZeroAdvToJournal( void );
#ifdef MAKE_INFADVONLINE 
static void SendInfAdvOnline( void );	//@pl 
#endif
static void Trn2InfAdv( void );	//@pl 
static void InfAdv2Trn( void );	//@pl 
static void AddRevTxnToJournal( void );
static void CertDeclinedByCard ( void );
static UBYTE HandleCertHostReferral( struct iccrec_rec * pIccEntry, UBYTE *TACDef );
static UWORD get_auth( void );
static struct trandata_rec TempTrandata;		// temporary TRANSACTION DATA buffer

/* --- Local variables  ---- */
SupportedAIDType *pSupportedAID;
struct taidrec_rec *pTAIDEntry;
static UWORD pinlen;
static UBYTE pinblock[13];

/* local constants */
static const ENTRY_STR PinEntry = {
	N_Pinpad,					/* Title */
	CustomMsg,					/* Prompt */
	CustomMsg2,					/* Default value */
};


/* Prompt the user to select or confirm an application  */
UBYTE UserSelection( CandTextType * pCandList, UBYTE ListLen );


/*-------- local static variables ---------*/
static const UBYTE POSENTRYMODE = 05;   // Default POS entry mode
static ENTRY_STR YNEntry;	// structure for YesNoEntry() 
// Hard Coded structure for Training Mode processing

static const SupportedAIDType SupportedAIDTraining = { 0x05,
													   { 0x98, 0x76, 0x54, 0x32, 0x10, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },	
													   0,
													   0,	
														};
/* Array of tagtab_rec structures, each entry represents
   a unique tag and the way to get the data */
/* 
 * Note on tagtab_rec: If function is used, it must move requested data
 * to address defined with pAddr in this structure because 
 * pAddr is used in any case. Please keep the order, add new tags only at the end 
 * of the table.
 */

//static const tagtab_rec TAG_TAB[] = {
//	{TAG_9F15, DOL_MCATCODE, NULL, 2},      // 1 MERCHANT CATEGORY CODE
//	{TAG_9F1E, NULL, TERM.TERMSer, 8},      // 2 IFD SERIAL NUMBER
//	{TAG_9F39, NULL, &POSENTRYMODE, 1},	    // 3 POS ENTRY MODE
//	{TAG_9F3C, NULL, TCONF.TCCURRCODE, 2},  // 4 REFERENCE TRANSACTION CURRENCY CODE
//	{TAG_9F3D, DOL_CURREXP, NULL, 1},       // 5 REFERENCE TRANSACTION CURRENCY EXPONENT
//	{TAG_9F41, DOL_SEQCNTR, NULL, 4},       // 6 TRANS. SEQ. COUNTER
//};

/*-----------------------------------------------------------------------
 * Name:           UserSelection
 * Description:    User entry for Application selection
 *
 * Parameters:     pCandList: pointer to candidate list
 *                 ListLen: number of candidates in list
 * Return Value:   original index as per selected AID, 
 *                 or 0xFF if cancelled
 * Notes:          The code table from candidate list is displayed in brackets
 *                 Will be modified not to use YNEntry on next pass
 *                 to show "YES/NEXT" for multi application candidate list
 *-----------------------------------------------------------------------*/

UBYTE UserSelection( CandTextType * pCandList, UBYTE ListLen )
{
	CandTextType *pCurrentListEntry;
	UBYTE CurrentListEntryIdx;
	UBYTE YNReturn;
	UBYTE RetVal;
	Bool CompletedSelection;

	GetMsg( APPLICATION_SELECT, CSTMSG );
	pCurrentListEntry = pCandList;
	CurrentListEntryIdx = 1;
	CompletedSelection = False;
	RetVal = 0xFF;				// cancelled

	while ( !CompletedSelection )	// complete on selection or cancel
	{
		// set select with code table
		// MsgSelect[8] = 0x30 + ((pCurrentListEntry->code_table & 0xF0) << 4);
		// MsgSelect[9] = 0x30 + (pCurrentListEntry->code_table & 0x0F);

		// copy one more than present to clear line
		GetMsg( SELECT_MSG, CSTMSG2 );

		// set up the application name
		memcpy( CSTMSG3, pCurrentListEntry->text,
				strlen( ( char * ) pCurrentListEntry->text ) + 1 );

		// set up entry display
		YNEntry.TitleMid = CustomMsg;
		YNEntry.Line1Mid = CustomMsg2;
		YNEntry.Line2Mid = CustomMsg3;

		// display the selection & seek conf
		YNReturn = 0;
		while ( YNReturn == 0 )
		{
			if ( ListLen == 1 )
			{
				YNReturn = YesNoEntry( &YNEntry );
				switch ( YNReturn )
				{
					case 0:	// timeout
						break;

					case ENTER_KY:	// YES key was pressed 
						// Return without allowing other choices
						CompletedSelection = True;
//                        RetVal = pCurrentListEntry->orig_index;
						RetVal = --CurrentListEntryIdx;
						break;

					case CLEAR_KY:	// NO key
					case CANCEL_KY:	/* CANCEL key pressed */
					default:
						CompletedSelection = True;
						RetVal = 0xFF;	// cancelled 
						break;

				}
			}
			else
			{
				YNReturn = YesNextEntry( &YNEntry );
				switch ( YNReturn )
				{
					case 0:	// timeout
						break;

					case CLEAR_KY:	// NEXT key
						//set up next
						if ( CurrentListEntryIdx < ListLen )
						{
							pCurrentListEntry++;
							CurrentListEntryIdx++;
						}
						else
						{
							// reset to start
							CurrentListEntryIdx = 1;
							pCurrentListEntry = pCandList;
						}
						break;

					case ENTER_KY:	// YES key was pressed 
						// Return without allowing other choices
						CompletedSelection = True;
//                      RetVal = pCurrentListEntry->orig_index;
						RetVal = --CurrentListEntryIdx;
						break;

					case CANCEL_KY:	/* CANCEL key pressed */
					default:
						CompletedSelection = True;
						RetVal = 0xFF;	// cancelled 
						break;

				}
			}
		}
	}

	return ( RetVal );
}

/*------------------------------------------------------------------
 *  Name:           PerformCVMProcessing
 *  Description:    Do CVM processing on behalf of txn
 *  Parameters:     none
 *  Return Values:  CVMstatus - the final word on CVM processing outcome
 *					EMVL2_CVM_DONE          process completed
 *					EMVL2_CVM_SIG           signature required 
 *					EMVL2_CVM_PIN_FAIL_SIG  pin failed, sig required 
 *					EMVL2_CVM_PIN_FAIL      pin failed 
 *					EMVL2_CVM_BLOCKED_SIG   pin blocked, sig required 
 *					EMVL2_CVM_BLOCKED       pin blocked
 *					EMVL2_CVM_ONP           send PIN online - not passed back at moment
 *					EMVL2_FAIL              process critically failed
 *  Notes:          Functionality isolated for use in various txn types
 *-----------------------------------------------------------------*/
UBYTE PerformCVMProcessing( void )
{
	UBYTE CVMstatus;
	UBYTE PINStatus;
	UBYTE CVFinished;
	Bool isPINReEntry;
	UBYTE PINdata[8];
	UBYTE c_len;
	Bool LastTry = False;
	UBYTE TempBuffer;

	// initialise global Suppress signature flag
	SuppressSigLine = True;
	TRINP.TROPTEMV |= TROPTEMV_NOSIG;

	// Initialise CVM processing flags
	PINStatus = EMVL2_UNUSED_CVM_STATUS; // value not used on first entry to CVM
	CVFinished = False;

    // Initialise PINData with Custom Conditions and Custom Methods
    // Bytes 0-3 are Conditions, 4-7 Methods, 00 indicates a null entry
    // Conditions example
    //    PINdata[0] = 0x80; // bottom of range
    //    PINdata[1] = 0xFF; // top of range
    //    PINdata[2] = 0x00; // null entry
    //    PINdata[3] = 0x00; // null entry
    // Methods example
    //    PINdata[4] = 0x70; // EP2 card custom method value 
    //    PINdata[5] = 0x20; // lowest allowable value custom methods (without "succeeding" bit)
    //    PINdata[6] = 0x3E; // highest allowable value custom methods (without "succeeding" bit)
    //    PINdata[7] = 0x00; // null entry

    PINdata[0] = 0x00; // null entry condition
    PINdata[1] = 0x00; // null entry condition
    PINdata[2] = 0x00; // null entry condition
    PINdata[3] = 0x00; // null entry condition
    PINdata[4] = 0x00; // null entry method
    PINdata[5] = 0x00; // null entry method
    PINdata[6] = 0x00; // null entry method
    PINdata[7] = 0x00; // null entry method


	isPINReEntry = False;
	while ( CVFinished == False )
	{
		CVMstatus = EMVL2_CardholderVerification( PINdata, &PINStatus );

		switch ( CVMstatus )
		{

   // The EMV PIN entry process can be performed either by the EMV library itself or by an external PED.
   // The choice of which to mode use depends upon upon the relative positioning of the Pin entry device
   // and card coupler. If both card coupler and PED are externally located in the same device it is 
   // better to allow the Pin entry process to be performed by the PED, otherwise the library should perform
   // PIN processing itself.
   //
   // Examples
   // Hardware  Coupler   PED         Recommendation 
   // ICE5500   Internal  Internal    PIN processing performed by library
   // T7/S8     Internal  External    PIN processing performed by library
   // T7/HFT105 External  External    PIN processing performed by PED
   // HFT825    Internal  External    PIN processing performed by library
   // HFT825    External  External    PIN processing performed by PED
   // 
   // The library supports both modes of operation and either is used by providing two parameters, a
   // buffer and a status value. The buffer contains data which is being submitted to the library and the 
   // status value identifies the precessing to be used.

   
   // 1. PIN processing performed by library.
   // PINStatus             // Meaning
   // EMVL2_PIN_ENTERED     // PINData buffer contains 8 byte PIN (format = plain text e.g 041234FFFFFFFFFF) 
   // EMVL2_EPIN_ENTERED    // PINData buffer contains 16 byte externally encrypted PIN block (format = unknown)
   //                          This is a high security mode which requires the presence of a crypto-processor.
   //                          This mode is "transparent" in that the 16 byte value supplied will pass through the library 
   //                          to the crypto-processor unmodified. It is assumed that the external PED and 
   //                          crypto-processor share a common secret but the method by which this is established is not 
   //                          defined here.
   // EMVL2_PIN_BYPASSED    // User has bypassed PIN entry
   // EMVL2_PINPAD_FAILURE  // PIN entry device not responding

   
   // 2. PIN processing performed by external PED
   // PINStatus             // Meaning
   // EMVL2_PIN_OK          // External PIN entry device has successfully validated a PIN
   // EMVL2_PIN_BLOCKED     // External PIN entry device reports PIN is blocked
   // EMVL2_PIN_BYPASSED    // External PIN entry device reports user has bypassed PIN entry
   // EMVL2_PINPAD_FAILURE  // External PIN entry device not responding
   // EMVL2_PINICC_FAIL     // External PIN entry device reports customer ICC failure
   // EMVL2_PINSIM_FAIL     // External PIN entry device reports SIM (Atmel failure)

   // Note, when using PINStatus = EMVL2_PINSIM_FAIL or EMVL2_PINICC_FAIL, the SW1 SW2 error code 
   // received from the PED should be supplied to the library via the PINdata buffer. Although not mandatory, values
   // supplied in this way will be copied to the library debug data buffer.


   // Note EMV supports two forms of offline PIN, plain text and encrypted. This means that the PIN block transmitted 
   // from the library to the ICC may be in plain text or encrypted. To complicate matters, the EMV library itself can 
   // accept incoming PIN data in either plain text or encrypted formats. There are 4 logical combinations.

   // Lib Input       Lib Output
   // Plain           Plain
   // Encrypted       Plain
   // Plain           Encrypted
   // Encrypted       Encrypted

   // The lib input format is determined by the programmer via the EMVL2_PIN_ENTERED and EMVL2_EPIN_ENTERED status values.
   // The lib output format is specified by the ICC and is automatically handled by the library.


   // Internal PIN Pad, handle plain text and encrypted cases together.
			case EMVL2_CVM_OFP:  // Plain text
                        case EMVL2_CVM_OFE:  // Encrypted

				LastTry	= False;

				if ( !isPINReEntry )
				{
					// This is not PIN Retry, analize the PIN Try Counter 
					// and display 'LAST TRY LEFT' in case of the last try 
					// (according to ADVT 47 test)

					c_len = 0;

					EMVL2_ReadTagData( &TempBuffer, &c_len, TAG_9F17);

					if ((c_len > 0) && (TempBuffer == 1)) 
					{
						LastTry = True;
					}
				}


				if ( isPINReEntry  || LastTry )
				{
					// convert pin try count to decimal
					GetMsg( TRIES_LEFT, CSTMSG );

					// Get the lenght of the Cashier/Server prompt
					c_len = StrLn( CSTMSG, sizeof( CSTMSG ) );

					CSTMSG[c_len] = ( PINStatus > 10 ) ? 0x31 : 0x20;
					CSTMSG[c_len+1] = 0x30 + PINStatus % 10;
					CSTMSG[c_len+2] = 0x00;
					ShowErrMsg( CustomMsg );
				}

#ifdef MAKE_EXTPIN
	// need this if we wish to have External Pinpad/Smart Card Reader
			if (ScrdIntExt() == 1)
			{
				switch (CVMstatus)
				{
					case EMVL2_CVM_OFP:  // Plain text
						APP_PerformCrossCall (X_PinExtOffline) ;
						break ;

                                      case EMVL2_CVM_OFE:  // Encrypted
						APP_PerformCrossCall (X_PinExtEncOffline) ;
						break ;
				}
				PINStatus = TRINP.TRPINBL[0] ;
				PINdata[0] = TRINP.TRPINBL[1] ; // copy SW1
				PINdata[1] = TRINP.TRPINBL[2] ; // copy SW2
			}
			else
#endif
			{
				// EMVPinEntry return values
				// 0 - PIN entered
				// !0 - PIN entry problem
				    PINStatus = EMVPinEntry( PINdata, isPINReEntry );

                                    // pin bypass izni yoksa çýk 
                                    if ( (PINStatus == EMVL2_PIN_BYPASSED) && (!(TCONF.TCOPT6 & TC6_PINBYPASSTERM)) )
                                    {	
					CVMstatus = EMVL2_FAIL;					
					CVFinished = True;
                                    }
                                  
			}

				isPINReEntry = True;
				break;			// going back in with PIN

			case EMVL2_FAIL:
				// exit and display critical failure
                                ShowErrMsg( CardFail);
				CVFinished = True;
				break;

			case EMVL2_CVM_SIG: // Signature required
				// carry on - get signature
				//@pl SuppressSigLine = False;
			    //@pl reset 'SIGNATURE IS NOT REQUIRED' bit
                                TRINP.TROPTEMV &= ~TROPTEMV_NOSIG;
				CVFinished = True;
				break;

			case EMVL2_CVM_ONE:  // Online Enciphered PIN required
				// get PIN
				   if ( GetPIN () == 2)
					{
					    PINStatus = EMVL2_PIN_ENTERED;
					}
				    else
					{
					    CVMstatus = EMVL2_FAIL;					
                                            CVFinished = True;
					}
				// have to go back in with PIN status
//@pl EMV test 				}
				
				
				break;

			case EMVL2_CVM_PIN_FAIL_SIG:
				// carry on - get signature
				//@pl SuppressSigLine = False;
			    //@pl reset 'SIGNATURE IS NOT REQUIRED' bit
			    TRINP.TROPTEMV &= ~TROPTEMV_NOSIG;

				// get sig - warning will be issued in case below
				CVMstatus = EMVL2_CVM_SIG;

				// no break intentional

			case EMVL2_CVM_PIN_FAIL:
				GetMsg( PIN_TRIES_EXCEEDED, CSTMSG );
				ShowErrMsg( CustomMsg );
				CVFinished = True;
				break;

			case EMVL2_CVM_PIN_BLOCKED_SIG:
				// carry on - get signature
				//@pl SuppressSigLine = False;
			    //@pl reset 'SIGNATURE IS NOT REQUIRED' bit
			    TRINP.TROPTEMV &= ~TROPTEMV_NOSIG;

				// get sig - warning will be issued in case below
				CVMstatus = EMVL2_CVM_SIG;

				// no break intentional

			case EMVL2_CVM_PIN_BLOCKED:
                                GetMsg( PIN_BLOCKED, CSTMSG );
				ShowErrMsg( CustomMsg );
				CVFinished = True;
				break;

            case EMVL2_CVM_CUST_COND:
                // The CVM process has passed us back a condition to evaluate.
                // Insert code here which will evaluate the condition and set PINstatus accordingly,
                // as in the example below:
                //      if (CondEvaluationFunc())
                          PINStatus = EMVL2_CONDITION_MET_TRUE;
                //      else 
                //          PINStatus = EMVL2_CONDITION_MET_FALSE;
                break;

            case EMVL2_CVM_CUST_METHOD:
				// The CVM process has passed us back a method to attempt in PINData[0].
                // Insert code here to perform the method and set PINStatus accordingly,
                // PINStatus should be set to: 
                //    EMVL2_CUST_METHOD_SUCCESS : method was succesful
                //    EMVL2_CUST_METHOD_FAIL    : method failed
                //    EMVL2_CUST_METHOD_UNKNOWN : method has unknown result (like signature)
                //
                // cf. following example
                // switch (PINData[0])
                // {
                //     case MY_CUST_METHOD_1: // A custom method 
                //         PINStatus = MyMethod1Func();
                //         break;
                //     case MY_CUST_METHOD_2: // A 2nd custom method
                //         PINStatus = MyMethod2Func();
                //         break;
                //     default: 
                //         // an unsupported method has been proposed
                //         // this means faulty data was passed into CVM in the first place
                         PINStatus = EMVL2_CUST_METHOD_FAIL;
                // }
                break;

			case EMVL2_CVM_DONE:
				// carry on - we're finished
				CVFinished = True;
				break;

            default:
                break;
		}
	}

	return ( CVMstatus );
}

/*------------------------------------------------------------------
 *  Name:           EMVPinEntry
 *  Description:    
 *  Parameters:     pindata
 *  Return Value:   
 *  Notes:          None.
 *-----------------------------------------------------------------*/
static UBYTE EMVPinEntry( UBYTE * pindata, Bool Retry )
{
	UBYTE status, i, c;
	UBYTE *pinptr = pinblock;
	static ENTRY_STR Entry;
         char PinDataXX[20];
        

        CustomAmount( AmountTotals, TRINP.TRTOTAM );

	memset( pinblock, 0, sizeof( pinblock ) );
	memset( pindata, 0xFF, 8 );
        
        if ( True == Open_Config(  ) )
        { 
           // external pinpad baðlý ise 
           if ( PIN_TYPE_INTERNAL != PIN_TYPE )
           {    
               // PinGetPin( True );
               if ( ReadFromS9( True ) )
               {
                   memset(PinDataXX, 0, 20);
                   if ( DecPin(PinDataXX) )
                   {
                      memcpy(( char * ) pinblock, PinDataXX, 12);  
                      status = EMVL2_PIN_ENTERED; 
                   }
                   else
                   {
                      status = EMVL2_PIN_BYPASSED;
                   }
               }
               else
               {
                  status = EMVL2_PIN_BYPASSED;
               }
       
           }
           else
           {
              if ( !Retry )
              {
                  if ( 1 == TRINP.TRPINBL[1] )
                  {
                        SDK_Beeper(30 * TENMS);
			GetMsg( LAST_PIN_TRY_LEFT, CSTMSG );
			GetMsg( PRESS_OK_TO_CONTINUE, CSTMSG2 );
			Entry.TitleMid = N_Pinpad;
			Entry.Line1Mid = CustomMsg;
			Entry.Line2Mid = CustomMsg2;
			// Wait for 'Continue' to be pressed
			ContinueEntry( &Entry );                  
                  }
                  else
		{
			// Normal sound if not retry
			SDK_Beeper(12 * TENMS);
		}	
              }

              if ( Retry )
              {
                  GetMsg( Msg_Reenter_Pin, CSTMSG2 );
              } 
              else
              {
                  GetMsg( EnterPin, CSTMSG2 );
              }
              do
              {
                  status = NumEntry( 2, 12, 4, &PinEntry );
                  if ( 1 == status )
                  {
                      MoveTillZero( ( char * ) pinblock, ( char * ) Dspbuf,
						  sizeof( pinblock ) );
                      status = EMVL2_PIN_ENTERED;
                      break;
                      
                  }
                  else
                  {
                      return EMVL2_PIN_BYPASSED;
                  }              
              }while ( True );
           }        
        }
        

        pinlen = strlen( ( char * ) pinblock );

        *pindata++ = (pinlen | 0x20);

	for ( i = 0; i < pinlen; i++ )
	{
		c = ( *pinptr++ ) << 4;	// Bump pointer and get one byte

		if ( 0 == *pinptr )		// If last char reached, stop building the pin block
		{
			*pindata = c | 0x0F;
			break;
		}

		*pindata++ = c | ( ( *pinptr++ ) & 0x0F );
		i++;
	}

	return status;				
}  

/*------------------------------------------------------------------
 *  Name:           PerformAppSelection 
 *  Description:    Do application selection on behalf of txn
 *  Parameters:     none
 *  Return Value:   status - the final word on app selection outcome
 *                  EMVL2_FAIL, EMVL2_SELECTED, EMVL2_EASY_ENTRY, 
 *                  EMVL2_MAG_STRIPE, EMVL2_NO_MORE_APPS,   
 *  Notes:          1. Functionality isolated for use in various txn types
 *                  2. Early return on failure to write terminal data to store
 *-----------------------------------------------------------------*/
UBYTE PerformAppSelection( CandTextType ** ppCandidate, UBYTE * AIDindex )
{
	UBYTE status;				// return value
	UBYTE appl;					// application
	UBYTE index = 0;			// search variable
        UBYTE TempAIDVer[2];
	UBYTE length;
	UBYTE tempRID[16];
	UBYTE mcdRID[] = {0xA0,0x00,0x00,0x00,0x04};
	UBYTE	vcdRID[] = {0xA0,0x00,0x00,0x00,0x03};
	UBYTE *debug;
	SupportedAIDType SupportedAID[10];
	CandTextType *pCandTextList = 0;

	// Send some terminal data to the library:
	// 9F1A - Country Code, 5F2A - Txn Currency Code
	// 9C   - Txn Type,     9F33 - TCap
	// 9F40 - Add Term Caps 9F1B - Terminal Floor Limit
	// 9F35 - Term Type     9F21 - Txn Time
	// 9A   - Txn Date      9F37 - Unpredictable number
	// (then get out if it didn't work)
	if ( WriteTermData( ) == EMVL2_FAIL )
		return ( EMVL2_FAIL );

	// Collect the magic data
    //EMVL2_ReadTagData(&tempRID[0], &length, TAG_9F21);  // Transaction Time
    //EMVL2_ReadTagData(&tempRID[3], &length, TAG_9F35);  // Terminal Type
	//EMVL2_ReadTagData(&tempRID[4], &length, TAG_9C);    // Transaction Type
    //EMVL2_ReadTagData(&tempRID[5], &length, TAG_9A);    // Transaction Date
    //EMVL2_ReadTagData(&tempRID[8], &length, TAG_9F37);  // Random Number
	//memcpy(&tempRID[8], &tempRID[12], 4);               // and again

	// Calculate the magic TestAppValue
	//for(index = 0; index < 8; index++)
	//{
	//	tempRID[index] ^= tempRID[index + 8];
	//}

	// Write the magic value to the tag store
    //EMVL2_WriteTagData(tempRID, 8, TAG_TE, TAG_9F1D);  // Terminal risk management data


	// Build terminal supported application list 
	while ( (index < TAIDMAX) && TAIDTAB[index].TAIDNUM )
	{
		SupportedAID[index].a_length = TAIDTAB[index].TAIDNAMELEN;
		memcpy( SupportedAID[index].aid, TAIDTAB[index].TAIDNAME,
				TAIDTAB[index].TAIDNAMELEN );
		SupportedAID[index].control = TAIDTAB[index].TAIDOPT1;
		SupportedAID[index].adt_id = TAIDTAB[index].TAIDADTID;
		index++;
	}

	status = EMVL2_FindSupportedApps( SupportedAID, &index, &pCandTextList );

	// Talk to the user about the result
	while ( status == EMVL2_NOT_SELECTED || status == EMVL2_AUTO_SELECT )
	{
		if ( status == EMVL2_AUTO_SELECT )
		{
			appl = 0;
		}
		else
		{
//@pl EMV test 			if (EMVAutoTest)
//@pl EMV test 			{
//@pl EMV test 				appl = GetAutoApp(); // Get application specified by the test card.
//@pl EMV test 			}
//@pl EMV test 			else
//@pl EMV test 			{
				appl = UserSelection( pCandTextList, index ); // Allow user to select application
//@pl EMV test 			}
		}

		if ( appl == 0xFF )		// no app accepted
		{
			status = EMVL2_NO_MORE_APPS;
		}
		else
		{

             GetMsg( SELECTING, CSTMSG );
			*ppCandidate = pCandTextList + appl;	//set pointer to selected application
			memcpy( CSTMSG2, ( *ppCandidate )->text,
					( UBYTE ) strlen( ( char * ) ( *ppCandidate )->text ) +
					1 );	
                        ShowInfoMsg( CustomMsg2, SmartCard );

//@pl             // write the first AID version entry into 9F09
//@pl             // as l2_select builds PDOL
//@pl             if (gCertFlag)
//@pl             {
                TempAIDVer[0] = TAIDTAB[appl].TAIDVER1[0];
                TempAIDVer[1] = TAIDTAB[appl].TAIDVER1[1];
	            EMVL2_WriteTagData( TempAIDVer, 2, TAG_TE, TAG_9F09 );
//@pl             }

			status = EMVL2_SelectApplication( appl, &index );

			// Check if app was blocked (top bit set)
			if (status & 0x80) 
			{
			    memcpy( CSTMSG, ( *ppCandidate )->text,
					   ( UBYTE ) strlen( ( char * ) ( *ppCandidate )->text ) + 1 );
                            GetMsg( BLOCKED_MSG, CSTMSG2 );
			    ShowInfoMsg( CustomMsg2, SmartCard );
				SDK_Wait(ONESECOND * 2);
				status &= 0x7F; // Mask top bit to reveal what else happened
			}

            if ( status == EMVL2_FAIL )
			{
               // Get the 12 bytes of debug data
		       EMVL2_GetDebugData( &debug );
			   
			   // Get the AID of the app we are trying to select
			   EMVL2_ReadTagData(tempRID, &length, TAG_9F06);

               // This is a disgusting frig which allows "MChip Lite" cards to fallback.
			   if (SC_CmdBytes[Cla] == 0x80 &&       // Class must be '80' 
				   SC_CmdBytes[Ins] == 0xA8 &&       // Instruction must be 'A8' ('80A8' = GetProcessingOptions)
				   *debug == 0x61 &&                 // SW1 must be '61'
				   memcmp(mcdRID, tempRID, 5) == 0)  // RID must be MasterCard 'A000000004'
			   {
				   status = EMVL2_MAG_STRIPE; // Fix status to allow this card to fallback
               }
               // ADVT test card 26 support
               else if (SC_CmdBytes[Cla] == 0x80 &&       // Class must be '80' 
               SC_CmdBytes[Ins] == 0xA8 &&                // Instruction must be 'A8' ('80A8' = GetProcessingOptions)
               *debug == 0x94 && *(debug+1) == 0x81 &&    // SW1 must be '94', SW2 '81'
               memcmp(vcdRID, tempRID, 5) == 0)           // RID must be Visa 'A000000003'
               {
                   status = EMVL2_MAG_STRIPE; // Fix status to allow this card to fallback
               }	
			}

			// display error if necessary
			if ( ( status == EMVL2_FAIL ) || ( status == EMVL2_BAD_FORMAT ) )
			{
				SDK_BeepIt( 2 );
				// if not selected, indicate error
//@pl EMV test 				if (!EMVAutoTest)
//@pl EMV test 				{
				    ShowErrMsg( CardErr );
//@pl EMV test                 }
			}
		}
	}
        
	// send the index back
	*( AIDindex ) = index;

	return ( status );
}























/*------------------------------------------------------------------
 *  Name:           BuildVerBuff
 *  Description:    
 *  Parameters:     buffptr
 *                  entry
 *  Return Value:   
 *  Notes:          Build a version buffer from the version nubers supported 
 *                  for the AID we want to use
 *-----------------------------------------------------------------*/
void BuildVerBuff( UBYTE * buffptr, struct taidrec_rec entry )
{
	/* First position in the buffer is how many version entries */
	*buffptr++ = 3;

	/* Now move the 3 2-byte supported version numbers */
	memcpy( buffptr, ( UBYTE * ) & entry.TAIDVER1, 2 );
	buffptr += 2;
	memcpy( buffptr, ( UBYTE * ) & entry.TAIDVER2, 2 );
	buffptr += 2;
	memcpy( buffptr, ( UBYTE * ) & entry.TAIDVER3, 2 );

	/* Return a pointer to the destination buffer */
	return;
}

/*-----------------------------------------------------------------------
 * Name:           ForceICCInsertion
 * Description:    Get the ICC inserted by the user
 *
 * Parameters:     None
 * Return Value:   Bool
 * Notes:          
 *-----------------------------------------------------------------------*/
Bool ForceICCInsertion( void )
{
	UBYTE ICCEntryStatus;
	//Bool ICCEntryFinished;
	UBYTE SCardStatus;
	Bool RetVal;





	// display chip card when asking for insertion
	GetMsg( CHIP_CARD, CSTMSG );
	GetMsg( PLEASE_INSERT_CARD, CSTMSG2 );
	GetMsg( CONTINUE_MSG, CSTMSG3 );
	YNEntry.TitleMid = CustomMsg;
	YNEntry.Line1Mid = CustomMsg2;
	YNEntry.Line2Mid = CustomMsg3;


	ICCEntryStatus = YesNoEntry( &YNEntry );

	if ( ICCEntryStatus == ENTER_KY )	// = yes
	{
		RetVal = False;
		while ( ( ICCEntryStatus == ENTER_KY ) &&	// as long as they keep saying yes
				( RetVal == False ) )
		{
			ScrdInserted( scrd_SCR, &SCardStatus );	// is card in?
			if ( OK == ScrdInserted( scrd_SCR, &SCardStatus ) )
			{
				if ( SCRD_INS_YES != SCardStatus )	// see if they still haven't put it in
				{
					ICCEntryStatus = YesNoEntry( &YNEntry );
				}
				else
				{
					RetVal = True;
				}

			}
			else				// the call to check reader went bad
			{
				ICCEntryStatus = CANCEL_KY;
			}
		}
	}

	if ( ENTER_KY == ICCEntryStatus )
	{
		RetVal = True ;
	}
	else
	{
		RetVal = False ;		// they chose not to continue
	}

	return RetVal;
}

/*-----------------------------------------------------------------------
 * Name:           isCardInserted
 * Description:    Get the ICC inserted by the user
 *
 * Parameters:     None
 * Return Value:   bool
 * Notes:          
 *-----------------------------------------------------------------------*/
Bool isCardInserted( void )
{
	UBYTE SCardStatus;
	Bool RetVal;

	RetVal = False; // initialise return value

    if ( OK == ScrdInserted( scrd_SCR, &SCardStatus ) )
	{
		if ( SCRD_INS_YES == SCardStatus )
		{
			RetVal = True;
		}
	}

	return RetVal;
}

/*------------------------------------------------------------------
 *  Name:           PerformCardClose
 *  Description:    Shut down the card ad prompt for removal
 *  Parameters:     none
 *  Return Value:   none
 *  Notes:          Uses CustomMsg for title bar
 *-----------------------------------------------------------------*/
void PerformCardClose( void )
{
	UBYTE status;
	UBYTE SCardStatus;

	// Eject the Smart Card - ICE5500 doesn't actually eject: force removal
    // ScrdEject also ends the session
	status = ScrdEject( scrd_SCR );
	status = !OK;

	while ( status != OK )
	{
		// if card removed (may have been actually ejected by 5000)
		if ( OK == ScrdInserted( scrd_SCR, &SCardStatus ) )
		{
			if ( SCRD_INS_YES != SCardStatus )
			{
				status = OK;
                ChipCardInserted = False; // global flag checked at txn start
                DelayIdleDisplay = False;
//@AAMELIN                DelayIdleDisplay = False;
				if ( PINSTAT & PinSmartCard_Read )
				{
					// if external pinpad clear screen
					// Send message to pinpad to display
					PinMessage ( Msg_ClrScrn );
				}
			}
			else
			{
//@TUM				ShowInfoMsg( PlsRemoveCard, CustomMsg );
				ShowInfoMsg( PlsRemoveCard, SmartCard );
			}
		}

		// Release Control
		OS_RelinqCPU(  );
	}
}


/*------------------------------------------------------------------
 *  Name:           InitUserCard
 *  Description:    
 *  Parameters:     none
 *  Return Value:   true if OK, else false
 *  Notes:          None.
 *-----------------------------------------------------------------*/
Bool InitUserCard( void )
{
	UBYTE SCBytes[33]; // BD no 33 is enough
	UWORD SCLength;
	UBYTE SCStatus;
	Bool EMV = 1;
	UBYTE HistBytesOffset;
	UBYTE HistLen;
	UBYTE status;
	Bool RetVal;
	UBYTE CardReadFails = 0;

	// fear the worst
	RetVal = False;

	do
	{
		status = ScrdGetATR( scrd_SCR, EMV, SCBytes,
							 &SCLength, &HistBytesOffset,
							 &HistLen, &SCStatus );

		if ( EMVL1_OK == status && ATR_GOOD == SCStatus )
		{
			RetVal = True;
		}
		else
		{
	 		// increment try counter
			CardReadFails++;
			// @Aydeniz BKM 01/12/2006 
			// ShowErrMsg( CardFail );

			memcpy( CSTMSG, "Card Fail", 10 );
			PerformCardClose(  );

			// prompt for card re-insertion
			 // @@A 1 denemeden sonra fallback                        
			if ( CardReadFails < 1 )
			{
				if ( !ForceICCInsertion( ) )
					break;	// no fallback if insertion declined
			}
			else
			{       
                          #ifdef MAKE_KIB
                                if ( TRINP.TRKEY == LOYALTY_INQ )
                                {
                                    TRINP.TRSTATUS[1] |= TS2_LOYALTYFBCK;
                                }
                                else
                                {
                          #endif
				// store original txn type (e.g. EMV_SALE)
				SecondEvent.TRNKEY = TRINP.TRORIGKEY = TRINP.TRKEY;

				// Get Transaction information
				FindTran ();

				// Set transaction name in TRTRANBUF
				memcpy( TRINP.TRTRANBUF, SecondEvent.TRNTEXT, sizeof( SecondEvent.TRNTEXT ) );

				// set nextevent = fallback, and change TRKEY
				CurrentEvent.NEXTEVENT = TRINP.TRKEY = EMV_FALLBACK;

				// allow natural return
                        #ifdef MAKE_KIB
                            }
                        #endif
			}
		}
		ScrdEndSession( scrd_SCR );
	}
	while ( ( RetVal == False ) && ( CardReadFails < 1 ) );

	return RetVal;
}

/*-----------------------------------------------------------------------
 * Name:            GoOnline
 * Description:     Online processing request
 * Parameters:      ICCEntry required for CompleteOnline call
 * Return Values:   EMVL2_ONLINE_FAILED, EMVL2_ONLINE_CANCELLED,
 *                  EMVL2_ONLINE_APPROVED, EMVL2_ONLINE_DECLINED
 * Notes:          
 *-----------------------------------------------------------------------*/
UBYTE GoOnline( struct iccrec_rec * pIccEntry, UBYTE *TACDef )
{
	UBYTE RetVal;
    UBYTE TempBuffer[40];
    UBYTE * pScriptEnd;
    UBYTE offset, ReadLen;
	enum online_desicion OnlineResult;
    Bool ScriptPerformed;
    UBYTE * debug;
    
    

	// Set to success initially
	RetVal = EMVL2_OK;
	OnlineResult = AUTHORISED_ONLINE;

	// Update the pfields post GenAC       
        if ( UpdFieldsPostGenAC(  ) != EMVL2_OK )
	{
		ShowErrMsg( UPDFIELDSPOSTGENAC_MSG );
		GetMsg( CardFail, CSTMSG );
                PerformCardClose(  );
		return ( EMVL2_FAIL );
	}

	// Perform a Pre-Dial if allowed (A4TVR_ARQC will trigger predial)
	PreDial ();

	// Set Issuer Script Length to zero before calling the host
	ScriptLen = 0;

	// Flag host
	FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;
	FlagHost ();
	RespDisplay();

	if (( TRINP.TRRSPOPT & R_APPROVED ) || ( TRINP.TRRSPOPT & R_REFERRAL ))
	{
        // note that voice referral treated as auth'd at this stage
		OnlineResult = AUTHORISED_ONLINE;
	}
    else // No host approval
	{
        if ( OnlineCancelled ) 
		{
			/* Transaction Cancelled by user */
            OnlineCancelled = False;
			RetVal = EMVL2_ONLINE_CANCELLED;
		}
		else if (
			 // Check for different 'Unable to go online' situations
			!memcmp( TRINP.TRRSPC, ( UBYTE * ) RSP_TO, S_TRRSPC ) ||	// Timeout
			!memcmp( TRINP.TRRSPC, ( UBYTE * ) RSP_CE, S_TRRSPC ) ||	// Comms error
			!memcmp( TRINP.TRRSPC, ( UBYTE * ) RSP_LC, S_TRRSPC ) ||	// Comms error
			!memcmp( TRINP.TRRSPC, ( UBYTE * ) RSP_ND, S_TRRSPC ) ||	// Comms error
			!memcmp( TRINP.TRRSPC, ( UBYTE * ) RSP_LB, S_TRRSPC ) ||	// Comms error
			!memcmp( TRINP.TRRSPC, ( UBYTE * ) RSP_NANSW, S_TRRSPC ) ||	// Comms error
			NullComp( ( char * ) TRINP.TRRSPC, S_TRRSPC )			// No response code
			)
		{
			OnlineResult = COMMS_FAIL;
		}
		else					// Host Declined
		{
			OnlineResult = DECLINED_ONLINE;
		}

	}
	
	// split out voice referrals if certification version
    if  ( TRINP.TRRSPOPT & R_REFERRAL )
    {
		// Have to handle voice referral
        RetVal = HandleCertHostReferral( pIccEntry, TACDef );
    }
    else if ( RetVal != EMVL2_ONLINE_CANCELLED )
    {
		// Complete online - indicate card busy
		ShowInfoMsg( ProcCard, SmartCard );
		EMVL2_WriteTagData( TRINP.TRRSPC, 0x02, TAG_TE, TAG_8A );	// Authorisation Response Code
		RetVal = EMVL2_CompleteOnlineTransaction( OnlineResult,
												  TACDef,
												  IssScriptBuf, ScriptLen );

        // Informational advice messages are an optional EMV feature and the EMVL2
        // library will indicate when the ICC has requested that such a message be created.
        // An informational advice should also be created if issuer script processing has
        // been performed and the library will also indicate when this has happened.
        // Add code here to handle the creation of such messages

        if (RetVal & BITMAP_RETVAL) // bitmapped card decision returned
        {
            if ( (RetVal & ADVICE_REQUESTED) || (FirstGenACAdviceRequired) )
            {
                // set application flag to indicate advice requested
                // currently being ignored as advices not supported
                // turn bit off
                RetVal ^= ADVICE_REQUESTED;
				SecondGenACAdviceRequired = True;
            }
            ScriptPerformed = False;
            if (RetVal & SCRIPT_PERFORMED)
            {
                // set flag to put Script Processing results in advice
                // currently being ignored as advices not supported
                // turn bit off
                RetVal ^= SCRIPT_PERFORMED;

                // populate the transaction structure
                EMVL2_ReadTagData(TempBuffer, &ReadLen, TAG_DF52);	// Issuer script processing results
                if (ReadLen != 0)
                {
	                pScriptEnd = TRINP.TRISSSCRIPTRES; 
                    for ( offset=0; offset < ReadLen; offset += 5)
                    {
                        // add presence indicator
                        *(pScriptEnd++) = 0xFF;
                        memcpy (pScriptEnd, &TempBuffer[offset], 5);
                        pScriptEnd += 5;
                    }

                    ScriptPerformed = True;
                    
                }

            }


            switch (RetVal) // should be left with bare result
            {
                case EMVL2_ACCEPT:
                    // Get final cryptogram into TRINP structure
                    UpdFinalCryptogram();

                    if (OnlineResult == COMMS_FAIL) // authorised offline post comms
                    {
						UpdFinalResponseCode();
                        EMV_Offline();
                    }
                    else
                    {                       
                        AddTxnToJournal();

#ifdef MAKE_INFADVONLINE 
						//@pl Try to send Informative ADVICE on Finally approved txn after talking to card
                        //@pl if it's not possible then Save Informative ADVICE to journal
                        // if ( TRINP.TRKEY != EMV_LOADPIN && TRINP.TRKEY != EMV_CHANGEPIN ) 
                            SendInfAdvOnline();
#else
						//@pl Informative ADVICE for online approved transactions
						//@pl only in case Script was processed or ICC require the Advice
                        if (ScriptPerformed  || SecondGenACAdviceRequired )
                        {
                                // add zero amount advice if script results
                            AddZeroAdvToJournal();
                        }
#endif
                    }
                    break;

                default: 
					// Get final cryptogram into TRINP structure
                    UpdFinalCryptogram();
					/* Get final response code from Tag Store into TRINP */
					UpdFinalResponseCode();

                    // we did scripts, so get the results online
                    if (ScriptPerformed  || SecondGenACAdviceRequired )
                    {
                        // add zero amount advice if script results
                        AddZeroAdvToJournal();
                    }

					// Process 'Card Declined'
					CertDeclinedByCard( );

					// check to see if forced after decline
					if ( ForcedAccept )
					{					 
						RetVal =  EMVL2_ACCEPT;
					}              
            }
        }
        else // serious failure happened on 2nd GenAC
        {
			// Process 'Card Declined'
			CertDeclinedByCard( );

//@TUM 13/11/2003 Europay/Mastercard requirement: If the card returns SW1,SW2 == '6985'
// in response to Generate AC or Ext. Authenticate, the terminal must allow the Fallback.
// The EMV library v.4.16a returns EMVL2_FAIL status in this case, the next release
// will return EMVL2_MAG_STRIPE. The patch below caters for both.
// Start of patch
			if ( RetVal == EMVL2_FAIL )
			{
				// Get pointer to 12 bytes of debug data
				EMVL2_GetDebugData( &debug );

				// This is a disgusting frig allowing cards returning 6985 to fallback.
				if ( *debug == 0x69 && *(debug+1) == 0x85 )  // SW12 must be '6985'
				{				                             
					// Change return status to Mag. Stripe
					RetVal = EMVL2_MAG_STRIPE;
				}
			}
// End of patch

        }
	}
    else // cancelled
    {
        // kill the transaction
        //if (TRINP.TRRSPOPT & R_APPROVED)
        TRINP.TRRSPOPT = 0;
        // blank the host message to avoid confusion
        memset (TRINP.TRRSPTXT, 0, S_TRRSPTXT);
    }

#ifdef MAKE_INFADVONLINE 
	    // Disconnect host here if Informative EMV Advice should be send just after online EMV transaction
		Hang_Up_Pin ();
#endif

	// return status of CompleteOnline

	return ( RetVal );
}



/*-----------------------------------------------------------------------
 * Name:            EMV_Offline
 * Description:     Do offline processing for EMV approved offlie txn
 * Parameters:      None
 * Return Values:   None
 * Notes:           Can't set txn time in here because this function is 
 *                  used for both auth'd offline and post comms fail 
 *                  offline and setting the time would invalidate the TC
 *-----------------------------------------------------------------------*/
void EMV_Offline( void )
{    

    // Set to APPROVED
    memcpy( TRINP.TRRSPC, RSP_APP, S_TRRSPC );
    TRINP.TRRSPOPT |= R_APPROVED;

    IncTraceNum( TRINP.TRSTAN );
    // Look up response in TRINP
    RspLookUp( &TRINP );

    if ( ForcedAccept )
    {
    	// indicate that this has been auth'd by force
		TRINP.TRPOCC = 0x16;
    }
    else
    {
    	// indicate that this has been auth'd offline 
		TRINP.TRPOCC = 0x06;
    }

    // Generate AUTH Code (2 or 6 digits) on TRINP structure
        GenAuth_TRINP( );

	// Set Additional Response Data to Offline
	TRINP.TRRSPA[0] = '0';
	TRINP.TRRSPA[1] = '6';
	// Set Advice Bit
	TRINP.TRSTATUS[0] |= TS1_ADV;

	GetMsg( AUTHORISED_MSG, (char *)&(TRINP.TRHOSTTEXT[0]) );
	GetMsg( AUTHORISED_MSG, (char *)TRINP.TRRSPTXT );

        // Lock the journal semaphore
	JournalLock ();
	// Must set up globals pJrnTransBuf and JrnRevFlag
	TRINP.TRSTATUS[0] &= ~TS1_REV;		
	TRINP.TRJRNIDENT = JournalNew (&TRINP);
	JournalRelease( );
}





/*-----------------------------------------------------------------------
 * Name:            GetPreviousAmount
 * Description:     Find the 'Previous Amount' for the Terminal Risk management
 * Parameters:      UBYTE *Amount - pointer to destination,
 *					TRINP.TRPAN & TRINP.TRPANSEQ are filled for the current txn.
 * Return Values:   None
 * Notes:			This function is supposed to scan the journal
 *					for the most recent transaction with the same PAN
 *					and PAN Sequence No. If found, the amount will be copied
 *					to destination.
 *-----------------------------------------------------------------------*/
void GetPreviousAmount( UBYTE *Amount )
{
	Bool Found = False;

	/* Lock the journal */
	JournalLock ();

	/* Set up selection record */
 	/* Set up default selection data in common record JSREC	*/
 	SetDefaultSelection ();

   /* Status Bits off already set to TS1_VOID + TS1_REV + TS1_SETL */	   

   /* Status Bits on: look for Stored Txns */
	JSREC.JSSTATON = TS1_ADV;

	/* Select this Acquirer */
	JSREC.JSAQID = TRINP.TRAQID;

	/* Select this Issuer (to narrow down the search,
	   we're interesting in the txns with same PAN only) */
	JSREC.JSISID = TRINP.TRISID;

	/* Find the last Offline Repeat in the Journal */
	if ( HostJSFirst(  ) )
	{
		/* 'Fast Forward' to find the last (most recent) Offline Repeat for this acquirer/issuer */
        while( HostJSNext( ) );

		/* JrnHostIdent points to the most recent txn */

		/* Loop from the end to the start (see HostJSPrev()) */
		do
		{
			/* Get the transaction record into TRREQ */
			HostJRead( );

			/* Check if PAN is the same */
			if ( 0 == memcmp( TRINP.TRPAN, TRREQ.TRPAN, S_TRPAN ) )
			{
				/* PANs are equal, check PAN Sequence numbers (optional fields) */
				if ( TRINP.TRPANSEQ && TRREQ.TRPANSEQ )
				{
					/* PAN Seq. No. present, compare */
					if ( TRINP.TRPANSEQ == TRREQ.TRPANSEQ )
					{
						Found = True;
					}
				}
				else
				{
					/* PAN Seq. No. not present, ignore them */
					Found = True;
				}
			}
			if ( Found )
			{
				/* Move Previous Amount to destination */
				memcpy( Amount, TRREQ.TRTOTAM, S_TRTOTAM );

				/* Exit the loop */
				break;
			}
		} while ( HostJSPrev(  ) );
	}

	/* Release the Journal */
	JournalRelease ();
}


/*-----------------------------------------------------------------------
 * Name:            TransactionVoid
 * Description:     Setup Response Code for 'TRANSACTION VOID'
 *					
 * Parameters:      None
 * Return Values:   None
 * Notes:           
 *-----------------------------------------------------------------------*/
void TransactionVoid( void )
{
	memcpy( TRINP.TRRSPC, RSP_XC, S_TRRSPC );
}


// FIXME: to be implemented

//-----------------------------------------------------------------------------
//  Name:           MerchantForceAccept
//
//  Purpose:        See if merchant thinks customer is suspicious
//
//  Parameters:     None
//
//  Global Inputs:  None
//
//  Returns:        Boolean, Dodgy or not
//
//  Notes:          
//
//-----------------------------------------------------------------------------
Bool MerchantForceAccept( UBYTE CardDecision )
{
    UBYTE keyval;

    if (!(TCONF.TCAPPCAP[1] & APP_CAP2_EMV_FORCED)) 
            return False;
	
	switch (CardDecision)
    {	
    	case EMVL2_REFERRAL: 
    		GetMsg( CARDREFERRAL_MSG, CSTMSG );
            break;
         case EMVL2_DECLINE:
         default:
         	GetMsg( CARD_DECLINE, CSTMSG );
	}
	
	
    GetMsg( OVERRIDE_MSG, CSTMSG2 );
    YNEntry.TitleMid = CustomMsg;
    YNEntry.Line1Mid = CustomMsg2;

    YNEntry.Line2Mid = N_NullStr;

    // Prompt for YES or NO Keys 
    keyval = YesNoEntry( &YNEntry );

    if ( ENTER_KY == keyval )   // If YES Key
    {
        return True;
    }
    return False;
}

/*-----------------------------------------------------------------------
 * Name:            IsTrack2EquivDataConsistent
 * Description:     Check Track 2 Equiv. content against PAN/Expiry tags
 *					
 * Parameters:      None
 * Return Values:   True - Ok, False - error (inconsistency detected).
 * Notes:           TRINP.TRTRK2, TRINP.TRPAN & TRINP.TRAPPEXP populated with
 *					the content of Tags '57', '5A' & '5F24' correspondingly.
 *					Although TRINP.TRAPPEXP is 3 bytes long, we only use
 *					the first 2 bytes for comparison (due to Trk2 Equiv. format).
 *-----------------------------------------------------------------------*/
Bool IsTrack2EquivDataConsistent( void )
{
	Bool retval = True;		// set default return value to OK
	UBYTE i;
	static UBYTE TempPAN[S_TRPAN];
	static UBYTE TempEXPD[2];
	static UBYTE *pTrk2;

	// First, check if the Track 2 Equiv. Data tag is present

	/* Check if Tag '57' is absent - in this case we have TRINP.TRTRK2 filled with 0xFF */
	for ( i = 0; i < S_TRTRK2; i++ )
	{
		if ( 0xFF != TRINP.TRTRK2[i] )
		{
			break;
		}
	}

	if ( i < S_TRTRK2 )
	{
		// Tag '57' is present (not all 0xFF's)

		/* What we're going to do here is retrieve the PAN and Expiry Date
		   from Track2 Equiv Data into a temporary storage and then compare them
		   to TRINP.TRPAN and first 2 bytes of TRINP.TRAPPEXP
	    */

		memset( TempPAN, 0xFF, S_TRPAN );
		memset( TempEXPD, 0x00, 2 );

		i = 0;

		// Track 2 data
		pTrk2 = TRINP.TRTRK2;

		while ( (*pTrk2 != 0x3d) && (*pTrk2 != 0xFF) && (i < S_TRPAN) )
		{
			TempPAN[i] = *pTrk2++;
			TempPAN[i] <<= 4;
			if ( *pTrk2 != 0x3d )
			{
				TempPAN[i] |= *pTrk2++ & 0x0f;
			}
			else
			{
				TempPAN[i] |= 0x0f;
				break;
			}

			i++;
		}

		if ( *pTrk2 == 0x3d )
		{
			pTrk2++;		// Skip the delimiter
			TempEXPD[0] = *pTrk2++;
			TempEXPD[0] <<= 4;
			TempEXPD[0] |= *pTrk2++ & 0x0f;
			TempEXPD[1] = *pTrk2++;
			TempEXPD[1] <<= 4;
			TempEXPD[1] |= *pTrk2++ & 0x0f;
		}

		if ( (0 != memcmp( TRINP.TRPAN, TempPAN, S_TRPAN )) || ((0 != memcmp( TRINP.TRAPPEXP, TempEXPD, 2 )) && (0 != TempEXPD[1])) )
		{
			/* Not equal, return error */
			retval = False;
		}
	}	// if ( i < S_TRTRK2 )

	return( retval );
}

/*-----------------------------------------------------------------------
 * Name:            HandleCertHostReferral
 * Description:     Handle where host returns a referral response
 *                  and we're getting auth code before 2nd GenAC
 * Parameters:      None
 * Return Values:   None
 * Notes:          
 *-----------------------------------------------------------------------*/
static UBYTE HandleCertHostReferral( struct iccrec_rec * pIccEntry, UBYTE *TACDef )
{
    UBYTE RetVal;
    UBYTE TempBuffer[40];
    UBYTE Length, offset;
    UBYTE * pScriptEnd;
    
    // @@Aydeniz - 18.07.2006 PROVUS istegi 
    // PrintReferralCardDetails();

    // delay while host message is displayed
    // Wait for 3 second 
    SDK_Wait( TENMS * 300 );

    // Get AUTH code from user
    // @@Aydeniz - 18.07.2006 PROVUS istegi
    // if (0 == get_auth())
    if ( 1 )
    {  
    	// @ydeniz -- BKM {
		//GetMsg( REFERRALDECLINE_MSG, (char *)&(TRINP.TRHOSTTEXT[0]) );
		//GetMsg( REFERRALDECLINE_MSG, (char *)TRINP.TRRSPTXT );
		  // }

        // ask card for AAC 
		ShowInfoMsg( ProcCard, SmartCard );
		EMVL2_WriteTagData( TRINP.TRRSPC, 0x02, TAG_TE, TAG_8A );	// Authorisation Response Code
		RetVal = EMVL2_CompleteOnlineTransaction( DECLINED_ONLINE,
												  TACDef,
												  IssScriptBuf, ScriptLen );

        if (RetVal & BITMAP_RETVAL) // bitmapped card decision returned
        {
            if (RetVal & ADVICE_REQUESTED)
            {
                // set application flag to indicate advice requested
                // currently being ignored as advices not supported
                // turn bit off
                RetVal ^= ADVICE_REQUESTED;
				SecondGenACAdviceRequired = True;
            }

            if (RetVal & SCRIPT_PERFORMED)
            {

                // Put Script Processing results in advice
                EMVL2_ReadTagData(TempBuffer, &Length, TAG_DF52);	// Issuer script processing results
                if (Length != 0)
                {
	                pScriptEnd = TRINP.TRISSSCRIPTRES; 
                    for ( offset=0; offset < Length; offset += 5)
                    {
                        // add presence indicator
                        *(pScriptEnd++) = 0xFF;
                        memcpy (pScriptEnd, &TempBuffer[offset], 5);
                        pScriptEnd += 5;
                    }
                    
                }

                AddZeroAdvToJournal();

                // currently being ignored as advices not supported
                // turn bit off
                RetVal ^= SCRIPT_PERFORMED;
            }
         }

    }
    else
    {
        // ask card for TC

        // approve the transaction
        // this line removed as result of certification failure at RFI 19/01/04
        // It does not seem necessary. TRINP will be written to the journal and will
        // have both R_REFERRAL and R_APPROVED bits set. In reality neither will make 
        // a difference in terms of certification
	    // MoveIt( TRINP.TRRSPC, RSP_APP, S_TRRSPC );
	    TRINP.TRRSPOPT = R_APPROVED;
        TRINP.TRRSPOPT |= R_REFERRAL;

        // get TC from card
		ShowInfoMsg( ProcCard, SmartCard );
		EMVL2_WriteTagData( TRINP.TRRSPC, 0x02, TAG_TE, TAG_8A );	// Authorisation Response Code
		RetVal = EMVL2_CompleteOnlineTransaction( AUTHORISED_ONLINE,
												  TACDef,
												  IssScriptBuf, ScriptLen );

        if (RetVal & BITMAP_RETVAL) // bitmapped card decision returned
        {
            if (RetVal & ADVICE_REQUESTED)
            {
                // We are storing an advice in any case - turn bit off
                // set application flag to indicate advice requested
                // currently being ignored as advices not supported
                // turn bit off
                RetVal ^= ADVICE_REQUESTED;
				SecondGenACAdviceRequired = True;
            }

            if (RetVal & SCRIPT_PERFORMED)
            {
                // Put Script Processing results in advice

                EMVL2_ReadTagData(TempBuffer, &Length, TAG_DF52);	// Issuer script processing results
                if (Length != 0)
                {
	                pScriptEnd = TRINP.TRISSSCRIPTRES; 
                    for ( offset=0; offset < Length; offset += 5)
                    {
                        // add presence indicator
                        *(pScriptEnd++) = 0xFF;
                        memcpy (pScriptEnd, &TempBuffer[offset], 5);
                        pScriptEnd += 5;
                    }
                    
                }

                // turn bit off
                RetVal ^= SCRIPT_PERFORMED;
            }
         }

        // Set Advice Bit
	    TRINP.TRSTATUS[0] |= TS1_ADV;
        
        // if ( TRINP.TRKEY != EMV_LOADPIN && TRINP.TRKEY != EMV_CHANGEPIN ) 
            AddTxnToJournal();

        RetVal = EMVL2_OK; // don't care if not TC - auth centre authorised already
    }

    // set up message and shut down card

    //PerformCardClose(); card is closed in main stream

    return (RetVal);
}



/*-----------------------------------------------------------------------
 * Name:            AddTxnToJournal
 * Description:     Finally approve txn after talking to card
 * Parameters:      None
 * Return Values:   None
 * Notes:           If issuer scripts add advice also to the journal for zero amount
 *-----------------------------------------------------------------------*/
static void AddTxnToJournal( void )
{
	// Lock the journal semaphore
	JournalLock ();

	// Must set up globals pJrnTransBuf and JrnRevFlag	
        TRINP.TRSTATUS[0] &= ~TS1_REV;	
	JournalNew(&TRINP);

    // Release the journal semaphore
	JournalRelease ();

    return;
}


#ifdef MAKE_INFADVONLINE 
/*-----------------------------------------------------------------------
 * Name:            SendInfAdvOnline
 * Description:     Try to send Informative ADVICE
 *					if it's not possible then Save Informative ADVICE to journal
 * Parameters:      None
 * Return Values:   None
 * Notes:           None	   
 *-----------------------------------------------------------------------*/
void SendInfAdvOnline( void )
{
	// Convert original EMV transaction information to Informative EMV Advice 
	Trn2InfAdv();

	// Wait for completion of original online transaction (probably an advice is still being sent)
	while ( ( TRREQBLD & BIT_0_SET ) )
	{
		SDK_RelinqCPU(  );
	}

	// Flag host - perform a Pre-Dial if allowed 
        OnlineCancelled = False;
	PreDial();
	FlagHostCntl = FLAG_ALLOW_CANCEL;
	// try to send Informative ADVICE online
	FlagHost(  );

	// is Informative ADVICE sent successfully?
	if ( TRINP.TRRSPOPT & R_APPROVED )
	{
		// Yes; 

		// Restore original EMV transaction information from Informative EMV Advice 
		InfAdv2Trn();
	}
	else
	{
		// No; 

		// Restore original EMV transaction information from Informative EMV Advice 
		InfAdv2Trn();

		// Save Informative ADVICE to journal
    	AddZeroAdvToJournal();
	}

    return;
}


//-----------------------------------------------------------------------------
//  get_auth        Get authorization code and save in TRINP.TRAUTH
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        UWORD   length of the input.
//
//  Notes:
//
//-----------------------------------------------------------------------------

static UWORD get_auth( void )
{
	UWORD retval = 0;
    UBYTE YNRetVal;

	ENTRY_STR AuthCodeEntry = {
		N_NullStr,				// Prompt
		N_NullStr,				// Default Value
		N_NullStr,				// Not used
	};

    // blank host receipt lines
    memset (TRINP.TRHOSTTEXT, 0, S_TRHOSTTEXT);
    memset (TRINP.TRRSPTXT, 0, S_TRRSPTXT);

    // Put text into YNEntry struct
	YNEntry.TitleMid = N_NullStr;
	YNEntry.Line1Mid = N_NullStr;
	YNEntry.Line2Mid = CustomMsg;
	
	
//@pl     memcpy (CSTMSG, "AUTH CODE OBTAINED ?", 21);
	GetMsg( ISAUTHCODEOBTAINED_MSG, CSTMSG );

    YNRetVal = 0; // timed out value

    while (	YNRetVal == 0 ) // until decision made - timeout redisplays
	{
	    YNRetVal = YesNoEntry( &YNEntry );
    }

    if (YNRetVal == ENTER_KY) // yes
    {
	    DefaultTitle[0] = 0;

        memset (TRINP.TRAUTH, 0, S_TRAUTH);

        AuthCodeEntry.TitleMid = N_EnterAuth;
	    retval = AlphaNumEntry( 1, S_TRAUTH, 2, &AuthCodeEntry );
	    if ( retval )
        {
	        retval = MoveTillZero( ( char * ) TRINP.TRAUTH, Dspbuf, S_TRAUTH );
        }
    }
    else // no
    {
        retval = 0;
    }

    if (retval != 0)
    {
//@pl         memcpy (&TRINP.TRHOSTTEXT[0], "REFERRAL", 9);
//@pl         memcpy (TRINP.TRRSPTXT, "REFERRAL", 9);
		GetMsg( REFERRAL_MSG, (char *)&(TRINP.TRHOSTTEXT[0]) );
		GetMsg( REFERRAL_MSG, (char *)TRINP.TRRSPTXT );
    }

	return ( retval );
}


#endif


/*-----------------------------------------------------------------------
 * Name:            CertDeclinedByCard
 * Description:     Handle the situation when txn approved by Host but then
 *					declined by card (or card error encountered).
 * Parameters:      None
 * Return Values:   None
 * Notes:           The EMV Lib. does not change the Response Code in the Tag Store,
 *					so we have to set our own 'declined' code into TRINP.
 * 					to show the correct message and print it on the receipt.
 *-----------------------------------------------------------------------*/
static void CertDeclinedByCard( void )
{
	// Update fields after 2nd Generate AC
	UpdFieldsPostGenAC();

	// we need to approve this and get it into the journal, as well as sort out the host
	// host referral -> doesn't come this way 
	// host comms fail -> comes this way only if card declines
	// host declined -> definitely in here - store advice, txn pair exists for first request
	// host approved and card declined -> definitely in here - reverse and store offline txn

	// Set 'Card Declined' response code 'DC'
	// memcpy( TRINP.TRRSPC, RSP_DC, S_TRRSPC );

    // kill the transaction
    if (TRINP.TRRSPOPT & R_APPROVED)
    {
        // if ( TRINP.TRKEY != EMV_LOADPIN || TRINP.TRKEY != EMV_CHANGEPIN )
        {
        // mark this for reversal since host approved the txn
            TRINP.TRSTATUS[0] |= TS1_REV;
            AddRevTxnToJournal();            
        }
        // DISapprove the transaction now as result of 2GenAC
        TRINP.TRRSPOPT ^= R_APPROVED;
    }

	// Look up actual Responce Code
	RspLookUp( &TRINP );
	// substitute actual Responce Code to prevent approval receipt printing for finaly declined transactions
    if (TRINP.TRRSPOPT & R_APPROVED)
    {
		// Set 'Card Declined' response code 'DC'
		memcpy( TRINP.TRRSPC, RSP_DC, S_TRRSPC );

		// Look up Response for a Transaction Data structure
		RspLookUp( &TRINP );
    }

    // blank the host message to avoid confusion
    memset (TRINP.TRRSPTXT, 0, S_TRRSPTXT);

	// Get rid of any host messages
	memset( TRINP.TRHOSTTEXT,  0, S_TRHOSTTEXT );
	memset( HSTPRDATA, 0, S_HSTPRDATA );

	// Handle merchant override (force scenario 2CM.081)
	if ( MerchantForceAccept( EMVL2_DECLINE) )
	{
        // EMV_FORCED transaction disabled - ASH 30-03-05
        // TRINP.TRORIGKEY = EMV_FORCED; // make sure no auth generated
        ForcedAccept = True;
        EMV_Offline();
	}
	else
	{
		// we are declining, but may still require an advice
#ifdef NOT_MAKE_DEBUG
        SecondGenACAdviceRequired = True;
#endif
		if ( ( FirstGenACAdviceRequired ) || ( SecondGenACAdviceRequired ) )
		{
			AddZeroAdvToJournal();
		}
	}

}
/*-----------------------------------------------------------------------
 * Name:            AddZeroAdvToJournal
 * Description:     Finally approve txn after talking to card
 * Parameters:      None
 * Return Values:   None
 * Notes:           If issuer scripts add advice also to the journal for zero amount
 *-----------------------------------------------------------------------*/
void AddZeroAdvToJournal( void )
{
	// Convert original EMV transaction information to Informative EMV Advice 
	Trn2InfAdv();

	// Add transaction to journal
    AddTxnToJournal();

	// Restore original EMV transaction information from Informative EMV Advice 
	InfAdv2Trn();

    return;
}

/*-----------------------------------------------------------------------
 * Name:            Trn2InfAdv
 * Description:     Convert original EMV transaction information  
 *					to Informative EMV Advice
 * Parameters:      None
 * Return Values:   None
 * Notes:           None
 *-----------------------------------------------------------------------*/
void Trn2InfAdv( void )
{
//@pl     memcpy (TempAmount, TRINP.TRTOTAM, S_TRTOTAM);
//@pl     memcpy (TempAmount2, TRINP.TRCASHBK, S_TRBASE);

    // store transaction data
	memcpy( &TempTrandata, &TRINP, TRSZE );

//@pl     // zero the amount temporarily
    memset (TRINP.TRTOTAM, 0, S_TRTOTAM);
    memset (TRINP.TRCASHBK, 0, S_TRBASE);

    // set advice bit
    TRINP.TRSTATUS[0] |= TS1_ADV;
    //@pl set 'INFORMATIVE EMV ADVICE' bit
    TRINP.TROPTEMV |= TROPTEMV_INFADV;

    return;
}

/*-----------------------------------------------------------------------
 * Name:            InfAdv2Trn
 * Description:     Repare original EMV transaction information  
 *					from Informative EMV Advice
 * Parameters:      None
 * Return Values:   None
 * Notes:           None
 *-----------------------------------------------------------------------*/
void InfAdv2Trn( void )
{
//@pl     // restore the amounts and turn off advice flag
//@pl     memcpy (TRINP.TRTOTAM, TempAmount, S_TRTOTAM);
//@pl     memcpy (TRINP.TRCASHBK, TempAmount2, S_TRBASE);

    // restore transaction data
	memcpy( &TRINP, &TempTrandata, TRSZE );

    TRINP.TRSTATUS[0] &= ~TS1_ADV;
    TRINP.TROPTEMV &= ~TROPTEMV_INFADV;

    return;
}

/*-----------------------------------------------------------------------
 * Name:            AddRevTxnToJournal
 * Description:     Finally approve txn after talking to card
 * Parameters:      None
 * Return Values:   None
 * Notes:          
 *-----------------------------------------------------------------------*/
static void AddRevTxnToJournal( void )
{   	
  JournalLock();
  TRINP.TRSTATUS[0] |= TS1_REV;					
  JournalNew(&TRINP);

// Save the journal identifier to use in functions like LEVEL II
  TRINP.TRJRNIDENT = JournalNew(&TRINP);;
  JournalRelease ();

  return;
}


/*-----------------------------------------------------------------------
 * Name:            HandleCertOfflineReferral
 * Description:     Handle where card  returns an AAR referral response
 * Parameters:      None
 * Return Values:   None
 * Notes:          
 *-----------------------------------------------------------------------*/
 UBYTE HandleCertOfflineReferral( struct iccrec_rec * pIccEntry, UBYTE *TACDef )
{

    UBYTE RetVal;

	//PrintReferralCardDetails();

    // Prompt for auth centre call and wait for acknowledgement
	ShowInfoMsg( CallAuthCentre, CallAuthCentre );
	GetKeyPress();

    // Get AUTH code from user
    if (0 == get_auth())
    {

        // ask card for AAC 
		ShowInfoMsg( ProcCard, SmartCard );
		// set up declined offline Auth Response Code 
		// Tag_8A is actually set up inside lib
		memcpy( TRINP.TRRSPC, "Z2", S_TRRSPC);		

		// ask card for AAC	, script len hardcoded to 0 for safety
		ShowInfoMsg( ProcCard, SmartCard );

		RetVal = EMVL2_CompleteOnlineTransaction( AAR_DECLINE,
												  TACDef,
												  IssScriptBuf, 0 );
        // store the final CID & cryptogram
        UpdFinalCryptogram();

        // transaction declined - inform user
//@pl         memcpy (&TRINP.TRHOSTTEXT[0], "REFERRAL DECLINE", 17);
//@pl         memcpy (TRINP.TRRSPTXT, "REFERRAL DECLINE", 17);
		GetMsg( REFERRALDECLINE_MSG, (char *)&(TRINP.TRHOSTTEXT[0]) );
		GetMsg( REFERRALDECLINE_MSG, (char *)TRINP.TRRSPTXT );

        if (RetVal & BITMAP_RETVAL) // bitmapped card decision returned
        {
            if ( (RetVal & ADVICE_REQUESTED) || (FirstGenACAdviceRequired) )
            {
                // We are storing an advice in any case - turn bit off
                RetVal ^= ADVICE_REQUESTED;
				IncTraceNum( TRINP.TRSTAN );
				AddZeroAdvToJournal();
            }

			// no script, of course
        }

    }
    else
    {
        // ask card for TC

        // approve the transaction
        // this line removed as result of certification failure at RFI 19/01/04
        // It does not seem necessary. TRINP will be written to the journal and will
        // have both R_REFERRAL and R_APPROVED bits set. In reality neither will make 
        // a difference in terms of certification
	    // MoveIt( TRINP.TRRSPC, RSP_APP, S_TRRSPC );
	    TRINP.TRRSPOPT = R_APPROVED;
        TRINP.TRRSPOPT |= R_REFERRAL;

		memcpy( TRINP.TRRSPC, "Y2", S_TRRSPC);		

        // get TC from card
		ShowInfoMsg( ProcCard, SmartCard );

		RetVal = EMVL2_CompleteOnlineTransaction( AAR_ACCEPT,
												  TACDef,
												  IssScriptBuf, 0 );

        // store the final CID & cryptogram
        UpdFinalCryptogram();

        if (RetVal & BITMAP_RETVAL) // bitmapped card decision returned
        {
            if (RetVal & ADVICE_REQUESTED)
            {
                // We are storing an advice in any case - turn bit off
                RetVal ^= ADVICE_REQUESTED;
            }

			// no script, of course
         }

        IncTraceNum(TRINP.TRSTAN);

        // Set Advice Bit
	    TRINP.TRSTATUS[0] |= TS1_ADV;
            // @A - BC DK DK 
            // pJrnTransBuf = &TRINP;
        
        // if ( TRINP.TRKEY != EMV_LOADPIN && TRINP.TRKEY != EMV_CHANGEPIN ) 
            AddTxnToJournal();

        RetVal = EMVL2_OK; // don't care if not TC - auth centre authorised already
    }

    // set up message and shut down card

    //PerformCardClose(); card is closed in main stream

    return (RetVal);
}

/*-----------------------------------------------------------------------
 * Name:            GetKeyPress
 * Description:     Get user acknowledgement for message
 *					
 * Parameters:      None
 * Return Values:   None
 * Notes:           
 *-----------------------------------------------------------------------*/
void GetKeyPress ( void )
{

	UBYTE retval;

	// repeat until EXIT button pressed
    do
	{
		retval = GetOneKey( DEV_KBD | DEV_PAD, 0); // user timeout 0

	} while (retval != ENTER_KY);
}



#endif	// MAKE_EMV

