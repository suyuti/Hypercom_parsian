
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             tabutil.c
//      Table formatting functions
//
//=============================================================================

#include <string.h>
#include "basictyp.h"
#include "utllib.h"


#include "struct.h"
#include "appdata.h"
#include "TERMtab.h"

#include "tabutil.h"

/*------------------------------------------------------------------
 *  Name:           ValidateTable
 *  Description:    Check validity of Tables
 *  Parameters:     pTabFMT
 *                  pTabData
 *                  len
 *                  pLRC
 *  Return Value:   True if ok, False if bad
 *  Notes:		    If bad, clear Terminal Initialization Done Flag
 *-----------------------------------------------------------------*/

UBYTE ValidateTable( UBYTE *pTabFMT, UBYTE *pTabData, UWORD len,
                            UBYTE *pTabLRC )
{
  	UBYTE oldlrc;
  	UBYTE newlrc;

  	/* Check if Table has been formatted */
  	if ( *pTabFMT == 0x55 )
  	{
       	/* Yes; get present LRC */
       	oldlrc = *pTabLRC;
       	
       	/* Calculate LRC */
       	newlrc = CalcLrc( pTabData, len );

       	/* Check if LRCs are the same */
       	if ( newlrc == oldlrc )
		{
			/* Yes; return okay */
           	return( True );
		}
  	}

  	/* Table is bad, so fix it */
  	InitTable( pTabFMT, pTabData, len, pTabLRC );

    /* Clear Terminal Initialization Done Flag */
    Clr_TermInit( );

  	/* Table has not been initialized */
  	return( False );
}

/*------------------------------------------------------------------
 *  Name:           ValidateTable_NC
 *  Description:    Check validity of Tables
 *  Parameters:     pTabFMT
 *                  pTabData
 *                  len
 *                  pLRC
 *  Return Value:   True if ok, False if bad
 *  Notes:		    If bad, DO NOT clear Terminal Initialization Done Flag
 *-----------------------------------------------------------------*/

UBYTE ValidateTable_NC( UBYTE *pTabFMT, UBYTE *pTabData, UWORD len,
                            UBYTE *pTabLRC )
{
  	UBYTE oldlrc;
  	UBYTE newlrc;

  	/* Check if Table has been formatted */
  	if ( *pTabFMT == 0x55 )
  	{
       	/* Yes; get present LRC */
       	oldlrc = *pTabLRC;
       	
       	/* Calculate LRC */
       	newlrc = CalcLrc( pTabData, len );

       	/* Check if LRCs are the same */
       	if ( newlrc == oldlrc )
		{
			/* Yes; return okay */
           	return( True );
		}
  	}

  	/* Table is bad, so fix it */
  	InitTable( pTabFMT, pTabData, len, pTabLRC );

  	/* Table has not been initialized */
  	return( False );
}

//-----------------------------------------------------------------------------
//  InitTable       Initialize the Table indicated 
//
//  Parameters:     UBYTE * pFrmFlag    pointer to Format Flag
//
//                  UBYTE * pTab        pointer to Table
//
//                  UWORD len           size of Table
//
//                  UWORD *pLRC         pointer to Table LRC
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

void InitTable( UBYTE * pFrmFlag, UBYTE * pTab, UWORD len,
					   UBYTE * pLRC )
{
	// Set Format Flag to 0x55 -- Formatted
	*pFrmFlag = 0x55;

	// Clear the Table
	memset( pTab, 0, len );

	// Reset the LRC
	*pLRC = 0x00;
}

/*------------------------------------------------------------------
 *  Name:           P60_0810_IdxPre
 *  Description:    Indexed Table Pre-Processing
 *  Parameters:     pDest  : Pointer to Array of Destination Buffers
 *                  szeRec : Size of one Destination Buffer in the array
 *                  numRec : Number of Dest Buffers in the array
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/

void P60_0810_IdxPre( UBYTE *pdest, UWORD szeRec, UBYTE numRec )
{
    UBYTE index;

    /* Get array index into table from subfield */
    index = CvtBin( *pSFData );      

    if ( (index == 0) || (index > numRec) ) 
    {
        /* No valid buffer to load */
        B60DBytesLeft = 0;            
    }
    else
    {
    	/* Destination buffer */
    	pB60DAddress  = pdest + szeRec*(index-1);        
    
    	/* How much to copy */
    	B60DBytesLeft = szeRec;                      
    }
}
