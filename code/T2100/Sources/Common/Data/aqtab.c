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


/* Local functions (static) */
static struct acquirer_rec * pAcqPtr;



/*-----------------------------------------------------------------------
 * Name:           AqTabPre
 * Description:    Setup destination address
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void AqTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *) &AQTAB, AQSZE, AQMAX );
}

/*------------------------------------------------------------------
 *  Name:           P60_LDIR_AqTab
 *  Description:    Move Data to a Local Buffer 
 *  Parameters:     None.
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/

void P60_LDIR_AqTab ( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

//-----------------------------------------------------------------------------
//  SetAQTabLRC     Set the checksum (LRC) for the Acquirer Table
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//  Notes:
//
//-----------------------------------------------------------------------------

void SetAQTabLRC( void )
{
	AQLRC = CalcLrc( (UBYTE *)&AQTAB, AQSZE*AQMAX );
}


/*-----------------------------------------------------------------------
 * Name:           MoveItAq
 * Description:    Moves data from AQTAB into destination
 *
 * Parameters:     same as for memcpy
 * Return Value:   none
 * Notes:          To be used instead of memcpy where the source is AQTAB data
 *-----------------------------------------------------------------------*/
void MoveItAq(  void * pDest, const void * pSource, UWORD  Bytes )
{
	GetSem(&AqSem);

    memcpy( pDest, pSource, Bytes );
    
    RelSem(&AqSem);
}
/*-----------------------------------------------------------------------
 * Name:           memcpy2Aq
 * Description:    Moves data into AQTAB
 *
 * Parameters:     same as for generic memcpy
 * Return Value:   none
 * Notes:          To be used instead of memcpy where the destination is AQTAB data
 * NOTE: The function changes the content of acquirer table, but doesn't recalculate
 *       the LRC. It's the responsibility of the programmer to call SetAQTabLRC after
 *		 calling this function.
 *-----------------------------------------------------------------------*/
void MoveIt2Aq(  void * pDest, const void * pSource, UWORD  Bytes )
{
	GetSem(&AqSem);

    memcpy( pDest, pSource, Bytes );
    
    RelSem(&AqSem);
}

//-----------------------------------------------------------------------------
//  FindAcquirer    Return Ptr to Acq Table for This ID
//
//  Parameters:     UBYTE acquirer_id       Acquirer id to find.
//
//  Global Inputs:
//
//  Returns:        struct acquirer_rec *   Pointer to acquirer or NULL 
//                                          if it does not exist. 
//
//  Notes:
//
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
//  FindFirstAcquirer   Find the first acquirer.
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:        struct acquirer_rec *   Pointer to first acquirer in tables
//                                          or NULL if it does not exist. 
//
//  Notes:
//
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
//  FindNextAcquirer    Find the next acquirer.
//
//  Parameters:         struct acquirer_rec *pCurrentAcquirer 
//
//                                              Pointer to current acquirer.
//
//  Global Inputs:
//
//  Returns:            struct acquirer_rec *   Returns pointer to next acquirer
//                                              in tables.
//                                              Returns NULL if no more acquirers.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern struct acquirer_rec *FindNextAcquirer( struct acquirer_rec
											  *pCurrentAcquirer )
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
//  FindPrevAcquirer    Find the previous acquirer.
//
//  Parameters:         struct acquirer_rec *pCurrentAcquirer 
//
//                                              Pointer to current acquirer.
//
//  Global Inputs:
//
//  Returns:            struct acquirer_rec *   Returns pointer to previous 
//                                              acquirer in tables.
//                                              Returns NULL if no more acquirers.
//
//  Notes:
//
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
//  AqidCount       Count the number of acquirers
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        char    The number of acquirers
//
//  Notes:
//
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



