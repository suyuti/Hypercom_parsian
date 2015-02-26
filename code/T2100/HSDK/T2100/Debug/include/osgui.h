/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef _OSGUI_H_
#define _OSGUI_H_

//=============================================================================
//
// Module overview:
//
//      This header file contains declarations of public functions and
//      data related to the Hypercom Graphical User Interface (GUI)
//      library provided by the operating system.
//
//=============================================================================

#include "execdef.inc"
#if INCLPAD
#include "paddev.inc"
#endif

//=============================================================================
// Public defines and typedefs
//=============================================================================
#if defined( MAKE_VIKING )

#include "hwdisplay.h"
#define GUI_SCREEN_WIDTH        VIKING_SCREEN_WIDTH
#define GUI_SCREEN_HEIGHT       VIKING_SCREEN_HEIGHT

#define MAKE_MONOCHROME

#elif defined( MAKE_OPTIMUM ) && !defined( MAKE_M2 )

#define GUI_SCREEN_WIDTH        ((unsigned short)  160)
#define GUI_SCREEN_HEIGHT       ((unsigned short)  80 )

#define MAKE_MONOCHROME

#elif defined( MAKE_M2 )

#define GUI_SCREEN_WIDTH        ((unsigned short)  240)
#define GUI_SCREEN_HEIGHT       ((unsigned short)  320)

#elif defined( MAKE_ICE6500 )

#define GUI_SCREEN_WIDTH        ((unsigned short)  320)
#define GUI_SCREEN_HEIGHT       ((unsigned short)  240)
#define VIRTUAL_SCR_MAX_HEIGHT  ((unsigned short) 5825)

#elif defined( MAKE_ICE4000 )

#define GUI_SCREEN_WIDTH        ((unsigned short)  160)
#define GUI_SCREEN_HEIGHT       ((unsigned short)  160)
#define VIRTUAL_SCR_MAX_HEIGHT  ((unsigned short) 1788)

#define MAKE_MONOCHROME

#elif defined( MAKE_ICE5000 )

#define GUI_SCREEN_WIDTH        ((unsigned short)  160)
#define GUI_SCREEN_HEIGHT       ((unsigned short)   80)
#define VIRTUAL_SCR_MAX_HEIGHT  ((unsigned short) 1708)

#define MAKE_MONOCHROME

#else

#error Terminal type is undefined!

#endif

#if ( !defined( __GNUC__ ) && !defined( __attribute__ ) ) || defined( DOXYGEN )
#define __attribute__(A)
#endif

//----------------------------------------------------------------------------
// Standard fonts (characters 0x00 through 0x7F)
//----------------------------------------------------------------------------
typedef unsigned char StdFontID;
enum
{
    FONT_SA05NN07,          ///< Normal 5x7 Font
    FONT_SA06BN08,          ///< Bold 6x8 Font
    FONT_SA06BN12,          ///< Bold 6x12 Font
    FONT_SA08BN09,          ///< Bold 8x9 Font
#ifndef MAKE_VIKING
//#if defined( MAKE_M2 )
    FONT_8X13BOLD,          ///< Bold 8x13 Font
    FONT_VT9X13,            ///< 9x13 Font
//#endif //MAKE_M2
#endif
    FONT_SA12BN18,          ///< Bold 12x18 Font

    NUM_STD_FONTS,          ///< Number of standard fonts provided by the system.

    // Applications can define custom fonts using IDs ranging from
    // FIRST_CUSTOM_FONT through MAX_CUSTOM_FONT (inclusive).  The
    // custom fonts can be either standard ASCII fonts or Unicode
    // fonts.  See GUI_SetCustomFont() or GUI_SetUnicodeFont() for
    // more information.
    FIRST_CUSTOM_FONT = 32,
    FONT_UCUSTOM,
    MAX_CUSTOM_FONT = 127,

    // FONT_SOFTFONT is the value returned by GUI_GetFontID() when
    // a soft font is active.
    FONT_SOFTFONT = 128
};


//----------------------------------------------------------------------------
// Extended fonts (characters 0x80 through 0xFF)
//----------------------------------------------------------------------------
typedef unsigned char ExtFontID;
enum
{
    FONT_ENULL,                     ///< Null Font, to be used when the
                                    ///< application does not support the
                                    ///< extended ASCII characters.

    FONT_EA05NN07,                  ///< Normal 5x7 Font
    FONT_EA06BN08,                  ///< Bold 6x8 Font
    FONT_EA06BN12,                  ///< Bold 6x12 Font
    FONT_EA08BN09,                  ///< Bold 8x9 Font
#ifndef MAKE_VIKING
    FONT_EA08BN13,                  ///< Bold 8x13 Font (unfinished)
#endif
    FONT_EA12BN18,                  ///< Bold 12x18 Font

    // Custom fonts
    FONT_EX05NN07,                  ///< Normal 5x7 Font
    FONT_EX06BN08,                  ///< Bold 6x8 Font
    FONT_EX06BN12,                  ///< Bold 6x12 Font
    FONT_EX08BN09,                  ///< Bold 8x9 Font
    FONT_EX12BN18,                  ///< Bold 12x18 Font

    FONT_ECUSTOM,                   ///< Unspecified size

    NUM_EXT_FONTS                   ///< Number of extended fonts
};

// Maximum size of a character bitmap in any font.
#define MAX_CHAR_BITMAP_WIDTH       16
#define MAX_CHAR_BITMAP_HEIGHT      20
#define MAX_CHAR_BITMAP_SIZE        (( MAX_CHAR_BITMAP_WIDTH / 8 ) * MAX_CHAR_BITMAP_HEIGHT )

// Maximum number of characters in a custom font.
#define MAX_NUM_CUST_FONT_CHARS     128

// FONT_FORMAT identifies the format of a custom font.
typedef enum
{
    FONT_FMT_ASCII      = 1,
    FONT_FMT_UNICODE    = 2
} FONT_FORMAT;

/**
	@brief Font section header.
*/
typedef struct FONT_SECTION_HEADER
{
    unsigned char   firstChar;      ///< ASCII code of the first char in this section.
    unsigned char   lastChar;       ///< ASCII code of the last char in this section.
    const struct FONT_SECTION_HEADER *pNext;  ///< Pointer to the next section.
    const unsigned char   *pFirstChar;   ///< Pointer to first character in char data array
} FONT_SECTION_HEADER;

/**
	@brief Font header.

	@note FONT_HEADER and FONT_SECTION_HEADER structures are used to define
	fonts for the standard and extended character sets (characters 0x00
	through 0x7F and 0x80 through 0xFF).
*/
typedef struct
{
    unsigned char   format;         ///< Format version (FONT_FMT_ASCII)
    unsigned char   bitmapSize;     ///< Number of bytes for each char definition.
    unsigned char   width;          ///< Width of each character (in pixels).
    unsigned char   height;         ///< Height of each character (in pixels).
    unsigned char   charSpace;      ///< Space between each character (in pixels).
    unsigned char   rowSpace;       ///< Space between each row of characters (in pixels).
    const FONT_SECTION_HEADER * pSection;   ///< Pointer to the first section.
} FONT_HEADER;



/**
	@brief Unicode font header.
*/
typedef struct
{
    unsigned char   format;         ///< Format version (FONT_FMT_UNICODE)
    unsigned char   bitmapSize;     ///< Number of bytes for each char definition.
    unsigned char   width;          ///< Width of each character (in pixels).
    unsigned char   height;         ///< Height of each character (in pixels).
    unsigned char   charSpace;      ///< Space between each character (in pixels).
    unsigned char   rowSpace;       ///< Space between each row of characters (in pixels).
    StdFontID       stdFontID;      ///< Font to use for characters 0x00 through 0x7F.
    unsigned char   vertAdjust;     ///< Vertical adjustment of standard font within Unicode cell height.
    const unsigned char * pRangeTable;  ///< Pointer to the list of character ranges for this font.
} UNICODE_HEADER;

// MAX_NUM_UNICODE_RANGES is the maximum number of entries in a Unicode
// range table, not including the two null bytes that terminate the list.
#define MAX_NUM_UNICODE_RANGES      20

/**
	@brief Unicode font adjust values.
*/
typedef enum
{
	UNI_TOP = 1,
	UNI_CENTER = 2,
	UNI_BOTTOM = 3
} UNIFONT_ADJUST; 

