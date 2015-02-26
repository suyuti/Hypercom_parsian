//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCMODEMISDN_HPP_
#define _HYPCMODEMISDN_HPP_

//=============================================================================
//
// HypCModemISDN.hpp
//
// ISDN Modem (Stollmann TA+SOC).
//
//=============================================================================
#include <eftos.h>
#include "HypCModem.hpp"
#include "HypCSerialPort.hpp"

///D channel protocols (ISDN signaling protocols)
enum IsdnSignaling
{
	///DSS1 (Euro-ISDN) signaling), default value
	ISDN_DSS1,
	///1TR6 (Germany national)
	ISDN_1TR6
};

///Transmission protocols for data communication in the B channel.
enum IsdnProto
{
	///V.110 protocol, default value
	ISDN_V110,
	///V.120 asynchronous 
	ISDN_V120,
	///X.75 SLP 
	ISDN_X75,
	/// HDLC async to sync conversion, PPP asynchronous
	ISDN_PPP_ASYNC,
	///HDLC transparent, octets are packed into HDLC frames
	ISDN_HDLC,
	///X.31 B channel, X.25 B channel
	ISDN_X25
};

/**
	@brief Internal ISDN Modem Control

  The HypCModemISDN class is used to control Optimum T2100 ISDN modem board
  (Stollman TA+SOC). The modem support following transmission method
  in B-channel:
	- V.110
	- V.120
	- X.75
	- Async PPP
	- HDLC
	- call a host connected to the X.25-network (X.31 B channel)

  The modem support following signaling protocols (D channel protocol):
	- DSS1 (Euro-ISDN)
	- 1TR6 (Germany national)
Additional signaling protocols can be available by upgrading modem firmware in the future.
Closing external TX/RX circuits via 100 Ohm termination resistors are controlled
by HC_ISDNTermination configuration parameter. The modem supports baud rate
auto detection and usually works on 115200 bit/s rate in the terminal.
Following parameters are used by the class for configuring modem:
	- HC_ISDNTermination, Enable/disable termination resistors.
	- HC_ISDNSignalling, Select ISDN D channel protocol, see IsdnSignaling
	- HC_ISDNProto, Define transmission protocol for data communication in the B channel. see IsdnProto
	- HC_ISDN_V110_BR, Select the line baudrate of the TA to the desired value (only valid for V.110). Values from HYP_SBR_1200 to HYP_SBR_38400, default value HYP_SBR_9600.
	- HC_ISDN_XNR, Setup an origination X.25 address. Only valid for X.25 B channel.
	- HC_ModemATCmd, Additional AT command. Will be inserted before ATD command in the modem initializing script.
	- HC_ConnectScript, Login chat script (see docs for chat script: chatscript.rtf)
	- HC_TelNo, Define call number.

  @see 
	-# HYPCMODEMISDN ISDN Modem Class FDS, 
	-# TA+POX, TA+HUX, TA+SOC User Manual. ISDN Terminal Adapter. Version 1.01. Date 10.2003.
*/
class HypCModemISDN : public HypCModem
{
public:

	HypCModemISDN( HypCSerialPort *serial, int deviceId );

	// Destructor
	virtual ~HypCModemISDN();

protected:

	virtual const char *CmdCommon();
	virtual int DoGetLineStatus();
	virtual int DoGetSignalLevel();
	virtual bool CheckConfig();
	virtual void CreateChatScript();
};


#endif // _HYPCMODEMISDN_HPP_
