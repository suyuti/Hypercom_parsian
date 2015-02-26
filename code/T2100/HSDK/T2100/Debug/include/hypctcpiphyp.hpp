//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================

#ifndef _HYPCTCPIPHYP_HPP_
#define _HYPCTCPIPHYP_HPP_

#include "HypCTransport.hpp"
#include "HypCBuffer.hpp"
#include "HypCEvent.hpp"
#include "HypCSocketsCommon.hpp"

#ifdef DOXYGEN
#define __attribute__(A)
#endif

#ifndef TCPCOMM_H
#define TCPCOMM_H

#define PS_NONE		0			// No processing needed
#define PS_IGNORE   1			// TCP Process should ignore

#define PS_UDP		2			// Ready for UDP to send this puppy
 #define  PSSS_REQSEND  0		// Wating for UDP to send
 #define  PSSS_PROCSEND 1		// UDP is processing a SEND
 #define  PSSS_SENDDONE 2		// UDP Send is compelete .. cleanup buffers
 #define  PSSS_RECV 80			// Waiting for UDP to receive

#define PS_IP 	   	3			// Ready for IP to send this puppy
 #define PSSS_REQSEND   0
 #define PSSS_TRANSPORT 1

#define PS_ARP		4			// Request ARP resolution for destinatino
 #define  PSSS_ARPREQ      0	// Begin an ARP request
 #define  PSSS_WAITRESP    1	// Waiting for an ARP response
 #define  PSSS_ARPTIMEDOUT 2	// ARP packet tries timed out

#define	PS_TCP		5
 #define  PSSS_REQSEND  	0	// Wating for TCP to send
 #define  PSSS_CONNECT	    1	// Wating for SYN for a connect...
 #define  PSSS_WAITCONNECT	2 	// Wating for response to SYN for a connect...
 #define  PSSS_SENDSYNACK	3
 #define  PSSS_CLOSING      4	// Trying to close
 #define  PSSS_WAITSENDACK  5	// Wait for an ACK on this send data
 #define  PSSS_RESET		6	// Force a reset on the connection
 #define  PSSS_REQFIN       7   // Request a FIN
 #define  PSSS_IPSENDING	8	// Sending via IP
 #define  PSSS_WINDOWADVERTISE 9 // Advertise our window size
 #define  PSSS_SENDFIN	    10	// We received a FIN and now we are SENDing one.

#define PS_COMPLETE    127		// Processing of this socket is complete
 #define   PSSS_COMPLETEGOOD 127 // 
 #define   PSSS_COMPLETEERROR 64

#define PS_CLOSED	126			// Connection is closed

#define SETSENDSTATE(sock,a,b)	sock->SendState = a;		\
								sock->SendSubState = b

#define SETRECVSTATE(sock,a,b)	sock->ReceiveState = a;		\
								sock->ReceiveSubState = b

#define SOCKETBLOCKS 0
#define SOCKETNONBLOCK 1

#define Align(x,y)	( ((x)+(~(-(y)))) & (-(y)) )
#define bcd2bin(x)	(((x)>>4) * 10) + ((x) & 0x0f)
#define sizeoftbl(x)	sizeof(x)/sizeof(x[0])

#define IPADDR(a,b,c,d) ((IPADDRESS)(((UDWORD)d<<24) | ((UDWORD)c<<16) | ((UDWORD)b<<8) | ((UDWORD)a)))
#define BENDWORD(a)     (  (*(((UBYTE *)&a)+1)) | (((UBYTE)(a))*256)  )

#ifndef max
	#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
	#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

