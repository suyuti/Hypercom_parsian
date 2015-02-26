
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             testprnt.c
//      Prints the receipt RP_TEST.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "prtdef.h"
#include "chkprinter.h"
#include "menu.h"
#include "prtdrv.h"
#include "msg.h"
#include "util.h"

#include "testprnt.h"

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
//  TestPrinter     It handles both the roll and sprocket printer.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          Display error message box if failed printing
//
//-----------------------------------------------------------------------------

extern void TestPrinter( void )
{
	UBYTE chr;

	// Enable Printer
	PRTSTAT = 0x01;

	// See if Printer is available
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller
		return;
	}

	ClearMenus ();

	/* Draw test printing menu */
	DrawMenu(TEST_PRINT_MENU);

	switch ( GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout ) )
	{
		case 'A':
			ShowInfoMsg( TestPrnC, N_Print );

			// Initialize printer PCL data and flags.
    		PrintInit ();

			// Turn on Printer Header
			PROPT |= OPT1MASK;
			// Turn on Printer Paper Feed
			PROPT |= OPT3MASK;

			// Print the Receipt
			PrintDriver (  RP_TEST, NULL );

            break;
		
		case 'B':
			ShowInfoMsg( TestPrnC, N_Print );

			// Initialize printer PCL data and flags.
    		PrintInit ();

			/* Set receipt layout */
			PROPT |= OPT1MASK;		// Turn on Printer Header
			PROPT |= OPT2MASK;		// Turn on Printer Character map section
			
			memset( TRINP.TRHOSTTEXT, ' ', S_TRHOSTTEXT );
			TRINP.TRHOSTTEXT[ 2] = 'h';
			TRINP.TRHOSTTEXT[ 8] = 'd';
			TRINP.TRHOSTTEXT[22] = 'h';
			TRINP.TRHOSTTEXT[28] = 'd';

			/* Print character map */
			chr = ' ';
			while ( 1 )
			{
				BfAscii( (char*)&TRINP.TRHOSTTEXT[0], &chr, 1 );
				TRINP.TRHOSTTEXT[5] = (char)(chr / 100) + '0';
				TRINP.TRHOSTTEXT[6] = (char)((chr % 100) / 10) + '0';
				TRINP.TRHOSTTEXT[7] = (char)(chr % 10) + '0';
				TRINP.TRHOSTTEXT[11] = chr;
				if (chr != 255) chr++;

				BfAscii( (char*)&TRINP.TRHOSTTEXT[20], &chr, 1 );
				TRINP.TRHOSTTEXT[25] = (char)(chr / 100) + '0';
				TRINP.TRHOSTTEXT[26] = (char)((chr % 100) / 10) + '0';
				TRINP.TRHOSTTEXT[27] = (char)(chr % 10) + '0';
				TRINP.TRHOSTTEXT[31] = chr;
				if (chr != 255) chr++;

				PrintDriver ( RP_TEST, &TRINP );
				
				if (chr > ' ')	PROPT &= ~OPT1MASK;	// Turn off Printer Header
				if (chr >= 254)	PROPT |= OPT3MASK;	// Turn on Printer Paper Feed
				if (chr == 255) break;
			}
			break;
		
		default:
			// bad key, error beep
			SDK_Beeper( TENMS * 40 );
			break;
	}

	// Close the Printer.
	SDK_PrinterClose(  );

	// Display appropriate printer error.
	if ( PaperOutError == PrinterErrorFlags )
		ShowErrMsg( PaperOut );

    else if ( PrinterOverheatError == PrinterErrorFlags )
		ShowErrMsg( PrinterOverheat );
	
    else if ( OtherPrintError == PrinterErrorFlags )
		ShowErrMsg( PrintError );
}

//=============================================================================
// Private function definitions
//=============================================================================
