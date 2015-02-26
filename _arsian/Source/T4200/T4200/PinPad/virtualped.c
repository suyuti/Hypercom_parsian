//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2007
//=============================================================================
//
// Module overview:             pinwrap.cpp
//      Internal PIN pad security functions
//
//=============================================================================
#include <basictyp.h>
#include <API_Interface/inc/system_extr.h>
#include <API_Interface/inc/smartcard_extr.h>
#include <MainAppForAppMgr/SecProvDefs.h>
#include <AppMgrDDL/secprov_exp.h>
//#include <cryptoservice.h>
#include "virtualped.h"
#include "cryptoservice.h"


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
// Private data definitions
//=============================================================================

// Virtual PinPAD Data structure
VPP_DATA    m_pVPPdata;
// PIN block buffer
unsigned char   m_PinBlock[8];
char m_AsciiPan[20];


//=============================================================================
// Private type definitions
//=============================================================================



//-----------------------------------------------------------------------------
//! \brief
//!     Gets Security Provider version.
//!
//! \param
//!		buf			buffer to place version string into
//!		buflen		length of the buf
//!				
//! \return
//!     version string length, 0 on error
//!
//! \note   
//!
//-----------------------------------------------------------------------------
int VPPGetVersion( char* buf, int buflen )
{
        sprintf( buf, "%03d", GetSecurityProviderVersion( ));
        return 3;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Initialize virtual pinpad
//!
//! \param
//!		DUKPTflag	1 in case if DUKPT mode, 0 - Master Session mode
//!		trpan		pointer to PAN
//!		panLength	PAN length
//!		ksn			pointer to Key Serial Number (for DUKPT only)
//!		ksn_size	KSN size (for DUKPT only) 
//!		key_id		Master Key ID in the key collection
//!		doubleKey	working key length (8,16,24)
//!		workingKey	pointer to working session key
//!				
//! \return
//!		1 is OK, 0 - otherwise
//!
//! \note   
//!
//-----------------------------------------------------------------------------
int VPPInit( VPP_ActionType action, UBYTE* trpan, int panLength, unsigned char* ksn,
			  int ksn_size, unsigned char key_id, int doubleKey, unsigned char* workingKey )
{
    CS_SetSessionKeyMode(SESSION_KEY_ECB);
	// Clear the ascii pan buffer
	memset( m_AsciiPan, 0, sizeof( m_AsciiPan ) );
	// Move PAN to output and convert to ASCII. (Max 19 bytes)
	MovPanAsc( m_AsciiPan, trpan, 19 );
            
	switch( action )
	{
		case DUKPT:
			m_pVPPdata.indexKey = 16;	// Index for desired key in the key collection. for DUKPT = 16
			m_pVPPdata.type = 'D';
			// Index for the desired key in the key collection :
			// For DUKPT this is a pointer to the Key Serial Number.
			// The KSN is returned at this address :
			m_pVPPdata.pAD = ksn;
			m_pVPPdata.sizeAD = ksn_size;
			break;

		case MASTER_SESSION:
			// Index for the desired key in the key collection :
			m_pVPPdata.indexKey = Binary( ( char ) key_id );
			m_pVPPdata.type = 'M';
			// See if Triple DES (Double Length key)
			m_pVPPdata.sizeAD = doubleKey;
			if ( NullComp ( ( char * ) workingKey, doubleKey ) )
			{
				m_pVPPdata.pAD = NULL;
				m_pVPPdata.sizeAD = 0;
			}
			else
			{
				// For Master Session this is the packed encrypted session key
				m_pVPPdata.pAD = ( UBYTE * ) workingKey;
			}
			break;

		default:
			break;
	}

	// Fill Virtual PinPad structure
	m_pVPPdata.timeout = 99;			// Timeout value (seconds), 5 to 200 seconds.
	m_pVPPdata.pPAN = m_AsciiPan;	    // Pointer to the PAN as a character string.
	m_pVPPdata.pPINBlock = m_PinBlock;

	return CS_VPPInit( NULL, (void *)&m_pVPPdata );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Initializes the Virtual PIN pad for EMV operation
//!
//! \param
//!		action		type of an operation
//!				
//! \return
//!		1 is OK, 0 - otherwise
//!
//! \note   
//!
//-----------------------------------------------------------------------------
int VPPEMVInit( VPP_EMV_ActionType action, const unsigned char* const publicKey, const unsigned int publicKeyLength )
{

	if( ( EMV_OFFLINE_ENCPIN == action ) && publicKey && publicKeyLength ) 
    {
   		// Fill Virtual PinPad Structure
		m_pVPPdata.type = VPP_EMV_OFFLINE_ENCPIN; 
		m_pVPPdata.pAD = (void *)(publicKey + 1);		        // Pointer to the public key
		m_pVPPdata.sizeAD = publicKeyLength - 1;	// Size of the Additional Data buffer
		m_pVPPdata.Exponent = (1==publicKey[0])?65537:publicKey[0];		// The exponent matching the public key

    }
    else if( EMV_OFFLINE_CLEARPIN == action )
    {
		// Fill Virtual PinPad Structure
		static uint32_t EmptyPublicKey[] = { 0x00 };
		m_pVPPdata.type = VPP_EMV_OFFLINE_CLEARPIN; 
		m_pVPPdata.pAD = &EmptyPublicKey;				// Pointer to Additional Data
		m_pVPPdata.sizeAD = sizeof( EmptyPublicKey );	// Size of the Additional Data 
    }
	m_pVPPdata.pPAN = NULL;								// Not Used
	m_pVPPdata.indexKey = 0;							// Not Used
	m_pVPPdata.pPINBlock = NULL; 						// Not Used

    return CS_VPPInit( NULL, (void *)&m_pVPPdata );

    return 1;
}

//-----------------------------------------------------------------------------
//  GetPIN			Get PIN Entry, Build the PIN BLOCK using Virtual Pin Pad Init
// 					and Virtual Pin Pad Event functionalities. Also Show the Encrypted 
// 					PIN BLOCK in the Screen.
//  Parameters:
// 		None
//  Global Inputs:
//		None
//  Returns:
//	VPP_InvalidKey,				// For internal use only
//	VPP_NotActive,				// Virtual PIN Pad is not active, call VPPInit first
//	VPP_TimedOut,				// VPPInit's timeout has expired
//	VPP_EncryptError,			// ENTER was pressed but error occurred
//	VPP_BufferFull,				// Input buffer is full (entered PIN is too large)
//	VPP_PINKey,					// Numeric key was pressed, echo with "*"
//	VPP_EnterKey,				// ENTER was pressed and key encrypted
//	VPP_BackspaceKey,			// BackSpace was pressed
//	VPP_ClearKey,				// CLEAR was pressed, clear out "*" display
//	VPP_CancelKey,				// CANCEL was pressed
//	VPP_GeneralError,			// The process can not continue. Internal error
//	VPP_CustomerCardNotPresent,	// The customer has removed the smart card from the slot
//	VPP_HTCCardError,			// Error accessing the smart card
//	VPP_WrongPINLastTry,		// Incorrect PIN, one more try
//	VPP_WrongPIN,				// Not the Last PIN Try
//	VPP_ICCError				// Too many tries
//		
//  Notes:
// 		this function receive the input data "VPP_DATA", get the PIN from the user
// 		and build the PIN BLOCK showing it on the screen. The user can check if the 
// 		PIN BLOCK resulted is Good or not using the Encryption Utility Tool.
//-----------------------------------------------------------------------------
VPP_EVENTS VPPGetPin( char Maxlen, char* screen_buf, char* dest, Call_Back UpdateInput, unsigned char *sw1sw2 )
{
	VPP_Events vpp_event = VPP_INVALID_KEY;
    VPP_EVENTS vpp_app_events;
    UBYTE pinlen = 0;
        
	while ( 1 )
	{
		screen_buf[pinlen] = 0;
		UpdateInput( screen_buf );
    
		// Wait for a VPP Event
		vpp_event = CS_VPPEvent( VPP_NormalMode );

		if( vpp_event == VPP_INVALID_KEY )
		{
			vpp_app_events = VPP_InvalidKey;
			break;
		}
		else if( vpp_event == VPP_NOT_ACTIVE )
		{
			// The Virtual PIN Pad is not active, call VPPInit first.
			vpp_app_events = VPP_NotActive;
			break;
		}
		else if( vpp_event == VPP_TIMED_OUT )
		{
			// VPPInit's timeout has expired.
			vpp_app_events = VPP_TimedOut;
			break;
		}
		else if( vpp_event == VPP_ENCRYPT_ERROR )
		{
			// Enter was pressed but an error occurred.
			vpp_app_events = VPP_EncryptError;
			break;
		}
		else if( vpp_event == VPP_BUFFER_FULL )
		{
			// The input buffer is full.  The entered PIN is too large. Ignore this keystroke.
			continue;
		}
		else if( vpp_event == VPP_PIN_KEY )
		{
			// A numeric key was pressed, echo a "*"
			if ( pinlen < Maxlen )
			{
				((volatile char*)screen_buf)[pinlen] = '*';
				pinlen++;
			}
			continue;
		}
		else if( vpp_event == VPP_ENTER_KEY )
		{       
			// Enter was pressed and key encrypted
			vpp_app_events = VPP_EnterKey;
			// Get the last status word
			memcpy( sw1sw2, m_pVPPdata.SWBytes, 2 );
			break;
		}
		else if( vpp_event == VPP_BACKSPACE_KEY )
		{
			// Backspace was pressed
			if ( pinlen ) 
			{
				pinlen--;
			}
			continue;
		}
		else if( vpp_event == VPP_CLEAR_KEY )
		{
			// Clear out the "*" display.
			pinlen = 0;
			continue;
		}
		else if( vpp_event == VPP_CANCEL_KEY )
		{
			// Cancel was pressed
			vpp_app_events = VPP_CancelKey;
			break;
		}
		else if( vpp_event == VPP_GENERALERROR || vpp_event == VPP_HTCCARDERROR )
		{
			//The process can not continue. Internal error
			vpp_app_events = VPP_GeneralError;
			break;
		}
		else if( vpp_event == VPP_CUSTOMERCARDNOTPRESENT )
		{
			// The customer has removed the smart card from the slot
			vpp_app_events = VPP_CustomerCardNotPresent;
			break;
		}
		else if( vpp_event == VPP_WRONG_PIN_LAST_TRY )
		{
			// Incorrect PIN, one more try
			vpp_app_events = VPP_WrongPINLastTry;
			break;
		}
		else if( vpp_event == VPP_WRONG_PIN )
		{
			// Incorrect PIN, one more try
			vpp_app_events = VPP_WrongPIN;
			break;
		}
		else if( vpp_event == VPP_ICCERROR )
		{
			// Too many tries
			vpp_app_events = VPP_ICCError;
			break;
		}
		else
		{
			// The process can not continue. Internal error
			vpp_app_events = VPP_GeneralError;
			break;
		}
	}
	if( vpp_app_events == VPP_EnterKey  && NULL != dest && NULL != m_pVPPdata.pPINBlock )
	{
		memcpy( dest, m_pVPPdata.pPINBlock, 8 );
	}
	return vpp_app_events;
}

//=============================================================================
// Private function definitions
//=============================================================================
//=============================================================================
// Private function declarations
//=============================================================================
//=============================================================================
// Private data definitions
//=============================================================================

//=============================================================================
// Private type definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//=============================================================================
// Private function definitions
//=============================================================================



