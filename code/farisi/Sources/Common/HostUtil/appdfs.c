//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             appdfs.c
//      Appends Field Separator (0x1c) to send data
//
//=============================================================================
#include "basictyp.h"

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
//! \brief
//!     Appends Field Separator (0x1c) to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendFS( void )
{
	// Append a Field Separator to the send data
	*pSendBuffer = IFS;

	// Increment the send data pointer
	pSendBuffer++;
}

//=============================================================================
// Private function definitions
//=============================================================================

