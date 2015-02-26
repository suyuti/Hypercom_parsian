//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      comm.cpp
//
// File Description:
//      Implmentation of an external interface of the ThreadTransport class
//
//=============================================================================

#include "comm_c.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "comm.h"
#include "sdktime.h"
#ifdef __cplusplus
 }
#endif

//=============================================================================
// External variables / declarations
//=============================================================================

//=============================================================================
// Private defines and typedefs
//=============================================================================

//=============================================================================
// Private function declarations
//=============================================================================

//=============================================================================
// Public data definitions
//=============================================================================

//=============================================================================
// Private data definitions
//=============================================================================
//! transport thread is running
Bool TRANSPORT_IS_RUNNING;

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Entry point for the communications process.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!		The function never returns!
//!
extern void INITCOMM(void)
{
	// Creates transport factory object used by hosts to create transport object
	ThreadTransport* threadTransport = new ThreadTransport( *( new HypCEvent ) );
	threadTransport->Execute();
	TRANSPORT_IS_RUNNING = False;
}

//-----------------------------------------------------------------------------
//!
//!      Shut down point for the communications process.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
extern void COM_ShutDown()
{
    TRANSPORT_IS_RUNNING = True;
    ThreadTransport::GetThreadInstance()->Shutdown();
    while( TRANSPORT_IS_RUNNING )
    {
        SDK_RelinqCPU();
    }
    delete ThreadTransport::GetThreadInstance();
}

//-----------------------------------------------------------------------------
//!
//!      Reset all transports.
//!
//! \param
//!		None
//!				
//! \return
//!     true if success
//!
//! \note
//!
extern Bool COM_ResetCompleted( )
{
	ThreadTransport::GetThreadInstance( )->COM_ResetCompleted_( );
}

//-----------------------------------------------------------------------------
//!
//!      Request to restart comms.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void COM_ResetReq( )
{ 
	ThreadTransport::GetThreadInstance( )->COM_ResetReq_();
}

//-----------------------------------------------------------------------------
//!
//!      Get the socket status.
//!
//! \param
//!		None
//!				
//! \return
//!     true if socket is idle
//!
//! \note
//!
extern Bool IsStateSocketIdle( )
{
	return (Bool)ThreadTransport::GetThreadInstance( )->IsStateSocketIdle_( );
}

//-----------------------------------------------------------------------------
//!
//!      Send the Message Request to a host.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void SendMessage( )
{
	ThreadTransport::GetThreadInstance( )->SendMessage_( );
}

//-----------------------------------------------------------------------------
//!
//!      Get response message.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void RecvMessage( )
{
	ThreadTransport::GetThreadInstance( )->RecvMessage_( );
}

//-----------------------------------------------------------------------------
//!
//!      Request message.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void RequestMessage( )
{
	ThreadTransport::GetThreadInstance( )->RequestMessage_( );
}

//-----------------------------------------------------------------------------
//!
//!      Set Up the Dialup Data.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void InitiateDial( )
{
	ThreadTransport::GetThreadInstance( )->InitiateDial_( );
}

//-----------------------------------------------------------------------------
//!
//!      Cancel host connect.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void CancelComms( )
{
	ThreadTransport::GetThreadInstance( )->CancelComms_( );
}

//-----------------------------------------------------------------------------
//!
//!      Drop line if bad PIN not returned.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void Hang_Up_Pin( )
{
	ThreadTransport::GetThreadInstance( )->Hang_Up_Pin_( );
}

//-----------------------------------------------------------------------------
//!
//!      Hang up the line.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void Hang_Up( )
{
	ThreadTransport::GetThreadInstance( )->Hang_Up_( );
}

//-----------------------------------------------------------------------------
//!
//!      Drop phone line.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void Hang_Up_Soon( )
{
	ThreadTransport::GetThreadInstance( )->Hang_Up_Soon_( );
}

//-----------------------------------------------------------------------------
//!
//!      Get line status.
//!
//! \param
//!		None
//!				
//! \return
//!     Line status
//!
//! \note
//!
extern UBYTE GetLineStatus( )
{
	return ThreadTransport::GetThreadInstance( )->GetLineStatus_( );
}

//-----------------------------------------------------------------------------
//!
//!      Reset transport.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void ResetCommModule( )
{
	ThreadTransport::GetThreadInstance( )->ResetCommModule_( );
}







