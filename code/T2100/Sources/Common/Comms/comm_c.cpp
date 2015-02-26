
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             comm_c.c
//      COMM process
//
//=============================================================================

#include "hw.h"
#include "HypCCommunication.hpp"
#include "HypCACSCommunication.hpp"
#include "hyputils.h"
#include "HypCConfig.hpp"
#include "sockets.h"

#include "comm_c.h"
#include "basictyp.h"

extern "C" {
#include "ssl.h"
#include "sdk.h"
#include "fixdata.h"
#include "struct.h"
#include "comdata.h"
#include "rs232tab.h"
#include "comm.h"
#include "comm_cfg.h"
#include "commmodule.h"
#include "SimPinSend.h"
#include "sdktime.h"
#include "basnmp.h"
}

#define DIALGAPTIME 20000  //20 sec
#define LINE_STATUS_TIME 1000  // check Line Status every 1 sec 

HypCConfigSet *COM_confSet = NULL;
HypCConfig    *COM_addConf = NULL;
int  COM_HPOSFCERR;
bool COM_hexLen;
bool COM_useSocket;
bool COM_firstCall;
bool COM_GSMLowSignalLevel;
bool COM_RetainLink;

struct sockaddr_in Dest;
struct sockaddr_in Dest2;

HypCCommunication *comm=NULL;
HypCEvent *event=NULL;
static HypCBuffer * buffer=NULL;
static HypCTimer * timer=NULL;
HypCTimer * retain_link_timer=NULL;
static SOCKET sock=INVALID_SOCKET;

enum HypEModuleType COM_moduleType;
enum HypEModuleType ProxyModuleType;

extern Bool IsNewPPPConnectionRequested( void );
extern void SavePPPSpecificParams( void );
Bool HangUpPPP( void );
//extern Bool IsStateSocketIdle( void );

static void Init_COMM();
static void Free();
static bool WaitEvent();

static void StIdle();
static void StConnecting();
static void StConnected();
static void StHangUpSoon();
static void StSocketIdle();

static void ReadCompleted ();
static void HangUp();
static void ConnectSocket();
static Bool ProcessSNMP();
bool CheckFrame(int &opt);


enum Events
{
    USER_COM_RESET_REQ=USERDEFINED_REQUEST,
    USER_CONNECT_REQ,
	USER_CLOSE_SESSION_REQ,
    USER_DISCONNECT_NOW_REQ,
    USER_DISCONNECT_SOON_REQ,
    USER_TRANSMIT_MSG_REQ,
    COMM_CONNECTED,
    DATA_RECEIVED,
    MSG_TRANSMITTED,
	READ_ERROR,
	WRITE_ERROR,
	CONNECT_ERROR,
	HANG_UP_SOON_TIMER_EXPIRED,
	CONNECT_TIMER_EXPIRED,
	LINE_STATUS_TIMER_EXPIRED,
	RETAIN_LINK_TIMER_EXPIRED
};

enum States
{
	ST_IDLE,
	ST_CONNECTING,
	ST_CONNECTED,
	ST_SOCKET_IDLE,
	ST_HANGUP_SOON
};


static int lastEvent;
static enum States state;
static int  status;
static int commStatus;


static bool connectInProgress;
static bool hangupInProgress;
static bool closeSessionInProgress;
static bool hangupSoonInProgress;
static bool resetInProgress;
static bool writeInProgress;
static bool readInProgress;
static bool retainTimerExpired = false;

bool COM_DBPreInitFlag;
bool COM_PreInitFlag;

static const char* writeBuf;
static unsigned int writeLen;
static char* readBuf;
static unsigned short *readLen;
static Bool CreatePermConnect();
const char BeginRS0[]="***** Begin Com1 trace *****\n\r";
const char BeginRS1[]="***** Begin Line trace *****\n\r";
const char BeginRS2[]="***** Begin Com2 trace *****\n\r";

const char EndRS0[]="***** End Com1 trace *****\n\r";
const char EndRS1[]="***** End Line trace *****\n\r";
const char EndRS2[]="***** End Com2 trace *****\n\r";


#define SSL_PIECE_SIZE 1024


//=============================================================================
// Public function definitions
//=============================================================================
void COM_ConnectReq()
{
       while (hangupInProgress || hangupSoonInProgress) 
       {
                  SDK_RelinqCPU(  );      
      }

	if ( !connectInProgress && ((state==ST_IDLE) || (state==ST_SOCKET_IDLE)) )
	{
		COM_firstCall = True;
		event->Send(USER_CONNECT_REQ);
		connectInProgress=true;
	}
}

int  COM_ConnectCompleted()
{
	return !connectInProgress;
}

void COM_HangUpReq()
{
	if (!hangupInProgress)
	{
		if (!COM_useSocket)
		{
			event->Send(USER_DISCONNECT_NOW_REQ);
			hangupInProgress=true;
		}
		else
		{
			// Close socket and switch into ST_SOCKET_IDLE
			COM_CloseSessionReq();
		}
	}
}
void COM_CloseSessionReq()
{
	if (!closeSessionInProgress)
	{
		event->Send(USER_CLOSE_SESSION_REQ);
		closeSessionInProgress=true;
	}
}
int  COM_CloseSessionCompleted()
{
	return closeSessionInProgress;
}

void COM_HangUpSoonReq()
{
	if (!(hangupSoonInProgress || hangupInProgress))
	{
		event->Send(USER_DISCONNECT_SOON_REQ);
		hangupSoonInProgress=true;
	}
}

