//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2006
//=============================================================================
#ifndef _HYPCSPDHNEVADA_HPP_
#define _HYPCSPDHNEVADA_HPP_

#include "HypCSPDHBase24.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Nevada SPDH transport layer implementation.
*/
class HypCSPDHNevada : public HypCSPDHBase24
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCSPDHNevada( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCSPDHNevada();

protected:
	/**
		@brief Build transaction frame.
	*/
	virtual void BuildFrame();

	/**
		@brief Send frame to next layer.
	*/
	virtual void SendFrame();

	/**
		@brief Send request state.
	*/
	virtual void StSendReq();

	/**
		@brief Wait acknowledge state.
	*/
	virtual void StWaitACK();
};

/**
	@}
*/

#endif	// _HYPCSPDHNEVADA_HPP_
