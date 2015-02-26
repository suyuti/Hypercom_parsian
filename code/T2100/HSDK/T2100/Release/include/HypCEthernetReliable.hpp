//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================
#ifndef _HYPCETHERNETRELIABLE_HPP_
#define _HYPCETHERNETRELIABLE_HPP_

#include <eftos.h>
#include <hyctypes.h>
#include "HypCSocketsCommon.hpp"
#include "HypCEthernetCommon.hpp"
#include "HypCBufferLink.hpp"

/**
	@addtogroup HPOSFC
	@{
*/

/**
	@brief Sockets interface over HEMS in reliable mode.
*/
class HypCEthernetReliable : public HypCSocketsCommon, public HypCEthernetCommon
{
protected:
	/**
		@brief State of the socket.
	*/
	enum HypEEthernetReliableState
	{
		STATE_UNUSED,           ///< Unused.
		STATE_IDLE,             ///< Idle.
		STATE_CONNECTING,       ///< Connecting.
		STATE_CONNECTED,        ///< Connected.
		STATE_REMOTELY_TERMINATED, ///< Remotely terminated.
		STATE_DISCONNECTING     ///< Disconnecting.
	};

	/**
		@brief Socket description.
	*/
	struct HemsReliableSocketDB
	{
		/**
			@brief Constructor.

			Initializes socket in unused state.
		*/
		HemsReliableSocketDB();

		/**
			@brief Destructor.

			Clean up the internal data.
		*/
		~HemsReliableSocketDB();

		/// Ethernet module socket ID.
		UWORD emSocket;

		/// Linked list of receive buffers.
		HypCBufferLink *recvBuf;

		/// Event associated with the socket.
		HypCEvent event;

		/// Socket state.
		HypEEthernetReliableState state;

		/// Connect time.
		int connectTime;

		/// Host timeout.
		int hostTimeout;
	};

public:
	/**
		@brief Constructor.

		@param transport Lower layer transport.
		@param nDeviceID Not used. Optional, defaults to 0.

		@note Calls parent constructor

		@see HypCEthernetCommon()
	*/
	HypCEthernetReliable( HypCTransport *transport, int nDeviceID = 0 );

	/**
		@brief Destructor.
	*/
	virtual ~HypCEthernetReliable();


	/**
		@brief Create socket.

		@param af Specifies the communications domain in which a socket
		          is to be created.
		@param type Specifies the type of socket to be created.
		@param protocol Specifies a particular protocol to be used with
		                the socket. Specifying a protocol of 0 causes
		                socket() to use an unspecified default protocol
		                appropriate for the requested socket type.

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
		@param addr Points to a sockaddr structure containing the
		            address to be bound to the socket. The length and
		            format of the address depend on the address family
		            of the socket.
		@param namelen Specifies the length of the sockaddr structure
		               pointed to by the address argument.

		@return SOCKET_ERROR if socket subsytem in not initialized.
	*/
	int bind( SOCKET s, struct sockaddr *addr, int namelen );

	/**
		@brief Connect socket.

		@param s Specifies the descriptor associated with the socket.
		@param addr Points to a sockaddr structure containing the peer
		            address. The length and format of the address
		            depend on the address family of the socket.
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
		@param tolen Specifies the length of the sockaddr structure
		             pointed to by the address argument.

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
		@brief Set configuration options.

		@param cfg Configuration options for Ethernet module.
	*/
	virtual void SetConfig( const HypCConfig &cfg );

	/** 
		@brief Get configuration options.

		@param cfg Configuration from Ethernet module.
	*/
	virtual void GetConfig( HypCConfig &cfg );

protected:
	/**
        	@brief Executed before disconnecting transport.
	*/
	virtual void BeforeDisconnectTransport();

	/**
		@brief Build HEMS transaction frame.
	*/
	virtual void BuildFrame();

	/**
		@brief Process application frame received from module.

		@return NOTHING_RECEIVED
	*/
	virtual TransparentCommonEvents ParseApplicationFrame();

	/**
		@brief Process service frame received from module.

		@return NOTHING_RECEIVED.
	*/
	virtual TransparentCommonEvents ParseServiceFrame();

	/**
		@brief Process service frame received from module.

		@param tag Tag value.
		@param tagsize Tag size.
		@param posn Position in buffer.

		@return true: Parsed OK.<br>
		        false: Tag is not recognized.
	*/
	virtual bool TagParser( HypEHemsTags tag, size_t tagsize, size_t posn );

	/**
		@brief Get Ethernet module mode.

		@return HEMS_MODE_RELIABLE.
	*/
	virtual uint8_t EthernetMode();

	/**
		@brief Sets the last error for the current process.

		@param ErrorCode Error code to set as last error.
	*/
	int SetSocketsError( int ErrorCode );

protected:
	/// Database for sockets.
	HemsReliableSocketDB m_SocketDB[ HEMS_MAX_SOCKETS ];

	/// Last error happened in socket interface.
	int m_LastError;

	/// Last index parsed from HEMS message (valid only during parsing).
	int m_LastIndex;

	/// Last socket no
	static SOCKET m_LastSocketNo;
};

/**
	@}
*/

#endif //_HYPCETHERNETRELIABLE_HPP_
