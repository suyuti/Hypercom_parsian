//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
//
// CommConfig.cpp
//
// Configuration for HypCCommunication class. It could be redefined by user
//
//=============================================================================
#include "transports.h"
#include "HypCCommunication.hpp"
#include "HypCCommMode.hpp"

#include "HypCTransportFactory.hpp"
#include "HypCSerialPortFactory.hpp"
#include "HypCModem.hpp"
#include "HypCModemV22b.hpp"
#include "HypCModemGSM.hpp"
#include "HypCModemFactory.hpp"
#include "HypCPadLogonModemFactory.hpp"
#include "HypCModemProxyFactory.hpp"
#include "HypCModemBTProxy.hpp"

#include "HypCSDLC.hpp"
#include "HypCApacs40Hypercom.hpp"
#include "HypCPosMiniNak.hpp"
#include "HypCSPDHBase24.hpp"
#include "HypCSPDHNevada.hpp"
#include "HypCVisaAuth.hpp"
#include "HypCVisaCapture.hpp"
#include "HypCVisaEmeaCapture.hpp"
#include "HypCEuroConexAuth.hpp"
#include "HypCHypercomVisa.hpp"
#include "HypCPPL.hpp"
#include "HypCTcpIp.hpp"
#include "HypCLan.hpp"
#include "HypCFDR.hpp"

//modem modes table
static const struct _modemModes
{
	int ModeNo;
	int ModemProto;
	enum HypETransports LinkProto;
} modemModesTable [] =
{
	{ 0x00, HypCModemV22b::Bell212aFC,	SDLC_PROTO},
	{ 0x02, HypCModemV22b::V22FC,		SDLC_PROTO},
	{ 0x06, HypCModemV22b::V22bis,		SDLC_PROTO},
	{ 0x08, HypCModemV22b::V22bis,		SDLC_PROTO},
	{ 0x31, HypCModemV22b::V22,		SDLC_PROTO},
	{ 0x32, HypCModemV22b::Bell212a,	SDLC_PROTO},
	{ 0x72, HypCModemV22b::V29_V22bis,	SDLC_PROTO},
	{ 0x73, HypCModemV22b::V29_V22FC,	SDLC_PROTO},
	{ 0x75, HypCModemV22b::V29a_V22bis,	SDLC_PROTO},
	{ 0x76, HypCModemV22b::V29a_V22FC,	SDLC_PROTO},
	{ 0x45, HypCModemV22b::V22,		NONE},
	{ 0x46, HypCModemV22b::Bell212a,	NONE},
	{ 0x52, HypCModemV22b::V22bis,		NONE},
	{ 0x65, HypCModemV22b::V22,		NONE}, //V42 
	{ 0x66, HypCModemV22b::V22bis,		NONE}, //V42

#ifndef APP_MGR
	{ 0x21, HypCModemV22b::Bell103,		FDR_PROTO},
	{ 0x22, HypCModemV22b::Bell212a,	FDR_PROTO},

	//for epsco protocol
	{ 0x40, HypCModemV22b::V22,		NONE},

	{ 0x58, HypCModemV22b::V22bis,		VISA_CAPTURE_PROTO},
	{ 0x59, HypCModemV22b::V22,		EUROCONEX_AUTH_PROTO},
	
	{ 0x61, HypCModemV22b::V21FC,		APACS40_PROTO},
	{ 0x63, HypCModemV22b::V22bis,		APACS40_PROTO},

	{ 0x12, HypCModemV22b::Bell103,		VISA_AUTH_PROTO},
	{ 0x13, HypCModemV22b::V21FC,		VISA_AUTH_PROTO},
	{ 0x14, HypCModemV22b::Bell212a,	VISA_AUTH_PROTO},
	{ 0x15, HypCModemV22b::V22,		VISA_AUTH_PROTO},
	{ 0x16, HypCModemV22b::V22bis,		VISA_AUTH_PROTO},
	{ 0x17, HypCModemV22b::V22bis,		VISA_AUTH_PROTO},
	{ 0x93, HypCModem::HighestSpeed,	VISA_AUTH_PROTO},
	{ 0x42, HypCModemV22b::V22,		VISA_EMEA_CAPTURE_PROTO},
	{ 0x41, HypCModemV22b::V21,		VISA_EMEA_CAPTURE_PROTO},

	{ 0x01, HypCModemV22b::Bell212a,	VISA_CAPTURE_PROTO},
	{ 0x03, HypCModemV22b::V22,		VISA_CAPTURE_PROTO},
	{ 0x04, HypCModemV22b::Bell103,		VISA_CAPTURE_PROTO},
	{ 0x05, HypCModemV22b::V21,		VISA_CAPTURE_PROTO},
	{ 0x09, HypCModemV22b::V22bis,		VISA_CAPTURE_PROTO},
#endif


	{ 0x68, HypCModem::HighestSpeed,	HYPERCOM_VISA_PROTO},//V42+MNP
	
	{ 0x90, HypCModem::HighestSpeed,	NONE},	  //V42+MNP
	{ 0x91, HypCModem::HighestSpeed,	POSMINI_PROTO}, //V42+MNP
	
#ifndef APP_MGR
	{ 0xA0, HypCModemV22b::V22,		SPDH_PROTO},
	{ 0xA1, HypCModemV22b::V22bis,		SPDH_PROTO},
	{ 0xA2, HypCModem::HighestSpeed,	PPL_PROTO},	//V42+MNP
	{ 0xA3, HypCModemV22b::V22,		NEVADA_PROTO},
	{ 0xA4, HypCModemV22b::V22bis,		NEVADA_PROTO},
#endif

	{ 0xE0, HypCModemV22b::V22,		TCPIP_PROTO}, //(TCPIP + PPP) -> modem -> serial 1
	{ 0xE1, HypCModemV22b::V22,		TCPIP_PROTO}, //V42+MNP
	{ 0xE2, HypCModemV22b::V22bis,		TCPIP_PROTO}, //
	{ 0xE3, HypCModemV22b::V22bis,		TCPIP_PROTO}, //V42+MNP
	{ 0xE4, HypCModemV22b::HighestSpeed,	TCPIP_PROTO}, //no V42+MNP
	{ 0xE5, HypCModem::HighestSpeed,	TCPIP_PROTO},  //V42+MNP

	{ 0xBD, HypCModemV22b::VoiceDial,	NONE},			//VoiceDialing

	{ 0xF1, HypCModemV22b::V22,		SDLC_PROTO}, // modem mode 31 + TotalControl bugfix
	{ 0xF2, HypCModemV22b::V22FC,		SDLC_PROTO}, // modem mode 02 + TotalControl bugfix
	{ 0xF6, HypCModemV22b::V22bis,		SDLC_PROTO}  // modem mode 06 + TotalControl bugfix
};




