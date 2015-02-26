//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCVISAAUTH_HPP_
#define _HYPCVISAAUTH_HPP_

//=============================================================================
//
// HypCVisaAuth.hpp
//
// Visa link level  protocol implementation.
//
//=============================================================================
#include <eftos.h>
#include "HypCVisaCommon.hpp"
#include "HypCTransport.hpp"
#include "HypCEvent.hpp"
#include "HypCTimer.hpp"

/// retry counter
#define VISA_RSP_RETRY		3




///default value for Visa T1 timer (waiting ENQ)
#define VISA_AUTH_T1_TIMER		15000
///default value for Visa T2 timer (waiting host responce)
#define VISA_AUTH_T2_TIMER		35000
///default value for Visa T3 timer (waiting acknolidge of confirmation)
#define VISA_AUTH_T3_TIMER		10000
///default value for Visa T4 timer (waiting final EOT)
#define VISA_AUTH_T4_TIMER		2000


/**
@brief Visa link level  authorization protocol.

   This class provides visa authorization link level protocol
   Following parameters are used for configure protocol:
   - HC_VisaAppType  (VISA_SINGLE_TRAN, VISA_MULTI_TRAN, VISA_AUTO_DETECT)
   - HC_VisaCardType (VISA_CREDIT, VISA_DEBIT)
   - HC_WaitENQ
   - HC_TimerT1
   - HC_TimerT2
   - HC_TimerT3
   - HC_TimerT4
   - HC_Retries

default mode: VISA_CREDIT+VISA_SINGLE_TRAN, waiting ENQ=true, T1=VISA_AUTH_T1_TIMER,
T2=VISA_AUTH_T2_TIMER, T3=VISA_AUTH_T3_TIMER, T4=VISA_AUTH_T4_TIMER,
Retr. counter=VISA_RSP_RETRY

Related Documents:
 EIS1051.pdf
		EXTERNAL INTERFACE SPECIFICATION
		SECOND GENERATION
		AUTHORIZATION LINK LEVEL PROTOCOL
		EIS 1051 VERSION 3.0

@see HypCTransport, HypCVisaCommon
*/
class HypCVisaAuth : public HypCVisaCommon
{
//=============================================================================
// Member structures, enumerations, and class definitions
//=============================================================================

	public:
		/// Application types
		enum HypEVisaAppTypes
		{
			/// auto-detect mode
			VISA_AUTO_DETECT=0,

			/// Single-transaction
			VISA_SINGLE_TRAN = '0',

			/// Multiple-transaction mode
			VISA_MULTI_TRAN = '2',

			/// interleaved mode
			VISA_INTERLEAVE = '4',

			///single-batch mode
			VISA_SINGLE_BATCH = '1',

			///multiple-batch mode
			VISA_MULTIPLE_BATCH = '3'
		};

		/// Card types
		enum HypEVisaAuthCardTypes
		{
			/// processing credit card authorization request
			VISA_CREDIT,
			/// processing debit card authorization request 
			/// (terminal sends a debit conformation message)
			VISA_DEBIT
		};

//=============================================================================
// Member functions
//=============================================================================
	public:
		// Creates visa protocol.
		HypCVisaAuth( HypCTransport *transport, int nDeviceID );

		// Destructor
		virtual ~HypCVisaAuth();

	protected:
		virtual void DoSetConfig (const HypCConfig &cfg);
		virtual void Expect();

		virtual void StateMachine();
		virtual bool StCommon();
		virtual void StWaitENQ2();
		virtual void StSendReq();
		virtual void StWaitACK();
		virtual void StSendConf();
		virtual void StWaitACK2();
		virtual void StWaitEOT();

		virtual int  CreateExpectMask();
		virtual void BuildFrame();

//=============================================================================
// Member variables
//=============================================================================
	protected:
		enum HypEVisaAppTypes m_Mode,m_CurrentMode;
		bool m_CreditTran;
};


#endif // _HYPCVISA_HPP_
