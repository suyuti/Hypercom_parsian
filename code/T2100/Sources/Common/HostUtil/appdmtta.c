

//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             appdmtta.c
//      Appends Transit and Account Numbers with a trailing "T" & "A" 
//      to the send data

//=============================================================================
//=============================================================================
#include "basictyp.h"

#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "hostutil.h"


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


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  AppendMICRTrailTA       Appends Transit and Account Numbers with a
//                          trailing "T" and "A" to send data
//
//  Parameters:             None.
//
//  Global Inputs:          None.
//
//  Returns:                Nothing.
//
//  Notes:
//                          Only call this if (TRCHECKF[0] & 0x58) == 0x50 
//                          or 0x48, making TRMICRfmt == 2
//
//-----------------------------------------------------------------------------

extern void AppendMICRTrailTA( void )
{
	// Append the Transit Number to the send data
	pSendBuffer += MoveTillZero(
			( char * ) pSendBuffer,
			( char * ) TRREQ.micr.f2.TRTRANSIT, 9 );

	// Append Routing Transit Indicator to the send data
	AppendChar( 'T' );

	// Append the Account Number to the send data 
	pSendBuffer += MoveTillZero(
			( char * ) pSendBuffer,
			( char * ) TRREQ.micr.f2.TRChkAcct, 18 );

	// Append Account Number Indicator to the send data
	AppendChar( 'A' );
}

//=============================================================================
// Private function definitions
//=============================================================================