//----------------------------------------------------------------------------
// Colors are specified using an 8-bit palette index.  This provides
// for a maximum of 256 colors.
//----------------------------------------------------------------------------
#if defined(MAKE_M2)
typedef UINT16 COLOR;
#else
typedef unsigned char COLOR;
#endif //MAKE_M2

/**
	@brief Display color.
*/
typedef struct 
{
    COLOR       Foreground;         ///< Foreground Color
    COLOR       Background;         ///< Background Color
    unsigned    Transparent : 1;    ///< Set if transparent background
} DISPLAY_COLOR;

#define RGB( r, g, b )  (  ((COLOR)(((unsigned char)r) >> 3 ) << 11 ) | ((COLOR)(((unsigned char)g) >> 2 ) << 5 ) | ((COLOR)((unsigned char)b) >> 3 )  )

#ifdef GUI_256_COLOR_SCREEN
//----------------------------------------------------------------------------
// Definitions of some commonly-used colors in the standard Hypercom palette.
//----------------------------------------------------------------------------
//      Name                   Index       R  G  B
#define COLOR_WHITE    ((COLOR) 249)    // FF FF FF
#define COLOR_YELLOW   ((COLOR) 209)    // FF FF 00
#define COLOR_FUCHSIA  ((COLOR) 240)    // FF 00 FF
#define COLOR_RED      ((COLOR) 200)    // FF 00 00
#define COLOR_SILVER   ((COLOR) 186)    // C0 C0 C0
#define COLOR_MAGENTA  ((COLOR) 180)    // C0 00 C0
#define COLOR_GRAY     ((COLOR) 124)    // 80 80 80
#define COLOR_OLIVE    ((COLOR) 104)    // 80 80 00
#define COLOR_PURPLE   ((COLOR) 120)    // 80 00 80
#define COLOR_MAROON   ((COLOR) 100)    // 80 00 00
#define COLOR_AQUA     ((COLOR)  49)    // 00 FF FF
#define COLOR_LIME     ((COLOR)   9)    // 00 FF 00
#define COLOR_CYAN     ((COLOR)  36)    // 00 C0 C0
#define COLOR_TEAL     ((COLOR)  24)    // 00 80 80
#define COLOR_GREEN    ((COLOR)   4)    // 00 80 00
#define COLOR_BLUE     ((COLOR)  40)    // 00 00 FF
#define COLOR_NAVY     ((COLOR)  20)    // 00 00 80
#define COLOR_BLACK    ((COLOR)   0)    // 00 00 00
#define COLOR_TRANSP   ((COLOR) 255)    // Transparent

#else // not GUI_256_COLOR_SCREEN

#define COLOR_WHITE    ((COLOR)   0)    // Turn pixel off
#define COLOR_BLACK    ((COLOR)   1)    // Turn pixel on

#endif


/**
	@brief Pixel location on the screen.

	The upper left corner of the screen is x=0, y=0.
*/
typedef struct
{
    unsigned short x;
    unsigned short y;
} POINT;


/**
	Rectangular area of the screen.
*/
typedef struct
{
    unsigned short x;       ///< X-coordinate of the upper-left corner.
    unsigned short y;       ///< Y-coordinate of the upper-left corner.
    unsigned short width;   ///< Width of the area.
    unsigned short height;  ///< Height of the area.
} RECT;


/**
	Get/put GUI block definition.
*/
typedef struct
{
    RECT rect;
    void * pBuf;
} GUI_GETPUT_BLK;


/**
	Move GUI block definition.
*/
typedef struct
{
    RECT rect;
    unsigned short destX;
    unsigned short destY;
} GUI_MOVE_BLK;

#if defined( _MSC_VER )
#pragma pack(1)
#endif

/**
	@brief Image header data for a Hypercom compressed graphics image.

	@note The 'C' programming language does not allow structures to contain
	variable-length elements, so the data[] element below is really just a
	place holder which identifies the starting location of the data.
	Because of this restriction, an image cannot be defined directly in
	'C'.
*/
typedef struct
{
    unsigned short size;        ///< Overall image size minus 2
    unsigned short id;          ///< Image ID
    unsigned short width;       ///< Image width (in pixels)
    unsigned short height;      ///< Image height (in pixels)
    unsigned char compressType; ///< Compression type
    unsigned short dataLen;     ///< Length of data (in bytes)
    unsigned char data[1];      ///< Image data (actual length varies)
} __attribute__((packed)) IMAGE;

/**
	Image header data for a Hypercom compressed graphics image.
*/
typedef struct
{
    unsigned short size;        ///< Overall image size minus 2
    unsigned short id;          ///< Image ID
    unsigned short width;       ///< Image width (in pixels)
    unsigned short height;      ///< Image height (in pixels)
    unsigned char compressType; ///< Compression type
    unsigned short dataLen;     ///< Length of data (in bytes)
} __attribute__((packed)) IMAGE_HEADER;

/**
	The 128-byte header that exists at the beginning of a PCX file.
*/
typedef struct
{
    unsigned char Manufacturer;     ///< Should always be 10 (0x0A)
    unsigned char Version;          ///< Version information:
                                    ///<   0 = Version 2.5
                                    ///<   2 = Version 2.8 w/palette information
                                    ///<   3 = Version 2.8 w/o palette information
                                    ///<   5 = Version 3.0
    unsigned char Encoding;         ///< Encoding:
                                    ///<   1 = .PCX run length encoding
    unsigned char BitsPerPixel;     ///< Number of bits/pixel per plane
    unsigned short Xmin;            // Image dimensions in pixels, inclusive
    unsigned short Ymin;            //
    unsigned short Xmax;            //
    unsigned short Ymax;            //
    unsigned short HRes;            ///< Horizontal Resolution of creating device
    unsigned short VRes;            ///< Vertical Resolution of creating device
    unsigned char ColorMap[48];     ///< Color palette setting
    unsigned char _reserved;        ///< Reserved
    unsigned char NPlanes;          ///< Number of color planes
    unsigned short BytesPerPlane;   ///< Number of bytes per scan line per color
                                    ///<   plane (always even for .PCX files)
    unsigned short PaletteInfo;     ///< How to interpret palette:
                                    ///<   1 = color/BW
                                    ///<   2 = grayscale
    unsigned char _filler[58];      ///< Filler to pad header to 128 bytes
} __attribute__((packed)) PCX_HEADER;

#if defined( _MSC_VER )
#pragma pack()
#endif

/**
	@brief PCX graphics image.

	Because that the 'C' programming language does not allow structures to
	contain variable-length elements, the PCX structure is undefined.
	Because of this restriction, a PCX image cannot be defined directly in
	'C'. The recommended method of defining a PCX image is with an array of
	unsigned characters:

	const unsigned char MyPCX[] = { ... PCX data here ... };

	The image can then be displayed with the following function call:

	GUI_DisplayPCX( x, y, page, MyPCX );
*/
typedef void PCX_IMAGE;


//----------------------------------------------------------------------------
// Button declarations
//----------------------------------------------------------------------------

// Button states
typedef char BUTTON_STATE;
#define BUTTON_DOWN     0   // Down state (pressed)
#define BUTTON_UP       1   // Up state (released)

// Button types
typedef char BUTTON_TYPE;
#define BTN_TYPE_TRANSPARENT    0
#define BTN_TYPE_INVERT         1
#define BTN_TYPE_3D             2
#define BTN_TYPE_HYPERLINK      3
#define NUM_BUTTON_TYPES        4

/**
	Button definition.
*/
typedef struct
{
    BUTTON_TYPE type;       ///< Type of button.
    char        id;         ///< ID to be returned when button is pressed.
    char        border;     ///< Border width (0=no border)
    RECT        rect;       ///< Button's defining rectangle (coordinates are
                            ///< relative to the current virtual window).
#if !defined( MAKE_MONOCHROME ) && !defined(MAKE_M2)
    COLOR       color1;     ///< Button color 1    See GUI_Button() for color
    COLOR       color2;     ///< Button color 2    usage information.
    COLOR       color3;     ///< Button color 3
    COLOR       color4;     ///< Button color 4
#endif // MAKE_MONOCHROME
} BUTTON_DEF;


//=============================================================================
// Public data declarations
//=============================================================================
#ifndef MAKE_MONOCHROME
extern const unsigned char GUI_StdPalette[768];
#endif

