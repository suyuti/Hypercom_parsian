//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCAT33_HPP_
#define _HYPCAT33_HPP_

#include "HypCVisaCommon.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief CAT33 transport layer implementation (terminal side).
*/
class HypCCat33 : public HypCVisaCommon
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCCat33( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCCat33();

protected:
	/**
		@brief Set protocol configuration.

		@param cfg Configuration set to take values from.
	*/
	virtual void DoSetConfig( const HypCConfig &cfg );
};

/**
	@}
*/

#endif //_HYPCAT33_HPP_
