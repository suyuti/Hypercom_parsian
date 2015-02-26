
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
//  Append2Chars        Appends two specified characters to send data.
//
//  Parameters:         char *Chars     pointer to array of two characters 
//                                      to append to send data.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void Append2Chars( char *Chars );

//-----------------------------------------------------------------------------
//  AppendAqAccID2      Appends first two characters of the Acceptor ID to 
//                      send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendAqAccID2( void );

//-----------------------------------------------------------------------------
//  AppendAqAccID3Plus      Appends the rest of the Acceptor ID after the
//                          first two characters to send data
//
//  Parameters:             None.
//
//  Global Inputs:          None.
//
//  Returns:                Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendAqAccID3Plus( void );

//-----------------------------------------------------------------------------
//  AppendCheckAmount       Appends amount to send data
//
//  Parameters:             None.
//
//  Global Inputs:          None.
//
//  Returns:                Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendCheckAmount( void );

//-----------------------------------------------------------------------------
//  AppendBirthDate         Appends Birth Date to send data
//
//  Parameters:             None.
//
//  Global Inputs:          None.
//
//  Returns:                Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendBirthDate( void );

//-----------------------------------------------------------------------------
//  AppendChar          Appends specified character to send data
//
//  Parameters:         char Char   character to append to send data.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendChar( char Char );

//-----------------------------------------------------------------------------
//  AppendCheckNum      Appends Customer's Check Number or the last four
//                      digits of the MICR# to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendCheckNum( void );

//-----------------------------------------------------------------------------
//  AppendCheckNumC     Appends check number with leading character 'C' 
//                      to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendCheckNumC( void );

//-----------------------------------------------------------------------------
//  AppendCheckNumP     Appends check number with leading character '#' 
//                      to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendCheckNumP( void );

//-----------------------------------------------------------------------------
//  AppendDrvLic        Appends Driver's License ID Number to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendDrvLic( void );

//-----------------------------------------------------------------------------
//  AppendExpDate       Appends Expiration Date to send data (Ascii MMYY)
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendExpDate( void );

//-----------------------------------------------------------------------------
//  AppendFS            Appends Field Separator (0x1c) to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendFS( void );

//-----------------------------------------------------------------------------
//  AppendMICR          Appends Customer's Check Reader Information to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendMICR( void );

//-----------------------------------------------------------------------------
//  AppendMICRTA        Appends Transit and Account Numbers with a
//                      prepended "T" and "A" to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendMICRTA( void );

//-----------------------------------------------------------------------------
//  AppendMICRTrailTA       Appends Transit and Account Numbers with a
//                          trailing "T" and "A" to send data
//
//  Parameters:             None.
//
//  Global Inputs:          None.
//
//  Returns:                Nothing.
//
//  Notes:
//                          Only call this if (TRCHECKF[0] & 0x58) == 0x50 
//                          or 0x48, making TRMICRfmt == 2
//
//-----------------------------------------------------------------------------

extern void AppendMICRTrailTA( void );

//-----------------------------------------------------------------------------
//  MoveMICRTAC         Appends FULL MICR with 'T', 'A' and 'C'
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void MoveMICRTAC( void );

//-----------------------------------------------------------------------------
//  AppendPanMan        Appends Primary Account Number (PAN) manually
//                      entered to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendPanMan( void );

//-----------------------------------------------------------------------------
//  AppendPanTrack2     Appends Primary Account Number (PAN) from Track 2 
//                      data to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendPanTrack2( void );

//-----------------------------------------------------------------------------
//  AppendPhone         Appends 10-digit telephone number to send data
//
//  Parameters:
//
//  Global Inputs:
//
//  Returns:
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendPhone( void );


//-----------------------------------------------------------------------------
//  AppendPhone7        Appends 7-digit telephone number to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendPhone7( void );

//-----------------------------------------------------------------------------
//  AppendRoutingTC     Appends Check Routing to send data - TELECHECK
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:              Special TeleCheck code substitutes the constant
//                      string "X.A01000" for the routeing info.
//
//-----------------------------------------------------------------------------

extern void AppendRoutingTC( void );

//-----------------------------------------------------------------------------
//  AppendState         Appends State (two characters) to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendState( void );

//-----------------------------------------------------------------------------
//  AppendSwipe         Appends card swipe indicator ( WCC )'S' or '@'
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendSwipe( void );

//-----------------------------------------------------------------------------
//  AppendVisaTID       Appends Visa Terminal ID to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendVisaTID( void );

//-----------------------------------------------------------------------------
//  AppendVisaTPDU      Appends a Visa TPDU if configured for HDLC to
//                      send data
//
//  Parameters:         None.
//
//  Global Inputs:
//
//  Returns:            Bool    True - Visa TPDU appended
//                              False - Visa TPDU not appended.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool AppendVisaTPDU( void );

//-----------------------------------------------------------------------------
//  AppendWrtCtrlChar       Appends Write Control Character to send data
//
//  Parameters:             None.
//
//  Global Inputs:          None.
//
//  Returns:                Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendWrtCtrlChar( void );

//-----------------------------------------------------------------------------
//  AppendZip5          Appends 5-digit zip code to send data
//
//  Parameters:         None.
//
//  Global Inputs:      None.
//
//  Returns:            Nothing.
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void AppendZip5( void );

#endif // _HOSTUTIL_H_
