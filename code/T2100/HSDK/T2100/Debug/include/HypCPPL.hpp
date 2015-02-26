//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCPPL_HPP_
#define _HYPCPPL_HPP_

#include "HypCVisaCommon.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/// PPL client waiting for ENQ after connect 20 seconds.
#define PPL_T1_TIMER 20000
/// PPL resend timer.
#define PPL_T2_TIMER 3000

/**
	@brief PPL transport layer implementation.

	The class implements PPL protocol. The protocol always waits ENQ, and
	does not send DLE EOT during disconnect, EOT from host is ignored.
	ACK/NAK is not used.

	Supported config parameters:@li
	@arg HC_TimerT1 - Wait ENQ timer (default value PPL_T1_TIMER).
	@arg HC_TimerT2 - Resend timer (default value PPL_T2_TIMER).

	@note Implemented according to the BABS and CEKAB Common Interfaces:
	Software & Parameter Download.
*/
class HypCPPL : public HypCVisaCommon
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCPPL( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCPPL();

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
		@brief Handle connected state.
	*/
	virtual void StConnected();

	/**
		@brief Wait acknowledge state.
	*/
	virtual void StWaitACK();
};

/**
	@}
*/

#endif //_HYPCPPL
