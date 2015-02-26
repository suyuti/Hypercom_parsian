
#ifdef MAKE_GERMAN
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             pclentext.c
//      PCL German Text Table.
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
 
//! FIRST Text table - TX_DEFAULT
static const char *const PclGrText1[] = {
	"gr",										// German
	"HAEDLER #: ",								// MERCHANT-  5
	"KEL : ",									// SERVER -   7
	"KAS : ",									// CASHIER -  7
	"KASSE : ",									// TILL -     7
	"STORNO ",									// VOID_S -   9
	"FOOD",										// FOOD -     10
	"BAR",										// CASH -     10
	"VORTEIL",									// BENEFIT -  10
	"STORNO",										// VOID   -   10
	"VERF.: ",									// EXPIRE -   10
	"BATCH: ",									// BATCH -    11
	"BELEG: ",								// INVOICE -  11
	"DAT.: ",									// DATE -     12
	"ZEIT: ",									// TIME -     12
	"RRN: ",									// RRN: -     13
	"AUTH NR: ",								// AUTH NO -  13
	"FCS ID: ",									// FCS -      13
	"AVS: ",									// AVS -      13
	"VERFg. BETRAG",							// AVAIL_BAL -15,18,19
	"HEUTE",									// TODAY -    15
	"BETRAG",										// BASE -     15
	"MWST",										// TAX -      15
	"BAR",										// BAR -      15
	"AUFSCHLAG",								// SURCHARGE - 15
	"TIP",										// TIP -      15
	"TOTAL",									// TOTAL -    15
	"TIP GUIDE",								// TIP GUIDE - 15
	"BETRAG",									// SALE_AMT - 15
	"RUECKGELD",								// CASH_BACK -15
	"GEBUEHR",									// FEE
	"AMOUNT DUE:",								// AMOUNT DUE
	"*** KEIN STOrno MOEGLICH",					// NoRefundsAllow
	"* OFFLINE EINGABE *",						// OfflineEntered
	"TOTAL ART.=",								// FoodBal
	"TOTAL BAR =",								// Cash Bal
	"*** K O P I E ***",						// DUPLICATE
	"UNTERSCHRIFT nicht noetig",				// SignatureNotReq
	" - AVS CANCELED",							// AvsCanceled
	"**** TRAINING MODE ****",					// TrainingMode
	"Ich ermaechtige hiermit das umseitige ",	// IAuthorizeElect
	"genannte Unternehmen umseitig ausge-",     // FundFromMyAccount
	"wiesenen Rechnungsbetrag von meinem",      // OrPaperDraft	
	"Konto einzuziehen",                        // ForTheAmount
	"Im Falle eines nicht gedeckten Kontos",	// InTheEvent
	"können weitere Gebuehren anfallen, welche ",// Unpaid
	".",										// Point
	"ZURÜCK",									// Return
	"diesem Konto weiterbelastet werden",       // IEM_FEE
	"I AUTHORIZE THE TRANSFER OF FUNDS",		// Transfer_funds
	" FOR THE AMOUNT OF THIS TRANSACTION",		// ForTAmount
	" Ich werde obigen Betrag bezahlen",		// IAGREE
	"Gemaess dem Kredit Karten Vereinbarungen", // ACCORDING 
	"",                                         // MerchantAgreement
	"TRANSACTION FAILED",						// TransactFailed
	" NORMAL  FONT ",							// NormalFont
	"ABCDEFGHIJ",								// ABCDEFGHI
	"KLMNOPQRST",								// KLMNOPQRST
	"UVWXYZ",									// UVWXYZ
	"abcdefghi",								// abcdefghi
	"klmnopqrst",								// klmnopqrst
	"uvwxyz0123456789",							// vwxyz0123
	"!#$%$()*+-./://<=>?@",						// SetSimbols1
	"[\\]^_'{|}",								// SetSimbols2
	"NORMAL",									// Normal
	"DOUBLE",									// Double
	"FONT CHANGE",								// FontChange
	"THE END",									// THE END
	"JOURNAL BERICHT",								// AuditReport
	"ALLE ACQUIRER",							// AllAcquirers
	"BELEG-NR.",								// InvTrans
	"KELLNER ",									// Server
	"KASSIERER ",									// Cashr
	"AUTH",										// AUTH
	"KARTEN NUMMER",								// CardNumber
	"KARTEN TYP",								// CardType
	"RUECKGELD",									// Cashbk
	"TIP TOTAL",								// TipTotal
	"BAR TOTAL",								// CashTotal
	"TRANS TOTAL",								// TransTotal
	"SCHICHT ",									// Shift
	"*",										// Star
	"OPEN TABS",								// OpenTabs
	"BELEG-NR.",								// InvTrans
	"TRANSAKTION TOTALS",						// TransTotals
	"KARTENTYP",								// By Issuer
	"KASSIERTER : ",								// Cashier:
	"  KELLNER  :  ",								// PCLServer:
	"   KASSE : ",								// Till:
	"ALLE TOTALE",								// GranTotals
	"Keine DATEN zum DRUCKEN",							// NothingToPrint
	"AKTUELL",									// Unadj
	"JOURNAL",									// Audit
	"TERMINAL",									// Terminal
	"DATABASE BERICHT ",							// DatabaseReport
	"TERMNAL EPROM CONFIG",						// TermEpromConfig
	"TERMINAL CONFIGURATION",					// termConfig
	"ACQUIRER ID :",							// AcqId
	"ISSUER ID : ",								// IssuerID
	"CARD RANGE ID: ",							// CardRangeId
	"ID DESCRIPTOR",							// Id_Descriptor
	"VISA 2 ID : ",								// Visa2Id
	"ADDITIONAL PROMPT ID",						// AddPromptId
	"RAW DATA (HEX BYTES) :",					// RawData
	"RECEIVED DATA :",							// ReceivedData
	"SEND DATA :",								// SendData
	"HOST TOTALS : ",							// HostTotals
	"DEBIT",									// Debit
	"ACQUIRER TABELLE",							// AcqTable
	"HAENDLER #:",								// Merchant#
	"TERMINAL ID",							// TerminalID
	"KAUF  ",									// Sales
	"GUTSCHRIFT",									// Refunds
	"RUECKGELD",									// CashBk
	"BETRAG  ",									// Amount
	"LOG",										// LOG
	"AN",										// ON
	"BERICHT",									// Report
	"STATUS ABSCHLUSS",						// Settlement Status
	"NOTIFICATION OF RECEIPT UPLOAD",			// NotOfrRec
	"ANZAHL EINGEREICHTER BELEGE: ",			// NumOfRec
	"EINREICHUNG FEHLGESCHLAGEN",					// RecUpload
	"1) Bitte manuellen ABSCHLUSS durchfuehren",				// PleasSettl
	"2) Falls Einreichung weiterhin fehlschlaegt,",	// UploadFaiL
	"Bitte HELP DESK kontaktieren",								// HelpDesk
	"PERFORMANCE BERICHT",       				// lblPerfRpt
    "Genehmigt :  " ,            				// lblTotAprorv  
    "Abgelehnt :  " ,           					// lblTotDenials 
    "Manuell   :  " ,          				    // lblTotManual  
    "Magnet.   :  " ,            				// lblTotSwipe    
    "Transaktionen  " ,        				    // lblTransCount 
	"ANZAHL        :  ",		    				// TransCount    
    "Redial       :  " ,        				// lblTotRedials 
    "Timeout      :  " ,        				// lblTottimeout 
    "Retransmit.  :  " ,        				// lblTotRetran  
    "ZEITRAUM: ",            	    				// Period        
	"  To  ",				    				// To            
	"KOMMUNIKATION STATUS",     				// Comm          
	"KONFIGURATION BERICHT",   					// lblConfig_Rpt 
	"AENDERN",									// Adjust		  
	"AUS",                      				// OFF               
};

