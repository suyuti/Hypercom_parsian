
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _TRANSDEF_H_
#define _TRANSDEF_H_
//=============================================================================
//
// Module overview:             transdef.h
//      Transaction and event identifiers
//
//=============================================================================

#include "screen.h"
#include "menu.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================
//********************** transaction keys **************************

enum transactions
{
	NOOPT,						// 0x0000 - No option - do not remove
	SALE,						// 0x0001 - SALE TRANSACTION
	REVIEW,						// 0x0002 - Journal Review(JRNLOOK)
	SETTLE,						// 0x0003 - Settle Transaction
	FUNCTION,					// 0x0004 - Function table
	INITIALIZE,					// 0x0005 - Terminal initialization
	CRDVER,						// 0x0006 - Pre-Authorize Transaction
	DEBIT,						// 0x0007 - Debit Transaction
	RESTART,                    // 0x0008 - Terminal Restart
	FIND,						// 0x0009 - Look at specific invoice
	ADJUST,						// 0x000A - ADJUST TRANSACTION
	BALINQ,						// 0x000B - Balance Inquiry
	REPRINT,					// 0x000C - Reprint Last Receipt
	VOIDTRAN,					// 0x000D - VOID TRANSACTION

    PREAUTHCANCEL,                                  //          Preauth Cancel
    CLOSEDPAUTHCANCEL,                              //          Closed Preauth Cancel

    REPORTS,					// 0x000E - Reports Menu(F79)
	TOTALS,						// 0x000F - Totals Display(TOTLOOK)
	
