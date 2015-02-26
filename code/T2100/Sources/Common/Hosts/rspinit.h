
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             rspinit.h
//      Initialize Response Parsing Routines.
//
//=============================================================================

extern void RspInit( OS_RETURN_VALS ReqCommStatus );
extern void FontInit( void );

#ifdef MAKE_SMARTINIT
extern UBYTE Build_I1_hash ( void );
extern UBYTE CRIndex;
#endif // MAKE_SMARTINIT
