
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
#ifndef _SCREEN_H_
#define _SCREEN_H_
//=============================================================================
//
// Module overview:             screen.h
//      screen information
//
//=============================================================================

#include "sdk.h"
#include "msg.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================
typedef enum
{
	SCREEN_START = 0x2000,		// place holder for start
	IDLE_SCR,					// IDLE Screen
	INIT_SCR,					// ININT Screen
	STATUS_SCR,					// Status Screen
	CARDSWIPE_SCR,				// Status Screen with card sprite
	SETCONTRAST_SCR,			// Set Contrast Scr
	INPUT_SCR,					// Input Screen
	YESNO_SCR,					// YES/NO Screen
	NUMENTRY_SCR,				// Numeric Entry Scr
	AMTENTRY_SCR,				// Special amount Entry Scr
	REVIEW_SCR,					// REVIEW Screen
	TEMPLATE_2BTNQUERY_SCR,		// template for Yes/No screens
	SIGNATURE_SCR,				// Signature Capture Screen
	MENU_5BTNTEXT_SCR,			// Standard 5 button text only menu
	MENU_10BTNTEXT_SCR,			// Standard 10 button text only menu
	CONTINUE_SCR,				// Continue Screen
	ALPHANUM1_SCR,				// Alpha numeric entry 1 (Numeric keypad)
	ALPHANUM2_SCR,				// Alpha numeric entry 2 (Capital Alphabetic keypad)
	ALPHANUM3_SCR,				// Alpha numeric entry 3 (Small Alphabetic keypad)
	ALPHANUM4_SCR,				// Alpha numeric entry 4 (Symbols keypad)
	IPNUM_SCR,
	RTOTALS_SCR,				// Report Totals Scr
	TITLEBAR_SCR,				// Title Bar
	BATCHINFO_SCR,				// Batch Info Screen
	CARDTEST_SCR,				// Cardreader Test Scr
	DESCRIPTOR_SCR,				// descriptor entry screen
	INFO_SCR,					// INFO Message Screen
	ERROR_SCR,					// ERROR Message Screen
	ICEPAK_IDLE_SCR,			// ICE PAK Idle Screen
	TEST_SCR,					// Diag Test Function Screen
	SHOW_INFO_SCR,				// Five Line Info Screen
	TST_DSPLY_SCR,				// Display Test Screen
	DIAL_MODE_SCR,				// Dial Mode Screen
	TWO_BTN_SCR,				// Flexable 2 button Yes/No Screen
	SERVER_SCR,					// View Servers Screen
	CASH_TEND_SCR,				// Amount Proc's Cash/Tender Screen
	SELACCT_SCR,				// Screen for internal Pin Pad Select Account
	HEADER_FOOTER_SCR,			// Screen for Receipt Header/Footer Text
	TIP_PERCENT_SCR,			// PreTip 2 to 4 button screen
	MEM_STATUS_SCR,				// Status of Memory Page Screen
	SOFT_VER_SCR,				// Software Version Screen
#ifdef MAKE_EMV
	EMV_VER_SCR,			/* EMV Level 1 & 2 versions */
	PINWAIT_SCR,			/* Pin entry on external pinpad prompt */
#endif

	LIBS_VER_SCR,				// Libraries Version Screen
	SET_TIP_SCR,				// Set Local Tip Screen
	STATISTICS_SCR,				// Network Statistics Screen
	SETTLE_STAT_SCR,			// Settlement Statistics Screen
	SETTLE_INFO_SCR,			// Settlement Status Info Screen
	CVV2_IND_SCR,				// CVV2 Indicator Screen
	LANG_SELECT_SCR,			// Select the current Language
	CONFIRM_LANG_SCR,			// Confirm the current Language
	YES_NEXT_SCR,				// Yes/Next screen
	SCREEN_MAX					// Must always stay at the end
}
SCREEN_ID;

typedef struct
{
	SCREEN_ID id;
	UWORD *pScript;
}
SCREEN_ENTRY;

