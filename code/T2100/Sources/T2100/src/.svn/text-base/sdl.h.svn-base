
#ifndef _SDL_H_
#define _SDL_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             sdl.h
//      Screen Design Language definitions, data, and functions
//
//=============================================================================

#include "basictyp.h"
#include "sdk.h"
#include "screen.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================

// structure to gloabbly pass data to SDL functions
typedef struct
{
	char *pText;				// pointer to text  
	UBYTE len;					// length of text
	PAGE_NUMBERS Page;			// page number
	UBYTE id;
}
SDL_PARMS;

typedef struct
{
	UWORD *pCommand;			// pointer to HDT Screen Language command array
	PAGE_NUMBERS Page;			// page of this command
}
SDL_COMMAND;

extern SDL_COMMAND CurrentCommand;	// current command

// command definitions
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


// justifications
enum JUSTIFICATION_TYPES
{
	JLEFT,
	JCENTER,
	JRIGHT
};

// vertical alignment
enum VERTICAL_ALIGNMENTS
{
	TOP,
	VCENTER,
	BOTTOM
};


// input box styles
enum INPUTBOX_STYLES
{
	IB_AUTO_BORDER,				// border based on max characters as last set by SetInputBoxMaxChars
	IB_FIXED_BORDER,			// fixed border based on x,y,w,h in INPUTBOX commands
	IB_NO_BORDER				// no border, uses x,y,w,h for text justification
};

// font size
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



// user variable counts
#define NUM_OF_USER_EVENTS      16
#define NUM_OF_USER_IMAGES      16
#define NUM_OF_USER_MESSAGES    16

// default definitions


#define DEFAULT_SFONT           FONT_SA05NN07
#define DEFAULT_EFONT           FONT_EA05NN07
#define DEFAULT_FONT_SIZE       FONT_05NN07


#define DEFAULT_USER_EVENT      EVENT_ERROR
#define DEFAULT_USER_MSG        N_QUESTION_MARK

// Macros to set user-selectable SDL items

//-----------------------------------------------------------------------
// Macro Name:      SetEnableFlag
// Description:     Sets SDL enable flag 
// Parameters:      flag - flag to set
//                  val  - value to set the flag ( zero or non-zero )
// Return Value:    None
// Notes:           None
//-----------------------------------------------------------------------
#define SetEnableFlag(flag, val) Enable_Flags[flag] = val


//-----------------------------------------------------------------------
// Macro Name:      SetUserMessage
// Description:     Assign SDL user message to message
// Parameters:      userMsg - user message number, MSG_USER0 .. MSG_USERn
//                  newMsg - message toi assign to user message
// Return Value:    None
// Notes:           None
//-----------------------------------------------------------------------
#define SetUserMessage( userMsg, newMsg ) User_Msgs[userMsg - MSG_USER0] = newMsg

//-----------------------------------------------------------------------
// Macro Name:      SetUserEvent
// Description:     Assign SDL user event to event
// Parameters:      userEvent - user event number, EVENT_USER0 .. EVENT_USERn
//                  newEvent - event to assign to user event
// Return Value:    None
// Notes:           None
//-----------------------------------------------------------------------
#define SetUserEvent( userEvent, newEvent ) User_Events[userEvent - EVENT_USER0] = newEvent


// HDT Screen Design Language Functions

typedef struct
{
	UBYTE Logical;				// non-zero for logical coordinates, else zero
	UWORD LeftX;				// Left X of window
	UWORD TopY;					// Left Y of window
	UWORD Width;				// Width of window
	UWORD Height;				// Height of window
	UWORD LogMaxWidth;			// logical max width of display, only for Logical = non-zero
	UWORD LogMaxHeight;			// logical max height of display, only for Logical = non-zero
}
SDLWINDOW;


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================
extern void InitScreenGenerator( void );
extern void InitReadScr( void );
extern void ReadScr( void );
extern void XUpdateTextArea( void );
extern void XSetDefaultSDLWindow( void );
extern void XSetSDLWindow( void );
extern void XUpdateInputBox( void );
extern UBYTE GetInputBoxMaxDsp( void );
extern void XSetInputBox( void );
extern void XSetInputBoxMaxChars( void );
extern void SetInputBoxMaxChars( UBYTE maxchars );
extern void SetInputBox( char *text );
extern void UpdateInputBox( char *text );
extern void DrawScreen( SCREEN_ID screeni );
extern void DrawSDLScript( UWORD * pSDLScript );
extern void SetSDLWindow( SDLWINDOW * pWin );
extern void UpdateTextArea( TEXTAREA_ID id, char *text );
extern void SetDefaultSDLWindow( void );

#endif // _SDL_H_
