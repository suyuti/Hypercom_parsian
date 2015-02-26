
#ifndef CRTAB_H_
#define CRTAB_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             crtab.h
//      Card range table funtionality
//
//=============================================================================

extern void CRTabPre ( void );
extern void P60_LDIR_CRTab ( void );
extern void SetCRTabLRC( void );
extern void FindCardRange( void );
extern void FindCardRangeById ( void );
extern struct card_rec * cr_offset ( UBYTE index );
extern void MoveItCR( void * pDest, const void * pSource, UWORD  Bytes );
extern UBYTE FindCR ( void );
extern struct card_rec * FindCR_ById ( UBYTE id );


#endif    // end of "ifndef CRTAB_H_" 
