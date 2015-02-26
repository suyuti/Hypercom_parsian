//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCETHERNETTRANSPARENT_HPP_
#define _HYPCETHERNETTRANSPARENT_HPP_

#include "HypCEthernetCommon.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief TCP/IP stack over HEMS in transparent mode
*/
class HypCEthernetTransparent : public HypCEthernetCommon
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Calls parent constructor.

		@see HypCEthernetCommon()
	*/
	HypCEthernetTransparent( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCEthernetTransparent();

protected:
	/**
		@brief Add framing to message for transmitting.

		@note Message resides in m_TxBuffer.
	*/
	virtual void BuildFrame();

	/**
		@brief Parse incoming frame.

		@return GOOD_FRAME_RECEIVED.

		@note Transparently pass frame to upper layer.
	*/
	virtual TransparentCommonEvents ParseTransparentFrame();

	/**
		@brief Get Ethernet module mode.

		@return HEMS_MODE_TRANSPARENT.
	*/
	virtual uint8_t EthernetMode();
};

/**
	@}
*/

#endif //_HYPCETHERNETTRANSPARENT_HPP_
