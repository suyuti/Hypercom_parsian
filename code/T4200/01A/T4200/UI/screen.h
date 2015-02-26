//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      screen.h
//
// File Description:
//      Screen information
//
//=============================================================================

#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "sdk.h"
#include "msg.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================
//! Screen ID
typedef enum
{
	//! place holder for start
	SCREEN_START = 0x2000,		
	//! IDLE Screen
	IDLE_SCR,					
	//! ININT Screen
	INIT_SCR,					
	//! Status Screen
	STATUS_SCR,					
	//! Status Screen with card sprite
	CARDSWIPE_SCR,				
	//! Set Contrast Scr
	SETCONTRAST_SCR,			
	//! Input Screen
	INPUT_SCR,					
	//! YES/NO Screen
	YESNO_SCR,					
	//! Numeric Entry Scr
	NUMENTRY_SCR,				
	//! Special amount Entry Scr
	AMTENTRY_SCR,				
	//! REVIEW Screen
	REVIEW_SCR,					
	//! template for Yes/No screens
	TEMPLATE_2BTNQUERY_SCR,		
	//! Signature Capture Screen
	SIGNATURE_SCR,				
	//! Standard 5 button text only menu
	MENU_5BTNTEXT_SCR,			
	//! Standard 10 button text only menu
	MENU_10BTNTEXT_SCR,			
	//! Continue Screen
	CONTINUE_SCR,				
	//! Alpha numeric entry 1 (Numeric keypad)
	ALPHANUM1_SCR,				
	//! Alpha numeric entry 2 (Capital Alphabetic keypad)
	ALPHANUM2_SCR,				
	//! Alpha numeric entry 3 (Small Alphabetic keypad)
	ALPHANUM3_SCR,				
	//! Alpha numeric entry 4 (Symbols keypad)
	ALPHANUM4_SCR,				
	//! Report Totals Scr
	RTOTALS_SCR,				
	//! Title Bar
	TITLEBAR_SCR,				
	//! Batch Info Screen
	BATCHINFO_SCR,				
	//! Cardreader Test Scr
	CARDTEST_SCR,				
	//! descriptor entry screen
	DESCRIPTOR_SCR,				
	//! INFO Message Screen
	INFO_SCR,					
	//! ERROR Message Screen
	ERROR_SCR,					
	//! ICE PAK Idle Screen
	ICEPAK_IDLE_SCR,			
	//! Diag Test Function Screen
	TEST_SCR,					
	//! Five Line Info Screen
	SHOW_INFO_SCR,				
	//! Display Test Screen
	TST_DSPLY_SCR,				
	//! Dial Mode Screen
	DIAL_MODE_SCR,				
	//! Flexable 2 button Yes/No Screen
	TWO_BTN_SCR,				
	//! View Servers Screen
	SERVER_SCR,					
	//! Amount Proc's Cash/Tender Screen
	CASH_TEND_SCR,				
	//! Screen for internal Pin Pad Select Account
	SELACCT_SCR,				
	//! Screen for Receipt Header/Footer Text
	HEADER_FOOTER_SCR,			
	//! PreTip 2 to 4 button screen
	TIP_PERCENT_SCR,			
	//! Status of Memory Page Screen
	MEM_STATUS_SCR,				
	//! Software Version Screen
	SOFT_VER_SCR,				

#ifdef MAKE_EMV
	//! EMV Level 1 & 2 versions
	EMV_VER_SCR,			
	//! Pin entry on external pinpad prompt
	PINWAIT_SCR,			
#endif
	//! Libraries Version Screen
	LIBS_VER_SCR,				
	//! Set Local Tip Screen
	SET_TIP_SCR,				
	//! Network Statistics Screen
	STATISTICS_SCR,				
	//! Settlement Status Info Screen
	SETTLE_INFO_SCR,			
	//! CVV2 Indicator Screen
	CVV2_IND_SCR,				
	//! Select the current Language
	LANG_SELECT_SCR,			
	//! Confirm the current Language
	CONFIRM_LANG_SCR,			
	//! Yes/Next screen
	YES_NEXT_SCR,	
    //! IP Entry Scr
    IPNUM_SCR,
    WARNING_SCR,
    

	//! Must always stay at the end
	SCREEN_MAX					
}
SCREEN_ID;

