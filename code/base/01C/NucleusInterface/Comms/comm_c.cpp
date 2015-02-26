/*=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// \file
//      comm_c.cpp
//
// \brief
//      Implmentation of ThreadTransport class
//
*/
#include <HypCPPP.hpp>
#ifdef MAKE_M2
#include <HypCGPRS.hpp>
#endif
#ifdef MAKE_SSL
#include <HypCSSLSocket.hpp>
#include <HypCCertificateProvider.hpp>
#endif // MAKE_SSL

#include "comm_c.hpp"
extern "C" {
#include "comdata.h"
#include "aqtab.h"
#include "findtran.h"
#include <sysstruc.inc>
}
#include <BCDUtil.h>
#include "DbgTrace.h"
#include <HypCModemAAE.hpp>
#include <HypCInternalModem.hpp>

//=============================================================================
// External variables / definitions
//=============================================================================
//! transport thread is running
Bool TRANSPORT_IS_RUNNING = True;

//=============================================================================
// External variables / declarations
//=============================================================================
//! receive message lenght
extern UWORD RecvMsgLen;
//! transaction data entry
extern trandata_rec TRINP;
//! current request message
extern trandata_rec TRREQ;
//! current response message
extern trandata_rec TRRSP;

static int AddPBAX(char *phoneStr);

//=============================================================================
// Private defines and typedefs
//=============================================================================

//=============================================================================
// Private function declarations
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Constructor
//!
//! \param
//!      ev				An event handler
//! \param
//!      protocol		Protocol type
//!
//! \return
//!      None
//!
//! \note
//!		The tread starts automatically
//!
ThreadTransport::ThreadTransport( HypCEvent& ev, HypEProtocol protocol ) : 
								  HypCThread( "Initcomm", 8 * 1024 ),
								  m_ev( ev ),
								  m_paq( NULL ),
								  m_Transport( DEVICE_ID_HOST_COMM ),
								  m_timer(m_event),
								  m_protocol( protocol ),
								  m_PPP( NULL )
#ifdef MAKE_M2
								  ,m_GPRS( NULL )
#endif
{
	GetThreadInstance( this );
	Init_COMM();
    Resume();
}

//-----------------------------------------------------------------------------
//!
//!      Destructor
//!
//! \param
//!      None
//!
//! \return
//!      None
//!
//! \note
//!		
//!
ThreadTransport::~ThreadTransport()
{
	delete &m_ev;
	delete m_PPP;
#ifdef MAKE_M2
	delete m_GPRS;
#endif
}

//-----------------------------------------------------------------------------
//!
//!      Get pointer to the communications process.
//!
//! \param
//!      this_		Pointer to itself
//!
//! \return
//!      Pointer to instance of the transport object
//!
//! \note
//!		This function is to be call from constructor the first of all
//!
ThreadTransport* ThreadTransport::GetThreadInstance( HypCThread* this_ )
{
	static HypCThread* curObj = NULL;
	if( NULL != this_ && NULL == curObj )
	{
		curObj = this_;
	}
	return static_cast<ThreadTransport*>(curObj);
}

//-----------------------------------------------------------------------------
//!
//!      Reset connect request
//!
//! \param
//!
//! \return
//!
//! \note
//!
void ThreadTransport::COM_ResetReq_( )
{
	m_event.Send( USER_COM_RESET_REQ );
	m_resetInProgress = true;
}

//-----------------------------------------------------------------------------
//!
//!      Sends message to communication port
//!
//! \param
//!
//! \return
//!
//! \note
//!
void ThreadTransport::SendMessage_( )
{
	static int first=0;
	MsgComFlag &= ~REQUEST_SENT;	// REQ is not sent yet

	if ( !m_online )
	{
		// Set the Acquirer ID for Dial
		DIALAQID = TRREQ.TRAQID;

		// Set the Transaction type for Dial
		DialTranKey = TRREQ.TRKEY;

        first=1;

		// start dialing process
        do
        {
            InitiateDial_( );

            // wait for connection to be established
            if ( ConnectWait( ) )
            {
                // connect m_error, HostOSRetval!=orvOK
                // Return to caller or try backup link
                if( !IsDialBackup() )
                {
                    return;
                }
            }
            else
            {
                // Connected!
                break;
            }
        } while ( true );
	}

	// If batch upload is in progress then do not
	// display processing now message.  Leave
	// 'BATCH TRANSFER, WAIT' on the display.
	if ( ( RUPLOAD != TRREQ.TRKEY ) && ( CLBATCHE != TRREQ.TRKEY ) )
	{
		// If Advice or Adjust then do not display Processing Now
		// message
		if ( ( !( TRREQ.TRSTATUS[0] & TS1_ADV ) ) &&
			 ( !( TRREQ.TRSTATUS[0] & TS1_ADJ ) ) )
		{
			if ((first)&&(ShowDialStatus))
			{
				// Processing Now
				ShowIntervalMsg( ProcNow, N_Comms );
				first=0;
			}
		}
	}

	HostOSRetval = orvOK;
	COM_WriteReq( SENDBUF,SendMsgLen );

	//30 sec timeout for sending message
	unsigned int timerId = 0;
    // Apply timeout
	HypCEvent ev;
	HypCEventInfo evInfo;
	HypCTimer tmpTimer( ev, timerId );
	tmpTimer.Start(	30000 );
	while ( !COM_WriteCompleted() )
	{
		// See if timeout has occurred
		if ( ev.Wait( evInfo, NU_NO_SUSPEND ) )
		{
			// Yes; save Timed Out m_status for the caller
			HostOSRetval = orvDEVICETIMEOUT;
			LNKERR=ERRDTO;
			// ignore further responses
			COM_HangUpReq( );
			break;
		}

		// Check if cancelled by user
		if ( OnlineCancelled )
		{
			// Yes; save device fault m_status for the caller
			HostOSRetval = orvUSER;
			// Hangup 
			COM_HangUpReq();
			break;
		}
		Relinquish( );
	}
	tmpTimer.Stop();

	if( ( HostOSRetval == orvOK ) && ( HTENOERROR == m_error ) )
	{
            //start read
		RECVLEN = S_RECVBUF;
        COM_ReadReq( RECVBUF, &RECVLEN );
	}
	
	if( ( HostOSRetval == orvOK ) && ( HTENOERROR != m_error ) )
	{
		// convert HPOSFC m_error codes
		HPOSFCError(  );
	}

	if( HostOSRetval == orvOK )
	{
		MsgComFlag |= REQUEST_SENT;	// REQ has been sent
	}
}