typedef struct
{
	SCREEN_ENTRY *pScreenEntry;	// address of SCREEN_ENTRY
	PAGE_NUMBERS Page;			// page number of table
}
SCREEN_CROSSREF;


// Defines for button locations used by draw_btns()


// Single Button "Continue" type Screens
#define B1_X					40
#define B1_Y					58
#define B1_W					80
#define B1_H					20

// 2 Button Screen
#define B2_B1_X					2	// Right Button
#define B2_B1_Y					60

#define B2_B2_X					80	// Left Button
#define B2_B2_Y					60

#define B2_W					70
#define B2_H					20

// 3 Button Screen - Initialize and Config
#define B3_B1_X   				0	// Top Button
#define B3_B1_Y	    		 	20
#define B3_B1_W   				155
#define B3_B1_H					30

#define B3_B2_X   				0	// Left Button
#define B3_B2_Y					54
#define B3_B2_W   				76
#define B3_B2_H					25

#define B3_B3_X   				80	// Right Button
#define B3_B3_Y					54
#define B3_B3_W   				76
#define B3_B3_H					25

// 4 Button Screen with space for SPRITE
#define B4S_B1_X   				0	// Left Top
#define B4S_B1_Y	    		27

#define B4S_B2_X   				75	// Right Top
#define B4S_B2_Y				27

#define B4S_B3_X   				0	// Left Bottom
#define B4S_B3_Y				54

#define B4S_B4_X   				75	// Right Bottom
#define B4S_B4_Y	    		54

#define B4S_W   				73
#define B4S_H					25

// 4 Button Screen without space for SPRITE
#define B4_B1_X   				0	// Left Top
#define B4_B1_Y	    			21

#define B4_B2_X   				81	// Right Top
#define B4_B2_Y					21

#define B4_B3_X   				0	// Left Bottom
#define B4_B3_Y					51

#define B4_B4_X   				81	// Right Bottom
#define B4_B4_Y	    			51

#define B4_W   					79
#define B4_H					28

// 6 Button Screen
#define B6_B1_X   				0	// Left Top
#define B6_B1_Y	    			20

#define B6_B2_X   				80	// Right Top
#define B6_B2_Y					20

#define B6_B3_X   				0	// Left Middle
#define B6_B3_Y					40

#define B6_B4_X   				80	// Right Middle
#define B6_B4_Y	    			40

#define B6_B5_X   				0	// Left Bottom
#define B6_B5_Y	    			60

#define B6_B6_X   				80	// Right Bottom
#define B6_B6_Y	    			60


#define B6_W   					75
#define B6_H					18

// - Viking button scr
// 6 Button Screen
#define Bv_B1_X   				17	// Left Top
#define Bv_B1_Y	    			11

#define Bv_B2_X   				Bv_B1_X	// Right Top
#define Bv_B2_Y					Bv_B1_Y + Bv_H - 1 

#define Bv_B3_X   				Bv_B1_X	// Left Middle
#define Bv_B3_Y					Bv_B2_Y + Bv_H - 1

#define Bv_B4_X   				Bv_B1_X	// Right Middle
#define Bv_B4_Y	    			Bv_B3_Y + Bv_H - 1 

#define Bv_B5_X   				Bv_B1_X	// Left Bottom
#define Bv_B5_Y	    			Bv_B4_Y + Bv_H - 1

#define Bv_B6_X   				Bv_B1_X	// Right Bottom
#define Bv_B6_Y	    			Bv_B5_Y + Bv_H - 1 

#define Bv_W   					128 - 2*Bv_B1_X
#define Bv_H					11

//Viking Dialog 3 Buttons

#define BDv_B1_X   				2	// Left
#define BDv_B1_Y	    		53

#define BDv_B2_X   				44	// Middle
#define BDv_B2_Y				53

#define BDv_B3_X   				86	// Right
#define BDv_B3_Y				53

#define BDv_H   				11 
#define BDv_W	    			40

//Viking Init Screen
#define InitScreenText_X		0
#define InitScreenText_Y		20
#define InitScreenText_H		10
#define InitScreenText_W		128