/// IP Header
typedef struct _IPHEADER
{
	UBYTE	VersionHeaderLength;	///< version and header length
	UBYTE	TypeOfService;			///< TOS
	UWORD	TotalLength;			///< total length
	UWORD	Identification;			///< identification

	UWORD	FragmentOffset;			///< fragment offset
#define FLAGMASK 0xe000
#define FRAGMASK 0x1fff
#define MOREFRAGMENTS 0x2000
#define DONTFRAGMENT  0x4000

	UBYTE	TimeToLive;				///< TTL
#define TTL	128

	UBYTE	Protocol;				///< protocol
#define IP_ICMP	1
#define IP_TCP  6
#define IP_UDP	17

	UWORD	HeaderChecksum;			///< header checksum
	IPADDRESS	Source;					///< source ip address
	IPADDRESS	Destination;			///< destination ip address
} __attribute__((packed)) IPHEADER;

#define VHL	0x45
#define TOSNORMAL 0

#define GetIPVersion(ip) ((ip->VersionHeaderLength >> 4) & 0xf)
#define GetIPHdrLen(ip)  ((ip->VersionHeaderLength & 0xf)
#define GetIPHdrLenBytes(ip)  ((ip->VersionHeaderLength << 2) & 0x3c)

#define tcp_FlagFIN     0x0001
#define tcp_FlagSYN     0x0002
#define tcp_FlagRST     0x0004
#define tcp_FlagPUSH    0x0008
#define tcp_FlagACK     0x0010
#define tcp_FlagURG     0x0020
#define tcp_FlagDO      0xF000
#define tcp_GetDataOffset(tp) ((tp)->flags >> 12)

//
// TCP states, from tcp manual.
// Note: close-wait state is bypassed by automatically closing a connection
//       when a FIN is received.  This is easy to undo.
//
#define StateLISTEN		0	// listening for connection 
#define StateSYNSENT	1	// syn sent, active open 
#define StateSYNREC		2	// syn received, synack+syn sent. 
#define StateESTAB		3	// established 
#define StateFINWT1		4	// sent FIN 
#define StateFINWT2		5	// sent FIN, received FINACK 
//#define StateCLOSEWT	6	// received FIN waiting for close 
#define StateCLOSING	6	// sent FIN, received FIN (waiting for FINACK) 
#define StateLASTACK	7	// fin received, finack+fin sent 
#define StateTIMEWT		8	// dally after sending final FINACK 
#define StateCLOSED		9	// finack received 
#define StateWaitingSend 10	// Waiting for IP to send our data
#define StateNotUsed	127	// Newly opened and not in use

#define ERRNOBUF	1

// Stack size for HEMS TCP/IP driver
#define TcpipStackSize 8192
#endif	// TCPCOMM_H



#ifndef ICMP_H
#define ICMP_H
//------------------------------------------------------------------------------
// ICMP definitions

// ICMP DATA
typedef struct
{
	UBYTE	Type;
#define	ICMP_REQUEST	8
#define	ICMP_REPLY		0
	UBYTE	Code;
	UWORD	Checksum;
	UWORD	Identifier;
	UBYTE	Data[2];
} __attribute__((packed)) ICMPHEADER;

// ICMP packet structure
typedef struct
{
	IPHEADER	Ip;
	ICMPHEADER	Icmp;		
} __attribute__((packed)) ICMPPACKET;
#endif



#ifndef UDP_H
#define UDP_H
//------------------------------------------------------------------------------
// UDP definitions

/// UDP header
typedef struct
{
	UWORD	SourcePort;			///< source port
	UWORD	DestinationPort;	///< destination port
	UWORD	Length;				///< length
	UWORD	Checksum;			///< checksum
} __attribute__((packed)) UDPHEADER;

/// UDP packet structure
typedef struct
{
	IPHEADER	Ip;				///< ip header
	UDPHEADER	Udp;			///< udp header
} __attribute__((packed)) UDPPACKET;

typedef struct _IPOVLY
{
	IPADDRESS 	Next;
	IPADDRESS	Previous;
	UBYTE		Zero;		// Not used, kept at 0
	UBYTE		Protocol;	
	UWORD		Length;		// Protocol length
	IPADDRESS	Source;
	IPADDRESS	Destination;
} __attribute__((packed)) IPOVLY;

