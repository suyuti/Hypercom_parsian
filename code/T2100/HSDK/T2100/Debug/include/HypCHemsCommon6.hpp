//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2006
//=============================================================================
#ifndef _HYPCHEMSCOMMON6_HPP_
#define _HYPCHEMSCOMMON6_HPP_

#include "HypCHemsCommon.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Base of all HEMS v1.6 powered transports.
*/
class HypCHemsCommon6 : public HypCHemsCommon
{
protected:
	/// Current HEMS revision.
	static const uint8_t HEMS_PROTOCOL_REVISION = 0x06;

	/// HEMS v1.6 events.
	enum HypEHems6Events
	{
		LAST_EV_TAG=HypCHemsCommon::LAST_EV_TAG ///< Next unused event.
	};

public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCHemsCommon6( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCHemsCommon6();

protected:
	/**
		@brief Process frame received from module.
	*/
	virtual void CheckFrame();

	/**
		@brief Parse transaction frame.

		@return NOTHING_RECEIVED
	*/
	virtual TransparentCommonEvents ParseTransactionFrame();

	/**
		@brief Send HEMS v1.6 message.

		Adds HEMS v1.6 framing and send message to the underlying level.

		@param event Event to send notification to upon transmit
		             completion.
		@param request Data pointer.
		@param len Data length.

		@return Result of transmission.
	*/
	bool TransmitService( HypCEvent &event, const uint8_t *request, size_t len );
};

/*
	@}
*/

#endif //_HYPCHEMSCOMMON6_HPP_
