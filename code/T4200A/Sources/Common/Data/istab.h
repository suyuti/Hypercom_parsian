
#ifndef ISTAB_H_
#define ISTAB_H_

//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             istab.h
//      Issuer table functionality
//
//=============================================================================

extern void IsTabPre ( void );
extern void P60_LDIR_IsTab ( void );
extern void SetISTabLRC( void );
extern void MoveItIss( void * pDest, const void * pSource, UWORD  Bytes );
extern struct issuer_rec *FindIssuer( UBYTE issuer_id );

#endif    // end of "ifndef ISTAB_H_" 
