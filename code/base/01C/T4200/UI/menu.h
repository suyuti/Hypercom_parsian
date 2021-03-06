//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      menu.h
//
// File Description:
//      Menu driver defintions
//
//=============================================================================

#ifndef _MENU_H_
#define _MENU_H_

//=============================================================================
// Public defines and typedefs
//=============================================================================

//-----------------------------------------------------------------------------
//
//!	WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//!
//!	Suggest to do NOT use conditional compilation while defining the Menu Ids.
//!	Having or not the definitions will not change the size of the application,
//!
//!	If MENU_ID does not change depending on compilation option, 
//!	MENU_IDLE_NEWTRANS always will be 0x1027 does not matter if the previous
//!	menus are used or not. This condition simplifies the TermMaster settings,
//!	in the table ICE Buttons. TermMaster does not have to know how the
//!	application was compiled, to decide which are the desired buttons in
//!	the Idle Menu.
//!
//	WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//
//-----------------------------------------------------------------------------

//! Menu ID's
typedef enum
{
	//! 0x1000 -  Place holder for start
	MENU_START = 0x1000,
	//! 0x1001 -  Main menu
	MENU_MAIN,
	//! 0x1002 -  Menu for transactions
	MENU_NEWTRANS,
	//! 0x1003 -  Batch menu
	MENU_BATCH,					
	//! 0x1004 -  Terminal options menu
	MENU_TERMINAL,				
	//! 0x1005 -  Setup menu
	MENU_SETUP,					
	//! 0x1006 -  Test menu
	MENU_TESTS,					
	//! 0x1007 -  Terminal setup menu
	MENU_TERM_SETUP,			
	//! 0x1008 -  Printer setup menu
	MENU_PRINT_SETUP,			
	//! 0x1009 -  Password options menu
	MENU_PASSWORD,				
	//! 0x100A -  Terminal view menu
	MENU_TERMVIEW,				
	//! 0x100B -  Terminal version menu
	MENU_TERMVERSION,			
	//! 0x100C -  Terminal reports menu
	MENU_REPORTS,				
	//! 0x100D -  Terminal reprint menu
	MENU_REPRINT,				
	//! 0x100E -  Terminal paperfeed menu
	MENU_PAPERFEED,				
	//! 0x100F -  Cashier report menu
	MENU_CASHIER_REPORT,		
	//! 0x1010 -  Cashier menu
	MENU_CASHIER,				
	//! 0x1011 -  Host menu for audit reports
	MENU_HOST_AUDIT_REPORT,		
	//! 0x1012 -  Host menu for batch settle
	MENU_HOST_BATCH_SETTLE,		
	//! 0x1013 -  Host logon menu
	MENU_HOST_LOGON,			
	//! 0x1014 -  Host batch control menu (function 33)
	MENU_HOST_BATCH_CONTROL,	
	//! 0x1015 -  Host test transaction menu
	MENU_HOST_TESTTRAN,			
	//! 0x1016 -  Host maintenance function menu
	MENU_MAINT_FUNC,			
	//! 0x1017 -  Host maintinacne clear menu
	MENU_MAINT_CLEAR,			
	//! 0x1018 -  Host clear acquirer menu
	MENU_HOST_CLEAR_ACQUIRER,	
	//! 0x1019 -  Passwords On/Off menu
	MENU_PASSWORDS_ONOFF,		
	//! 0x101A -  menu for local options
	MENU_LOCALOPTS,				
	//! 0x101B -  Previous menu in stack
	MENU_PREVIOUS,				
	//! 0x101C -  Next menu
	MENU_NEXT,					
	//! 0x101D -  Redraw the last menu, do not save on menu stack again
	MENU_REDRAW,				
	//! 0x101E -  Tab Menu
	MENU_TAB,					
	//! 0x101F -  Menu for credit, debit or EBT selection
	MENU_CREDIT_DEBIT_EBT,		
	//! 0x1020 -  Checks menu
	MENU_CHECK,					
	//! 0x1021 -  RMRS check menu
	RMRS_MENU_CHECK,			
	//! 0x1022 -  RMRS ACH check menu
	RMRS_MENU_CHECK_ACH,		
	//! 0x1023 -  Shift menu
	MENU_SHIFT,					
	//! 0x1024 -  Menu for debit or EBT selection
	MENU_DEBIT_EBT,				
	//! 0x1025 -  Menu for host batch clearing
	MENU_HOST_BATCH_CLEAR,		
	//! 0x1026 -  MENY_EVENT_TYPE -New Funtion Idle Button Screen
	MENU_EVENT_TYPE,			
	//! 0x1027 -  MENU_IDLE_NEWTRAN - -New Transaction in Idle Button Screen
	MENU_IDLE_NEWTRANS,			
	//! 0x1028 -  MENU_IDLE_NEWFUNC -New Funtion Idle Button Screen
	MENU_IDLE_NEWFUNC,			
	//! 0x1029 -  MENU_IDLE_INITCHG -New Funtion Idle Button Screen
	MENU_IDLE_INITCHG,			
	//! 0x102A -  Menu to Select Button
	MENU_SELECT_BTN,			
	//! 0x102B -  Menu Config Report
	MENU_CONFIG_RPT,			
	//! 0x102C -  Menu for Foot Messages
	MENU_MESSAGES,				
	//! 0x102D -  Utility menu for RDM check scanner
	MENU_RDMUTILS,				
	//! 0x102E -  Menu History Report
	MENU_HISTORY_RPT,           
	//! 0x102F -  Biometric Enrollment
	MENU_BIOENROLL,				
	//! 0x1030 -  Switch Bio-transaction
	MENU_SWITCH_BIOTRANS,		
	//! 0x1031 -  Check Reader Test
	MENU_CHECKREADER_TEST,		
	//! 0x1032 -  Current/Previous day menu
	MENU_CURR_PREV_DAY,			
	//! 0x1033 -  T7PLUS CVV2 menu
	MENU_CVV2,					
	//! 0x1034 -  Acquirer Delete/Reset menu
	MENU_HOST_DELRES,			
	//! 0x1035 -  Acquirer Delete or Reset?
	MENU_DELORRES,				
	//! 0x1036 -  Acquirer Reset Security Error
	MENU_HOST_RESERR,			
	//! 0x1037 -  Menu to select dialing mode
	MENU_DIAL_MODE,				
	//! 0x1038 -  COMMS: Interface menu
	RS232_CHOOSE_MODEM_RS,		
	//! 0x1039 -  COMMS: Protocol menu
	RS232_PROT_MENU,			
	//! 0x103A -  COMMS: Bit Rate menu
	RS232_BAUD_MENU,			
	//! 0x103B -  COMMS: Stop Bits menu
	RS232_STOP_MENU,			
	//! 0x103C -  COMMS: Data Bits menu
	RS232_DATA_MENU,			
	//! 0x103D -  COMMS: Parity menu
	RS232_PARITY_MENU,			
	//! 0x103E -  Test print menu
	TEST_PRINT_MENU,			
	//! 0x103F -  COMMS: Flow Control menu
	RS232_FLOWC_MENU,			
	//! 0x1040 -  Language selection menu
    LANGUAGE_MENU,              
	//! 0x1042 -  Serial driver trace
    MENU_TRACE,                   
	//! 0x1043 -  Init Preferred Service
    MENU_INIT_PREFERRED,        
	//! 0x1044 -  NMS Preferred Service
    MENU_NMS_PREFERRED,         
	//! 0x1045 -  Settlement status menu
    MENU_SETTLE_STATUS,  
    //! 0x1046 - Set TM Communication parameter menu 
    MENU_SET_TMCOMPARAM,   
    MENU_HOST_INFO_TYPE,
    MENU_TRX_PRIM_SECOND,
    MENU_SETTLE_PRIM_SECOND,
    MENU_SET_PRIM_PARAM,
    MENU_SET_SECOND_PARAM,  
    MENU_IPADDRESS,
    MENU_COMTYPE,

	//! Max meaning
    MENU_MAX
}
MENU_ID;


//=============================================================================
// Public data declarations
//=============================================================================
//! number of button on menu
extern UBYTE ButtonCnt;


//=============================================================================
// Public function declarations
//=============================================================================
//! Draws given menu
extern void DrawMenu( MENU_ID MenuNumber );	
//! Clears menu stack
extern void ClearMenus( void );	
//! Funciton to draw menu based on menu id
extern void XDrawMenu( void );

#endif // _MENU_H_
