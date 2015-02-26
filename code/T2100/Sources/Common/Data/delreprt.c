
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             delreprt.c
//      Delete all reportXX files
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "utllib.h"
#include "hfs.h"
#include "sdk.h"
#include "sdktime.h"

#include "delreprt.h"

//=============================================================================
// External variables / declarations
//=============================================================================

// Public from other modules

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
//  DeleteReports   Delete all ReportXX files
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          Delete all Report*.* and rpt*.*
//
//-----------------------------------------------------------------------------

extern void DeleteReports( void )
{
	Bool again;
	char filename[FILENAME_MAX + 1];
	FFBLK findfileblk;

	do
	{
		// Assume we won't need to keep looping
		again = False;

		// Get the name of each file in the file system, one file at a time
		if ( findfirst( "*", &findfileblk ) != 0 )
		{
			do
			{
				memcpy( filename, findfileblk.dirInfo.d_name, FILENAME_MAX + 1 );

				// Found a file, does it's name match "Report??rpt" ?
				if ( ( memcmp( ( UBYTE * ) filename, ( UBYTE * ) "Report", 6 )
					   == 0 ) &&
					 ( memcmp
					   ( ( UBYTE * ) & filename[8], ( UBYTE * ) "rpt",
						 3 ) == 0 ) )
				{
					// Found a "Report*.*", delete it
					remove( filename );

					// Start over looking for another report file.
					again = True;

					// After calling remove, we have to break out
					// of the findfirst/findnext loop.
					break;
				}

				SDK_RelinqCPU(  );
			}
			while ( findnext( &findfileblk ) != 0 );
		}

	}
	while ( again );
}


//=============================================================================
// Private function definitions
//=============================================================================
