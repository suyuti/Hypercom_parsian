//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      sdl.c
//
// File Description:
//      Screen design language implementation
//
//=============================================================================

#include "string.h"
#include "stdlib.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "screen.h"
#include "sdl.h"
#include "utllib.h"
#include "sprite.h"
#include "img.h"
#include "getkey.h"
#include "runtime.h"
#include "text.h"
#include "tabutil.h"
#include "tables.h"

//-----------------------------------------------------------------------------
//!  Command descriptions 
//!
//-----------------------------------------------------------------------------
//!  COMMAND_NAME
//!      List of parameters
//!
//!      Description
//!
//-----------------------------------------------------------------------------
//!  BUTTON_CUSTOM
//!      left x, top y, width, height, initial state, border type, event 
//!
//!      Draw a blank button with border only. 
//!
//-----------------------------------------------------------------------------
//!  BUTTON_KEYPAD
//!      border type, start x, start_y, width, height, x spacing, 
//!      y spacing, n rows, n cols, character 1,  event 1, character 2,  
//!      event 2, ?? , character (n rows)*(n cols) , event (n rows) * (n cols) 
//!
//!      Draw an array of buttons with a single character, useful for key pads.
//!
//-----------------------------------------------------------------------------
//!  BUTTON_IMAGE
//!      image, left x, top y, width, height, initial state, border type, event 
//!
//!      Draw a button with specified image in center.
//!
//-----------------------------------------------------------------------------
//!  BUTTON_TEXT
//!      message, left x, top y, width, height, initial state, border type, event 
//!
//!      Draw a button with a single text line.
//!
//-----------------------------------------------------------------------------
//!  BUTTON_TEXT2
//!      message 1, message 2, left x, top y, width, height, initial state, 
//!      border type, event 
//!
//!  Draw a button with 2 lines of text.
//!
//-----------------------------------------------------------------------------
//!  BUTTON_TRANSPARENT
//!      left x, top y, width, height, initial state, border type, event 
//!
//!      Draw a button without changing background.
//!
//-----------------------------------------------------------------------------
//!  CLEAR_ALL
//!
//!      Clear screen buffer, set window to full screen, set default font, and remove all sprites.
//!
//-----------------------------------------------------------------------------
//!  CLEAR_AREA
//!      left x, top y, width, height 
//!
//!      Clear area of screen buffer.
//!
//-----------------------------------------------------------------------------
//!  CLEAR_WINDOW
//!
//!      Clear area of current virtual window in screen buffer.
//!
//-----------------------------------------------------------------------------
//!  DEFAULT_WINDOW 
//!
//!      Set virtual window to full screen.
//!
//-----------------------------------------------------------------------------
//!  DELETE_SPRITES 
//!
//!      Stop sprites and remove them from processing queue.
//!
//-----------------------------------------------------------------------------
//!  DISPLAY_ALL 
//!
//!      Show screen buffer.
//!
//-----------------------------------------------------------------------------
//!  DISPLAY_AREA
//!      left x, top y, width, height 
//!
//!      Show area of screen buffer.
//!
//-----------------------------------------------------------------------------
//!  DISPLAY_WINDOW 
//!
//!      Show virtual window.
//!
//-----------------------------------------------------------------------------
//!  ENABLE_FLAG
//!      flag n
//!
//!      Enables next command if flag n is non-zero.
//!
//-----------------------------------------------------------------------------
//!  ENDSCREEN
//!
//!      End of a set of SDL commands.
//!
//-----------------------------------------------------------------------------
//!  FILL
//!      left x, top y, width, height 
//!
//!      Fills given area of screen buffer.
//!
//-----------------------------------------------------------------------------
//!  FONT
//!      standard font, extended font 
//!
//!      Set font.
//!
//-----------------------------------------------------------------------------
//!  LANG_FONT
//!      font size 
//!
//!      Set font by current language
//!
//-----------------------------------------------------------------------------
//!  HORIZONTAL_LINE
//!      left x, top y, length, width 
//!
//!      Draw horizontal line to screen buffer
//!
//-----------------------------------------------------------------------------
//!  DISPLAY_IMAGE
//!      left x, top y, image 
//!
//!      Draw image to screen buffer
//!
//-----------------------------------------------------------------------------
//!  INPUTBOX
//!      border style, justification, left x, top y, width, height, spacing
//!
//!      Draw an input box which can be updated directly from application.
//!
//-----------------------------------------------------------------------------
//!  INVERT_AREA
//!      left x, top y, width, height 
//!
//!  Invert area in screen buffer.
//!
//-----------------------------------------------------------------------------
//!  LANGUAGE
//!      language 
//!
//!      Set language.
//!
//-----------------------------------------------------------------------------
//!  NEWSCREEN
//!      screen 
//!
//!      Generate another SDL screen from an within an SDL script
//!
//-----------------------------------------------------------------------------
//!  RECTANGLE
//!      left x, top y, width, height , border width
//!
//!      Draw a rectangle in screen buffer.
//!
//-----------------------------------------------------------------------------
//!  RESET_LOGICAL_WIN_SIZE
//!
//!      Turn-off logical coordinate processing. Use actual coordinates.
//!
//-----------------------------------------------------------------------------
//!  SET_DEFAULT 
//!
//!  Sets language and font to default values.
//!
//-----------------------------------------------------------------------------
//!  SET_ENABLE_FLAG
//!      flag , set
//!
//!      Sets or resets flag. set = non-zero to set or zero to reset.
//!
//-----------------------------------------------------------------------------
//!  SET_LOGICAL_WIN_SIZE
//!      logical width, logical height
//!
//!      Set logical pixel width and height of the current virtual
//!      window. All coordinates of subsequent commands will be 
//!      based on the logical size.
//!
//-----------------------------------------------------------------------------
//!  SET_USER_EVENT
//!      user event, event
//!
//!      Sets user event to value of event.
//!
//-----------------------------------------------------------------------------
//!  SET_USER_IMAGE
//!      user image, image
//!
//!      Sets user image to value of image.
//!
//-----------------------------------------------------------------------------
//!  SET_USER_MSG
//!      user message , message
//!  
//!      Sets user message to value of message.
//!
//-----------------------------------------------------------------------------
//!  SPRITE
//!      sprite 
//!
//!      Starts a sprite.
//!
//-----------------------------------------------------------------------------
//!  STARTSCREEN
//!      screen
//!
//!      First command for any set of SDL commands.
//!
//-----------------------------------------------------------------------------
//!  TEXTAREA
//!      id, justification, vert aligmnment, left x, top y, 
//!      width, height, message 
//!
//!      Draws a text area whihc cna be updated using UpdateTextArea() function.
//!      16 text areas available, TEXTAREA0 - TEXTAREA16
//!
//-----------------------------------------------------------------------------
//!  TEXTJ
//!      justification, vert aligmnment, left x, top y, width, height, message 
//!
//!      Draw justified text in screen buffer for given area.
//!
//-----------------------------------------------------------------------------
//!  TEXTL
//!      line, column, message 
//!
//!      Draws text at zero-based line number. Line location calculation
//!      based on virtual window.
//!
//-----------------------------------------------------------------------------
//!  TEXTLJ
//!      line, justification, message 
//!
//!      Draws justified text at zero-based line number.  Line location
//!      calculation based on virtual window.
//!
//-----------------------------------------------------------------------------
//!  TEXTP
//!      x, y, message 
//!
//!      Draws  text at the specified coordinates
//!
//-----------------------------------------------------------------------------
//!  VERTICAL_LINE
//!      left x, top y, length, width 
//!
//!      Draw vertical line to screen buffer
//!
//-----------------------------------------------------------------------------
//!  WINDOW
//!      left x, top y, width, height 
//!
//!      Sets coordinates of virtual window
//!
//-----------------------------------------------------------------------------


