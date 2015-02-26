
#ifndef _RSPTEXTINIT_H_
#define _RSPTEXTINIT_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             rsptextinit.h
//      Response text initialization functions
//
//=============================================================================


//=============================================================================
// Public defines and typedefs
//=============================================================================


//=============================================================================
// Public data declarations
//=============================================================================



//=============================================================================
// Public function declarations
//=============================================================================
//! Add English Response table for DML support
extern void RspTextEn_Init( void );
//! Add German Response table for DML support
extern void RspTextGr_Init( void );
//! Add Swedish Response table for DML support
extern void RspTextSw_Init( void );
//! Add Spanish Response table for DML support
extern void RspTextEs_Init( void );
//! Add Portuguese Response table for DML support
extern void RspTextPt_Init( void );

#ifdef MAKE_FARSI
//! Add Farsi Response table for DML support
extern void RspTextFa_Init( void );
#endif

//! Add Response messages tables for DML support
extern void RspText_Init( void );


#endif // _RSPTEXTINIT_H_