//! Second Text table - TX_EXTENDED
static const char *const PclGrText2[] = {
   	"PERCENT #1",	   			   //	Percent1 
   	"PERCENT #2",	   	   	       //	Percent2 
   	"PERCENT #3", 	 		       //	Percent3 
	"ACQUIRER AVAILABLE",	       //	lblAcqAvail		  
	" CONFIGURATION BY ACQUIRER ", //	lblConfbyAcq
	"NII",					  	   //	NII
	"CARD ACCEPT TERMINAL",		   //	CardAcceptTerm
	"CARD ACCEPT MERCHANT",		   //	CardAcceptMerc
	"VISA I TERMINAL ID",		   //	VisaITermId
	"NOT A SETTLEMENT ACQ.",       //	NotASettlement
	"IGNORE HOST TIME",			   //	IgnoreHostTime
	"ISDN PROTOCOL",    		   //	ISDNprotocol
	"ERC COMPRESSION",			   //	ErcCompression
	"CURRENT BATCH",			   //	CurrentBatch
	"NEXT BATCH",				   //	NextBatch
	"MAX.SETTLE. ATTEMPS",  	   //	MaxSettleAttemp
    "SIGN ON HOST INFORMATION",//	lblSignOnHostInform
	"TRANSACTION HOST INFORMATION",//	lblTransHostInform
	"PRIMARY",					   //	Primary
	"PHONE NUMBER",				   //	PhoneNum
	"CONNECT TIME",				   //	ConnectTime
	"DIAL ATTEMPS",				   //	DialAttemps
	"MODEM MODE",				   //	ModemMode
	"SECONDARY",				   //	Secondary
	"SETTLEMENT HOST INFORMATION", //	lblSettleHostInf
	"ISSUER TABLE", 			   //	lblIssuerTab - Issuer Table
	"ISSUER AVAILABLE",			   //	lblIssAvail
	"CONFIGURATION BY ISSUER",	   //	lblConfigByIss
	"CAPTURE TRANSACTION",		   //	CaptureTrans
	"PRINT A RECEIPT",			   //	PrintReceipt
	"CAPTURE RECEIPT",			   //	CaptureReceipt
	"FLOOR LIMIT",				   //	FloorLimit
	"REAUTH PERCENT",			   //	ReauthPercent
	"ACH CHECK TRANS.",			   //	AchCheckTrans
	"USE 6 DIGIT AUTH",			   //	Use6Digit
	"PIN ENTRY REQUIRED",		   //	PinEntryReq
	"ALLOW CASH BACK",			   //	AllowCashBack
	"ALLOW CASH ONLY",			   //	AllowCashOnly
	"PROCCESING OPTIONS",   	   //	lblProcOption
	"INVOICE NUMBER",			   //	InvoiceNum
	"MANUAL PAN ENTRY",			   //	ManualPan
	"MOD 10 CHECK ON PAN",		   //	Mod10Check
	"IGNORE TRACK 1 ERROR", 	   //	IgnoreTrack
	"EXPIRY DATE REQUIRED",		   //	ExpiryDate
	"CHECK EXPIRY DATE",		   //	CheckExpiry
	"OFFLINE ALLOWED",             //	OfflineAllow
	"ADJUST ALLOWED",			   //	AdjustAllow
	"VOICE REFERRAL",			   //	VoiceReferral
	"PROMPT FOR TAX",			   //	PromptForTax
	"AVS OPTIONS",				   //	lblAvsOptions
	"AVS ENABLED ON SALE",  	   //	AvsEnab
 	"PROMPT ADDRESS",			   //	PromptAddress
	"PROMPT ZIP CODE",			   //	PromptZipCode
	"BLOCKED TRANSACTIONS",  	   //	lblBlockTrans
	"CARD VERIFY",				   //	CardVerif
	"BALANCE INQUIRE",			   //	BalInq
	"PIN REQUIRED",                //   lblPinRequired
	"CARD RANGE TABLE",			   //	lblCardRangTab
	"CARD RANGE AVAILABLE",		   //	lblCardRangAvai
	"CONFIGURATION BY CARD RANGE", //	lblConfByCard
	"CARD RANGE",				   //	CardRang
	"PAN RANGE LOW",    		   //	PanRangeLow
	"PAN RANGE HIGH",			   //	PanRangeHigh
	"PAN LENGTH",				   //	PanLength
	"ID",	        			   //   lblID
	"NAME ",                       //   lblName
	"CARD NUMBER",  			   //	lblCrnum
	"SUPPORTED IN", 			   //	SupportIn
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
//!      Add German PCL messages table for DML support
//!
//! \param
//!		
//! \return
//!     None
//!
//! \note
//!
extern void PclTextGr_Init( void )
{
	short pgnum;

	// Verify the number of messages match
	TableItemCount = LAST_PCLTEXTDEF_ID;
	if( TableItemCount != ( sizeof( PclGrText1 ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "PclGrText1" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg( );
	}

	// Verify the number of messages match
	TableItemCount = LAST_PCLTEXTEXT_ID;
	if( TableItemCount != ( sizeof( PclGrText2 ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "PclGrText2" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg( );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put PCL text table PclEnText into database
	TextIndexAddTable( pcltext_tableDef_id, LNG_DE, PclGrText1, pgnum );
	TextIndexAddTable( pcltext_tableExt_id, LNG_DE, PclGrText2, pgnum );
}

//=============================================================================
// Private function definitions
//=============================================================================

#endif // MAKE_GERMAN

