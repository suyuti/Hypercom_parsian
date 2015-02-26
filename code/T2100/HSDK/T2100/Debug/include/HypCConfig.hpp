//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCCONFIG_HPP_
#define _HYPCCONFIG_HPP_

#ifdef __cplusplus
#include "HypCMap.hpp"
#endif

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Configuration keys.
*/
enum HypEConfigKeys
{
	HC_None,                        ///< Not defined.
	HC_CommModeNo,                  ///< Communication mode number (integer in the range 0x00-0xff).
	HC_ModemModeNo = HC_CommModeNo, ///< Modem mode number (same as communication mode).
	HC_LinkProto,                   ///< Not used (enum HypELinkProto).
	HC_ModemProto,                  ///< Modem protocol (enum HypEModemProto or HypEConexantModemProto).
	HC_ModemMode,                   ///< Modem mode (enum HypCModemV22b::HypEModemMode).
	HC_ModemErrorCorrection,        ///< Error correction (enum HypCModemV22b::HypEModemErrCorr).
	HC_ModemCompression,            ///< Compression (enum HypCModemV22b::HypEModemErrCorr).
	HC_SerialDriverMode,            ///< Serial driver packetize mode (enum HypESerialDriverMode).
	HC_LineModeScript,              ///< Not used (enum HypESerialLineMode).
	HC_LineMode,                    ///< Serial driver line mode (enum HypESerialLineMode).
	HC_TelNo,                       ///< Phone number (string).
	HC_Username,                    ///< Username, may be used in chat script (string).
	HC_Password,                    ///< Password, may be used in chat script (string).
	HC_Nua,                         ///< APACS NUA (string).
	HC_Nua1 = HC_Nua,               ///< Not used (same as APACS NUA).
	HC_Nua2,                        ///< Not used (string).
	HC_CU,                          ///< SDLC CU addr (integer in range 0x00-0xff).
	HC_ConnectScript,               ///< Login chat script (string).
	HC_CountryCode,                 ///< Country code (integer, see Conexant modem documentation).
	HC_ModemInitScript,             ///< Modem init script, used only when HC_ModemDialScript is defined (string).
	HC_ModemDialScript,             ///< Modem dial script (string).
	HC_ModemATCmd,                  ///< Additional modem AT command (string).
	HC_Timeout,                     ///< Chat script connect timeout (integer, milliseconds).
	HC_ConnAttempt,                 ///< Number of attempt to setup connection (integer).
	HC_Retries,                     ///< Number of retries (integer).
	HC_BaudRate,                    ///< Baud rate (enum HypESerialBaudRate).
	HC_StopBits,                    ///< Stop bits (enum HypESerialStop).
	HC_LineParity,                  ///< Parity (enum HypESerialParity).
	HC_DataBits,                    ///< Data bits (enum HypESerialDataBits).
	HC_PulseDial,                   ///< Pulse or tone dialing (boolean, true for pulse).
	HC_FlowControl,                 ///< Flow control (enum HypESerialFlowCtrl).
	HC_ReadIntervalTimeout,         ///< Intercharacter timeout (integer, milliseconds).
	HC_ErrorChar,                   ///< Character used to replace bytes with parity error (character).
	HC_Timer,                       ///< Protocol timer (integer, milliseconds).
	HC_TimerT1 = HC_Timer,          ///< VISA timer T1 (integer, milliseconds).
	HC_TimerA = HC_TimerT1,         ///< APACS40 timer A (integer, milliseconds).
	HC_TimerT2,                     ///< VISA timer T2 (integer, milliseconds).
	HC_TimerC = HC_TimerT2,         ///< APACS40 timer C (integer, milliseconds).
	HC_TimerT3,                     ///< VISA timer T3 (integer, milliseconds).
	HC_TimerT4,                     ///< VISA timer T4 (integer, milliseconds).
	HC_TimerT5,                     ///< VISA timer T5 (integer, milliseconds).
	HC_WaitENQ,                     ///< Wait ENQ mode (boolean, true for wait).
	HC_WaitEOT,                     ///< Wait EOT mode (boolean, true for wait).
	HC_UseACK,                      ///< Send/receive ACK/NAK mode (boolean, true for use).
	HC_SendEOT,                     ///< Send EOT during disconnect (boolean, true for send).
	HC_SendDLEEOT,                  ///< Send DLE-EOT during disconnect (boolean, true for send).
	HC_UseEOT,                      ///< Not used (integer).
	HC_UseLRC,                      ///< Wait/send LRC in GS1 (boolean, true for use).
	HC_ETXinLRC,                    ///< Is ETX included in LRC (boolean, true if included).
	HC_STXinLRC,                    ///< Is STX included in LRC (boolean, true if included).
	HC_ParityChar,                  /**< Check parity in received frames (integer, if enabled should be in range 0x00-0xff, otherwise should be out of range or not defined).
	                                     @note Serial driver checks parity
	                                     in received frames. It replaces
	                                     chars with wrong parity to the
	                                     HC_ErrorChar. After that transport
	                                     from above just looks for this
	                                     special char.
	                                */
	HC_VisaAppType,                 ///< VISA Application (enum HypEVisaAppTypes).
	HC_VisaCardType,                ///< VISA credit or debit transactions (enum HypEVisaAuthCardTypes, default - credit)
	HC_LineInUse,                   ///< Line in use detection (boolean, true for detection).
	HC_BlindDial,                   ///< Blind dialing (boolean, true for blind dialing).
	HC_PulseRatio,                  ///< Dial pulse ratio (integer, 0 - 60/40, 1 - 67/33).
	HC_DTMFSpeed,                   ///< DTMF speed (integer, 0 - slow, 1 - normal, 2 - fast).
	HC_MinDiscTime,                 ///< Minimum disconnect time (integer, 0.1s).
	HC_ToneWaitTime,                ///< Tone wait time (integer, 0.1s).
	HC_PauseTime,                   ///< Pause time (integer, 0.1s).
	HC_TransmitLevel,               ///< Transmit level (integer, negative dB, i.e. use 10 for -10dB).
	HC_NII,                         ///< Not used (integer).
	HC_NII1 = HC_NII,               ///< Not used (integer).
	HC_NII2,                        ///< Not used (integer).
	HC_Ip,                          ///< Destination IP address (integer, host order).
	HC_Ip1 = HC_Ip,                 ///< 1st destination IP address (integer, host order).
	HC_Port,                        ///< Destination IP port (integer, host order).
	HC_Port1 = HC_Port,             ///< 1st destination IP port (integer, host order).
	HC_IpOptions,                   ///< Not used (integer).
	HC_IpOptions1 = HC_IpOptions,   ///< Not used (integer).
	HC_IpTimeout,                   ///< Not used (integer).
	HC_IpTimeout1 = HC_IpTimeout,   ///< Not used (integer).
	HC_Ip2,                         ///< 2nd destination IP address (integer, host order).
	HC_Port2,                       ///< 2nd destination IP port (integer, host order).
	HC_IpOptions2,                  ///< Not used (integer).
	HC_IpTimeout2,                  ///< Not used (integer).
	HC_LocalIpAddr,                 ///< Local IP address (integer, host order).
	HC_LocalNetMask,                ///< Local net mask (integer, host order).
	HC_DefaultGwAddr,               ///< Default gateway address (integer, host order).
	HC_HemsParam,                   ///< Parameter for TCP mode selection and PPP configuration.
	HC_UseHexLength,                ///< Indicate if hex length is used in protocol stack (boolean, true when used).
	HC_SslEnabled,                  ///< SSL enabled flag (boolean, true if enabled).
	HC_SslCheckCert,                ///< Pointer to check certificate function (pointer casted to integer).
	HC_SslMutualEnabled,            ///< SSL mutual authentication enabled flag (boolean, true if enabled).
	HC_SslClientCertPtr,            ///< Client certificate pointer (pointer casted to integer).
	HC_SslClientCertLen,            ///< Length of client certificate (integer).
	HC_SslPrivateKeyPtr,            ///< Private key pointer (pointer casted to integer).
	HC_SslPrivateKeyLen,            ///< Bit length of private key (integer).
	HC_SslPrivateKeyFmt,            ///< Private key format (integer, see @ref SSLInitMutual).
	HC_SslMasterKeyIdx,             ///< Master key index (integer).
	HC_PPPMode,                     ///< PPP mode (integer).
	HC_PPPUsername,                 ///< PPP username (string).
	HC_PPPPassword,                 ///< PPP password (string).
	HC_PPPConnectionTimeout,        ///< PPP connection timeout (integer, seconds).
	HC_PPPLCPTimeout,               ///< LCP timeout (integer, seconds).
	HC_BTAddr,                      ///< BlueTooth address (string).
	HC_BTUUID,                      ///< BlueTooth UUID (string).
	HC_BTEncryption,                ///< BlueTooth encription flag (boolean, true for enable).
	HC_BTAuthentication,            ///< BlueTooth authentication flag (boolean, true for enable).
	HC_ISDNTermination,             ///< Enable/disable line termination resistors for ISDN modem (boolean, true for enable).
	HC_ISDNSignalling,              ///< ISDN D channel protocol (enum IsdnSignaling).
	HC_ISDNProto,                   ///< Transmission protocol for data communication in the B channel (enum IsdnProto).
	HC_ISDN_V110_BR,                ///< Selects the line baudrate of the TA to the desired value (enum HypESerialBaudRate, only valid for V.110).
	HC_ISDN_XNR,                    ///< Setup an origination X.25 address (string, only valid for X.25 B channel).
	HC_GSMProto,                    ///< Transmission protocol for GSM network (enum GsmProto).
	HC_LANBaudRate,                 ///< POS LAN baud rate (enum HypESerialBaudRate).
	HC_LANTPDU,                     ///< POS LAN TPDU (string).
	HC_LANTPDU1 = HC_LANTPDU,       ///< Obsolete, use HC_LANTPDU instead (string).
	HC_LANConnectionType,           ///< POS LAN connection type (integer, 0 - permanent, 1 - on-demand).
	HC_LANConnectionType1 = HC_LANConnectionType, ///< Obsolete, use HC_LANConnectionType instead (integer).
	HC_LANConnectTime,              ///< POS LAN connection time (integer, seconds).
	HC_LANConnectTime1 = HC_LANConnectTime, ///< Obsolete, use HC_LANConnectTime instead (integer).
	HC_LANHostTimeout,              ///< POS LAN host timeout (integer, seconds).
	HC_LANHostTimeout1 = HC_LANHostTimeout, ///< Obsolete, use HC_LANHostTimeout instead (integer).
	HC_SNRMTimeout,                 ///< POS LAN SNRM timeout (integer, seconds).
	HC_ConnTimeout,                 ///< ACS connection timeout (integer, seconds).
	HC_ACSLinkType,                 ///< ACS link type (integer, see HEMS_LINK_* defines).
	HC_TerminalTimeout,             ///< ACS terminal timeout (integer, seconds).
	HC_RetentionTimeout,            ///< ACS retention timeout (integer, seconds).
	HC_LoadConexantPatch,           ///< Enable load patch before V.29 connection (enum PatchType).
	HC_ConexantPatchPtr,            ///< Conexant patch pointer (pointer casted to integer).
	HC_ConexantPatchLength,         ///< Conexant patch length (integer).
	HC_SDLCFeatures,                ///< SDLC protocol features (integer bitmask, see HypCSDLC.hpp).
	HC_SIMPIN,                      ///< GSM SIM PIN (string).
	HC_ModemPreinitted,             ///< Assume modem was preinitialized (boolean, true if modem was preinitialized).
	HC_FirmwareVersion,             ///< Ethernet/POS LAN firmware version (string).
	HC_RxDataMode,                  ///< Receive data mode for HypCTransparent (enum HypEDataMode).
	HC_TxDataMode,                  ///< Transmit data mode for HypCTransparent (enum HypEDataMode).
	HC_ConexantPatchID,             ///< Conexant patch ID (integer).
	HC_UserDefined                  ///< Last key in the enum.
};

