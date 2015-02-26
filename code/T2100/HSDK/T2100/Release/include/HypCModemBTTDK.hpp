//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCMODEMBTTDK_HPP_
#define _HYPCMODEMBTTDK_HPP_

//=============================================================================
//
// HypCModemBTTDK.hpp
//
// TDK BlueTooth Modem (AT based, single point firmware).
//

//	BTAddr,
//	BTUUID,
//	BTEncryption,
//	BTAuthentication

//=============================================================================
#include <eftos.h>
#include "HypCModem.hpp"
#include "HypCSerialPort.hpp"


class HypCModemBTTDK : public HypCModem
{
public:

	HypCModemBTTDK( HypCSerialPort *serial, int deviceId );

	// Destructor
	virtual ~HypCModemBTTDK();

protected:
	virtual const char *CmdCommon();
	virtual int DoGetLineStatus();
	virtual int DoGetSignalLevel();
	virtual bool CheckConfig();
	virtual void CreateChatScript();
};


#endif // _HYPCMODEMBTTDK_HPP_
