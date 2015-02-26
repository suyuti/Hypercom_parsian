//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCACSCOMMUNICATION_HPP_
#define _HYPCACSCOMMUNICATION_HPP_

#include "HypCCommunication.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Communication handler class for use with ASC router.
*/
class HypCACSCommunication : public HypCCommunication
{
public:
	/**
		@brief Constructor.

		@param deviceId Not used.
		@param module Communication board module type (should be
		              BlueTooth always).

		@note Initializes work variables
	*/
	HypCACSCommunication( int deviceId, enum HypEModuleType module );

	/**
		@brief Destructor.
	*/
	virtual ~HypCACSCommunication();

	/**
		@brief Retrieves connection state of the terminal modem.

		@return Connection state.
	*/
	virtual HypCTransport::HypETransportStatus GetModemStatus();

	/**
		@brief Retrieves connection state of the ACS.

		@return Connection state.
	*/
	virtual HypCTransport::HypETransportStatus GetGateStatus();

protected:
	/**
		@brief Verify that current config is valid.

		@return true: Config is valid.<br>
		        false: Config is invalid.
	*/
	virtual bool CheckConfig();

	/**
		@brief Process connecting state.
	*/
	virtual void StConnecting();

protected:
	/// Default configuration of HEMS protocol.
	HypCConfig m_HEMSCfg;

	/// Default modem mode for ACS.
	HypCCommMode m_ACSMode;

	/// Default modem mode for ACS multichannel mode.
	HypCCommMode m_ACSModeMC;
};

/**
	@}
*/

#endif // _HYPCACSCOMMUNICATION_HPP_
