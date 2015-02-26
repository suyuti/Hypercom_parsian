
#ifndef PDNTAB_H_
#define PDNTAB_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             pdntab.h
//      PDN table functionality
//
//=============================================================================


extern void PDNTabPre ( void );
extern void P60_LDIR_PDNTab ( void );
extern void SetPDNTabLRC( void );
extern PDNSTRUCT * FindPDN(UBYTE acquirer_id);


#endif    // end of "ifndef PDNTAB_H_" 
