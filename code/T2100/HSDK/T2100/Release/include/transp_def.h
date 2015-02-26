//=============================================================================
//                         Hypercom Inc (c) Copyright 2003
//                         Viking
//=============================================================================
#ifndef _TRANSP_DEF_HPP_
#define _TRANSP_DEF_HPP_

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Common definitions of request IDs for transport protocols.
*/
enum HypETransportOperations
{
	CONNECT_REQUEST = 0,    ///< Connect to the remote.
	DISCONNECT_REQUEST,     ///< Disconnect.
	READ_REQUEST,           ///< Read request.
	WRITE_REQUEST,          ///< Write request.
	LOST_COMMS,             /**< Disconect has happened because of network.
	                             E.g. lost carrier for modem or dropping
	                             DSR signal for com port. */
	PIN_REQUEST,            /**< Request for monitoring serial port pins
	                             (DSR, CTS, DCD). */
	USERDEFINED_REQUEST	///< Start of user defined request IDs.
};

/**
	@}
*/

#endif // _TRANSP_DEF_HPP_
