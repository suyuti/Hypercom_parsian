//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCACSREMOTE_HPP_
#define _HYPCACSREMOTE_HPP_

#include "HypCHemsCommon6.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief HEMS protocol implementation for ACS.
*/
class HypCACSRemote : virtual public HypCHemsCommon6
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCACSRemote( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCACSRemote();

	/**
		@brief Get gate connection status.

		@return Last gate connection status received from ACS.
	*/
	enum HypCTransport::HypETransportStatus GetGateStatus();

protected:
	/**
		@brief Process service frame received from module.

		@return NOTHING_RECEIVED.
	*/
	virtual HypCTransparentCommon::TransparentCommonEvents ParseServiceFrame();

	/**
		@brief Process transaction frame received from module.

		@return GOOD_FRAME_RECEIVED.
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
	virtual bool TagParser( HypEHemsTags tag, size_t tagsize, size_t posn );

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
		@brief Adds tag to the buffer from config as byte.

		@param tag Tag ID.
		@param buf Pointer to the buffer to add tag.
		@param key Key of the config data to extract data by.
	*/
	void PackByte( HypEHemsTags tag, HypCBuffer *buf, HypEConfigKeys key );

	/**
		@brief Adds tag to the buffer from config as two bytes.

		@param tag Tag ID.
		@param buf Pointer to the buffer to add tag.
		@param key Key of the config data to extract data by.
	*/
	void PackWord( HypEHemsTags tag, HypCBuffer *buf, HypEConfigKeys key );

	/**
		@brief Adds tag to the buffer from config as string.

		@param tag Tag ID.
		@param buf Pointer to the buffer to add tag.
		@param key Key of the config data to extract data by.
		@param force_nul Force NUL string if empty string.
	*/
	void PackStr( HypEHemsTags tag, HypCBuffer *buf, HypEConfigKeys key, bool force_nul = false );

	/**
		@brief Adds IP address into the given buffer.

		@param buf Pointer to the buffer to add IP address.
		@param key Key of the config data to extract data by.
	*/
	void InsertIP( HypCBuffer *buf, HypEConfigKeys key );

	/**
		@brief Send start in HEMS protocol.

		@return false.

		@note Not used in ACS remote solution.
	*/
	virtual bool DoSendStart();

protected:
	/// Last gate connection status received from ACS.
	enum HypCTransport::HypETransportStatus m_GateStatus;
};

/**
	@}
*/

#endif //_HYPCACSREMOTE_HPP_
