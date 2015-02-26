
#ifdef MAKE_SPANISH
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             pclestext.c
//      PCL Spanish Text Table.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkmem.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "pcltextinit.h"
#include "pcltext.h"
#include "syserr.h"

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


//=============================================================================
// Private data definitions
//=============================================================================
//! Because the text field processing is limited to up to 255 records 
//! an extended table capability has been included. Now PCL is supporting 
//! 2 tables - "PclEstext1" referred as "TX_Default" and "PclEstext2" 
//! referred as "TX_Extended". 

static const char *const PclEsText1[] = {
	"es",										// English
	"COMERCIO #: ",								// MERCHANT-  5
	"MES : ",									// SERVER -   7
	"CAJ : ",									// CASHIER -  7
	" TURNO : ",								// TILL -     7
	"CANC ",									// VOID_S -   9
	"ALIM",										// FOOD -     10
	"EFEC",										// CASH -     10
	" BENEFIC",									// BENEFIT -  10
	"CANC",										// VOID   -   10
	"EXP.: ",									// EXPIRE -   10
	"LOTE: ",									// BATCH -    11
	"CARGO: ",									// INVOICE -  11
	"FECHA: ",									// DATE -     12
	"HORA: ",									// TIME -     12
	"RRN: ",									// RRN: -     13
	"NRO AUTOR: ",								// AUTH NO -  13
	"ID-FCS : ",								// FCS -      13
	"AVS: ",									// AVS -      13
	"BAL. DISPL.",								// AVAIL_BAL -15,18,19
	"HOY ",										// TODAY -    15
	"BASE",										// BASE -     15
	"IMPUESTO",									// TAX -      15
	"BAR",										// BAR -      15
	"SOBRECARGO",								// SURCHARGE - 15
	"PROP",										// TIP -      15
	"TOTAL",									// TOTAL -    15
	"GUIA PROPINA",								// TIP GUIDE - 15
	"MONTO VENTA",								// SALE_AMT - 15
	"EFECTIVO",									// CASH_BACK -15
	"CARGO X SERVICIO",							// FEE
	"CARGO ADIC.",								// SURCHARGE
	"** DEVOL NO PERMITIDA",					// NoRefundsAllow
	"* FUERA DE LINEA *",						// OfflineEntered
	"BAL ALIMENTOS =",							// FoodBal
	"BAL. EFECTIVO=",							// Cash Bal
	"*** DUPLICADO ***",						// DUPLICATE
	"NO REQUIERE FIRMA",						// SignatureNotReq
	" - AVS CANCELADO",							// AvsCanceled
	"**** MODO ENTRENAMIENTO ****",				// TrainingMode
	"AUTORIZO TRANSFERENCIA ELECTRONICAR",		// IAuthorizeElect
	"DE FONDOS DE MI CUENTA ",					// FundFromMyAccount
	"OR PAPER DRAFT, AND TO DEBIT MY ACCOUNT",	// OrPaperDraft	
	"POR EL TOTAL DE LA TRANSACCION",			// ForTheAmount
	"EN CASO DE ESTE CHEQUE SER DEVUELTO",		// InTheEvent
	"ENTIENDO QUE UN CARGO   ",					// Unpaid
	".",										// Point
	"DEVOL.",									// Return
	"POR SERVICIO SERA CARGADO A MI CUENTA",	// IEM_FEE
	"AUTORIZO TRANSFERENCIA DE FONDOS",			// Transfer_funds
	" POR EL TOTAL DE LA TRANSACCION",			// ForTAmount
	" PAGARE INCONDICIONALMENTE A LA",			// IAGREE
	"ORDEN Y BAJO LAS CONDICIONES", 			// ACCORDING 
	"ESTABLECIDAS POR EL EMISOR",				// MerchantAgreement
	"TRANSACCION FALLIDA",						// TransactFailed
	" LETRA NORMAL ",							// NormalFont
	"ABCDEFGHIJ",								// ABCDEFGHI
	"KLMNOPQRST",								// KLMNOPQRST
	"UVWXYZ",									// UVWXYZ
	"abcdefghi",								// abcdefghi
	"klmnopqrst",								// klmnopqrst
	"uvwxyz0123456789",							// vwxyz
	"!#$%$()*+-./://<=>?@",						// SetSimbols1
	"[\\]^_'{|}",								// SetSimbols2
	"NORMAL",									// Normal
	"DOBLE",									// Double
	"CAMBIO LETRA",								// FontChange
	"FINAL",									// THE END
	"REP. RESUMEN",								// AuditReport
	"TODOS LOS ACQUIRERS",						// AllAcquirers
	"FACT#  TRANS",								// InvTrans
	"MESERO ",									// Server
	"CAJA ",									// Cashr
	"AUTORIZ",									// AUTH
	"NUM. TARJ.",								// CardNumber
	"TIPO TARJ",								// CardType
	"EFECT.",									// Cashbk
	"TOT PROPI :",								// TipTotal
	"TOT EFECT.:",								// CashTotal
	"TOTAL TRANSAC.:",							// TransTotal
	"TURNO ",									// Shift
	"*",										// Star
	"TABS ABIERTO",								// OpenTabs
	"FACT# TRANS  ",							// InvTrans
	"TOTALS TRANSACCIONES",						// TransTotals
	"POR EMISOR",								// By Issuer
	"CAJERO : ",								// Cashier:
	"MESERO :  ",								// Server:
	"  CAJA : ",								// Till:
	"TOTALES GLOBALES",							// GranTotals
	"NADA PARA IMPRIMIR",						// NothingToPrint
	"DESAJ",									// Unadj
	"RESUM",									// Audit
	"TERMINAL",									// Terminal
	"REPORTE DATABASE ",						// DatabaseReport
	"CONFIG-EPROM TERMINAL",					// TermEpromConfig
	"CONFIGURATION-TERMINAL",					// termConfig
	"ID - ACQUIRER :",							// AcqId
	"ID - ISSUER  : ",							// IssuerID
	"ID RANGO TARJETA: ",						// CardRangeId
	"ID - DESCRIPTOR: ",						// Id_Descriptor
	"ID VISA 2 : ",								// Visa2Id
	"ID Prompt ADICIONAL",						// AddPromptId
	"RAW DATA (HEX BYTES) :",					// RawData
	"DATOS RECIBIDOS :",						// ReceivedData
	"DATOS ENVIADOS :",							// SendData
	"TOTAL POR HOST: ",							// HostTotals
	"DEBITO",									// Debit
	"TABLA DE ACQUIRER ",						// AcqTable
	"COMERCIO #:",								// Merchant#
	"TERMINAL ID. :",							// TerminalID
	"VENTA  ",									// Sales
	"DEVOLUC.",									// Refunds
	"EFECTIVO",									// CashBk
	"MONTO  ",									// Amount
	"HOTEL",									// LOG
	"ACTIVO",									// ON
	"REPORTE",									// Report
	"STATUS DE CIERRE",							// Settlement Status
	"NOTIFICACION DE RECIBO",					// NotOfrRec
	"NUMERO DE RECIBOS: ",						// NumOfRec
	"FALLA EN LA CARGA DE RECIBOS",				// RecUpload
	"1) FAVOR CIERRE MANUALMENTE",				// PleasSettl
	"2) SI LA CARGA FALLA DE NUEVO CONTACTE",	// UploadFaiL
	"SOPORTE TECNICO.",							// HelpDesk
	"PERFORMANCE REPORT",      			  	    // PerfReportTitle
    "Approvals:  " ,            				// lblTotAprorv  
    "Denials  :  " ,          				    // lblTotDenials 
    "Manuals  :  " ,           				    // lblTotManual  
    "Swipe    :  " ,           				    // lblTotSwipe    
    "Transactions  " ,          				// lblTransCount 
	"Count        :  ",		   				    // TransCount    
    "Redials      :  " ,       					// lblTotRedials 
    "Timeout      :  " ,        				// lblTottimeout 
    "Retransmits  :  " ,       					// lblTotRetran  
    "Period: ",            	    				// Period        
	"  To  ",				    				// To            
	"COMMUNICATION STATUS",     				// Comm          
	"REPORTE CONFIGURACION",   					// lblConfig_Rpt 
	"DIALING OPTIONS",							// lblDialOption 
	"INACTIVO",                 				//  OFF               
    "CARD HOLDER NAME",
    "STAN:",
    "SIGN:",
    "", //Kart i'tibari
    "", // Parsian
    " ", // Space
};


