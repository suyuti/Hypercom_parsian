//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      HostTransport.cpp
//
// File Description:
//      Implmentation of HostTransport class
//
//=============================================================================
#include <HypCModem.hpp>
#include "HostTransport.hpp"

//-----------------------------------------------------------------------------
//!
//!      Constructor. Creates communication module.
//!
//! \param
//!      deviceId        Unique ID to identify this device.
//!
//! \return
//!
//! \note
//!
HostTransport::HostTransport( int TransportId ) :
#if defined(AAE_MODEM)
	HypCHostMLTransportAAE( TransportId )
#else
	HypCHostMLTransport( TransportId )
#endif
	, m_CheckLink( true )
#ifdef MAKE_SSL
    , m_pSSLMemoryPool( NULL )
    , m_pSSL( NULL )
#endif // MAKE_SSL
{
}

//-----------------------------------------------------------------------------
//!
//!      Delete the current transport object
//!
//! \return
//!      Nothing
//!
//! \note
//!
void HostTransport::ResetTransport()
{
    Reset();
#ifdef MAKE_SSL
    // SSL socket has to be destroyed in Reset() function
    m_pSSL= NULL;
    delete m_pSSLMemoryPool;
    m_pSSLMemoryPool= NULL;
#endif // MAKE_SSL
}

//-----------------------------------------------------------------------------
//!
//!      Returns NetClient object if object has proper transport type
//!
//! \return
//!      Pointer to HypCNetClient if object has proper transport type (IP)
//!      otherwise NULL
//!
HypCNetClient *HostTransport::GetNetClient()
{
    HypCTransport   *pTransport;
    HypCNetClient   *pClient= NULL;

    if( LINE_TCPIP == GetTransportType() )
    {
        pTransport= SubTransport();
        if( pTransport )
        {
            pClient= static_cast<HypCNetClient*>(pTransport);
        }
    }
    return pClient;
}

//-----------------------------------------------------------------------------
//!
//!      Use prefix length bytes on send, strip on receive data
//!
//! \param
//!      prefix            use prefix or not
//!
//! \return
//!      None
//!
//! \note
//!		The function can be called  in case if IP protocol only
//!
void HostTransport::Prefix( bool prefix )
{
    HypCNetClient   *pClient= GetNetClient();

	if( pClient )
	{
		pClient->ProcessPacketLength( prefix );
	}
}

#ifdef MAKE_SSL
//-----------------------------------------------------------------------------
//!
//!      Creates SSL based socket if object has proper transport type
//!
//! \return
//!      Pointer to HypCSSLSocket on success else NULL
//!
//! \note
//!		The function can be called  in case if IP protocol only
//!
HypCSSLSocket *HostTransport::CreateSSL()
{
    HypCNetClient   *pClient= GetNetClient();

    if( pClient && !m_pSSL )
    {
        // SSL memory pool has to be created once before using of SSL socket!!!
        if( !m_pSSLMemoryPool )
        {
            m_pSSLMemoryPool= new HypCSSLMemoryPool();
        }
        if( m_pSSLMemoryPool )
        {
            // Creates SSL socket and substitutes instead current
            m_pSSL= new HypCSSLSocket();
            if( m_pSSL )
            {
                pClient->SetSocket( m_pSSL );
            }
        }
    }
    return m_pSSL;
}
#endif // MAKE_SSL

