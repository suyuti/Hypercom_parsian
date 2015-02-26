//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:     app1.h
//  Functions used on the first application page
//
//=============================================================================
//=============================================================================
//! The function that really handles the SALE transaction.
extern void APP1SALE( void );
//! The function handles the Card Verification transaction
extern void APP1CARDVER( void );
//! Accept descriptor selection
extern UBYTE DoDesc( void );
//! Handles the REFUND transaction
extern void Refund( void );
//! Initialize the Terminal
extern void Initialize( void );
//! The function handles the Offline transaction
extern void SalOffl( void );
//! This function is used to indicate an unsupported function or transaction.
extern void FuncNotSupported( void );
//! The function handles the test transaction
extern void TstTran( void );
//! The function handles the Auth transaction
extern void Auth( void );
//! Save last response text from the host
extern void SaveResponseText( void );
//! Check for expired card
extern UBYTE CheckCardExp( void );
//! Check for display of track 2 data
extern UBYTE CheckDisplaySwipe( void );
//! Mail/Phone Order Transaction (Visanet Only)
extern void MailOrder( void );
//! Check for Manual Issue Number entry
extern UBYTE CheckIssueNum( void );
//! Restart the terminal via booter
extern void TerminalQuickReset( void );
