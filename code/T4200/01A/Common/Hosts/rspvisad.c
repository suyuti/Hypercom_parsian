//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             rspvisad.c
//      Visa D Response Parsing Routines
//
//=============================================================================
#include "string.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "utllib.h"
#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "fixdata.h"
#include "msg.h"
#include "comm.h"
#include "host.h"
#include "hstmsg.h"
#include "hstresp.h"
#include "updttime.h"
#include "isdebit.h"


//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================

//  ANY RESPONSE CODE NOT IN THE FOLLOWING TABLE IS DECLINED
//  AUTHORIZATION RESPONSE CODES

#define RSP00		"00"
//! APPROVED
#define RSP00_OUR	"00"
#define RSP85		"85"
//! NO REASON TO DECLINE
#define RSP85_OUR	"00"
#define RSPN7		"N7"
//! CVV2 MISMATCH
#define RSPN7_OUR	"79"

//! SETTLEMENT RESPONSE CODES
#define RSPQD   	"QD"
//! DUPLICATE BATCH - RECONCILE ERROR
#define RSPQD_OUR	"77"
#define RSPGB		"GB"
//! APPROVED
#define RSPGB_OUR	"00"
#define RSPRB		"RB"
//! REJECTED BATCH - RECONCILE ERROR
#define RSPRB_OUR	"77"


//=============================================================================
// Private function declarations
//=============================================================================

static void RespInit( void );
static void RspUnPackVisaD( void );
static void RspUnPackAuth( void );
static void RspUnPackSettle( void );
static void TranslateRspCode( void );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

