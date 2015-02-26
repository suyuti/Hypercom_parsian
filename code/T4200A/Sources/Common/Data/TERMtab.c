//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             TERMtab.c
//      Set the Checksum (LRC) of the Terminal Table.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "utllib.h"
#include "fixdata.h"

#include "defines.h"
#include "TERMtab.h"

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
TERMREC EpromDefaults =
{
	{ '2','0','0','0','0','0','0','3' },// terminal serial number
	'2',								// terminal serial number check digit
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	// initialization telephone number
	   0xFF,0xFF,0xFF,0xFF,0xFF,0xFF },
	{ 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	// NMS telephone number
	   0xFF,0xFF,0xFF,0xFF,0xFF,0xFF },
	0,									// Dial mode = DTMF
	{ 0xFF,0xFF,0xFF,0xFF },			// PABX access code
	30,									// Timeout for initialize
	{ 0x30,0x30,0x30,0x30 },			// Merchant password
	1,									// Line type = dial up
	0x30,								// CU address
	1,									// Options 1: Enable entry of terminal id
	0,									// Options 2: none
#if defined(MAKE_VIKING)
	TM3_HEXLENGTH,						// Options 3: hex length in header
#else
    0,
#endif // MAKE_VIKING
    TM4_TRK123|TM4_ICC,					// Options 4: Enable Track 3 
	{ 0x00,0x03 },						// Initialization NII
	0,									// Modem mode = 0 = Bell 212A SDLC 1200 bps
	0,									// Pulse dial ratio = 0 = 60/40 for USA
	0,									// Pulse dial mode = 0 = Normal
	1,									// DTMF dialing speed = 0 = slow (140msec ON)
	50,									// Dial tone wait timer
	50,									// Pause wait timer
	{ 0,0,0 },							// Systems Trace Audit Number (STAN)
	{ 0,0,0 },							// Terminal invoice number
	1,									// Hook wait timer
	{ 0,2 },							// Line hold time
	0x30,								// Connection time
	0x30,								// Minimum disconnect time
	{ 0,0,0,0,0, 0,0,0,0,0,
	   0,0,0,0,0, 0,0,0,0,0,
	   0,0,0 },							// Merchant Name				
	0,									// Initialization done flag, 0=> not done
	5,									// LCD contrast
	{ 0x30,0x00 },						// Busy line wait time
	0,									// Language, 0=>English
	0x0e,								// Terminal printer type, 0=>none
	{ '0','0','0','0','0','0','0','0' },// Terminal hardware id (serial number)
	{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },	// Terminal key for TNMS data
	{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },	// TNMS key for terminal data
	1,									// Transmit level, 0=> -10dBm (default)
	0,									// FTPxx, main board type
	0,									// FTP build
	' ',								// FTP revision
	0,									// Dial device, 0=>no internal device
	{ 'S','N','S','G' },				// Terminal hardware id prefix
										// Upper 4 characters from the ORACLE
										// Serial Number Sequence Generator
										// none given in the document, 
										// so SNSG is arbitrary
	{ 0x00,0x06 },						// Pad calibration min X axis value
	{ 0x03,0x9B },						// Pad calibration max X axis value
	{ 0x00,0x45 },						// Pad calibration min Y axis value
	{ 0x03,0x9A },						// Pad calibration max Y axis value
	{ 0 },								// Inter-digit pause
	{ 0,0 },							// Minimum ring duration
	{ 0,0 },							// Maximum ring duration
	{ 2 },								// Redial attempts
	{ 0 },								// Secondary redial attempts
	{ 12 },								// DTMF level
	{ 0 },								// Dial tone duration
	{ 0 },								// Dial tone interruption
	{ 50 },								// Dial tone start wait time

	{									// Cypher Region
		0xFF,							// Page number
		NULL,							// Cypher Region Address
		0xFFFF,							// Cypher Region Size
	},

	0xFF,								// Secure-It Application Option Bits

	0x00000000,							// Terminal's Local IP Address
	0x00000000,							// Terminal's Subnet IP Address
	0x00000000,							// Terminal's Gateway IP Address
	0x00000000,							// NMS Destination IP Address

	0xFFFF,								// NMS Destination Port Number

	0x00000000,							// Init Destination IP Address

	0xFFFF,								// Init Destination Port Number

	0x00000000,							// Init DNS1 IP Address
	0x00000000,							// Init DNS2 IP Address

	{									// Cypher Backup Region
		0xFF,							// Page number
		NULL,							// Cypher Region Address
		0xFFFF,							// Cypher Region Size
	},

	{  0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	// 63 filler
	   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
       0xFF, 0xFF, 0xFF
	   }
};


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Sets the Checksum (LRC) for the Terminal Table
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void SetTERMTabLRC( void )
{
	TERMLRC = CalcLrc( ( UBYTE * ) & TERM, ( UWORD ) sizeof( TERM ) );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&TERM );
#endif
}

extern void SetDialBackLRC( void )
{
}

//-----------------------------------------------------------------------------
//! \brief
//!     Initializes the Terminal Invoice Number
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void InitTermInv( void )
{
	UBYTE cbuf[RTCLEN];

	// Check if the Invoice Number exists
	if ( NullComp( ( char * ) TERM.TERMInv, ( UWORD ) 3 ) )
	{
		// No; read the Real Time Clock
		SDK_RtcRead( cbuf );

		// SS
		AscHex( (UBYTE *)&TERM.TERMInv[0], ( char * ) &cbuf[10], ( UBYTE ) 1 );

		// MM
		AscHex( (UBYTE *)&TERM.TERMInv[1], ( char * ) &cbuf[8], ( UBYTE ) 1 );

		// HH
		AscHex( (UBYTE *)&TERM.TERMInv[2], ( char * ) &cbuf[6], ( UBYTE ) 1 );

		// Recalculate Checksum
		SetTERMTabLRC();
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Increments Terminal Systems Trace Audit Number
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void IncTermStan( void )
{
	// Increment BCD number
	IncBcdLen( (UBYTE *)TERM.TERMStan, 3 );

	// Set table LRC
	SetTERMTabLRC();
}

//-----------------------------------------------------------------------------
//! \brief
//!     Increment 'STAN' and move System Trace Number from 'TERM' to 'TRREQ'
//!     or 'TRINP', whichever is passed
//!
//! \param
//!		[out]pTraceNum		Pointer to Trace Number destination
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void IncTraceNum( UBYTE * pTraceNum )
{
	// Increment Terminal Systems Trace Audit Number
	IncTermStan();

	// Move Terminal Systems Trace Audit Number
	memcpy( pTraceNum, (UBYTE *)TERM.TERMStan, sizeof( TERM.TERMStan ) );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Initializes Term Table per Eprom Defaults
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void InitTermTab( void )
{

	// Move Eprom Defaults to TERM 
	memcpy( ( UBYTE * ) & TERM, ( UBYTE * ) & EpromDefaults, SZTERM );

	// Clear Terminal Initialization Done Flag 
	Clr_TermInit();

	// Set Format Value 
	TERMFMT = 0x55;

    // Recalculate LRC
    SetTERMTabLRC();
}

//-----------------------------------------------------------------------------
//! \brief
//!     Sets Terminal Initialization Done Flag
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void Set_TermInit( void )
{
	TERM.TERMInit = 0xff;

	SetTERMTabLRC();
}

//-----------------------------------------------------------------------------
//! \brief
//!     Clears Terminal Initialization Done Flag
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void Clr_TermInit( void )
{
	TERM.TERMInit = 0x00;

	SetTERMTabLRC();
}


//=============================================================================
// Private function definitions
//=============================================================================

