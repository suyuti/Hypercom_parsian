/*=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// \file
//      comm_c.hpp
//
// \brief
//      Declaration of ThreadTransport class
//
*/
#ifndef _COMM_C_H_
#define _COMM_C_H_

#include <HypCThread.hpp>
#include <HypCEvent.hpp>
#include <HypCTimer.hpp>
#include <HypCBlob.hpp>
#include "HostTransport.hpp"

extern "C"{
#include "drivdef.h"
#include "fixdata.h"
#include "fixdata_app.h"
#include "basictyp.h"
#include "struct.h"
#include "sysstruc.inc"
#include "appdata.h"
}

//=============================================================================
// Public defines and typedefs
//=============================================================================

//=============================================================================
// Public defines and typedefs
//=============================================================================

//=============================================================================
// Forward definitions
//=============================================================================
class HypCPPP;
class HypCGPRS;
//=============================================================================
//!
//! \brief
//!     Creates thread transport object used for host communication.
//!
//! \sa
//!     HypCThread, HypCTransport, HypCMLTransport
//!
//=============================================================================
class ThreadTransport : public HypCThread
{
//=============================================================================
// Member structures, enumerations, and class definitions
//=============================================================================
		//! Events of the transport are to be begun from HypCTransport::USER_REQUEST
		enum Events
		{
			//! reset request
			USER_COM_RESET_REQ = HypCTransport::USER_REQUEST,
			//! connect request
			USER_CONNECT_REQ,
			//! close session request
			USER_CLOSE_SESSION_REQ,
			//! disconnect now request
            USER_DISCONNECT_NOW_REQ,
			//! disconnect soon request
			USER_DISCONNECT_SOON_REQ,
			//! transmit message
			USER_TRANSMIT_MSG_REQ,
			//! comm is connected
			COMM_CONNECTED,
			//! data is received
			DATA_RECEIVED,
			//! data is transmitted
			MSG_TRANSMITTED,
			//! read error
			READ_ERROR,
			//! write error
			WRITE_ERROR,
			//! connect error
			CONNECTING_ERROR,
			//! hung up soon timer expired
			HANG_UP_SOON_TIMER_EXPIRED,
			//! connect timer expired
			CONNECT_TIMER_EXPIRED
		};

		//! The state machine states
		enum States
		{
			//! idke
			ST_IDLE,
			//! connecting
			ST_CONNECTING,
			//! connected to host
			ST_CONNECTED,
			//! hung up soon
			ST_HANGUP_SOON
		};

		//! Module type
		enum ModuleType
		{
			//! No module installed.
			HW_NOMODULE,    
			//! Conexant V90 modem.
			HW_MODEM,    
			//! Ethernet module.
			HW_ETHERNET,
			//! GPRS module.
			HW_GPRS,
			//! PPP module.
			HW_PPP,
            //! HLAN module.
            HW_HLAN
		};

		//! The type of a transaction
		enum TRAN_MODE
		{
			//! transaction settle
			SETTLE_TRAN,
			//! common transaction
			COMMON_TRAN,
			//! init transaction
			INIT_TRAN,
			//! NAC dial
			NAC_DIAL_TRAN,
			//! ICE PAC transaction
			ICE_PACK_TRAN
		};

//=============================================================================
// Member functions
//=============================================================================
	public:
		//! constructor
		ThreadTransport( HypCEvent& ev, HypEProtocol protocol = PROTO_TRANSPARENT );

		//! destructor. Makes sure to disconnect and delete protocol objects.
		virtual ~ThreadTransport( );

		// Get pointer to public function of the class
		static ThreadTransport* GetThreadInstance( HypCThread* this_ = NULL );

		//! Reset all transports
		bool COM_ResetCompleted_();

		//! Reset request
		void COM_ResetReq_();

		//! Send message
		void SendMessage_();

		//! Receive message
		void RecvMessage_();

		//! Request to receive message
		void RequestMessage_();

		//! Initiate dial
		void InitiateDial_();

		//! Cancel communicate
		void CancelComms_();

		//! Do not hung up in case if PIN bad
		void Hang_Up_Pin_();

		//! Hung up
		void Hang_Up_();

		//! Hung up soon
		void Hang_Up_Soon_();

		//! Get line status
		HypCTransport::HypETransportState GetLineStatus_();

		//! Reset comm module
		void ResetCommModule_();

		//! Is socket idle?
		bool IsStateSocketIdle_();

    protected:
		//! Thread entry point
		virtual void Execute();

	private:
		//! Init communicate
		void Init_COMM();

		//! Free all transports
		void Free();

		//! Wait event from queue
		bool WaitEvent();

		//! State idle( state machine )
		void StIdle();

		//! State connecting( state machine )
		void StConnecting();

