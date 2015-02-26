//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPCMODEM_HPP_
#define _HYPCMODEM_HPP_

//=============================================================================
//
// HypCModem.hpp
//
// Modem control.
//
//=============================================================================
#include <eftos.h>
#include "HypCSerialPort.hpp"
#include "HypCEvent.hpp"
#include "HypCTimer.hpp"
#include "HypCConfig.hpp"
#include "HypCChat.hpp"
#include "HypCBuffer.hpp"


//=============================================================================
//
// HypCModem
//
// Base Class:
//      HypCSerialTransport
//
// Overview:
//      This class provides iterface and controlling routines for
//		any AT-based modems.
//
// Related Documents:
//      HypCChat.pdf                  Description of modem command script
//                                    used by this class.
//
// See Also:
//      HypCTransparentCommon, HypCSerialPort, HypCChat
//
//=============================================================================

class HypCModem : public HypCChat 
{
//=============================================================================
// Member structures, enumerations, and class definitions
//=============================================================================
	public:
		//Common Modem Protocols:
		enum HypEModemProto 
		{
			HighestSpeed=0,
			VoiceDial
		};

	protected:
		enum HypEModemEvents
		{
			DISCONNECT_TIMER_EXPIRED=HypCTransparentCommon::LAST_EV_TAG,
			WAKEUP_TIMER_EXPIRED,
			GET_LINE_STATUS,
			GET_SIGNAL_LEVEL,
			PREINIT,
			LAST_EV_TAG
		};

//=============================================================================
// Member functions
//=============================================================================
	public:
		// Constructor
		HypCModem( HypCSerialPort *serial, int deviceId );

		// Destructor
		virtual ~HypCModem();

		// Line availability status
		int GetLineStatus();

		// Signal level
		int GetSignalLevel();

		bool PreInit();

	protected:
		virtual void DoFlush (void);
		virtual int DoGetLineStatus();
		virtual int DoGetSignalLevel();
		virtual bool DoPreInit();
		virtual void StConnectedVoiceDial();
		bool ExecScript(const char *s);
		int StatusRequest(int req);

		void CmdModemATCmd();

		virtual bool CheckConfig();
		virtual const char *CmdAbort();
		virtual const char *CmdCommon();
		virtual void CheckAbort();
		virtual bool CheckLine();
		virtual void CreateChatScript();
		virtual void CreatePreinitScript();
		virtual void ChatScriptCompleted();

		virtual void WaitEvent();
		virtual void TransportConnected();
		virtual void StateMachine();

		// Place command to modem, get response
		HypCBuffer *SendCommand(const char *str, int timeout=2000);

	private:
		virtual enum HypCTransport::HypETransportStatus DoGetStatus();
		virtual void PrintConfigBeforeDialing();

//=============================================================================
// Member variables
//=============================================================================
	protected:
		HypCBuffer m_InitScript;
		HypCBuffer m_DialScript;
		// Wakeup timer
		HypCTimer m_WakeupTimer;
		// Connecting status
		HypETransportStatus m_ConnectingStatus;
		// Line status requests
		HypCEvent *m_pStatusRequest;
		// Pointer to the serial port
		HypCSerialPort *m_pSerialPort;
		bool m_VoiceDialMode;
};


#endif // _HYPCMODEM_HPP_
