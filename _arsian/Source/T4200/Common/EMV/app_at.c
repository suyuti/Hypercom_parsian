#ifdef MAKE_EMV
/*  *****************************************************************        */
/*  *                                                               *        */
/*  *                      Hypercom Inc                             *        */
/*  *                     (c) Copyright 1998                        *        */
/*  *                                                               *        */
/*  *  PRODUCT   :  Hypercom Base Application in C                  *        */
/*  *                                                               *        */
/*  *  COMPONENT :  EMV Application Page 2                          *        */
/*  *                                                               *        */
/*  *  FILENAME  :  app_at.c                                        *        */
/*  *                                                               *        */
/*  *  HISTORY   :                                                  *        */
/*  *                                                               *        */
/*  *    DATE         AUTHOR       COMMENT                          *        */
/*  *  --------     ----------    ----------                        *        */
/*  *  29/01/02     J Alvarez      1st Draft                        *        */
/*  *                                                               *        */
/*  *****************************************************************        */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"

#include "defines.h"
#include "utllib.h"

#include "emv2_pk.h"
#include "emv_l2.h"

/***************** External Function prototypes **************/
extern void InitTable( UBYTE *, UBYTE *, UWORD, UBYTE * );
extern void Clr_TermInit( void );
//
/*--------------- EXTERNAL VARIABLE DATA ---------------------*/
#ifdef MAKE_EMV
/* Parameters passed by RSPINIT to EMV initialisation routines */
//! Subfield data start pointer
extern UBYTE *pSFData;
//! Pointer to destination buffer
extern UBYTE *pB60DAddress;
//! Number of bytes to move
extern UWORD MoveSize;
//! B60DBytesLeft is passed from EMV Pre-processing routines to RSPINIT
//! Bytes left in destination buffer
extern UWORD B60DBytesLeft;
#endif /* MAKE_EMV */

//! Additional Tags Table formatter
extern	UBYTE	ADTFMT;
//! Additional Tags Table
extern	ADTRecType ADTData[EMVL2_ADT_MAX];
//! Additional Tags Table LRC
extern	UBYTE	ADTLRC;

/***************** Exported Function prototypes **************/
void InitADTTab ( void );
void ADTTabPre ( void );
void P60_LDIR_ADTTab ( void );
void ADTTabPost ( void );
void Val_ADTTab ( void );

/***************** Local Function prototypes **************/

static void P60_0810_IdxPre( UBYTE *pdest, UWORD szeRec, UBYTE numRec );
static UBYTE ValidateTable( UBYTE *pTabFMT, UBYTE *pTabData, UWORD len,
                            UBYTE *pTabLRC );
/* -- Local functions -- */
//-----------------------------------------------------------------------------
//! \brief
//!     Initialises Additional Tags Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void InitADTTab ( void )
{
// Clear the ADT table                                       
	InitTable( &ADTFMT, (UBYTE *) ADTData,  EMVL2_ADT_MAX * sizeof(ADTRecType), &ADTLRC );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Setup Additional Tags Table destination address
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void ADTTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *) ADTData, sizeof(ADTRecType), EMVL2_ADT_MAX );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Post-processing for Additional Tags Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void ADTTabPost ( void )
{
	ADTLRC = CalcLrc( (UBYTE *) ADTData, EMVL2_ADT_MAX * sizeof(ADTRecType) );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&ADTData );
#endif
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
static void P60_0810_IdxPre( UBYTE *pdest, UWORD szeRec, UBYTE numRec )
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
void P60_LDIR_ADTTab( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Validates Additional Tags Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void Val_ADTTab ( void )
{
  	ValidateTable( &ADTFMT, (UBYTE * ) ADTData, EMVL2_ADT_MAX * sizeof(ADTRecType), &ADTLRC );
}

//-----------------------------------------------------------------------------
//! \brief Checks validity of table
//!
//! \param[in/out] pTabFMT      Pointer to the table formatter byte
//! \param[in/out] pTabData     Pointer to the table data
//! \param[in] len              Table data length
//! \param[out] pTabLRC         Pointer to the table LRC byte
//! 
//! \return
//!     True if ok, False if bad.
//!
//! \note
//!     If bad, clear Terminal Initialization Done Flag.
//-----------------------------------------------------------------------------
static UBYTE ValidateTable( UBYTE *pTabFMT, UBYTE *pTabData, UWORD len, UBYTE *pTabLRC )
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
#endif

