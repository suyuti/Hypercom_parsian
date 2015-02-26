/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
//! \file sdkdsp.h
//!     \brief SDK functions to handle the DISPLAY device.
//!     \since 2002
//!

#ifndef _SDKDSP_H_
#define _SDKDSP_H_

//=============================================================================
//
// Module overview:             SDKDSP.h
//      SDK functions to handle the DISPLAY device.
//
//=============================================================================

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup GroupSDK
	@{
*/

//=============================================================================
// Public defines and typedefs
//=============================================================================

#ifdef MAKE_T7PLUS

/**
	@brief Enumeration for identifying different display types.
*/
typedef enum
{
	dt_NORMAL,			///< Text Only 2x20 Display, Built-In Text Font
	dt_GRAPHICS,		///< Text & Graphics 128x64 Display (8 Line), Built-In Text Font
	dt_GPH4LINE,		///< Graphics Only 140x32 Display (4 Line), Text Font supplied
	dt_NORMAL4LINE,		///< Text Only 4x20 Display
	dt_ST7920			///< Text & Graphics 140x32 Display (4 Line), Built-In Chinese Font
} DISPLAYTYPE;

#else

/**
	@brief Enumeration for identifying different display types.
*/
typedef enum
{
	dt_NONE,			///< Display does Not Exist
	dt_GPH10LINE = 0x03,///< Graphics Only 160x80 Display (10 Line)
	dt_GPH20LINE,		///< Graphics Only 160x160 Display (20 line)
	dt_CQVGA			///< Color 1/4 VGA Graphics Only 320x240 (30 Line)
} DISPLAYTYPE;

#endif

#if !defined(MAKE_ICE6500) &&  !defined(MAKE_ICE6000)

/**
	@brief Enumeration for configuring display with different contrasts.
*/
typedef enum
{
    dc_LEVEL0,			///< Lowest contrast setting
    dc_LEVEL1,			//
    dc_LEVEL2,			//
    dc_LEVEL3,			//
    dc_LEVEL4,			//
    dc_LEVEL5,			//
    dc_LEVEL6,			//
    dc_LEVEL7			///< Highest contrast setting
} DISPCONTRAST;

#endif	// !( MAKE_ICE6500) && !( MAKE_ICE6000 )

//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================

#if defined(MAKE_ICE6500) || defined(MAKE_ICE6000)

//-----------------------------------------------------------------------------
//! \brief Sets the contrast on the display
//!
//! \param[in]  Level           level of contrast (0-255)
//!
//! \return orvOK on success else OS error value
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS SDK_DisplayContrast( UBYTE Level );

#else

//-----------------------------------------------------------------------------
//! \brief Sets the contrast on the display
//!
//! \param[in]  Level           level of contrast
//!
//! \return orvOK on success else OS error value
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS SDK_DisplayContrast( DISPCONTRAST Level );

#endif	// ( MAKE_ICE6500) || ( MAKE_ICE6000 )

//-----------------------------------------------------------------------------
//! \brief Gets the type of the display
//!
//! \return returns display type
//!
//! \see DISPLAYTYPE
//-----------------------------------------------------------------------------
extern DISPLAYTYPE SDK_DisplayType( void );


//-----------------------------------------------------------------------------
//! \brief Turns backlight on or off
//!
//! \param[in]  On              turn off backlight if zero else turn on
//!
//! \return orvOK on success else OS error value
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS SDK_DisplayBackLight( UBYTE On );

/**	@example SDK_DisplayBackLight.c */

//-----------------------------------------------------------------------------
//! \brief Displays graphic image starting with specified line
//!
//! \param[in]  pGraphics       pointer to uncompressed graphic image
//! \param[in]  Line            line to display image on
//! \param[in]  Length          length of the image
//!
//! \return orvOK on success else OS error value
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS SDK_DisplayGraphics(	UBYTE *pGraphics,
						UBYTE Line,
						UWORD Length );
/**	@example SDK_DisplayGraphics.c */

//-----------------------------------------------------------------------------
//! \brief Displays text on the graphis screen
//!
//! \param[in]  pText           pointer to text to display
//! \param[in]  Line            line to display it on
//! \param[in]  Column          column to display it on
//! \param[in]  Length          length of the data
//!
//! \return orvOK on success else OS error value
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS SDK_DisplayGrphText(	char *pText,
						UBYTE Line,
						UBYTE Column,
						UWORD Length );
/**	@example SDK_DisplayGrphText.c */

//-----------------------------------------------------------------------------
//! \brief Displays text on the lower line
//!
//! \param[in]  pText           pointer to text to display
//!
//! \return orvOK on success else OS error value
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS SDK_DisplayLower( char *pText );


//-----------------------------------------------------------------------------
//! \brief Resets the display
//!
//! \return orvOK on success else OS error value
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS SDK_DisplayReset( void );

//-----------------------------------------------------------------------------
//! \brief Displays text on the upper line
//!
//! \param[in]  pText           pointer to text to display
//!
//! \return orvOK on success else OS error value
//-----------------------------------------------------------------------------
extern OS_RETURN_VALS SDK_DisplayUpper( char *pText );

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif	// _SDKDSP_H_