// 4 ICON Button Screen with space for SPRITE
#define B4IS_B1_X   			0	// Left Top
#define B4IS_B1_Y	    		16

#define B4IS_B2_X   			72	// Right Top
#define B4IS_B2_Y				16

#define B4IS_B3_X   			0	// Left Bottom
#define B4IS_B3_Y				48

#define B4IS_B4_X   			72	// Right Bottom
#define B4IS_B4_Y	    		48

#define B4IS_W   				72
#define B4IS_H					32

// 4 ICON Button Screen without space for SPRITE
#define B4I_B1_X   				0	// Left Top
#define B4I_B1_Y	    		16

#define B4I_B2_X   				80	// Right Top
#define B4I_B2_Y				16

#define B4I_B3_X   				0	// Left Bottom
#define B4I_B3_Y				48

#define B4I_B4_X   				80	// Right Bottom
#define B4I_B4_Y	    		48

#define B4I_W   				80
#define B4I_H					32

// 6 ICON Button Screen
#define B6I_B1_X   				0	// Left Top
#define B6I_B1_Y	    		16

#define B6I_B2_X   				53	// Middle Top
#define B6I_B2_Y				16

#define B6I_B3_X   				106	// Right Top
#define B6I_B3_Y				16

#define B6I_B4_X   				0	// Left Bottom
#define B6I_B4_Y	    		48

#define B6I_B5_X   				53	// Middle Bottom
#define B6I_B5_Y	    		48

#define B6I_B6_X   				106	// Right Bottom
#define B6I_B6_Y	    		48


#define B6I_W   				53
#define B6I_H					32

#define ICON_W   				32
#define ICON_H					16

// CANCEL Button ('X' button in corner)
#define CLOSE_X					143
#define CLOSE_Y					0
#define CLOSE_W					18
#define CLOSE_H					15


// BACK Button
#define BACK_X 					64
#define BACK_Y 					0
#define BACK_W 					30
#define BACK_H 					9


// FORWARD or More Button
#define NEXT_X 	   				96
#define NEXT_Y 	   				0
#define NEXT_W 	   				30
#define NEXT_H 					9

// BACK Button 2
#define BACK2_X 				128 - 16
#define BACK2_Y 				Bv_B1_Y
#define BACK2_W 				15
#define BACK2_H 				10

// FORWARD or More Button 2
#define NEXT2_X 	   			128 - 16
#define NEXT2_Y 	   			Bv_B5_Y + 2
#define NEXT2_W 	   			15
#define NEXT2_H 				10


// Title Box
#define TITLE_X 				0
#define TITLE_Y  				0
#define TITLE_N_W 				62 	// Half Title Width
#define TITLE_F_W 				128	// Full Title Width
#define TITLE_NC_W   			128	// Full Title NO Cancel Btn
#define TITLE_H 				9



// Input Area for Num Entry Input Screen
#define INPUT_X 				0
#define INPUT_Y 				38  // Normal Input
#define INPUTA_Y 				0	// Alpha Input
#define INPUT_W					126
#define INPUT_H					13  // Normal Height
#define INPUTA_H				14	// Alpha Height
#define INPUT_S					2	// Border size

// Alphanum Screen Buttons
#define ANUM_Y					64
#define ANUM_H    				16

// Extended Key
#define ANUM_B1_X				0
#define ANUM_B1_W				20

// Back Arrow
#define ANUM_B2_X				22
#define ANUM_B2_W				44

// Clear Key
#define ANUM_B3_X				68
#define ANUM_B3_W				44

// Enter Key
#define ANUM_B4_X				114
#define ANUM_B4_W				44

// Second and Third Alphanum Screen Buttons
#define ANUMN_Y					64
#define ANUMN_H    				16

// Extended Key
#define ANUMN_B1_X				0
#define ANUMN_B1_W				41

// SPACE Key
#define ANUMN_B2_X				40
#define ANUMN_B2_W				81

