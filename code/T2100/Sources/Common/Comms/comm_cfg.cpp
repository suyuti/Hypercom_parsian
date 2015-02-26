//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             comm_cfg.cpp
//      setup configuration for comm process
//
//=============================================================================

#include "HypCConfig.hpp"
#include "HypCConfigSet.hpp"
#include "HypCBuffer.hpp"
#include "HypCSerialPort.hpp"
#include "HypCModemISDN.hpp"
#include "HypCHemsCommon.hpp"
#include "transports.h"
#include "hyputils.h"
#include "sockets.h"
#include "hwconexant.h"

#include "comm_c.h"
#include "basictyp.h"

//#include "sdktime.h"
//#include "sdk.h"
//#include "sdkos.h"
//#include "sdkio.h"
//#include "sdkmem.h"

extern "C" {
#include "utllib.h"
#include "fixdata.h"
#include "rs232tab.h"
#include "siodev.inc"
#include "struct.h"
#include "comdata.h"
#include "appdata.h"
#include "tables.h"
#include "findtran.h"
#include "comm_cfg.h"
#include "SSL_auth.h"
#ifdef MAKE_CERTIFICATE
#include "crttab.h"									
#endif //  MAKE_CERTIFICATE
}


extern enum HypEModuleType COM_moduleType;



//=============================================================================
// External variables / declarations
//=============================================================================



//=============================================================================
// Private defines and typedefs
//=============================================================================
#define DIALINITTIME	40		// The connect wait time for Initialize


enum CFG_TYPE {
	ERROR_CFG,
	DIALUP_CFG,
	SERIAL1_CFG,
	SERIAL2_CFG,
	ETHERNET_CFG,
	POSLAN_CFG,
	BT_CFG,
	DIALUP_TCPIP_CFG,
};

enum TRAN_MODE {
	SETTLE_TRAN,
	COMMON_TRAN,
	INIT_TRAN,
	NAC_DIAL_TRAN,
	ICE_PACK_TRAN,
        LOGON_TRAN    
};
#define  CONNECT_ERROR USERDEFINED_REQUEST+11

static enum CFG_TYPE cfgType;
static enum TRAN_MODE tranMode;
static int  tranKey;
static int  aqID;
static int  modemMode; //current modem mode
static struct acquirer_rec *paq=NULL;

extern HypCConfigSet *COM_confSet;
extern HypCConfig    *COM_addConf;
extern bool           COM_hexLen;
extern bool           COM_useSocket;
extern bool           COM_firstCall;
extern Bool COM_GSMLowSignalLevel;
extern HypCEvent *event;
extern bool COM_DBPreInitFlag;
extern bool COM_PreInitFlag;

extern struct sockaddr_in Dest;
extern struct sockaddr_in Dest2;

static char pTelNoLastUsed[64];
static char pPPPUsernameLastUsed[32];
static char pModemATCmdLastUsed[64];

static void GetModemMode();
static void Serial1Config();
static void Serial2Config();
static void DialUpConfig();
static void EthernetConfig();
static void PosLanConfig();
static void BTConfig();
//static void DialupTCPIPConfig(); @AAMELIN Moved to DialUpConfig

static Bool SetDialConfig(HypCConfig &cfg, bool primary=true);
static void SetPadLogon (HypCConfig &cfg);
static void SetSerialPortConfig (HypCConfig &cfg);
static void SetPosLanConfig(HypCConfig &cfg);
static void SetTCPIPConfig(HypCConfig &cfg, Bool bDialBackup);
static void SetPPPConfig(HypCConfig &cfg, unsigned int to, Bool bDialBackup);
static void SetBTConfig(HypCConfig &cfg);
static void SetISDNConfig(HypCConfig &cfg);

static int  GetNumDialDest();
static void GetDialParam (int &cw, int &da, unsigned char* &pn, PHONE_STRUCT* data);
static bool ConvertNum(char *phoneStr);
static bool ConvertPrefix(char *phoneStr);
struct ipcontab *FindIPCById( UBYTE id );
//static UDWORD inet_addr(const char *s);
static bool IfDialTCPIP( );
static bool IfSyncMode( );
static bool IfHexLength();
static bool IfNII8087();

Bool IsNewPPPConnectionRequested( void );
void SavePPPSpecificParams( void );
//extern Bool IsStateSocketIdle( void );
extern void CommPermEthernetConfig(void);
extern void CommPermGSMConfig(void);

//-----------------------------------------------------------------------------
//  CommConfig        perform communication configuration
//  Parameters:       
//  Global Inputs:    DialTranKey, DIALAQID
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void CommConfig ()
{
	tranKey=DialTranKey;
	aqID=DIALAQID;

// FIXME: why??? can't use it for COM ports!
	paq=NULL;

//-----------------------------------------------------------------------------
// Derermine configuration type required (depended on comm module installed)
// and Modem Mode
//-----------------------------------------------------------------------------

	// If init via RS is active then return POSMiniNAK/serial line
	if (RSTAB.RSOPT & RSOPT_RSACTIVE)
	{
		if(RSTAB.RSOPT & RSOPT_COMNUM)
		{
			// COM2 (RS232) selected for communication (EFTOS RS2) (modem mode D1)
			cfgType=SERIAL2_CFG;	//pos mini nak over serial port 
		}
		else
		{
			// COM1 (RS422) selected for communication (EFTOS RS0) (modem mode D0)
			cfgType=SERIAL1_CFG;
		}
	}
	else
	{
		bool modem_installed=false;
		//get modem mode
		GetModemMode();
		if (tranMode==NAC_DIAL_TRAN)
		{
			COM_HPOSFCERR=HTEINVAL;
			return;
		}
		switch(COM_moduleType)
		{
			case HW_ETHERNET:
				cfgType=ETHERNET_CFG;
				break;

			case HW_BLUETOOTH:
				cfgType=BT_CFG;	//Blue tooth comm mode
				break;

			case HW_POSLAN:
				cfgType=POSLAN_CFG;	//POS LAN comm mode
				break;

			case HW_MODEMV22:
			case HW_MODEMV90:
			case HW_GSMMODEM_OLD:
			case HW_GSMMODEM:
			case HW_MODEMISDN:
				modem_installed=true;
			default:
			{
				//dial up, PPP or serial config
		
				if (modemMode==0xD0)
				{
					cfgType=SERIAL1_CFG;
					paq=NULL;
				}
				else if (modemMode==0xD1)
				{
					cfgType=SERIAL2_CFG;
					paq=NULL;
				}
				else if ((modem_installed==false)||(modemMode==-1))
				{
					cfgType=ERROR_CFG;
				}
				else
				{
					cfgType=DIALUP_CFG;
				}
				break;
			}
		}//switch
	}

//-----------------------------------------------------------------------------
// Create config
//-----------------------------------------------------------------------------

//	COM_useSocket=false; // default to no socckets
/*	if				// Moved to DialUpConfig
		(
			( cfgType == DIALUP_TCPIP_CFG ) &&
			( TCONF.TCOPT7 & TC7_PERMN_PPP ) && // must have the option set
    		(
    			TM3_GSM == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ||
        		TM3_GSM_OLD == (TERM.TERMOpt3 & TM3_HWTYPE_MASK)
        	) && // must use a gsm modem
			( SETTLE_TRAN == tranMode || COMMON_TRAN == tranMode) // use sockets for txn only
		)
		{
			COM_useSocket=true;
		}
*/
// DEBUG
//	COM_useSocket=true;


	COM_hexLen=false;
	COM_HPOSFCERR=HTENOERROR;
	switch (cfgType)
	{
	case DIALUP_CFG:
		DialUpConfig();
		break;

    case SERIAL1_CFG:
		Serial1Config();
		break;

    case SERIAL2_CFG:
		Serial2Config();
		break;

    case ETHERNET_CFG:
		EthernetConfig();
		break;

    case POSLAN_CFG:
		PosLanConfig();
		break;

    case BT_CFG:
		BTConfig();
       // check for ACS communication
      if ( !(TERM.TERMOpt3 & TM3_HEMSTRANSP)  &&  
			((TERM.TERMRemoteModule & TMR_HWTYPE_MASK) != TMR_ETHERNET ) &&	
			((TERM.TERMRemoteModule & TMR_HWTYPE_MASK) != TMR_LAN ))
      {
      	DialUpConfig();
      }
		break;

    default:
		COM_HPOSFCERR=HTEINVAL;
		break;
	}

	if (COM_HPOSFCERR==HTEINVAL)
	{
		COM_useSocket=false;
		return;
	}

	//append predefined additional config
	if (COM_addConf != NULL)
	{
		for (HypCConfig* b=COM_confSet->begin();b<COM_confSet->end() ;b++)
		{
			CopyConfig(*b, *COM_addConf);
		}
	}
}