typedef struct _UDPPSUEDO
{
	union {
		IPOVLY Pseudo;
		IPHEADER Real;
	} Ip;
	UDPHEADER	Udp;	
} __attribute__((packed)) UDPPSUEDO;
#endif	// ICMP_H



#ifndef TCP_H
#define TCP_H

//------------------------------------------------------------------------------
// TCP definitions
//

/// The first retry timer
#define TCP_STARTTIMER	    16

///Total times to try TCPing
#define TCP_TRIES	3

/// TCP header
typedef struct
{
	UWORD	SourcePort;			///< source port
	UWORD	DestinationPort;	///< destination port
	UDWORD	SendSequence;		///< sequence
	UDWORD	AckSequence;		///< acknoldege
	UBYTE	HeaderLength;		///< header length
	UBYTE	Flags;				///< tcp flags
	UWORD	WindowSize;			///< window size
	UWORD	Checksum;			///< checksum
	UWORD	UrgentPointer;		///< urgent pointer
} __attribute__((packed)) TCPHEADER;
#define TCPHEADERLENGTH  (sizeof(TCPHEADER)/4)

/// The TCP/UDP Pseudo Header
typedef struct
{
    IPADDRESS	IPSource;		///< source ip address
    IPADDRESS	IPDest;			///< destination ip address
    UBYTE       MustBeZero;		///< 0
    UBYTE       IPProtocol;		///< 6
    UWORD       Length;			///< TCP Header + data
	UWORD       Checksum;		///< checksum of tcp header and data
} __attribute__((packed)) TCPPSEUDOHEADER;
#define TCPPSEUDOHEADERLENGTH  (sizeof(TCPPSEUDOHEADER))

#endif	// TCP_H


/**
	@brief TCP/IP stack by PW
*/
class HypCTcpIpHyp : public HypCSocketsCommon
{
protected:
	/**
		@brief Flags to identify data in chainmember.

		@note Used for internal purposes.
	*/
	enum CHAINMEMBER_FLAGS
	{
		CHAIN_DATALOCAL = 1,    /**< Set if the data associated with this chain
		                             header is in this same buffer PAGE+1 field
		                             is FIRST data byte if this is set! */
		CHAIN_HEADER = 2,       ///< Set if this is a HEADER packet.
		CHAIN_DATA = 4          ///< Set if this is a DATA packet.
	};

	/**
		@brief Single member of our buffer chain (internal).

		@note Used for internal purposes.
	*/
	typedef struct _CHAINMEMBER
	{
		struct _CHAINMEMBER *Next; ///< Next in this chain
		struct _CHAINMEMBER *Prior; ///< Prior member in this chain
		size_t Length;          ///< # of bytes in this fragment
		UBYTE Flag;             ///< Flag used to identify type of data here
		UDWORD Sequence;        ///< Used for starting TCP sequencing
		UBYTE *PullPtr;         ///< Pointer to next byte to pull from the buffer
		UBYTE *BasePtr;         ///< Pointer to data in this fragment
	} __attribute__((packed)) CHAINMEMBER;

	/// Length of the CHAINMEMBER structure.
	static const int CHAINMEMBERLENGTH = (sizeof(CHAINMEMBER));

	/**
		@brief Chain of buffers (internal).

		@note Used for internal purposes.
	*/
	typedef struct _BUFFERCHAIN
	{
		struct _BUFFERCHAIN *Next; ///< Next buffer chain (PACKET).
		UBYTE FragmentCount;    ///< Fragment count.
		CHAINMEMBER *ChainFirst; ///< Pointer to the first fragment.
		CHAINMEMBER *ChainLast; ///< Pointer to the last fragment.
	} __attribute__((packed)) BUFFERCHAIN;

	/// Valid signature for ICESOCKET
	static const int VALIDSIGNATURE = 0x3841;

