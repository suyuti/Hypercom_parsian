//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//
// Module overview:             sertrce.h
//      serial trace interface
//
//=============================================================================
extern void Trace_Start( void );
extern void Trace_Stop( void );
extern void Trace_Print( void );
extern void Trace_Send_COM1( void );
extern void Trace_Send_COM2( void );
extern Bool Trace_Check( void );
extern Bool Trace_ChkOverflow( void );
extern void Trace_Start_Mask( void );
