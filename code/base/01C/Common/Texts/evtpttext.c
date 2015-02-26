//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      evtpttext.c
//
// File Description:
//      Portuguese event text table
//
//=============================================================================

#ifdef MAKE_PORTUGUESE

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
//!===  PORTUGUESE EVENT MESSAGE TABLE              ==
//====================================================
const char *const EvtPtText[] = {
	// NOOPT must be first entry in this table -- do not remove
	"          ",				// NOOPT             Event Text
	"VENDA     ",				// SALE              Event Text
	"REVISAO   ",				// REVIEW            Event Text
	"FECHAMENTO",				// SETTLE            Event Text
	"FUNCAO    ",				// FUNCTION          Event Text
	"INICIALIZE",				// INITIALIZE        Event Text
	"VERIF CART",				// CRDVER            Event Text
	"DEBITO    ",				// DEBIT             Event Text
	"",							// RESERVED			 Event Text
	"BUSCA     ",				// FIND              Event Text
	"AJUSTE    ",				// ADJUST            Event Text
	"SALDO     ",				// BALINQ            Event Text
	"REIMPRIMIR",				// REPRINT           Event Text
	"CANCELACAO",				// VOIDTRAN          Event Text
	"RELATORIOS",				// REPORTS           Event Text
	"TOTAIS    ",				// TOTALS            Event Text
	"ESTORNO   ",				// REFUND            Event Text
	"F. LINEA  ",				// SALOFFL           Event Text
	"IMPR.LISTA",				// PRTLIST           Event Text
	"IMP GAVETA",				// PRTTILL           Event Text
	"TESTE     ",				// TESTTRAN          Event Text
	"AUTORIZAR ",				// AUTH              Event Text
	"AJUSTE VD ",				// ADJSALE           Event Text
	"AJUSTE EST",				// ADJREFUND         Event Text
	"MODO ENTR.",				// TRAINMODE         Event Text
	"FECHAMENTO",				// CLBATCH           Event Text
	"FECHAMENTO",				// CLBATCHA          Event Text
	"FECHAMENTO",				// CLBATCHE          Event Text
	"ENV. LOTE ",				// RUPLOAD           Event Text
	"RESPOSTA  ",				// LASTRSP           Event Text
	"ABRIR TAB ",				// OPENTAB           Event Text
	"FECHAR TAB",				// CLOSETAB          Event Text
	"          ",				// DEBITCONF         Event Text
	"      LOTE",				// CLTERM            Event Text
	"EST DEBITO",				// REFDEBIT          Event Text
	"VENDA+DINH",				// SALCASH           Event Text
	"DINHEIRO  ",				// DBCASH            Event Text
	"          ",				// CHANGESHIFT_FUN   Event Text
	"          ",				// SETUP_FUN         Event Text
	"          ",				// SETDT_FUN         Event Text
	"          ",				// CASHIER_FUN       Event Text
	"          ",				// TESTTRAN_FUN      Event Text
	"          ",				// TESTPRT_FUN       Event Text
	"          ",				// TESTCRDR_FUN      Event Text
	"          ",				// RESET_FUN         Event Text
	"          ",				// PROGLD_FUN        Event Text
	"          ",				// TERMID_FUN        Event Text
	"          ",				// INITPHONE_FUN     Event Text
	"          ",				// INITNMS_FUN       Event Text
	"          ",				// DIALTYPE_FUN      Event Text
	"          ",				// PABX_FUN          Event Text
	"          ",				// TERMALL_FUN       Event Text
	"          ",				// TERMSETUP_FUN     Event Text
	"          ",				// PRTENABLE_FUN     Event Text
	"          ",				// RECEIPTTEXT_FUN   Event Text
	"          ",				// PAPERFEED_FUN     Event Text
	"          ",				// PSWDCHANGE_FUN    Event Text
	"          ",				// PSWDONOFF_FUN     Event Text
	"          ",				// COMM_FUN          Event Text
	"          ",				// SETCONTRAST_FUN   Event Text
	"          ",				// DIALOPTS_FUN      Event Text
	"          ",				// TABPROC_FUN       Event Text
	"          ",				// CLEAR_AQ_FUN      Event Text
	"          ",				// PRTH_FUN          Event Text
	"          ",				// INITR_FUN         Event Text
	"          ",				// TXRX_FUN          Event Text
	"          ",				// TESTDISP_FUN      Event Text
	"          ",				// TESTKYBD_FUN      Event Text
	"          ",				// PINPADVER_FUN     Event Text
	"          ",				// TERMVER_FUN       Event Text
	"          ",				// TESTPINPAD_FUN    Event Text
	"LOGON     ",				// LOGON             Event Text
	"ICEPAC    ",				// ICEPAK_DOWNLOAD   Event Text
	"          ",				// SIGBATCH          Event Text
	"          ",				// SIGBATCHE         Event Text
	"          ",				// SIGUPL            Event Text
	"BROWSER   ",				// BROWSER_APP       Event Text
	"          ",				// CLEAR_REV_FUN     Event Text
	"          ",				// CLEAR_ALL_FUN     Event Text
	"VALE-POST.",				// MAIL_ORDER       Event Text
	"          ",				// TIPS_FUN         Event Text
	"          ",				// SETTLE_STATUS_FUN Event Text
	"          ",				// SET_LANGUAGE     Event Text
	"",							// SET_IDLEBUTTON_FUN   Event Text
	"",							// Print Performance Report
	"",							// Print Config. Report
	"",							// TESTCHKREADER_FUN Event Text
	"",							// CLEAR_HFS_FUN    Event Text

#ifdef MAKE_EMV
	"SALE",                     // EMV_SALE
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
                                // 
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
//!      Add Portuguese Event messages tables for DML support.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void EvtTextPt_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	if ( EventTableSize(  ) != ( sizeof( EvtPtText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "EvtPtText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Puts text table EvtPtText into database
	TextIndexAddTable( evttext_table_id, LNG_PT, EvtPtText, pgnum );
}


//=============================================================================
// Private function definitions
//=============================================================================


#endif // MAKE_PORTUGUESE
