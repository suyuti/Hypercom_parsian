
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     manuissn.c
//    Function to entry issuer number
//
//=============================================================================

#include "basictyp.h"

#include "struct.h"
#include "appdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "tables.h"

#include "manuissn.h"

static const ENTRY_STR IssueNumEntry =
{
	N_NullStr,			/* Default Title */
	IssueNum,				/* Prompt */
	N_NullStr,				/* Default value */
};

//-----------------------------------------------------------------------
//! \brief
//!     Accept Issue Number from keyboard
//!
//! \param
//!     None
//!
//! \return
//!     None
//!
//! \note
//!     Data will be stored in global buffer TRINP.TRISSUENUM in BCD.
//!		Global variable chRetval contains the status of the entry:
//!  	0 CANCEL_KY key was pressed
//!		1 data was entered successfully
//!
//-----------------------------------------------------------------------
UBYTE ManualIssueNum(void)
{
	UBYTE chRetval = 1;

/* 		Get Issue Number if exists */
	if ( TRINP.TRCRPTR->CRINPOS )
	{
		/* Save the Issue Number */
		if ((TRINP.TRCRPTR->CROPT1) & CR1_2DIGISSN)
		{
			/* Two digits */
			chRetval = NumEntry( 1, 2, 2, &IssueNumEntry );
			if(chRetval)
			{
				TRINP.TRISSUENUM =  Dspbuf[0] - '0';
				TRINP.TRISSUENUM <<= 4;
				TRINP.TRISSUENUM |=  (Dspbuf[1] - '0');
			}
		}
		else
		{
			/* One digit */
			chRetval = NumEntry( 1, 1, 1, &IssueNumEntry );
			if(chRetval)
			{
				TRINP.TRISSUENUM =  Dspbuf[0] - '0';
			}
		}
	}

	return chRetval;
}
