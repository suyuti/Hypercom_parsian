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

/* --- Global variables  ---- */
//! Flag of PIN retry
Bool isPINReEntry;

//=============================================================================
// Private function declarations
//=============================================================================
UBYTE PerformCVMProcessing( void );
UBYTE PerformAppSelection( CandTextType **, UBYTE * );

Bool ForceICCInsertion( void );
Bool isCardInserted( void );
Bool InitUserCard( void );

void BuildVerBuff( UBYTE * buffptr, struct taidrec_rec entry );
UBYTE GoOnline( struct iccrec_rec *pIccEntry );
static void HandleReferral( void );
static void DeclinedByCard ( void );
Bool IsTrack2EquivDataConsistent( void );

/* --- Local variables  ---- */
SupportedAIDType *pSupportedAID;
struct taidrec_rec *pTAIDEntry;
static UWORD pinlen;

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

static const SupportedAIDType SupportedAIDTraining = {
    0x05,
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

//-----------------------------------------------------------------------------
//! \brief
//!     User entry for Application selection
//!
//! \param
//!		[in]pCandList		Pointer to candidate list
//!		[in]ListLen			Number of candidates in list
//!
//! \return
//!     Original index as per selected AID or 0xFF if cancelled
//!
//! \note
//!     The code table from candidate list is displayed in brackets
//!     Will be modified not to use YNEntry on next pass to show "YES/NEXT"
//!     for multi application candidate list
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! \brief
//!     Does CVM processing on behalf of transaction
//!
//! \return
//!     CVM status (the final word on CVM processing outcome):<br>
//!     EMVL2_CVM_DONE          process completed,<br>
//!     EMVL2_CVM_SIG           signature required,<br>
//!     EMVL2_CVM_PIN_FAIL_SIG  pin failed, sig required,<br>
//!     EMVL2_CVM_PIN_FAIL      pin failed,<br>
//!     EMVL2_CVM_BLOCKED_SIG   pin blocked, sig required,<br>
//!     EMVL2_CVM_BLOCKED       pin blocked,<br>
//!     EMVL2_CVM_ONP           send PIN online - not passed back at moment,<br>
//!     EMVL2_FAIL              process critically failed
//!
//! \note
//!     Functionality isolated for use in various transaction types
//-----------------------------------------------------------------------------
UBYTE PerformCVMProcessing( void )
{
	UBYTE CVMstatus;
	UBYTE PINStatus;
	UBYTE CVFinished;
	UBYTE PINdata[8];
	UBYTE length;

	// initialise global Suppress signature flag
	SuppressSigLine = True;

	// Initialise CVM processing flags
	PINStatus = EMVL2_PIN_ENTERED;	// value not used on first entry to CVM
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
#ifndef	MAKE_EMV416		// NB: if not defined
    PINdata[0] = 0x00; // null entry condition
    PINdata[1] = 0x00; // null entry condition
    PINdata[2] = 0x00; // null entry condition
    PINdata[3] = 0x00; // null entry condition
    PINdata[4] = 0x00; // null entry method
    PINdata[5] = 0x00; // null entry method
    PINdata[6] = 0x00; // null entry method
    PINdata[7] = 0x00; // null entry method
#endif	// MAKE_EMV416

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
				if ( isPINReEntry )
				{
					if ( 1 == PINStatus )
					{
						GetMsg( LAST_TRY_LEFT, CSTMSG );
					}
					else
					{
                        GetMsg( TRIES_LEFT, CSTMSG );
                        // Get the lenght of the Cashier/Server prompt
                        length = StrLn( CSTMSG, sizeof( CSTMSG ) );
                        // convert pin try count to decimal
                        CSTMSG[length] = ( PINStatus > 10 ) ? 0x31 : 0x20;
                        CSTMSG[length+1] = 0x30 + PINStatus % 10;
                        CSTMSG[length+2] = 0x00;
					}

					ShowErrMsg( CustomMsg );
				}
//@TUM 08/09/2003 Patch for Natwest:
				else
				{
					/* This is not PIN Retry, get the PIN Try Counter
				   into a global variable */
					if ( !EMVL2_ReadTagData( &TRINP.TRPINBL[1], &length ,TAG_9F17) )
					{
						/* Tag not found */
						TRINP.TRPINBL[1] = 0;
					}
				}	// if ( isPINReEntry )

				// Clear 'Cancel' flag
				OnlineCancelled = False;

				// need this if we wish to have External Pinpad/Smart Card Reader
				if( !EMV_PINVerifyOffline( CVMstatus, TRINP.TRPINBL[1], PINdata, &PINStatus ) )
				{
                    // Pinpad failure was detected
                    CVMstatus = EMVL2_FAIL;
                    // Set CVM finished flag to True
                    CVFinished = True;
                    break;
				}
				if ( PINStatus == EMVL2_ONLINE_CANCELLED )
				{	// Transaction was cancelled by user during PIN entry
					CVMstatus = PINStatus;
					// Set CVM finished flag to True
					CVFinished = True;
				}
				else
				{
					isPINReEntry = True;
					ShowInfoMsg(ProcCard, SmartCard);	// 'PROCESSING CARD'
				}

				if (PINSTAT & PinSmartCard_Read)
				{
					// if external pinpad send a "Processing Card" message
					// Send message to pinpad to display
					PinMessage ( Msg_ProcCard );
				}

                // Clear out PIN Block storage to prevent
				// it's sending to host
				memset( TRINP.TRPINBL, 0, S_TRPINBL );
				break;			// going back in with PIN

			case EMVL2_FAIL:
				// oh dear - get out
				// display critical failure
				GetMsg( CARD_FAIL, CSTMSG );
				ShowErrMsg( CustomMsg );
				CVFinished = True;
				break;

			case EMVL2_CVM_SIG: // Signature required
				// carry on - get signature
				SuppressSigLine = False;
				CVFinished = True;
				break;

			case EMVL2_CVM_ONE:  // Online Enciphered PIN required
                if(EMV_PINVerifyOnline( &PINStatus ))
                {
                    if( PINStatus == EMVL2_ONLINE_CANCELLED )
                    {
                        CVMstatus = PINStatus;
                        // Set CVM finished flag to True
                        CVFinished = True;
                    }
                }
                else
                {
                    // Pinpad failure was detected
                    CVMstatus = EMVL2_FAIL;
                    // Set CVM finished flag to True
                    CVFinished = True;
                    break;
                }
				// have to go back in with PIN status
				break;

			case EMVL2_CVM_PIN_FAIL_SIG:
				// carry on - get signature
				SuppressSigLine = False;

				// get sig - warning will be issued in case below
				CVMstatus = EMVL2_CVM_SIG;

				// no break intentional

			case EMVL2_CVM_PIN_FAIL:
                // like done, except in a bad way
				// display PIN tries exceeded message and continue
				GetMsg( PIN_TRIES_EXCEEDED, CSTMSG );
				ShowErrMsg( CustomMsg );
                CVFinished = True;
				break;

			case EMVL2_CVM_PIN_BLOCKED_SIG:
				// carry on - get signature
				SuppressSigLine = False;

				// get sig - warning will be issued in case below
				CVMstatus = EMVL2_CVM_SIG;

				// no break intentional

			case EMVL2_CVM_PIN_BLOCKED:
				// like done, except in a bad way
				// display PIN tries exceeded message and continue
				GetMsg( PIN_BLOCKED, CSTMSG );
				ShowErrMsg( CustomMsg );
				CVFinished = True;
				break;

			case EMVL2_CVM_DONE:
				// carry on - we're finished
				CVFinished = True;
				break;
		}
	}

	if (SuppressSigLine)
	{
		/* Set PIN Block ready flag */
		TRINP.TRSTATUS[1] |= TS2_PINBLOCK;
	}

	return ( CVMstatus );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Does Application selection on behalf of transaction
