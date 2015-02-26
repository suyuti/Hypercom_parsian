//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCETHERNETCOMMON_HPP_
#define _HYPCETHERNETCOMMON_HPP_

#include "HypCHemsCommon6.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Base for Ethernet transports
*/
class HypCEthernetCommon : virtual public HypCHemsCommon6
{
public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.

		@see HypCHemsCommon()
	*/
	HypCEthernetCommon( HypCTransport *transport, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCEthernetCommon();

protected:
	/**
		@brief Incoming message parser.

		@param tag Tag value.
		@param tagsize Tag size.
		@param posn Position in buffer.

		@note Message is in m_ReadBuffer.
	*/
	virtual bool TagParser( HypEHemsTags tag, size_t tagsize, size_t posn );

	/**
		@brief Get IP addres from message.

		@param data Pointer to address.

		@return 32bit address value.

		@note The IP address under pointer is stored in big endian
		      format.
	*/
	uint32_t RetrieveIP( const char *data );
	
	/**
		@brief Store IP address to message.

		@param addr 32bit address value.
		@param data Pointer to destination.

		@return Incremented (+4) pointer to destination.

		@note Destination will be filled in the big endian format.
	*/
	char *InsertIP( uint32_t addr, char *data );

	/**
		@brief Send start command to Ethernet module.

		Creates and sends HEMS START message.

		@return The result of transmitting.
	*/
	virtual bool DoSendStart();

	/**
		@brief Get Ethernet module mode.

		@return Ethernet module mode.
	*/
	virtual uint8_t EthernetMode() = 0;
};

/**
	@}
*/

#endif //_HYPCETHERNETCOMMON_HPP_