		//! State connected( state machine )
		void StConnected();

		//! State Hung up soon ( state machine )
		void StHangUpSoon();

		//! Read completed
		void ReadCompleted();

		//! Hung up
		void HangUp();

		//! Connect request
		void COM_ConnectReq();

		//! Connect completed
		bool COM_ConnectCompleted();

		//! Hung up request
		void COM_HangUpReq();

		//! Close session request
		void COM_CloseSessionReq();

		//! Session is closed
		bool COM_CloseSessionCompleted();

		//! Hung up soon request
		void COM_HangUpSoonReq();

		//! Hung up completed
		bool COM_HangUpCompleted();

		//! Write request
		void COM_WriteReq( const void* buf, unsigned int len );

		//! Write completed
		bool COM_WriteCompleted();

		//! Read request
		void COM_ReadReq( void* buf, unsigned short* len );

		//! Read completed
		bool COM_ReadCompleted();

		//! Cancel read
		void COM_CancelRead();

		//! Get line status
		HypCTransport::HypETransportState COM_GetLineStatus();

		//! Cancel dial
		void CancelDial();

		//! Get error message
		void HPOSFCError();

		//! Connect wite
		bool ConnectWait();

		//! Get status message
		enum msg_id GetStatusMes( unsigned char cCOMStatus );

        //! Make backup dialup connection
        bool IsDialBackup();

        //! Get modem mode
        bool GetModemMode();

		//! Configure communication
		HypETransportError CommConfig();

		//! Dial up config
		HypETransportError DialUpConfig();

		//! Set dial configure
		HypETransportError SetDialConfig();

		//! TCP/IP config
		HypETransportError SetTCPIPConfig();

		//! Fills address set with IP destination
		bool AddIP( HypSAddress& address, struct ipcontab *pIp, int timeout );

		//! Fills address set with Dial Up destination
		bool AddDialUp( HypCTransportAddressSet& trAddress, HypSAddress& address, PS_DIAL_STRUCT* phone );

#ifdef MAKE_M2
		//! GPRS+TCP/IP config
		HypETransportError SetGPRSConfig();
#endif
		//! PPP config
		HypETransportError SetPPPConfig();

        //! HLAN configuration
        HypETransportError SetHLANConfig();

		// Get dialing number
		int GetNumDialDest();

		//! Get dialing parameters
		void GetDialParam( int &cw, int &da, unsigned char* &pn, PHONE_STRUCT* data );

		//! Finds the IP Connect Entry given the Id
		ipcontab *FindIPCById( unsigned char id_ );

		//! Get status
		int COM_GetStatus();

#ifdef MAKE_SSL
        //! Extracts certificate's file from database field
        int GetCertificateFilename( char *pBuff,char *pFilename );
        //! Makes private key file name
        int MakePrivateKeyFilename( char *pFilename );
#endif  // MAKE_SSL

//=============================================================================
// Member variables
//=============================================================================
	protected:

	private:
		//! An event
		HypCEvent&	m_ev;

		// acquirer record
		struct acquirer_rec* m_paq;

		//! transport
        HostTransport m_Transport;

		//! inside event
		HypCEvent m_event;

		//! timer
		HypCTimer m_timer;

		//! last event
		int m_lastEvent;

		//! transport states
		enum States m_state;

		//! status
		int m_status;

		//! communicate status
		int m_commStatus;

		//! Is connect in progress?
		bool m_connectInProgress;

		//! Is hung up in progress?
		bool m_hangupInProgress;

		//! Is session close in progress?
		bool m_closeSessionInProgress;

		//! Is hung up soon in progress?
		bool m_hangupSoonInProgress;

		//! Is reset in progress
		bool m_resetInProgress;

		//! Is write in progress?
		bool m_writeInProgress;

		//! Is read in progress?
		bool m_readInProgress;

        //! Is backup exist?
        bool m_ExistBackup;

		//! write buffer
		const char* m_writeBuf;

		//! write buffer lenght
		unsigned int m_writeLen;

		//! transport error
		HypETransportError m_error;

		// Is online?
		bool m_online;

		//! transaction mode
		TRAN_MODE m_tranMode;

		//! transaction key
		int  m_tranKey;

		//! acquirer ID
		int  m_aqID;
		//! current modem mode
		int  m_modemMode;

		//! transport module type
		ModuleType m_moduleType;

		//! Host protocol
		HypEProtocol m_protocol;

		//! Time of connect
		int m_timeOfConnect;

		//! pointer to PPP connection
		HypCPPP* m_PPP;

#ifdef MAKE_M2
		//! pointer to GPRS connection
		HypCGPRS* m_GPRS;
#endif
};
#endif // _COMM_C_H_

