
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             settlesum.c
//      settle summary report
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "fixdata.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "utllib.h"
#include "journal.h"
#include "tables.h"
#include "settlesum.h"
#include "hstprint2.h"
#include "cvtamt.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================
#define 	Left 		1
#define 	Right 		2
#define		Center  	3
#define		PrintWidth	40


//=============================================================================
// Private function declarations
//=============================================================================
static void PrepBuffers( void );
static void PlaceData( char *pBuf, UBYTE Form );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static struct acquirer_rec *pAcquirerPtr;	// Pointer to AQTAB entry  
static struct acquirer_rec *pHostPtr;	// Pointer to AQTAB entry  
static struct batch_rec *pBatch_Table;	// Pointer to BATTAB entry            
static UBYTE length;			// Length counter
static char TEMPCSTMSG[41];		// Custom Message holder 



//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//!  \brief
//!     Print the Settlement Status Report
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void SettleSumRpt( void )
{
	UBYTE ReportOptions;

	// Notify user -- "PRINTING, WAIT..."
	ShowInfoMsg( PrintingWait, PleaseWait );

	// Find first Acquirer
	pHostPtr = FindFirstAcquirer(  );

	// Make sure there is an Aquirer Set
	if ( 0 == pHostPtr )
	{
		// Done, No Acquirer set
		ShowErrMsg( NoAcq );
		return;
	}

	// Set flag, so Report will Init PCL the first time
	//  ReportOptions   0x01 = Print Header
	//                  0x02 = Print Line and Two extra line feeds 
	//                  0x03 = Done, Send FormFeed
	ReportOptions = 1;

	// Print Information for each Acquirer
	while ( NULL != pHostPtr )
	{
		// Prepare the Custom Mesage Buffers
		PrepBuffers(  );

		// Find the Batch Pointer
        pBatch_Table = FindBatTab( pHostPtr -> AQID );

		// Load the Acquirer Name, Left Justified
		MoveItAq( TEMPCSTMSG, ( pHostPtr->AQNAME ), 10 );
		TEMPCSTMSG[10] = 0;
		PlaceData( CSTMSG, Left );

		// Check to see if this Acquirer has settled
		if ( 0 == pBatch_Table->BATSETLINI )
		{
			// Acquirer has not settled, Set Option to Print "NEVER" Centered
			GetMsg( Msg_Never, TEMPCSTMSG );
			PlaceData( CSTMSG, Center );
			ReportOptions |= 0x02;
		}
		else
		{
			// Acquirer has settled, Display all Summary Information
			// Determine method of Settlement
			switch ( pBatch_Table->BATSETLINI )
			{
				case CLBATCHA:	// Auto Settlement
					GetMsg( Msg_Auto, TEMPCSTMSG );
					break;

				case SETTLE:	// Group Settlement
					GetMsg( Msg_Group, TEMPCSTMSG );
					break;

				default:		// Manual Settlement
					GetMsg( Msg_Manual, TEMPCSTMSG );
					break;
			}
			// Remove last two spaces, then Center text
			length = StrLn( TEMPCSTMSG, sizeof( TEMPCSTMSG ) );
			TEMPCSTMSG[( length - 2 )] = 0;
			PlaceData( CSTMSG, Center );

			// Place the Response Code in the Buffer
			TEMPCSTMSG[0] = pBatch_Table->BATSETLRSPC[0];
			TEMPCSTMSG[1] = pBatch_Table->BATSETLRSPC[1];
			TEMPCSTMSG[2] = 0;
			PlaceData( CSTMSG, Right );

			// Get Settlement Date
			BfAscii( TEMPCSTMSG, pBatch_Table->BATSETLDAT, S_BATSETLDAT );
			TEMPCSTMSG[7] = TEMPCSTMSG[5];
			TEMPCSTMSG[6] = TEMPCSTMSG[4];
			TEMPCSTMSG[5] = '/';
			TEMPCSTMSG[4] = TEMPCSTMSG[3];
			TEMPCSTMSG[3] = TEMPCSTMSG[2];
			TEMPCSTMSG[2] = '/';
			TEMPCSTMSG[8] = 0;
			PlaceData( CSTMSG2, Left );

			// Get Settlement Time
			BfAscii( TEMPCSTMSG, pBatch_Table->BATSETLTIM, S_BATSETLTIM );
			TEMPCSTMSG[4] = TEMPCSTMSG[3];
			TEMPCSTMSG[3] = TEMPCSTMSG[2];
			TEMPCSTMSG[2] = ':';
			TEMPCSTMSG[5] = 0;
			PlaceData( CSTMSG2, Center );

			// Get Batch Number, convert to ASCII 
			BfAscii( TEMPCSTMSG, pBatch_Table->BATCUR, S_BATCUR );
			TEMPCSTMSG[( S_BATCUR * 2 )] = 0;
			PlaceData( CSTMSG2, Right );

			// Display "SALES ITEMS", Left Justified
			GetMsg( Msg_SaleItems, TEMPCSTMSG );
			PlaceData( CSTMSG3, Left );

			// Display Sales count, Right Justified
			BfAscii( TEMPCSTMSG, pBatch_Table->BATSETLTOT.TOTDBCNT,
					 S_TOTDBCNT );
			TEMPCSTMSG[( S_TOTDBCNT * 2 )] = 0;
			PlaceData( CSTMSG3, Right );

			// Display "AMOUNT", Left Justified
			GetMsg( AmountEnt, TEMPCSTMSG );
			PlaceData( CSTMSG4, Left );

			// Display the Sales Total Amount, Right Justified
			memset( ( UBYTE * ) TEMPCSTMSG, 0,sizeof( TEMPCSTMSG ) );
			CvtAmt( ( char * ) TEMPCSTMSG,
					pBatch_Table->BATSETLTOT.TOTDBAMT );
			PlaceData( CSTMSG4, Right );

			// Display "REFUND ITEMS", Left Justified
			GetMsg( Msg_RefundItems, TEMPCSTMSG );
			PlaceData( CSTMSG5, Left );

			// Display Sales count, Right Justified
			BfAscii( TEMPCSTMSG, pBatch_Table->BATSETLTOT.TOTCRCNT,
					 S_TOTCRCNT );
			TEMPCSTMSG[( S_TOTCRCNT * 2 )] = 0;
			PlaceData( CSTMSG5, Right );

			// Display "AMOUNT", Left Justified
			GetMsg( AmountEnt, TEMPCSTMSG );
			PlaceData( CSTMSG6, Left );

			// Display the Sales Total Amount, Right Justified
			memset( ( UBYTE * ) TEMPCSTMSG, 0,sizeof( TEMPCSTMSG ) );
			CvtAmt( ( char * ) TEMPCSTMSG,
					pBatch_Table->BATSETLTOT.TOTCRAMT );
			PlaceData( CSTMSG6, Right );

			// Print the first six lines of the Settlement Summary for selected Acquirer
			// See if printer is still functional.
			if ( 0 == PrintSettleSummary ( ReportOptions ) )
			{
				// No; return to caller
				return;
			}

			// Turn off the header  
			ReportOptions &= ~1;

			// Prepare the Custom Mesage Buffers
			PrepBuffers(  );

			// Display "ATTEMPT #" and the attempt number, Left Justified
			GetMsg( Msg_Attempt, TEMPCSTMSG );
			length = StrLn( TEMPCSTMSG, sizeof( TEMPCSTMSG ) );
			TEMPCSTMSG[length++] = Ascii( pBatch_Table->BATSETLTRY );
			TEMPCSTMSG[length] = 0;
			PlaceData( CSTMSG, Left );


			// Display "RESP " and then number , Right Justified
			GetMsg( Msg_Resp, TEMPCSTMSG );
			length = StrLn( TEMPCSTMSG, sizeof( TEMPCSTMSG ) );
			TEMPCSTMSG[length++] = pBatch_Table->BATSETLRSPC[0];
			TEMPCSTMSG[length++] = pBatch_Table->BATSETLRSPC[1];
			TEMPCSTMSG[length] = 0;
			PlaceData( CSTMSG, Right );

			// Get Attempt Date and time, Left Justified
			BfAscii( TEMPCSTMSG, pBatch_Table->BATATTMDAT, S_BATATTMDAT );
			TEMPCSTMSG[8] = 0;
			TEMPCSTMSG[7] = TEMPCSTMSG[5];
			TEMPCSTMSG[6] = TEMPCSTMSG[4];
			TEMPCSTMSG[5] = '/';
			TEMPCSTMSG[4] = TEMPCSTMSG[3];
			TEMPCSTMSG[3] = TEMPCSTMSG[2];
			TEMPCSTMSG[2] = '/';
			PlaceData( CSTMSG2, Left );

			// Get Attempt Time
			BfAscii( TEMPCSTMSG, pBatch_Table->BATATTMTIM, S_BATATTMTIM );
			TEMPCSTMSG[4] = TEMPCSTMSG[3];
			TEMPCSTMSG[3] = TEMPCSTMSG[2];
			TEMPCSTMSG[2] = ':';
			TEMPCSTMSG[5] = 0;
			PlaceData( CSTMSG2, Center );

			// RESP

			// Check to see if there is any Host Text to Display
			if ( pBatch_Table->BATSETLTEXT[0] )
			{
				// Get first line of display
				memcpy( TEMPCSTMSG, ( pBatch_Table->BATSETLTEXT ), 20 );
				TEMPCSTMSG[20] = 0;
				PlaceData( CSTMSG3, Left );

				// Get Second line of display
				memcpy( TEMPCSTMSG, &( pBatch_Table->BATSETLTEXT[20] ), 20 );
				PlaceData( CSTMSG4, Left );
			}
			// End of field, Print line and line feeds
			ReportOptions |= 0x02;
		}

		// Print Settlement Summary for selected Acquirer
		// See if printer is still functional.
		if ( 0 == PrintSettleSummary ( ReportOptions ) )
		{
			// No; return to caller
			return;
		}

		// Look for Next Batch 
		pAcquirerPtr = FindNextAcquirer( pHostPtr );
		if ( NULL == pAcquirerPtr )
		{
			// No More Acquires to Print, Form Feed and exit
			// Set flag, so Report will Do a form feed and close the printer
			ReportOptions |= 0x04;
			PrintSettleSummary ( ReportOptions );
			return;
		}
		// Set the pointer to the next Acquirer
		pHostPtr = pAcquirerPtr;

		// Reset the Printer Report Options 
		ReportOptions = 0;

	}							// End of while loop

}