int  COM_HangUpCompleted()
{
	return !(hangupSoonInProgress || hangupInProgress);
}

void COM_ResetReq()
{
	event->Send(USER_COM_RESET_REQ);
	resetInProgress=true;
}

int  COM_ResetCompleted()
{
	return !resetInProgress;
}

void COM_WriteReq(const void* buf , unsigned int len)
{
	if (writeInProgress==true)
	{
		return;
	}
	writeInProgress=true;
	if (state!=ST_CONNECTED)
	{
		writeInProgress=false;
		COM_HPOSFCERR=HTENOTCONN;
		return;
	}
	if (COM_HPOSFCERR!=HTENOERROR)
	{
		writeInProgress=false;
		return;
	}
	writeBuf=(const char *)buf;
	writeLen=len;
	event->Send(USER_TRANSMIT_MSG_REQ);
}

int COM_WriteCompleted()
{
	return !writeInProgress;
}


void COM_ReadReq(void* buf , unsigned short* len)
{
	if (readInProgress==true)
	{
		return;
	}
	readInProgress=true;
	if (state!=ST_CONNECTED)
	{
		readInProgress=false;
		COM_HPOSFCERR=HTENOTCONN;
		return;
	}
	readBuf=(char*)buf;
	readLen=len;
	ReadCompleted();
}

int  COM_ReadCompleted()
{
	return !readInProgress;
}
void COM_CancelRead()
{
	readInProgress=false;
}


//-----------------------------------------------------------------------------
//  INITCOMM                Entry point for the communications process.
//
//  Parameters:             None.
//
//  Returns:                Never returns!
//-----------------------------------------------------------------------------
void INITCOMM(void)
{
	// Wait until applications are enabled
	while (!(ENAPPL.initialized))
	{
		HW_Relinquish();
	}

	// init the COMM process.
	Init_COMM();

	while (1)
	{
		switch (state)
		{
		case ST_IDLE:
			StIdle();
			continue;
		case ST_SOCKET_IDLE:
			StSocketIdle();
			continue;
		case ST_CONNECTING:
			StConnecting();
			continue;
		case ST_CONNECTED:
			StConnected();
			continue;
		case ST_HANGUP_SOON:
			StHangUpSoon();
			continue;
		}
	}
}

void Init_COMM()
{
	if (comm!=NULL)
    {
		resetInProgress=false;
		return;
    }

	event=new HypCEvent(4);
	timer=new HypCTimer(*event);
	retain_link_timer = new HypCTimer(*event);
	COM_addConf = new HypCConfig();
	COM_confSet= new HypCConfigSet(2);

	//get installed modem type
	switch(TERM.TERMOpt3 & TM3_HWTYPE_MASK)
	{
		case TM3_GSM:
			COM_moduleType = HW_GSMMODEM;
			break;

		case TM3_GSM_OLD:
			COM_moduleType= HW_GSMMODEM_OLD;
			break;

		case TM3_V90:
			COM_moduleType = HW_MODEMV90;
			break;

		case TM3_V22:
			COM_moduleType = HW_MODEMV22;
			break;

		case TM3_ETHERNET:
			COM_moduleType = HW_ETHERNET;
			break;

		case TM3_BLUETOOTH:
			COM_moduleType = HW_BLUETOOTH;
			break;

		case TM3_LAN:
			COM_moduleType = HW_POSLAN;
			break;

		case TM3_ISDN:
			COM_moduleType = HW_MODEMISDN;
			break;

		case TM3_DIALBACKUP: //@AAMELIN
			COM_moduleType = HW_GSMMODEM;
			break;

		default:
			COM_moduleType = HW_NOMODULE;
			break;
	}

    // Check for Bluetooth ASC communication
    if ( (!(TERM.TERMOpt3 & TM3_HEMSTRANSP)) && 
         ( (TERM.TERMOpt3 & TM3_HWTYPE_MASK) == TM3_BLUETOOTH ) &&
         !(RSTAB.RSOPT & RSOPT_RSACTIVE) 
       )
    {
        comm=new HypCACSCommunication(0, COM_moduleType);
    }
    // Check for Bluetooth trasparent
	else if ( (COM_moduleType == HW_BLUETOOTH) && 
              ((TERM.TERMRemoteModule & TMR_HWTYPE_MASK) != TMR_ETHERNET) &&
              !(RSTAB.RSOPT & RSOPT_RSACTIVE)
            )
	{
		ProxyModuleType = COM_moduleType;

		//Get installed modem type
		switch(TERM.TERMRemoteModule & TMR_HWTYPE_MASK)
		{
			case TMR_GSM:
				COM_moduleType = HW_GSMMODEM;
				break;
	        
			case TMR_GSM_OLD:
				COM_moduleType = HW_GSMMODEM_OLD;
				break;
	        
			case TMR_V90:
				COM_moduleType = HW_MODEMV90;
				break;
	        
			case TMR_V22:
				COM_moduleType = HW_MODEMV22;
				break;
	        
			case TMR_ETHERNET:
				COM_moduleType = HW_ETHERNET;
				break;
	        
			case TMR_BLUETOOTH:
				COM_moduleType = HW_BLUETOOTH;
				break;
	        
			case TMR_LAN:
				COM_moduleType = HW_POSLAN;
				break;
	        
            case TMR_ISDN:
				COM_moduleType = HW_MODEMISDN;
				break;
	        
			default:
				if ( (TERM.TERMLine == 1) || (TERM.TERMLine == 2) )
				{
					COM_moduleType = HW_MODEMV90;
					break;
				}
                else
                {
                    // Unknown module type
                    COM_moduleType = HW_NOMODULE;
                }
		}

		comm=new HypCCommunication(0, COM_moduleType, ProxyModuleType);
	}

    else
    {
        comm=new HypCCommunication(0, COM_moduleType);
    }

    ONLINE = 0;
	state=ST_IDLE;
	connectInProgress=false;
	hangupInProgress=false;
	closeSessionInProgress=false;
	hangupSoonInProgress=false;
	resetInProgress=false;
	writeInProgress=false;
	readInProgress=false;
	buffer=NULL;
	commStatus=cs_IDLE;
	COM_useSocket=false;
	COM_HPOSFCERR=HTENOERROR;
}

