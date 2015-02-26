
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             reqvisad.c
//      Visa D Request Building Routines.
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "sdkos.h"
#include "sdk.h"
#include "sdkmem.h"
#include "utllib.h"
#include "fixdata.h"

#include "defines.h"
#include "struct.h"
#include "transdef.h"
#include "appdata.h"
#include "comm.h"
#include "host.h"
#include "hstmsg.h"
#include "hstresp.h"
#include "findtran.h"
#include "tables.h"
#include "isdebit.h"


//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================

#define ATC_SALE	     "54"	// PURCHASE
#define ATC_MO		     "56"	// MAIL ORDER
#define ATC_CARDV	     "58"	// CARD AUTHENTICATION
#define ATC_DEBIT	     "93"	// DEBIT PURCHASE
#define ATC_DEBIT_REF    "94"	// DEBIT REFUND
#define ATC_DEBIT_BI     "9A"	// DEBIT BALANCE INQUIRY
#define ATC_REFUND	     "CR"	// CREDIT REFUND (OFFLINE)
#define V2_DEVICE        "00"	// Device Code.
#define DBC_FORMAT      "V0."	// 3 VISA II FIELDS: RECORD FORMAT, APPLICATION TYPE AND DELIMITER.
#define V2_FORMAT       "D0."	// 3 VISA II FIELDS: RECORD FORMAT, APPLICATION TYPE AND DELIMITER.
#define V2_DEBIT_FMT    "T0."	// 3 VISA II FIELDS AS NOTED IN ABOVE COMMENT.
#define V2_LANGUAGE_IND  "00"	// '00' = ENGLISH
#define V2_GROUP_III_001 "001!010"	// GROUP III VERSION 001 - COMMERCIAL CARD REQUEST INDICATOR
#define V2_GROUP_III_007 "007"	// GROUP III VERSION 007 - CVV2
#define V2_DEVICE_RETL   "DR"	// DEVICE & INDUSTRY CODES - DIAL TERMINAL RETAIL
#define V2_DEVICE_REST   "DF"	// DEVICE & INDUSTRY CODES - DIAL TERM RESTAURANT
#define V2_HEAD        "K1.Z"	// THIS FIELD CONTAINS 4 VISA II
#define V2_RTYPE      " @@@@"	// FIELDS: RECORD FORMAT, APPLICATION TYPE,
									// MSG DELIMITER, & X.25 ROUTING ID 
// Sizes //RECORD TYPE 
#define S_RCOUNT      	   3	// Record Counter size
#define S_ATC_SALE	       2	// PURCHASE
#define S_ATC_MO	       2	// MAIL ORDER
#define S_ATC_CARDV	       2	// CARD AUTHENTICATION
#define S_ATC_DEBIT	       2	// DEBIT PURCHASE
#define S_ATC_DEBIT_REF    2	// DEBIT REFUND
#define S_ATC_DEBIT_BI     2	// DEBIT BALANCE INQUIRY
#define S_ATC_REFUND	   2	// CREDIT REFUND (OFFLINE)
#define S_V2_DEVICE        2	// Device Code.
#define S_DBC_FORMAT       3	// 3 VISA II FIELDS: RECORD FORMAT, APPLICATION TYPE AND DELIMITER.
#define S_V2_FORMAT        3	// 3 VISA II FIELDS: RECORD FORMAT, APPLICATION TYPE AND DELIMITER.
#define S_V2_DEBIT_FMT     3	// 3 VISA II FIELDS AS NOTED IN ABOVE COMMENT.
#define S_V2_LANGUAGE_IND  2	// '00' = ENGLISH
#define S_V2_GROUP_III_001 7	// GROUP III VERSION 001 - COMMERCIAL CARD REQUEST INDICATOR
#define S_V2_GROUP_III_007 3	// GROUP III VERSION 007 - CVV2
#define S_V2_DEVICE_RETL   2	// DEVICE & INDUSTRY CODES - DIAL TERMINAL RETAIL
#define S_V2_DEVICE_REST   2	// DEVICE & INDUSTRY CODES - DIAL TERM RESTAURANT
#define S_V2_HEAD          4	// THIS FIELD CONTAINS 4 VISA II
#define S_V2_RTYPE         5	// FIELDS: RECORD FORMAT, APPLICATION TYPE,


//=============================================================================
// Private function declarations
//=============================================================================

static void MoveATC( void );
static void MoveCIDC( void );
static void MoveREQ_SOURCE( void );
static void MoveREQ_CUST( void );
static void MoveREQ_CIDDATA( void );
static void MoveAMT( void );
static void MoveSECAMT( void );
static void MoveSHAREGROUP( void );
static void MoveDEBIT_ABA( void );
static void MoveSETTLEAGENT( void );
static void MoveBATCHNO( void );
static void MoveREIM( void );
static void Req_Sale( void );
static void Req_Confirm( void );
static void Req_SetHead( void );
static void Req_SetParm( void );
static void Req_SetDetail( void );
static void Req_SetTrail( void );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static struct batch_rec *pBatchRec;
static struct v2_rec *pVisa2Ptr;	// Pointer to V2TAB entry  
static UBYTE *pSendBuff;		// Pointer into send buffer 
static UBYTE *pSendBuffSave;	// Second Pointer into send buffer 
static UBYTE HashTotal[S_TRTOTAM];	// HASHING TOTAL 
static UBYTE CashBackTotal[S_TRTOTAM];	// CASH BACK TOTAL 
static UBYTE NetTotal[S_TRTOTAM];	// NET DEPOSIT 
static UBYTE RCOUNT[S_RCOUNT];	// RECORD COUNTER 


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  ReqVisaD        Format VisaD request packet.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        OS_RETURN_VALS  orvINVDATA  if Request Packet not built.
//                                  otherwise   Comm Status of Request send
//
//  Notes:          MsgComFlag is updated.
//
//-----------------------------------------------------------------------------

