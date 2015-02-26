#ifndef RSPINIT_H_
#define RSPINIT_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2006
//=============================================================================
//
// Module overview:             rspinit.h
//      Initialize Response Parsing Routines.
//
//=============================================================================

//=============================================================================
// External variables / declarations
//=============================================================================
extern UBYTE *p60Data;

extern void RspInit( OS_RETURN_VALS ReqCommStatus );
extern void FontInit( void );
extern void StoreTabData( UBYTE *pTabData );

#ifdef MAKE_SMARTINIT
extern UBYTE Build_I1_hash ( void );
extern UBYTE CRIndex;
#endif // MAKE_SMARTINIT

#endif    // end of "ifndef RSPINIT_H_"

