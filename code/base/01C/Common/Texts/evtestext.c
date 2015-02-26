//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      evtestext.c
//
// File Description:
//      Spanish event text table
//
//=============================================================================

#ifdef MAKE_SPANISH
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
//!===  SPANISH EVENT MESSAGE TABLE                 ==
//====================================================
const char *const EvtEsText[] = {
	// NOOPT must be first entry in this table -- do not remove
	"",							// NOOPT            Event Text
	"VENTA",					// SALE             Event Text
	"REVISION",					// REVIEW           Event Text
	"CORTE",					// SETTLE           Event Text
	" FUNCION ",				// FUNCTION         Event Text
	"INICIALICE",				// INITIALIZE       Event Text
	"VERIF TARJ",				// CRDVER           Event Text
	"DEBITO",					// DEBIT            Event Text
	"",							// RESERVED			Event Text
	"BUSQUEDA",					// FIND             Event Text
	"AJUSTE",					// ADJUST           Event Text
	"SALDO",					// BALINQ           Event Text
	"REIMPRIMIR",				// REPRINT          Event Text
	"CANCELACION",				// VOIDTRAN         Event Text
	"REPORTES",					// REPORTS          Event Text
	"TOTALES",					// TOTALS           Event Text
	"DEVOLUCION",				// REFUND           Event Text
	"F. LINEA",					// SALOFFL          Event Text
	"IMPR. LISTA",				// PRTLIST          Event Text
	"IMPR. TILL",				// PRTTILL          Event Text
	"PRUEBA",					// TESTTRAN         Event Text
	"AUTORIZAR",				// AUTH             Event Text
	"AJUSTE VENT",				// ADJSALE          Event Text
	"AJUSTE DEV",				// ADJREFUND        Event Text
	"ENTRENARSE",				// TRAINMODE        Event Text
	"CORTE",					// CLBATCH          Event Text
	"CORTE",					// CLBATCHA         Event Text
	"CORTE",					// CLBATCHE         Event Text
	"ENV. LOTE",				// RUPLOAD          Event Text
	"RESPUESTA",				// LASTRSP          Event Text
	"ABRIR TAB",				// OPENTAB          Event Text
	"CERRAR TAB",				// CLOSETAB         Event Text
	"         ",				// DEBITCONF        Event Text
	"    LOTE",					// CLTERM           Event Text
	"DEV DEBITO",				// REFDEBIT         Event Text
	"VENTA+EFECT",				// SALCASH          Event Text
	"EFECT",					// DBCASH           Event Text
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
	"POR CORREO",				// MAIL_ORDER       Event Text
	"",							// TIPS_FUN         Event Text
	"",							// SETTLE_STATUS_FUN Event Text
	"",							// SET_LANGUAGE     Event Text
	"",							// SET_IDLEBUTTON_FUN   Event Text
	"",							// Print Performance Report
	"",							// Print Config. Report
	"",							// TESTCHKREADER_FUN Event Text
	"",							// CLEAR_HFS_FUN    Event Text

#ifdef MAKE_EMV
	"VENTA",                    // EMV_SALE
	"EFECT",                    // EMV_CASH
	"VENTA+EFECT",              // EMV_SALCASH
	"DEVOLUCION",               // EMV_REFUND
	"FALLBACK",                 // EMV_FALLBACK
#endif // MAKE_EMV

	"CHANGE APP",               // CHG_APP - change application
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
//!      Add Spanish Event messages tables for DML support.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void EvtTextEs_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	if ( EventTableSize(  ) != ( sizeof( EvtEsText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "EvtEsText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table EvtEsText into database
	TextIndexAddTable( evttext_table_id, LNG_ES, EvtEsText, pgnum );
}

//=============================================================================
// Private function definitions
//=============================================================================



#endif //  MAKE_SPANISH
