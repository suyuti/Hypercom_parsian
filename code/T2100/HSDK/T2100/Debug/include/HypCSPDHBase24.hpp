//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCSPDHBASE24_HPP_
#define _HYPCSPDHBASE24_HPP_

#include "HypCVisaCommon.hpp"

#define SPDH_T1_TIMER		30000
#define SPDH_T2_TIMER		60000
#define SPDH_T4_TIMER		4000


class HypCSPDHBase24 : public HypCVisaCommon
{
	public:
		HypCSPDHBase24(HypCTransport *transport, int nDeviceID);
		// Destructor
		virtual ~HypCSPDHBase24();

	protected:
		virtual void StWaitACK();
		virtual void StSendReq();

		virtual void StateMachine();
		virtual void BuildFrame();
		virtual int  CreateExpectMask();

		virtual void DoSetConfig (const HypCConfig &cfg);

	protected:
		bool m_ProFlag1;
};

#endif //_HYPCSPDHBASE24_HPP_
