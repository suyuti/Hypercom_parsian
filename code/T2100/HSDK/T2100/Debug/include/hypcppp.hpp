//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================

#ifndef _HYPCPPP_HPP_
#define _HYPCPPP_HPP_

#include "HypCTransparentCommon.hpp"

/// Configuration options for peer
struct PPPOptions
{
	/// PPP ACCM
	UDWORD AsyncMap;

	/// Magic number
	UDWORD Magic;
	
	/// Initial IP
	UDWORD MyStartIP;

	/// PPP MRU
	UWORD MRU;

	/// Necessary to send magic number
	BOOL SendMagic;

	/// Protocol commpression
	BOOL ProtComp;

	/// Header compression
	BOOL AddComp;

	/// Authentication protocol
	/**
	- PPP_AUTH_NONE No authentication
	- PPP_AUTH_PAP Use PAP
	- PPP_AUTH_CHAP Use CHAP
	*/
	UBYTE AuthProtocol;
#define PPP_AUTH_NONE 0
#define PPP_AUTH_PAP 1
#define PPP_AUTH_CHAP 2

	/// Authentication state
	/**
	- PPP_AUTH_UNDONE Not done yet
	- PPP_AUTH_REQ_SENT Request sent
	- PPP_AUTH_RSP_SENT Response sent
	- PPP_AUTH_OK Autenticated OK
	- PPP_AUTH_FAIL Authentication failed
	*/
	UBYTE AuthState;
#define PPP_AUTH_UNDONE 10
#define PPP_AUTH_REQ_SENT 11
#define PPP_AUTH_RSP_SENT 12
#define PPP_AUTH_OK 13
#define PPP_AUTH_FAIL 14

	/// Pointer to user name for authentication
	char *PAPname;

	/// Pointer to password for authentication
	char *PAPpassword;
};

/// Default options for local side
const struct PPPOptions DefLocOptions = {
	0,
	0,
	0,
	1500,
	TRUE, TRUE, TRUE,
	PPP_AUTH_NONE, PPP_AUTH_UNDONE,
	NULL, NULL
};

/// Default options for remote side
const struct PPPOptions DefRemOptions = {
	0xffffffff,
	0,
	0,
	1500,
	FALSE, FALSE, FALSE,
	PPP_AUTH_NONE, PPP_AUTH_UNDONE,
	NULL, NULL
};

/// PPP protocol implementation
class HypCPpp : public HypCTransparentCommon
{
protected:
		/// Events used in HypCPPP
		enum HypEPppEvents
		{
			/// PPP timeout event
			PPP_TIMEOUT=HypCTransparentCommon::LAST_EV_TAG,

			/// PPP state changed event
			STATE_CHANGED,

			/// PPP shutdown timed out
			PPP_SHUTDOWN_TIMEOUT,

			/// PPP shutdown complete
			PPP_SHUTDOWN_COMPLETE,

			/// last event
			LAST_EV_TAG
		};

		/// FSM states
		enum HypEPppComState
		{
			PS_IDLE,				///< looking for start flag
			PS_PACKET,			///< packet receiving in progress
			PS_UNSTUFF,			///< next byte should be unstuffed
			PS_RESET				///< clear buffer and continue to receive
		};

public:
	HypCPpp( HypCTransport *transport, int nDeviceID );
	virtual ~HypCPpp();
	
	enum {
		/// internal timer event id
		PPPS_TIMER,

		/// internal send event id
		PPPS_SEND,

		/// Number of tries to establish connection
		HemsPppLcpTries = 5,

		/// Size of receive buffer
		HemsComBufferSize = 1600,

		/// Size of PPP event processing task stack
		HemsPppDriverStackSize = 8192,
		
		/// Time to wait for terminate ack from remote before destroying connection
		HemsPppShutdownTimeout = 10000
	};

protected:
	virtual void CheckFrame();
	virtual void BuildFrame();
	virtual void TransportConnected();
	virtual void BeforeDisconnectTransport();
	virtual void StConnected();
	virtual bool Transmit(HypCBuffer *buf);

	bool PPPstart( void );
	BOOL PPPCheckAuth( void );
	void PPPsetAuthenticatee( char *username, char *password );
	UWORD PPPshutdown( unsigned long timeout );
	void ResetPPP( void );

private:
	virtual void StateMachine();

