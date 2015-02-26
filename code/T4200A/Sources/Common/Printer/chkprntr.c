//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             chkprntr.c
//      Routine to check printer status
//
//=============================================================================
#include "basictyp.h"
#include "sdk.h"
#include "sdktime.h"

#include "struct.h"
#include "appdata.h"

#include "chkprinter.h"


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
//! \brief
//!     Checks if printer is initialized and if it is enabled
//!
//!     Checks if printer is initialized and if it is enabled. The printer
//!     status is tested if it is configured and logically enabled.
//!
//! \param
//!		[in]DispErrMsg			0 - do not display message on error,<br>
//!                             1 - display message if enabled,<br>
//!                             2 - display disabled msg only
//!
//! \return
//!     True if Printer is in good condition else False (not configured or disabled)
//-----------------------------------------------------------------------------
extern Bool ChkPrinter( UBYTE DispErrMsg )
{
	PRTSTATUS printStat;

	if ( TCONF.TCOPT2 & TC2_PRINT )
	{
		// Test for printer logically enabled and no errors.
		if ( PRTSTAT & 0x01 )
		{
			// Printer is logically enabled.

			// Test status of the printer.
			if ( orvOK == SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ) )
			{
				// Get the status.
				if ( orvOK == SDK_PrinterStatus( &printStat ) )
				{
					if ( prt_PAPER_OK == printStat )
					{
						return True;
					}
					else if ( ( prt_PAPER_OUT == printStat ) &&
							  ( DispErrMsg & ErrNotReady ) )
						ShowErrMsg( PaperOut );
					else if ( DispErrMsg & ErrNotReady )
						ShowErrMsg( PrintError );
				}
				// Close printer on same page where it was opened.
				SDK_PrinterClose(  );
			}
		}
		else
		{
			// Printer is not logically enabled.
			if ( DispErrMsg & ErrNotEnabled )
				ShowErrMsg( PrinterDP );
		}
	}
	else
	{
		if ( DispErrMsg & ErrNotEnabled )
		{
			// Printer is not available
			ShowErrMsg( NoPrinter );
		}
	}

    return False;
}


//=============================================================================
// Private function definitions
//=============================================================================

