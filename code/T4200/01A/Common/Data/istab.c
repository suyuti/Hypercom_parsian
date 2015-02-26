//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             istab.c
//      Issuer table functionality
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
#include "istab.h"
#include "tabutil.h"
#ifdef MAKE_OPTIMUM
#include "persistence.h"
#endif


/* Local functions (Cross-Callable) */
void ISPUTBYTE ( void );
void CLRISREC ( void );

//-----------------------------------------------------------------------------
//! \brief Setup destination address
//!                             
//! \return
//!     None.
//-----------------------------------------------------------------------------
void IsTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *) ISTAB, ISSZE, ISMAX );
}

//-----------------------------------------------------------------------------
//! \brief Moves Data to a Local Buffer
//!                             
//! \return
//!     None.
//-----------------------------------------------------------------------------
void P60_LDIR_IsTab ( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

//-----------------------------------------------------------------------------
//! \brief Sets the Checksum (LRC) for the Issuer Table
//!                             
//! \return
//!     None.
//-----------------------------------------------------------------------------
void SetISTabLRC( void )
{
	ISLRC = CalcLrc( (UBYTE *)&ISTAB, ISSZE*ISMAX );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&ISTAB );
#endif
}


//-----------------------------------------------------------------------------
//! \brief Puts one byte from issByte into address pointed to by gpIssPtr
//!                             
//! \param[out] gpIssPtr        Pointer to the issuer
//! \param[in] issByte          The issuer number to be copied
//!                             
//! \return
//!     None.
//!                             
//! \note
//!     No validity checking of any kind is performed.
//-----------------------------------------------------------------------------
void ISPUTBYTE ( void )
{
	*(UBYTE *)gpIssPtr = issByte;
}

//-----------------------------------------------------------------------------
//! \brief Clears the ISTAB record pointed to by gpIssPtr
//!                             
//! \param[out] gpIssPtr        Pointer to the issuer
//!                             
//! \return
//!     None.
//!                             
//! \note
//!     No validity checking of any kind is performed.
//-----------------------------------------------------------------------------
void CLRISREC ( void )
{
	memset( (UBYTE *)gpIssPtr, 0, ISSZE);
}


//-----------------------------------------------------------------------------
//! \brief Moves data from ISTAB into destination
//!
//! \param[out] pDest           Pointer to the destination memory
//! \param[in] pSource          Pointer to the source memory
//! \param[in] Bytes            Number of bytes to be copied
//!                             
//! \return
//!     None.
//!                             
//! \note
//!     To be used instead of memcpy where the source is ISTAB data.
//-----------------------------------------------------------------------------
void MoveItIss(  void * pDest, const void * pSource, UWORD  Bytes )
{
	GetSem(&IssSem);
	memcpy( pDest, pSource, Bytes );
    RelSem(&IssSem);
}

//-----------------------------------------------------------------------------
//! \brief Finds the Issuer Entry for specified ID
//!
//! \param[in] issuer_id        Issuer id to find
//!                             
//! \return
//!     Pointer to Issuer Entry or NULL if does not exist.
//-----------------------------------------------------------------------------
extern struct issuer_rec *FindIssuer( UBYTE issuer_id )
{
	static struct issuer_rec * pIssPtr;
	struct issuer_rec * issoffsmax;

	issoffsmax = &ISTAB[ISMAX];

	for (pIssPtr=&ISTAB[0]; pIssPtr < issoffsmax; pIssPtr++)
	{
		if ((pIssPtr->ISID) == issuer_id)
		{
			return( pIssPtr ); 
		}
	} 
    return ((struct issuer_rec *)NULL);
}

