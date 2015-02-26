//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPCTRANSPORT_HPP_
#define _HYPCTRANSPORT_HPP_

//=============================================================================
//
// HypCTransport.hpp
//
// Transports and protocols.
//
//=============================================================================
#include "transp_def.h"
#include "HypEErrors.h"
#include "HypCEvent.hpp"
#include "HypCBuffer.hpp"
#include "HypCConfig.hpp"
#include "HypCStatistics.hpp"

//=============================================================================
// Public defines and typedefs
//=============================================================================

// Destination specifier

//=============================================================================
// Forward definitions
//=============================================================================



//=============================================================================
//
// HypCTransport
//
// Base Class:
//      HypCDevice
//
// Overview:
//      This class provides common interface for for the transport interfaces
//      and protocols. There are should be no instances of this class.
//
// Related Documents:
//      None.
//
// See Also:
//      HypCDevice, HypCSerialTransport, HypCCommunication, HypCTransparent,
//      and other protocol related classes.
//
//=============================================================================
class HypCTransport
{
//=============================================================================
// Member structures, enumerations, and class definitions
//=============================================================================
	public:
		
		// Transport internal state
		enum HypETransportState
		{
			// Idle
			IDLE,
			// Connecting
			CONNECTING,
			// Connected
			CONNECTED,
			// Disconnecting
			DISCONNECTING
		};

		enum HypETransportStatus
		{
			STATUS_IDLE=IDLE, ///< Idle state.
			STATUS_CONNECTING=CONNECTING, ///< Connecting to remote.
			STATUS_CONNECTED=CONNECTED, ///< Connected to remote.
			STATUS_DISCONNECTING=DISCONNECTING, ///< Disconnecting.
			STATUS_INIT, ///<Initializing modem.
			STATUS_DIALING, ///<Dialing stage.
			STATUS_LOGON,	///<Logon into network (pdn logon).
			STATUS_REDIALING, ///<Redialing stage (only reported by HypCCommunication).
			STATUS_WAITLINE, ///<Waiting for line.
			STATUS_CHECKLINE, ///<Checking line.
			STATUS_USERDEFINED,
			STATUS_PPPCONNECTING, ///<Connecting to PPP host.
			STATUS_TCPCONNECTING, ///<Connecting to TCP/IP host.
			STATUS_PAGING, ///< Connecting via BlueTooth.
			STATUS_GATECONNECTING, ///< Connecting on D2100.
			STATUS_NOTAVAILABLE, ///< Not available.
			STATUS_HANDSETUP ///< User have to rise handset
		};

		
//=============================================================================
// Member functions
//=============================================================================
	public:

		// Device controls should always have a type and an ID. Making
		// this constructor explicit and hiding the default constructor
		// helps to keep this policy.
		HypCTransport( int nDeviceID );

		// The destructor is declared virtual to allow for deletion of
		// devices pointed by generic HypCTransport pointers
		virtual ~HypCTransport();

		int GetID() const
		{ return m_DeviceID; };

		// Asynchronous read request
		void ReadReq( HypCEvent &event, bool bWaitForData=true );

		// Asynchronous write request
		void WriteReq( HypCEvent &event, HypCBuffer *pBuffer);

		// Asynchronous connect request
		void ConnectReq( HypCEvent &event );

		// Asynchronous disconnect request
		void DisconnectReq( HypCEvent &event );

		// Asynchronous disconnect monitor
		void TrackDisconnectReq( HypCEvent &event );

		void CancelRequest ( HypETransportOperations RequestId );

		void SetConfig (const HypCConfig &cfg);
		void GetConfig (HypCConfig &cfg);

		enum HypETransportStatus GetStatus();

		// Get error
		HypETransportError GetError()
		{ 
			return DoGetError();
		}

		// Get transport communication state
		HypETransportState GetCommState( void )
		{
			return DoGetCommState();
		}

		// Flushes the receive buffer
		void Flush( void )
		{
			DoFlush ();
		}
		
		// Read with timeout
		int Read( HypCBuffer *&pBbuffer, bool bWaitForData=true );
		
		// Write with timeout
		int Write( HypCBuffer *pBuffer);
		
		// Perform connect
		int Connect( void );

		// perform disconnect
		int Disconnect( void );

		virtual HypCStatistics * GetStatistics()
		{
			return NULL;
		}

	protected:


		// Set transport error
		virtual void SetError( HypETransportError error )
		{ 
			m_Error = error;
		}

				// Get error
		virtual HypETransportError DoGetError()
		{ 
			return m_Error;
		}

		// Set transport error to indicate no error
		virtual void ResetError(void)
		{ 
			m_Error = HTENOERROR; 
		}

		// Get transport communication state
		virtual HypETransportState DoGetCommState( void )
		{
			return m_CommState;
		}

	private:


		// DoReadReq is called by ReadReq. Devices implement needed
		// functionality here.
		virtual void DoReadReq( HypCEvent &event, bool bWaitForData=true ) = 0;

		// DoWrite is called by Write. Devices implement needed
		// functionality here.
		virtual void DoWriteReq( HypCEvent &event, HypCBuffer* pBuffer ) = 0;
		
		// DoConnectReq is called by ConnectReq. Devices implement needed
		// functionality here.
		virtual void DoConnectReq(HypCEvent &event) = 0;
		
		// DoDisconnectReq is called by DisconnectReq. Devices implement needed
		// functionality here.
		virtual void DoDisconnectReq(HypCEvent &event) = 0;

		virtual void DoTrackDisconnectReq(HypCEvent &event);

		virtual void DoCancelRequest ( HypETransportOperations RequestId ) = 0;

		virtual void DoFlush (void);

		
		virtual void DoSetConfig (const HypCConfig &cfg);
		virtual void DoGetConfig (HypCConfig &cfg);

		virtual enum HypETransportStatus DoGetStatus();

		
		// Not implemented on purpose. We do not want to create devices
		// without a type and an ID
		HypCTransport();

//=============================================================================
// Member variables
//=============================================================================
	protected:

		// Communicaiton state of transport object
		HypETransportState m_CommState;
		HypCEvent *m_pTrackDisconnectEvent;
		HypCConfig m_Config;

	private:

		// Last error
		HypETransportError m_Error;

		int m_DeviceID;

};

/**
        @brief Send null-terminated string to transport and wait responce
        @param t Pointer to HypCTransport 
        @param cmd sent command (request)
        @param rsp expected responce
        @param timeout timeout for receiving responce
        @return 
			- HTEINVAL if parameters are invalid
			- HTENOERROR if expected responce received before expiring timeout
			- HTETIMEOUT if expected responce did not receive before expiring timeout
			- HTENOTCONN if transport is not connected
			- or return status of the Read/Write operations
        @note Transport must be opened. If <i>timeout</i> is equal to 0 then function does not
		check timeout.
*/
extern int SendStr(HypCTransport *t, const char * cmd, const char * rsp, size_t timeout);

#endif // _HYPCTRANSPORT_HPP_
