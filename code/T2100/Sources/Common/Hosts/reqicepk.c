
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             reqicepk.c
//      Build an initialization request message.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "fixdata.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "transdef.h"
#include "comm.h"
#include "reqicepk.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static const char ReqHdrIP[] = {
	'\x60',							// TPDU ID                 
	'\x00', '\x03', '\x00', '\x00',	// Dest/Source             
	'\x08', '\x00',					// Initialization Msg Type 
	'\x20', '\x20', '\x01', '\x00',	// Bit Map                 
	'\x00', '\x80', '\x00', '\x10',
	'\x94', '\x00', '\x00'			// Processing Code         
};


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//	ReqIcePak		Build an initialization request message for 
//					IcePak graphic files.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		OS_RETURN_VALS			Comm Status of Request send.
//
//	Notes:			MsgComFlag is updated.
//
//-----------------------------------------------------------------------------

extern OS_RETURN_VALS ReqIcePak( void )
{
	UBYTE *pSendBuf;			// Pointer into transmit buffer 

	// Reset Request Packet built okay flag 
	MsgComFlag &= ~REQUEST_BUILT;

	// Clear out transmit buffer 
	memset( ( UBYTE * ) SENDBUF, 0,S_SENDBUF );

	// Set pointer into transmit buffer 
	pSendBuf = SENDBUF;

	// Init Request Header 
	memcpy( pSendBuf, ReqHdrIP, sizeof( ReqHdrIP ) );

	// Update NII in header so message will be routed properly.  
	memcpy( pSendBuf + 1, IPTAB.IPNII, sizeof( IPTAB.IPNII ) );

	// Update pointer into transmit buffer 
	pSendBuf += sizeof( ReqHdrIP );

	// Check if continuation of initialization 
	if ( TRREQ.TRAGAIN )
	{
		// Yes; indicate continuation of initialization 
		pSendBuf--;
		*pSendBuf = 0x01;
		pSendBuf++;
	}

	// System Trace Audit Number 
	memcpy( pSendBuf, TRREQ.TRSTAN, S_TRSTAN );

	// Update pointer into transmit buffer 
	pSendBuf += S_TRSTAN;

	// Network International Identifier for ICEPAK 
	memcpy( pSendBuf, IPTAB.IPNII, sizeof( IPTAB.IPNII ) );

	// Update pointer into transmit buffer 
	pSendBuf += sizeof( IPTAB.IPNII );

	// Terminal Serial Number 
	memcpy( pSendBuf, (UBYTE *)TERM.TERMSer, sizeof( TERM.TERMSer ) );

	// Update pointer into transmit buffer 
	pSendBuf += sizeof( TERM.TERMSer );

	pSendBuf++;

	// Length (BCD) of Screen Set ID 
	*pSendBuf++ = 0x12;

	// Screen Set Id 
	memcpy( pSendBuf, IPTAB.IPSID, sizeof( IPTAB.IPSID ) );

	// Update pointer into transmit buffer 
	pSendBuf += sizeof( IPTAB.IPSID );

	*pSendBuf++ = '0';

	// Indicate that the Request was built okay 
	MsgComFlag |= REQUEST_BUILT;

	// Calculate the send length 
	SendMsgLen = pSendBuf - SENDBUF;

	// Send the Transaction to the Line 
	SendMessage ();

	// Return the Comm status 
	return ( HostOSRetval );
}


//=============================================================================
// Private function definitions
//=============================================================================
