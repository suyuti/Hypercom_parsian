
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             void.h
//      Void Transaction.
//
//=============================================================================
//=============================================================================
//! Void transaction processing.
extern void Voidtran( void );
//! Void transaction processing with jrn identifier set.
extern void VoidIdent( UWORD VoidJrnIdent );
extern void PreAuthCancel();
extern void PreVoidtran();
extern void PreAuthClosedCancel();

