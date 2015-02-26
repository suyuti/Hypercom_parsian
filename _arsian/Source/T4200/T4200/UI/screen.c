//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      screen.c
//
// File Description:
//      SDL screen scripts and screen ID table
//
//=============================================================================

#include "basictyp.h"
#include "fixdata.h"
#include "sdk.h"
#include "sdkmem.h"
#include "sdkgui.h"

#include "struct.h"
#include "appdata.h"
#include "screen.h"
#include "sprite.h"

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
#define NAV_BUTTON_TYPE BUTTON_NOBORDER

//! Screen Script
//! This Screen is for Setting a 5 button menu screen, all buttons optional
//!	Long  inverted Text area in title bar to display contents of MSG_USER11
//!	Buttons are controled by Enable Flags.
//!	Button 1 - Displays contents of MSG_USER1  - Returns EVENT_USER1 when pressed
//!	Button 2 - Displays contents of MSG_USER2  - Returns EVENT_USER2 when pressed
//!	Button 3 - Displays contents of MSG_USER3  - Returns EVENT_USER3 when pressed
//!	Button 4 - Displays contents of MSG_USER4  - Returns EVENT_USER4 when pressed
//!	Button 4 - Displays contents of MSG_USER5  - Returns EVENT_USER5 when pressed
//!	Enable Flag 0 ON, Displays NEXT Arrow in Title Bar
//!	Enable Flag 1 ON, Display Box 1
//!	Enable Flag 2 ON, Display Box 2
//!	Enable Flag 3 ON, Display Box 3
//!	Enable Flag 4 ON, Display Box 4
//!	Enable Flag 5 ON, Display Box 5
//!	Enable Flag 11 ON, Displays PREVIOUS Arrow in Title Bar

static const UWORD menu5BtnTextScr[] = {
	STARTSCREEN, MENU_5BTNTEXT_SCR,
	CLEAR_ALL,

    // Use short title if Enable flag 0 or 11
    // Set enable flag 12 if flag 11 or 0 set
    SET_ENABLE_FLAG, 12, 0,
    ENABLE_FLAG, 0, SET_ENABLE_FLAG, 12, 1,
    ENABLE_FLAG, 11, SET_ENABLE_FLAG, 12, 1,

	// Title, long 
    ENABLE_FLAG_NOT, 12, 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, MSG_USER11,
    ENABLE_FLAG_NOT, 12, 
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

    // or Title short
    ENABLE_FLAG, 12,
	TEXTJ, JCENTER, VCENTER, TITLE_S_X, TITLE_Y, TITLE_S_W, TITLE_H, MSG_USER11,
    ENABLE_FLAG, 12,
	INVERT_AREA, TITLE_S_X, TITLE_Y, TITLE_S_W, TITLE_H,

	// Title Bar returns CANCEL_KY when pressed
	BUTTON_TRANSPARENT, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, BUTTON_UP,
	BUTTON_NOBORDER, CANCEL_KY,


    // Previous Button
    ENABLE_FLAG, 7,
    BUTTON_IMAGE, IMAGE_BACKARROW, BACK_X, BACK_Y, BACK_W, BACK_H, BUTTON_UP,
    BUTTON_NOBORDER, UP_KY,

    // Next Button
    ENABLE_FLAG, 0,
    BUTTON_IMAGE, IMAGE_FORWARDARROW, NEXT_X, NEXT_Y, NEXT_W, NEXT_H,
    BUTTON_UP, BUTTON_NOBORDER, DOWN_KY,


	// Optional Previous image
#if defined(MAKE_OPTIMUM) && defined(MAKE_M2)
    ENABLE_FLAG, 11,
#endif
    BUTTON_IMAGE, IMAGE_UPARROW, Bn_B1_X, Bn_B1_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, UP_KY,

#if defined(MAKE_OPTIMUM) && !defined(MAKE_M2)
    // Select image
    BUTTON_IMAGE, IMAGE_SELECT, Bn_B2_X, Bn_B2_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, SELECT_KY,
#endif

	// Optional Next image
#if defined(MAKE_OPTIMUM) && defined(MAKE_M2)
	ENABLE_FLAG, 0,
#endif
    BUTTON_IMAGE, IMAGE_DOWNARROW, Bn_B3_X, Bn_B3_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, DOWN_KY,

	// Menu items
	// 5 Buttons text
	LANG_FONT, DEFAULT_FONT_SIZE,
	ENABLE_FLAG, 1,
	BUTTON_TEXT, MSG_USER1, Bv_B1_X, Bv_B1_Y, Bv_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER1,
	ENABLE_FLAG, 2,
	BUTTON_TEXT, MSG_USER2, Bv_B2_X, Bv_B2_Y, Bv_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER2,
	ENABLE_FLAG, 3,
	BUTTON_TEXT, MSG_USER3, Bv_B3_X, Bv_B3_Y, Bv_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER3,
	ENABLE_FLAG, 4,
	BUTTON_TEXT, MSG_USER4, Bv_B4_X, Bv_B4_Y, Bv_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER4,
	ENABLE_FLAG, 5,
	BUTTON_TEXT, MSG_USER5, Bv_B5_X, Bv_B5_Y, Bv_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER5,

	DISPLAY_ALL,
	ENDSCREEN
};

//! Screen Script
//! This Screen is for Setting a 10 button menu screen, all buttons optional
//!	Long  inverted Text area in title bar to display contents of MSG_USER11
//!	Buttons are controled by Enable Flags.
//!	Button 1 - Displays contents of MSG_USER1  - Returns EVENT_USER1 when pressed
//!	Button 2 - Displays contents of MSG_USER2  - Returns EVENT_USER2 when pressed
//!	Button 3 - Displays contents of MSG_USER3  - Returns EVENT_USER3 when pressed
//!	Button 4 - Displays contents of MSG_USER4  - Returns EVENT_USER4 when pressed
//!	Button 5 - Displays contents of MSG_USER5  - Returns EVENT_USER5 when pressed
//!	Button 6 - Displays contents of MSG_USER6  - Returns EVENT_USER6 when pressed
//!	Button 7 - Displays contents of MSG_USER7  - Returns EVENT_USER7 when pressed
//!	Button 8 - Displays contents of MSG_USER8  - Returns EVENT_USER8 when pressed
//!	Button 9 - Displays contents of MSG_USER9  - Returns EVENT_USER9 when pressed
//!	Button 10- Displays contents of MSG_USER10 - Returns EVENT_USER10 when pressed
//!	Enable Flag 0 ON, Displays NEXT Arrow in Title Bar
//!	Enable Flag 11 ON, Displays PREVIOUS Arrow in Title Bar
//!	Enable Flag 1 ON, Display Box 1
//!	Enable Flag 2 ON, Display Box 2
//!	Enable Flag 3 ON, Display Box 3
//!	Enable Flag 4 ON, Display Box 4
//!	Enable Flag 5 ON, Display Box 5
//!	Enable Flag 6 ON, Display Box 6
//!	Enable Flag 7 ON, Display Box 7
//!	Enable Flag 8 ON, Display Box 8
//!	Enable Flag 9 ON, Display Box 9
//!	Enable Flag 10 ON, Display Box 10

static const UWORD menu10BtnTextScr[] = {

	STARTSCREEN, MENU_10BTNTEXT_SCR,
	CLEAR_ALL,

    // Use short title if Enable flag 0 or 11
    // Set enable flag 12 if flag 11 or 0 set
    SET_ENABLE_FLAG, 12, 0,
    ENABLE_FLAG, 0, SET_ENABLE_FLAG, 12, 1,
    ENABLE_FLAG, 11, SET_ENABLE_FLAG, 12, 1,

	// Title long
    ENABLE_FLAG_NOT, 12, 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, MSG_USER11,
    ENABLE_FLAG_NOT, 12, 
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

    // or Title short
    ENABLE_FLAG, 12,
	TEXTJ, JCENTER, VCENTER, TITLE_S_X, TITLE_Y, TITLE_S_W, TITLE_H, MSG_USER11,
    ENABLE_FLAG, 12,
	INVERT_AREA, TITLE_S_X, TITLE_Y, TITLE_S_W, TITLE_H,

	// Optional Previous image
#if defined(MAKE_OPTIMUM) && defined(MAKE_M2)
	ENABLE_FLAG, 11,
#endif
    BUTTON_IMAGE, IMAGE_UPARROW, Bn_B1_X, Bn_B1_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, UP_KY,

#if defined(MAKE_OPTIMUM) && !defined(MAKE_M2)
    // Select image
    BUTTON_IMAGE, IMAGE_SELECT, Bn_B2_X, Bn_B2_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, SELECT_KY,
#endif

	// Optional Next image
#if defined(MAKE_OPTIMUM) && defined(MAKE_M2)
	ENABLE_FLAG, 0,
#endif
    BUTTON_IMAGE, IMAGE_DOWNARROW, Bn_B3_X, Bn_B3_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, DOWN_KY,

	// Menu items
	// 10 Buttons text
	LANG_FONT, DEFAULT_FONT_SIZE,
	ENABLE_FLAG, 1,
	BUTTON_TEXT, MSG_USER1, B10_X_LEFT, Bv_B1_Y, B10_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER1,
	ENABLE_FLAG, 2,
	BUTTON_TEXT, MSG_USER2, B10_X_LEFT, Bv_B2_Y, B10_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER2,
	ENABLE_FLAG, 3,
	BUTTON_TEXT, MSG_USER3, B10_X_LEFT, Bv_B3_Y, B10_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER3,
	ENABLE_FLAG, 4,
	BUTTON_TEXT, MSG_USER4, B10_X_LEFT, Bv_B4_Y, B10_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER4,
	ENABLE_FLAG, 5,
	BUTTON_TEXT, MSG_USER5, B10_X_LEFT, Bv_B5_Y, B10_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER5,
	ENABLE_FLAG, 6,
	BUTTON_TEXT, MSG_USER6, B10_X_RIGHT, Bv_B1_Y, B10_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER6,
	ENABLE_FLAG, 7,
	BUTTON_TEXT, MSG_USER7, B10_X_RIGHT, Bv_B2_Y, B10_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER7,
	ENABLE_FLAG, 8,
	BUTTON_TEXT, MSG_USER8, B10_X_RIGHT, Bv_B3_Y, B10_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER8,
	ENABLE_FLAG, 9,
	BUTTON_TEXT, MSG_USER9,  B10_X_RIGHT, Bv_B4_Y, B10_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER9,
	ENABLE_FLAG, 10,
	BUTTON_TEXT, MSG_USER10, B10_X_RIGHT, Bv_B5_Y, B10_W, Bv_H, BUTTON_UP,
	BUTTON_INVERT,EVENT_USER10,

	DISPLAY_ALL,
	ENDSCREEN
};

