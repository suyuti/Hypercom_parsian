
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             prtdata.h
//      Print a raw (HEX) Data Structure
//
//=============================================================================

extern void PrintData( enum msg_id Title, UBYTE * pData, short Length,
					   Bool FormFeed );

extern char LineBuffer[S_LineBuffer];
