//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCTRANSPARENTCOMMON_HPP_
#define _HYPCTRANSPARENTCOMMON_HPP_

#include "eftos.h"
#include "transp_def.h"
#include "HypCEvent.hpp"
#include "SimpleVector.hpp"
#include "CtrlChar.h"
#include "HypCConfig.hpp"

#include "HypCTransport.hpp"

#define TRANSPARENT_STACK_SIZE 4096

class HypCTransparentCommon : public HypCTransport
{
	public:
		HypCTransparentCommon(HypCTransport *transport, int nDeviceID);
		// Destructor
		virtual ~HypCTransparentCommon();

	protected:

		enum TransparentCommonEvents{
			USER_CONNECT=USERDEFINED_REQUEST, //6
			USER_WRITE,
			USER_DISCONNECT,
			NETWORK_DISCONNECT,


			TRANSPORT_CONNECTED,  //10
			TRANSPORT_DISCONNECTED,

			DATA_RECEIVED,
			GOOD_FRAME_RECEIVED,
			BAD_FRAME_RECEIVED,
			NOTHING_RECEIVED, //15
			DATA_SENT,

			CONNECT_ERROR,
			READ_ERROR,         //18
			WRITE_ERROR,
			LAST_EV_TAG        //20
		};

		enum HypETransparentStates
		{
			STATE_IDLE,
			STATE_CONNECTING,
			STATE_CONNECTED
		};


	protected:
		void ClearQueue();
		void ClearBuffers();
		virtual void WaitEvent();

		void ConnectCompleted();
		void WriteCompleted();
		void ReadCompleted(bool wait=true);
		virtual void Discon();
		
		virtual void StartRead();
		void EraseReadBuf(size_t p=(size_t)-1);
		virtual void ClearReadBuf();
		
		virtual void BuildFrame();
		virtual void CheckFrame();

		virtual void StIdle();
		virtual void StConnecting();
		virtual void StConnected();

		virtual void TransportConnected();
		virtual void BeforeDisconnectTransport();
		
		virtual void BeforeDelete();

	private:
		static  void Task( );
		virtual void StateMachine();


	private:
		virtual void DoReadReq( HypCEvent &event, bool bWaitForData=true );
		virtual void DoWriteReq( HypCEvent &event, HypCBuffer* pBuffer );
		virtual void DoConnectReq(HypCEvent &event);
		virtual void DoDisconnectReq(HypCEvent &event);
		virtual void DoCancelRequest( HypETransportOperations requestId );

	protected:
		HypCTransport *m_Transport;
		HypCBuffer * m_TxBuffer;
		SimpleVector <HypCBuffer *> m_RxQ;

		int m_LastEvent, m_Status, m_Opt;
		HypCBuffer *m_ReadBuffer;

		HypCEvent m_Event;

		HypCEvent *m_pReadEvent;
		HypCEvent *m_pWriteEvent;
		HypCEvent *m_pConnectEvent;
		HypCEvent *m_pDisconnectEvent;
		int m_State; //fixme int
		DWORD m_nJobNr;
		bool m_ReadInProgress;
		bool m_DeleteDone;
};

#endif //_HYPCTRANSPARENTCOMMON_HPP_
