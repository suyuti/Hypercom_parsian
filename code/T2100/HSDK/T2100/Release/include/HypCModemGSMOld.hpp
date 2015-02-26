//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCMODEMGSMOLD_HPP_
#define _HYPCMODEMGSMOLD_HPP_

//=============================================================================
//
// HypCModemGSMOld.hpp
//
// Modem control for old GSM hardware.
//
//=============================================================================
#include "HypCModemGSM.hpp"

class HypCModemGSMOld : public HypCModemGSM
{
public:
	HypCModemGSMOld( HypCSerialPort *serial, int deviceId );

	// Destructor
	virtual ~HypCModemGSMOld();

protected:
	virtual const char *CmdFlowControl();
	virtual void BeforeDisconnectTransport();
};


#endif // _HYPCMODEMGSMOLD_HPP_
