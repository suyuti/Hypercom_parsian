//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCHYPVISA_HPP_
#define _HYPCHYPVISA_HPP_

#include "HypCVisaCommon.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/// Default T1 timer value for Hypercom VISA protocol.
#define HYPVISA_T1_TIMER 15000

/// Default T2 timer value for Hypercom VISA protocol.
#define HYPVISA_T2_TIMER  3000

/**
	@brief Hypercom VISA protocol implementation.
	
	The protocol always waits ENQ, and does not send DLE EOT during
	disconnect, EOT from host is ignored.

	Packet format:
	@<STX@> @<LL LL@> @<data@> @<CRC1 CRC2@>
	LL LL - binary length. CRC - Hypercom CRC16 code. Maximum data size 4199.

	Supported config parameters:
	@li UseACK - default true.
	@li TimerT1 - wait ENQ timer (default value HYPVISA_T1_TIMER).
	@li TimerT2 - wait ACK timer (default value HYPVISA_T2_TIMER).

	Single byte frames are sent as is, without framing.
*/
class HypCHypercomVisa : public HypCVisaCommon
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCHypercomVisa(HypCTransport *transport, int nDeviceID);

	/**
		@brief Destructor.
	*/
	virtual ~HypCHypercomVisa();

protected:
	/**
		@brief Build transaction frame.
	*/
	virtual void BuildFrame();

	/**
		@brief Process frame received from module.
	*/
	virtual void CheckFrame();

	/**
		@brief Send frame to next layer.
	*/
	virtual void SendFrame();

	/**
		@brief Set protocol configuration.

		@param cfg Configuration set to take values from.
	*/
	virtual void DoSetConfig (const HypCConfig &cfg);
};

/**
	@}
*/

#endif //_HYPCHYPVISA_HPP_
