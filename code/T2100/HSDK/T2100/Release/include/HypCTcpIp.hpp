//=============================================================================
//                         Hypercom Inc
//             Hypercom POS Foundation Classes HPOSFC
//                      (c) Copyright 2003
//=============================================================================
#ifndef _HYPCTCPIP_HPP_
#define _HYPCTCPIP_HPP_

#include <eftos.h>
#include <hyctypes.h>
#include "sockets.h"
#include "HypCTransparentCommon.hpp"
#include "HypCSocketsCommon.hpp"
#include "HypCTimer.hpp"


//=============================================================================
//
// HypCTcpIp
//
// Base Class:
//      HypCTransparentCommon
//
// Overview:
//	This is wraper class over HEMS lib. Provides HypCTransport interface for
//	 HEMS library
//FIXME:
//  ConnectReq return following status:
//		HTECOMM if hems lib cannot  connect
//		HTECANCELIO if DicconectReq has been performed during connect
//			(during connecting modem phase only)
//
// ReadReq return following status:
//		HTECOMM if underlaing lost communications (lost carrier)
//		HTECANCELIO if DicconectReq has been performed during io
//		HTENOERROR if read operation has been performed successfully
//
// WriteReq return following status:
//		HTECOMM if hems lib cannot send packet
//		HTENOERROR if io-operation has been performed successfully
// Supported config parameters:
//	Parameters for setup tcp connection:
//		Ip1
//		Port1
//		Timeout1
//		Ip2
//		Port2
//		Timeout2
//	PPP parameters:
//		HC_PPPMode - ppp mode
//		HC_PPPUsername - ppp username.
//		HC_PPPPassword - ppp pasword
//		HC_PPPLCPTimeout - ppp lcp timeout
//		HC_PPPConnectionTimeout - ppp connection timeout
//
// Related Documents:
//
// See Also:
//      HypCTransport, HypCTransparentCommon, hems library docs
//
//
//=============================================================================

#define SOCKET_READ_INTERVAL 50

/**
	@brief High level class for tcp/ip connection
*/
class HypCTcpIp: public HypCTransparentCommon
{
//=============================================================================
// Member functions
//=============================================================================
	public:
		HypCTcpIp( HypCTransport *transport, int nDeviceID);

		virtual ~HypCTcpIp();

	protected:
		virtual void WaitEvent();
		virtual void StateMachine();
		virtual void TransportConnected();
		virtual void BeforeDisconnectTransport();
		virtual enum HypCTransport::HypETransportStatus DoGetStatus();
		virtual void CheckFrame();
		void StConnected();

	private:
		SOCKET m_Socket;
		HypCTransport *m_LinkTransport;
		HypCSocketsCommon *m_SocketsIF;
		HypCTransport::HypETransportStatus m_ConnectingStatus;

		bool m_bSslEnabled;
		int (*m_pSslFunc)(void*);

		bool m_bSslMutualEnabled;
		BYTE *m_pSslClientCertPtr;
		BYTE *m_pSslPrivateKeyPtr;
		DWORD m_nSslClientCertLen;
		DWORD m_nSslPrivateKeyLen;
		DWORD m_nSslPrivateKeyFmt;
		DWORD m_SslMasterKeyIdx;

		
		bool m_SslWaitLen;
		UWORD m_SslLength;
		UWORD m_SslRemain;
		int m_SslCurPos;
		UBYTE m_SslLenBuf[2];
		HypCBuffer* m_SslBuffer;

		bool m_UseHexLength;
};

#endif //_HYPCTCPIP_HPP_
