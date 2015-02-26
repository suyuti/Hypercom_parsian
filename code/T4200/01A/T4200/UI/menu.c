//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      menu.c
//
// File Description:
//      Menu driver functions
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
#include "fixdata.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "transdef.h"
#include "msg.h"
#include "pinpad.h"
#include "util.h"
#include "utllib.h"
#include "icepak.h"
#include "screen.h"
#include "sprite.h"
#include "tables.h"
#include "prtdev.inc"
#include "siodev.inc"
#include "rs232tab.h"
#include "runtime.h"

#include "appdata.h"


//=============================================================================
// External variables / declarations
//=============================================================================

//=============================================================================
// Private defines and typedefs
//=============================================================================
//! Menu definitons
#define MAX_MENU_LEVELS				10
#define NEXT_ENABLE_FLAG			0
#define PREV_ENABLE_FLAG			11


static UBYTE PreAuthCancel( void );
static UBYTE EnableDiscount( void );

//! Menu type
typedef enum
{
    //! 10 buttons, text menu
    MT_10BTN,
    //! 5 buttons, text menu
    MT_5BTN,                    
}
MENU_TYPE;

//! Munu items structure
typedef struct
{
    //! enable function this button, return non-zero for enable, else disable
    UBYTE( *pEnableFunc ) ( void ); 
    //! button label line 1
    enum msg_id msgID1; 
    //! button label line 2 (if used)
    enum msg_id msgID2;  
    //! event for this button
    UWORD eventID;              
}
MENUITEM;

//! Menu entry
typedef struct
{
    //! Menu id of this table from transdef.h
    MENU_ID menuID;         
    //! Overall enable function return non-zero for enable, else disable
    UBYTE( *pEnableFunc ) ( void );
    //! menu title
    enum msg_id TitleMsg;  
    //! pointer to list of menu item (buttons) on this menu
    MENUITEM *pMenuList;     
    //! size in items of MENUITEM list pointed to by pMenuList
    UBYTE Size;
    //! menu type as defined by enum MENU_TYPE
    MENU_TYPE Type;             
}
MENU_ENTRY;

//! Menu locator
typedef struct
{
    //! menu entry
    MENU_ENTRY *pMenu;
    //! first menu item
    MENUITEM *pItem;
    //! ending menu item
    MENUITEM *pEndItem;
    //! stored item
    UBYTE   MenuItemStored;
}
MENU_LOCATOR;


//=============================================================================
// Private function declarations
//=============================================================================
//! Sets up button labels and events and then draws the given menu
static void DrawMenuType( MENU_TYPE type );
//! Tests cashier menu item if required
static UBYTE EnableCashier( void );
//! Tests if tabs are enabled
static UBYTE EnableTab( void );
//! Tests if shift proecessing required
static UBYTE EnableShift( void );
//! Tests if server menu item if required
static UBYTE EnableServer( void );
//! Tests if cashier menu item if required
static UBYTE EnableCashier( void );
//! Tests if till menu item if required
static UBYTE EnableTill( void );
//! Setup dynamic hostmenu
static UBYTE EnableHostAuditReport( void );
//! Setup dynamic hostmenu for settlement
static UBYTE EnableHostBatchSettle( void );
//! Setup dynamic hostmenu
static UBYTE EnableHostLogon( void );
//! Simply set CSTMSG to title for Cashier Menu and return 1
static UBYTE SetCashierMenuTitle( void );
//! Enable debit selection if available
static UBYTE EnableDebit( void );
//! Disable a menu item
static UBYTE Disable( void );
//! Pop menu off stack
static void PopMenu( MENU_LOCATOR * pMenuLoc );
//! Push Menu on stack
static void PushMenu( MENU_LOCATOR * pMenuLoc );
//! Returns pointer to MENU_ENTRY in menu table based on given menu transaction number.
static MENU_ENTRY *GetMenu( enum transactions menuID );
//! Fills hostMenu[] array with available hosts.
static void SetHostMenu( UBYTE IncludeAll );
//! Set long Title invert box
static UBYTE LongTitleOpt( void );
//! Test if Configuration Report is enabled
static UBYTE EnableConfReport( void );
//! Test if Performance Report is enabled
static UBYTE EnablePerfReport( void );
//! Test if there are some applications
static UBYTE EnableMutiApp( void );
//! Check if telephone modem configured
static UBYTE Check_Modem( void );
//! Check if GSM modem configured
static UBYTE Check_GSM( void );
//! Check if Ethernet module configured
static UBYTE Check_Ethernet( void );
//! Check if not POS LAN module configured
static UBYTE Check_notLAN( void );
//! Check if Dial Backup  module configured
static UBYTE Check_DialBackUp( void );
//! Check if COM2 available
static UBYTE Check_COM2( void );
//! Exit (enabled only in debug buid)
static UBYTE Check_Exit( void );

static UBYTE SelectTrx( void );
static UBYTE SelectSettle( void );
static UBYTE SelectPrimary( void );
static UBYTE SelectSecondary( void );



//=============================================================================
// Public data definitions
//=============================================================================
//! Button counter
UBYTE ButtonCnt = 0; 

//=============================================================================
// Private data definitions
//=============================================================================
//! Current menu locator
static MENU_LOCATOR Current;
//! Previous menu locators array
static MENU_LOCATOR Previous[MAX_MENU_LEVELS];
//! Previous index of a menu
static UBYTE PreviousIndex;
//! Assume next button is enabled
static Bool NextEnabled;
//! Next menu locator
static MENU_LOCATOR Next;


//! Menu definition tables
//!
//!  If the enable conditional function is present and returns non-zero
//!  then the menu item is shown on the screen. If a zero is returned, the
//!  menu item does not appear and is skipped.
//!
//!  If the conditional function is NULL, then the menu item is always
//!  enabled.

//! dynamic menu for hosts
static MENUITEM hostMenu[AQMAX];

// *INDENT-OFF*
//! list of all possible items for Main Menu
static const MENUITEM MainMenu[] = {
//	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_NewTrans,			MENU_NEWTRANS },
	{ NULL,				IMAGE_EMPTY,		N_Batch,			MENU_BATCH },
	{ NULL,				IMAGE_EMPTY,		N_Reports,			REPORTS },
	{ NULL,				IMAGE_EMPTY,		N_Terminal,			MENU_TERMINAL },		
	{ EnableMutiApp,    IMAGE_EMPTY,		N_ChangeApp,        CHG_APP },
	{ NULL,				IMAGE_EMPTY,		N_Setup,			MENU_SETUP },
	{ NULL,				IMAGE_EMPTY,		N_Function,			FUNCTION },
	{ NULL,				IMAGE_EMPTY,		N_Tests,			MENU_TESTS },
	{ NULL,				IMAGE_EMPTY,		N_Reprint,			REPRINT },	
};


//! list of all possible items for New Trans Menu
static const MENUITEM NewTransMenu[] = {
//!	  Enable func		Image				Button label		Event    
	{ NULL,				IMAGE_EMPTY,		N_Sale,			    SALE },
    { NULL,				IMAGE_EMPTY,		N_Void,             VOIDTRAN },
    { NULL,				IMAGE_EMPTY,		N_BalanceInq,       BALINQ },
    { NULL,				IMAGE_EMPTY,		N_Logon,            LOGON },
    { NULL,             IMAGE_EMPTY,		N_Refund,           REFUND },
        // TODO !!!
        // Preauth menüsü çalýþýyo
        //{ NULL,                         IMAGE_EMPTY,            N_PreAuth,              MENU_PREAUTH },
    { NULL,             IMAGE_EMPTY,		N_Settle,           SETTLE }
        //{ NULL,                       IMAGE_EMPTY,            RefundTran,             REFUND},
        //{ NULL,                       IMAGE_EMPTY,            
};

//! list of all possible items for Batch Menu
static const MENUITEM BatchMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		SettleTran,			SETTLE },
	{ NULL,				IMAGE_EMPTY,		N_Review,			REVIEW },		
	{ NULL,				IMAGE_EMPTY,		N_Void,				VOIDTRAN },	
};

//! list of all possible items for Terminal Menu
static const MENUITEM TerminalMenu[] = {
//!	  Enable func		Image				Button label		Event
//###	{ NULL,				IMAGE_EMPTY,        N_Initialize,		INITIALIZE },
    { NULL,				IMAGE_EMPTY,        N_Initialize,       INITIALIZE },
	{ NULL,				IMAGE_EMPTY,		N_ProgLoad,			PROGLD_FUN },	
	{ NULL,				IMAGE_EMPTY,		N_View,				MENU_TERMVIEW },
	{ NULL,				IMAGE_EMPTY,		N_ResetMenu,		RESET_FUN },		
    { NULL,				IMAGE_EMPTY,		N_TrainMode,        TRAINMODE },
};

//! list of all possible items for Terminal Setup Menu
static const MENUITEM TermSetupMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ Check_DialBackUp,	IMAGE_EMPTY,		N_DBInitPref,		DBINITPREF_FUN }, //@AAMELIN
	{ Check_DialBackUp,	IMAGE_EMPTY,		N_DBNMSPref,		DBNMSPREF_FUN }, //@AAMELIN
	{ NULL,				IMAGE_EMPTY,		N_InitPhone,		INITPHONE_FUN },
	{ Check_DialBackUp,	IMAGE_EMPTY,		N_DBInitPhone,		DBINITPHONE_FUN }, //@AAMELIN
	{ NULL,				IMAGE_EMPTY,		N_NMSPhone,		    INITNMS_FUN },
	{ Check_DialBackUp,	IMAGE_EMPTY,		N_DBNMSPhone,		DBNMSPHONE_FUN }, //@AAMELIN
	{ NULL,				IMAGE_EMPTY,		N_DialType,		    DIALTYPE_FUN },
	{ NULL,				IMAGE_EMPTY,		N_PABX,			    PABX_FUN },
	{ NULL,				IMAGE_EMPTY,		N_TermId,		    TERMID_FUN },
	{ NULL,				IMAGE_EMPTY,		N_All,			    TERMALL_FUN },
};

static const MENUITEM TermSetupMenu_Ip[] = {
//	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_IpCon,			IPCONFIG_FUN },
	{ NULL,				IMAGE_EMPTY,		N_NMSIp,			NMSIP_FUN },
	{ NULL,				IMAGE_EMPTY,		N_NMSPort,			NMSPORT_FUN },	
	{ NULL,				IMAGE_EMPTY,		N_InitIp,			INITIP_FUN },
	{ NULL,				IMAGE_EMPTY,		N_InitPort,			INITPORT_FUN },
	{ NULL,				IMAGE_EMPTY,		N_LocalIp,			LOCALIP_FUN },
	{ NULL,				IMAGE_EMPTY,		N_LocalSubnetMask,	LOCALSUBNET_FUN },
	{ NULL,				IMAGE_EMPTY,		N_GatewayIp,		GATEWAYIP_FUN },
	{ NULL,				IMAGE_EMPTY,		N_TermId,			TERMID_FUN },
	{ NULL,				IMAGE_EMPTY,		N_All,				TERMALL_FUN },
};

