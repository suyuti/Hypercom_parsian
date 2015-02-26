
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             appdedat.c
//      Appends Expiration Date to send data (Ascii MMYY)

//=============================================================================
//=============================================================================
#include "basictyp.h"

#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"


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
//  AppendExpDate       Appends Expiration Date to send data (Ascii MMYY)
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendExpDate( void )
{
	// Append the Expiration Date Month in Ascii to the send data
	BufAsc( ( char * ) pSendBuffer, &TRREQ.TREXPD[1] );

	// Increment the send data pointer
	pSendBuffer += 2;

	// Append the Expiration Date Year in Ascii to the send data
	BufAsc( ( char * ) pSendBuffer, TRREQ.TREXPD );

	// Increment the send data pointer
	pSendBuffer += 2;
}

//=============================================================================
// Private function definitions
//=============================================================================
