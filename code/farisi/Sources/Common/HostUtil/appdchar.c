//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             appdchar.c
//      Appends a specified character to send data
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
//!     Appends specified character to send data
//!
//! \param
//!		[in]Char			�haracter to append to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendChar( char Char )
{
	// Append the specified character to the send data
	*pSendBuffer = Char;

	// Increment the send data pointer
	pSendBuffer++;
}

//=============================================================================
// Private function definitions
//=============================================================================

