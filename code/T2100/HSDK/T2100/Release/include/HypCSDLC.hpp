//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCSDLC_HPP_
#define _HYPCSDLC_HPP_

//=============================================================================
//
// HypCSDLC.hpp
//
// SDLC protocol implementation.
//
//=============================================================================
#include "HypCTransport.hpp"
#include "HypCEvent.hpp"
#include "HypCTimer.hpp"
#include "SimpleVector.hpp"


#ifdef SNRM
#undef SNRM
#endif
#ifdef DM
#undef DM
#endif
#ifdef DISC
#undef DISC
#endif
#ifdef UA
#undef UA
#endif
#ifdef FRMR
#undef FRMR
#endif
#ifdef RR
#undef RR
#endif
#ifdef RD
#undef RD
#endif
#ifdef RNR
#undef RNR
#endif
#ifdef INFO
#undef INFO
#endif

// Receive/transmit buffer size
#define SDLC_STACK_SIZE 4096
#define SDLC_DEFAULT_CU 0x30

// Bits defined for SDLCFeatures (or these values)
#define SDLC_ENFORCE_HALFDUPLEX	0x01
#define SDLC_SEND_RD		0x02
#define SDLC_IGNORE_OUTOFORDER	0x04

// Timeouts
#define SDLC_SNRM_WAIT_TIMER	30000
#define SDLC_RD_TIMEOUT		3000
#define SDLC_RD_HOLD		1000

// SDLC will ignore up to this number of frames when counters does not match
// and this feature enabled
#define SDLC_OUTOFORDER_INIT	3

//=============================================================================
//
// HypCSDLC
//
// Base Class:
//      HypCTransport
//
// Overview:
//      This class is used by the HypCCommunication to provide SDLC
//	protocol capability.
//
// Related Documents:
//      lapm.pdl                      LAPM protocol design on T7/ICE terminals.
//      lapm.doc                      LAPM protocol specification.
//      hdlc.doc                      HDLC protocol specification.
//
// See Also:
//      HypCTransport, HypCDevice, HypCVisa1,
//      HypCTransparent
//
//=============================================================================
class HypCSDLC : public HypCTransport
{
//=============================================================================
// Member structures, enumerations, and class definitions
//=============================================================================
	private:
		// SDLC commands
		enum HypESDLCCommands
		{
			// C   : set normal response mode
			SNRM = 0x83,
			//   R : disconnected mode
			DM = 0x0f,
			// C   : disconnect
			DISC = 0x43,
			RD = DISC,
			//   R : unnumbered acknowledge
			UA = 0x63,
			//   R : frame reject
			FRMR = 0x87,
			// C R : receiver ready
			RR = 0x01,
			// C R : receiver not ready
			RNR = 0x05,
			// C R : information frame
			INFO = 0x00,
			// Invalid command
			INVALID_COMMAND = -1
		};

		enum HypESDLCReceiveStatus
		{
			RECEIVE_CANCELLED,
			RECEIVE_SUCCEED,
			RECEIVE_FAILED
		};

		// State machine
		enum HypESDLCStates
		{
			// Lower layer connect
			STATE_LL_CONNECT,
			// Wait for SNRM command frame
			STATE_SNRM_WAIT,
			// Transmit UA response frame
			STATE_UA_XMIT,
			// Receive/transmit message
			STATE_NORMAL_XMIT,
			// Transmit FRMR response frame
			STATE_FRMR_XMIT,
			// Transmit DM response frame
			STATE_DM_XMIT,
			// Transmit RD response frame
			STATE_RD_XMIT,
			// Disconnected
			STATE_DISCONNECTED,
			// Abort connection attempt
			STATE_ABORT
		};

//=============================================================================
// Member functions
//=============================================================================
	public:
		// Creates SDLC protocol.
		HypCSDLC( HypCTransport *transport, int nDeviceID );

		// Destructor
		virtual ~HypCSDLC();

		virtual HypCStatistics * GetStatistics()
		{
			return &m_Stat;
		}

	private: 
		// DoRead is called by Read. Devices implement needed
		// functionality here.
		virtual void DoReadReq( HypCEvent &event, bool bWaitForData );