#if defined(MAKE_VIKING) || defined(MAKE_OPTIMUM) || defined(MAKE_M2)
// Standard extended fonts
extern const FONT_HEADER FONT_EA05NN07_DEF;
extern const FONT_HEADER FONT_EA06BN08_DEF;
extern const FONT_HEADER FONT_EA06BN12_DEF;
extern const FONT_HEADER FONT_EA08BN07_DEF;
extern const FONT_HEADER FONT_EA08BN09_DEF;
extern const FONT_HEADER FONT_EA08BN13_DEF;
extern const FONT_HEADER FONT_EA12BN18_DEF;
// Croatian fonts
extern const FONT_HEADER FONT_EC05NN07_DEF;
extern const FONT_HEADER FONT_EC06BN08_DEF;
extern const FONT_HEADER FONT_EC06BN12_DEF;
extern const FONT_HEADER FONT_EC08BN09_DEF;
// Greek fonts
extern const FONT_HEADER FONT_EG05NN07_DEF;
extern const FONT_HEADER FONT_EG06BN08_DEF;
extern const FONT_HEADER FONT_EG06BN12_DEF;
extern const FONT_HEADER FONT_EG08BN09_DEF;
// Polish fonts
extern const FONT_HEADER FONT_EP05NN07_DEF;
extern const FONT_HEADER FONT_EP06BN08_DEF;
extern const FONT_HEADER FONT_EP06BN12_DEF;
extern const FONT_HEADER FONT_EP08BN09_DEF;
// Russian fonts
extern const FONT_HEADER FONT_ER05NN07_DEF;
extern const FONT_HEADER FONT_ER06BN08_DEF;
extern const FONT_HEADER FONT_ER06BN12_DEF;
extern const FONT_HEADER FONT_ER08BN09_DEF;
// Turkey fonts
extern const FONT_HEADER FONT_ET05NN07_DEF;
extern const FONT_HEADER FONT_ET06BN08_DEF;
extern const FONT_HEADER FONT_ET06BN12_DEF;
extern const FONT_HEADER FONT_ET08BN09_DEF;
// Western ISO-8859-1 font
extern const FONT_HEADER FONT_EW05NN07_DEF;
// Litvianian fonts
extern const FONT_HEADER FONT_LT05NN07_DEF;
extern const FONT_HEADER FONT_LT06BN08_DEF;
extern const FONT_HEADER FONT_LT06BN12_DEF;
extern const FONT_HEADER FONT_LT08BN09_DEF;
// Latvian fonts
extern const FONT_HEADER FONT_LV05NN07_DEF;
extern const FONT_HEADER FONT_LV06BN08_DEF;
extern const FONT_HEADER FONT_LV06BN12_DEF;
extern const FONT_HEADER FONT_LV08BN09_DEF;
// Standard fonts
extern const FONT_HEADER FONT_SA05NN07_DEF;
extern const FONT_HEADER FONT_SA06BN08_DEF;
extern const FONT_HEADER FONT_SA06BN12_DEF;
extern const FONT_HEADER FONT_SA08BN07_DEF;
extern const FONT_HEADER FONT_SA08BN09_DEF;
extern const FONT_HEADER FONT_SA12BN18_DEF;
extern const FONT_HEADER FONT_8X13BOLD_DEF;
extern const FONT_HEADER FONT_VT9X13_DEF;
#endif

//=============================================================================
// Public function declarations
//=============================================================================
/**
	@defgroup GroupOSGUI OSGUI public interface
	@{
*/

//-----------------------------------------------------------------------------
//  GUI_Button
///		@brief
///      This function draws a button of the given type and adds it to
///      the button table, making the given screen area "active".  When
///      that area of the screen is pressed and released, the specified
///      button ID will be returned by a subsequent call to OS_GetUserInput()
///      or OS_CheckUserInput().  The button will not appear on the screen
///      until a subsequent call to GUI_DispPart() (or similar function)
///      is made.
///
///      The button content (e.g. text label) is not drawn by this
///      function.
///
///      There are several types of buttons supported as described below:
///
///          BTN_TYPE_TRANSPARENT is analogous to the BUTTON_SQ command in
///          SCL.  It creates an active button area that responds to the
///          user, but there is no visual representation.
///
///
///          BTN_TYPE_INVERT is analogous to the BUTTON_SQINV command in
///          SCL.  It creates a rectangular button with the given border
///          width.  When the button is pressed, the entire area is
///          inverted, including anything that was drawn inside the
///          button.
///
///          In the up state, the button is drawn using color1 as the
///          foreground and color2 as the background.  In the down state,
///          the colors are reversed.  color3 and color4 are not used.
///
///
///
///          BTN_TYPE_3D is analogous to the BUTTON_SQ3D command in SCL.
///          It creates a rectangular button with the given border
///          width and shading on the right side and bottom of the
///          button.  When the button is pressed, the button is drawn
///          in the down state with the shading on the left side and
///          top of the button.  As the button is pressed and released,
///          its content is shifted down and to the right or up and
///          to the left respectively.
///
///          The button is drawn using the following color scheme:
///
///              color1:  Face foreground (e.g. text label)\n
///              color2:  Face background\n
///              color3:  Border\n
///              color4:  Shading\n
///    
///          The given button width and height includes the shading,
///          which is always one pixel wide and one pixel high.
///
///
///
///          BTN_TYPE_HYPERLINK implements a button in which the foreground
///          color changes as the button is pressed and released but the
///          background color remains unchanged.
///
///          In the up state, the foreground is drawn using color1.  In
///          the down state, the foreground is drawn using color3.  The
///          background is always drawn with color2.
///
///
///  @param
///      page        Memory page of the structure pointed to by pBtn.
///  @param
///      pBtn        Pointer to button definition structure.
///
///  @return
///      Nothing
///
///  @note
///      The button coordinates are relative to the current virtual
///      window.
//-----------------------------------------------------------------------------
extern void GUI_Button( unsigned char page, const BUTTON_DEF * pBtn );

//-----------------------------------------------------------------------------
//  GUI_ButtonAdd
///		@brief
///      This function adds the specified button to the button table,
///      making the given screen area "active".  Nothing is drawn on
///      the screen by this function.  However, the button will redraw
///      itself in response to touch-pad events.
///
///  @param
///      page        Memory page of the structure pointed to by pBtn.
///  @param
///      pBtn        Pointer to button definition structure.
///
///  @return
///      Nothing.
///
///  @note
///      The button coordinates are relative to the current virtual
///      window.
//-----------------------------------------------------------------------------
extern void GUI_ButtonAdd( unsigned char page, const BUTTON_DEF * pBtn );

//-----------------------------------------------------------------------------
//  GUI_ButtonClearAll
///		@brief
///      This function clears all buttons from the button table, making
///      the entire touch-pad "inactive".
///
///  @return
///      Nothing
//-----------------------------------------------------------------------------
extern void GUI_ButtonClearAll( void );

//-----------------------------------------------------------------------------
//  GUI_ButtonInvert
///		@brief
///      Invert button re-draw routine. Given the coordinates of the button,
///      the button is inverted. 
///
///  @param
///      page        Memory page of the structure pointed to by pRect.
///  @param
///      pRect       Pointer to the button's coordinates.
///
///  @return
///      Nothing.
///
///  @note
///      The coordinates are absolute screen coordinates.
//-----------------------------------------------------------------------------
extern void GUI_ButtonInvert( unsigned char page, const RECT *pRect );

//-----------------------------------------------------------------------------
//  GUI_ButtonSquare3D
///		@brief
///      Square 3D button re-draw routine. Given the coordinates of the button,
///      and the button's state, the button is redrawn.
///
///      Note: The area given is either shifted down and to the right, or
///      shifted up and to the left when the state is BUTTON_UP or BUTTON_DOWN
///      respectively.  The shading for the 3D button is then drawn.  Therefore,
///      it is assumed that GUI_ButtonSquare3D is never called twice in a row
///      for the same button in the same state. 
///
///  @param
///      page        Memory page of the structure pointed to by pRect.
///  @param
///      pRect       Pointer to the button's coordinates.
///  @param
///      state       Desired state of the button (i.e. up or down).
///
///  @return
///      Nothing.
///
///  @note
///      The coordinates are absolute screen coordinates.
//-----------------------------------------------------------------------------
extern void GUI_ButtonSquare3D( unsigned char page, const RECT *pRect, BUTTON_STATE state );

