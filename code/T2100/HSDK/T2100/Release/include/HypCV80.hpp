//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPCV80_HPP_
#define _HYPCV80_HPP_

//=============================================================================
//
// HypCV80.hpp
//
// V80 coder/decoder.
//
//=============================================================================
#include <eftos.h>
#include "HypCTransport.hpp"
#include "HypCTransparentCommon.hpp"

#define V80_TIMEOUT 10000  //10s FIXME

//=============================================================================
//
// HypCV80
//
// Base Class:
//      HypCTransparentCommon
//
// Overview:
//      This class implements V80 coder/decoder
//
// Related Documents:
//      HypCChat.pdf                  Description of modem command script
//                                    used by this class.
//
// See Also:
//      HypCDevice, HypCModem, HypCSerialTransport, HypCSerialPort, HypCChat
//
//=============================================================================
class HypCV80 : virtual public HypCTransparentCommon
{

//=============================================================================
// Member functions
//=============================================================================
	public:
		// Constructor
		HypCV80( HypCTransport *transport, int deviceId );

		// Destructor
		virtual ~HypCV80();
		// Reset retries count

	protected:

		virtual void BuildFrame();
		virtual void CheckFrame();
		unsigned int DecodeRate (char speed);
		void MoveToQ(size_t len);

//=============================================================================
// Member variables
//=============================================================================
	protected:
		unsigned int m_TxRate;
		unsigned int m_RxRate;
		bool m_V80Mode;
};


#endif // _HYPCV80_HPP_
