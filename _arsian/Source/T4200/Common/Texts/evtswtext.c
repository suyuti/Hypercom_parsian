//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      evtswtext.c
//
// File Description:
//      Swedish event text table
//
//=============================================================================

#ifdef MAKE_SWEDISH

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
//!===  SWEDISH EVENT MESSAGE TABLE                 ==
//====================================================
const char *const EvtSwText[] = {
	// NOOPT must be first entry in this table -- do not remove
	"",							// NOOPT
	"KÖP",						// PURCHASE
	"GRANSKNING",				// REVIEW
	"BETALA",					// SETTLE
	"HANDLAR FN",   			// FUNCTION
	"INITIERA",					// INITIALIZE
	"KORT VER",					// CRDVER
	"DEBIT",					// DEBIT
	"",							// RESERVED
	"HITTA",					// FIND
	"JUSTERA",					// ADJUST
	"SALDO",					// BALINQ
	"REPETERA",					// REPRINT
	"OGILTIG",					// VOIDTRAN
    "PREAUTHCANCEL",             //          Preauth Cancel
    "CLOSEDPAUTHCANCEL",         //          Closed Preauth Cancel
	"RAPPORTER",				// REPORTS
	"TOTALT",					// TOTALS
	//"ÅTERGÅ",					// RETURN
	"EJ ANSL",					// SALOFFL
	"SKRIV LIST",				// PRTLIST
	"SKRIV TILL",				// PRTTILL
	"TEST",						// TESTTRAN
	"GODKÄNNA",					// AUTH    
	"JUST FÖRS",				// ADJSALE 
	"JUST ÅTERB",				// ADJREFUND   
	"TRÄN MODE",				// TRAINMODE   
	"FASTSTÄLLA",				// CLBATCH     
	"FASTSTÄLLA",				// CLBATCHA    
	"FASTSTÄLLA",				// CLBATCHE    
	"LADDA UPP",				// RUPLOAD     
	"SVAR",						// LASTRSP     
	"ÖPPNA TAB",				// OPENTAB     
	"STÄNG TAB",				// CLOSETAB    
	"         ",				// DEBITCONF   
	"    OMGÅNG",				// CLTERM      
	"DEBIT REF",				// REFDEBIT    
	"FÖRS+KONT",				// SALCASH
	"KONTANT AV",				// DBCASH
	"",							// CHANGESHIFT_FUN
	"",							// SETUP_FUN      
	"",							// SETDT_FUN      
	"",							// CASHIER_FUN    
	"",							// TESTTRAN_FUN   
	"",							// TESTPRT_FUN    
	"",							// TESTCRDR_FUN   
	"",							// RESET_FUN      
	"",							// PROGLD_FUN     
	"",							// TERMID_FUN     
	"",							// INITPHONE_FUN  
	"",							// INITNMS_FUN    
	"",							// DIALTYPE_FUN   
	"",							// PABX_FUN       
	"",							// TERMALL_FUN    
	"",							// TERMSETUP_FUN  
	"",							// PRTENABLE_FUN   
	"",							// RECEIPTTEXT_FUN 
	"",							// PAPERFEED_FUN   
	"",							// PSWDCHANGE_FUN  
	"",							// PSWDONOFF_FUN   
	"",							// COMM_FUN        
	"",							// SETCONTRAST_FUN 
	"",							// DIALOPTS_FUN    
	"",							// TABPROC_FUN     
	"",							// CLEAR_AQ_FUN    
	"",							// PRTH_FUN        
	"",							// INITR_FUN       
	"",							// TXRX_FUN        
	"",							// TESTDISP_FUN    
	"",							// TESTKYBD_FUN    
	"",							// PINPADVER_FUN   
	"",							// TERMVER_FUN     
	"",							// TESTPINPAD_FUN  
	"LOGGA IN",					// LOGON           
	"ICEPAC",					// ICEPAK_DOWNLOAD
	"         ",				// SIGBATCH        
	"         ",				// SIGBATCHE       
	"         ",				// SIGUPL          
	"BROWSER",					// BROWSER_APP
	"",							// CLEAR_REV_FUN   
	"",							// CLEAR_ALL_FUN   
	"MAIL ORDER",				// MAIL_ORDER
	"",							// TIPS_FUN  
	"",							// SETTLE_STATUS_FUN 
	"",							// SET_LANGUAGE     
	"",							// SET_IDLEBUTTON_FUN
	"",							// Print Performance Report
	"",							// Print Config. Report
	"",							// TESTCHKREADER_FUN 
	"",							// CLEAR_HFS_FUN 

#ifdef MAKE_EMV
	 "KÖP",						// EMV_SALE	
    "EMV_INSTALLMENT",	
	 "KONTANT AV", 				// EMV_CASH	
	 "FÖRS+KONT",          		// EMV_SALCASH	
	 "ÅTERGÅ",					// EMV_RETURN	
	 "RETRÄTT",					// EMV_FALLBACK	
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

	"",							// NOT_SUPPORTED
};


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add Swedish Event table for DML support.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void EvtTextSw_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	if ( EventTableSize(  ) != ( sizeof( EvtSwText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "EvtSwText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table EvtEnText into database
	TextIndexAddTable( evttext_table_id, LNG_SV, EvtSwText, pgnum );

}


//=============================================================================
// Private function definitions
//=============================================================================


#endif // MAKE_SWEDISH
