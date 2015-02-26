
#ifndef _UTIL2_H_
#define _UTIL2_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             util2.h
//      Utility functions
//
//=============================================================================

extern UBYTE InInvoiceNum( void );
extern Bool InDupCheck( void );
extern void InShift( void );
extern Bool ReadData( void );
extern Bool ReadDataPrompt( enum msg_id Mid );


#endif // _UTIL2_H_
