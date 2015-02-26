
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             getkey.c
//      Keyboard routines
//
//=============================================================================

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkgui.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "msg.h"
#include "util.h"
#include "util2.h"

#include "getkey.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================
#define DISABLED '\x7f'			// KEY is not enabled

//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static KEYMASK const NUMKEYS = {
	DISABLED, DISABLED, UP_KY,    DOWN_KY, CANCEL_KY, SELECT_KY, DISABLED,
	DISABLED, DISABLED, '3',      '2',      '1',      DISABLED, DISABLED,
	DISABLED, DISABLED, '6',      '5',      '4',      DISABLED, DISABLED,
	DISABLED, DISABLED, '9',      '8',      '7',      DISABLED, DISABLED,
	DISABLED, DISABLED, ENTER_KY, '0',      CLEAR_KY, DISABLED, DISABLED,
	DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,

    DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
	DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
	DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
	DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
	DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
	DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED
};



//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  OpenKeyPad      Call SDK function starting to read keypad.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void OpenKeyPad( void )
{
	SDK_StartReadKey( KeyBuffer, &NUMKEYS, &KeyStatus );
}

//=============================================================================
// Private function definitions
//=============================================================================