void Free()
{
	if (comm==NULL)
	{
		resetInProgress=false;
		return;
	}
	delete comm;
	comm=NULL;

	delete COM_addConf;
	COM_addConf=NULL;

	delete COM_confSet;
	COM_confSet=NULL;

	delete timer;
	timer=NULL;

	ClearEvent(event);
	delete event;
	event=NULL;

	if (buffer!=NULL)
	{
		delete buffer;
		buffer=NULL;
	}
}



void StIdle()
{
unsigned int connectTimeout;
	while (true)
	{

		if ( (TCONF.TCOPT7 & TC7_PERMN_IP) && (lastEvent!=USER_CONNECT_REQ)) 
		{
			if ( CreatePermConnect() == True)
				return;
		}

		// Wait for a command from the user process and then process it.
		if (WaitEvent()==true)
		{
			//event allready proccesed,
			return;
		}
		if (lastEvent==USER_CONNECT_REQ)
		{
			CommConfig();
			if (COM_HPOSFCERR!=HTENOERROR)
			{
				connectInProgress=false;
				return;
			}
			comm->SetDestination( *COM_confSet );
			//start dialing
			comm->ConnectReq(*event);

			connectTimeout=0;
			
            for (HypCConfig *c=COM_confSet->begin(); c<COM_confSet->end(); c++)
			{
				int count=1;
				int to=60000;

                // get timeout if exist
				if (c->IsExist(HC_Timeout))
				{
					to=c->Get(HC_Timeout);
				}

                // get retry attempts if exist
				if (c->IsExist(HC_ConnAttempt))
				{
					count=c->Get(HC_ConnAttempt);
				}

                // calculate connect timeout
				connectTimeout+=(count*(to+DIALGAPTIME));
			}

            // set default timeout if it invalid
			if (connectTimeout==0)
			{
				connectTimeout=60000;
			}
			if ( TCONF.TCOPT7 & TC7_RETAIN_LINK )
			{
				COM_RetainLink = True;
			}

			//start conect timeout timer
			timer->Start(connectTimeout, CONNECT_TIMER_EXPIRED);
			commStatus=cs_DIALING;
			state=ST_CONNECTING;
			return;				//transaction throw socket interface
		}
	}
}

void StSocketIdle()
{
	while (true)
	{
#ifdef MAKE_SNMP
		if ( ProcessSNMP() == False )
		{
			// If we need to STOP agent (after Init)
			HangUp();
			return;
		}
#endif  // MAKE_SNMP

		// Wait for a command from the user process and then process it.
		if (WaitEvent()==true)
		{
			//event already processed
			return;
		}

		if ( (RETAIN_LINK_TIMER_EXPIRED == lastEvent) )
		{
			HangUp();
			return;
		}
		if ( CONNECT_ERROR == lastEvent)
		{
			HangUp();
			return;
		}

		if ( USER_CONNECT_REQ == lastEvent)
		{
			COM_confSet->clear();
			CommConfig();
			if (COM_HPOSFCERR!=HTENOERROR)
			{
				HangUp();
				COM_HPOSFCERR=HTEINVAL;
				return;
			}

			if ( HangUpPPP( ) )
				return;			// must reconnect; quit this state

			ConnectSocket();
			if (COM_HPOSFCERR!=HTENOERROR)
			{
				return;
			}

			commStatus=cs_RECEIVING;
			state=ST_CONNECTED;
			connectInProgress=false;
			ONLINE = 1;
			return;
		}
	}
}

void ConnectSocket()
{
	int err;

	if ( (TCONF.TCOPT7 & TC7_RETAIN_LINK) && ( COM_RetainLink == True) )
	{
		retain_link_timer->Start(  atoi( (char *) TCONF.TCRETAINTIME ) * 1000, RETAIN_LINK_TIMER_EXPIRED);
	}	

	COM_HPOSFCERR=HTENOERROR;
	commStatus=cs_TCPCONNECTING;
	sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( sock  == INVALID_SOCKET )
	{
		goto L_ERR;
	}

	// prepare destination address	
	Dest.sin_family = AF_INET;

	// connect socket
	err = connect( sock, (struct sockaddr *)&Dest, sizeof(struct sockaddr) );
	if(err)
	{
		err = connect( sock, (struct sockaddr *)&Dest2, sizeof(struct sockaddr) );
		if(err)
		{
			// clear Dest Structures
			memset (&Dest,0,sizeof(struct sockaddr));
			memset (&Dest2,0,sizeof(struct sockaddr));
			L_ERR:
			COM_HPOSFCERR=HTECOMM;
			event->Send(CONNECT_ERROR);
			return;
		}
	}
	// clear Dest Structures
	memset (&Dest,0,sizeof(struct sockaddr));
	memset (&Dest2,0,sizeof(struct sockaddr));
	UDWORD push=1;
	ioctlsocket(sock, SIOPUSH, &push);
	// MAKE SSL CONNECT if we need it
#ifdef MAKE_SSL
	HypCConfig *conf=COM_confSet->begin();
	if ( conf->Get(HC_SslEnabled) == 1 )
	{
		SSLInit(sock, (int(*)(void*)) conf->Get(HC_SslCheckCert));
		if (conf->Get(HC_SslMutualEnabled))
		{
			if(SSLInitMutual((BYTE*)conf->Get(HC_SslClientCertPtr), conf->Get(HC_SslClientCertLen),
						(BYTE*)conf->Get(HC_SslPrivateKeyPtr), conf->Get(HC_SslPrivateKeyLen),
						conf->Get(HC_SslPrivateKeyFmt), conf->Get(HC_SslMasterKeyIdx)) == IPERR_SSLCERT)
			{

				COM_HPOSFCERR=HTECOMM;
				event->Send(CONNECT_ERROR);
				return;
			}
		}
	}
#endif // MAKE_SSL

}