//insert  supported comm modes configurations in m_CommModesTable
//comment unused modes!!
void HypCCommunication::InitCommModeTable()
{
	//standard dialup modes
	HypCConfig serial_cfg;
	HypCConfig modem_cfg;
	for (size_t i=0; i<sizeof(modemModesTable)/sizeof(struct _modemModes); i++)
	{
		int lp=modemModesTable[i].LinkProto;
		int mm=modemModesTable[i].ModeNo;

		serial_cfg.Clear();
		modem_cfg.Clear();
		//set default parameters depending on link protocol
		switch (lp)
		{
		case SDLC_PROTO:
			serial_cfg.Set(HC_SerialDriverMode,HYP_V80);
			modem_cfg.Set(HC_ModemMode, HypCModemV22b::SYNC);
			break;
#ifndef APP_MGR
		case APACS40_PROTO:
			serial_cfg.Set(HC_ReadIntervalTimeout, 500);
			serial_cfg.Set(HC_SerialDriverMode,HYP_APACS40);
			serial_cfg.Set(HC_LineMode,HYP_7E1);
			break;
#endif
		case HYPERCOM_VISA_PROTO:
#ifndef APP_MGR
		case PPL_PROTO:
#endif
			serial_cfg.Set(HC_ReadIntervalTimeout, 2000);
			serial_cfg.Set(HC_SerialDriverMode,HYP_HYPVISA);
			break;
		case POSMINI_PROTO:
			serial_cfg.Set(HC_ReadIntervalTimeout, 2000);
			serial_cfg.Set(HC_SerialDriverMode,HYP_POSMINI);
			break;
		case TCPIP_PROTO:
			serial_cfg.Set(HC_ReadIntervalTimeout, 2000);
			serial_cfg.Set(HC_SerialDriverMode,HYP_PPP);
			break;
#ifndef APP_MGR
		case NEVADA_PROTO:
		case SPDH_PROTO:
		case EUROCONEX_AUTH_PROTO:
		case VISA_CAPTURE_PROTO:
		case VISA_AUTH_PROTO:
		case VISA_EMEA_CAPTURE_PROTO:
		case FDR_PROTO:
			serial_cfg.Set(HC_LineMode,HYP_7E1);
#endif
		case NONE:
		default:
			serial_cfg.Set(HC_ReadIntervalTimeout, 100);
			serial_cfg.Set(HC_SerialDriverMode,HYP_NORMAL);
			break;
		}

		modem_cfg.Set(HC_ModemProto, modemModesTable[i].ModemProto);
		if ((mm==0x65) || (mm==0x66))
		{
			modem_cfg.Set(HC_ModemErrorCorrection,HypCModemV22b::V42);
		}
		if ((mm==0xE1) || (mm==0xE3))
		{
			modem_cfg.Set(HC_ModemErrorCorrection,HypCModemV22b::V42|HypCModemV22b::MNP);
		}
		if (mm==0xE4)
		{
			//switch off modem error correction and correction for Highest speed.
			//by default error correction is switched ON for Highest speed
			modem_cfg.Set(HC_ModemErrorCorrection,0);
			modem_cfg.Set(HC_ModemCompression,0);
		}
		if (mm==0x68)
		{
			modem_cfg.Set(HC_GSMProto,GSM_V110);
		}
		if ((mm==0x90) || (mm==0x91))
		{
			modem_cfg.Set(HC_GSMProto,GSM_V32);
		}

		//for epsco protocol
		if (mm==0x40)
		{
			serial_cfg.Set(HC_LineMode,HYP_7E1);
		}

		HypCTransportFactory *com1=NULL;
		HypCTransportFactory *modem=NULL;
		HypCTransportFactory *proto=NULL;
		
		HypCConfig tmpcfg(1);

		//comment unused protocol here!
		switch (lp)
		{
		case SDLC_PROTO:
			// TotalControl fix for mode F2 and F6
			if ((mm==0xf2) || (mm==0xf6))
			{
				tmpcfg.Set(HC_SDLCFeatures,SDLC_ENFORCE_HALFDUPLEX|SDLC_IGNORE_OUTOFORDER);
			}
			// Send DM in V29 modes
			if ((mm==0x72) || (mm==0x73) || (mm==0x75) || (mm==0x76))
			{
				tmpcfg.Set(HC_SDLCFeatures,SDLC_SEND_RD);
			}
			proto=new HypCFactoryT<HypCSDLC, SDLC_PROTO>(&tmpcfg);
			break;

		case POSMINI_PROTO:
			tmpcfg.Set(HC_UseACK,0);
			proto=new HypCFactoryT<HypCPosMiniNak, POSMINI_PROTO>(&tmpcfg);
			break;
#ifndef APP_MGR
		case APACS40_PROTO:
			proto=new HypCFactoryT<HypCApacs40Hypercom, APACS40_PROTO>();
			break;
		case SPDH_PROTO:
			proto=new HypCFactoryT<HypCSPDHBase24, SPDH_PROTO>();
			break;
		case NEVADA_PROTO:
			proto=new HypCFactoryT<HypCSPDHNevada, NEVADA_PROTO>();
			break;
		case EUROCONEX_AUTH_PROTO:
			proto=new HypCFactoryT<HypCEuroConexAuth, EUROCONEX_AUTH_PROTO>();
			break;
		case VISA_CAPTURE_PROTO:
			if (mm==0x58)
			{
				//single batch mode for euroconex settelment
				tmpcfg.Set(HC_VisaAppType,HypCVisaAuth::VISA_SINGLE_BATCH);
				proto=new HypCFactoryT<HypCVisaCapture, VISA_CAPTURE_PROTO>(&tmpcfg);
			}
			else
			{
				proto=new HypCFactoryT<HypCVisaCapture, VISA_CAPTURE_PROTO>();
			}
			break;
		case VISA_AUTH_PROTO:
			proto=new HypCFactoryT<HypCVisaAuth, VISA_AUTH_PROTO>();
			break;
		case VISA_EMEA_CAPTURE_PROTO:
			proto=new HypCFactoryT<HypCVisaEmeaCapture, VISA_EMEA_CAPTURE_PROTO>();
			break;
		case PPL_PROTO:
			proto=new HypCFactoryT<HypCPPL, PPL_PROTO>();
			break;
		case FDR_PROTO:
			proto=new HypCFactoryT<HypCFDR, FDR_PROTO>();
			break;
#endif
		case HYPERCOM_VISA_PROTO:
			proto=new HypCFactoryT<HypCHypercomVisa, HYPERCOM_VISA_PROTO>();
			break;
		case TCPIP_PROTO:
			proto=new HypCFactoryT<HypCTcpIp, TCPIP_PROTO>();
			break;
		case NONE:
			break;
		default:
			//unsupported mode, check next
			continue;
		}
		
		modem=new HypCModemFactory(&modem_cfg);
		com1=new HypCSerialPortFactory(&serial_cfg, 1);
	
		HypCCommMode *mode=new HypCCommMode (modemModesTable[i].ModeNo);

		//insert mode into table
		mode->AddLayer(com1);
#ifndef MAKE_D2100
		if (m_proxyModuleType == HW_BLUETOOTH)
		{
			mode->AddLayer(new HypCModemProxyFactory(&modem_cfg)); // ???
		}
#endif
		mode->AddLayer(modem);
		if (proto!=NULL)
		{
			mode->AddLayer(proto);
		}
		m_CommModesTable.AddMode(mode);
	}

	HypCTransportFactory *com;
	HypCTransportFactory *proto;
	HypCCommMode *mode;

	//0xE8 TCPIP -> serial 1
	serial_cfg.Clear();
	serial_cfg.Set(HC_ReadIntervalTimeout, 100);
	serial_cfg.Set(HC_SerialDriverMode,HYP_HEMS);
	serial_cfg.Set(HC_BaudRate,HYP_SBR_115200);
	serial_cfg.Set(HC_FlowControl,HYP_NONE_FC);

	com=new HypCSerialPortFactory(&serial_cfg, 1);
	proto=new HypCFactoryT<HypCTcpIp, TCPIP_PROTO>();
	mode=new HypCCommMode (0xE8);
	//insert mode into table
	mode->AddLayer(com);
	mode->AddLayer(proto);
	m_CommModesTable.AddMode(mode);

	// 0xEE POS LAN over serial port 1
	// use same serial_cfg as for Ethernet board
	com=new HypCSerialPortFactory(&serial_cfg, 1);
	proto=new HypCFactoryT<HypCLan, POSLAN_PROTO>();
	mode=new HypCCommMode (0xEE);
	//insert mode into table
	mode->AddLayer(com);
	mode->AddLayer(proto);
	m_CommModesTable.AddMode(mode);

#ifndef MAKE_D2100
	// 0xD0 PosMiniNak over serial port 0
	HypCConfig cfg(1);
	cfg.Set(HC_UseACK,0);

	serial_cfg.Clear();
	serial_cfg.Set(HC_ReadIntervalTimeout, 2000);
	serial_cfg.Set(HC_SerialDriverMode,HYP_POSMINI);

	com=new HypCSerialPortFactory(&serial_cfg, 0);
	proto=new HypCFactoryT<HypCPosMiniNak, POSMINI_PROTO>(&cfg);
	mode=new HypCCommMode (0xD0);
	//insert mode into table
	mode->AddLayer(com);
	mode->AddLayer(proto);
	m_CommModesTable.AddMode(mode);

	// 0xD1 PosMiniNak over serial port 2
	com=new HypCSerialPortFactory(&serial_cfg, 2);
	proto=new HypCFactoryT<HypCPosMiniNak, POSMINI_PROTO>(&cfg);
	mode=new HypCCommMode (0xD1);
	//insert mode into table
	mode->AddLayer(com);
	mode->AddLayer(proto);
	m_CommModesTable.AddMode(mode);

	// 0xD2 PadLogon over serial port 0
	serial_cfg.Clear();
	serial_cfg.Set(HC_ReadIntervalTimeout, 1000);
	com=new HypCSerialPortFactory(&serial_cfg, 0);
	modem_cfg.Clear();
	modem_cfg.Set(HC_ModemDialScript,"");
	HypCTransportFactory *modem = new HypCPadLogonModemFactory(&modem_cfg,0);
	mode=new HypCCommMode (0xD2);
	//insert mode into table
	mode->AddLayer(com);
	mode->AddLayer(modem);
	m_CommModesTable.AddMode(mode);

	// 0xD3 PadLogon over serial port 2
	com=new HypCSerialPortFactory(&serial_cfg, 2);
	modem = new HypCPadLogonModemFactory(&modem_cfg, 2);
	mode=new HypCCommMode (0xD3);
	//insert mode into table
	mode->AddLayer(com);
	mode->AddLayer(modem);
	m_CommModesTable.AddMode(mode);
#endif

	// 0xE9 Direct access to serial port 1
	serial_cfg.Clear();
	serial_cfg.Set(HC_ReadIntervalTimeout, 100);
	serial_cfg.Set(HC_SerialDriverMode,HYP_NORMAL);
	serial_cfg.Set(HC_BaudRate,HYP_SBR_115200);
	serial_cfg.Set(HC_FlowControl,HYP_NONE_FC);

	com=new HypCSerialPortFactory(&serial_cfg, 1);
	mode=new HypCCommMode (0xE9);
	//insert mode into table
	mode->AddLayer(com);
	m_CommModesTable.AddMode(mode);
}