//-----------------------------------------------------------------------------
//  GUI_Char2Pixel
///		@brief
///      This function converts a character position (column, line) to a
///      pixel coordinate (x, y).
///
///  @param
///      col         Character column position.
///  @param
///      line        Character line position.
///  @param
///      page        Memory page of the structure pointed to by pPoint.
///  @param
///      pPoint      Pointer to the structure in which to store the x,y position.
///
///  @return
///      Nothing.
///
///  @note
///      - The character position is not checked for validity.\n
///      - The pixel coordinates returned in *pPoint are relative to the
///        current virtual window.
//-----------------------------------------------------------------------------
extern void GUI_Char2Pixel( unsigned char col, unsigned char line,
                            unsigned char page, POINT * pPoint );

//-----------------------------------------------------------------------------
//  GUI_ClearChars
///		@brief
///      This function clears a character string from the screen.  It does NOT
///      wrap to the next line if the number of characters specified extends
///      beyond the right edge of the virtual window.
///
///  @param
///      col         Column position to start clearing from
///  @param
///      line        Line on which to clear characters
///  @param
///      n           Number of characters to clear
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_ClearChars( unsigned char col, unsigned char line, unsigned char n );

//-----------------------------------------------------------------------------
//  GUI_ClearScr
///		@brief
///      This function clears all of the bits in the video bitmap memory
///      to zero.  The visible screen is not cleared until a subsequent
///      call is made to one of the screen update functions, such as
///      GUI_DispAll().
///
///  @return
///      Nothing
//-----------------------------------------------------------------------------
extern void GUI_ClearScr( void );

//-----------------------------------------------------------------------------
//  GUI_ClrArea
///		@brief
///      This function clears the bits in the specified portion of video
///      bitmap memory to zero.
///
///      The visible screen is not affected until a subsequent call
///      is made to one of the screen update functions, such as
///      GUI_DispPart().
///
///  @param
///      page        Memory page of the structure pointed to by pRect.
///  @param
///      pRect       Pointer to the area's coordinates.
///
///  @return
///      Nothing
///
///  @note
///      The specified coordinates must be absolute screen coordinates.
///      Virtual window coordinates are ignored by this function.
//-----------------------------------------------------------------------------
extern void GUI_ClrArea( unsigned char page, const RECT * pRect );

//-----------------------------------------------------------------------------
//  GUI_ClrPixel
///		@brief
///      This function erases the pixel at the given coordinates by
///      clearing the corresponding bit in video bitmap memory.  The
///      visible screen is not affected until a subsequent call is made
///      to one of the screen update functions, such as GUI_DispPart().
///
///  @param
///      x           X-coordinate of the pixel.
///  @param
///      y           Y-coordinate of the pixel.
///  @note the coordinates are relative to the current
///                  virtual window.
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_ClrPixel( unsigned short x, unsigned short y );

//-----------------------------------------------------------------------------
//  GUI_ClrVirWin
///		@brief
///      This function clears all of the bits in the virtual window area
///      of video bitmap memory.  The visible screen is not affected
///      until a subsequent call is made to one of the screen update
///      functions, such as GUI_DispVirWin().
///
///  @return
///      Nothing
//-----------------------------------------------------------------------------
extern void GUI_ClrVirWin( void );

//-----------------------------------------------------------------------------
//  GUI_ColorBrightIndex
///		@brief
///      This function calculates the relative brightness of the color in
///      the standard Hypercom color palette referenced by the given index.
///
///      If the given index is COLOR_TRANSP, the brightness is not
///      computed and COLOR_TRANSP is returned.
///
///  @param
///      index       Index into the standard Hypercom palette.
///
///  @return
///      The value returned ranges from 72 through 184 (inclusive) with
///      72 representing the darkest color (black) and 184 representing
///      the brightest color (white).
//-----------------------------------------------------------------------------
#ifndef MAKE_MONOCHROME
extern unsigned char GUI_ColorBrightIndex( unsigned char index );
#endif

//-----------------------------------------------------------------------------
//  GUI_ColorBrightRGB
///		@brief
///      This function calculates the relative brightness of the
///      specified color in the standard Hypercom color palette.
///
///  @param
///      r           Red component of the color.
///  @param
///      g           Green component of the color.
///  @param
///      b           Blue component of the color.
///
///  @return
///      The value returned ranges from 72 through 184 (inclusive) with
///      72 representing the darkest color (black) and 184 representing
///      the brightest color (white).
//-----------------------------------------------------------------------------
#ifndef MAKE_MONOCHROME
extern unsigned char GUI_ColorBrightRGB( unsigned char r, unsigned char g, unsigned char b );
#endif

//-----------------------------------------------------------------------------
//  GUI_ColorIndex
///		@brief
///      This function converts the given RGB color to the corresponding
///      index in the standard Hypercom color palette.
///
///      If the given RGB values do not exactly match a color definition in
///      the palette, the index of the closest matching color is returned.
///
///  @param
///      r           Red component of the color.
///  @param
///      g           Green component of the color.
///  @param
///      b           Blue component of the color.
///
///  @return
///      Index of the correpsonding color in the Hypercom standard palette.
//-----------------------------------------------------------------------------
#ifndef MAKE_MONOCHROME
extern unsigned char GUI_ColorIndex( unsigned char r, unsigned char g, unsigned char b );
#endif

//-----------------------------------------------------------------------------
//  GUI_DefWindow
///		@brief
///      This function sets the coordinates of the virtual window to
///      the visible screen area.
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_DefWindow( void );

//-----------------------------------------------------------------------------
//  GUI_DispAll
///		@brief
///      This function displays the contents of video bitmap memory
///      on the visible screen.  Each bit in the video bitmap memory
///      affects a corresponding pixel on the screen as described below.
///
///      For color screens, each '1' bit in the video bitmap memory causes
///      the corresponding pixel to be drawn in the current foreground
///      color and each '0' bit causes the corresponding pixel to be drawn
///      in the current background color.
///
///      For monochrome screens, each '1' bit in the video bitmap memory
///      turns on the correpsonding screen pixel and each '0' bit turns
///      off the corresponding pixel.
///
///      This function updates the entire screen.  Use GUI_DispPart
///      to update just a portion of the screen.
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_DispAll( void );

//-----------------------------------------------------------------------------
//  GUI_DispPart
///		@brief
///      This function displays the contents of video bitmap memory
///      on the visible screen.  Each bit in the video bitmap memory
///      affects a corresponding pixel on the screen as described below.
///
///      For color screens, each '1' bit in the video bitmap memory causes
///      the corresponding pixel to be drawn in the current foreground
///      color and each '0' bit causes the corresponding pixel to be drawn
///      in the current background color.
///
///      For monochrome screens, each '1' bit in the video bitmap memory
///      turns on the correpsonding screen pixel and each '0' bit turns
///      off the corresponding pixel.
///
///      This function updates just the specified portion of the screen.
///      Use GUI_DispAll to update the entire screen.
///
///  @param
///      page        Memory page of the structure pointed to by pRect.
///  @param
///      pRect       Pointer to the area's coordinates.
///
///  @return
///      Nothing.
///
///  @note
///      The specified coordinates must be absolute screen coordinates.
///      Virtual window coordinates are ignored by this function.
//-----------------------------------------------------------------------------
extern void GUI_DispPart( unsigned char page, const RECT * pRect );

//-----------------------------------------------------------------------------
//  GUI_DispPartBtn3D
///		@brief
///      This function is similar to GUI_DispPart() in that it updates the
///      the screen with information from the screen buffer, setting the
///      foreground and background colors accordingly.  However, this
///      function only updates the movable portion of a 3D button so as not
///      to affect the lower-left and upper-right pixels of the button area.
///      Those pixels need to remain unchanged since they are part of the
///      background surrounding the button and therefore may be a different
///      color than the button itself.
///
///      The button is drawn using the following color scheme:
///
///          color1:     Face foreground (e.g. text label)\n
///          color2:     Face background\n
///          color3:     Border\n
///          color4:     Shading\n
///
///  @param
///      page        Memory page of the structure pointed to by pBtn.
///  @param
///      pBtn        Pointer to the button definition structure.
///  @param
///      state       Current state of the button (BUTTON_UP or BUTTON_DOWN).
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
#ifndef MAKE_MONOCHROME
extern void GUI_DispPartBtn3D( unsigned char page, const BUTTON_DEF * pBtn, BUTTON_STATE state );
#endif

