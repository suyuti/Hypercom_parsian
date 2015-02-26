
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             hstjs.h
//      Host module journal select routines
//
//=============================================================================

// Public functions defined in this module
extern Bool HostJRead( void );
extern Bool HostJSFirst( void );
extern Bool HostJSNext( void );
#ifdef	MAKE_EMV
extern Bool HostJSPrev( void );
#endif	// MAKE_EMV
extern void DefHostJSRec( void );

