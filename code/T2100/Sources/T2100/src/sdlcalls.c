
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             sdlcalls.c
//      Function calls for using Screen Design Language
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
//	SetInputBox		Sets the data for the input box to be used when
//					screen is drawn.
//
//	Parameters:		char * text		pointer to text to be placed in box
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			This function simply sets the input box buffer - 
//					does not update the screen. Usually used to set up the
//					default text for the input box before the screen is
//					displayed.
//
//-----------------------------------------------------------------------------

extern void SetInputBox( char *text )
{
	SDLparms.pText = text;
	SDLparms.Page = SDK_ThisPage(  );
	SDLparms.len = StrLn( text, sizeof( CSTMSG ) );
	XSetInputBox ();
}


//-----------------------------------------------------------------------------
//	UpdateInputBox		Updates data for the input box on current screen
//
//	Parameters:			char * text		pointer to text to be placed in box
//
//	Global Inputs:		None.
//
//	Returns:			Nothing.
//
//	Notes:				This functions will directly update the screen. The
//						does not need to be re-drawn. If there is no input 
//						box on the screen the function on changes the 
//						input box buffer.
//
//-----------------------------------------------------------------------------

extern void UpdateInputBox( char *text )
{
	SDLparms.pText = text;
	SDLparms.Page = SDK_ThisPage(  );
	SDLparms.len = StrLn( text, sizeof( CSTMSG ) );
	XUpdateInputBox ();
}


//-----------------------------------------------------------------------------
//	DrawScreen		Draws screen based on given screen number as 
//					defined in screen.h
//
//	Parameters:		SCREEN_ID * ScreenNum	screen number or ID as defined 
//											in screen.h
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

extern void DrawScreen( SCREEN_ID ScreenNumber )
{
	SDLScreenId = ScreenNumber;
	XDrawScreen ();
}


//-----------------------------------------------------------------------------
//	SetInputBoxMaxChars		Sets the maximum characters which can be displayed
//							in the input box
//
//	Parameters:				UBYTE maxchars		desired maximum characters
//
//	Global Inputs:			None.
//
//	Returns:				Nothing.
//
//	Notes:					None.
//
//-----------------------------------------------------------------------------

extern void SetInputBoxMaxChars( UBYTE maxchars )
{
	SDLparms.len = maxchars;
	XSetInputBoxMaxChars ();
}


//-----------------------------------------------------------------------------
//	DrawMenu		Draws menu based on given menu number defined in 
//					transdef.h and menu.c
//
//	Parameters:		MENU_ID MenuNumber		menu number or ID
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

extern void DrawMenu( MENU_ID MenuNumber )
{
	SDLScreenId = MenuNumber;
	XDrawMenu ();
}


//-----------------------------------------------------------------------------
//	DrawSDLScript		Draws a screen based on pointer to an SDL script
//
//	Parameters:			UWORD * pSDLScript		pointer to SDL command array
//
//	Global Inputs:		None.
//
//	Returns:			Nothing.
//
//	Notes:				None.
//
//-----------------------------------------------------------------------------

extern void DrawSDLScript( UWORD * pSDLScript )
{
	pTableStart = ( UBYTE * ) pSDLScript;	// pointer to HDT Screen Language command array
	TablePage = SDK_ThisPage(  );			// page of caller
	XDrawSDLScript ();
}


//-----------------------------------------------------------------------------
//	SetSDLWindow	Sets the current window
//
//	Parameters:		SDLWINDOW * pWin	pointer to SDL window structure
//
//	Global Inputs:	None.
//
//	Returns:		Bool	True  - okay to continue,
//							False - reprompt for a different Amount.
//
//	Notes:			SDL drawing commands will operate relative to window
//					SDL commands WINDOW, SET_LOGICAL_WINDOW_SIZE, 
//					RESET_LOGICAL_WINDOW_SIZE, and DEFAULT_WINDOW
//					will override window paramters set by this function.
//
//-----------------------------------------------------------------------------
extern void SetSDLWindow( SDLWINDOW * pWin )
{
	pTableStart = ( UBYTE * ) pWin;	// pointer to window parameters
	TablePage = SDK_ThisPage(  );	// page of caller
	XSetSDLWindow ();
}


//-----------------------------------------------------------------------------
//	SetDefaultSDLWindow		Sets the window used by SDL to full screen size 
//							and sets logical pixel size equal to physical 
//							pixel size
//
//	Parameters:				None.
//
//	Global Inputs:			None.
//
//	Returns:				Nothing.
//
//	Notes:					None.
//
//-----------------------------------------------------------------------------
extern void SetDefaultSDLWindow( void )
{
	XSetDefaultSDLWindow ();
}


//-----------------------------------------------------------------------------
//	UpdateTextArea		Updates data for given text area on current screen
//
//	Parameters:			TEXTAREA_ID id	id of the text area to update
//						char * text		pointer to text to be placed in box
//
//	Global Inputs:		None.
//
//	Returns:			Bool	True  - okay to continue,
//								False - reprompt for a different Amount.
//
//	Notes:				This functions will directly update the screen. The
//						does not need to be re-drawn. If the given
//						text box does not exist, then there is no change.
//
//-----------------------------------------------------------------------------
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
