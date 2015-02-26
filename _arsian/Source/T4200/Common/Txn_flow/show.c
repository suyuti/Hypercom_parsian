
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             show.c
//      General "Show Information" functions.
//
//=============================================================================

#include "string.h"
#include "stdlib.h"

#include "hw.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "fixdata.h"

#include "struct.h"
#include "appdata.h"
#include "defines.h"
#include "msg.h"
#include "screen.h"
#include "show.h"
#include "util.h"
#include "utllib.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!	\brief
//!     Display Software Version.
//!
//!	\param
//!     None.
//!
//!	\return
//!	    None
//!
//-----------------------------------------------------------------------------
extern void ShowSWVersion( void )
{
    UWORD           nMaxMove= 25;

    // Display the software version 
	memcpy( CSTMSG, VERSION, 10 );
	CSTMSG[10] = '\0';

    // Display receipt line 1.
    ClrIt( (UBYTE*)CSTMSG2, sizeof(CSTMSG2) );
    MovTrim( CSTMSG2, (char*)TCONF.TCNAM1, nMaxMove );

    // Display receipt line 2.
    ClrIt( (UBYTE*)CSTMSG3, sizeof(CSTMSG3) );
    MovTrim( CSTMSG3, (char*)TCONF.TCNAM2, nMaxMove );

    // Display receipt line 3.
    ClrIt( (UBYTE*)CSTMSG4, sizeof(CSTMSG4) );
    MovTrim( CSTMSG4, (char*)TCONF.TCNAM3, nMaxMove );

    // Display Screen 
	DrawScreen( SOFT_VER_SCR );
}


//=============================================================================
// Private function definitions
//=============================================================================

