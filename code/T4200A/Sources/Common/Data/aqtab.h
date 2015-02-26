
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             aqtab.h
//      Acquirer table functions and data declarations
//
//=============================================================================

#ifndef AQTAB_H_
#define AQTAB_H_

extern void AqTabPre ( void );
extern void P60_LDIR_AqTab ( void );
extern void SetAQTabLRC( void );
extern void MoveItAq( void * pDest,	const void * pSource, UWORD  Bytes );
extern void MoveIt2Aq(  void * pDest, const void * pSource, UWORD  Bytes );
extern struct acquirer_rec *FindAcquirer( UBYTE acquirer_id );
extern struct acquirer_rec *FindFirstAcquirer( void );
extern struct acquirer_rec *FindNextAcquirer( struct acquirer_rec
											  *pCurrentAcquirer );
extern struct acquirer_rec *FindPrevAcquirer( struct acquirer_rec *pCurrentAcquirer );
extern char AqidCount( void );


#endif    // end of "ifndef AQTAB_H_" 
