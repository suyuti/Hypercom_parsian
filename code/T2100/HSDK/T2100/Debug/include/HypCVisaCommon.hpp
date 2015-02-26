//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCVISACOMMON_HPP_
#define _HYPCVISACOMMON_HPP_

#include "HypCTimer.hpp"
#include "HypCTransparentCommon.hpp"

// Max time to wait for EOT
#define VISA_WAIT_EOT_TIMER 5000

class HypCVisaCommon : public HypCTransparentCommon
{
	protected:
		enum HypETimers
		{
			T1,
			TA=T1,
			T2,
			TC=T2,
			T3,
			T4,
			T5
		};

		enum HypEVISAStates
		{
			STATE_IDLE=HypCTransparentCommon::STATE_IDLE,
			STATE_CONNECTING=HypCTransparentCommon::STATE_CONNECTING,
			STATE_CONNECTED=HypCTransparentCommon::STATE_CONNECTED,
			STATE_SENDING_REQ=STATE_CONNECTED,
			STATE_WAIT_ENQ,
			STATE_WAIT_SECOND_ENQ,
			STATE_WAIT_RSP,
			STATE_WAIT_ACK,
			STATE_WAIT_ACK2,
			STATE_SENDING_CONF,
			STATE_WAIT_EOT,
			STATE_SENDING_OFFLINE_REQ,
			STATE_DISCONNECTING,
			STATE_LAST_TAG
		};

		enum VisaCommonEvents
		{
			ENQ_RECEIVED=HypCTransparentCommon::LAST_EV_TAG,
			EOT_RECEIVED,
			STX_RECEIVED,
			ETX_RECEIVED,
			ETB_RECEIVED,
			ACK_RECEIVED,
			BEL_RECEIVED,
			NAK_RECEIVED,
			DC2_RECEIVED,
			DLE_RECEIVED,
			LF_RECEIVED,
			VISA_PARITY_ERROR,
			TIMEOUT_T1,
			TIMEOUT_TA=TIMEOUT_T1,
			TIMEOUT_T2,
			TIMEOUT_TC=TIMEOUT_T2,
			TIMEOUT_T3,
			TIMEOUT_T4,
			TIMEOUT_T5,
			LAST_EV_TAG
		};

	public:
		HypCVisaCommon(HypCTransport *transport, int nDeviceID);

		// Destructor
		virtual ~HypCVisaCommon();

	protected:
		virtual size_t DecodeCtrlChar(int mask);

		void StartTimer(enum HypETimers t);
		/// Stop timer
		void StopTimer()
		{ m_Timer.Stop(); }

		virtual void Discon();
		virtual void TransportConnected();
		
		virtual bool StCommon();
		virtual void StWaitENQ();
		virtual void StConnected();
		virtual void StWaitACK();
		virtual void StDisconnecting();
		virtual void StWaitEOT(enum HypETimers t=T4);

		virtual void BuildFrame();
		virtual void CheckFrame();
		virtual int CreateExpectMask();
		virtual void Expect();
		void MoveToQ (size_t len);

		virtual void SendFrame();
		void SendDLEEOT();
		void Send(char data);
		/// Send ACK to transport
		void SendACK() { Send(VISAACK); }
		/// Send NAK to transport
		void SendNAK() { Send(VISANAK); }

		virtual void DoSetConfig (const HypCConfig &cfg);
		void SetConfigTimers (const HypCConfig &cfg);

	private:
		virtual void StateMachine();

	protected:
		HypCTimer m_Timer;

		//timer values in ms
		size_t m_T1; 
		size_t m_T2;
		size_t m_T3;
		size_t m_T4;
		size_t m_T5;

		size_t m_Retries;
		bool m_WaitENQ;
		bool m_WaitEOT;
		bool m_UseACK;
		bool m_SendDLEEOT;
		int m_PEChar;

		size_t m_RequestNo;
		size_t m_RetrCount;

		bool m_IsDataSent;
		bool m_XmitBusy;
		bool m_DataPending;

		bool m_STXinLRC;
		bool m_ETXinLRC;
};

#endif //_HYPCVISACOMMON_HPP_
