
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             comm.c
//      Modem communications
//
//=============================================================================

#include "string.h"

#include "hw.h"
#include "basictyp.h"
#include "sockets.h"
#include "fixdata.h"
#include "hyputils.h"

#include "struct.h"
#include "appdata.h"
#include "rs232tab.h"
#include "comm_c.h"
#include "comm.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static UBYTE ConnectWait( );
static void HPOSFCError();
static unsigned int StartTimer(unsigned int ms, int *flag);
static enum msg_id  GetStatusMes(UBYTE cCOMStatus);

//=============================================================================
// Public data definitions
//=============================================================================
UDWORD Captured_IPaddr;

extern Bool COM_useSocket;
//=============================================================================
// Private data definitions
//=============================================================================
static int WriteInProgress=0;
static int read_in_progress=0;

//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//  SendMessage     Sends message to communication port
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          HostOSRetval contains return value from this
//                  cross-callable function.
//-----------------------------------------------------------------------------

extern void SendMessage( void )
{
	static int first=0;
	MsgComFlag &= ~REQUEST_SENT;	// REQ is not sent yet

	if ( !ONLINE || COM_useSocket )
	{
		// Set the Acquirer ID for Dial
		DIALAQID = TRREQ.TRAQID;

		// Set the Transaction type for Dial
		DialTranKey = TRREQ.TRKEY;

                first=1;
		// start dialing process
		InitiateDial( );

		// wait for connection to be established
		if ( ConnectWait(  ) !=0 )
		{
			// connect error, HostOSRetval!=orvOK
			// Return to caller
			return;
		}
	}

	// If batch upload is in progress then do not
	// display processing now message.  Leave
	// 'BATCH TRANSFER, WAIT' on the display.
	if ( ( RUPLOAD != TRREQ.TRKEY ) && ( CLBATCHE != TRREQ.TRKEY ) )
	{
		// If Advice or Adjust then do not display Processing Now
		// message
		// if ( ( !( TRREQ.TRSTATUS[0] & TS1_ADV ) ) )
		{
			if ((first)&&(ShowDialStatus))
			{
				// Processing Now
				ShowIntervalMsg( ProcNow, N_Comms );
				first=0;
			}
		}
	}


    // Save IP addr for this connection
	ioctlsocket(0, SIOMYIP, &Captured_IPaddr);


	WriteInProgress=1;
	HostOSRetval = orvOK;
	COM_WriteReq(SENDBUF,SendMsgLen);

	{
		//30 sec timeout for sending message
		unsigned int timerId;
		int timerFlag=1;

		timerId = StartTimer(30000, &timerFlag);
		while ( !COM_WriteCompleted() )
		{
			// See if timeout has occurred
			if ( 0 == timerFlag )
			{
				// Yes; save Timed Out status for the caller
				HostOSRetval = orvDEVICETIMEOUT;
				LNKERR=ERRDTO;
				// ignore further responses
				COM_HangUpReq();
				break;
			}

			// Check if cancelled by user
			if ( OnlineCancelled )
			{
				// Yes; save device fault status for the caller
				HostOSRetval = orvUSER;
				// Hangup 
				COM_HangUpReq();
				break;
			}
			HW_Relinquish(  );
		}
		HW_StopTimer( timerId  );

	}
	WriteInProgress=0;

	if ((HostOSRetval==orvOK)&&(HTENOERROR==COM_HPOSFCERR))
	{
        if (!read_in_progress)
        {
            //start read
            RECVLEN=S_RECVBUF;
            COM_ReadReq(RECVBUF, &RECVLEN);
            read_in_progress=1;
        }
	}
	
	if ((HostOSRetval==orvOK)&&(HTENOERROR!=COM_HPOSFCERR))
	{
		// convert HPOSFC error codes
		HPOSFCError(  );
	}

	if (HostOSRetval==orvOK)
	{
		MsgComFlag |= REQUEST_SENT;	// REQ has been sent
	}
}

