
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             cashier.h
//      Cashier processing functions
//
//=============================================================================
//! Set the Checksum (LRC) for the Cashier Table.
extern void SetCashTabLRC( void );
//! Cashier Sign-On/Off
extern void Fun_77( void );
//! Cashier report
extern void Fun_78( void );
//! Sign on/off a cashier according to its status
extern UBYTE CashierStatus( void );
//! Sign off all cashiers in the CASHTAB
extern void Signoff_All_Cashiers( void );
//! Get user input of cashier number
extern BYTE GetCashier( Bool AllowNull );