Bool IsNewPPPConnectionRequested( void )
{
    char *pTelNo;
	char *pPPPUsername;
	char *pModemATCmd;

	if ( TM3_ETHERNET == (TERM.TERMOpt3 & TM3_HWTYPE_MASK))
		return False;

    for (HypCConfig *c=COM_confSet->begin(); c<COM_confSet->end(); c++)
	{
		pTelNo = c->GetString(HC_TelNo);
		pPPPUsername = c->GetString(HC_PPPUsername);
		pModemATCmd = c->GetString(HC_ModemATCmd);

		if ( strcmp( pTelNo, pTelNoLastUsed ) ) // differs, hangup needed
			return True;
		if ( strcmp( pModemATCmd, pModemATCmdLastUsed ) )
			return True;
//		if ( strcmp( pPPPUsername, pPPPUsernameLastUsed ) )
//			return True;
	}

	return False;
}

void SavePPPSpecificParams( void )
{
    char *pTelNo;
	char *pPPPUsername;
	char *pModemATCmd;
	UBYTE i = 0;

	    for (HypCConfig *c=COM_confSet->begin(); c<COM_confSet->end(); c++, i++)
		{
			if( i ) // allow only one destination for this to work
			{
				// make it zero length to fail later comparison
				pTelNoLastUsed[0] = '\0';
				pModemATCmdLastUsed[0] = '\0';
				pPPPUsernameLastUsed[0] = '\0';
				break;
			}
			else
			{
				pTelNo = c->GetString(HC_TelNo);
				pPPPUsername = c->GetString(HC_PPPUsername);
				pModemATCmd = c->GetString(HC_ModemATCmd);

				strcpy( pTelNoLastUsed, pTelNo );
				strcpy( pPPPUsernameLastUsed, pPPPUsername );
				strcpy( pModemATCmdLastUsed, pModemATCmd );
			}
		}
}

