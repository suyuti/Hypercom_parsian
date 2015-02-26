//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      sdl.h
//
// File Description:
//      Screen Design Language definitions, data, and functions
//
//=============================================================================

#ifndef _SDL_H_
#define _SDL_H_

#include "basictyp.h"
#include "sdk.h"
#include "screen.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================

//! structure to gloabbly pass data to SDL functions
typedef struct
{
	//! pointer to text  
	char *pText;	  
	//! length of text
	UBYTE len;	
	//! page number
	PAGE_NUMBERS Page;			
	//! page ID
	UBYTE id;
}
SDL_PARMS;

//! SDL command structure
typedef struct
{
	//! pointer to HDT Screen Language command array
	UWORD *pCommand;
	//! page of this command
	PAGE_NUMBERS Page;		   
}
SDL_COMMAND;

//! current command
extern SDL_COMMAND CurrentCommand;	// current command

//! command definitions
enum SCREEN_COMMANDS
{
	ENDSCREEN,
	STARTSCREEN,
	LANGUAGE,
	BUTTON_TEXT,
	BUTTON_TEXT2,
	BUTTON_IMAGE,
	BUTTON_CUSTOM,
	BUTTON_TRANSPARENT,
	BUTTON_KEYPAD,
	FONT,
	LANG_FONT,
	DISPLAY_IMAGE,
	TEXTJ,
	RECTANGLE,
	INPUTBOX,
	INVERT_AREA,
	TEXTP,
	TEXTL,
	WINDOW,
	DEFAULT_WINDOW,
	SET_USER_MSG,
	SET_USER_EVENT,
	ENABLE_FLAG,
	ENABLE_FLAG_NOT,
	SET_ENABLE_FLAG,
	FILL,
	CLEAR_ALL,
	CLEAR_AREA,
	NEWSCREEN,
	SET_DEFAULT,
	SPRITE,
	DISPLAY_ALL,
	DISPLAY_AREA,
	DISPLAY_WINDOW,
	VERTICAL_LINE,
	HORIZONTAL_LINE,
	DELETE_SPRITES,
	SET_LOGICAL_WIN_SIZE,
	RESET_LOGICAL_WIN_SIZE,
	TEXTLJ,
	CLEAR_WINDOW,
	TEXTAREA,
	SET_USER_IMAGE
};


//! justifications
enum JUSTIFICATION_TYPES
{
	JLEFT,
	JCENTER,
	JRIGHT
};

//! vertical alignment
enum VERTICAL_ALIGNMENTS
{
	TOP,
	VCENTER,
	BOTTOM
};


//! input box styles
enum INPUTBOX_STYLES
{
	//! border based on max characters as last set by SetInputBoxMaxChars
	IB_AUTO_BORDER,
	//! fixed border based on x,y,w,h in INPUTBOX commands
	IB_FIXED_BORDER,
	//! no border, uses x,y,w,h for text justification
	IB_NO_BORDER				
};

//! font size
enum DISPLAY_FONT_SIZES
{
	FONT_05NN07,
	FONT_06BN08,
	FONT_06BN12,
	FONT_08BN09	
};

typedef enum
{
	FLAG0,
	FLAG1,
	FLAG2,
	FLAG3,
	FLAG4,
	FLAG5,
	FLAG6,
	FLAG7,
	FLAG8,
	FLAG9,
	FLAG10,
	FLAG11,
	FLAG12,
	FLAG13,
	FLAG14,
	FLAG15,
	NUM_OF_ENABLE_FLAGS
}
ENABLE_FLAGS;

typedef enum
{
	TEXTAREA0,
	TEXTAREA1,
	TEXTAREA2,
	TEXTAREA3,
	TEXTAREA4,
	TEXTAREA5,
	TEXTAREA6,
	TEXTAREA7,
	TEXTAREA8,
	TEXTAREA9,
	TEXTAREA10,
	TEXTAREA11,
	TEXTAREA12,
	TEXTAREA13,
	TEXTAREA14,
	TEXTAREA15,
	NUM_OF_TEXTAREAS
}
TEXTAREA_ID;



//! user variable counts
#define NUM_OF_USER_EVENTS      16
#define NUM_OF_USER_IMAGES      16
#define NUM_OF_USER_MESSAGES    16