//-----------------------------------------------------------------------------
//  RecvMessage     Receive message from Host
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          HostOSRetval contains return value from this
//                  cross-callable function.
//-----------------------------------------------------------------------------
extern void RecvMessage( void )
{
	unsigned int timerId;
	int timerFlag=1;
    int timeout;
	// Response not Received yet
	MsgComFlag &= ~RESPONSE_RECEIVED;

	if ( !ONLINE )
	{
		// Save device not awailable error (offline) status for the caller
		HostOSRetval = orvDEVICEUNAVAILABLE;
		// Return to caller
        read_in_progress=0;
		return;
	}

	// Message was sent OK, now waiting for RSP 
	HostOSRetval = orvOK;

    // Set timeout value for Comms Read
    if ( INITIALIZE == TRREQ.TRKEY)
    {
        // for initialisation
        timeout = bcd2bin (TERM.TERMTOut);
    }
    else if ( NULL != TRREQ.TRAQPTR )
    {
        // for acquirer transaction
        timeout = bcd2bin ( TRREQ.TRAQPTR -> AQTOUT );
    }

    // Validate timeout (must be 2..99 sec)
    if ( ( timeout > 99 ) || ( timeout < 2 ) )
    {
        // set default 30 sec if incorrect value
        timeout = 30;
    }

    // Apply timeout
	timerId = StartTimer( 1000 *  timeout, &timerFlag);

	while ( !COM_ReadCompleted() )
	{
		// See if timeout has occurred
		if ( 0 == timerFlag )
		{
			// Yes; save Timed Out status for the caller
            HostOSRetval = orvDEVICETIMEOUT;
			// ignore further responses
			COM_CancelRead();
			break;
		}

        // Check if cancelled by user
        if ( OnlineCancelled )
        {
            // Yes; save device fault status for the caller
            HostOSRetval = orvUSER;
			// ignore further responses
			COM_CancelRead();
			break;
        }
		HW_Relinquish(  );
	}

	if ((HostOSRetval==orvOK)&&(HTENOERROR==COM_HPOSFCERR))
	{
		// Yes; RSP was Received
		MsgComFlag |= RESPONSE_RECEIVED;
		// Set Received Message Length to read length
		RecvMsgLen = RECVLEN;
	}
	else
	{
		// ignore further responses
		COM_CancelRead();
	}
	
	if ((HostOSRetval==orvOK)&&(HTENOERROR!=COM_HPOSFCERR))
	{
		// convert HPOSFC error codes
		HPOSFCError(  );
	}
	HW_StopTimer( timerId  );

    read_in_progress=0;
}

//-----------------------------------------------------------------------------
//  RequestMessage      Post a read request to the OS for a new message
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:              HostOSRetval contains the return value from this
//                      cross-callable function.
//                      HostOSRetval = orvOK if no error, else error code
//
//-----------------------------------------------------------------------------

extern void RequestMessage( void )
{
	RECVLEN=S_RECVBUF;
	COM_ReadReq(RECVBUF, &RECVLEN);
	HostOSRetval = orvOK;
    read_in_progress=1;
}


//-----------------------------------------------------------------------------
//  CancelDial      Cancels current dialing if possible
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

extern void CancelDial( void )
{
	COM_HangUpReq();		// attempt to cancel dial
}

//-----------------------------------------------------------------------------
//  InitiateDial        Initiates dialing
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void InitiateDial( void )
{
	COM_ConnectReq();

	// Able to make a call
//	HW_Relinquish(); - this is not enough
	HW_Sleep(50);
}



//-----------------------------------------------------------------------------
//  Hang_Up_Pin     Initiate Dial Hangup 
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
extern void Hang_Up_Pin( void )
{
	// If bad PIN returned, don't drop line
	if ( !( TRINP.TRRSPOPT & R_BADPIN ) )
	{
		// Bad PIN not returned, drop line
		COM_HangUpReq();
	}
}



//-----------------------------------------------------------------------------
//  Hang_Up         Initiate Dial Hangup 
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

extern void Hang_Up( void )
{
	COM_HangUpReq();
}


