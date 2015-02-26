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
	"K�P",						// PURCHASE
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
	"RAPPORTER",				// REPORTS
	"TOTALT",					// TOTALS
	"�TERG�",					// RETURN
	"EJ ANSL",					// SALOFFL
	"SKRIV LIST",				// PRTLIST
	"SKRIV TILL",				// PRTTILL
	"TEST",						// TESTTRAN
	"GODK�NNA",					// AUTH    
	"JUST F�RS",				// ADJSALE 
	"JUST �TERB",				// ADJREFUND   
	"TR�N MODE",				// TRAINMODE   
	"FASTST�LLA",				// CLBATCH     
	"FASTST�LLA",				// CLBATCHA    
	"FASTST�LLA",				// CLBATCHE    
	"LADDA UPP",				// RUPLOAD     
	"SVAR",						// LASTRSP     
	"�PPNA TAB",				// OPENTAB     
	"ST�NG TAB",				// CLOSETAB    
	"         ",				// DEBITCONF   
	"    OMG�NG",				// CLTERM      
	"DEBIT REF",				// REFDEBIT    
	"F�RS+KONT",				// SALCASH
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
	 "K�P",						// EMV_SALE	
	 "KONTANT AV", 				// EMV_CASH	
	 "F�RS+KONT",          		// EMV_SALCASH	
	 "�TERG�",					// EMV_RETURN	
	 "RETR�TT",					// EMV_FALLBACK	
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
