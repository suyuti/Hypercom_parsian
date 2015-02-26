//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             aptab.c
//      Additional Prompts table functionality
//
//=============================================================================
#include "clib.h"

#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "comdata.h"

#include "aptab.h"
#ifdef MAKE_OPTIMUM
#include "persistence.h"
#endif


//-----------------------------------------------------------------------------
//! \brief Sets the Checksum (LRC) for the Additional prompt table
//!
//! \param[in] image            Pointer to the graphics image or filename
//!                             
//! \return
//!     None.
//!                             
//! \note
//!     Upon booting up, if checksum is wrong, application will report
//!     error and will not run.
//-----------------------------------------------------------------------------
extern void SetAPTabLRC( void )
{
	APLRC = CalcLrc( ( UBYTE * ) & APTAB, APSZE * APMAXENT );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&APTAB );
#endif
}