	/**
		@brief Flags for sockets.

		@note Used for internal purposes.
	*/
	enum ICESOCKET_FLAGS
	{
		SOCK_BOUND = 0x01,      ///< Socket is bound.
		SOCK_CONNECTED = 0x02,	///< Socket is connected.
		SOCK_BUSY = 0x04,       ///< Socket is busy.
		SOCK_BLOCK_SEND = 0x08, ///< Socket is blocked for sending.
		SOCK_PUSH = 0x10        ///< Set push flags on outgoing packets.
	};

	/**
		@brief State of send operation.

		@note Used for internal purposes.
	*/
	enum ICESOCKET_SEND_STATE
	{
		SEND_BUSY_REQUEST = 0x01, ///< Packet is sent.
		SEND_BUSY_ACKNOWLEDGE = 0x02 ///< Waiting for acknowledge.
	};

	/**
		@brief Send request bits.

		@note Used for internal purposes.
	*/
	enum ICESOCKET_SEND_REQUEST_BITS
	{
		SRB_REQFIN = 1,         ///< Request a FIN.
		SRB_REQCLOSE = 2        ///< Request a CLOSE sequence.
	};

	/// Do not send acknowldge until this time.
	static const int RECVACKHOLDOFF = 100;
	
	/**
		@brief Pull flags.

		@note Used for internal purposes.
	*/
	enum ICESOCKET_PULL_FLAGS
	{
		PULLCOMMANDMASK = 0x3f, ///< Mask for the pull flags
		PULLSTSOK = 0x00,       /**< This is a return status from the
		                             PULLGETDATA command and means that the
		                             command was ok. */
		PULLNOENOUGHBF = 0x01,  /**< This is a return status from the
		                             PULLGETDATA command and means that the
		                             buffer provided is not big enough to get
		                             all the data that is in the socket (but
		                             what fit in the buffer was transfered)
		                             (all status are valid only with
		                             PULLDATADONE set). */
		PULLGETDATA = 0x03,	///< This is a pull command.
		PULLREMOVE = 0x40,	///< Remove read data.
		PULLDATADONE = 0x80	///< Pull data done.
	};

	/**
		@brief Socket structure definition (internal).

		@note Used for internal purposes.
	*/
	typedef struct _icesocket
	{
		struct _icesocket *Next; ///< Linked list of sockets.
		struct _icesocket *Parent; /**< This pointer must point to NULL if it
		                                is a simple socket and to parent socket,
		                                if it is a socket after accept. */
		UWORD Signature;         ///< Used to determine if a pointer is valid.
		UBYTE Type;              ///< SOCK_DGRAM, SOCK_STREAM, or SOCK_RAW.
		UBYTE Protocol;          ///< IPPROTO_TCP, UDP, ICMP, or RAW.
		UBYTE Flags;             ///< Flags.
		IPPORT Local;            ///< Local Address of this socket.
		IPPORT Remote;           ///< Remote Address of this socket.
		UBYTE State;             ///< Connection state.
		UDWORD Timeout;          ///< Timeout value.
		UBYTE Mode;              ///< SOCKETBLOCKS, or SOCKETNONBLOCK
		UBYTE OwningProcess;     ///< PID for the process that owns this socket.
		UBYTE SendState;         ///< Current protocol state.
		UBYTE SendSubState;      ///< Substate used by the various states.
		UBYTE SendStateBusy;     ///< Set when a function is in progress.
		UBYTE TCPState;          ///< TCP Process state.
		UBYTE ReceiveSubState;   ///< Substate used by the various states.
		UBYTE SendRequestBits;   /**< Set by code other than send to request
		                              send. */
		UBYTE SendBusy;          ///< Set when sending.
		UBYTE *SendData;         ///< Data buffer for sending data.
		size_t SendLength;       ///< Length of sending data.
		UDWORD SendLastTime;     ///< Last time this packet was serviced.
		UBYTE SendTries;         /**< Generally used for retries. Depends on
		                              state. */
		UBYTE SendTimeMult;      ///< Used to exponentiate the time delay.
		UWORD SendErrorCode;     ///< Set if there was a send error.
		UBYTE SendFlags;         ///< TCP Flags to send. SYN, ACK, FIN, etc.
		UBYTE SendNeedsACK;      /**< ACK set if this send needs an
		                              acknowledgement. */
		UDWORD SendSequence;     ///< Last sent segment number.
		UBYTE RecvAcksNeeded;    ///< Ack's the receive didn't get to send.
		UDWORD RecvAckHoldoff;   /**< Don't send ack until this time (cuts down
		                              total number of packets). */
		UBYTE RecvBusy;          ///< Receive lock count.
		UDWORD RecvLastAcked;    ///< Last sequence acked.
		UDWORD RecvSequence;     ///< Last Received segment number.
		WORD RecvTotal;          ///< Total bytes waiting in receive buffer(s).
		UWORD RecvBufStart;      ///< Starting sequence in the receive buffer.
		UBYTE *PullBuffPtr;      ///< Where to put the pulled data.
		size_t PullLength;       /**< Max number of bytes to pull or bytes
		                              actually pulled. */
		UBYTE PullFlag;          ///< Pull request flags.
		BUFFERCHAIN SendChain;   ///< Buffer chain for send.
		BUFFERCHAIN RecvChain;   /**< Buffer chain for receive (used for UDP
		                              datagram CHAIN. */
		UBYTE TCP_Tries;         ///< Number of packet resend tries.
		UWORD TCP_StartTimer;    ///< Start timer for packet resending.
		BYTE CanAccepted;        /**< This field must be set only for listening
		                              sockets. */
		BYTE MaxBackLog;         ///< Maximum count of pending connections.
		BYTE BackLog;            ///< Current count of pending connections.
	} ICESOCKET;

public:
	HypCTcpIpHyp(HypCTransport *pt);
	virtual ~HypCTcpIpHyp();