//-----------------------------------------------------------------------------
//!  \brief
//!     View the Settlement Status Report.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void ViewSettleSum( void )
{
	UBYTE scroll_cnt;
	UBYTE keyval;


	Enable_Flags[0] = 1;		// Default to Next Arrow ON 
	Enable_Flags[1] = 0;		// Previous Arrow OFF
	Enable_Flags[2] = 0;		// SCROLL Button OFF
	scroll_cnt = 0;				// Reset Scroll Count

	// Find first Acquirer
	pHostPtr = FindFirstAcquirer(  );

	while ( NULL != pHostPtr )
	{
		// Clear The custom buffers
		memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );
		memset( ( UBYTE * ) CSTMSG2, 0, sizeof( CSTMSG2 ) );
		memset( ( UBYTE * ) CSTMSG3, 0, sizeof( CSTMSG3 ) );
		memset( ( UBYTE * ) CSTMSG4, 0, sizeof( CSTMSG4 ) );

		// Build data to Display for each Acquirer
		switch ( scroll_cnt )
		{
			case 0:			// First Scroll Screen
				// Find the Batch Pointer
        		pBatch_Table = FindBatTab( pHostPtr -> AQID );

				// Get the Acquirer Name, Left Justified 
				MoveItAq( CSTMSG, ( pHostPtr->AQNAME ), 10 );

				// Check to see if this Acquirer has settled
				if ( 0 == pBatch_Table->BATSETLINI )
				{
					// Batch has not been settled, turn OFF Scroll Button
					Enable_Flags[2] = 0;

					// Display "NEVER", Right Justified
					GetMsg( Msg_Never, CSTMSG2 );
				}
				else
				{
					// Batch has been settled, Turn ON SCROLL Button
					Enable_Flags[2] = 1;

					// Determine method of Settlement, Right Justified
					switch ( pBatch_Table->BATSETLINI )
					{
						case CLBATCHA:	// Auto Settlement
							GetMsg( Msg_Auto, CSTMSG2 );
							break;

						case SETTLE:	// Group Settlement
							GetMsg( Msg_Group, CSTMSG2 );
							break;

						default:	// Manual Settlement
							GetMsg( Msg_Manual, CSTMSG2 );
							break;
					}
					// Get Length of text string
					length = StrLn( CSTMSG2, sizeof( CSTMSG2 ) );

					// Place the Response Code in the Buffer
					CSTMSG2[length++] = pBatch_Table->BATSETLRSPC[0];
					CSTMSG2[length++] = pBatch_Table->BATSETLRSPC[1];

					// Get Settlement Date, Left Justified
					BfAscii( CSTMSG3, pBatch_Table->BATSETLDAT,
							 S_BATSETLDAT );
					CSTMSG3[8] = ' ';
					CSTMSG3[7] = CSTMSG3[5];
					CSTMSG3[6] = CSTMSG3[4];
					CSTMSG3[5] = '/';
					CSTMSG3[4] = CSTMSG3[3];
					CSTMSG3[3] = CSTMSG3[2];
					CSTMSG3[2] = '/';

					// Get Settlement Time
					BfAscii( &CSTMSG3[9], pBatch_Table->BATSETLTIM,
							 S_BATSETLTIM );
					CSTMSG3[13] = CSTMSG3[12];
					CSTMSG3[12] = CSTMSG3[11];
					CSTMSG3[11] = ':';
					CSTMSG3[14] = '\0';

					// Get Batch Number, convert to ASCII, Right Justified 
					BfAscii( CSTMSG4, pBatch_Table->BATCUR, S_BATCUR );

				}
				break;

			case 1:			// Second Scroll Screen

				// Display "SALES ITEMS", Left Justified
				GetMsg( Msg_SaleItems, CSTMSG );

				// Display Sales count, Right Justified
				BfAscii( CSTMSG2, pBatch_Table->BATSETLTOT.TOTDBCNT,
						 S_TOTDBCNT );
				CSTMSG2[( S_TOTDBCNT * 2 )] = '\0';

				// Display "AMOUNT", Left Justified
				GetMsg( AmountEnt, CSTMSG3 );

				// Display the Sales Total Amount, Right Justified
				CvtAmt( ( char * ) CSTMSG4,
						pBatch_Table->BATSETLTOT.TOTDBAMT );
				break;

			case 2:			// Third Scroll Screen

				// Display "REFUND ITEMS", Left Justified
				GetMsg( Msg_RefundItems, CSTMSG );

				// Display Refund count, Right Justified
				BfAscii( CSTMSG2, pBatch_Table->BATSETLTOT.TOTCRCNT,
						 S_TOTCRCNT );
				CSTMSG2[( S_TOTDBCNT * 2 )] = '\0';

				// Display "AMOUNT", Left Justified
				GetMsg( AmountEnt, CSTMSG3 );

				// Display the Sales Total Amount, Right Justified
				CvtAmt( ( char * ) CSTMSG4,
						pBatch_Table->BATSETLTOT.TOTCRAMT );
				break;

			case 3:			// Fourth Scroll Screen

				// Display "ATTEMPT #", Left Justified
				GetMsg( Msg_Attempt, CSTMSG );
				length = StrLn( CSTMSG, sizeof( CSTMSG ) );

				// Display the number of attempts
				CSTMSG[length++] = Ascii( pBatch_Table->BATSETLTRY );
				CSTMSG[length] = '\0';

				// Display "RESP " and then the Response cose, Right Justified
				GetMsg( Msg_Resp, CSTMSG2 );
				length = StrLn( CSTMSG2, sizeof( CSTMSG2 ) );
				CSTMSG2[length++] = pBatch_Table->BATSETLRSPC[0];
				CSTMSG2[length++] = pBatch_Table->BATSETLRSPC[1];
				CSTMSG2[length] = 0;


				// Get Attempt Date and time, Left Justified
				BfAscii( CSTMSG3, pBatch_Table->BATATTMDAT, S_BATATTMDAT );
				CSTMSG3[8] = ' ';
				CSTMSG3[7] = CSTMSG3[5];
				CSTMSG3[6] = CSTMSG3[4];
				CSTMSG3[5] = '/';
				CSTMSG3[4] = CSTMSG3[3];
				CSTMSG3[3] = CSTMSG3[2];
				CSTMSG3[2] = '/';

				// Get Attempt Time
				BfAscii( &CSTMSG3[9], pBatch_Table->BATATTMTIM,
						 S_BATATTMTIM );
				CSTMSG3[13] = CSTMSG3[12];
				CSTMSG3[12] = CSTMSG3[11];
				CSTMSG3[11] = ':';
				CSTMSG3[14] = '\0';

				// Clear the Right side of the display
				CSTMSG4[0] = '\0';
				break;

			case 4:			// Fifth Scroll Screen

				// Check to see if there is any Host Text to Display
				if ( pBatch_Table->BATSETLTEXT[0] )
				{
					// Get first line of display
					memcpy( CSTMSG, ( pBatch_Table->BATSETLTEXT ), 20 );

					// Get Second line of display
					memcpy( CSTMSG3, &( pBatch_Table->BATSETLTEXT[20] ), 20 );

				}
				else
				{
					// No text, Go to beginning of function
					scroll_cnt = 0;	// Reset Scroll Count
					continue;
				}

		}

		// Display Settlement Status Info Screen  
		DrawScreen( SETTLE_INFO_SCR );

		// Get a key from the user 
		keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

		switch ( keyval )
		{
			case 0:			// Timeout or CANCEL Key
			case CANCEL_KY:
			case CLEAR_KY:
				return;

			case B_PREVIOUS:	// PREVIOUS Button Pressed        
				// Look for Previous Batch 
				pAcquirerPtr = FindPrevAcquirer( pHostPtr );
				if ( NULL != pAcquirerPtr )
				{
					// Set pointer if Acquirer found 
					pHostPtr = pAcquirerPtr;
					Enable_Flags[0] = 1;	// Turn Next Arrow ON 
				}
				else
				{
					Enable_Flags[1] = 0;	// Turn Previous Arrow OFF 
				}
				scroll_cnt = 0;	// Reset Scroll Count
				break;

			case B_NEXT:		// NEXT Button Pressed        
				// Look for Next Batch 
				pAcquirerPtr = FindNextAcquirer( pHostPtr );
				if ( NULL != pAcquirerPtr )
				{
					pHostPtr = pAcquirerPtr;
					Enable_Flags[1] = 1;	// Turn Previous Arrow ON 
				}
				else
				{
					Enable_Flags[0] = 0;	// Turn Next Arrow OFF 
				}
				scroll_cnt = 0;	// Reset Scroll Count
				break;

			case 'S':			// SCROLL Button Pressed
				// Get next data to display
				if ( 4 == scroll_cnt )
				{
					scroll_cnt = 0;	// Reset Scroll Count
				}
				else
					scroll_cnt++;	// Increment Scroll Count
				break;
		}
	}

	// No Acquirer set
	ShowErrMsg( NoAcq );
}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Space fill then NULL terminate each custom buffer.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void PrepBuffers( void )
{
	// Printer is 40 characters wide, so fill 40 spaces
	memset( CSTMSG, ' ',PrintWidth );
	memset( CSTMSG2, ' ',PrintWidth );
	memset( CSTMSG3, ' ',PrintWidth );
	memset( CSTMSG4, ' ',PrintWidth );
	memset( CSTMSG5, ' ',PrintWidth );
	memset( CSTMSG6, ' ',PrintWidth );
	CSTMSG[PrintWidth] = 0;
	CSTMSG2[PrintWidth] = 0;
	CSTMSG3[PrintWidth] = 0;
	CSTMSG4[PrintWidth] = 0;
	CSTMSG5[PrintWidth] = 0;
	CSTMSG6[PrintWidth] = 0;
}