//! Screen Script
//! This Screen is displayed while the terminal is at Idle State
//!	Title - Alternates between Menu message and date/time message, no cancel button
//!	Four one line text buttons, set up when terminal is initialized
//!	Button 1 - Displays contents of MSG_ICEBUTTON1_L1  - Returns EVENT_ICEBUTTON1
//!	Button 2 - Displays contents of MSG_ICEBUTTON2_L1  - Returns EVENT_ICEBUTTON2
//!	Button 3 - Displays contents of MSG_ICEBUTTON3_L1  - Returns EVENT_ICEBUTTON3
//!	Button 4 - Displays contents of MSG_ICEBUTTON4_L1  - Returns EVENT_ICEBUTTON4
//!	Button 5 - Displays "MAIN MENU"  - Returns EVENT MENU_MAIN
//!	Enable Flag 2..9 ON, Display GSM signal scale
//!	Enable Flag 10..15 ON, Display Battary charge
//!	Swipe card sprite enable
//!	Alternating menu sprite enable

static const UWORD idleScreen[] = {

	STARTSCREEN, IDLE_SCR,
	CLEAR_ALL,

	// Swipe_card sprite
	SPRITE, SPRITE_SWIPE_CARD,

	// Title sprite
	SPRITE, SPRITE_ALTERNATING_MENU,

#if defined(MAKE_OPTIMUM) && !defined(MAKE_M2)
    // Previous image
    BUTTON_IMAGE, IMAGE_UPARROW, Bn_B1_X, Bn_B1_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, UP_KY,

    // Select image
    BUTTON_IMAGE, IMAGE_SELECT, Bn_B2_X, Bn_B2_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, SELECT_KY,

	// Next image
    BUTTON_IMAGE, IMAGE_DOWNARROW, Bn_B3_X, Bn_B3_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, DOWN_KY,
#endif

    // Idle screen buttons
	LANG_FONT, DEFAULT_FONT_SIZE,
	
    BUTTON_TEXT, MSG_ICEBUTTON1_L1, Bv_B1_X, Bv_B1_Y, Bv_W, Bv_H, 
	BUTTON_UP, BUTTON_INVERT, EVENT_ICEBUTTON1,

	BUTTON_TEXT, MSG_ICEBUTTON2_L1, Bv_B2_X, Bv_B2_Y, Bv_W, Bv_H, 
	BUTTON_UP, BUTTON_INVERT, EVENT_ICEBUTTON2,
	
    BUTTON_TEXT, MSG_ICEBUTTON3_L1, Bv_B3_X, Bv_B3_Y, Bv_W, Bv_H, 
	BUTTON_UP, BUTTON_INVERT, EVENT_ICEBUTTON3,
	
    BUTTON_TEXT, MSG_ICEBUTTON4_L1, Bv_B4_X, Bv_B4_Y, Bv_W, Bv_H, 
	BUTTON_UP, BUTTON_INVERT, EVENT_ICEBUTTON4,
	
    BUTTON_TEXT, N_Main,            Bv_B5_X, Bv_B5_Y, Bv_W, Bv_H, 
	BUTTON_UP, BUTTON_INVERT, MENU_MAIN,

	LANG_FONT, DEFAULT_FONT_SIZE,
	DISPLAY_ALL,
	ENDSCREEN
};

//! Screen Script
//! This Screen is used to prompt the user to Initialize or configure the terminal
//!	Title - Alternates between Menu message and date/time message, no cancel button
//!	Currenlty two buttons, set up for more to be added
//!	Three button screen, button 1 has no border, looks like a text message
//!	Button 1 - Contents of CSTMSG  - Returns '0' when pressed
//!	Button 2 - N_Configure message  - Returns SETUP_FUN when pressed
//!	Button 3 - N_Initialize message  - Returns INITIALIZE when pressed
//!	No Enable Flags 
//!	Alternating menu sprite enable

static const UWORD initScreen[] = {

	STARTSCREEN, INIT_SCR,
	CLEAR_ALL,

    // Title bar sprite
	SPRITE, SPRITE_ALTERNATING_MENU,

    BUTTON_TEXT, N_Configure, Bv_B1_X, Bv_B1_Y, Bv_W, Bv_H, 
	BUTTON_UP, BUTTON_INVERT, SETUP_FUN,

    BUTTON_TEXT, N_Comms, Bv_B2_X, Bv_B2_Y, Bv_W, Bv_H, 
	BUTTON_UP, BUTTON_INVERT, COMM_FUN,

	BUTTON_TEXT, InitTran, Bv_B3_X, Bv_B3_Y, Bv_W, Bv_H, 
	BUTTON_UP, BUTTON_INVERT, INITIALIZE,
	
    BUTTON_TEXT, N_Main, Bv_B4_X, Bv_B4_Y, Bv_W, Bv_H, 
	BUTTON_UP, BUTTON_INVERT, MENU_MAIN,

	DISPLAY_ALL,
	ENDSCREEN
};

//! Screen Scripts
//! This Screen is used to set the terminals display contrast level
//!	Title - Fixed to display "N_Contrast, with Cancel Button
//!	Three text buttons
//!	Button 1 - Display N_Lower  - Returns 'L' when pressed
//!	Button 2 - Display N_Higher - Returns 'H' when pressed
//!	Button 3 - Display N_Quit   - Returns CANCEL_KY when pressed
//!	No Enable Flags
//!	No sprites

