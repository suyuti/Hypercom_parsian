//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Common POS Application
//      (c) Copyright 2006
//
// File Name:
//      pinwrap.h
//
// File Contents:
//      Declaration of pinpad security utilites
//
//=============================================================================
#ifndef PINWRAP_H
#define PINWRAP_H

//! VPP action type
typedef enum
{
    DUKPT,			// DUKPT
	MASTER_SESSION	// Master Session
}VPP_ActionType;

//! VPP action for EMV operations( is not used for T4100 )
typedef enum
{
	EMV_OFFLINE_CLEARPIN,	// EMV offline with clear PIN
	EMV_OFFLINE_ENCPIN,		// EMV offline with encrypted password
	EMV_PIN_VERIFY_CLEARPIN,// EMV offline with clear verification using a PIN block
	EMV_PIN_VERIFY_ENCPIN,	// EMV offline with encrypted password using a PIN block
	INVALID_SESSION			// For parameter testing purposes
}VPP_EMV_ActionType;

//! VPP events
typedef enum
{
	VPP_InvalidKey,				// For internal use only
	VPP_NotActive,				// Virtual PIN Pad is not active, call VPPInit first
	VPP_TimedOut,				// VPPInit's timeout has expired
	VPP_EncryptError,			// ENTER was pressed but error occurred
	VPP_BufferFull,				// Input buffer is full (entered PIN is too large)
	VPP_PINKey,					// Numeric key was pressed, echo with "*"
	VPP_EnterKey,				// ENTER was pressed and key encrypted
	VPP_BackspaceKey,			// BackSpace was pressed
	VPP_ClearKey,				// CLEAR was pressed, clear out "*" display
	VPP_CancelKey,				// CANCEL was pressed
	VPP_GeneralError,			// The process can not continue. Internal error
	VPP_CustomerCardNotPresent,	// The customer has removed the smart card from the slot
	VPP_HTCCardError,			// Error accessing the smart card
	VPP_WrongPINLastTry,		// Incorrect PIN, one more try
	VPP_WrongPIN,				// Not the Last PIN Try
	VPP_ICCError				// Too many tries
}VPP_EVENTS;

#ifdef __cplusplus
extern "C" {
#endif

typedef void( *Call_Back )( char* );

//! Returns a version number for the requested Security Provider.
int VPPGetVersion( char* buf, int buflen );
//! Close VPP
void VPPClose( );

//! Initializes the Virtual PIN pad
int VPPInit( VPP_ActionType action, unsigned char* trpan, int panLength,
				 unsigned char* ksn, int ksn_size, unsigned char key_id,
				 int doubleKey, unsigned char* workingKey );
//! Initializes the Virtual PIN pad for EMV operation( is not implemented for T4100 )
int VPPEMVInit( VPP_EMV_ActionType action, const unsigned char* const publicKey, const unsigned int publicKeyLength );

//! gets PIN
VPP_EVENTS VPPGetPin( char Maxlen, char* screen_buf, char* dest, Call_Back UpdateInput, unsigned char *sw1sw2 );

#ifdef __cplusplus
}
#endif

#endif


