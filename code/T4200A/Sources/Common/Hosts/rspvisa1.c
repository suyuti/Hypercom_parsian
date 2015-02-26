//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             rspvisa1.c
//      Visa 1 Response Parsing Routines.
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"

#include "utllib.h"
#include "settime.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "fixdata.h"
#include "comm.h"
#include "host.h"
#include "hstmsg.h"
#include "hstresp.h"
#include "reqvisa1.h"


//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================

#define S_APPROVAL 3


//=============================================================================
// Private function declarations
//=============================================================================

static void respInit( void );
static void saveRspText( void );
static void setApprovalCode( void );
static void processHostText( void );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Handles Visa 1 Response
//!
//! \param
//!		[in]ReqCommStatus		status Req Communication
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void RspVisa1( OS_RETURN_VALS ReqCommStatus )
{
	OS_RETURN_VALS CommStatus;

	// Clear the Buffer
	memset( ( UBYTE * ) & TRRSP, 0,TRSZE );

	// Set Initial Responses
	respInit(  );

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

		// Save Host Response Text
		saveRspText(  );
		setApprovalCode(  );
		processHostText(  );
	}

	// CALCULATE BASE
	SubAmt( TRRSP.TRBASE + 5, TRRSP.TRTOTAM + 5, TRRSP.TRTIP + 5 );

	// MOVE FIELD FROM TRRSP TO TRREQ
	// Move the Response and Auth Code to TRREQ
	memcpy( TRREQ.TRRSPC, TRRSP.TRRSPC, S_TRRSPC );
	memcpy( TRREQ.TRAUTH, TRRSP.TRAUTH, S_TRAUTH );

	// Look up Response in TRREQ
	RspLookUp( &TRREQ );

	// Now set the time
	SetTranTime( TRREQ.TRYEAR );
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
static void respInit( void )
{
	// Approved Response
	memcpy( TRRSP.TRRSPC, RSP_APP, S_TRRSPC );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Saves host response text to TRREQ.TRHOSTTEXT
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void saveRspText( void )
{
	UWORD dataLen;

	memset( ( char * ) TRREQ.TRHOSTTEXT, ' ',S_TRHOSTTEXT );
	if ( HDLC_VISA )
	{
		// HDLC TPUD ON VISA MSG
		RecvMsgLen -= 5;		// TPDU Length is 5
		pRecvData += 5;			// Go past TPDU
		HDLC_VISA = False;
	}

	dataLen = S_TRHOSTTEXT;		// Assume RecvMsgLen < 256
	
	if ( RecvMsgLen < S_TRHOSTTEXT )
		dataLen = RecvMsgLen;

	// Move Max of S_TRHOSTTEXT data to TRHOSTTEXT, if receive data
	// less than S_TRHOSTTEXT, move actual length of receive data.
	memcpy( TRREQ.TRHOSTTEXT, pRecvData, dataLen );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Extracts and save host approval code
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void setApprovalCode( void )
{
	const char appr[] = "APP";
	UBYTE rspLen;
	UBYTE leftByte;
	UBYTE i;
	UBYTE *pTrHostText;

	//  Make "DECLINED" the default response code
	memcpy( TRRSP.TRRSPC, RSP_DECLINED, S_TRRSPC );

	// Examine first 30 bytes of HOSTTEXT (40-10)
	rspLen = S_TRHOSTTEXT - 10;
	pTrHostText = TRREQ.TRHOSTTEXT;

	// Strip off LEADING SPACES
	for ( i = 0; i < rspLen; i++ )
	{
		if ( pTrHostText[i] != ' ' )
		{
			// Response Text
			break;
		}
	}

	if ( i >= rspLen )
	{
		// Host Text is empty
		return;
	}

	// Processing remaining characters Dump pointer
	pTrHostText += i;

	if ( 0 != memcmp( pTrHostText, ( UBYTE * ) appr, S_APPROVAL ) )
	{
		// Not match to APP. Go to next byte to check
		// whether there is *APP
		pTrHostText += 1;
		if ( 0 != memcmp( pTrHostText, ( UBYTE * ) appr, S_APPROVAL ) )
		{
			// Not found *APP, return, the default is RSP_DECLINED
			return;
		}
	}

	// From HOSTTEXT, APP is found,put APPROVED CODE to TRRSP
	memcpy( TRRSP.TRRSPC, RSP_APP, S_TRRSPC );

	// Clear the AUTH CODE first
	memset( ( char * ) TRRSP.TRAUTH, ' ',S_TRAUTH );

	// Move pointer to the last byte of HOSTTEXT
	pTrHostText = TRREQ.TRHOSTTEXT + S_TRHOSTTEXT - 1;

	// Extract authno
	for ( i = 0; i < S_TRHOSTTEXT; i++ )
	{
		if ( ( *pTrHostText == '-' ) || ( *pTrHostText == 0x0A )
			 || ( *pTrHostText == ' ' ) )
		{
			// Ignore '-', SPACE, 0A
			pTrHostText--;
		}
		else
		{
			// Not a DASH,SPACE, 0A
			break;
		}
	}

	leftByte = S_TRHOSTTEXT - i;

	if ( leftByte <= 0 )
	{
		// ALL HOSTTEXT are DASH,SPACE or 0A
		return;
	}

	for ( i = 0; i < leftByte; i++ )
	{
		if ( *pTrHostText != ' ' )
		{
			// If not SPACE, go further
			pTrHostText--;
		}
		else
		{
			// Yes, this is a SPACE
			break;
		}
	}

	if ( i >= leftByte )
	{
		// There is not a authno
		return;
	}

	pTrHostText++;				// Past the space

	memcpy( TRRSP.TRAUTH, pTrHostText, S_TRAUTH );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Beginning from the last byte of HOSTTEXT, downcount looking for a non
//!     space character. Clear all other bytes.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void processHostText( void )
{
	UBYTE *pHostText;
	UBYTE leftByte;
	UBYTE i;

	// Pointer to the last byte of HOSTTEXT
	pHostText = TRREQ.TRHOSTTEXT + S_TRHOSTTEXT - 1;

	for ( i = 0; i < S_TRHOSTTEXT; i++ )
	{
		if ( *pHostText != ' ' )
		{
			break;
		}
		pHostText--;
	}

	leftByte = S_TRHOSTTEXT - i;

	if ( leftByte <= 0 )
	{
		// Nothing there, clear all bytes
		memset( TRREQ.TRHOSTTEXT, 0,S_TRHOSTTEXT );
	}
	else
	{
		if ( leftByte <= 20 )
		{
			// Less than 20 bytes in HOSTTEXT, clear all other bytes
			memset( TRREQ.TRHOSTTEXT + 20, 0,20 );
		}
	}
}

