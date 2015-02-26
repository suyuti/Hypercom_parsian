//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2006
//=============================================================================
#ifndef _HYPCSIBS_HPP_
#define _HYPCSIBS_HPP_

#include "HypCVisaCommon.hpp"

/**
	@brief SIBS Protocol Implementation
*/
class HypCSIBS : public HypCVisaCommon
{
	public:
		HypCSIBS(HypCTransport *transport, int nDeviceID);
		// Destructor
		virtual ~HypCSIBS();

	protected:
		virtual void BuildFrame();
		virtual void CheckFrame();
		virtual void DoSetConfig (const HypCConfig &cfg);
		virtual size_t DecodeCtrlChar(int mask);
};

#endif //_HYPCGS1_HPP_