//-----------------------------------------------------------------------------
//  Hang_Up_Soon    Wait a period of time specified in TERM.TERMHold
//                  (0.1 second units), then initiate Dial Hangup.
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

extern void Hang_Up_Soon( void )
{
	COM_HangUpSoonReq();
}


extern UBYTE GetLineStatus( )
{
	return (UBYTE)COM_GetLineStatus();
}

extern UBYTE GetSignalLevel( )
{
	return (UBYTE)COM_GetSignalLevel();
}




//-----------------------------------------------------------------------------
//  ConnectWait     Wait for connection to complete and optionally 
//                  show comm status
//
//  Parameters:     None
//
//  Global Inputs:  Bool DispStatus - display all status changes.
//
//  Returns:        UBYTE   0 for ok, else non-zero  
//
//  Notes:
//
//-----------------------------------------------------------------------------
static UBYTE ConnectWait( )
{
	UBYTE prevComStatus = 0xff;	// Set to invalid COMSTATUS type
	UBYTE curComStatus;
	UBYTE prev_request = 0;
	UBYTE request;
	UBYTE prevGateStaus = 0xff;	// Set to invalid COMSTATUS type
	UBYTE curGateStatus;

	HostOSRetval = orvOK;

	while ( !COM_ConnectCompleted() )	// loop until dial complete
	{
		// If DispStatus is ture or Transation request built
		if ( ShowDialStatus )
		{
			// Yes; display message at change of status
			request = TRREQBLD & BIT_0_SET;
			curComStatus = COM_GetStatus();

			if ( ( curComStatus != prevComStatus ) || ( request != prev_request ) )
			{
				prev_request = request;
				prevComStatus = curComStatus;
				if ( (0 != GetStatusMes(curComStatus)) && (cs_GATECONNECTING != curComStatus))
					ShowIntervalMsg(GetStatusMes(curComStatus),N_Comms);

			}
			if ( (cs_GATECONNECTING == curComStatus) )
			{
				curGateStatus = COM_GetGateStatus();
				if (curGateStatus != prevGateStaus)
				{
				  prevGateStaus = curGateStatus;
				  GetMsg(GetStatusMes(curGateStatus),CSTMSG2);
				  GetMsg( GateConnecting, CSTMSG );
				  ShowInfoMsg( CustomMsg2, N_Comms );
				}
			}
		}

        // Check if cancelled by user
        if ( OnlineCancelled )
        {
            HostOSRetval = orvUSER;
			COM_HangUpReq();
			break;
        }
		HW_Relinquish(  );
	}

	if ((HostOSRetval==orvOK)&&(HTENOERROR!=COM_HPOSFCERR))
	{
		// convert HPOSFC error codes
		HPOSFCError(  );
	}

	if ( HostOSRetval==orvDEVICETIMEOUT )	// timout
	{
		COM_HangUpReq();
	}
	
	if (HostOSRetval!=orvOK )
	{
		return 1;
	}
    read_in_progress=0;
    return 0;
}

/*------------------------------------------------------------------
 *  Name:           CancelComms
 *  Description:    Cancels current comms session
 *  Parameters:     None
 *  Return Value:   None 
 *  Notes:          @TUM This is a new function, called from FlagHost()
 *	when 'Cancel' is pressed by user. According to Apacs40 standard,
 *	user can cancel the transaction at any time.
 *
 *-----------------------------------------------------------------*/
extern void CancelComms(void)
{
    // Do not allow to drop the line while HPOSFC is sending the message
	while (WriteInProgress)
	{
		HW_Relinquish();
	}
	/* Drop the line */
	COM_HangUpReq();
}