//! list of all possible items for Printer Setup Menu
static const MENUITEM PrintSetupMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_OnOff,			PRTENABLE_FUN,},
	{ NULL,				IMAGE_EMPTY,		N_Receipt,			RECEIPTTEXT_FUN,},
	{ NULL,				IMAGE_EMPTY,		N_PaperFeed,		PAPERFEED_FUN,},
};

//! list of all possible items for Setup Menu
static const MENUITEM SetupMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_Datime,			SETDT_FUN },
	{ NULL,				IMAGE_EMPTY,		N_Terminal,			TERMSETUP_FUN },
	{ NULL,				IMAGE_EMPTY,		N_Printer,			MENU_PRINT_SETUP },
	{ NULL,				IMAGE_EMPTY,		N_Comms,			COMM_FUN },
	{ NULL,				IMAGE_EMPTY,		N_Passwords,		MENU_PASSWORD },
	{ NULL,				IMAGE_EMPTY,		N_Contras,			SETCONTRAST_FUN},	
	{ NULL,				IMAGE_EMPTY,		Msg_language,		SET_LANGUAGE },
	{ NULL,				IMAGE_EMPTY,		N_IdlBtn,			SET_IDLEBUTTON_FUN	},
};

//! list of all possible items for Test Menu
static const MENUITEM TestsMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_Host,				TESTTRAN_FUN },
	{ NULL,				IMAGE_EMPTY,		N_Display,			TESTDISP_FUN },
	{ NULL,				IMAGE_EMPTY,		N_Keyboard,			TESTKYBD_FUN },
	{ NULL,				IMAGE_EMPTY,		N_Printer,			TESTPRT_FUN },
	{ NULL,				IMAGE_EMPTY,        N_CardRdr,			TESTCRDR_FUN },
	{ NULL,				IMAGE_EMPTY,		N_Pinpad,			TESTPINPAD_FUN },
};

//! list of all possible items for Password Menu
static const MENUITEM PasswordMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_Change,			PSWDCHANGE_FUN },
	{ NULL,				IMAGE_EMPTY,		N_OnOff,			PSWDONOFF_FUN },
};

//! list of all possible items for Terminal View Menu
static const MENUITEM TermViewMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_Version,			MENU_TERMVERSION },
	{ NULL,				IMAGE_EMPTY,		N_Last,				LASTRSP },
};

//! list of all possible items for Terminal Version
static const MENUITEM TermVersionMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_Pinpad,			PINPADVER_FUN },
	{ NULL,				IMAGE_EMPTY,		N_Terminal,			TERMVER_FUN },
};

//! list of all poissible items for Report Menu
static const MENUITEM ReportsMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		ReportMenuAudit,	'C' },
	{ NULL,				IMAGE_EMPTY,		ReportMenuSummary,	'D' },		
    { NULL,             IMAGE_EMPTY,		N_LastSettle,       'I' },
	{ EnablePerfReport, IMAGE_EMPTY,		Prt_PerformBtn, 	'G' },
	{ EnableConfReport, IMAGE_EMPTY,		Prt_ConfigBtn,  	'H' },
    { NULL,				IMAGE_EMPTY,		N_Reprint,		    'F' },	
};

//! list of all poissible items for Reprint Menu
static const MENUITEM ReprintMenu[] = {
//!   Enable function   Button label        Button label    Event
	{ NULL,				IMAGE_EMPTY,		ReprintLast,        'C' },
	{ NULL,				IMAGE_EMPTY,		ReprintInvoice,     'D' },
};

//! list of all poissible items for Paper Feed Menu
static const MENUITEM PaperfeedMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_PaperFeed,		PAPERFEED_FUN },
	{ NULL,				IMAGE_EMPTY,		N_Quit,				CANCEL_KY },
};

//! list of all possible items for Cashier Report Menu
static const MENUITEM CashierReportMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_Detail,			'A' },
	{ NULL,				IMAGE_EMPTY,		N_Summary,			'B' },
	{ NULL,				IMAGE_EMPTY,    	N_Unadjusted,		'C' },
};


//! list of all possible items for Cashier Menu
static const MENUITEM CashierMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_AddRmv,			'D' },
	{ NULL,				IMAGE_EMPTY,		N_View,				'B' },
	{ NULL,				IMAGE_EMPTY,		N_Report,			'A' },
	{ NULL,				IMAGE_EMPTY,		N_Clear,			'C' },
};

//! list of all possible items for Maintenance Function Menu
static const MENUITEM MaintFuncMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_Clear,				MENU_MAINT_CLEAR },
	{ NULL,				IMAGE_EMPTY,		N_PrintHost,			PRTH_FUN },
	{ NULL,				IMAGE_EMPTY,		N_DBRpt,				INITR_FUN },
	{ NULL,				IMAGE_EMPTY,		N_TxRxRpt,				TXRX_FUN },
	{ NULL,				IMAGE_EMPTY,		N_TraceLog,				MENU_TRACE },
	{ NULL,             IMAGE_EMPTY,        N_DBFormattedReport,	MENU_DBFORMATTED},
};


//! list of all possible items for Maintenance Clear Menu
static const MENUITEM MaintClearMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_Reversals,		CLEAR_REV_FUN },
	{ NULL,				IMAGE_EMPTY,		N_Batch,			CLEAR_AQ_FUN  },
	{ NULL,				IMAGE_EMPTY,		Msg_Evrythg,		CLEAR_ALL_FUN },
};

//! list of all possible items for On/Off Password Menu
static const MENUITEM PasswordOnOffMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_Void,				'V' },
	{ NULL,				IMAGE_EMPTY,		RefundTran,			'R' },
	{ NULL,				IMAGE_EMPTY,		N_Adjust,			'A' },
	{ NULL,				IMAGE_EMPTY,		N_Report,			'P' },
	{ NULL,				IMAGE_EMPTY,		N_All,				'L' },
};

//! list of all possible items for Local Options Menu
static const MENUITEM LocalOptionsMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,             IMAGE_EMPTY,        N_Passwords,        'P',},
//	{ NULL,             IMAGE_EMPTY,        N_Dial,             'D',},	
};

static const MENUITEM TabMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_Open,				'O' },
	{ NULL,				IMAGE_EMPTY,		N_Close,			'C' },
	{ NULL,				IMAGE_EMPTY,		N_Print,			'P' },
};

static const MENUITEM CreditDebitEbtMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ EnableDebit,		IMAGE_EMPTY,		N_Debit,			'D' },
	{ NULL,				IMAGE_EMPTY,		N_Credit,			'C' },
};

static const MENUITEM ShiftMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ NULL,				IMAGE_EMPTY,		IncrementShift,		'I' },
	{ NULL,				IMAGE_EMPTY,		ResetShift,			'R' },
};

static const MENUITEM DebitEbtMenu[] = {
//!	  Enable func		Image				Button label		Event
	{ EnableDebit,		IMAGE_EMPTY,		N_Debit,			'D' },
};

static const MENUITEM EventTypeMenu[] =
{
//!	  Enable function	Button label		Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_NewTrans,			'T'	},
	{ NULL,				IMAGE_EMPTY,		N_Function,			'F'	},
};

//! List of all possible items for New Trans Menu
static const MENUITEM IdleNewTransMenu[] =
{
//!	  Enable function	Button label		Button label		Event
	{ NULL,				IMAGE_EMPTY,  		N_Sale,				'A'	},
	{ NULL,				IMAGE_EMPTY,		RefundTran,			'B'	},
	{ NULL,				IMAGE_EMPTY,		N_Void,				'C'	},
	{ EnableDebit,		IMAGE_EMPTY,		N_Debit,			'D'	},
	{ NULL,				IMAGE_EMPTY,		SettleTran,			'E'	},
	{ NULL,				IMAGE_EMPTY,		N_CardVer,			'F'	},
	{ NULL,				IMAGE_EMPTY,		N_Force,			'G'	},	
	{ NULL,				IMAGE_EMPTY,		N_TrainMode,		'J'	},
};

//! List of all possible items for New Trans Menu
static const MENUITEM  IdleNewFuncMenu[] =
{
//!	  Enable function	Button label		Button label		Event
    { NULL,             IMAGE_EMPTY,        N_Review,           'A'},
    { NULL,             IMAGE_EMPTY,        N_Find,             'B'},
    { NULL,             IMAGE_EMPTY,        N_Function,         'C'},
    { NULL,             IMAGE_EMPTY,        N_Reprint,          'D'},
    { NULL,             IMAGE_EMPTY,        N_Reports,          'E'},
    { EnableShift,      IMAGE_EMPTY,        N_Shift,            'F'},
    { NULL,             IMAGE_EMPTY,        Msg_language,       'G'},
#ifndef MAKE_OPTIMUM
    { NULL,             IMAGE_EMPTY,        N_ChangeApp,        'H'},
#endif
};

//! List of all possible items for New Trans Menu
static const MENUITEM IdleInitMenu[] =
{
//!	  Enable function	Button label		Button label		Event
    { NULL,             IMAGE_EMPTY,        N_InitIdlBtn,       'A'},
    { NULL,             IMAGE_EMPTY,        N_ChgIdlBtn,        'B'},
};
//! Menu to select the Idle button
static const MENUITEM SelectBtn[] =
{
//!	  Enable function	Button label		Button label		Event
	{ NULL,				IMAGE_EMPTY,		B_1,                '1'	},
	{ NULL,				IMAGE_EMPTY,		B_2,                '2'	},
	{ NULL,				IMAGE_EMPTY,		B_3,                '3'	},
	{ NULL,				IMAGE_EMPTY,		B_4,                '4'	},

};


//! List of all possible items for Configuration table
static const MENUITEM ConfigReport[] =
{
//!	  Enable function	Button label		Button label		Event
	{ NULL,				IMAGE_EMPTY,		Issuer_Tab,			'B'	},
	{ NULL,				IMAGE_EMPTY,		Acq_Tab,			'C'	},
	{ NULL,				IMAGE_EMPTY,		Card_Tab,			'D'	},
	{ NULL,				IMAGE_EMPTY,		All_Tab,			'E'	},
};


//! List of all possible items for History Report
static const MENUITEM HistoryReport[] =
{
//!	  Enable function	Button label		Button label		Event
//	{ NULL,				IMAGE_EMPTY,		N_Last_5,	    	'A'	},
//	{ NULL,				IMAGE_EMPTY,		N_Last_10,			'B'	},
//	{ NULL,				IMAGE_EMPTY,		N_Last_15,			'C'	},
//	{ NULL,				IMAGE_EMPTY,		N_Last_30,			'D'	},
//	{ NULL,				IMAGE_EMPTY,		Btn_Delete,			'E'	},
};

