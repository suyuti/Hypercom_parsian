
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     autosetl.c
//  Function used to implement the Auto Settle Transaction
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "fixdata.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "tables.h"

#include "autosetl.h"

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
//  AutoSettle      Auto Settle at a specified time or forced by App Manager
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        True
//                  False
//
//  Notes:          Entered once a second, when idling
//                  checks settle time once a minute.
//                  Must be in same page as hypertcf.c. 
//
//-----------------------------------------------------------------------------

extern Bool AutoSettle( void )
{
	Bool retval;
        UBYTE i;
	UBYTE RtcBuffer[RTCLEN];	// RTC Read Buffer 
	UBYTE RtcBcd[5];			// Current YYMMDDHHMM in BCD.  
	UBYTE AcqBcd[2];			// Acquirer hour and minute in BCE. 
	UBYTE temp_acq[4];			// Temp storage of acquirer time.   
        Bool Forced_by_AppMngr = False;  // "Forced by App Manager" flag
	static UBYTE prev_min[2];
	struct acquirer_rec *pAcquirerPtr;	// Pointer to AQTAB entry  
	struct batch_rec *pBatchRec;

	// Preset return value. 
	retval = False;

	// Read the real time clock.  
	SDK_RtcRead( RtcBuffer );

	// If read minute is different from previous then check times. 
	if ( memcmp( prev_min, &RtcBuffer[8], 2 ) )
	{
		// Read is different than previous. Save new minute.  
		prev_min[0] = RtcBuffer[8];
		prev_min[1] = RtcBuffer[9];

		// Convert YYMMDDHHMM to BCD for memcmpes with acquirer times.  
		AscHex( RtcBcd, ( char * ) &RtcBuffer[0], 5 );

		pAcquirerPtr = FindFirstAcquirer(  );

		// If acquirer exists see if time to settle.  
		while ( NULL != pAcquirerPtr )
		{
			// Locate the batch table for this acquirer. 
			pBatchRec = FindBatTab( pAcquirerPtr->AQID );

			// Force acquirer settle time to numeric. 
			// Set invalid time characters to zero.      
			MoveItAq( temp_acq, pAcquirerPtr->AQSETLTIME, 4 );
			for ( i = 0; i < 4; i++ )
			{
				if ( ( temp_acq[i] = ( temp_acq[i] & 0x0f ) ) > 9 )
					temp_acq[i] = '0';
				temp_acq[i] = ( temp_acq[i] | '0' );
			}

			// Convert acquirer settle time to BCD for memcmpes.  
			 AscHex( AcqBcd, ( char * ) temp_acq, 2 );

			if ( Forced_by_AppMngr || ( !NullComp( ( char * ) AcqBcd, 2 ) ) )
            {
				// If hour has past or is equal keep checking.  
				if ( Forced_by_AppMngr || (  memcmp( &RtcBcd[3], &AcqBcd[0], 1 ) >=0 ) )
				{
					// If hour has past or hour is the same and minute has passed. 
					if ( Forced_by_AppMngr || 
                        ( (  memcmp( &RtcBcd[3], &AcqBcd[0], 1 ) >0 ) ||
                          ( (  memcmp( &RtcBcd[3], &AcqBcd[0], 1 ) ==0 ) &&
                           ( (  memcmp( &RtcBcd[4], &AcqBcd[1], 1 ) >=0 ) ) ) ) )
					{
						// See if batch has been settled today. 
						if ( Forced_by_AppMngr || ( memcmp( &RtcBcd[0], pBatchRec->BATSETLDAT, 3 ) ) )
						{
							// Not settled yet today. Test retry timer. 
							if ( pBatchRec->BATSETLIDL )
							{
								// Decrement idle count.  
								pBatchRec->BATSETLIDL--;
								SetBATTabLRC(  );
							}
							else
							{
								// Don't settle if retries equal or more than host allows. 
								if ( !( pAcquirerPtr->AQSMAXTRIES ) )
								{
									// No max - keep trying forever. 
									retval = True;
								}
								else
								{
									if ( pBatchRec->BATSETLTRY < pAcquirerPtr->AQSMAXTRIES )
									{
										retval = True;
									}

								}

								if ( True == retval )
								{
									// Finally - Have one to settle.  
									// Move current batch number to TRINP.  
									BfAscii( ( char * ) TRINP.TRBATNO,
											 pBatchRec->BATCUR, S_BATCUR );
									TRINP.TRAQID = pAcquirerPtr->AQID;
									TRINP.TRAQPTR = pAcquirerPtr;
									TRINP.TRKEY = CLBATCHA;
									break;
								}
							}

						}
					}

				}
			}

			// Settle next acquirer if it exists 
			pAcquirerPtr = FindNextAcquirer( pAcquirerPtr );

        }
	}
	return ( retval );
}

//=============================================================================
// Private function definitions
//=============================================================================
