//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             hstresp.c
//      Host response checking routines
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "utllib.h"
#include "fixdata.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "perfutil.h"
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


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     Checks Host Response
//!
//! \param
//!		[in]CommReturn			Host Comm function return status
//!
//! \return
//!     True if Host Response received okay else False
//-----------------------------------------------------------------------------
extern Bool HostResp( OS_RETURN_VALS CommReturn )
{
	Bool RespRecv;

	// See if a Response Received
	if ( MsgComFlag & RESPONSE_RECEIVED )
	{
		// Yes; clear unused portion of receive buffer of leftover 
		// data
		memset( &RECVBUF[RecvMsgLen], 0,( UWORD ) ( S_RECVBUF - RecvMsgLen ) );

        // No, Set Host Response received okay flag
        RespRecv = True;
	}
	else
	{
		// No; check if the request packet was built
		if ( MsgComFlag & REQUEST_BUILT )
		{
			// Yes; check Comm function return value and set the Host
			// Response Code to indicate a Comm error
			GetErrResp( CommReturn, &TRRSP );
		}
		else
		{
			// No; set Host Response Code to indicate Unknown 
			// Message/Response
			memcpy( TRRSP.TRRSPC, RSP_UN, S_TRRSPC );
		}

		// Reset Host Response received okay flag
		RespRecv = False;
	}

	// Return Host Response received okay flag
	return ( RespRecv );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets Host Error Response
//!
//! \param
//!		[in]CommReturn			Host Comm function return status
//!		[in]TRLOC				Pointer to Transaction Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void GetErrResp( OS_RETURN_VALS CommReturn, struct trandata_rec *TRLOC )
{
	// Check Comm function return value and set the Host Response Code 
	// to indicate a Comm error
	switch ( CommReturn )
	{
		case orvUSER:           // Cancelled by user
            memcpy( TRLOC->TRRSPC, RSP_UC, S_TRRSPC );
			break;

        case orvDEVICETIMEOUT:
		case orvXMODEMTIMEOUT:	// Response Timed Out
             memcpy( TRLOC->TRRSPC, RSP_TO, S_TRRSPC );

			// If  Performance Report, enable 
			if ( TCONF.TCOPT7 & TC7_PERFORMANCE ) 	
			{
				// Set Timeout for Performance Report (Statistic) 
				Data_kind = TTIMEOUT;	   		 
	    
				// Increase counter for Transaction Timeout- Performance Report
				IncrementCnt ();
			}

			break;

		case orvDCDLOST:		// Carrier Lost
			memcpy( TRLOC->TRRSPC, RSP_LC, S_TRRSPC );
			break;

		case orvDEVICEDISCONNECT:	// Device Disconnected
//			memcpy( TRLOC->TRRSPC, RSP_CE, S_TRRSPC );
            memcpy( TRLOC->TRRSPC, RSP_NL, S_TRRSPC );
			break;

		case orvLINEBUSY:		// Line busy
			memcpy( TRLOC->TRRSPC, RSP_LB, S_TRRSPC );
			break;

		case orvNOANSWER:		// No answer
			memcpy( TRLOC->TRRSPC, RSP_NANSW, S_TRRSPC );
			break;

		default:				// Response Comms Error
            switch ( LNKERR )
            {
                case ERRNOL:        // No line
                    memcpy( TRLOC->TRRSPC, RSP_NL, S_TRRSPC );
                    break;

                case ERRLBSY:       // Busy tone detected
                    memcpy( TRLOC->TRRSPC, RSP_LB, S_TRRSPC );
                    break;

                case ERRNOAN:       // No answer
                    memcpy( TRLOC->TRRSPC, RSP_NANSW, S_TRRSPC );
                    break;

                default:            // CE - communication error
                    memcpy( TRLOC->TRRSPC, RSP_CE, S_TRRSPC );
                    break;
            }
			break;
	}
}

//=============================================================================
// Private function definitions
//=============================================================================

