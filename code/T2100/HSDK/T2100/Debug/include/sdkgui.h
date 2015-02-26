/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/
//!
//! \file sdkgui.h
//!     \brief SDK GUI library defintions and function prototypes.<br>
//!     This file should be included in all 'C' source code files. 
//!     \since 1999
//!

#ifndef SDKGUI
#define SDKGUI


#ifdef __cplusplus
extern "C" {
#endif

/* include files with typdefs needed for function prototypes */
#ifdef MAKE_VHDT
	#if !defined( MAKE_VIKING )
		#include <..\include\stdio.h>
		#include "hfs.h"
	#else
		#include <stdio.h>
		#include "usevhdtfilesys.h"
	#endif
#else
	#include <stdio.h>
#endif

#include "basictyp.h"
#include "sdk.h"
#include "sdkdsp.h"
#include "osgui.h"

/**
	@addtogroup GroupSDK
	@{
*/

// **************** DEFINITIONS AND ENUMERATIONS ******************
/* guibtn.inc */

/* Button definitaions for use in BTN_Entry structure */
#define BUTTON_DOWN	0	///< Down Button
#define BUTTON_UP	1	///< Up button

/*	Usefull for BTNType field of BTN_Entry structure */
#define BUTTON_INVERT	0x04	///< rectangular border, normal in up state, and inverted in down state
#define BUTTON_3D		0x08	///< special border with shading. Shading appears at right and bottom for up state, at left and top for down state
#define BUTTON_SQUARE	0x10	///< rectangular border, up and down states are the same
#define BUTTON_NOBORDER	0x20	///< no border; up and down states are the same
#define BUTTON_PCX		0x40	///< Button with PCX image pLabel1 points to up image for the button, pLabel2 points to down image or NULL. ContentType is ignored for this buton type

/*	Usefull for ContentType field of BTN_Entry structure */
#define CT_TEXT		0x00	/*!< \brief A single line of text centered on the button.

                                     Drawn in the font specified in FontID and ExFontID structure
                                     members. The pLabel1 member points to the text. If this content
                                     type is specified, pLabel2 is not used.\n\n
				     CAUTION: The text is not truncated to fit the button. */

#define CT_IMAGE	0x01	/*!< \brief In the center of the button, draws an image as specified by the
                                     user.

				     The pLabel1 member of the BTN_Entry structure points to the image.
				     See SDK_GuiImage for information on the format of the image data.\n\n
				     CAUTION: The image is not resized to fit the button.*/

#define CT_CUSTOM	0x02	/*!< \brief The button has no content, only a border.

                                     After calling SDK_Button(), users can draw custom items inside
                                     the button. The pLabel1 and pLabel2 members are not used.*/

#define CT_TEXT_2LINE	0x03	/*!< \brief Two lines of text centered on the button.

                                     Drawn in the font specified in FontID and ExFontID structure
                                     members. The pLabel1 member points to the text for the line.
                                     The pLabel1 member points to the text for line 2.\n\n
				     CAUTION: The text is not truncated to fit the button.*/
#define CT_TRANSPARENT	0x04	/*!< \brief The button has no content and no border.

                                     Whatever is on the screen remains on the screen when the button
                                     is shown. This is useful for detecting touches on existing
                                     screen content in a particular area.*/
#define CT_TEXTIMAGE	0x05

/* message box definitions */

/**
	Message box types.
*/
typedef enum {
	MB_OKCANCEL,
	MB_OKONLY
} MESSAGEBOX_TYPE;

/* message box button event id's */
#define MB_OK		1
#define MB_CANCEL	2

/*-------------------------------------------------------------------
Structures used:
---------------------------------------------------------------------*/
#if defined(_MSC_VER)	// Visual C++
#pragma pack(1)
#endif

/**	@brief	Button definition*/
typedef struct
{
	UWORD LeftX;		///< leftmost x pixel of button
	UWORD TopY;		///< topmost y pixel of button
	UWORD BTNWidth;		///< width of button in pixels
	UWORD BTNHeight;	///< height of button in pixels
	UBYTE BTNState;		///< button initial state (#BUTTON_UP or #BUTTON_DOWN)
	UBYTE BTNType;		///< button type (#BUTTON_INVERT, #BUTTON_3D, #BUTTON_SQUARE, #BUTTON_NOBORDER, #BUTTON_PCX)
	UBYTE ContentType;	///< content type (#CT_TEXT, #CT_TEXT_2LINE, #CT_IMAGE, #CT_CUSTOM, #CT_TRANSPARENT)
	const UBYTE *pLabel1;	///< pointer to button label for line 1
	const UBYTE *pLabel2;	///< pointer to button label for line 2 or pointer to image
	UWORD EventID;		///< event ID associated with this button
	UBYTE FontID;		///< standard font to use for button text
	UBYTE ExFontID;		///< extended font to use for extended characters in text
	PAGE_NUMBERS Page;	///< page for PCX Images (Ice6000,6500 only)
#if defined(MAKE_M2)
	COLOR BackgroundColor;
	COLOR TextColor;
	COLOR BorderColor;
	char* BackgroundImage;	// pointer to background image if it exists
#endif //MAKE_M2
} BTN_Entry;

#if defined(_MSC_VER)	// Visual C++
#pragma pack()
#endif

/**	@brief	Character definition*/
typedef struct
{
	UWORD Width;		///< character width pixels, including space between characters
	UWORD Height;		///< character height pixels, including space between lines
	UWORD Width_NoSpc;	///< same as Width minus space between characters
	UWORD Height_NoSpc;	///< same as Height minus space between lines
} CHARSIZE;

#if defined(_MSC_VER)	// Visual C++
#pragma pack(1)
#endif

/**	@brief	Virtual window text parametrs*/
typedef struct
{
	UBYTE MaxLines;		///< max lines in current virtual window
	UBYTE MaxChars;		///< max characters in one line for current virtual window
	UWORD Xoff;		///< pixel x offset to first character
	UWORD Yoff;		///< pixel y offset to top of first line
}  TEXTPARMS;

#if defined(_MSC_VER)	// Visual C++
#pragma pack()
#endif

/**
	Message box definition.
*/
typedef struct
{
	MESSAGEBOX_TYPE Type;
	char *pText;			
	char *pTitle;			
	UBYTE FontID;			
	UBYTE ExFontID;
} MESSAGEBOX;

#if defined(_MSC_VER)	// Visual C++
#pragma pack(1)
#endif

/**	@brief	Some image parametrs */
typedef struct
{
	UWORD Height;		///< image height
	UWORD Width;		///< image width
	UWORD UserId;		///< user id
	UBYTE CompressionType;	///< image compression type: 1 for compressed, else 0
}  IMAGE_PARMS;

#if defined(_MSC_VER)	// Visual C++
#pragma pack()
#endif

#if defined(_MSC_VER)	// Visual C++
#pragma pack(1)
#endif

/**	@brief	Unicode font params */
typedef struct 
{
	UBYTE NumBytes;		///< Character Size ( in bytes ).
	UBYTE Width;		///< Width  of each character.
	UBYTE Height;		///< Height of each character.
	UBYTE Spacing;		///< Width  of space between each character.
	UBYTE Leading;		///< Height of space between each character.
	UBYTE StandardFont;	///< Standard Font ID for ASCII characters.
	UBYTE Aligment;		///< Vertical font adjustment for ASCII.
} SDK_UNICODE_HEADER;

/**
	Unicode font ranges.
*/
typedef enum
{
	UNI_RANGE_0,
	UNI_RANGE_1,
	UNI_RANGE_2,
	UNI_RANGE_3,
	UNI_RANGE_4,
	UNI_RANGE_LAST
} SDK_UNICODE_RANGES;

/**
	Unicode range definition.
*/
typedef struct
{
	UBYTE Index;			///< Index range.
	UBYTE SubRangeFirst;	///< Low  boundary SubRange.
	UBYTE SubRangeLast;		///< High boundary SubRange.
	UBYTE *BitMap;
} UNICODE_RANGE;

#if defined(_MSC_VER)	// Visual C++
#pragma pack()
#endif


#if !defined(MAKE_MONOCHROME)


#ifndef _TARG_Z80
#pragma pack(1)
#endif

// video pages, 16 pages 0 - 15
typedef enum
{
    video_PAGE0,
    video_PAGE1,
    video_PAGE2,
    video_PAGE3,
    video_PAGE4,
    video_PAGE5,
    video_PAGE6,
    video_PAGE7,
    video_PAGE8,
    video_PAGE9,
    video_PAGE10,
    video_PAGE11,
    video_PAGE12,
    video_PAGE13,
    video_PAGE14,
    video_PAGE15
} VIDEOPAGE;

#ifndef _TARG_Z80
#pragma pack()
#endif

#endif

#if defined(T7FAMILY_OS) || defined(MAKE_P2100) || defined(MAKE_H2200)

typedef struct
{
	// Remapping Table
	UBYTE PageRemapCodes;
	UWORD NumberRemapCodes;
	UBYTE *pRemapCodes;

	// Bitmaps to load in CGRAM
	UBYTE PageCGRAMBitmaps;
	UBYTE *pCGRAMBitmaps;
} CGRAM_REMAP_DATA;

#endif

// ******************* PROTOTYPE DEFINITIONS ************************

/** 	@brief	Initializes the SDK graphical user interface (GUI) and other SDK screen functions.

        This function initializes data buffers and other data used by the SDK GUI functions. This
        function is used with other GUI-related functions such as SDK_GuiRectangle() and other SDK
        screen-related functions such as SDK_Button(). The SDK_GuiInit function must be called once
        in an application, usually near the start.

        This function also initializes the SDK internal Address table and Cross-call table as by
        functions SDK_RegisterAddress( ) and SDK_RegisterCrossCall( ).*/
extern  void SDK_GuiInit( void );

/** 	@brief	Sets the standard and extended fonts.

        This function sets the font for SDK_GuiText() and SDK_GuiTextP() to use. The available fonts
        are defined in sdkgui.h. The width and height of the extended font must be the same as the
        standard font. If not, extended characters are not drawn properly. Before using this
        function, SDK_GetFontID() and SDK_GetXFontID() can be used to save the currently set font.
 	@param 	s - standard font id
 	@param	e - extended font id
	@note	Please refer to the Appendix B for the pre-built fonts available for each HSDK release.*/

#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiSetFont( UBYTE s, UBYTE e );
#endif

/** 	@brief	Sets the virtual window location and size.

        This function sets the virtual window coordinates. All following calls to SDK_GUI functions
        are relative to the coordinates this function sets. To return the coordinates to the full
        screen, use SDK_GuiDefWindow().
        @param  pRect - pointer to the RECT structure that contains the bordering rectangle of the
	        virtual window. RECT structure units are in pixels
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiSetWindow( const RECT *pRect );
#endif

/** 	@brief	Draws text into the screen buffer starting at the given pixel location.

        This function draws text starting at the given pixel coordinates. The x and y input
        coordinates are relative to the virtual window. The text is drawn into the screen buffer
        that pText points to.

        After setting up the data by using the SDK_GuiTextP(), use the SDK_GuiDispAll() function to
        actually display the text. The text is drawn using the currently set font.
	
	See SDK_GuiSetFont() to change the font settings.

 	@param 	x - x - coordinate
 	@param 	y - y - coordinate
 	@param 	txt - pointer to the text to be drawn
*/
extern  void SDK_GuiTextP( UWORD x, UWORD y, char *txt );

/** 	@brief	Draws text into the screen buffer starting at the given line and character location.

        This function draws text starting at the given line and character locations. The text is
        drawn into the screen buffer.

        Use SDK_GuiDispAll() to display the text relative to the coordinates of the virtual window
        last set by SDK_GuiSetWindow(). Line and character locations begin at 0. The number of lines
        and character locations available depends on the currently set font and the size of the
        virtual window. Text is drawn in the currently set font. See SDK_GuiSetFont().

 	@param 	l - line
 	@param 	c - column
 	@param 	*txt - pointer to text to be drawn
*/

extern  void SDK_GuiText( UBYTE l, UBYTE c, char *txt );

/** 	@brief	Clears the GUI screen buffer.

        This function clears the GUI screen buffer and sets the virtual window to the full-screen
        default window. The function is most often used before creating a new screen. This function
        also invalidates any buttons previously drawn using SDK_Button(). However, the screen itself
        is not cleared until SDK_GuiDispAll() is called.
*/ 
extern  void SDK_GuiClearScr( void );

/** 	@brief	Sets the pixels in a specific area of the screen buffer.

        This function sets the pixels to black in a given area of the screen buffer. The results do
        not appear on the screen until SDK_GuiDispAll() or SDK_GuiDispPart()) is called. The set
        area is relative to the virtual window last set by SDK_GuiSetWindow().
        @param pRect - pointer to the RECT structure containing the rectangular area to be set. RECT
		        structure units are in pixels
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiSetArea( RECT *pRect );
#endif

/** 	@brief	Inverts a specific area of the screen buffer. 

        This function inverts a given area of the screen buffer. The results do not appear on the
        screen until SDK_GuiDispAll() or SDK_GuiDispPart() is called. The inverted area is relative
        to the virtual window last set by SDK_GuiSetWindow().
        @param pRect - pointer to the RECT structure containing the rectangle of the area to be
		        inverted. RECT structure units are in pixels
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiInvArea( RECT *pRect );
#endif

/** 	@brief	Clears a given area in the screen buffer.

        The function clears a given area of the screen buffer. The results do not appear on the
        screen until SDK_GuiDispAll() or SDK_GuiDispPart() is called. The cleared area is relative
        to the virtual window as last set by SDK_GuiSetWindow().

        @param pRect - pointer to the RECT structure containing the rectangle of area to be cleared.
	        The RECT structure units are in pixels
*/
extern  void SDK_GuiClrArea( RECT *pRect );

/** 	@brief	Gets the rectangle that borders the virtual window.

        This function gets the rectangle that borders the virtual window. The virtual window is as
        previously set by SDK_GuiSetWindow().
        @param  pRect - pointer to the RECT structure for storage of the rectangle that borders the
		        virtual window. RECT structure units are in pixels
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiGetVirWin( RECT *pRect );
#endif

/** 	@brief	Converts character and line position to pixel coordinates.

        This function converts a LINE and a CHARACTER position to ROW and COLUMN pixel coordinates.
        The LINE and CHARACTER position and pixel coordinates are all relative to the virtual
        window.

	The SDK_GuiChar2Pixel function is also located in the SDKGUI.H header file. 

 	@param c - character position
 	@param l - line
  	@return	UWORD. High byte - pixel row position, low byte - pixel column position
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  UWORD SDK_GuiChar2Pixel ( UBYTE c, UBYTE l);
#endif

/** 	@brief	Gets the character size for the current font.

        This function gets the character size for the current font set by SDK_GuiSetFont(). The
        character size parameters are placed into the CHARSIZE structure pointed to by pCharSize as
        passed to the function.

        @param  pCharSize - pointer to the CHARSIZE structure where the character size parameters
		        are to be stored. CHARSIZE structure units are in pixels
*/
extern  void SDK_GuiGetCharSize ( CHARSIZE *pCharSize );

/** 	@brief	Gets the text parameters associated with the virtual window and the current font.

        This function gets various text parameters associated with the current virtual window and
        current font. The text parameters are placed into the TEXTPARMS structure pointed to by
        pTextParms as passed to the function.

	The TEXTPARMS structure is defined in sdkgui.h.
 	@param 	pTextParms - pointer to the TEXTPARMS structure where text parameters are to be stored
*/

extern  void SDK_GuiVirWinTextParms( TEXTPARMS *pTextParms );

/** 	@brief	Draws a horizontal line into the screen buffer.

        This function draws a horizontal line of a given length at a given location. The line is
        drawn into the screen buffer relative to the virtual window. Use SDK_GuiDispAll() or
        SDK_GuiDispPart() to display the line. The line is drawn from left to right.

 	@param x - Start X
	@param y - Start Y
 	@param l - Length
 	@note Drawn from left to right*/

#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiHorizontal( UWORD x, UWORD y, UWORD l );
#endif
 
/** 	@brief	Draws a vertical line into the screen buffer.

        This function draws a vertical line of given length at given location. The line is drawn
        into the screen buffer relative to the virtual window. Use SDK_GuiDispAll() or
        SDK_GuiDispPart() to actually display the line. The line is drawn starting from top.

 	@param 	x - start X
 	@param 	y - start Y
 	@param 	l - lenght
	*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiVertical( UWORD x, UWORD y, UWORD l );
#endif
 
/** 	@brief	Draws a rectangle in the screen buffer. 

        This function draws a rectangle in the screen buffer. The rectangle does not appear on the
        screen until SDK_GuiDispAll() or SDK_GuiDispPart() is called. The size and location of the
        rectangle is determined by the RECT structure pointed to by pRect as passed to the function.
        @param pRect - pointer to the RECT structure containing the size and location of the
		        rectangle to be drawn. RECT structure units are in pixels
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiRectangle( RECT *pRect );
#endif

/** 	@brief	Displays the screen buffer.

        The function places the complete screen buffer onto the screen. Whatever the size of the
        virtual window, the full screen buffer is shown.
*/
extern  void SDK_GuiDispAll( void );

/** 	@brief	Displays a portion of the screen buffer.

        This function displays a portion of the screen buffer. It puts part of the screen buffer on
        the actual screen as determined by the given RECT structure. Other areas of the screen are
        unaffected. The area to be displayed is not relative to the virtual window.

	This function rounds the displayed area to the nearest 8th column.

        @param  pRect - pointer to the RECT structure containing the area of the screen buffer to
	        display. RECT structure units are in pixels
*/
extern  void SDK_GuiDispPart( RECT *pRect );

/** 	@brief	Displays the virtual window part of the screen buffer.

        This function puts the virtual window part of the screen buffer onto the actual screen. The
        virtual window is as last set by SDK_GuiSetWindow(). Other areas of the screen are not
        affected.
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiDispVirWin( void );
#endif

/** 	@brief	Draws an image into the screen buffer. 

        This function draws an image into the screen buffer at the pixel location given by the x and
        y parameters. Function SDK_GuiDispAll() or SDK_GuiDispPart() must be used to display the
        image.

	Image format: @include ImageFormat.txt

 	@param x - pixel x coordinate
 	@param y - pixel y coordinate
	@param img - pointer to image data
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiImage( UWORD x, UWORD y, const UBYTE *img );
#endif

/** 	@brief	Retrieves the current extended font identifier.

        This function gets the current extended font set by SDK_GuiSetFont(). The function is most
        useful in saving the current font before changing it.
	@return	Returns the extended font ID of the current font
*/ 
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  UBYTE SDK_GuiGetXFontID( void );
#endif

/** 	@brief	Gets the identifier for the current standard font.

        This function gets the current standard font set by SDK_GuiSetFont(). The function is most
        useful in saving the current font before changing it.
	@return Returns the standard font ID of the current font
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  UBYTE SDK_GuiGetFontID( void );
#endif

/**	@example SDK_GuiGetFontID.c */

/** 	@brief	Sets the virtual window to the full-screen default window. Sets a window to a specific screen size.

        This function sets the virtual window to the full screen default window. All subsequent
        calls to SDK_GUI functions are relative to the full screen after this function is called.
*/
extern  void SDK_GuiDefWindow( void );

/** 	@brief	Draws a button into the screen buffer.

        This function draws a button as specified by the supplied BTN_Entry structure. The button is
        drawn relative to the virtual window in the screen buffer. As with other SDK GUI functions,
        you must call SDK_GuiDispAll() or SDK_GuiDispPart() before the button is shown on the
        screen.

        In addition to drawing the button, this function adds the button to a table that maintains
        the state and location of all buttons drawn on the screen. The user can then use
        SDK_GetButtonEvent() to indicate which button, if any, was pressed. SDK_GetButtonEvent()
        returns an ID specified by the EventID member of the BTN_Entry structure. Since the default
        return value of SDK_GetButtonEvent() is zero, the event ID should always be specified as a
        non-zero number. The Button table is also maintained so that SDK_RedrawButton() can be used
        to change the state of a button.

        The Button table is cleared when either SDK_GuiClearScr() or SDK_InvalidateButtons() is
        called. Several types of buttons can be drawn with text or image content. Text is drawn in
        the font specified in the BTN_Entry structure. SDK_Button() saves the current font function
        and restores it after the button is drawn.

	The BTN_Entry structure to be filled by user is defined in sdkgui.h.

        @param  button - a pointer to the BTN_Entry structure, which contains the size, location,
	        type, and other aspects of the button to be drawn
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern void SDK_Button ( const BTN_Entry *button );
#endif

/**	@example SDK_Button.c */

/**     @brief  Redraws a button that contains a specific screen point.

        The button must have been previously drawn using SDK_Button(). The button is drawn in
        BUTTON_UP or BUTTON_DOWN state as specified.

        This function redraws a button that contains the specified screen point at x, y. The button
        is drawn in the state specified by the btn_state parameter. The button is drawn on the
        actual screen as well as in the screen buffer. You do not need to call SDK_GuiDispAl() or
        SDK_GuiDispPart().

        The point passed to this function is usually obtained by reading the touch pad. See
        SDK_SetPad() and SDK_GetPad().

        SDK_RedrawButton() searches a table of buttons on the screen, and if a button contains the
        point, the button is drawn in the state specified by the btn_state parameter.

        If no button is found that contains the point, the function just returns to the caller. The
        state specified in btn_entrys usually corresponds to the state of the touch-pad pen. (See
        the following example.)

        The screen point passed to SDK_RedrawButton() is considered to be relative to the full
        screen, and the current virtual window has no effect. Although SDK_Button() draws buttons
        relative to the virtual window, their coordinates are stored relative to the full screen.
        Therefore, the point passed to this function is relative to the full screen.

 	@param	x - x pixel coordinate of the screen point
	@param	y - y pixel coordinate of the screen point
	@param	btn_state - state to draw button, BUTTON_UP or BUTTON_DOWN
	@note	Redraws only buttons drawn using SDK_Button(). Whenever,
		SDK_GuiCLearScr() is called the buttons are no longer valid.
		Buttons can also be made invalid for update by calling SDK_InvalidateButtons().
*/ 
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern void SDK_RedrawButton(UWORD x,UWORD y, UBYTE btn_state);
#endif

/**	@example SDK_RedrawButton.c */

/** 	@brief	Removes from the Button table all buttons drawn by SDK_Button(). 

        This function removes buttons drawn by SDK_Button() from the Button table maintained by SDK.
        The button is not removed from the actual screen or the screen buffer.

        Calls to SDK_GetButtonEvent() and SDK_RedrawButton() have no effect after
        SDK_InvalidateButtons() is called.

        This function is probably only useful when a user is drawing a new set of buttons in a
        portion of the screen. Normally, a whole screen is cleared using SDK_GuiClearScr(), which
        also removes all buttons from the table. However, if only a portion of the screen is cleared
        using SDK_GuiClrArea, for example, any buttons in that area still remain in the Button
        table.

        In some cases, the user might want to remove these buttons from the table and draw a new set
        of buttons. Then, SDK_InvalidateButtons(); should be used.
*/ 
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_InvalidateButtons ( void );
#endif

/**	@example SDK_InvalidateButtons.c */

/**     @brief  Gets the event ID of a button that contains a specified screen point.

	This button must have been previously drawn using the function SDK_Button().

        This function returns the event ID of a button drawn by SDK_Button(), which contains the
        specific screen point at coordinates X,Y. The returned event ID is specified in a previous
        call to SDK_Button();

	The point passed to this function is usually obtained by reading the touch-pad.

	See SDK_SetPad() and SDK_GetPad().

        SDK_GetButtonEvent() searches a table of buttons on the screen, and if a button contains the
        point, the event ID of the button is returned.

        If no button is found that contains the point, zero is returned. Generally, buttons should
        not be assigned an event ID of zero. The event ID is specified in the EventID member of the
        BTN_Entry structure passed to SDK_Button().

        The screen point passed to SDK_GetButtonEvent() is considered to be relative to the full
        screen, and the current virtual window has no effect. Although buttons are drawn by
        SDK_Button() relative to the virtual window, their coordinates are stored relative to the
        full screen. Therefore, the point passed to this function is relative to the full screen.

 	@param	x - x pixel coordinate of the screen point
	@param	y - y pixel coordinate of the screen point
        @return Event ID of a button that contains the specified point.If no button contains the
	        point, zero is returned
*/ 
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  UWORD SDK_GetButtonEvent(UWORD x,UWORD y);
#endif

/**	@example SDK_GetButtonEvent.c */

#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
#if defined(MAKE_ICE5000) || defined (MAKE_ICE4000) || defined(MAKE_VHDT)

/** 	@brief	Saves the entire screen buffer.

        This function saves the present screen buffer. Use SDK_GuiRestoreScr(); to restore the
        present buffer. Since only one screen-save buffer exists, nesting this function produces
        undesired results.

	@note This function saves buttons but does not invalidate them. Users can call
        SDK_InvalidateButtons() or SDK_GuiClearScr() to invalidate buttons. A call to
        SDK_GuiRestoreScr(); still restores buttons totally.
*/ 
extern  void SDK_GuiStoreScr( void );

/** 	@brief	Restores a previously saved screen buffer
        This function restores the screen buffer as previously saved with SDK_GuiStoreScr(); This
        function also restores any buttons saved by SDK_GuiStoreScr().
*/ 
extern  void SDK_GuiRestoreScr( void );

/**	@example SDK_GuiRestoreScr.c */

/** 	@brief	Puts a letter keypad in the screen buffer. Starting location is (0,15).
	@note	Use GetButtonEvent() to determine key presses, the returned code is ASCII. The Clear key is  
		set to 0x1b ( ASCII code for Escape ).  	  
*/
extern  void SDK_AlphaKeyPad(void);

/**     @brief  Draws a QWERTY keypad to the screen buffer. Each key is a button with an
		ASCII-assigned event ID. The caller can use SDK_GetButtonEvent() to read keys pressed.

        This function draws a mini-QWERTY keypad to the screen buffer. The SDK function
        SDK_GuiDispAll() or SDK_GuiDispPart() must be called to show the keypad on the screen. Each
        key is a button with an ASCII-assigned event ID. The caller can use SDK_GetButtonEvent() to
        read keys pressed.

        The top-left corner of the keypad is at pixel location 0,15 and uses the entire screen below
        y=15. The screen from the upper-left corner at 0,0 to the lower-right corner at 160,15 is
        available to the user. This part of the screen is usually used for character input.

        The alpha keys are upper case. Numeric keys 0-9 and several keys such as a Space, Clear,
        Backspace, and Enter are also available. This function assigns normal ASCII codes to the
        button event ID of each key. The Clear key does not have exact ASCII codes and returns the
        ASCII code for Escape or 1Bh.

	IMPORTANT: This is an obsolete function not recommended for new development. 

	@note	Use GetButtonEvent() to determine key presses, the returned code is ASCII.
*/
extern  void SDK_QwertyKeyPad(void);

/**	@example SDK_QwertyKeyPad.c */
 
/** 	@brief	Displays a message box on the screen.

					   Saves present screen which is restored
					   when SDK_CloseMessageBox() is called. 	
					   Present button table is also saved.	
                      
					   The message box contains a user specified
					   title and text message. The text message 
					   is wrapped to next line if neccessary.
					   Two lines are available. Pointers to
					   the text and title are in the MESSAGEBOX
					   structure passed to this function.	

					   The message box contains 1 or 2 buttons,	
					   depending upon the type. The available 
					   types are MB_OKONLY and MB_OKCANCEL.
  					   The 'OK' button is assigned an event id of
					   MB_OK. The 'CANCEL' button is assigned an
					   event id of MB_CANCEL. The type is also
					   set in the MESSAGEBOX structure.
						
					   User is responsible for reading touch pad. 
					   SDK_GetButtonEvent() is used to get the
					   event id.	

	@param	pMessageBox -  pointer to MESSAGEBOX structure continaing message box parameters	 
*/
extern void SDK_OpenMessageBox(MESSAGEBOX *pMessageBox);

/** 	@brief	Closes a message box as previously opened by 
					   SDK_OpenMessageBox(). Restores screen
					   as it was before last call to SDK_OpenMessage().	
					   Also, restores any buttons.	
*/
extern void SDK_CloseMessageBox(void);

/** 	@brief	Puts a row of numeric buttons 1->9,0 on screen. Starting at location is (0,y) where y is specified.  
	@param	TopY - y pixel location to draw keypad
	@note	Use GetButtonEvent() to determine key presses, the returned code is ASCII. 
*/
extern  void SDK_NumericKeyPad(UWORD TopY);

#endif // #if defined( MAKE_ICE5000) || defined (MAKE_ICE4000)
#endif // !P2100

/**     @brief  Converts specific coordinates relative to the virtual window to absolute coordinates
		relative to the full screen.

        This function converts the specific coordinates relative to the virtual window to absolute
        coordinates relative to the full screen. The virtual window is as last defined by
        SDK_GuiSetWindow(). The converted coordinates are placed in a POINT structure pointed to by
        the pPoint parameter.

 	@param x - x pixel to convert
 	@param y - y pixel to convert
 	@param pPoint - pointer to a POINT structure where the absolute coordinates are to be stored
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiConvertToAbs( UWORD x, UWORD y, POINT *pPoint);
#endif

/** 	@brief	Clears the virtual window part of the screen buffer.

        This function clears the virtual window part of the screen buffer. The display itself is not
        cleared until SDK_GuiDispAll(), SDK_GuiDispPart(), or SDK_GuiDispVirWin is called.

        The virtual window is as last set by SDK_GuiSetWindow(). Other areas of the screen buffer
        are unaffected.
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern  void SDK_GuiClearVirWin( void );
#endif

/**	@example SDK_GuiClearVirWin.c */

/** 	@brief	Clears a line of text in the screen buffer at the given line location.

        This function clears a line of text at the given line. The line is cleared in the screen
        buffer. You must call one of the SDK_GuiDisXXX() functions to clear the line on the screen.

        The cleared line is relative to the coordinates of the virtual window last set by
        SDK_GuiSetWindow(). The location and size of the line area cleared depends on the currently
        set font and the size of virtual window.

	@param	line - zero-based line number
*/
void SDK_GuiClearLine( UBYTE line );

/** 	@brief	Displays a line of the screen buffer at the given line location

        This function displays a line area of the screen buffer. This function is usually used to
        show a line of text as drawn by SDK_GuiText(). The line number is relative to the
        coordinates of the virtual window last set by SDK_GuiSetWindow().

	@note	line is zero based, location of line depends upon location of virtual window and curent font*/
void SDK_GuiDispLine( UBYTE line );

/** 	@brief	Gets the width, height, user ID, and compression type from an image data buffer. 

        This function places image parameters into the IMAGE_PARM structure of the caller. The image
        parameters are width, height, user ID, and compression type. There is no error checking of
        the validity of the image pointed to by pImage.

        Image parameters are stored in an image buffer as shown in the following list. This function
        provides a convenient way to obtain some of the more useful image parameters.

	Image format: \include ImageFormat.txt

	The IMAGE_PARM structure is defined in SDKGUI.H.

 	@param	pImage - pointer to the image data
 	@param	pIparms - pointer to the structure to store the image parameters
 	@note Image buffer must be in Hypercom's format. No validation of image buffer is made. Works for printer or screen images.
 	*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
void SDK_GetImageParms( UBYTE *pImage, IMAGE_PARMS *pIparms);
#endif
 
/** 	@brief	Gets the width, height, user ID, and compression type from an image file. 

        This function places image parameters into the caller IMAGE_PARMS structure. The image
        parameters are width, height, user ID, and compression type. The image data is read from the
        open file denoted by handle.

	Image parameters are stored in an image buffer as shown in the following list. This function
	provides a convenient way to obtain some of the more useful image parameters.

	Image format: \include ImageFormat.txt

 	@param	handle - handle of the open image file
 	@param	pIparms - pointer to the structure to store the image parameters
 	@return	0: success, 1: missing image data or file error
 	@note Image file must be in Hypercom's format. File system must be initialized before this function will work properly. Works for printer or screen images. 
 	*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
int SDK_GetFileImageParms( FILE *handle, IMAGE_PARMS *pIparms);
#endif

/** 	@brief	Draws an image into the screen buffer from a file. 

        This function draws an image into the screen buffer at the pixel location given by the x and
        y parameters. Function SDK_GuiDispAll() or SDK_GuiDispPart() must be used to actually
        display the image.

	Image format: \include ImageFormat.txt

	@param	x - leftmost x pixel
	@param	y - uppermost y pixel
	@param	handle - handle of the file with the image data
	@return	0 - success, 1 - for invalid file or image too large for screen*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
int SDK_GuiFileImage( UWORD x, UWORD y, FILE *handle );
#endif

/**	@example SDK_GuiFileImage.c */

/** 	@brief	Draws centered text into the screen buffer on the given line.

        This function draws centered text on the specific line. The text is drawn into the screen
        buffer. Use SDK_GuiDispAll() to display the text.

        The text is displayed relative to the coordinates of the virtual window last set by
        SDK_GuiSetWindow(). Line and locations begin at 0. The number of lines available depends on
        the currently set font and the size of the virtual window.

        SDK_GuiGetVirWinTextParms() can determine the number of lines available in the current
        virtual window. Text is drawn in the currently set font. See SDK_GuiSetFont().

	@param	line - line to draw text
	@param	txt - pointer to the text to be drawn
*/
void SDK_GuiCenterText( UBYTE line, char *txt );
 
/** 	@brief	Clears a given number of characters in the screen buffer starting at the given line location.

        This function clears a line of text at the given character position and line number. The
        characters are cleared in the screen buffer. One of the SDK_GuiDisXXX() functions must be
        called to clear the characters on the screen.

        The cleared characters are relative to the coordinates of the virtual window last set by
        SDK_GuiSetWindow(). The size of the cleared characters depends on the currently set font.

 	@param line - zero-based line number
 	@param col - zero-based column number or character position
 	@param count - number of characters to clear*/
void SDK_GuiClearChars( UBYTE line, UBYTE col, UBYTE count );

/**     @brief  Sets first page of Unicode image pages. Unicode image pages must be stored on
		consecutive pages, starting at page set by this function.

        The Unicode pages can be loaded into any consecutive pages. The pages do not need to be
        re-built to be relocated. However, the application must specify where the Unicode pages are
        with the function SDK_GuiSetUnicodePage(), it must be done before any attempt is made to
        draw the UNICODE characters.
 	@param 	page - starting Unicode page*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
void SDK_GuiSetUnicodePage( PAGE_NUMBERS page );
#endif

/** 	@brief	This sets the vertical alignment of ASCII text drawn with Unicode characters.

        When the current font is Unicode, ASCII and extended characters may be drawn together with
        the Unicode. Since these fonts are usually of different sizes, this routine allows the user
        to align the ASCII or extended font to the top, center, or bottom of the Unicode.

        When the ASCII character is bigger than the Unicode characters, SDK_GuiUnicodeFontAdjust()
        has no effect.
 	@param 	adjust - One of 3 constants, UNI_TOP, UNI_CENTER, or UNI_BOTTOM 
 	@note	When SDK_SetFont() is called, the alignment is always initially set to UNI_BOTTOM. 
	*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
void SDK_GuiUnicodeFontAdjust(UNIFONT_ADJUST adjust);
#endif

/** 	@brief	Configures the custom unicode font.

        This function configures the Unicode header for a custom font, the desired configuration is
        defined by fontheader. The SDK_UNICODE_HEADER is defined in SDKGUI.h as follow:

        This function only affects the FONT_UCUSTOM font. Please refer to the Appendix B for the
        pre-built fonts available for each HSDK release.

	@param	fontheader	- pointer to structure defining the Custom Unicode font
        @note   This function must be called before calling SDK_GuiUnicodeCustomRanges(), as the
	        ranges table is cleared.
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
void SDK_GuiUnicodeCustomHeader( SDK_UNICODE_HEADER *fontheader );
#endif

/**	@example SDK_GuiUnicodeCustomHeader.c */

/** 	@brief	Sets a particular range for the Custom Unicode font 

        Sets a particular range for the Custom Unicode font. The SDK_UNICODE_RANGES enumeration is
        defined in SDKGUI.h.

        This function only affects the FONT_UCUSTOM font. Please refer to the Appendix B for the
        pre-built fonts available for each HSDK release.

	@param	index -	index for the range to configure,
	@param	Low - low boundary of the range,
	@param	High -	high boundary of the range.
	@return	True - okay to continue, False - reprompt for a different Amount 
	@note	This function must be used everytime the function 
		SDK_GuiUnicodeCustomHeader is used, as the table for the ranges is cleared. 
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
void SDK_GuiUnicodeCustomRanges( SDK_UNICODE_RANGES index, UBYTE Low, UBYTE High );
#endif

/** 	@brief	SDK_GuiUnicodeCustomBitmaps
	@note	This function must be used everytime the function SDK_GuiUnicodeCustomHeader is used, as the Unicode Bitmaps pointer is cleared. 
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
extern void SDK_GuiUnicodeCustomBitmaps( const UNICODE_RANGE *pUnicodeRangeBmps );
#endif

/**
	@brief Sets the foreground and background colors and options for
	       display on ICE 6000 terminals.

        This function sets the foreground and background colors.

	Presently, only one option flag is available. This flag causes the
	background to remain as is and only the foreground color is used by
        subsequent calls to GUI functions.

        @param pDispColor Pointer to the DISPLAY_COLOR structure containing
	                  foreground color, background color, and options
	                  information.
*/ 
void SDK_GuiSetColor( DISPLAY_COLOR *pDispColor );

/**	@example SDK_GuiSetColor.c */

/**
	@brief Gets foreground and background colors and options for display
	       on ICE 6000 terminals.

        This function gets the foreground and background colors.

        @param pDisplayColor Pointer to the DISPLAY_COLOR structure containing
	                     foreground color, background color, and options
	                     information.
*/ 
void SDK_GuiGetColor( DISPLAY_COLOR *pDisplayColor );

/**	@example SDK_GuiGetColor.c */

#if !defined(MAKE_MONOCHROME)
/** 	@brief	Sets the current update video page for ICE 6000 terminals.

        This function sets the current update video page. This is the page where the screen buffer
        is stored on a call to one of the SDK GUI display functions such as SDK_GuiDispAll() or
        SDK_GuiDispPart(). Also, the update video page is where data is written with the functions
        SDK_GuiColorImage() and SDK_GuiColorImagePage().

        The update video page may or may not be the same as the visible video page. Screen
        information in the update page appears on the screen only when it is set as the visible
        video page. See SDK_GuiSetVisiblePage.

        The ICE 6000 has 16 video pages that can be used to retain screen information. Each page
        corresponds to one screen. VIDEOPAGE is an enumeration defined in sdkgui.h. The video pages
        are defined as video_PAGE0 through video_PAGE15.

        The page number passed to this function should always be one defined in the VIDEOPAGE
        enumeration. In this case, the return value is always orvOK and can be ignored.

 	@param 	page - number of the video page to be used as the update video page
 	@return	OS_RETURN_VALS - orvINVDATA or orvOK.*/

OS_RETURN_VALS SDK_GuiSetUpdatePage( VIDEOPAGE page );

/**	@example SDK_GuiSetUpdatePage.c */

/** 	@brief	Gets the current update video page for ICE 6000 terminals.

        This function gets the current update video page. The screen buffer is stored on this page
        on a call to one of the SDK GUI display functions such as SDK_GuiDispAll() or
        SDK_GuiDispPart().

        The update video page may or may not be the same as the visible video page. Screen
        information in the update page appears on the screen only when it is set as the visible
        video page. See SDK_GuiSetVisiblePage( ).

        The ICE 6000 has 16 video pages that can be used to retain screen information. Each page
        corresponds to one screen. VIDEOPAGE is an enumeration defined in sdkgui.h. The video pages
        are defined as video_PAGE0 through video_PAGE15.

	@return page number of the current update or working video page
*/
VIDEOPAGE SDK_GuiGetUpdatePage(void);

/** 	@brief	Sets the current visible video page for ICE 6000 terminals.

        This function sets the video page currently visible. This is the page that currently appears
        on the screen. The ICE 6000 has 16 video pages that can be used to retain screen
        information. Each page corresponds to one screen and only the current visible page set by
        this function is shown.

        VIDEOPAGE is an enumeration defined in sdkgui.h. The video pages are defined as video_PAGE0
        through video_PAGE15. The page number passed to this function should always be one defined
        in the VIDEOPAGE enumeration. In this case, the return value is always orvOK and therefore
        can be ignored.

 	@param 	page - number of the video page that actually appears on the screen
 	@return	OS_RETURN_VALS - orvINVDATA or orvOK.*/
OS_RETURN_VALS SDK_GuiSetVisiblePage( VIDEOPAGE page );

/** 	@brief	Gets the current visible video page for ICE 6000 terminals

        This function gets the current visible video page. This is the page that currently appears
        on the screen. The ICE 6000 has 16 video pages that can be used to retain screen
        information. Each page corresponds to one screen, and only the current visible page
        retrieved by this function is actually shown.

        VIDEOPAGE is an enumeration defined in sdkgui.h. The video pages are defined as video_PAGE0
        through video_PAGE15.

	@return page number of the current visible video page
*/
VIDEOPAGE SDK_GuiGetVisiblePage(void);

/**	@example SDK_GuiGetVisiblePage.c */

#endif

#if defined(MAKE_ICE6500) || defined(MAKE_ICE6000) || defined(MAKE_VIKING) || defined(MAKE_OPTIMUM)

/** 	@brief	Draws a color image into current update video page.  

        This function draws an image into the current update video page at the pixel location given
        by the x and y parameters. See SDK_GuiSetUpdatePage.

        The image appears only when the video page on which it is drawn is set as the current
        visible page. See SDK_GuiSetVisiblePage.

	The image data pointed to by pImage must be in PCX format.

 	@param x - leftmost x pixel
 	@param y - uppermost y pixel
 	@param img - pointer to image data
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
void SDK_GuiColorImage( UWORD x, UWORD y, UBYTE *img );
#endif

/**	@example SDK_GuiColorImage.c */

/** 	@brief	Draws a color image into the current update video page.

        This function draws an image into current update video page at the pixel location given by
        the x and y parameters. See SDK_GuiSetUpdatePage( ). The image appears only
        when the video page on which it is drawn is set as the current visible page. See
        SDK_GuiSetVisiblePage. The image data pointed to by pImage must be in PCX
        format. The pImage parameter is the address of the image data on the page given by page.

 	@param x - leftmost x pixel
 	@param y - uppermost y pixel
 	@param *img - pointer to image data
	@param page - page number where the image data resides
*/
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
void SDK_GuiColorImagePage( UWORD x, UWORD y, const UBYTE *img, PAGE_NUMBERS page );
#endif

/**	@example SDK_GuiColorImagePage.c */

#endif

#if !defined(MAKE_MONOCHROME)

/** 	@brief	Changes the current color palette to the one specified. For ICE 6000 and  ICE 6500 terminals. 

        This function sets the color palette (look-up table)  as used by the  operating system to
        display colors. The palette  is formatted as 256*3 bytes. The first byte in a row
        corresponds to red content, the second corresponds to green content, and the third blue
        content. However, only the 4 most-significant bits of each byte are utilized.

	The color bytes (0-255 value) as specified by SDK_GuiSetColor() are then used as offsets into the table. 

	@param	pPalette - pointer to 768 byte palette
	@param	Page   - page number where palette is located
	@note The operating system contains a default palette which is sufficient for most
        applications and generally this function does need to be called.
*/
void SDK_GuiSetPalette( UBYTE *pPalette, PAGE_NUMBERS Page );


/** 	@brief	Sets current video page to current background color. Applies only to terminals with color screens.

        The sets the current video page to the current background color. This function is a faster
        way to clear the screen for ICE 6000 and ICE 6500 terminals than the similar
        SDK_GuiClearScr() function.
*/
extern  void SDK_GuiClrDisplay( void );

#endif

/** 	@brief	Configures the CGRAM Application Remap Data
		@param	pRemapData - Pointer to structure defining RemapBitmaps
		@note	This function must be used when FONT_CGRAM font
 			is used. All addresses inside pRemapData are 
 			assumed in lower memory.
*/

#if defined(T7FAMILY_OS) || defined(MAKE_P2100) || defined(MAKE_H2200)
extern void SDK_GuiSetCGRAMRemapData( CGRAM_REMAP_DATA *pRemapData );
#endif

/** 	@brief	Function to copy the extended font bitmaps from an application page to the system FON page.
 	@param FontId          Font ID to associate with the font.
 	@param Page            Logical page number.
 	@param pAddress        Font bitmaps address at application page.
 	@param DataSize        Total data to copy.
	@return	0 - Font loaded
		1 - Error, DataSize too big
		2 - Invalid Font ID
	@note  This function must be used before calling SDK_GuiSetFont( xxx, FONT_ECUSTOM ) or SDK_GuiSetFont( xxx, FONT_EXxxxxxx ).
 */
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
UBYTE SDK_GuiLoadExtCustomFont( UBYTE FontId, UBYTE Page, const void *pAddress, UWORD DataSize);
#endif

/** 	@brief	Function to copy the extended font bitmaps from an application page to the system FON page.
 	@param Page            Logical page number.
 	@param pAddress        Font bitmaps address at application page.
 	@param DataSize        Total data to copy.
	@return	0 - Font loaded,
		1 - Error, DataSize too big.
	@note  This function must be used before calling SDK_GuiSetFont( xxx, FONT_ECUSTOM ).
 */
#if !defined(MAKE_P2100) && !defined(MAKE_H2200)
UBYTE SDK_GuiLoadECustomFont( UBYTE Page, const void *pAddress, UWORD DataSize);
#endif

#ifdef __cplusplus
}
#endif

/**
	@}
*/

#endif /* SDKGUI */
