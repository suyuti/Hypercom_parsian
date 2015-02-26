//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCMODEMGSM_HPP_
#define _HYPCMODEMGSM_HPP_

#include <eftos.h>
#include "HypCModem.hpp"
#include "HypCSerialPort.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Transmission protocols for GSM network.
*/
enum GsmProto
{
	GSM_V32,                ///< V.32 protocol.
	GSM_V110                ///< V.110 protocol.
};

/**
	@brief GSM modem handler implementation.
*/
class HypCModemGSM : public HypCModem
{
public:
	/**
		@brief Constructor.

		@param serial Lower layer transport.
		@param nDeviceID Not used.

		@note Initializes work variables.
	*/
	HypCModemGSM( HypCSerialPort *serial, int nDeviceID );

	/**
		@brief Destructor.
	*/
	virtual ~HypCModemGSM();

protected:
	virtual const char *CmdCommon();
	virtual const char *CmdFlowControl();
	virtual int DoGetLineStatus();
	virtual int DoGetSignalLevel();
	virtual void CreateChatScript();
	virtual void CreatePreinitScript();

	/**
	        @brief Executes before disconnecting transport.
	*/
	virtual void BeforeDisconnectTransport();
};

/**
	@}
*/

#endif // _HYPCMODEMGSM_HPP_
