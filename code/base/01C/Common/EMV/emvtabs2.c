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

/***************** Local Function prototypes **************/
UBYTE GetSignFlag( void );
static void P60_0810_IdxPreM( UBYTE *pdest, UWORD szeRec, UBYTE numRec );

/***************** Local Variables **************/
//! ICC Table formatter
__attribute__((section("iccpar")))
UBYTE ICCFMT = 0;
//! ICC Table
__attribute__((section("iccpar")))
struct iccrec_rec ICCTAB[ICCMAX] = {{0}};
//! ICC Table LRC
__attribute__((section("iccpar")))
UBYTE ICCLRC = 0;
									  
//! TAID Table formatter
__attribute__((section("iccpar")))
UBYTE TAIDFMT = 0;
//! TAID Table
__attribute__((section("iccpar")))
struct taidrec_rec TAIDTAB[TAIDMAX] = {{0}};
//! TAID Table LRC
__attribute__((section("iccpar")))
UBYTE TAIDLRC = 0;


//-----------------------------------------------------------------------------
//! \brief
//!     Clears ICC table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void InitICCTab ( void )
{
	InitTable( &ICCFMT, (UBYTE *)ICCTAB,  ICCMAX*ICCSZE, &ICCLRC );  
}

//-----------------------------------------------------------------------------
//! \brief
//!     Clears the TAID table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void InitTAIDTab ( void )
{
    // Clear the TAID table
	InitTable( &TAIDFMT, (UBYTE *)TAIDTAB,  TAIDMAX*TAIDSZE, &TAIDLRC );  
}

//-----------------------------------------------------------------------------
//! \brief
//!     Setup ICC table destination address
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void ICCTabPre ( void )
{
	P60_0810_IdxPreM( (UBYTE *) ICCTAB, ICCSZE, ICCMAX );
}
//-----------------------------------------------------------------------------
//! \brief
//!     Setup TAID table destination address
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void TAIDTabPre ( void )
{
	P60_0810_IdxPreM( (UBYTE *) TAIDTAB, TAIDSZE, TAIDMAX );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Indexed Table Pre-Processing
//!
//! \param
//!		[out]pdest			Pointer to Array of Destination Buffers
//!		[in]szeRec			Size of one Destination Buffer in the array
//!		[in]numRec			Number of Dest Buffers in the array
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! \brief
//!     Moves Data to a Local Buffer
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void P60_LDIR_EMVTabs2( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-processing for ICC Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void ICCTabPost ( void )
{
	ICCLRC = CalcLrc( (UBYTE *)&ICCTAB, ICCSZE*ICCMAX );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&ICCTAB );
#endif
}

//-----------------------------------------------------------------------------
//! \brief
//!     Post-processing for TAID Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void TAIDTabPost ( void )
{
	TAIDLRC = CalcLrc( (UBYTE *)&TAIDTAB, TAIDSZE*TAIDMAX );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&TAIDTAB );
#endif
}


//-----------------------------------------------------------------------------
//! \brief Returns Ptr to ICC Table for specified ID
//!
//! \param[in] iss_id           Issuer Id to find
//!                             
//! \return
//!     Pointer to ICC Table or NULL if it does not exist.
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! \brief
//!     Gets "ALWAYS PRINT SIGNATURE" flag from ICC table
//!
//! \return
//!     !0 - Yes, 0 - No
//-----------------------------------------------------------------------------
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

#endif // MAKE_EMV

