
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             host.c
//      Host control module
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "fixdata.h"
#include "utllib.h"

#include "defines.h"
#include "transdef.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "i_host.h"
#include "tables.h"
#include "host.h"
#include "comm.h"
#include "init.h"
#include "hstmsg.h"
#include "hstresp.h"

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
UBYTE CurrentAqid;				// Active Acquirer's ID

UBYTE TmpBuf[256];				// Temporary data storage for host functions
HOSTENTRY *Host;
UBYTE *pRecvData;

//=============================================================================
// Private data definitions
//=============================================================================
static const char DEMONAME[] = "DEMO      ";

//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//  INITHOST        Host Process
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          This Procedure should loop forever!!
//                  Its a process.
//-----------------------------------------------------------------------------

extern void INITHOST( void )
{
	HOSTENTRY *LastHostEntry;
	Bool Found;
	char *WantedHost;
	UBYTE tmpacq[S_AQPNAME];

	// Make sure we are enabled
	while ( !ENAPPL.initialized )
	{
		SDK_RelinqCPU(  );
	}

	// Clear Control Flag
	TRREQBLD = 0;

	while ( True )
	{
		// See if a Transaction is pending
		if ( TRREQBLD & BIT_0_SET )
		{
			// Are we in Training Mode?
			if ( TCONF.TCOPT4 & TC4_TRAINING )
			{
				// Yes; use the Demo Host
				WantedHost = ( char * ) DEMONAME;
			}
			else
			{
				// No; check if INITIALIZE message
				if ( TRINP.TRKEY == INITIALIZE )
				{
					// Yes; process INITIALIZE message
					Init ();

					// Wait for next pending Transaction
					continue;
				}
				else if ( TRINP.TRKEY == ICEPAK_DOWNLOAD )
				{
					DownloadIcePak ();
					continue;
				}
				else
				{
					// No; use the Acquirer Host
					MoveItAq( tmpacq, TRINP.TRAQPTR->AQPNAME, S_AQPNAME);
					WantedHost = ( char * ) tmpacq;
				}
			}

			// Reset host found flag
			Found = False;

			// Initialize pointer to Host entry
			// and search the Host entry table

			Host = ( HOSTENTRY * ) VHDT_HOST_TBL1;
			LastHostEntry = ( HOSTENTRY * ) ( &HOST_MARKER1 );

			while ( Host != LastHostEntry )
			{
				// See if it matches
				if ( memcmp( ( UBYTE * ) Host->HostName,
							 ( UBYTE * ) WantedHost, S_AQPNAME ) )
				{
					// No; point to next host
					Host++;
				}
				else
				{
					// Yes; found a match
					Found = True;

					// Exit the loop
					break;
				}
			}

			// See if the host name has been found
			if ( Found )
			{
				// Yes; clear the request and response buffers
				memset( ( UBYTE * ) & TRREQ, 0,TRSZE );
				memset( ( UBYTE * ) & TRRSP, 0,TRSZE );

				// Clear Additional Host Print Data Buffer too
				memset( ( UBYTE * ) & HSTPRDATA, 0,S_HSTPRDATA );

				// Save Acquirer ID for Host routines
				CurrentAqid = TRINP.TRAQID;

				// Call selected host
				Host->pHostFunc(  );
			}
			else
			{
                // Yes; set in a Bad Host Response Code
				memcpy( TRINP.TRRSPC, RSP_BH, S_TRRSPC );

				// Look up Response in TRINP
				RspLookUp( &TRINP );

				// Hang up now in case predialed.
				Hang_Up ();

				// Flag Complete
				TRRSPREADY |= BIT_1_SET;
            }
			// Clear Request Flag.
			TRREQBLD &= ~BIT_0_SET;

		}

		// Release Control
		SDK_RelinqCPU(  );
	}
}



//-----------------------------------------------------------------------------
//  HostList        Report Host Names
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          This Procedure uses TRINP to hold the list of
//                  Host Names in printable format.
//
//-----------------------------------------------------------------------------

extern void HostList( void )
{
	HOSTENTRY *Host;
	HOSTENTRY *LastHostEntry;
    char *pHostname;

	// Clear the TRINP structure
	memset( ( UBYTE * ) & TRINP, 0,TRSZE );

	// Set pointer to the TRINP structure
	pHostname = ( char * ) &TRINP;

	// Initialize pointer to Host entry
	// and search the Host entry table

	Host = ( HOSTENTRY * ) VHDT_HOST_TBL1;
	LastHostEntry = ( HOSTENTRY * ) ( &HOST_MARKER1 );

	while ( Host != LastHostEntry )
	{
		// Space over
		memset( ( char * ) pHostname, ' ',5 );

		// Insert Host Name
		memcpy( &pHostname[5], Host->HostName, 10 );

		// Add linefeed
		pHostname[15] = '\n';

		// Update List pointer
		pHostname += 16;

		// Point to next host
		Host++;
	}

	// leave pHostname for second & third host pages
}

//=============================================================================
// Private function definitions
//=============================================================================