void StConnecting()
{
	while (true)
	{
		// Wait for a command from the user process and then process it.
		if (WaitEvent()==true)
		{
			//event allready proccesed,
			return;
		}

		switch (lastEvent)
		{
		case COMM_CONNECTED:
			timer->Stop();

			if (COM_useSocket)
			{
				SavePPPSpecificParams( );

				// Start SNMP agent if need it
#ifdef MAKE_SNMP
				ProcessSNMP();
#endif // MAKE_SNMP
				// If permanent IP connection transaction starts only from Socket Idle
				if ( 0 == Dest.sin_addr.s_addr )
				{
					state = ST_SOCKET_IDLE;
					return;
				}
				else
					ConnectSocket();
	
				if (COM_HPOSFCERR!=HTENOERROR)
				{
					return;
				}
			}

			commStatus=cs_RECEIVING;
			COM_HPOSFCERR=HTENOERROR;
			state=ST_CONNECTED;
			connectInProgress=false;
			ONLINE = 1;
			if (!COM_useSocket)
			{
				//start reading
				comm->ReadReq(*event);
			}
			return;
		case CONNECT_ERROR:
			HangUp();
			if (COM_firstCall)
			{
				event->Send(USER_CONNECT_REQ);
				connectInProgress=true;
				COM_firstCall = false;
				return;
			}
			COM_HPOSFCERR=status;
			return;
		case CONNECT_TIMER_EXPIRED:
			HangUp();
			if (COM_firstCall)
			{
				event->Send(USER_CONNECT_REQ);
				connectInProgress=true;
				COM_firstCall = false;
				return;
			}
			COM_HPOSFCERR=HTETIMEOUT;
			return;
		default:
			continue;
		}
	}
}

void StConnected() 
{
	while (true)
	{
		// Wait for a command from the user process and then process it.
		if (WaitEvent()==true)
		{
			//event allready proccesed,
			return;
		}
		switch (lastEvent)
		{
		case USER_TRANSMIT_MSG_REQ:
		{
			HypCBuffer *b=new HypCBuffer (writeBuf, writeLen);
			//append hex len
			if (COM_hexLen)
			{
				//add hex len
				b->insert(0,(char)(writeLen&0xFF));
				b->insert(0,(char)((writeLen>>8)&0xFF));
			}

			if (COM_useSocket)
			{
				if (COM_confSet->begin()->Get(HC_SslEnabled) == 1)
					SSLsend( (char*)b->data(), b->size(), 0);
				else
					send(sock, (char*)b->data(), b->size(), 0);
				delete b;
				lastEvent=MSG_TRANSMITTED;
				goto L_MSG_TRANSMITTED;
			}
			else
			{
				comm->WriteReq(*event, b);
			}
			continue;
		}

		case MSG_TRANSMITTED:
L_MSG_TRANSMITTED:
			COM_HPOSFCERR=HTENOERROR;
			writeInProgress=false;
			continue;

		case WRITE_ERROR:
			COM_HPOSFCERR=status;
			writeInProgress=false;
			continue;

		case DATA_RECEIVED:
			ReadCompleted ();
			if (!COM_useSocket)
			{
				//read again
				comm->ReadReq(*event);
			}
			continue;
		case READ_ERROR:
			COM_HPOSFCERR=status;
			readInProgress=false;
		case USER_CLOSE_SESSION_REQ:
			closesocket(sock);
			sock=INVALID_SOCKET;
			state=ST_SOCKET_IDLE;
			closeSessionInProgress=false;
			if ( retainTimerExpired != true )
				return;
		case USER_DISCONNECT_SOON_REQ:
			timer->Stop();
			timer->Start(1000, HANG_UP_SOON_TIMER_EXPIRED);
			state=ST_HANGUP_SOON;
			return;
		case RETAIN_LINK_TIMER_EXPIRED:
			retainTimerExpired = true;
			continue;
		default:
			continue;
		}
	}
}



void StHangUpSoon()
{
	while (true)
	{
		// Wait for a command from the user process and then process it.
		if (WaitEvent()==true)
		{
			//event allready proccesed,
			return;
		}
		if (lastEvent==HANG_UP_SOON_TIMER_EXPIRED)
		{
			HangUp();
			COM_HPOSFCERR=HTENOERROR;
			return;
		}
	}
}