	virtual SOCKET	socket(int, int, int);
	virtual int 	closesocket(SOCKET);
	virtual int 	bind(SOCKET, struct sockaddr *, int);
	virtual int 	connect(SOCKET, struct sockaddr *, int);
	virtual int	listen(SOCKET, int);	//< listen on specified socket
	virtual SOCKET	accept(SOCKET, struct sockaddr *, int *); //< accept incoming connection
	virtual int 	sendto(SOCKET, const char *, int, int, struct sockaddr *, int);
	virtual int 	send(SOCKET, const char *, int, int);
	virtual int 	recvfrom(SOCKET, char *, int, int, struct sockaddr *, int *);
	virtual int 	recv(SOCKET, char *, int, int);
	virtual int 	recv_to(SOCKET, char *, int, int, int);
	virtual int 	ioctlsocket(SOCKET, long, UDWORD *);
	virtual int		lasterror();

	virtual void	SetConfig (const HypCConfig &cfg)
	{
		m_LocalConfig = cfg;
	}

	virtual void	GetConfig(HypCConfig &param)
	{
		param = m_LocalConfig;
	}

	void SetHostIp(IPADDRESS OurIP);
	void SetMTU(size_t MTU);
	UDWORD GetHostIp(void);
	void TCPSetSubnetAndGatewayIP(IPADDRESS LclSubnetIP, IPADDRESS LclGatewayIP);
	void TCPGetSubnetAndGatewayIP(IPADDRESS *LclSubnetIP, IPADDRESS *LclGatewayIP);

	int setpush(SOCKET);
	int clearpush(SOCKET);

private:

	void	SocketsInit(void);
	void	deletesocket(ICESOCKET *);
	SOCKET	FindUsedAddress(UBYTE Protocol, struct sockaddr *Address);
	ICESOCKET	*FindSocketFor(UBYTE Protocol, IPADDRESS DestAdd, UWORD DestPort, IPADDRESS SourceAdd, UWORD SourcePort);
	int		sendEx(SOCKET s, const char *buf,int len, int flags);
	UWORD	FindFreePort(UBYTE Protocol);
	UBYTE	CheckConnected(void);
	int		getsockname(SOCKET Sock, struct sockaddr *Address, int NameLength);

