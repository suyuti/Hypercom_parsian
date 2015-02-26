
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:     app2.h
//  Functions used on the second generic application page
//
//=============================================================================
//! Display last response data. Cross called when LASTRSP key pressed
extern void LastRsp( void );
//! Print Till Report
extern void DoPrtTill( void );
//! The function that handles the SAL+CASH transaction
extern void SalCash( void );
//! The function that handles the CASH transaction
extern void DbCash( void );
//! The function that handles the Balance Inquiry transaction
extern void BalInq( void );
//! Load DefaultTitle with acquired and transaction
extern void SetDefaultTitle( void );
extern void Logon(void);
extern void ChangeApplication( void );

