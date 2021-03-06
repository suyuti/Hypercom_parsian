//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _HYPERTCF_H_
#define _HYPERTCF_H_
//=============================================================================
//
// Module overview:             hypertcf.h
//      Hypercom transaction control framework header
//

//=============================================================================
//=============================================================================
//=============================================================================
// Public defines and typedefs
//=============================================================================
#define ST_IDLE         0
#define ST_MENUFUNC     1
#define ST_ICEPAC       2
#define ST_CARDSWIPE    3
#define ST_MANUAL       4
#define ST_TRANSACTION  5



//=============================================================================
// Public data declarations
//=============================================================================

//=============================================================================
// Public function declarations
//=============================================================================
extern void INITTCFW( void );
extern UBYTE BuildPAN_EXP( void );


#endif // _HYPERTCF_H_