//-----------------------------------------------------------------------------
//!  \brief
//!     This function will take the data in the custom buffer
//!     TEMPCSTMSG, and move it to the custom buffer pointer to
//!     The data will be placed in the left side, centered,
//!     or on the right edge of the printer line.
//!
//!  \param
//!     pBuf     Pointer to the custom buffer to place data in.
//!
//!  \param
//!     Form      Format of the placement, right, left or center
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void PlaceData( char *pBuf, UBYTE Form )
{
	// Select how the the data will be formatted
	switch ( Form )
	{
		case Left:				// Left Justified
			// Just copy data to the start of the buffer, till NULL
			MoveTillZero( pBuf, TEMPCSTMSG, PrintWidth );
			break;

		case Right:			// Right Justified
			// Find out how many characters to set text to right edge
			length = StrLn( TEMPCSTMSG, PrintWidth );

			// Move the data to the offset from the left position
			memcpy( &pBuf[( PrintWidth - length )], TEMPCSTMSG, length );

			break;

		case Center:			// Center Justified
			// Find out how many characters to set text to center
			length = StrLn( TEMPCSTMSG, PrintWidth );

			// Move the data to the offset from the left position
			memcpy( &pBuf[( ( PrintWidth - length ) / 2 )], TEMPCSTMSG,
					length );

			break;

	}
}
