//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCPOSMININAK_HPP_
#define _HYPCPOSMININAK_HPP_

#include "HypCVisaCommon.hpp"

class HypCPosMiniNak : public HypCVisaCommon
{
	public:
		HypCPosMiniNak(HypCTransport *transport, int nDeviceID);
		// Destructor
		virtual ~HypCPosMiniNak();

	protected:
		virtual void BuildFrame();
		virtual void CheckFrame();
		virtual void DoSetConfig (const HypCConfig &cfg);
};

#endif //_HYPCPOSMININAK_HPP_
