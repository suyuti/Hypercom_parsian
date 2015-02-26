//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             pdntab.c
//      PDN table functionality
//
//=============================================================================
#include "clib.h"

#include "basictyp.h"
#include "sdkos.h"
#include "sdk.h"
#include "sdkmem.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "tabutil.h"

#include "pdntab.h"
#ifdef MAKE_OPTIMUM
#include "persistence.h"
#endif

#ifdef MAKE_EXTRA_TABLES
//-----------------------------------------------------------------------------
//! \brief Setup PDN table destination address
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void PDNTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *) PDNTAB, PDNSZE, PDNMAX );
}

//-----------------------------------------------------------------------------
//! \brief Moves Data to a Local Buffer
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void P60_LDIR_PDNTab ( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

//-----------------------------------------------------------------------------
//! \brief Sets the Checksum (LRC) for the PDN Table
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void SetPDNTabLRC( void )
{
	PDNLRC = CalcLrc( (UBYTE *)&PDNTAB, PDNSZE*PDNMAX );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&PDNTAB );
#endif
}

//-----------------------------------------------------------------------------
//! \brief Finds the PDN Table Entry by the given Acquirer ID
//!
//! \param[in] acquirer_id      Acquirer id to find
//!                             
//! \return
//!     Pointer to PDN Entry or NULL if it does not exist.
//-----------------------------------------------------------------------------
PDNSTRUCT * FindPDN(UBYTE acquirer_id)
{
  static PDNSTRUCT * pPDNPtr;
  
  for (pPDNPtr=PDNTAB; pPDNPtr < &PDNTAB[PDNMAX]; pPDNPtr++)
  {
     if ( pPDNPtr->PDNAQID == acquirer_id )
     {
        return ( pPDNPtr );
     }
  }
    return ((PDNSTRUCT *)NULL);
}
#endif // MAKE_EXTRA_TABLES