// Back Arrow
#define ANUMN_B3_X				63
#define ANUMN_B3_W				44

// Full Text Window below Title
#define T_W_X					0
#define T_W_Y					10
#define T_W_W					128
#define T_W_H					54
#define T_W_SH					28

// Full Window
#define FW_X					0
#define FW_Y					0
#define FW_W					128
#define FW_H					63

// Used to define indent of text areas
#define TEXT_INDENT				0

#define SHOWAD_FLG				13

//*********************************************
//  Defines for  the ICON Screen Button Logic
//*********************************************

// 4 ICON Button Screen text
#define ITB4_B1_X   			B4I_B1_X	// Top
#define ITB4_B1_Y	    		B4I_B1_Y

#define ITB4_B2_X   			B4I_B2_X	// Next to top
#define ITB4_B2_Y				B4I_B2_Y

#define ITB4_B3_X   			B4I_B3_X	// Next to bottom
#define ITB4_B3_Y				B4I_B3_Y

#define ITB4_B4_X   			B4I_B4_X	// Bottom
#define ITB4_B4_Y	    		B4I_B4_Y

#define ITB4_W   				B4I_W
#define ITB4_T_H				(B4I_H-ICON_H)	// Text Height

// 4 ICON Button images
#define ICON4_1_X   			(B4I_B1_X+B4I_W/2-ICON_W/2)	// First
#define ICON4_1_Y	    		(B4I_B1_Y+ICON_H-2)

#define ICON4_2_X   			(B4I_B2_X+B4I_W/2-ICON_W/2)	// Second
#define ICON4_2_Y				(B4I_B2_Y+ICON_H-2)

#define ICON4_3_X   			(B4I_B3_X+B4I_W/2-ICON_W/2)	// Third
#define ICON4_3_Y				(B4I_B3_Y+ICON_H-2)

#define ICON4_4_X   			(B4I_B4_X+B4I_W/2-ICON_W/2)	// Fourth
#define ICON4_4_Y	    		(B4I_B4_Y+ICON_H-2)

// 4 ICON Button Screen text with Sprite
#define ITB4S_B1_X   			B4IS_B1_X	// Top
#define ITB4S_B1_Y	    		B4IS_B1_Y

#define ITB4S_B2_X   			B4IS_B2_X	// Next to top
#define ITB4S_B2_Y				B4IS_B2_Y

#define ITB4S_B3_X   			B4IS_B3_X	// Next to bottom
#define ITB4S_B3_Y				B4IS_B3_Y

#define ITB4S_B4_X   			B4IS_B4_X	// Bottom
#define ITB4S_B4_Y	    		B4IS_B4_Y

#define ITB4S_W   				B4IS_W
#define ITB4S_T_H				(B4IS_H-ICON_H)	// Text Height

// 4 ICON Button images with Sprite
#define ICON4S_1_X   			(B4IS_B1_X+B4IS_W/2-ICON_W/2)	// First
#define ICON4S_1_Y	    		(B4IS_B1_Y+ICON_H-2)

#define ICON4S_2_X   			(B4IS_B2_X+B4IS_W/2-ICON_W/2)	// Second
#define ICON4S_2_Y				(B4IS_B2_Y+ICON_H-2)

#define ICON4S_3_X   			(B4IS_B3_X+B4IS_W/2-ICON_W/2)	// Third
#define ICON4S_3_Y				(B4IS_B3_Y+ICON_H-2)

#define ICON4S_4_X   			(B4IS_B4_X+B4IS_W/2-ICON_W/2)	// Fourth
#define ICON4S_4_Y	    		(B4IS_B4_Y+ICON_H-2)

// 6 ICON Button Screen text
#define ITB6_B1_X   			B6I_B1_X	// Left Top
#define ITB6_B1_Y	    		B6I_B1_Y

#define ITB6_B2_X   			B6I_B2_X	// Right Top
#define ITB6_B2_Y				B6I_B2_Y

#define ITB6_B3_X   			B6I_B3_X	// Left Middle
#define ITB6_B3_Y				B6I_B3_Y

