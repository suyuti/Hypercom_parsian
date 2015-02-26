
#ifdef MAKE_TURKISH
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             pclentext.c
//      PCL English Text Table.
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
// Because the text field processing is limited to up to 255 records 
// an extended table capability has been included. Now PCL is supporting 
// 2 tables - "PclEstext1" referred as "TX_Default" and "PclEstext2" 
// referred as "TX_Extended". 


// FIRST Text table - TX_DEFAULT -

static const char *const PclTrText1[] = {
	"en",										// English
	"MERCHANT #: ",								// MERCHANT-  5
	"SRV : ",									// SERVER -   7
	"CSH : ",									// CASHIER -  7
	" TILL : ",									// TILL -     7
	"VOID ",									// VOID_S -   9
	"FOOD",										// FOOD -     10
	"CASH",										// CASH -     10
	" BENEFIT",									// BENEFIT -  10
	"EXP.: ",									// EXPIRE -   10
	"DATE: ",									// DATE -     12
	"TIME: ",									// TIME -     12
	"AUTH NO: ",								// AUTH NO -  13
	"FCS ID: ",									// FCS -      13
	"AVS: ",									// AVS -      13
	"AVAIL BALANCE",							// AVAIL_BAL -15,18,19
	"TODAY",									// TODAY -    15
	"BASE",										// BASE -     15
	"TAX",										// TAX -      15
	"BAR",										// BAR -      15
	"SURCHARGE",								// SURCHARGE - 15
	"TIP",										// TIP -      15
	"TIP GUIDE",								// TIP GUIDE - 15
	"SALE AMT",									// SALE_AMT - 15
	"CASH BACK",								// CASH_BACK -15
	"FEE",										// FEE
	"AMOUNT DUE:",								// AMOUNT DUE
	"*** NO REFUNDS ALLOWED",					// NoRefundsAllow
	"* OFFLINE ENTERED *",						// OfflineEntered
	"FOOD BAL =",								// FoodBal
	"CASH BAL =",								// Cash Bal
	"*** DUPLICATE ***",						// DUPLICATE
	"SIGNATURE NOT REQUIRED",					// SignatureNotReq
	" - AVS CANCELED",							// AvsCanceled
	"**** TRAINING MODE ****",					// TrainingMode
	"I AUTHORIZE THE MERCHANT TO CONVERT MY",	// IAuthorizeElect
	"CHECK TO AN ELECTRONIC FUNDS TRANSFER",	// FundFromMyAccount
	"OR PAPER DRAFT, AND TO DEBIT MY ACCOUNT",	// OrPaperDraft	
	"FOR THE AMOUNT OF THE TRANSACTION.",		// ForTheAmount
	"IN THE EVENT THIS CHECK IS RETURNED",		// InTheEvent
	"UNPAID, I UNDERSTAND THAT A ",				// Unpaid
	".",										// Point
	"RETURN",									// Return
	"ITEM FEE MAY BE CHARGED TO MY ACCOUNT",	// IEM_FEE
	"I AUTHORIZE THE TRANSFER OF FUNDS",		// Transfer_funds
	" FOR THE AMOUNT OF THIS TRANSACTION",		// ForTAmount
	" I AGREE TO PAY ABOVE TOTAL AMOUNT",		// IAGREE
	"ACCORDING TO CARD ISSUER AGREEMENT", 		// ACCORDING 
	"MERCHANT AGREEMENT IF CREDIT VOUCHER",		// MerchantAgreement	
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
	"ALL ACQUIRERS",							// AllAcquirers
	"INV#  TRANS",								// InvTrans
	"SERVER ",									// Server
	"CASHR. ",									// Cashr
	"AUTH",										// AUTH	
	"CASHBK",									// Cashbk
	"TOPLAM",								// LastTotal 
	"CASH TOTAL",								// CashTotal
	"TRANS TOTAL",								// TransTotal
	"SHIFT ",									// Shift
	"*",										// Star
	"OPEN TABS",								// OpenTabs
	"INV# TRANS",								// InvTrans
	"TRANSACTION TOTALS",						// TransTotals
	"BY ISSUER",								// By Issuer
	"CASHIER : ",								// Cashier:
	" SERVER :  ",								// PCLServer:
	"   TILL : ",								// Till:
	"GRAND TOTALS",								// GranTotals
	"NOTHING TO PRINT",							// NothingToPrint
	"UNADJ",									// Unadj
	"AUDIT",									// Audit
	"TERMINAL",									// Terminal
	"DATABSE REPORT ",							// DatabaseReport
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
	"ACQUIRER TABLE",							// AcqTable
	"MERCHANT #:",								// Merchant#
	"SALES  ",									// Sales
	"REFUNDS",									// Refunds
	"CASH BK",									// CashBk
	"AMOUNT  ",									// Amount
	"LOG",										// LOG
	"ON",										// ON
	"REPORT",									// Report
	"SETTLEMENT STATUS",						// Settlement Status
	"NOTIFICATION OF RECEIPT UPLOAD",			// NotOfrRec
	"NUMBER OF RECEIPTS UPLOADED: ",			// NumOfRec
	"RECEIPT UPLOAD FAILED",					// RecUpload
	"1) PLEASE SETTLE MANUALLY",				// PleasSettl
	"2) IF UPLOAD FAILS AGAIN PLEASE CONTACT",	// UploadFaiL
	"HELP DESK.",								// HelpDesk
	"PERFORMANCE REPORT",       				// lblPerfRpt
    "Approvals:  " ,            				// lblTotAprorv  
    "Denials  :  " ,           					// lblTotDenials 
    "Manuals  :  " ,          				    // lblTotManual  
    "Swipe    :  " ,            				// lblTotSwipe    
    "Transactions  " ,        				    // lblTransCount 
	"Count        :  ",		    				// TransCount    
    "Redials      :  " ,        				// lblTotRedials 
    "Timeout      :  " ,        				// lblTottimeout 
    "Retransmits  :  " ,        				// lblTotRetran  
    "Period: ",            	    				// Period        
	"  To  ",				    				// To            
	"COMMUNICATION STATUS",     				// Comm          
	"CONFIGURATION REPORT",   					// lblConfig_Rpt 
	"ADJUST",									// Adjust		  
	"OFF",                      				// OFF               
};

