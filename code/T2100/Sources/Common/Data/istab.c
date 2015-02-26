
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


/* Local functions (Cross-Callable) */
void ISPUTBYTE ( void );
void CLRISREC ( void );

/*-----------------------------------------------------------------------
 * Name:           IsTabPre
 * Description:    Setup destination address
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void IsTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *) ISTAB, ISSZE, ISMAX );
}

/*------------------------------------------------------------------
 *  Name:           P60_LDIR_IsTab
 *  Description:    Move Data to a Local Buffer 
 *  Parameters:     None.
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/

void P60_LDIR_IsTab ( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

//-----------------------------------------------------------------------------
//  SetISTabLRCFuncName     Set the Checksum (LRC) for the Issuer Table
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

void SetISTabLRC( void )
{
	ISLRC = CalcLrc( (UBYTE *)&ISTAB, ISSZE*ISMAX );
}


/*-----------------------------------------------------------------------
 * Name:           ISPUTBYTE
 * Description:    Put one byte from issByte into address pointed to by gpIssPtr
 *
 * Parameters:     Global gpIssPtr, issByte
 * Return Value:   none
 * Notes:          No validity checking of any kind is performed
 *-----------------------------------------------------------------------*/
void ISPUTBYTE ( void )
{
	*(UBYTE *)gpIssPtr = issByte;
}

/*-----------------------------------------------------------------------
 * Name:           CLRISREC
 * Description:    Clear the ISTAB record pointed to by gpIssPtr
 *
 * Parameters:     Global gpIssPtr
 * Return Value:   none
 * Notes:          No validity checking of any kind is performed
 *-----------------------------------------------------------------------*/
void CLRISREC ( void )
{
	memset( (UBYTE *)gpIssPtr, 0, ISSZE);
}


/*-----------------------------------------------------------------------
 * Name:           MoveItIss
 * Description:    Moves data from ISTAB into destination
 *
 * Parameters:     same as for memcpy
 * Return Value:   none
 * Notes:          To be used instead of memcpy where the source is ISTAB data
 *-----------------------------------------------------------------------*/
void MoveItIss(  void * pDest, const void * pSource, UWORD  Bytes )
{
	GetSem(&IssSem);
	memcpy( pDest, pSource, Bytes );
    RelSem(&IssSem);
}
//-----------------------------------------------------------------------------
//  FindIssuer      Return Ptr to Issuer Table for This ID
//
//  Parameters:     UBYTE issuer_id         Issuer id to find.
//
//  Global Inputs:
//
//  Returns:        struct issuer_rec *     Issuer pointer or NULL if does
//                                          not exist.
//
//  Notes:
//
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