static const char *const PclEsText2[] = {

	"PORCIENTO #1",								//	Percent1	
	"PORCIENTO #2",								//	Percent2	
	"PORCIENTO #3",								//	Percent3
	"ACQUIRER AVAILABLE",	      			    //	lblAcqAvail		  
	" CONFIGURATION BY ACQUIRER ",	 			//	lblConfbyAcq
	"NII",					  	   				//	NII
	"CARD ACCEPT TERMINAL",		   				//	CardAcceptTerm
	"CARD ACCEPT MERCHANT",		   				//	CardAcceptMerc
	"VISA I TERMINAL ID",		   				//	VisaITermId
	"NOT A SETTLEMENT ACQ.",       				//	NotASettlement
	"IGNORE HOST TIME",			   				//	IgnoreHostTime
	"ISDN PROTOCOL",                            //	ISDNprotocol
	"ERC COMPRESSION",			   				//	ErcCompression
	"CURRENT BATCH",			   				//	CurrentBatch
	"NEXT BATCH",				   				//	NextBatch
	"MAX.SETTLE. ATTEMPS",  	   				//	MaxSettleAttemp
	"TRANSACTION HOST INFORMATION",				//	lblTransHostInform
	"PRIMARY",					   				//	Primary
	"PHONE NUMBER",				   				//	PhoneNum
	"CONNECT TIME",				   				//	ConnectTime
	"DIAL ATTEMPS",				   				//	DialAttemps
	"MODEM MODE",				   				//	ModemMode
	"SECONDARY",				   				//	Secondary
	"SETTLEMENT HOST INFORMATION", 				//	lblSettleHostInf
	"ISSUER TABLE", 			   				//	lblIssuerTab - Issuer Table
	"ISSUER AVAILABLE",			   				//	lblIssAvail
	"CONFIGURATION BY ISSUER",	   				//	lblConfigByIss
	"CAPTURE TRANSACTION",		   				//	CaptureTrans
	"PRINT A RECEIPT",			   				//	PrintReceipt
	"CAPTURE RECEIPT",			   				//	CaptureReceipt
	"FLOOR LIMIT",				   				//	FloorLimit
	"REAUTH PERCENT",			   				//	ReauthPercent
	"ACH CHECK TRANS.",			   				//	AchCheckTrans
	"USE 6 DIGIT AUTH",			   				//	Use6Digit
	"PIN ENTRY REQUIRED",		  			    //	PinEntryReq
	"ALLOW CASH BACK",			   				//	AllowCashBack
	"ALLOW CASH ONLY",			   				//	AllowCashOnly
	"PROCCESING OPTIONS",   	  			    //	lblProcOption
	"INVOICE NUMBER",			   				//	InvoiceNum
	"MANUAL PAN ENTRY",			   				//	ManualPan
	"MOD 10 CHECK ON PAN",		   				//	Mod10Check
	"IGNORE TRACK 1 ERROR", 	   				//	IgnoreTrack
	"EXPIRY DATE REQUIRED",		   				//	ExpiryDate
	"CHECK EXPIRY DATE",		   				//	CheckExpiry
	"OFFLINE ALLOWED",             				//	OfflineAllow
	"ADJUST ALLOWED",			   				//	AdjustAllow
	"VOICE REFERRAL",			   				//	VoiceReferral
	"PROMPT FOR TAX",			   				//	PromptForTax
	"AVS OPTIONS",				   				//	lblAvsOptions
	"AVS ENABLED ON SALE",  	   				//	AvsEnab
 	"PROMPT ADDRESS",			   				//	PromptAddress
	"PROMPT ZIP CODE",			  			    //	PromptZipCode
	"BLOCKED TRANSACTIONS",  	   			   	//	lblBlockTrans
	"CARD VERIFY",				   				//	CardVerif
	"BALANCE INQUIRE",			   				//	BalInq
	"PIN REQUIRED",			                    //  lblPinRequired
	"CARD RANGE TABLE",			  			    //	lblCardRangTab
	"CARD RANGE AVAILABLE",		   				//	lblCardRangAvai
	"CONFIGURATION BY CARD RANGE", 				//	lblConfByCard
	"CARD RANGE",				   				//	CardRang
	"PAN RANGE LOW",    		   				//	PanRangeLow
	"PAN RANGE HIGH",			  			    //	PanRangeHigh
	"PAN LENGTH",				   				//	PanLength
	"ID",	  					 			    //  lblID
	"NAME ",                      				//  lblName
	"CARD NUMBER",						   		//	lblCrnum
	"SUPPORTED IN", 			   				//	SupportIn
	"I UNDERSTAND AND CONSENT THAT MY CHECK",	// IUndestand
	"MAY BE CONVERTED TO A BANK DRAFT TO",		// MayBeConverted	
	"DEBIT MY ACCOUNT IN THE EVENT THAT THE",	// DebitMy
	"ITEM IS RETURNED UNPAID FOR ANY REASON,",	// ItemIs
	"I WILL PAY A SERVICE CHARGE UP TO THE",	// IWillPay
	"MAXIMUM AMOUNT AS SET FORTH BY STATE",		// MaximumAmount
	"LAW WITH THE SERVICE CHARGE DEBITED",		// LawWith
	"FROM MY ACCOUNT EITHER BY AN ELECTRONIC",	// FromMy
	"FUNDS TRANSFER (EFT), OR IN THE",			// FundsTransfer
	"MANNER SET FORTH ABOVE",					// MannerSer

};

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add Spanish PCL messages table for DML support
//!
//! \param
//!		
//! \return
//!     None
//!
//! \note
//!
void PclTextEs_Init( void )
{
	short pgnum;

	// Verify the number of messages match
	TableItemCount = LAST_PCLTEXTDEF_ID;
	if( TableItemCount != ( sizeof( PclEsText1 ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "PclEsText1" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg( );
	}

	// Verify the number of messages match
	TableItemCount = LAST_PCLTEXTEXT_ID;
	if( TableItemCount != ( sizeof( PclEsText2 ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "PclEsText2" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg( );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put PCL text table PclEsText into database
	TextIndexAddTable( pcltext_tableDef_id, LNG_ES, PclEsText1, pgnum );
	TextIndexAddTable( pcltext_tableExt_id, LNG_ES, PclEsText2, pgnum );
}

#endif // MAKE_SPANISH


//=============================================================================
// Private function definitions
//=============================================================================
