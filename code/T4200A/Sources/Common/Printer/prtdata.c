//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             prtdata.c
//      Print a raw (HEX) Data Structure
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "comdata.h"
#include "msg.h"

#include "prtdata.h"

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
char LineBuffer[S_LineBuffer];


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//! \brief
//!     Prints a raw (HEX) Data Structure
//!
//! \param
//!		[in]Title			Pointer to label for the data print, NULL for
//!                         default label
//!		[in]pData			Pointer to the data
//!		[in]Length			Length of the data
//!		[in]FormFeed		Perform a form feed if True otherwise perform
//!                         a line feed at the end of printing
//!
//! \return
//!     None
//!
//! \note
//!     Assumes that the caller will handle the printer open and close.
//!     Length of the data <= 255 bytes.
//-----------------------------------------------------------------------------
void PrintData( enum msg_id Title, UBYTE * pData, short Length, Bool FormFeed )
{
	UBYTE MaxChars;
	short PrintChars;

	// Yes; get default width of the printer
	switch ( TCONF.TCPRINTER )
	{
		case prt_P7SPROCKET212:
		case prt_P7SPROCKET215:
		case prt_T7P23:
		case prt_P8SPROCKET:
		case prt_P8SPROCKETGRAPHICS:
			MaxChars = 23;
			break;

		case prt_P8ROLL:
			MaxChars = 35;
			break;

		default:
            MaxChars = 40;
			break;
	}

	// Divide by 2 (Maximum characters per line)
	MaxChars = MaxChars >> 1;

	// See if the title is provided
	if ( N_NullStr != Title )
	{
		// Print the given title
		GetMsg( Title, LineBuffer );
	}
	else
	{
		// Print the default title
		GetMsg( Prt_RawDataHexBytes, LineBuffer );
	}

	// Print Title
	SDK_PrintString( LineBuffer );

	// Perform a line feed
	SDK_PrintString( "\n" );

	// Loop until all bytes have been printed
	while ( Length > 0 )
	{
		// Is the remaining number of bytes over the maximum of a
		// line?
		if ( Length > MaxChars )
		{
			// Yes; use the maximum per line
			PrintChars = MaxChars;
		}
		else
		{
			// No; use the remaining length
			PrintChars = Length;
		}

		// Clear the printer buffer
		memset( ( UBYTE * ) LineBuffer, 0,S_LineBuffer );

		// Convert data to ASCII
		BfAscii( LineBuffer, pData, PrintChars );

		// Print the data
		SDK_PrintString( LineBuffer );

		// Perform a line feed
		SDK_PrintString( "\n" );

		// Wait till printing is done
		while ( SDK_PrinterBusy( ) && ( 0x0E != TCONF.TCPRINTER ) )
		{
			// Give up the CPU
			SDK_RelinqCPU(  );
		}

		// Update pointer to the data
		pData += PrintChars;

		// Update length of data remaining
		Length -= PrintChars;
	}

	// See if a form feed is requested
	if ( FormFeed )
	{
		// Yes; perform a form feed which does not cut paper -
		// actually six blank lines
		SDK_PrintString( "\n\n\n\n\n\n" );
	}
	else
	{
		// No; perform a line feed
		SDK_PrintString( "\n" );
	}
}

//=============================================================================
// Private function definitions
//=============================================================================

