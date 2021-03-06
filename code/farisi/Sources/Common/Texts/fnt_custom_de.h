//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      fnt_custom_de.h
//
// File Description:
//      German extended font definition (for display).
// 		This Extended Windiws/Western ASCII Font uses the Font parameters (height, width, ...)
//  	defined for its associated Standard ASCII Font.  This Extended
//		ASCII Font is associated with the following Standard ASCII Font.
//
//		Standard Font Name: FONT_SA05NN07
//
//=============================================================================

#include "osclib.h"         // For NULL definition

//=============================================================================
// Private defines and typedefs
//=============================================================================
//! bitmap size
#define CHAR_BITMAP_SIZE    7
//! character data
static const unsigned char char_80_FF[];
//! font sections definition
static const FONT_SECTION_HEADER Sections;

//=============================================================================
// Private function declarations
//=============================================================================

//! font section header
static const FONT_SECTION_HEADER section1;

//=============================================================================
// Public data definitions
//=============================================================================


//--------------------------------------------------
//! Font header definition
//--------------------------------------------------
const FONT_HEADER FONT_EX_DE_05NN07_DEF =
{
    FONT_FMT_ASCII,     // format
    CHAR_BITMAP_SIZE,   // bitmapSize
    5,                  // width
    7,                  // height
    1,                  // charSpace
    1,                  // rowSpace
    &Sections           // pSection
};
//
//--------------------------------------------------
//! Pull of sections definition (this is single section font)
//--------------------------------------------------
static const FONT_SECTION_HEADER Sections =
{
    0x80,           // firstChar
    0xFF,           // lastChar
    NULL,		    // pNext section
	char_80_FF		// character data
};
//
//--------------------------------------------------
//! Section 1 character bitmap definition
//--------------------------------------------------
static const unsigned char char_80_FF [] =
{
//!Character 0x80 - Euro Sign
//
//             84218
//            -------   
    0x30, //  |  ** |
    0x48, //  | *  *|
    0xF0, //  |**** |
    0x40, //  | *   |
    0xF0, //  |**** |
    0x48, //  | *  *|
    0x30, //  |  ** |
//            -------   
//
//Character 0x81
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x82 - 	Quotation Mark
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x30, //  |  ** |
    0x30, //  |  ** |
    0x10, //  |   * |
//            -------   
//
//Character 0x83 - Latin Small Letter F With Hook
//
//             84218
//            -------   
    0x18, //  |   **|
    0x20, //  |  *  |
    0x70, //  | *** |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0xC0, //  |**   |
//            -------   
//
//Character 0x84 - Double Low-9 Quotation Mark
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0xD8, //  |** **|
    0xD8, //  |** **|
    0x48, //  | *  *|
//            -------   
//
//Character 0x85 - Horizontal Ellipsis 
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0xA8, //  |* * *|
    0x00, //  |     |
//            -------   
//
//Character 0x86 - Dagger
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x20, //  |  *  |
    0xF8, //  |*****|
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
//            -------   
//
//Character 0x87 - Double Dagger
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x20, //  |  *  |
    0xF8, //  |*****|
    0x20, //  |  *  |
    0xF8, //  |*****|
    0x20, //  |  *  |
    0x20, //  |  *  |
//            -------   
//
//Character 0x88 - Modifier Letter Circumflex Accent
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x50, //  | * * |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x89 - Per Mille Sign
//
//             84218
//            -------   
    0xC8, //  |**  *|
    0xD0, //  |** * |
    0x20, //  |  *  |
    0x40, //  | *   |
    0x80, //  |*    |
    0xD8, //  |** **|
    0xD8, //  |** **|
//            -------   
//
//Character 0x8A - Latin Capitaj Letter S With Caron
//
//             84218
//            -------   
    0x50, //  | * * |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x80, //  |*    |
    0x70, //  | *** |
    0x08, //  |    *|
    0xF0, //  |**** |
//            -------   
//
//Character 0x8B - Single Left-Pointing Angle Quotation Mark
//
//             84218
//            -------   
    0x08, //  |    *|
    0x10, //  |   * |
    0x20, //  |  *  |
    0x40, //  | *   |
    0x20, //  |  *  |
    0x10, //  |   * |
    0x08, //  |    *|
//            -------   
//
//Character 0x8C - Latin Capital Ligature Oe
//
//             84218
//            -------   
    0x78, //  | ****|
    0xA0, //  |* *  |
    0xA0, //  |* *  |
    0xB8, //  |* ***|
    0xA0, //  |* *  |
    0xA0, //  |* *  |
    0x78, //  | ****|
//            -------   
//
//Character 0x8D
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x8E - Latin Capital Letter Z With Caron
//
//             84218
//            -------   
    0x50, //  | * * |
    0x20, //  |  *  |
    0xF8, //  |*****|
    0x10, //  |   * |
    0x20, //  |  *  |
    0x40, //  | *   |
    0xF8, //  |*****|
//            -------   
//
//Character 0x8F
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x90
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x91 - Left Single Quotation Mark
//
//             84218
//            -------   
    0x40, //  | *   |
    0x60, //  | **  |
    0x60, //  | **  |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x92 - Right Single Quotation Mark
//
//             84218
//            -------   
    0x30, //  |  ** |
    0x30, //  |  ** |
    0x10, //  |   * |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x93 - Left Double Quotation Mark
//
//             84218
//            -------   
    0x90, //  |*  * |
    0xD8, //  |** **|
    0xD8, //  |** **|
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x94 - Right Double Quotation Mark
//
//             84218
//            -------   
    0xD8, //  |** **|
    0xD8, //  |** **|
    0x48, //  | *  *|
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x95 - Bullet
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x20, //  |  *  |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x96 - En Dash
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x70, //  | *** |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x97 - Em Dash
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0xF8, //  |*****|
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x98 - Small Tilde
//
//             84218
//            -------   
    0x28, //  |  * *|
    0x50, //  | * * |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x99 - Trade Mark Sign
//
//             84218
//            -------   
    0xE8, //  |*** *|
    0x78, //  | ****|
    0x68, //  | ** *|
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x9A - Latin Small Letter S With Caron
//
//             84218
//            -------   
    0x50, //  | * * |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x80, //  |*    |
    0x70, //  | *** |
    0x08, //  |    *|
    0xF0, //  |**** |
//            -------   
//
//Character 0x9B - Single Right-Pinted Angle Quotation Mark
//
//             84218
//            -------   
    0x80, //  |*    |
    0x40, //  | *   |
    0x20, //  |  *  |
    0x10, //  |   * |
    0x20, //  |  *  |
    0x40, //  | *   |
    0x80, //  |*    |
//            -------   
//
//Character 0x9C - Latin Small Ligature Oe
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x78, //  | ****|
    0xA0, //  |* *  |
    0xB8, //  |* ***|
    0xA0, //  |* *  |
    0x78, //  | ****|
//            -------   
//
//Character 0x9D
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0x9E - Latin Small Letter Z With Caron
//
//             84218
//            -------   
    0x50, //  | * * |
    0x20, //  |  *  |
    0xF8, //  |*****|
    0x10, //  |   * |
    0x20, //  |  *  |
    0x40, //  | *   |
    0xF8, //  |*****|
//            -------   
//
//Character 0x9F - Latin Capital Letter Y With Diaeresis
//
//             84218
//            -------   
    0x50, //  | * * |
    0x00, //  |     |
    0x88, //  |*   *|
    0x50, //  | * * |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
//            -------   
//
//Character 0xA0
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xA1 - Inverted Exclamation Mark
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x00, //  |     |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
//            -------   
//
//Character 0xA2 - Centr Sign
//
//             84218
//            -------   
    0x00, //  |     |
    0x08, //  |    *|
    0x78, //  | ****|
    0xA0, //  |* *  |
    0x78, //  | ****|
    0x80, //  |*    |
    0x00, //  |     |
//            -------   
//
//Character 0xA3 - Pound Sign
//
//             84218
//            -------   
    0x70, //  | *** |
    0x88, //  |*   *|
    0x40, //  | *   |
    0xF0, //  |**** |
    0x40, //  | *   |
    0x40, //  | *   |
    0xF8, //  |*****|
//            -------   
//
//Character 0xA4 - Currency Sign
//
//             84218
//            -------   
    0x00, //  |     |
    0x88, //  |*   *|
    0x70, //  | *** |
    0x50, //  | * * |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x00, //  |     |
//            -------   
//
//Character 0xA5 - Yen Sign
//
//             84218
//            -------   
    0x88, //  |*   *|
    0x50, //  | * * |
    0xF8, //  |*****|
    0x20, //  |  *  |
    0xF8, //  |*****|
    0x20, //  |  *  |
    0x20, //  |  *  |
//            -------   
//
//Character 0xA6 - Broken Bar
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x00, //  |     |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
//            -------   
//
//Character 0xA7 - Section Sign
//
//             84218
//            -------   
    0x30, //  |  ** |
    0x40, //  | *   |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x70, //  | *** |
    0x10, //  |   * |
    0x60, //  | **  |
//            -------   
//
//Character 0xA8 - Diaeresis
//
//             84218
//            -------   
    0xD8, //  |** **|
    0xD8, //  |** **|
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xA9 - Copyright Sign
//
//             84218
//            -------   
    0x00, //  |     |
    0x70, //  | *** |
    0xE8, //  |*** *|
    0xC8, //  |**  *|
    0xE8, //  |*** *|
    0x70, //  | *** |
    0x00, //  |     |
//            -------   
//
//Character 0xAA - Feminine Ordinal Indicator
//
//             84218
//            -------   
    0x60, //  | **  |
    0x10, //  |   * |
    0x70, //  | *** |
    0x90, //  |*  * |
    0x70, //  | *** |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xAB - Left-Pointed Double Angle Quotation Mark
//
//             84218
//            -------   
    0x00, //  |     |
    0x20, //  |  *  |
    0x48, //  | *  *|
    0x90, //  |*  * |
    0x48, //  | *  *|
    0x20, //  |  *  |
    0x00, //  |     |
//            -------   
//
//Character 0xAC - Not Sign
//
//             84218
//            -------   
    0x78, //  | ****|
    0x08, //  |    *|
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xAD - Soft Hyphen
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x30, //  |  ** |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xAE - Registred Sign
//
//             84218
//            -------   
    0x00, //  |     |
    0x70, //  | *** |
    0xE8, //  |*** *|
    0xE8, //  |*** *|
    0xD8, //  |** **|
    0x70, //  | *** |
    0x00, //  |     |
//            -------   
//
//Character 0xAF - Euro Sign
//
//             84218
//            -------   
    0x30, //  |  ** |
    0x48, //  | *  *|
    0xF0, //  |**** |
    0x40, //  | *   |
    0xF0, //  |**** |
    0x48, //  | *  *|
    0x30, //  |  ** |
//            -------   
//
//Character 0xB0 - Degree Sign
//
//             84218
//            -------   
    0x60, //  | **  |
    0x90, //  |*  * |
    0x90, //  |*  * |
    0x60, //  | **  |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xB1 - Plus-Minus Sign
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x20, //  |  *  |
    0xF8, //  |*****|
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x00, //  |     |
    0xF8, //  |*****|
//            -------   
//
//Character 0xB2 - Superscript Two
//
//             84218
//            -------   
    0x60, //  | **  |
    0x90, //  |*  * |
    0x20, //  |  *  |
    0x40, //  | *   |
    0xF0, //  |**** |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xB3 - Superscript Three
//
//             84218
//            -------   
    0xF0, //  |**** |
    0x20, //  |  *  |
    0x10, //  |   * |
    0x90, //  |*  * |
    0x60, //  | **  |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xB4 - Acute Accent
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x40, //  | *   |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xB5 - Micro Sign
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0xD8, //  |** **|
    0xA8, //  |* * *|
    0x80, //  |*    |
//            -------   
//
//Character 0xB6 - Pilcrow Sign
//
//             84218
//            -------   
    0x00, //  |     |
    0x78, //  | ****|
    0xD0, //  |** * |
    0xD0, //  |** * |
    0x50, //  | * * |
    0x50, //  | * * |
    0x50, //  | * * |
//            -------   
//
//Character 0xB7 - Middle Dot
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x60, //  | **  |
    0x60, //  | **  |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xB8 - Cedilla
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
    0x20, //  |  *  |
    0x10, //  |   * |
    0x60, //  | **  |
//            -------   
//
//Character 0xB9 - Superscript One
//
//             84218
//            -------   
    0x40, //  | *   |
    0xC0, //  |**   |
    0x40, //  | *   |
    0x40, //  | *   |
    0xE0, //  |***  |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xBA - Masculine Ordinal Indicator
//
//             84218
//            -------   
    0x60, //  | **  |
    0x90, //  |*  * |
    0x90, //  |*  * |
    0x60, //  | **  |
    0x00, //  |     |
    0x00, //  |     |
    0x00, //  |     |
//            -------   
//
//Character 0xBB - Righth-Pointed Double Angle Quotation Mark
//
//             84218
//            -------   
    0x00, //  |     |
    0x20, //  |  *  |
    0x90, //  |*  * |
    0x48, //  | *  *|
    0x90, //  |*  * |
    0x20, //  |  *  |
    0x00, //  |     |
//            -------   
//
//Character 0xBC - Vulgar Fraction One Quarter
//
//             84218
//            -------   
    0x88, //  |*   *|
    0x90, //  |*  * |
    0xA0, //  |* *  |
    0x48, //  | *  *|
    0x98, //  |*  **|
    0x38, //  |  ***|
    0x08, //  |    *|
//            -------   
//
//Character 0xBD - Vilgar Fraction One Half
//
//             84218
//            -------   
    0x88, //  |*   *|
    0x90, //  |*  * |
    0xA0, //  |* *  |
    0x50, //  | * * |
    0xA8, //  |* * *|
    0x10, //  |   * |
    0x38, //  |  ***|
//            -------   
//
//Character 0xBE - Vulgar Fraction Three Quarters
//
//             84218
//            -------   
    0xC8, //  |**  *|
    0xD0, //  |** * |
    0xE0, //  |***  |
    0x48, //  | *  *|
    0x98, //  |*  **|
    0x38, //  |  ***|
    0x08, //  |    *|
//            -------   
//
//Character 0xBF - Inverted Question Mark
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x00, //  |     |
    0x20, //  |  *  |
    0x40, //  | *   |
    0x80, //  |*    |
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xC0 - Latin Capital Letter A With Grave
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x10, //  |   * |
    0x20, //  |  *  |
    0x50, //  | * * |
    0x88, //  |*   *|
    0xF8, //  |*****|
    0x88, //  |*   *|
//            -------   
//
//Character 0xC1 - Latin Capital Letter A With Acute
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x40, //  | *   |
    0x20, //  |  *  |
    0x50, //  | * * |
    0x88, //  |*   *|
    0xF8, //  |*****|
    0x88, //  |*   *|
//            -------   
//
//Character 0xC2 - Latin Capital Letter A With Cirumflex
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x50, //  | * * |
    0x20, //  |  *  |
    0x50, //  | * * |
    0x88, //  |*   *|
    0xF8, //  |*****|
    0x88, //  |*   *|
//            -------   
//
//Character 0xC3 - Latin Capital Letter A With Tilde
//
//             84218
//            -------   
    0x28, //  |  * *|
    0x50, //  | * * |
    0x20, //  |  *  |
    0x50, //  | * * |
    0x88, //  |*   *|
    0xF8, //  |*****|
    0x88, //  |*   *|
//            -------   
//
//Character 0xC4 - Latin Capital Letter A With Diaeresis
//
//             84218
//            -------   
    0x88, //  |*   *|
    0x20, //  |  *  |
    0x50, //  | * * |
    0x88, //  |*   *|
    0xF8, //  |*****|
    0x88, //  |*   *|
    0x88, //  |*   *|
//            -------   
//
//Character 0xC5 - Latin Capital Letter A With Ring Above
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x50, //  | * * |
    0x20, //  |  *  |
    0x50, //  | * * |
    0x88, //  |*   *|
    0xF8, //  |*****|
    0x88, //  |*   *|
//            -------   
//
//Character 0xC6 - Latin Capital Letter Ae
//
//             84218
//            -------   
    0x78, //  | ****|
    0xA0, //  |* *  |
    0xA0, //  |* *  |
    0xB8, //  |* ***|
    0xE0, //  |***  |
    0xA0, //  |* *  |
    0xB8, //  |* ***|
//            -------   
//
//Character 0xC7 - Latin Capital Letter C With Cedilla
//
//             84218
//            -------   
    0x70, //  | *** |
    0x88, //  |*   *|
    0x80, //  |*    |
    0x80, //  |*    |
    0xA8, //  |* * *|
    0x70, //  | *** |
    0x20, //  |  *  |
//            -------   
//
//Character 0xC8 - Latin Capital Letter E With Grave
//
//             84218
//            -------   
    0x40, //  | *   |
    0x20, //  |  *  |
    0xF8, //  |*****|
    0x80, //  |*    |
    0xF0, //  |**** |
    0x80, //  |*    |
    0xF8, //  |*****|
//            -------   
//
//Character 0xC9 - Latin Capital Letter E With Acute
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0xF8, //  |*****|
    0x80, //  |*    |
    0xF0, //  |**** |
    0x80, //  |*    |
    0xF8, //  |*****|
//            -------   
//
//Character 0xCA Latin Capital Letter E With Circumflex
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x50, //  | * * |
    0xF8, //  |*****|
    0x80, //  |*    |
    0xF0, //  |**** |
    0x80, //  |*    |
    0xF8, //  |*****|
//            -------   
//
//Character 0xCB - Latin Capital Letter E With Diaeresis
//
//             84218
//            -------   
    0x50, //  | * * |
    0x00, //  |     |
    0xF8, //  |*****|
    0x80, //  |*    |
    0xF0, //  |**** |
    0x80, //  |*    |
    0xF8, //  |*****|
//            -------   
//
//Character 0xCC - Latin Capital Letter I With Grave
//
//             84218
//            -------   
    0x40, //  | *   |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x70, //  | *** |
//            -------   
//
//Character 0xCD - latin Capital Letter I With Acute
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x70, //  | *** |
//            -------   
//
//Character 0xCE - Latin Capital Letter I With Circumflex
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x50, //  | * * |
    0x00, //  |     |
    0x70, //  | *** |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x70, //  | *** |
//            -------   
//
//Character 0xCF - Latin Capital Letter I With Diaeresis
//
//             84218
//            -------   
    0x50, //  | * * |
    0x00, //  |     |
    0x70, //  | *** |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x70, //  | *** |
//            -------   
//
//Character 0xD0 - Latin Capital Letter Eth
//
//             84218
//            -------   
    0x60, //  | **  |
    0x50, //  | * * |
    0x48, //  | *  *|
    0xE8, //  |*** *|
    0x48, //  | *  *|
    0x50, //  | * * |
    0x60, //  | **  |
//            -------   
//
//Character 0xD1 - Latin Capital Letter N With Tilde
//
//             84218
//            -------   
    0x28, //  |  * *|
    0x50, //  | * * |
    0x88, //  |*   *|
    0xC8, //  |**  *|
    0xA8, //  |* * *|
    0x98, //  |*  **|
    0x88, //  |*   *|
//            -------   
//
//Character 0xD2 - Latint Capital Letter O With Grave
//
//             84218
//            -------   
    0x40, //  | *   |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xD3 - Latin Capital Letter O With Acute
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xD4 - Latin Capital Letter O With Circumflex
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x50, //  | * * |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xD5 - Latin Capital Letter O With Tilde
//
//             84218
//            -------   
    0x28, //  |  * *|
    0x50, //  | * * |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xD6 - Latin Capital Letter O With Diaeresis
//
//             84218
//            -------   
    0x50, //  | * * |
    0x00, //  |     |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xD7 - Mutiplication Sign
//
//             84218
//            -------   
    0x00, //  |     |
    0x88, //  |*   *|
    0x50, //  | * * |
    0x20, //  |  *  |
    0x50, //  | * * |
    0x88, //  |*   *|
    0x00, //  |     |
//            -------   
//
//Character 0xD8 - Latin Capital Letter O With Stroke
//
//             84218
//            -------   
    0x70, //  | *** |
    0x98, //  |*  **|
    0xA8, //  |* * *|
    0xA8, //  |* * *|
    0xA8, //  |* * *|
    0xC8, //  |**  *|
    0x70, //  | *** |
//            -------   
//
//Character 0xD9 - Latin Capital Letter U With Grave
//
//             84218
//            -------   
    0x40, //  | *   |
    0x20, //  |  *  |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xDA - Latin Capital Letter U With 	Acute
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xDB - Latin Capital Letter U With Circumflex
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x50, //  | * * |
    0x00, //  |     |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xDC - Latin Capital Letter U With Diaeresis
//
//             84218
//            -------   
    0x50, //  | * * |
    0x00, //  |     |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xDD - Latin Capital Letter Y With Acute
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0x88, //  |*   *|
    0x50, //  | * * |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
//            -------   
//
//Character 0xDE - Latin Capital Letter Thorn
//
//             84218
//            -------   
    0x80, //  |*    |
    0xE0, //  |***  |
    0x90, //  |*  * |
    0x88, //  |*   *|
    0x90, //  |*  * |
    0xE0, //  |***  |
    0x80, //  |*    |
//            -------   
//
//Character 0xDF - Latin Small Letter Sharp S
//
//             84218
//            -------   
    0x60, //  | **  |
    0x90, //  |*  * |
    0xA0, //  |* *  |
    0x90, //  |*  * |
    0x88, //  |*   *|
    0x98, //  |*  **|
    0x80, //  |*    |
//            -------   
//
//Character 0xE0 - Latin Small Letter A With Grave
//
//             84218
//            -------   
    0x40, //  | *   |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x08, //  |    *|
    0x78, //  | ****|
    0x88, //  |*   *|
    0x78, //  | ****|
//            -------   
//
//Character 0xE1 - Latin Small Letter A With Acute
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x08, //  |    *|
    0x78, //  | ****|
    0x88, //  |*   *|
    0x78, //  | ****|
//            -------   
//
//Character 0xE2 - Latin Small Letter A With Circumflex
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x08, //  |    *|
    0x78, //  | ****|
    0x88, //  |*   *|
    0x78, //  | ****|
//            -------   
//
//Character 0xE3 - Latin Small Letter A With Tilde
//
//             84218
//            -------   
    0x50, //  | * * |
    0x90, //  |* *  |
    0x70, //  | *** |
    0x08, //  |    *|
    0x78, //  | ****|
    0x88, //  |*   *|
    0x78, //  | ****|
//            -------   
//
//Character 0xE4 - Latin Small Letter A With Diaeresis
//
//             84218
//            -------   
    0x50, //  | * * |
    0x00, //  |     |
    0x70, //  | *** |
    0x08, //  |    *|
    0x78, //  | ****|
    0x88, //  |*   *|
    0x78, //  | ****|
//            -------   
//
//Character 0xE5 - Latin Small Letter A With Ring Above
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x00, //  |     |
    0x70, //  | *** |
    0x08, //  |    *|
    0x78, //  | ****|
    0x88, //  |*   *|
    0x78, //  | ****|
//            -------   
//
//Character 0xE6 - Latin Small Letter Ae
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0xD8, //  |** **|
    0x28, //  |  * *|
    0xF8, //  |*****|
    0xA0, //  |* *  |
    0xD8, //  |** **|
//            -------   
//
//Character 0xE7 - Latin Small Letter C With Cedilla
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x78, //  | ****|
    0x80, //  |*    |
    0x80, //  |*    |
    0x78, //  | ****|
    0x60, //  | **  |
//            -------   
//
//Character 0xE8 - Latin Small Letter E With Grave
//
//             84218
//            -------   
    0x40, //  | *   |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x88, //  |*   *|
    0xF8, //  |*****|
    0x80, //  |*    |
    0x70, //  | *** |
//            -------   
//
//Character 0xE9 - Latin Small Letter E With Acute
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x88, //  |*   *|
    0xF8, //  |*****|
    0x80, //  |*    |
    0x70, //  | *** |
//            -------   
//
//Character 0xEA - Latin Small Letter E With Circumflex
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x50, //  | * * |
    0x70, //  | *** |
    0x88, //  |*   *|
    0xF8, //  |*****|
    0x80, //  |*    |
    0x70, //  | *** |
//            -------   
//
//Character 0xEB - Latin Small Letter E With Diaeresis
//
//             84218
//            -------   
    0x50, //  | * * |
    0x00, //  |     |
    0x70, //  | *** |
    0x88, //  |*   *|
    0xF8, //  |*****|
    0x80, //  |*    |
    0x70, //  | *** |
//            -------   
//
//Character 0xEC - Latin Small Letter I With Grave
//
//             84218
//            -------   
    0x40, //  | *   |
    0x20, //  |  *  |
    0x00, //  |     |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
//            -------   
//
//Character 0xED - Latin Small Letter I With Acute
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0x00, //  |     |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
//            -------   
//
//Character 0xEE - Latin Small Letter I With Circumflex
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x50, //  | * * |
    0x00, //  |     |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
//            -------   
//
//Character 0xEF - Latin Small Letter I With Diaeresis
//
//             84218
//            -------   
    0x00, //  |     |
    0x50, //  | * * |
    0x00, //  |     |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
    0x20, //  |  *  |
//            -------   
//
//Character 0xF0 - Latin Small Letter Eth
//
//             84218
//            -------   
    0x50, //  | * * |
    0x20, //  |  *  |
    0x50, //  | * * |
    0x08, //  |    *|
    0x78, //  | ****|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xF1 - Latin Small Letter N With Tilde
//
//             84218
//            -------   
    0x28, //  |  * *|
    0x50, //  | * * |
    0x00, //  |     |
    0xB0, //  |* ** |
    0xC8, //  |**  *|
    0x88, //  |*   *|
    0x88, //  |*   *|
//            -------   
//
//Character 0xF2 - Latin Small Letter O With Grave
//
//             84218
//            -------   
    0x40, //  | *   |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xF3 - Latin Small Letter O With Acute
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xF4 - Latin Small Letter O With Circumflex
//
//             84218
//            -------   
    0x20, //  |  *  |
    0x50, //  | * * |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xF5 - Latin Small Letter O With Tilde
//
//             84218
//            -------   
    0x28, //  |  * *|
    0x50, //  | * * |
    0x00, //  |     |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xF6 - Latin Small Letter O With Diaeresis
//
//             84218
//            -------   
    0x50, //  | * * |
    0x00, //  |     |
    0x70, //  | *** |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x70, //  | *** |
//            -------   
//
//Character 0xF7 - Division Sign
//
//             84218
//            -------   
    0x00, //  |     |
    0x20, //  |  *  |
    0x00, //  |     |
    0x70, //  | *** |
    0x00, //  |     |
    0x20, //  |  *  |
    0x00, //  |     |
//            -------   
//
//Character 0xF8 - Latin Small Letter O With Stroke
//
//             84218
//            -------   
    0x00, //  |     |
    0x00, //  |     |
    0x70, //  | *** |
    0x98, //  |*  **|
    0xA8, //  |* * *|
    0xC8, //  |**  *|
    0x70, //  | *** |
//            -------   
//
//Character 0xF9 - Latin Small Letter U With Grave
//
//             84218
//            -------   
    0x40, //  | *   |
    0x20, //  |  *  |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x98, //  |*  **|
    0x68, //  | ** *|
//            -------   
//
//Character 0xFA - Latin Small Letter U With Acute
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x98, //  |*  **|
    0x68, //  | ** *|
//            -------   
//
//Character 0xFB - Latin Small Letter U With Circumflex
//             84218
//            -------   
    0x20, //  |  *  |
    0x50, //  | * * |
    0x00, //  |     |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x98, //  |*  **|
    0x68, //  | ** *|
//            -------   
//
//Character 0xFC - Latin Small Letter U With Diaeresis
//
//             84218
//            -------   
    0x50, //  | * * |
    0x00, //  |     |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x98, //  |*  **|
    0x68, //  | ** *|
//            -------   
//
//Character 0xFD - Latin Small Letter Y With Acute
//
//             84218
//            -------   
    0x10, //  |   * |
    0x20, //  |  *  |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x78, //  | ****|
    0x08, //  |    *|
    0xF0, //  |**** |
//            -------   
//
//Character 0xFE - Latin Small Letter Thorn
//
//             84218
//            -------   
    0x80, //  |*    |
    0x80, //  |*    |
    0xE0, //  |***  |
    0x90, //  |*  * |
    0xE0, //  |***  |
    0x80, //  |*    |
    0x80, //  |*    |
//            -------   
//
//Character 0xFF
//
//             84218
//            -------   
    0x50, //  | * * |
    0x00, //  |     |
    0x88, //  |*   *|
    0x88, //  |*   *|
    0x78, //  | ****|
    0x08, //  |    *|
    0xF0, //  |**** |
//            -------   
//
};