static const MENUITEM CheckReaderTestMenu[] = {
//!	  Enable func		Image				Button label		Event
//	{ NULL,				IMAGE_EMPTY,		N_Magtek,			'M' },
};



//! List of all possible items for Dial Mode menu
static const MENUITEM  DialModeMenu[] =
{
//!	  Enable function	Button label		Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_Tone,  			'A'	},
	{ NULL,				IMAGE_EMPTY,		N_Pulse, 			'B'	},
};

//! List of all possible items for GSM/GPRS preferred service menu
static const MENUITEM  PrefServiceMenu[] =
{
//!	  Enable function	Button label		Button label		Event
	{ NULL,			IMAGE_EMPTY,		N_GSMwDial,		'A'	},
	{ NULL,			IMAGE_EMPTY,		N_DialwGSM,		'B'	},
	{ NULL,			IMAGE_EMPTY,		N_GSMonly, 		'C'	},
	{ NULL,			IMAGE_EMPTY,		N_Dialonly,		'D'	},
};

//! Choose RS232 or Modem
static const MENUITEM  RsChooseModemRS[] =
{
//!	  Enable function	Button label		Button label		Event
#if defined( MAKE_OPTIMUM) && !defined( MAKE_M2 )
    { Check_Modem,      IMAGE_EMPTY,        N_Modem,            '0'},
//	{ Check_GSM,		IMAGE_EMPTY,		N_GSM,              '0'	},
    { Check_Ethernet,   IMAGE_EMPTY,        N_Ethernet,         '0'},
//	{ Check_LAN,		IMAGE_EMPTY,		N_LAN,              '0'	},
    { Check_DialBackUp, IMAGE_EMPTY,        N_DialBackUp,       '0'},
#endif
    { NULL,             IMAGE_EMPTY,        N_RS232RS0,         '1'},
//	{ Check_COM2,		IMAGE_EMPTY,		N_RS232RS1,			'2'	},
};

//! RS232 Protocol
static const MENUITEM  RsCfgProtocol[] =
{
//!	  Enable function	Button label		Button label		Event
/*
    { NULL,				IMAGE_EMPTY,		N_ProtPosMini, 		PRTP_POSMINI},
    { NULL,				IMAGE_EMPTY,		N_ProtTrans,		PRTP_TRANS  },
    { NULL,				IMAGE_EMPTY,		N_ProtCat33, 		PRTP_CAT33	},
*/
};

//! RS232 CFG
static const MENUITEM  RsCfgBaudRate[] =
{
//!	  Enable function	Button label		Button label		Event
    { Check_notLAN,     IMAGE_EMPTY,        N_Baud1200,         SIO_B1200},
    { Check_notLAN,     IMAGE_EMPTY,        N_Baud2400,         SIO_B2400},
//! Note, do not use SIO_B4800 - this has the same value as '0' key
    { NULL,             IMAGE_EMPTY,        N_Baud4800,         'D'},
    { Check_notLAN,     IMAGE_EMPTY,        N_Baud9600,         SIO_B9600},
    { NULL,             IMAGE_EMPTY,        N_Baud19200 ,       SIO_B19200},
//! Note, unable to deliver constants SIO_B38400, SIO_B57600, SIO_B115200 -
//! do use 'A', 'B', 'C' instead it.
    { Check_notLAN,     IMAGE_EMPTY,        N_Baud38400 ,       'A'},
    { Check_notLAN,     IMAGE_EMPTY,        N_Baud57600 ,       'B'},
    { Check_notLAN,     IMAGE_EMPTY,        N_Baud115200,       'C'},
};

//! RS232 CFG
static const MENUITEM  RsCfgStopMenu[] =
{
//!	  Enable function	Button label		Button label		Event
    { NULL,             IMAGE_EMPTY,        N_RS232Stop1,       STOP1},
    { NULL,             IMAGE_EMPTY,        N_RS232Stop2,       STOP2},

};

//! RS232 CFG
static const MENUITEM  RsCfgDataMenu[] =
{
//!	  Enable function	Button label		Button label		Event
    { NULL,             IMAGE_EMPTY,        N_RS232Data8,       SIO_8BIT},
    { NULL,             IMAGE_EMPTY,        N_RS232Data7,       SIO_7BIT},

};

//! RS232 CFG
static const MENUITEM  RsCfgParityMenu[] =
{
//!	  Enable function	Button label		Button label		Event
    { NULL,             IMAGE_EMPTY,        N_RS232None,        SIO_PNONE | 0x80},
    { NULL,             IMAGE_EMPTY,        N_RS232Odd,         SIO_PODD},
    { NULL,             IMAGE_EMPTY,        N_RS232Even,        SIO_PEVEN},

};

// @erd 06.03.2007 
// Veritabanýndaki tablolarý formatlý bir biçimde yazdýrmak için Db detailed Menu sü
static const MENUITEM FormattedDbMenu [] = {
//	  Enable func		Image				Button label		Event
        { NULL,                 IMAGE_EMPTY,                    N_AcqTableReport,        'A'},//PRT_DETAILED_TABLE },
        { NULL,                 IMAGE_EMPTY,                    N_TermTableReport,       'T'},//PRT_DETAILED_TABLE },
        { NULL,                 IMAGE_EMPTY,                    N_IssTableReport,        'I'},//PRT_DETAILED_TABLE },
        { NULL,                 IMAGE_EMPTY,                    N_CardTableReport,       'C'},//PRT_DETAILED_TABLE },
};


//! TEST PRINT MENU
static const MENUITEM  TestPrintMenu[] =
{
//!	  Enable function	Button label		Button label		Event
    { NULL,             IMAGE_EMPTY,        N_Receipt,          'A'},
    { NULL,             IMAGE_EMPTY,        N_CharMap,          'B'},

};

//! RS232 Flow Control
static const MENUITEM  RsCfgFlowCMenu[] =
{
//!	  Enable function	Button label		Button label		Event
    { NULL,             IMAGE_EMPTY,        N_RS232None,        '0'},
    { NULL,             IMAGE_EMPTY,        N_RX,               '1'},
    { NULL,             IMAGE_EMPTY,        N_RXTX,             '2'},
};

// Ip Configuration Menu
static const MENUITEM IpConfigMenu[] =
{
//	  Enable function	Button label		Button label		Event
	{ NULL,				IMAGE_EMPTY,		N_IpAuto,  			'A'	},
	{ NULL,				IMAGE_EMPTY,		N_IpManual,			'B'	},
};


//! Language selection
static const MENUITEM  LanguageMenu[] =
{
//!	  Enable function	Button label		Button label		Event
#ifdef MAKE_ENGLISH
    { NULL,             IMAGE_EMPTY,        Lng_English,        LNG_EN},
#endif

#ifdef MAKE_GERMAN
    { NULL,             IMAGE_EMPTY,        Lng_German,         LNG_DE},
#endif

#ifdef MAKE_SWEDISH
    { NULL,             IMAGE_EMPTY,        Lng_Swedish,        LNG_SV},
#endif

#ifdef MAKE_SPANISH
    { NULL,             IMAGE_EMPTY,        Lng_Spanish,        LNG_ES},
#endif

#ifdef MAKE_PORTUGUESE
    { NULL,             IMAGE_EMPTY,        Lng_Portugues,      LNG_PT},
#endif

#ifdef MAKE_FARSI
	{ NULL,				IMAGE_EMPTY,		Lng_Farsi, 		    LNG_FA	},
#endif
};


//! Trace Log Control
static const MENUITEM  TraceLogMenu[] =
{
//!	  Enable function	Button label		Button label		Event
    { NULL,             IMAGE_EMPTY,        N_StartNew,         TRACE_START},
    { NULL,             IMAGE_EMPTY,        N_Print,            TRACE_PRINT},
    { NULL,             IMAGE_EMPTY,        N_Send1,            TRACE_SEND1},
    { Check_COM2,       IMAGE_EMPTY,        N_Send,             TRACE_SEND2},
    { NULL,             IMAGE_EMPTY,        N_Close,            TRACE_STOP},
};

static const MENUITEM PreAuthMenu[] = {
//	  Enable func		Image				Button label            Event
	{ NULL,			    IMAGE_EMPTY,		N_PreAuth_Long,         PREAUTH },
	{ PreAuthCancel,	IMAGE_EMPTY,		N_CancelPreAuth,        PREAUTHCANCEL},
    { NULL,			    IMAGE_EMPTY,		N_ClosedPreAuth,        CLOSED_PREAUTH },
    { PreAuthCancel,	IMAGE_EMPTY,		N_CancelClosedPreAuth,	CLOSEDPAUTHCANCEL },	
};

#ifdef MAKE_KIB 
static const MENUITEM LoyaltyListMenu[] =
{
//	  Enable function	Button label		Button label                  Event
	{ NULL,                 IMAGE_EMPTY,            N_SaleWithLoyalty,              '1'	},
        { NULL,                 IMAGE_EMPTY,            N_InstallmentWithLoyalty,  	'2'	},
        { NULL,                 IMAGE_EMPTY,            N_DiscountSale,                 '3'     },
        { NULL,                 IMAGE_EMPTY,            N_DiscountInst,                 '4'     },
	
};

static const MENUITEM DiscountListMenu[] =
{
//	  Enable function	Button label		Button label                  Event
	{ EnableDiscount,       IMAGE_EMPTY,            N_DiscountSale,               SALE	},
        { NULL,                 IMAGE_EMPTY,            N_DiscountInst,               INSTALLMENT},
	
};
    
#endif

static const MENUITEM RefundMenu[] =
{
//	Enable function                 Button label		Button label            Event  
        { NULL,				IMAGE_EMPTY,		N_SaleRefund,		REFUND },
        { NULL,				IMAGE_EMPTY,		N_InstalRefund,		INSTAL_REFUND },	
};



static const MENUITEM SettleStatusMenu[] =
{
//!	  Enable function	Button label		Button label		Event
    { NULL,             IMAGE_EMPTY,        N_View,             'V'},
    { NULL,             IMAGE_EMPTY,        N_Print,            'P'},
    { NULL,             IMAGE_EMPTY,        N_Quit,             CANCEL_KY},
};


//! List of all possible items for New Trans Menu
static const MENUITEM SetTMComParamMenu[] =
{
//!	  Enable function	Button label		Button label		Event
 //   { NULL,             IMAGE_EMPTY,        N_ComType,          SET_TM_COMTYPE},
 //   { NULL,             IMAGE_EMPTY,        N_PosTPDUEnable,    SET_TM_POSTPDU},
 //   { NULL,             IMAGE_EMPTY,        N_HostInfo,         MENU_HOST_INFO_TYPE},
};

