//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2005
//=============================================================================
#ifndef _HYPCMODEMBTPROXY_HPP_
#define _HYPCMODEMBTPROXY_HPP_

#include "HypCModemBTTDK.hpp"

/**
	@brief Implementation of proxy mode for TDK BlueTooth modem
*/
class HypCModemBTProxy : virtual public HypCModemBTTDK, virtual public HypCSerialPort
{
public:
	HypCModemBTProxy( HypCSerialPort *serial, int deviceId );

	// Destructor
	virtual ~HypCModemBTProxy();

protected:
	virtual const char *CmdCommon();
	virtual bool CheckConfig();
	virtual void CreateChatScript();

	virtual bool GetPin(enum HypESerialPortLeads pin);
	virtual void SetPin(enum HypESerialPortLeads pin, bool bState);
	virtual void PinReq(HypCEvent &event, int pins);

	virtual void StateMachine();
	virtual void DoFlush();

	virtual void StartRead();

	void StConnected();

	void ClearReadBuf();
	void BeforeDisconnectTransport();

	void DoSetConfig(const HypCConfig &cfg);
};

#endif // _HYPCMODEMBTPROXY_HPP_