//!
//! \param
//!		[in]ppCandidate		Pointer to candidate list
//!		[in]AIDindex		Pointer to AID (not used)
//!
//! \return
//!     Status of operation (the final word on application selection outcome)
//!     and could be:<br>
//!     EMVL2_FAIL,<br>
//!     EMVL2_SELECTED,<br>
//!     EMVL2_EASY_ENTRY,<br>
//!     EMVL2_MAG_STRIPE,<br>
//!     EMVL2_NO_MORE_APPS
//!
//! \note
//!     1. Functionality isolated for use in various transaction types<br>
//!     2. Early return on failure to write terminal data to store
//-----------------------------------------------------------------------------
UBYTE PerformAppSelection( CandTextType ** ppCandidate, UBYTE * AIDindex )
{
	UBYTE status;				// return value
	UBYTE appl;					// application
	UBYTE index = 0;			// search variable
    UBYTE TempAIDVer[2];
//@TUM 25/07/2003 The 4 lines below are added for "MChip Lite" issue:
	UBYTE	length;
	UBYTE	tempRID[16];
	UBYTE	mcdRID[] = {0xA0,0x00,0x00,0x00,0x04};
	UBYTE	vcdRID[] = {0xA0,0x00,0x00,0x00,0x03};
	UBYTE	*debug;
	SupportedAIDType SupportedAID[TAIDMAX];
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

	// Build terminal supported application list 
//@TUM TAIDNUM is BCD, so the TAIDNUM == 0x10 used to be excluded
//	while ( TAIDTAB[index].TAIDNUM <= TAIDMAX && TAIDTAB[index].TAIDNUM )
	if ( TCONF.TCOPT4 & TC4_TRAINING )
	{
		/* TRAINING MODE */
		/* Move 'special' AID into the list */
		SupportedAID[0] = SupportedAIDTraining;

		/* This is the only supported application */
		index = 1;
	}
	else
	{
		pTAIDEntry = TAIDTAB;
		pSupportedAID = SupportedAID;

	    while ( (index < TAIDMAX) && (0 != pTAIDEntry->TAIDNUM) )
		{
			pSupportedAID->a_length = pTAIDEntry->TAIDNAMELEN;
			memcpy( pSupportedAID->aid, pTAIDEntry->TAIDNAME, pTAIDEntry->TAIDNAMELEN );
			pSupportedAID->control = pTAIDEntry->TAIDOPT1;
			pSupportedAID->adt_id = pTAIDEntry->TAIDADTID;
			pSupportedAID++;
			pTAIDEntry++;
			index++;
		}

	}

	/* The '&index' here is actually the address of the total number of apps,
	   which is changed by EMVL2_FindSupportedApps() into actual index */
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
			appl = UserSelection( pCandTextList, index );
		}

		if ( appl == 0xFF )		// no app accepted
		{
			status = EMVL2_NO_MORE_APPS;
		}
		else
		{
			// we've got it - give the user confirmation
			GetMsg( SELECTING, CSTMSG );
			*ppCandidate = pCandTextList + appl;	//set pointer to selected application
			memcpy( CSTMSG2, ( *ppCandidate )->text,
					( UBYTE ) strlen( ( char * ) ( *ppCandidate )->text ) +
					1 );
			ShowInfoMsg( CustomMsg2, SmartCard );

            // write the first AID version entry into 9F09
            // as l2_select builds PDOL
                        TempAIDVer[0] = TAIDTAB[appl].TAIDVER1[0];
			TempAIDVer[1] = TAIDTAB[appl].TAIDVER1[1];
			EMVL2_WriteTagData( TempAIDVer, 2, TAG_TE, TAG_9F09 );

			status = EMVL2_SelectApplication( appl, &index );

			// Check if app was blocked (top bit set)
			if (status & 0x80) 
			{
			    memcpy( CSTMSG, ( *ppCandidate )->text,
					   ( UBYTE ) strlen( ( char * ) ( *ppCandidate )->text ) + 1 );
                            GetMsg( BLOCKED_MSG, CSTMSG2 );
			    ShowInfoMsg( CustomMsg2, SmartCard );
                            OS_Wait(ONESECOND * 2);
                            status &= 0x7F; // Mask top bit to reveal what else happened
			}
//@TUM 25/07/2003 The code below is addressing the "MChip Lite" card issue:

            if ( status == EMVL2_FAIL )
			{
               // Get the pointer to the 12 bytes of debug data
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

//@TUM End of "MChip Lite" related code.

			// display error if necessary
			if ( ( status == EMVL2_FAIL ) || ( status == EMVL2_BAD_FORMAT ) )
			{
				OS_Beepit( 2 );
				// if not selected, indicate error
				ShowErrMsg( CardErr );
			}
		}
	}

	// send the index back
	*( AIDindex ) = index;

	return ( status );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Builds a version info from the given Application entry
