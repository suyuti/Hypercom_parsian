
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             battab.c
//      Batch Table functionality
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "comdata.h"

#include "battab.h"

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
//  SetBATTabLRC        Set the Checksum (LRC) for the Batch Table
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:              Assumes LRC byte immediately follows table
//
//-----------------------------------------------------------------------------

extern void SetBATTabLRC( void )
{
	BATLRC = CalcLrc( ( UBYTE * ) & BATTAB, ( UWORD ) sizeof( BATTAB ) );
}
//-----------------------------------------------------------------------------
//  FindBatTab      Return Pointer to the batch table for aqid.
//                  If entry does not exist for this acquirer 
//                  then create it now.

//  Parameters:     UBYTE aqid      Acquired id. 
//
//  Global Inputs:
//
//  Returns:        struct batch_rec *  Pointer to batch table entry.
//
//  Notes:
//
//-----------------------------------------------------------------------------
struct batch_rec * FindBatTab( UBYTE aqid )
{
    static struct batch_rec * pBatPtr;

	for(pBatPtr=BATTAB; pBatPtr < &BATTAB[AQMAX]; pBatPtr++)
	{
		if( pBatPtr->BATFLAG & BT_USED )
		{
			if( pBatPtr->BATAQID == aqid )
			{
				return( pBatPtr );
			}
  		}
	}

	/* Batch table entry not found for requested acquirer - Make one */
	/* Find first unused batch table entry and use it.  If none		 */
	/* available then reuse the first entry.						 */
	for( pBatPtr = BATTAB; pBatPtr < &BATTAB[AQMAX]; pBatPtr++ )
	{
		if( !(pBatPtr->BATFLAG & BT_USED ))
		{
			break;			
		}
	}

	if( pBatPtr >= &BATTAB[AQMAX])
	{
		/* Entry was not available so reuse the first entry */
		pBatPtr = BATTAB;
	}

	/* Format the entry. */
	memset( (UBYTE *) pBatPtr, 0, BATSZE);  /* Clear this entry */
	pBatPtr->BATFLAG = BT_USED;  /* Mark entry as used */
	pBatPtr->BATCUR[2] = 0x01;   /* Default current to 000001 */
	pBatPtr->BATNXT[2] = 0x02;   /* Default next to 000002 */
	pBatPtr->BATAQID = aqid;     /* Set the acquirer ID  */

	/* Recalculate the LRC on the batch table */
	SetBATTabLRC(); 
	
	return( pBatPtr );
}

//-----------------------------------------------------------------------------
//  DelayAutoSettle     Don't autosettle until idle timer expires.
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:
//
//-----------------------------------------------------------------------------

void DelayAutoSettle( void )
{
	UBYTE i;

	for ( i = 0; i < AQMAX; i++ )
	{
		if ( BATTAB[i].BATFLAG & BT_USED )
		{
			BATTAB[i].BATSETLIDL = 5;
		}
	}
	// Recalculate the LRC on the batch table
	SetBATTabLRC(  );
}

//-----------------------------------------------------------------------------
//  SetBatchNum     Set Batch number after initialization.
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:
//
//-----------------------------------------------------------------------------

void SetBatchNum( void )
{
	struct batch_rec *pBatchRec;
	UBYTE i;

	for ( i = 0; i < AQMAX; i++ )
	{
		if ( !NullComp(AQTAB[i].AQBATCUR,S_AQBATCUR))
		{
			pBatchRec = FindBatTab(AQTAB[i].AQID);
                        memcpy(pBatchRec->BATCUR,AQTAB[i].AQBATCUR,S_AQBATCUR);
		}
		if ( !NullComp(AQTAB[i].AQBATNXT,S_AQBATNXT)  )
		{
			pBatchRec = FindBatTab(AQTAB[i].AQID);
                        memcpy(pBatchRec->BATNXT,AQTAB[i].AQBATNXT,S_AQBATNXT);
		}
	}
	// Recalculate the LRC on the batch table
	SetBATTabLRC(  );
}
