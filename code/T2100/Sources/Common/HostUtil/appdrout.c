
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             appdrout.c
//      Appends Check Routing to send data

//=============================================================================
//=============================================================================
#include <string.h>
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
//  AppendRoutingTC     Appends Check Routing to send data - TELECHECK
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:              Special TeleCheck code substitutes the constant
//                      string "X.A01000" for the routeing info.
//
//-----------------------------------------------------------------------------

extern void AppendRoutingTC( void )
{
	// Append the Check Routing to the send data
	memcpy( pSendBuffer, "X.A01000", 8 );

	// Increment the send data pointer
	pSendBuffer += 8;
}

//=============================================================================
// Private function definitions
//=============================================================================