//!
//! \param
//!		[in/out]buffptr		Pointer to version info destination buffer
//!		[in]entry			Application entry
//!
//! \note
//!     Build a version buffer from the version nubers supported for the
//!     AID we want to use
//-----------------------------------------------------------------------------
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
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets the ICC inserted by the user
//!
//! \return
//!		True if card has been inserted else False
//-----------------------------------------------------------------------------
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
		RetVal = True;
	}
	else
	{
		RetVal = False;		// they chose not to continue
	}

	return RetVal;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets the status of ICC insertion
//!
//! \return
//!		True if card is inserted else False
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! \brief
//!     Shuts down the card and prompts for removal
//!
//! \note
//!		Uses CustomMsg for title bar
//-----------------------------------------------------------------------------
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
//@AAMELIN                DelayIdleDisplay = False;
				if ( PINSTAT & PinSmartCard_Read )
				{
					// if external pinpad send a "Ready" message
					// Send message to pinpad to display
					PinMessage ( Msg_PinReady );
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


//-----------------------------------------------------------------------------
//! \brief
//!     Init User Card insertion
//!
//! Init User Card insertion dialog. The function allows 3 attempts.
//!
//! \return
//!     True if OK else False
//-----------------------------------------------------------------------------
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
							 &SCLength, &HistBytesOffset, &HistLen, &SCStatus );

		if ( EMVL1_OK == status && ATR_GOOD == SCStatus )
		{
			RetVal = True;
		}
		else
		{
	 		// increment try counter
			CardReadFails++;
			ShowErrMsg( CardFail );
			PerformCardClose();

			// prompt for card re-insertion
			if ( CardReadFails < 3 )
			{
				if ( !ForceICCInsertion() )
					break;	// no fallback if insertion declined
			}
			else
			{
				// store original txn type (e.g. EMV_SALE)
				SecondEvent.TRNKEY = TRINP.TRORIGKEY = TRINP.TRKEY;

				// Get Transaction information
				FindTran();

				// Set transaction name in TRTRANBUF
				memcpy( TRINP.TRTRANBUF, SecondEvent.TRNTEXT, sizeof( SecondEvent.TRNTEXT ) );

				// set nextevent = fallback, and change TRKEY
				CurrentEvent.NEXTEVENT = TRINP.TRKEY = EMV_FALLBACK;

				// allow natural return
			}
		}
		ScrdEndSession( scrd_SCR );
	}
	while ( ( RetVal == False ) && ( CardReadFails < 3 ) );

	return RetVal;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Online processing request
