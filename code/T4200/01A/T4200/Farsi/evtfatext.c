//=============================================================================
// Company:
//      Altius It
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      evtfatext.c
//
// File Description:
//      Farsi event text table
//
//=============================================================================
#ifdef MAKE_FARSI

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
//!===  FARSI EVENT MESSAGE TABLE                ===
//====================================================
const char *const EvtFaText[] = {
	// NOOPT must be first entry in this table -- do not remove
	// NOOPT must be first entry in this table -- do not remove
	"",							// NOOPT            Event Text
	"\xB4\xB9\xED\xB1\xD2",                                                                  // SALE             Event Text
	"\xB1\xED\xB1\xE6",                                                                     // REVIEW           Event Text
	"\xF1\xF3\xED\xB7\x9D",                                                                 // SETTLE           Event Text
	"MERCH FUNC",				// FUNCTION         Event Text //Note:  Must be re-translated shorter.
	"\xF2\xB2\x91\xAF\xEA\x91\x20\xEE\x91\xB1",                                          // INITIALIZE       Event Text
	"\x9C\xB1\x93\xDA\x20\xAF\xF4\xF4\x90\x9D",                                           // CRDVER           Event Text
	"\xF5\xEF\xAF\x95",                                                                      // DEBIT            Event Text
	"",							// RESERVED			Event Text
	"\xED\xA5\x9E\xB7\xA5",                                                                  // FIND             Event Text
	"\xAB\xF4\xAA\xBF\x9D",                                                                 // ADJUST           Event Text
	"\xEE\xAF\xEA\x93\xE6",                                                                // BALINQ           Event Text
	"\xEE\xB1\x93\x95\xED\xAF\x20\x98\x93\xA8",                                            // REPRINT          Event Text
	"\xF1\xF4\xD7\x95\x20",                                                                  // VOIDTRAN         Event Text
       "PREAUTHCANCEL",
    "CLOSEDPAUTHCANCEL",
	"\x9C\x93\xBA\xB1\x91\xB2\xDE",                                                        // REPORTS          Event Text
	"\xCC\xE7\xA5",                                                                         // TOTALS           Event Text
	"\x9F\xAD\x91\xAF\xB1\x99\xB2\x93\x95",                                                 // REFUND           Event Text
	"OFFLINE",					// SALOFFL          Event Text //Note:  Must be re-translated shorter.
	"\x9F\xB7\xF4\xE2\x20\x98\x93\xA8",                                                  // PRTLIST          Event Text
	"\x93\x9D\x20\x98\x93\xA8",                                                          // PRTTILL          Event Text
	"\x9F\xB7\x9D",                                                                          // TESTTRAN         Event Text
	"\xAF\xF4\xF4\x90\x9D",                                                               // AUTH             Event Text
	"",                                                                                  // ADJSALE          Event Text
	"",                                                                                  // ADJREFUND        Event Text
	"TRAIN MODE",				// TRAINMODE        Event Text //Note:  Must be re-translated shorter.
	"\xF1\xF3\xED\xB7\x9D",                                                                 // CLBATCH          Event Text
	"\xF1\xF3\xED\xB7\x9D",                                                                 // CLBATCHA         Event Text
	"\xF1\xF3\xED\xB7\x9D",                                                                 // CLBATCHE         Event Text
	"\xE1\x93\xD7\x9E\xEA\x91",                                                             // RUPLOAD          Event Text
	"\xAE\xB6\x93\x99",                                                                   // LASTRSP          Event Text
	"",                                                                                   // OPENTAB          Event Text
	"",                                                                                   // CLOSETAB         Event Text
	"         ",				// DEBITCONF        Event Text
	"    BATCH",				// CLTERM           Event Text
	"\xF5\xEF\xAF\x95\x20\xD4\xC6\xCA",                                                   // REFDEBIT         Event Text
	"",                                                                                    // SALCASH          Event Text
	"\xF2\xAF\xD7\xEA",                                                                      // DBCASH           Event Text
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
	"\xAF\xED\xB1\xED",                                                                      // LOGON            Event Text
	"",                                                                                     // ICEPAK_DOWNLOAD  Event Text
	"         ",				// SIGBATCH         Event Text
	"         ",				// SIGBATCHE        Event Text
	"         ",				// SIGUPL           Event Text
	"\xB1\xDE\xB1\xED\xB1\xE6",                                                            // BROWSER_APP      Event Text
	"",							// CLEAR_REV_FUN    Event Text
	"",							// CLEAR_ALL_FUN    Event Text
	"\xF5\x9E\xB7\x99\x20\xB4\xB9\xB1\x93\xD3\xB6",                                      // MAIL_ORDER       Event Text
	"",							// TIPS_FUN         Event Text
	"",							// SETTLE_STATUS_FUN Event Text
	"",							// SET_LANGUAGE     Event Text
	"",							// SET_IDLEBUTTON_FUN   Event Text
	"",							// Print Performance Report
	"",							// Print Config. Report
	"",							// TESTCHKREADER_FUN Event Text
	"",							// CLEAR_HFS_FUN    Event Text

#ifdef MAKE_EMV
	"\xB4\xB9\xED\xB1\xD2",                                                   // EMV_SALE
    "", // EMV_Installment
    "\xF2\xAF\xD7\xEA",                                                       // EMV_CASH
	"SALE+CASH",                // EMV_SALCASH
	"\x9F\xAD\x91\xAF\xB1\x99\xB2\x93\x95",                                   // EMV_REFUND
	"FALLBACK",                 // EMV_FALLBACK
#endif // MAKE_EMV

	"CHANGE APP",               // CHG_APP //Note:  Must be re-translated shorter.
    "",                         // TRACE_START
    "",                         // TRACE_STOP
    "",                         // TRACE_PRINT
    "",                         // TRACE_SEND2
    "",                         // TRACE_SEND1
    "",                         // DBINITPREF_FUN
    "",                         // DBNMSPREF_FUN
    "",                         // DBINITPHONE_FUN
    "",                         // DBNMSPHONE_FUN
    //"",                         // EXIT_FUN
    "INSTALLMENT",  //Installment
    "REFUND",                   // REFUND
    "INSTALLREFUND",    // Install Refund
    "PREAUTH", // Preauth
    "CLOSEDPREAUTH", // Closed Preauth
    "STATISTICS", 
    "DBFORMATTED",
    "IPCONFIG",
    "NMSIP_FUN",
    "NMSPORT_FUN",
    "INITIP_FUN",
    "INITPORT_FUN",
    "LOCALIP_FUN",
    "LOCALSUBNET_FUN",
    "GATEWAYIP_FUN",
#ifdef MAKE_KIB
    "LOY. INQ.",                                        // LOYALTY_INQ
    "DISC. SALE",                                        // DISCOUNT_SALE
    "DISC. INST",                                       // DISCOUNT_INST
#endif    
    "",                         // NOT_SUPPORTED    Event Text
};


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add Farsi Event table for DML support.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void EvtTextFa_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	if ( EventTableSize(  ) != ( sizeof( EvtFaText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "EvtFaText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table EvtEnText into database
	TextIndexAddTable( evttext_table_id, LNG_FA, EvtFaText, pgnum );

}


//=============================================================================
// Private function definitions
//=============================================================================


#endif // MAKE_FARSI