	SALOFFL,					// 0x0011 - Offline Sale
	PRTLIST,					// 0x0012 - Audit Log Report
	PRTTILL,					// 0x0013 - Till Report
	TESTTRAN,					// 0x0014 - Test Transaction
	AUTH,						// 0x0015 - Auth Transaction
	ADJSALE,					// 0x0016 - Adjust Sale Transaction
	ADJREFUND,					// 0x0017 - Adjust Refund Transaction
	TRAINMODE,					// 0x0018 - Training Mode Transaction
	CLBATCH,					// 0x0019 - Close Batch
	CLBATCHA,					// 0x001A - Close Batch
	CLBATCHE,					// 0x001B - Close Batch - End upload
	RUPLOAD,					// 0x001C - Close Batch - Upload
	LASTRSP,					// 0x001D - Last Response Display
	OPENTAB,					// 0x001E - Open Tab
	CLOSETAB,					// 0x001F - Close Tab
	DEBITCONF,					// 0x0020 - Debit confirmation message
	CLTERM,						// 0x0021 - Visa 2 terminal parameters
	REFDEBIT,					// 0x0022 - Debit Refund Transaction
	SALCASH,					// 0x0023 - Debit Sale + Cash
	DBCASH,						// 0x0024 - Debit CASH only
	CHANGESHIFT_FUN,			// 0x0025 - Change Shift Function
	SETUP_FUN,					// 0x0026 - Setup Terminal Info
	SETDT_FUN,					// 0x0027 - Setup Date Time
	// CASHIER_FUN,				// 0x0028 - Cashier Function
	TESTTRAN_FUN,				// 0x0029 - Test Transaction
	TESTPRT_FUN,				// 0x002A - Printer Test
	TESTCRDR_FUN,				// 0x002B - Cardreader Test
	RESET_FUN,					// 0x002C - Restart Terminal
	PROGLD_FUN,					// 0x002D - Program Load Function
	TERMID_FUN,					// 0x002E - Terminal ID Function
	INITPHONE_FUN,				// 0x002F - Initialization Phone Info
	INITNMS_FUN,				// 0x0030 - Init NMS Phone Info
	DIALTYPE_FUN,				// 0x0031 - Dial Type Function
	PABX_FUN,					// 0x0032 - PABX Number
	TERMALL_FUN,				// 0x0033 - All Terminal Info
	TERMSETUP_FUN,				// 0x0034 - Setup Terminal
	PRTENABLE_FUN,				// 0x0035 - Enable Printer
	RECEIPTTEXT_FUN,			// 0x0036 - Set 4 Text Lines- Receipt
	PAPERFEED_FUN,				// 0x0037 - Printer Paperfeed
	PSWDCHANGE_FUN,				// 0x0038 - Change Merchant Password
	PSWDONOFF_FUN,				// 0x0039 - Turn ON/OFF Password
	COMM_FUN,					// 0x003A - Communications Function
	SETCONTRAST_FUN,			// 0x003B - Set Display Contrast
	DIALOPTS_FUN,				// 0x003C - Phone Dial Options
	TABPROC_FUN,				// 0x003D - Tab Processing
	CLEAR_AQ_FUN,				// 0x003E - Clear Acquirer Tables
	PRTH_FUN,					// 0x003F - Print Host Table
	INITR_FUN,					// 0x0040 - Print Init Reprot
	TXRX_FUN,					// 0x0041 - Print Transmit/Receive Log
	TESTDISP_FUN,				// 0x0042 - Test the Display
	TESTKYBD_FUN,				// 0x0043 - Test the Keyboard
	PINPADVER_FUN,				// 0x0044 - Display Pinpa Version
	TERMVER_FUN,				// 0x0045 - Display Terminal Version
	TESTPINPAD_FUN,				// 0x0046 - Test the pinpad
	LOGON,						// 0x0047 - Logon to get working key
	ICEPAK_DOWNLOAD,			// 0x0048 - download ICEPAK graphics files
	SIGBATCH,					// 0x0049 - Signature batch upload
	SIGBATCHE,					// 0x004A - Signature batch end
	SIGUPL,						// 0x004B - Signature upload trans
	BROWSER_APP,				// 0x004C - HTML/POP3 Browser
	CLEAR_REV_FUN,				// 0x004D - Clear all reversals function.
	CLEAR_ALL_FUN,				// 0x004E - Clear everything function.
	MAIL_ORDER,					// 0x004F - Mail/Phone Order(Visanet Only)
	TIPS_FUN,					// 0x0050 - Tip Percentages function
	SETTLE_STATUS_FUN,			// 0x0051 - Settlement Status function
	SET_LANGUAGE,				// 0x0052 - Set the Current Language
	SET_IDLEBUTTON_FUN,			// 0x0053 - Set new idle button screen
	PERFORMANCE_REPORT,			// 0x0054 - Print Performance Report
	CONFIG_REPORT,				// 0x0055 - Print Config. Report
	TESTCHKREADER_FUN,			// 0x0056 - Check Reader Test
	CLEAR_HFS_FUN,				// 0x0057 - Clear HFS function

#ifdef MAKE_EMV
	EMV_SALE,					// 0x0058 - EMV Sale transaction (depends on build)	
    EMV_INSTALLMENT,	
	EMV_CASH,					// 0x0059 - EMV Cash
	EMV_SALCASH,				// 0x005A - EMV Sale with Cash
	EMV_REFUND,					// 0x005B - EMV Refund
	EMV_FALLBACK,				// 0x005C - EMV FallBack        
#endif // MAKE_EMV

