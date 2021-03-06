//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             tconf.c
//      Terminal Config Table functionality
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "comdata.h"

#include "tconf.h"
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
//! \brief Sets the Checksum (LRC) for the Terminal Configuration Table
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void SetTCTabLRC( void )
{
	TCLRC = CalcLrc( ( UBYTE * ) & TCONF, TCSZE );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&TCONF );
#endif
}


//=============================================================================
// Private function definitions
//=============================================================================

