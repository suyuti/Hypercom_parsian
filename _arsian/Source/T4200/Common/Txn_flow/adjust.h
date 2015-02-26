
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:     adjust.h
//      Functions used to implement Adjust Transaction
//
//=============================================================================
//! Adjust Transaction
extern void Adjust( void );
//! Perform the Adjust
extern void DoAdjust( void );
//! Adds up the Amounts and stores a new TRINP.TRTOTAM
extern Bool SaveAdjust( UBYTE * pSaveAmount );
//! Copy Amounts to temporary buffers
extern void CopyAmounts( void );

extern UBYTE NEWBAR[S_TRBASE];	// Adjust temporary Amount 
extern UBYTE NEWBASE[S_TRBASE];	// Adjust temporary Amount 
extern UBYTE NEWTAX[S_TRBASE];	// Adjust temporary Amount 
extern UBYTE NEWTIP[S_TRTIP];	// Adjust temporary Amount 
extern UBYTE NEWTOTAM[S_TRTOTAM];	// Adjust temporary Amount 
extern UBYTE NEWTRST1;			// Copy of first byte of TRSTATUS (for TS1_TIPENT) 
