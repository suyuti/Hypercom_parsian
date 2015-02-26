
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

/*-----------------------------------------------------------------------
 * Name:           CRTabPre
 * Description:    Setup destination address
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void CRTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *) CRTAB, CRSZE, CRMAX );
}

/*------------------------------------------------------------------
 *  Name:           P60_LDIR_CRTab
 *  Description:    Move Data to a Local Buffer 
 *  Parameters:     None.
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/

void P60_LDIR_CRTab ( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

//-----------------------------------------------------------------------------
//  SetCRTabLRC     Set the Checksum (LRC) for the Card Range Table
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

void SetCRTabLRC( void )
{
	CRLRC = CalcLrc( (UBYTE *)&CRTAB, CRSZE*CRMAX );
}

/*-----------------------------------------------------------------------
 * Name:           FindCardRange
 * Description:    Look the PAN (in TRINP.TRPAN) up in the card range table
 *                 Also, do some capability checking
 * Parameters:     None. The card number is in TRINP.TRPAN in BCD form
 * Return Value:   Global CRByte = card range table index, if successful,
 *                 CRByte = 0xFF if unsuccessful
 * Notes:          None.
 *-----------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------
 * Name:           FindCardRangeById
 * Description:    Find Card Range table entry by Id
 *
 * Parameters:     Global CRByte contains Card Range Number (1 to CRMAX)
 * Return Value:   gpCRPtr set to CRTAB[CRByte-1] if valid, NULL if invalid
 * Notes:          
 *-----------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------
 * Name:           memcpyCR
 * Description:    Moves data from CRTAB into destination
 *
 * Parameters:     same as for memcpy
 * Return Value:   none
 * Notes:          To be used instead of memcpy where the source is CRTAB data
 *-----------------------------------------------------------------------*/
void MoveItCR(  void * pDest, const void * pSource, UWORD  Bytes )
{
	GetSem(&CRSem);
	memcpy( pDest, pSource, Bytes );
    RelSem(&CRSem);
}
/*------------------------------------------------------------------
 *  Name:         FindCR
 *  Description:  
 *  Parameters:   TRINP.TRPAN is used to find the corresponding Card Range entry.
 *  Return Value: Card range Number (CRNUM) or False if not found
 *  Notes:        
 *-----------------------------------------------------------------*/
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

/*------------------------------------------------------------------
 *  Name:         FindCRById
 *  Description:  Finds the Card Range Entry given the Id (CRNUM)
 *  Parameters:   Card Range Number (Id)
 *  Return Value: Pointer to Card Range Entry or NULL if not found
 *  Notes:        
 *-----------------------------------------------------------------*/
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

