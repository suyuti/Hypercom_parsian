//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCVISAEMEACAPTURE_HPP_
#define _HYPCVISAEMEACAPTURE_HPP_

#include "HypCVisaAuth.hpp"


//default Visa emea data capture protocol timers
#define VISA_EMEA_CAP_TA_TIMER 10000
#define VISA_EMEA_CAP_TC_TIMER 30000

//=============================================================================
//
// HypCVisaEmeaCapture
//
// Base Class:
//      HypCVisaCommon
//
// Overview:
//	This class provides visa data capture link level protocol
//  Packet format:
//  SOH <Block Sequence Number> STX <data> ETX LRC or
//  STX <data> ETX LRC
//
//  ConnectReq return following status:
//		HTETIMEOUT if timer TC expired
//		HTENOERROR after underlaing transport has been connected and ENQ
//			 received.
//		HTECOMM if EOT received instead of ENQ during connect
//		HTELOSTCOMMS if underlaing lost communications (lost carrier)
//		HTECANCELIO if DicconectReq has been performed during connect
//
// ReadReq/WriteReq return following status:
//		HTETIMEOUT if timers TC expired
//		HTECOMM if EOT received
//		HTELOSTCOMMS if underlaing lost communications (lost carrier)
//		HTELOSTCOMMS if retry counter exeeded
//		HTECANCELIO if DicconectReq has been performed during io
//		HTENOERROR if io-operation has been performed successfully
//
// Supported config parameters:
//		TimerA
//		TimerC  - awaiting ENQ timer
//
// Related Documents:
// VISA EMEA BATCH DATA CAPTURE EQUIPMENT SPECIFICATIONS (RELEASE 24/07/1989)
// JCCLINK CERTIFICATION GUIDE (Version 8	23 January 2004)
// APPENDIX D: BATCH DATA CAPTURE DATA LINK PROTOCOL SPECIFICATION
//
// See Also:
//      HypCTransport, HypCVisaAuth, HypCVisaCapture
//
//=============================================================================
class HypCVisaEmeaCapture : public HypCVisaCommon
{
//=============================================================================
// Member functions
//=============================================================================
	public:
		// Creates visa protocol.
		HypCVisaEmeaCapture( HypCTransport *transport, int nDeviceID);

		// Destructor
		virtual ~HypCVisaEmeaCapture();

	protected:
		virtual void DoSetConfig (const HypCConfig &cfg);
		virtual void TransportConnected();
		virtual void BuildFrame();
		virtual int CreateExpectMask();

		virtual void StWaitENQ();
		virtual void StConnected();
		virtual void StWaitACK();

		virtual void Expect();


	private:
		bool m_DLEReceived;
};

#endif //_HYPCVISAEMEACAPTURE_HPP_