extern OS_RETURN_VALS ReqVisaD( void )
{
	OS_RETURN_VALS CommStatus;	// Comm Status of Request send  
	char buildstatus;

	// Reset Comm status 
	CommStatus = orvINVDATA;

	// Reset Request Packet built okay flag 
	MsgComFlag &= ~REQUEST_BUILT;

	// Yes; clear out Transmit Buffer 
	memset( SENDBUF, 0,sizeof( SENDBUF ) );

	// Set up the data pointer 
	pSendBuff = SENDBUF;

	if ( (pVisa2Ptr = FindVisa2( TRREQ.TRAQID )) )
	{
		buildstatus = 0;

		switch ( TRREQ.TRKEY )
		{
			case SALE:
			case OPENTAB:
			case MAIL_ORDER:
			case AUTH:
				// Move in Group I data 
				// Move the Visa D format "D0." in the Header 
				memcpy( pSendBuff, V2_FORMAT, S_V2_FORMAT );

				// Update the data pointer 
				pSendBuff += S_V2_FORMAT;

				Req_Sale(  );
				buildstatus = 1;
				break;

			case DEBIT:
			case DBCASH:
			case SALCASH:
			case REFDEBIT:
				// Move in Group I data 
				// Move the Visa D format "T0."in the Header 
				memcpy( pSendBuff, V2_DEBIT_FMT, S_V2_DEBIT_FMT );

				// Update the data pointer 
				pSendBuff += S_V2_DEBIT_FMT;

				Req_Sale(  );
				buildstatus = 1;
				break;

			case DEBITCONF:
				Req_Confirm(  );
				buildstatus = 1;
				break;

			case CLBATCH:
			case CLBATCHA:
				Req_SetHead(  );
				buildstatus = 1;
				break;

			case CLTERM:
				Req_SetParm(  );
				buildstatus = 1;
				break;

			case RUPLOAD:
				Req_SetDetail(  );
				buildstatus = 1;
				break;

			case CLBATCHE:
				Req_SetTrail(  );
				buildstatus = 1;
				break;

			default:
				break;
		}

		if ( buildstatus )
		{
			// Okay; indicate that the Request was built okay 
			MsgComFlag |= REQUEST_BUILT;

			// Calculate the length 
			SendMsgLen = pSendBuff - SENDBUF;

			// Send the Message Request to the Host 
			SendMessage();

			// Get the Comm status 
			CommStatus = HostOSRetval;
		}
	}

	// Return the Comm status 
	return ( CommStatus );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  Req_Sale        Build a visanet, base ii authorization message for
//                  sale and authorization only transactions.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void Req_Sale( void )
{
	//  PUT ACQUIRER BANK# IN SEND BUFFER 
	memcpy( pSendBuff, pVisa2Ptr->V2BNUM, ( UWORD ) 6 );

	// Update the data pointer 
	pSendBuff += 6;

	// PUT VISA II MERCHANT#, STORE# AND TERMINAL# IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2MERCH, S_V2MERCH + S_V2STORE + S_V2TERM );

	// Update the data pointer 
	pSendBuff += S_V2MERCH + S_V2STORE + S_V2TERM;

	// Yes; see if Restaurant Mode Selected 
	if ( 0 )
	{
		

	}
	else
	{
		// No Restaurant mode so Retail 
		// Move DEVICE CODE & INDUSTRY CODE 
		memcpy( pSendBuff, V2_DEVICE_RETL, S_V2_DEVICE_RETL );

		// Update the data pointer 
		pSendBuff += S_V2_DEVICE_RETL;
	}

	// PUT MERCHANT CURRENCY CODE IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2CURRENCY, S_V2CURRENCY );

	// Update the data pointer 
	pSendBuff += S_V2CURRENCY;

	// PUT MERCHANT COUNTRY CODE IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2COUNTRY, S_V2COUNTRY );

	// Update the data pointer 
	pSendBuff += S_V2COUNTRY;

	// PUT MERCHANT CITY CODE IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2CITY, S_V2CITY );

	// Update the data pointer 
	pSendBuff += S_V2CITY;

	// PUT LAST 4 DIGITS OF ZIP CODE IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2EXTZIP, S_V2EXTZIP );

	// Update the data pointer 
	pSendBuff += S_V2EXTZIP;

	// PUT Language Indicator IN SEND BUFFER. 
	memcpy( pSendBuff, V2_LANGUAGE_IND, S_V2_LANGUAGE_IND );

	// Update the data pointer 
	pSendBuff += S_V2_LANGUAGE_IND;

	// PUT Time Zone Differential IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2TIMEZONE, S_V2TIMEZONE );

	// Update the data pointer 
	pSendBuff += S_V2TIMEZONE;

	// PUT Merchant Category Code IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2CATEG, S_V2CATEG );

	// Update the data pointer 
	pSendBuff += S_V2CATEG;

	// PUT if DEVICE IS CPS CAPABLE IN SEND BUFFER. requested ACI 
	*( pSendBuff ) = 'Y';

	// Update the data pointer 
	pSendBuff++;

	// Move the SEQNUM "Transaction sequence number" 
	BfAscii( ( char * ) pSendBuff, &TRREQ.TRSTAN[1], ( UBYTE ) 2 );

	// Update the data pointer 
	pSendBuff += 2 * 2;

	// AUTHORIZATION TRANSACTION CODE 
	MoveATC(  );

	// CARDHOLDER IDENTIFICATION CODE 
	MoveCIDC(  );

	// ACCOUNT DATA SOURCE CODE 
	MoveREQ_SOURCE(  );

	MoveREQ_CUST(  );

	// Move IFS Update the data pointer 
	*pSendBuff = IFS;
	pSendBuff++;

	MoveREQ_CIDDATA(  );

	// Move IFS Update the data pointer 
	*pSendBuff = IFS;
	pSendBuff++;

	// Move IFS Update the data pointer 
	*pSendBuff = IFS;
	pSendBuff++;

	MoveAMT(  );

	// Move IFS Update the data pointer 
	*pSendBuff = IFS;
	pSendBuff++;

	MoveSECAMT(  );

	// Move IFS Update the data pointer 
	*pSendBuff = IFS;
	pSendBuff++;

	// Move IFS Update the data pointer 
	*pSendBuff = IFS;
	pSendBuff++;

	// Moves merchant name, plus city and state, Update the data pointer 
	memcpy( pSendBuff, pVisa2Ptr->V2MERCHNAME, S_V2MERCHNAME + S_V2CITYST );
	pSendBuff += S_V2MERCHNAME + S_V2CITYST;

	// Move IFS Update the data pointer 
	*pSendBuff = IFS;
	pSendBuff++;

	// Move IFS Update the data pointer 
	*pSendBuff = IFS;
	pSendBuff++;

	// Move IFS Update the data pointer 
	*pSendBuff = IFS;
	pSendBuff++;

	// check if this is a debit transaction. 
	if ( IsDebitTrans( TRREQ.TRKEY ) )
	{
		// Yes, Move in Group II Data 
		// Move IFS Update the data pointer 
		*pSendBuff = IFS;
		pSendBuff++;

		// Move the VISA II SHARING GROUP data into send buff 
		MoveSHAREGROUP(  );

		// Move IFS Update the data pointer 
		*pSendBuff = IFS;
		pSendBuff++;

		MoveDEBIT_ABA(  );

		MoveSETTLEAGENT(  );

		// Move IFS Update the data pointer 
		*pSendBuff = IFS;
		pSendBuff++;

		// Moves agent number and chain number, Update the data pointer 
		memcpy( pSendBuff, ( pVisa2Ptr->V2BNUM ) + 6, ( UWORD ) ( 6 + 6 ) );
		pSendBuff += 6 + 6;

		// Move the Batch Number 
		MoveBATCHNO(  );

		// Reimbursement Attribute 
		MoveREIM(  );

		// Move IFS Update the data pointer 
		*pSendBuff = IFS;
		pSendBuff++;
	}
	else
	{
		// No, Move in Group II Data 
		// Move in Group III Version 001 - Commercial Card Request Indicator 
		memcpy( pSendBuff, V2_GROUP_III_001, S_V2_GROUP_III_001 );
		pSendBuff += S_V2_GROUP_III_001;

		// Move in Group III Version 007 - CVV2 
                /*
		// if ( TRREQ.TRCVV2IND > 0x20 )
		{
			*pSendBuff = 0x1D;
			pSendBuff++;

			// Move in Group III Version 007 
			memcpy( pSendBuff, V2_GROUP_III_007, S_V2_GROUP_III_007 );
			pSendBuff += S_V2_GROUP_III_007;

			// Move in CVV2 Presence Indicator 
			*pSendBuff = TRREQ.TRCVV2IND;
			pSendBuff++;

			// See if PAN manually entry and CVV2 turned on 
			if ( ( TRINP.TRPOSE[1] & BIT_4_SET ) &&
				 ( TRINP.TRISPTR->ISOPT5 & IS5_CVV2 ) )
			{
				// Move in CVV2 Response Type 
				*pSendBuff = '1';
			}
			else
			{
				// Move in CVV2 Response Type 
				*pSendBuff = '0';
			}

			pSendBuff++;

			memcpy( pSendBuff, TRREQ.TRCVV2, S_TRCVV2 );

			// Update the data pointer 
			pSendBuff += S_TRCVV2;
		}
                */
	}
}


//-----------------------------------------------------------------------------
//  MoveATC         Move Authorization Transaction Code into send Buffer.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void MoveATC( void )
{
	// TRANSACTION KEY HOLDER 
	UBYTE TRKEY_HOLD;

	if ( RUPLOAD == TRREQ.TRKEY )
	{
		TRKEY_HOLD = TRREQ.TRORIGKEY;
	}
	else
	{
		TRKEY_HOLD = TRREQ.TRKEY;
	}

	switch ( TRKEY_HOLD )
	{
		case MAIL_ORDER:
			// MAIL ORDER 
			memcpy( pSendBuff, ATC_MO, S_ATC_MO );
			break;

		case DEBIT:
		case SALCASH:
		case DBCASH:
			// PURCHASE 
			memcpy( pSendBuff, ATC_DEBIT, S_ATC_DEBIT );
			break;

		case REFDEBIT:
			// 
			memcpy( pSendBuff, ATC_DEBIT_REF, S_ATC_DEBIT_REF );
			break;

		case REFUND:
			// 
			memcpy( pSendBuff, ATC_REFUND, S_ATC_REFUND );
			break;

		default:
			// PURCHASE 
			memcpy( pSendBuff, ATC_SALE, S_ATC_SALE );
			break;
	}

	// Update the data pointer 
	pSendBuff += 2;
}


//-----------------------------------------------------------------------------
//  MoveCIDC        Move Cardholder identification code into send buffer.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void MoveCIDC( void )
{
	UBYTE CIDC;

	// if Cardholder ID already defined, use it 
	if ( !( TRREQ.TRCARDHOLDERID ) )
	{
		// if pin data exists, then use it 
		if ( !( TRREQ.TRPINBL[0] ) )
		{
			// if Mail Order Transaction  
			if ( MAIL_ORDER == TRREQ.TRKEY )
			{
				CIDC = 'N';
			}
			else
			{
				// if address or zip code data exists, then avs 
				if ( !( TRREQ.TRAVSADDR[0] || TRREQ.TRZIP[0] ) )
				{
					// Cardholder signature 
					CIDC = '@';
				}
				else
				{
					// AVS data -- manually entered sale 
					CIDC = 'M';
				}
			}
		}
		else
		{
			// pin data 
			CIDC = 'K';
		}
	}
	else
	{
		// Cardholder Id 
		CIDC = TRREQ.TRCARDHOLDERID;
	}

	// Add the Cardholder Identification Code to the buffer 
	*pSendBuff = CIDC;

	// Save it for Detail Upload too 
	TRREQ.TRCARDHOLDERID = CIDC;

	// Update the data pointer 
	pSendBuff++;
}


//-----------------------------------------------------------------------------
//  MoveREQ_SOURCE  Move account data source code into send buffer.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void MoveREQ_SOURCE( void )
{
	// SWIPED ? 
	if ( TRREQ.TRPOSE[1] & BIT_4_SET )
	{
		// manually keyed, track 2 capable 
		*pSendBuff = 'T';
	}
	else
	{
		// full mag-stripe read and transmit, track 2 
		*pSendBuff = 'D';
	}

	// Update the data pointer 
	pSendBuff++;

}


//-----------------------------------------------------------------------------
//  MoveREQ_CUST    Move customer data field into send buffer.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void MoveREQ_CUST( void )
{
	UBYTE RemBytes;
	UBYTE Length;
	UBYTE Value;

	// SWIPED ? 
	if ( TRREQ.TRPOSE[1] & BIT_4_SET )
	{
		// Move manually entered PAN 
		RemBytes = MovPanAsc( ( char * ) pSendBuff, TRREQ.TRPAN, MAXPANL );

		// Update the data pointer 
		pSendBuff += ( MAXPANL - RemBytes );

		// Move IFS Update the data pointer 
		*pSendBuff = IFS;
		pSendBuff++;

		// Append the Expiration Date Month in Ascii to the send data 
		BufAsc( ( char * ) pSendBuff, &TRREQ.TREXPD[1] );

		// Increment the send data pointer 
		pSendBuff += 2;

		// Append the Expiration Date Year in Ascii to the send data 
		BufAsc( ( char * ) pSendBuff, TRREQ.TREXPD );

		// Increment the send data pointer 
		pSendBuff += 2;

		// Move IFS Update the data pointer 
		*pSendBuff = IFS;
		pSendBuff++;
	}
	else
	{
		// Initialize Track 2 data length 
		Length = 0;

		// Move Track 2 Data  
		while ( True )
		{
			// Get byte value 
			Value = TRREQ.TRTRK2[Length];

			// See if Terminator found 
			if ( 0xff == Value )
			{
				// Yes; break out of the loop 
				break;
			}

			// Move this character.  Update the data pointer 
			*pSendBuff = Value;
			pSendBuff++;

			// Bump counter 
			Length++;

			// Check if done 
			if ( S_TRTRK2 == Length )
			{
				// Yes; break out of the loop 
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------
//  MoveREQ_CIDDATA Move Cardholder identification DATA into send buffer.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void MoveREQ_CIDDATA( void )
{
	UWORD DataLen;

	// pin data exists, so use it 
	if ( !( TRREQ.TRPINBL[0] ) )
	{
		// if address or zip code data exists, then avs 
		if ( TRREQ.TRAVSADDR[0] || TRREQ.TRZIP[0] )
		{
				
			// AVS address, update pointer 
			
			// Note: 19 used as length limit instead of S_TRAVSADDR (24) 
			// The reason is that address(24)+space+(zip)9  will exceed
			// field maximum length of 29 per VISAK spec - so the address
			// is truncated here.
			
			DataLen = MoveTillZero( (char *) pSendBuff, (char *)TRREQ.TRAVSADDR , 19);
			pSendBuff += DataLen;

			
			// force a space if one not there
			if( ( *(pSendBuff-1) != ' ' ))
			{
				*pSendBuff = ' ';
				pSendBuff++ ;
			}

			// AVS zip code, update pointer 
			DataLen = MoveTillZero( ( char * ) pSendBuff, ( char * ) TRREQ.TRZIP,
						  S_TRZIP );
			pSendBuff += DataLen;
			
		}
	}
	else
	{
		// convert the packed pin block to ascii 
		BfAscii( ( char * ) pSendBuff, TRREQ.TRPINBL, S_TRPINBL );
		pSendBuff += S_TRPINBL * 2;

		// KSID (Key Set Identifier) is 6 digits (24 bits)          
		// TSRM (PIN Pad Serial Number)is 5 digits - 1 bit (19 bits) 
		// Transaction Counter is 5 digits + 1 bit (21 bits)        
		// Total = 64 bits, or 8 bytes (leftmost 8 bytes of TRKSN)  
		// Converting to ASCII yields 16 bytes.                     
		BfAscii( ( char * ) pSendBuff, TRREQ.TRKSN, ( UBYTE ) 8 );
		pSendBuff += 8 * 2;
	}
}


//-----------------------------------------------------------------------------
//  MoveSECAMT      Move the secoundary amount field, see notes bellow.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          The visa ii 'J'/'G' authorization message formats use 
//                  the secondary amount field for cash back amounts on 
//                  debit sales only. no tip amount is ever put in this field.
//
//-----------------------------------------------------------------------------

static void MoveSECAMT( void )
{
	static int i;

	//  if no cashback amount exixts, leave field empty 
	if ( NullComp( ( char * ) TRREQ.TRCASHBK, S_TRBASE ) )
		return;

	// Clears the temp buffer  
	memset( ( char * ) &TmpBuf, '0',S_TRBASE * 2 );

	// Converts the cashback amount into ASCII and puts it into the temp buffer 
	BfAscii( ( char * ) &TmpBuf, TRREQ.TRCASHBK, S_TRBASE );

	// This loop gets rid of up to 9 leading zeros ('0') 
	for ( i = 0; i < ( ( S_TRBASE * 2 ) - 3 ); i++ )
	{
		if ( !( '0' == TmpBuf[i] ) )
		{
			break;
		}
	}

	// This loop loads the nonzero part of the cashback amount (ASCII) into the 
	// send buffer, if there is no cashback amount it loads 3 zeros ('0')
	while ( i < S_TRBASE * 2 )
	{
		// Load a byte from the buffer and Update the data pointer 
		*pSendBuff = TmpBuf[i];
		pSendBuff++;
		i++;
	}
}


//-----------------------------------------------------------------------------
//  MoveSHAREGROUP  Move the VISA II SHARING GROUP data to send 
//                  buff till '0' or ' '
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void MoveSHAREGROUP( void )
{
	static int i;

	// This loop moves  into the send buffer the 
	// V2DEBIT_GROUP data until ' ' or '0' 
	for ( i = 0; i < 14; i++ )
	{
		if ( ( '0' == pVisa2Ptr->V2DEBIT_GROUP[i] )
			 || ( ' ' == pVisa2Ptr->V2DEBIT_GROUP[i] ) )
		{
			break;
		}
		*pSendBuff = pVisa2Ptr->V2DEBIT_GROUP[i];
		pSendBuff++;
	}
}


//-----------------------------------------------------------------------------
//  MoveBATCHNO     Find batch number for this acquirer and put the last 
//                  three digits of it into send buffer for settlement 
//                  header and trailer messages.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void MoveBATCHNO( void )
{
	// Get the batch table record for this acquirer 
	pBatchRec = FindBatTab( TRREQ.TRAQID );

	// Moves the current batch number to the temp buffer 
	BfAscii( ( char * ) &TmpBuf, pBatchRec->BATCUR, S_BATCUR );

	// Moves the last 3 digits of current batch number 
	// from temp buffer to send buff 
	memcpy( pSendBuff, &TmpBuf[3], ( UWORD ) 3 );

	// Update the data pointer 
	pSendBuff += 3;

}


//-----------------------------------------------------------------------------
//  MoveREIM        Put reimbursement attribute in send buffer.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void MoveREIM( void )
{
	// TRANSACTION KEY HOLDER 
	UBYTE TRKEY_HOLD;

	if ( RUPLOAD == TRREQ.TRKEY )
	{
		TRKEY_HOLD = TRREQ.TRORIGKEY;
	}
	else
	{
		TRKEY_HOLD = TRREQ.TRKEY;
	}

	// check if this is a debit transaction. 
	if ( IsDebitTrans( TRKEY_HOLD ) )
	{
		// Yes; move the reimbursement attribute in send buffer 
		*pSendBuff = pVisa2Ptr->V2REIMBURSE;
	}
	else
	{
		// Move '0' in send buffer 
		*pSendBuff = '0';
	}
	// Update the data pointer 
	pSendBuff++;
}


//-----------------------------------------------------------------------------
//  MoveDEBIT_ABA   Move the debit aba no data to send buff if 
//                  it is not all ' 's or  '0's or 0s .
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void MoveDEBIT_ABA( void )
{
	static int i;
	Bool Status;

	// Initialize Status to okay 
	Status = False;

	// This loop check if in ABA there is something 
	// different to all ' 's or '0's or 0s 
	for ( i = 0; i < 9; i++ )
	{
		if ( !( ( '0' == pVisa2Ptr->V2DEBIT_ABA[i] )
				|| ( ' ' == pVisa2Ptr->V2DEBIT_ABA[i] )
				|| ( 0 == pVisa2Ptr->V2DEBIT_ABA[i] ) ) )
		{

			// Status to True 
			Status = True;
			break;
		}
	}
	if ( Status )
	{
		// Moves in first 9 of debit ABA No, Update the data pointer 
		memcpy( pSendBuff, pVisa2Ptr->V2DEBIT_ABA, ( UWORD ) 9 );
		pSendBuff += 9;
	}
}


//-----------------------------------------------------------------------------
//  MoveSETTLEAGENT     Move the V2DEBIT_SETTLEAGENT data to send buff if 
//                      it is not all ' 's or  '0's or 0s .
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:              None.
//
//-----------------------------------------------------------------------------

static void MoveSETTLEAGENT( void )
{
	static int i;
	UBYTE Status;

	// Initialize Status to okay 
	Status = False;

	// This loop check if in V2DEBIT_SETTLEAGENT there is something
	// different to all ' 's or '0's or 0s 
	for ( i = 0; i < 4; i++ )
	{
		if ( !( ( '0' == pVisa2Ptr->V2DEBIT_SETTLEAGENT[i] )
				|| ( ' ' == pVisa2Ptr->V2DEBIT_SETTLEAGENT[i] )
				|| ( 0 == pVisa2Ptr->V2DEBIT_SETTLEAGENT[i] ) ) )
		{
			// Status to True 
			Status = True;
			break;
		}
	}

	if ( Status )
	{
		// Moves in V2DEBIT_SETTLEAGENT, Update the data pointer 
		memcpy( pSendBuff, pVisa2Ptr->V2DEBIT_SETTLEAGENT,
				S_V2DEBIT_SETTLEAGENT );
		pSendBuff += S_V2DEBIT_SETTLEAGENT;
	}
}


//-----------------------------------------------------------------------------
//  MoveAMT         Put transaction amount in send buffer for sale/auth
//                  only transactions.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void MoveAMT( void )
{
	static int i;

	if ( NullComp( ( char * ) TRREQ.TRTIP, S_TRTIP ) )
	{
		// TIP is empty move TRTOTAMI into TRAUTHAMT 
		memcpy( TRREQ.TRAUTHAMT, TRREQ.TRTOTAMI, S_TRAUTHAMT );
	}
	else
	{
		// TIP is NOT empty move TRTOTAM into TRAUTHAMT 
		memcpy( TRREQ.TRAUTHAMT, TRREQ.TRTOTAM, S_TRAUTHAMT );
	}

	// Convert the AUTHAMT into ASCII and put in TempBuf 
	BfAscii( ( char * ) &TmpBuf, TRREQ.TRAUTHAMT, S_TRAUTHAMT );

	// This loop gets rid of up to 9 leading zeros ('0') 
	for ( i = 0; i < ( ( S_TRAUTHAMT * 2 ) - 3 ); i++ )
	{
		if ( !( '0' == TmpBuf[i] ) )
		{
			break;
		}
	}

	// This loop loads the nonzero part of the cashback amount (ASCII) into the 
	// send buffer, if there is no cashback amount it loads 3 zeros ('0') 
	while ( i < S_TRAUTHAMT * 2 )
	{
		// Load a byte from the buffer and Update the data pointer 
		*pSendBuff = TmpBuf[i];
		pSendBuff++;

		i++;
	}
}


//-----------------------------------------------------------------------------
//  Req_Confirm     Build a visanet, base ii debit confirmation
//                  for all debit trans.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void Req_Confirm( void )
{
	// Move the Visa D format 'V0.' in the Header 
	memcpy( pSendBuff, DBC_FORMAT, S_DBC_FORMAT );

	// Update the data pointer 
	pSendBuff += S_DBC_FORMAT;

	//  PUT ACQUIRER BANK# IN SEND BUFFER 
	memcpy( pSendBuff, pVisa2Ptr->V2BNUM, ( UWORD ) 6 );

	// Update the data pointer 
	pSendBuff += 6;

	// Host Message Identifier 
	MoveOrSpace( ( char * ) pSendBuff, ( char * ) TRREQ.TRHSTMSGID,
				 S_TRHSTMSGID );

	// Update the data pointer 
	pSendBuff += S_TRHSTMSGID;

	// Move the SYSTEM TRACE AUDIT NUMBER 
	BfAscii( ( char * ) pSendBuff, TRREQ.TRSTAN, S_TRSTAN );

	// Update the data pointer 
	pSendBuff += S_TRSTAN * 2;
}


//-----------------------------------------------------------------------------
//  MoveHeader      Put the first 5 fields into the record.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void MoveHeader( void )
{
	// Move the Visa D format 'K1.Z' in the Header 
	memcpy( pSendBuff, V2_HEAD, S_V2_HEAD );

	// Update the data pointer 
	pSendBuff += S_V2_HEAD;

	// Save pointer for later use 
	pSendBuffSave = pSendBuff;

	// Move the Visa D format ' @@@@' in the Header 
	memcpy( pSendBuff, V2_RTYPE, S_V2_RTYPE );

	// Update the data pointer 
	pSendBuff += S_V2_RTYPE;
}


//-----------------------------------------------------------------------------
//  Req_SetTrail    Build a visanet, base ii settlement batch trailer
//                  Message is for 'K' device types, record type is 'T'
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void Req_SetTrail( void )
{
	UBYTE RtcBuf[RTCLEN];	// Real time clock Buffer 

	// Increment RCOUNT (BCD) number 
	IncBcdLen( RCOUNT, S_RCOUNT );

	// Move Header into send Buffer 
	MoveHeader(  );

	// Move 'T' 
	*pSendBuffSave = 'T';

	// Get the current Time to set Year 
	SDK_RtcRead( RtcBuf );

	// Put transmission date in MMYY into send buffer 
	memcpy( pSendBuff, RtcBuf + 2, ( UWORD ) 4 );

	// Update the data pointer 
	pSendBuff += 4;

	// Move the Batch Number 
	MoveBATCHNO(  );

	//  hard wire first 3 digits to '0'. 
	memset( ( char * ) pSendBuff, '0',3 );

	// Update the data pointer 
	pSendBuff += 3;

	// Batch Record Count 
	UnPack( ( char * ) pSendBuff, RCOUNT, S_RCOUNT );

	// Update the data pointer 
	pSendBuff += S_RCOUNT * 2;

	//  hard wire first 4 digits to '0'. 
	memset( ( char * ) pSendBuff, '0', 4 );

	// Update the data pointer 
	pSendBuff += 4;

	// Convert Batch Hashing Total to ASCII 
	// put into send buff 
	BfAscii( ( char * ) pSendBuff, HashTotal, S_TRTOTAM );

	// Update the data pointer 
	pSendBuff += S_TRTOTAM * 2;

	//  hard wire first 4 digits to '0'. 
	memset( ( char * ) pSendBuff, '0', 4 );

	// Update the data pointer 
	pSendBuff += 4;

	// Convert Cash back Total to ASCII 
	// put into send buff 
	BfAscii( ( char * ) pSendBuff, CashBackTotal, S_TRTOTAM );

	// Update the data pointer 
	pSendBuff += S_TRTOTAM * 2;

	// hard wire first 4 digits to '0'. 
	memset( ( char * ) pSendBuff, '0', 4 );

	// Update the data pointer 
	pSendBuff += 4;

	// Is this a Negative Amount 
	if ( NetTotal[0] )
	{
		// Yes; clear the temp buffer  
		memset( TmpBuf, 0,S_TRTOTAM );

		// Change the sign of the NetTotal 
		SubAmt( TmpBuf, TmpBuf, NetTotal );

		// Batch Net Deposit, Put into send buff 
		BfAscii( ( char * ) pSendBuff, TmpBuf, S_TRTOTAM );
	}
	else
	{
		// Batch Net Deposit, Put into send buff 
		BfAscii( ( char * ) pSendBuff, NetTotal, S_TRTOTAM );
	}

	// Update the data pointer 
	pSendBuff += S_TRTOTAM * 2;
}


//-----------------------------------------------------------------------------
//  Req_SetHead     Build a visanet, base ii settlement header message this 
//                  message is for the 'K' device type, record type is 'K'.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void Req_SetHead( void )
{
	UBYTE RtcBuf[RTCLEN];	// Real time clock Buffer 

	// Clear out hashing total 
	memset( HashTotal, 0,S_TRTOTAM );

	// Clear out Cash back total 
	memset( CashBackTotal, 0,S_TRTOTAM );

	// Clear out Net total 
	memset( NetTotal, 0,S_TRTOTAM );

	// Clear out Record Counter 
	memset( RCOUNT, 0,S_RCOUNT );

	// Increment RCOUNT (BCD) number 
	IncBcdLen( RCOUNT, S_RCOUNT );

	// Move Header into send Buffer 
	MoveHeader(  );

	// Move 'H' 
	*pSendBuffSave = 'H';

	//  PUT ACQUIRER BANK# IN SEND BUFFER 
	memcpy( pSendBuff, pVisa2Ptr->V2BNUM, ( UWORD ) 6 );

	// Update the data pointer 
	pSendBuff += 6;

	// Moves agent number and chain number, Update the data pointer 
	memcpy( pSendBuff, ( pVisa2Ptr->V2BNUM ) + 6, ( UWORD ) ( 6 + 6 ) );
	pSendBuff += 12;

	// PUT VISA II MERCHANT#, STORE# AND TERMINAL# IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2MERCH, S_V2MERCH + S_V2STORE + S_V2TERM );
	pSendBuff += S_V2MERCH + S_V2STORE + S_V2TERM;

	// Move the device code & industry code 
	// Yes; see if Restaurant Mode Selected 
	if ( 0 )
	{
		// Yes Restaurant mode so Restaurant 
		// Move DEVICE CODE & INDUSTRY CODE 
		memcpy( pSendBuff, V2_DEVICE_REST, S_V2_DEVICE_REST );
	}
	else
	{
		// No Restaurant mode so Retail 
		// Move DEVICE CODE & INDUSTRY CODE 
		memcpy( pSendBuff, V2_DEVICE_RETL, S_V2_DEVICE_RETL );
	}
	// Update the data pointer 
	pSendBuff += 2;

	// PUT MERCHANT CURRENCY CODE IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2CURRENCY, S_V2CURRENCY );
	// Update the data pointer 
	pSendBuff += S_V2CURRENCY;

	// PUT Language Indicator IN SEND BUFFER. 
	memcpy( pSendBuff, V2_LANGUAGE_IND, S_V2_LANGUAGE_IND );
	// Update the data pointer 
	pSendBuff += S_V2_LANGUAGE_IND;

	// PUT Time Zone Differential IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2TIMEZONE, S_V2TIMEZONE );
	// Update the data pointer 
	pSendBuff += S_V2TIMEZONE;

	// Get the current Time to set Year 
	SDK_RtcRead( RtcBuf );

	// Put transmission date in MMYY into send buffer 
	memcpy( pSendBuff, RtcBuf + 2, ( UWORD ) 4 );

	// Update the data pointer 
	pSendBuff += 4;

	// Move the Batch Number 
	MoveBATCHNO(  );

	// Move Blocking Indicator, 
	// 2 means blocking used, 0 means not used. 
	// Update the data pointer 
	*pSendBuff = '0';
	pSendBuff++;

	// End of Block Indicator 
	*pSendBuff = ETB;
	pSendBuff++;
}


//-----------------------------------------------------------------------------
//  Req_SetParm     Build a visanet, base ii settlement TERMINAL PARAMETER 
//                  message this message is for the 'K' device type, 
//                  record type is 'P'.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void Req_SetParm( void )
{
	// Increment RCOUNT (BCD) number 
	IncBcdLen( RCOUNT, S_RCOUNT );

	// Move Header into send Buffer 
	MoveHeader(  );

	// Move 'P' 
	*pSendBuffSave = 'P';

	// PUT MERCHANT COUNTRY CODE IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2COUNTRY, S_V2COUNTRY );
	pSendBuff += S_V2COUNTRY;

	// PUT MERCHANT CITY CODE IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2CITY, S_V2CITY );
	pSendBuff += S_V2CITY;

	// PUT LAST 4 DIGITS OF ZIP CODE IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2EXTZIP, S_V2EXTZIP );
	pSendBuff += S_V2EXTZIP;

	// PUT Merchant Category Code IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2CATEG, S_V2CATEG );
	pSendBuff += S_V2CATEG;

	// Moves merchant name, plus city and state, Update the data pointer 
	memcpy( pSendBuff, pVisa2Ptr->V2MERCHNAME, S_V2MERCHNAME + S_V2CITYST );
	pSendBuff += S_V2MERCHNAME + S_V2CITYST;

	// Get first byte OF LOCATION CODE 
	if ( pVisa2Ptr->V21STLOCAT <= 0x20 )
	{
		// DEFAULT TO CHAR '0' 
		*pSendBuff = '0';
	}
	else
	{
		// FIRST DIGIT OF LOCATION CODE 
		*pSendBuff = pVisa2Ptr->V21STLOCAT;
	}

	// Update the data pointer 
	pSendBuff++;

	// PUT LOCATION CODE and TERMINAL IDENTIFICATION/SERIAL NUMBER IN SEND BUFFER. 
	memcpy( pSendBuff, pVisa2Ptr->V2LOCAT, S_V2LOCAT + S_V2TSER );
	pSendBuff += S_V2LOCAT + S_V2TSER;

	// End of Block Indicator 
	*pSendBuff = ETB;
	pSendBuff++;
}


//-----------------------------------------------------------------------------
//  Req_SetDetail   Build a visanet, base ii DETAIL TRANSACTION message for
//                  Message is for 'K' device types, record type is 'D'.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

static void Req_SetDetail( void )
{
	// Increment RCOUNT (BCD) number 
	IncBcdLen( RCOUNT, S_RCOUNT );

	//  Add cash back amount to cash back total 
	AddAmt( HashTotal, HashTotal, TRREQ.TRTOTAM );

	//  Add cash back amount to cash back total 
	AddAmt( CashBackTotal, CashBackTotal, TRREQ.TRCASHBK );

	// Get the Transaction from the Request 
	HostEvent.TRNKEY = TRREQ.TRORIGKEY;

	// Locate the Transaction - will be in HostEvent 
	HostFindTran();

	// See what type of Adjust 
	if ( HostEvent.TRNOPT1 & T1_CREDIT )
	{
		//  Add cash back amount to cash back total 
		SubAmt( NetTotal, NetTotal, TRREQ.TRTOTAM );
	}
	else
	{
		//  Add cash back amount to cash back total 
		AddAmt( NetTotal, NetTotal, TRREQ.TRTOTAM );
	}

	// Move Header into send Buffer 
	MoveHeader(  );

	// Move 'D' 
	*pSendBuffSave = 'D';

	// AUTHORIZATION TRANSACTION CODE 
	MoveATC(  );

	// CARDHOLDER IDENTIFICATION CODE 
	MoveCIDC(  );

	// ACCOUNT DATA SOURCE CODE 
	MoveREQ_SOURCE(  );

	// SPACE FILLED 
	memset( ( char * ) pSendBuff, ' ',( UWORD ) 22 );

	// Move manually entered PAN 
	MovPanAsc( ( char * ) pSendBuff, TRREQ.TRPAN, MAXPANL );

	// Update the data pointer 
	pSendBuff += 22;

	// PUT if DEVICE IS CPS CAPABLE IN SEND BUFFER. requested ACI 
	*pSendBuff = 'Y';

	// Update the data pointer 
	pSendBuff++;

	// Payment Message Identifier 
	MoveOrSpace( ( char * ) pSendBuff, ( char * ) &TRREQ.TRPSI, ( UWORD ) 1 );

	// Update the data pointer 
	pSendBuff++;

	// Authorization Source Code 
	// IF IT IS ALREADY DEFINED, USE IT 
	if ( TRREQ.TRAUTHSOURCE > 0x20 )
	{
		*pSendBuff = TRREQ.TRAUTHSOURCE;
	}
	else
	{
		// See what type of Adjust 
		if ( HostEvent.TRNOPT1 & T1_CREDIT )
		{
			// USE '9' FOR REFUNDS  
			*pSendBuff = '9';
		}
		else
		{
			//  NOT ANYTHING ELSE SO MUST BE 
			//  OFFLINE VOICE APPROVED 
			*pSendBuff = 'E';
		}
	}

	// Update the data pointer 
	pSendBuff++;

	// Move the SEQNUM 'Transaction sequence number' 
	BfAscii( ( char * ) pSendBuff, &TRREQ.TRSTAN[1], ( UBYTE ) 2 );

	// Update the data pointer 
	pSendBuff += 2 * 2;

	// Response Code 
	MoveOrSpace( ( char * ) pSendBuff, ( char * ) TRREQ.TRV2RESP,
				 S_TRV2RESP );

	// Update the data pointer 
	pSendBuff += S_TRV2RESP;

	// Authorization Code 
	MoveOrSpace( ( char * ) pSendBuff, ( char * ) TRREQ.TRAUTH, S_TRAUTH );

	// Update the data pointer 
	pSendBuff += S_TRAUTH;

	// Convert date to ASCII and put it into send buff' 
	BfAscii( ( char * ) pSendBuff, TRREQ.TRDATE, S_TRDATE );

	// Update the data pointer 
	pSendBuff += S_TRDATE * 2;

	//  Local Transaction Time (HHMMSS)' 
	BfAscii( ( char * ) pSendBuff, TRREQ.TRTIME, S_TRTIME );

	// Update the data pointer 
	pSendBuff += S_TRTIME * 2;

	// Get first byte, IS AVS RESULT CODE DEFINED ? 
	if ( TRREQ.TRAVSRSP[0] )
	{
		// AVS Result Code  
		*pSendBuff = TRREQ.TRAVSRSP[0];
        }
	else
	{
		// DEFAULT TO CHAR '0' 
		*pSendBuff = '0';
	}

	// Update the data pointer 
	pSendBuff++;

	// Move Transaction Identifier 
	MovAscZFill( ( char * ) pSendBuff, ( char * ) TRREQ.TRTRANID, S_TRTRANID,
				 '0' );

	// Update the data pointer 
	pSendBuff += S_TRTRANID;

	// Move Validation Code 
	MovAscZFill( ( char * ) pSendBuff, ( char * ) TRREQ.TRVALID, S_TRVALID,
				 ' ' );

	// Update the data pointer 
	pSendBuff += S_TRVALID;

	// Void Indicator, Update the data pointer 
	*pSendBuff = ' ';
	pSendBuff++;

	// Void Indicator, Update the data pointer 
	*pSendBuff = '0';
	pSendBuff++;

	// WAS AN ACK RECEIVED FOR DEBIT CONFIRM MSG  ? 
	if ( TRREQ.TRDEBIT_STATUS )
	{
		// ACK WAS NOT RECEIVED 
		*pSendBuff = '1';
	}
	else
	{
		// ACK WAS RECEIVED 
		*pSendBuff = '0';
	}

	// Update the data pointer 
	pSendBuff++;

	// Reimbursement Attribute 
	MoveREIM(  );

	// Settlement Amount 
	BfAscii( ( char * ) pSendBuff, TRREQ.TRTOTAM, S_TRTOTAM );

	// Update the data pointer 
	pSendBuff += S_TRTOTAM * 2;

	//  Authorized Amount 
	BfAscii( ( char * ) pSendBuff, TRREQ.TRAUTHAMT, S_TRAUTHAMT );

	// Update the data pointer 
	pSendBuff += S_TRAUTHAMT * 2;

	// Group 1 -- Cashback 
	// See what type of Adjust 
	if ( ( HostEvent.TRNOPT4 & T4_DEBCARD ) &&
		 ( !NullComp( ( char * ) TRREQ.TRCASHBK, S_TRTOTAM ) ) )
	{
		// Cashback Amount 
		BfAscii( ( char * ) pSendBuff, TRREQ.TRCASHBK, S_TRBASE );

		// Update the data pointer 
		pSendBuff += S_TRBASE * 2;

		// Set 0 bit  
		*( pSendBuffSave + 4 ) |= BIT_0_SET;
	}

	
	//Group 10 -- Direct Debit 
	if ( HostEvent.TRNOPT4 & T4_DEBCARD )
	{
		//Retrieval Reference Number 
		memcpy( pSendBuff, TRREQ.TRRRN, S_TRRRN );

		// Update the data pointer 
		pSendBuff += S_TRRRN;

		// Move the Original SYSTEM TRACE AUDIT NUMBER 
		BfAscii( ( char * ) pSendBuff, TRREQ.TROSTAN, S_TRSTAN );

		// Update the data pointer 
		pSendBuff += S_TRSTAN * 2;

		// Network Identification Code 
		*pSendBuff = TRREQ.TRDEBIT_NETWORKID;
		pSendBuff++;

		// Settlement Date (mmdd) 
		memcpy( pSendBuff, TRREQ.TRDEBIT_SETLDATE, S_TRDEBIT_SETLDATE );

		// Update the data pointer 
		pSendBuff += S_TRDEBIT_SETLDATE;

		// Set 3 bit  
		*( pSendBuffSave + 3 ) |= BIT_3_SET;
	}

	// Group 22 - level II commercial/purchasing card (non-t&e,u.s.only) 
	// See if optional amount or P.O. number is present? 
	if ( ( TRREQ.TROPTAMTID > 0x20 ) || ( TRREQ.TRPONUM[0] > 0x20 ) )
	{
		// Set 3 bit  
		*( pSendBuffSave + 1 ) |= BIT_3_SET;

		if ( TRREQ.TROPTAMTID > 0x20 )
		{
			*pSendBuff = TRREQ.TROPTAMTID;
			pSendBuff++;

			BfAscii( ( char * ) pSendBuff, TRREQ.TROPTAMT, S_TROPTAMT );

			// Update the data pointer 
			pSendBuff += S_TROPTAMT * 2;
		}
		else
		{
			memset( ( char * ) pSendBuff, '0', ( UWORD ) 13 );

			// Update the data pointer 
			pSendBuff += 13;
		}

		// Move in Purchase Order Number 
		MoveOrSpace( ( char * ) pSendBuff, ( char * ) TRREQ.TRPONUM,
					 ( UWORD ) 17 );
	}

	// End of Block Indicator 
	*pSendBuff = ETB;
	pSendBuff++;
}

