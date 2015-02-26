//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      runtime.c
//
// File Description:
//      Display run time error.
//
//=============================================================================

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "struct.h"
#include "appdata.h"

#include "sprite.h"
#include "runtime.h"

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
//!
//!      Report runtime error and halt.
//!
//! \param
//!		ErrCode	  error code  
//!				
//! \return
//!		None
//!
//! \note
//!
extern void RunTimeError( UWORD ErrCode )
{
	char codestr[5];

	DeleteAllSprites();
	SDK_GuiClearScr(  );

	GetMsg( Msg_ProgramError, Dspbuf );

	SDK_GuiText( 0, 0, Dspbuf );

	codestr[0] = ( char ) ( ( ( ErrCode & 0xf000 ) >> 12 ) | 0x30 );
	codestr[1] = ( char ) ( ( ( ErrCode & 0x0f00 ) >> 8 )  | 0x30 );
	codestr[2] = ( char ) ( ( ( ErrCode & 0x00f0 ) >> 4 )  | 0x30 );
	codestr[3] = ( char ) (   ( ErrCode & 0x000f )         | 0x30 );
	codestr[4] = 0;

	SDK_GuiText( 1, 0, codestr );
	SDK_GuiDispAll(  );

	// Loop forever
	while ( True )
	{
		SDK_RelinqCPU(  );
	}
}

//=============================================================================
// Private function definitions
//=============================================================================