//! Screen entry
typedef struct
{
	SCREEN_ID id;
	UWORD *pScript;
}
SCREEN_ENTRY;

//! Screen cross reference
typedef struct
{
	//! address of SCREEN_ENTRY
	SCREEN_ENTRY *pScreenEntry;	
	//! page number of table
	PAGE_NUMBERS Page;			
}
SCREEN_CROSSREF;


//! Defines for button locations used by draw_btns()
#define TOP_Y_OFFSET            0
#define BOT_Y_OFFSET            0

//! Full Window
#define FW_X					0
#define FW_Y					0
#define FW_W					GUI_SCREEN_WIDTH
#define FW_H					(GUI_SCREEN_HEIGHT-(TOP_Y_OFFSET+BOT_Y_OFFSET))

//! Full Text Window below Title
#define T_W_X					0
#define T_W_Y					(FW_Y+10)
#define T_W_W					FW_W
#define T_W_H					(FW_H-10)
#define T_W_SH					28


#define ICON_W   				32
#define ICON_H					16

//! CANCEL Button ('X' button in corner)
#define CLOSE_W					18
#define CLOSE_H					15
#define CLOSE_X					(FW_W-CLOSE_W)
#define CLOSE_Y					TOP_Y_OFFSET


// !BACK Button
#define BACK_X 					(FW_W-(2*ICON_W))
#define BACK_Y 					TOP_Y_OFFSET
#define BACK_W 					30
#define BACK_H 					9


//! FORWARD or More Button
#define NEXT_X 	   				(FW_W-(1*ICON_W))
#define NEXT_Y 	   				TOP_Y_OFFSET
#define NEXT_W 	   				30
#define NEXT_H 					9


//! Title Box
#define TITLE_X 				0
#define TITLE_Y  				TOP_Y_OFFSET
#define TITLE_S_GAP				15
#define TITLE_S_X 				TITLE_S_GAP
//! Half Title Width
#define TITLE_S_W 				(FW_W-2*TITLE_S_GAP)
//! Half Title Width
#define TITLE_N_W 				(FW_W/2)
//! Full Title Width
#define TITLE_F_W 				FW_W  
//! Full Title NO Cancel Btn
#define TITLE_NC_W   			FW_W  
#define TITLE_H 				9



//! Input Area for Num Entry Input Screen
//! Border size
#define INPUT_S					2
#define INPUT_X 				0
//! Normal Input
#define INPUT_Y 				(TOP_Y_OFFSET+38)
//! Alpha Input
#define INPUTA_Y 				(TOP_Y_OFFSET+TITLE_H)
#define INPUT_W					(FW_W-INPUT_S)
//! Normal Height
#define INPUT_H					13
//! Alpha Height
#define INPUTA_H				10

#define KEY_IMG_W				128	// Keypad Image	Width
#define KEY_IMG_X			    ((FW_W-KEY_IMG_W)/2)
#define KEY_IMG_Y			    (INPUTA_Y+INPUT_H+INPUT_S)

//! Used to define indent of text areas
#define TEXT_INDENT				0

#define SHOWAD_FLG				13

#define Bn_LEFT                 17

//! Single Button "Continue" type Screens
#define B1_W					80
#define B1_H					20
#define B1_X					((FW_W-80)/2) // 40
#define B1_Y					FW_H-58

//! Buttons vertical offset
#define Bn_H    				16	// Left Top
#define Bn_W    				16	// Left Top
#define Bn_Y                    (TITLE_Y+TITLE_H+2)
#define Bn_X    				(0)

