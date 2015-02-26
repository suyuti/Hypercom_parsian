
#ifdef MAKE_EMV
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     emvtabs2.c
//  EMV application processing
//
//=============================================================================

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

#include "basictyp.h"
#include "utllib.h"
#include "emv_l2.h"
#include "emv2_pk.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "tabutil.h"
#include "tables.h"

#include "emvtabs2.h"

/***************** External Function prototypes **************/
//
/*--------------- EXTERNAL VARIABLE DATA ---------------------*/
//

/***************** Local Function prototypes **************/
UBYTE GetSignFlag( void );
static void P60_0810_IdxPreM( UBYTE *pdest, UWORD szeRec, UBYTE numRec );

/***************** Local Variables **************/


/*-----------------------------------------------------------------------
 * Name:           InitICCtab
 * Description:    Clear ICC table.
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void InitICCTab ( void )
{
	InitTable( &ICCFMT, (UBYTE *)ICCTAB,  ICCMAX*ICCSZE, &ICCLRC );  
}

/*-----------------------------------------------------------------------
 * Name:           InitTAIDtab
 * Description:    Clear the TAID table.
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void InitTAIDTab ( void )
{
// Clear the TAID table 
	InitTable( &TAIDFMT, (UBYTE *)TAIDTAB,  TAIDMAX*TAIDSZE, &TAIDLRC );  
}

/*-----------------------------------------------------------------------
 * Name:           ICCTabPre
 * Description:    Setup destination address
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void ICCTabPre ( void )
{
	P60_0810_IdxPreM( (UBYTE *) ICCTAB, ICCSZE, ICCMAX );
}
/*-----------------------------------------------------------------------
 * Name:           TAIDTabPre
 * Description:    Setup destination address
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void TAIDTabPre ( void )
{
	P60_0810_IdxPreM( (UBYTE *) TAIDTAB, TAIDSZE, TAIDMAX );
}

/*------------------------------------------------------------------
 *  Name:           P60_0810_IdxPreM
 *  Description:    Indexed Table Pre-Processing
 *  Parameters:     pDest  : Pointer to Array of Destination Buffers
 *                  szeRec : Size of one Destination Buffer in the array
 *                  numRec : Number of Dest Buffers in the array
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/

static void P60_0810_IdxPreM( UBYTE *pdest, UWORD szeRec, UBYTE numRec )
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

/*------------------------------------------------------------------
 *  Name:           P60_LDIR_EMVTabs2
 *  Description:    Move Data to a Local Buffer 
 *  Parameters:     None.
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/

void P60_LDIR_EMVTabs2( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}


/*-----------------------------------------------------------------------
 * Name:           ICCTabPost
 * Description:    ICC Table, Post-processing
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void ICCTabPost ( void )
{
	ICCLRC = CalcLrc( (UBYTE *)&ICCTAB, ICCSZE*ICCMAX );
}
/*-----------------------------------------------------------------------
 * Name:           TAIDTabPost
 * Description:    TAID Table, Post-processing
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void TAIDTabPost ( void )
{
	TAIDLRC = CalcLrc( (UBYTE *)&TAIDTAB, TAIDSZE*TAIDMAX );
}


/*------------------------------------------------------------------
 *  Name:         FindIcc
 *  Description:  Return Ptr to ICC Table for The issuer ID given
 *  Parameters:   Issuer Id to match.
 *  Return Value: ICC Table pointer or NULL if does not exist.
 *  Notes:        
 *-----------------------------------------------------------------*/
struct iccrec_rec * FindIcc(UBYTE iss_id)
{
	UBYTE i;

	for (i=0; i < ICCMAX; i++)
	{
		if (ICCTAB[i].ICCISID == iss_id)
		{
			return (struct iccrec_rec *)&ICCTAB[i];
		}
	} 
    return ((struct iccrec_rec *)NULL);
} 

/*------------------------------------------------------------------
 *  Name:         GetSignFlag
 *  Description:  Get "ALWAYS PRINT SIGNATURE" flag from ICC table
 *  Parameters:   None
 *  Return Value: !0 - Yes, 0 - No
 *  Notes:        
 *-----------------------------------------------------------------*/
UBYTE GetSignFlag( void )
{
static struct iccrec_rec *pIccPtr;		/* Pointer to ICCTAB entry  */

	/* Find ICC table given the Issuer Id */
	pIccPtr = FindIcc( TRINP.TRISID );
	if ( pIccPtr )
		return( pIccPtr->ICCOPT1 & ICC1_SIGNATURE );
	else
		return( False );/* ICC entry not found (should never happen) */
}

/* ------------------------------------------------------------------- */
__attribute__((section("iccpar")))
UBYTE ICCFMT = 0;                          /* FORMAT INDICATOR*/
__attribute__((section("iccpar")))
struct iccrec_rec ICCTAB[ICCMAX] = {{0}};    /* ICC TABLE */
__attribute__((section("iccpar")))
UBYTE ICCLRC = 0;                          /* LRC OF TABLE*/
									  
__attribute__((section("iccpar")))
UBYTE TAIDFMT = 0;
__attribute__((section("iccpar")))
struct taidrec_rec TAIDTAB[TAIDMAX] = {{0}}; /* TAID TABLE */
__attribute__((section("iccpar")))
UBYTE TAIDLRC = 0;
									  
#endif // MAKE_EMV