//-----------------------------------------------------------------------------
//  GUI_DispVirWin
///		@brief
///      This function displays the contents of the virtual window area
///      of video bitmap memory on the visible screen.  Each bit in the
///      video bitmap memory affects a corresponding pixel on the screen
///      as described below.
///
///      For color screens, each '1' bit in the video bitmap memory causes
///      the corresponding pixel to be drawn in the current foreground
///      color and each '0' bit causes the corresponding pixel to be drawn
///      in the current background color.
///
///      For monochrome screens, each '1' bit in the video bitmap memory
///      turns on the correpsonding screen pixel and each '0' bit turns
///      off the corresponding pixel.
///
///  @return
///      Nothing
//-----------------------------------------------------------------------------
extern void GUI_DispVirWin( void );

//-----------------------------------------------------------------------------
//  GUI_DisplayPCX
///		@brief
///      Display a PCX graphics image.
///
///  @param
///      x           X coordinate of where to display the image.
///  @param
///      y           Y coordinate of where to display the image.
///  @param
///      page        Logical page number of the structure pointed to by pPCX.
///  @param
///      pPCX        Pointer to the graphics image.
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_DisplayPCX( unsigned short x, unsigned short y,
                            unsigned char page, const PCX_IMAGE * pPCX );

//-----------------------------------------------------------------------------
//  GUI_DisplayPCXFile
///		@brief
///      Display the given PCX graphics image file.
///
///  @param
///      x           X coordinate of where to display the image.
///  @param
///      y           Y coordinate of where to display the image.
///  @param
///      page        Logical page number on which the filename string exists.
///  @param
///      pFilename   Pointer to the filename.
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_DisplayPCXFile( unsigned short x, unsigned short y,
                                unsigned char page, const char * pFilename );

//-----------------------------------------------------------------------------
//  GUI_Frame
///		@brief
///      This function draws a rectangle at the given coordinates with
///      the given width and height.  The thickness of the rectangle's
///      lines are specified with the border parameter.  The area inside
///      the rectangle is not affected.
///
///      The frame is rendered in video bitmap memory.  The visible
///      screen is not affected until a subsequent call is made to one
///      of the screen update functions, such as GUI_DispPart().
///
///  @param
///      page        Memory page of the structure pointed to by pRect.
///  @param
///      pRect       Pointer to the frame's area.  The coordinates
///                  are relative to the current virtual window.
///  @param
///      border      Border width (in pixels).
///
///  @return
///      Nothing.
///
///  @note See Also:
///      GUI_Rectangle
//-----------------------------------------------------------------------------
extern void GUI_Frame( unsigned char page, const RECT *pRect, unsigned short border );

//-----------------------------------------------------------------------------
//  GUI_GetBitMap
///		@brief
///      This function copies an area of bitmap video memory to a buffer
///      supplied by the application.  Entire lines of the video memory
///      are copied, unlike GUI_GetBlock which copies a rectangular area
///      of the color video memory.
///
///  @param
///      page        Memory page on which the application buffer resides.
///  @param
///      buffer      Pointer to the destination buffer.
///  @param
///      y           Y-coordinate of the first video memory row to copy.
///  @param
///      height      Number of rows to copy.
///
///  @return
///      Nothing.
///
///  @note See Also:
///      GUI_PutBitMap, GUI_MoveBitMap, GUI_GetBlock, GUI_PutBlock, GUI_MoveBlock
//-----------------------------------------------------------------------------
extern void GUI_GetBitMap( unsigned char page,
                           void * buffer,
                           unsigned short y,
                           unsigned short height
                         );

//-----------------------------------------------------------------------------
//  GUI_GetBlock
///		@brief
///      This function copies a rectangular area of the color video
///      memory to a buffer supplied by the application.
///
///  @param
///      page        Memory page on which the GUI_GETPUT_BLK structure AND
///                  the buffer reside.
///  @param
///      pBlk        Pointer to the structure containing the block
///                  information.
///  @return
///      Nothing.
///
///  @note See Also:
///      GUI_PutBlock, GUI_MoveBlock, GUI_GetBitMap, GUI_PutBitMap, GUI_MoveBitMap
//-----------------------------------------------------------------------------
extern void GUI_GetBlock( unsigned char page, const GUI_GETPUT_BLK * pBlk );

//-----------------------------------------------------------------------------
//  GUI_GetCharHeight
///		@brief
///      This function returns the height of a character in the current font.
///
///  @return
///      The height of a character (in pixels).
///
///  @note
///      The value returned by this function includes the pixels between
///      lines of text.  Use GUI_GetCharHeightNoSpc to get the height of
///      a character without the line spacing.
//-----------------------------------------------------------------------------
extern unsigned char GUI_GetCharHeight( void );

//-----------------------------------------------------------------------------
//  GUI_GetCharHeightNoSpc
///		@brief
///      This function returns the height of a character in the current font,
///      excluding the space below the character which separates lines of text.
///
///  @return
///      The height of a character (in pixels).
///
///  @note
///      The value returned by this function excludes the pixels between
///      lines of text.  Use GUI_GetCharHeight to get the height of a
///      character including the line spacing.
//-----------------------------------------------------------------------------
extern unsigned char GUI_GetCharHeightNoSpc( void );

//-----------------------------------------------------------------------------
//  GUI_GetCharWidth
///		@brief
///      This function returns the width of a character in the current font.
///
///  @return
///      The width of a character (in pixels).
///
///  @note
///      The value returned by this function includes the pixels between
///      characters.  Use GUI_GetCharWidthNoSpc to get the width of a
///      character without the inter-character spacing.
//-----------------------------------------------------------------------------
extern unsigned char GUI_GetCharWidth( void );

//-----------------------------------------------------------------------------
//  GUI_GetCharWidthNoSpc
///		@brief
///      This function returns the width of a character in the current font,
///      excluding the space to the right of the character which separates
///      it from other characters.
///
///  @return
///      The width of a character (in pixels).
///
///  @note
///      The value returned by this function excludes the pixels between
///      characters.  Use GUI_GetCharWidth to get the width of a character
///      with the inter-character spacing.
//-----------------------------------------------------------------------------
extern unsigned char GUI_GetCharWidthNoSpc( void );

//-----------------------------------------------------------------------------
//  GUI_GetColors
///		@brief
///      This function returns the system's current color settings.
///
///  @param
///      pColorInfo  Pointer to structure to hold current color information.
///
///  @return
///      Nothing
//-----------------------------------------------------------------------------
extern void GUI_GetColors( DISPLAY_COLOR * pColorInfo );

//-----------------------------------------------------------------------------
//  GUI_GetFontID
///		@brief
///      This function returns the ID of the current font.  The value
///      returned will depend on which font was most recently selected
///      as described below:
///
///          \li If a system font is active, the value returned will be the
///          font ID passed to the GUI_SetFont() function.
///
///          \li If a custom font is active, the value returned will be the
///          application-defined font ID passed to the GUI_SetCustomFont()
///          function.
///
///          \li If a Unicode font is active, the value returned will be the
///          application-defined font ID passed to the GUI_SetUnicodeFont()
///          function.
///
///          \li If a soft font is active, the constant FONT_SOFTFONT will be
///          returned.
///
///      For system, custom, or soft fonts, the returned value indicates
///      the font used for characters 0x00 through 0x7F.
///
///  @return
///      The font ID of the current font, or FONT_SOFTFONT if a soft
///      font is active.
///
///  @note
///      Use GUI_GetXFontID to get the font ID for the current extended font.
//-----------------------------------------------------------------------------
extern StdFontID GUI_GetFontID( void );

//-----------------------------------------------------------------------------
//  GUI_GetMaxChars
///		@brief
///      This function returns the number of characters in the current font
///      that can be displayed on a line in the current virtual window.
///
///  @return
///      The maximum number of characters per line.
//-----------------------------------------------------------------------------
extern unsigned char GUI_GetMaxChars( void );

//-----------------------------------------------------------------------------
//  GUI_GetMaxLines
///		@brief
///      This function returns the number of lines of text that can be
///      displayed in the current virtual window (using the current font).
///
///  @return
///      The maximum number of lines of text.
//-----------------------------------------------------------------------------
extern unsigned char GUI_GetMaxLines( void );

//-----------------------------------------------------------------------------
//  GUI_GetUnicodeBitmap
///		@brief
///      The GUI library calls this function when it's time to draw a
///      Unicode character.  The application must provide an implementation
///      of this function that, when called, copies the bitmap image for
///      the given character into the specified buffer on the GUI page.
///
///      ------ THIS FUNCTION MUST BE SUPPLIED BY THE APPLICATION -----
///
///      The destination buffer on the GUI page is MAX_CHAR_BITMAP_SIZE
///      bytes in size.  The application must not write beyond of the
///      buffer.
///
///  @param
///      guiPage     Logical memory page on which the buffer pointed to
///                  by pGUIBuffer exists.
///
///  @param
///      pGUIBuffer  The buffer to which the character bitmap must
///                  be copied.
///
///  @param
///      ch          The two-byte Unicode character code.
///
///  @return
///      Nothing
//-----------------------------------------------------------------------------
extern void GUI_GetUnicodeBitmap( unsigned char guiPage,
                                  unsigned char * pGUIBuffer,
                                  unsigned short ch );

