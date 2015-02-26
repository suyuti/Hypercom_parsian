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
#ifdef MAKE_VIKING
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
#elif defined( MAKE_OPTIMUM )
    DISABLED, CANCEL_KY, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
    DISABLED, DISABLED,  '9',      '8',      '7',      DISABLED, DISABLED,
    DISABLED, BSP_KY,    '6',      '5',      '4',      DISABLED, DISABLED,
    DISABLED, CLEAR_KY,  '3',      '2',      '1',      DISABLED, DISABLED,
    DISABLED, ENTER_KY,  DISABLED, '0',      DISABLED, DISABLED, DISABLED,
    DOWN_KY,  SELECT_KY, UP_KY,    DOWN_KY,    SELECT_KY, UP_KY,

    DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
    DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
    DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
    DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
    DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED, DISABLED,
    DOWN_KY,  SELECT_KY, UP_KY,   DOWN_KY,  SELECT_KY, UP_KY
#endif
};


//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//!  \brief     Call SDK function starting to read keypad.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void OpenKeyPad( void )
{
	SDK_StartReadKey( KeyBuffer, &NUMKEYS, &KeyStatus );
}

//=============================================================================
// Private function definitions
//=============================================================================
