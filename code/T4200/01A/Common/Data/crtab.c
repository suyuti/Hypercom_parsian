//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             crtab.c
//      Card range table funtionality
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

#include "crtab.h"
#ifdef MAKE_OPTIMUM
#include "persistence.h"
#endif

//-----------------------------------------------------------------------------
//! \brief Setup Card Range table destination address.
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void CRTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *) CRTAB, CRSZE, CRMAX );
}

//-----------------------------------------------------------------------------
//! \brief Moves Data to a Local Buffer.
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void P60_LDIR_CRTab ( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

//-----------------------------------------------------------------------------
//! \brief Sets the Checksum (LRC) for the Card Range Table.
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void SetCRTabLRC( void )
{
	CRLRC = CalcLrc( (UBYTE *)&CRTAB, CRSZE*CRMAX );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&CRTAB );
#endif
}

//-----------------------------------------------------------------------------
//! \brief Finds the PAN (in TRINP.TRPAN) up in the card range table.
//!
//! Finds the PAN (in TRINP.TRPAN) up in the card range table. Also, do some
//! capability checking.
//!
//! \return
//!     Global CRByte: card range table index, if successful else 0xFF
//!
//! \note
//!     The card number as a parameter is in TRINP.TRPAN in BCD form
//-----------------------------------------------------------------------------
void FindCardRange( void )
{
   static struct card_rec * pCrPtr;
   static UBYTE i;
   UBYTE cardlen;
   UBYTE *pTMP;

   /* calculate length of PAN */
   cardlen = 0;
   pTMP = &(TRINP.TRPAN[0]);           
   while( cardlen <= 19 )
   {
       // do upper nibble 
       if (0xF0 == (*pTMP & 0xF0))
           break;
       cardlen++;

       // do lower nibble
       if (0x0F == (*pTMP & 0x0F))
           break;
       cardlen++;

       pTMP++;
   }
   
   i = 0;			// init counter
   CRByte = 0xFF;	/* FAILURE */
   /* look through the card range table */
   for (pCrPtr = CRTAB; pCrPtr < &CRTAB[CRMAX]; pCrPtr++, i++)
   {
      /* if length ok, check range                                 */
      /* Added code to convert bcd length to binary  */
      if( (pCrPtr->CRPLEN == 0) ||               /* variable length, or */
         (CvtBin(pCrPtr->CRPLEN) == cardlen) )  /* length matches */
      {
         /* is the PAN within this range? */
         if( (memcmp(TRINP.TRPAN, pCrPtr->CRPANL, S_CRPANL) >= 0) &&
             (memcmp(TRINP.TRPAN, pCrPtr->CRPANH, S_CRPANH) <= 0) )
         {
            /* must be OK */
		   CRByte = i;
           break;
         }  /* range match */
      }  /* length match */
   }  /* loop */

}

//-----------------------------------------------------------------------------
//! \brief Finds Card Range table entry by Id.
//!
//! \return
//!     Global gpCRPtr sets to CRTAB[CRByte-1] if valid, NULL if invalid
//!
//! \note
//!     Global CRByte contains Card Range Number (1 to CRMAX) as input
//!     paramemter
//-----------------------------------------------------------------------------
void FindCardRangeById ( void )
{
	if( (CRByte == 0) || (CRByte > CRMAX) )
	{
		gpCRPtr = NULL;
    }
	else
	{
		gpCRPtr = &CRTAB[CRByte - 1];
	}
}

//-----------------------------------------------------------------------------
//! \brief Moves data from CRTAB into destination
//!
//! \param[out] pDest           Pointer to the destination memory
//! \param[in] pSource          Pointer to the source memory
//! \param[in] Bytes            Number of bytes to be copied
//!                             
//! \return
//!     None.
//!                             
//! \note
//!     To be used instead of memcpy where the source is CRTAB data.
//-----------------------------------------------------------------------------
void MoveItCR(  void * pDest, const void * pSource, UWORD  Bytes )
{
	GetSem(&CRSem);
	memcpy( pDest, pSource, Bytes );
    RelSem(&CRSem);
}

//-----------------------------------------------------------------------------
//! \brief Finds Card Range table entry by TRINP.TRPAN
//! 
//! \return
//!     Card range Number (CRNUM) or False if not found.
//!                             
//! \note
//!     TRINP.TRPAN is used to find the corresponding Card Range entry.
//-----------------------------------------------------------------------------
UBYTE FindCR ( void )
{
	UBYTE	num;

	/* Try to find the card range */
	/* Find card range given the PAN in TRINP.TRPAN (result in global CRByte) */
	GetSem(&CRSem);
	FindCardRange ();
	num = CRByte;			// Get the result
	RelSem(&CRSem);

	if (num == 0xFF)
	{
		/* Card range not found */
		return( False );
	}
	else
	{
		/* return card range id */
		return( CRTAB[num].CRNUM );
	}
} 

//-----------------------------------------------------------------------------
//! \brief Finds the Card Range Entry given the Id (CRNUM)
//!
//! \param[in] id               Card Range Number
//!                             
//! \return
//!     Pointer to Card Range Entry or NULL if not found.
//-----------------------------------------------------------------------------
struct card_rec * FindCR_ById ( UBYTE id )
{
	struct card_rec * ptr;

   	/* Locate card range table */
	GetSem(&CRSem);

#ifndef MAKE_SMARTINIT
	CRByte = CvtBin( id );			// Put BCD Card Range Id into global
#else // MAKE_SMARTINIT
	CRByte = id;					// Put BIN Card Range Id into global
#endif // MAKE_SMARTINIT

    FindCardRangeById ();	// gpCRPtr is not NULL if found
	ptr = (struct card_rec *) gpCRPtr;
	RelSem(&CRSem);

	return( ptr );
}

