//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCAT33PINPAD_HPP_
#define _HYPCAT33PINPAD_HPP_

#include "HypCVisaCommon.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief CAT33 transport layer implementation (PINPAD side).
*/
class HypCCat33Pinpad : public HypCVisaCommon
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCCat33Pinpad( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCCat33Pinpad();

protected:
	/**
		@brief Disconnect from host.
	*/
	virtual void StDisconnecting();

	/**
		@brief Set protocol configuration.

		@param cfg Configuration set to take values from.
	*/
	virtual void DoSetConfig( const HypCConfig &cfg );
};

/**
	@}
*/

#endif // _HYPCAT33PINPAD_HPP_
