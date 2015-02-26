//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      evtentext.c
//
// File Description:
//      English event text table
//
//=============================================================================
#ifdef MAKE_ENGLISH

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
//!===  ENGLISH EVENT MESSAGE TABLE                ===
//====================================================
const char *const EvtEnText[] = {
    // NOOPT must be first entry in this table -- do not remove
    "",                         // NOOPT            Event Text
    "SALE",                     // SALE             Event Text
    "REVIEW",                   // REVIEW           Event Text
    "SETTLE",                   // SETTLE           Event Text
    "MERCH FUNC",               // FUNCTION         Event Text
    "INITIALIZE",               // INITIALIZE       Event Text
    "CARD VER",                 // CRDVER           Event Text
    "DEBIT",                    // DEBIT            Event Text
    "",                         // RESERVED			Event Text// Restart
    "FIND",                     // FIND             Event Text
    "ADJUST",                   // ADJUST           Event Text
    "BALANCE",                  // BALINQ           Event Text
    "REPRINT",                  // REPRINT          Event Text
    "VOID",                     // VOIDTRAN         Event Text
                                // 
    "PREAUTHCANCEL",
    "CLOSEDPAUTHCANCEL",

    "REPORTS",                  // REPORTS          Event Text
    "TOTALS",                   // TOTALS           Event Text
    //"REFUND",                   // REFUND           Event Text
    "OFFLINE",                  // SALOFFL          Event Text
    "PRINT LIST",               // PRTLIST          Event Text
    "PRINT TILL",               // PRTTILL          Event Text
    "TEST",                     // TESTTRAN         Event Text
    "AUTHORIZE",                // AUTH             Event Text
    "ADJUST SAL",               // ADJSALE          Event Text
    "ADJUST REF",               // ADJREFUND        Event Text
    "TRAIN MODE",               // TRAINMODE        Event Text
    "SETTLE",                   // CLBATCH          Event Text
    "SETTLE",                   // CLBATCHA         Event Text
    "SETTLE",                   // CLBATCHE         Event Text
    "UPLOAD",                   // RUPLOAD          Event Text
    "RESPONSE",                 // LASTRSP          Event Text
    "OPEN TAB",                 // OPENTAB          Event Text
    "CLOSE TAB",                // CLOSETAB         Event Text
    "         ",                // DEBITCONF        Event Text
    "    BATCH",                // CLTERM           Event Text
    "DEBIT REF",                // REFDEBIT         Event Text
    "SAL+CSH",                  // SALCASH          Event Text
    "CASH",                     // DBCASH           Event Text
    "",                         // CHANGESHIFT_FUN  Event Text
    "",                         // SETUP_FUN        Event Text
    "",                         // SETDT_FUN        Event Text
    //"",                         // CASHIER_FUN      Event Text
    "",                         // TESTTRAN_FUN     Event Text
    "",                         // TESTPRT_FUN      Event Text
    "",                         // TESTCRDR_FUN     Event Text
    "",                         // RESET_FUN        Event Text
    "",                         // PROGLD_FUN       Event Text
    "",                         // TERMID_FUN       Event Text
    "",                         // INITPHONE_FUN    Event Text
    "",                         // INITNMS_FUN      Event Text
    "",                         // DIALTYPE_FUN     Event Text
    "",                         // PABX_FUN         Event Text
    "",                         // TERMALL_FUN      Event Text
    "",                         // TERMSETUP_FUN    Event Text
    "",                         // PRTENABLE_FUN    Event Text
    "",                         // RECEIPTTEXT_FUN  Event Text
    "",                         // PAPERFEED_FUN    Event Text
    "",                         // PSWDCHANGE_FUN   Event Text
    "",                         // PSWDONOFF_FUN    Event Text
    "",                         // COMM_FUN         Event Text
    "",                         // SETCONTRAST_FUN  Event Text
    "",                         // DIALOPTS_FUN     Event Text
    "",                         // TABPROC_FUN      Event Text
    "",                         // CLEAR_AQ_FUN     Event Text
    "",                         // PRTH_FUN         Event Text
    "",                         // INITR_FUN        Event Text
    "",                         // TXRX_FUN         Event Text
    "",                         // TESTDISP_FUN     Event Text
    "",                         // TESTKYBD_FUN     Event Text
    "",                         // PINPADVER_FUN    Event Text
    "",                         // TERMVER_FUN      Event Text
    "",                         // TESTPINPAD_FUN   Event Text
    "LOGON",                    // LOGON            Event Text
    "ICEPAC",                   // ICEPAK_DOWNLOAD  Event Text
    "         ",                // SIGBATCH         Event Text
    "         ",                // SIGBATCHE        Event Text
    "         ",                // SIGUPL           Event Text
    "BROWSER",                  // BROWSER_APP      Event Text
    "",                         // CLEAR_REV_FUN    Event Text
    "",                         // CLEAR_ALL_FUN    Event Text
    "MAIL ORDER",               // MAIL_ORDER       Event Text
    "",                         // TIPS_FUN         Event Text
    "",                         // SETTLE_STATUS_FUN Event Text
    "",                         // SET_LANGUAGE     Event Text
    "",                         // SET_IDLEBUTTON_FUN   Event Text
    "",                         // Print Performance Report
    "",                         // Print Config. Report
    "",                         // TESTCHKREADER_FUN Event Text
    "",                         // CLEAR_HFS_FUN    Event Text

#ifdef MAKE_EMV
    "SALE",                     // EMV_SALE
    "", // EMV_Installment
    "CASH",                     // EMV_CASH
    "SALE+CASH",                // EMV_SALCASH
    "REFUND",                   // EMV_REFUND
    "FALLBACK",                 // EMV_FALLBACK
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
/*

    "",                         // EXIT_FUN
    "",                         // SET_TMCOMPARAM
    "", //    SET_TM_PHONENUMBER,
    "", //SET_TM_CONNECTTIME,
    "", //SET_TM_DIALATTEMPT,
    "", //SET_TM_MODEMMODE,
    "", //SET_TM_PPPMODE,
    "", //SET_TM_IPDDRESS,
    "", //SET_TM_PPPENABLE,
    "", //SET_TM_COMTYPE,
    "",    //     SET_TM_IPPORT_NUMBER,
    "", //SET_TM_IPNII,
    "", //SET_TM_IPHEXLEN,
    "", //SET_TM_IPPOSTPDU,
    "", //SET_TM_NII,
    "",// SET_TM_POSTPDU
*/
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
//!      Add English Event table for DML support.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void EvtTextEn_Init( void )
{
    short pgnum;

    int v1 = EventTableSize(  );
    int v2 = ( sizeof( EvtEnText ) / sizeof( char * ) );
    // Verify the number of messages match the number of events
    if ( EventTableSize(  ) != ( sizeof( EvtEnText ) / sizeof( char * ) ) )
    {
        strcpy( Dspbuf, "EvtEnText" );
        strcat( Dspbuf, MsgSizeMismatch );

        // Yes; notify the user of this error condition
        SystemErrMsg(  );
    }

    // Get our page number
    pgnum = SDK_ThisPage(  );

    // Put text table EvtEnText into database
    TextIndexAddTable( evttext_table_id, LNG_EN, EvtEnText, pgnum );

}


//=============================================================================
// Private function definitions
//=============================================================================


#endif // MAKE_ENGLISH
