//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             reqvisa1.c
//      Visa 1 Request Building Routines.
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdkos.h"
#include "sdk.h"
#include "sdkmem.h"

#include "utllib.h"
#include "defines.h"
#include "struct.h"
#include "transdef.h"
#include "appdata.h"
#include "fixdata.h"
#include "comm.h"
#include "hostutil.h"
#include "reqvisa1.h"


//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static void appendAMT( void );
static void reqVisa1Sale( void );

//=============================================================================
// Public data definitions
//=============================================================================
//! HDLC protocol flag
Bool HDLC_VISA;


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Formats Visa1 request packet
//!
//! \return
//!     orvINVDATA if Request Packet not built otherwise Comm Status of
//!     Request send
//!
//! \note
//!     MsgComFlag is updated.
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS ReqVisa1( void )
{
	Bool buildStatus = False;
	OS_RETURN_VALS CommStatus;	// Comm Status of Request send  

	// Reset Comm status 
	CommStatus = orvINVDATA;

	// Reset flag before reqVisa1Sale sets it for SaveRspText 
	HDLC_VISA = False;

	// Reset Request Packet built okay flag 
	MsgComFlag &= ~REQUEST_BUILT;

	// Yes; clear out Transmit Buffer 
	memset( SENDBUF, 0,sizeof( SENDBUF ) );

	// Set up the data pointer 
	pSendBuffer = SENDBUF;

	switch ( TRREQ.TRKEY )
	{
		case SALE:
		case AUTH:
		case SALCASH:
		case CRDVER:
			reqVisa1Sale(  );
			buildStatus = True;
			break;

		default:
			break;
	}							// end switch 

	if ( buildStatus )
	{
		// Okay; indicate that the Request was built okay 
		MsgComFlag |= REQUEST_BUILT;

		// Calculate the length 
		SendMsgLen = pSendBuffer - SENDBUF;

		// Send the Message Request to the Host 
		SendMessage();

		// Get the Comm status 
		CommStatus = HostOSRetval;
	}							// end if(buildStatus) 

	// Return the Comm status 
	return ( CommStatus );
}								// end of ReqVisa1 


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Builds a visanet, base i authorization message for sale and
//!     authorization only transactions
//!
//! \param
//!		[in]Amount			Pointer to Destination Buffer
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void reqVisa1Sale( void )
{
	// Add  HDLC TPDU if required 
	if ( AppendVisaTPDU(  ) )
	{
		HDLC_VISA = True;
	}

	// Put VISA I terminal ID in the send BUFFER 
	AppendVisaTID(  );

	// FS 
	AppendFS(  );

	//  WCC 
	AppendWrtCtrlChar(  );

	// ACCOUNT 
	if ( TRREQ.TRTRK2[0] )
	{
		// TRACK2 available 
		AppendPanTrack2(  );
	}
	else
	{
		AppendPanMan(  );
		// FS 
		AppendFS(  );

		// Experiation Date 
		AppendExpDate(  );
	}
	AppendFS(  );

	// AMOUNT 
	appendAMT(  );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Put transaction amount in send buffer for sale/auth only transactions
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void appendAMT( void )
{
	UBYTE *pDefAMT;

	memcpy( TRREQ.TRAUTHAMT, TRREQ.TRTOTAM, S_TRAUTHAMT );
	pDefAMT = TRREQ.TRTOTAM;

	if ( NullComp( ( char * ) TRREQ.TRTIP, S_TRTIP ) )
	{
		// TIP is empty move TRTOTAMI into TRAUTHAMT 
		memcpy( TRREQ.TRAUTHAMT, TRREQ.TRTOTAMI, S_TRAUTHAMT );
		pDefAMT = TRREQ.TRTOTAMI;
	}

	pSendBuffer += MovAmt( ( char * ) pSendBuffer, pDefAMT, S_TRTOTAM, 3, 3 );
}

