
#ifdef MAKE_TURKISH
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             evttrtext.c
//      English event text table
//
//=============================================================================

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
//===  TURKISH EVENT MESSAGE TABLE                 ===
//====================================================


const char *const EvtTrText[] = {
	// NOOPT must be first entry in this table -- do not remove
	"",							// NOOPT            Event Text
	"SATIÞ",						// SALE             Event Text
	"REVIEW",					// REVIEW           Event Text
	"GÜNSONU",					// SETTLE           Event Text
	"FONKSÝYON",				// FUNCTION         Event Text
	"PARAM. YUK",				// INITIALIZE       Event Text
	"CARD VER",					// CRDVER           Event Text
	"DEBIT",					// DEBIT            Event Text
	"",							// RESERVED			Event Text
	"FIND",						// FIND             Event Text
	"ADJUST",					// ADJUST           Event Text
	"BALANCE",					// BALINQ           Event Text
	"REPRINT",					// REPRINT          Event Text
	"ÝPTAL",					// VOIDTRAN         Event Text
	"RAPORLAR",				// REPORTS          Event Text
	"TOPLAMLAR",		// TOTALS           Event Text	
	"OFFLINE",					// SALOFFL          Event Text
	"PRINT LIST",				// PRTLIST          Event Text
	"PRINT TILL",				// PRTTILL          Event Text
	"TEST",						// TESTTRAN         Event Text
	"AUTHORIZE",				// AUTH             Event Text
	"ADJUST SAL",				// ADJSALE          Event Text
	"ADJUST REF",				// ADJREFUND        Event Text
	"TRAIN MODE",				// TRAINMODE        Event Text
	"SETTLE",					// CLBATCH          Event Text
	"SETTLE",					// CLBATCHA         Event Text
	"SETTLE",					// CLBATCHE         Event Text
	"UPLOAD",					// RUPLOAD          Event Text
	"YANIT",					// LASTRSP          Event Text
	"OPEN TAB",					// OPENTAB          Event Text
	"CLOSE TAB",				// CLOSETAB         Event Text
	" ",				// DEBITCONF        Event Text
	"BATCH",				// CLTERM           Event Text
	"DEBIT REF",				// REFDEBIT         Event Text
	"SAL+CSH",					// SALCASH          Event Text
	"CASH",						// DBCASH           Event Text
	"",							// CHANGESHIFT_FUN  Event Text
	"",							// SETUP_FUN        Event Text
	"",							// SETDT_FUN        Event Text	
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
	" ",				// SIGBATCH         Event Text
	" ",				// SIGBATCHE        Event Text
	" ",				// SIGUPL           Event Text
	"BROWSER",					// BROWSER_APP      Event Text
	"",							// CLEAR_REV_FUN    Event Text
	"",							// CLEAR_ALL_FUN    Event Text
	"MAIL ORDER",                                           // MAIL_ORDER       Event Text
	"",							// TIPS_FUN         Event Text
	"",							// SETTLE_STATUS_FUN Event Text
	"",							// SET_LANGUAGE     Event Text
	"",							// SET_IDLEBUTTON_FUN   Event Text
	"",							// Print Performance Report
	"",							// Print Config. Report
	"",							// TESTCHKREADER_FUN Event Text
	"",							// CLEAR_HFS_FUN    Event Text

#ifdef MAKE_EMV
	"SATIÞ",                    // EMV_SALE
	"TAKSÝTLÝ" ,
	" ",                 // EMV_FALLBACK
#endif // MAKE_EMV

	"CHANGE APP",               // CHG_APP
    "",                         // TRACE_START
    "",                         // TRACE_STOP
    "",                         // TRACE_PRINT
    "",                         // TRACE_SEND2
    "",                         // TRACE_SEND1
    "",                         // DBINITPREF_FUN
    "",                         // DBNMSPREF_FUN
    "",                         // DBINITPHONE_FUN
    "",                         // DBNMSPHONE_FUN
    "TAKSÝTLÝ",              // Installment   @A -- BC 
    "ÝADE",			// REFUND        @A -- BC 
    "TAK. ÝADE",		// INSTAL_REFUND @A -- BC 
    "ÖNPROVÝZYON",                  // PREAUTH       @A -- BC        
    "ÖNPROVÝZYON KAPAMA",              // CLOSED PREAUTH @A -- BC    
    "ÝSTATÝSTÝK",               // STATISTICS  @ydeniz -- BC 
    "DB DETAY",
    "",							// IPCONFIG_FUN			Event Text
    "",							// NMSIP_FUN			Event Text
    "",							// NMSPORT_FUN			Event Text
    "",							// INITIP_FUN			Event Text
    "",							// INITPORT_FUN			Event Text
    "",							// LOCALIP_FUN			Event Text
    "",							// LOCALSUBNET_FUN		Event Text
    "",							// GATEWAYIP_FUN		Event Text
#ifdef MAKE_KIB
    "PUAN SORGU",                                       // LOYALTY_INQ
    "IND. SATIÞ",                                        // DISCOUNT_SALE
    "IND. TAKS.",                                       // DISCOUNT_INST
#endif    
    "",							// NOT_SUPPORTED    Event Text
};


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  EvtTextTr_Init      Add English Event table for DML support 
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern void EvtTextTr_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	if ( EventTableSize(  ) != ( sizeof( EvtTrText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "EvtTrText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table EvtEnText into database
	TextIndexAddTable( evttext_table_id, LNG_TR, EvtTrText, pgnum );

}


//=============================================================================
// Private function definitions
//=============================================================================
#endif 