//-----------------------------------------------------------------------------
//  GUI_GetUpdatePage
///		@brief
///      This function returns the logical video page number on which
///      drawing is performed.
///
///      See GUI_SetUpdatePage() for more information about video pages.
///
///  @return
///      The logical video page number on which drawing is performed.
//-----------------------------------------------------------------------------
#ifndef MAKE_MONOCHROME
extern unsigned char GUI_GetUpdatePage( void );
#endif

//-----------------------------------------------------------------------------
//  GUI_GetVisualPage
///		@brief
///      This function returns the logical video page number currently
///      being displayed on the screen.
///
///      See GUI_SetUpdatePage() for more information about video pages.
///
///  @return
///      The logical video page number currently being displayed on
///      the screen.
//-----------------------------------------------------------------------------
#ifndef MAKE_MONOCHROME
extern unsigned char GUI_GetVisualPage( void );
#endif

//-----------------------------------------------------------------------------
//  GUI_GetWindow
///		@brief
///      This function returns the coordinates of the virtual window.
///
///  @param
///      page        Memory page of the structure pointed to by pRect.
///  @param
///      pRect       Pointer to the structure which is to receive the
///                  virtual window's coordinates.
///                  Note: the returned coordinates are absolute
///                  screen coordinates.
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_GetWindow( unsigned char page, RECT *pRect );

//-----------------------------------------------------------------------------
//  GUI_GetXFontID
///		@brief
///      This function returns the ID of the current extended font (for
///      characters 0x80 through 0xFF).
///
///  @return
///      Extended font ID.
///
///  @note
///      Use GUI_GetFontID to get the font ID for the current standard font.
//-----------------------------------------------------------------------------
extern ExtFontID GUI_GetXFontID( void );

//-----------------------------------------------------------------------------
//  GUI_Horizontal
///		@brief
///      This function draws a horizontal line of the specified length
///      starting at the given coordinates.  The coordinates are
///      relative to the current virtual window and the line is always
///      one pixel thick.
///
///      The line is rendered in video bitmap memory.  The visible
///      screen is not affected until a subsequent call is made to one
///      of the screen update functions, such as GUI_DispPart().
///
///  @param
///      x           Starting X coordinate.
///  @param
///      y           Starting Y coordinate.
///  @param
///      len         Length of the line (drawn left to right).
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_Horizontal( unsigned short x, unsigned short y, unsigned short len );

//-----------------------------------------------------------------------------
//  GUI_Image
///		@brief
///      This function draws a Hypercom Graphics Format image.
///
///  @param
///      x           X coordinate of where to draw the image.
///  @param
///      y           Y coordinate of where to draw the image.
///  @param
///      page        Memory page of the structure pointed to by pImage.
///  @param
///      pImage      Pointer to the image header.
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_Image( unsigned short x, unsigned short y, unsigned char page, const IMAGE * pImage );

//-----------------------------------------------------------------------------
//  GUI_ImageTiled
///    @brief
///     This function draws a Hypercom Graphics Format image.
///
///  @param
///     x           X coordinate of where to draw the image.
///  @param
///     y           Y coordinate of where to draw the image.
///  @param
///     width       Width of area (not used).
///  @param
///     height      Height of area (not used).
///  @param
///     page        Memory page of the structure pointed to by pImage.
///  @param
///     pImage      Pointer to the image header.
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_ImageTiled( unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char page, const IMAGE * pImage );

//-----------------------------------------------------------------------------
//  GUI_Init
///		@brief
///      This function initializes the GUI library and must be called
///      before calling any other GUI routine.
///
///      In addition to initializing the library, the following
///      operations are performed:
///
///          1) The visual and update pages are set to logical page 0.
///
///          2) The standard Hypercom palette is set.
///
///          3) The foreground color is set to COLOR_BLUE and the
///             background color is set to COLOR_WHITE.
///
///          4) The screen is cleared.
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_Init( void );

//-----------------------------------------------------------------------------
//  GUI_InvArea
///		@brief
///      This function inverts a portion of video bitmap memory by
///      setting all '0' bits to '1' and vice versa.
///
///      The visible screen is not affected until a subsequent call
///      is made to one of the screen update functions, such as
///      GUI_DispPart().
///
///  @param
///      page        Memory page of the structure pointed to by pRect.
///  @param
///      pRect       Pointer to the area's coordinates.
///
///  @return
///      Nothing
///
///  @note
///      The specified coordinates must be absolute screen coordinates.
///      Virtual window coordinates are ignored by this function.
//-----------------------------------------------------------------------------
extern void GUI_InvArea( unsigned char page, const RECT * pRect );

//-----------------------------------------------------------------------------
//  GUI_MoveBitMap
///		@brief
///      This function copies a screen image from one area of the bitmap
///      video memory to another.  Entire lines of the video memory are
///      copied, unlike GUI_MoveBlock which copies a rectangular area of
///      the color video memory.
///
///  @param
///      srcY        Y coordinate of the first video memory row to copy.
///  @param
///      dstY        Y coordinate of the destination row.
///  @param
///      h           Number of rows to copy.
///
///  @return
///      Nothing.
///
///  @note See Also:
///      GUI_MoveBlock
//-----------------------------------------------------------------------------
extern void GUI_MoveBitMap( unsigned short srcY, unsigned short dstY, unsigned short h );

//-----------------------------------------------------------------------------
//  GUI_MoveBlock
///		@brief
///      This function copies a screen image from one area of color video
///      memory to another.
///
///  @param
///      page        Memory page on which the GUI_MOVE_BLK structure resides.
///  @param
///      pBlk        Pointer to the structure containing the block
///                  information.
///  @return
///      Nothing.
///
///  @note See Also:
///      GUI_MoveBitMap
//-----------------------------------------------------------------------------
extern void GUI_MoveBlock( unsigned char page, const GUI_MOVE_BLK * pBlk );

//-----------------------------------------------------------------------------
//  GUI_PutBitMap
///		@brief
///      This function copies a screen image from a buffer supplied by the
///      application directly to bitmap video memory.  Entire lines of
///      are copied to video memory, unlike GUI_PutBlock which copies a
///      rectangular area to color video memory.
///
///  @param
///      page        Memory page on which the application buffer resides.
///  @param
///      buffer      Pointer to the source buffer.
///  @param
///      y           Y-coordinate of the destination row in video memory.
///  @param
///      height      Number of rows to copy.
///
///  @return
///      Nothing.
///
///  @note See Also:
///      GUI_GetBitMap, GUI_MoveBitMap, GUI_GetBlock, GUI_PutBlock, GUI_MoveBlock
//-----------------------------------------------------------------------------
extern void GUI_PutBitMap( unsigned char page,
                           void * buffer,
                           unsigned short y,
                           unsigned short height
                         );

//-----------------------------------------------------------------------------
//  GUI_PutBlock
///		@brief
///      This function copies a screen image from a buffer supplied by the
///      application directly to the color video memory.
///
///  @param
///      page        Memory page on which the GUI_GETPUT_BLK structure AND
///                  the buffer reside.
///  @param
///      pBlk        Pointer to the structure containing the block
///                  information.
///  @return
///      Nothing.
///
///  @note See Also:
///      GUI_GetBlock, GUI_MoveBlock, GUI_GetBitMap, GUI_PutBitMap, GUI_MoveBitMap
//-----------------------------------------------------------------------------
extern void GUI_PutBlock( unsigned char page, const GUI_GETPUT_BLK * pBlk );

//-----------------------------------------------------------------------------
//  GUI_Rectangle
///		@brief
///      This function draws a rectangle at the given coordinates with
///      the given width and height.  The lines of the rectangle are
///      always one pixel thick.  The area inside the rectangle is not
///      affected.
///
///      The rectangle is rendered in video bitmap memory.  The visible
///      screen is not affected until a subsequent call is made to one
///      of the screen update functions, such as GUI_DispPart().
///
///  @param
///      page        Memory page of the structure pointed to by pRect.
///  @param
///      pRect       Pointer to the rectangle's area.  The coordinates
///                  are relative to the current virtual window.
///  @return
///      Nothing.
///
///  @note See Also:
///      GUI_Frame
//-----------------------------------------------------------------------------
extern void GUI_Rectangle( unsigned char page, const RECT *pRect );