static const UWORD contrastScreen[] = {

	STARTSCREEN, SETCONTRAST_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, N_Contrast,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// 3 Buttons
	BUTTON_TEXT, N_Lower, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'L',

	BUTTON_TEXT, N_Quit, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	BUTTON_TEXT, N_Higher, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'H',

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is a template for 2 button Screens
//! Title - Contents of MSG_USER7, with Cancel Button
//! Two text lines and Two Buttons 
//! Text Line 1 - MSG_USER1 
//! Text Line 2 - MSG_USER2
//! Button 1 - Display MSG_USER3  - Returns EVENT_USER3 when pressed
//! Button 2 - Display MSG_USER4  - Returns EVENT_USER4 when pressed
//! No Enable Flags 
//! No sprites
static const UWORD template2BtnQuery[] = {

	// one big button on top, two lower smaller ones titlebar
	STARTSCREEN, TEMPLATE_2BTNQUERY_SCR,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, MSG_USER7,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Text Box full window below title bar
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Text lines
	TEXTL, 0, 0, MSG_USER1,		// Text, Line 1
	TEXTL, 1, 0, MSG_USER2,		// Text, Line 2

	//Restart Full Window
	DEFAULT_WINDOW,

	// 2 Buttons
	BUTTON_TEXT, MSG_USER3, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, EVENT_USER3,

	BUTTON_TEXT, MSG_USER4, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, EVENT_USER4,

	ENDSCREEN
};

//! This Screen is for a screen with two buttons YES and NO
//! Title - Contents of MSG_USER7, with Cancel Button ( set in template )
//! Two text lines and Two Buttons 
//! Text Line 1 - MSG_USER1
//! Text Line 2 - MSG_USER2
//! Button 1 - Display N_NO   - Returns CLEAR_KY when pressed
//! Button 2 - Display N_YES  - Returns ENTER_KY when pressed
//! No Enable Flags 
//! No sprites
static const UWORD yesnoScreen[] = {

	STARTSCREEN, YESNO_SCR,
	CLEAR_ALL,

	// Set up text and event for 2 button template
	SET_USER_MSG, 3, N_NO,		// Button 1 text 
	SET_USER_MSG, 4, N_YES,		// Button 2 text 
	SET_USER_EVENT, 3, CLEAR_KY,	// Button 1 event 
	SET_USER_EVENT, 4, ENTER_KY,	// Button 2 event 

	// Use the 2 button template
	NEWSCREEN, TEMPLATE_2BTNQUERY_SCR,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for a screen with two buttons YES and NO
//! Title - Contents of MSG_USER7, with Cancel Button ( set in template )
//! Two text lines and Two Buttons 
//! Text Line 1 - MSG_USER1
//! Text Line 2 - MSG_USER2
//! Button 1 - Display N_NEXT - Returns CLEAR_KY when pressed
//! Button 2 - Display N_YES  - Returns ENTER_KY when pressed
//! No Enable Flags 
static const UWORD YesNextScr[] = {

	STARTSCREEN, YES_NEXT_SCR,
	CLEAR_ALL,

	// Set up text and event for 2 button template
	SET_USER_MSG, 3, N_YES,		// Button 1 text
	SET_USER_MSG, 4, N_NEXT,	// Button 2 text
	SET_USER_EVENT, 3, ENTER_KY,	// Button 1 event
	SET_USER_EVENT, 4, CLEAR_KY,	// Button 2 event

	// Use the 2 button template
	NEWSCREEN, TEMPLATE_2BTNQUERY_SCR,
	DISPLAY_ALL,
	ENDSCREEN
};


//! This Screen is for displaying a status value or state 
//! Title - Contents of MSG_DEFAULT_TITLE, with Cancel Button
//! Single Line of text , MSG_USER1, left justifed
//! No Buttons 
//! No Enable Flags 
//! No sprites
static const UWORD statusScr[] = {

	STARTSCREEN, STATUS_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	MSG_DEFAULT_TITLE,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Text Line, Left Justified
	TEXTJ, JLEFT, VCENTER, 0, 20, 128, 20, MSG_USER1,		

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is to notify user to use the card reader
//! Title - Contents of MSG_DEFAULT_TITLE, with Cancel Button
//! Single Line of text , MSG_USER1, left justifed
//! No Buttons
//! No Enable Flags
//! Swipe card sprite enable
static const UWORD cardswipeScr[] = {

	STARTSCREEN, CARDSWIPE_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	MSG_DEFAULT_TITLE,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Text Line, Left Justifed
	TEXTJ, JLEFT, VCENTER, 0, 20, 128, 20, MSG_USER1, 

	// Enable Card Swipe
	SPRITE, SPRITE_SWIPE_CARD,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for Numeric Keyboard/PAD Entry
//! Input Box drawn to according to maximum input size
//! Two text lines and three edit buttons
//! Title bar - Fixed to display contents of MSG_USER7, Length variable
//! but the with of the bar , and additional buttons are controled by Enable Flags.
//! Input Box - Drawn to size of maximum input value
//! Text Line 1 -  MSG_USER1
//! Text Line 2 -  MSG_USER2
//! Button 1 - Draw Back Arrow Image  - Returns BSP_KY when pressed
//! Button 2 - Display N_Clear  - Returns CLEAR_KY when pressed
//! Button 3 - Display N_Enter  - Returns ENTER_KY when pressed
//! Enable Flag 0 ON, Displays Long inverted text title bar, No Cancel Button
//! Enable Flag 1 ON, Displays Short inverted text title bar, Cancel Button  
//! No sprites
static const UWORD inputScr[] = {

	STARTSCREEN, NUMENTRY_SCR,
	CLEAR_ALL,

	// Long Title used if Enable_flags[0] = non-zero
	ENABLE_FLAG, 0,
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H, MSG_USER7,
	ENABLE_FLAG, 0,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,

	// Short Title used if Enable_flags[1] = non-zero
	ENABLE_FLAG, 1,
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, MSG_USER7,
	ENABLE_FLAG, 1,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	// Title Bar returns CANCEL_KY when pressed
	ENABLE_FLAG, 1,
	BUTTON_TRANSPARENT, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, BUTTON_UP,
	BUTTON_NOBORDER, CANCEL_KY,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_SH,

	// Text Lines 
	TEXTL, 0, 0, MSG_USER1,		// Text Line 1 
	TEXTL, 1, 0, MSG_USER2,		// Text Line 2 

	//Restart Full Window
	DEFAULT_WINDOW,

	// Input Box
	INPUTBOX, IB_AUTO_BORDER, JRIGHT, INPUT_X, INPUT_Y, INPUT_W, INPUT_H, INPUT_S,

	//  Buttons
	BUTTON_IMAGE, IMAGE_BACKARROW, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, BSP_KY,

	BUTTON_TEXT, N_Enter, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, ENTER_KY,

	BUTTON_TEXT, N_Clear, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CLEAR_KY,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for Setting The Descriptor
//! Title - Fixed to display MSG_DEFAULT_TITLE, with Cancel Button
//! Text Line 1 - Display CSTMSG Buffer
//! Optional Text Line 2 - Display CSTMSG2 Buffer  
//! Button 1 - Draw Back Arrow Image  - Returns BSP_KY when pressed
//! Button 2 - Display N_Clear  - Returns CLEAR_KY when pressed
//! Button 3 - Display N_Enter  - Returns ENTER_KY when pressed
//! Enable Flag 3 ON, Displays second text line
//! No sprites
static const UWORD descriptorScr[] = {

	STARTSCREEN, DESCRIPTOR_SCR,
	CLEAR_ALL,

	// Title 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	MSG_DEFAULT_TITLE,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Title Bar returns CANCEL_KY when pressed
	BUTTON_TRANSPARENT, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, BUTTON_UP,
	BUTTON_NOBORDER, CANCEL_KY,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_SH,

	// Display Line/Lines of text
	TEXTL, 0, 0, CustomMsg,		// First Text Line
	ENABLE_FLAG, 3,
	TEXTL, 1, 0, CustomMsg2,	// Optional Second Text Line

	//Restart Full Window
	DEFAULT_WINDOW,

    // 3 buttons
  	BUTTON_IMAGE, IMAGE_BACKARROW, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, BSP_KY,

	BUTTON_TEXT, N_Enter, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, ENTER_KY,

	BUTTON_TEXT, N_Clear, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H,BUTTON_UP,
	BUTTON_SQUARE, CLEAR_KY,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for AlphaNumeric Keyboard Entry Mode 1 (NUMERIC)
//! The text line on the title MSG_USER7
//! The text line MSG_USER1 before Input Box activated
//! Input Box drawn to according to maximum input size
//! One invisible edit buttons:
//! Button 1 (SELECT)- Returns ABC_KY    when pressed
static const UWORD alphaInput1Scr[] = {

	STARTSCREEN, ALPHANUM1_SCR,
	CLEAR_ALL,

	// Long Title UserMsg
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H, MSG_USER7,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,

	// Current value Text Line 1
	TEXTP, 5, TITLE_H+1, MSG_USER1,

    // Current value Text Line 2
	TEXTP, 5, (TITLE_H+1)*2, MSG_USER2,	

	// Input Box with flashing current entry position
	INPUTBOX, IB_NO_BORDER, JRIGHT, INPUT_X, INPUTA_Y, INPUT_W, INPUTA_H, 0,
    SPRITE, SPRITE_FLASH,

    // Invisible SELECT button - change keypad layout
	BUTTON_TRANSPARENT, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_NOBORDER, ABC_KY,

	// Display NUMERIC keypad image
	DISPLAY_IMAGE, KEY_IMG_X, KEY_IMG_Y, IMAGE_KEYNUM,

#if defined(MAKE_OPTIMUM) && !defined(MAKE_M2)
    // Select button image
    BUTTON_IMAGE, IMAGE_SELECT, Bn_B2_X, Bn_B2_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, SELECT_KY,
#endif

    // Show it
	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for AlphaNumeric Keyboard Entry Mode 2 (CAPITAL ALPHABETIC)
//! The text line on the title MSG_USER7
//! The text line MSG_USER1 before Input Box activated
//! Input Box drawn to according to maximum input size
//! One invisible edit buttons:
//! Button 1 (SELECT)- Returns ABC_KY    when pressed
static const UWORD alphaInput2Scr[] = {

	STARTSCREEN, ALPHANUM2_SCR,
	CLEAR_ALL,

	// Long Title UserMsg
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H, MSG_USER7,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,

	// Current value Text Line 1
	TEXTP, 5, TITLE_H+1, MSG_USER1,

    // Current value Text Line 2
	TEXTP, 5, (TITLE_H+1)*2, MSG_USER2,	

	// Input Box with flashing current entry position
	INPUTBOX, IB_NO_BORDER, JRIGHT, INPUT_X, INPUTA_Y, INPUT_W, INPUTA_H, 0,
    SPRITE, SPRITE_FLASH,

    // Invisible SELECT button - change keypad layout
	BUTTON_TRANSPARENT, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_NOBORDER, ABC_KY,

	// Display NUMERIC keypad image
	DISPLAY_IMAGE, KEY_IMG_X, KEY_IMG_Y, IMAGE_KEYASC,

#if defined(MAKE_OPTIMUM) && !defined(MAKE_M2)
    // Select button image
    BUTTON_IMAGE, IMAGE_SELECT, Bn_B2_X, Bn_B2_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, SELECT_KY,
#endif

    // Show it
	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for AlphaNumeric Keyboard Entry Mode 2 (SMALL ALPHABETIC)
//! The text line on the title MSG_USER7
//! The text line MSG_USER1 before Input Box activated
//! Input Box drawn to according to maximum input size
//! One invisible edit buttons:
//! Button 1 (SELECT)- Returns ABC_KY    when pressed
static const UWORD alphaInput3Scr[] = {

	STARTSCREEN, ALPHANUM3_SCR,
	CLEAR_ALL,

	// Long Title UserMsg
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H, MSG_USER7,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,

	// Current value Text Line 1
	TEXTP, 5, TITLE_H+1, MSG_USER1,

    // Current value Text Line 2
	TEXTP, 5, (TITLE_H+1)*2, MSG_USER2,	

	// Input Box with flashing current entry position
	INPUTBOX, IB_NO_BORDER, JRIGHT, INPUT_X, INPUTA_Y, INPUT_W, INPUTA_H, 0,
    SPRITE, SPRITE_FLASH,

    // Invisible SELECT button - change keypad layout
	BUTTON_TRANSPARENT, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_NOBORDER, ABC_KY,

	// Display NUMERIC keypad image
	DISPLAY_IMAGE, KEY_IMG_X, KEY_IMG_Y, IMAGE_KEYASCSM,

#if defined(MAKE_OPTIMUM) && !defined(MAKE_M2)
    // Select button image
    BUTTON_IMAGE, IMAGE_SELECT, Bn_B2_X, Bn_B2_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, SELECT_KY,
#endif

    // Show it
	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for AlphaNumeric Keyboard Entry Mode 3 (SYMBOLS)
//! The text line on the title MSG_USER7
//! The text line MSG_USER1 before Input Box activated
//! Input Box drawn to according to maximum input size
//! One invisible edit buttons:
//! Button 1 (SELECT)- Returns ABC_KY    when pressed
static const UWORD alphaInput4Scr[] = {

	STARTSCREEN, ALPHANUM4_SCR,
	CLEAR_ALL,

	// Long Title UserMsg
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H, MSG_USER7,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,

	// Current value Text Line 1
	TEXTP, 5, TITLE_H+1, MSG_USER1,

    // Current value Text Line 2
	TEXTP, 5, (TITLE_H+1)*2, MSG_USER2,	

	// Input Box with flashing current entry position
	INPUTBOX, IB_NO_BORDER, JRIGHT, INPUT_X, INPUTA_Y, INPUT_W, INPUTA_H, 0,
    SPRITE, SPRITE_FLASH,

    // Invisible SELECT button - change keypad layout
	BUTTON_TRANSPARENT, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_NOBORDER, ABC_KY,

	// Display NUMERIC keypad image
	DISPLAY_IMAGE, KEY_IMG_X, KEY_IMG_Y, IMAGE_KEYSYM,

#if defined(MAKE_OPTIMUM) && !defined(MAKE_M2)
    // Select button image
    BUTTON_IMAGE, IMAGE_SELECT, Bn_B2_X, Bn_B2_Y, Bn_W, Bn_H, BUTTON_UP,
    NAV_BUTTON_TYPE, SELECT_KY,
#endif

    // Show it
	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for Amount Entry
//! The Screen defaults to $0.00 that must be placed in message buffer.
//! Title bar - Fixed to display contents of MSG_USER7 with a Cancel Button 
//! Input Box - Drawn to size of maximum input value
//! Text Line 1 -  MSG_USER1
//! Text Line 2 -  MSG_USER2
//! Input Box, with No Border - Set to Size of Max entry
//! Button 1 - Draw Back Arrow Image - Returns BSP_KY   when pressed
//! Button 2 - Display N_Clear       - Returns CLEAR_KY when pressed
//! Button 3 - Display N_Enter       - Returns ENTER_KY when pressed
//! No Enable Flags  
//! No sprites
static const UWORD amtentryScr[] = {

	STARTSCREEN, AMTENTRY_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, MSG_USER7,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Title Bar returns CANCEL_KY when pressed
	BUTTON_TRANSPARENT, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, BUTTON_UP,
	BUTTON_NOBORDER, CANCEL_KY,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_SH,

	// Text Lines 
	TEXTL, 0, 0, MSG_USER1,
	TEXTL, 1, 0, MSG_USER2,

	//Restart Full Window
	DEFAULT_WINDOW,

	// Input Box
	INPUTBOX, IB_AUTO_BORDER, JRIGHT, INPUT_X, INPUT_Y, INPUT_W, INPUT_H, INPUT_S,

	// 3 Buttons
	BUTTON_IMAGE, IMAGE_BACKARROW, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, BSP_KY,

	BUTTON_TEXT, N_Enter, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, CLEAR_KY,
	BUTTON_SQUARE, ENTER_KY,

	BUTTON_TEXT, N_Clear, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CLEAR_KY,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for a one button 'OK' screen
//! Title - MSG_DEFAULT_TITLE, with Cancel Button
//! Text Line 1 - MSG_USER1
//! Text Line 2 - MSG_USER2
//! Button 1 - Displays "OK" - Returns 'C' when pressed
//! No Enable Flags 
//! No sprites
static const UWORD continueScr[] = {

	STARTSCREEN, CONTINUE_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	MSG_DEFAULT_TITLE,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Title Bar returns CANCEL_KY when pressed
	BUTTON_TRANSPARENT, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, BUTTON_UP,
	BUTTON_NOBORDER, CANCEL_KY,

	// Set Text Window area
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Text Lines
	TEXTL, 0, 0, MSG_USER1,		// Text Line 1
	TEXTL, 1, 0, MSG_USER2,		// Text Line 2

	//Restart Full Window
	DEFAULT_WINDOW,

	// Button
	BUTTON_TEXT, N_OK, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, (UWORD) 'C',

	// Set Text Window
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_SH,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for accepting a signature
//! No Title 
//! Text Line 1 - Display SignaturePlease
//! Text Line 2 - Display PressEnter
//! No Buttons 
//! No Enable Flags
//! No sprites
static const UWORD signatureScr[] = {

	STARTSCREEN, SIGNATURE_SCR,
	CLEAR_ALL,

	// Text lines, No real title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,
	SignaturePlease,
	TEXTJ, JCENTER, VCENTER, 0, 11, 160, 15, PressEnter,

	// Insert Letter 'X'
	TEXTP, 0, 50, N_X,			// put "X" before signature line

	HORIZONTAL_LINE, 10, 60, 160, 1,	// sig line at x,y,len,thickness 

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for the Review Function
//! Title - Fixed to display N_Review, with NEXT, PREVIOUS and Cancel Buttons
//! Text Line 1 - Display CSTMSG Buffer
//! Text Line 2 - Display CSTMSG2 Buffer
//! Text Line 3 - Display CSTMSG3 Buffer
//! Text Line 4 - Display CSTMSG4 Buffer
//! Button 1 - Display N_Scroll  - Returns 'S' when pressed
//! Button 2 - Display N_Adjust  - Returns 'A' when pressed
//! Button 3 - Display N_Reprint - Returns 'R' when pressed
//! Button 4 - Display N_Void    - Returns 'V' when pressed
//! No Enable Flags 
//! No sprites
static const UWORD reviewScr[] = {

	STARTSCREEN, REVIEW_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H, N_Review,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H,

	// Previous Button
	BUTTON_IMAGE, IMAGE_BACKARROW, BACK_X, BACK_Y, BACK_W, BACK_H, BUTTON_UP,
	BUTTON_NOBORDER, B_PREVIOUS,

	// Next Button
	BUTTON_IMAGE, IMAGE_FORWARDARROW, NEXT_X, NEXT_Y, NEXT_W, NEXT_H,
	BUTTON_UP, BUTTON_NOBORDER, B_NEXT,


	// Set Virtual Window for text
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Text Lines
	TEXTL, 0, 0, CustomMsg,		// Text Line 1
	TEXTL, 1, 0, CustomMsg2,	// Text Line 2
	TEXTL, 2, 0, CustomMsg3,	// Text Line 3
	TEXTL, 3, 0, CustomMsg4,	// Text Line 4

	DEFAULT_WINDOW,

	// 3 Buttons
	ENABLE_FLAG, 1,
	BUTTON_IMAGE, IMAGE_BACKARROW, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, B_PREVIOUS,

	ENABLE_FLAG, 1,
	BUTTON_TEXT, N_Select, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, ENTER_KY,

	ENABLE_FLAG, 1,
	BUTTON_IMAGE, IMAGE_FORWARDARROW, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, B_NEXT,

	// 3 Buttons
	ENABLE_FLAG, 2,
	BUTTON_TEXT, N_Scroll, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'S',

	ENABLE_FLAG, 2,
	BUTTON_TEXT, N_Select, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'D',

	ENABLE_FLAG, 2,
	BUTTON_TEXT, N_ReprintShort, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'R',

	// 3 Buttons
	ENABLE_FLAG, 3,
	BUTTON_TEXT, N_Adjust, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE,'A',

	ENABLE_FLAG, 3,
	BUTTON_TEXT, N_ReprintShort, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'R',

	ENABLE_FLAG, 3,
	BUTTON_TEXT, N_Void, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'V',

	// Button
	ENABLE_FLAG, 4,
	BUTTON_TEXT, N_ReprintShort, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'R',

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for the Batch Totals Function
//! Title - Fixed to display BatchTotals, with NEXT, PREVIOUS and Cancel Buttons
//! Text Line 1 - Display CSTMSG Buffer - centered
//! Text Line 2 - Display CSTMSG2 Buffer
//! Text Line 3 - Display CSTMSG3 Buffer
//! Text Line 4 - Display CSTMSG4 Buffer
//! Text Line 5 - Display CSTMSG5 Buffer
//! No Buttons
//! No Enable Flags 
//! No sprites
static const UWORD rtotalsScr[] = {

	STARTSCREEN, RTOTALS_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H,
	BatchTotals,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H,

	// Previous Button
	BUTTON_IMAGE, IMAGE_BACKARROW, BACK_X, BACK_Y, BACK_W, BACK_H, BUTTON_UP,
	BUTTON_NOBORDER, B_PREVIOUS,

	// Next Button
	BUTTON_IMAGE, IMAGE_FORWARDARROW, NEXT_X, NEXT_Y, NEXT_W, NEXT_H,
	BUTTON_UP, BUTTON_NOBORDER, B_NEXT,

	// Set Virtual Window for text
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Text Lines
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H, CustomMsg,	// Text Line 1
	TEXTL, 1, 0, CustomMsg2,	// Text Line 2
	TEXTL, 2, 0, CustomMsg3,	// Text Line 3
	TEXTL, 3, 0, CustomMsg4,	// Text Line 4
	TEXTL, 4, 0, CustomMsg5,	// Text Line 5

	//Restart Full Window
	DEFAULT_WINDOW,

	// 3 Buttons
	BUTTON_IMAGE, IMAGE_BACKARROW, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, B_PREVIOUS,

	BUTTON_TEXT, N_Quit, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	BUTTON_IMAGE, IMAGE_FORWARDARROW, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, B_NEXT,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen will Draw the Title Bar
//! Title - Fixed to display MSG_DEFAULT_TITLE, with Cancel Button
//! No Buttons 
//! No Enable Flags
//1 No sprites
//! Sets text window for functions will change soon
static const UWORD titlebarScr[] = {

	STARTSCREEN, TITLEBAR_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	MSG_DEFAULT_TITLE,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Title Bar returns CANCEL_KY when pressed
	BUTTON_TRANSPARENT, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, BUTTON_UP,
	BUTTON_NOBORDER, CANCEL_KY,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,
	ENDSCREEN
};

//! This Screen is to display Batch Information
//! Title - Fixed to display N_BatchInfo, with Cancel Button
//! Text Line 1 - Display contents of CSTMSG Buffer
//! Text Line 2 - Display contents of CSTMSG2 Buffer
//! Text Line 3 - Display contents of CSTMSG3 Buffer
//! No Buttons 
//! Enable Flag 0 ON, Displays NEXT Arrow in Title Bar
//! Enable Flag 7 ON, Displays PREVIOUS Arrow in Title Bar
//! No sprites
static const UWORD batchinfoScr[] = {

	STARTSCREEN, BATCHINFO_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H,
	N_BatchInfo,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H,

	// Previous Button
	ENABLE_FLAG, 7,
	BUTTON_IMAGE, IMAGE_BACKARROW, BACK_X, BACK_Y, BACK_W, BACK_H, BUTTON_UP,
	BUTTON_NOBORDER, B_PREVIOUS,

	// Next Button
	ENABLE_FLAG, 0,
	BUTTON_IMAGE, IMAGE_FORWARDARROW, NEXT_X, NEXT_Y, NEXT_W, NEXT_H,
	BUTTON_UP, BUTTON_NOBORDER, B_NEXT,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Text Lines
	TEXTL, 1, 0, CustomMsg,
	TEXTL, 2, 0, CustomMsg2,
	TEXTL, 3, 0, CustomMsg3,

	//Restart Full Window
	DEFAULT_WINDOW,

	// 3 Buttons
	ENABLE_FLAG, 7,
	BUTTON_IMAGE, IMAGE_BACKARROW, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, B_PREVIOUS,

	BUTTON_TEXT, N_Quit, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	ENABLE_FLAG, 0,
	BUTTON_IMAGE, IMAGE_FORWARDARROW, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, B_NEXT,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for displaying a Card Test Screen 
//! Title - Displays CardRdT
//! First Line of text , Contents of CSTMSG Buffer
//! Swipe card sprite enable
static const UWORD cardtestScr[] = {

	STARTSCREEN, CARDTEST_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, CardRdT,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

    // Card swipe sprite
	SPRITE, SPRITE_SWIPE_CARD,

	// Text Lines
	TEXTJ, JLEFT, TOP, T_W_X, T_W_Y + 10, T_W_W - 10, T_W_H, CustomMsg,

    // Display it
	DISPLAY_ALL,
	ENDSCREEN
};



//!  Info Screen
//! This Screen is to display information on a full screen
//! Title - Fixed to display contents of CSTMSG4, with optional Cancel Button
//! Enable Flag 3 ON, Displays Only data in CSTMSG5, data centered in Text Window 
//! Enable Flag 4 ON, Displays data in CSTMSG and CSTMSG2, data centered in Text Window
//! Sprites are disabled
static const UWORD Info_Scr[] = {

	STARTSCREEN, INFO_SCR,
	CLEAR_ALL,

	// Title, long 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, CustomMsg4,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Stop Sprites
	DELETE_SPRITES,

	// Display Text in CSTMSG5 Buffer, if flag enabled
	ENABLE_FLAG, 3,
	BUTTON_TEXT, CustomMsg5, IB_X, IB_Y, IB_W, IB_H, BUTTON_UP, BUTTON_SQUARE,
	CANCEL_KY,

	// Display both CSTMSG and CSTMSG2 Buffer data, if flag enabled
	ENABLE_FLAG, 4,
	BUTTON_TEXT2, CustomMsg, CustomMsg2, IB_X, IB_Y, IB_W, IB_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	DEFAULT_WINDOW,
	LANG_FONT, DEFAULT_FONT_SIZE,

	DISPLAY_ALL,

	ENDSCREEN
};


//!  Error Screen
//! This Screen is used to display an error message over a current screen
//! Only clears the area required, so the previous screen can bee seen behind.
//! Title - Fixed to display N_Error, with no Cancel Button
//! Clears portion of the screen
//! Draws a border just inside area cleared
//! Halts Sprites
//! Displays Text in CSTMSG Buffer.  Centers text in box, uses two lines if needed
//! Button 1 - Displays N_OK  - Returns ENTER_KY when pressed
//! No Enable Flags 
//! Note Display area can not be set less than 8 pixels from border or it will
//! clear the edges.  That is why Clear Area was used here.
static const UWORD Error_Scr[] = {

	STARTSCREEN, ERROR_SCR,

	CLEAR_AREA, EB_X, EB_Y, EB_W, EB_H,

	// Stop Sprites
	DELETE_SPRITES,

	// Draw a border
	RECTANGLE, EB_X+EB_BC, EB_Y+EB_BC, EB_W-EB_B, EB_H-EB_B, EB_B,

	// Title, long 
	TEXTJ, JCENTER, VCENTER,
    EB_X+EB_B, EB_Y+EB_B, EB_W-EB_B, EB_TH, N_Error,
	INVERT_AREA, EB_X+EB_B+1, EB_Y+EB_B+1, EB_W-2*(EB_B+1), EB_TH,


	// Display Text in center of Text Area, convert to two line if required
	TEXTJ, JCENTER, VCENTER, EB_X+EB_B, EB_Y+EB_TH+1, EB_W-EB_B, EB_MH, CustomMsg,

	// Draw OK Button
	DEFAULT_WINDOW,

    // Button
	BUTTON_TEXT, N_OK, EB_B_X, EB_B_Y, EB_B_W, EB_B_H, BUTTON_UP,
	BUTTON_SQUARE, ENTER_KY,

	LANG_FONT, DEFAULT_FONT_SIZE,

	DISPLAY_ALL,
	ENDSCREEN
};


//! ICE PAK Screen
//! This Screen is for Ice Pak displays.
//! No Title 
//! No Buttons 
//! No Enable Flags 
//! Halts Sprites and clears the screen
static const UWORD IcePak_Scr[] = {

	STARTSCREEN, ICEPAK_IDLE_SCR,
	CLEAR_ALL,

	// Stop sprites
	DELETE_SPRITES,

	ENDSCREEN
};

//! This Screen is for displaying Test information 
//! Title - Displays CSTMSG Buffer, with Cancel Button
//! First Line of text , Contents of CSTMSG2 Buffer
//! Optional Second Line of text , Contents of CSTMSG3 Buffer
//! 1 Button - Display N_Quit - Returns CANCEL_KY when pressed 
//! Enable Flag 3 - Display second line of text when ON
//! Enable Flag 4 - Display QUIT button
//! No sprites
static const UWORD Test_Screen[] = {

	STARTSCREEN, TEST_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, CustomMsg,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Create window to draw Text Lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Text Lines
	ENABLE_FLAG, 3,
	TEXTL, 0, 0, CustomMsg2,	// Optional Line of text 
	TEXTL, 1, 0, CustomMsg3,	// Line of text always displayed 

	//Restart Full Window
	DEFAULT_WINDOW,

    // Button
	ENABLE_FLAG, 4,
	BUTTON_TEXT, N_Quit, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, SELECT_KY,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for displaying random information
//! Title - Display MSG_DEFAULT_TITLE, with Cancel Button
//! No Buttons 
//! Five lines of text. Text Buffers used CSTMSG - CSTMSG5
//! No sprites
static const UWORD Show_Info_Scr[] = {

	STARTSCREEN, SHOW_INFO_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	MSG_DEFAULT_TITLE,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Lines of text
	TEXTL, 0, 0, CustomMsg,		// Text Line 1
	TEXTL, 1, 0, CustomMsg2,	// Text Line 2
	TEXTL, 2, 0, CustomMsg3,	// Text Line 3
	TEXTL, 3, 0, CustomMsg4,	// Text Line 4
	TEXTL, 4, 0, CustomMsg5,	// Text Line 5

	//Restart Full Window
	DEFAULT_WINDOW,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for Diagnostics Testing of the display
//! Title - Fixed to display N_DisplayTest, with Cancel Button
//! One button that says "TEST"  returns 'T' when pressed
//! Enable Flag 3 ON, Inverts entire screen
//! No sprites
static const UWORD TestDisplayScr[] = {

	STARTSCREEN, TST_DSPLY_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	N_DisplayTest,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H - BDv_H,

	// Lines of text
	TEXTL, 0, 0, CustomMsg,		// Text Line 1
	TEXTL, 1, 0, CustomMsg2,	// Text Line 2
	TEXTL, 2, 0, CustomMsg3,	// Text Line 3
	TEXTL, 3, 0, CustomMsg4,	// Text Line 4
	TEXTL, 4, 0, CustomMsg5,	// Text Line 5

	//Restart Full Window
	DEFAULT_WINDOW,

	BUTTON_TEXT, N_Test, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'T',

	BUTTON_TEXT, N_NEXT, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'N',

	// Control if entire screen gets inverted
	ENABLE_FLAG, 3,
	INVERT_AREA, FW_X, FW_Y, FW_W, FW_H,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for Setting The Dial Type
//! Title - Fixed to display "DIAL MODE", with Cancel Button
//! Currenlty two buttons, set up for more to be added
//! Button 1 - "TONE"  - Returns '0' when pressed
//! Button 2 - "PULSE  - Returns '1' when pressed
//! Enable Flag 3 ON, Inverts Button 1
//! Enable Flag 4 ON, Inverts Button 2
//! No sprites
static const UWORD DialModeScr[] = {

	STARTSCREEN, DIAL_MODE_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	EnterDialMode,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

    // 2 Buttons
	BUTTON_TEXT, N_Tone, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, '0',

	BUTTON_TEXT, N_Pulse, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, '1',

	LANG_FONT, DEFAULT_FONT_SIZE,

	// Invert buttons that have enable flag set
	ENABLE_FLAG, 1,
	INVERT_AREA, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H,
	ENABLE_FLAG, 2,
	INVERT_AREA, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H,
	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for a two button screen
//! Title - Display CSTMSG, with Cancel Button
//! Text Line1 - Displays CSTMSG2
//! Text Line2 - Displays CSTMSG3
//! Button 1 - "NO"  - Returns CLEAR_KEY when pressed
//! Button 2 - "YES" - Returns ENTER_KY when pressed
//! No Enable Flags
//! No sprites
static const UWORD TwoBtnScr[] = {

	STARTSCREEN, TWO_BTN_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, CustomMsg,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Lines of text
	TEXTL, 0, 0, CustomMsg2,	// Text Line 1
	TEXTL, 1, 0, CustomMsg3,	// Text Line 2

	//Restart Full Window
	DEFAULT_WINDOW,

    // 2 Buttons
	BUTTON_TEXT, N_NO, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CLEAR_KY,

	BUTTON_TEXT, N_YES, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, ENTER_KY,

	LANG_FONT, DEFAULT_FONT_SIZE,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is to view the Servers( Cashiers) that are signed on
//! Title - Fixed to display N_ViewTable, with NEXT, PREVIOUS and Cancel Buttons
//! Text Line 1 - Display CSTMSG Buffer
//! Text Line 2 - Display CSTMSG2 Buffer
//! Text Line 3 - Display CSTMSG3 Buffer
//! Button 1 - Display N_Quit  - Returns CANCEL_KY when pressed
//! Enable Flag 0 ON, Displays NEXT Arrow in Title Bar
//! Enable Flag 7 ON, Displays PREVIOUS Arrow in Title Bar
//! No sprites
static const UWORD ServerScr[] = {

	STARTSCREEN, SERVER_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H,
	N_ViewTable,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H,

	// Previous Button
	ENABLE_FLAG, 7,
	BUTTON_IMAGE, IMAGE_BACKARROW, BACK_X, BACK_Y, BACK_W, BACK_H, BUTTON_UP,
	BUTTON_NOBORDER, 'P',

	// Next Button
	ENABLE_FLAG, 0,
	BUTTON_IMAGE, IMAGE_FORWARDARROW, NEXT_X, NEXT_Y, NEXT_W, NEXT_H,
	BUTTON_UP, BUTTON_NOBORDER, 'N',

	// Set Virtual Window for text
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Text Lines
	TEXTL, 0, 0, CustomMsg,		// Text Line 1
	TEXTL, 1, 0, CustomMsg2,	// Text Line 2
	TEXTL, 2, 0, CustomMsg3,	// Text Line 3

	//Restart Full Window
	DEFAULT_WINDOW,

	// 3 Buttons
	ENABLE_FLAG, 7,
	BUTTON_IMAGE, IMAGE_BACKARROW, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'P',

	BUTTON_TEXT, N_Quit, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	ENABLE_FLAG, 0,
	BUTTON_IMAGE, IMAGE_FORWARDARROW, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'N',

	BUTTON_TEXT, N_Quit, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for a the Cash Tender Screen in Amtproc.c
//! Title - Display MSG_DEFAULT_TITLE, with Cancel Button
//! Text Line1 - Displays CSTMSG
//! Text Line2 - Displays CSTMSG2
//! Text Line3 - Displays CSTMSG3
//! Button 1 - "NO"  - Returns CLEAR_KEY when pressed
//! Button 2 - "YES" - Returns ENTER_KY when pressed
//! No Enable Flags
//! No sprites
static const UWORD CashTenderScr[] = {

	STARTSCREEN, CASH_TEND_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	MSG_DEFAULT_TITLE,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Lines of text
	TEXTL, 0, 0, CustomMsg,		// Text Line 1
	TEXTL, 1, 0, CustomMsg2,	// Text Line 2
	TEXTL, 2, 0, CustomMsg3,	// Text Line 3

	//Restart Full Window
	DEFAULT_WINDOW,

	// 2 Buttons
	BUTTON_TEXT, N_NO, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	BUTTON_TEXT, N_YES, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, ENTER_KY,

	LANG_FONT, DEFAULT_FONT_SIZE,

	DISPLAY_ALL,
	ENDSCREEN
};


//! Screen Scripts
//! This Screen is for Setting a 4 button screen for Pin Pad Account Select
//! Title bar - Fixed to display contents of N_Pinpad, with a Cancel Button
//! No Text Lines
//! Button 1 - Displays contents of N_Account, N_1  - Returns '1' when pressed
//! Button 2 - Displays contents of N_Account, N_2  - Returns '2' when pressed
//! Button 3 - Displays contents of N_Account, N_3  - Returns '3' when pressed
//! Button 4 - Displays contents of N_Account, N_4  - Returns '4' when pressed
//! No Enable Flags
//! No sprites
static const UWORD SelectAcctScr[] = {

	STARTSCREEN, SELACCT_SCR,
	CLEAR_ALL,

	// Title, long 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, N_Pinpad,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// 4 Buttons
	TEXTL, 2, 0, N_Account,		// Text Line 1
	TEXTL, 3, 0, N_Account,	// Text Line 2
	TEXTL, 4, 0, N_Account,	// Text Line 3
	TEXTL, 5, 0, N_Account,	// Text Line 3

	TEXTL, 2, 18, N_1,		// Text Line 1
	TEXTL, 3, 18, N_2,	// Text Line 2
	TEXTL, 4, 18, N_3,	// Text Line 3
	TEXTL, 5, 18, N_4,	// Text Line 3

	// Button
	BUTTON_TEXT, N_Quit, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	DISPLAY_ALL,
	ENDSCREEN
};


//! Screen Scripts
//! This Screen is for Setting a Receipt Header and Footer Texts
//! Title bar - Fixed to display contents of RecieptTXT
//! 5 Text Lines
//! Line 1 - Displays "0" + Msg_HeaderLine
//! Line 2 - Displays "1" + CSTMSG
//! Line 3 - Displays "2" + CSTMSG2
//! Line 4 - Displays "3" + CSTMSG3
//! Line 5 - Displays "4" + CSTMSG4
//! No Enable Flags
//! No sprites
//! No buttons
static const UWORD HeaderFooterScr[] = {

	STARTSCREEN, HEADER_FOOTER_SCR,
	CLEAR_ALL,

	// Title, long 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, RecieptTXT,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

    // "0  Header 4"
	TEXTL, 2, 3, N_0,
	TEXTL, 2, 10, Msg_HeaderLine,

    // "1  Footer 1"
	TEXTL, 3, 3, N_1,
	TEXTL, 3, 10, CustomMsg,	

    // "2  Footer 2"
	TEXTL, 4, 3, N_2,
	TEXTL, 4, 10, CustomMsg2,	

    // "3  Footer 3"
	TEXTL, 5, 3, N_3,
	TEXTL, 5, 10, CustomMsg3,	

    // "4  Footer 4"
	TEXTL, 6, 3, N_4,
	TEXTL, 6, 10, CustomMsg4,	

	LANG_FONT, DEFAULT_FONT_SIZE,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for Setting The Percentage the Tip will be
//! Title - Fixed to display MSG_DEFAULT_TITLE, with Cancel Button
//! Text Line, Displays Msg_SelectTip
//! Button 1 - Displays contents of CSTMSG  - Returns 'a' when pressed
//! Button 2 - Displays contents of CSTMSG  - Returns 'b' when pressed
//! Button 3 - Displays contents of CSTMSG  - Returns 'c' when pressed
//! Button 4 - Displays contents of CSTMSG  - Returns 'd' when pressed
//! Enable Flag 3 ON, Draw third button
//! Enable Flag 4 ON, Draw forth button
//! No sprites
static const UWORD TipPercentScr[] = {

	STARTSCREEN, TIP_PERCENT_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	MSG_DEFAULT_TITLE,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

    // Text Line 0
	TEXTL, 0, 0, Msg_PreTip,

    // Text Line 1
	TEXTL, 1, 0, CustomMsg,
	TEXTL, 1, 19, N_1,		

    // Text Line 2
	TEXTL, 2, 0, CustomMsg2,
	TEXTL, 2, 19, N_2,	

    // Text Line 3
	ENABLE_FLAG, 3,
	TEXTL, 3, 0, CustomMsg3,
	ENABLE_FLAG, 3,
	TEXTL, 3, 19, N_3,	

    // Text Line 4
	ENABLE_FLAG, 4,
	TEXTL, 4, 0, CustomMsg4,
	ENABLE_FLAG, 4,
	TEXTL, 4, 19, N_4,

	//Restart Full Window
	DEFAULT_WINDOW,

	// Button
	BUTTON_TEXT, N_Quit, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	LANG_FONT, DEFAULT_FONT_SIZE,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for displaying the stauts of the Memory Pages
//! Title bar - Fixed to display contents of MemPgStatus, with a No Cancel Button
//! 5  Text Lines
//! Text Line 1 - Displays CSTMSG  
//! Text Line 2 - Displays CSTMSG2 
//! Text Line 3 - Displays CSTMSG3
//! Text Line 4 - Displays CSTMSG4
//! Text Line 5 - Displays CSTMSG5
//! No Enable Flags
//! No sprites
static const UWORD MemStatusScr[] = {

	STARTSCREEN, MEM_STATUS_SCR,
	CLEAR_ALL,

	// Title, long 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,
	MemPgStatus,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Text Lines
	TEXTLJ, 0, JLEFT, CustomMsg,	// Text Line 1
	TEXTLJ, 1, JLEFT, CustomMsg2,	// Text Line 2
	TEXTLJ, 2, JLEFT, CustomMsg3,	// Text Line 3
	TEXTLJ, 3, JLEFT, CustomMsg4,	// Text Line 4
	TEXTLJ, 4, JLEFT, CustomMsg5,	// Text Line 5

	LANG_FONT, DEFAULT_FONT_SIZE,
	DEFAULT_WINDOW,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for displaying the Software Version
//! Title bar - Fixed to display contents of N_Identification, with a No Cancel Button
//! 4  Text Lines
//! Text Line 1 - Displays N_Software Left, CSTMSG Right justified   
//! Text Line 2 - Displays N_DLL Left, CSTMSG2 Right justified 
//! Text Line 3 - Displays N_EPROM, CSTMSG3  Right justified
//! Text Line 4 - Displays N_Terminal, CSTMSG4 Right justified
//! Enable Flag 0 ON, Displays Cancel Button in Title Bar
//! Enable Flag 0 OFF, Displays SelfTest
//! Inverted Text area Displaying SelfTest
static const UWORD SoftwareVerScr[] = {

	STARTSCREEN, SOFT_VER_SCR,
	CLEAR_ALL,

	// Title, long 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,
	N_Identification,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, INFO_H,

	// Text Lines
	TEXTL, 0, 0, N_Software,	// Text Line 1 - Left Justified
	TEXTLJ, 0, JRIGHT, CustomMsg,	// Text Line 1 - Right Justified
	TEXTLJ, 2, JLEFT, CustomMsg2,	// Text Line 2 - Right Justified
	TEXTLJ, 3, JLEFT, CustomMsg3,	// Text Line 3 - Right Justified
    TEXTLJ, 4, JLEFT, CustomMsg4,	// Text Line 4 - Right Justified

	//Restart Full Window
	DEFAULT_WINDOW,

	ENABLE_FLAG_NOT, 0,
	TEXTJ, JCENTER, VCENTER, FW_X, FW_H - TITLE_H, FW_W, TITLE_H, SelfTest,
	ENABLE_FLAG_NOT, 0,
	INVERT_AREA, FW_X, FW_H - TITLE_H, FW_W, TITLE_H,

	// Button
	ENABLE_FLAG, 0,
	BUTTON_TEXT, N_NEXT, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, ENTER_KY,

	LANG_FONT, DEFAULT_FONT_SIZE,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for displaying the Libraries Version
//! Title bar - Fixed to N_LibsVersion, with Cancel Button
//! Displays Cancel Button in Title Bar
//! 4  Text Lines
//! Text Line 1 - Displays N_SDKVersion Left,   CSTMSG  Right justified   
//! Text Line 2 - Displays N_OSVersion Left,    CSTMSG2 Right justified 
//! Text Line 3 - Displays N_OSRevision Left,   CSTMSG3 Right justified
//! Text Line 4 - Displays N_ScardVersion Left, CSTMSG4 Right justified
static const UWORD LibrariesVerScr[] = {

	STARTSCREEN, LIBS_VER_SCR,
	CLEAR_ALL,

	// Title, long 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,
	N_LibsVersion,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, INFO_H,

    // Text Lines
	TEXTL,  0, 0,      N_SDKVersion,	// Text Line 1 - Left Justified
	TEXTLJ, 0, JRIGHT, CustomMsg,		// Text Line 1 - Right Justified
	TEXTL,  1, 0,      N_HFCVersion,	// Text Line 2 - Left Justified
	TEXTLJ, 1, JRIGHT, CustomMsg2,		// Text Line 2 - Right Justified
	TEXTL,  2, 0,      N_OSVersion,		// Text Line 3 - Left Justified
	TEXTLJ, 2, JRIGHT, CustomMsg3,		// Text Line 3 - Right Justified
	TEXTL,  3, 0,      N_OSRevision,	// Text Line 4 - Left Justified
	TEXTLJ, 3, JRIGHT, CustomMsg4,		// Text Line 4 - Right Justified

	//Restart Full Window
	DEFAULT_WINDOW,
	LANG_FONT, DEFAULT_FONT_SIZE,

	// Button
	BUTTON_TEXT, N_OK, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is to Setting The Tip Percentages 
//! Title - Fixed to display Msg_Tips, with Cancel and Previous Buttons
//! No Text Lines
//! Button 1 - Displays contents of CSTMSG   - Returns '1' when pressed
//! Button 2 - Displays contents of CSTMSG2  - Returns '2' when pressed
//! Button 3 - Displays contents of CSTMSG3  - Returns '3' when pressed
//! Button 4 - Displays N_Quit               - Returns 'd' when pressed
//! No Enable Flags
//! No sprites
static const UWORD SetTipScr[] = {

	STARTSCREEN, SET_TIP_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H, Msg_Tips,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H,

	// Previous Button
	BUTTON_IMAGE, IMAGE_BACKARROW, BACK_X, BACK_Y, BACK_W, BACK_H, BUTTON_UP,
	BUTTON_NOBORDER, B_PREVIOUS,

	TEXTL, 2, 0, CustomMsg,	// Text Line 1
	TEXTL, 3, 0, CustomMsg2,	// Text Line 2
	TEXTL, 4, 0, CustomMsg3,	// Text Line 3

	TEXTL, 2, 19, N_1,		// Text Line 1
	TEXTL, 3, 19, N_2,	// Text Line 2
	TEXTL, 4, 19, N_3,	// Text Line 3

	// 2 Buttons
	BUTTON_IMAGE, IMAGE_BACKARROW, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, B_PREVIOUS,

	BUTTON_TEXT, N_Quit, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'd',

	LANG_FONT, DEFAULT_FONT_SIZE,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for displaying the Network Statistics
//! Title - Fixed to display Msg_TxRxErrors with Cancel Button
//! 2  Text Lines
//! Text Line 1 - Displays Msg_TranErrors Left, CSTMSG Right justified   
//! Text Line 2 - Displays Msg_RecvErrors Left, CSTMSG2 Right justified 
//! 1 Button - Display N_Quit - Returns CANCEL_KY when pressed 
//! No Enable Flags
//! No sprites
static const UWORD StatisticsScr[] = {

	STARTSCREEN, STATISTICS_SCR,
	CLEAR_ALL,

	// Title, long 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,
	Msg_TxRxErrors,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, INFO_H,

	// Text Lines
	TEXTL, 0, 0, Msg_TranErrors,	// Text Line 1 - Left Justified
	TEXTLJ, 0, JRIGHT, CustomMsg,	// Text Line 1 - Right Justified
	TEXTL, 1, 0, Msg_RecvErrors,	// Text Line 2 - Left Justified
	TEXTLJ, 1, JRIGHT, CustomMsg2,	// Text Line 2 - Right Justified

	//Restart Full Window
	DEFAULT_WINDOW,

	// Button
	BUTTON_TEXT, N_Quit, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	LANG_FONT, DEFAULT_FONT_SIZE,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is to display Settelement Status Information
//! Title - Fixed to display SettleTran, with Cancel Button and Previous/Next
//! Text Line 1 - Display N_SettleStat centered
//! Text Line 2 - Display contents of CSTMSG Buffer
//! Text Line 3 - Display contents of CSTMSG2 Buffer
//! SCROLL Button - Returns 'S' when pressed
//! Enable Flag 0 ON, Displays NEXT Arrow in Title Bar
//! Enable Flag 1 ON, Displays PREVIOUS Arrow in Title Bar
//! Enable Flag 2 ON, Displays SCROLL
static const UWORD SettleInfoScr[] = {

	STARTSCREEN, SETTLE_INFO_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H, SettleTran,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_N_W, TITLE_H,

	// Previous Button
	ENABLE_FLAG, 1,
	BUTTON_IMAGE, IMAGE_BACKARROW, BACK_X, BACK_Y, BACK_W, BACK_H, BUTTON_UP,
	BUTTON_NOBORDER, B_PREVIOUS,

	// Next Button
	ENABLE_FLAG, 0,
	BUTTON_IMAGE, IMAGE_FORWARDARROW, NEXT_X, NEXT_Y, NEXT_W, NEXT_H,
	BUTTON_UP, BUTTON_NOBORDER, B_NEXT,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Text Lines
	TEXTLJ, 0, JCENTER, N_SettleStat,	// Text Line 1 - Centered 
	TEXTLJ, 1, JLEFT, CustomMsg,	// Text Line 2 - Left Justified
	TEXTLJ, 1, JRIGHT, CustomMsg2,	// Text Line 2 - Right Justified
	TEXTLJ, 2, JLEFT, CustomMsg3,	// Text Line 3 - Left Justified
	TEXTLJ, 2, JRIGHT, CustomMsg4,	// Text Line 3 - Right Justified

	//Restart Full Window
	DEFAULT_WINDOW,

	// 3 Buttons
	ENABLE_FLAG, 1,
	BUTTON_IMAGE, IMAGE_BACKARROW, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, B_PREVIOUS,

	ENABLE_FLAG, 2,
	BUTTON_TEXT, N_Scroll, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, 'S',

	ENABLE_FLAG, 0,
	BUTTON_IMAGE, IMAGE_FORWARDARROW, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, B_NEXT,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for setting the CVV2 Indicator
//! Title - Fixed to display MSG_DEFAULT_TITLE
//! Text Line, Displays N_CVV2Indicator
//! Button 1 - Displays N_CVV2IndEntry1  - Returns '0' when pressed
//! Button 2 - Displays N_CVV2IndEntry2  - Returns '1' when pressed
//! Button 3 - Displays N_CVV2IndEntry3  - Returns '2' when pressed
//! Button 4 - Displays N_CVV2IndEntry4  - Returns '9' when pressed
//! Button 5 - displays N_X and returns CANCEL_KY when pressed
//! No sprites
static const UWORD CVV2IndScr[] = {

	STARTSCREEN, CVV2_IND_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	MSG_DEFAULT_TITLE,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	// Lines of text
	TEXTL, 0, 0, N_CVV2Indicator,	// Text Line 1
	TEXTL, 1, 0, N_CVV2IndEntry1,	// Text Line 1
	TEXTL, 2, 0, N_CVV2IndEntry2,	// Text Line 2
	TEXTL, 3, 0, N_CVV2IndEntry3,	// Text Line 3
	TEXTL, 4, 0, N_CVV2IndEntry4,	// Text Line 3

	//Restart Full Window
	DEFAULT_WINDOW,

	// Button
	BUTTON_TEXT, N_Quit, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CANCEL_KY,

	LANG_FONT, DEFAULT_FONT_SIZE,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen is for Select the current Language
//! Title bar - Fixed to display Msg_SelectLanguage, with a Cancel Button
//! Text Left,  Fixed to display Currently
//! Text Right, Fixed to display contents of MSG_USER7, current language
//! Button 1 - Displays contents of MSG_USER1  - Returns EVENT_USER1 when pressed
//! Button 2 - Displays contents of MSG_USER2  - Returns EVENT_USER2 when pressed
//! Button 3 - Displays contents of MSG_USER3  - Returns EVENT_USER3 when pressed
//! Button 4 - Displays contents of MSG_USER4  - Returns EVENT_USER4 when pressed
//! Enable Flag 1 ON, Display Button 1
//! Enable Flag 2 ON, Display Button 2
//! Enable Flag 3 ON, Display Button 3
//! Enable Flag 4 ON, Display Button 4
//! No sprites
static const UWORD LangSelectScr[] = {

	STARTSCREEN, LANG_SELECT_SCR,
	CLEAR_ALL,

	// Title 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	Msg_SelectLanguage,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Create window to draw text lines
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,

	TEXTL, 1, 0, Currently,	// Text Line 1 - Left Justified
	TEXTLJ, 1, JRIGHT, MSG_USER7,	// Text Line 1 - Right Justified

	//Restart Full Window
	DEFAULT_WINDOW,

	// 3 Buttons
	ENABLE_FLAG, 1,
	BUTTON_TEXT, MSG_USER1, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, EVENT_USER1,

	ENABLE_FLAG, 2,
	BUTTON_TEXT, MSG_USER2, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, EVENT_USER2,

	ENABLE_FLAG, 3,
	BUTTON_TEXT, MSG_USER3, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, EVENT_USER3,

	DISPLAY_ALL,
	ENDSCREEN
};

//! This Screen Will allow the user to confirm correct language is selected
//! Title - Contents of Msg_SelectLanguage, Returns CANCEL_KY when pressed
//! Two text lines and Two Buttons 
//! Text Line 1 - Msg_CurrentLang 
//! Text Line 2 - MSG_USER7 - Centered
//! Button 1 - Display N_NO   - Returns CLEAR_KY when pressed
//! Button 2 - Display Y_YES  - Returns ENTER_KY when pressed
//! No Enable Flags 
static const UWORD ConfirmLangScr[] = {

	// one big button on top, two lower smaller ones titlebar
	STARTSCREEN, CONFIRM_LANG_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,
	Msg_SelectLanguage,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Text Lines 
	TEXTJ, JCENTER, TOP, T_W_X, T_W_Y, T_W_W, T_W_H, Msg_CurrentLang,	// Text Line 1 
	TEXTJ, JCENTER, VCENTER, T_W_X, T_W_Y, T_W_W, T_W_H, MSG_USER7,	// Text Line 2 

	BUTTON_TEXT, N_NO, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, CLEAR_KY,

	BUTTON_TEXT, N_YES, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, ENTER_KY,

	DISPLAY_ALL,
	ENDSCREEN
};

#ifdef MAKE_EMV	 
//! This Screen is for displaying the EMV Versions
//! Title bar - Fixed to	display contents of N_Identification, with a No Cancel Button
//! 4  Text Lines
//! Text Line 1 - Displays N_Software Left, CSTMSG Right justified   
//! Text Line 2 - Displays N_DLL Left, CSTMSG2 Right justified 
//! Text Line 3 - Displays N_EPROM, CSTMSG3  Right justified
//! Text Line 4 - Displays N_Terminal, CSTMSG4 Right justified
//! Enable Flag 0 ON, Displays Cancel Button in Title Bar
//! Enable Flag 0 OFF, Displays SelfTest
//! Inverted Text area Displaying SelfTest
const UWORD EMVVerScr[] =
{

	STARTSCREEN, EMV_VER_SCR,
	CLEAR_ALL,

	// Title, long 
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H, N_Identification,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_NC_W, TITLE_H,

	/* Create window to draw text lines	*/
	WINDOW, T_W_X, T_W_Y, T_W_W, INFO_H,

	/* Text Lines */
    TEXTL,  0, 0,      CustomMsg,	// Text Line 1 - Position 0
    TEXTLJ, 0, JRIGHT, CustomMsg2,	// Text Line 1 - Right Justified
    TEXTL,  1, 0,      CustomMsg3,	// Text Line 2 - Position 0
    TEXTLJ, 1, JRIGHT, CustomMsg4,	// Text Line 2 - Right Justified
	DEFAULT_WINDOW,

	ENABLE_FLAG_NOT, 0,	
	TEXTJ, JCENTER, VCENTER, FW_X, FW_H-TITLE_H, FW_W, TITLE_H, SelfTest,
	ENABLE_FLAG_NOT, 0,		
	INVERT_AREA, FW_X, FW_H-TITLE_H, FW_W, TITLE_H,

	// Buttons
	ENABLE_FLAG, 0,
	BUTTON_TEXT,  N_OK, BDv_B2_X, BDv_B2_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, ENTER_KY,

	LANG_FONT, DEFAULT_FONT_SIZE,

	DISPLAY_ALL,
	ENDSCREEN
};

//! Title - Contents of MSG_USER7, with Cancel ('X') Button ( set in template )
//! Two text lines and Two Buttons 
//! Text Line 1 - MSG_USER1
//! Text Line 2 - MSG_USER2
//! Button 1 - Display N_Cancel - Returns CLEAR_KY when pressed
//! Button 2 - Display N_Bypass - Returns ENTER_KY when pressed
//! No Enable Flags 
const UWORD PinWaitScr[] = 
{

	STARTSCREEN, PINWAIT_SCR,
	CLEAR_ALL,

	// Title
	TEXTJ, JCENTER, VCENTER, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H, MSG_USER7,
	INVERT_AREA, TITLE_X, TITLE_Y, TITLE_F_W, TITLE_H,

	// Text Box full window below title bar
	WINDOW, T_W_X, T_W_Y, T_W_W, T_W_H,


	DEFAULT_WINDOW,
	// Text lines
    // Display Text in center of Text Area, convert to two line if required
    TEXTJ, JCENTER, VCENTER, 10, 20, 110, 15, WaitingForPin,
    TEXTJ, JCENTER, VCENTER, 10, 35, 110, 15, OnPinpad,

	// 2 Buttons
	ENABLE_FLAG,	4,
	BUTTON_TEXT, N_Cancel, BDv_B1_X, BDv_B1_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, EVENT_USER3,

	ENABLE_FLAG,	4,
	BUTTON_TEXT, N_Bypass, BDv_B3_X, BDv_B3_Y, BDv_W, BDv_H, BUTTON_UP,
	BUTTON_SQUARE, EVENT_USER4,

	// Set up text and event for 2 button template
	SET_USER_EVENT, 3, CLEAR_KY, 	/* Button 1 event */
	SET_USER_EVENT, 4, ENTER_KY,  	/* Button 2 event */

	DISPLAY_ALL,
	ENDSCREEN
};
#endif


// *INDENT-OFF*
//! table screen functions and scripts on this page 
static const SCREEN_ENTRY IdTable[] = {
//!  Screen ID				Script
//!
//! IDLE Screen
	{IDLE_SCR,              (UWORD *)idleScreen},  
//! ININT Screen
	{INIT_SCR,              (UWORD *)initScreen},
//! Status Screen
	{STATUS_SCR,			(UWORD *)statusScr},
//! Status Screen with card sprite
	{CARDSWIPE_SCR,			(UWORD *)cardswipeScr},
//! Set Contrast Scr
	{SETCONTRAST_SCR,		(UWORD *)contrastScreen},
//! Input Screen
	{INPUT_SCR,             (UWORD *)inputScr},
//! YES/NO Screen
	{YESNO_SCR,             (UWORD *)yesnoScreen},
//! Numeric Entry Scr
	{NUMENTRY_SCR,			(UWORD *)inputScr},
//! Special amount Entry Scr
	{AMTENTRY_SCR,			(UWORD *)amtentryScr},
//! REVIEW Screen
	{REVIEW_SCR,			(UWORD *)reviewScr},
//! template for Yes/No screens
	{TEMPLATE_2BTNQUERY_SCR,(UWORD *)template2BtnQuery},
//! Signature Capture Screen
	{SIGNATURE_SCR,			(UWORD *)signatureScr},
//! Standard 5 button text only menu
	{MENU_5BTNTEXT_SCR,		(UWORD *)menu5BtnTextScr},
//! Standard 5 button text only menu
	{MENU_10BTNTEXT_SCR,	(UWORD *)menu10BtnTextScr},
//! Continue Screen
	{CONTINUE_SCR,			(UWORD *)continueScr},
//! Alpha numeric entry 1
	{ALPHANUM1_SCR,			(UWORD *)alphaInput1Scr},
//! Alpha numeric entry 2
	{ALPHANUM2_SCR,			(UWORD *)alphaInput2Scr},
//! Alpha numeric entry 3
	{ALPHANUM3_SCR,			(UWORD *)alphaInput3Scr},
//! Alpha numeric entry 4
	{ALPHANUM4_SCR,			(UWORD *)alphaInput4Scr},
//! Report Totals Scr
	{RTOTALS_SCR,			(UWORD *)rtotalsScr},
//! Title Bar
	{TITLEBAR_SCR,			(UWORD *)titlebarScr},
//! Batch Info Screen
	{BATCHINFO_SCR,			(UWORD *)batchinfoScr},
//! Cardreader Test Scr
	{CARDTEST_SCR,			(UWORD *)cardtestScr},
//! descriptor entry screen
	{DESCRIPTOR_SCR,		(UWORD *)descriptorScr},
//! INFO Message Screen
	{INFO_SCR,              (UWORD *)Info_Scr},
//! ERROR Message Screen
	{ERROR_SCR,             (UWORD *)Error_Scr},
//! ICE PAK Idle Screen
	{ICEPAK_IDLE_SCR,		(UWORD *)IcePak_Scr},
//! Diag Test Function Screen
	{TEST_SCR,              (UWORD *)Test_Screen},
//! Five Line Info Screen
	{SHOW_INFO_SCR,			(UWORD *)Show_Info_Scr},
//! Display Test Screen
	{TST_DSPLY_SCR,			(UWORD *)TestDisplayScr},
//! Dial Mode Screen
	{DIAL_MODE_SCR,			(UWORD *)DialModeScr},
//! Flexable 2 button Yes/No Screen
	{TWO_BTN_SCR,			(UWORD *)TwoBtnScr},
//! View Servers Screen
	{SERVER_SCR,			(UWORD *)ServerScr},
//! Amount Proc's Cash/Tender Screen
	{CASH_TEND_SCR,			(UWORD *)CashTenderScr},
//! Screen for internal Pin Pad Select Account
	{SELACCT_SCR,			(UWORD *)SelectAcctScr},
//! Screen for Receipt Header/Footer Text
	{HEADER_FOOTER_SCR,		(UWORD *)HeaderFooterScr},
//! 2 to 4 buttons of pretip percentage screen
	{TIP_PERCENT_SCR,		(UWORD *)TipPercentScr},
//! Status of Memory Page Screen
	{MEM_STATUS_SCR,		(UWORD *)MemStatusScr},
//! Set Local Tip Screen
	{SET_TIP_SCR,			(UWORD *)SetTipScr},
//! Software Version Screen
	{SOFT_VER_SCR,			(UWORD *)SoftwareVerScr},

#ifdef MAKE_EMV
//! EMV Level 1 & 2 versions
	{EMV_VER_SCR,			(UWORD *)EMVVerScr},
//! Waiting for PIN entry on External Pinpad
	{PINWAIT_SCR,			(UWORD *)PinWaitScr},
#endif
//! Libraries Version Screen
	{LIBS_VER_SCR,			(UWORD *)LibrariesVerScr},
//! Network Statistics Screen
	{STATISTICS_SCR,		(UWORD *)StatisticsScr},
//! Settlement Status Info Screen
	{SETTLE_INFO_SCR,		(UWORD *)SettleInfoScr},
//! CVV2 Indicator Screen
	{CVV2_IND_SCR,			(UWORD *)CVV2IndScr},
//! Select the current Language
	{LANG_SELECT_SCR,		(UWORD *)LangSelectScr},
//! Confirm the current Language
	{CONFIRM_LANG_SCR,		(UWORD *)ConfirmLangScr},
//! YES/NEXT Screen
	{YES_NEXT_SCR,			(UWORD *)YesNextScr}
};
// *INDENT-ON*

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Puts Screen table address into global data area.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void GetScreenTable1( void )
{
	pTableStart = ( UBYTE * ) IdTable;
	TablePage = SDK_ThisPage(  );
	TableItemCount = sizeof( IdTable ) / sizeof( SCREEN_ENTRY );
}

//=============================================================================
// Private function definitions
//=============================================================================

