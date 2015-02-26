
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             tabutil.h
//      Table formatting functions
//
//=============================================================================

extern UBYTE ValidateTable( UBYTE *pTabFMT, UBYTE *pTabData, UWORD len,
                            UBYTE *pTabLRC );
extern UBYTE ValidateTable_NC( UBYTE *pTabFMT, UBYTE *pTabData, UWORD len,
                            UBYTE *pTabLRC );
extern void InitTable( UBYTE * pFrmFlag, UBYTE * pTab, UWORD len,
					   UBYTE * pLRC );
extern void P60_0810_IdxPre( UBYTE *pdest, UWORD szeRec, UBYTE numRec );