//=============================================================================
// External variables / declarations
//=============================================================================
//! Screens cross reference table
extern SCREEN_CROSSREF ScreenCrossRef[];

//=============================================================================
// Private defines and typedefs
//=============================================================================

//! Macros to convert logical to a phsyical coordinates
#define LXtoPX(x) ((LogicalOn) ? CalculateX(x):x)
#define LYtoPY(y) ((LogicalOn) ? CalculateY(y):y)

//! minimum pixels of text from border edge of InputBox
#define INPUTBOX_XSPACING	3
//! max number of command paramters
#define MAX_PRM 			10
//! 6 is width of smallest font
#define MAX_CHARS_PER_LINE	SCR_PIXEL_WIDTH/6	
//! 8 is height of smallest font
#define MAX_TEXT_LINES		SCR_PIXEL_HEIGHT/8	

typedef struct
{
	RECT Rct;
	UWORD justification;
	UWORD vertalign;
	UBYTE sfont;
	UBYTE efont;
}
TEXTAREA_PARMS;
//! max number of command paramters
#define MAX_PRM 			10	

//=============================================================================
// Private function declarations
//=============================================================================
//! Converts logical x-coordinate or width to actual physical coordinate or width
static UWORD CalculateX( UWORD xpixel );
//! Converts logical y-coordinate or height to actual physical coordinate or height
static UWORD CalculateY( UWORD ypixel );
//! Get the next token
static UWORD GetToken( void );
//! Get the specified number of parameters & put in Parm[]
static void GetParms( UBYTE num );
//! Get rectangle coordinates & put them in Rct
static void GetRectCoordinates( void );
//! Get 1 lines of text and set Btn.pLabel1
static void ButtonGetText( void );
//! Get the 2 lines of text and set Btn.pLabel1 & pLabel2
static void ButtonGetText2( void );
//! Common routine for drawing all types of buttons
static void ButtonCommon( void );
//! Display an array of buttons
static void ButtonArray( void );
//! Draw a rectangle
static void DrawRectangle( RECT * pRct, UWORD bordersz );
//!Draws text based on current input box parameters
static void DrawInputBoxText( void );
//! Verify idle button
static UWORD Verify_Idle_Btn( UWORD id );
//! Draws justified text to screen buffer in given area
static void DrawJustifiedText( RECT * pRct, UWORD Justification,
							   UWORD VertAlignment, char *pText );
//! Justifies given text in given area
static void TextJustify( void );
//! Justifies text on a line
static void TextLineJustify( void );
//! Sets up text area and draws initial contents
static void DrawTextArea( void );
//! Draws input box border and initial contents
static void DrawInputBox( void );
//! Recursive call to read another script
static void RecursiveScript( void );  
//! Get the next token which is assummed to be an eventID and perform special event ID processing as required
static UWORD GetEventID( void );
//! Justifies text on a line
static void TextLineJustify( void );

//=============================================================================
// Public data definitions
//=============================================================================
//! Current command
SDL_COMMAND CurrentCommand;


//=============================================================================
// Private data definitions
//=============================================================================
//! text buffer
static char TextAreaBuffer[MAX_MSG_LEN + 1];
//! text area parameters
static TEXTAREA_PARMS taparms[NUM_OF_TEXTAREAS];

// input box variables - must be declared globally
//! variables apply to current screen
static TEXTAREA_PARMS ibTextArea;	// used for input box text area
//! input box buffer
static char InputBoxBuffer[MAX_CHARS_PER_LINE + 1];
//! input box max characters
static UBYTE InputBoxMaxChars;
//! max display characters
static UBYTE InputBoxMaxDispChar;

//! Number of parameters for each command
static const UBYTE CommandParmCount[] = {
	//! ENDSCREEN
	0,
	//! STARTSCREEN
	1,							
	//! LANGUAGE							   
	1,							
	//! BUTTON_TEXT							  
	8,							
	//! BUTTON_TEXT2							  
	9,							
	//! BUTTON_IMAGE								
	8,							
	//! BUTTON_CUSTOM								
	7,							
	//! BUTTON_TRANSPARENT								
	7,							
	//! BUTTON_KEYPAD - variable								
	9,							
	//! LANG_FONT								
	1,	
	//! FONT								
	2,		
	//! DISPLAY_IMAGE							   
	3,							
	//! TEXTJ								
	7,							
	//! RECTANGLE								
	5,							
	//! INPUTBOX								
	7,							
	//! INVERT								
	4,							
	//! TEXTP								
	3,							
	//! TEXTL							   
	3,							
	//! WINDOW								
	4,							
	//! DEFAULT_WINDOW								
	0,							
	//! SET_USER_MSG								
	2,							
	//! SET_USER_EVENT								
	2,							
	//! ENABLE_FLAG							   
	1,							
	//! ENABLE_FLAG_NOT							   
	1,							
	//! SET_ENABLE_FLAG								
	2,							
	//! FILL								
	4,							
	//! CLEAR_ALL							   
	0,							
	//! CLEAR_AREA							   
	4,							
	//! NEWSCREEN							   
	1,							
	//! SET_DEFAULT							   
	0,							
	//! SPRITE								
	1,							
	//! DISPLAY_ALL								
	0,							
	//! DISPLAY_AREA							   
	4,							
	//! DISPLAY_WINDOW							   
	0,							
	//! VERTICAL_LINE							   
	4,							
	//! HORIZONTAL_LINE							   
	4,							
	//! DELETE_SPRITES							   
	0,							
	//! SET_LOGICAL_WIN_SIZE							   
	2,							
	//! RESET_LOGICAL_WIN_SIZE								
	0,							
	//! TEXTLJ								
	3,							
	//! CLEAR_WINDOW								
	0,							
	//! TEXTAREA								
	8,							
	//! SET_USER_IMAGE								
	2,							
};

