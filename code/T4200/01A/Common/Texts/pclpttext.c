
#ifdef MAKE_PORTUGUESE
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002, 2004
//=============================================================================
//
// Module overview:             pclpttext.c
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
static const char *const PclPtText1[] = {
	"PT",											// Portugues
	"LOJISTA #:",									// MERCHANT-  5
	"SRV :",										// SERVER -   7
	"CX :",											// CASHIER -  7
	"GAVETA :",										// TILL -     7
	"CANCELAR",										// VOID_S -   9
	"COMIDA",										// FOOD -     10
	"DINHEIRO",										// CASH -     10
	"BENEFICIO",									// BENEFIT -  10
	"CANCELAR",										// VOID   -   10
	"EXP.:",										// EXPIRE -   10
	"LOTE: ",										// BATCH -    11
	"FATURA:",										// INVOICE -  11
	"DATA: ",										// DATE -     12
	"HORA: ",										// TIME -     12
	"RRN: ",										// RRN: -     13
	"AUTO NO:",										// AUTH NO -  13
	"FCS ID: ",										// FCS -      13
	"AVS: ",										// AVS -      13
	"SALDO DISPONIVEL",								// AVAIL_BAL -15,18,19
	"HOJE",											// TODAY -    15
	"BASE",											// BASE -     15
	"TAX",											// TAX -      15
	"BAR",											// BAR -      15
	"ACRESCIMO",									// SURCHARGE - 15
	"GOR",											// TIP -      15
	"TOTAL",										// TOTAL -    15
	"GORJETO SUGERIDA",								// TIP GUIDE - 15
	"VL VENDA",										// SALE_AMT - 15
	"TROCO",										// CASH_BACK -15
	"ENTRADA",										// FEE
	"VALOR DEVIDO:",								// AMOUNT DUE
	"*** NAO PERMITE REEMBOLSO",					// NoRefundsAllow
	"* ENTRADA OFFLINE *",							// OfflineEntered
	"SALDO COMIDA =",								// FoodBal
	"SALDO DINHEIRO =",								// Cash Bal
	"*** DUPLICADO ***",							// DUPLICATE
	"ASSINATURA NAO REQUERIDA",						// SignatureNotReq
	" - AVS CANCELADA",								// AvsCanceled
	"**** MODO TREINAMENTO ****",					// TrainingMode
	"AUTORIZO A TRANSFERENCIA ELETRONICA",			// IAuthorizeElect
	"DE FUNDOS DE MINHA CONTA ",					// FundFromMyAccount
	"OR PAPER DRAFT, AND TO DEBIT MY ACCOUNT",		// OrPaperDraft	
	"NO VALOR DESTA TRANSACAO",						// ForTheAmount
	"CASO ESTE CHEQUE VOLTE",						// InTheEvent
	"SEM PAGAMENTO, ENTENDO COMO SENDO ",			// Unpaid
	".",											// Point
	"RETORNO",										// Return
	"TAXA IEM TALVEZ MUDOU PARA MINHA CONTA",		// IEM_FEE
	"AUTORIZO A TRANSFERENCIA DE FUNDOS",			// Transfer_funds
	" NO VALOR TOTAL",								// ForTAmount
	" EU CONCORDO EM PAGAR O VALOR TOTAL ACIMA",	// IAGREE
	"DE ACORDO COM O CONTRATO DO EMISSOR DO",		// ACCORDING
	"CARTAO E O CONTRATO DO LOJISTA SE O",			// MerchantAgreement
	"RECIBO DE CREDITO DA TRANSACAO FALHAR",		// TransactFailed
	" FONTE NORMAL",								// NormalFont
	"ABCDEFGHIJ",									// ABCDEFGHI
	"KLMNOPQRST",									// KLMNOPQRST
	"UVWXYZ",										// UVWXYZ
	"abcdefghi",									// abcdefghi
	"klmnopqrst",									// klmnopqrst
	"uvwxyz0123456789",								// vwxyz
	"!#$%$()*+-./://<=>?@",							// SetSimbols1
	"[\\]^_'{|}",									// SetSimbols2
	"NORMAL",										// Normal
	"NEGRITO",										// Double
	"TROCA FONTE",									// FontChange
	"FIM",											// THE END
	"RELATORIO DE AUDITORIA",						// AuditReport
	"TODAS BANDEIRAS",								// AllAcquirers
	"FAT#  TRANS",									// InvTrans
	"SERVIDOR",										// Server
	"CAIXA",										// Cashr
	"AUTO",											// AUTH
	"NUMERO CARTAO",								// CardNumber
	"TIPO CARTAO",									// CardType
	"TROCO",										// Cashbk
	"GOR TOTAL",									// TipTotal
	"DINH TOTAL",									// CashTotal
	"TRANS TOTAL",									// TransTotal
	"TRANSFERENCIA",								// Shift
	"*",											// Star
	"ABRIR TABS",									// OpenTabs
	"FAT# TRANS",									// InvTrans
	"TOTAIS TRANSACOES",							// TransTotals
	"PELO EMISSOR",									// By Issuer
	"CAIXA :",										// Cashier:
	"SERVIDOR :",									// Server:
	"GAVETA :",										// Till:
	"TOTAL GERAL",									// GranTotals
	"NADA A IMPRIMIR",								// NothingToPrint
	"NAO AJUSTADO",									// Unadj
	"AUDITORIA",									// Audit
	"TERMINAL",										// Terminal
	"RELATORIO BANCO DE DADOS",						// DatabaseReport
	"CONFIGURACAO EPROM TERMNAL",					// TermEpromConfig
	"CONFIGURACAO TERMINAL",						// termConfig
	"ID BANDEIRA :",								// AcqId
	"ID EMISSOR :",									// IssuerID
	"ID INTERVALO CARTAO :",						// CardRangeId
	"ID DESCRITOR",									// Id_Descriptor
	"ID VISA 2 :",									// Visa2Id
	"ID MOVIMENTO ADICIONAL",						// AddPromptId
	"DADO ORIGINAL (HEX BYTES) :",					// RawData
	"DADO RECEBIDO :",								// ReceivedData
	"DADO ENVIADO :",								// SendData
	"TOTAIS HOST :",								// HostTotals
	"DEBITO",										// Debit
	"TABELA BANDEIRA",								// AcqTable
	"LOJISTA #:",									// Merchant#
	"ID TERMINAL :",								// TerminalID
	"VENDAS",										// Sales
	"REEMBOLSOS",									// Refunds
	"TROCO",										// CashBk
	"VALOR",										// Amount
	"LOG",											// LOG
	"LIGADO",										// ON
	"RELATORIO",									// Report
	"STATUS FECHAMENTO",							// Settlement Status
	"NOTIFICACAO DE RECIBO",						// NotOfrRec
	"NUMERO DE RECIBOS: ",							// NumOfRec
	"UPLOAD DE RECIBO FALHOU",						// RecUpload
	"1) POR FAVOR FECHE MANUALMENTE",				// PleasSettl
	"2) SE UPLOAD FALHA NOVAMENTE CONTATAR",		// UploadFaiL
	"CENTRAL DE ATENDIMENTO",						// HelpDesk
	"PERFORMANCE REPORT",       					// PerfReportTitle
    "Approvals:  " ,            					// lblTotAprorv  
    "Denials  :  " ,          					    // lblTotDenials 
    "Manuals  :  " ,           					    // lblTotManual  
    "Swipe    :  " ,            					// lblTotSwipe    
    "Transactions  " ,         					    // lblTransCount 
	"Count        :  ",		    					// TransCount    
    "Redials      :  " ,        					// lblTotRedials 
    "Timeout      :  " ,      					    // lblTottimeout 
    "Retransmits  :  " ,        					// lblTotRetran  
    "Period: ",            	    					// Period        
	"  To  ",				  					    // To            
	"COMMUNICATION STATUS",     					// Comm          
	"CONFIGURATION REPORT",   						// lblConfig_Rpt 
	"ADJUST",										//	Adjust		  
	"OFF ",                    					    //  OFF              
};

