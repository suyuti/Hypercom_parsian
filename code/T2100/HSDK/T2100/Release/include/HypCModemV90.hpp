//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPCMODEMV90_HPP_
#define _HYPCMODEMV90_HPP_

//=============================================================================
//
// HypCModemV90.hpp
//
// Modem control.
//
//=============================================================================
#include <eftos.h>
#include "HypCModemV22b.hpp"
#include "HypCV29.hpp"
#include "HypCSerialPort.hpp"


class HypCModemV90 : public HypCModemV22b, public HypCV29
{
public:

	HypCModemV90( HypCSerialPort *serial, int deviceId );

	// Destructor
	virtual ~HypCModemV90();

protected:
	virtual const char *CmdModemProto();
	virtual const char *CmdCommonAfterCountry();
	virtual const char *CmdCompression();
	virtual void CmdToneWaitTime(HypCBuffer &buf);

	virtual void ChatScriptCompleted();
	virtual void StateMachine();
	virtual bool CheckConfig();

	virtual bool DoPreInit();

private:
	virtual void PrintConfigBeforeDialing();
	void ResetModem();
};


#endif // _HYPCMODEMV90_HPP_
