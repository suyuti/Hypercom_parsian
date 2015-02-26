//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
#ifndef _HOSTUTIL_H_
#define _HOSTUTIL_H_
//=============================================================================
//
// Module overview:             Host Library function prototypes
//......Host utility functions
//      This file should be included in all 'C' source code files that 
//      use the host library.
//
//=============================================================================

//=============================================================================
// Public defines and typedefs
//=============================================================================


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Appends two specified characters to send data
//!
//! \param
//!		[in]Chars			Pointer to array of two characters to append to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void Append2Chars( char *Chars );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends first two characters of the Acceptor ID to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendAqAccID2( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends the rest of the Acceptor ID after the first two characters to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendAqAccID3Plus( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends amount to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendCheckAmount( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Birth Date to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendBirthDate( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends specified character to send data
//!
//! \param
//!		[in]Char			Ñharacter to append to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendChar( char Char );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Customer's Check Number or the last four digits of the MICR#
//!     to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendCheckNum( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends check number with leading character 'C' to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendCheckNumC( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends check number with leading character '#' to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendCheckNumP( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Driver's License ID Number to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendDrvLic( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Expiration Date to send data (Ascii MMYY)
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendExpDate( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Field Separator (0x1c) to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendFS( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Customer's Check Reader Information to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendMICR( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Transit and Account Numbers with a prepended "T" and "A" to
//!     send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendMICRTA( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Transit and Account Numbers with a trailing "T" and "A" to
//!     send data
//!
//! \return
//!     None.
//!
//! \note
//!     Only call this if (TRCHECKF[0] & 0x58) == 0x50 or 0x48,
//!     making TRMICRfmt == 2
//-----------------------------------------------------------------------------
extern void AppendMICRTrailTA( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends FULL MICR with 'T', 'A' and 'C'
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void MoveMICRTAC( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Primary Account Number (PAN) manually entered to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendPanMan( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Primary Account Number (PAN) from Track 2 data to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendPanTrack2( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends 10-digit telephone number to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendPhone( void );


//-----------------------------------------------------------------------------
//! \brief
//!     Appends 7-digit telephone number to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendPhone7( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Check Routing to send data - TELECHECK
//!
//! \return
//!     None.
//!
//! \note
//!     Special TeleCheck code substitutes the constant string "X.A01000"
//!     for the routeing info.
//-----------------------------------------------------------------------------
extern void AppendRoutingTC( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends State (two characters) to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendState( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends card swipe indicator ( WCC )'S' or '@'
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendSwipe( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Visa Terminal ID to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendVisaTID( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends a Visa TPDU if configured for HDLC to send data
//!
//! \return
//!     True if Visa TPDU appended otherwise False
//-----------------------------------------------------------------------------
extern Bool AppendVisaTPDU( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends Write Control Character to send data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void AppendWrtCtrlChar( void );

//-----------------------------------------------------------------------------
//! \brief
//!     Appends 5-digit zip code to send data
//!
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void AppendZip5( void );

#endif // _HOSTUTIL_H_