static BTN_Entry Btn;
static UBYTE *UBPtr;
static RECT Rct;
static char SDLTextLineBuf[MAX_CHARS_PER_LINE + 1];
static UWORD cmd;
static UBYTE ignore;
static UWORD Parm[MAX_PRM];
static UBYTE LogicalOn;
static UWORD LogicalWidth;
static UWORD LogicalHeight;
//! used as comand buffer for scripts on other pages
static UWORD SDLCommandBuffer[64];
static UWORD *pBufferCommand;
//! indicates that script data is on another page
static UBYTE PageProcessing;

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!      Perform initializations as required at application startup for ReadScr()
//!				
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
extern void InitScreenGenerator( void )
{
	memset( ( UBYTE * ) User_Events, 0,sizeof( User_Events ) );
	memset( ( UBYTE * ) User_Msgs, 0,sizeof( User_Msgs ) );
	memset( ( UBYTE * ) Enable_Flags, 0,sizeof( Enable_Flags ) );

	InputBoxMaxChars = 0;
	ignore = 0;
	LogicalOn = 0;
}

//-----------------------------------------------------------------------------
//! \brief
//!      Called before processing each screen screen script to rerform initializations as required  for ReadScr()
//!				
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
extern void InitReadScr( void )
{
	// empty command buffer by setting pointer to end
	pBufferCommand =
		SDLCommandBuffer + sizeof( SDLCommandBuffer ) / sizeof( UWORD );

	if ( CurrentCommand.Page != SDK_ThisPage(  ) )
	{
		PageProcessing = 1;		// page processing required
	}
	else
	{
		PageProcessing = 0;		// page processing not required     
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!      Read a screen script and translate to the appropriate SDK calls
//!				
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
extern void ReadScr( void )
{
	UWORD tmp;
	UBYTE CurFont, CurExFont;
	// Read a command token
	while ( 0 != ( cmd = GetToken(  ) ) )
	{
		// Do we want to ignore this instruction?
		if ( ignore )
		{
			// Consume the parameters
			GetParms( CommandParmCount[cmd] );

			// BUTTON_KEYPAD has a variable number of commands
			if ( BUTTON_KEYPAD == cmd )
			{
				// compute remaining parameters and skip
				// remaining parameters = row*columns*2
				tmp = Parm[7] * Parm[8] * 2;
				while ( tmp )
				{
					GetToken(  );
					tmp--;
				}
			}

			// Only ignore 1 command at a time
			ignore = False;

			// Skip to the next command
			continue;
		}

		// Handle each command
		switch ( cmd )
		{
			case ENDSCREEN:	// Terminate the screen descriptor array
				return;

			case LANGUAGE:		// Change Current Language
				LOCTAB.LANGID = GetToken(  );
				SetLOCTabLRC(  );
				break;

			case BUTTON_TEXT:	// Display a button with 1 line of text
				Btn.ContentType = CT_TEXT;

				// Get 1 lines of text and set Btn.pLabel1 & pLabel2
				ButtonGetText(  );

				// Get the rest of the parameters & draw the button
				ButtonCommon(  );
				break;

			case BUTTON_TEXT2:	// Display a button with 2 lines of text
				Btn.ContentType = CT_TEXT_2LINE;

				// Get the 2 lines of text and set Btn.pLabel1 & pLabel2
				ButtonGetText2(  );

				// Get the rest of the parameters & draw the button
				ButtonCommon(  );
				break;

			case BUTTON_IMAGE:	// Add button with graphics image
				Btn.ContentType = CT_IMAGE;

				// Get the image
				tmp = GetToken(  );
				Btn.pLabel1 = GetImage( tmp );

				// Get the rest of the parameters & draw the button
				ButtonCommon(  );
				break;

			case BUTTON_TRANSPARENT:	// Add invisible button
				Btn.ContentType = CT_TRANSPARENT;
				ButtonCommon(  );
				break;

			case BUTTON_CUSTOM:	// Add empty button (only border is visable)
				Btn.ContentType = CT_CUSTOM;
				ButtonCommon(  );
				break;

			case BUTTON_KEYPAD:	// Draw rows and columns of buttons
				ButtonArray(  );
				break;

			case LANG_FONT:
				FontSize = GetToken(  );
				SetCurrentLanguageFont ();
				break;

			case FONT:
				CurFont = ( UBYTE ) GetToken(  );
				CurExFont = ( UBYTE ) GetToken(  );
				SDK_GuiSetFont( CurFont, CurExFont );
				break;

			case DISPLAY_IMAGE:
				Parm[0] = LXtoPX( GetToken(  ) );	// X
				Parm[1] = LYtoPY( GetToken(  ) );	// Y
				tmp = GetToken(  );	// ImageID
				UBPtr = GetImage( tmp );
				SDK_GuiImage( Parm[0], Parm[1], UBPtr );
				break;

			case TEXTJ:
				TextJustify(  );
				break;

			case RECTANGLE:
				GetRectCoordinates(  );
				DrawRectangle( &Rct, GetToken(  ) );
				break;

			case INPUTBOX:
				DrawInputBox(  );
				break;

			case INVERT_AREA:
				GetRectCoordinates(  );
				SDK_GuiInvArea( &Rct );
				break;

			case TEXTP:		// Draw text at coordinates
				// Get the X & Y parameters
				GetParms( 2 );

				// Get the text
				GetMsg( GetToken(  ), Dspbuf );

				// Display the text
				SDK_GuiTextP( ( UWORD ) LXtoPX( Parm[0] ),
							  ( UWORD ) LYtoPY( Parm[1] ),
							  ( char * ) Dspbuf );
				break;

			case TEXTL:		// Draw text at line, col
				// Get the line & col parameters
				GetParms( 2 );

				// Get the text
				GetMsg( GetToken(  ), Dspbuf );

				// Display the text
				SDK_GuiText( ( UBYTE ) Parm[0], ( UBYTE ) Parm[1],
							 ( char * ) Dspbuf );
				break;

			case WINDOW:		// Set virtual window
				GetRectCoordinates(  );
				SDK_GuiSetWindow( &Rct );
				break;

			case DEFAULT_WINDOW:	// Set virtual window to full screen
				SDK_GuiDefWindow(  );
				break;

			case SET_USER_MSG:
				// Get the index and value parameters
				GetParms( 2 );

				// Check index range
				if ( Parm[0] < NUM_OF_USER_MESSAGES )
					User_Msgs[Parm[0]] = Parm[1];
				break;

			case SET_USER_EVENT:
				// Get the index and value parameters
				GetParms( 2 );

				// Check index range
				if ( Parm[0] < NUM_OF_USER_EVENTS )
					User_Events[Parm[0]] = Parm[1];
				break;

			case ENABLE_FLAG:
				tmp = GetToken(  );
				if ( 0 == Enable_Flags[tmp] )
					ignore = True;
				break;

			case ENABLE_FLAG_NOT:
				tmp = GetToken(  );
				if ( Enable_Flags[tmp] )
					ignore = True;
				break;

			case SET_ENABLE_FLAG:
				// Get the index and value parameters
				GetParms( 2 );

				// Check index range
				if ( Parm[0] < NUM_OF_ENABLE_FLAGS )
					Enable_Flags[Parm[0]] = ( UBYTE ) Parm[1];
				break;

			case FILL:
				GetRectCoordinates(  );
				SDK_GuiSetArea( &Rct );
				break;

			case CLEAR_ALL:
				DeleteAllSprites(  );	// clear sprite queue
                SDK_GuiClearScr(  );
				FontSize = DEFAULT_FONT_SIZE;
				SetCurrentLanguageFont ();
				break;

			case DISPLAY_ALL:
				SDK_GuiDispAll(  );
				break;

			case CLEAR_AREA:
				GetRectCoordinates(  );
				SDK_GuiClrArea( &Rct );
				break;

			case DISPLAY_AREA:
				GetRectCoordinates(  );
				SDK_GuiDispPart( &Rct );
				break;

			case DISPLAY_WINDOW:
				SDK_GuiGetVirWin( &Rct );
				SDK_GuiDispPart( &Rct );
				break;

			case NEWSCREEN:
				RecursiveScript(  );	// Recursive call to read another script
				break;

			case STARTSCREEN:	// Set screen ID for this screen
				// STARTSCREEN, SDLScreenID
				GetToken(  );
				break;

			case SET_DEFAULT:
				FontSize = DEFAULT_FONT_SIZE;
				SetCurrentLanguageFont ();
				break;

			case SPRITE:
				AddSprite( ( SPRITE_ID ) GetToken(  ) );
				break;

			case VERTICAL_LINE:
				// Get x,y, length, thickness
				GetParms( 4 );
				Parm[0] = LXtoPX( Parm[0] );
				Parm[1] = LYtoPY( Parm[1] );
				Parm[2] = LXtoPX( Parm[2] );
				Parm[3] = LYtoPY( Parm[3] );
				for ( tmp = 0; tmp < Parm[3]; tmp++, Parm[0]++ )
					SDK_GuiVertical( Parm[0], Parm[1], Parm[2] );
				break;

			case HORIZONTAL_LINE:
				// Get x,y, length, thickness
				GetParms( 4 );
				Parm[0] = LXtoPX( Parm[0] );
				Parm[1] = LYtoPY( Parm[1] );
				Parm[2] = LXtoPX( Parm[2] );
				Parm[3] = LYtoPY( Parm[3] );
				for ( tmp = 0; tmp < Parm[3]; tmp++, Parm[1]++ )
					SDK_GuiHorizontal( Parm[0], Parm[1], Parm[2] );
				break;

			case DELETE_SPRITES:
				DeleteAllSprites(  );	// clear sprite queue
				break;

			case SET_LOGICAL_WIN_SIZE:
				LogicalWidth = GetToken(  );
				LogicalHeight = GetToken(  );
				LogicalOn = 1;
				break;

			case RESET_LOGICAL_WIN_SIZE:
				LogicalOn = 0;
				break;

			case TEXTLJ:		// Draw justified text at line
				TextLineJustify(  );
				break;

			case CLEAR_WINDOW:
				SDK_GuiGetVirWin( &Rct );
				SDK_GuiClrArea( &Rct );
				break;

			case TEXTAREA:
				DrawTextArea(  );
				break;

			case SET_USER_IMAGE:
				//Get the index and value parameters
				GetParms( 2 );

				// Check index range
				if ( Parm[0] < NUM_OF_USER_IMAGES )
					User_Imgs[Parm[0]] = Parm[1];
				break;
		}
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!      Draws text directly to text area on display
//!				
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
extern void XUpdateTextArea( void )
{
	TEXTAREA_PARMS *pTA;

	if ( SDLparms.id >= NUM_OF_TEXTAREAS )
		return;

	memset( ( UBYTE * ) TextAreaBuffer, 0,sizeof( TextAreaBuffer ) );
	SDK_ReadPage( ( UBYTE * ) TextAreaBuffer, SDLparms.len, SDLparms.Page,
				  ( UBYTE * ) SDLparms.pText );

	pTA = &taparms[SDLparms.id];

	GetSpriteSem(  );			// halt sprites while updating screen

	// use font when text area was initialized
	SDK_GuiSetFont( pTA->sfont, pTA->efont );

	SDK_GuiClrArea( &pTA->Rct );
	DrawJustifiedText( &pTA->Rct, pTA->justification, pTA->vertalign,
					   TextAreaBuffer );
	SDK_GuiDispPart( &pTA->Rct );

	SetCurrentLanguageFont ();

	ReleaseSpriteSem(  );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Sets the window used by SDL to full screen size and sets 
//!		 logical pixel size equal to physical pixel size
//!				
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
extern void XSetDefaultSDLWindow( void )
{
	SDK_GuiDefWindow(  );
	LogicalOn = 0;				// turn off logical calculations
}

//-----------------------------------------------------------------------------
//! \brief
//!      Sets the current window
//!
//! \return
//!     None
//!
//! \note
//!     Pointer to SDLWINDOW passed in pTableStart as global input argument.
//!		SDL drawing commands will operate relative to window
//!     SDL commands WINDOW, SET_LOGICAL_WINDOW_SIZE, 
//!     RESET_LOGICAL_WINDOW_SIZE, and DEFAULT_WINDOW
//!     will override window paramters set by this function.
//-----------------------------------------------------------------------------
extern void XSetSDLWindow( void )
{
	SDLWINDOW sw;
	SDK_ReadPage( ( UBYTE * ) & sw, sizeof( SDLWINDOW ), TablePage,
				  ( UBYTE * ) pTableStart );

	if ( sw.Logical )
	{
		LogicalOn = 1;
		LogicalWidth = sw.LogMaxWidth;
		LogicalHeight = sw.LogMaxHeight;
	}
	else
		LogicalOn = 0;

	Rct.x = LXtoPX( sw.LeftX );
	Rct.y = LYtoPY( sw.TopY );
	Rct.width = LXtoPX( sw.Width );
	Rct.height = LYtoPY( sw.Height );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Gets the maximum number of characters that can 
//!      be can be displayed in the virtual window set for
//!      the  input box.  This takes the virtual window width
//!      and subtracts the character padding and then divides
//!      this by the pixel width of the current font.
//!      The value is calculated in InputBox(), this function
//!      is used to provide the value to the Utility function
//!      InputBox().Description Line 1
//!	
//! \return
//!     UBYTE
//!
//! \note
//!		This function must be preceded by a function that will 
//!     draw a screen with an input box.  The SDL INPUTBOX will
//!     calculate the  maximum number of characters to display 
//!     and place it in InputBoxMaxDispChar.This function just
//!     places the value in global memory MaxInputDispChar.
//-----------------------------------------------------------------------------
extern UBYTE GetInputBoxMaxDsp( void )
{
	return InputBoxMaxDispChar;
}

//-----------------------------------------------------------------------------
//! \brief
//!      Sets the data for the input box to be used when screen 
//!      is drawn and perform special event ID processing as required
//!
//!  \param
//!		  (global)uses SDLparms to pass parameters on entry:<br>
//!       \li \c SDLparms.pText = pointer to text string
//!       \li \c SDLparms.len = len of text
//!       \li \c SDLparms.Page = page of text data
//!
//! \return
//!     None
//!
//! \note
//!		This function simply sets the input box buffer - 
//!     does not update the screen. Usually used to set up the
//!     default text for the input box before the screen is displayed.
//-----------------------------------------------------------------------------
extern void XSetInputBox( void )
{

	memset( ( UBYTE * ) InputBoxBuffer, 0,sizeof( InputBoxBuffer ) );
	SDK_ReadPage( ( UBYTE * ) InputBoxBuffer, SDLparms.len, SDLparms.Page,
				  ( UBYTE * ) SDLparms.pText );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Updates data for the input box on current screen
//!
//! \param
//!		  (global)uses SDLparms to pass parameters on entry:<br.
//!       \li \c SDLparms.pText = pointer to text string
//!       \li \c SDLparms.len = len of text
//!       \li \c SDLparms.Page = page of text data
//!
//! \return
//!     None
//!
//! \note
//!		This function will directly update the screen.
//!     The screen does not need to be re-drawn.
//!     Should only be called when an input box is 
//!     on the screen.
//-----------------------------------------------------------------------------
extern void XUpdateInputBox( void )
{
	memset( ( UBYTE * ) InputBoxBuffer, 0,sizeof( InputBoxBuffer ) );
	SDK_ReadPage( ( UBYTE * ) InputBoxBuffer, SDLparms.len, SDLparms.Page,
				  ( UBYTE * ) SDLparms.pText );

	GetSpriteSem(  );			// halt sprites while updating screen

	SDK_GuiClrArea( &ibTextArea.Rct );
	DrawInputBoxText(  );
	SDK_GuiDispPart( &ibTextArea.Rct );

	ReleaseSpriteSem(  );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Sets the maximum characters which can be displayed in the input box
//!
//! \param
//!		  (global)SDLparms.len - desired max chars
//!
//! \return
//!     None
//!
//! \note
//!		This function will directly update the screen.
//!     The screen does not need to be re-drawn.
//!     Should only be called when an input box is 
//!     on the screen.
//-----------------------------------------------------------------------------
extern void XSetInputBoxMaxChars( void )
{
	InputBoxMaxChars = SDLparms.len;
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!      Get the specified number of parameters & put in Parm[]
//!
//! \param
//!		num			number of parameters expected
//!
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
static void GetParms( UBYTE num )
{
	UWORD *p;

	// Don't overflow the Parm array
	if ( num > MAX_PRM )
		num = MAX_PRM;

	// Get the specified number of parameters
	p = Parm;

	while ( num-- )
		*p++ = GetToken(  );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Get rectangle coordinates & put them in Rct
//!
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
static void GetRectCoordinates(  )
{
	Rct.x = LXtoPX( GetToken(  ) );
	Rct.y = LYtoPY( GetToken(  ) );
	Rct.width = LXtoPX( GetToken(  ) );
	Rct.height = LYtoPY( GetToken(  ) );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Get 1 lines of text and set Btn.pLabel1
//!
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
static void ButtonGetText( void )
{
	// Get the message
	GetMsg( GetToken(  ), Dspbuf );
	Btn.pLabel1 = ( UBYTE * ) Dspbuf;
}

//-----------------------------------------------------------------------------
//! \brief
//!      Get the 2 lines of text and set Btn.pLabel1 & pLabel2
//!
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
static void ButtonGetText2( void )
{
	// Get the first line message
	GetMsg( GetToken(  ), Dspbuf2 );
	Btn.pLabel1 = ( UBYTE * ) Dspbuf2;

	// Get the second line message
	GetMsg( GetToken(  ), Dspbuf );
	Btn.pLabel2 = ( UBYTE * ) Dspbuf;
}

//-----------------------------------------------------------------------------
//! \brief
//!      Common routine for drawing all types of buttons
//!
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
static void ButtonCommon( void )
{
	// Get the common button parameters
	Btn.LeftX = LXtoPX( GetToken(  ) );
	Btn.TopY = LYtoPY( GetToken(  ) );
	Btn.BTNWidth = LXtoPX( GetToken(  ) );
	Btn.BTNHeight = LYtoPY( GetToken(  ) );
	Btn.BTNState = ( UBYTE ) GetToken(  );
	Btn.BTNType = ( UBYTE ) GetToken(  );
	Btn.FontID = SDK_GuiGetFontID();
	Btn.ExFontID = SDK_GuiGetXFontID();

	Btn.EventID = GetEventID(  );

	// Draw this button
	SDK_Button( &Btn );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Display an array of buttons
//!
//! \return
//!     None
//!
//! \note
//!		Displays buttons across, then down.
//-----------------------------------------------------------------------------
static void ButtonArray( void )
{
	WORD tmp;
	char labelstr[2];
	static UWORD r, rows, c, cols;
	static UWORD x0;
	static WORD dx, dy;

	Btn.BTNType = ( UBYTE ) ( GetToken(  ) & 0x00ff );
	Btn.ContentType = CT_TEXT;
	Btn.BTNState = BUTTON_UP;
	x0 = GetToken(  );			// Coordinates of first button
	Btn.LeftX = LXtoPX( x0 );
	Btn.TopY = LYtoPY( GetToken(  ) );
	Btn.BTNWidth = LXtoPX( GetToken(  ) );
	Btn.BTNHeight = LYtoPY( GetToken(  ) );
	tmp = ( WORD ) LXtoPX( GetToken(  ) );	// x spacing
	dx = ( WORD ) Btn.BTNWidth + tmp;
	tmp = ( WORD ) LYtoPY( GetToken(  ) );	// y spacing
	dy = ( WORD ) Btn.BTNHeight + tmp;
	rows = GetToken(  );
	cols = GetToken(  );
	Btn.FontID = SDK_GuiGetFontID();
	Btn.ExFontID = SDK_GuiGetXFontID();
	labelstr[1] = 0;
	Btn.pLabel1 = ( UBYTE * ) labelstr;

	for ( r = 0; r < rows; r++ )
	{
		for ( c = 0; c < cols; c++ )
		{
			labelstr[0] = ( char ) ( GetToken(  ) & 0x00ff );

			// Get the event ID for this button
			Btn.EventID = GetEventID(  );

			// Draw this button, unless label is 0
			if ( labelstr[0] )
				SDK_Button( &Btn );

			// Adjust the X coordinate
			Btn.LeftX += dx;
		}

		// Set X & Y for start of next row
		Btn.TopY += dy;
		Btn.LeftX = x0;
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!      Draw a rectangle
//!
//! \param
//!		pRct		pointer to border rectangle in physical coordinates
//! \param
//!		bordersz	border width in pixels
//!
//! \return
//!     None
//!
//! \note
//!		border width should be in logical coordinates.
//-----------------------------------------------------------------------------
static void DrawRectangle( RECT * pRct, UWORD bordersz )
{
	RECT tmpRct;

	memcpy( ( UBYTE * ) & tmpRct, ( UBYTE * ) pRct, sizeof( RECT ) );

	// Top
	tmpRct.height = LYtoPY( bordersz );	// Border height
	SDK_GuiSetArea( &tmpRct );

	// Bottom border
	tmpRct.y += pRct->height;	// Y + Height - Border height
	tmpRct.y -= LYtoPY( bordersz );
	SDK_GuiSetArea( &tmpRct );

	// Left border
	tmpRct.y = pRct->y;			// Y
	tmpRct.width = LXtoPX( bordersz );	// Border width
	tmpRct.height = pRct->height;	// Height
	SDK_GuiSetArea( &tmpRct );

	// Right border
	tmpRct.x += pRct->width;	// X + Width - Border width
	tmpRct.x -= LXtoPX( bordersz );
	SDK_GuiSetArea( &tmpRct );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Get the next token
//!
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
static UWORD GetToken( void )
{
	UWORD tok;

	if ( PageProcessing )		// if script is on another page
	{
		// Is command buffer empty?
		if ( pBufferCommand ==
			 ( SDLCommandBuffer +
			   sizeof( SDLCommandBuffer ) / sizeof( UWORD ) ) )
		{
			// get more commands from script on other page
			SDK_ReadPage( ( UBYTE * ) SDLCommandBuffer,
						  sizeof( SDLCommandBuffer ), CurrentCommand.Page,
						  ( UBYTE * ) CurrentCommand.pCommand );
			pBufferCommand = SDLCommandBuffer;
		}
		tok = *pBufferCommand++;
		CurrentCommand.pCommand++;	// remember where we are in script
	}
	else
	{
		tok = *CurrentCommand.pCommand;
		CurrentCommand.pCommand++;
	}

	return tok;
}

//-----------------------------------------------------------------------------
//! \brief
//!      Recursive call to read another script.
//!
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
static void RecursiveScript( void )
{
	UWORD scrId;
	SDL_COMMAND SaveCommand;
	UWORD *pScript;
	SCREEN_ENTRY Screen;

	scrId = GetToken(  );		// better be a screen id

	if ( scrId <= SCREEN_START || scrId >= SCREEN_MAX )
	{
		// invalid screen id
		RunTimeError ( 0x0003 );
	}


	memcpy( ( UBYTE * ) & SaveCommand, &CurrentCommand,
			sizeof( SDL_COMMAND ) );

	SDK_ReadPage( ( UBYTE * ) & Screen, sizeof( SCREEN_ENTRY ),
				  ScreenCrossRef[scrId - SCREEN_START].Page,
				  ( UBYTE * ) ScreenCrossRef[scrId -
											 SCREEN_START].pScreenEntry );
	pScript = CurrentCommand.pCommand = Screen.pScript;

	if ( pScript )				// if script is available
	{
		CurrentCommand.Page = ScreenCrossRef[scrId - SCREEN_START].Page;
		InitReadScr(  );
		ReadScr(  );
	}

	memcpy( ( UBYTE * ) & CurrentCommand, &SaveCommand,
			sizeof( SDL_COMMAND ) );
	if ( CurrentCommand.Page != SDK_ThisPage(  ) )
	{
		PageProcessing = 1;		// page processing required
		// Empty command buffer
		pBufferCommand =
			( SDLCommandBuffer +
			  sizeof( SDLCommandBuffer ) / sizeof( UWORD ) );
	}
	else
	{
		PageProcessing = 0;		// page processing not required     
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!      Get the next token which is assummed to be an eventID
//!      and perform special event ID processing as required
//!
//! \return
//!     UWORD		event id
//!
//! \note
//!		10/28/02 changed line 1814 to EVENT_USER15 to fix SCR 3629
//-----------------------------------------------------------------------------
static UWORD GetEventID( void )
{
	UWORD id;

	id = GetToken(  );

	// process special events here
	if ( id >= EVENT_SPECIAL )
	{
		if ( EVENT_USER0 <= id && id <= EVENT_USER15 )
		{
			id = User_Events[id - EVENT_USER0];
		}
		else
		{
			// Check if Idle Button Pressed
			if ( ( EVENT_ICEBUTTON1 <= id ) && ( EVENT_ICEBUTTON4 >= id ) )
			{
				id = Verify_Idle_Btn( id );
			}
		}
	}							// end of special event processing

	return id;
}

//-----------------------------------------------------------------------------
//! \brief
//!      Justifies given text in given area
//!
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
static void TextJustify( void )
{
	RECT rct;
	// Read in 6 parameters: Justification type, vert aligmnment, X, Y, Width, Height, Msg ID
	// Parm Index            0                   1                2  3  4      5       6 

	GetParms( 7 );

	rct.x = LXtoPX( Parm[2] );
	rct.y = LYtoPY( Parm[3] );
	rct.width = LXtoPX( Parm[4] );
	rct.height = LYtoPY( Parm[5] );

	GetMsg( Parm[6], Dspbuf );

	DrawJustifiedText( &rct, Parm[0], Parm[1], Dspbuf );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Draws input box border and initial contents
//!
//! \return
//!     None
//!
//! \note
//-----------------------------------------------------------------------------
static void DrawInputBox( void )
{
	CHARSIZE charsz;
	UWORD xborderSz, yborderSz;
	static RECT WinRct, borderRct;

	// Read in 7 parameters: Input box style, Justification type, X, Y, Width, Height, Border width
	// Parm Index            0                1                   2  3  4      5     , 6           

	GetParms( 7 );

	// save input box data
	ibTextArea.sfont = SDK_GuiGetFontID();
	ibTextArea.efont = SDK_GuiGetXFontID();
	ibTextArea.justification = Parm[1];
	borderRct.x = LXtoPX( Parm[2] );
	borderRct.y = LYtoPY( Parm[3] );
	borderRct.width = LXtoPX( Parm[4] );
	borderRct.height = LYtoPY( Parm[5] );
	xborderSz = LXtoPX( Parm[6] );
	yborderSz = LYtoPY( Parm[6] );
	ibTextArea.vertalign = VCENTER;

	SDK_GuiGetVirWin( &WinRct );

	// compute border size and location
	SDK_GuiGetCharSize( &charsz );

	// compute maximum characters for input box
	InputBoxMaxDispChar =
		( ( borderRct.width - 2 * INPUTBOX_XSPACING - xborderSz * 2 ) /
		  charsz.Width );


	if ( IB_AUTO_BORDER == Parm[0] )	// border type
	{
		if ( InputBoxMaxChars < InputBoxMaxDispChar )
		{
			borderRct.width = ( InputBoxMaxChars * charsz.Width )
				+ 2 * INPUTBOX_XSPACING + xborderSz * 2;
			if ( borderRct.width > WinRct.width )
			{
				// we have exceeded given width, just make full screen size
				borderRct.width = WinRct.width;	// virtual window size
				borderRct.x = 0;
			}
			else
			{
				borderRct.x =
					( borderRct.x + ( LXtoPX( Parm[4] ) ) ) - borderRct.width;
			}
		}
		else
		{
			borderRct.x = ( WinRct.width - borderRct.width ) / 2;
		}
	}


	if ( IB_NO_BORDER != Parm[0] )	// draw border except for no border type
	{
		DrawRectangle( &borderRct, Parm[6] );	// draw border
	}

	// compute text area for input box

	ibTextArea.Rct.x = borderRct.x + INPUTBOX_XSPACING + xborderSz;
	ibTextArea.Rct.width =
		borderRct.width - 2 * INPUTBOX_XSPACING - xborderSz * 2;
	ibTextArea.Rct.height = borderRct.height - yborderSz * 2;
	ibTextArea.Rct.y = borderRct.y + yborderSz;

	if ( ( SDLScreenId == ALPHANUM1_SCR ) ||
		 ( SDLScreenId == ALPHANUM2_SCR ) ||
		 ( SDLScreenId == ALPHANUM3_SCR ) ||
         ( SDLScreenId == ALPHANUM4_SCR ) )
    {
    }
    else
    {
        DrawInputBoxText(  );
    }
}

//-----------------------------------------------------------------------------
//! \brief
//!      Draws text based on current input box parameters
//!
//! \return
//!     None
//!
//! \note
//!		Uses ibTextArea which should be set befor calling function.
//-----------------------------------------------------------------------------
static void DrawInputBoxText( void )
{
	CHARSIZE charsz;
	UBYTE len, maxlen;
	char *pTxt;

	// use font when inputbox was initialized
	SDK_GuiSetFont( ibTextArea.sfont, ibTextArea.efont );

	SDK_GuiGetCharSize( &charsz );

	// compute text length, text will be truncated from start of text 
	// if text width exceeds input box size 
	//
	// ex. Text = "abcdefg"   
	// if text box only allows 4 chars, TEXT will become
	// "defg". Starting chacters removed, this is unique feature of
	// Input Box.
	//

	len = StrLn( ( char * ) InputBoxBuffer, sizeof( InputBoxBuffer ) - 1 );
	maxlen = ( UBYTE ) ( ibTextArea.Rct.width / charsz.Width );
	if ( len > maxlen )
	{
		// chop off start of text
		memcpy( TextAreaBuffer, &InputBoxBuffer[len-maxlen],(UWORD)(maxlen+1) );
		pTxt = TextAreaBuffer;
	}
	else
	{
		pTxt = InputBoxBuffer;
	}

	DrawJustifiedText( &ibTextArea.Rct, ibTextArea.justification, VCENTER,
					   ( char * ) pTxt );

	SetCurrentLanguageFont ();
}

//-----------------------------------------------------------------------------
//! \brief
//!      This function should only be called when one of the
//!      Idle buttons has been pressed from Idle.  This 
//!      function will get the Event ID from the Idle Button
//!      Table, and convert the value to an id.  Next the
//!      id will be tested to see if it is a valid event.
//!      The ID will be set to an error  message routine
//!      when the ID is not valid.
//!
//! \param
//!		id			button ID
//!
//! \return
//!		UWORD		the event id
//!
//! \note
//-----------------------------------------------------------------------------
static UWORD Verify_Idle_Btn( UWORD id )
{
	UWORD event;

	switch ( id )
	{
		case EVENT_ICEBUTTON1:
			event = IBTAB.IBTRANBTN1[0];
			event = ( event << 8 ) | IBTAB.IBTRANBTN1[1];
			break;
		case EVENT_ICEBUTTON2:
			event = IBTAB.IBTRANBTN2[0];
			event = ( event << 8 ) | IBTAB.IBTRANBTN2[1];
			break;
		case EVENT_ICEBUTTON3:
			event = IBTAB.IBTRANBTN3[0];
			event = ( event << 8 ) | IBTAB.IBTRANBTN3[1];
			break;
		case EVENT_ICEBUTTON4:
			event = IBTAB.IBTRANBTN4[0];
			event = ( event << 8 ) | IBTAB.IBTRANBTN4[1];
			break;
	}

	// Check the value to see if valid id
	if ( ( event < END_OF_EVENTS ) ||
		 ( ( MENU_START < event ) && ( event < MENU_MAX ) ) )
		return event;

	// The ID was bad, display not supported message.
	return NOT_SUPPORTED;
}

//-----------------------------------------------------------------------------
//! \brief
//!      Draws justified text to screen buffer in given area
//!
//! \param
//!		pRct	 			   	pointer to rect containg phyical coordinates of area to draw text.
//! \param
//!		Justification			horizontal justification type JLEFT, JCENTER, or JRIGHT  
//! \param
//!		VertAlignment			vertical alignment TOP, BOTTOM, or VCENTER
//! \param
//!		pText			 		pointer NULL-terminated text 
//!
//! \return
//!		UWORD		the event id
//!
//! \note
//!		Creates mutiple lines for text too long for one
//!     line. Breaks at spaces where possible.
//-----------------------------------------------------------------------------
static void DrawJustifiedText( RECT * pRct, UWORD Justification,
							   UWORD VertAlignment, char *pText )
{
	UWORD textX, textY;
	UWORD textWidth;
	CHARSIZE charSz;
	UBYTE len, done;
	UBYTE Line, LineTotal;
	UBYTE charsPerLine;
	UBYTE maxLines;
	char *pChar;
	static struct
	{
		char *pText;
		UBYTE len;
	}
	TextData[MAX_TEXT_LINES];

	SDK_GuiGetCharSize( &charSz );

	// compute text length
	len = StrLn( pText, 0xff );

	// compute total number of lines needed and pointers to text
	charsPerLine = ( UBYTE ) ( pRct->width / charSz.Width );

	TextData[0].pText = pText;	// set first text pointer

	if ( len <= charsPerLine )	// text on one line only
	{
		LineTotal = 1;
		TextData[0].len = len;
	}
	else						// text exceeds boundary, need multi-line processing
	{
		Line = 0;
		done = 0;
		pChar = pText;

		maxLines = pRct->height / charSz.Height;

		maxLines = ( maxLines > MAX_TEXT_LINES ) ? MAX_TEXT_LINES : maxLines;

		if ( charsPerLine == 0 )
			done = 1;			// exit loop

		do
		{
			pChar = pChar + charsPerLine - 1;	// point to last character on line

			// go backward through text to find a space
			for ( len = charsPerLine; len > 0; len--, pChar-- )
			{
				if ( *pChar == ' ' || len == 1 )	// if at a space or first character of string
				{
					if ( *pChar == ' ' )
					{
						TextData[Line].len = len - 1;	// we found a space, do not include
						// space as part of text
						pChar++;	// pointer to start of next line
					}
					else
					{
						TextData[Line].len = charsPerLine;
						pChar = pChar + charsPerLine;	// pointer to start of next line
					}

					while ( *pChar == ' ' )	// want to skip any leading spaces
					{
						pChar++;
					}

					if ( *pChar == 0 )	// if next string is NULL string, exit
					{
						done = 1;
						break;
					}

					Line++;
					TextData[Line].pText = pChar;

					// done if remaining text will fit on line or max lines reached
					if ( ( len = StrLn( pChar, 255 ) ) <= charsPerLine ||
						 ( Line + 1 ) >= maxLines )
					{
						// save length of last line
						TextData[Line].len =
							( len > charsPerLine ) ? charsPerLine : len;
						done = 1;	// last line complete, so exit outer loop
					}
					break;		// this line done so break
				}
			}
		}
		while ( !done );

		LineTotal = Line + 1;	// save the total number of lines
	}

	// now justify and draw each line in do loop
	Line = 0;

	do
	{
		// compute text length
		len = TextData[Line].len;

		// compute text size in pixels - remove white spacing on end for accuracy
		textWidth =
			( ( UWORD ) len ) * charSz.Width - ( charSz.Width -
												 charSz.Width_NoSpc );

		switch ( VertAlignment )
		{
			case TOP:
				textY = pRct->y + Line * charSz.Height;
				break;

			case VCENTER:
				textY =
					pRct->y + ( pRct->height -
								LineTotal * charSz.Height_NoSpc ) / 2 +
					Line * charSz.Height;
				break;

			case BOTTOM:
				textY =
					pRct->y + pRct->height - ( LineTotal -
											   Line ) * charSz.Height;
				break;
		}

		switch ( Justification )	// justification type
		{
			case JLEFT:		// just draw text at left side of input box
				textX = pRct->x;
				break;

			case JRIGHT:		// justify at right, last character must stop at right border
				textX = ( pRct->x + pRct->width ) - textWidth;
				break;

			case JCENTER:		// compute x for center justification
				textX = pRct->x + ( pRct->width - textWidth ) / 2;
				break;

			default:
				break;
		}


		if ( LineTotal == 1 )	// single line
		{
			SDK_GuiTextP( textX, textY, ( char * ) TextData[Line].pText );
			break;				// just one line, so exit
		}
		else					// multi-line
		{
			len = TextData[Line].len;
			memcpy( SDLTextLineBuf, TextData[Line].pText, len );
			SDLTextLineBuf[len] = 0;	// NULL-terminate
			SDK_GuiTextP( textX, textY, SDLTextLineBuf );

			// next text line
			Line++;
		}
	}
	while ( Line < LineTotal );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Justifies text on a line
//!
//! \return
//!		None
//!
//! \note
//-----------------------------------------------------------------------------
static void TextLineJustify( void )
{
	TEXTPARMS tp;
	RECT rct;
	RECT WinRct;
	CHARSIZE charSz;

	// Read in 3 parameters: Line                Justification type, Msg ID
	// Parm Index            0                   1                   2     

	// Get the line & justification parameters
	GetParms( 3 );

	// Get the text
	GetMsg( Parm[2], Dspbuf );

	// Get Text Parameters
	SDK_GuiVirWinTextParms( &tp );
	SDK_GuiGetCharSize( &charSz );
	SDK_GuiGetVirWin( &WinRct );


	rct.x = tp.Xoff;
	rct.y = tp.Yoff + Parm[0] * charSz.Height;
	rct.width = WinRct.width - tp.Xoff * 2;
	rct.height = charSz.Height;

	DrawJustifiedText( &rct, Parm[1], TOP, Dspbuf );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Sets up text area and draws initial contents
//!
//! \return
//!		None
//!
//! \note
//-----------------------------------------------------------------------------
static void DrawTextArea( void )
{
	TEXTAREA_PARMS *pTA;

	// Read in 8 parameters: Id, Justification type, Vertical align X, Y,
	// Parm Index            0   1                   2              3  4 

	// Width, Height,  default message id
	// 5    , 6     ,  7                 

	GetParms( 8 );

	if ( Parm[0] >= NUM_OF_TEXTAREAS )
		return;

	pTA = &taparms[Parm[0]];
	pTA->sfont = SDK_GuiGetFontID();
	pTA->efont = SDK_GuiGetXFontID();
	pTA->justification = Parm[1];
	pTA->vertalign = Parm[2];
	pTA->Rct.x = LXtoPX( Parm[3] );
	pTA->Rct.y = LYtoPY( Parm[4] );
	pTA->Rct.width = LXtoPX( Parm[5] );
	pTA->Rct.height = LYtoPY( Parm[6] );

	// Get the text
	GetMsg( Parm[7], Dspbuf );

	DrawJustifiedText( &pTA->Rct, pTA->justification, pTA->vertalign,
					   Dspbuf );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Converts logical x-coordinate or width to actual physical coordinate or width
//!
//! \param
//!		ogicalX			logical x coordinate or width
//!
//! \return
//!		UWORD			converted X value
//!
//! \note
//-----------------------------------------------------------------------------
static UWORD CalculateX( UWORD logicalX )
{
	return ( UWORD ) ( ( SCR_PIXEL_WIDTH * logicalX ) / LogicalWidth );
}

//-----------------------------------------------------------------------------
//! \brief
//!      Converts logical y-coordinate or height to actual physical coordinate or height
//!
//! \param
//!		ogicalY			logical y coordinate or heightpExample
//!
//! \return
//!		UWORD			converted Y value
//!
//! \note
//-----------------------------------------------------------------------------
static UWORD CalculateY( UWORD logicalY )
{
	return ( UWORD ) ( ( SCR_PIXEL_HEIGHT * logicalY ) / LogicalHeight );
}

