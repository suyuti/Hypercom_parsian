/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
//!
//! \file sdk.h
//!     \brief SDK Library defintions and function prototypes.
//!     \since 1996
//!

#ifndef SDKINC
#define SDKINC

#ifdef __cplusplus
extern "C" {
#endif

#include "hyctypes.h"

/**
	@defgroup GroupSDK SDK public interface
	@{
*/

/*
*  CONSTANT DEFINITIONS FOR SDK LIB
*
*/

#define SDK_MAX_REGSTRING 16    /* Max Registration String Size */


/*
 * TIMES
 * List of standard O/S Timing Increments.
 * Values are in units of 10 ms.
 */
typedef UWORD OS_TIMES;
#define TENMS 1
#define ONESECOND ( 100 * TENMS )
#define ONEMINUTE ( 60 * ONESECOND )



/**
 * OS_RETURN_VALS\n
 * Enumeration of values returned by O/S calls\n
 * Values match execdef.h\n
 */
typedef enum
{
	orvOK,					///< Normal completion
	orvUNKNOWNBLOCKADDR,			///< PTCS/IOCS block address error
	orvUNKNOWNCMD,				///< Unknown Command
	orvUNKNOWNSUBCMD,			///< Unknown Subcommand/Option
	orvUNKNOWNDEVICENUM,			///< Unknown Device Number
	orvUNKNOWNBUFFERADDR,			///< Buffer address error
	orvUNKNOWNFLAGADDR,			///< Interrupt/Flag Address error
	orvIOCSBLOCKINUSE,			///< IOCS Block is Already in Use error
	orvCMDUNSUPPORTED,			///< Command not supported
	orvSUBCMDUNSUPPORTED,			///< Subcommand/Option not supported
	orvMODEUNSUPPORTED,			///< Mode not supported
	orvINVCONFIGURATION,			///< Configuration not valid
	orvDEVICETIMEOUT,			///< Device time out
	orvBUFFEROVERFLOW,			///< Buffer overflow
	orvDEVICEBUSY,				///< Device busy
	orvDEVICEUNAVAILABLE,			///< Device not available
	orvDEVICEFAULT,				///< Device fault
	orvDEVICENOTOPEN,			///< Device not open
	orvDEVICEREWOUND,			///< Device rewound during I/O
	orvDEVICENOTCONFIGURED,			///< Device configured during I/O
	orvDEVICEDISCONNECT,			///< Device disconnected during I/O
	orvDEVICERESET,				///< Device reset during I/O
	orvDEVICECLOSED,			///< Device closed during I/O
	orvTIMERSUNAVAILABLE,			///< No Timers Available
	orvTIMERNOTFOUND,			///< Timer Not Found
	orvINVTIMEINTERVAL,			///< Invalid Time Interval
	orvPROCESSINBADSTATE,			///< Process in Wrong State
	orvPROCESSNOTFOUND,			///< Process Not Found
	orvPARITYERROR,				///< Parity error in data
	orvLRCERROR,				///< LRC error in data
	orvDATAOVERRUN,				///< Data overrun
	orvINVDATA,				///< Invalid data
	orvNAK,					///< Negative acknowledge
	orvFRAMINGERROR,			///< Framing error
	orvNODIALLINE,				///< NO DIAL LINE AVAILABLE
	orvXMODEMCRCERROR,			///< XMODEM 1st block error 'crc'
	orvXMODEMLRCERROR,			///< XMODEM 1st block error 'lrc'
	orvLINEBUSY,				///< Modem response: line busy
	orvNOANSWER,				///< Modem response: no answer
	orvPAPEROUT,				///< Paper out (lpt)
	orvOVERHEAT,				///< Over heat (lpt)
	orvCTLGENERALERROR,			///< Contactless General Error
	orvCTLREQUESTERROR,			///< Contactless Request Error (0x11)
	orvCTLSELECTERROR,			///< Contactless Select Error (0x21)
	orvCTLUNSUPPORTEDCARD,			///< Contactless Unsupported Card Error (0x31)
	orvCTLAUTHERROR,			///< Contactless Authentication Error (0x41)
	orvCTLVALUEUNDERFLOW,			///< Contactless Value Underflow (02)
	orvCTLVALUEOVERFLOW,			///< Contactless Value Overflow (03)
	orvDCDLOST,				///< DCD lost
	orvDMRECEIVED,				///< DM received
	orvSENDINGERROR,			///< Sending error
	orvUNEXPECTEDERROR,			///< Unexpected SNRM received
	orvPOLLINGLOSS,				///< Loss of polling
	orvXMODEMTIMEOUT,			///< XMODEM READ TIMEOUT - MUST BE > ERRDCD
	orvLAPMINVFRAME,			///< Invalid LAPM Frame received
	orvLAPMDINVSEQNUM,			///< Invalid LAPM Receive Sequence Number (Nr) received
	orvLAPMTIMEOUT,				///< LAPM Response Time-out Error
	orvLAPMDMRECEIVED,			///< LAPM Disconnected Mode (DM) Frame received
	orvLAPMDISCRECEIVED,			///< LAPM Disconnect (DISC) Frame received
	orvLAPMFRMRRECEIVED,			///< LAPM Frame Reject (FRMR) Frame received
	orvUNKNOWNERROR,			///< Unknown O/S error value
	orvUSER,				///< User error value
	orvDEVICENOCARD,			///< Card is absent in a card reader	
	orvMAXERRORS				///< Maximum number of errors
} OS_RETURN_VALS;



/**
 * PAGE_NUMBERS\n
 * Enumeration indicating page identifier\n
 */
typedef enum
{
    pn_BOOT3 = -3,
    pn_BOOT2,
    pn_BOOT1,
    pn_PAGE0, ///< BOOT   
    pn_PAGE1,
    pn_PAGE2,
    pn_PAGE3,
    pn_PAGE4,
    pn_PAGE5,
    pn_PAGE6,
    pn_PAGE7,
    pn_PAGE8,
    pn_PAGE9,
    pn_PAGE10,
    pn_PAGE11,
    pn_PAGE12,
    pn_PAGE13,
    pn_PAGE14,
    pn_PAGE15,
    pn_PAGE16,
    pn_PAGE17,
    pn_PAGE18,
    pn_PAGE19,
    pn_PAGE20,
    pn_PAGE21,
    pn_PAGE22,
    pn_PAGE23,
    pn_PAGE24,
    pn_PAGE25,
    pn_PAGE26,
    pn_PAGE27,
    pn_PAGE28,
    pn_PAGE29,
    pn_PAGE30,
    pn_PAGE31,
    pn_PAGE32,
    pn_PAGE33,
    pn_PAGE34,
    pn_PAGE35,
    pn_PAGE36,
    pn_PAGE37,
    pn_PAGE38,
    pn_PAGE39,
    pn_PAGE40,
    pn_PAGE41,
    pn_PAGE42,
    pn_PAGE43,
    pn_PAGE44,
    pn_PAGE45,
    pn_PAGE46,
    pn_PAGE47,
    pn_PAGE48,
    pn_PAGE49,
    pn_PAGE50,
    pn_PAGE51,
    pn_PAGE52,
    pn_PAGE53,
    pn_PAGE54,
    pn_PAGE55,
    pn_PAGE56,
    pn_PAGE57,
    pn_PAGE58,
    pn_PAGE59,
    pn_PAGE60,
    pn_PAGE61,
    pn_PAGE62,
    pn_PAGE63
}   PAGE_NUMBERS;

/**
	SDK and terminal components version info.
*/
typedef struct
{
  char SDKplatform[11];	///< platform or terminal type 
  char SDKvariant[11];	///< variant,   
  char SDKversion[4];	///< number + letter version ex 01A 
  char OSrelease[11];	///< OS release date 
  char OSversion[7];	///< OS version 
  char BTversion[11];	///< BOOT LOADER version 
} VERSION_INFO;


/**
	Enumeration of possible values returned by SDK_ValidatePage()
*/
typedef enum
{
	PAGE_OK,
	PAGE_CORRUPT,
	PAGE_NOT_ACTIVE
} PAGE_VALIDATE;

//-----------------------------------------------------------------------------
//! \brief Retrieve SDK version
//!
//! \par
//! Retrieve SDK version information
//!	and O/S version information on 
//!	which this SDK is based. Also
//!	returns BOOT version which independent of SDK
//!	This function returns version information in a structure supplied by caller.
//!	Each item in the structure will be returned from SDK_Version as a null-terminated string.
//!	Length of string will not exceed array size minus 1 ( null terminator).
//!
//! \param[in]  pVerInfo		pointer to struct where version 
//!                             information will be placed
//!
//! \return none
//-----------------------------------------------------------------------------
void SDK_Version(VERSION_INFO *pVerInfo);

/**
	@example SDK_Version.c
	This is an example of how to use the SDK_Version() function.
*/

//-----------------------------------------------------------------------------
//! \brief Delays further processing until<br>
//!        SDK data structures have been initialized.
//!
//! For Now, do a three seconds delay before continuing.
//!
//! \return none
//!
//! \see SDK_Wait()
//-----------------------------------------------------------------------------
extern void SDK_Ready(void);

//-----------------------------------------------------------------------------
//! \brief Initializes SDK,HFS and FON pages.
//!
//! \return none
//!
//-----------------------------------------------------------------------------
extern void InitSDKPages(void);


//-----------------------------------------------------------------------------
//! \brief Initializes pages used by browser
//!
//! \return none
//!
//-----------------------------------------------------------------------------
extern void InitBrowserPages(void);


//-----------------------------------------------------------------------------
//! \brief Compare calculated checksum of given page
//!
//! \par
//! Compare calculated checksum of given page 
//! with checksum stored in Page Status Table.
//! The Page Status Table is maintained by O/S.
//! If checksums match, returns PAGE_OK. The
//! function is useful for detecting memory 
//! corruption in the constant regions of a page.
//!
//! \param[in]  lpagenum		logical page number to validate
//!
//! \return
//! PAGE_OK         for good checksum<br>
//! PAGE_CORRUPT    for bad checksum<br>
//! PAGE_NOT_ACTIVE attempt to validate inactive page<br>
//!
//! \note
//! just return PAGE_OK, no actions inside
//-----------------------------------------------------------------------------
extern PAGE_VALIDATE SDK_ValidatePage(PAGE_NUMBERS lpagenum);


//-----------------------------------------------------------------------------
//! \brief Initializes TCP and PLL pages.
//!
//! Must be called on application start up in applications that utilize TCP/IP.
//! Initializes TCP and PLL pages as supplied with HSDK toolkit for ICE 6000
//! and ICE 6500 PEM terminals.
//!
//! \return none
//!
//-----------------------------------------------------------------------------
extern void InitTcpIpPages(void);

/**
	@}
*/

#ifdef __cplusplus
}
#endif


#endif          /* ifndef SDKINC */
