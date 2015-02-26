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
extern void EntryScreen( struct ESParms_str *pESParms );
extern void XYesNoEntry( struct ESParms_str *pESParms );
extern void XContinueEntry( struct ESParms_str *pESParms );
extern void XContinueEntryTimeout(struct ESParms_str *pESParms);
extern void XAmountEntry( struct ESParms_str *pESParms );
extern void XYesNextEntry( struct ESParms_str *pESParms );
extern int getFormattedLen( int len, int decimalLen );
extern void FormAmt( int cnt, char *pDest, const char *pBuf, char *CrrCode);
