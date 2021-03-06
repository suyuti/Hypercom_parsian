//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             tftab.c
//      Terminal Flags Table functionality.
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "comdata.h"

#include "tftab.h"
#ifdef MAKE_OPTIMUM
#include "persistence.h"
#endif

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
//!     Sets the Checksum (LRC) for the TF Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void SetTFTabLRC( void )
{
	TFLRC = CalcLrc( ( UBYTE * ) & TFTAB, ( UWORD ) sizeof( TFTAB ) );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&TFTAB );
#endif
}

//=============================================================================
// Private function definitions
//=============================================================================

