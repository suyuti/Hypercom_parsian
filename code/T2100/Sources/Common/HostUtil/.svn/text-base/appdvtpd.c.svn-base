
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             appdvtpd.c
//      Appends a Visa TPDU if configured for HDLC to send data

//=============================================================================
//=============================================================================
#include <string.h>
#include "basictyp.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "fixdata.h"


//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================

#define S_VisaTPDU		5		// Length of Visa TPDU


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

// Visa TPDU w/blank NII
static const UBYTE VisaTPDU[S_VisaTPDU] = { 0x60, 0x40, 0x00, 0x00, 0x00 };


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  AppendVisaTPDU      Appends a Visa TPDU if configured for HDLC to
//                      send data
//
//  Parameters:         None.
//
//  Global Inputs:
//
//  Returns:            Bool    True - Visa TPDU appended
//                              False - Visa TPDU not appended.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool AppendVisaTPDU( void )
{
	// See if using Dial Mode
	if ( cl_DIALUP == TERM.TERMLine )
	{
		// Yes; see if Modem Mode indicates SDLC
		switch ( TRREQ.TRAQPTR->conn.dial.aqModem )
		{
			case 0:			// 212A 1200 SDLC
			case 2:			// V.22 1200 SDLC
			case 6:			// V.22 2400 SDLC
			case 8:			// V.22 2400 SDLC (Bell answer)

				// Yes
				break;

			default:			// No; return false to caller
				return ( False );
		}
	}

	// Append Visa TPDU to the send data
	memcpy( pSendBuffer, VisaTPDU, S_VisaTPDU );

	// Or High digit of NII with Visa Code "4X"
	pSendBuffer[1] |= TRREQ.TRAQPTR->AQNII[0];

	// Add Low Digits of NII
	pSendBuffer[2] = TRREQ.TRAQPTR->AQNII[1];

	// Increment the send data pointer
	pSendBuffer += S_VisaTPDU;

	// Return true to caller
	return ( True );
}


//=============================================================================
// Private function definitions
//=============================================================================