//! default definitions
#define DEFAULT_SFONT           FONT_SA05NN07
#define DEFAULT_EFONT           FONT_EA05NN07
#define DEFAULT_FONT_SIZE       FONT_05NN07


#define DEFAULT_USER_EVENT      EVENT_ERROR
#define DEFAULT_USER_MSG        N_QUESTION_MARK

//-----------------------------------------------------------------------------
//! \brief
//!      Macros to set user-selectable SDL items.
//!
//! \param
//!		flag		flag to set
//!	\param
//!		val			value to set the flag ( zero or non-zero )
//!				
//! \return
//!     None
//!
//! \note
//!		Sets SDL enable flag 
//-----------------------------------------------------------------------------
#define SetEnableFlag(flag, val) Enable_Flags[flag] = val

//-----------------------------------------------------------------------------
//! \brief
//!      Assign SDL user message to message.
//!
//! \param
//!		userMsg		user message number, MSG_USER0 .. MSG_USERn
//!	\param
//!		newMsg		message toi assign to user message
//!				
//! \return
//!     None
//!
//! \note
//!		Sets SDL enable flag 
//-----------------------------------------------------------------------------
#define SetUserMessage( userMsg, newMsg ) User_Msgs[userMsg - MSG_USER0] = newMsg

//-----------------------------------------------------------------------------
//! \brief
//!      Assign SDL user event to event.
//!
//! \param
//!		userEvent	 user event number, EVENT_USER0 .. EVENT_USERn
//!	\param
//!		newEvent	 event to assign to user event
//!				
//! \return
//!     None
//!
//! \note
//!		Sets SDL enable flag 
//-----------------------------------------------------------------------------
#define SetUserEvent( userEvent, newEvent ) User_Events[userEvent - EVENT_USER0] = newEvent


// HDT Screen Design Language Functions

typedef struct
{
    //! non-zero for logical coordinates, else zero
    UBYTE Logical;				
    //! Left X of window
    UWORD LeftX;	
    //! Left Y of window
    UWORD TopY;
    //! Width of window
    UWORD Width;
    //! Height of window
    UWORD Height;
    //! logical max width of display, only for Logical = non-zero
    UWORD LogMaxWidth;
    //! logical max height of display, only for Logical = non-zero
    UWORD LogMaxHeight;			
}
SDLWINDOW;


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================
//! Perform initializations as required at application startup for ReadScr()
extern void InitScreenGenerator( void );
//! Called before processing each screen screen script to rerform initializations as required  for ReadScr()
extern void InitReadScr( void );
//! Read a screen script and translate to the appropriate SDK calls
extern void ReadScr( void );
//! Draws text directly to text area on display
extern void XUpdateTextArea( void );
//! Sets the window used by SDL to full screen size and sets logical pixel size equal to physical pixel size
extern void XSetDefaultSDLWindow( void );
//! X-Call to set the current window
extern void XSetSDLWindow( void );
//! Updates data for the input box on current screen
extern void XUpdateInputBox( void );
//! Gets the maximum number of characters that can be can be displayed in the virtual window set for the  input box.
extern UBYTE GetInputBoxMaxDsp( void );
//! Sets the data for the input box
extern void XSetInputBox( void );
//! Sets the maximum characters which can be displayed in the input box
extern void XSetInputBoxMaxChars( void );
//! Sets the maximum characters which can be displayed in the input box
extern void SetInputBoxMaxChars( UBYTE maxchars );
//! Sets the data for the input box to be used when	screen is drawn
extern void SetInputBox( char *text );
//! Updates data for the input box on current screen
extern void UpdateInputBox( char *text );
//! Draws screen based on given screen number as defined in screen.h
extern void DrawScreen( SCREEN_ID screeni );
//! Draws a screen based on pointer to an SDL script
extern void DrawSDLScript( UWORD * pSDLScript );
//! Sets the current window
extern void SetSDLWindow( SDLWINDOW * pWin );
//! Updates data for given text area on current screen
extern void UpdateTextArea( TEXTAREA_ID id, char *text );
//! Sets the window used by SDL to full screen size	and sets logical pixel size equal to physical pixel size
extern void SetDefaultSDLWindow( void );

#endif // _SDL_H_
