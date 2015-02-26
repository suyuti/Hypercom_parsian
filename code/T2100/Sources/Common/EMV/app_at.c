// $Workfile:   app_at.c  $
// $Header:   N:/pos/PVCS6.6/POSDev/West_Europe/archives/Sweden/Optimum_x2100_Application/Sources/Common/EMV/app_at.c-arc   1.1   Jan 21 2006 15:24:06   aamelin  $
// $Log:   N:/pos/PVCS6.6/POSDev/West_Europe/archives/Sweden/Optimum_x2100_Application/Sources/Common/EMV/app_at.c-arc  $
 //
 //   Rev 1.1   Jan 21 2006 15:24:06   aamelin
 //MoveIt SpcIt functions removing
 //
 //   Rev 1.0   Aug 25 2005 18:20:54   aasiounk
 //Initial revision.
 //
 //   Rev 1.0   Mar 24 2004 13:09:58   rzajtsev
 //Initial revision.
 //
 //   Rev 1.0   Mar 16 2004 16:47:54   aaganichev
 //Initial revision.
//LOG: 
//LOG:    Rev 1.0   05 Jun 2002 16:50:24   jwhitake
//LOG: Initial revision.
/*                                                                           */
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
/*                                                                           */

/* Include directives */

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
extern UBYTE *pSFData;					/* Subfield data start pointer        */
extern UBYTE *pB60DAddress;   			/* Pointer to destination buffer      */
extern UWORD MoveSize;					/* Number of bytes to move            */
/* B60DBytesLeft is passed from EMV Pre-processing routines to RSPINIT */
extern UWORD B60DBytesLeft;  			/* Bytes left in destination buffer   */
#endif /* MAKE_EMV */
//
extern	UBYTE	ADTFMT;
extern	ADTRecType ADTData[EMVL2_ADT_MAX];
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
/*-----------------------------------------------------------------------
 * Name:           InitADTTab
 * Description:    The function initialises CAPK and REVK keys.
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void InitADTTab ( void )
{
// Clear the ADT table                                       
	InitTable( &ADTFMT, (UBYTE *) ADTData,  EMVL2_ADT_MAX * sizeof(ADTRecType), &ADTLRC );
}

/*-----------------------------------------------------------------------
 * Name:           ADTTabPre
 * Description:    Setup destination address
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void ADTTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *) ADTData, sizeof(ADTRecType), EMVL2_ADT_MAX );
}


/*-----------------------------------------------------------------------
 * Name:           ADTTabPost
 * Description:    Additional Tags Table, Post-processing
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void ADTTabPost ( void )
{
	ADTLRC = CalcLrc( (UBYTE *) ADTData, EMVL2_ADT_MAX * sizeof(ADTRecType) );
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
/*------------------------------------------------------------------
 *  Name:           P60_LDIR_EMVTabs1
 *  Description:    Move Data to a Local Buffer 
 *  Parameters:     None.
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/

void P60_LDIR_ADTTab( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

/*-----------------------------------------------------------------------
 * Name:           Val_ADTTab
 * Description:    Validate tables on this page
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void Val_ADTTab ( void )
{
  	ValidateTable( &ADTFMT, (UBYTE * ) ADTData, EMVL2_ADT_MAX * sizeof(ADTRecType), &ADTLRC );
}

/*------------------------------------------------------------------
 *  Name:           ValidateTable
 *  Description:    Check validity of Tables
 *  Parameters:     pTabFMT
 *                  pTabData
 *                  len
 *                  pLRC
 *  Return Value:   True if ok, False if bad
 *  Notes:		   None.
 *-----------------------------------------------------------------*/

static UBYTE ValidateTable( UBYTE *pTabFMT, UBYTE *pTabData, UWORD len,
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