// Second Text table - TX_EXTENDED -

static const char *const PclTrText2[] = {
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

static const char *const PclTrText3[] = 
{
	"TÝP",								// Type
	"ADET",							// Count
	"TOP. TUTAR",						// TotAmount
	"YÝ Kredi",						// YiKredi
	"YD Visa Kredi",					// YdVisaKredi
	"YD Euro Kredi",					// YdEuroKredi
	"YÝ Debit",							// YiDebit
	"YD Debit",							// YdDebit
	"SATIÞ",								// Sale
	"ÝADE",								// Refund
	"ÝPTAL",							// Void
	"ÖDEME",							// Payment

	"ÝÞLEM TUTARI",							// Total
	"NET TUTAR",						// NetTotal

	"ÝÞYERÝ NO : ",
	"TERM NO   : ",
	"STAN : ",
	"BATCH : ",
	"INVOICE : ",
	"SÜRÜM : ",
	"KAYNAK : ",
	"KARD TÝPÝ : ",
	"AID : ",
	"REF. NO : ",

	"DETAY RAPOR",
	"ÖZET RAPOR",
	"ISSUER TOPLAMLARI",
	"GRAND TOTALS",

	// Response texts
	"AUTH NO : ",						// AuthNo
	"ÝÞLEM BAÞARISIZ!!!",			// TransactFailed

	// Successfull Txt
	"GÜNSONU BAÞARI ÝLE TAMAMLANDI",	// SucSettle
	"Sale is done Successfully",		// SucSale
	"Refund is done Successfully",		// SucRefund
	"Void is done Successfully",		// SucVoid

	// Information Texts
	"Not: ",
	"YÝ : 'YURTÝÇÝ'",
	"YD : 'YURTDIÞI'",
	"2. NÜSHA",
	"KART SAHÝBÝNE AÝTTÝR",
	"IMZA : ",
	".: ÞÝFRE KULLANILMIÞTIR :.",
	"GÜVENLÝ ÝÞLEM YAPTINIZ",
	"TEÞEKKÜRLER",

	"YUKARIDA KAYITLI TUTAR KARÞILIÐI MAL ", 
	"VEYA HÝZMET TESLÝM ALINMIÞTIR",
	"",
	"TAKSÝT BÝLGÝLERÝ",					// InstallmentInfo
	"TAKSÝTLER  ",						// Installs	 
	"ÝLK HESAP KESÝM TRH: ",            // FirstBillDate                     
    "UYG. ADI:",						// AppLbl
	"VADE:",							// ExpDate
	"KART NO",							// CardNumber
	"PARAMETRE YÜKLEME",				// Initial
	"TAMAMLANDI",						// Success
#ifdef MAKE_KIB        
        "KULLANILABÝLÝR SMARTPUAN'INIZ",                        // AvailablePoint
        "SMART BÝLGÝ NOTU",                                  // Note
        "***** SMARTCARD ****** ",                           // Note2 
        "    ALIÞVERÝÞ KEYFÝ    ",                            // Note3        
        "SMART PUAN TUTARI",                                  //SmartTotal,
        "Smartcard Ýndirim",                                  // SmartDiscount,
        "Avantajý %",                                         // Advantage,
        "TOPLAM" ,                                            // LastTotal
        "ARA TOPLAM",                                         // SubTotal        
        "KULLANILABÝLÝR SMARTPUAN TUTARINIZ",                 // FailMsg1
        "ÝÞLEM TUTARINDAN BÜYÜKTÜR.",                         // FailMsg2
        "BU DURUMDA PUAN ÝLE TAKSÝTLÝ ÝÞLEM ",                // FailMsg3
        "YAPAMAZSINIZ",                                       // FailMsg4
        "PUAN ÝLE PEÞÝN ALIÞVERÝÞ YAPMANIZ ",                 // FailMsg5
        "UYGUNDUR.",                                          // FailMsg6
        
#endif
	// Debug Txt Fields
	"Not Supported Yet!!!!"				// NotSupport
};

//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//	PclTextEn_Init	Add English PCL messages table for DML support.
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
//-----------------------------------------------------------------------------

extern void PclTextTr_Init( void )
{
	short pgnum;

	// Verify the number of messages match
	TableItemCount = LAST_PCLTEXTDEF_ID;
	if( TableItemCount != ( sizeof( PclTrText1 ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "PclTrText1" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg( );
	}

	// Verify the number of messages match
	TableItemCount = LAST_PCLTEXTEXT_ID;
	if( TableItemCount != ( sizeof( PclTrText2 ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "PclTrText2" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg( );
	}

	// Verify the number of messages match
	TableItemCount = LAST_SERVUSTEXTDEF_ID;
	if( TableItemCount != ( sizeof( PclTrText3 ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "PclTrText3" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg( );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put PCL text table PclEnText into database
	TextIndexAddTable( pcltext_tableDef_id, LNG_TR, PclTrText1, pgnum );
	TextIndexAddTable( pcltext_tableExt_id, LNG_TR, PclTrText2, pgnum );
	TextIndexAddTable( pcltext_servusDef_id, LNG_TR, PclTrText3, pgnum );
}

//=============================================================================
// Private function definitions
//=============================================================================

#endif 