#if defined(__cplusplus) || defined(DOXYGEN)

/**
	@brief Configuration container.

	HypCConfig objects are used to configure all objects derived from
	HypCTransport class.
*/
class HypCConfig : public HypCMap
{
public:
	/**
		@brief Constructor.

		@param sz Size of an empty initial controlled sequence. Memory
		          is allocated for store up to sz elements and will
		          dynamically grow as needed. Optional, defaults to 10.

		@note Initializes work variables.
	*/
	HypCConfig( size_t sz = 10 );

	/**
		@brief Copy constructor.

		@param addr Config to copy parameters from.

		@note Initializes work variables.
	*/
	HypCConfig( const HypCConfig &addr );

	/**
		@brief Destructor.
	*/
	~HypCConfig();

	/**
		@brief Copy config.

		Function merges all elements from source to destination. If
		destination already contains element with the same key as
		source, then destination element will be replaced with source
	        element.

		@param dst Destination config.
		@param src Source config.
	*/
	friend void CopyConfig( HypCConfig &dst, const HypCConfig &src );

	/**
		@brief Copy single config key.

		Function merges defined element from source to destination. If
		destination already contains element with the same key as
		source, then destination element will be replaced with source
	        element. If source does not contain defined element then
		no changes will be made.

		@param dst Destination config.
		@param src Source config.
		@param key Key to copy.
	*/
	friend void CopyConfig( HypCConfig &dst, const HypCConfig &src, int key );