//!
//! \param
//!		[in]pIccEntry		Pointer to ICCEntry required for CompleteOnline call
//!
//! \return
//!     EMVL2_ONLINE_FAILED, EMVL2_ONLINE_CANCELLED, EMVL2_ONLINE_APPROVED or
//!     EMVL2_ONLINE_DECLINED
//-----------------------------------------------------------------------------
UBYTE GoOnline( struct iccrec_rec * pIccEntry )
{
	UBYTE RetVal;
	enum online_desicion OnlineResult;
//    UBYTE TempBuffer[40];
    UBYTE * debug;
//    UBYTE offset, ReadLen;
///    Bool ScriptPerformed;

	// Set to success initially
	RetVal = EMVL2_OK;
	OnlineResult = AUTHORISED_ONLINE;

	// Update the fields post GenAC 
///	UpdFieldsPostGenAC(  );
//    if ( EMVRetVal != EMVL2_OK )
//	{
//		ShowErrMsg( CardFail );
// 		// catastrophic failure - we're out of here  
//        PerformCardClose(  );
//		return ( EMVL2_FAIL );
//	}

	// Perform a Pre-Dial if allowed (A4TVR_ARQC will trigger predial)
	PreDial ();

	// Set Issuer Script Length to zero before calling the host
	ScriptLen = 0;

	// Flag host
	FlagHostCntl = FLAG_PREPRINT | FLAG_ALLOW_CANCEL;
	FlagHost ();

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

    if ( RetVal != EMVL2_ONLINE_CANCELLED )
	{
		// Complete online - indicate card busy
		ShowInfoMsg( ProcCard, SmartCard );
        
        // Moved out into host module to be able to write the untranslated response code
		EMVL2_WriteTagData( TRINP.TRRSPC, S_TRRSPC, TAG_TE, TAG_8A );	// Authorisation Response Code
		RetVal = EMVL2_CompleteOnlineTransaction( OnlineResult,
												  pIccEntry->ICCTACDEF,
												  IssScriptBuf, ScriptLen );

        // Informational advice messages are an optional EMV feature and the EMVL2
        // library will indicate when the ICC has requested that such a message be created.
        // An informational advice should also be created if issuer script processing has
        // been performed and the library will also indicate when this has happened.
        // Add code here to handle the creation of such messages

        if (RetVal & BITMAP_RETVAL) // bitmapped card decision returned
        {
            if (RetVal & ADVICE_REQUESTED)
            {
                // set application flag to indicate advice requested
                // currently being ignored as advices not supported
                // turn bit off
                RetVal ^= ADVICE_REQUESTED;
            }

            if (RetVal & SCRIPT_PERFORMED)
            {
                // set flag to put Script Processing results in advice
                // currently being ignored as advices not supported
                // turn bit off
                RetVal ^= SCRIPT_PERFORMED;

            }	// if (RetVal & SCRIPT_PERFORMED)

            switch (RetVal) // should be left with bare result
            {
                case EMVL2_ACCEPT:
                    // Get final cryptogram into TRINP structure
                    UpdFinalCryptogram();

					// Get rid of any text (set up by Apacs host module)
// we may want to see actual host response; rather than just 'APPROVED'
//					memset( TRINP.TRHOSTTEXT, 0, S_TRHOSTTEXT );
//					memset( HSTPRDATA, 0, S_HSTPRDATA );

                    if ( COMMS_FAIL == OnlineResult ) // authorised offline after 'unable to go online'
                    {
						// Check if 'Enabled Post-comms for EMV' flag is set
						if ( IS4_ICCPOSTCOMMS & (TRINP.TRISPTR->ISOPT4) )
						{
							// Generate Voice Referral
   	                        HandleReferral( );
						}
						else
						{
							// Authorise the transaction offline after comms failure
							// Get final Response Code into TRINP structure
							UpdFinalResponseCode();
       		                EMV_Offline();
						}
                    }	// if ( COMMS_FAIL == OnlineResult )
                    else
                    {
                        // check if Host Referral requested
                        if ( TRINP.TRRSPOPT & R_REFERRAL )
                        {
							// handle host referral
                            HandleReferral( );
                        }
                        else
                        {
							// Approved Online
							/* Replace the Response Message for Refund type transaction */
							if ( CurrentEvent.TRNOPT1 & T1_CREDIT )
							{
								GetMsg( RefundTran, (char *)&TRINP.TRHOSTTEXT[0] );	// 1st line 'REFUND'
								GetMsg( Accepted, (char *)&TRINP.TRHOSTTEXT[20] );	// 2nd line 'ACCEPTED'
							}

                            // Add transaction to the journal
                            JournalNew ( &TRINP );

                        }	// if (TRINP.TRRSPOPT & R_REFERRAL)
                    }	// if ( COMMS_FAIL == OnlineResult )

                    break;

                default:	// EMVL2_DECLINE

					/* Get final response code from Tag Store into TRINP */
					UpdFinalResponseCode();

					if ( COMMS_FAIL_FORCEDECLINE != OnlineResult )
					{
						/* Process 'Card Declined' */
						DeclinedByCard( );
					}
					else
					{
						/* 'STORE FULL' after comms failure or 'Security Error' */
						// Update fields after 2nd Generate AC
						UpdFieldsPostGenAC();
					}
            }	// switch (RetVal)
        }	// if (RetVal & BITMAP_RETVAL)
        else // serious failure happened on 2nd GenAC
        {
		    // Process 'Card Declined'
		    DeclinedByCard( );

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

        }	// if (RetVal & BITMAP_RETVAL)
	}	// if ( RetVal != EMVL2_ONLINE_CANCELLED )
    else // cancelled
    {
        // Transaction cancelled by User
		TransactionVoid();
    }	// if ( RetVal != EMVL2_ONLINE_CANCELLED )

	// return status of CompleteOnline
	return ( RetVal );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Does offline processing for EMV approved offline transaction
//!
//! \return
//!     None.
//!
//! \note
//!		Can't set transaction time in here because this function is used for
//!     both auth'd offline and post comms fail offline and setting the time
//!     would invalidate the TC
//-----------------------------------------------------------------------------
void EMV_Offline( void )
{
    UBYTE ReadLen;

    // Set to APPROVED
	memcpy( TRINP.TRRSPC, RSP_APP, S_TRRSPC );
	TRINP.TRRSPOPT |= R_APPROVED;

    // Look up response in TRINP
	RspLookUp( &TRINP );

	// Set Advice Bit
	TRINP.TRSTATUS[0] |= TS1_ADV;

	// Replace the Response Message for Refund type transaction
	// and do not generate an Auth Code
	if ( CurrentEvent.TRNOPT1 & T1_CREDIT )
	{
		GetMsg( RefundTran, (char *)&TRREQ.TRHOSTTEXT[0] );	// 1st line 'REFUND'
		GetMsg( Accepted, (char *)&TRREQ.TRHOSTTEXT[20] );	// 2nd line 'ACCEPTED'
	}
/*
	else if ( EMV_COMPLETION == TRINP.TRKEY )
	{
		// Do not generate Auth Code for Completion
		GetMsg( N_Completion, (char *)&TRREQ.TRHOSTTEXT[0] );	// 1st line 'COMPLETION'
		GetMsg( Accepted, (char *)&TRREQ.TRHOSTTEXT[20] );		// 2nd line 'ACCEPTED'
	}
*/
	else
	{
		// Are we in Training Mode?
		if ( TCONF.TCOPT4 & TC4_TRAINING )
		{
			// Yes; fill the Auth Number with X's
			memset( TRINP.TRAUTH, 'X', S_TRAUTH );
		}
		else
		{
		    // generate auth code in TRINP
			GenAuth( &TRINP );
		}
	}

	// store offline auth code
	EMVL2_ReadTagData( TRINP.TREMVRSPC, &ReadLen, TAG_8A );

	// Set Additional Response Data..
	if ( !(TRINP.TRTVR[3] & 0x80) && !memcmp(TRINP.TREMVRSPC, "Y1", S_TREMVRSPC ) )
	{
		// to Floor
		TRINP.TRRSPA[0] = '0';
		TRINP.TRRSPA[1] = '3';
	}
	else
	{
		// to Offline
		TRINP.TRRSPA[0] = '0';
		TRINP.TRRSPA[1] = '5';
	}

	// Increment 'STAN' and move System Trace Number to 'TRINP'
	IncTraceNum( TRINP.TRSTAN );

    //Add transaction to the journal
    JournalNew ( &TRINP );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Finds the 'Previous Amount' for the Terminal Risk management
//!
//! \param
//!		[in]Amount			Pointer to Destination Buffer
//!
//! \return
//!     None
//!
//! \note
//!     This function is supposed to scan the journal for the most recent
//!     transaction with the same PAN and PAN Sequence No. If found, the amount
//!     will be copied to destination.
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! \brief
//!     Handles voice referral
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void HandleReferral( void )
{
    // set up message for remove card
    GetMsg( N_Referral, CSTMSG );
    PerformCardClose();

    Referral ( &TRINP );

	/* If approved - store the txn and update totals */
	if ( TRINP.TRRSPOPT & R_APPROVED )
	{
		// Approved, don't print or display any messages from response
		memset( TRINP.TRHOSTTEXT, 0, S_TRHOSTTEXT );
		memset( HSTPRDATA, 0, S_HSTPRDATA );
    
        // Add transaction to the journal
        JournalNew ( &TRINP );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Handles the situation when transaction approved by Host but then
//!     declined by card (or card error encountered)
//!
//! \return
//!     None
//!
//! \note
//!     The EMV Library does not change the Response Code in the Tag Store,
//!     so we have to set our own 'declined' code into TRINP to show the
//!     correct message and print it on the receipt.
//-----------------------------------------------------------------------------
static void DeclinedByCard( void )
{
	// Update fields after 2nd Generate AC
	UpdFieldsPostGenAC();

	// Set 'Card Declined' response code 'DC'
	memcpy( TRINP.TRRSPC, RSP_DC, S_TRRSPC );

	// Get rid of any host messages
	memset( TRINP.TRHOSTTEXT,  0, S_TRHOSTTEXT );
	memset( HSTPRDATA, 0, S_HSTPRDATA );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Setup Response Code for 'TRANSACTION VOID'
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void TransactionVoid( void )
{
	memcpy( TRINP.TRRSPC, RSP_XC, S_TRRSPC );
}


// FIXME: to be implemented

//-----------------------------------------------------------------------------
//! \brief
//!     Tests if merchant thinks customer is suspicious
//!
//! \return
//!     True if user has been accepted the card otherwise False
//-----------------------------------------------------------------------------
Bool MerchantForceAccept( void )
{
    UBYTE keyval;

    if (!(LocalAppCap[1] & APP_CAP2_EMV_FORCED)) 
            return False;

    GetMsg( CARD_DECLINE, CSTMSG );
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

//-----------------------------------------------------------------------------
//! \brief
//!     Checks Track 2 Equiv. content against PAN/Expiry tags
//!
//! \return
//!     False if error (inconsistency detected) else True.
//!
//! \note
//!     TRINP.TRTRK2, TRINP.TRPAN and TRINP.TRAPPEXP populated with the content
//!     of Tags '57', '5A' & '5F24' correspondingly. Although TRINP.TRAPPEXP is
//!     3 bytes long, we only use the first 2 bytes for comparison (due to Trk2
//!     Equiv. format).
//-----------------------------------------------------------------------------
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

#endif	// MAKE_EMV

