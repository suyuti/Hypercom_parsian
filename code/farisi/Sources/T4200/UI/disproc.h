//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      disproc.h
//
// File Description:
//      Display process header file.
//
//=============================================================================
//! Entry point for the Display Proccessor
extern void INITDISP( void );
//! Initializes the screen cross reference table
extern void InitScreenCrossRef( void );
//! Draws screen based on given screen number
extern void XDrawScreen( void );
//! Calculates the maximum number of characters that can be displayed on one line
extern void MaxCharDisp( void );
//! Draws a screen based on pointer to an SDL script
extern void XDrawSDLScript( void );
//! Perform initializations as required at application startup for SDL and menu system
extern void InitSDL( void );