	/**
		@brief Copy single config key with default (integer version).

		Function merges defined element from source to destination. If
		destination already contains element with the same key as
		source, then destination element will be replaced with source
	        element. If source does not contain defined element then it
		will be defined to default value.

		@param dst Destination config.
		@param src Source config.
		@param key Key to copy.
		@param def_val Value to use if key does not exist in the source.
	*/
	friend void CopyConfig( HypCConfig &dst, const HypCConfig &src, int key, int def_val );

	/**
		@brief Copy single config key with default (string version).

		Function merges defined element from source to destination. If
		destination already contains element with the same key as
		source, then destination element will be replaced with source
		element. If source does not contain defined element then it
		will be defined to default value.

		@param dst Destination config.
		@param src Source config.
		@param key Key to copy.
		@param def_str Value to use if key does not exist in the source.
	*/
	friend void CopyConfig( HypCConfig &dst, const HypCConfig &src, int key, const char *def_str );

private:
	/**
		@brief Copy single config key (internal).

		Function merges defined element from source to destination. If
		destination already contains element with the same key as
		source, then destination element will be replaced with source
	        element. If source does not contain defined element then
		no changes will be made.

		@param dst Destination config.
		@param src Source config.
		@param key Key to copy.

		@return true if key was copied, false otherwise.
	*/
	friend bool CopyConfigInt( HypCConfig &dst, const HypCConfig &src, int key );
};

#endif

/**
	@}
*/

#endif
