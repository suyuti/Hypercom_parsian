
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _I8583UTL_H_
#define _I8583UTL_H_
//=============================================================================
//
// Module overview:             i8583utl.h
//      ISO8583 utility defintions and data
//
//=============================================================================

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"

#include "struct.h"
#include "appdata.h"
#include "defines.h"
#include "transdef.h"
#include "msg.h"
#include "util.h"
#include "journal.h"
#include "utllib.h"


//=============================================================================
// Public defines and typedefs
//=============================================================================
#define RSPA_FLOOR  "03"		// Additional Resp Data for Floor Limit


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================
extern Bool Offline( void );
extern Bool Floor( void );
extern void CalcReauth( UBYTE * pResult, UBYTE * pAmount );
extern Bool Xfer( void );
extern void Trickle( void );
extern Bool SendAdvice( void );
extern void SetAdjustType( void );
extern void Upload( void );
extern void AllVoids( void );

extern void SendStatistics( void );

#endif // _I8583UTL_H_
