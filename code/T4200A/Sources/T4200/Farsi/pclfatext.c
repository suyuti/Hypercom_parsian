
#ifdef MAKE_FARSI
//=============================================================================
//                       Altius It
//                      (c) Copyright 2007
//=============================================================================
//
// Module overview:             pclfatext.c
//      PCL Farsi Text Table.
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
static const char *const PclFaText1[] = {
	"fa",																						// English
	"\xF1\xAF\xEA\xB1\xF3\xB0\x99\x20\xEE\xB1\x93\xE7\xBA",									// MERCHANT-  5
	"",																							// SERVER -   7
	"\xAF\xD7\xEA",																			// CASHIER -  7
	"\x93\x9D",																				// TILL -     7
	"\xF5\xF0\x9D",																				// VOID_S -   9
	"\x91\xB0\xCE",																				// FOOD -     10
	"\xAF\xD7\xEA",																				// CASH -     10
	"BENEFIT",																					// BENEFIT -  10
	"VOID",																						// VOID   -   10
	"\xF6\x93\xC3\xD7\xEA\x91",																// EXPIRE -   10
	"\xF1\x9E\xB6\xAF",																					// BATCH -    11
	"INVOICE",																				// INVOICE -  11
	"\xAE\xF3\xB1\x93\x9D",																	// DATE -     12
	"\xE9\x93\xE6\xB2",																		// TIME -     12
	"RRN",																					// RRN: -     13
	"\xAF\xF4\xF4\xF8\x9D\x20\xEE\xB1\x93\xE7\xBA",											// AUTH NO -  13
	"",																							// FCS -      13
	"\xB4\xB8\xB1\xAF\x92\x20\xAF\xF4\xF4\xF8\x9D\x20\xE8\x9E\xB7\xF4\xB6",					// AVS -      13
	"\xEE\xAF\xEA\x93\xE6",																		// AVAIL_BAL -15,18,19
	"\xB2\xED\xB1\xE6\x91",																		// TODAY -    15
	"",																							// BASE -     15
	"\x9C\x93\xF4\xE2\x93\xE6",																	// TAX -      15
	"",																							// BAR -      15
	"\x93\xF0\x95\x20\xF1\xD2\x93\xC2\x91",														// SURCHARGE - 15
	"\xE5\x93\xCB\xEA\x91",																		// TIP -      15
	"\xCC\xE7\xA5",																				// TOTAL -    15
	"",																							// TIP GUIDE - 15
	"\xB4\xB9\xED\xB1\xD2\x20\xE9\x91\xB2\xF4\xE6",												// SALE_AMT - 15
	"\xF2\xAF\xD7\xEA\x20\xF1\xF4\xD7\x95",														// CASH_BACK -15
	"\xF5\xD2",																					// FEE
	"\x9F\xAD\x91\xAF\xB1\x99\x20\xAF\xCA\xED\xE6",											// AMOUNT DUE
	"\x9F\xB7\xF4\xEA\x20\xB2\x93\xA5\xE6\x20\x9F\xAD\x91\xAF\xB1\x99\xB2\x93\x95\x20\x2A\x2A\x2A",// NoRefundsAllow
	"\x2A\x20\x6F\x66\x66\x6C\x69\x6E\x65\x20\xAF\xED\xB1\xED\x20\x2A",							// OfflineEntered
	"",																							// FoodBal
	"\x3D\xAF\xD7\xEA\x20\xEE\xAF\xEA\x93\xE6",													// Cash Bal
	"\x2A\x2A\x2A\x20\xF2\xB1\x91\xB1\xDB\x9D\x20\x2A\x2A\x2A",									// DUPLICATE
	"\x9F\xB7\xF4\xEA\x20\xE5\xB2\x93\xE2\x20\xF6\x93\xC3\xE6\x91",								// SignatureNotReq
	"\xAF\xBA\x20\xED\xCF\xE2\x20\xB4\xB5\xB1\xAF\x92\x20\xAF\xF4\xF4\xFF\x9D\x20\x2D\x20",		// AvsCanceled
	"\x2A\x2A\x2A\x20\xF5\xBA\xB2\xED\xE7\x92\x20\x9F\xE2\x93\xAA\x20\x2A\x2A\x2A\x2A",			// TrainingMode
	"I AUTHORIZE THE MERCHANT TO CONVERT MY",													// IAuthorizeElect //Note:  Must be re-translated shorter.
	"",																							// FundFromMyAccount
	"",																							// OrPaperDraft	
	"",																							// ForTheAmount
	"IN THE EVENT THIS CHECK IS RETURNED",														// InTheEvent //Note:  Must be re-translated shorter.
	"\x7E",																						// Unpaid
	".",																						// Point
	"\x7E",																						// Return
	"\x7E",																						// IEM_FEE
	"\xB1\xF3\xAF\x93\xD7\xE6\x20\xF5\xF3\x93\xA5\x95\x93\xA5",									// Transfer_funds
	" FOR THE AMOUNT OF THIS TRANSACTION",														// ForTAmount
	" I AGREE TO PAY ABOVE TOTAL AMOUNT",														// IAGREE
	"ACCORDING TO CARD ISSUER AGREEMENT", 														// ACCORDING  //Note:  Must be re-translated shorter.
	"\x20\xEE\xAF\xEA\xB1\xF3\xB0\x99\x20\xF2\xED\xB6\x20\xB2\x91\x20\xB4\xC4\x96\xD6\x20\xAF\xF4\xF4\xF8\x9D",	// MerchantAgreement
	"TRANSACTION FAILED",						// TransactFailed
	"FARSI (PERSIAN) CHARACTERS:",			// NormalFont
	"\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89 \xB4\xB5 \xB4\xB9 \xB4\xBD \xB4\xC1",	// S_ABCDEFGHIJ
	"\x90 \x91 \x92 \x93 \x94 \x95 \x96 \x97 \x98 \x99 \x9A \x9B \x9C \x9D \x9E \x9F",// S_KLMNOPQRST
	"\xA0 \xA1 \xA2 \xA3 \xA4 \xA5 \xA6 \xA7 \xA8 \xA9 \xAA \xAB \xAC \xAD \xAE \xAF",// S_UVWXYZ
	"\xB0 \xB1 \xB2 \xB3 \xB4 \xB5 \xB6 \xB7 \xB8 \xB9 \xBA \xBB \xBC \xBD \xBE \xBF",// S_abcdefghi
	"\xC0 \xC1 \xC2 \xC3 \xC4 \xC5 \xC6 \xC7 \xC8 \xC9 \xCA \xCB \xCC \xCD \xCE \xCF",// S_klmnopqrst
	"\xD0 \xD1 \xD2 \xD3 \xD4 \xD5 \xD6 \xD7 \xD8 \xD9 \xDA \xDB \xDC \xDD \xDE \xDF",// S_uvwxyz0123
	"\xE0 \xE1 \xE2 \xE3 \xE4 \xE5 \xE6 \xE7 \xE8 \xE9 \xEA \xEB \xEC \xED \xEE \xEF",// S_SetSymbol1
	"\xF0 \xF1 \xF2 \xF3 \xF4 \xF5 \xF6 \xF7 \xF8 \xF9 \xFA \xFB \xFC \xFD \xFE \xFF",// S_SetSimbols2
	"\xF5\xE2\xED\xE7\xCB\xE6",																	// Normal
	"\xB1\x95\x91\xB1\x95\xED\xAF",																// Double
	"\xE8\xE3\xD6\x20\xB1\xF4\xF4\xCF\x9D",														// FontChange
	"\xE9\x93\xF3\x93\x99",																		// THE END
	"\xF5\xB6\xB1\x95\x93\xB7\xAA\x20\xB4\xB9\xB1\x91\xB2\xDE",									// AuditReport
	"\xE9\x93\xDE\xAF\xEA\xB1\xF3\xB0\x99\x20\xF1\xE7\xEF",										// AllAcquirers
	"",																							// InvTrans
	"\xB1\xED\xB1\xB6",																			// Server
	"\xD5\xED\xAF\xEB\xBE",																		// Cashr
	"\xAF\xF4\xF4\xF8\x9D",																		// AUTH
	"\x9C\xB1\x93\xDA\x20\xEE\xB1\x93\xE7\xBA",													// CardNumber
	"CARD TYPE",																				// CardType
	"\xF5\x9E\xBB\xDE\xB2\x93\x95\x20\xD0\xE3\x96\xE6",											// Cashbk
	"\xE5\x93\xCB\xEA\x91\x20\xCC\xE7\xA5",														// TipTotal
	"\xAF\xD7\xEA\x20\xCC\xE7\xA5",																// CashTotal
	"\xB4\xBC\xEB\xDA\x91\xB1\x9D\x20\xCC\xE7\xA5",												// TransTotal
	"\xE4\xF3\xAF\x96\x9D",																		// Shift
	"*",																						// Star
	"",																							// OpenTabs
	"",																							// InvTrans
	"\xB4\xBC\xEB\xDA\x91\xB1\x9D\x20\xCC\xE7\xA5",												// TransTotals
	"\xEE\xAF\xEB\xEB\xDA\xB1\xAF\x93\xBE\x20\xC6\xB6\xED\x9D",									// By Issuer
	"\xB1\x91\xAF\xD6\xED\xAF\xEB\xBE",														// Cashier:
	"SERVER",																				// PCLServer:
	"TILL",																				// Till:
	"\xE4\xDA\x20\xCC\xE7\xA5",																	// GranTotals
	"\x9F\xEB\xF3\xB1\x99\x20\xE4\x95\x93\xD6\x20\xB1\xF4\xCE",									// NothingToPrint
	"",																							// Unadj
	"\xF2\xB2\xF4\xE7\xE6",																		// Audit
	"\xF1\xEA\x93\xF3\x93\x99",																	// Terminal
	"\x93\xEF\x20\xEE\xAF\x91\xAF\x20\xEE\x93\xDF\xF3\x93\x99\x20\xB4\xB9\xB1\x91\xB2\xDE",		// DatabaseReport
	"\xF1\xEA\x93\xF3\x93\x99\x20\xF1\xC8\xD2\x93\xAA\x20\xE8\xF4\xC8\xEB\x9D",					// TermEpromConfig
	"\xF1\xEA\x93\xF3\x93\x99\x20\xE8\xF4\xC8\xEB\x9D",											// termConfig
	"\xF1\xAF\xEA\xB1\xF3\xB0\x99\x20\xAF\xDA",												// AcqId
	"\xF1\xAF\xEB\xEB\xDA\xB1\xAF\x93\xBE\x20\xAF\xDA",										// IssuerID
	"\x9F\xB1\x93\xDA\x20\xEE\xAF\xED\xAF\xAA\xE6\x20\xAF\xDA",																// CardRangeId
	"\xAF\xDA\x20\xF1\xB6\x93\xEB\xBA",																							// Id_Descriptor
	"\x32\x93\xB2\xF3\xED\x20\xAF\xDA",																							// Visa2Id
	"",																															// AddPromptId
	"\x29\xE4\x93\xE7\xF4\xB6\xAF\x91\xB2\xDF\xEF\x20\x28\x97\x93\xF4\x9D\x20\xE5\x93\xAD\x20\x9C\x93\xCA\x93\xE3\xC5\x91",	// RawData
	"\xF1\xAF\xBA\x20\x9F\xD2\x93\xF3\xB1\xAF\x20\x9C\x93\xCA\x93\xE3\xC5\x91",												// ReceivedData
	"\xF5\xE2\x93\xB6\xB1\x91\x20\xEE\xAF\x91\xAF",																			// SendData
	"\x68\x6F\x73\x74\xF1\x91\x20\xCC\xE7\xA5",																					// HostTotals
	"\xB1\x93\xDB\xEF\xAF\x95",																									// Debit
	"\xEE\xAF\xEA\xB1\xF3\xB0\x99\x20\xE1\xED\xAF\xA5",																			// AcqTable
	"\xF1\xAF\xEA\xB1\xF3\xB0\x99\x20\xEE\xB1\x93\xE7\xBA",																	// Merchant#
    "\xF1\xEA\x93\xF3\x93\x99\x20\xEE\xB1\x93\xE7\xBA",																											// TerminalID
	"\xB4\xB9\xED\xB1\xD2",																										// Sales
	"\xAF\x91\xAF\xB1\x9E\xB6\x91",																								// Refunds
	"",																															// CashBk
	"\xB1\x91\xAF\xD7\xE6",																										// Amount
	"\xF1\xA8\xAD\xF3\xB1\x93\x9D",																								// LOG
	"\xEC\xBA\xED\xB1",																											// ON
	"\xB4\xB9\xB1\x91\xB2\xDE",																									// Report
	"\xD8\xD2\x91\xED\x9D\x20\x9F\xF4\xCB\xC2\xED",																				// Settlement Status
	"\xAF\xF4\xB6\xB1\x20\xE9\xAF\x93\x9E\xB6\xB1\xD2\x20\xB4\xB9\xB1\x91\xB2\xDE",												// NotOfrRec
	"\xF1\xAF\xBA\x20\xEE\xAF\x93\x9E\xB6\xB1\xD2\x20\xF2\x93\xEF\xAF\xF4\xB6\xB1\x20\xAF\x91\xAF\xCB\x9D",					// NumOfRec
	"\xAF\xED\x95\x20\xD8\xD2\xED\xE6\x93\xEA\x20\xAF\xF4\xB6\xB1\x20\xE9\xAF\x93\x9E\xB6\xB1\xD2",								// RecUpload
	"1) PLEASE SETTLE MANUALLY",																								// PleasSettl //Note:  Must be re-translated shorter.
	"2) IF UPLOAD FAILS AGAIN PLEASE CONTACT",																					// UploadFaiL //Note:  Must be re-translated shorter.
	"\xF5\xF3\xED\xDF\xAD\xB6\x93\x99\x20\xAF\xAA\x91\xED",																		// HelpDesk
	"\xF5\xD3\xF4\xDA\x20\xB4\xB9\xB1\x91\xB2\xDE",																				// lblPerfRpt
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
	"\xC5\x93\x96\x9D\xB1\x91\x20\x9F\xF4\xCB\xC2\xED",						// Comm          
	"\x9C\x93\xE7\xF4\xC8\xEB\x9D\x20\xB4\xB9\xB1\x91\xB2\xDE",				// lblConfig_Rpt 
	"\xE8\xF4\xC8\xEB\x9D",                                                 // Adjust		  
	"\xB4\xB9\xED\xE6\x93\xAD",												// OFF               
    "\xF2\xB1\x9E\xBB\xE6\x20\xE5\x93\xEA",                        // CardHolderName
    "\xE8\x9E\xB7\xF4\xB6\x20\xF5\xDB\xEA\x93\x95\x20\xEE\xB1\x93\xE7\xBA",
    "\xF6\x93\xC3\xE6\x91",         // Imza'
    "\xF2\xB1\x93\xFD\x96\x9E\xCA\x91\x20\x9C\xB1\x93\xFD\xDA",     //Kart i'tibari
    "\xE9\x93\xFD\xFD\xFD\xF4\xB6\xB1\x93\xFD\x99",                // Parsian
    " ", // Space
};

