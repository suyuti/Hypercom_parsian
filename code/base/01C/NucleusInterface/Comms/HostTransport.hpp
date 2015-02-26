//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Common POS Application
//      (c) Copyright 2006
//
// File Name:
//      HostTransport.hpp
//
// File Contents:
//      Definition of HostTransport class
//
//=============================================================================
#if !defined( _HOSTTRANSPORT_HPP_ )
#define _HOSTTRANSPORT_HPP_

#include <compiler.h>
#if defined(AAE_MODEM)
#include <HypCHostMLTransportAAE.hpp>
#else
#include <HypCHostMLTransport.hpp>
#endif
#include <HypCTransportAddressSet.hpp>
#include <HypCNetClient.hpp>
#ifdef MAKE_SSL
#include <HypCSSLMemoryPool.hpp>
#include <HypCSSLSocket.hpp>
#endif // MAKE_SSL

//=============================================================================
// Public defines and typedefs
//=============================================================================

//=============================================================================
// Forward definitions
//=============================================================================
class HypCTransport;
class HypCSerialPort;
class HypCModem;

//=============================================================================
//!
//! \brief
//!      Creates multi-layered transport objects used for host communication.
//!
//!      Implementation of a HypCMLTransport class type used for HFC host 
//!      communication. 
//!
//!      Refer to HypCMLTransport for using the methods of this class.
//!
//! \sa
//!      HypCMLTransport
//!
//=============================================================================
class HostTransport : public
#if defined(AAE_MODEM)
HypCHostMLTransportAAE
#else
HypCHostMLTransport
#endif
{
//=============================================================================
// Member functions
//=============================================================================
	public:
		//! Constructor
		explicit HostTransport( int Transport_IDl );

        //! Delete the current transport object and reset parameters
        void ResetTransport();

        //! Use prefix length bytes on send, strip on receive data
		void Prefix( bool prefix );

        //! Returns NetClient object if object has proper transport type
        HypCNetClient *GetNetClient();

#ifdef MAKE_SSL
        //! Creates SSL transport type
        HypCSSLSocket *CreateSSL();
#endif // MAKE_SSL

//=============================================================================
// Member variables
//=============================================================================
	private:
		//! Does check external link  do.
		bool                m_CheckLink;
#ifdef MAKE_SSL
        //! SSL socket memory pool.
        HypCSSLMemoryPool   *m_pSSLMemoryPool;
        //! SSL socket.
        HypCSSLSocket       *m_pSSL;
#endif // MAKE_SSL
};
// HostTransport

#endif // #if !defined( _HOSTTRANSPORT_HPP_ )



