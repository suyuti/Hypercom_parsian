//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPCMODEMV22B_HPP_
#define _HYPCMODEMV22B_HPP_

//=============================================================================
//
// HypCModemV22b.hpp
//
// Modem control.
//
//=============================================================================
#include <eftos.h>
#include "HypCModem.hpp"
#include "HypCSerialPort.hpp"
#include "HypCEvent.hpp"
#include "HypCTimer.hpp"
#include "HypCConfig.hpp"
#include "HypCV80.hpp"


//=============================================================================
//
// HypCModem
//
// Base Class:
//      HypCModem
//
// Overview:
//      This class provides controlling routines for conexant V22b modem and
//      used by the HypCCommunictaion to establish dialup connections.
//
// Related Documents:
//      HypCChat.pdf                  Description of modem command script
//                                    used by this class.
//
// See Also:
//      HypCModem, HypCSerialPort, HypCChat
//
//=============================================================================

class HypCModemV22b : public HypCV80, public HypCModem
{
//=============================================================================
// Member structures, enumerations, and class definitions
//=============================================================================
	public:
		//Modem Protocols supported by Conexant modems
		enum HypEConexantModemProto 
		{
			HighestSpeed=HypCModem::HighestSpeed,
			VoiceDial=HypCModem::VoiceDial,
			V21,
			V21FC,
			Bell212a,
			Bell212aFC,		//fast connect
			V22,
			V22FC,			//fast connect
			V22bis,
			Bell103,
			V32,
			V32bis,
			V34,
			K56flex,
			V90,
			V92,
			V29_V22bis,		//V.29FC, V.22bis fallback
			V29=V29_V22bis, 
			V29a_V22bis,	//V.29FC alernate, V.22bis fallback
			V29a=V29a_V22bis,
			V29_V22FC,		//V.29FC, V.22FC fallback
			V29a_V22FC,		//V.29FC alternate, V.22FC fallback

			UserDefined
		};
		//ModemMode:
		enum HypEModemMode
		{
			ASYNC		= 0x1, //default mode
			SYNC		= 0x2
		};

		enum HypEModemErrCorr
		{
			V42=0x01,
			MNP=0x02
		};

	protected:
		enum HypEDuplex
		{
			FDX		= 0x0, //default mode
			HDX		= 0x1
		};

		static const struct _ModemProtoModesTable
		{
			enum HypEConexantModemProto proto;
			enum HypEModemMode			defaultMode;
			int							acceptableModes;
			int							duplex;
		} m_modemProtoModes[];

		enum HypEModemStates
		{
			STATE_IDLE=HypCTransparentCommon::STATE_IDLE,
			STATE_CONNECTING=HypCTransparentCommon::STATE_CONNECTING,
			STATE_CONNECTED=HypCTransparentCommon::STATE_CONNECTED,
			STATE_HDX_CONNECTED
		};

//=============================================================================
// Member functions
//=============================================================================
	public:
		// Constructor
		HypCModemV22b( HypCSerialPort *serial, int deviceId );

		// Destructor
		virtual ~HypCModemV22b();

	protected:
		const struct _ModemProtoModesTable* FindProtoConfig() const;

		virtual void StConnectedVoiceDial();

		virtual const char *CmdCommon();
		virtual void CmdCountry(HypCBuffer &buf);
		virtual const char *CmdCommonAfterCountry();
		virtual const char *CmdV80();
		virtual const char *CmdErrorCorrection();
		virtual const char *CmdCompression();
		virtual const char *CmdLineInUse();
		virtual const char *CmdBlindDial();
		virtual const char *CmdPulseRatio();
		virtual const char *CmdDTMFSpeed();
		virtual void CmdToneWaitTime(HypCBuffer &buf);
		virtual void CmdPauseTime(HypCBuffer &buf);
		virtual void CmdTransmitLevel(HypCBuffer &buf);
		virtual const char *CmdModemProto();
		// Check if line busy
		virtual bool CheckLineBusy();
	
		virtual bool CheckConfig();
		virtual void CreatePreinitScript();
		virtual void CreateChatScript();
		virtual void ChatScriptCompleted();
		virtual bool CheckLine();
		virtual int DoGetLineStatus();
		virtual int DoGetSignalLevel();

		bool CheckLineData();
		bool CheckLineVoice();

	private:
		virtual void PrintConfigBeforeDialing();

//=============================================================================
// Member variables
//=============================================================================
	protected:
		enum HypEConexantModemProto m_Proto;
		bool m_SyncMode;
		bool m_V29HalfDuplexMode;
		// Min disc timer
		HypCTimer m_MinDiscTimer;
};


#endif // _HYPCMODEM_HPP_
