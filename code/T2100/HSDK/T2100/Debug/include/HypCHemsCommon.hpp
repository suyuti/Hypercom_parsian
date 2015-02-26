//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCHEMSCOMMON_HPP_
#define _HYPCHEMSCOMMON_HPP_

#include "hyputils.h"
#include "HypCTransparentCommon.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

#define HEMS_LINK_ST 1 ///< Single thread link.
#define HEMS_LINK_MT 2 ///< Multithread link.
#define HEMS_LINK_MC 3 ///< Multichannel link.
#define HEMS_LINK_CL 4 ///< Control link.

/**
	@brief Base of all HEMS powered transports.
*/
class HypCHemsCommon : public HypCTransparentCommon
{
protected:
	/// HEMS protocol IDs.
	enum HypEHemsProtocolDiscriminator
	{
		HEMS_TRANSPARENT_FRAME = 0xe1,  ///< Transparent frame.
		HEMS_TRANSACTION_FRAME = 0xe2,  ///< Transaction frame (v1.6 only).
		HEMS_APPLICATION_FRAME = 0xe3,  ///< Application frame.
		HEMS_FIRMWARE_LOAD_FRAME = 0xe5, ///< Firmware load frame.
		HEMS_SERVICE_FRAME = 0xe7       ///< Service frame.
	};

	/// HEMS message IDs.
	enum HypEHemsMessages
	{
		HEMS_CONNECT_REQUEST = 0x21,    ///< Connect request.
		HEMS_CONNECT_RESULT,            ///< Connect result.
		HEMS_DISCONNECT_REQUEST,        ///< Disconnect request.
		HEMS_DISCONNECT_RESULT,         ///< Disconnect result.
		HEMS_STATUS_REQUEST,            ///< Status request.
		HEMS_STATUS_RESULT,             ///< Status result.
		HEMS_START,                     ///< Start request.
		HEMS_LINK_SUCCESS,              ///< Link success (v1.6 only).
		HEMS_LINK_REQUEST               ///< Link request (v1.6 only).
	};

	/// HEMS tag IDs.
	enum HypEHemsTags
	{
		HEMS_CONTROL_OPTIONS = 0x50,    ///< Control options.
		HEMS_LOCAL_IP_CONFIG,           ///< Local IP config.
		HEMS_LOCAL_MAC_ADDRESS,         ///< Local MAC address.
		HEMS_ETHERNET_STATUS,           ///< Ethernet status.
		HEMS_TCPIP_DESTINATION,         ///< TCP/IP destination.
		HEMS_CONNECTION_STATUS,         ///< Connection status.
		HEMS_FIRMWARE_LOAD_STATUS,      ///< Firmware load status.
		HEMS_FIRMWARE_VERSION,          ///< Firmware revision.
		HEMS_SERIAL_NUMBER,             ///< Serial number.
		HEMS_POSLAN_DESTINATION,        ///< POS LAN destination.
		HEMS_POSLAN_CONFIG,             ///< POS LAN config.
		HEMS_BT_LINK_TYPE = 0x60,	///< Link type (v1.6 only).
		HEMS_BT_TERMINAL_TIMEOUT,	///< Terminal timeout, s (v1.6 only).
		HEMS_BT_RETENTION_TIMEOUT,	///< Retention timeout, s (v1.6 only).
		HEMS_BT_PHONE_NUMBER,		///< Phone number, ASCII (v1.6 only).
		HEMS_BT_DIAL_ATTEMPTS,		///< Dial attempts (v1.6 only).
		HEMS_BT_CONNECT_TIME,		///< Connect time, s (v1.6 only).
		HEMS_BT_NUA,			///< NUA, ASCII (v1.6 only).
		HEMS_BT_MODEM_AT_CMD,		///< Additional modem AT-command, ASCII (v1.6 only).
		HEMS_BT_CONNECT_SCRIPT,		///< Connect script, ASCII (v1.6 only).
		HEMS_BT_MODEM_PROTO,		///< Modem protocol (v1.6 only).
		HEMS_BT_MODEM_MODE,		///< Modem mode (v1.6 only).
		HEMS_BT_SERIAL_MODE,		///< Serial driver mode (v1.6 only).
		HEMS_BT_SERIAL_ICT,		///< Inter-character timeout, ms (v1.6 only).
		HEMS_BT_DIAL_MODE,		///< Dial mode (v1.6 only).
		HEMS_BT_BLIND_DIAL,		///< Blind dial (v1.6 only).
		HEMS_BT_MIN_DISC_TIME,		///< Minimum disconnect time, 100ms (v1.6 only).
		HEMS_BT_ISDN_TERMINATION,	///< ISDN termination (v1.6 only).
		HEMS_BT_ISDN_SIGNALLING,	///< ISDN signalling (v1.6 only).
		HEMS_BT_ISDN_PROTO,		///< ISDN Proto (v1.6 only).
		HEMS_BT_ISDN_V110_BR,		///< ISDN Baud rate (v1.6 only).
		HEMS_BT_ISDN_XNR,		///< ISDN for X.25 (v1.6 only).
		HEMS_BT_LOGON_USERNAME,		///< Logon username (v1.6 only).
		HEMS_BT_LOGON_PASSWORD,		///< Logon password (v1.6 only).
		HEMS_BT_PPP_AUTH_MODE,		///< PPP auth mode (v1.6 only).
		HEMS_BT_PPP_RETRY_TO,		///< PPP retry timeout (v1.6 only).
		HEMS_BT_GSM_SIM_PIN,		///< GSM SIM PIN (v1.6 only).
		HEMS_BT_SERIAL_LINE_MODE,	///< Serial line mode (v1.6 only).
		HEMS_BT_COUNTRY_CODE,		///< Country code (v1.6 only).
		HEMS_BT_MODEM_PATCH,		///< Modem patch ID (v1.6 only).
		HEMS_BT_RSSI_LINKQUAL = 0x90,	///< Link quality (v1.6 only).
		HEMS_BT_LINK_STATUS		///< Link status (v1.6 only).
	};