//-----------------------------------------------------------------------------
//  GUI_SetArea
///		@brief
///      This function fills a portion of video bitmap memory with '1' bits.
///
///      The visible screen is not affected until a subsequent call
///      is made to one of the screen update functions, such as
///      GUI_DispPart().
///
///  @param
///      page        Memory page of the structure pointed to by pRect.
///  @param
///      pRect       Pointer to the area's coordinates.
///
///  @return
///      Nothing
///
///  @note
///      The specified coordinates must be absolute screen coordinates.
///      Virtual window coordinates are ignored by this function.
//-----------------------------------------------------------------------------
extern void GUI_SetArea( unsigned char page, const RECT * pRect );

//-----------------------------------------------------------------------------
//  GUI_SetColorLUT
///		@brief
///      This function sets the color palette which the display uses
///      to convert an 8-bit color index into an RGB value.
///
///  @param
///      palette     Pointer to a 768-byte array containing the
///                  3-byte RGB values for each of the 256 colors.
///  @return
///      Nothing
//-----------------------------------------------------------------------------
#ifndef MAKE_MONOCHROME
extern void GUI_SetColorLUT( const unsigned char palette[768] );
#endif

//-----------------------------------------------------------------------------
//  GUI_SetColorPair
///		@brief
///      This function sets the system foreground and background colors to
///      the given colors and turns off the background transparency bit.
///
///      This function does not change the color of anything on the
///      screen; it merely saves the new colors in library variables.
///      A subsequent call to one of the screen update functions, such
///      as GUI_DispPart(), is required to change colors on the screen.
///
///  @param
///      fgColor     Desired foreground color.
///  @param
///      bgColor     Desired background color.
///
///  @return
///      Nothing
//-----------------------------------------------------------------------------
extern void GUI_SetColorPair( COLOR fgColor, COLOR bgColor );

//-----------------------------------------------------------------------------
//  GUI_SetColors
///		@brief
///      This function changes the system's color settings.
///
///      This function does not change the color of anything on the
///      screen; it merely saves the new colors in library variables.
///      A subsequent call to one of the screen update functions, such
///      as GUI_DispPart(), is required to change colors on the screen.
///
///  @param
///      pColorInfo  Pointer to structure with desired color information.
///
///  @return
///      Nothing
//-----------------------------------------------------------------------------
extern void GUI_SetColors( const DISPLAY_COLOR * pColorInfo );

//-----------------------------------------------------------------------------
//  GUI_SetCustomFont
///		@brief
///      This function sets an application-defined font as the active font.
///
///      If the current extended font does not have the same character
///      height as the custom font, the extended font is set to FONT_ENULL.
///
///  @param
///      id          Application-defined font ID for the custom font.
///                  The font ID must be in the range of FIRST_CUSTOM_FONT
///                  through MAX_CUSTOM_FONT (inclusive).  This value will
///                  be returned by GUI_GetFontID() while the custom font
///                  is active.
///
///  @param
///      page        Memory page on which the custom font exists.  The
///                  entire font must be on the same page.
///
///  @param
///      pHdr        Pointer to the font header.
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_SetCustomFont( StdFontID id, unsigned char page, const FONT_HEADER * pHdr );

//-----------------------------------------------------------------------------
//  GUI_SetFont
///		@brief
///      This function sets the system font for both the standard and
///      extended character sets.
///
///      The standard font is used for drawing characters 0x00 through 0x7F.
///      The extended font is used for drawing characters 0x80 through 0xFF.
///
///      If the specified standard font ID is invalid, the smallest
///      system font is selected.
///
///      If the specified extended font ID is invalid, FONT_ENULL is selected.
///
///  @param
///      std         Standard font ID
///  @param
///      ext         Extended font ID (characters 0x80 through 0xFF)
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_SetFont( StdFontID std, ExtFontID ext );

//-----------------------------------------------------------------------------
//  GUI_SetPixel
///		@brief
///      This function draws a pixel at the given coordinates by
///      setting the corresponding bit in video bitmap memory.  The
///      visible screen is not affected until a subsequent call is made
///      to one of the screen update functions, such as GUI_DispPart().
///
///  @param
///      x           X-coordinate of the pixel.
///  @param
///      y           Y-coordinate of the pixel.
///                  Note: the coordinates are relative to the current
///                  virtual window.
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_SetPixel( unsigned short x, unsigned short y );

//-----------------------------------------------------------------------------
//  GUI_SetStdPalette
///		@brief
///      This function sets the system color palette to the standard
///      Hypercom palette.  It simply calls GUI_SetColorLUT() with
///      a pointer to GUI_StdPalette.
///
///  @return
///      Nothing
//-----------------------------------------------------------------------------
#ifndef MAKE_MONOCHROME
extern void GUI_SetStdPalette( void );
#endif

//-----------------------------------------------------------------------------
//  GUI_SetUnicodeFont
///		@brief
///      This function sets an application-defined Unicode font as the
///      active font.
///
///      If the current extended font does not have the same character
///      height as the standard font associated with the Unicode font,
///      the extended font is set to FONT_ENULL.
///
///      In order for Unicode characters to be drawn, the application must
///      supply a function called GUI_GetUnicodeBitmap().  For more
///      information, see the function description of GUI_GetUnicodeBitmap()
///      in this header file.
///
///  @param
///      id          Application-defined font ID for the Unicode font.
///                  The font ID must be in the range of FIRST_CUSTOM_FONT
///                  through MAX_CUSTOM_FONT (inclusive).  This value will
///                  be returned by GUI_GetFontID() while the Unicode font
///                  is active.
///
///  @param
///      page        Memory page on which the font header and range table
///                  exists.  The individual character bitmaps may reside
///                  on any page accessible by the callback function.
///
///  @param
///      pHdr        Pointer to the Unicode font header.
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_SetUnicodeFont( StdFontID id, unsigned char page,
                                const UNICODE_HEADER * pHdr );

//-----------------------------------------------------------------------------
//  GUI_SetUpdatePage
///		@brief
///      This function sets the logical video page to be used for
///      drawing and for the display update functions such as
///      GUI_DispAll().
///
///      Description of Video Pages
///
///          For ICE6000 and ICE6500 terminals, there are two modes of
///          operation and up to 16 logical video pages.
///
///          In the "normal" mode of operation, there are 16 logical video
///          pages each equal in size to the physical screen (320 x 240
///          pixels).  The pages are numbered 0 through 15.  Each page is
///          independent of the others and can be individually selected for
///          drawing and updating (with GUI_SetUpdatePage()) or for display
///          (with GUI_SetVisualPage()).
///
///          In the "virtual display" mode of operation, the 16 logical pages
///          are replaced by a single video page with Y coordinates ranging
///          from 0 through VIRTUAL_SCR_MAX_HEIGHT - 1.  The physical screen
///          always displays the first 240 lines of video memory (Y coordinates
///          0 through 239).  Other areas of video memory can be displayed by
///          coping them into the visible portion of video memory using
///          GUI_MoveBlock() and/or GUI_MoveBitMap().  Virtual display mode is
///          selected by setting the update page to page 16.  Consequently,
///          this mode is sometimes referred to as "page 16 mode".
///          
///  @param
///      page        Desired logical video page to be used for drawing.
///
///  @return
///      OK if the video page was set successfully, or ERRINV if the
///      given page number is invalid.
//-----------------------------------------------------------------------------
#ifndef MAKE_MONOCHROME
extern OSRETURNCODES GUI_SetUpdatePage(unsigned char page);
#endif

//-----------------------------------------------------------------------------
//  GUI_SetVisualPage
///		@brief
///      This function selects which logical video page is displayed
///      on the screen.
///
///      See GUI_SetUpdatePage() for more information about video pages.
///      In "normal" video mode, the visual page can be set to any logical
///      page in the range 0 through 15.  In "virtual display" mode,
///      this function is unusable and always returns ERRINV.
///
///  @param
///      page        Desired logical video page to be displayed.
///
///  @return
///      OK if the video page was set successfully, or ERRINV if the
///      given page number is invalid.
//-----------------------------------------------------------------------------
#ifndef MAKE_MONOCHROME
extern OSRETURNCODES GUI_SetVisualPage(unsigned char page);
#endif