		// DoWriteReq is called by WriteReq. Devices implement needed
		// functionality here.
		virtual void DoWriteReq( HypCEvent &event, HypCBuffer *pBuffer );

		//Connect request
		virtual void DoConnectReq( HypCEvent &event );

		//Disconnect request
		virtual void DoDisconnectReq( HypCEvent &event );

		// Cancels specified request
		virtual void DoCancelRequest( HypETransportOperations requestId );

	private:
		// Wait for SNRM command frame
		void WaitForSNRM();

		// Transmit UA response frame
		void TransmitUA();

		// Transmit message
		void Transmit();

		// Transmit FRMR response frame
		void TransmitFRMR();

		// Transmit DM response frame
		void TransmitDM();

		// Transmit RD response frame
		void TransmitRD();

		// Handle disconnected state
		void Disconnected();

		// Abort connection attempt
		void Abort();

		// Check if frame received and ready for processing
		bool ReceiveFrame();

		// Process received frame
		void ProcessFrame();

		// Filter received frame type
		HypESDLCCommands ReceivedFrameType();

		// Check counters
		bool CheckCounters();

		// Make transmitted frame control byte
		unsigned char MakeFrameType( HypESDLCCommands command );

		// Encode and transmit frame
		bool TransmitFrame( HypESDLCCommands command,
		                    const unsigned char *buffer,
		                    unsigned int bufferSize );

		// Releases memory of the last received frame
		void ReleaseReceivedFrame( void );

		// Receive complete handler
		void ReceiveComplete( HypESDLCReceiveStatus status );

		// Transmit complete handler
		void TransmitComplete( bool status );

		// SDLC controlling task
		static void SDLCTask( void );

		// Single session SDLC processing for the task
		void SDLCTask1( void );

		// Lock thread synchronization semaphore
		void LockSemaphore();

		// Unlock thread synchronization semaphore
		void UnlockSemaphore();

//=============================================================================
// Member variables
//=============================================================================
	private:
		// Serial port handle
		HypCTransport *m_Transport;

		// CU address
		unsigned char m_CU;

		// Receive count
		int m_Vr;

		// Send count
		int m_Vs;

		// State machine
		HypESDLCStates m_State;

		// T401 acknowledgment frame timer
		HypCTimer m_T401;

		// LAPM controlling task
		DWORD m_nJobNr;

		// Thread syncronization semaphore
		BOOL m_Semaphore;

		// Write request data
		// NOTE: access must be protected with m_Semaphore
		HypCBuffer *m_WriteBuffer;
		HypCEvent *m_pWriteRequest;
		bool m_DataSent;

		// Read request data
		// NOTE: access must be protected with m_Semaphore
		HypCBuffer *m_pReadBuffer;
		int m_ReadSize;
		HypCEvent *m_pReadRequest;

		// Connect request data
		HypCEvent *m_pConnectRequest;
		
		// Disconnect request
		HypCEvent *m_pDisconnectRequest;

		// Last received command
		unsigned char m_LastCommand;

		// Timer T401 Expired flag
		HypCEvent m_TimerT401Expired;

		// Is own receiver busy
		bool m_OwnReceiverBusy;

		// Is peer receiver busy
		bool m_PeerReceiverBusy;

		// Is there unacknowledged frame
		bool m_UnacknowledgedFrame;

		// Is send response required
		bool m_SendResponse;

		// Event manager for the read/write operations within task
		HypCEvent m_Event;

		// Transmit in progress flag
		bool m_TransmitInProgress;

		// Receive in progress flag
		bool m_ReceiveInProgress;

		//Queue for received frames
		SimpleVector <HypCBuffer *> m_RxQ;

		// Are we in retransmit phase?
		bool m_Retransmitting;

		// Statistics of connection
		HypCStatistics m_Stat;

		// SDLC features
		int m_SDLCFeatures;

		// T401 value
		int m_T401Value;

		// Out of order frame ignore counter
		int m_IgnoreCounter;
};

#endif // _HYPCSDLC_HPP_
