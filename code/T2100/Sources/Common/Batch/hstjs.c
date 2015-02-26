
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             hstjs.c
//      Host module journal select routines
//
//=============================================================================

#include "string.h"

#include "basictyp.h"

#include "struct.h"
#include "appdata.h"
#include "journal.h"

#include "hstjs.h"

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
//  HostJRead       Read selected Journal Record
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Journal record read okay
//                              False - Journal record not found.
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool HostJRead( void )
{
	// Read data into TRREQ
	return JournalRead ( JrnHostIdent, &TRREQ );
}


//-----------------------------------------------------------------------------
//  HostJSFirst     Select first Journal Record
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - record is available
//                              False - no records available.
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool HostJSFirst( void )
{
    UWORD JrIdentifier;

	// Select first record that matches
	JrIdentifier = JournalSelFirst ( );

	// See if exists
	if ( JrIdentifier != 0x0000 )
	{
        JrnHostIdent = JrIdentifier;

		// Return record found
		return ( True );
	}
	else
	{
		// No; don't change JrnHostIdent
		// Return no records available
		return ( False );
	}
}



//-----------------------------------------------------------------------------
//  HostJSNext      Select next Journal Record
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - record is available
//                              False - no more records available.
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool HostJSNext( void )
{
    UWORD HSNJrnIdent;

	// Select next record that matches
	HSNJrnIdent = JournalSelNext ( JrnHostIdent );

	// See if exists
	if ( HSNJrnIdent != 0x0000 )
	{
        JrnHostIdent = HSNJrnIdent;
		// Return record found
		return ( True );
	}
	else
	{
		// Return no records available
		return ( False );
	}
}

#ifdef	MAKE_EMV
/*------------------------------------------------------------------
 *  Name:           HostJSPrev
 *  Description:    Select previous Journal Record
 *  Parameters:     None.
 *  Return Value:   True  - record is available
 *                  False - no more records available.
 *  Notes:          None.
 *-----------------------------------------------------------------*/

Bool HostJSPrev( void )
{
    UWORD HSPJrnIdent;

	/* Select next record that matches */
	HSPJrnIdent = JournalSelPrevious ( JrnHostIdent );

	/* See if exists */
	if ( HSPJrnIdent != 0x0000 )
	{
        JrnHostIdent = HSPJrnIdent;
		/* Return record found */
		return( True );
	}
	else
	{
		/* Return no records available */
		return( False );
	}
}
#endif	// MAKE_EMV

//-----------------------------------------------------------------------------
//  DefHostJSRec    Set JSREC to the default value, including AQID
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void DefHostJSRec( void )
{
	// Set up default selection data in common record JSREC
	SetDefaultSelection ();

	// Status Bits off
	JSREC.JSSTATOFF = TS1_SETL;

	// Select this Acquirer
	JSREC.JSAQID = TRREQ.TRAQID;
}

//=============================================================================
// Private function definitions
//=============================================================================