//! Second Text table - TX_EXTENDED
static const char *const PclFaText2[] = {
   	"\x31\x20\x23\x20\xAF\xBE\xB1\xAF",																			//	Percent1 
   	"\x32\x20\x23\x20\xAF\xBE\xB1\xAF",																			//	Percent2 
   	"\x33\x20\x23\x20\xAF\xBE\xB1\xAF",																			//	Percent3 
	"\xB4\xB5\xB1\x9E\xB6\xAF\x20\xB1\xAF\x20\xEE\xAF\xEA\xB1\xF3\xB0\x99",										//	lblAcqAvail		  
	"\xEE\xAF\xEA\xB1\xF3\xB0\x99\x20\xC6\xB6\xED\x9D\x20\xE8\xF4\xC8\xEB\x9D",									//	lblConfbyAcq
	"\x4E\x49\x49\x20\xC5\x93\x96\x9D\xB1\x91\x20\xB1\xF4\xB7\xE6\x20\xAF\xDA",									//	NII
	"\x9C\xB1\x93\xDA\x20\xEE\xAF\xEB\xEB\xDA\x20\xE1\xED\x96\xD6\x20\xF1\xEA\x93\xF3\x93\x99",					//	CardAcceptTerm
	"\x9C\xB1\x93\xDA\x20\xEE\xAF\xEB\xEB\xDA\x20\xE1\xED\x96\xD6\x20\xEE\xAF\xEA\xB1\xF3\xB0\x99",				//	CardAcceptMerc
	"\x49\x20\x56\x49\x53\x41\x20\xF1\xEA\x93\xF3\x93\x99\x20\xAF\xDA",											//	VisaITermId
	"\x9F\xB7\xF4\xEA\x20\xE5\xB2\x93\xE2\x20\xF1\xF3\xED\xB7\x9D",												//	NotASettlement
	"\xAF\xED\xBA\x20\xF1\x9E\xD2\xB1\xDE\x20\xEE\xAF\xF3\xAF\x93\xEA\x20\x68\x6F\x73\x74\x20\xE9\x93\xE6\xB2",	//	IgnoreHostTime
	"\x49\x53\x44\x4E\x20\xE4\xDB\x9D\xED\xB1\x99",											//	ISDNprotocol
	"\x45\x52\x43\x20\xF2\xB2\x93\xB6\x20\xEE\xAF\xB1\xBB\xD2",								//	ErcCompression
	"\xF5\xE3\xCB\xD2\x20\xF1\x9E\xB6\xAF",													//	CurrentBatch
	"\xF2\xAF\xCB\x95\x20\xF1\x9E\xB6\xAF",													//	NextBatch
	"\xF2\xB1\x91\xB1\xD6\xB1\x95\x20\xB4\xB9\x93\xE3\x9D\x20\xE8\xE7\xF3\xB2\xDA\x93\xE6",	//	MaxSettleAttemp
	"\xB4\xBC\xEB\xDA\x91\xB1\x9D\x20\xB2\xDA\xB1\xE6\x20\x9C\x93\xCA\x93\xE3\xC5\x91",		//	lblTransHostInform
	"\xF1\xF4\xE2\xED\x91",																	//	Primary
	"\xEC\xD3\xE3\x9D\x20\xEE\xB1\x93\xE7\xBA",												//	PhoneNum
	"\xB4\xB5\x93\xE7\x9D\x20\xE9\x93\xE6\xB2",												//	ConnectTime
	"\xB4\xB5\x93\xE7\x9D\x20\xAF\x91\xAF\xCB\x9D",											//	DialAttemps
	"\xE5\xAF\xED\xE6\x20\x9F\xE2\x93\xAA",													//	ModemMode
	"\xF1\xF3\xED\xEA\x93\xA1",																//	Secondary
	"\xF1\xF3\xED\xB7\x9D\x20\xB2\xDA\xB1\xE6\x20\x9C\x93\xCA\x93\xE3\xC5\x91",				//	lblSettleHostInf
	"\xEE\xAF\xEB\xEB\xDA\x20\xB1\xAF\x93\xBE\x20\xE1\xED\xAF\xA5",							//	lblIssuerTab - Issuer Table
	"\xB4\xB5\xB1\x9E\xB6\xAF\x20\xB1\xAF\x20\xEE\xAF\xEB\xEB\xDA\xB1\xAF\x93\xBE",			//	lblIssAvail
	"\xEE\xAF\xEB\xEB\xDA\xB1\xAF\x93\xBE\x20\xC6\xB6\xED\x9D\x20\xE8\xF4\xC8\xEB\x9D",		//	lblConfigByIss
	"\xB4\xBC\xEB\xDA\x91\xB1\x9D\x20\x9F\xD2\x93\xF3\xB1\xAF",								//	CaptureTrans
	"\xAF\xF4\xB6\xB1\x20\x98\x93\xA8",														//	PrintReceipt
	"\xAF\xF4\xB6\xB1\x20\x9F\xD2\x93\xF3\xB1\xAF",											//	CaptureReceipt
	"\xE4\xD6\x91\xAF\xAA",																	//	FloorLimit
	"",																						//	ReauthPercent
	"",																						//	AchCheckTrans
	"\xAF\xF4\xEB\xDA\x20\xEE\xAF\x93\xD3\x9E\xB6\x91\x20\xF5\xE7\xD6\xB1\x2D\xF5\xD2\xB1\xAA\x20\x36\x20\xB2\xE6\xB1\x20\xB2\x91",//	Use6Digit
	"\x9F\xB6\x91\x20\xE5\xB2\x93\xE2\x20\xB2\xE6\xB1\x20\xAF\xED\xB1\xED",					//	PinEntryReq
	"\xEC\xDB\xE7\xE6\x20\x9F\xAD\x91\xAF\xB1\x99\xB2\x93\x95",								//	AllowCashBack
	"\xAF\xD7\xEA\x20\xC6\xD7\xD2",															//	AllowCashOnly
	"\xB4\xB9\xB2\x91\xAF\xB1\x99\x20\xF2\x93\xF0\x95\x93\xAD\x9E\xEA\x91",					//	lblProcOption
	"\xB1\xED\x9E\xDA\x93\xD2\x20\xEE\xB1\x93\xE7\xBA",										//	InvoiceNum
	"\xF5\x9E\xB6\xAF\x20\x9C\xB1\xED\xBE\x20\xF1\x95\x20\x50\x41\x4E\x20\xAF\xED\xB1\xED",	//	ManualPan
	"MOD 10 CHECK ON PAN",																	//	Mod10Check //Note:  Must be re-translated shorter.
	"\x31\x20\x54\x72\x61\x63\x6B\x20\xEC\x9E\xD2\xB1\xDE\x20\xEE\xAF\xF3\xAF\x93\xEA",		//	IgnoreTrack
	"\x9F\xB6\x91\x20\xE5\xB2\x93\xE2\x20\xF6\x93\xC3\xD7\xEA\x91\x20\xAE\xF3\xB1\x93\x9D",	//	ExpiryDate
	"\xAF\xF4\xEB\xDA\x20\xF5\xB6\xB1\xB1\x95\x20\x91\xB1\x20\xF6\x93\xC3\xD7\xEA\x91\x20\xAE\xF3\xB1\x93\x9D",//	CheckExpiry
	"\xE1\xED\x96\xD6\x20\xE4\x95\x93\xD6\x20\x4F\x66\x66\x6C\x69\x6E\x65\x20\x9F\xE2\x93\xAA",//	OfflineAllow
	"\xE1\xED\x96\xD6\x20\xE4\x95\x93\xD6\x20\xAB\xF4\xAA\xBF\x9D",							//	AdjustAllow
	"",																						//	VoiceReferral
	"\xAF\xED\xBA\x20\xEE\xAF\xF4\xB6\xB1\x99\x20\x9C\x93\xF4\xE2\x93\xE6\x20\xE9\x91\xB2\xF4\xE6",//	PromptForTax
	"\xB4\xB5\xB1\xAF\x92\x20\xAF\xF4\xF4\xF6\x9D\x20\xE8\x9E\xB7\xF4\xB6\x20\xF2\x93\xF0\xE7\xF4\xC8\xEB\x9D",//	lblAvsOptions
	"AVS ENABLED ON SALE",  																//	AvsEnab //Note:  Must be re-translated shorter.
 	"\xAF\xED\xBA\x20\xEE\xAF\xF4\xB6\xB1\x99\x20\xB4\xB5\xB1\xAF\x92",						//	PromptAddress
	"\xAF\xED\xBA\x20\xEE\xAF\xF4\xB6\xB1\x99\x20\xF5\x9E\xB7\x99\x20\xAF\xDA",				//	PromptZipCode
	"\xEE\xAF\xBA\x20\xF1\xDA\xED\xE3\x95\x20\xF2\x93\xF0\xBB\xEB\xDA\x91\xB1\x9D",			//	lblBlockTrans
	"\x9C\xB1\x93\xDA\x20\xAF\xF4\xF4\xF8\x9D",												//	CardVerif
	"BALANCE INQUIRE",																		//	BalInq
	"\x9F\xB6\x91\x20\xE5\xB2\x93\xE2\x20\xB2\xE6\xB1\x20\xAF\xED\xB1\xED",					//   lblPinRequired
	"\x9C\xB1\x93\xDA\x20\xEE\xAF\xED\xAF\xAA\xE6\x20\xE1\xED\xAF\xA5",						//	lblCardRangTab
	"\xB4\xB5\xB1\x9E\xB6\xAF\x20\xE4\x95\x93\xD6\x20\x9C\xB1\x93\xDA\x20\xEE\xAF\xED\xAF\xAA\xE6",//	lblCardRangAvai
	"\x9C\xB1\x93\xDA\x20\xEE\xAF\xED\xAF\xAA\xE6\x20\xF1\xE3\xF4\xB6\xED\x95\x20\xE8\xF4\xC8\xEB\x9D",//	lblConfByCard
	"\x9C\xB1\x93\xDA\x20\xEE\xAF\xED\xAF\xAA\xE6",											//	CardRang
	"\x50\x41\x4E\x20\xEC\xF4\xF3\x93\x99\x20\xEE\xAF\xED\xAF\xAA\xE6",						//	PanRangeLow
	"\x50\x41\x4E\x20\xF2\x93\xE2\x93\x95\x20\xEE\xAF\xED\xAF\xAA\xE6",						//	PanRangeHigh
	"\x50\x41\x4E\x20\xE1\xED\xC5",															//	PanLength
	"\xAF\xDA\x20\xF1\xB6\x93\xEB\xBA",														//   lblID
	"\xE8\xB6\x91",																			//   lblName
	"\x9C\xB1\x93\xDA\x20\xEE\xB1\x93\xE7\xBA",												//	lblCrnum
	"",																						//	SupportIn
	"",																// IUndestand
	"",																// MayBeConverted	
	"",																// DebitMy
	"",																// ItemIs
	"",																// IWillPay
	"",																// MaximumAmount
	"",																// LawWith
	"",																// FromMy
	"",																// FundsTransfer
	"",                                                             // MannerSer
};

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add Farsi PCL messages table for DML support
//!
//! \param
//!		
//! \return
//!     None
//!
//! \note
//!
extern void PclTextFa_Init( void )
{
	short pgnum;

	// Verify the number of messages match
	TableItemCount = LAST_PCLTEXTDEF_ID;
	if( TableItemCount != ( sizeof( PclFaText1 ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "PclFaText1" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg( );
	}

	// Verify the number of messages match
	TableItemCount = LAST_PCLTEXTEXT_ID;
	if( TableItemCount != ( sizeof( PclFaText2 ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "PclFaText2" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg( );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put PCL text table PclEnText into database
	TextIndexAddTable( pcltext_tableDef_id, LNG_FA, PclFaText1, pgnum );
	TextIndexAddTable( pcltext_tableExt_id, LNG_FA, PclFaText2, pgnum );
}

//=============================================================================
// Private function definitions
//=============================================================================

#endif // MAKE_FARSI