//-----------------------------------------------------------------------------
//!
//!      Receive message from Host
//!
//! \param
//!
//! \return
//!
//! \note
//!
void ThreadTransport::RecvMessage_( )
{
	unsigned int timerId = 0;
    int timeout = 30;
	// Response not received yet
	MsgComFlag &= ~RESPONSE_RECEIVED;

	if ( !m_online )
	{
		// Save device not awailable m_error (offline) m_status for the caller
		HostOSRetval = orvDEVICEUNAVAILABLE;
		return;
	}

	// Message was sent OK, now waiting for RSP 
	HostOSRetval = orvOK;

    // Set timeout value for Comms Read
    if ( INITIALIZE == TRREQ.TRKEY ) // 0x0005 - Terminal initialization
    {
        // for initialisation

		const uint8_t timeOut = TERM.TERMTOut;
        timeout = BcdBin( static_cast<const uint8_t* const>(&timeOut) );
    }
    else if ( NULL != TRREQ.TRAQPTR )
    {
        // for acquirer transaction
        timeout = BcdBin( static_cast<const uint8_t* const>(&TRREQ.TRAQPTR->AQTOUT) );
    }

    // Validate timeout (must be 2..99 sec)
    if ( ( timeout > 99 ) || ( timeout < 2 ) )
    {
        // set default 30 sec if incorrect value
        timeout = 30;
    }

    // Apply timeout
	HypCEvent ev;
	HypCEventInfo evInfo;
	HypCTimer tmpTimer( ev, timerId );
	tmpTimer.Start(	1000 * timeout );

	while ( !COM_ReadCompleted( ) )
	{
		// See if timeout has occurred
		if ( ev.Wait( evInfo, NU_NO_SUSPEND ) )
		{
			// Yes; save Timed Out m_status for the caller
            HostOSRetval = orvDEVICETIMEOUT;
			// ignore further responses
			COM_CancelRead( );
			break;
		}

        // Check if cancelled by user
        if ( OnlineCancelled )
        {
            // Yes; save device fault m_status for the caller
            HostOSRetval = orvUSER;
			// ignore further responses
			COM_CancelRead( );
			// Hangup 
			COM_HangUpReq();
			break;
        }
		Relinquish(  );
	}

	if( ( HostOSRetval == orvOK ) && ( HTENOERROR == m_error ) )
	{
		// Yes; RSP was Received
		MsgComFlag |= RESPONSE_RECEIVED;
		// Set Received Message Length to read length
		RecvMsgLen = RECVLEN;
	}
	else
	{
		// ignore further responses
		COM_CancelRead( );
		// Hangup 
		COM_HangUpReq();
	}
	
	if ((HostOSRetval==orvOK)&&(HTENOERROR!=m_error))
	{
		// convert HPOSFC m_error codes
		HPOSFCError(  );
	}
	tmpTimer.Stop( );
}

//-----------------------------------------------------------------------------
//!
//!      Post a read request for a new message
//!
//! \param
//!
//! \return
//!
//! \note
//!
void ThreadTransport::RequestMessage_( )
{
	RECVLEN = S_RECVBUF;
	COM_ReadReq( RECVBUF, &RECVLEN );
	HostOSRetval = orvOK;
}

//-----------------------------------------------------------------------------
//!
//!      Initiates dialing
//!
//! \param
//!
//! \return
//!
//! \note
//!
void ThreadTransport::InitiateDial_( )
{
	COM_ConnectReq( );
}

//-----------------------------------------------------------------------------
//!
//!      Cancels current comms session
//!
//! \param
//!
//! \return
//!
//! \note
//!
void ThreadTransport::CancelComms_( )
{
    // Do not allow to drop the line while HFC is sending the message
	while( m_writeInProgress )
	{
		Relinquish( );
	}
	/* Drop the line */
	COM_HangUpReq( );
}

//-----------------------------------------------------------------------------
//!
//!      Initiate Dial Hangup for PIN entering
//!
//! \param
//!
//! \return
//!
//! \note
//!
void ThreadTransport::Hang_Up_Pin_( void )
{
    // If bad PIN returned, don't drop line
	if ( !( TRINP.TRRSPOPT & R_BADPIN ) )
	{
		// Bad PIN not returned, drop line
		COM_HangUpReq( );
	}
}

//-----------------------------------------------------------------------------
//!
//!      Initiate Dial Hangup 
//!
//! \param
//!
//! \return
//!
//! \note
//!
void ThreadTransport::Hang_Up_( void )
{
	COM_HangUpReq( );
}

//-----------------------------------------------------------------------------
//!
//!      Initiate Dial Hangup request
//!
//! \param
//!
//! \return
//!
//! \note
//!		Wait a period of time specified in TERM.TERMHold
//!     (0.1 second units), then initiate Dial Hangup.
//!
void ThreadTransport::Hang_Up_Soon_( )
{
	COM_HangUpSoonReq( );
}

//-----------------------------------------------------------------------------
//!
//!      Gets line status
//!
//! \param
//!
//! \return
//!		Line state as HypCTransport::HypETransportState enum
//!
//! \note
//!
HypCTransport::HypETransportState ThreadTransport::GetLineStatus_( )
{
	return COM_GetLineStatus();
}

//-----------------------------------------------------------------------------
//!
//!      Resets comms module
//!
//! \param
//!
//! \return
//!
//! \note
//!
void ThreadTransport::ResetCommModule_( )
{
	COM_ResetReq_( );
}