//! Temporary response code
static UBYTE RspCode[2];


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Handles Visa D Response
//!
//! \param
//!		[in]ReqCommStatus		status Req Communication
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void RspVisaD( OS_RETURN_VALS ReqCommStatus )
{
	OS_RETURN_VALS CommStatus;

	// Clear the Buffer
	memset( ( UBYTE * ) & TRRSP, 0,TRSZE );

	// Set Initial Responses
	RespInit(  );

	// Check if Request Packet was sent okay
	if ( ReqCommStatus == orvOK )
	{
		// Yes; get Response Message from COM
		RecvMessage();
		CommStatus = HostOSRetval;
	}
	else
	{
		// No; set Comm status to reflect the send status
		CommStatus = ReqCommStatus;
	}

	// Check if Normal Data Was Received
	if ( HostResp( CommStatus ) )
	{
		// Yes; set Receive Buffer Pointer to start
		pRecvData = RECVBUF;

		// Unpack the Message
		RspUnPackVisaD(  );
	}

	// Move the Response Code to TRREQ
	memcpy( TRREQ.TRRSPC, TRRSP.TRRSPC, S_TRRSPC );

	// Look up Response in TRREQ
	RspLookUp( &TRREQ );

	// Set the Time for this Transaction
	UpdateTime(  );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Sets Initial Values for Response
//!
//! \return
//!     None
//!
//! \note
//!		Any values validated in ValRsp must be set up in this routine for
//!     timed out responses, and other errors.
//-----------------------------------------------------------------------------
static void RespInit( void )
{
	// Approved Response
	memcpy( TRRSP.TRRSPC, RSP_APP, S_TRRSPC );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Unpacks the Visa D response
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void RspUnPackVisaD( void )
{
	if ( *pRecvData == 'K' )
	{
		RspUnPackSettle(  );
	}
	else
	{
		RspUnPackAuth(  );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Unpacks the Visa D Authorization Response
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void RspUnPackAuth( void )
{
	static UBYTE bytecnt;

	// Skip over first 3 fields
	pRecvData += 3;

	// Get Returned aci
	TRREQ.TRPSI = *pRecvData;
	pRecvData++;

	// Skip over next 2 fields
	pRecvData += 8;

	// Get Authorization source code
	TRREQ.TRAUTHSOURCE = *pRecvData;
	pRecvData++;

	// Skip over next field
	pRecvData += 4;

	// Move Response code to local buffer
	memcpy( RspCode, pRecvData, S_TRV2RESP );

	// Get response code
	memcpy( TRREQ.TRV2RESP, pRecvData, S_TRV2RESP );
	pRecvData += 2;

	// Translate received Response code to our Response Codes
	TranslateRspCode(  );

	// Get Authorization code /approval number
	memcpy( TRREQ.TRAUTH, pRecvData, S_TRAUTH );
	pRecvData += S_TRAUTH;

	// LOCAL TRANSACTION DATE (MMDDYY)
	// EXTRACT MMDD
	AscHex( TRREQ.TRDATE, ( char * ) pRecvData, S_TRDATE );
	pRecvData += ( 2 * S_TRDATE );

	// EXTRACT YY
	AscHex( TRREQ.TRYEAR, ( char * ) pRecvData, S_TRYEAR );
	pRecvData += ( 2 * S_TRYEAR );

	// LOCAL TRANSACTION TIME (HHMMSS)
	AscHex( TRREQ.TRTIME, ( char * ) pRecvData, S_TRTIME );
	pRecvData += ( 2 * S_TRTIME );

	// Space filled
	memset( ( char * ) TRREQ.TRHOSTTEXT, ' ',( UWORD ) 20 );

	// Get response text
	memcpy( TRREQ.TRHOSTTEXT, pRecvData, ( UWORD ) 16 );
	pRecvData += 16;

	// Get AVS result code
	TRREQ.TRAVSRSP[0] = *pRecvData;
        pRecvData++;

	// Temp fix to prevent AVS from being printer when '0'
	if ( '0' == TRREQ.TRAVSRSP[0] )
		TRREQ.TRAVSRSP[0] = 0;

	// Get retreival reference number.
	memcpy( TRREQ.TRRRN, pRecvData, S_TRRRN );
	pRecvData += S_TRRRN;

	// Skip over next field
	pRecvData++;

	// Get transaction identifier
	bytecnt = ( UBYTE ) MoveTillChar( ( char * )TRREQ.TRTRANID,
									  ( char * )pRecvData, 
									  ( UWORD ) 16, ( char ) 0x1C );
	pRecvData += bytecnt;

	// Skip FS
	pRecvData++;

	// Get validation code
	bytecnt = ( UBYTE ) MoveTillChar( ( char * ) TRREQ.TRVALID,
									  ( char * ) pRecvData, 
									  ( UWORD ) 5, ( char ) 0x1C );
	pRecvData += bytecnt;

	// Skip FS
	pRecvData++;

	// See if a Debit Type Transaction
	if ( IsDebitTrans( TRREQ.TRKEY ) )
	{
		// Yes; skip over key set identifier.
		pRecvData += 6;

		//  HOST MESSAGE IDENTIFIER.
		memcpy( TRREQ.TRHSTMSGID, pRecvData, S_TRHSTMSGID );

		// Update the data pointer
		pRecvData += S_TRHSTMSGID;

		// CONVERT RETURNED STAN TO BCD
		AscHex( TRREQ.TRSTAN, ( char * ) pRecvData, S_TRSTAN );

		// Update the data pointer
		pRecvData += S_TRSTAN;

		// NETWORK ID.
		TRREQ.TRDEBIT_NETWORKID = *pRecvData;

		// Update the data pointer
		pRecvData++;

		// SETTLEMENT DATE
		memcpy( TRREQ.TRDEBIT_SETLDATE, pRecvData, 4 );

		// Update the data pointer
		pRecvData += 4;

		// Skip over field separator
		pRecvData++;
	}
	else
	{
		while ( 1 )
		{
			// Check group III version number
			if ( 0 == memcmp( ( UBYTE * ) pRecvData, 
						  ( UBYTE * ) "001", ( UWORD ) 3 ) )
			{
				// Yes, Skip the version number "001"
				pRecvData += 3;

				// Get level II commercial/purchasing card indicator
				TRREQ.TRLVL2ID = *pRecvData;

				// Skip over group separator
				pRecvData += 2;
			}
			else if ( 0 == memcmp( ( UBYTE * ) pRecvData, 
							   ( UBYTE * ) "007", ( UWORD ) 3 ) )
			{
				// Version number is 007, It is the CVV2 response messages
				// Get the CVV2 data on the future
				break;
			}
			else
				break;
		}
	}

	return;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Transalates VISANET BASE II response codes into our response codes
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void TranslateRspCode( void )
{
	//  Make "DECLINED" the default response code
	memcpy( TRRSP.TRRSPC, RSP_DECLINED, S_TRRSPC );

	if ( 0 == memcmp( RspCode, ( UBYTE * ) RSP00, S_TRRSPC ) )
	{
		//  "00" Is converted to our response code
		memcpy( TRRSP.TRRSPC, RSP00_OUR, S_TRRSPC );
	}
	else if ( 0 == memcmp( RspCode, ( UBYTE * ) RSP85, S_TRRSPC ) )
	{
		//  "85" Is converted to our response code
		memcpy( TRRSP.TRRSPC, RSP85_OUR, S_TRRSPC );
	}
	else if ( 0 == memcmp( RspCode, ( UBYTE * ) RSPN7, S_TRRSPC ) )
	{
		//  "N7" Is converted to our response code
		memcpy( TRRSP.TRRSPC, RSPN7_OUR, S_TRRSPC );
	}
	else if ( 0 == memcmp( RspCode, ( UBYTE * ) RSPQD, S_TRRSPC ) )
	{
		//  "QD" Is converted to our response code
		memcpy( TRRSP.TRRSPC, RSPQD_OUR, S_TRRSPC );
	}
	else if ( 0 == memcmp( RspCode, ( UBYTE * ) RSPGB, S_TRRSPC ) )
	{
		//  "GB" Is converted to our response code
		memcpy( TRRSP.TRRSPC, RSPGB_OUR, S_TRRSPC );
	}
	else if ( 0 == memcmp( RspCode, ( UBYTE * ) RSPRB, S_TRRSPC ) )
	{
		//  "RB" Is converted to our response code
		memcpy( TRRSP.TRRSPC, RSPRB_OUR, S_TRRSPC );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Unpacks the Visa D Settlement response
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void RspUnPackSettle( void )
{
	UBYTE RtcBuf[RTCLEN];		// Real time clock Buffer

	// Skip over first 7 fields
	pRecvData += ( 1 + 1 + 1 + 1 + 5 + 9 + 16 );

	//  Get Response code to local buffer
	memcpy( RspCode, pRecvData, S_TRRSPC );

	// Translate received Response code to our Response Codes
	TranslateRspCode(  );

	// Space filled
	memset( ( char * ) TRREQ.TRHOSTTEXT, ' ',S_TRHOSTTEXT );

	// Get batch response code
	memcpy( TRREQ.TRHOSTTEXT, pRecvData, S_TRRSPC );
	pRecvData += S_TRRSPC;

	TRREQ.TRHOSTTEXT[2] = '-';

	// Skip over next field
	pRecvData += 2;

	// Get batch number
	memcpy( &TRREQ.TRHOSTTEXT[3], pRecvData, 3 );
	pRecvData += 3;

	TRREQ.TRHOSTTEXT[6] = '-';

	// Get miscellaneous
	memcpy( &TRREQ.TRHOSTTEXT[7], pRecvData, 13 );
	pRecvData += 13;

	// Get the current Time
	SDK_RtcRead( RtcBuf );

	// Fill the Transaction Date/Time with current RTC Date/Time
	AscHex( TRREQ.TRYEAR, ( char * ) &RtcBuf[0], 2 );
	AscHex( TRREQ.TRDATE, ( char * ) &RtcBuf[2], 2 );

	AscHex( &TRREQ.TRTIME[0], ( char * ) &RtcBuf[6], 2 );
	AscHex( &TRREQ.TRTIME[2], ( char * ) &RtcBuf[8], 2 );

	return;
}

