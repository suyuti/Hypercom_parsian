
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             eventbl.c
//      Event table
//
//=============================================================================

#include "string.h"
#include "basictyp.h"

#include "defines.h"
#include "transdef.h"
#include "struct.h"
#include "syserr.h"
#include "appdata.h"
#include "trainmod.h"
#include "function.h"
#include "totlook.h"
#include "doprtlst.h"
#include "setup.h"
#include "idlebutt.h"
#include "pinpad.h"
#include "app1.h"
#include "app2.h"
#include "app4.h"
#include "review.h"
#include "adjust.h"
#include "void.h"
#include "tabclose.h"
#include "settle.h"
#include "reports.h"
#include "shift.h"
#include "tabproc.h"
#include "cashier.h"
#include "debugprt.h"
#include "perfutil.h"
#include "testprnt.h"
#include "eventbl.h"
#include "prtDetayli.h"

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


// Eventtable for HYPERTFC.C
// Note that the Transaction Name Text is no longer in the event structure.
// It is in the event language tables.

const struct event_entry EVENT_TABLE[] = {
	// NOOPT must be first entry in this table -- do not remove
	{
	 NOOPT,						// Transaction Key
	 FuncNotSupported,		// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 SALE,						// Transaction Key
	 APP1SALE,  				// Action handler
	 "",						// Transaction name
	 T1_POSTADV,				// OPTION BYTE 1
	 T2_REFER + T2_FLOOR + T2_D8_SUPPORT,	// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 REVIEW,					// Transaction Key
	 Review,					// Action handler
	 "",						// Transaction name
	 T1_NOTIP + T1_NOACCT + T1_NORECEIPT + T1_NOCAPTURE,	// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NOISNAME + T3_NO_INIT,	// OPTION BYTE 3
	 T4_NOCLRBAT + T4_PINRES,	// OPTION BYTE 4
	 0},

	{
	 SETTLE,					// Transaction Key
	 Settle,					// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NOISNAME,				// OPTION BYTE 3
	 T4_PINRES + T4_NOPINDSP,	// OPTION BYTE 4
	 0},

	{
	 FUNCTION,					// Transaction Key
	 MerchFunction,             // Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 INITIALIZE,				// Transaction Key
	 Initialize,				// Action handler
	 "",						// Transaction name
	 T1_NOTIP + T1_NOACCT +		// OPTION BYTE 1
	 T1_NORECEIPT + T1_NOCAPTURE + T1_NOTIME,
	 T2_NOPREREV + T2_COMPLETE,	// OPTION BYTE 2
	 T3_NOISNAME + T3_NO_INIT +	// OPTION BYTE 3
	 T3_TERMPARAM,
	 T4_PINRES + T4_NOPINDSP,	// OPTION BYTE 4
	 0},

	{
	 CRDVER,					// Transaction Key
	 APP1CARDVER,				// Action handler
	 "",						// Transaction name
	 T1_NOCAPTURE + T1_NOTIP + T1_NORECEIPT,	// OPTION BYTE 1
	 T2_REFER,					// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 DEBIT,						// Transaction Key
	 FuncNotSupported,		// Action handler
	 "",						// Transaction name
	 T1_POSTADV,				// OPTION BYTE 1
	 T2_REFER + T2_FLOOR + T2_D8_SUPPORT,	// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 T4_DEBCARD,				// OPTION BYTE 4
	 0},

	{
	 RESTART,					// Transaction Key
     TerminalQuickReset,
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 FIND,						// Transaction Key
	 Find,  					// Action handler
	 "",						// Transaction name
	 T1_NOTIP + T1_NOACCT + T1_NORECEIPT + T1_NOCAPTURE,	// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NOISNAME + T3_NO_INIT,	// OPTION BYTE 3
	 T4_NOCLRBAT + T4_PINRES,	// OPTION BYTE 4
	 0},

	{
	 ADJUST,					// Transaction Key
	 Adjust,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_CHK_PEND,				// OPTION BYTE 3
	 T4_PINRES,					// OPTION BYTE 4
	 0},

	{
	 BALINQ,					// Transaction Key
	 BalInq,					// Action handler
	 "",						// Transaction name
	 T1_NOCAPTURE + T1_NOTIP,	// OPTION BYTE 1
	 T2_D8_SUPPORT,				// OPTION BYTE 2
	 T3_CHK_PEND,				// OPTION BYTE 3
	 T4_NOPINDSP,				// OPTION BYTE 4
	 0},

	{
	 REPRINT,					// Transaction Key
	 Reprint,					// Action handler
	 "",						// Transaction name
	 T1_NOTIP + T1_NOACCT + T1_NORECEIPT + T1_NOCAPTURE,	// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NOISNAME + T3_NO_INIT,	// OPTION BYTE 3
	 T4_NOCLRBAT + T4_PINRES + T4_NOPINDSP,	// OPTION BYTE 4
	 0},

	{
	 VOIDTRAN,					// Transaction Key
	 PreVoidtran,  				// Action handler
	 "",						// Transaction name
	 T1_NOCAPTURE + T1_NOTIP + T1_CREDIT,	// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV + T3_ACCEPTED,	// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

    {
	 PREAUTHCANCEL,				// Transaction Key
	 //Voidtran,  				// Action handler
     PreAuthCancel,
	 "",						// Transaction name
	 T1_NOCAPTURE + T1_NOTIP + T1_CREDIT,	// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV + T3_ACCEPTED,	// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0
    },
    {
	 CLOSEDPAUTHCANCEL,				// Transaction Key
	 //Voidtran,  				// Action handler
     PreAuthClosedCancel,
	 "",						// Transaction name
	 T1_NOCAPTURE + T1_NOTIP + T1_CREDIT,	// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV + T3_ACCEPTED,	// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0
    },

	{
	 REPORTS,					// Transaction Key
	 Reports,					// Action handler
	 "",						// Transaction name
	 T1_NOTIP + T1_NOACCT +		// OPTION BYTE 1
	 T1_NORECEIPT + T1_NOCAPTURE,
	 0,							// OPTION BYTE 2
	 T3_NOISNAME + T3_NO_INIT,	// OPTION BYTE 3
	 T4_NOCLRBAT + T4_PINRES,	// OPTION BYTE 4
	 0},

	{
	 TOTALS,					// Transaction Key
	 TotalLook,                 // Action handler
	 "",						// Transaction name
	 T1_NOTIP + T1_NOACCT +		// OPTION BYTE 1
	 T1_NORECEIPT + T1_NOCAPTURE,
	 0,							// OPTION BYTE 2
	 T3_NOISNAME + T3_NO_INIT,	// OPTION BYTE 3
	 T4_NOCLRBAT + T4_PINRES,	// OPTION BYTE 4
	 0},

/*
	{
	 REFUND,					// Transaction Key
	 Refund,					// Action handler
	 "",						// Transaction name
	 T1_CREDIT,					// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},
*/
	{
	 SALOFFL,					// Transaction Key
	 SalOffl,					// Action handler
	 "",						// Transaction name
	 T1_NOTIME,					// OPTION BYTE 1
	 T2_OFFLINE + T2_REPRINT + T2_D8_SUPPORT,	// OPTION BYTE 2
	 T3_CHK_PEND + T3_ACCEPTED,	// OPTION BYTE 3
	 T4_CHKEXPD,				// OPTION BYTE 4
	 0},

	{
	 PRTLIST,					// Transaction Key
	 DoPrtList, 				// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 T4_NOPINDSP,				// OPTION BYTE 4
	 0},

	{
	 PRTTILL,					// Transaction Key
	 DoPrtTill, 				// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 T4_NOPINDSP,				// OPTION BYTE 4
	 0},

	{
	 TESTTRAN,					// Transaction Key
	 TstTran,					// Action handler
	 "",						// Transaction name
	 T1_NOCAPTURE + T1_NOTIP + T1_NORECEIPT + T1_NOACCT,	// OPTION BYTE 1
	 T2_NOPREREV + T2_COMPLETE,	// OPTION BYTE 2
	 T3_NOISNAME,				// OPTION BYTE 3
	 T4_NOPINDSP,				// OPTION BYTE 4
	 0},

	{
	 AUTH,						// Transaction Key
	 Auth,  					// Action handler
	 "",						// Transaction name
	 T1_NOCAPTURE,				// OPTION BYTE 1
	 T2_REFER + T2_FLOOR,		// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 ADJSALE,					// Transaction Key
	 FuncNotSupported,		// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 ADJREFUND,					// Transaction Key
	 FuncNotSupported,		// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT + T1_CREDIT,	// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TRAINMODE,					// Transaction Key
	 TrainMode,                 // Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_CHK_PEND,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 CLBATCH,					// Transaction Key
	 ClBatch,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT + T1_NOACCT + T1_PREADV + T1_NOCAPTURE,	// OPTION BYTE 1
	 T2_SETTLE,					// OPTION BYTE 2
	 T3_BATPEND,				// OPTION BYTE 3
	 T4_NOPINDSP,				// OPTION BYTE 4
	 0},

	{
	 CLBATCHA,					// Transaction Key
	 ClBatcha,              	// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT + T1_NOACCT + T1_PREADV + T1_NOCAPTURE,	// OPTION BYTE 1
	 T2_SETTLE,					// OPTION BYTE 2
	 T3_BATPEND,				// OPTION BYTE 3
	 T4_NOPINDSP,				// OPTION BYTE 4
	 0},

	{
	 CLBATCHE,					// Transaction Key
	 ClBatche,  				// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT + T1_PREADV + T1_NOACCT + T1_NOCAPTURE,	// OPTION BYTE 1
	 T2_SETTLE,					// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 T4_NOPINDSP,				// OPTION BYTE 4
	 0},

	{
	 RUPLOAD,					// Transaction Key
	 RUpload, 					// Action handler
	 "",						// Transaction name
	 T1_NOCAPTURE,				// OPTION BYTE 1
	 T2_SETTLE,					// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 T4_NOPINDSP,				// OPTION BYTE 4
	 0},

	{
	 LASTRSP,					// Transaction Key
	 LastRsp,   				// Action handler
	 "",						// Transaction name
	 T1_NOTIP + T1_NOACCT + T1_NORECEIPT + T1_NOCAPTURE,	// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NOISNAME + T3_NO_INIT,	// OPTION BYTE 3
	 T4_NOCLRBAT,				// OPTION BYTE 4
	 0},

	{
	 OPENTAB,					// Transaction Key
	 OpenTabProc,				// Action handler
	 "",						// Transaction name
	 T1_POSTADV,				// OPTION BYTE 1
	 T2_REFER + T2_FLOOR,		// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 CLOSETAB,					// Transaction Key
	 CloseTabProc,  			// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_CHK_PEND,				// OPTION BYTE 3
	 T4_PINRES,					// OPTION BYTE 4
	 0},

	{
	 DEBITCONF,					// Transaction Key
	 FuncNotSupported,          // Action handler
	 "",						// Transaction name
	 T1_POSTADV,				// OPTION BYTE 1
	 T2_REFER + T2_FLOOR,		// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 T4_CHKEXPD,				// OPTION BYTE 4
	 0},

	{
	 CLTERM,					// Transaction Key
	 FuncNotSupported,          // Action handler
	 "",						// Transaction name
	 T1_NORECEIPT + T1_NOACCT + T1_PREADV + T1_NOCAPTURE,	// OPTION BYTE 1
	 T2_SETTLE,					// OPTION BYTE 2
	 T3_BATPEND,				// OPTION BYTE 3
	 T4_NOPINDSP,				// OPTION BYTE 4
	 0},

	{
	 REFDEBIT,					// Transaction Key
	 FuncNotSupported,          // Action handler
	 "",						// Transaction name
	 T1_NOTIP + T1_CREDIT,		// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 T4_DEBCARD,				// OPTION BYTE 4
	 0},

	{
	 SALCASH,					// Transaction Key
	 SalCash,   				// Action handler
	 "",						// Transaction name
	 T1_POSTADV,				// OPTION BYTE 1
	 T2_REFER + T2_FLOOR + T2_D8_SUPPORT,	// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 T4_DEBCARD,				// OPTION BYTE 4
	 0},

	{
	 DBCASH,					// Transaction Key
	 DbCash,					// Action handler
	 "",						// Transaction name
	 T1_POSTADV,				// OPTION BYTE 1
	 T2_REFER + T2_FLOOR + T2_D8_SUPPORT,	// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 T4_DEBCARD,				// OPTION BYTE 4
	 0},

	{
	 CHANGESHIFT_FUN,			// Transaction Key
	 ChangeShift,   			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 SETUP_FUN,					// Transaction Key
	 Fun_2,  					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 SETDT_FUN,					// Transaction Key
	 Fun_10,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 CASHIER_FUN,				// Transaction Key
	 Fun_78,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TESTTRAN_FUN,				// Transaction Key
	 Fun_0,					    // Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TESTPRT_FUN,				// Transaction Key
	 TestPrinter,				// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TESTCRDR_FUN,				// Transaction Key
	 Fun_89,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 RESET_FUN,					// Transaction Key
	 Fun_3, 					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 PROGLD_FUN,				// Transaction Key
	 Fun_92,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TERMID_FUN,				// Transaction Key
	 XInputTermSer,  			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 INITPHONE_FUN,				// Transaction Key
	 XInputInitTel,  			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 INITNMS_FUN,				// Transaction Key
	 XInputNMSTel,   			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 DIALTYPE_FUN,				// Transaction Key
	 XInputDialMode,  			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 PABX_FUN,					// Transaction Key
	 XInputPABX,  				// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TERMALL_FUN,				// Transaction Key
	 TermAll, 					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TERMSETUP_FUN,				// Transaction Key
	 TerminalSetup, 			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 PRTENABLE_FUN,				// Transaction Key
	 Fun_71,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 RECEIPTTEXT_FUN,			// Transaction Key
	 Fun_26,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 PAPERFEED_FUN,				// Transaction Key
	 PaperFeed, 				// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 PSWDCHANGE_FUN,			// Transaction Key
	 Fun_90,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 PSWDONOFF_FUN,				// Transaction Key
	 PasswordOptions,			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 COMM_FUN,					// Transaction Key
	 Fun_98,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 SETCONTRAST_FUN,			// Transaction Key
	 SetContrast, 				// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 DIALOPTS_FUN,				// Transaction Key
	 FuncNotSupported,		// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TABPROC_FUN,				// Transaction Key
	 TabProcessing, 			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 CLEAR_AQ_FUN,				// Transaction Key
	 Fun_99_9_2,				// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 PRTH_FUN,					// Transaction Key
	 Fun_99_96, 				// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},
	{
	 INITR_FUN,					// Transaction Key
	 PrintDataBase, 			// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},
	{
	 TXRX_FUN,					// Transaction Key
	 PrintDataScope,			// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TESTDISP_FUN,				// Transaction Key
	 Fun_83,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TESTKYBD_FUN,				// Transaction Key
	 Fun_84,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 PINPADVER_FUN,				// Transaction Key
	 (void (*)())PinDisplayDiag, // Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TERMVER_FUN,				// Transaction Key
	 Fun_91,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 TESTPINPAD_FUN,			// Transaction Key
	 Fun_94,					// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 LOGON,						// Transaction Key
	 Logon,					// Action handler
	 "",						// Transaction name
	 T1_NOTIP + T1_NOACCT + T1_NORECEIPT + T1_NOCAPTURE,	// OPTION BYTE 1
	 T2_NOPREREV + T2_COMPLETE,	// OPTION BYTE 2
	 T3_NOISNAME,				// OPTION BYTE 3
	 T4_NOPINDSP,				// OPTION BYTE 4
	 0},

	{
	 ICEPAK_DOWNLOAD,			// Transaction Key
	 Fun_14,					// Action handler
	 "",						// Transaction name
	 T1_NOCAPTURE,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 SIGBATCH,					// Transaction Key
	 FuncNotSupported,		// Action handler
	 "         ",				// Transaction name
	 T1_NORECEIPT + T1_NOACCT + T1_PREADV + T1_NOCAPTURE,	// OPTION BYTE 1
	 T2_SETTLE,					// OPTION BYTE 2
	 T3_BATPEND,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 SIGBATCHE,					// Transaction Key
	 FuncNotSupported,		// Action handler
	 "         ",				// Transaction name
	 T1_NORECEIPT + T1_PREADV + T1_NOACCT + T1_NOCAPTURE,	// OPTION BYTE 1
	 T2_SETTLE,					// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 SIGUPL,					// Transaction Key
	 FuncNotSupported,		// Action handler
	 "         ",				// Transaction name
	 T1_NOCAPTURE,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 BROWSER_APP,				// Transaction Key
	 FuncNotSupported,          // Action handler
	 "",						// Transaction name
	 T1_NOTIP + T1_NOACCT +		// OPTION BYTE 1
	 T1_NORECEIPT + T1_NOCAPTURE + T1_NOTIME,
	 T2_NOPREREV + T2_COMPLETE,	// OPTION BYTE 2
	 T3_NOISNAME + T3_NO_INIT,	// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0							// Next event
	 },

	{
	 CLEAR_REV_FUN,				// Transaction Key
	 Fun_99_9_6,				// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 CLEAR_ALL_FUN,				// Transaction Key
	 Fun_99_9_1,				// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 MAIL_ORDER,				// Transaction Key
	 MailOrder, 				// Action handler
	 "",						// Transaction name
	 T1_POSTADV,				// OPTION BYTE 1
	 T2_REFER + T2_FLOOR + T2_D8_SUPPORT,	// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0							// Next event
	 },

	{
	 TIPS_FUN,					// Transaction Key
	 TipPercentages,			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 SETTLE_STATUS_FUN,			// Transaction Key
	 SettleStatus,  			// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 SET_LANGUAGE,				// Transaction Key
	 TextLanguageSelectMenu,	// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 SET_IDLEBUTTON_FUN,		// Transaction Key
	 SetIdleButton, 			// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 PERFORMANCE_REPORT,		// Transaction Key
	 PrintPerformRpt, 			// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 CONFIG_REPORT,				// Transaction Key           
	 DoConfigRpt,				// Action handler 
	 "",						// Transaction name          
	 0,							// OPTION BYTE 1             
	 0,							// OPTION BYTE 2             
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4             
	 0},

	{
	 TESTCHKREADER_FUN,			// Transaction Key
	 FuncNotSupported,          // Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 CLEAR_HFS_FUN,				// Transaction Key
	 Fun_99_9_8,				// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

#ifdef MAKE_EMV
	{
		EMV_SALE,			 	// Transaction Key
		EMVPROC,			 	// Action handler (common for all EMV Txns)
		"",						// Transaction name
		T1_POSTADV,				// OPTION BYTE 1
		T2_REFER,		// OPTION BYTE 2
		T3_CHK_PEND + T3_REV,	// OPTION BYTE 3
		0,					 	// OPTION BYTE 4
		0					 	// Next event
	},
        {
		EMV_INSTALLMENT,			 	// Transaction Key
		EMVPROC,			 	// Action handler (common for all EMV Txns)
		"",						// Transaction name
		T1_POSTADV ,				// OPTION BYTE 1
		T2_REFER,		// OPTION BYTE 2
		T3_CHK_PEND + T3_REV,	// OPTION BYTE 3
		0,					 	// OPTION BYTE 4
		0					 	// Next event
	},

	{
		EMV_CASH,			 	// Transaction Key
		EMVPROC,			 	// Action handler
		"",						// Transaction name
		T1_POSTADV,				// OPTION BYTE 1
		T2_REFER, 		// OPTION BYTE 2
		T3_CHK_PEND + T3_REV,	// OPTION BYTE 3
		0,					 	// OPTION BYTE 4
		0					 	// Next event
	},

	{
		EMV_SALCASH,		 	// Transaction Key
		EMVPROC,				// Action handler
		"",						// Transaction name
		T1_POSTADV,				// OPTION BYTE 1
		T2_REFER,		// OPTION BYTE 2
		T3_CHK_PEND + T3_REV,	// OPTION BYTE 3
		0,					 	// OPTION BYTE 4
		0					 	// Next event
	},

	{
		EMV_REFUND,				// Transaction Key
		EMVPROC,				// Action handler
		"",						// Transaction name
		T1_POSTADV+T1_CREDIT,	// OPTION BYTE 1
		T2_REFER,		// OPTION BYTE 2
		T3_CHK_PEND + T3_REV,	// OPTION BYTE 3
		0,					 	// OPTION BYTE 4
		0					 	// Next event
	},

	{
		EMV_FALLBACK,	  		// Transaction Key
		FALLBACK,				// Action handler
		"",						// Transaction name
		T1_NOCAPTURE,				// OPTION BYTE 1
		0,							// OPTION BYTE 2
		0,							// OPTION BYTE 3
		0,							// OPTION BYTE 4
		0							// Next event
	},

#endif	// MAKE_EMV

	{
        CHG_APP,                    // Transaction Key
        FuncNotSupported,           // Action handler 
        "",                         // Transaction Name
        0,                          // OPTION BYTE 1
        0,                          // OPTION BYTE 2
		T3_NO_INIT,					// OPTION BYTE 3
        0,                          // OPTION BYTE 4
        0
    },

	{
        TRACE_START,                // Transaction Key
        FuncNotSupported,           // Action handler 
        "",                         // Transaction Name
        0,                          // OPTION BYTE 1
        0,                          // OPTION BYTE 2
		T3_NO_INIT,					// OPTION BYTE 3
        0,                          // OPTION BYTE 4
        0
    },

	{
        TRACE_STOP,                 // Transaction Key
        FuncNotSupported,           // Action handler 
        "",                         // Transaction Name
        0,                          // OPTION BYTE 1
        0,                          // OPTION BYTE 2
		T3_NO_INIT,					// OPTION BYTE 3
        0,                          // OPTION BYTE 4
        0
    },

	{
        TRACE_PRINT,                // Transaction Key
        FuncNotSupported,           // Action handler 
        "",                         // Transaction Name
        0,                          // OPTION BYTE 1
        0,                          // OPTION BYTE 2
		T3_NO_INIT,					// OPTION BYTE 3
        0,                          // OPTION BYTE 4
        0
    },

	{
        TRACE_SEND2,                // Transaction Key
        FuncNotSupported,           // Action handler 
        "",                         // Transaction Name
        0,                          // OPTION BYTE 1
        0,                          // OPTION BYTE 2
		T3_NO_INIT,					// OPTION BYTE 3
        0,                          // OPTION BYTE 4
        0
    },

	{
        TRACE_SEND1,                // Transaction Key
        FuncNotSupported,           // Action handler 
        "",                         // Transaction Name
        0,                          // OPTION BYTE 1
        0,                          // OPTION BYTE 2
		T3_NO_INIT,					// OPTION BYTE 3
        0,                          // OPTION BYTE 4
        0
    },

	{
	 DBINITPREF_FUN,				// Transaction Key
	 (void (*)())InputDBInitPref,  			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 DBNMSPREF_FUN,				// Transaction Key
	 (void (*)())InputDBNMSPref,   			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 DBINITPHONE_FUN,				// Transaction Key
	 (void (*)())InputDBInitTel,  			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 DBNMSPHONE_FUN,				// Transaction Key
	 (void (*)())InputDBNMSTel,   			// Action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

        {							// 66
	 INSTALLMENT,				// Transaction Key
	 Installment,				// Cross-Call action handler
	 "",						// Transaction name
	 0,				// OPTION BYTE 1
	 T2_REFER + T2_FLOOR,		// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 T4_CHKEXPD + T4_DEBCARD,	// OPTION BYTE 4
	 0},

         {
	 REFUND,					// Transaction Key
	 Refund,					// Action handler
	 "",						// Transaction name
	 T1_CREDIT,					// OPTION BYTE 1
	 0,						// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,                          // OPTION BYTE 3
	 0,						// OPTION BYTE 4
	 0},
        
        {
	 INSTAL_REFUND,					// Transaction Key
	 Instal_Refund,					// Action handler
	 "",						// Transaction name
	 T1_CREDIT,					// OPTION BYTE 1
	 0,                                             // OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,                          // OPTION BYTE 3
	 0,						// OPTION BYTE 4
	 0},

         {							// 75
	 PREAUTH,					// Transaction Key
	 PreAuth,					// Cross-Call action handler
	 "",						// Transaction name
         0,				// OPTION BYTE 1
         T2_REFER,                      // OPTION BYTE 2
         T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
         0,							// OPTION BYTE 4
	 0},

        {							// 75
	 CLOSED_PREAUTH,					// Transaction Key
	 ClosedPreAuth,					// Cross-Call action handler
	 "",						// Transaction name
	 0,				// OPTION BYTE 1
         T2_REFER,                      // OPTION BYTE 2
         T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
         0,							// OPTION BYTE 4
	 0},

         {							// 75
	 STATISTICS,					// Transaction Key
	 RUpload,					// Cross-Call action handler
	 "",						// Transaction name
	 T1_NOTIP+T1_NOACCT+T1_NORECEIPT+T1_NOCAPTURE+T1_NOTIME,	// OPTION BYTE 1
         T2_NOPREREV+T2_COMPLETE,	// OPTION BYTE 2
         T3_NOISNAME+T3_NO_INIT,	// OPTION BYTE 3
         T4_NOPINDSP,				// OPTION BYTE 4						// OPTION BYTE 4
	 0},
	 // DB Tablolarýný Formatlý Yazdýrýr @erd 06.03.2007 !!!         
         {
         MENU_DBFORMATTED,					// Transaction Key
	 PrtFormattedDB,			// Cross-Call action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 0,							// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},


	{
	 IPCONFIG_FUN,				// Transaction Key
	 FuncNotSupported,
        //XInputIPCONFIG,			// Cross-Call action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 NMSIP_FUN,					// Transaction Key
	 FuncNotSupported,
        //XInputNMSIP,				// Cross-Call action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 NMSPORT_FUN,				// Transaction Key
	 FuncNotSupported,
        //XInputNMSPORT,				// Cross-Call action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},	

	{
	 INITIP_FUN,				// Transaction Key
	 FuncNotSupported,
        //XInputINITIP,				// Cross-Call action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 INITPORT_FUN,				// Transaction Key
	 FuncNotSupported,
        //XInputINITPORT,				// Cross-Call action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 LOCALIP_FUN,				// Transaction Key
	 FuncNotSupported,
        //XInputLOCALIP,				// Cross-Call action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 LOCALSUBNET_FUN,			// Transaction Key
	 FuncNotSupported,
        //XInputLOCALSUBNET,			// Cross-Call action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

	{
	 GATEWAYIP_FUN,				// Transaction Key
	 FuncNotSupported,
        //XInputGATEWAYIP,			// Cross-Call action handler
	 "",						// Transaction name
	 T1_NORECEIPT,				// OPTION BYTE 1
	 0,							// OPTION BYTE 2
	 T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},
       
#ifdef MAKE_KIB
      {
        LOYALTY_INQ,				// Transaction Key
        LoyaltyInquiry,				// Cross-Call action handler
        "",						// Transaction name
        T1_NOCAPTURE + T1_NOTIP + T1_NOACCT,	// OPTION BYTE 1
        T2_D8_SUPPORT,				// OPTION BYTE 2
	T3_CHK_PEND,				// OPTION BYTE 3
        T4_NOPINDSP,				// OPTION BYTE 4
        0
      },
      
      {
        DISCOUNT_SALE,				// Transaction Key
        FuncNotSupported,				// Cross-Call action handler
        "",						// Transaction name
	 T1_POSTADV ,				// OPTION BYTE 1
	 T2_REFER + T2_FLOOR + T2_D8_SUPPORT,	// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 0,							// OPTION BYTE 4	  
      },

      {
        DISCOUNT_INSTALLMENT,				// Transaction Key
        FuncNotSupported,				// Cross-Call action handler
        "",						// Transaction name
	 0,				// OPTION BYTE 1
	 T2_REFER + T2_FLOOR,		// OPTION BYTE 2
	 T3_CHK_PEND + T3_REV,		// OPTION BYTE 3
	 T4_CHKEXPD + T4_DEBCARD,	// OPTION BYTE 4
	 0					// OPTION BYTE 4	   
      },


#endif


    {
     EXIT_FUN,				// Transaction Key
     (void (*)())ExitApplication,		// Action handler
     "",						// Transaction name
     0,				            // OPTION BYTE 1
     0,							// OPTION BYTE 2
     T3_NO_INIT,				// OPTION BYTE 3
     0,							// OPTION BYTE 4
     0},

	{
	 NOT_SUPPORTED,				// Transaction Key
	 FuncNotSupported,		// Action handler
	 "",						// Transaction name
	 0,							// OPTION BYTE 1
	 0,							// OPTION BYTE 2
		T3_NO_INIT,				// OPTION BYTE 3
	 0,							// OPTION BYTE 4
	 0},

};


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Returns the number of entries defined in EVENT_TABLE.
//!
//!  \param
//!     None
//!
//!  \return
//!     UWORD       Number of entries in EVENT_TABLE.
//!
//!  \note
//!     Globals:  EVENT_TABLE
//-----------------------------------------------------------------------------
UWORD EventTableSize( void )
{
	return ( sizeof( EVENT_TABLE ) / sizeof( struct event_entry ) );
}

//-----------------------------------------------------------------------------
//!	\brief
//!     Verify the number of entried in EVENT_TABLE
//!		against the enumerations in TRANSDEF.h.
//!
//!	\param
//!     None
//!
//!	\return
//!     None
//!
//!	\note
//!     Does NOT return if the size does not match.
//!     Globals:  EVENT_TABLE
//-----------------------------------------------------------------------------
void VerifyEventTableSize( void )
{
	TableItemCount = END_OF_EVENTS;

	if ( TableItemCount != EventTableSize() )
	{
		strcpy( Dspbuf, "Event Table" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}
}

//=============================================================================
// Private function definitions
//=============================================================================
