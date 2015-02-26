//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      evtgrtext.c
//
// File Description:
//      German event text table
//
//=============================================================================

#ifdef MAKE_GERMAN

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkmem.h"

#include "struct.h"
#include "appdata.h"
#include "evttextinit.h"
#include "text.h"
#include "syserr.h"
#include "eventbl.h"

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

//====================================================
//!===  GERMAN EVENT MESSAGE TABLE                 ===
//====================================================
const char *const EvtGrText[] = {
	// NOOPT must be first entry in this table -- do not remove
	"",							// NOOPT            Event Text
	"VERKAUF",						// SALE             Event Text
	"RUECKBLICK",					// REVIEW           Event Text
	"ABSCHLUSS",					// SETTLE           Event Text
	"FUNKTION",				// FUNCTION         Event Text
	"INITIALIS.",				// INITIALIZE       Event Text
	"KARTE PRUE",					// CRDVER           Event Text
	"DEBIT",					// DEBIT            Event Text
	"",							// RESERVED			Event Text
	"FINDEN",						// FIND             Event Text
	"AENDRN",					// ADJUST           Event Text
	"BALANCE",					// BALINQ           Event Text
	"KOPIE",					// REPRINT          Event Text
	"STORNO",						// VOIDTRAN         Event Text
    "PREAUTHCANCEL",             //          Preauth Cancel
    "CLOSEDPAUTHCANCEL",         //          Closed Preauth Cancel
	"BERICHT",					// REPORTS          Event Text
	"TOTALE",					// TOTALS           Event Text
	//"GUTSCHRIFT",					// REFUND           Event Text
	"OFFLINE",					// SALOFFL          Event Text
	"DRUCK.LIST",				// PRTLIST          Event Text
	"DRUCK.KASS",				// PRTTILL          Event Text
	"TEST",						// TESTTRAN         Event Text
	"AUTHORIS.",				// AUTH             Event Text
	"VERK.AEND.",				// ADJSALE          Event Text
	"GUTS.AEND.",				// ADJREFUND        Event Text
	"TRAIN MODE",				// TRAINMODE        Event Text
	"ABSCHLUSS",					// CLBATCH          Event Text
	"ABSCHLUSS",					// CLBATCHA         Event Text
	"ABSCHLUSS",					// CLBATCHE         Event Text
	"UPLOAD",					// RUPLOAD          Event Text
	"ANTWORT",					// LASTRSP          Event Text
	"RECH.OEFFN",					// OPENTAB          Event Text
	"RECH.SCHLI",				// CLOSETAB         Event Text
	"         ",				// DEBITCONF        Event Text
	"    BATCH",				// CLTERM           Event Text
	"DEBIT REF",				// REFDEBIT         Event Text
	"KAUF+BAR",					// SALCASH          Event Text
	"BAR",						// DBCASH           Event Text
	"",							// CHANGESHIFT_FUN  Event Text
	"",							// SETUP_FUN        Event Text
	"",							// SETDT_FUN        Event Text
	"",							// CASHIER_FUN      Event Text
	"",							// TESTTRAN_FUN     Event Text
	"",							// TESTPRT_FUN      Event Text
	"",							// TESTCRDR_FUN     Event Text
	"",							// RESET_FUN        Event Text
	"",							// PROGLD_FUN       Event Text
	"",							// TERMID_FUN       Event Text
	"",							// INITPHONE_FUN    Event Text
	"",							// INITNMS_FUN      Event Text
	"",							// DIALTYPE_FUN     Event Text
	"",							// PABX_FUN         Event Text
	"",							// TERMALL_FUN      Event Text
	"",							// TERMSETUP_FUN    Event Text
	"",							// PRTENABLE_FUN    Event Text
	"",							// RECEIPTTEXT_FUN  Event Text
	"",							// PAPERFEED_FUN    Event Text
	"",							// PSWDCHANGE_FUN   Event Text
	"",							// PSWDONOFF_FUN    Event Text
	"",							// COMM_FUN         Event Text
	"",							// SETCONTRAST_FUN  Event Text
	"",							// DIALOPTS_FUN     Event Text
	"",							// TABPROC_FUN      Event Text
	"",							// CLEAR_AQ_FUN     Event Text
	"",							// PRTH_FUN         Event Text
	"",							// INITR_FUN        Event Text
	"",							// TXRX_FUN         Event Text
	"",							// TESTDISP_FUN     Event Text
	"",							// TESTKYBD_FUN     Event Text
	"",							// PINPADVER_FUN    Event Text
	"",							// TERMVER_FUN      Event Text
	"",							// TESTPINPAD_FUN   Event Text
	"LOGON",					// LOGON            Event Text
	"ICEPAC",					// ICEPAK_DOWNLOAD  Event Text
	"         ",				// SIGBATCH         Event Text
	"         ",				// SIGBATCHE        Event Text
	"         ",				// SIGUPL           Event Text
	"BROWSER",					// BROWSER_APP      Event Text
	"",							// CLEAR_REV_FUN    Event Text
	"",							// CLEAR_ALL_FUN    Event Text
	"MAIL ORDER",				// MAIL_ORDER       Event Text
	"",							// TIPS_FUN         Event Text
	"",							// SETTLE_STATUS_FUN Event Text
	"",							// SET_LANGUAGE     Event Text
	"",							// SET_IDLEBUTTON_FUN   Event Text
	"",							// Print Performance Report
	"",							// Print Config. Report
	"",							// TESTCHKREADER_FUN Event Text
	"",							// CLEAR_HFS_FUN    Event Text

#ifdef MAKE_EMV
	"KAUF",                     // EMV_SALE
    "EMV_INSTALLMENT",	
	"BAR",                     // EMV_CASH
	"KAUF+BAR",                // EMV_SALCASH
	"GUTSCHRIFT",                   // EMV_REFUND
	"FALLBACK",                 // EMV_FALLBACK
#endif // MAKE_EMV

	"APP.WECHS.",               // CHG_APP
    "",                         // TRACE_START
    "",                         // TRACE_STOP
    "",                         // TRACE_PRINT
    "",                         // TRACE_SEND
    "",                         // TRACE_SEND1
    "",                         // DBINITPREF_FUN
    "",                         // DBNMSPREF_FUN
    "",                         // DBINITPHONE_FUN
    "",                         // DBNMSPHONE_FUN
    "INSTALLMENT",				// 0x0067 -Installment @Aydeniz -- BC --
    "REFUND",						// 0x0068 - REFUND TRANSACTION
    "I. REFUND",              // 0x0069 - INSTALMENT REFUND TRANSACTION @Aydeniz BC 
    "PREAUTH",                    // 0x0070 - PREAUTH TRANSACTION @Aydeniz BC 
    "CLOSED PRE",             // 0x0071 - CLOSED PREAUTH TRANSACTION @Aydeniz BC 
    "STATISTICS",                 // 0x0072 - STATISTIC
    "DB FORMATTED",           // 0x0073 - DB tablolarýný formatlý biçimde yazdýrmak için @erd 06.03.2007
    "IPCONFIG",				// 0x0074 - 106 - IP Configuration: 0=Auto 1=Manual
    "NMSIP",					// 0x0075 - 107 - NMS Dest. IP Address
    "NMSPORT",				// 0x0076 - 108 - NMS Dest. Port Number
    "INITIP",					// 0x0077 - 109 - Init Dest IP Address
    "INITPORT",				// 0x0078 - 110 - Init Dest Port Number
    "LOCALIP",				// 0x0079 - 111 - Local IP Address
    "LOCALSUBNET",			// 0x0080 - 112 - Local Subnet Mask
    "GATEWAYIP",				// 0x0081 - 113 - Gateway IP Address
#ifdef MAKE_KIB
        "LOY. INQ.",                            // 0x0082 - 114 - Loyalty Inquiry
        "DISC. SALE",                          // 0x0083 - 115 - Ýndirimli Satýþ 
        "DISC. INST",                   // 0x0084 - 116 - Ýndirimli Taksitli Satýþ
#endif
    "",                         // EXIT_FUN

	"",							// NOT_SUPPORTED    Event Text
};


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add German Event table for DML support.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void EvtTextGr_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	if ( EventTableSize(  ) != ( sizeof( EvtGrText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "EvtGrText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table EvtEnText into database
	TextIndexAddTable( evttext_table_id, LNG_DE, EvtGrText, pgnum );

}


//=============================================================================
// Private function definitions
//=============================================================================


#endif // MAKE_GERMAN
