//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             utilfunc.h
//      Generic User Interface functions.
//
//=============================================================================
//=============================================================================
//! Accepts input from user & saves in Dspbuf.
extern void EntryScreen( struct ESParms_str *pESParms );
//! Accepts input from virtual Pin Pad & saves in Dspbuf.
extern void VPP_EntryScreen( struct ESParms_str *pESParms, UBYTE* dest );
//! Accept YES or NO from user.
extern void XYesNoEntry( struct ESParms_str *pESParms );
//! Accept Continue or Cancel from user
extern void XContinueEntry( struct ESParms_str *pESParms );
//! Accept Continue or Cancel from user
extern void XContinueEntryTimeout(struct ESParms_str *pESParms);
//! Accept user input for amount.
extern void XAmountEntry( struct ESParms_str *pESParms );
//! Accept YES or NEXT from user.
extern void XYesNextEntry( struct ESParms_str *pESParms );
