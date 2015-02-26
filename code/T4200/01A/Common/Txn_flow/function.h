//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             function.h
//      Function module
//
//=============================================================================
//=============================================================================
//! Handle Merchant Function Selection
extern void MerchFunction( void );
//! Test Transaction
extern void Fun_0( void );
//! Merchant Programming
extern void Fun_2( void );
//! Restart Terminal
extern void Fun_3( void );
//! Lock Keyboard
extern void Fun_4( void );
//  Set DATE/TIME
extern void Fun_10( void );
//! ICEPAK graphics download
extern void Fun_14( void );
//! Accept input for the receipt/report header/footer
extern void Fun_26( void );
//! Printer Enable/Disable
extern void Fun_71( void );
//! Display Test
extern void Fun_83( void );
//! Keyboard Test
extern void Fun_84( void );
//! Cardreader Test
extern void Fun_89( void );
//! Password Change
extern void Fun_90( void );
//! Show Software Version
extern void Fun_91( void );
//  Program Load
extern void Fun_92( void );
//  Logon to get working key
extern void Fun_93( void );
//! PinPad TEST Message
extern void Fun_94( void );
//! Local Configuration
extern void Fun_98( void );
//! Maintenance - Clear tables and journal.
extern void Fun_99_9_1( void );
//! Maintenance - Clear acquirer
extern void Fun_99_9_2( void );
//! Maintenance - Clear all reversals from the journal.
extern void Fun_99_9_6( void );
//! Maintenance - Clear file system.
extern void Fun_99_9_8( void );
//! Print Host List
extern void Fun_99_96( void );

//! Input Terminal Serial Number
extern void XInputTermSer( void );
//! input Initialization Telephone Number
extern void XInputInitTel( void );
//! Input NMS Telephone Number
extern void XInputNMSTel( void );
//! Input Dial Mode
extern void XInputDialMode( void );
//! input PABX Access Code
extern void XInputPABX( void );
//! Do all terminal setup steps
extern void TermAll( void );
//! Local Options Select for passwords
extern void PasswordOptions( void );
//! Reprint function
extern void Reprint( void );
//! Local Options Select for Tip Percentages
extern void TipPercentages( void );
//! Prompt the user to either view or Print the Settlement Status.
extern void SettleStatus( void );

//! Input NMS Preferred Service
extern BYTE InputDBNMSPref( void );
//! Input Init Preferred Service
extern BYTE InputDBInitPref( void );
//! Input Dial Backup Initialization Telephone Number
extern BYTE InputDBNMSTel( void );
//! Input Dial Backup Initialization Telephone Number
extern BYTE InputDBInitTel( void );
// Exit application
extern void ExitApplication( void );