static const MENUITEM HostInfoTypeMenu[] = 
{
   //!	  Enable function	Button label		Button label		Event
   { NULL,             IMAGE_EMPTY,        N_TransactionHost,        MENU_TRX_PRIM_SECOND},
   { NULL,          IMAGE_EMPTY,           N_SettlementHost,         MENU_SETTLE_PRIM_SECOND},

};

static const MENUITEM PrimSecondMenu[] = 
{
    //!	  Enable function	Button label		Button label		Event
    { NULL,           IMAGE_EMPTY,        N_Primary,        MENU_SET_PRIM_PARAM},
    { NULL,           IMAGE_EMPTY,        N_Secondary,      MENU_SET_SECOND_PARAM},

};

static const MENUITEM SetParamMenu[] = 
{
//!	  Enable function	Button label		Button label		Event
   // { NULL,           IMAGE_EMPTY,        N_NII,            SET_TM_NII },
   // { NULL,           IMAGE_EMPTY,        N_PhoneNumber,    SET_TM_PHONENUMBER },
    { NULL,           IMAGE_EMPTY,        N_IpAddress,      MENU_IPADDRESS},
   // { NULL,           IMAGE_EMPTY,        N_ConnectTime,    SET_TM_CONNECTTIME},
   // { NULL,           IMAGE_EMPTY,        N_DialAttemp,     SET_TM_DIALATTEMPT},
   // { NULL,           IMAGE_EMPTY,        N_ModemMode,      SET_TM_MODEMMODE},
   // { NULL,           IMAGE_EMPTY,        N_PPPEnable,      SET_TM_PPPENABLE},
   // { NULL,           IMAGE_EMPTY,        N_PPPMode,        SET_TM_PPPMODE},
 };

static const MENUITEM IpAddressMenu[] = 
{
   // { NULL,           IMAGE_EMPTY,        N_IpAddress,      SET_TM_IPDDRESS },
   // { NULL,           IMAGE_EMPTY,        N_PortNumber,     SET_TM_IPPORT_NUMBER},
   // { NULL,           IMAGE_EMPTY,        N_NII,            SET_TM_IPNII},
   // { NULL,           IMAGE_EMPTY,        N_HexLenEnable,   SET_TM_IPHEXLEN},
   // { NULL,           IMAGE_EMPTY,        N_PosTPDUEnable,  SET_TM_IPPOSTPDU},

};

static const MENUITEM ComTypeMenu[] = 
{
    { NULL,           IMAGE_EMPTY,        N_IP,         'A'},
    { NULL,           IMAGE_EMPTY,        N_GPRS,       'B'},
    { NULL,           IMAGE_EMPTY,        N_HLAN,       'C'},
    { NULL,           IMAGE_EMPTY,        N_DIALBACKUP, 'D'},
    { NULL,           IMAGE_EMPTY,        N_ONLYDIALUP, 'E'},

};