//! Navigate Buttons
#define Bn_B1_X	    			Bn_X
#define Bn_B1_Y	    			Bn_Y
#define Bn_B3_X	    			Bn_X
#define Bn_B3_Y	    			(FW_H-Bn_H)
#define Bn_B2_X	    			Bn_X
#define Bn_B2_Y	    			(Bn_B1_Y+(Bn_B3_Y-Bn_B1_Y)/2)

//! 6 Button Screen
#define Bv_B1_X   				Bn_LEFT	// Left Top
#define Bv_B1_Y	    			Bn_Y

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

#define Bv_W   					FW_W - 2*Bv_B1_X
#define Bv_H					11

//! 10 Button Screen
#define B10_LEFT_OFFSET			Bn_LEFT
#define B10_RIGHT_OFFSET		0
#define B10_X_GAP			    2
#define B10_W					((FW_W-(B10_LEFT_OFFSET+B10_RIGHT_OFFSET+B10_X_GAP))/2)
#define B10_X_LEFT				B10_LEFT_OFFSET
#define B10_X_RIGHT				(B10_X_LEFT+B10_W+B10_X_GAP)

#define B10_M_LEFT				(B10_X_LEFT+B10_W/2)
#define B10_M_RIGHT				(B10_X_RIGHT+B10_W/2)

//! Dialog 3 Buttons

#define BDv_HG   				2	// horizontal gap
#define BDv_H   				11
#define BDv_W	    			((FW_W-4*BDv_HG)/3) //40

#define BDv_B1_X   				BDv_HG	// Left
#define BDv_B1_Y	    		(FW_H-BDv_H)

#define BDv_B2_X   				(BDv_W+2*BDv_HG)	// Middle
#define BDv_B2_Y				(FW_H-BDv_H)

#define BDv_B3_X   				(2*BDv_W+3*BDv_HG)	// Right
#define BDv_B3_Y				(FW_H-BDv_H)

#define CTL_BTN1_X				BDv_B1_X+1
#define CTL_BTN2_X				BDv_B2_X+1
#define CTL_BTN3_X				BDv_B3_X+1
#define CTL_BTN_Y				BDv_B1_Y+1

#define CTL1_X_OFFSET			5
#define CTL1_BTN_H				Bn_H
#define CTL1_BTN_W				(FW_W-2*CTL1_X_OFFSET)
#define CTL1_BTN_X				CTL1_X_OFFSET
#define CTL1_BTN1_Y				(TITLE_Y+TITLE_H+12)
#define CTL1_BTN2_Y				(CTL1_BTN1_Y+1*(CTL1_BTN_H+2))
#define CTL1_BTN3_Y				(CTL1_BTN1_Y+2*(CTL1_BTN_H+2))

//*********************************************
//  Defines for  the ICON Screen Button Logic
//*********************************************

//! Miscellaneous
#define INFO_H                  (FW_H-2*TITLE_H)	// Information Box Height

// Information Box
#define IB_GAP                  0
#define IB_X                    IB_GAP
#define IB_Y                    (TITLE_H+2)
#define IB_H                    (FW_H-IB_Y)
#define IB_W                    (FW_W-(2*IB_GAP))

//! Error Box
#define EB_B                    2                   // Border width
#define EB_BC                   (EB_B/2)
#define EB_X_OFFSET             10                  // box side gap (at the left and at the right)
#define EB_W                    (FW_W-2*EB_X_OFFSET)
#define EB_H                    50
#define EB_X                    EB_X_OFFSET
#define EB_Y                    ((FW_H-EB_H)/2)
#define EB_TH                   9
#define EB_MH                   20
#define EB_B_W                  BDv_W
#define EB_B_H                  BDv_H
#define EB_B_X                  (EB_X+(EB_W-EB_B_W)/2)
#define EB_B_Y                  (EB_Y+EB_H-(EB_B_H+5))

//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================


#endif // _SCREEN_H_

