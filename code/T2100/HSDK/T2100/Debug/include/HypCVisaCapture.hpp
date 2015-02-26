//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCVISACAPTURE_HPP_
#define _HYPCVISACAPTURE_HPP_

#include "HypCVisaAuth.hpp"


//default Visa data capture protocol timers
#define VISA_CAP_T1_TIMER 15000
#define VISA_CAP_T2_TIMER 16000
#define VISA_CAP_T3_TIMER 4000
#define VISA_CAP_T4_TIMER 30000
#define VISA_CAP_T5_TIMER 2000

//=============================================================================
//
// HypCVisaCapture
//
// Base Class:
//      HypCVisaCommon
//
// Overview:
//	This class provides visa data capture link level protocol
//  ConnectReq return following status:
//		HTETIMEOUT if timer T1 expired
//		HTENOERROR after underlaing transport has been connected and ENQ
//			 received. (Awaiting ENQ is switched off by config parameter)
//		HTECOMM if EOT received instead of ENQ during connect
//		HTELOSTCOMMS if underlaing lost communications (lost carrier)
//		HTECANCELIO if DicconectReq has been performed during connect
//
// ReadReq/WriteReq return following status:
//		HTETIMEOUT if timers T3,T4 expired
//		HTECOMM if EOT received
//		HTELOSTCOMMS if underlaing lost communications (lost carrier)
//		HTELOSTCOMMS if retry counter exeeded
//		HTELOSTCOMMS if out-of-sequence acknowledge has been received while
//			Timer T2 does not expire yet
//		HTECANCELIO if DicconectReq has been performed during io
//		HTENOERROR if io-operation has been performed successfully
//
// Supported config parameters:
//		TimerT1
//		TimerT2
//		TimerT3
//		TimerT4
//		WaitENQ
//		VisaAppType - defines batch mode (single, multiple or auto detect),
//				auto detect checks Application Type field inside visa
//				packet (see doc Eis1181). 
//				See enum HypCVisaAuth::HypEVisaAppTypes.
//				default - AUTO_DETECT
//		Retries - retry counter. Default - 3
//
// Related Documents:
// EIS1052.pdf
//		EXTERNAL INTERFACE SPECIFICATION
//		SECOND GENERATION
//		DATA CAPTURE LINK LEVEL PROTOCOL
//		EIS 1052 VERSION 3.0
//
// See Also:
//      HypCTransport, HypCVisaAuth, HypCEvent
//
//=============================================================================
class HypCVisaCapture : public HypCVisaCommon
{
//=============================================================================
// Member functions
//=============================================================================
	public:
		// Creates visa protocol.
		HypCVisaCapture( HypCTransport *transport, int nDeviceID);

		// Destructor
		virtual ~HypCVisaCapture();

	protected:
		virtual void StateMachine();
		virtual bool StCommon();
		virtual void StSendReq();
		virtual void StWaitACK();
		virtual void StWaitENQ2();
		virtual void StWaitRSP();
		virtual void StWaitEOT();

		void InitVars();

		virtual void DoSetConfig (const HypCConfig &cfg);
		virtual void BuildFrame();
		virtual int CreateExpectMask();

	private:
		//m_Trailer == true for last sent frame
		bool m_Trailer;
		char m_Mode,m_CurrentMode;
};

#endif //_HYPCVISACAPTURE_HPP_
