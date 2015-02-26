//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCGS1_HPP_
#define _HYPCGS1_HPP_

#include "HypCVisaCommon.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief GS1 protocol implementation.
*/
class HypCGS1 : public HypCVisaCommon
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCGS1( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCGS1();

protected:
	/**
		@brief Build transaction frame.
	*/
	virtual void BuildFrame();

	/**
		@brief Process received frame.
	*/
	virtual void CheckFrame();

	/**
		@brief Set protocol configuration.

		@param cfg Configuration set to take values from.
	*/
	virtual void DoSetConfig( const HypCConfig &cfg );

	/**
		@brief Decode control characters.

		@param mask Mask of control characters to lookup in incoming
		            buffer.

		@return Position at which control character found.
	*/
	virtual size_t DecodeCtrlChar( int mask );

protected:
	/// Use LRC flag.
	bool m_UseLRC;
};

/**
	@}
*/

#endif //_HYPCGS1_HPP_