	/// HEMS modes.
	enum HypEHemsMode
	{
		HEMS_MODE_TRANSPARENT = 0x11,   ///< Transparent mode.
		HEMS_MODE_RELIABLE = 0x12,      ///< Reliable mode (depricated).
		HEMS_MODE_LAN = 0x20,           ///< POS LAN.
		HEMS_MODE_LOADER = 0x40         ///< Loader mode.
	};

	/// HEMS DHCP modes.
	enum HypEHemsDHCP
	{
		HEMS_DHCP_DISABLED = 0x00,      ///< DHCP disabled.
		HEMS_DHCP_ENABLED = 0x01        ///< DHCP enabled.
	};

	/// Current HEMS version.
	static const uint8_t HEMS_PROTOCOL_VERSION = 0x01;

	/// Current HEMS revision.
	static const uint8_t HEMS_PROTOCOL_REVISION = 0x05;

	/// Maximum socket number supported by HEMS.
	static const unsigned int HEMS_MAX_SOCKETS = 0x0A;

	/// HEMS events.
	enum HypEHemsEvents
	{
		GET_LINE_STATUS = HypCTransparentCommon::LAST_EV_TAG, ///< Get line status.
		SEND_START,                     ///< Send start request.
		STATUS_TIMEOUT,                 ///< Status request timeout.
		LAST_EV_TAG                     ///< Next unused event.
	};

	/// HEMS states.
	enum HypEHemsStates
	{
		STATE_IDLE = HypCTransparentCommon::STATE_IDLE, ///< State idle.
		STATE_CONNECTING = HypCTransparentCommon::STATE_CONNECTING, ///< State connecting.
		STATE_CONNECTED = HypCTransparentCommon::STATE_CONNECTED, ///< State connected.
		STATE_REQUEST_INFO              ///< State request info.
	};

	/// HEMS response timeout.
	static const int HEMS_RESPONSE_TIMEOUT = 3000;

public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCHemsCommon( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCHemsCommon();

	/**
		@brief Get line availability status.

		@return Line status according to HEMS specification.
	*/
	int GetLineStatus();

	/**
		@brief Send start message.

		@return Line status according to HEMS specification.
	*/
	int SendStart();

	/**
		@brief Get local configuration.

		@param cfg Reference to configuration container to put local
		           configuration to.
	*/
	void GetLocalConfig( HypCConfig &cfg );

	/**
		@brief Get maximum message length.

		@return Maximum transferrable message length.
	*/
	size_t GetMaxMessageLength( void ) { return m_MaxMessageLength; }

protected:
	/**
		@brief Process frame received from module.
	*/
	virtual void CheckFrame();

	/**
		@brief Process info request command.
	*/
	virtual void StRequestInfo();

	/*-*
		@brief Process idle state.
	*/
	virtual void StIdle();

	/*-*
		@brief Process connected state.
	*/
	virtual void StConnected();

	/**
		@brief Start command execution.

		@param operation Operation to perform.

		@return Operation result.
	*/
	int RequestOperation( HypEHemsEvents operation );

	/**
		@brief Request line availability status.

		@return true if request was successfull, false otherwise.
	*/
	bool DoGetLineStatus();

	/**
		@brief Send start message.

		@return true if request was successfull, false otherwise.
	*/
	virtual bool DoSendStart() = 0;

	/**
		@brief Parse transparent frame.

		@return NOTHING_RECEIVED
	*/
	virtual TransparentCommonEvents ParseTransparentFrame();

	/**
		@brief Parse service frame.

		@return NOTHING_RECEIVED
	*/
	virtual TransparentCommonEvents ParseServiceFrame();

	/**
		@brief Parse application frame.

		@return NOTHING_RECEIVED
	*/
	virtual TransparentCommonEvents ParseApplicationFrame();

	/**
		@brief Parse firmware load frame.

		@return NOTHING_RECEIVED
	*/
	virtual TransparentCommonEvents ParseFirmwareLoadFrame();

	/**
		@brief Process tag in service frame received from module.

		@param tag Tag value.
		@param tagsize Tag size.
		@param posn Position in buffer.

		@return true if parsed OK, false if tag is not recognized.
	*/
	virtual bool TagParser( HypEHemsTags tag, size_t tagsize, size_t posn );

	/**
		@brief Send HEMS message.

		Adds HEMS framing and send message to the underlying level.

		@param event Event to send notification to upon transmit
		             completion.
		@param request Data pointer.
		@param len Data length.

		@return Result of transmission.
	*/
	virtual bool TransmitService( HypCEvent &event, const uint8_t *request, size_t len );

private:
	/**
		@breif State machine.
	*/
	virtual void StateMachine();

protected:
	/// Line status requests.
	HypCEvent *m_pStatusRequest;

	/// Firmware version.
	HypCBuffer m_FirmwareVersion;

	/// Serial number.
	HypCBuffer m_SerialNumber;

	/// Message length limit.
	size_t m_MaxMessageLength;

	/// Local configuration.
	HypCConfig m_LocalConfig;
};

/**
	@}
*/

#endif //_HYPCHEMSCOMMON_HPP_