//convert HPOSFC error codes to old-style orv* and ERR* errors
void  HPOSFCError()
{
	switch (COM_HPOSFCERR)
	{
	case HTENOERROR:
		HostOSRetval=orvOK;
		LNKERR=0;
		break;
	case HTEINVAL:
		HostOSRetval=orvINVCONFIGURATION;
		LNKERR=ERRCNV;
		break;
	case HTEISCONN:
	case HTEINPROGRESS:
		HostOSRetval=orvDEVICEBUSY;
		LNKERR=ERRBSY;
		break;
	case HTENOTCONN:
		HostOSRetval=orvDEVICENOTOPEN;
		LNKERR=ERRDNO;
		break;
	case HTETIMEOUT:
		HostOSRetval=orvDEVICETIMEOUT;
		LNKERR=ERRDTO;
		break;
	case HTELOSTCOMMS:
		HostOSRetval=orvDCDLOST;
		LNKERR=ERRDCD;
		break;
	case HTENODIALTONE:
		HostOSRetval=orvNODIALLINE;
		LNKERR=ERRNOL;
		break;
	case HTELINEINUSE:
		HostOSRetval=orvNODIALLINE;
		LNKERR=ERRNOL;
		break;
	case HTENOANSWER:
		HostOSRetval=orvNOANSWER;
		LNKERR=ERRNOAN;
		break;
	case HTEBUSY:
		HostOSRetval=orvLINEBUSY;
		LNKERR=ERRLBSY;
		break;
	case HTEBADCHATSCRIPT:
	case HTECOMM:
	case HTECHATFAILED:
	case HTEUNEXPECTED:
	default:
		HostOSRetval=orvUNKNOWNERROR;
		LNKERR=ERRFLT;
		break;
	}
}

static void callback(unsigned int arg)
{
	*(int *)arg=0;
}

unsigned int StartTimer(unsigned int ms, int *flag)
{
	return  HW_StartTimer( HW_MsToTics(ms), 0, callback, (unsigned int)flag);
}



/*------------------------------------------------------------------
 *  Name:           CancelComms   //@AAMELIN
 *  Description:    Return Message code depends of communication status
 *  Parameters:     None
 *  Return Value:   None 
 *  Notes:         
 *
 *-----------------------------------------------------------------*/
static enum msg_id  GetStatusMes(UBYTE cCOMStatus)
{
	// Enhanced dial messages set
        if (TCONF.TCDOPT & TCD_ENH) 
        	switch ( cCOMStatus )
		{
		case cs_ANSWER:	//to maintain similarity to SPOS.
		case cs_DIALING:	// DIALING NOW
			return (DialNow);

		case cs_IDLE:	// "IDLE STATE"
			return( LineIdle );

		case cs_CHECKLINE:	// "checking line"
			return( CheckingLine );

		case cs_WAITLINE:	// "Waiting for line"
			return( WaitForLine );

		case cs_DIALTONE:	// "W. for dialtone" 
			return( WaitDialTone );

		case cs_SENDING:	// Logon to network
			return( NetworkLogon );

		case cs_REDIAL:	// NOW DIALING
			return( NowDialing );

		case cs_INITIALIZING:	// initializing modem
			return( InitModem );

		case cs_PPPCONNECTING:	// Linking...
			return( Linking );

		case cs_TCPCONNECTING:	// Connecting...
			return( Connecting );

		case cs_PAGING:	// Connecting...
			return( Paging );

		case cs_GATECONNECTING:	// Gate connecting...
			return( GateConnecting );
		}
                // Normal (not enhanced) dial messages set
	else 
		switch ( cCOMStatus )
                {
		case cs_WAITLINE:       // Waiting for line
			return( WaitForLine );

		case cs_INITIALIZING:	// Initializing modem
		case cs_DIALTONE:       // Waiting for dialtone 
		case cs_CHECKLINE:      // Checking line
		case cs_IDLE:           // Idle state
		case cs_ANSWER:         // To maintain similarity to SPOS.
		case cs_DIALING:        // DIALING NOW
		case cs_SENDING:        // Logon to network
			return( DialNow );

		case cs_REDIAL:         // Now dialing
			return( NowDialing );

		case cs_PPPCONNECTING:	// Linking...
			return( Linking );
					
		case cs_TCPCONNECTING:	// Connecting...
			return( Connecting );
					
		case cs_PAGING:         // Paging...
			return( Paging );
					
		case cs_GATECONNECTING:	// Gate connecting...
			return( GateConnecting );
		}
	return (0);
}



