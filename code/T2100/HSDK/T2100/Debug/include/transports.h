//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _TRANSPORTS_H_
#define _TRANSPORTS_H_

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Definition of supported transport protocols.
*/
enum HypETransports
{
	NONE = 0,               ///< None or transparent transport.
	SERIAL,                 ///< Serial line.
	MODEM,                  ///< Modem.
	SDLC_PROTO,             ///< SDLC.
	SPDH_PROTO,             ///< SPDH Base24.
	APACS40_PROTO,          ///< APACS40.
	VISA_AUTH_PROTO,        ///< Visa authorization.
	VISA_CAPTURE_PROTO,     ///< Visa capture.
	VISA_EMEA_CAPTURE_PROTO, ///< Visa EMEA capture.
	POSMINI_PROTO,          ///< POS Mini NAK.
	EUROCONEX_AUTH_PROTO,   ///< Euroconex.
	HYPERCOM_VISA_PROTO,    ///< Hypercom Visa.
	PPL_PROTO,              ///< PPL.
	PPP_PROTO,              ///< PPP.
	TCPIP_PROTO,            ///< TCP/IP.
	POSLAN_PROTO,           ///< POS LAN.
	ACS_PROTO,              ///< ACS router.
	FDR_PROTO,              ///< FDR.
	NEVADA_PROTO,           ///< SPDH Nevada.
	USER_DEFINED_PROTO      ///< Protocol defined by user.
};

/**
	@brief Hardware module types.
*/
enum HypEModuleType
{
	HW_NOMODULE,            ///< No module installed.
	HW_MODEMV22,            ///< Conexant V22 modem.
	HW_GSMMODEM_OLD,        ///< GSM modem with Emergency Off connected to DTR.
	HW_MODEMV90,            ///< Conexant V90 modem.
	HW_ETHERNET,            ///< Ethernet module.
	HW_POSLAN,              ///< POS LAN module.
	HW_BLUETOOTH,           ///< BlueTooth module.
	HW_GSMMODEM,            ///< GSM modem.
	HW_MODEMISDN,           ///< ISDN modem.
	HW_DIALBACKUP,          /**< DialBackup module. A special module type
	                             that should be used by HW_ResetCommModule,
	                             but not by the HypCCommunication class.
	                             For HypCCommunication HW_GSMMODEM and
	                             HW_MODEMV90 should be used instead (as
	                             applicable). */
	IncompatibleDevice,     ///< Unknown device.
	HW_BLUETOOTH_EXT        ///< BlueTooth module in D2100.
};

/**
	@}
*/

#endif //_TRANSPORTS
