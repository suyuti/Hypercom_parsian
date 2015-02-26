
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             receipt.c
//      Receipt Support Functions header file.
//
//=============================================================================

extern void Receipt( void );
extern void PreprintReceipt( void );
extern void RctCapture( void );
extern Bool CaptureIt( char *SrcPtr, UWORD DataLen, Bool ForceIt );
extern Bool PrintCapture( enum REPORTTYPE Report, struct trandata_rec *pTranData );
extern void ClickKey( void );