//-----------------------------------------------------------------------------
//  GUI_SetWindow
///		@brief
///      This function sets the coordinates of the virtual window.
///      The virtual window can be located anywhere within the
///      logical display area.
///
///  @param
///      page        Memory page of the structure pointed to by pRect.
///  @param
///      pRect       Pointer to the virtual window's coordinates.
///                  Note: the given coordinates must be absolute
///                  screen coordinates.
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_SetWindow( unsigned char page, const RECT *pRect );

//-----------------------------------------------------------------------------
//  GUI_Shutdown
///		@brief
///      This function shuts down the GUI hardware.  It should never be
///      called on a terminal; it is provided only for when the terminal
///      is emulated on a desktop PC.
///
///  @return
///      Nothing
//-----------------------------------------------------------------------------
extern void GUI_Shutdown( void );

//-----------------------------------------------------------------------------
//  GUI_Text
///		@brief
///      This function draws a text string starting at the given character
///      location (column and line).
///
///  @param
///      col         Character column position.
///  @param
///      line        Character line position.
///  @param
///      page        Memory page of the string pointed to by pText.
///  @param
///      pText       Pointer to the text string.
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_Text( unsigned char col, unsigned char line, unsigned char page, const char *pText );

//-----------------------------------------------------------------------------
//  GUI_TextP
///		@brief
///      This function draws a text string starting at the given pixel
///      location (x, y).
///
///  @param
///      x           X coordinate of where to draw the text.
///  @param
///      y           Y coordinate of where to draw the text.
///  @param
///      page        Memory page of the string pointed to by pText.
///  @param
///      pText       Pointer to the text string.
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_TextP( unsigned short x, unsigned short y, unsigned char page, const char *pText );

//-----------------------------------------------------------------------------
//  GetCharBitmap
///	@brief This function returns a pointer to the bitmap image of the
///	       specified character in the given font.
///
///	@param pFont Pointer to the font definition header.
///	@param ch ASCII code of the requested character.
///
///	@return A pointer to the bitmap of the requested character.
//-----------------------------------------------------------------------------
extern const unsigned char * GetCharBitmap( const FONT_HEADER * pFont, unsigned char ch );

#if INCLPAD
//-----------------------------------------------------------------------------
//  GUI_UpdateButtons
///		@brief
///      This function visually updates any buttons on the screen which are
///      affected by the given touch-pad event.  If the pen action caused
///      a button to be activated (previously pressed and now released), the
///      corresponding button ID is returned.
///
///  @param
///      x       The X-coordinate of the touch-pad event.
///  @param
///      y       The Y-coordinate of the touch-pad event.
///  @param
///      state   The state of the pen which caused the event.
///
///  @return
///      The button ID of the button which was activated (pressed and released)
///      or zero if no button was activated.
//-----------------------------------------------------------------------------
extern unsigned char GUI_UpdateButtons( unsigned short x, unsigned short y, PEN_STATE state );
#endif

//-----------------------------------------------------------------------------
//  GUI_Vertical
///		@brief
///      This function draws a vertical line of the specified length
///      starting at the given coordinates.  The coordinates are
///      relative to the current virtual window and the line is always
///      one pixel thick.
///
///      The line is rendered in video bitmap memory.  The visible
///      screen is not affected until a subsequent call is made to one
///      of the screen update functions, such as GUI_DispPart().
///
///  @param
///      x           Starting X coordinate.
///  @param
///      y           Starting Y coordinate.
///  @param
///      len         Length (height) of the line (drawn top to bottom).
///
///  @return
///      Nothing.
//-----------------------------------------------------------------------------
extern void GUI_Vertical( unsigned short x, unsigned short y, unsigned short len );

//-----------------------------------------------------------------------------
//  GUI_VirWinEndX
///		@brief
///      This function returns the X-coordinate of the right edge of
///      the current virtual window (i.e. Starting X + Width - 1).
///
///  @return
///      The X-coordinate in absolute screen coordinates.
//-----------------------------------------------------------------------------
extern unsigned short GUI_VirWinEndX( void );

//-----------------------------------------------------------------------------
//  GUI_VirWinEndY
///		@brief
///      This function returns the Y-coordinate of the bottom edge of
///      the current virtual window (i.e. Starting Y + Height - 1).
///
///  @return
///      The Y-coordinate in absolute screen coordinates.
//-----------------------------------------------------------------------------
extern unsigned short GUI_VirWinEndY( void );

//-----------------------------------------------------------------------------
//  GUI_VirWinLenX
///		@brief
///      This function returns the width (X length) of the current
///      virtual window.
///
///  @return
///      The width of the virtual window (in pixels).
//-----------------------------------------------------------------------------
extern unsigned short GUI_VirWinLenX( void );

//-----------------------------------------------------------------------------
//  GUI_VirWinLenY
///		@brief
///      This function returns the height (Y length) of the current
///      virtual window.
///
///  @return
///      The height of the virtual window (in pixels).
//-----------------------------------------------------------------------------
extern unsigned short GUI_VirWinLenY( void );

//-----------------------------------------------------------------------------
//  GUI_VirWinOffsetX
///		@brief
///      This function returns the offset (in pixels) from the left edge of
///      the virtual window necessary to center a full line of text in the
///      current font.
///
///  @return
///      X offset.
//-----------------------------------------------------------------------------
extern unsigned char GUI_VirWinOffsetX( void );

//-----------------------------------------------------------------------------
//  GUI_VirWinOffsetY
///		@brief
///      This function returns the offset (in pixels) from the top edge of
///      the virtual window necessary to center the maximum number of lines
///      of text in the current font.
///
///  @return
///      Y offset.
//-----------------------------------------------------------------------------
extern unsigned char GUI_VirWinOffsetY( void );

//-----------------------------------------------------------------------------
//  GUI_VirWinStartX
///		@brief
///      This function returns the X-coordinate of the left edge of
///      the current virtual window.
///
///  @return
///      The X-coordinate in absolute screen coordinates.
//-----------------------------------------------------------------------------
extern unsigned short GUI_VirWinStartX( void );

//-----------------------------------------------------------------------------
//  GUI_VirWinStartY
///		@brief
///      This function returns the Y-coordinate of the top edge of
///      the current virtual window.
///
///  @return
///      The Y-coordinate in absolute screen coordinates.
//-----------------------------------------------------------------------------
extern unsigned short GUI_VirWinStartY( void );

//-----------------------------------------------------------------------------
//  GUI_RestoreScr
///		@brief
///      Restore an image of the screen which was saved with GUI_StoreScr.
///
///  @return
///      Nothing.
///
///  @note
///      1) Since there is only one save buffer, calls to this function cannot
///         be nested.
///
///      2) The screen can only be saved and restored on ICE5000, ICE5500, and
///         ICE4000 terminals.  On ICE6000 and ICE6500 terminals, this function
///         is defined but does nothing.
//-----------------------------------------------------------------------------
extern void GUI_RestoreScr( void );

//-----------------------------------------------------------------------------
//  GUI_StoreScr
///		@brief
///      Store an image of the screen to a save buffer.  Use GUI_RestoreScr to
///      move the save buffer back to the screen.
///
///  @return
///      Nothing.
///
///  @note
///      1) Since there is only one save buffer, calls to this function cannot
///         be nested.
///
///      2) The screen can only be saved and restored on ICE5000, ICE5500, and
///         ICE4000 terminals.  On ICE6000 and ICE6500 terminals, this function
///         is defined but does nothing.
//-----------------------------------------------------------------------------
extern void GUI_StoreScr( void );

//-----------------------------------------------------------------------------
//  GUI_LoadExtCustomFont
///		@brief
///      Function to copy the extended font bitmaps from an application page to
///      the system FON page.
///
///  @param
///      FontId          Font ID to associate with the font.
///  @param
///      Page            Logical page number.
///  @param
///      pAddress        Font bitmaps address at application page.
///  @param
///      DataSize        Total data to copy.
///  @return
///      0 - Font loaded.\n
///      1 - Error, DataSize too big.\n
///      2 - Invalid Font ID.\n
///
///  @note
///      This function must be used before calling
///      GUI_SetFont( xxx, FONT_ECUSTOM ) or GUI_SetFont( xxx, FONT_EXxxxxxx ).
///
//-----------------------------------------------------------------------------
UBYTE GUI_LoadExtCustomFont( UBYTE FontId, UBYTE Page, const void *pAddress, UWORD DataSize);

/**
	@}
*/

#endif  // _OSGUI_H_
