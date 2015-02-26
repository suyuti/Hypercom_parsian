
#ifndef _DATETIME_H_
#define _DATETIME_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             datetime.h
//      Date and time support functions
//
//=============================================================================
//! Format the Date & Time string and store in user buffer
extern void FormatDateTime( void );
//! Convert the date and time to ASCII and return in CSTMSG5
extern void RptDateTime( void );
//! Get the Month string
extern void GetMonth( UBYTE ByteMonth );

#endif // _DATETIME_H_
