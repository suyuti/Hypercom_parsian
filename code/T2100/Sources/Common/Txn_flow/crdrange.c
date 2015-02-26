
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             crdrange.c
//      Card range check function
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "tables.h"

#include "crdrange.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static UBYTE CheckIssuerAcquirer( void );
static UBYTE SetIssuerAcquirer( enum transactions t, UBYTE IsId, UBYTE AqId );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  CheckCardRange      Check if the account number in TRINP is in the card
//                      range table.  If so, check the issuer & acquirer
//                      capabilites for this transaction.  If O.K., set the
//                      issuer & acquirer numbers & pointers in TRINP.
//
//  Parameters:     None
//
//  Global Inputs:  The card number is in TRINP.TRPAN in BCD form
//
//  Returns:        char    1 if acceptable card range, acquirer, & 
//                            issuer found

//                          0 if not found
//  Notes:
//
//-----------------------------------------------------------------------------

extern char CheckCardRange( void )
{
	if ( False == (TRINP.TRCRNUM = FindCR()) )
	{
		/* Card Range Not Found for TRINP.TRPAN */
		return( False );
	}

	/* Setup pointer to Card Range Entry */
	TRINP.TRCRPTR = FindCR_ById( TRINP.TRCRNUM );

	/* Set issuer & acquirer IDs and pointers */
	if ( !CheckIssuerAcquirer( ) )
	{
		return 0;
	}

	/* If transaction allows it, copy issuer name to the left */
	/* half of TRTRANBUF.                                     */
	if ( !(CurrentEvent.TRNOPT3 & T3_NOISNAME) )
	{
		MoveItIss( TRINP.TRTRANBUF, (TRINP.TRISPTR)->ISNAME, S_ISNAME );
	}
	else
	{
		memset( (char *)TRINP.TRTRANBUF, ' ', S_ISNAME );
	}

	return 1;
}

//-----------------------------------------------------------------------------
//  CheckIssuerAcquirer     Choose one of two issuers/acqirers associated with
//                          TRINP.TRCRPTR as appropriate for DEBIT, CREDIT
//                          transactions.
//
//  Parameters:             None
//
//  Global Inputs:          uses TRINP and CurrentEvent.NEXTEVENT
//
//  Returns:                UBYTE   non-zero if Acquirer ID is found
//                                  0 if Acquirer ID is not found
//
//  Notes:
//
//-----------------------------------------------------------------------------

static UBYTE CheckIssuerAcquirer( void )
{
	UBYTE isid,isid2,aqid,aqid2;

	isid  = TRINP.TRCRPTR->CRISID;
	isid2 = TRINP.TRCRPTR->CRISID2;
	aqid  = TRINP.TRCRPTR->CRAQID;
	aqid2 = TRINP.TRCRPTR->CRAQID2;

	/* Is issuer 1 defined? */
	if ( 0 != isid )
	{
		/* Are both issuers defined? */
		if ( 0 != isid2 )
		{
			/* If first issuer is not debit, choose it */
			if ( SetIssuerAcquirer( SALE, isid, aqid ) )
			{
				return 1;
			}

			/* otherwise, take the second issuer, ignoring IS4_DEBIT */
			if ( SetIssuerAcquirer( NOOPT, isid2, aqid2 ) )
			{
				return 1;
			}
		}
		else
		{   /* Only issuer 1 defined? */
            /* take issuer 1, ignoring IS4_DEBIT */
			if ( SetIssuerAcquirer( NOOPT, isid, aqid ) )
			{
				return 1;
			}
		}
	}
	/* Only issuer 2 defined? */
	else if ( 0 != isid2 )
	{
		/* otherwise, take the second issuer, ignoring IS4_DEBIT */
		if ( SetIssuerAcquirer( NOOPT, isid2, aqid2 ) )
		{
			return 1;
		}
	}

   /* Couldn't find an issuer and acquirer, abort transaction */
   return 0;
}


//-----------------------------------------------------------------------------
//  SetIssuerAcquirer       Try to set 
//
//  Parameters:             None
//
//  Global Inputs:          uses TRINP
//
//  Returns:                UBYTE   non-zero if Acquirer ID is found
//                                  0 if Acquirer ID is not found
//  Notes:
//
//-----------------------------------------------------------------------------

static UBYTE SetIssuerAcquirer( enum transactions t, UBYTE IsId, UBYTE AqId )
{
	struct issuer_rec *pIR;
	struct acquirer_rec *pAQ;
	UBYTE  isopt4,isopt5;

	/* can we find the issuer? */
	pIR = FindIssuer(IsId);
	if (pIR == NULL)
	{
		return 0;
	}

   /* Get option bytes */
   isopt4 = pIR->ISOPT4;
   isopt5 = pIR->ISOPT5;

   /* select this issuer */
   TRINP.TRISPTR = pIR;
   TRINP.TRISID =  pIR->ISID;

   /* look up the corresponding acquirer */
   pAQ = FindAcquirer( AqId );
   if (pAQ == NULL)
      return 0;
   TRINP.TRAQPTR = pAQ;
   TRINP.TRAQID =  pAQ->AQID;

   /* SUCCESS! */
   return 1;
}
