//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================

#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include <netinet/in.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
	@defgroup GroupSockets Sockets public interface API.
	@{

	This layer provides sockets API for IP similar to Berkley sockets.
*/

#ifdef DOXYGEN
#define __attribute__(A)
#endif

/**
	@brief IP protocols
*/
enum IPPROTOS
{
	IPPROTO_IP = 0,         ///< Dummy for IP.
	IPPROTO_ICMP = 1,       ///< Control message protocol.
	IPPROTO_TCP = 6,        ///< TCP protocol.
	IPPROTO_UDP = 17,       ///< User datagram protocol.
	IPPROTO_RAW = 255,      ///< Raw IP packet.
	IPPROTO_MAX = 256       ///< Maximum protocol number.
};

/**
	@brief Port numbers: network standard functions
*/
enum IPPORTS
{
	IPPORT_ECHO = 7,        ///< Echo service.
	IPPORT_DISCARD = 9,     ///< Discard transmissions service.
	IPPORT_SYSTAT = 11,     ///< System status service.
	IPPORT_DAYTIME = 13,    ///< Time of day service.
	IPPORT_NETSTAT = 15,    ///< Network status service.
	IPPORT_FTP = 21,        ///< File transfer protocol.
	IPPORT_TELNET = 23,     ///< Telnet protocol.
	IPPORT_SMTP = 25,       ///< Simple mail transfer Protocol.
	IPPORT_TIMESERVER = 37, ///< Timeserver service.
	IPPORT_NAMESERVER = 42, ///< Domain name service.
	IPPORT_WHOIS = 43,      ///< Internet whois service.
	IPPORT_MTP = 57,        ///< Transcoder/Message Transfer Part (deprecated).
	IPPORT_TFTP = 69,       ///< Trivial file transfer protocol.
	IPPORT_RESERVED = 1024  /**< Ports @< IPPORT_RESERVED are reserved for
	                             privileged processes (e.g. root). */
};

/**
	@brief Link numbers
*/
enum IMPLINKS
{
	IMPLINK_IP = 155,
	IMPLINK_LOWEXPER = 156,
	IMPLINK_HIGHEXPER = 158
};

/*
 * Definitions of bits in internet address integers.
 * On subnets, the decomposition of addresses to host and net parts
 * is done according to subnet mask, not the masks here.
 */
#define IN_CLASSA(i)            (((long)(i) & 0x80000000) == 0)
#define IN_CLASSA_NET           0xff000000
#define IN_CLASSA_NSHIFT        24
#define IN_CLASSA_HOST          0x00ffffff
#define IN_CLASSA_MAX           128

#define IN_CLASSB(i)            (((long)(i) & 0xc0000000) == 0x80000000)
#define IN_CLASSB_NET           0xffff0000
#define IN_CLASSB_NSHIFT        16
#define IN_CLASSB_HOST          0x0000ffff
#define IN_CLASSB_MAX           65536

#define IN_CLASSC(i)            (((long)(i) & 0xc0000000) == 0xc0000000)
#define IN_CLASSC_NET           0xffffff00
#define IN_CLASSC_NSHIFT        8
#define IN_CLASSC_HOST          0x000000ff

#define INADDR_ANY              (u_long)0x00000000
#define INADDR_LOOPBACK         0x7f000001
#define INADDR_BROADCAST        (u_long)0xffffffff    
#define INADDR_NONE             0xffffffff

/**
	@brief Options for use with [gs]etsockopt at the IP level.
*/
enum IPOPTIONS
{
	IP_OPTIONS = 1          ///< Set/get IP per-packet options
};

/// Invalid socket return value.
#define	INVALID_SOCKET (SOCKET)(~0)

/// Operation on socket failed.
#define	SOCKET_ERROR (-1)

/**
	@brief Socket types.
*/
enum SOCK_TYPES
{
	SOCK_STREAM = 1,        ///< Stream socket.
	SOCK_DGRAM = 2,         ///< Datagram socket.
	SOCK_RAW = 3,           ///< Raw-protocol interface.
	SOCK_RDM = 4,           ///< Reliably-delivered message.
	SOCK_SEQPACKET = 5      ///< Sequenced packet stream.
};

/**
	@brief Option flags per-socket.
*/
enum SO_FLAGS
{
	SO_DEBUG = 0x0001,      ///< Turn on debugging info recording.
	SO_ACCEPTCONN = 0x0002, ///< Socket has had listen().
	SO_REUSEADDR = 0x0004,  ///< Allow local address reuse.
	SO_KEEPALIVE = 0x0008,  ///< Keep connections alive.
	SO_DONTROUTE = 0x0010,  ///< Just use interface addresses.
	SO_BROADCAST = 0x0020,  ///< Permit sending of broadcast msgs.
	SO_USELOOPBACK = 0x0040, ///< Bypass hardware when possible.
	SO_LINGER = 0x0080,     ///< Linger on close if data present.
	SO_DONTLINGER = (unsigned int)(~SO_LINGER), ///< Do not linger on close if data present.
	SO_OOBINLINE = 0x0100,  ///< Leave received OOB data in line.
	SO_SNDBUF = 0x1001,     ///< Send buffer size.
	SO_RCVBUF = 0x1002,     ///< Receive buffer size.
	SO_SNDLOWAT = 0x1003,   ///< Send low-water mark.
	SO_RCVLOWAT = 0x1004,   ///< Receive low-water mark.
	SO_SNDTIMEO = 0x1005,   ///< Send timeout.
	SO_RCVTIMEO = 0x1006,   ///< Receive timeout.
	SO_ERROR = 0x1007,      ///< Get error status and clear.
	SO_TYPE = 0x1008        ///< Get socket type.
};

/// TCP options.
#define TCP_NODELAY     0x0001

/**
	@brief Address families.
*/
enum AF_TYPES
{
	AF_UNSPEC = 0,          ///< Unspecified.
	AF_UNIX = 1,            ///< Local to host (pipes, portals).
	AF_INET = 2,            ///< Internetwork: UDP, TCP, etc.
	AF_IMPLINK = 3,         ///< ARPANet IMP addresses.
	AF_PUP = 4,             ///< PUP protocols: e.g. BSP.
	AF_CHAOS = 5,           ///< MIT CHAOS protocols.
	AF_NS = 6,              ///< XEROX NS protocols.
	AF_ISO = 7,             ///< ISO protocols.
	AF_OSI = AF_ISO,        ///< OSI is ISO.
	AF_ECMA = 8,            ///< European computer manufacturers.
	AF_DATAKIT = 9,         ///< Datakit protocols.
	AF_CCITT = 10,          ///< CCITT protocols, X.25 etc.
	AF_SNA = 11,            ///< IBM SNA.
	AF_DECnet = 12,         ///< DECnet.
	AF_DLI = 13,            ///< Direct data link interface.
	AF_LAT = 14,            ///< LAT.
	AF_HYLINK = 15,         ///< NSC Hyperchannel.
	AF_APPLETALK = 16,      ///< AppleTalk.
	AF_NETBIOS = 17,        ///< NetBios-style addresses.
	AF_MAX = 18             ///< Maximum number.
};

/**
	@brief Protocol families, same as address families for now.
*/
enum PF_TYPES
{
	PF_UNSPEC = AF_UNSPEC,  ///< Unspecified.
	PF_UNIX = AF_UNIX,      ///< Local to host (pipes, portals).
	PF_INET = AF_INET,      ///< Internetwork: UDP, TCP, etc.
	PF_IMPLINK = AF_IMPLINK,///< ARPANet IMP addresses.
	PF_PUP = AF_PUP,        ///< PUP protocols: e.g. BSP.
	PF_CHAOS = AF_CHAOS,    ///< MIT CHAOS protocols.
	PF_NS = AF_NS,          ///< XEROX NS protocols.
	PF_ISO = AF_ISO,        ///< ISO protocols.
	PF_OSI = AF_OSI,        ///< OSI is ISO.
	PF_ECMA = AF_ECMA,      ///< European computer manufacturers.
	PF_DATAKIT = AF_DATAKIT,///< Datakit protocols.
	PF_CCITT = AF_CCITT,    ///< CCITT protocols, X.25 etc.
	PF_SNA = AF_SNA,        ///< IBM SNA.
	PF_DECnet = AF_DECnet,  ///< DECnet.
	PF_DLI = AF_DLI,        ///< Direct data link interface.
	PF_LAT = AF_LAT,        ///< LAT.
	PF_HYLINK = AF_HYLINK,  ///< NSC Hyperchannel.
	PF_APPLETALK = AF_APPLETALK, ///< AppleTalk.
	PF_MAX = AF_MAX         ///< NetBios-style addresses.
};                              ///< Maximum number.

/**
	@brief Structure used for manipulating linger option.
*/
struct linger
{
	UWORD l_onoff;          ///< Option on/off.
	UWORD l_linger;         ///< Linger time.
};

/// Level number for (get/set)sockopt() to apply to socket itself.
#define SOL_SOCKET 0xffff

/// Maximum queue length specifiable by listen.
#define SOMAXCONN 5

/**
	@brief Flags to be used on read and write opeations on socket.
*/
enum MSG_FLAGS
{
	MSG_OOB = 0x1,          ///< Process out-of-band data.
	MSG_PEEK = 0x2,         ///< Peek at incoming message.
	MSG_DONTROUTE = 0x4,    ///< Send without using routing tables.
	MSG_MAXIOVLEN = 0x10    ///< Option level for socket level options.
};

/// Constant based on rfc883, used by gethostbyxxxx() calls.
#define MAXGETHOSTSTRUCT        1024

/**
	@brief Flags to be used with the WSAAsyncSelect() call.
*/
enum FD_FLAGS
{
	FD_READ = 0x01,         /**< Set to receive notification of readiness
	                             for reading. */
	FD_WRITE = 0x02,        /**< Wants to receive notification of readiness
	                             for writing. */
	FD_OOB = 0x04,          /**< Wants to receive notification of the
	                             arrival of OOB data. */
	FD_ACCEPT = 0x08,       /**< Wants to receive notification of incoming
	                             connections. */
	FD_CONNECT = 0x10,      /**< Wants to receive notification of completed
	                             connection or multipoint join operation. */
	FD_CLOSE = 0x20         /**< Wants to receive notification of socket
	                             closure. */
};

/**
	@brief ioctlsocket() commands.
*/
enum IOCTL_COMMANDS
{
	FIONBIO = 27,           /**< Use with a nonzero argp parameter to
	                             enable the nonblocking mode of socket.
	                             The argp parameter is zero if nonblocking
	                             is to be disabled. The argp parameter
	                             points to an unsigned long value. When a
	                             socket is created, it operates in blocking
	                             mode by default (nonblocking mode is
	                             disabled). */
	FIONREAD = 37,          /**< Use to determine the amount of data
	                             pending in the network's input buffer that
	                             can be read from socket. The argp
	                             parameter points to an unsigned long value
	                             in which ioctlsocket stores the result.
	                             If s is stream oriented (for example, type
	                             SOCK_STREAM), FIONREAD returns the amount
	                             of data that can be read in a single call
	                             to the recv function; this might not be
	                             the same as the total amount of data
	                             queued on the socket. If s is message
	                             oriented (for example, type SOCK_DGRAM),
	                             FIONREAD returns the size of the first
	                             datagram (message) queued on the socket. */
	SIOCATMARK = 47,        /**< Informs you, as you receive data from the
	                             stream, when the stream pointer has
	                             reached the out-of-band pointer. Always
	                             return TRUE in this implementation. */
	SIOPUSH = 57,		/**< Set PUSH flag on the outgoing packets if
	                             the argp parameter points to non-zero or
	                             send them withot PUSH if it zero. */
	SIOMYIP = 67,           /**< Specify local IP address to IP stack. */
	SIOTCPTRIES = 77,	/**< Specify TCP packet resend counter. */
	SIOTCPSTARTTIMER = 87,	/**< Specify initial delay on TCP packets
	                             retransmission (in hundreds of ms). */
	SIOIFSOCKET = 100       ///< Verify if socket is allocated.
};

/**
	@brief Sockets errors.
*/
enum SOCKETS_ERRORS
{
	WSABASEERR = 10000,     ///< All Windows Sockets error constants are biased by WSABASEERR from the "normal".
	WSAEINTR = (WSABASEERR+4), ///< Interrupted function call.
	WSAEBADF = (WSABASEERR+9), ///< The file handle supplied is not valid.
	WSAEACCES = (WSABASEERR+13), ///< Permission denied.
	WSAEFAULT = (WSABASEERR+14), ///< Bad address.
	WSAEINVAL = (WSABASEERR+22), ///< Invalid argument.
	WSAEMFILE = (WSABASEERR+24), ///< Too many open files.
	WSAEWOULDBLOCK = (WSABASEERR+35), ///< A non-blocking socket operation could not be completed immediately.
	WSAEINPROGRESS = (WSABASEERR+36), //< A blocking operation is currently executing.
	WSAEALREADY = (WSABASEERR+37), ///< An operation was attempted on a non-blocking socket that already had an operation in progress.
	WSAENOTSOCK = (WSABASEERR+38), ///< An operation was attempted on something that is not a socket.
	WSAEDESTADDRREQ = (WSABASEERR+39), ///< A required address was omitted from an operation on a socket.
	WSAEMSGSIZE = (WSABASEERR+40), ///< A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself.
	WSAEPROTOTYPE = (WSABASEERR+41), ///< A protocol was specified in the socket function call that does not support the semantics of the socket type requested.
	WSAENOPROTOOPT = (WSABASEERR+42), ///< An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call.
	WSAEPROTONOSUPPORT = (WSABASEERR+43), ///< The requested protocol has not been configured into the system, or no implementation for it exists.
	WSAESOCKTNOSUPPORT = (WSABASEERR+44), ///< The support for the specified socket type does not exist in this address family.
	WSAEOPNOTSUPP = (WSABASEERR+45), ///< The attempted operation is not supported for the type of object referenced.
	WSAEPFNOSUPPORT = (WSABASEERR+46), ///< The protocol family has not been configured into the system or no implementation for it exists.
	WSAEAFNOSUPPORT = (WSABASEERR+47), ///< An address incompatible with the requested protocol was used.
	WSAEADDRINUSE = (WSABASEERR+48), ///< Only one usage of each socket address (protocol/network address/port) is normally permitted.
	WSAEADDRNOTAVAIL = (WSABASEERR+49), ///< The requested address is not valid in its context.
	WSAENETDOWN = (WSABASEERR+50), ///< A socket operation encountered a dead network.
	WSAENETUNREACH = (WSABASEERR+51), ///< A socket operation was attempted to an unreachable network.
	WSAENETRESET = (WSABASEERR+52), ///< The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.
	WSAECONNABORTED = (WSABASEERR+53), ///< An established connection was aborted by the software in your host machine.
	WSAECONNRESET = (WSABASEERR+54), ///< An existing connection was forcibly closed by the remote host.
	WSAENOBUFS = (WSABASEERR+55), ///< An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.
	WSAEISCONN = (WSABASEERR+56), ///< A connect request was made on an already connected socket.
	WSAENOTCONN = (WSABASEERR+57), ///< A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.
	WSAESHUTDOWN = (WSABASEERR+58), ///< A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call.
	WSAETOOMANYREFS = (WSABASEERR+59), ///< Too many references to some kernel object.
	WSAETIMEDOUT = (WSABASEERR+60), ///< A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond.
	WSAECONNREFUSED = (WSABASEERR+61), ///< No connection could be made because the target machine actively refused it.
	WSAELOOP = (WSABASEERR+62), ///< Cannot translate name.
	WSAENAMETOOLONG = (WSABASEERR+63), ///< Name component or name was too long.
	WSAEHOSTDOWN = (WSABASEERR+64), ///< A socket operation failed because the destination host was down.
	WSAEHOSTUNREACH = (WSABASEERR+65), ///< A socket operation was attempted to an unreachable host.
	WSAENOTEMPTY = (WSABASEERR+66), ///< Cannot remove a directory that is not empty.
	WSAEPROCLIM = (WSABASEERR+67), ///< A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously.
	WSAEUSERS = (WSABASEERR+68), ///< Ran out of quota.
	WSAEDQUOT = (WSABASEERR+69), ///< Ran out of disk quota.
	WSAESTALE = (WSABASEERR+70), ///< File handle reference is no longer available.
	WSAEREMOTE = (WSABASEERR+71), ///< Item is not available locally.
	WSASYSNOTREADY = (WSABASEERR+91), ///< WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable.
	WSAVERNOTSUPPORTED = (WSABASEERR+92), ///< The Windows Sockets version requested is not supported.
	WSANOTINITIALISED = (WSABASEERR+93), ///< Either the application has not called WSAStartup, or WSAStartup failed.
	WSAHOST_NOT_FOUND = (WSABASEERR+1001), ///< Authoritative Answer: Host not found.
	WSATRY_AGAIN = (WSABASEERR+1002), ///< Non-Authoritative: Host not found, or SERVERFAIL.
	WSANO_RECOVERY = (WSABASEERR+1003), ///< Non recoverable errors, FORMERR, REFUSED, NOTIMP.
	WSANO_DATA = (WSABASEERR+1004), ///< Valid name, no data record of requested type.
	WSANO_ADDRESS = WSANO_DATA, ///< No address, look for MX record.
	WSAECONNCLOSED = (WSABASEERR+2001), ///< Connection closed.
	WSAECONNCLOSEDEX = (WSABASEERR+2002), ///< Connection closed by remote host.
	WSAESENDNOTACKED = (WSABASEERR+2003), ///< The frame was not acknowledged within retry timeout.
};

/// Socket type definition.
typedef UDWORD SOCKET;

/**
	@brief Internet address.
*/
struct in_addr
{
	union
	{
		struct { UBYTE s_b1, s_b2, s_b3, s_b4; } S_un_b; ///< Split in bytes
		struct { UWORD s_w1, s_w2; } S_un_w; ///< Split in words
		in_addr_t S_addr; ///< Single dword
	} S_un;

	#define s_addr  S_un.S_addr		// can be used for most tcp & ip code 
	#define s_net   S_un.S_un_b.s_b1	// network 
	#define s_host  S_un.S_un_b.s_b2	// host on imp 
	#define s_lh    S_un.S_un_b.s_b3	// logical host 
	#define s_impno S_un.S_un_b.s_b4	// imp number
	#define s_imp   S_un.S_un_w.s_w2	// imp 
	
	#define s_ad_b1 S_un.S_un_b.s_b1
	#define s_ad_b2 S_un.S_un_b.s_b2
	#define s_ad_b3 S_un.S_un_b.s_b3
	#define s_ad_b4 S_un.S_un_b.s_b4
};

/**
	@brief Combined IP address and PORT structure
*/
typedef struct
{
	UWORD Port;             ///< Port number
	struct in_addr add;     ///< Internet address
} IPPORT;

/**
	@brief Socket address, internet style.
*/
struct sockaddr_in
{
	short sin_family;       ///< Address family.
	UWORD sin_port;         ///< Port number.
	struct in_addr sin_addr; ///< Internet address.
	char sin_zero[8];       ///< Filler.
};

/// Winsock compatible typedef for sockaddr_in.
typedef struct sockaddr_in SOCKADDR_IN;

/**
	@brief Structure used by kernel to store most addresses.
*/
struct sockaddr
{
	short sa_family;        ///< Address family.
	char sa_data[14];       ///< Up to 14 bytes of direct address.
};

/// Winsock compatible typedef for sockaddr.
typedef struct sockaddr SOCKADDR;

/// Winsock compatible typedef for pointer to sockaddr.
typedef struct sockaddr *PSOCKADDR;

/// Winsock compatible typedef for pointer to sockaddr.
typedef struct sockaddr *LPSOCKADDR;

/**
	@brief Structure used to pass protocol information in raw sockets.
*/
struct sockproto
{
	UWORD sp_family;        ///< address family
	UWORD sp_protocol;      ///< protocol
};

/**
	@brief Converts an IP address in a string to a in_addr_t in network
               order.

	@param text String representation of IP address.

	@return Binary representation of IP address or (in_addr_t)-1 on error.
*/
in_addr_t inet_addr( const char *text );

/// Alias for inet_addr for backward compatibility.
#define inet_addrs(A) ((inet_addr(A) == (in_addr_t)-1) ? 0 : inet_addr(A))

/**
	@brief Converts an IP address in a DWORD in network order to string.

	@param in Input address to convert.

	@return Pointer to the static buffer that holds resulting string.
*/
char *inet_ntoa( struct in_addr in );

/**
	@brief Convert unsigned long integer to string.

	@param value The integer to be converted to string representation.
	@param string Pointer to the buffer that is to hold resulting string.
	@param radix Is the base of the number, a value between 2 and 36.

	@return Pointer to string.
*/
char *ultoa( unsigned long int value, char *string, int radix );

/**
	@brief Check if text is IP address.

	@return Nonzero if text is IP address or zero if it is not.
*/
int isaddr( const char *text );

/**
	@brief Convert string to IP address.

	Converts string in form "[a.b.c.d]" or "a.b.c.d" to DWORD.

	@param text String representation of IP address.

	@return Binary representation of IP address or zero on error.
*/
DWORD aton( const char *text );

/**
	@brief Create socket.

	@param af Specifies the communications domain in which a socket is to
	          be created.
	@param type Specifies the type of socket to be created.
	@param protocol Specifies a particular protocol to be used with the
	                socket. Specifying a protocol of 0 causes socket() to
	                use an unspecified default protocol appropriate for the
	                requested socket type.

	@return Socket descriptor or INVALID_SOCKET on error.
*/
SOCKET socket( int af, int type, int protocol );

/**
	@brief Close socket.

	Closes connection and destroys socket.

	@param s Socket previously opened with socket()

	@return SOCKET_ERROR if socket subsytem in not initialized
*/
int closesocket( SOCKET s );

/**
	@brief Bind socket.

	Set local IP address.

	@param s Specifies the descriptor of the socket to be bound.
	@param addr Points to a sockaddr structure containing the address to be
	            bound to the socket. The length and format of the address
	            depend on the address family of the socket.
	@param namelen Specifies the length of the sockaddr structure pointed
	               to by the address argument.

	@return SOCKET_ERROR if socket subsytem in not initialized.
*/
int bind( SOCKET s, struct sockaddr *addr, int namelen );

/**
	@brief Connect socket.

	@param s Specifies the descriptor associated with the socket.
	@param addr Points to a sockaddr structure containing the peer address.
	            The length and format of the address depend on the address
	            family of the socket.
	@param namelen Specifies the length of the sockaddr structure pointed
	               to by the address argument.

	@return SOCKET_ERROR if socket subsytem in not initialized.
*/
int connect( SOCKET s, struct sockaddr *addr, int namelen );

/**
	@brief Send data to socket (UDP).

	@param s Specifies the descriptor associated with the socket.
	@param buf Pointer to source buffer.
	@param len Length of data to send.
	@param flags Operation flags.
	@param to Pointer to the address of receiver.
	@param tolen Specifies the length of the sockaddr structure pointed to
	             by the address argument.

	@return Number of sent bytes or SOCKET_ERROR on error.
*/
int sendto( SOCKET s, const char *buf, int len, int flags, struct sockaddr *to, int tolen );

/**
	@brief Send data to socket.

	@param s Specifies the descriptor associated with the socket.
	@param buf Pointer to source buffer
	@param len Length of data to send
	@param flags Operation flags

	@return Number of sent bytes or SOCKET_ERROR on error.
*/
int send( SOCKET s, const char *buf, int len, int flags );

/**
	@brief Receive data from socket (UDP).

	@param s Specifies the descriptor associated with the socket.
	@param buf Buffer for the incoming data.
	@param len Length of buf, in bytes.
	@param flags Indicator specifying the way in which the call is made.
	@param from Optional pointer to a buffer in a sockaddr structure that
	            will hold the source address upon return.
	@param fromlen Optional pointer to the size, in bytes, of the from
	               buffer.

	@return Number of received bytes or SOCKET_ERROR on error.
*/
int recvfrom( SOCKET s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen );

/**
	@brief Receive data from socket

	@param s Specifies the descriptor associated with the socket.
	@param buf The buffer for incoming data.
	@param len The length, in bytes, of buf.
	@param flags Pointer to flags.

	@return Number of received bytes or SOCKET_ERROR on error.
*/
int recv( SOCKET s, char *buf, int len, int flags );

/**
	@brief Receive with timeout

	@param s Socket
	@param buf Pointer to destination buffer
	@param len Length of destination buffer
	@param flags Operation flags
	@param to Timeout in ms

	@return Number of received bytes or SOCKET_ERROR on error.
*/
int recv_to( SOCKET s, char *buf, int len, int flags, int to );

/**
	@brief Different socket operations

	@param s Descriptor identifying a socket.
	@param cmd Command to perform on the socket.
	@param argp Pointer to a parameter for cmd.

	@return On successful returns zero. Otherwise, a value of SOCKET_ERROR
	        is returned, and a specific error code can be retrieved by
	        calling lasterror().
*/
int ioctlsocket( SOCKET s, long cmd, UDWORD *argp );

/**
	@brief Listen socket.

	Listen on specified socket incoming connections.

	@param s Specifies the descriptor of the socket to be marked as
	         accepting connections.
	@param backlog Limit of the number of outstanding connections in the
	               socket's listen queue.

	@return SOCKET_ERROR if socket subsytem in not initialized.
*/
int listen( SOCKET s, int backlog );

/**
	@brief Accept socket connection.

	Accept incoming connection and return a new socket for this connection.

	@param s Specifies a socket that was created with socket(), has been
	         bound to an address with bind(), and has issued a successful
	         call to listen().
	@param addr Either a null pointer, or a pointer to a sockaddr structure
	            where the address of the connecting socket will be returned.
	@param addrlen Points to a socklen_t which on input specifies the
	               length. of the supplied sockaddr structure, and on
	               output specifies the length of the stored address.

	@return INVALID_SOCKET if socket subsytem in not initialized.
*/
SOCKET accept( SOCKET s, struct sockaddr *addr, int* addrlen );

/**
	@brief Get last error code

	@return Last socket subsystem error or WSAENOTSOCK if socket subsytem
	        in not initialized.
*/
int lasterror( void );

/**
	@brief Synchronous I/O multiplexing.

	This function indicates which of the specified sockets is ready for
	reading, ready for writing, or has an error condition pending. If the
	specified condition is false for all of the specified sockets, select()
	blocks, up to the specified timeout interval, until the specified
	condition is true for at least one of the specified sockets.

	@param nfds Specifies the range of sockets to be tested. The select()
	            function tests sockets in the range of 0 to nfds-1.
	@param readfds If not a null pointer, it points to an object of type
	              fd_set that on input specifies the sockets to be checked
	              for being ready to read, and on output indicates which
	              sockets are ready to read.
	@param writefds If not a null pointer, it points to an object of type
	               fd_set that on input specifies the sockets to be checked
	               for being ready to write, and on output indicates which
	               sockets are ready to write.
     	@param exceptfds If not a null pointer, it points to an object of type
	                 fd_set that on input specifies the sockets to be
	                 checked for error conditions pending, and on output
	                 indicates which sockets have error conditions pending.
	@param timeout If not a null pointer, it points to an object of type
	               struct timeval that specifies a maximum interval to wait
	               for the selection to complete. If the timeout argument
	               points to an object of type struct timeval whose members
	               are 0, select() does not block. If the timeout argument
	               is a null pointer, select() blocks until an event causes
	               one of the masks to be returned with a valid (non-zero)
	               value. If the time limit expires before any event occurs
	               that would cause one of the masks to be set to a
	               non-zero value, select() completes successfully and
	               returns 0.

	On successful completion, the objects pointed to by the readfds,
	writefds, and exeptfds arguments are modified to indicate which sockets
	are ready for reading, ready for writing, or have an error condition
	pending, respectively.

	For each socket less than nfds, the corresponding bit will be set on
	successful completion if it was set on input and the associated
	condition is true for that socket.

	If the readfds, writefds, and errorfds arguments are all null pointers
	and the timeout argument is not a null pointer, select() blocks for the
	time specified. If the readfds, writefds, and exeptfds arguments are
	all null pointers and the timeout argument is a null pointer, select()
	returns -1.

	Sockets associated with regular files always select true for ready to
	read, ready to write, and error conditions.

	On failure, the objects pointed to by the readfds, writefds, and
	exeptfds arguments are not modified. If the timeout interval expires
	without the specified condition being true for any of the specified
	sockets, the objects pointed to by the readfds, writefds, and exeptfds
	arguments have all bits set to 0.

	@return Returns the total number of bits set in the bit masks on
	        success, and -1 on error.

	@note This is very limited implementation. It does not handle writefds
	      parameter at all and not set errno or last error. Also, it does
	      not implemented properly for listen sockets.
*/
int select( int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout );

#ifdef __cplusplus
}
#endif

#if defined(__cplusplus) || defined(DOXYGEN)

/// Forward reference to common sockets class.
class HypCSocketsCommon;

/**
	@brief Initialize sockets subsystem.

	@param ps Pointer to HypCSocketsCommon derived object.

	@note This object will be used for all socket operations.
*/
void sockets_init( HypCSocketsCommon *ps );

/**
	@brief Shutdown socket subsystem.

	@note Future socket operations will be impossible.
*/
void sockets_shutdown( void );

#endif

/**
	@}
*/

#endif	// _SOCKETS_H_