//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void GetModemMode()
{
	if (TERM.TERMOpt1 & TM1_NACDIAL)
	{
		tranMode=NAC_DIAL_TRAN;
	}
	else if (tranKey==INITIALIZE)
	{
		modemMode=TERM.TERMModem;
		tranMode=INIT_TRAN;
	}
	else if (tranKey==ICEPAK_DOWNLOAD)
	{
		modemMode=IPTAB.IPMM;
		tranMode=ICE_PACK_TRAN;
	}
	else
	{
		//common transaction
		paq=FindAcquirer(aqID);
		//
		if ( paq==NULL )
		{
			// Acquirer not found
			modemMode=-1;
		}
        else
        {
            // Get the Transaction from the Request 
            HostEvent.TRNKEY = tranKey;

            // Locate the Transaction - will be in HostEvent 
            HostFindTran();
            
            if( HostEvent.TRNOPT4 & T4_LOGON )
            {
                tranMode =  LOGON_TRAN;
                modemMode = paq->AQLOGONPMODEM;
            }
            else if ( HostEvent.TRNOPT2 & T2_SETTLE )
            {
                //settlement
                tranMode=SETTLE_TRAN;
                modemMode=paq->AQSMODEM;
            }
            else
            {
                tranMode=COMMON_TRAN;
                modemMode=paq->conn.dial.aqModem;
            }


          }	// if ( paq==NULL )
	}

	// Ethernet(TERM.TERMRemoteModule & TMR_HWTYPE_MASK)
	if (((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET) ||
            (((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_BLUETOOTH) && !(TERM.TERMOpt3 & TM3_HEMSTRANSP) && ((TERM.TERMRemoteModule & TMR_HWTYPE_MASK) == TMR_ETHERNET)))
	{
		modemMode = 0xE8;	//Ethernet comm mode
	}
	
	// POS LAN
	if (((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_LAN) ||
            (((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_BLUETOOTH) && !(TERM.TERMOpt3 & TM3_HEMSTRANSP) && ((TERM.TERMRemoteModule & TMR_HWTYPE_MASK) == TMR_LAN)))
	{
		modemMode = 0xEE;	//POS LAN comm mode
	}
}


//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void Serial1Config()
{
	// COM1 (RS422) selected for communication (EFTOS RS0) (modem mode D0)
	HypCConfig cfg;
	SetSerialPortConfig(cfg);
	cfg.Set(HC_ModemModeNo, 0xD0);
	cfg.Set(HC_ConnAttempt, 1);
	COM_confSet->append(cfg);
}

//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void Serial2Config()
{
	// COM2 (RS232) selected for communication (EFTOS RS2) (modem mode D1)
	HypCConfig cfg;
	SetSerialPortConfig(cfg);
	cfg.Set(HC_ModemModeNo, 0xD1);
	cfg.Set(HC_ConnAttempt, 1);
	COM_confSet->append(cfg);
}

//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void DialUpConfig()
{
	int dest=GetNumDialDest();
	Bool bDialBackup;
	if (dest==0)
	{
		//report error, none of destination was defined
		COM_HPOSFCERR=HTEINVAL;
		return;
	}

	//set up primary destination
	HypCConfig cfg;
	if (dest == 1)
	  bDialBackup = SetDialConfig(cfg,True);
	else
	  bDialBackup = SetDialConfig(cfg,False);

	// Dial_Up_TCPIPCONfig
	if (IfDialTCPIP()) 
	{
	if	 
		(
			( TCONF.TCOPT7 & TC7_PERMN_PPP ) && // must have the option set
    		(
    			TM3_GSM == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ||
        		TM3_GSM_OLD == (TERM.TERMOpt3 & TM3_HWTYPE_MASK)
        	) && // must use a gsm modem
			( SETTLE_TRAN == tranMode || COMMON_TRAN == tranMode || LOGON_TRAN == tranMode) // use sockets for txn only
		)
		{
			COM_useSocket=true;
		}

		SetTCPIPConfig(cfg, bDialBackup);
		SetPPPConfig(cfg, bcd2bin(TERM.TERMTOut),bDialBackup); 

	// FIXME: is it safe to wipe out all the config?
		if ( IsStateSocketIdle( ) )
	    	COM_confSet->clear();	// get rid of last config, as we have a new one

	}

  
    // check for ISDN communication
    if ((HW_MODEMISDN == COM_moduleType) || (TMR_ISDN == (TERM.TERMRemoteModule & TMR_HWTYPE_MASK)) )
    {
        SetISDNConfig(cfg);
    }

    // check for Bluetooth ACS communication 
    if ( (!(TERM.TERMOpt3 & TM3_HEMSTRANSP)) && ( HW_BLUETOOTH == COM_moduleType ) )
	{
        // ACS enabled
		CopyConfig(*COM_confSet->begin(), cfg);
	}
	else
	{
		COM_confSet->append(cfg);

	}
}

//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void EthernetConfig()
{
	HypCConfig cfg;
	SetTCPIPConfig(cfg, False);
    cfg.Set(HC_ModemModeNo,modemMode);
	cfg.Set(HC_ConnAttempt, 1);
	COM_confSet->append(cfg);
}

//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void PosLanConfig()
{
	HypCConfig cfg;
	SetPosLanConfig(cfg);
	cfg.Set(HC_ConnAttempt, 1);
    cfg.Set(HC_ModemModeNo,modemMode);
    
	if (SIO_B4800 == RSTAB.RSSPEED)
	{
		cfg.Set(HC_LANBaudRate, HYP_SBR_4800);
	}
	else
    {
		cfg.Set(HC_LANBaudRate, HYP_SBR_19200);
	}
	cfg.Set(HC_CU, TERM.TERMCuAd);
	cfg.Set(HC_SNRMTimeout, 30);
    
    COM_confSet->append(cfg);
}

//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void BTConfig()
{
	HypCConfig cfg;
	SetBTConfig(cfg);
	COM_confSet->append(cfg);
}


//-----------------------------------------------------------------------------
//  GetNumDialDest   Function check for current Dest avaliable 
//  Parameters:       
//  Global Inputs:    
//  Returns:         Current settings (0 - None, 1 - First, 2 - Second)
//			For INIT_TRAN 1 - GSM/GPRS , 2 - Dial Backup
//  Notes:
//-----------------------------------------------------------------------------
int GetNumDialDest()
{
	int cw, da;
	unsigned char *pn;

	switch (tranMode)
	{
        
        case LOGON_TRAN:
		// Get secondary dial params for trans
		GetDialParam (cw, da, pn, &paq->logonTrans.primary);
		if ((pn[0]!=0xFF)&&(da!=0) && COM_firstCall)
			return(1);
		// Get secondary dial params for settlement
		GetDialParam (cw, da, pn, &paq->logonTrans.secondary);
		if ((pn[0]!=0xFF)&&(da!=0) )
			return(2);
		else
		 	return(0);
		break;

	case SETTLE_TRAN:
		// Get primary dial params for settlement
		GetDialParam (cw, da, pn, &paq->conn.dial.settle.primary);
		if ((pn[0]!=0xFF)&&(da!=0) && (True == COM_firstCall))
			return(1);
		// Get secondary dial params for settelment
		GetDialParam (cw, da, pn, &paq->conn.dial.settle.secondary);
		if ((pn[0]!=0xFF)&&(da!=0))
			return(2);
		else
			return(0);

	case COMMON_TRAN:
		// Get secondary dial params for trans
		GetDialParam (cw, da, pn, &paq->conn.dial.trans.primary);
		if ((pn[0]!=0xFF)&&(da!=0) && COM_firstCall)
			return(1);
		// Get secondary dial params for settlement
		GetDialParam (cw, da, pn, &paq->conn.dial.trans.secondary);
		if ((pn[0]!=0xFF)&&(da!=0) )
			return(2);
		else
		 	return(0);
		break;
	case INIT_TRAN:
		if (TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK))
		{
			if (True == COM_firstCall) 
			{
				if ((DBInitPrefferedService == 0) || (DBInitPrefferedService == 2))	
					return(1);
				else
					return(2);
			}
			else
			{
				if (DBInitPrefferedService == 0) 	
					return(2);
				else if (DBInitPrefferedService == 1)
					return(1);
				else
					return(0);
			}
		}
		else 
		{
			if (COM_firstCall)
				return(1);
			else
				return(0);
		}


	default:
		return(1);
	}
}

//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void GetDialParam (int &cw, int &da, unsigned char* &pn, PHONE_STRUCT* data)
{
	//connection wait time
	cw = bcd2bin( data->timeout );

	// Number of dial attempts
	da = data->trys;

	// Telephone number (BCD)
	pn=data->phoneNum;
}

//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
Bool SetDialConfig(HypCConfig &cfg, bool primary)
{
	//connection wait time
	int cw;
	//dial attempts
	int da;
	//telephone number (BCD)
	unsigned char *pn;
	bool bDialBackup;
	UBYTE cPPPMode;

	switch (tranMode)
	{
        case LOGON_TRAN:
		if (primary)
		{
			// Get primary dial params for trans
			GetDialParam (cw, da, pn, &paq->logonTrans.primary);
			bDialBackup = (paq->AQOPT5) & AQ5_LOGON_DBPHONE1;
			modemMode=paq->AQLOGONPMODEM;
                        cPPPMode = paq->AQLOGONPPPMODE1;
		}
		else
		{
			// Get secondary dial params for tarns
			GetDialParam (cw, da, pn, &paq->logonTrans.secondary);
			bDialBackup = (paq->AQOPT5) & AQ5_LOGON_DBPHONE2;
			modemMode=paq->AQLOGONSMODEM;
                        cPPPMode = paq->AQLOGONPPPMODE2;
		}
		break;

	case SETTLE_TRAN:
		if (primary)
		{
			// Get primary dial params for settlement
			GetDialParam (cw, da, pn, &paq->conn.dial.settle.primary);
			bDialBackup = (paq->AQOPT5) & AQ5_SETTLE_DBPHONE1;
			modemMode=paq->AQSMODEM;
                        cPPPMode = paq->AQSPPPMODE1;
		}
		else
		{
			// Get secondary dial params for settlement
			GetDialParam (cw, da, pn, &paq->conn.dial.settle.secondary);
			bDialBackup = (paq->AQOPT5) & AQ5_SETTLE_DBPHONE2;
			modemMode=paq->AQSMODEM2;
                        cPPPMode = paq->AQSPPPMODE2;
		}
		break;

	case COMMON_TRAN:
		if (primary)
		{
			// Get primary dial params for trans
			GetDialParam (cw, da, pn, &paq->conn.dial.trans.primary);
			bDialBackup = (paq->AQOPT5) & AQ5_DBPHONE1;
			modemMode=paq->conn.dial.aqModem;
                        cPPPMode = paq->AQPPPMODE1;
		}
		else
		{
			// Get secondary dial params for tarns
			GetDialParam (cw, da, pn, &paq->conn.dial.trans.secondary);
			bDialBackup = (paq->AQOPT5) & AQ5_DBPHONE2;
			modemMode=paq->AQMODEM2;
                        cPPPMode = paq->AQPPPMODE2;
		}
		break;
	default:        
	case INIT_TRAN:
		if (primary )
		{
                        if ( flagTag42 & FLG_INIT ) 
                          pn=(unsigned char*)TempInitNo;// set tel number
                        else
                          pn=(unsigned char*)TERM.TERMInitTel;// set tel number

			cw= CvtBin(TERM.TERMCon);           // init connection time
			da= TERM.TERMPTRY[0];               // number of dial attempts
			modemMode=TERM.TERMModem;
			bDialBackup = False;
			cPPPMode = PPPMode;
		}
		else
		{
			pn=(unsigned char*)DBInitTel;// set tel number
			cw= CvtBin(DBCon);           // init connection time
			da= TERM.TERMPTRY[1];               // number of dial attempts
			modemMode=DBModem;
			bDialBackup = True;			
			cPPPMode = DBPPPMode;
		}
		break;

	case ICE_PACK_TRAN:
		pn=IPTAB.IPTEL;                     // set tel number
		cw= CvtBin(TERM.TERMCon);           // init connection time
		da= TERM.TERMPTRY[0];               // number of dial attempts
		break;
	}
	
	if (bDialBackup && (TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ))
	{
		DBSwitchToAnalog();
		if (COM_DBPreInitFlag)
		{
			COM_DBPreInitFlag = False;
			cfg.Set(HC_ModemPreinitted, 1);
		}
	}
	else
	{
		if (COM_GSMLowSignalLevel && (TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ))
			event->Send(CONNECT_ERROR);
		bDialBackup = False;
		if (COM_PreInitFlag)
		{
			COM_PreInitFlag = False;
			cfg.Set(HC_ModemPreinitted, 1);
		}
	}

	cfg.Set(HC_ConnAttempt,da);
	cfg.Set(HC_Timeout,cw*1000);

	// create dial string from pn and TERM.TERMPabx
	char phoneStr[25];
	hex2ascii(phoneStr, pn, 12 );
	phoneStr[24]='\0'; //add null terminator
    //find phone number terminator  ('F')
    char *p=(char *)memchr(phoneStr,'F', 24);
    if (p!=NULL)
    {
      *p='\0';
    }

	bool insertPABX=ConvertNum(phoneStr);
	HypCBuffer str (64,64);
	str.append(phoneStr);

	if( insertPABX )
	{
		hex2ascii( phoneStr, (unsigned char *)TERM.TERMPabx, 4 );
		phoneStr[8]='\0'; //add null terminator
		if (ConvertPrefix(phoneStr))
		{
			// insert wait dial tone
			str.insert(0,'W');
		}
		str.insert(0,phoneStr, strlen(phoneStr));
	}
	cfg.Set(HC_TelNo, str.c_str());

	if (TERM.TERMDMode != 0)
	{
		cfg.Set(HC_PulseDial, 1); /* set mode to pulse */
	}

	cfg.Set(HC_CU, TERM.TERMCuAd);				// cu address
	cfg.Set(HC_PulseRatio,TERM.TERMPDRatio);	// pulse dial ratio  
	cfg.Set(HC_DTMFSpeed,TERM.TERMDTMFS);		// dtmf speed
	if ( 0 != TERM.TERMDialTw )
	{
		cfg.Set(HC_ToneWaitTime,bcd2bin( TERM.TERMDialTw));// dial tone wait
	}

	if ( 0 ==  TERM.TERMHookW )
	{
		cfg.Set(HC_BlindDial,1);
	}
	else
	{
		cfg.Set(HC_LineInUse,1);
	}

	if ( 0 != TERM.TERMDiscW )
	{
		cfg.Set(HC_MinDiscTime,bcd2bin(TERM.TERMDiscW));	// Min Disconnect Time
	}
	if ( 0 != TERM.TERMPauseW )
	{
		cfg.Set(HC_PauseTime,bcd2bin( TERM.TERMPauseW ));
	}

	//cfg.Set(TransmitLevel,10); -default
	cfg.Set(HC_ModemModeNo, modemMode);

	//additional at command for the modem
	if (bDialBackup)
	{
		if(DBModemATCommand[0]!=0)
		{
			cfg.Set(HC_ModemATCmd,(char *)DBModemATCommand);
		}
	}
	else
	{
		if(ModemATCommand[0]!=0)
		{
			cfg.Set(HC_ModemATCmd,(char *)ModemATCommand);
		}
	}

	//set pad logon script
	SetPadLogon (cfg);

	if (IfHexLength())
	{
		if ((modemMode == 0xD2) || (modemMode == 0xD3) ||
			(modemMode == 0x45) || (modemMode == 0x66) ||
			(modemMode == 0x52) || (modemMode == 0x90))
		{
			cfg.Set(HC_SerialDriverMode,HYP_POSTRANSPAD); 
			cfg.Set(HC_ReadIntervalTimeout,2000);
			COM_hexLen=true;
		}
	}
	else
	{
		switch(modemMode)
		{
		case 0xD2:
		case 0xD3:
		case 0x45:
		case 0x66:
			cfg.Set(HC_ReadIntervalTimeout,1200);
			break;

		case 0x52:
			cfg.Set(HC_ReadIntervalTimeout,600);
			break;

		case 0x90:
			cfg.Set(HC_ReadIntervalTimeout,300);
			break;

		default:
			break;
		}
	}
	cfg.Set(HC_PPPMode, 0x0f & cPPPMode);
    // Set LCP timeout as high nibble of PPPMode (or 10s if not defined)
	if ( (cPPPMode>>4) == 0 )
	{
		if (	TM3_GSM == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ||
        		TM3_GSM_OLD == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ||
        		(( False == bDialBackup) && (TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) )))
				cfg.Set(HC_PPPLCPTimeout, 2);
		else
				cfg.Set(HC_PPPLCPTimeout, 6);
	}
	else
	{
		cfg.Set(HC_PPPLCPTimeout, (cPPPMode>>4));
	}
	return (bDialBackup);

}

//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void SetPadLogon (HypCConfig &cfg)
{
	if (IfSyncMode())
	{
		// Do not use Network Logon for SDLC modem modes
		return;
	}

    // Determine, where from get the script for Network Logon
	PDNSTRUCT *pPDNPtr;
	switch ( tranMode )
	{
        // Settlement or Acquirer transaction - get from PDN table
	case SETTLE_TRAN:
	case COMMON_TRAN:
        case LOGON_TRAN:
        // Try to find PDN table
        pPDNPtr = FindPDN( aqID );
        // Do nothing if no PDN table found
        if ((NULL==pPDNPtr)||(pPDNPtr->PDNPTLOGON[0]==0))
		{
            return;
		}
        else
        {
            cfg.Set(HC_ConnectScript,(char *)pPDNPtr->PDNPTLOGON);
        }

    default: 
		if (PADLOGON[0]!=0)
		{
			// Initialize - get from Application Manager level
			cfg.Set(HC_ConnectScript,(char *)PADLOGON);
		}
    }
}


//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void SetSerialPortConfig (HypCConfig &cfg)
{
	int baudrate, databits, parity, stopbits;

	// Set Data Bits, eftos supports 7 or 8 only
	switch (RSTAB.RSDPS & SIO_BIT_MASK)
	{
	case SIO_7BIT:
		databits = 7;
		break;

	default:
	case SIO_8BIT:
		databits = 8;
		break;
	}

	switch (RSTAB.RSSPEED)
	{
	case SIO_B300:
		baudrate=HYP_SBR_300;
		break;

	case SIO_B1200:
		baudrate=HYP_SBR_1200;
		break;

	case SIO_B2400:
		baudrate=HYP_SBR_2400;
		break;

	case SIO_B4800:
		baudrate=HYP_SBR_4800;
		break;

	case SIO_B9600:
		baudrate=HYP_SBR_9600;
		break;

	default:
	case SIO_B19200:
		baudrate=HYP_SBR_19200;
		break;

	case SIO_B38400:
		baudrate=HYP_SBR_38400;
		break;

	case SIO_B57600:
		baudrate=HYP_SBR_57600;
		break;

	case SIO_B115200:
		baudrate=HYP_SBR_115200;
		break;
	}

	// Set Parity
	switch (RSTAB.RSDPS & SIO_PMASK)
	{
	default:
	case SIO_PNONE:
		parity = HYP_PARITY_NONE;
		break;

	case SIO_PEVEN:
		parity = HYP_PARITY_EVEN;
		break;

	case SIO_PODD:
		parity = HYP_PARITY_ODD;
		break;
	}

	// Set Stop Bits
	switch (RSTAB.RSDPS & SIO_STOP_MASK)
	{
	default:
	case SIO_1STOP:
		stopbits = HYP_STOP_1;
		break;

	case SIO_2STOP:
		stopbits = HYP_STOP_2;
		break;
	}

	cfg.Set(HC_BaudRate,baudrate);
	cfg.Set(HC_DataBits,databits);
	cfg.Set(HC_LineParity,parity);
	cfg.Set(HC_StopBits,stopbits);

	if ( RSTAB.RSOPT & RSOPT_FC )
	{
		cfg.Set(HC_FlowControl,HYP_HARDWARE_FC); 
	}else
	{
		cfg.Set(HC_FlowControl,HYP_NONE_FC); 
	}
}



//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void SetPosLanConfig(HypCConfig &cfg)
{
	//fixme: check local LAN config. do we need to add?
	unsigned char buf_hex[5];
    // Add TPDU id
	buf_hex[0]=TPDUID_APP;
	buf_hex[3]=0;
	buf_hex[4]=0;

    // Add Destination NII
	struct ipcontab* pIPC;
	switch (tranMode)
	{
        case LOGON_TRAN:
                pIPC = FindIPCById(paq->AQ_LOGON_P_IPTAB);
		buf_hex[1]=pIPC->nii[0];
		buf_hex[2]=pIPC->nii[1];
		break;

	case SETTLE_TRAN:
		pIPC = FindIPCById(paq->AQ_SETTLE_P_IPTAB);
		buf_hex[1]=pIPC->nii[0];
		buf_hex[2]=pIPC->nii[1];
		break;

	case COMMON_TRAN:
		pIPC = FindIPCById(paq->AQ_TRANS_P_IPTAB);
		buf_hex[1]=pIPC->nii[0];
		buf_hex[2]=pIPC->nii[1];
		break;

	default:
	case INIT_TRAN:
 		buf_hex[1]=TERM.TERMNII[0];
		buf_hex[2]=TERM.TERMNII[1];
		break;
	}

	char buf_ascii[11];
	hex2ascii(buf_ascii, buf_hex,5);
	buf_ascii[10]=0;

	cfg.Set(HC_LANTPDU1, buf_ascii);
	cfg.Set(HC_LANConnectionType1, 0);
	cfg.Set(HC_LANConnectTime1, 30000);
	cfg.Set(HC_LANHostTimeout1, 600);
}


#ifdef MAKE_SSL
//-----------------------------------------------------------------------------
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void SetSSLConfig( HypCConfig &cfg, Bool use_ssl )
{
#ifdef MAKE_CERTIFICATE
	struct certificate_rec *pCert;
#endif // MAKE_CERTIFICATE

	// Let know for HSDK the address of SSL authentication function
	if ( use_ssl )
	{
		cfg.Set( HC_SslEnabled, 1 );
		cfg.Set( HC_SslCheckCert, (int)SSL_authentication );

#ifdef MAKE_CERTIFICATE
		// Find out client' certificate pointer
		if (NULL != (pCert = GetCertificate( 0 )))
		{
			// client certificate exist - init mutual SSL auth
			cfg.Set( HC_SslMutualEnabled, 1 );
			cfg.Set( HC_SslClientCertPtr, (int)(pCert->CERT) );
			cfg.Set( HC_SslClientCertLen, pCert->CERTLEN);
			cfg.Set( HC_SslPrivateKeyPtr, (int)(pCert->KEY) );
			cfg.Set( HC_SslPrivateKeyLen, pCert->KEYMODULE );
			cfg.Set( HC_SslPrivateKeyFmt, pCert->KEYFORMAT );
			cfg.Set( HC_SslMasterKeyIdx, pCert->PKMKID );
		}
		else
#endif // MAKE_CERTIFICATE
		{
			// Client certificate does not exis
			// disable SSL mutual authentication
			cfg.Set( HC_SslMutualEnabled, 0 );
		}
	}
	else
	{
		cfg.Set( HC_SslEnabled, 0 );
		cfg.Set( HC_SslCheckCert, 0 );
	}
}
#endif	// MAKE_SSL

//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void SetTCPIPConfig(HypCConfig &cfg, Bool bDialBackup)
{
	unsigned int hemsParam = 0;
	struct ipcontab* pIPC;

	if ( (TCONF.TCOPT7 & TC7_RETAIN_LINK) && (bDialBackup == False))
	{
			COM_useSocket = true;
	}

    cfg.Set(HC_LocalIpAddr, TERM.TERMIPAddress);
	cfg.Set(HC_LocalNetMask, TERM.TERMSubnetIP);
	cfg.Set(HC_DefaultGwAddr, TERM.TERMGatewayIP);


	hemsParam = ((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET) ?
	               (TERM.TERMOpt3 & TM3_HEMSTRANSP) ? 1 : 2 :
	               (((TERM.TERMOpt3 & TM3_HWTYPE_MASK) != TM3_BLUETOOTH) || (TERM.TERMOpt3 & TM3_HEMSTRANSP)) ? 3 : 4;
	hemsParam |= (TERM.TERMPDMode << 8) & 0xff00;
	cfg.Set(HC_HemsParam,hemsParam); 

	switch (tranMode)
	{
        case LOGON_TRAN:
		pIPC = FindIPCById(paq->AQ_LOGON_P_IPTAB);
        if((pIPC!=NULL)&&(pIPC->ipaddr[0]!=0)&&(pIPC->ipaddr[0]!=0x20)&&(pIPC->ipaddr[0]!=0xFF))
		{
#ifdef MAKE_SSL
			// Configure SSL if required
			SetSSLConfig( cfg, paq->AQOPT1 & AQ1_SSLTRAN );
#endif	// MAKE_SSL
			if (!COM_useSocket)
			{
				cfg.Set(HC_Ip1, aton((char *)pIPC->ipaddr));
				cfg.Set(HC_Port1, atoi((char *)pIPC->port));
			}
			else
			{
				cfg.Set(HC_Ip1, 0x0000);
				Dest.sin_addr.s_addr = inet_addr((char *)pIPC->ipaddr);
				Dest.sin_port = htons(atoi((char *)pIPC->port));
			}
			cfg.Set(HC_IpTimeout1, paq->AQ_LOGON_P_CON);
			
			pIPC = FindIPCById(paq->AQ_LOGON_S_IPTAB);
            if((pIPC!=NULL)&&(pIPC->ipaddr[0]!=0)&&(pIPC->ipaddr[0]!=0x20)&&(pIPC->ipaddr[0]!=0xFF))
			{
				if (!COM_useSocket)
				{
					cfg.Set(HC_Ip2, aton((char *)pIPC->ipaddr));
					cfg.Set(HC_Port2, atoi((char *)pIPC->port));
				}
				else
				{
					Dest2.sin_addr.s_addr = inet_addr((char *)pIPC->ipaddr);
					Dest2.sin_port = htons(atoi((char *)pIPC->port));
				}
				cfg.Set(HC_IpTimeout2, paq->AQ_LOGON_S_CON);

			}
		}
		break;
                
	case SETTLE_TRAN:
		
		pIPC = FindIPCById(paq->AQ_SETTLE_P_IPTAB);
		if((pIPC!=NULL)&&(pIPC->ipaddr[0]!=0)&&(pIPC->ipaddr[0]!=0x20)&&(pIPC->ipaddr[0]!=0xFF))
		{
#ifdef MAKE_SSL
			// Configure SSL if required
			SetSSLConfig( cfg, paq->AQOPT1 & AQ1_SSLSETTLE );
#endif	// MAKE_SSL
			if (!COM_useSocket)
			{
				cfg.Set(HC_Ip1, aton((char *)pIPC->ipaddr));
				cfg.Set(HC_Port1, atoi((char *)pIPC->port));
			}
			else
			{
				cfg.Set(HC_Ip1, 0x0000);
				Dest.sin_addr.s_addr = inet_addr((char *)pIPC->ipaddr);
				Dest.sin_port = htons(atoi((char *)pIPC->port));
			}
			cfg.Set(HC_IpTimeout1, paq->AQ_SETTLE_P_CON);


			
			pIPC = FindIPCById(paq->AQ_SETTLE_S_IPTAB);
            if((pIPC!=NULL)&&(pIPC->ipaddr[0]!=0)&&(pIPC->ipaddr[0]!=0x20)&&(pIPC->ipaddr[0]!=0xFF))
			{
				if (!COM_useSocket)
				{
					cfg.Set(HC_Ip2, aton((char *)pIPC->ipaddr));
					cfg.Set(HC_Port2, atoi((char *)pIPC->port));
				}
				else
				{
					Dest2.sin_addr.s_addr = inet_addr((char *)pIPC->ipaddr);
					Dest2.sin_port = htons(atoi((char *)pIPC->port));
				}
				cfg.Set(HC_IpTimeout2, paq->AQ_SETTLE_S_CON);

			}
		}
		break;
	case COMMON_TRAN:
		pIPC = FindIPCById(paq->AQ_TRANS_P_IPTAB);
        if((pIPC!=NULL)&&(pIPC->ipaddr[0]!=0)&&(pIPC->ipaddr[0]!=0x20)&&(pIPC->ipaddr[0]!=0xFF))
		{
#ifdef MAKE_SSL
			// Configure SSL if required
			SetSSLConfig( cfg, paq->AQOPT1 & AQ1_SSLTRAN );
#endif	// MAKE_SSL
			if (!COM_useSocket)
			{
				cfg.Set(HC_Ip1, aton((char *)pIPC->ipaddr));
				cfg.Set(HC_Port1, atoi((char *)pIPC->port));
			}
			else
			{
				cfg.Set(HC_Ip1, 0x0000);
				Dest.sin_addr.s_addr = inet_addr((char *)pIPC->ipaddr);
				Dest.sin_port = htons(atoi((char *)pIPC->port));
			}
			cfg.Set(HC_IpTimeout1, paq->AQ_TRANS_P_CON);
			
			pIPC = FindIPCById(paq->AQ_TRANS_S_IPTAB);
            if((pIPC!=NULL)&&(pIPC->ipaddr[0]!=0)&&(pIPC->ipaddr[0]!=0x20)&&(pIPC->ipaddr[0]!=0xFF))
			{
				if (!COM_useSocket)
				{
					cfg.Set(HC_Ip2, aton((char *)pIPC->ipaddr));
					cfg.Set(HC_Port2, atoi((char *)pIPC->port));
				}
				else
				{
					Dest2.sin_addr.s_addr = inet_addr((char *)pIPC->ipaddr);
					Dest2.sin_port = htons(atoi((char *)pIPC->port));
				}
				cfg.Set(HC_IpTimeout2, paq->AQ_TRANS_S_CON);

			}
		}
		break;
	default:
	case INIT_TRAN:
		if ( TERM.TERMInitIP == 0x0000)
			break;
#ifdef MAKE_SSL
		// Configure SSL if required
		SetSSLConfig( cfg, TERM.TERMOpt3 & TM3_SSL );
#endif	// MAKE_SSL
		if (!COM_useSocket)
		{
			cfg.Set(HC_Ip1, TERM.TERMInitIP);
			cfg.Set(HC_Port1, TERM.TERMInitPort);
		}
		else
		{
			cfg.Set(HC_Ip1, 0x0000);
			Dest.sin_addr.s_addr = htonl(TERM.TERMInitIP);
			Dest.sin_port = htons(TERM.TERMInitPort);
		}
		cfg.Set(HC_IpTimeout1, bcd2bin(TERM.TERMCon));
		break;
	}

	if (!cfg.IsExist(HC_Ip1))
	{
		COM_HPOSFCERR=HTEINVAL;
		return;
	}
	if (IfHexLength())
	{
		cfg.Set(HC_UseHexLength, 1);
		COM_hexLen=true;
	}
    else
    {
 		cfg.Set(HC_UseHexLength, 0);
 		COM_hexLen=false;
    }
}


//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void SetPPPConfig(HypCConfig &cfg, unsigned int to,Bool bDialBackup)
{
	//to - timeout in seconds
	if (False ==  cfg.IsExist(HC_PPPMode))
	{
		cfg.Set(HC_PPPMode, 0x0f & PPPMode);
	    // Set LCP timeout as high nibble of PPPMode (or 10s if not defined)
		if ( (PPPMode>>4) == 0 )
		{
			if (	TM3_GSM == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ||
					TM3_GSM_OLD == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ||
					(( False == bDialBackup) && (TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) )))
					cfg.Set(HC_PPPLCPTimeout, 2);
			else
					cfg.Set(HC_PPPLCPTimeout, 6);
		}
		else
		{
			cfg.Set(HC_PPPLCPTimeout, (PPPMode>>4));
		}
	}


	if (bDialBackup)
	{
		cfg.Set(HC_PPPUsername, (const char *)DBLogonUsername);
		cfg.Set(HC_PPPPassword, (const char *)DBLogonPassword);
		cfg.Set(HC_PPPConnectionTimeout, to);
		cfg.Set(HC_SerialDriverMode,HYP_PPP); 
	}
	else
	{
		cfg.Set(HC_PPPUsername, (const char *)LogonUsername);
		cfg.Set(HC_PPPPassword, (const char *)LogonPassword);
		cfg.Set(HC_PPPConnectionTimeout, to);
		cfg.Set(HC_SerialDriverMode,HYP_PPP); 
	}
}


//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void SetBTConfig(HypCConfig &cfg)
{
	char ascii_str[13];
	HypCConfig tmp;
    int br;

    // Set Bluetooth Hub address
    hex2ascii(ascii_str, RSTAB.HUBADDR, 6);
	ascii_str[12]=0;
	cfg.Set(HC_BTAddr, ascii_str);

	// Set Bluetooth UUID
	hex2ascii(ascii_str, RSTAB.UUID, 2);	
	ascii_str[4]=0;
	cfg.Set(HC_BTUUID, ascii_str );

	// Set Bluetooth encryption
	cfg.Set(HC_BTEncryption, 1 );

    // Set Bluetooth authentication
	cfg.Set(HC_BTAuthentication, 1 );

	//FIXME: Set pdn logon for BT???
//    SetPadLogon (cfg);

	//Set serial speed
	SetSerialPortConfig(tmp);
	br=tmp.Get(HC_BaudRate);
	cfg.Set(HC_BaudRate, br);

    // Set ACS parameters
    if ( !(TM3_HEMSTRANSP & TERM.TERMOpt3) )
    {
        cfg.Set(HC_ConnTimeout, CvtBin(TERM.TERMCon)); // Connection timeout

        // ACS link type
	if (IfDialTCPIP())
        {
            SetTCPIPConfig(cfg, False);
            SetPPPConfig(cfg, bcd2bin(TERM.TERMTOut), False);
            cfg.Set(HC_ACSLinkType, HEMS_LINK_MC);
        }
	else if (modemMode == 0xe8)
	{
            SetTCPIPConfig(cfg, False);
            cfg.Set(HC_ACSLinkType, HEMS_LINK_MC);
	}
        else if ((TM3_HEXLENGTH & TERM.TERMOpt3) || (IfNII8087()))
        {
            cfg.Set(HC_ACSLinkType, HEMS_LINK_ST); // ST_Link
        }
        else
        {
            cfg.Set(HC_ACSLinkType, HEMS_LINK_MT); // MT_Link
        }

        cfg.Set(HC_TerminalTimeout, 5 + bcd2bin(TERM.TERMTOut)); // Terminal timeout
        cfg.Set(HC_RetentionTimeout, 15); // Retention timeout
    }
    // Bluetooth transparent with PC
    else if ((TERM.TERMRemoteModule & TMR_HWTYPE_MASK) == TMR_ETHERNET ) 
    {
        // Set PPP config for modem modes E0..EF
        if ( 0xE0 == ( modemMode & 0xF0 ))
        {
            SetPPPConfig(cfg, bcd2bin(TERM.TERMTOut),False);
        }

        SetTCPIPConfig(cfg, False);
        cfg.Set(HC_ConnTimeout, CvtBin(TERM.TERMCon)); // Connection timeout
    }

    // Set connect attempts
    cfg.Set(HC_ConnAttempt, 1);

    // Set modem mode
    cfg.Set(HC_ModemModeNo,modemMode);
}


//-----------------------------------------------------------------------------
//  SetISDNConfig   set special ISDN module configuration
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
void SetISDNConfig(HypCConfig &cfg)
{
    UBYTE ISDNproto;
    PDNSTRUCT *pPDNPtr;
    char *pStr;
    char *pTelNo;

    // Acquirer transaction
    if ( (tranMode==SETTLE_TRAN) || (tranMode==COMMON_TRAN) || ( tranMode == LOGON_TRAN ) )
    {
        ISDNproto = paq->AQDMODE;
    }
    // Init transaction
    else if (tranMode==INIT_TRAN)
    {
        ISDNproto = TERM.TERMDialDev;
    }
    else return;    

    // Determine ISDN protocol:
    switch (ISDNproto)
    {
        case TMDM_V110_9600:
            cfg.Set(HC_ISDNProto, ISDN_V110);
            cfg.Set(HC_ISDN_V110_BR, HYP_SBR_9600);
            break;

        case TMDM_V110_19200:
            cfg.Set(HC_ISDNProto, ISDN_V110);
            cfg.Set(HC_ISDN_V110_BR, HYP_SBR_19200);
            break;

        case TMDM_V120:
            cfg.Set(HC_ISDNProto, ISDN_V120);
            cfg.Set(HC_ModemModeNo,0x90);   // overwrite Modem Mode
            break;

        case TMDM_X75:
            cfg.Set(HC_ISDNProto, ISDN_X75);
            cfg.Set(HC_ModemModeNo,0x90);   // overwrite Modem Mode
            break;

        case TMDM_X25:
            cfg.Set(HC_ISDNProto, ISDN_X25);

            // Clear PAD logon script if was configured before
            cfg.Remove(HC_ConnectScript);
                 
            // Get the pointer to the string need to be append
            if (tranMode==INIT_TRAN)
            {
                // Init transaction
                pStr = (char*) PADLOGON;
            }
            else
            {
                // Acquirer transaction
                pPDNPtr = FindPDN( aqID );
                
                //If no PDN table found - exit
                if ( NULL == pPDNPtr ) break;
                pStr = (char*)(pPDNPtr->PDNPTLOGON);
            }

            // Do nothing if not started by "X"
            if ( *pStr=='X' )
            {
                // Get predefined Tel No
                pTelNo = cfg.GetString( HC_TelNo );

                // Allocate temporary buffer for new
                void *pNewTelNo = malloc(PDNSZE+100);

                // Copy it to new 
                strcpy((char*)pNewTelNo, (const char*)pTelNo);

                // Append PDN
                strcat((char*)pNewTelNo, (const char*)pStr);

                // Set new Tel No
                cfg.Set(HC_TelNo, (char*)pNewTelNo);

                // Free temporary buffer
                free (pNewTelNo);
            }
            break;

        default: // do nothing (assume modem was manually configured)
            break;
    }
}


	
//-----------------------------------------------------------------------------
//
//  Parameters:       
//  Global Inputs:    
//  Returns:
//  Notes:
//
// phoneStr - ascii null terminated string with hex digits.
//return true if need to insert PABX prefix
//
// 0-9 dial digits as specified
// A - select dial mode as DTMF
// B - Dial Tone Pause
// C - Select Pulse
// D - skip PABX
// E - extended DTMF
//     The next digit is
//     A - *
//     B - #
//     C - A
//     D - B
//     E - C
//     F - D
// F - terminator
//-----------------------------------------------------------------------------

bool ConvertNum(char *phoneStr)
{
	int i;
	bool insertPABX=true;
	char *pStrPut = phoneStr;

	int len=strlen (phoneStr);

	for( i=0; i < len; i++)
	{
		switch( phoneStr[i] )
		{
			case 'A': // DTMF
				*pStrPut++ = 'T';
				continue;

			case 'B': // Wait dial tone
				*pStrPut++ = 'W';
				continue;

			case 'C': // Pulse
				*pStrPut++ = 'P';
				continue;

			case 'D': // skip PABX
				if ((i+1==len)||(phoneStr[i+1]=='F'))
				{
					//'D' is a last char
					insertPABX = false;
				}
				else
				{
					//pause
					*pStrPut++ = ',';
				}
				continue;

			case 'E': // extended DTMF
			{

				char ch;
				// get next character and translate
				switch( phoneStr[++i] )
				{
					case '0':
						ch = '+';
						break;

					case 'A':
						ch = '*';
						break;

					case 'B':
						ch = '#';
						break;

					case 'C':
						ch = 'A';
						break;

					case 'D':
						ch = 'B';
						break;

					case 'E':
						ch = 'C';
						break;

					case 'F':
						ch = 'D';
						break;

					case 0:
						*pStrPut=0; //null terminate
						return insertPABX;

					default:
						//skip 
						continue;
				}

				*pStrPut++ = ch;
				continue;
			}

			case 'F':
				break;

			default:           // 0-9
				*pStrPut++ = phoneStr[i];
				continue;
		}
	}
	*pStrPut = 0; // null terminate
	return insertPABX;
}


//-----------------------------------------------------------------------------
//  ConvertPrefix
//  Parameters:       
//  Global Inputs:    
//  Returns:
//            1 if 'W' is requires
//            0 if 'W' must be skipped
//  Notes:
//-----------------------------------------------------------------------------
bool ConvertPrefix(char *phoneStr)
{
	bool insertW=true;
	if ((phoneStr[0]=='F') || (strlen(phoneStr)==0))
	{
		phoneStr[0]=0;
		return false;
	}

	if (phoneStr[0]=='D')
	{
		//first char is D, skip 'W'
		insertW=false;
	}

	if (ConvertNum(phoneStr)==false)
	{
		//last char is 'D' -> replace to ','
		int l=strlen (phoneStr);
		phoneStr[l]=',';
		phoneStr[l+1]=0;
	};
	return insertW;
}


//-----------------------------------------------------------------------------
//  FindIPCById		Finds the IP Connect Entry given the Id
//  Parameters:		IP Connect Number (Id)
//  Global Inputs:
//  Returns:		Pointer to IP Connect Entry or NULL if not found
//  Notes:
//-----------------------------------------------------------------------------
struct ipcontab *FindIPCById( UBYTE id )
{
	UBYTE i;

	for( i = 0; i < IPCMAX; i++ )
	{
		if( IPCTAB[i].id == id )
		{
			return &IPCTAB[i];
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//  inet_addr		Converts an IP address in a string to a UDWORD
//  Parameters:
//  Global Inputs:
//  Returns:
//  Notes:
//-----------------------------------------------------------------------------
/*UDWORD inet_addr(const char *s)
{
//	int i;
	union
	{
		UDWORD l;
		UBYTE b[4];
	} ip;
	
	const char *text = s;

	while(1) 
	{
		ip.b[3] = (UBYTE)atoi( text );
		if(!(text = strchr( text, '.' ))) break;
		text++;
		ip.b[2] = (UBYTE)atoi( text );
		if(!(text = strchr( text, '.' ))) break;
		text++;
		ip.b[1] = (UBYTE)atoi( text );
		if(!(text = strchr( text, '.' ))) break;
		text++;
		ip.b[0] = (UBYTE)atoi( text );

		return ip.l;
	}
	return 0;
}
*/

//-----------------------------------------------------------------------------
//  IfSyncMode        Check if current mode is synchronous
//  Parameters:       UBYTE Modem Mode
//  Global Inputs:    None
//  Returns:          true - Yes, this is synchronous modem mode
//                    false - No, this is asynchronous modem mode
//  Notes:
//-----------------------------------------------------------------------------
bool IfSyncMode( )
{
    switch ( modemMode )
    {
        // The list of SDLC Modem Modes for Optimum T2100
		case 0x00:
		case 0x02:
		case 0x06:
		case 0x08:
		case 0x72:
		case 0x73:
		case 0x75:
		case 0x76:
            return true;

        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
//  IfDialTCPIP       Check if current mode is TCP/IP
//  Parameters:       UBYTE Modem Mode
//  Global Inputs:    None
//  Returns:          true - Yes, this is TCP/IP modem mode
//                    false - No, this is TCP/IP modem mode
//  Notes:
//-----------------------------------------------------------------------------
bool IfDialTCPIP( )
{
    if ((modemMode>=0xE0)&&(modemMode <= 0xE7))
    {
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//  IfHexLength       Check if hex length required in header
//  Parameters:       UBYTE Modem Mode
//  Global Inputs:    None
//  Returns:          true - Yes, hex length required 
//                    false - No, hex length does not required
//  Notes:
//-----------------------------------------------------------------------------
bool IfHexLength()
{
	struct ipcontab* pIPC=NULL;
	bool hl=false;
	switch (tranMode)
	{
        case LOGON_TRAN:
                // Find pointer to IP tab entry
                pIPC = FindIPCById(paq->AQ_LOGON_P_IPTAB);
		break;

	case SETTLE_TRAN:
		// Find pointer to IP tab entry for settlement
		//FIXME check - Is hexlen bit placed IP table??
		pIPC = FindIPCById( paq->AQ_SETTLE_P_IPTAB );
		break;

	case COMMON_TRAN:
        // Find pointer to IP tab entry
        pIPC = FindIPCById(paq->AQ_TRANS_P_IPTAB);
		break;

	default:
		if (TERM.TERMOpt3 & TM3_HEXLENGTH)
		{
			hl=true;
		}
		break;
	}

	// Check if Hex Len required
	if ((pIPC!=NULL)&&(pIPC->opt1&IPC_LENGTH))
	{
		hl=true;
	}

	if (hl)
    { 
        // The list of modem modes required hex length
        switch (modemMode)
        {
            case 0x45:
            case 0x52:
            case 0x65:
            case 0x66:
            case 0x90:
            case 0xE0:
            case 0xE1:
            case 0xE2:
            case 0xE3:
            case 0xE4:
            case 0xE5:
            case 0xE6:
            case 0xE7:
            case 0xE8:
                return true;
        }
    }

	return false;
}

//-----------------------------------------------------------------------------
//  IfNII8087         Check if NII 0x0080...0x0087
//  Parameters:       
//  Global Inputs:    
//  Returns:          true - Yes NII 80..87  
//                    false - No,
//  Notes:
//-----------------------------------------------------------------------------
bool IfNII8087()
{
    static UBYTE currentNII[S_AQNII] ; // current NII

    memset (currentNII, 0, S_AQNII);

    switch (tranMode)
    {
        case INIT_TRAN:
            memcpy( currentNII, (UBYTE *)TERM.TERMNII, sizeof( TERM.TERMNII ) );
            break;

        case LOGON_TRAN:
        case COMMON_TRAN:
        case SETTLE_TRAN:
            MoveItAq( currentNII, paq->AQNII, S_AQNII );
            break;

        default:
            return false;
    }

    if ( 0 != currentNII[0] )
        return false;

    if ( (currentNII[1] >= 0x80)&&(currentNII[1] <= 0x87) )
        return true;
    else
        return false;

}


extern void CommPermEthernetConfig(void)
{
	HypCConfig cfg;
	modemMode = 0xE8;	//Ethernet comm mode
	cfgType=ETHERNET_CFG;

	SetTCPIPConfig(cfg, False);

	unsigned int hemsParam = 0;

    cfg.Set(HC_LocalIpAddr, TERM.TERMIPAddress);
	cfg.Set(HC_LocalNetMask, TERM.TERMSubnetIP);
	cfg.Set(HC_DefaultGwAddr, TERM.TERMGatewayIP);


	hemsParam = ((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET) ?
	               (TERM.TERMOpt3 & TM3_HEMSTRANSP) ? 1 : 2 :
	               (((TERM.TERMOpt3 & TM3_HWTYPE_MASK) != TM3_BLUETOOTH) || (TERM.TERMOpt3 & TM3_HEMSTRANSP)) ? 3 : 4;
	hemsParam |= (TERM.TERMPDMode << 8) & 0xff00;
	cfg.Set(HC_HemsParam,hemsParam); 

	cfg.Set(HC_Ip1, 0x0000);

    cfg.Set(HC_ModemModeNo,modemMode);
	cfg.Set(HC_ConnAttempt, 1);
	COM_confSet->append(cfg);
	Dest.sin_addr.s_addr = 0;

}

extern void CommPermGSMConfig(void)
{
	HypCConfig cfg;
	modemMode = 0xE5;	//Ethernet comm mode
	UBYTE cPPPMode;

	cfg.Set(HC_ConnAttempt,1);
	cfg.Set(HC_Timeout,30*1000);

	cfg.Set(HC_TelNo, "*99***1#" );

	//cfg.Set(TransmitLevel,10); -default
	cfg.Set(HC_ModemModeNo, modemMode);

	cfg.Set(HC_ModemATCmd,(char *)ModemATCommand);


	//set pad logon script
	SetPadLogon (cfg);
	
	cPPPMode = 0x21;

	cfg.Set(HC_PPPMode, 0x0f & cPPPMode);
    // Set LCP timeout as high nibble of PPPMode (or 10s if not defined)
	if ( (cPPPMode>>4) == 0 )
	{
		cfg.Set(HC_PPPLCPTimeout, 10);
	}
	else
	{
		cfg.Set(HC_PPPLCPTimeout, (cPPPMode>>4));
	}

	cfg.Set(HC_PPPUsername, (const char *)LogonUsername);
	cfg.Set(HC_PPPPassword, (const char *)LogonPassword);
	cfg.Set(HC_PPPConnectionTimeout, bcd2bin(TERM.TERMTOut));
	cfg.Set(HC_SerialDriverMode,HYP_PPP); 


	SetTCPIPConfig(cfg, False);

	unsigned int hemsParam = 0;

    cfg.Set(HC_LocalIpAddr, TERM.TERMIPAddress);
	cfg.Set(HC_LocalNetMask, TERM.TERMSubnetIP);
	cfg.Set(HC_DefaultGwAddr, TERM.TERMGatewayIP);


	hemsParam = ((TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_ETHERNET) ?
	               (TERM.TERMOpt3 & TM3_HEMSTRANSP) ? 1 : 2 :
	               (((TERM.TERMOpt3 & TM3_HWTYPE_MASK) != TM3_BLUETOOTH) || (TERM.TERMOpt3 & TM3_HEMSTRANSP)) ? 3 : 4;
	hemsParam |= (TERM.TERMPDMode << 8) & 0xff00;
	cfg.Set(HC_HemsParam,hemsParam); 

	cfg.Set(HC_Ip1, 0x0000);

    cfg.Set(HC_ModemModeNo,modemMode);
	cfg.Set(HC_ConnAttempt, 1);
	COM_confSet->append(cfg);

	Dest.sin_addr.s_addr = 0;

}