//! Menu Table
//!
//! Definition of menus, If the enable funtion (2nd item of MENU_ENTRY)
//! is NULL this menu will be allowed,
//!   
//! If an enable function is present and returns 0, the menu will not be 
//! drawn. The enable function itself must draw any error box which may
//! be required. Typically,  the enable functions will ask for a 
//! password and return 0 if the password fails.
//!
//! The enable function can also be used to setup dynamic menus such
//! as MENU_HOST.

        static const MENU_ENTRY Menus[] = {
            {
	 MENU_MAIN,					// Menu ID
	 NULL,						// Overall enable function
	 N_Main,					// Title
	 ( MENUITEM * ) MainMenu,	// Menu Data
	 sizeof( MainMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,					// Type
	 }
	,

	{
	 MENU_NEWTRANS,				// Menu ID
	 NULL,						// Overall enable function
	 N_NewTrans,				// Title
	 ( MENUITEM * ) NewTransMenu,	// Menu Data
	 sizeof( NewTransMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,					// Type
	 }
	,

	{
	 MENU_BATCH,				// Menu ID
	 NULL,						// Overall enable function
	 N_Batch,					// Title
	 ( MENUITEM * ) BatchMenu,	// Menu Data
	 sizeof( BatchMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,					// Type
	 }
	,

	{
	 MENU_TERMINAL,				// Menu ID
	 NULL,						// Overall enable function
	 N_Terminal,				// Title
	 ( MENUITEM * ) TerminalMenu,	// Menu Data
	 sizeof( TerminalMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,					// Type
	 }
	,

	{
	 MENU_SETUP,				// Menu ID
	 NULL,						// Overall enable function
	 N_Setup,					// Title
	 ( MENUITEM * ) SetupMenu,	// Menu Data
	 sizeof( SetupMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,					// Type
	 }
	,

	{
	 MENU_TESTS,				// Menu ID
	 NULL,						// Overall enable function
	 N_Tests,					// Title
	 ( MENUITEM * ) TestsMenu,	// Menu Data
	 sizeof( TestsMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,					// Type
	 }
	,

	{
	 MENU_TERM_SETUP,			// Menu ID
	 NULL,						// Overall enable function
	 N_Terminal,				// Title
	 ( MENUITEM * ) TermSetupMenu,	// Menu Data
	 sizeof( TermSetupMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,					// Type
	 }
	,

	{
	 MENU_TERM_SETUP_IP,								// Menu ID
	 NULL,												// Overall enable function
	 N_Terminal,										// Title
	 ( MENUITEM * ) TermSetupMenu_Ip,					// Menu Data
	 sizeof( TermSetupMenu_Ip ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,					// Type
	 }
	,

	{
	 MENU_PRINT_SETUP,			// Menu ID
	 NULL,						// Overall enable function
	 N_Printer,					// Title
	 ( MENUITEM * ) PrintSetupMenu,	// Menu Data
	 sizeof( PrintSetupMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_PASSWORD,				// Menu ID
	 NULL,						// Overall enable function
	 N_Passwords,				// Title
	 ( MENUITEM * ) PasswordMenu,	// Menu Data
	 sizeof( PasswordMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_TERMVIEW,				// Menu ID
	 NULL,						// Overall enable function
	 N_View,					// Title
	 ( MENUITEM * ) TermViewMenu,	// Menu Data
	 sizeof( TermViewMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_TERMVERSION,			// Menu ID
	 NULL,						// Overall enable function
	 N_Version,					// Title
	 ( MENUITEM * ) TermVersionMenu,	// Menu Data
	 sizeof( TermVersionMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_REPORTS,				// Menu ID
	 NULL,						// Overall enable function
	 N_Reports,					// Title
	 ( MENUITEM * ) ReportsMenu,	// Menu Data
	 sizeof( ReportsMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,					// Type
	 }
	,

	{
	 MENU_REPRINT,				// Menu ID
	 NULL,						// Overall enable function
	 N_Reprint,					// Title
	 ( MENUITEM * ) ReprintMenu,	// Menu Data
	 sizeof( ReprintMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_PAPERFEED,			// Menu ID
	 NULL,						// Overall enable function
	 N_PaperFeed,				// Title
	 ( MENUITEM * ) PaperfeedMenu,	// Menu Data
	 sizeof( PaperfeedMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_CASHIER_REPORT,		// Menu ID
	 NULL,						// Overall enable function
	 ReportMenuCashier,			// Title
	 ( MENUITEM * ) CashierReportMenu,	// Menu Data
	 sizeof( CashierReportMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_CASHIER,				// Menu ID
	 SetCashierMenuTitle,		// Overall enable function
	 CustomMsg,					// Title
	 ( MENUITEM * ) CashierMenu,	// Menu Data
	 sizeof( CashierMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_HOST_AUDIT_REPORT,	// Menu ID
	 EnableHostAuditReport,		// Overall enable function
	 N_Print,					// Title
	 ( MENUITEM * ) hostMenu,	// Menu Data
	 sizeof( hostMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_HOST_BATCH_SETTLE,	// Menu ID
	 EnableHostBatchSettle,		// Overall enable function
	 SettleTran,				// Title
	 ( MENUITEM * ) hostMenu,	// Menu Data
	 sizeof( hostMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_HOST_LOGON,			// Menu ID
	 EnableHostLogon,			// Overall enable function
	 N_Logon,					// Title
	 ( MENUITEM * ) hostMenu,	// Menu Data
	 sizeof( hostMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_HOST_BATCH_CONTROL,	// Menu ID
	 EnableHostBatchSettle,		// Overall enable function
	 N_Batch,					// Title
	 ( MENUITEM * ) hostMenu,	// Menu Data
	 sizeof( hostMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_HOST_TESTTRAN,		// Menu ID
	 EnableHostLogon,			// Overall enable function
	 TestTransaction,			// Title
	 ( MENUITEM * ) hostMenu,	// Menu Data
	 sizeof( hostMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,

	{
	 MENU_MAINT_FUNC,			// Menu ID
	 NULL,						// Overall enable function
	 MaintFunction,				// Title
	 ( MENUITEM * ) MaintFuncMenu,	// Menu Data
	 sizeof( MaintFuncMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_MAINT_CLEAR,			// Menu ID
	 NULL,						// Overall enable function
	 N_Clear,					// Title
	 ( MENUITEM * ) MaintClearMenu,	// Menu Data
	 sizeof( MaintClearMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_HOST_CLEAR_ACQUIRER,	// Menu ID
	 EnableHostBatchSettle,		// Overall enable function
	 N_Clear,					// Title
	 ( MENUITEM * ) hostMenu,	// Menu Data
	 sizeof( hostMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_PASSWORDS_ONOFF,		// Menu ID
	 NULL,						// Overall enable function
	 N_Passwords,				// Title
	 ( MENUITEM * ) PasswordOnOffMenu,	// Menu Data
	 sizeof( PasswordOnOffMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_LOCALOPTS,			// Menu ID
	 NULL,						// Overall enable function
	 Msg_LocalOpt,				// Title
	 ( MENUITEM * ) LocalOptionsMenu,	// Menu Data
	 sizeof( LocalOptionsMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_TAB,					// Menu ID
	 NULL,						// Overall enable function
	 N_Tab,						// Title
	 ( MENUITEM * ) TabMenu,	// Menu Data
	 sizeof( TabMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_CREDIT_DEBIT_EBT,		// Menu ID
	 NULL,						// Overall enable function
	 MSG_DEFAULT_TITLE,			// Title
	 ( MENUITEM * ) CreditDebitEbtMenu,	// Menu Data
	 sizeof( CreditDebitEbtMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_SHIFT,				// Menu ID
	 NULL,						// Overall enable function
	 N_Shift,					// Title
	 ( MENUITEM * ) ShiftMenu,	// Menu Data
	 sizeof( ShiftMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_DEBIT_EBT,			// Menu ID
	 NULL,						// Overall enable function
	 MSG_DEFAULT_TITLE,			// Title
	 ( MENUITEM * ) DebitEbtMenu,	// Menu Data
	 sizeof( DebitEbtMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_EVENT_TYPE,			// Menu ID
	 NULL,						// Overall enable function
	 MSG_DEFAULT_TITLE,			// Title
	 ( MENUITEM * ) EventTypeMenu,	// Menu Data
	 sizeof( EventTypeMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_IDLE_NEWTRANS,		// Menu ID
	 NULL,						// Overall enable function
	 N_NewTrans,				// Title
	 ( MENUITEM * ) IdleNewTransMenu,	// Menu Data
	 sizeof( IdleNewTransMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_IDLE_NEWFUNC,			// Menu ID
	 NULL,						// Overall enable function
	 N_Function,				// Title
	 ( MENUITEM * ) IdleNewFuncMenu,	// Menu Data
	 sizeof( IdleNewFuncMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,

	{
	 MENU_IDLE_INITCHG,			// Menu ID
	 LongTitleOpt,				// Overall enable function
	 N_IdleButton,				// Title
	 ( MENUITEM * ) IdleInitMenu,	// Menu Data
	 sizeof( IdleInitMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,

	{
	 MENU_SELECT_BTN,			// Menu ID
	 LongTitleOpt,				// Overall enable function
	 SelectButton,				// Title
	 ( MENUITEM * ) SelectBtn,	// Menu Data
	 sizeof( SelectBtn ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,
	{
	 MENU_CONFIG_RPT,			// Menu ID
	 LongTitleOpt,				// Overall enable function
	 Config_RPT,				// Title
	 ( MENUITEM * ) ConfigReport,	// Menu Data
	 sizeof( ConfigReport ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,

	{
	 MENU_HISTORY_RPT,			// Menu ID
	 LongTitleOpt,				// Overall enable function
	 N_NullStr,				// Title
	 ( MENUITEM * ) HistoryReport,	// Menu Data
	 sizeof( HistoryReport ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,

	{
	 MENU_CHECKREADER_TEST,		// Menu ID
	 NULL,						// Overall enable function
	 N_NullStr,				// Title
	 ( MENUITEM * ) CheckReaderTestMenu,	// Menu Data
	 sizeof( CheckReaderTestMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,					// Type
	 }
	,


	{
	 MENU_DIAL_MODE,			// Menu ID
	 NULL,						// Overall enable function
	 EnterDialMode,				// Title
	 ( MENUITEM * ) DialModeMenu,	// Menu Data
	 sizeof( DialModeMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,

	{
	 RS232_CHOOSE_MODEM_RS,			// Menu ID
	 NULL,						// Overall enable function
	 N_EnableComm,				// Title
	 ( MENUITEM * ) RsChooseModemRS,	// Menu Data
	 sizeof( RsChooseModemRS ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,

	{
	 RS232_PROT_MENU,			// Menu ID
	 NULL,						// Overall enable function
	 N_RS232Prot,				// Title
	 ( MENUITEM * ) RsCfgProtocol,	// Menu Data
	 sizeof( RsCfgProtocol ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,
	{
	 RS232_BAUD_MENU,			// Menu ID
	 NULL,						// Overall enable function
	 N_RS232Baud,				// Title
	 ( MENUITEM * ) RsCfgBaudRate,	// Menu Data
	 sizeof( RsCfgBaudRate ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,				// Type
	 }
	,
	{
	 RS232_STOP_MENU,			// Menu ID
	 NULL,						// Overall enable function
	 N_RS232Stop,				// Title
	 ( MENUITEM * ) RsCfgStopMenu,	// Menu Data
	 sizeof( RsCfgStopMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,
	{
	 RS232_DATA_MENU,			// Menu ID
	 NULL,						// Overall enable function
	 N_RS232Data,				// Title
	 ( MENUITEM * ) RsCfgDataMenu,	// Menu Data
	 sizeof( RsCfgDataMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,
	{
	 RS232_PARITY_MENU,			// Menu ID
	 NULL,						// Overall enable function
	 N_RS232Parity,				// Title
	 ( MENUITEM * ) RsCfgParityMenu,	// Menu Data
	 sizeof( RsCfgParityMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,

	{
	 TEST_PRINT_MENU,			// Menu ID
	 NULL,						// Overall enable function
	 N_Print,					// Title
	 ( MENUITEM * ) TestPrintMenu,	// Menu Data
	 sizeof( TestPrintMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,

    {
     RS232_FLOWC_MENU,
	 NULL,						// Overall enable function
	 N_RS232FlowC,				// Title
	 ( MENUITEM * ) RsCfgFlowCMenu,	// Menu Data
	 sizeof( RsCfgFlowCMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
    }
    ,

    {
     LANGUAGE_MENU,
	 NULL,                          // Overall enable function
	 Msg_SelectLanguage,			// Title
	 ( MENUITEM * ) LanguageMenu,	// Menu Data
	 sizeof( LanguageMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,                      // Type
    }
    ,

/*	{
	 APPLICATION_LIST_MENU,         // Menu ID
	 Get_ApplicationList,           // Overall enable function
	 N_SelectApplication,           // Title
	 ( MENUITEM * ) applistMenu,	// Menu Data
	 sizeof( applistMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,                       // Type
	 }
	,
*/
	{
	 MENU_TRACE,                    // Menu ID
	 NULL,                          // Overall enable function
	 N_TraceLog,                    // Title
	 ( MENUITEM * ) TraceLogMenu,	// Menu Data
	 sizeof( TraceLogMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,                       // Type
	 }
	,
	{
	 MENU_INIT_PREFERRED,			// Menu ID
	 NULL,					// Overall enable function
	 InitPrefService,			// Title
	 ( MENUITEM * ) PrefServiceMenu,	// Menu Data
	 sizeof( PrefServiceMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,
	{
	 MENU_NMS_PREFERRED,			// Menu ID
	 NULL,					// Overall enable function
	 NMSPrefService,			// Title
	 ( MENUITEM * ) PrefServiceMenu,	// Menu Data
	 sizeof( PrefServiceMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,
      
        {
	 MENU_PREAUTH,				// Menu ID
	 NULL,						// Overall enable function
	 N_PreAuth,					// Title
	 ( MENUITEM * ) PreAuthMenu,	// Menu Data
	 sizeof( PreAuthMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_10BTN,					// Type
        }
        ,
			        
		// Formatlý Ayrýntýlý DB raporu @erd 06.03.2007
		{
		MENU_DBFORMATTED,                                 // Menu ID
		NULL,                                             // Overall enable function  
		N_DBFormattedReport,                              // Title
		( MENUITEM *) FormattedDbMenu,                    // Menu Data
		sizeof ( FormattedDbMenu ) / sizeof ( MENUITEM ), // Number of Menu Items
		MT_5BTN,                                          // Type
		},

/*	{
	 MENU_IP_CONFIG,			// Menu ID
	 NULL,						// Overall enable function
	 EnterIpConfig,				// Title
	 ( MENUITEM * ) IpConfigMenu,	// Menu Data
	 sizeof( DialModeMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,
*/
        {
	 MENU_REFUND,			// Menu ID
	 NULL,                                  // Overall enable function
	 RefundTran,				// Title
	 ( MENUITEM * ) RefundMenu,	// Menu Data
	 sizeof( RefundMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,
#ifdef MAKE_KIB 
       {
	 MENU_LOYALTY_LIST,			// Menu ID
	 NULL,                                  // Overall enable function
	 N_LoyaltyList,				// Title
	 ( MENUITEM * ) LoyaltyListMenu,	// Menu Data
	 sizeof( LoyaltyListMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,

        {
	 MENU_DISCOUNT,			// Menu ID
	 NULL,                                  // Overall enable function
	 N_Discount,				// Title
	 ( MENUITEM * ) DiscountListMenu,	// Menu Data
	 sizeof( DiscountListMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }
	,
#endif        

        {
         MENU_PREAUTH,			// Menu ID
	 NULL,                                  // Overall enable function
	 N_PreAuth,				// Title
	 ( MENUITEM * ) PreAuthMenu,	// Menu Data
	 sizeof( PreAuthMenu ) / sizeof( MENUITEM ),	// Number of Menu Items
	 MT_5BTN,				// Type
	 }

/*
            {
                MENU_MAIN,                 // Menu ID
                NULL,                      // Overall enable function
                N_Main,                    // Title
                ( MENUITEM * ) MainMenu,   // Menu Data
                sizeof( MainMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_10BTN,                  // Type
            }
            ,

            {
                MENU_NEWTRANS,             // Menu ID
                NULL,                      // Overall enable function
                N_NewTrans,                // Title
                ( MENUITEM * ) NewTransMenu,   // Menu Data
                sizeof( NewTransMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_10BTN,                  // Type
            }
            ,

            {
                MENU_BATCH,                // Menu ID
                NULL,                      // Overall enable function
                N_Batch,                   // Title
                ( MENUITEM * ) BatchMenu,  // Menu Data
                sizeof( BatchMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_10BTN,                  // Type
            }
            ,

            {
                MENU_TERMINAL,             // Menu ID
                NULL,                      // Overall enable function
                N_Terminal,                // Title
                ( MENUITEM * ) TerminalMenu,   // Menu Data
                sizeof( TerminalMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_10BTN,                  // Type
            }
            ,

            {
                MENU_SETUP,                // Menu ID
                NULL,                      // Overall enable function
                N_Setup,                   // Title
                ( MENUITEM * ) SetupMenu,  // Menu Data
                sizeof( SetupMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_10BTN,                  // Type
            }
            ,

            {
                MENU_TESTS,                // Menu ID
                NULL,                      // Overall enable function
                N_Tests,                   // Title
                ( MENUITEM * ) TestsMenu,  // Menu Data
                sizeof( TestsMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_10BTN,                  // Type
            }
            ,

            {
                MENU_TERM_SETUP,           // Menu ID
                NULL,                      // Overall enable function
                N_Terminal,                // Title
                ( MENUITEM * ) TermSetupMenu,  // Menu Data
                sizeof( TermSetupMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_10BTN,                  // Type
            }
            ,

            {
                MENU_PRINT_SETUP,          // Menu ID
                NULL,                      // Overall enable function
                N_Printer,                 // Title
                ( MENUITEM * ) PrintSetupMenu, // Menu Data
                sizeof( PrintSetupMenu ) / sizeof( MENUITEM ), // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_PASSWORD,             // Menu ID
                NULL,                      // Overall enable function
                N_Passwords,               // Title
                ( MENUITEM * ) PasswordMenu,   // Menu Data
                sizeof( PasswordMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_TERMVIEW,             // Menu ID
                NULL,                      // Overall enable function
                N_View,                    // Title
                ( MENUITEM * ) TermViewMenu,   // Menu Data
                sizeof( TermViewMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_TERMVERSION,          // Menu ID
                NULL,                      // Overall enable function
                N_Version,                 // Title
                ( MENUITEM * ) TermVersionMenu,    // Menu Data
                sizeof( TermVersionMenu ) / sizeof( MENUITEM ),    // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_REPORTS,              // Menu ID
                NULL,                      // Overall enable function
                N_Reports,                 // Title
                ( MENUITEM * ) ReportsMenu,    // Menu Data
                sizeof( ReportsMenu ) / sizeof( MENUITEM ),    // Number of Menu Items
                MT_10BTN,                  // Type
            }
            ,

            {
                MENU_REPRINT,              // Menu ID
                NULL,                      // Overall enable function
                N_Reprint,                 // Title
                ( MENUITEM * ) ReprintMenu,    // Menu Data
                sizeof( ReprintMenu ) / sizeof( MENUITEM ),    // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_PAPERFEED,            // Menu ID
                NULL,                      // Overall enable function
                N_PaperFeed,               // Title
                ( MENUITEM * ) PaperfeedMenu,  // Menu Data
                sizeof( PaperfeedMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_CASHIER_REPORT,       // Menu ID
                NULL,                      // Overall enable function
                ReportMenuCashier,         // Title
                ( MENUITEM * ) CashierReportMenu,  // Menu Data
                sizeof( CashierReportMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_CASHIER,              // Menu ID
                SetCashierMenuTitle,       // Overall enable function
                CustomMsg,                 // Title
                ( MENUITEM * ) CashierMenu,    // Menu Data
                sizeof( CashierMenu ) / sizeof( MENUITEM ),    // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_HOST_AUDIT_REPORT,    // Menu ID
                EnableHostAuditReport,     // Overall enable function
                N_Print,                   // Title
                ( MENUITEM * ) hostMenu,   // Menu Data
                sizeof( hostMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_HOST_BATCH_SETTLE,    // Menu ID
                EnableHostBatchSettle,     // Overall enable function
                SettleTran,                // Title
                ( MENUITEM * ) hostMenu,   // Menu Data
                sizeof( hostMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_HOST_LOGON,           // Menu ID
                EnableHostLogon,           // Overall enable function
                N_Logon,                   // Title
                ( MENUITEM * ) hostMenu,   // Menu Data
                sizeof( hostMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_HOST_BATCH_CONTROL,   // Menu ID
                EnableHostBatchSettle,     // Overall enable function
                N_Batch,                   // Title
                ( MENUITEM * ) hostMenu,   // Menu Data
                sizeof( hostMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_HOST_TESTTRAN,        // Menu ID
                EnableHostLogon,           // Overall enable function
                TestTransaction,           // Title
                ( MENUITEM * ) hostMenu,   // Menu Data
                sizeof( hostMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,

            {
                MENU_MAINT_FUNC,           // Menu ID
                NULL,                      // Overall enable function
                MaintFunction,             // Title
                ( MENUITEM * ) MaintFuncMenu,  // Menu Data
                sizeof( MaintFuncMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_MAINT_CLEAR,          // Menu ID
                NULL,                      // Overall enable function
                N_Clear,                   // Title
                ( MENUITEM * ) MaintClearMenu, // Menu Data
                sizeof( MaintClearMenu ) / sizeof( MENUITEM ), // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_HOST_CLEAR_ACQUIRER,  // Menu ID
                EnableHostBatchSettle,     // Overall enable function
                N_Clear,                   // Title
                ( MENUITEM * ) hostMenu,   // Menu Data
                sizeof( hostMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_PASSWORDS_ONOFF,      // Menu ID
                NULL,                      // Overall enable function
                N_Passwords,               // Title
                ( MENUITEM * ) PasswordOnOffMenu,  // Menu Data
                sizeof( PasswordOnOffMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_LOCALOPTS,            // Menu ID
                NULL,                      // Overall enable function
                Msg_LocalOpt,              // Title
                ( MENUITEM * ) LocalOptionsMenu,   // Menu Data
                sizeof( LocalOptionsMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_TAB,                  // Menu ID
                NULL,                      // Overall enable function
                N_Tab,                     // Title
                ( MENUITEM * ) TabMenu,    // Menu Data
                sizeof( TabMenu ) / sizeof( MENUITEM ),    // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_CREDIT_DEBIT_EBT,     // Menu ID
                NULL,                      // Overall enable function
                MSG_DEFAULT_TITLE,         // Title
                ( MENUITEM * ) CreditDebitEbtMenu, // Menu Data
                sizeof( CreditDebitEbtMenu ) / sizeof( MENUITEM ), // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_SHIFT,                // Menu ID
                NULL,                      // Overall enable function
                N_Shift,                   // Title
                ( MENUITEM * ) ShiftMenu,  // Menu Data
                sizeof( ShiftMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_DEBIT_EBT,            // Menu ID
                NULL,                      // Overall enable function
                MSG_DEFAULT_TITLE,         // Title
                ( MENUITEM * ) DebitEbtMenu,   // Menu Data
                sizeof( DebitEbtMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_EVENT_TYPE,           // Menu ID
                NULL,                      // Overall enable function
                MSG_DEFAULT_TITLE,         // Title
                ( MENUITEM * ) EventTypeMenu,  // Menu Data
                sizeof( EventTypeMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_IDLE_NEWTRANS,        // Menu ID
                NULL,                      // Overall enable function
                N_NewTrans,                // Title
                ( MENUITEM * ) IdleNewTransMenu,   // Menu Data
                sizeof( IdleNewTransMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_IDLE_NEWFUNC,         // Menu ID
                NULL,                      // Overall enable function
                N_Function,                // Title
                ( MENUITEM * ) IdleNewFuncMenu,    // Menu Data
                sizeof( IdleNewFuncMenu ) / sizeof( MENUITEM ),    // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_IDLE_INITCHG,         // Menu ID
                LongTitleOpt,              // Overall enable function
                N_IdleButton,              // Title
                ( MENUITEM * ) IdleInitMenu,   // Menu Data
                sizeof( IdleInitMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,

            {
                MENU_SELECT_BTN,           // Menu ID
                LongTitleOpt,              // Overall enable function
                SelectButton,              // Title
                ( MENUITEM * ) SelectBtn,  // Menu Data
                sizeof( SelectBtn ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,
            {
                MENU_CONFIG_RPT,           // Menu ID
                LongTitleOpt,              // Overall enable function
                Config_RPT,                // Title
                ( MENUITEM * ) ConfigReport,   // Menu Data
                sizeof( ConfigReport ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,

            {
                MENU_HISTORY_RPT,          // Menu ID
                LongTitleOpt,              // Overall enable function
                N_NullStr,             // Title
                ( MENUITEM * ) HistoryReport,  // Menu Data
                sizeof( HistoryReport ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,

            {
                MENU_CHECKREADER_TEST,     // Menu ID
                NULL,                      // Overall enable function
                N_NullStr,             // Title
                ( MENUITEM * ) CheckReaderTestMenu,    // Menu Data
                sizeof( CheckReaderTestMenu ) / sizeof( MENUITEM ),    // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,


            {
                MENU_DIAL_MODE,            // Menu ID
                NULL,                      // Overall enable function
                EnterDialMode,             // Title
                ( MENUITEM * ) DialModeMenu,   // Menu Data
                sizeof( DialModeMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,

            {
                RS232_CHOOSE_MODEM_RS,         // Menu ID
                NULL,                      // Overall enable function
                N_EnableComm,              // Title
                ( MENUITEM * ) RsChooseModemRS,    // Menu Data
                sizeof( RsChooseModemRS ) / sizeof( MENUITEM ),    // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,

            {
                RS232_PROT_MENU,           // Menu ID
                NULL,                      // Overall enable function
                N_RS232Prot,               // Title
                ( MENUITEM * ) RsCfgProtocol,  // Menu Data
                sizeof( RsCfgProtocol ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,
            {
                RS232_BAUD_MENU,           // Menu ID
                NULL,                      // Overall enable function
                N_RS232Baud,               // Title
                ( MENUITEM * ) RsCfgBaudRate,  // Menu Data
                sizeof( RsCfgBaudRate ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_10BTN,              // Type
            }
            ,
            {
                RS232_STOP_MENU,           // Menu ID
                NULL,                      // Overall enable function
                N_RS232Stop,               // Title
                ( MENUITEM * ) RsCfgStopMenu,  // Menu Data
                sizeof( RsCfgStopMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,
            {
                RS232_DATA_MENU,           // Menu ID
                NULL,                      // Overall enable function
                N_RS232Data,               // Title
                ( MENUITEM * ) RsCfgDataMenu,  // Menu Data
                sizeof( RsCfgDataMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,
            {
                RS232_PARITY_MENU,         // Menu ID
                NULL,                      // Overall enable function
                N_RS232Parity,             // Title
                ( MENUITEM * ) RsCfgParityMenu,    // Menu Data
                sizeof( RsCfgParityMenu ) / sizeof( MENUITEM ),    // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,

            {
                TEST_PRINT_MENU,           // Menu ID
                NULL,                      // Overall enable function
                N_Print,                   // Title
                ( MENUITEM * ) TestPrintMenu,  // Menu Data
                sizeof( TestPrintMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,

            {
                RS232_FLOWC_MENU,
                NULL,                      // Overall enable function
                N_RS232FlowC,              // Title
                ( MENUITEM * ) RsCfgFlowCMenu, // Menu Data
                sizeof( RsCfgFlowCMenu ) / sizeof( MENUITEM ), // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,

            {
                LANGUAGE_MENU,
                NULL,                          // Overall enable function
                Msg_SelectLanguage,            // Title
                ( MENUITEM * ) LanguageMenu,   // Menu Data
                sizeof( LanguageMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_10BTN,                      // Type
            }
            ,

            {
                MENU_TRACE,                    // Menu ID
                NULL,                          // Overall enable function
                N_TraceLog,                    // Title
                ( MENUITEM * ) TraceLogMenu,   // Menu Data
                sizeof( TraceLogMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                       // Type
            }
            ,
            {
                MENU_INIT_PREFERRED,           // Menu ID
                NULL,                  // Overall enable function
                InitPrefService,           // Title
                ( MENUITEM * ) PrefServiceMenu,    // Menu Data
                sizeof( PrefServiceMenu ) / sizeof( MENUITEM ),    // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,
            {
                MENU_NMS_PREFERRED,            // Menu ID
                NULL,                  // Overall enable function
                NMSPrefService,            // Title
                ( MENUITEM * ) PrefServiceMenu,    // Menu Data
                sizeof( PrefServiceMenu ) / sizeof( MENUITEM ),    // Number of Menu Items
                MT_5BTN,               // Type
            }
            ,

            {
                MENU_SETTLE_STATUS,        // Menu ID
                NULL,      // Overall enable function
                N_SettleStat,                  // Title
                ( MENUITEM * ) SettleStatusMenu,   // Menu Data
                sizeof( SettleStatusMenu ) / sizeof( MENUITEM ),   // Number of Menu Items
                MT_5BTN,                   // Type
            },
           
             
            // set TM communication parameter  
            {
                MENU_SET_TMCOMPARAM,       // Menu ID
                NULL,                      // Overall enable function
                N_SetComParam,             // Title
                ( MENUITEM * ) SetTMComParamMenu,  // Menu Data
                sizeof( SetTMComParamMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_HOST_INFO_TYPE,       // Menu ID
                NULL,                      // Overall enable function
                N_HostInfoType,             // Title
                ( MENUITEM * ) HostInfoTypeMenu,  // Menu Data
                sizeof( HostInfoTypeMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,         
               
            {
                MENU_TRX_PRIM_SECOND,       // Menu ID
                SelectTrx,                      // Overall enable function
                N_Select,             // Title
                ( MENUITEM * ) PrimSecondMenu,  // Menu Data
                sizeof( PrimSecondMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,


            {
                MENU_SETTLE_PRIM_SECOND,       // Menu ID
                SelectSettle,                      // Overall enable function
                N_Select,             // Title
                ( MENUITEM * ) PrimSecondMenu,  // Menu Data
                sizeof( PrimSecondMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_SET_PRIM_PARAM,       // Menu ID
                SelectPrimary,                      // Overall enable function
                N_SetParam,             // Title
                ( MENUITEM * ) SetParamMenu,  // Menu Data
                sizeof( SetParamMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_SET_SECOND_PARAM,       // Menu ID
                SelectSecondary,                      // Overall enable function
                N_SetParam,             // Title
                ( MENUITEM * ) SetParamMenu,  // Menu Data
                sizeof( SetParamMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,

            {
                MENU_IPADDRESS,       // Menu ID
                NULL,                      // Overall enable function
                N_IpAddress,             // Title
                ( MENUITEM * ) IpAddressMenu,  // Menu Data
                sizeof( IpAddressMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,   

            {
                MENU_COMTYPE,       // Menu ID
                NULL,                      // Overall enable function
                N_ComType,             // Title
                ( MENUITEM * ) ComTypeMenu,  // Menu Data
                sizeof( ComTypeMenu ) / sizeof( MENUITEM ),  // Number of Menu Items
                MT_5BTN,                   // Type
            }
            ,   
*/

        };

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!      Funciton to draw menu based on menu id.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!		SDLScreenId is used to globbally pass menu id
//-----------------------------------------------------------------------------
        extern void XDrawMenu( void )
        {
            MENU_ENTRY *pMenu;

            // get menu based on id, check for special menu types also
            switch ( SDLScreenId )
            {
            case MENU_PREVIOUS:
                if ( PreviousIndex > 1 )    // current + previous
                {
                    PopMenu( &Current );    // get rid of current
                    MenuItemCounter = Current.MenuItemStored;
                    PopMenu( &Current );    // get the previous one
                }
                else
                {
                    // attempt to draw non-existant previous menu
                    RunTimeError ( 0x0010 );
                }
                break;

            case MENU_NEXT:
                if ( NextEnabled )  // current + previous
                {
                    memcpy( &Current, &Next, sizeof( MENU_LOCATOR ) );
                    Current.MenuItemStored = MenuItemCounter;
                    MenuItemCounter = 0;
                }
                else
                {
                    // attempt to draw non-existant next menu
                    RunTimeError ( 0x0011 );
                }
                break;

            case MENU_REDRAW:
                if ( PreviousIndex > 0 )    // do we have current menu
                {
                    // This is a redraw, so pop the menu off the stack,
                    // will get pushed back on again
                    PopMenu( &Current );    // get last menu drawn
                }
                else
                {
                    // attempt to draw non-existant menu, no previous menu was drawn
                    RunTimeError ( 0x0012 );
                }
                break;

            default:
                pMenu = GetMenu( SDLScreenId );

                if ( pMenu == NULL )
                {
                    // attempt to draw non-existant menu
                    RunTimeError ( 0x0013 );
                }

                Current.pMenu = pMenu;  // set current menu
                Current.pItem = pMenu->pMenuList;   // set current item pointer

                //  find pointer to last
                Current.pEndItem = pMenu->pMenuList + ( pMenu->Size - 1 );  // end item for menu

                Current.MenuItemStored = MenuItemCounter;
                MenuItemCounter = 0;
                break;


            }

            // at this point "MENU_LOCATOR Current" assumed to be properly filled

            if ( NULL != Current.pMenu->pEnableFunc )   // Enable function exist?
            {
                if ( !Current.pMenu->pEnableFunc(  ) )  // yes
                    return;             // don't show this menu
            }

            // set title
            User_Msgs[11] = Current.pMenu->TitleMsg;

            // assume no menu buttons will be enabled
            memset( &Enable_Flags[1], 0, 10 );

            if ( 0 == PreviousIndex )
                Enable_Flags[PREV_ENABLE_FLAG] = 0; // disable previous button
            else
            {
                Enable_Flags[PREV_ENABLE_FLAG] = 1; // enable previous button   
                if ( Current.pMenu->pMenuList == Current.pItem )    // if previous button will trigger new menu
                {
                    // we are at top of menu
                    User_Events[7] = B_PREVIOUS_NEW_MENU;   // event id for previous button
                }
                else
                {
                    User_Events[7] = B_PREVIOUS;    // event id for previous button
                }
            }
            // assume next button will be disabled
            Enable_Flags[NEXT_ENABLE_FLAG] = 0; // disable next button

            DrawMenuType( Current.pMenu->Type );    // draw the menu, enable next button if required

            PushMenu( &Current );       // save menu on stack

        }

//-----------------------------------------------------------------------------
//!      Clear menu stack.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//-----------------------------------------------------------------------------
        extern void ClearMenus( void )
        {
            PreviousIndex = 0;
            NextEnabled = False;
            MenuItemCounter = 0;

        }

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!      Returns pointer to MENU_ENTRY in menu table based on given menu transaction number.
//!
//! \param
//!		menuID			transaction id of menu
//!				
//! \return
//!     MENU_ENTRY*		pointer to MENU_ENTRY in menu table,
//!                      NULL if not found 
//!
//! \note
//-----------------------------------------------------------------------------
        MENU_ENTRY *GetMenu( enum transactions menuID )
        {
            UBYTE i;
            // search menu table for id
            for ( i = 0; i < sizeof( Menus ) / sizeof( MENU_ENTRY ); i++ )
            {
                if ( Menus[i].menuID == menuID )
                {
                    return( MENU_ENTRY * ) & Menus[i];
                }
            }

            return NULL;                // not found

        }

//-----------------------------------------------------------------------------
//!      Test if tabs are enabled.
//!
//! \param
//!				
//! \return
//!     UBYTE			1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE EnableTab( void )
        {
            // If  restaurant, enable
            if ( TCONF.TCOPT1 & TC1_REST )
            {
                return 1;
            }

            return 0;
        }

//-----------------------------------------------------------------------------
//!      Test if shift proecessing required.
//!
//! \param
//!				
//! \return
//!     UBYTE			1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE EnableShift( void )
        {
            if ( ( TCONF.TCOPT5 & TC5_SHIFT ) )
            {
                // SHIFT PROCESSING ENABLED
                return 1;
            }

            return 0;
        }

//-----------------------------------------------------------------------------
//!      Test if server menu item if required.
//!
//! \param
//!				
//! \return
//!     UBYTE			1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE EnableServer( void )
        {
            // If Cashier and server, return 1 for enable
            if ( TCONF.TCOPT1 & TC1_CASHIER )
            {
                if ( TCONF.TCOPT4 & TC4_SRV_PRM )
                    return 1;
            }

            return 0;
        }

//-----------------------------------------------------------------------------
//!      Test if cashier menu item if required.
//!
//! \param
//!				
//! \return
//!     UBYTE			1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE EnableCashier( void )
        {
            // If Cashier and Not server, then return 1
            if ( TCONF.TCOPT1 & TC1_CASHIER )
            {
                if ( !( TCONF.TCOPT4 & TC4_SRV_PRM ) )
                    return 1;
            }

            return 0;
        }

//-----------------------------------------------------------------------------
//!      Test if till menu item if required.
//!
//! \param
//!				
//! \return
//!     UBYTE			1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE EnableTill( void )
        {
            // If Cashier and till processing, then return 1
            if ( TCONF.TCOPT1 & TC1_CASHIER )
            {
                if ( TCONF.TCOPT1 & TC1_TILL )
                    return 1;
            }
            return 0;
        }

//-----------------------------------------------------------------------------
//!      Sets up button labels and events and then draws the given menu type using DrawScreeen.
//!
//! \param
//!		type		pointer to the menu type structure
//!				
//! \return
//!
//! \note
//!		Also determines if next button required, if so set up MENU_LOCATOR Next.
//-----------------------------------------------------------------------------
        static void DrawMenuType( MENU_TYPE type )
        {
            UBYTE MaxButtons;
            MENUITEM *pTempItem, *pSaveItem;
            Bool ButtonEnabled;
            SCREEN_ID Screen;

            switch ( type )
            {
            case MT_10BTN:
                MaxButtons = 10;
                Screen = MENU_10BTNTEXT_SCR;
                break;

            case MT_5BTN:
                MaxButtons = 5;
                Screen = MENU_5BTNTEXT_SCR;
                break;
            }

            pTempItem = Current.pItem;

            NextEnabled = True;         // assume next button is enabled

            // setup buttons for this screen
            for ( ButtonCnt = 0; ButtonCnt < MaxButtons; )
            {

                ButtonEnabled = True;   // assume this menu button is enabled
                if ( NULL != pTempItem->pEnableFunc )   // if a enable function exists
                {
                    if ( !pTempItem->pEnableFunc(  ) )  // do we include this item?
                    {
                        ButtonEnabled = False;  // do not enable this menu button
                    }
                }

                if ( ButtonEnabled )
                {
                    // add button to screen, set up button labels and events

                    User_Imgs[ButtonCnt + 1] = pTempItem->msgID1;
                    User_Msgs[ButtonCnt + 1] = pTempItem->msgID2;

                    User_Events[ButtonCnt + 1] = pTempItem->eventID;
                    Enable_Flags[ButtonCnt + 1] = 1;
                    ButtonCnt++;        // next button
                }

                if ( pTempItem == Current.pEndItem )
                {
                    NextEnabled = False;    // end of list so do not enable next button
                    break;              // break at end of list
                }

                pTempItem++;            // next item

            }

            if ( NextEnabled )
            {
                // will only get here, if there is a next menu, 
                // at this point, pTempItem contains pointer to next menu item

                pSaveItem = pTempItem;

                // do we really need a next menu? make sure at least one of remaining menu
                // item is enabled before allowing a next button

                do
                {
                    if ( NULL != pTempItem->pEnableFunc )   // if an enable function exists
                    {

                        if ( !pTempItem->pEnableFunc(  ) )  // do we include this item?
                        {
                            if ( pTempItem == Current.pEndItem )
                                break;
                            pTempItem++;
                            continue;   // no, so keep looping
                        }
                    }


                    Enable_Flags[NEXT_ENABLE_FLAG] = 1; // enable next button
                    memcpy( &Next, &Current, sizeof( MENU_LOCATOR ) );
                    Next.pItem = pSaveItem; // set proper item pointer for next menu    
                    break;

                }
                while ( 1 );
            }

            // Change 10 button menu to 5-button menu if less than 6 buttons available at the moment
            if ( 6 > ButtonCnt )
                Screen = MENU_5BTNTEXT_SCR;

            // Draw screen
            DrawScreen( Screen );

        }

//-----------------------------------------------------------------------------
//!      Simply set CSTMSG to title for Cashier Menu and return 1.
//!
//! \param
//!		type		pointer to the menu type structure
//!				
//! \return
//!		UBYTE		Always returns 1
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE SetCashierMenuTitle( void )
        {
            enum msg_id mid;
            // Prompt for Server ?
            if ( TCONF.TCOPT4 & TC4_SRV_PRM )
                mid = ReportMenuServer;
            else
                mid = ReportMenuCashier;

            GetMsg( mid, CSTMSG );

            return 1;
        }

//-----------------------------------------------------------------------------
//!      Pop menu off stack.
//!
//! \param
//!		pMenuLoc		pointer to menu location and identifier
//!				
//! \return
//!
//! \note
//-----------------------------------------------------------------------------
        static void PopMenu( MENU_LOCATOR * pMenuLoc )
        {
            if ( PreviousIndex != 0 )
            {
                PreviousIndex--;
                memcpy( pMenuLoc, &Previous[PreviousIndex], sizeof( MENU_LOCATOR ) );
            }
        }

//-----------------------------------------------------------------------------
//!      Push Menu on stack.
//!
//! \param
//!		pMenuLoc		pointer to menu location and identifier
//!				
//! \return
//!
//! \note
//-----------------------------------------------------------------------------
        static void PushMenu( MENU_LOCATOR * pMenuLoc )
        {

            if ( MAX_MENU_LEVELS == PreviousIndex )
            {
                // max number of nested menu items exceeded
                // change MAX_MENU_LEVELS definition
                RunTimeError ( 0x0002 );
            }

            memcpy( &Previous[PreviousIndex], pMenuLoc, sizeof( MENU_LOCATOR ) );
            PreviousIndex++;
        }

//-----------------------------------------------------------------------------
//!      Fills hostMenu[] array with available hosts.
//!
//! \param
//!		IncludeAll		0 - All Hosts menu button is Not drawn
//!                          but all of the Hosts are displayed.
//!                      1 - All Hosts menu button is drawn
//!                          and all of the Hosts are displayed.
//!                      2 - All Hosts menu button is drawn
//!                          but only Settle Hosts are displayed.
//!				
//! \return
//!
//! \note
//-----------------------------------------------------------------------------
        static void SetHostMenu( UBYTE IncludeAll )
        {
            UBYTE i;
            static MENUITEM * phostMenu;

            phostMenu = hostMenu;   // initialise pointer

            /* Display the ALL HOSTS button if condition set */
            if (IncludeAll)
            {
                phostMenu->pEnableFunc = NULL;
                phostMenu->msgID1 = IMAGE_ALL;
                phostMenu->msgID2 = N_AllHosts;
                phostMenu->eventID = 'Z';
                phostMenu++;
            }

            for ( i = 0; i < AQMAX; i++, phostMenu++ )
            {
                if ( !AQTAB[i].AQID )
                {
                    phostMenu->pEnableFunc = Disable;   // disable this item
                }
                else
                {
                    // See if "Not a Settlement Acquirer" is ON, 
                    if ( ( AQTAB[i].AQOPT3 & AQ3_HSEXCL ) && ( 2 == IncludeAll ) )
                    {
                        phostMenu->pEnableFunc = Disable;   /* disable this item */
                    }
                    else
                    {
                        /* set MENUITEM for this host */
                        phostMenu->pEnableFunc = NULL;
                        phostMenu->msgID1 = IMAGE_HOST;     /* use special custom messages */
                        phostMenu->msgID2 = N_Ctxt1 + i;
                        phostMenu->eventID = 'A'+i;
                        CtxtPtr[i] = ( char * ) AQTAB[i].AQNAME;
                    }
                }
            }
        }

//-----------------------------------------------------------------------------
//!      Function to always disable a menu item.
//!
//! \param
//!				
//! \return
//!		UBYTE		Always returns 0
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE Disable( void )
        {
            return 0;
        }

//-----------------------------------------------------------------------------
//!      Setup dynamic hostmenu.
//!
//! \param
//!				
//! \return
//!		UBYTE		Always returns 1
//!
//! \note
//!		Special use of enable function to setup host menu.
//-----------------------------------------------------------------------------
        static UBYTE EnableHostAuditReport( void )
        {
            SetHostMenu( 1 );
            return 1;
        }

//-----------------------------------------------------------------------------
//!      Setup dynamic hostmenu.
//!
//! \param
//!				
//! \return
//!		UBYTE		Always returns 1
//!
//! \note
//!		Special use of enable function to setup host menu.
//-----------------------------------------------------------------------------
        static UBYTE EnableHostBatchSettle( void )
        {
            SetHostMenu( 2 );
            return 1;
        }

//-----------------------------------------------------------------------------
//!      Setup dynamic hostmenu.
//!
//! \param
//!				
//! \return
//!		UBYTE		Always returns 1
//!
//! \note
//!		Special use of enable function to setup host menu.
//-----------------------------------------------------------------------------
        static UBYTE EnableHostLogon( void )
        {
            SetHostMenu( 0 );
            return 1;
        }

//-----------------------------------------------------------------------------
//!      Enable debit selection if available.
//!
//! \param
//!				
//! \return
//!		UBYTE		1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE EnableDebit( void )
        {
            if ( IssuerTrans & TRANS_DEBIT )
                return 1;
            else
                return 0;
        }

//-----------------------------------------------------------------------------
//!      Set long Title invert box.
//!
//! \param
//!				
//! \return
//!		UBYTE		Always returns 1
//!
//! \note
//!		Special use of enable function to setup host menu 
//-----------------------------------------------------------------------------
        static UBYTE LongTitleOpt( void )
        {
            PreviousIndex = 0;
            NextEnabled = False;
            return 1;
        }

//-----------------------------------------------------------------------------
//!      Test if Configuration Report is enabled.
//!
//! \param
//!				
//! \return
//!		UBYTE		1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE EnableConfReport( void )
        {
            // If  Configuration Report, enable
            if ( TCONF.TCOPT7 & TC7_CONFIG )
            {

                return 1;
            }

            return 0;
        }

//-----------------------------------------------------------------------------
//!      Test if Performance Report is enabled.
//!
//! \param
//!				
//! \return
//!		UBYTE		1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE EnablePerfReport( void )
        {
            // If  Performance Report, enable
            if ( TCONF.TCOPT7 & TC7_PERFORMANCE )
            {

                return 1;
            }

            return 0;
        }

//-----------------------------------------------------------------------------
//!      Test if there are some applications.
//!
//! \param
//!				
//! \return
//!		UBYTE		1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE EnableMutiApp( void )
        {
            return 0;
        }

//-----------------------------------------------------------------------------
//!      Check if telephone modem configured.
//!
//! \param
//!				
//! \return
//!		UBYTE		1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE Check_Modem( void )
        {
            return 1;
        }

//-----------------------------------------------------------------------------
//!      Check if GSM modem configured.
//!
//! \param
//!				
//! \return
//!		UBYTE		1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE Check_GSM( void )
        {
            return 0;
        }

//-----------------------------------------------------------------------------
//!      Check if Ethernet module configured.
//!
//! \param
//!				
//! \return
//!		UBYTE		1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE Check_Ethernet( void )
        {
#if defined( MAKE_OPTIMUM) && !defined( MAKE_M2 )
            return 1;
#else 
            return 0;
#endif
        }

//-----------------------------------------------------------------------------
//!      Check if not POS LAN module configured.
//!
//! \param
//!				
//! \return
//!		UBYTE		1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE Check_notLAN( void )
        {
            if ( RSTAB.RSOPT & RSOPT_RSACTIVE )
                return 1;

#if defined( MAKE_OPTIMUM ) && !defined( MAKE_M2 )
            return 0;
#else 
            return 1;
#endif
        }

//-----------------------------------------------------------------------------
//!      Check if Dial Backup  module configured.
//!
//! \param
//!				
//! \return
//!		UBYTE		1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE Check_DialBackUp( void )
        {
#if defined( MAKE_OPTIMUM ) && !defined( MAKE_M2 )
            return 1;
#else 
            return 0;
#endif
        }

//-----------------------------------------------------------------------------
//!      Check if COM2 available.
//!
//! \param
//!				
//! \return
//!		UBYTE		1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE Check_COM2( void )
        {
            return 0;
        }

//-----------------------------------------------------------------------------
//!      Exit menu item. Available only on debud buid
//!
//! \param
//!				
//! \return
//!		UBYTE		1 - enable, 0 - do not enable
//!
//! \note
//-----------------------------------------------------------------------------
        static UBYTE Check_Exit( void )
        {
#if defined (MAKE_OPTIMUM) && ( defined( MAKE_DEBUG ) || defined(_DEBUG) )
            return 1;
#else
            return 0;
#endif
        }

static UBYTE SelectTrx( void )
{
    SetTMParameterVariables |= SELECT_TRX_HOST_INFO;
    return True;
}

static UBYTE SelectSettle( void )
{
    SetTMParameterVariables |= SELECT_SETTLE_HOST_INFO;
    return True;
}

static UBYTE SelectPrimary( void )
{
    SetTMParameterVariables |= SELECT_PRIMARY_INFO;
    return True;
}

static UBYTE SelectSecondary( void )
{
    SetTMParameterVariables |= SELECT_SECONDARY_INFO;
    return True;
}

static UBYTE PreAuthCancel ( void ) 
{
//    flagPreCancel = 1;
    return 1;
}
static UBYTE EnableDiscount( void ) 
{
#ifdef MAKE_KIB
    TRINP.TRLOYALTYSTATUS |= TS_DISCOUNT;
#endif

    return 1;
}

