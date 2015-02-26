//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:     acctproc.h
//      Functions used to implement the Account Process functionality
//
//=============================================================================
//=============================================================================
//! Do acct validation & card range check
extern UBYTE AcctProc( void );
//! Perform MOD10 on account number
extern UBYTE ValidateAcct( void );
