
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

//! Input invoice number or use default from terminal.
extern UBYTE InInvoiceNum( void );
//! If option set check for duplicate trans in batch.
extern Bool InDupCheck( void );
//! Set up shift number if shift enabled.
extern void InShift( void );
//! Get card or keypad data.
extern Bool ReadData( void );
//! Get card or keypad data.
extern Bool ReadDataPrompt( enum msg_id Mid );


#endif // _UTIL2_H_
