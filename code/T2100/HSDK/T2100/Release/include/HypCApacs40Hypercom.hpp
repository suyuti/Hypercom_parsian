//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCAPACS40HYPERCOM_HPP_
#define _HYPCAPACS40HYPERCOM_HPP_

#include "HypCApacs40.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Hypercom APACS 40 transport layer implementation.

	In difference to basic APACS 40 implementation send single bytes
	without framing.
*/
class HypCApacs40Hypercom : public HypCApacs40
{

public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCApacs40Hypercom ( HypCTransport *transport, int nDeviceID ) 
		: HypCApacs40 ( transport, nDeviceID ) {}

	/**
		@brief Destructor.
	*/
	virtual ~HypCApacs40Hypercom ();

private:
	/**
		@brief Build transaction frame.
	*/
	virtual void BuildFrame();

	/**
		@brief Send frame to next layer.
	*/
	virtual void SendFrame();
};

/**
	@}
*/

#endif	// _HYPCAPACS40HYPERCOM_HPP_
