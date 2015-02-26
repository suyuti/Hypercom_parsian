//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             prtdrv.h
//      Print driver routines header file
//
//=============================================================================
extern void PrintInit( void );
extern void PrintFormFeed( void );
extern Bool PrintDriver( enum REPORTTYPE Report,
						 struct trandata_rec *pTranData );
extern Bool FlushBuffers( char appendChar );

extern UBYTE PresetPRFLAG;

