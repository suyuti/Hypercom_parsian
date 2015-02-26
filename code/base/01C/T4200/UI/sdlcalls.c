//=============================================================================
//
// \file
//		sdlcalls.c
//	\brief
//		Function calls for using Screen Design Language
//
//=============================================================================

#include "basictyp.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "sdkmem.h"
#include "screen.h"
#include "sdl.h"
#include "utllib.h"
#include "disproc.h"
#include "menu.h"


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
//!      Sets the data for the input box to be used when screen is drawn
//!
//! \param
//!		text		pointer to text to be placed in box
//!				
//! \return
//!     None
//!
//! \note
//!		This function simply sets the input box buffer - 
//!		does not update the screen. Usually used to set up the
//!		default text for the input box before the screen is	displayed.
//!
extern void SetInputBox( char *text )
{
	SDLparms.pText = text;
	SDLparms.Page = SDK_ThisPage(  );
	SDLparms.len = StrLn( text, sizeof( CSTMSG ) );
	XSetInputBox ();
}

//-----------------------------------------------------------------------------
//!
//!      Updates data for the input box on current screen
//!
//! \param
//!		text		pointer to text to be placed in box
//!				
//! \return
//!     None
//!
//! \note
//!		This functions will directly update the screen. The
//!		does not need to be re-drawn. If there is no input 
//!		box on the screen the function on changes the 
//!		input box buffer.
//!
extern void UpdateInputBox( char *text )
{
	SDLparms.pText = text;
	SDLparms.Page = SDK_ThisPage(  );
	SDLparms.len = StrLn( text, sizeof( CSTMSG ) );
	XUpdateInputBox ();
}

//-----------------------------------------------------------------------------
//!
//!      Draws screen based on given screen number as defined in screen.h
//!
//! \param
//!		ScreenNumber		screen number or ID as defined in screen.h
//!				
//! \return
//!     None
//!
//! \note
//!
extern void DrawScreen( SCREEN_ID ScreenNumber )
{
	SDLScreenId = ScreenNumber;
	XDrawScreen ();
}

//-----------------------------------------------------------------------------
//!
//!      Sets the maximum characters which can be displayed	in the input box
//!
//! \param
//!		maxchars		desired maximum characters
//!				
//! \return
//!     None
//!
//! \note
//!
extern void SetInputBoxMaxChars( UBYTE maxchars )
{
	SDLparms.len = maxchars;
	XSetInputBoxMaxChars ();
}

//-----------------------------------------------------------------------------
//!
//!      Draws menu based on given menu number defined in transdef.h and menu.c
//!
//! \param
//!		MenuNumber			menu number or ID
//!				
//! \return
//!     None
//!
//! \note
//!
extern void DrawMenu( MENU_ID MenuNumber )
{
	SDLScreenId = MenuNumber;
	XDrawMenu ();
}

//-----------------------------------------------------------------------------
//!
//!      Draws a screen based on pointer to an SDL script
//!
//! \param
//!		pSDLScript			pointer to SDL command array
//!				
//! \return
//!     None
//!
//! \note
//!
extern void DrawSDLScript( UWORD * pSDLScript )
{
	pTableStart = ( UBYTE * ) pSDLScript;	// pointer to HDT Screen Language command array
	TablePage = SDK_ThisPage(  );			// page of caller
	XDrawSDLScript ();
}

//-----------------------------------------------------------------------------
//!
//!      Sets the current window
//!
//! \param
//!		pWin			pointer to SDL window structure
//!				
//! \return
//!     None
//!
//! \note
//!		SDL drawing commands will operate relative to window
//!		SDL commands WINDOW, SET_LOGICAL_WINDOW_SIZE, 
//!		RESET_LOGICAL_WINDOW_SIZE, and DEFAULT_WINDOW
//!		will override window paramters set by this function.
//!
extern void SetSDLWindow( SDLWINDOW * pWin )
{
	pTableStart = ( UBYTE * ) pWin;	// pointer to window parameters
	TablePage = SDK_ThisPage(  );	// page of caller
	XSetSDLWindow ();
}

//-----------------------------------------------------------------------------
//!
//!      Sets the window used by SDL to full screen size 
//!		 and sets logical pixel size equal to physical pixel size
//!
//! \param
//!				
//! \return
//!     None
//!
//! \note
//!
extern void SetDefaultSDLWindow( void )
{
	XSetDefaultSDLWindow ();
}

//-----------------------------------------------------------------------------
//!
//!      Updates data for given text area on current screen
//!
//! \param
//!		id			id of the text area to update
//! \param
//!		text		pointer to text to be placed in box
//!				
//! \return
//!     None
//!
//! \note
//!		This functions will directly update the screen. The
//!		does not need to be re-drawn. If the given
//!		text box does not exist, then there is no change.
//!
extern void UpdateTextArea( TEXTAREA_ID id, char *text )
{
	SDLparms.pText = text;
	SDLparms.Page = SDK_ThisPage(  );
	SDLparms.len = StrLn( text, sizeof( CSTMSG ) );
	SDLparms.id = id;
	XUpdateTextArea ();
}


//=============================================================================
// Private function definitions
//=============================================================================
