//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2005
//=============================================================================
#ifndef _HYPCACSREMOTEMC_HPP_
#define _HYPCACSREMOTEMC_HPP_

#include "HypCACSRemote.hpp"
#include "HypCEthernetReliable.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief HEMS protocol implementation for ACS (multichannel mode)
*/
class HypCACSRemoteMC : virtual public HypCACSRemote, virtual public HypCEthernetReliable
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCACSRemoteMC( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCACSRemoteMC();

protected:
	/**
		@brief Process service frame received from module.

		@return NOTHING_RECEIVED.
	*/
	virtual HypCTransparentCommon::TransparentCommonEvents ParseServiceFrame();

	/**
		@brief Process transaction frame received from module.

		@return NOTHING_RECEIVED.
	*/
	virtual HypCTransparentCommon::TransparentCommonEvents ParseTransactionFrame();

	/**
		@brief Process service frame received from module.

		@param tag Tag value.
		@param tagsize Tag size.
		@param posn Position in buffer.

		@return true: Parsed OK.<br>
		        false: Tag is not recognized.
	*/
	virtual bool TagParser(HypEHemsTags tag, size_t tagsize, size_t posn);

	/**
		@brief Build HEMS transaction frame.
	*/
	virtual void BuildFrame();

	/**
	        @brief Executed when transport gets connected.

	        @note Reads additional configuration.
	*/
	virtual void TransportConnected();

	/**
        	@brief Executed before disconnecting transport.
	*/
	virtual void BeforeDisconnectTransport();

	/**
		@brief Send start in HEMS protocol.

		@return false.

		@note Not used in ACS remote solution.
	*/
	virtual bool DoSendStart();
};

/**
	@}
*/

#endif //_HYPCACSREMOTEMC_HPP_