#define ITB6_B4_X   			B6I_B4_X	// Right Middle
#define ITB6_B4_Y	    		B6I_B4_Y

#define ITB6_B5_X   			B6I_B5_X	// Left Bottom
#define ITB6_B5_Y	    		B6I_B5_Y

#define ITB6_B6_X   			B6I_B6_X	// Right Bottom
#define ITB6_B6_Y	    		B6I_B6_Y

#define ITB6_W   				B6I_W
#define ITB6_H					(B6I_H-ICON_H)

// 6 ICON Button images
#define ICON6_1_X   			(B6I_B1_X+B6I_W/2-ICON_W/2)	// Left Top
#define ICON6_1_Y	    		(B6I_B1_Y+ICON_H-2)

#define ICON6_2_X   			(B6I_B2_X+B6I_W/2-ICON_W/2)	// Right Top
#define ICON6_2_Y				(B6I_B2_Y+ICON_H-2)

#define ICON6_3_X   			(B6I_B3_X+B6I_W/2-ICON_W/2)	// Left Middle
#define ICON6_3_Y				(B6I_B3_Y+ICON_H-2)

#define ICON6_4_X   			(B6I_B4_X+B6I_W/2-ICON_W/2)	// Right Middle
#define ICON6_4_Y	    		(B6I_B4_Y+ICON_H-2)

#define ICON6_5_X   			(B6I_B5_X+B6I_W/2-ICON_W/2)	// Left Bottom
#define ICON6_5_Y	    		(B6I_B5_Y+ICON_H-2)

#define ICON6_6_X   			(B6I_B6_X+B6I_W/2-ICON_W/2)	// Right Bottom
#define ICON6_6_Y	    		(B6I_B6_Y+ICON_H-2)

// Dial Mode 2 Button Screen text
#define DMB2_B1_X   			(SCR_PIXEL_WIDTH/4-B4_W*3/8)	// First
#define DMB2_B1_Y	    		B4_B3_Y

#define DMB2_B2_X   			(SCR_PIXEL_WIDTH*3/4-B4_W*3/8)	// Second
#define DMB2_B2_Y				B4_B4_Y

#define DMB2_W   				(B4_W*3/4)
#define DMB2_H					(B4_H*2/3)	// Text Height

#define DMB2I_B1_X   			(SCR_PIXEL_WIDTH/4-B4_W*3/8+1)	// First
#define DMB2I_B1_Y	    		(B4_B3_Y+1)

#define DMB2I_B2_X   			(SCR_PIXEL_WIDTH*3/4-B4_W*3/8+1)	// Second
#define DMB2I_B2_Y				(B4_B4_Y+1)

#define DMB2I_W   				(B4_W*3/4-2)
#define DMB2I_H					(B4_H*2/3-2)	// Text Height

// Line Type 4 Button Screen text
#define LTB4_B1_X   			B4_B1_X	// Top
#define LTB4_B1_Y	    		(B4_B1_Y+B6_H*2/3-4)
#define LTB4_B1_W   			(B4_W*3/4+2)

#define LTB4_B2_X   			(B4_B2_X-10)	// Next to top
#define LTB4_B2_Y				(B4_B2_Y+B6_H*2/3-4)
#define LTB4_B2_W   			(B4_W*5/4-2)

#define LTB4_B3_X   			B4_B3_X	// Next to bottom
#define LTB4_B3_Y				(B4_B3_Y+B6_H/3-2)
#define LTB4_B3_W   			(B4_W*3/4+2)

#define LTB4_B4_X   			(B4_B4_X-10)	// Bottom
#define LTB4_B4_Y	    		(B4_B4_Y+B6_H/3-2)
#define LTB4_B4_W   			(B4_W*5/4-2)

#define LTB4_H					(B6I_H*2/3)	// Text Height

// Miscellaneous
#define INFO_H                  (FW_H-2*TITLE_H)	// Information Box Height



//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================


#endif // _SCREEN_H_