static const char *const PclPtText2[] = {

	"PERCENT #1",									//	Percent1	   
	"PERCENT #2",									//	Percent2	   
	"PERCENT #3",									//	Percent3	   
	"ACQUIRER AVAILABLE",	       					//	lblAcqAvail		  
	" CONFIGURATION BY ACQUIRER ",   			    //	lblConfbyAcq
	"NII",					  	   				  	//	NII
	"CARD ACCEPT TERMINAL",		   					//	CardAcceptTerm
	"CARD ACCEPT MERCHANT",		 				    //	CardAcceptMerc
	"VISA I TERMINAL ID",		  				    //	VisaITermId
	"NOT A SETTLEMENT ACQ.",      				    //	NotASettlement
	"IGNORE HOST TIME",							    //	IgnoreHostTime
	"ISDN PROTOCOL",                                //	ISDNprotocol
	"ERC COMPRESSION",			  				    //	ErcCompression
	"CURRENT BATCH",			   					//	CurrentBatch
	"NEXT BATCH",				  				    //	NextBatch
	"MAX.SETTLE. ATTEMPS",  	   					//	MaxSettleAttemp
    "SIGN ON HOST INFORMATION",//	lblSignOnHostInform
	"TRANSACTION HOST INFORMATION",					//	lblTransHostInform
	"PRIMARY",					   					//	Primary
	"PHONE NUMBER",				   					//	PhoneNum
	"CONNECT TIME",				  				    //	ConnectTime
	"DIAL ATTEMPS",				   					//	DialAttemps
	"MODEM MODE",							   	    //	ModemMode
	"SECONDARY",				   					//	Secondary
	"SETTLEMENT HOST INFORMATION", 					//	lblSettleHostInf
	"ISSUER TABLE", 			   					//	lblIssuerTab - Issuer Table
	"ISSUER AVAILABLE",			   					//	lblIssAvail
	"CONFIGURATION BY ISSUER",	   					//	lblConfigByIss
	"CAPTURE TRANSACTION",		   					//	CaptureTrans
	"PRINT A RECEIPT",			  				    //	PrintReceipt
	"CAPTURE RECEIPT",			  				    //	CaptureReceipt
	"FLOOR LIMIT",				   					//	FloorLimit
	"REAUTH PERCENT",			  				    //	ReauthPercent
	"ACH CHECK TRANS.",			  				  	//	AchCheckTrans
	"USE 6 DIGIT AUTH",			   					//	Use6Digit
	"PIN ENTRY REQUIRED",		   					//	PinEntryReq
	"ALLOW CASH BACK",			   					//	AllowCashBack
	"ALLOW CASH ONLY",			   					//	AllowCashOnly
	"PROCCESING OPTIONS",   	   					//	lblProcOption
	"INVOICE NUMBER",			   					//	InvoiceNum
	"MANUAL PAN ENTRY",			   					//	ManualPan
	"MOD 10 CHECK ON PAN",		   					//	Mod10Check
	"IGNORE TRACK 1 ERROR", 	   					//	IgnoreTrack
	"EXPIRY DATE REQUIRED",		   					//	ExpiryDate
	"CHECK EXPIRY DATE",		   					//	CheckExpiry
	"OFFLINE ALLOWED",             					//	OfflineAllow
	"ADJUST ALLOWED",			   					//	AdjustAllow
	"VOICE REFERRAL",			   					//	VoiceReferral
	"PROMPT FOR TAX",			   					//	PromptForTax
	"AVS OPTIONS",				   					//	lblAvsOptions
	"AVS ENABLED ON SALE",  	   					//	AvsEnab
 	"PROMPT ADDRESS",			   					//	PromptAddress
	"PROMPT ZIP CODE",			   					//	PromptZipCode
	"BLOCKED TRANSACTIONS",  	  				    //	lblBlockTrans
	"CARD VERIFY",				   					//	CardVerif
	"BALANCE INQUIRE",			  				    //	BalInq
	"PIN REQUIRED",				                    //  lblPinRequired 
	"CARD RANGE TABLE",			   					//	lblCardRangTab
	"CARD RANGE AVAILABLE",		   					//	lblCardRangAvai
	"CONFIGURATION BY CARD RANGE", 					//	lblConfByCard
	"CARD RANGE",				   					//	CardRang
	"PAN RANGE LOW",    		   					//	PanRangeLow
	"PAN RANGE HIGH",			   					//	PanRangeHigh
	"PAN LENGTH",				  				    //	PanLength
	"ID",	 					       				//  lblID
	"NAME ", 	                  				   	//  lblName
	"CARD NUMBER",  								//	lblCrnum
	"SUPPORTED IN", 			   				   	//	SupportIn
	"I UNDERSTAND AND CONSENT THAT MY CHECK",		// IUndestand
	"MAY BE CONVERTED TO A BANK DRAFT TO",			// MayBeConverted	
	"DEBIT MY ACCOUNT IN THE EVENT THAT THE",		// DebitMy
	"ITEM IS RETURNED UNPAID FOR ANY REASON,",		// ItemIs
	"I WILL PAY A SERVICE CHARGE UP TO THE",		// IWillPay
	"MAXIMUM AMOUNT AS SET FORTH BY STATE",			// MaximumAmount
	"LAW WITH THE SERVICE CHARGE DEBITED",			// LawWith
	"FROM MY ACCOUNT EITHER BY AN ELECTRONIC",		// FromMy
	"FUNDS TRANSFER (EFT), OR IN THE",				// FundsTransfer
	"MANNER SET FORTH ABOVE",						// MannerSer


};

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add Portuguese PCL messages table for DML support
//!
//! \param
//!		
//! \return
//!     None
//!
//! \note
//!
void PclTextPt_Init( void )
{
	short pgnum;

	// Verify the number of messages match
	TableItemCount = LAST_PCLTEXTDEF_ID;
	if( TableItemCount != ( sizeof( PclPtText1 ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "PclPtText1" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg( );
	}

	// Verify the number of messages match
	TableItemCount = LAST_PCLTEXTEXT_ID;
	if( TableItemCount != ( sizeof( PclPtText2 ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "PclPtText2" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg( );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put PCL text table PclPtText into database
	TextIndexAddTable( pcltext_tableDef_id, LNG_PT, PclPtText1, pgnum );
	TextIndexAddTable( pcltext_tableExt_id, LNG_PT, PclPtText2, pgnum );
}


//=============================================================================
// Private function definitions
//=============================================================================

#endif // MAKE_PORTUGUESE