//-----------------------------------------------------------------------------
//!
//!      Get comms reset satus
//!
//! \param
//!
//! \return
//!		true in case if reset is completed
//!
//! \note
//!
bool ThreadTransport::COM_ResetCompleted_( )
{
	return !m_resetInProgress;
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Entry point for the communications process.
//!
//! \param
//!      None
//!
//! \return
//!      None
//!
//! \note
//!		This function is the main function of the
//!		thread object. It is called inmediately after object initialization.
//!
void ThreadTransport::Execute()
{
	// Wait until applications are enabled
	while ( !( ENAPPL.initialized ) )
	{
		Relinquish( );
	}

	Init_COMM();
	
	while( true )
	{
		switch( m_state )
		{
		case ST_IDLE:
			StIdle();
			break;
		case ST_CONNECTING:
			StConnecting();
			break;
		case ST_CONNECTED:
			StConnected();
			break;
		case ST_HANGUP_SOON:
			StHangUpSoon();
			break;
		}
		Relinquish();
	}
}

//-----------------------------------------------------------------------------
//!
//!      Connect request
//!
//! \param
//!      None
//!
//! \return
//!      None
//!
//! \note
//!
void ThreadTransport::COM_ConnectReq()
{
    while( m_hangupInProgress || m_hangupSoonInProgress ) 
    {
		Relinquish( );       
	}

	if( !m_connectInProgress && ( ( m_state == ST_IDLE ) ) )
	{
		m_event.Send( USER_CONNECT_REQ );
		m_connectInProgress = true;
	}
}

//-----------------------------------------------------------------------------
//!
//!      Is connect completed?
//!
//! \param
//!      None
//!
//! \return
//!      true in case if connect to Host is completed
//!
//! \note
//!
bool ThreadTransport::COM_ConnectCompleted()
{
	return !m_connectInProgress;
}

//-----------------------------------------------------------------------------
//!
//!      Hung up request
//!
//! \param
//!      None
//!
//! \return
//!
//! \note
//!
void ThreadTransport::COM_HangUpReq()
{
	if( !m_hangupInProgress )
	{
		COM_CloseSessionReq();
		m_event.Send( USER_DISCONNECT_NOW_REQ );
	}
}

//-----------------------------------------------------------------------------
//!
//!      Close session request
//!
//! \param
//!      None
//!
//! \return
//!
//! \note
//!
void ThreadTransport::COM_CloseSessionReq()
{
	if( !m_closeSessionInProgress )
	{
		m_event.Send( USER_CLOSE_SESSION_REQ );
		m_closeSessionInProgress = true;
	}
}

//-----------------------------------------------------------------------------
//!
//!      Close session completed?
//!
//! \param
//!      None
//!
//! \return
//!      true in case if connect to Host is completed
//!
//! \note
//!
bool ThreadTransport::COM_CloseSessionCompleted()
{
	return m_closeSessionInProgress;
}

//-----------------------------------------------------------------------------
//!
//!      Hung up soon request
//!
//! \param
//!      None
//!
//! \return
//!
//! \note
//!
void ThreadTransport::COM_HangUpSoonReq()
{
	if( !( m_hangupSoonInProgress || m_hangupInProgress ) )
	{
		m_event.Send( USER_DISCONNECT_SOON_REQ );
		m_hangupSoonInProgress=true;
	}
}

//-----------------------------------------------------------------------------
//!
//!      Is hung up completed?
//!
//! \param
//!      None
//!
//! \return
//!      true in case if hung up is completed
//!
//! \note
//!
bool ThreadTransport::COM_HangUpCompleted()
{
	return !( m_hangupSoonInProgress || m_hangupInProgress );
}

//-----------------------------------------------------------------------------
//!
//!      Write request
//!
//! \param
//!      buf		Pointer to write buffer
//! \param
//!      len		Lenght of the write buffer
//!
//! \return
//!      None
//!
//! \note
//!
void ThreadTransport::COM_WriteReq( const void* buf, unsigned int len )
{
	if( true == m_writeInProgress )
	{
		return;
	}
	m_writeInProgress = true;
	if( m_state != ST_CONNECTED )
	{
		m_writeInProgress = false;
		m_error = HTENOTCONN;
		return;
	}
	if( m_error != HTENOERROR )
	{
		m_writeInProgress=false;
		return;
	}
	m_writeBuf = ( const char * )buf;
	m_writeLen = len;
	m_event.Send( USER_TRANSMIT_MSG_REQ );
}

//-----------------------------------------------------------------------------
//!
//!      Is write completed?
//!
//! \param
//!
//! \return
//!      true in case if write is completed
//!
//! \note
//!
bool ThreadTransport::COM_WriteCompleted()
{
	return !m_writeInProgress;
}

//-----------------------------------------------------------------------------
//!
//!      Read request
//!
//! \param
//!      buf		Pointer to read buffer
//! \param
//!      len		Lenght of the read buffer
//!
//! \return
//!
//! \note
//!
void ThreadTransport::COM_ReadReq( void* buf , unsigned short* len )
{
	if( m_readInProgress == true )
	{
		return;
	}
	m_readInProgress = true;

	if( m_state != ST_CONNECTED )
	{
		m_readInProgress=false;
		m_error=HTENOTCONN;
		return;
	}
	m_Transport.Transport( )->ReadReq( buf, *len, m_event );//READ_REQUEST
}

//-----------------------------------------------------------------------------
//!
//!      Is read request completed?
//!
//! \param
//!
//! \return
//!		true in case if read is completed
//!
//! \note
//!
bool ThreadTransport::COM_ReadCompleted()
{
	return !m_readInProgress;
}

//-----------------------------------------------------------------------------
//!
//!      Cancel read
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
void ThreadTransport::COM_CancelRead()
{
	m_readInProgress = false;
}

//-----------------------------------------------------------------------------
//!
//!      Init transport object
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
void ThreadTransport::Init_COMM()
{
	m_online = false;
	m_state = ST_IDLE;
	m_connectInProgress = false;
	m_hangupInProgress = false;
	m_closeSessionInProgress = false;
	m_hangupSoonInProgress = false;
	m_resetInProgress = false;
	m_writeInProgress = false;
	m_readInProgress = false;
	m_ExistBackup = false;
	m_commStatus = cs_IDLE;
	m_error = HTENOERROR;
}

//-----------------------------------------------------------------------------
//!
//!      Free transport object
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
void ThreadTransport::Free()
{
	if( m_Transport.Transport() )
	{
		m_Transport.ResetTransport();
	}
}

//-----------------------------------------------------------------------------
//!
//!      State "idle" of the state machine
//!
//! \param
//! 
//! \return
//!		None
//!
//! \note
//!
void ThreadTransport::StIdle()
{
	while( true )
	{
		// Wait for a command from the user process and then process it.
		if ( true == WaitEvent() )
		{
			//event allready proccesed,
			return;
		}
		if( m_lastEvent == USER_CONNECT_REQ )
		{
			if( CommConfig() != HTENOERROR )
			{
				m_connectInProgress = false;
				return;
			}

			// start dialing
			if( !m_Transport.Transport()->ConnectReq( m_event ) )
			{
				// link layer error reporting to StConnecting
				m_event.SendToFront( CONNECTING_ERROR, m_Transport.Transport()->GetError() );
			}

			// start conect timeout timer
			m_timer.Start( m_timeOfConnect, CONNECT_TIMER_EXPIRED );
			m_commStatus = cs_DIALING;
			m_state = ST_CONNECTING;
			return;				//transaction throw socket interface
		}
	}
}

//-----------------------------------------------------------------------------
//!
//!      State "connecting" of the state machine
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
void ThreadTransport::StConnecting()
{
	while (true)
	{
		// Wait for a command from the user process and then process it.
		if( WaitEvent() == true )
		{
			//event allready proccesed,
			return;
		}

		switch( m_lastEvent )
		{
		case COMM_CONNECTED:
			m_timer.Stop();
			m_commStatus = cs_RECEIVING;
			m_error = HTENOERROR;
			m_state = ST_CONNECTED;
			m_connectInProgress = false;
			m_online = true;
			m_ExistBackup = false;
			return;
		case CONNECTING_ERROR:
			m_error = m_Transport.Transport()->GetError();
			DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Connecting error = %d\n", __FUNCTION__, m_error );
			HangUp();
			return;
		case CONNECT_TIMER_EXPIRED:
			HangUp();
			m_error = HTETIMEOUT;
			DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Connecting timeout = %d\n", __FUNCTION__, m_error );
			return;
		default:
			continue;
		}
	}
}

//-----------------------------------------------------------------------------
//!
//!    Check if dial backup is set
//!
//! \param
//!		none
//!
//! \return
//!		none
//!
//! \note
//!     true    if dial backup exist
bool ThreadTransport::IsDialBackup()
{
    if( m_ExistBackup == true )
    {
        m_ExistBackup = false;
        return m_ExistBackup;
    }
    m_ExistBackup = false;

    if( m_moduleType == HW_MODEM )
    {
        // if previous connection was dialup
        DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Perious connection was dialup.\n", __FUNCTION__ );
        return m_ExistBackup;
    }

    int dest = GetNumDialDest();
    if( dest == 0 ) 
    {
        // none of destination was defined
        return m_ExistBackup; 
    }
    if( !(m_paq->AQOPT5 & AQ5_DBPHONE1) )
    {
        // backup is not set
        DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Backup mode is not set\n", __FUNCTION__ );
        return m_ExistBackup;
    }
    DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Backup mode is set\n", __FUNCTION__ );
    m_ExistBackup = true;
    return m_ExistBackup; 
}

//-----------------------------------------------------------------------------
//!
//!      State "connected" of the state machine
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
void ThreadTransport::StConnected() 
{
	while (true)
	{
		// Wait for a command from the user process and then process it.
		if( WaitEvent() == true )
		{
			//event allready proccesed,
			return;
		}
		switch( m_lastEvent )
		{
		case USER_TRANSMIT_MSG_REQ:
			{
				m_Transport.Transport()->WriteReq( m_writeBuf, m_writeLen, m_event );
				return;
			}

		case MSG_TRANSMITTED:
			m_error = HTENOERROR;
			m_writeInProgress = false;
			//event allready proccesed,
			return;

		case WRITE_ERROR:
			m_error = m_Transport.Transport()->GetError();
			m_writeInProgress = false;
			//event allready proccesed,
			return;

		case DATA_RECEIVED:
		case READ_ERROR:
			m_error = m_Transport.Transport()->GetError();
			m_readInProgress = false;
			//event allready proccesed,
			return;

		case USER_CLOSE_SESSION_REQ:
			m_error = m_Transport.Transport()->GetError();
			m_Transport.Transport( )->DisconnectReq( m_event );
			m_closeSessionInProgress = false;
			//event allready proccesed,
			return;

		case USER_DISCONNECT_SOON_REQ:
			m_timer.Stop();
			m_timer.Start( 1000, HANG_UP_SOON_TIMER_EXPIRED );
			m_state = ST_HANGUP_SOON;
			//event allready proccesed,
			return;

		default:
			//event allready proccesed,
			return;
		}
	}
}


//-----------------------------------------------------------------------------
//!
//!      State "hung up soon" of the state machine
//!
//! \param
//! 
//! \return
//!		
//!
//! \note
//!
void ThreadTransport::StHangUpSoon()
{
	while (true)
	{
		// Wait for a command from the user process and then process it.
		if (WaitEvent() == true)
		{
			//event allready proccesed,
			return;
		}
		if (m_lastEvent == HANG_UP_SOON_TIMER_EXPIRED)
		{
			HangUp();
			DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: HANG_UP_SOON_TIMER_EXPIRED = %d\n", __FUNCTION__, 0 );
			m_error = HTENOERROR;
			return;
		}
	}
}

//-----------------------------------------------------------------------------
//!
//!      Wait event
//!
//! \param
//! 
//! \return
//!		returns true if event has been already serviced
//!
//! \note
//!
bool ThreadTransport::WaitEvent()
{
	while( true )
	{
		//check event: non-blocking call
		HypCEventInfo ei;
		if ( m_event.Wait( ei,NU_NO_SUSPEND ) )
		{
			m_lastEvent = ei.GetRequestID();
			m_status = ei.GetStatus();
		}
		else
		{
            Relinquish();
            continue;
		}

		//COM_RESET_REQ, DISCONNECT_NOW_REQ, DISCONNECT_SOON_REQ are processed in all states
		switch( m_lastEvent )
		{
		case USER_COM_RESET_REQ:
			Free();
			Init_COMM();
			return true;
		case USER_DISCONNECT_NOW_REQ:
L_USER_DISCONNECT_NOW_REQ:
			HangUp();
			m_error = HTENOERROR;
			DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: USER_DISCONNECT_NOW_REQ = %d\n", __FUNCTION__, m_error );
			return true;
		case USER_DISCONNECT_SOON_REQ:
			//'Hang up soon' can be processed only in the connected m_state
			//it is considered as Hang up now in any other m_state 
			if( m_state == ST_CONNECTED )
			{
				return false;
			}
			else
			{
				m_lastEvent = USER_DISCONNECT_NOW_REQ;
				DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: USER_DISCONNECT_NOW_REQ = %d\n", __FUNCTION__, m_error );
				HangUp();
				m_error = HTENOERROR;
				return true;
			}
		case HypCTransport::READ_REQUEST:
			m_lastEvent = DATA_RECEIVED;
			RECVLEN = m_status;
			return false;

		case HypCTransport::CONNECT_REQUEST:
			if( ( m_status == HTENOERROR ) || ( m_status == HTEISCONN ) )
			{
				m_lastEvent = COMM_CONNECTED;
			}
			else
			{
				m_lastEvent = CONNECTING_ERROR;
			}
			return false;

		case HypCTransport::WRITE_REQUEST:
			if( m_status == HTENOERROR )
			{
				m_lastEvent = MSG_TRANSMITTED;
			}
			else
			{
				m_lastEvent = WRITE_ERROR;
			}
			return false;

		case USER_CLOSE_SESSION_REQ:
			if( m_state != ST_CONNECTED )
			{
				goto L_USER_DISCONNECT_NOW_REQ;
			}
			return false;

		default:
			return false;
		}
	}
}

//-----------------------------------------------------------------------------
//!
//!      Read completed
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
void ThreadTransport::ReadCompleted()
{
	if( m_Transport.Transport()->GetError() != HTENOERROR )
	{
		m_readInProgress = false;
	}
}

//-----------------------------------------------------------------------------
//!
//!      Hung up
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!		Hang up immediately!  This function sends a hang up
//!     command.
//!
void ThreadTransport::HangUp()
{
	DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: OnEntry->Progress= %d m_state %d", __FUNCTION__, m_hangupInProgress, m_state );
	if( m_state != ST_IDLE )
	{
		m_online = false;
		m_hangupInProgress = true;
		// Disconnect transport
		// Stop timer
		m_timer.Stop();
		Free();
		m_event.Reset();

		// PPP disconnect 
		delete m_PPP;
		m_PPP = NULL;
#ifdef MAKE_M2
		// GPRS disconnect
		delete m_GPRS;
		m_GPRS = NULL;
#endif
	}
	m_hangupInProgress = false;
	m_closeSessionInProgress = false;
	m_hangupSoonInProgress = false;
	m_writeInProgress = false;
	m_readInProgress = false;
	m_connectInProgress = false;
	m_error = HTECANCELIO;
	m_commStatus = cs_IDLE;
	m_state = ST_IDLE;
    DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: OnExit->Progress= %d\n", __FUNCTION__, m_hangupInProgress );
}

//-----------------------------------------------------------------------------
//!
//!      Get status 
//!
//! \param
//!
//! \return
//!		Status of a connect
//!
//! \note
//!
int ThreadTransport::COM_GetStatus()
{
	if( m_hangupInProgress )
		return ( cs_RECEIVING ); 

	if( m_commStatus == cs_DIALING )
	{
		HypCModem *pModem= static_cast<HypCModem*>(m_Transport.SubTransport());
		if( pModem && m_moduleType == HW_MODEM )
		{
			switch( pModem->GetModemState() )
			{
			case HypCModem::MDM_IDLE:
				return cs_IDLE;
			case HypCModem::MDM_BUSY:
			case HypCModem::MDM_NODIALTONE:
				return cs_WAITLINE;
			case HypCModem::MDM_CONNECTING:
				return cs_DIALING;
			case HypCModem::MDM_TRAINING:
				return cs_SYNCHING;
			case HypCModem::MDM_CONNECTED:
				return cs_RECEIVING;
			case HypCModem::MDM_DISCONNECTING:
				return cs_RECEIVING;
			default:
				break;
			}
		}
		else
		{
			switch( m_Transport.Transport()->GetCommState() )
			{
			case HypCTransport::CONNECTING:
				return cs_DIALING;
			case HypCTransport::IDLE:
				return cs_IDLE;
			case HypCTransport::DISCONNECTING:
				return cs_RECEIVING;
			case HypCTransport::CONNECTED:
				return cs_RECEIVING;
			default:
				break;
			}
		}
	}
	return m_commStatus;
}

//-----------------------------------------------------------------------------
//!
//!      Get line status
//!
//! \param
//!
//! \return
//!		current line status
//!
//! \note
//!
HypCTransport::HypETransportState ThreadTransport::COM_GetLineStatus()
{
	return m_Transport.Transport()->GetCommState();
}

//-----------------------------------------------------------------------------
//!
//!      Is state "idle"?
//!
//! \param
//!
//! \return
//!		current transport state
//!
//! \note
//!
bool ThreadTransport::IsStateSocketIdle_()
{
	return ( m_state == ST_IDLE );
}

//-----------------------------------------------------------------------------
//!
//!      Cancels current dialing if possible
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
void ThreadTransport::CancelDial()
{
	// attempt to cancel dial
	COM_HangUpReq();
}

//-----------------------------------------------------------------------------
//!
//!      convert HFC m_error codes to old-style orv* and ERR* errors
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
void  ThreadTransport::HPOSFCError()
{
	switch ( m_error )
	{
	case HTENOERROR:
		HostOSRetval = orvOK;
		LNKERR = 0;
		break;
	case HTEINVAL:
		HostOSRetval = orvINVCONFIGURATION;
		LNKERR = ERRCNV;
		break;
	case HTEISCONN:
	case HTEINPROGRESS:
		HostOSRetval = orvDEVICEBUSY;
		LNKERR = ERRBSY;
		break;
	case HTENOTCONN:
		HostOSRetval = orvDEVICENOTOPEN;
		LNKERR = ERRDNO;
		break;
	case HTETIMEOUT:
		HostOSRetval = orvDEVICETIMEOUT;
		LNKERR = ERRDTO;
		break;
	case HTELOSTCARRIER:
		HostOSRetval = orvDCDLOST;
		LNKERR = ERRDCD;
		break;
	default:
		HostOSRetval = orvUNKNOWNERROR;
		LNKERR = ERRFLT;
		break;
	}
}

//-----------------------------------------------------------------------------
//!
//!      Wait for connection to complete and optionally 
//!      show comm m_status
//!
//! \param
//!
//! \return
//!		unsigned char   0 for ok, else non-zero
//!
//! \note
//!
bool ThreadTransport::ConnectWait()
{
	unsigned char prevComStatus = 0xff;	// Set to invalid COMSTATUS type
	unsigned char curComStatus;
	unsigned char prev_request = 0;
	unsigned char request;
	unsigned char prevGateStaus = 0xff;	// Set to invalid COMSTATUS type
	unsigned char curGateStatus;

	HostOSRetval = orvOK;

	while ( !COM_ConnectCompleted() )	// loop until dial complete
	{
		// If DispStatus is true or Transation request built
		if ( ShowDialStatus )
		{
			// Yes; display message at change of m_status
			request = TRREQBLD & BIT_0_SET;
			curComStatus = COM_GetStatus();

			if ( ( curComStatus != prevComStatus ) || ( request != prev_request ) )
			{
				prev_request = request;
				prevComStatus = curComStatus;
				if ( (0 != GetStatusMes(curComStatus)) && (cs_GATECONNECTING != curComStatus))
					ShowIntervalMsg(GetStatusMes(curComStatus),N_Comms);

			}
		}

        // Check if cancelled by user
        if( OnlineCancelled )
        {
            HostOSRetval = orvUSER;
			COM_HangUpReq();
			break;
        }
		Relinquish();
	}

	if( ( HostOSRetval == orvOK ) && ( HTENOERROR != m_error ) )
	{
		// convert HPOSFC m_error codes
		HPOSFCError();
	}

	if( HostOSRetval == orvDEVICETIMEOUT )	// timout
	{
		COM_HangUpReq();
	}
	
	return ( HostOSRetval != orvOK );
}

//-----------------------------------------------------------------------------
//!
//!      Return Message code depends of communication m_status
//!
//! \param
//!
//! \return
//!		message ID
//!
//! \note
//!
enum msg_id ThreadTransport::GetStatusMes( unsigned char cCOMStatus )
{
	// Enhanced dial messages set
	if (TCONF.TCDOPT & TCD_ENH) 
		switch ( cCOMStatus )
		{
		case cs_ANSWER:	//to maintain similarity to SPOS.
		case cs_DIALING:	// DIALING NOW
			return( DialNow );

		case cs_IDLE:	// "IDLE STATE"
			return( LineIdle );

		case cs_CHECKLINE:	// "checking line"
			return( CheckingLine );

		case cs_WAITLINE:	// "Waiting for line"
			return( WaitForLine );

		case cs_DIALTONE:	// "W. for dialtone" 
			return( WaitDialTone );

		case cs_SYNCHING:	// "Training"
			return TrainModem;

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
		case cs_IDLE:           // Idle m_state
		case cs_ANSWER:         // To maintain similarity to SPOS.
		case cs_DIALING:        // DIALING NOW
		case cs_SENDING:        // Logon to network
		case cs_SYNCHING:       // Synchronizing with other modem
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
	return static_cast<msg_id>(0);
}

//-----------------------------------------------------------------------------
//!
//!      Perform communication configuration
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
HypETransportError ThreadTransport::CommConfig()
{
	m_tranKey = DialTranKey;
	m_aqID = DIALAQID;

    // Derermine configuration type required (depended on comm module installed)
    // and Modem Mode
	if( false == GetModemMode() )
	{
		m_error = HTEINVAL;
	}
	else
	{
//-----------------------------------------------------------------------------
// Create config
//-----------------------------------------------------------------------------
		if( m_paq->AQOPT1 & AQ1_USEIP )
		{
            m_moduleType = HW_ETHERNET;
        }
        else if( m_paq->AQOPT5 & AQ5_USEGPRS )
        {
            m_moduleType = HW_GPRS;
        }
        else if( ( m_paq->AQOPT5 & AQ5_SETTLE_PPP ) || ( m_paq->AQOPT5 & AQ5_TRANS_PPP ) )
        {
            m_moduleType = HW_PPP;
        }
        else if( m_paq->AQOPT5 & AQ5_USEHLAN )
        {
            m_moduleType = HW_HLAN;
        }
        else
        {
            m_moduleType = HW_MODEM;
        }
        // use modem if dial backup is set
        if( m_ExistBackup == true )
        {
            DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Dial backup!\n", __FUNCTION__ );
            if( ( m_paq->AQOPT5 & AQ5_SETTLE_PPP ) ||
                ( m_paq->AQOPT5 & AQ5_TRANS_PPP ) ) 
            {
                m_moduleType = HW_PPP;
            }
            else
            {
                m_moduleType = HW_MODEM;
            }   		
        }

        switch ( m_moduleType )
        {
        case HW_MODEM:
            DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Using modem!\n", __FUNCTION__ );
            m_error = DialUpConfig();
            break;

        case HW_ETHERNET:
            DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Using Ethernet!\n", __FUNCTION__ );
            m_error = SetTCPIPConfig();
            break;
    #ifdef MAKE_M2
        case HW_GPRS:
            DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Using GPRS!\n", __FUNCTION__ );
            m_error = SetGPRSConfig();
            break;
    #endif
        case HW_PPP:
            DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Using PPP!\n", __FUNCTION__ );
            m_error = SetPPPConfig();
            break;

        case HW_HLAN:
            DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Using HLAN!\n", __FUNCTION__ );
            m_error = SetHLANConfig();
            break;

        default:
            m_error = HTEINVAL;
            break;
        }
	}
    DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: error= %d\n", __FUNCTION__,m_error );
    return m_error;
}

//-----------------------------------------------------------------------------
//!
//!      Get modem mode
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
bool ThreadTransport::GetModemMode()
{
	//common transaction
	m_paq = FindAcquirer( m_aqID );

	if( m_paq == NULL )
	{
		// Acquirer not found
		m_modemMode = -1;
	}
	else
    {
		// Get the Transaction from the Request 
        HostEvent.TRNKEY = m_tranKey;

        // Locate the Transaction - will be in HostEvent 
        HostFindTran();

        if ( HostEvent.TRNOPT2 & T2_SETTLE )
        {
			//settlement
            m_tranMode = SETTLE_TRAN;
            m_modemMode = m_paq->AQSMODEM;
        }
        else
        {
			m_tranMode = COMMON_TRAN;
            m_modemMode = m_paq->conn.dial.aqModem;
        }
	}
    return m_paq ? true : false;
}

//-----------------------------------------------------------------------------
//!
//!      Perform dial up configuration
//!
//! \param
//!
//! \return
//!		type of destination number
//!
//! \note
//!
HypETransportError ThreadTransport::DialUpConfig()
{
	int dest = GetNumDialDest();
	if( 0 == dest )
	{
		// report none of destination was defined
		return HTEINVAL;
	}

	//set up destination
    return SetDialConfig( 1 == dest ? true : false );
}

//-----------------------------------------------------------------------------
//!
//!      Set dial up configuration
//!
//! \param
//!		primary		true in case if it is primary connection
//!
//! \return
//!		false       if invalid dial mode
//!
//! \note
//!
HypETransportError ThreadTransport::SetDialConfig( bool primary )
{
	//connection wait time
	int cw;
	//dial attempts
	int da;
	//telephone number (BCD)
	unsigned char *pn;
	HypSAddress address;
	HypCTransportAddressSet m_trAddress;

	switch( m_tranMode )
	{
	case SETTLE_TRAN:
		// Get primary/secondary dial params for settlement
		GetDialParam( cw, da, pn,
				primary ? &m_paq->conn.dial.settle.primary : &m_paq->conn.dial.settle.secondary );
		break;

	case COMMON_TRAN:
		// Get primary/secondary dial params for tarns
		GetDialParam( cw, da, pn,
				primary ? &m_paq->conn.dial.trans.primary : &m_paq->conn.dial.trans.secondary );
		break;

	default:
		return HTEINVAL;
	}

	address.retries = da;
	address.timeout = cw * 1000;

	// create dial string from pn
	char phoneStr[25];
    int index;
    index = AddPBAX(phoneStr);

    BfAscii( &phoneStr[index], pn, 12 );
	phoneStr[24] = '\0'; //add null terminator
    //find phone number terminator  ('F')
    char *p = (char *)memchr(phoneStr,'F', 24);
    if(p != NULL)
    {
      *p='\0';
    }
	address.address = (const char*)phoneStr;
    DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Dialing on %s number!\n", __FUNCTION__, phoneStr );
	m_trAddress.AddAddress( &address );
	m_Transport.SetDestination( m_trAddress );
	m_timeOfConnect = 0;
	for( int i = 0; i < m_trAddress.AddressCount( ); i++ )
	{
		const HypSAddress* addr = m_trAddress.GetNextAddress();
		m_timeOfConnect += ( addr->retries * addr->timeout );
	}

    if( !m_Transport.SetMode( LINE_DIALUP, m_modemMode ) )
	{
		// invalid mode
		return HTEINVAL;
	}
#ifdef MAKE_M2
#if defined(AAE_MODEM)
	HypCModemAAE* modem = static_cast<HypCModemAAE*>(m_Transport.GetModem());
#else
	HypCInternalModem* modem = static_cast<HypCInternalModem*>(m_Transport.GetModem());
#endif
	modem->SetMdmPortNumber( HYP_SP_DOCK_MODEM );
#endif
	return HTENOERROR;
}

#ifdef MAKE_M2
//-----------------------------------------------------------------------------
//!
//!      Set GPRS configuration
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
HypETransportError ThreadTransport::SetGPRSConfig()
{
	if( NULL == m_GPRS )
	{
		if( NULL != (m_GPRS = new HypCGPRS) )
		{
			DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: GPRS object is created\n", __FUNCTION__ );
			// connect to GPRS server
			if( m_GPRS->Connect() )
			{
				DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: GPRS connect OK!\n", __FUNCTION__ );
				// add default route
				if( !m_GPRS->AddRoute( "0.0.0.0", DEFAULT_ROUTE, (unsigned char*) "0.0.0.0" ) )
				{
					// invalid mode
					DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: GPRS route is invalid!!!\n", __FUNCTION__ );
					return HTEINVAL;
				}
				else
				{
					DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: GPRS route is added\n", __FUNCTION__ );
				}
			}
			else
			{
				// invalid mode
				return HTEINVAL;
			}
		}
	}
	return SetTCPIPConfig();
}
#endif

//-----------------------------------------------------------------------------
//!
//!      Set PPP configuration
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
HypETransportError ThreadTransport::SetPPPConfig()
{
	if( NULL == m_PPP )
	{
		if( m_PPP = new HypCPPP )
		{
			// set login information
			m_PPP->SetLoginInformation( "", "", "", 30 );
			// connect to RAS server
			if( !m_PPP->Connect() ||
				// add default route
				!m_PPP->AddRoute( "0.0.0.0", HypCPPP::DEFAULT_ROUTE, (unsigned char*) "0.0.0.0" ) )
			{
				// invalid mode
				return HTEINVAL;
			}
		}
	}
	return SetTCPIPConfig();
}

//-----------------------------------------------------------------------------
//!
//!      Fills address set with IP destination
//!
//! \param
//!     address         address
//!     pIp             IP configuration
//!     timeout         timeout
//! \return
//!		true on success otherwise flase
//!
static bool AddIP( HypSAddress& address, struct ipcontab *pIp, int timeout )
{
    if( pIp->ipaddr[0] && pIp->ipaddr[0] != 0x20 && pIp->ipaddr[0] != 0xFF )
    {
        address.timeout= timeout;
        address.address = (char *)pIp->ipaddr;
        address.portnum = atoi( (char *)pIp->port );
        address.retries = 1;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
//!
//!      Set TCP/IP configuration
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
HypETransportError ThreadTransport::SetTCPIPConfig()
{
	struct ipcontab* pIPC;
	HypCTransportAddressSet m_trAddress;
	HypSAddress address;

	switch( m_tranMode )
	{
	case SETTLE_TRAN:
		pIPC = FindIPCById( m_paq->AQ_SETTLE_P_IPTAB );
		if( pIPC != NULL && AddIP( address, pIPC, m_paq->AQ_SETTLE_P_CON ) )
		{
			m_trAddress.AddAddress( &address );
			DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Settle: IP = %s:%s\n",
						__FUNCTION__, pIPC->ipaddr, pIPC->port );

			pIPC = FindIPCById(m_paq->AQ_SETTLE_S_IPTAB);
			if( pIPC != NULL && AddIP( address, pIPC, m_paq->AQ_SETTLE_S_CON ) )
			{
				m_trAddress.AddAddress( &address );
			}
		}
		break;

	case COMMON_TRAN:
		pIPC = FindIPCById( m_paq->AQ_TRANS_P_IPTAB );
		if( pIPC != NULL && AddIP( address, pIPC, m_paq->AQ_TRANS_P_CON ) )
		{
			m_trAddress.AddAddress( &address );
			DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Common: IP = %s:%s\n",
						__FUNCTION__, pIPC->ipaddr, pIPC->port );

			pIPC = FindIPCById(m_paq->AQ_TRANS_S_IPTAB);
			if( pIPC != NULL && AddIP( address, pIPC, m_paq->AQ_TRANS_S_CON ) )
			{
				m_trAddress.AddAddress( &address );
			}
		}
		break;

	default:
		return HTEINVAL;
	}

	m_Transport.SetDestination( m_trAddress );
	m_timeOfConnect = 0;
	for( int i = 0; i < m_trAddress.AddressCount(); i++ )
	{
		const HypSAddress* addr = m_trAddress.GetNextAddress();
		m_timeOfConnect += ( addr->retries * addr->timeout );
	}
	m_timeOfConnect *= 1000;

	DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: Module type = %d\n", __FUNCTION__, m_moduleType );

	// setup for TCPIP
	if( !m_Transport.SetMode( LINE_TCPIP, m_protocol, 0, "", "" ) )
	{
		// invalid mode
		DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: invalid TCP/IP communication mode\n", __FUNCTION__ );
		return HTEINVAL;
	}
	else
	{
#ifdef MAKE_SSL
		if( pIPC && !!(pIPC->opt1 & IPC_SSL) )
		{
			HypCSSLSocket *pSSL= m_Transport.CreateSSL();
			HypCCertificateProvider cert;
			if( pSSL )
			{
				char *pCert;
				bool bSuccess;

                // Enables access to a disk where certificate(s) are located
                NU_Become_File_User();
                NU_Open_Disk("C:");
                // allocates temporary buffer for filename(s)
                pCert= new char[128];

                if( !!(pIPC->opt1 & IPC_SSL_PEER) )
                {
                    // Server Authentication is enabled
                    if( GetCertificateFilename( pCert,(char*)pIPC->cert_CA ) )
                    {
                        // Set up terminal CA certificate's chain from database
                        bSuccess= cert.LoadVerifyLocations( *pSSL,(const char*)pCert );
                        DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: SSL LoadVerifyLocations() returned %d\n", __FUNCTION__, bSuccess );
                        if( bSuccess )
                        {
                            // enable peer verification if we have at least one CA
                            pSSL->SetVerifyPeer( true );
                        }
                    }
                }
                if( !!(pIPC->opt1 & IPC_SSL_CLIENT) )
                {
                    // Sending client certificate during Mutual Authentication is enabled
                    if( GetCertificateFilename( pCert,(char*)pIPC->certificate ) )
                    {
                        // Set up terminal certificate if we want
                        // to override the default certificate
                        bSuccess= cert.LoadCertificateFile( *pSSL,(const char*)pCert );
                        DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: SSL LoadCertificateFile() returned %d\n", __FUNCTION__, bSuccess );

                        if( bSuccess && MakePrivateKeyFilename( pCert ) )
                        {
                            bSuccess= cert.LoadRSAPrivateKeyFile( *pSSL,(const char*)pCert );
                            DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: SSL LoadRSAPrivateKeyFile() returned %d\n", __FUNCTION__, bSuccess );
                        }
                    }
                    else
                    {
                        // if database does not contain client certificate, trying to load
                        // the default terminal certificate/private key
                        bSuccess= cert.LoadTerminalCertificate( *pSSL );
                        DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: SSL LoadTerminalCertificate() returned %d\n", __FUNCTION__, bSuccess );
                    }
                }
                // deletes temporary buffer
                delete pCert;
            }
        }
#endif  // MAKE_SSL
	}
	m_Transport.Prefix( !pIPC ? true : !(pIPC->opt1 & IPC_LENGTH) ? false : true );
	return HTENOERROR;
}

//-----------------------------------------------------------------------------
//!
//!      Set HLAN configuration
//!
//! \param
//!
//! \return
//!		None
//!
//! \note
//!
HypETransportError ThreadTransport::SetHLANConfig()
{
    HypCTransportAddressSet m_trAddress;
    HypSAddress address;

    if( m_paq )
    {
        char strTemp[ S_AQSCUADDRESS + 1 ];

        MoveItAq( strTemp, m_paq->AQSCUADDRESS, S_AQSCUADDRESS );
        strTemp[ S_AQSCUADDRESS ] = '\0';

        address.address = strTemp;
        address.address.Trim();

        m_trAddress.AddAddress( &address );

        if( !m_Transport.SetDestination( m_trAddress ) ||
            !m_Transport.SetMode( LINE_HLAN_HIGH, PROTO_HLAN,
                                      0, "", "" ) )
        {
            // invalid mode
            return HTEINVAL;
        }
    }
    return HTENOERROR;
}

//-----------------------------------------------------------------------------
//!
//!      Function check for current Dest avaliable
//!
//! \param
//!
//! \return
//!		Current settings (0 - None, 1 - First, 2 - Second)
//!		For INIT_TRAN 1 - GSM/GPRS , 2 - Dial Backup
//!
//! \note
//!
int ThreadTransport::GetNumDialDest()
{
	int cw, da;
	unsigned char *pn;

	switch( m_tranMode )
	{
	case SETTLE_TRAN:
		// Get primary dial params for settlement
		GetDialParam( cw, da, pn, &m_paq->conn.dial.settle.primary );
		if( ( pn[0] != 0xFF ) && ( da != 0 ) )
			return 1;
		// Get secondary dial params for settelment
		GetDialParam( cw, da, pn, &m_paq->conn.dial.settle.secondary );
		if( ( pn[0] != 0xFF ) && ( da != 0 ) )
			return 2;
		else
			return 0;

	case COMMON_TRAN:
		// Get secondary dial params for trans
		GetDialParam( cw, da, pn, &m_paq->conn.dial.trans.primary );
		if( ( pn[0] != 0xFF ) && ( da != 0 ) )
			return 1;
		// Get secondary dial params for settlement
		GetDialParam( cw, da, pn, &m_paq->conn.dial.trans.secondary );
		if( ( pn[0] != 0xFF ) && ( da != 0 ) )
			return 2;
		else
			return 0;

	default:
		return 1;
	}
}

//-----------------------------------------------------------------------------
//!
//!      Get dial parameters
//!
//! \param
//!		cw		connection wait time
//!	\param
//!		da		number of dial attempts
//!	\param
//!		pn		telephone number (BCD)
//!	\param
//!		data	phone dial structure
//!
//! \return
//!		None
//!
//! \note
//!
void ThreadTransport::GetDialParam( int &cw, int &da, unsigned char* &pn, PHONE_STRUCT* data )
{
	//connection wait time
	cw = CvtBin( data->timeout );

	// Number of dial attempts
	da = data->trys;

	// Telephone number (BCD)
	pn = data->phoneNum;
}

//-----------------------------------------------------------------------------
//!
//!      Finds the IP Connect Entry given the Id
//!
//! \param
//!		id_		IP Connect Number (Id)
//!
//! \return
//!		Pointer to IP Connect Entry or NULL if not found
//!
//! \note
//!
ipcontab* ThreadTransport::FindIPCById( unsigned char id_ )
{
	unsigned char i;

	for( i = 0; i < IPCMAX; i++ )
	{
		if( IPCTAB[i].id == id_ )
		{
			return &IPCTAB[i];
		}
	}
    return NULL;
}

static int AddPBAX(char *phoneStr)
{
    int index, i;
    char tmp[8];

    BfAscii( tmp, (unsigned char *)TERM.TERMPabx, 4 );
    for( index = 0, i = 0; i < 8; ++i ) 
        if( tmp[i] == 'F' )
            break;
        else
            phoneStr[index++] = tmp[i];
    
    index > 0 ? phoneStr[index++] = 'B': index = 0;
    return index;

}

#ifdef MAKE_SSL
static char WhiteChars[]= { ' ', '\0', '\xFF' };
static char *pKeyFileSuffix= "-KEY";

//-----------------------------------------------------------------------------
//!
//!      Extracts certificate's file from database field
//!
//! \param
//!		ppBuff      certificate file
//!		pFilename   certificate file (could be non-ASCIIZ)
//!
//! \return
//!		Zero on error else non-zero and fills pBuff with file name
//!
int ThreadTransport::GetCertificateFilename( char *pBuff,char *pFilename )
{
    char            *p;
    size_t          len;
    int             nRes= 0;

    if( pFilename )
    {
        // makes empty filename by default
        *pBuff= '\0';
        // filename length calculating
        len= 0;
        while( S_CERTFILE > len && !memchr( WhiteChars,pFilename[len],sizeof(WhiteChars) ) )
            ++len;
        if( len )
        {
            // File name generating
            strcpy( pBuff,"C:\\" );
            memcpy( pBuff+3,pFilename,len );
            pBuff[3+len]= '\0';
            nRes= 1;
        }
    }
    return nRes;
}

//-----------------------------------------------------------------------------
//!
//!      Makes private key file name
//!
//! \param
//!		pFilename[in/out]   client certificate file
//!
//! \return
//!		Zero on error else non-zero and pFilename will contain
//!     private key file name
//!
int ThreadTransport::MakePrivateKeyFilename( char *pFilename )
{
    int             nRes= 0;

    if( pFilename )
    {
        int nSuffixLen= strlen(pKeyFileSuffix);
        int nNameLen= strlen(pFilename);
        char *pExt= (char *)memchr( pFilename,'.',nNameLen );
        if( pExt )
        {
            // copy extension to place where "-key" will have ending
            memmove( pExt+nSuffixLen,pExt,strlen(pExt)+1 );
        }
        else
        {
            // no file extension found, prepare to insert file name suffix
            // to the end
            pExt= pFilename+strlen(pFilename);
            pExt[nSuffixLen]= '\0';
        }
        // insert private key file name suffix
        memcpy( pExt,pKeyFileSuffix,nSuffixLen );
        nRes= 1;
    }
    return nRes;
}

#endif  // MAKE_SSL

