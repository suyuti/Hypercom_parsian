
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             debugprt.c
//      Debug print support functions
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "utllib.h"
#include "fixdata.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "tables.h"
#include "chkprinter.h"
#include "prtdata.h"

#include "debugprt.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static void PrintMsg( enum msg_id msgid );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static const char NewLine[] = "\n";
static UBYTE LOCBUF[AQSZE];				// Local buffer for AQTAB/ISTAB/CRTAB records (the size is set to max of the three)
static struct acquirer_rec * pAcqPtr;	// Pointer to AQTAB record
static struct issuer_rec * pIssPtr;		// Pointer to ISTAB record
static struct card_rec * pCRPtr;		// Pointer to CRTAB record

//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//  PrintMsg        Print a message from the Multilingual table.
//
//  Parameters:     enum msg_id msgid   message id to be printed
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          Use LineBuffer to get and print the message.
//
//-----------------------------------------------------------------------------

static void PrintMsg( enum msg_id msgid )
{
	GetMsg( msgid, LineBuffer );
	SDK_PrintString( LineBuffer );
}


//-----------------------------------------------------------------------------
//  PrintDataBase   Print INITIALIZE Data Base
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

extern void PrintDataBase( void )
{
	UBYTE i;
	static UBYTE crnum;
	UBYTE	*pcrnum;

	// Notify user -- "PRINTING, WAIT..."
	ShowInfoMsg( PrintingWait, PleaseWait );

	// See if Printer is available
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// return
		return;
	}

	// See if printer may be opened
	if ( SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ) == orvOK )
	{
		// Yes; print the Database Report title lines
		PrintMsg( Prt_DatabaseReport );
		PrintMsg( Prt_Dashes );

		// Perform a line feed
		SDK_PrintString( ( char * ) NewLine );

		// Print the Terminal Eprom title line
		PrintMsg( Prt_TermEpromCfg );

		// Print the Terminal Eprom data
		PrintData( N_NullStr, ( UBYTE * ) & TERM, 255, True );

		// Print the Terminal Configuration title line
		PrintMsg( Prt_TermConfig );

		// Print the Terminal Configuration Table data
		PrintData( N_NullStr, ( UBYTE * ) & TCONF, TCSZE, True );

		// Print the Aquirer Table
		for ( pAcqPtr = &AQTAB[0]; pAcqPtr < &AQTAB[AQMAX]; pAcqPtr++ )
		{
			// See if Acquirer ID exists for this entry
			if ( pAcqPtr->AQID )
			{
				// Yes; clear the printer buffer
				memset( ( UBYTE * ) LineBuffer, 0, S_LineBuffer );

				// Get Acquirer ID label
				GetMsg( Prt_AcquirerID, LineBuffer );

				/* Move the AQTAB record into local buffer */
				MoveItAq( &LOCBUF, pAcqPtr, AQSZE );

				/* Get Acquirer ID (offset for AQID = 1) */
				BfAscii( &LineBuffer[StrLn(LineBuffer, sizeof(LineBuffer))], 
						 &LOCBUF[1] , 1 );

				// Print the Acquirer ID
				SDK_PrintString( LineBuffer );

				// Perform a line feed
				SDK_PrintString( ( char * ) NewLine );

				// Clear the printer buffer
				memset( ( UBYTE * ) LineBuffer, 0, S_LineBuffer );

				// Get the Acquirer Name
				MoveItAq( LineBuffer, pAcqPtr->AQNAME, S_AQNAME );

				// Add a space
				LineBuffer[S_AQNAME] = ' ';

				// Get the Program Name
				MoveItAq( &LineBuffer[S_AQNAME + 1], pAcqPtr->AQPNAME, S_AQPNAME );

				// Print the Acquirer and Program Names
				SDK_PrintString( LineBuffer );

				// Perform a line feed
				SDK_PrintString( ( char * ) NewLine );

				// Print the Acquirer Table data for this entry
				PrintData( N_NullStr, (UBYTE *)LOCBUF, AQSZE, True );
			}
		}

		// Print the Issuer Table
		for ( pIssPtr = &ISTAB[0]; pIssPtr < &ISTAB[ISMAX]; pIssPtr++ )
		{
			// See if Issuer ID exists for this entry
			if ( pIssPtr->ISID )
			{
				// Yes; clear the printer buffer
				memset( ( UBYTE * ) LineBuffer, 0, S_LineBuffer );

				// Get Issuer ID label
				GetMsg( Prt_IssuerID, LineBuffer );

				/* Move the ISTAB record into local buffer */
				MoveItIss( &LOCBUF, pIssPtr, ISSZE );

				/* Get Issuer ID (offset for ISID = 1) */
				BfAscii( &LineBuffer[StrLn(LineBuffer, sizeof(LineBuffer))], 
						 &LOCBUF[1] , 1 );

				// Print the Issuer ID
				SDK_PrintString( LineBuffer );

				// Perform a line feed
				SDK_PrintString( ( char * ) NewLine );

				// Clear the printer buffer
				memset( ( UBYTE * ) LineBuffer, 0, S_LineBuffer );

				// Get the Issuer Name
				MoveItIss( LineBuffer, pIssPtr->ISNAME, S_ISNAME );

				// Print the Issuer Name
				SDK_PrintString( LineBuffer );

				// Perform a line feed
				SDK_PrintString( ( char * ) NewLine );

				// Print the Issuer Table data for this entry
				PrintData( N_NullStr, (UBYTE *)LOCBUF, ISSZE, True );
			}
		}

		// Print the Card Table
		for ( pCRPtr = &CRTAB[0]; pCRPtr < &CRTAB[CRMAX]; pCRPtr++ )
		{
			// See if Card Range exists for this entry
			if ( ( crnum = (pCRPtr->CRNUM) ) != 0 )
			{
				// Yes; clear the printer buffer
				memset( ( UBYTE * ) LineBuffer, 0, S_LineBuffer );

				// Get Card Range ID label
				GetMsg( Prt_CardRangeID, LineBuffer );

				// Get Card Range ID
				BfAscii( &LineBuffer[StrLn( LineBuffer, sizeof(LineBuffer) )], 
						 pcrnum = &crnum, 1 );

				// Print the Card Range ID
				SDK_PrintString( LineBuffer );

				// Perform a line feed
				SDK_PrintString( ( char * ) NewLine );

				/* Move the Card Table entry into local buffer */
				MoveItCR( (UBYTE *)&LOCBUF, pCRPtr, CRSZE );
				/* Print the Card Table data for this entry */
				PrintData( N_NullStr, (UBYTE *)&LOCBUF, CRSZE, True );
			}
		}

		// Print the Descriptor Table
		for ( i = 0; i < DSMAX; i++ )
		{
			// See if Descriptor ID exists for this entry
			if ( DSTAB[i].DSNUM )
			{
				// Yes; clear the printer buffer
				memset( ( UBYTE * ) LineBuffer, 0, S_LineBuffer );

				// Get Descriptor ID label
				GetMsg( Prt_DescriptorID, LineBuffer );

				// Get Descriptor ID
				BfAscii( &LineBuffer
						 [StrLn( LineBuffer, sizeof( LineBuffer ) )],
						 &DSTAB[i].DSNUM, 1 );

				// Print the Descriptor ID
				SDK_PrintString( LineBuffer );

				// Perform a line feed
				SDK_PrintString( ( char * ) NewLine );

				// Clear the printer buffer
				memset( ( UBYTE * ) LineBuffer, 0, S_LineBuffer );

				// Get the Descriptor Key
				LineBuffer[0] = DSTAB[i].DSKEY;

				// Add a space
				LineBuffer[1] = ' ';

				// Get the Descriptor Text
				memcpy( &LineBuffer[2], DSTAB[i].DSTXT, S_DSTXT );

				// Print the Descriptor Key and Text
				SDK_PrintString( LineBuffer );

				// Perform a line feed
				SDK_PrintString( ( char * ) NewLine );

				// Print the Descriptor Table data for this entry
				PrintData( N_NullStr, ( UBYTE * ) & DSTAB[i], DSSZE, True );
			}
		}

		// Perform a form feed which cuts the paper
		SDK_PrintString( "\f" );

		// Wait till printing is done
		while ( SDK_PrinterBusy( ) && ( 0x0E != TCONF.TCPRINTER ) )
		{
			// Give up the CPU
			SDK_RelinqCPU(  );
		}

		// Close the printer
		SDK_PrinterClose(  );
	}

}


//-----------------------------------------------------------------------------
//  PrintDataScope  Print Send and Receive Buffers
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

extern void PrintDataScope( void )
{
	// Notify user -- "PRINTING, WAIT..."
	ShowInfoMsg( PrintingWait, PleaseWait );

	// See if Printer is available
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller
		return;
	}

	// See if printer may be opened
	if ( SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ) == orvOK )
	{
		// Yes; dump the Receive Buffer
		PrintData( Prt_ReceivedData, RECVBUF, 255, True );

		// Dump the Send Buffer
		PrintData( Prt_SendData, SENDBUF, 255, True );

		// Perform a form feed which cuts the paper
		SDK_PrintString( "\f" );

		// Wait till printing is done
		while ( SDK_PrinterBusy( ) && ( 0x0E != TCONF.TCPRINTER ) )
		{
			// Give up the CPU
			SDK_RelinqCPU(  );
		}

		// Close the printer
		SDK_PrinterClose(  );
	}

}

//=============================================================================
// Private function definitions
//=============================================================================
