//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      font.cpp
//
// File Description:
//      Implmentation of external of C functions dealing with Nucleus fonts.
//
//=============================================================================
#include <HypCGrafixFont.hpp>
#include <HString.hpp>
#include "font.h"

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
static HypCGrafixFont* font = NULL;
//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Gets pointer to Nucleus font.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!		
//!
void* GetNucleusFont( fonts_ fnt )
{
	if( !font )
	{
		font = new HypCGrafixFont;
	}
	HString fontName;

	switch( fnt )
	{
		case FNT_6X10:
			fontName = "6X10.fnt";
			break;
		case FNT_6X12:
			fontName = "6X12.fnt";
			break;
		case FNT_6X13:
			fontName = "6X13.fnt";
			break;
		case FNT_8X13:
			fontName = "8X13.fnt";
			break;
		case FNT_8X13BOLD:
			fontName = "8X13BOLD.fnt";
			break;
		case FNT_9X15:
			fontName = "9X15.fnt";
			break;
		case FNT_CHICAM12:
			fontName = "CHICAM12.fnt";
			break;
		case FNT_CHICAM15:
			fontName = "CHICAM15.fnt";
			break;
		case FNT_CRTOON16:
			fontName = "CRTOON16.fnt";
			break;
		case FNT_CURSOR:
			fontName = "CURSOR.fnt";
			break;
		case FNT_HELVB15:
			fontName = "HELVB15.fnt";
			break;
		case FNT_HELVM12:
			fontName = "HELVM12.fnt";
			break;
		case FNT_HELVM15:
			fontName = "HELVM15.fnt";
			break;
		case FNT_VT9X13:
			fontName = "VT9X13.fnt";
			break;
		default:
			fontName = "";
	}

	if( font->Load( fontName.c_str() ) )
		return font->GetFontPtr( );
	return NULL;
}

//-----------------------------------------------------------------------------
//!
//!      Unloads Nucleus font.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!		
//!
void UnloadNucleusFont( )
{
	delete font;
	font = NULL;
}
