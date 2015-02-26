//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             aqtab.h
//      Acquirer table functionality
//
//=============================================================================
#include "clib.h"

#include "basictyp.h"
#include "sdkos.h"
#include "sdk.h"
#include "sdkmem.h"
#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "utllib.h"
#include "tabutil.h"
#include "aqtab.h"
#ifdef MAKE_OPTIMUM
#include "persistence.h"
#endif


/* Local functions (static) */
static struct acquirer_rec * pAcqPtr;


//-----------------------------------------------------------------------------
//! \brief Setup destination address
//!                             
//! \return
//!     None.
//-----------------------------------------------------------------------------
void AqTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *) &AQTAB, AQSZE, AQMAX );
}

//-----------------------------------------------------------------------------
//! \brief Moves Data to a Local Buffer
//!                             
//! \return
//!     None.
//-----------------------------------------------------------------------------
void P60_LDIR_AqTab ( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

//-----------------------------------------------------------------------------
//! \brief Sets the checksum (LRC) for the Acquirer Table
//!                             
//! \return
//!     None.
//-----------------------------------------------------------------------------
void SetAQTabLRC( void )
{
	AQLRC = CalcLrc( (UBYTE *)&AQTAB, AQSZE*AQMAX );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&AQTAB );
#endif
}


//-----------------------------------------------------------------------------
//! \brief Moves data from AQTAB into destination
//!
//! \param[out] pDest           Pointer to the destination memory
//! \param[in] pSource          Pointer to the source memory
//! \param[in] Bytes            Number of bytes to be copied
//!                             
//! \return
//!     None.
//!                             
//! \note
//!     To be used instead of memcpy where the source is AQTAB data.
//-----------------------------------------------------------------------------
void MoveItAq(  void * pDest, const void * pSource, UWORD Bytes )
{
	GetSem(&AqSem);

    memcpy( pDest, pSource, Bytes );
    
    RelSem(&AqSem);
}

//-----------------------------------------------------------------------------
//! \brief Moves data into AQTAB
//!
//! \param[out] pDest           Pointer to the destination memory
//! \param[in] pSource          Pointer to the source memory
//! \param[in] Bytes            Number of bytes to be copied
//!                             
//! \return
//!     None.
//!                             
//! \note
//!     To be used instead of memcpy where the destination is AQTAB data.
//!     The function changes the content of acquirer table, but doesn't
//!     recalculate the LRC. It's the responsibility of the programmer to call
//!     SetAQTabLRC after calling this function.
//-----------------------------------------------------------------------------
void MoveIt2Aq(  void * pDest, const void * pSource, UWORD  Bytes )
{
	GetSem(&AqSem);

    memcpy( pDest, pSource, Bytes );
    
    RelSem(&AqSem);
}

//-----------------------------------------------------------------------------
//! \brief Returns Ptr to Acq Table for specified ID
//!
//! \param[in] acquirer_id      Acquirer id to find
//!                             
//! \return
//!     Pointer to acquirer or NULL if it does not exist.
//-----------------------------------------------------------------------------
extern struct acquirer_rec *FindAcquirer( UBYTE acquirer_id )
{
	struct acquirer_rec * aqoffsmax;

	if ( acquirer_id )
	{
		// Valid Acquirer Id, perform the search
		aqoffsmax = &AQTAB[AQMAX];

		for (pAcqPtr = &AQTAB[0]; pAcqPtr < aqoffsmax; pAcqPtr++)
		{
			if ( pAcqPtr->AQID == acquirer_id )
			{
				return ( pAcqPtr );
			}
		}
	}	// if ( acquirer_id )

	// Invalid Acquirer Id or not found
    return ((struct acquirer_rec *)NULL);
}

//-----------------------------------------------------------------------------
//! \brief Find the first acquirer
//!                             
//! \return
//!     Pointer to first acquirer in tables or NULL if it does not exist.
//-----------------------------------------------------------------------------
extern struct acquirer_rec *FindFirstAcquirer( void )
{
	struct acquirer_rec * aqoffsmax;

	aqoffsmax = &AQTAB[AQMAX];

	/* Search acquirer table for the first entry. */
	for (pAcqPtr=&AQTAB[0]; pAcqPtr < aqoffsmax; pAcqPtr++)
	{ 
		/* Look at every entry - if ID then entry is used. */
		if ( pAcqPtr -> AQID )
		{
			return( pAcqPtr );
		}
	}
	return(NULL);
}

//-----------------------------------------------------------------------------
//! \brief Finds the next acquirer
//!
//! \param[in] pCurrentAcquirer Pointer to current acquirer
//!                             
//! \return
//!     Pointer to next acquirer in tables or NULL if no more acquirers.
//-----------------------------------------------------------------------------
extern struct acquirer_rec *FindNextAcquirer( struct acquirer_rec *pCurrentAcquirer )
{
	struct acquirer_rec * aqoffsmax;

	aqoffsmax = &AQTAB[AQMAX];

	/* Search acquirer table for the next entry. */
	for (pAcqPtr=pCurrentAcquirer + 1; pAcqPtr < aqoffsmax; pAcqPtr++)
	{ 
		/* Look at every entry - if ID then entry is used. */
		if ( pAcqPtr -> AQID )
		{
			return( pAcqPtr );
		}
	}
	return(NULL);
}


//-----------------------------------------------------------------------------
//! \brief Finds the previous acquirer
//!
//! \param[in] pCurrentAcquirer Pointer to current acquirer
//!                             
//! \return
//!     Pointer to previous acquirer in tables or NULL if no more acquirers.
//-----------------------------------------------------------------------------
struct acquirer_rec *FindPrevAcquirer( struct acquirer_rec *pCurrentAcquirer )
{
	struct acquirer_rec * aqoffsmin;

	aqoffsmin = &AQTAB[0];

	/* Search acquirer table for the previous entry. */
	for (pAcqPtr=pCurrentAcquirer - 1; pAcqPtr >= aqoffsmin; pAcqPtr--)
	{ 
		/* Look at every entry - if ID then entry is used. */
		if ( pAcqPtr -> AQID )
		{
			return( pAcqPtr );
		}
	}
	return(NULL);
}

//-----------------------------------------------------------------------------
//! \brief Gets the number of acquirers
//!                             
//! \return
//!     The number of acquirers.
//-----------------------------------------------------------------------------
extern char AqidCount( void )
{
	char count;
	struct acquirer_rec *pAcquirerPtr;	// Pointer to AQTAB entry  

	// Count the number of acquirers.  Preset count to zero. 
	count = 0;
	pAcquirerPtr = FindFirstAcquirer(  );
	if ( NULL != pAcquirerPtr )
	{
		// Have at least one acquirer. 
		count = 1;

		// Count the rest of the acquirers if they exist. 
		while ( True )
		{
			pAcquirerPtr = FindNextAcquirer( pAcquirerPtr );
			if ( NULL == pAcquirerPtr )
				break;

			count++;
		}
	}
	return ( count );
}