//returns true if event has been already serviced
bool WaitEvent()
{
	while (true)
	{
		int opt, id;
		if (COM_useSocket)
		{
			//check event: non-blocking call
			HypCEventInfo ei;
			if (event->Wait(ei,0))
			{
				lastEvent=ei.GetRequestID();
				status=ei.GetStatus();
				opt=ei.GetOpt();
			}
			else
			{
				//event was not received
				//check events from socket
				if (state==ST_CONNECTED)
				{
						if (CheckFrame ( opt ) == true)
					//@AAMELIN Check socket was already closed 
							goto L_SWITCH;
				}
				HW_Relinquish();
				continue;
			}
		}
		else
		{
			event->Wait(lastEvent, status, id, opt);
		}

L_SWITCH:
		//COM_RESET_REQ, DISCONNECT_NOW_REQ, DISCONNECT_SOON_REQ are processed in all states
		switch (lastEvent)
		{
		case USER_COM_RESET_REQ:
			Free();
			Init_COMM();
			return true;
		case USER_DISCONNECT_NOW_REQ:
L_USER_DISCONNECT_NOW_REQ:
			HangUp();
			COM_HPOSFCERR=HTENOERROR;
			return true;
		case USER_DISCONNECT_SOON_REQ:
			//'Hang up soon' can be proccesed only in the connected state
			//it is considered as Hang up now in any other state 
			if (state==ST_CONNECTED)
			{
				return false;
			}
			else
			{
				lastEvent=USER_DISCONNECT_NOW_REQ;
				HangUp();
				COM_HPOSFCERR=HTENOERROR;
				return true;
			}
		case READ_REQUEST:
			if ((status==HTENOERROR)&&(opt!=0))
			{
				if (buffer!=NULL)
				{
					delete buffer; 
				}
				buffer=(HypCBuffer *)opt;
				lastEvent=DATA_RECEIVED;
			}
			else
			{
				lastEvent=READ_ERROR;
			}
			return false;
		case CONNECT_REQUEST:
			if ((status==HTENOERROR) || (status==HTEISCONN))
			{
				lastEvent=COMM_CONNECTED;
			}
			else
			{
				lastEvent=CONNECT_ERROR;
			}
			return false;

		case WRITE_REQUEST:
			if (status==HTENOERROR)
			{
				lastEvent=MSG_TRANSMITTED;
			}
			else
			{
				lastEvent=WRITE_ERROR;
			}
			return false;
		case USER_CLOSE_SESSION_REQ:
			if ((state!=ST_CONNECTED)||!COM_useSocket)
			{
				goto L_USER_DISCONNECT_NOW_REQ;
			}
			return false;

		default:
			return false;
		}
	} //while
}

void ReadCompleted ()
{
	if (COM_HPOSFCERR!=HTENOERROR)
	{
		readInProgress=false;
		return;
	}
	if ((buffer!=NULL)&&(readInProgress))
	{
		int len= buffer->size();
		char * data= buffer->data();
		if (COM_hexLen)
		{
			data+=2;
			len-=2;
		}
		if (len>0)
		{

			if ((*readLen)<len)
			{
				len=*readLen;
			}
			memcpy(readBuf, data , len);
			*readLen=len;
		}
		else
		{
			*readLen=0;
		}
		delete buffer;
		buffer=NULL;
		readBuf=NULL;
		readLen=NULL;
		COM_HPOSFCERR=HTENOERROR;
		readInProgress = false;
	}
}


//-----------------------------------------------------------------------------
//  COM_PreInit                  Perform preinitialization of communications when idle
//
//  Parameters:             None.
//
//  Returns:                Nothing.
//-----------------------------------------------------------------------------
void COM_PreInit()
{
	HypCConfig conf;

	// Depending on the module type setup additional common parameters
	switch (COM_moduleType)
	{
	case HW_GSMMODEM:
	case HW_GSMMODEM_OLD:
			// For GSM modem we can play with HC_FlowControl,
			// but app normally use default value
			break;

	case HW_MODEMV90:
			// For V90 modem set HC_LoadConexantPatch,
			// HC_ConexantPatchPtr, and HC_ConexantPatchLength
			if (TERM.TERMOpt2 & TM2_9600NF)
			{
				conf.Set(HC_LoadConexantPatch, 1);
			}
			// fall through
	case HW_MODEMV22:
			// For V90 and V22 modems - HC_CountryCode
			if (TERM.TERMCountry!=0xff)
			{
				conf.Set(HC_CountryCode,(int)(TERM.TERMCountry));
			}
			break;

	default:
			break;
	}

	// Preinitialize communications
	if (comm->PreInit( &conf ))
	{
		// Set preinitialize is done if successfully completed
		if ((TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK)) && (COM_moduleType == HW_MODEMV90) )
			COM_DBPreInitFlag = True;	   
		else
			COM_PreInitFlag = True;	   
	}

}


//-----------------------------------------------------------------------------
//  HangUp                  Hang up immediately!  This function sends a hang up
//                          command, thereby cancelling all outstanding I/O
//                          commands.
//
//  Parameters:             None.
//
//  Returns:                Nothing.
//-----------------------------------------------------------------------------
void HangUp()
{
	ONLINE = 0;
	hangupInProgress=true;
	// Disconnect transport
	// Stop timer
	timer->Stop();
	if (COM_useSocket)
	{
		closesocket(sock);
		sock=INVALID_SOCKET;
	}
	comm->Disconnect();
	COM_PreInit();		 //@AAMELIN


	if (TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK))
	  DBSwitchToGSM(); //@AAMELIN

	ClearEvent(event);
	delete buffer;
	buffer=NULL;
