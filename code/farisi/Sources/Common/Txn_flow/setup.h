
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//

// Module overview:             setup.h
//      Screen/Printer setup functions
//
//=============================================================================
//=============================================================================
//! Entry point for setting up terminal.
extern void TerminalSetup( void );
//! Performs line feeds as required.
extern void PaperFeed( void );
//! Screen to set display contrast, also sets global variable TERM.TERMContrast.
extern void SetContrast( void );