	int		SetError(UWORD ErrorCode);
	UWORD	SetError_WSAENOTSOCK(void);
	UWORD	SetError_WSAENETDOWN(void);
	UWORD	SetError_WSAEWOULDBLOCK(void);
	UWORD	SetError_WSAENOTCONN(void);
	
	void	Unthread(ICESOCKET *DelSocket);
	int		checksocket(SOCKET s);

	// RAW stuff
	void IPProcess(ICESOCKET *is);
	size_t IPPullBytes(ICESOCKET *is);
	void IPIncoming(IPHEADER *IPPacket);

	// ICMP stuff
	void ICMPIncoming(IPHEADER *IHead);
	int ICMPProcIncoming(IPHEADER *IHead);
	
	// UDP stuff
	void UDPIncoming(IPHEADER *IPPacket);
	void UDPProcess(ICESOCKET *is);
	size_t UDPPullBytes(ICESOCKET *is);
	void UDPProcessSocketState(ICESOCKET *ISocket);
	void UDPCleanupSend(ICESOCKET *is);
	UWORD UDPSendData(ICESOCKET *is, UBYTE *Buffer, UWORD Length);

	// TCP stuff
	void tcpip_startjob(void);
	void tcpip_stopjob(void);
	static void TCPMainLoop(void);
	void RetryInit(ICESOCKET *is);
	void IncrementRetries(ICESOCKET *is);
	void IncRetries(ICESOCKET *is);
	void TCPProcess(ICESOCKET *is);
	void TCPProcessSocketState(ICESOCKET *IS);
	void TCPIncoming(IPHEADER *Ip);
	int TCPSendData(ICESOCKET *IS);
	void TCPCleanupSend(ICESOCKET *is);
	UWORD TCPPullBytes(ICESOCKET *is);
	
	// IP stuff
	void IPProcessSocketState(ICESOCKET *IS);
	UBYTE IPSend(ICESOCKET *is, IPHEADER *ip);
	UWORD CalcIPChecksum(IPHEADER *IncomingIP, UWORD len);
	UWORD Checksum(UWORD *dp, UWORD length);
	int IpSendPacket(UBYTE *ptr, UWORD len);
	int IpProcesPacket(HypCBuffer *pb);
	UBYTE TransportConnected(void);

	// Buffer stuff
	UBYTE TCPAddBufferToChain(ICESOCKET *IS, TCPHEADER *Tcp, UWORD Len);
	void AppendMemberToChain(BUFFERCHAIN *Chain, CHAINMEMBER *cm, int Length);
	UBYTE* DeleteFirstBufferFromChain(BUFFERCHAIN *Chain );
	UBYTE* DeleteLastBufferFromChain(BUFFERCHAIN *Chain);

	// Wait for busy socket
	bool WaitOnBusySocket(ICESOCKET *is, DWORD timeout = 0);
	bool WaitRecvOnBusySocket(ICESOCKET *is, DWORD timeout = 0);
	bool WaitListenOnSocket(ICESOCKET* is);

private:
	/// transport pointer
	HypCTransport *m_pTransport;
	
	UWORD SocketCount;	///< Total number of sockets
	ICESOCKET *FirstSocket;	///< Pointer to our first socket
	static UWORD NextPort;	///< Used for dynamic port assignments
	DWORD TcpIpJob;		///< tcp/ip stack task id
	int LastError;		///< Last error code

	/// stack is busy sending
	int Sending;

	/// local subnet mask
	IPADDRESS SubnetMaskIP;
	/// local gateway ip address
	IPADDRESS GatewayIP;
	/// local ip address
	IPADDRESS MyIP;

	UWORD PacketCount;
	IPHEADER *PacketHeader;
	IPHEADER *PacketIP;
	UWORD Size;

	/// MTU of the link
	size_t m_MTU;

	/// FD mask
	static int m_fdlast;

	/// Map of the socket ID
	HypCMap m_Posix2Ice;
};

#endif	// _HYPCTCPIPHYP_HPP_
