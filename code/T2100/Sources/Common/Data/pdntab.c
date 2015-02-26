
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             pdntab.c
//      PDN table functionality
//
//=============================================================================
                                         
#include "clib.h"

#include "basictyp.h"
#include "sdkos.h"
#include "sdk.h"
#include "sdkmem.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "tabutil.h"

#include "pdntab.h"

/*-----------------------------------------------------------------------
 * Name:           PDNTabPre
 * Description:    Setup destination address
 *
 * Parameters:     None
 * Return Value:   None.
 * Notes:          
 *-----------------------------------------------------------------------*/
void PDNTabPre ( void )
{
	P60_0810_IdxPre( (UBYTE *) PDNTAB, PDNSZE, PDNMAX );
}

/*------------------------------------------------------------------
 *  Name:           P60_LDIR_PDNTab
 *  Description:    Move Data to a Local Buffer 
 *  Parameters:     None.
 *  Return Value:   None.
 *  Notes:          None.
 *-----------------------------------------------------------------*/

void P60_LDIR_PDNTab ( void )
{
   	memcpy( pB60DAddress, pSFData, MoveSize );
}

/*---------------------------------------------------------
 *  Name:           SetPDNTabLRC
 *  Description:    Set the Checksum (LRC) for the PDN Table
 *  Parameters:     None.
 *  Return Value:   None.
 *--------------------------------------------------------*/

void SetPDNTabLRC( void )
{
	PDNLRC = CalcLrc( (UBYTE *)&PDNTAB, PDNSZE*PDNMAX );
}

/*------------------------------------------------------------------
 *  Name:         FindPDN
 *  Description:  Return Ptr to PDN Table Entry given the Acquirer ID
 *  Parameters:   Acquirer id.
 *  Return Value: Pointer to PDN Entry or NULL if it does not exist. 
 *  Notes:        
 *-----------------------------------------------------------------*/
PDNSTRUCT * FindPDN(UBYTE acquirer_id)
{
  static PDNSTRUCT * pPDNPtr;
  
  for (pPDNPtr=PDNTAB; pPDNPtr < &PDNTAB[PDNMAX]; pPDNPtr++)
  {
     if ( pPDNPtr->PDNAQID == acquirer_id )
     {
        return ( pPDNPtr );
     }
  }
    return ((PDNSTRUCT *)NULL);
}


