//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      comm.h
//
// File Description:
//      Declaration of C functions dealing with transport object.
//
//=============================================================================
#ifndef _COMM_H_
#define _COMM_H_

//=============================================================================
// Public defines and typedefs
//=============================================================================


//=============================================================================
// Public data declarations
//=============================================================================

//=============================================================================
// Public function declarations
//=============================================================================

//! Entry point for the communications process.
extern void INITCOMM( void );
//! Shut down transport
extern void COM_ShutDown();
//! Reset all transports
extern Bool COM_ResetCompleted();
//! Request to restart comms
extern void COM_ResetReq();
//! Get the socket status
extern Bool IsStateSocketIdle();
//! Send the Message Request to the Host 
extern void SendMessage();
//! Get response message 
extern void RecvMessage();
//! Request message
extern void RequestMessage();
//! Set Up the Dialup Data
extern void InitiateDial();
//! Cancel host connect
extern void CancelComms();
//! Drop line if bad PIN not returned
extern void Hang_Up_Pin();
//! Hang up the line
extern void Hang_Up();
//! Drop phone line
extern void Hang_Up_Soon();
//! Get line status
extern UBYTE GetLineStatus();
//! Reset transport
extern void ResetCommModule();

#endif // _COMM_H_