	void Pack8Bits( char MyByte );
	UWORD UnPack16Bits( char *MyPntr );
	void Pack16Bits( UWORD MyInt );
	void PPPpack16Bits( UWORD value, char *location );
	unsigned long UnPack32Bits( char *MyPntr );
	void Pack32Bits( unsigned long MyInt );
	void PPPpack32Bits( unsigned long value, char *location );
	UWORD UnPackProtocol( void );
	
	void StickInFullLength( void );
	void PPP_SendPkt( void );
	void PPP_FormPkt( UWORD MyProtocol );
	void LCP_SendPkt( void );
	void SendLCPRequest( void );
	void LCP_BuildEchoReply( UWORD id );
	void LCP_BuildReject( UWORD id, UWORD code, UWORD data_length, char *data );
	void LCP_BuildACK( UWORD id, UWORD code, UWORD data_length, char *data );
	void LCP_BuildNAK( UWORD id, UWORD code, UWORD data_length, char *data );
	void ProcessLCP( void );
	
	void SendPAPRequest( void );
	void SendPAPAck( UBYTE id );
	void SendPAPNak( UBYTE id );
	void ProcessPAP( void );
	
	void SendCHAPResponse( UBYTE id );
	void SendCHAPChallenge(void );
	void SendCHAPResult( UBYTE id );
	void ProcessCHAP( void );
	
	void SendIPCPRequest( void );
	void IPCP_BuildPacket(UBYTE type, UBYTE id, UBYTE code, UWORD data_length, char *data);
	void IPCP_BuildReject(UBYTE id, UBYTE code, UWORD data_length, char *data);
	void IPCP_BuildAck(UBYTE id, UBYTE code, UWORD data_length, char *data);
	void IPCP_BuildNak(UBYTE id, UBYTE code, UWORD data_length, char *data );
	void ProcessIPCP( void );
	
	void PPPSendTermRequest( void );
	void PPPSendTermAck( UBYTE id );
	void SendProtocolReject( UWORD protocol );
	void ProcessPPP_Pkt( void );

	UWORD GetProtocol(UBYTE* buf, int &Pos);
	bool PppSend(LPBYTE from, UWORD len);
	void AddStuffedByte(HypCBuffer& buf, int &pos, UDWORD accm);
	void AddStuffing(HypCBuffer &buf, UDWORD accm);

public:
	UDWORD GetLocalIp(void);
	void SetLocalIp(UDWORD addr);
	size_t GetMRU(void) { return LocOptions.MRU; }

private:
	/// PPP shutdown timer
	HypCTimer m_ShutdownTimer;

	/// PPP receive buffer
	UBYTE PppRecBuf[HemsComBufferSize];
	
	///Pointer to input buffer
	char *InBuffer;
	
	///Length of data in input buffer
	UWORD InBufLength;
	
	char LCPState;					///<State of LCP Connection State Machine
	char LCPCode;					///<Message Id for LCP Protocol
	
	char IPCPState;					///<State of IPCP State Machine
	char IPCPCode;					///<Message Id for IPCP Protocol

	/// fsm initial state
	HypEPppComState m_St;
	/// current position in receive buffer
	int m_Idx;

	///Message Id for PAP Protocol
	UBYTE PAPCode;
	
	///Message Id for CHAP Protocol
	UBYTE CHAPCode;
	
	/// pointer to output buffer
	char *ppp_outbuffer;

	/// PPP send buffer
	UBYTE databuf[HemsComBufferSize];
	
	///signals if address compression is being used
	char ppp_addcompflag;

	///signals if protocol compression is being used
	char ppp_protocompflag;
	
	/// Work pointer - input parse point
	char *InDataPntr;
	
	/// Work pointer - start of output
	char *OutBufPntr;
	
	/// Work pointer - end of output
	char *OutBufEnd;
	
	/// Local LCP options
	struct PPPOptions LocOptions;
	
	/// Remote LCP options
	struct PPPOptions RemOptions;
	
	/// Currently negotiated remote LCP options
	struct PPPOptions NewRemOptions;
	
	/// Local copy of configuration parameter - user name
	char* m_PppUsername;

	/// Local copy of configuration parameter - user password
	char* m_PppPassword;
	
	/// Local copy of configuration parameter - user LCP timeout
	UWORD m_LcpTO;
	
	/// Local copy of configuration parameter - user PPP timeout
	UWORD m_PppTO;
	
	/// Local copy of configuration parameter - user Authrntication mode
	UBYTE m_AuthMode;
	
	/// Terminate request sent
	UBYTE m_trs;
};

#endif	// _HYPCPPP_HPP_