//	if (TM3_GSM_OLD == (TERM.TERMOpt3 & TM3_HWTYPE_MASK))
//    	SimPinSend ( );  // unlock GSM SIM card if required
    state= ST_IDLE;
	hangupInProgress=false;
	closeSessionInProgress=false;
	hangupSoonInProgress=false;
	writeInProgress=false;
	if (readInProgress)
	{
		*readLen=0;
		readInProgress=false;
	}
	COM_addConf->Clear();
    COM_confSet->clear();
	COM_useSocket=false;
	connectInProgress=false;
	COM_HPOSFCERR=HTECANCELIO;
	commStatus=cs_IDLE;
	COM_RetainLink = False;
	retainTimerExpired = false;
	retain_link_timer->Stop();
	
}

void COM_AddCfgParam(enum HypEConfigKeys k, int val)
{
	COM_addConf->Set(k, val);
}

void COM_AddCfgParamStr(enum HypEConfigKeys k, const char * s)
{
	COM_addConf->Set(k, s);
}

void COM_AddCfgClear()
{
	COM_addConf->Clear();
}

int COM_GetStatus( )
{
	if (hangupInProgress)
		return (cs_RECEIVING); 
	if ((commStatus==cs_DIALING)&&(comm!=NULL))
	{
		switch (comm->GetModemStatus())
		{
		case HypCTransport::STATUS_INIT:
			return cs_INITIALIZING;
		case HypCTransport::STATUS_DIALING:
			return cs_DIALING;
		case HypCTransport::STATUS_REDIALING:
			return cs_REDIAL;
		case HypCTransport::STATUS_LOGON:
			return cs_SENDING;
		case HypCTransport::STATUS_IDLE:
			return cs_IDLE;
		case HypCTransport::STATUS_WAITLINE:
			return cs_WAITLINE;
		case HypCTransport::STATUS_CHECKLINE:
			return cs_CHECKLINE;
		case HypCTransport::STATUS_DISCONNECTING:
			return cs_RECEIVING;
		case HypCTransport::STATUS_CONNECTED:
			return cs_RECEIVING;
		case HypCTransport::STATUS_PPPCONNECTING:
			return cs_PPPCONNECTING;
		case HypCTransport::STATUS_TCPCONNECTING:
			return cs_TCPCONNECTING;
		case HypCTransport::STATUS_PAGING:
			return cs_PAGING;
		case HypCTransport::STATUS_GATECONNECTING:
			return cs_GATECONNECTING;
		default:
			break;
		}
	}
	return commStatus;
}

int COM_GetGateStatus( )  //@AAMELIN
{
	if (hangupInProgress)
		return (cs_RECEIVING); 
	if ((commStatus==cs_DIALING)&&(comm!=NULL))
	{
		switch (((HypCACSCommunication *)comm)->GetGateStatus())
		{
		case HypCTransport::STATUS_INIT:
			return cs_INITIALIZING;
		case HypCTransport::STATUS_DIALING:
			return cs_DIALING;
		case HypCTransport::STATUS_REDIALING:
			return cs_REDIAL;
		case HypCTransport::STATUS_LOGON:
			return cs_SENDING;
		case HypCTransport::STATUS_IDLE:
			return cs_IDLE;
		case HypCTransport::STATUS_WAITLINE:
			return cs_WAITLINE;
		case HypCTransport::STATUS_CHECKLINE:
			return cs_CHECKLINE;
		case HypCTransport::STATUS_DISCONNECTING:
			return cs_RECEIVING;
		case HypCTransport::STATUS_CONNECTED:
			return cs_RECEIVING;
		case HypCTransport::STATUS_PPPCONNECTING:
			return cs_PPPCONNECTING;
		case HypCTransport::STATUS_TCPCONNECTING:
			return cs_TCPCONNECTING;
		case HypCTransport::STATUS_PAGING:
			return cs_PAGING;
		case HypCTransport::STATUS_GATECONNECTING:
			return cs_GATECONNECTING;
		default:
			break;
		}
	}
	return commStatus;
}

//-----------------------------------------------------------------------------
// COM_GetLineStatus
//      Return line availability status
//
// Parameters:
//
// Returns:
//      current status
//
int COM_GetLineStatus( )
{
	if ((state!=ST_IDLE) || (comm==NULL))
	{
		return -1;
	}
	return comm->GetLineStatus( );
}

//-----------------------------------------------------------------------------
// HW_GetSignalLevel
//      Return current signal level
//
// Parameters:
//
// Returns:
//      current status
//
int COM_GetSignalLevel( )
{
	if ((state!=ST_IDLE) || (comm==NULL))
	{
		return -1;
	}
	return comm->GetSignalLevel( );
}


//-----------------------------------------------------------------------------
// SendTrace
//      Send trace trough COM1/COM2
//
// Parameters:
//
// Returns:
//      current status
//
extern void SendTrace(int RS_Port, int Trace_Port)
{
    static char buf[512];
    size_t count;
	HypCBuffer * b;

    HypCSerialPort * sp0= new HypCSerialPort (RS_Port);
    HypCConfig cfg;
    cfg.Set(HC_BaudRate,HYP_SBR_115200);
//   cfg.Set(HC_FlowControl,HYP_HARDWARE_FC);
    cfg.Set(HC_FlowControl,HYP_NONE_FC);
    sp0->SetConfig(cfg);
    sp0->Connect();

	SerialTraceResetOffset(Trace_Port);

	if (RS_0 == Trace_Port) // COM1
		b=new HypCBuffer(BeginRS0, sizeof (BeginRS0));
	if (RS_1 == Trace_Port) // LINE
		b=new HypCBuffer(BeginRS1, sizeof (BeginRS1));
	if (RS_2 == Trace_Port) // COM2
		b=new HypCBuffer(BeginRS2, sizeof (BeginRS2));

    sp0->Write(b);
    HW_Sleep(100);

	while ( (count=SerialTraceReadBuf(Trace_Port, buf, 512))>0)
	{
            b=new HypCBuffer (buf,count);
            sp0->Write(b);
            HW_Sleep(100);
    }

	if (RS_0 == Trace_Port) // COM1
		b=new HypCBuffer(EndRS0, sizeof (EndRS0));
	if (RS_1 == Trace_Port) // LINE
		b=new HypCBuffer(EndRS1, sizeof (EndRS1));
	if (RS_2 == Trace_Port) // COM2
		b=new HypCBuffer(EndRS2, sizeof (EndRS2));

    sp0->Write(b);
    HW_Sleep(100);

    sp0->Disconnect();
    delete sp0;

}

