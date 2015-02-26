//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2006
//=============================================================================
#ifndef _HYPCTRANSPARENT_HPP_
#define _HYPCTRANSPARENT_HPP_

#include "HypCTransparentCommon.hpp"

/**
	@brief Transparent Protocol Implementation
*/
class HypCTransparent : public HypCTransparentCommon
{
	public:
		/// Data mode
		enum HypEDataMode
		{
			MODE_7E,
			MODE_7O,
			MODE_7N,
			MODE_8N
		};

		HypCTransparent(HypCTransport *transport, int nDeviceID);
		// Destructor
		virtual ~HypCTransparent();

	protected:
		virtual void BuildFrame();
		virtual void CheckFrame();
		virtual void DoSetConfig (const HypCConfig &cfg);

	protected:
		HypEDataMode m_RxMode;
		HypEDataMode m_TxMode;
		int m_ParityChar;
};

#endif //_HYPCTRANSPARENT_HPP_