    CHG_APP,                    // 0x005D - Switch to another Application
    TRACE_START,                // 0x005E - Serial Trace start  
    TRACE_STOP,                 // 0x005F - Serial Trace stop   
    TRACE_PRINT,                // 0x0060 - Serial Trace print  
    TRACE_SEND2,                // 0x0061 - Serial Trace send trough COM2 port
    TRACE_SEND1,                // 0x0062 - Serial Trace send trough COM1 port
    DBINITPREF_FUN,				// 0x0063 - Dial Backup Init preferred service
    DBNMSPREF_FUN,				// 0x0064 - Dial Backup NMS preferred service
    DBINITPHONE_FUN,	  		// 0x0065 - Dial Backup Init phone number
    DBNMSPHONE_FUN,				// 0x0066 - Dial Backup NMS phone number
    INSTALLMENT,				// 0x0067 -Installment @Aydeniz -- BC --
    REFUND,						// 0x0068 - REFUND TRANSACTION
    INSTAL_REFUND,              // 0x0069 - INSTALMENT REFUND TRANSACTION @Aydeniz BC 
    PREAUTH,                    // 0x0070 - PREAUTH TRANSACTION @Aydeniz BC 
    CLOSED_PREAUTH,             // 0x0071 - CLOSED PREAUTH TRANSACTION @Aydeniz BC 
    STATISTICS,                 // 0x0072 - STATISTIC
	MENU_DBFORMATTED,           // 0x0073 - DB tablolarýný formatlý biçimde yazdýrmak için @erd 06.03.2007
	IPCONFIG_FUN,				// 0x0074 - 106 - IP Configuration: 0=Auto 1=Manual
	NMSIP_FUN,					// 0x0075 - 107 - NMS Dest. IP Address
	NMSPORT_FUN,				// 0x0076 - 108 - NMS Dest. Port Number
	INITIP_FUN,					// 0x0077 - 109 - Init Dest IP Address
	INITPORT_FUN,				// 0x0078 - 110 - Init Dest Port Number
	LOCALIP_FUN,				// 0x0079 - 111 - Local IP Address
	LOCALSUBNET_FUN,			// 0x0080 - 112 - Local Subnet Mask
	GATEWAYIP_FUN,				// 0x0081 - 113 - Gateway IP Address
#ifdef MAKE_KIB
        LOYALTY_INQ,                            // 0x0082 - 114 - Loyalty Inquiry
        DISCOUNT_SALE,                          // 0x0083 - 115 - Ýndirimli Satýþ 
        DISCOUNT_INSTALLMENT,                   // 0x0084 - 116 - Ýndirimli Taksitli Satýþ
#endif


	NOT_SUPPORTED,				// Display not supported message.
	END_OF_EVENTS,				// this marks end of function events

	// key definitions must be defined below 231
	// SDKOS will reseve the values from 231 - 255
	ENTER_KY = 200,				//  ENTER key from keypad
	CLEAR_KY,					//  CLEAR key from keypad
	BSP_KY,						//  Back space from keypad
	B_PREVIOUS,					//  Previous Screen
	B_PREVIOUS_NEW_MENU,		//  Previous Screen and new Menu
	B_NEXT,						//  Next Screen
	ABC_KY,						//  Alpha select key
	CANCEL_KY,					//  CANCEL Key from touch pad
	DOWN_KY,                    // menu down key
    LEFT_DOWN_KY = DOWN_KY, 
    SELECT_KY,                  // menu select key 
    LEFT_MIDDLE_KY = SELECT_KY,
	UP_KY,                      // menu up key
    LEFT_UP_KY = UP_KY,         // sol ust
    RIGHT_DOWN_KY,    
    RIGHT_MIDDLE_KY,            // sag orta 
    RIGHT_UP_KY,                // sag ust buton
	
    POINT_KEY,

	EVENT_SPECIAL = 256,		// special events used by SDL below this
	EVENT_USER0,
	EVENT_USER1,
	EVENT_USER2,
	EVENT_USER3,
	EVENT_USER4,
	EVENT_USER5,
	EVENT_USER6,
	EVENT_USER7,
	EVENT_USER8,
	EVENT_USER9,
	EVENT_USER10,
	EVENT_USER11,
	EVENT_USER12,
	EVENT_USER13,
	EVENT_USER14,
	EVENT_USER15,
	EVENT_ICEBUTTON1,
	EVENT_ICEBUTTON2,
	EVENT_ICEBUTTON3,
	EVENT_ICEBUTTON4,
	EVENT_MAX = 0x1000			// NO FUNCTION EVENT BEYOND THIS POINT
};


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================


#endif // _TRANSDEF_H_