// drops current PPP if new PPP features new params
// returns False if did nothing
Bool HangUpPPP( void )
{
	if ( COM_useSocket ) // use PPP permanent link?
	{
		if ( IsNewPPPConnectionRequested( ) )
		{
			timer->Stop();  

			closesocket(sock);
			sock=INVALID_SOCKET;

			comm->Disconnect();
			COM_addConf->Clear();
			COM_confSet->clear();
			COM_useSocket=false;
			state = ST_IDLE;

			//@AAMELIN CHECK IT alhorithm
			event->Send(USER_CONNECT_REQ);
			return True;

/*            HW_Sleep(100);
			comm->SetDestination( *COM_confSet );
			//start dialing
			comm->ConnectReq(*event);
			connectTimeout=0;
            for (HypCConfig *c=COM_confSet->begin(); c<COM_confSet->end(); c++)
			{
				int count=1;
				int to=60000;

                // get timeout if exist
				if (c->IsExist(HC_Timeout))
				{
					to=c->Get(HC_Timeout);
				}

                // get retry attempts if exist
				if (c->IsExist(HC_ConnAttempt))
				{
					count=c->Get(HC_ConnAttempt);
				}

                // calculate connect timeout
				connectTimeout+=(count*(to+DIALGAPTIME));
			}

            // set default timeout if it invalid
			if (connectTimeout==0)
			{
				connectTimeout=60000;
			}

			//start conect timeout timer
			timer->Start(connectTimeout, CONNECT_TIMER_EXPIRED);
			commStatus=cs_DIALING;
			state=ST_CONNECTING;
			return True;
*/		}

	}
	return False;
}

extern Bool IsStateSocketIdle( void )
{
	return ( state == ST_SOCKET_IDLE );
}


extern void DBSwitchToGSM()
{
	delete (comm);
	HW_DBSwitchToGSM();
	comm=new HypCCommunication(0, HW_GSMMODEM);
	COM_moduleType = HW_GSMMODEM;

}
extern void DBSwitchToAnalog()
{
	delete (comm);
	HW_DBSwitchToAnalog();
	comm=new HypCCommunication(0, HW_MODEMV90);
	COM_moduleType = HW_MODEMV90;

}

#ifdef MAKE_SNMP
// Function process SNMP flags if return False need HangUp
//-----------------------------------------------------------------------------
// ProcessSNMP
//      Process SNMP (set flags)
//
// Parameters:
//
// Returns:
//      False - if need to Stop SNMP agent (usually make hangUp)
//
//-----------------------------------------------------------------------------
Bool ProcessSNMP()
{
	struct ipcontab* pIP1;
	struct ipcontab* pIP2;

	if (TCONF.TCSNMPOPT & TCSNMP_ENABLE)
	{
		// If SNMP not started yet Start it and send traps
		if ( False == bSNMPAgentStart )
		{
			pIP1 = FindIPCById( TCONF.TRAP_DEST1_IP_TAB_ID );
			pIP2 = FindIPCById( TCONF.TRAP_DEST2_IP_TAB_ID );
			// Set trasp destination 
			trapDest1 = (char*) pIP1->ipaddr;
			trapDest2 = (char*) pIP2->ipaddr;
			trapPort1 = (char*) pIP1->port;
			trapPort2 = (char*) pIP2->port;
			// Start SNMP Agent 
			bSNMPAgentStart = True;
			// Wait for sending TRAPs
			while (send_traps == True)
			{
				SDK_RelinqCPU();
			}
		}
		if  ( TCONF.TCSNMPOPT & TCSNMP_LISTEN )
			bSNMPAgentListen = True;
		else
		{
			if (bSNMPAgentListen == True)
			{
				bSNMPAgentListen = False;
				return False;
			}
		}
	}
	else
	{
		if ( bSNMPAgentStart )
		{
			bSNMPAgentStart = False;
			// if SNMP started shout down it (possible after initialization)
			return False;
			// Maybe another feature 
		}
	}
	return True;
}

#endif //MAKE_SNMP

