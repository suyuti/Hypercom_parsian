//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2004
//=============================================================================

#ifndef __ssl_h
#define __ssl_h

#ifdef DOXYGEN
#define __attribute__(A)
#endif

#include "sockets.h"

#define	Align(x,y)	( ((x)+(~(-(y)))) & (-(y)) )

///	SSL Message Types
#define	ChgCipherSpec		20
#define	Alert				21
#define	Handshake			22
#define	ApplData			23

///	Hanshake Message Types
#define	HelloRequest		0		// Server Hello Request
#define	ClientHello			1		// Client Hello
#define	ServerHello			2		// Server Hello
#define	Certificate			11		// Server Certificate
#define	ServerKeyXchg		12		// Server Key Exchange
#define	CertificateReq		13		// Server Certificate Request
#define	ServerDone			14		// Server Done
#define	CertificateVerify	15		// Client Certificate Verify
#define	ClientKeyXchg		16		// Client Key Exchange
#define	Finished			20		// Either Finished

/**
	@defgroup GroupSSL SSL public interface
	@{
*/

/// SSL Alert levels
typedef enum
{
	SSL_ALERT_NONE      = 0,			///< No alert
	SSL_ALERT_WARNING   = 1,			///< Warning
	SSL_ALERT_FATAL     = 2,			///< Fatal
	SSL_ALERT_RESERVED  = 255			///< Reserved
} SSL_ALERT_LEVEL;

/// SSL Alert codes
typedef enum
{
	SSL_ALERT_CLOSE_NOTIFY              = 0,	///< Close notify 
	SSL_ALERT_UNEXPECTED_MESSAGE        = 10,	///< Unexpected message
	SSL_ALERT_BAD_RECORD_MAC            = 20,	///< Bad record MAC
	SSL_ALERT_DECOMPRESSION_FAILURE     = 30,	///< Decompression failure
	SSL_ALERT_HANDSHAKE_FAILURE         = 40,	///< Handshake failure
	SSL_ALERT_NO_CERTIFICATE            = 41,	///< No certificate
	SSL_ALERT_BAD_CERTIFICATE           = 42,	///< Bad certificate
	SSL_ALERT_UNSUPPORTED_CERTIFICATE   = 43,	///< Unsupported certificate
	SSL_ALERT_CERTIFICATE_REVOKED       = 44,	///< Certificate revoked
	SSL_ALERT_CERTIFICATE_EXPIRED       = 45,	///< Certificate expired
	SSL_ALERT_CERTIFICATE_UNKNOWN       = 46	///< Certificate unknown
} SSL_ALERT_CODE;

/// SSL alert structure
typedef struct
{
	SSL_ALERT_LEVEL level;	///< Alert level
	SSL_ALERT_CODE  code;	///< Alert code
} SSL_ALERT;

/// SSL retrn error codes
typedef enum
{
	IPERR_NONE = 0,			///< No error
	IPERR_SENDING,			///< SSL Sending error
	IPERR_RECEIVING,		///< SSL Receiving error
	IPERR_SESSION,			///< SSL Session error
	IPERR_SSL = 1000,		///< Common SSL error
	IPERR_SSLALERT,			///< SSL alert received
	IPERR_SSLFMT,			///< SSL Message format error
	IPERR_SSLHELLO,			///< SSL Hello message error
	IPERR_SSLSEND,			///< SSL Sending error
	IPERR_SSLRECV,			///< SSL Receiving error
	IPERR_SSLCERT = 1100,	///< SSL certificate error
	IPERR_TCP = 10000		///< Socket error
} IPERRTYPE;

/**
	@brief Init SSL
	@param s Connected socket to work with
	@param func Pointer to certificate check function
	@return SSL error code
	@note Should be called before any other SSL fuctions
*/
IPERRTYPE SSLInit(SOCKET s, int (*func)(void*));

/**
	@brief Initialize mutual authentication parameters
	@param pCertificate pointer to the client certificate
	@param nCertLen length of the client certificate
	@param pPrivateKey pointer to the RSA private key
	@param nPrivateKeyLen length of key
	@param nPrivateKeyFormat private key format
	@param nMasterKeyIndex master key index
	@return SSL error code
	@note Should be called before any other SSL fuctions and after SSLInit
*/
IPERRTYPE SSLInitMutual(BYTE *pCertificate, DWORD nCertLen, BYTE *pPrivateKey, DWORD nPrivateKeyLen, DWORD nPrivateKeyFormat, DWORD nMasterKeyIndex);

/**
	@brief Receive application data thru SSL
	@param pDest Destination pointer
	@param pcBufSz Destination buffer length
	@param iFlags Flags to pass to socket recv function
	@return SSL error code
	@note pcBufSz will contain the actual number of bytes read
*/
IPERRTYPE SSLrecv(char *pDest, int *pcBufSz, int iFlags);

/**
	@brief Send application data thru SSL
	@param pBuf Data pointer
	@param iLen Data length
	@param iFlags Flags to pass to socket send function
	@return SSL error code
*/
IPERRTYPE SSLsend(char *pBuf, int iLen, int iFlags);

/**
	@brief Get last SSL error
	@return Last stored internal error code
*/
IPERRTYPE SSLGetError(void);

/**
	@brief Get last server alert
	@param pAlert Pointer to destination
	@return IPERR_NONE
	@note Should be called after one of SSL functions returns IPERR_SSLALERT
*/
IPERRTYPE SSLGetLastAlert(SSL_ALERT *pAlert);

/**
	@brief Start SSL session
	@return SSL error code
	@note Perform handshake if not done yet
*/
IPERRTYPE SSLstartSession(void);

/**
	@brief Process received SSL message
	@param pBuf Pointer to destination buffer
	@param pLen Pointer to destination length
	@param LenRemain Length remaining
	@return SSL error code
	@note Decrypts data and updates hashes if necessary
*/
IPERRTYPE SSLRcvRec(UBYTE *pBuf, UWORD *pLen, UWORD LenRemain);

/**
	@brief Set SSL internal error code
	@param Code Error code to set
	@return IPERR_SSL
*/
IPERRTYPE SSLSetError(UDWORD Code);

extern SOCKET ssls;
extern int (*check_auth)(void*);

/**
	@}
*/

#endif // __ssl_h
