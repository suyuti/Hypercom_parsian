//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      font.h
//
// File Description:
//      Declaration of C functions dealing with Nucleus fonts.
//
//=============================================================================
#ifndef _FONT_H_
#define _FONT_H_

//=============================================================================
// Public defines and typedefs
//=============================================================================


//=============================================================================
// Public data declarations
//=============================================================================

//=============================================================================
// Public function declarations
//=============================================================================
 
typedef enum
{
	FNT_6X10,
	FNT_6X12,
	FNT_6X13,
	FNT_8X13,
	FNT_8X13BOLD,
	FNT_9X15,
	FNT_CHICAM12,
	FNT_CHICAM15,
	FNT_CRTOON16,
	FNT_CURSOR,
	FNT_HELVB15,
	FNT_HELVM12,
	FNT_HELVM15,
	FNT_VT9X13,
}fonts_;

#ifdef __cplusplus
extern "C" {
#endif
//! Gets pointer to an Nucleus font
extern void* GetNucleusFont( fonts_ );
//! Unloads Nucleus font from memory
extern void UnloadNucleusFont( );
 
#ifdef __cplusplus
}
#endif
#endif // _FONT_H_