// Create new IP connection if it needed
// Return True we create permanent connection
// Return False we are not create Perm Connection 
Bool CreatePermConnect()
{
	//  CHECK FOR COM LINE
	if (RSTAB.RSOPT & RSOPT_RSACTIVE)
		return False;
	if ( (COM_GetLineStatus() != 1 ) && (COM_GetLineStatus() != 5) )
	{
		timer->Start(1000, LINE_STATUS_TIMER_EXPIRED);
		return False;
	}
	// If the terminal requires initialization, do not permanent PPP 
	if ( 0x00 == TERM.TERMInit )
		return False;

	switch ( TERM.TERMOpt3 & TM3_HWTYPE_MASK )
	{
		case TM3_ETHERNET:
			CommPermEthernetConfig();
			break;
		case TM3_GSM:
		case TM3_GSM_OLD:
		case TM3_DIALBACKUP:
			CommPermGSMConfig();
			break;
		default:
			return False;
	}

	comm->SetDestination( *COM_confSet );
	//start dialing
	comm->ConnectReq(*event);

	COM_useSocket = true;
	timer->Start( 60000, CONNECT_TIMER_EXPIRED);
	commStatus=cs_DIALING;
	state=ST_CONNECTING;
	return True;

}

// This function parted from HSDK I do not check how it work 
bool CheckFrame(int  &opt)
{

	UDWORD have = 0;

	if ((sock != INVALID_SOCKET) && (ioctlsocket(sock, FIONREAD, &have) != 0))
	{
		if (lasterror()==WSAENOTCONN)
		{
			//TCPIP state: disconnected
			closesocket(sock);
			sock=INVALID_SOCKET;
			state=ST_SOCKET_IDLE;
			return false;
		}
	}

	if (have == 0)
	{
		return false;
	}
	
	HypCConfig *conf=COM_confSet->begin();
	bool m_SslWaitLen= true;
	UWORD m_SslRemain = 2;
	int m_SslCurPos = 0;
	UBYTE m_SslLenBuf[2];
	HypCBuffer* m_SslBuffer = NULL;
	HypCBuffer* m_ReadBuffer = NULL;

	if ( conf->Get(HC_SslEnabled) == 1 )
	{
		if( conf->Get(HC_UseHexLength) )
		{
			int len;
			if(m_SslWaitLen)
			{
				// waiting for packet length
				len = m_SslRemain;
				if(!SSLrecv((char*)&m_SslLenBuf[m_SslCurPos], &len, 0))
				{
					m_SslRemain -= len;
					m_SslCurPos += len;
					if(m_SslRemain == 0)
					{
						// length received
						m_SslRemain = (m_SslLenBuf[0] << 8) + m_SslLenBuf[1];
						m_SslBuffer = new HypCBuffer(m_SslRemain+2);
						m_SslBuffer->insert(0, m_SslLenBuf[0]);
						m_SslBuffer->insert(0, m_SslLenBuf[1]);
						m_SslBuffer->append('\0', m_SslRemain);
						m_SslWaitLen = false;
						m_SslCurPos = 2;
					}
					else
					{
						// continue receiving length
						return false;
					}
				}
				else
				{
					// read error
					lastEvent = READ_ERROR;
					status=HTECOMM;	
					return true;
				}
			}
			
			if(!m_SslWaitLen)
			{
				// receiving packet data
				len = m_SslRemain;
				if(!SSLrecv(m_SslBuffer->data() + m_SslCurPos, &len, 0))
				{
					m_SslRemain -= len;
					m_SslCurPos += len;
					if(m_SslRemain == 0)
					{
						// packet received
						m_SslWaitLen = true;
						m_SslRemain = 2;
						m_SslCurPos = 0;
	
						if(m_SslBuffer->size() == 0)
						{
							delete m_SslBuffer;
							m_SslBuffer = NULL;
							return false;
						}
						opt = (int) m_SslBuffer;
						lastEvent = READ_REQUEST;
						return true;
					}
					else
					{
						// continue receiving packet
						return false; 
					}
				}
				else
				{
					// TODO CONNECT ERROR
					// read error
					lastEvent = READ_ERROR;
					status=HTECOMM;	
					return true;
				}
			}
		}
		else
		{
			m_ReadBuffer = new HypCBuffer(SSL_PIECE_SIZE);

			if (m_ReadBuffer==NULL)
			{
				return false;
			}

			for(have = 0;;)
			{
				int len = SSL_PIECE_SIZE;

				m_ReadBuffer->append('\0', len);
	
				if(SSLrecv(m_ReadBuffer->data() + have, &len, 0))
				{
					have += len;
					if(have == 0)
					{
						delete m_ReadBuffer;
						m_ReadBuffer=NULL;
						// TODO CONNECT ERROR
						// read error
						lastEvent = READ_ERROR;
						status=HTECOMM;	
						return true;
					}
				}

				have += len;

				if (len < SSL_PIECE_SIZE)
				{
					m_ReadBuffer->erase(have, SSL_PIECE_SIZE - len);
					break;
				}
			}

			if (m_ReadBuffer->size()==0)
			{
				delete m_ReadBuffer;
				m_ReadBuffer=NULL;
				return false;
			}
			lastEvent = READ_REQUEST;
			opt = (int) m_ReadBuffer;
			return true;
		}
	}
	else
	{
		if (conf->Get(HC_UseHexLength))
		{
			UBYTE len[2];
			if ((recv(sock, (char *)len, 2, MSG_PEEK) != 2) ||
			    ((have - 2) < (UDWORD)((len[0] << 8) + len[1])))
			{
				return false;
			}
			have = (UDWORD)((len[0] << 8) + len[1]) + 2;
		}
	
		m_ReadBuffer = new HypCBuffer(have);
	
		//move buffer to output packet queue
		if (m_ReadBuffer==NULL)
		{
			return false;
		}
	
		m_ReadBuffer->append('\0', have);
		recv(sock, m_ReadBuffer->data(), have, 0);
		lastEvent = READ_REQUEST;
		opt = (int) m_ReadBuffer;
		return true;
	}
	return false;
}
