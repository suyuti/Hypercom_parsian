
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             disproc.c
//      display process for showing sprite images
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "tlib.h"
#include "fixdata.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "transdef.h"
#include "msg.h"
#include "pinpad.h"
#include "util.h"
#include "utllib.h"
#include "icepak.h"
#include "screen.h"
#include "sdl.h"
#include "sprite.h"
#include "disproc.h"
#include "menu.h"
#include "screentab.h"

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
SCREEN_CROSSREF ScreenCrossRef[SCREEN_MAX - SCREEN_START];


//=============================================================================
// Private data definitions
//=============================================================================
#define NUM_OF_SCREEN_PAGES		1
static const XCALL_T ScreenPages[NUM_OF_SCREEN_PAGES] = {GetScreenTable1};


//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  INITDISP            Entry point for the Display Proccessor.
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

extern void INITDISP( void )
{
	// Make sure we are enabled
	while ( !( ENAPPL.initialized ) )
	{
		SDK_RelinqCPU(  );
	}


	// Display Processor Loop  - simply draw sprites

	while ( True )
	{
        DoSprites(  );

		SDK_RelinqCPU(  );
	}							// end while(True)
}



//-----------------------------------------------------------------------------
//  InitScreenCrossRef  Initializes the screen cross reference table.
//                      The cross reference table is used to quickly locate
//                      screen scripts.
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:              Must be called once at program start up.
//-----------------------------------------------------------------------------

extern void InitScreenCrossRef( void )
{
	UBYTE i, j;
	SCREEN_ENTRY Screen;
	SCREEN_ENTRY *pScreenBase;
	UWORD index;

	memset( ( UBYTE * ) ScreenCrossRef, 0, sizeof( ScreenCrossRef ) );

	for ( i = 0; i < sizeof( ScreenPages ) / sizeof( XCALL_T ); i++ )
	{
		// get pTableStart, TablePage, and TableItemCount
		APP_PerformCrossCall2( ScreenPages[i] );
		pScreenBase = ( SCREEN_ENTRY * ) pTableStart;


		// fill in cross reference table
		for ( j = 0; j < TableItemCount; j++ )
		{
			// get table item
			SDK_ReadPage( ( UBYTE * ) & Screen, sizeof( SCREEN_ENTRY ),
						  TablePage, ( UBYTE * ) pScreenBase );
			index = Screen.id - SCREEN_START;

			// make sure we have valid index
			if ( index <
				 ( sizeof( ScreenCrossRef ) / sizeof( SCREEN_CROSSREF ) ) )
			{
				// since page can't be zero, use to check for duplicate entry
				if ( ScreenCrossRef[index].Page == 0 )	// if not duplicate entry
				{
					// add script pointer to cross ref
					ScreenCrossRef[index].pScreenEntry = pScreenBase;
					ScreenCrossRef[index].Page = TablePage;
				}
			}
			pScreenBase++;		// next table item
		}

	}
}



//-----------------------------------------------------------------------------
//  XDrawScreen         Draws screen based on given screen number as
//                      defined in screen.h
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:              X-Callable implementation of DrawScreen
//                      Does not use process to draw screen
//-----------------------------------------------------------------------------

extern void XDrawScreen( void )
{
	SCREEN_ENTRY Screen;
	UWORD *pSDLScript;

	if ( ( SDLScreenId < IDLE_SCR ) || ( SDLScreenId >= SCREEN_MAX ) )
	{
		// INVALID SCREEN ID
		return;
	}

	Titlebuf[0] = 0;			// clear title buf

	GetSpriteSem(  );			// halt sprites while drawing screen

	SDK_ReadPage( ( UBYTE * ) & Screen, sizeof( SCREEN_ENTRY ),
				  ScreenCrossRef[SDLScreenId - SCREEN_START].Page,
				  ( UBYTE * ) ScreenCrossRef[SDLScreenId -
											 SCREEN_START].pScreenEntry );
	pSDLScript = Screen.pScript;

	if ( pSDLScript )
	{
		CurrentCommand.pCommand = pSDLScript;	// pointer to HDT Screen Language command array
		CurrentCommand.Page = ScreenCrossRef[SDLScreenId - SCREEN_START].Page;	// call screen function
		InitReadScr(  );
		ReadScr(  );
	}

	ReleaseSpriteSem(  );		// start sprites
}

//-----------------------------------------------------------------------------
//  MaxCharDisp         This function will calculate the maximum number of
//                      characters that can be displayed on one line.
//                      This value is based on the number of pixels for 
//                      the display, current font width, and the indent, if one
//                      is being used.  The value will be loaded into the
//                      global screen_limit. 
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

extern void MaxCharDisp( void )
{
	CHARSIZE charsz;

	// Get Font size
	SDK_GuiGetCharSize( &charsz );

	// Maximum characters for the line
	screen_limit =
		( SCR_PIXEL_WIDTH - ( TEXT_INDENT * 2 ) ) / ( charsz.Width );

}



//-----------------------------------------------------------------------------
//  XDrawSDLScript      Draws a screen based on pointer to an SDL script
//
//  Parameters:         None
//
//  Global Inputs:      Global Parms pTableStart and TablePage used
//                      to pass data to this function
//
//  Returns:            Bool
//                      True  - okay to continue
//                      False - reprompt for a different Amount
//
//  Notes:              X-Callable implementation of DrawSDLScript
//                      Does not use process to draw screen.
//-----------------------------------------------------------------------------

extern void XDrawSDLScript( void )
{

	Titlebuf[0] = 0;			// clear title buf

	GetSpriteSem(  );			// halt sprites while drawing screen

	CurrentCommand.pCommand = ( UWORD * ) pTableStart;	// pointer to HDT Screen Language command array
	CurrentCommand.Page = TablePage;	// page of caller
	InitReadScr(  );
	ReadScr(  );

	ReleaseSpriteSem(  );		// start sprites
}



//-----------------------------------------------------------------------------
//  InitSDL             Perform initializations as required at application
//                      startup for SDL and menu system
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:              Must be called before screens are draw using SDL
//-----------------------------------------------------------------------------

extern void InitSDL( void )
{

	InitScreenCrossRef(  );		// initialize screen reference table
	InitScreenGenerator(  );	// initialize the SDL screen generator
	InitSprites(  );			// make sure sprite cue is empty
	ClearMenus(  );				// clean slate for menu stack
}



//=============================================================================
// Private function definitions
//=============================================================================
