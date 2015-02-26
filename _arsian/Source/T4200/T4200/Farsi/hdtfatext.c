//=============================================================================
// Company:
//      Altius It
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      hdfantext.c
//
// File Description:
//      Farsi message table
//
//=============================================================================

#ifdef MAKE_FARSI

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkmem.h"

#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "rsptext.h"
#include "hdttextinit.h"
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

//====================================================
//!===  ENGLISH MESSAGE TABLE                       ==
//====================================================
//! Refer to msg.h for enum msg_id definition
const char *const FarsiMessages[] = {
	"CM",						// CustomMsg
	"CM2",						// CustomMsg2
	"CM3",						// CustomMsg3
	"CM4",						// CustomMsg4
	"CM5",						// CustomMsg5
	"CM6",						// CustomMsg6
	"CM7",						// CustomMsg7
	"CM8",						// CustomMsg8
	"SWIPE CUSTOMER CARD",		// Swipecard
	"ENTER CARD NUMBER",															// EnterCard //Note:  Must be re-translated shorter.
	"\x2A\x2A\x2A\x20\x9C\xB1\x93\xDA\x20\xF2\x93\xC6\xAD\x20\x2A\x2A\x2A",         // CardErr
	"\xD8\x96\xC6\xEB\xE6\x20\xB1\xF4\xCE\x20\x9C\xB1\x93\xDA\x20\xC9\xED\xEA",     // UnsuppCard
	"\xB1\x96\x9E\xCB\xE6\x93\xEA\x20\x9C\xB1\x93\xDA",                             // InvalidCard
	"\xF6\xF4\xC3\xD7\xEB\xE6\x20\x9C\xB1\x93\xDA",                                 // ExpiredCard
    "",																				// PrevalidCard @@@@
	"\x9F\xB7\xF4\xEA\x20\xB2\x93\xA5\xE6\x20\xF5\x9E\xB6\xAF\x20\xAF\xED\xB1\xED",	// NoManEntry
    "\x4D\x4D\x59\x59\x20\xF6\x93\xC3\xD7\xEA\x91\x20\xAE\xF3\xB1\x93\x9D",			// ExpireDate
    "\x4D\x4D\x59\x59\x20\xA9\x93\x9E\x9E\xD2\x91\x20\xAE\xF3\xB1\x93\x9D",			// StartDate
    "\xB1\xED\xAF\xBE\x20\xEE\xB1\x93\xE7\xBA",										// IssueNum
	"",																				// TakeImprint
	"\xB1\xAD\x92\x20\xEE\xB1\x93\xE7\xBA\x20\x34\x20\xAF\xED\xB1\xED",				// EntPan4
	"\xF2\xB1\x9E\xBB\xE6\x20\xEE\xB1\x93\xE7\xBA",                                 // CustomerCard
	"\xB1\x91\xAF\xD6\xED\xAF\xEB\xBE\x20\xEE\xB1\x93\xE7\xBA",						// CashierNum
	"\xB1\x96\x9E\xCB\xE6\x93\xEA\x20\xF2\xAF\xED\xB1\xED",                         // InvalidEntry
    "\xB1\x96\x9E\xCB\xE6\x93\xEA\x20\xD0\xE3\x96\xE6",								// InvalidAmount
	"\xD0\xE3\x96\xE6",                                                             // AmountEnt
	"\xD0\xE3\x96\xE6\x20\xAF\xAF\xA5\xE6\x20\xAF\xED\xB1\xED",                     // AmountAgain
	"\xF1\xF3\x93\x99\x20\xD0\xE3\x96\xE6",                                         // AmountBase
	"",                                                                             // AmountBar
	"\x9C\x93\xF4\xE2\x93\xE6\x20\xE9\x91\xB2\xF4\xE6",                             // AmountTax
	"\xE5\x93\xCB\xEA\x91\x20\xE9\x91\xB2\xF4\xE6",                                 // AmountTip
	"\xB4\xB9\xED\xB1\xD2\x20\xE9\x91\xB2\xF4\xE6",                                 // AmountSale
	"\xF2\xAF\xD7\xEA\x20\xE9\x91\xB2\xF4\xE6",                                     // AmountCash
	"\xF1\xBF\xD6\x93\xEB\xE6\x20\xE9\x91\xB2\xF4\xE6",                             // AmountTender
	"\xCC\xE7\xA5",                                                                 // AmountTotal
	"\xF9\xAA\xF4\xAA\xBE",                                                         // CorrectYN
	"\xF2\xB1\x93\x96\x9E\xCA\x91\x20\xF1\xE6\x93\xEA\xB1\x95\x20\xE5\x91\xAF\xDA", // CreditPlan
	"\xF1\xF4\xAA\xF4\xC2\xED\x9D\x20\xEE\xB1\x93\xE7\xBA\x20\xAF\xED\xB1\xED",		// EntDescrip
	"\xB1\xED\x9E\xDA\x93\xD2\x20\xEE\xB1\x93\xE7\xBA",                             // InvoiceNum
	"\xAF\xF4\xF4\xF8\x9D\x20\xB1\xC8\x9E\xEB\xE6",									// WaitApprov @@@@
	"\xEE\xAF\x93\xD3\x9E\xB6\x91\x20\xF5\x95\x20\xC6\xAD",                         // LineIdle
	"\xC6\xAD\x20\xE1\xB1\x9E\xEB\xDA",                                             // CheckingLine
	"* LINE BUSY,  WAIT *",															// LineBusy //Note:  Must be re-translated shorter.
	"\xAF\x91\xB2\x92\x20\xD5\xED\x95\x20\xB1\xC8\x9E\xEB\xE6",						// WaitDialTone
	"\xF2\xB1\xF4\xDE\x20\xEE\xB1\x93\xE7\xBA\x20\xE1\x93\xAA\xB1\xAF",             // DialNow
	"\x94\x91\xED\xA5\x20\xB1\xC8\x9E\xEB\xE6",										// WaitForAnswer
	"\xC6\xAD\x20\xB1\xC8\x9E\xEB\xE6",                                             // WaitForLine
	"\xE5\xAF\xED\xE6\x20\xB4\xB9\xB2\xED\xE7\x92",                                 // TrainModem
	"\xB4\xB9\xB2\x91\xAF\xB1\x99\x20\xE1\x93\xAA\x20\xB1\xAF",                     // ProcNow
	"\xE1\x93\xD7\x9E\xEA\x91\x20\xE1\x93\xAA\xB1\xAF",                             // Transmitting
	"\xF2\xB1\xF4\xDE\x20\xEE\xB1\x93\xE7\xBA\x20\xE1\x93\xAA\xB1\xAF",             // NowDialing
	"\xE5\xAF\xED\xE6\x20\xF2\xB2\x91\xAF\xEA\x91\x20\xEE\x91\xB1",					// InitModem
    "NETWORK LOGON",																// NetworkLogon //Note:  Must be re-translated shorter.
    "\xF2\xAF\xEB\x95\x20\xF1\xAA\xD3\xBE",											// Paging
    "\xEE\xB2\x91\xED\xB1\xAF\x20\x93\x95\x20\xE1\x93\xBF\x9D\x91",					// GateConnecting
    "\x2E\x2E\x2E\xAF\xEA\xED\xF4\x99\x20\xE1\x93\xAA\xB1\xAF",						// Linking
    "\x2E\x2E\x2E\xE4\x93\xBF\x9D\x91\x20\xE1\x93\xAA\xB1\xAF",						// Connecting
	"\x3A\xF1\xC8\xD2\x93\xAA\x20\xF1\xAA\xD3\xBE\x20\x9F\xF4\xCB\xC2\xED",			// MemPgStatus
	"\xEE\x93\xDF\x9E\xB6\xAF\x20\xE8\xF4\xC8\xEB\x9D",								// ConfigTerm
	"",																				// CallHelp
	"\xAF\xF4\xEB\xDA\x20\xF2\xB2\x91\xAF\xEA\x91\x20\xEE\x91\xB1\x20\x93\xD3\xC6\xE2",// PleaseInit
	" ",																			// BlankMsg
	"\x98\x93\xA8\x20\xB1\xAF\x20\x93\xC6\xAD",                                     // PrintError
	"\xB4\xBC\xEB\xDA\x91\xB1\x9D\x20\xF1\xC8\xD2\x93\xAA",							// TransMemErr
	"WAIT  SELF-TEST",																// SelfTest //Note:  Must be re-translated shorter.
	"NOT ENOUGH MEMORY",															// NotEnoughMem //Note:  Must be re-translated shorter.
	"\x7E",																			// ToRunApplic
	"\xF2\xB2\x91\xAF\xEA\x91\x20\xEE\x91\xB1",                                     // InitTran
	"",                                                                             // MerchantFunc
	"\xF1\xF3\xED\xB7\x9D",                                                         // SettleTran
	"\xEE\xAF\xEA\x93\xE6",                                                         // BalanceTran
	"\xF1\xF4\xD7\x95",                                                             // RefundTran
	"\x20\xB1\xED\x9E\xDA\x93\xD2\x20\xEE\xB1\x93\x95\xED\xAF\x20\x98\x93\xA8",     // ReprintTran
	"\xB1\xAD\x92",                                                                 // ReprintLast
	"\xB1\xED\x9E\xDA\x93\xD2",                                                     // ReprintInvoice
	"\x2A\x2A\x20\xC6\xAD\x20\xB1\x95\x20\xB1\xF4\xCE\x20\x9F\xE2\x93\xAA\x20\x2A\x2A",// OfflineTran
	"ENTER STAN NUMBER",															// EnterInvNum //Note:  Must be re-translated shorter.
	"",                                                                             // EnterSOCNum
	"",																				// EnterPONum
	"",                                                                             // MercFunction
	"ENTER MAINT PASSWORD",															// MaintPswd //Note:  Must be re-translated shorter.
	"\x9F\xB7\xF4\xEA\x20\xF5\xDB\xF3\x20\xE8\xEF\x93\x95\x20\x93\xEF\xB2\xE6\xB1",	// PswdNotMatch @@@@
	"\xF9\xAA\xF4\xAA\xBE",                                                         // CorrectYesNo
	"\xEE\xAF\xEA\xB1\xF3\xB0\x99\x20\xF1\xE6\x93\xEA\xB1\x95",                     // MerchProg
	"\xB2\xE6\xB1\x20\xAF\xED\xB1\xED",                                             // EnterPswd
	"\xEE\x93\xDF\x9E\xB6\xAF\x20\xF5\xF3\x93\xB6\x93\xEB\xBA\x20\xEE\xB1\x93\xE7\xBA",// EnterTSer
	"\xF1\xF4\xE2\xED\x91\x20\xEC\xD3\xE3\x9D\x20\xEE\xB1\x93\xE7\xBA",				// EnterInitTel
	"NMS TELEPHONE NO.",															// EnterNMSTel //Note:  Must be re-translated shorter.
	"\xF2\xB1\xF4\xDE\x20\xEE\xB1\x93\xE7\xBA\x20\x9F\xE2\x93\xAA",                 // EnterDialMode
    "\xF6\x93\xC3\xE6\x91",															// Signature
	"Please sign below",															// SignaturePlease //Note:  Must be re-translated shorter.
	"\xAF\xF4\xEA\xB2\x95\x20\x91\xB1\x20\x45\x4E\x54\x45\x52\x20\xED",             // PressEnter
	"",                                                                             // RetrySigCap
	"\x50\x41\x42\x58\x20\xF5\xB6\xB1\x9E\xB6\xAF\x20\xAF\xDA",                     // EnterPABX
	"EXIT, PRESS 'CANCEL'",															// ExitPressCncl //Note:  Must be re-translated shorter.
	"",                                                                             // DuplicateTrans
	"\xF9\xAF\xED\xBA\x20\xF1\xD2\x93\xC2\x91",                                     // AddYesNo
	"NO INV. TO REPRINT",															// NoInvoice //Note:  Must be re-translated shorter.
	"\xAF\xF4\xEB\xDA\x20\xB1\x96\xBE\x20\x93\xD3\xC6\xE2",                         // PleaseWait
	"\xAF\xBA\x20\xE4\xE6\x93\xDA\x20\xB4\xBC\xEB\xDA\x91\xB1\x9D",					// TransComplete
	"\xAF\xBA\x20\xAF\xF4\xF4\xF8\x9D\x20\xB4\xBC\xEB\xDA\x91\xB1\x9D",				// TransAccepted
	"\xF1\xE6\x93\xEA\xB1\x95\x20\xF2\xB1\x91\xB0\xDE\xB1\x93\x95",                 // ProgamLoad
	"",																				// Fun99Password
	"",                                                                             // ProgFunction
	"LOCAL OPTIONS SELECT",															// LocalOptSel //Note:  Must be re-translated shorter.
	"\x9C\x93\xC5\x93\x96\x9D\xB1\x91\x20\xE8\xF4\xC8\xEB\x9D",						// LocalConf
	"\xB2\x93\xA5\xE6\x20\xAF\x91\xAF\xCB\x9D\x20\xB2\x91\x20\xB1\xB0\xDE",			// ExceedMax
	"\xF1\x9E\xD2\xB1\xBB\xF4\x99",													// EnhDial
	"",																				// LDLPmt
	"\x43\x55\x20\xB4\xB5\xB1\xAF\x92",                                             // CUAddrP
	"\x93\xE3\xCB\xD2",                                                             // Currently
	"\xA4\xED\xB1\xAD\x2F\xAF\xED\xB1\xED",                                         // SignOnOff
	"\xEE\xB1\x93\xE7\xBA\x20\x93\x9D\x20\xAF\xED\xB1\xED",                         // TillNum
	"\xF9\xAF\xED\xB1\xED",                                                         // SignOnYN
	"VOID PASSWORD USED",															// VoidPassM //Note:  Must be re-translated shorter.
	"REFUND PASSWORD USED",															// RefPassM //Note:  Must be re-translated shorter.
	"",																				// AdjPassM
	"REPORT PASSWORD USED",															// RepPassM //Note:  Must be re-translated shorter.
	"",																				// KbdDialM
	"",                                                                             // TollDialM
	"\xCC\xF3\xB1\xB6\x20\xF2\xB1\xF4\xDE\x20\xEE\xB1\x93\xE7\xBA",					// MemDialM
	"",																				// RefDialM
	"\xFF\xAF\xED\xBA\x20\xE4\xD3\xD6\x20\xEE\x93\xDF\x9E\xB6\xAF",					// KeyLock
	"\x94\x93\xB7\xAA\x20\xEE\xB1\x93\xE7\xBA\x20\xAF\xED\xB1\xED",					// EnterAcct
	"\xF5\xE3\x96\xD6\x20\xB2\xE6\xB1",												// OldPassword
	"\xAF\xF3\xAF\xA5\x20\xB2\xE6\xB1",												// NewPassword
	"\xEE\xB1\x93\x95\xED\xAF\x20\xAF\xF3\xAF\xA5\x20\xB2\xE6\xB1",					// NewPassAgn
	"\x9F\xB7\xF4\xEA\x20\xAF\xED\xA5\xED\xE6\x20\xB1\xDE\x20\x98\x93\xA8",         // NoPrinter
	"\xAF\xBA\x20\xE4\xF4\xE7\xDB\x9D\x20\xAF\xAF\xA5\xE6\x20\x98\x93\xA8",         // ReprintComplete
	"STAN NUMBER MISSING",															// InvNumberMissing //Note:  Must be re-translated shorter.
	"\x98\x93\xA8\x20\xE1\x93\xAA\x20\xB1\xAF",                                     // PrintingWait
	"\xB1\x91\xB1\xD6\xB1\x95\x20\x93\xEF\xB1\xED\xB1\xB6",                         // ServerOn
	"\xB1\x91\xB1\xD6\xB1\x95\x20\x93\xEF\xB1\x91\xAF\xD6\xED\xAF\xEB\xBE",			// CashierOn
	"\xAF\xF4\xE3\xDA\x20\xF1\xAA\xD3\xBE\x20\x9F\xB7\x9D",                         // KeyBrdTest
	"CANCEL TWICE TO END",															// Cancel2End //Note:  Must be re-translated shorter.
	"\x3D\xAF\xF4\xE3\xDA",                                                         // DispKeyP
	"\xE5\xB2\x93\xE2\x20\xF1\xF3\xED\xB7\x9D",										// SettlementRequired
	"\x20\x9C\xB1\x93\xDA\x20\xE9\xAF\xEA\x91\xED\xAD",                             // CardRdT
	"\xE1\x93\xCB\xD2\x20\xB1\xDF\x99\x93\xA8",										// PrinterEP
	"\xE1\x93\xCB\xD2\xB1\xF4\xCE\x20\xB1\xDF\x99\x93\xA8",							// PrinterDP
	"\xF9\xAF\xED\xBA\x20\xE1\x93\xCB\xD2",                                         // EnablePP
	"\xF9\xAF\xED\xBA\x20\xE1\x93\xCB\xD2\xB1\xF4\xCE",                             // DisablePP
	"",                                                                             // ScanningBatch
	"",                                                                             // NoBatchTotals
	"",                                                                             // NoMoreTotals
	"\xB1\xDF\x99\x93\xA8\x20\x9F\xB7\x9D",                                         // TestPrnC
	"\xE9\x93\xF3\x93\x99",                                                         // Closed
	"ENTER DATE MMDDYY",															// DateMDY //Note:  Must be re-translated shorter.
	"ENTER TIME HHMM",																// TimeHM //Note:  Must be re-translated shorter.
	"",																				// HostBatch
	"\x9F\xB7\xF4\xEA\x20\xAF\xED\xA5\xED\xE6\x20\xEE\xAF\xEA\xB1\xF3\xB0\x99",     // NoAcq
	"\x3A\xF5\x91\x20\xF1\x9E\xB6\xAF\x20\x9C\x93\xF4\xE3\xE7\xCA\x20\xC9\xED\xB1\xBA",// StartOfBatch
	"",                                                                             // EndOfBatch
	"",																				// SelectShiftNum
	"",                                                                             // ZeroForAllShift
	"\x93\x9D",                                                                     // ReportMenuTill
	"\xB1\x91\xAF\xD6\xED\xAF\xEB\xBE",                                             // ReportMenuCashier
	"\xB1\xED\xB1\xB6",                                                             // ReportMenuServer
	"\xF5\xB6\xB1\x95\x93\xB7\xAA",                                                 // ReportMenuAudit
	"\xF1\xBE\x93\xE3\xAD",                                                         // ReportMenuSummary
	"",                                                                             // ReportMenuOpenTabs
	"1=DETAIL 3=UNADJUSTD",															// CashServMenuLine1 //Note:  Must be re-translated shorter.
	"\x32\x3D\xAE\xE2\x93\xBF\xEF",                                                 // CashServMenuLine2
	"\x9F\xB7\xF4\xEA\x20\xB2\x93\xA5\xE6\x20\xF5\xE2\x93\xAD",                     // VoidNotAllowed
	"ENTER HOST NUMBER",															// HostNum //Note:  Must be re-translated shorter.
	"\xF9\x20\xC9\xED\xEA",                                                         // Type
	"\x9F\xB7\xF4\xEA\x20\xB2\x93\xA5\xE6\x20\xAB\xF4\xAA\xBF\x9D",					// AdjustNotAllowed
	"RE-TRY BATCH TRANS",															// RetryBatchTrans //Note:  Must be re-translated shorter.
	"\xCC\xE7\xA5",                                                                 // AmountTotals
	"\xFF\xAF\xF3\xAF\xA5\x20\xD0\xE3\x96\xE6",                                     // AmountNew
	"ENTER RECEIPT TEXT",															// RecieptTXT //Note:  Must be re-translated shorter.
	"ENTER APPROVAL CODE",															// EnterApp //Note:  Must be re-translated shorter.
	"\x2A\x2A\x2A\x20\xF2\xB1\xF4\xDE\xAF\x93\xF3\x20\x9F\xE2\x93\xAA\x20\x2A\x2A\x2A",// TrainingMode
	"SWITCH TO TRAIN MODE",															// GoToTrainingMode //Note:  Must be re-translated shorter.
	"\xF2\xB1\xF4\xDE\xAF\x93\xF3\x20\x9F\xE2\x93\xAA\x20\xB2\x91\x20\xA4\xED\xB1\xAD",// LeaveTrainingMode
	"\x9F\xB7\xF4\xEA\x20\xB2\x93\xA5\xE6\x20\x9F\xAD\x91\xAF\xB1\x99\xB2\x93\x95",	// RefundNotAllowed
	"\xB4\xB9\xED\xB1\xD2\x20\xCC\xE7\xA5",                                         // SalesTotal
	"\xC6\xE3\xCE\x20\xB4\xB9\xED\xB1\xD2\x20\xCC\xE7\xA5",							// SalesTotalsWrong
	"\x9F\xAD\x91\xAF\xB1\x99\xB2\x93\x95\x20\xCC\xE7\xA5",                         // RefundTotal
	"\xC6\xE3\xCE\x20\x9F\xAD\x91\xAF\xB1\x99\xB2\x93\x95\x20\xCC\xE7\xA5",			// RefundTotalsWrong
	"\xE4\xDA\x20\xCC\xE7\xA5",                                                     // BatchTotals
	"",                                                                             // BatchRelease
	"\xF5\xBB\xF3\x93\xE6\xB2\x92\x20\xB4\xBC\xEB\xDA\x91\xB1\x9D",                 // TestTransactions
	"\xAF\xB1\x91\xAF\xEA\x20\xAF\xED\xA5\xED\x20\xF5\xBB\xEB\xDA\x91\xB1\x9D",     // NoTransactions
	"\xB4\xBC\xF3\x91\xB2\xD2\x91",                                                 // IncrementShift
	"\xAF\xAF\xA5\xE6\x20\xF2\xB2\x91\xAF\xEA\x91\x20\xEE\x91\xB1\x2D\x9F\xB7\xF3\xB1",      // ResetShift
	"\xC5\x93\x96\x9D\xB1\x91\x20\xE9\xED\xAF\x95",                                 // NoComms
    "\xCC\xC6\xD2\x20\xEC\xD3\xE3\x9D\x20\xC6\xAD",									// NoPhoneline
	"\xE5\x93\xCB\xEA\x91\x20\xE9\x91\xB2\xF4\xE6\x20\xAF\xED\xB1\xED",             // EnterTip
	"\xCC\xE7\xA5\x20\xAF\xED\xB1\xED",                                             // EnterTotal
	"1-OPEN TAB 3-TAB RPT",															// TabProcMsg1 //Note:  Must be re-translated shorter.
	"\xE9\xED\x9E\xB6\x20\xEC\x9E\xB7\x95\x20\x32\x2D",                             // TabProcMsg2
	"\xB1\xED\xB1\xB6\x20\xEE\xB1\x93\xE7\xBA\x20\xAF\xED\xB1\xED",					// ServerNum
	"\xB2\x93\x95\x20\xF2\x93\xF0\xEA\xED\x9E\xB6\x20\xF1\xE7\xEF\x20\xEC\x9E\xB7\x95",// CloseOpenTabs
	"",																				// NotOpenTab
	"\xE8\xF4\xC8\xEB\x9D",                                                         // N_Configure 
	"\xF2\xB2\x91\xAF\xEA\x91\x20\xEE\x91\xB1",                                     // N_Initialize
	"\xF5\xE3\xBE\x91\x20\xF2\xED\xEB\xE6",											// N_Main
	"\xB4\xBC\xEB\xDA\x91\xB1\x9D",                                                 // N_NewTrans
	"\xF2\x91\x20\xF1\x9E\xB6\xAF\x20\x9C\x93\xF4\xE3\xE7\xCA",                     // N_Batch
	"\x9C\x93\xBA\xB1\x91\xB2\xDE",                                                 // N_Reports
	"\xF1\xEA\x93\xF3\x93\x99",                                                     // N_Terminal
	"\xB1\x9E\xBB\xF4\x95",                                                         // N_More
	"\xED\xEB\xE6",                                                                 // N_Menu
	"DATE/TIME",																	// N_Datime
	"\xB1\xDF\x99\x93\xA8",                                                         // N_Printer
	"\xC5\x93\x96\x9D\xB1\x91",                                                     // N_Comms
	"",                                                                             // N_Dial
	"\xB2\xE6\xB1\x20\xF1\xE7\xE3\xDA",                                             // N_Passwords
	"\xCC\xF3\xB1\xB6\x20\xF2\xB1\xF4\xDE\x20\xEE\xB1\x93\xE7\xBA",                 // N_Speedial
	"\x20\x9C\xB1\x93\xDA\x20\xE9\xAF\xEA\x91\xED\xAD",                             // N_CardRdr
	"\x9F\xB6\x91\xB1\x9E\xEB\xDA\x20\xE9\x91\xB2\xF4\xE6",                         // N_Contras
	"\xB4\xB9\xED\xB1\xD2",                                                         // N_Sale
	"\xAF\xD7\xEA\x2B\xB4\xBC\xED\xB1\xD2",                                         // N_SalCash
	"\xF5\x9E\xB7\x99\x20\xB4\xB9\xB1\x93\xD3\xB6",                                 // N_MailOrder
	"\xE4\xBF\x9E\xE6\x20\xB1\xF4\xCE",												// N_Force
	"\xAF\xF4\xF4\xF8\x9D",                                                         // N_Auth
	"\x9C\xB1\x93\xDA\x20\xAF\xF4\xF4\xF8\x9D",                                     // N_CardVer
	"\xB1\xED\xB1\xE6",                                                             // N_Review
	"\xED\xA5\x9E\xB7\xA5",                                                         // N_Find
	"\xE8\xF4\xC8\xEB\x9D",                                                         // N_Adjust
	"\xF1\xF4\xD7\x95",                                                             // N_Void
	"\xAF\xD7\xEA",                                                                 // N_Cash
	"\xF1\xE6\x93\xEA\xB1\x95\x20\xF2\xB1\x91\xB0\xDE\xB1\x93\x95",                 // N_ProgLoad
	"\xF5\xF3\x93\xA5\x95\x93\xA5",                                                 // N_Shift
	"\xB4\xBC\xF3\x93\xE7\xEA",                                                     // N_View
	"\xF2\xB1\xF4\xDE\xAF\x93\xF3",                                                 // N_TrainMode
	"\xE8\xF4\xC8\xEB\x9D",                                                         // N_Setup
	"\xF5\x9D\x93\xF4\xE3\xE7\xCA",                                                 // N_Function
	"\x93\xF0\x9E\xB7\x9D",                                                         // N_Tests
	"\xE9\xED\x9E\xB6",                                                             // N_Tab
	"\xAF\xAF\xA5\xE6\x20\x98\x93\xA8",                                             // N_Reprint
    "EXIT",																			// N_Exit
	"\x93\xC6\xAD",                                                                 // N_Error
	"\xE9\xAF\xED\xE7\xEA\x20\xD9\x93\x99",                                         // N_Clear
	"CLEAR CASHIER TABLE",															// N_ClearChsTbl //Note:  Must be re-translated shorter.
	"\xAF\xED\xBA\x20\xD9\x93\x99\x20\xB1\xED\xB1\xB6\x20\xE1\xED\xAF\xA5",			// N_ClearSerTbl
	"\xAF\xED\xB1\xED",                                                             // N_Enter
	"\xED\xCF\xE2",                                                                 // N_Cancel
	"\xAF\xAF\xA5\xE6\x20\xF2\xB2\x91\xAF\xEA\x91\x20\xEE\x91\xB1\x2D\x9F\xB7\xF3\xB1",      // N_Reset
	"\xCC\xF3\xB1\xB6",                                                             // N_Quick
	"\xE8\xDA\x2D\xF1\xD2\x93\xC2\x91",                                             // N_AddRmv
	"\x9C\x93\xF4\xFB\xB2\xA5",                                                     // N_Detail
	"\xF1\xBE\x93\xE3\xAD",                                                         // N_Summary
	"",                                                                             // N_Unadjusted
	"\x54\x53\x4F\x48\x2D\xB2\xDA\xB1\xE6",                                         // N_Host
	"\xB4\xBC\xF3\x93\xE7\xEA",                                                     // N_Display
	"\xAF\xF4\xE3\xDA\x20\xF1\xAA\xD3\xBE",                                         // N_Keyboard
	"\x50\x49\x4E\x50\x41\x44",                                                     // N_Pinpad
	"",                                                                             // N_InitPhone
	"NMS PHONE",																	// N_NMSPhone //Note:  Must be re-translated shorter.
	"\xF2\xB1\xF4\xDE\x20\xEE\xB1\x93\xE7\xBA\x20\x9F\xE2\x93\xAA",                 // N_DialType
	"",                                                                             // N_PABX
	"\xF1\xEA\x93\xF3\x93\x99\x20\xF1\xB6\x93\xEB\xBA\x20\xAF\xDA",                 // N_TermId
	"\xF1\xE7\xEF",                                                                 // N_All
	"\xB4\xB9\xED\xE6\x93\xAD\x2D\xEC\xBA\xED\xB1",                                 // N_OnOff
	"\xB4\xC4\x96\xD6\x2D\xAF\xF4\xB6\xB1",                                         // N_Receipt
	"\xEC\x9E\xE6",                                                                 // N_Text
	"\xB0\xCE\x93\xDA\x20\xF1\xF3\xB0\xCF\x9D",                                     // N_PaperFeed
	"\xA4\xED\xB1\xAD",                                                             // N_Quit
	"\xB1\xF4\xF4\xCF\x9D",                                                         // N_Change
	"",																				// N_NullStr
	"\x9C\x93\xC5\x93\x96\x9D\xB1\x91",                                             // N_Communications
	"\xB1\xED\x96\xCA\x20\xF1\xE7\xE3\xDA\x20\xB1\xF4\xF4\xCF\x9D",                 // N_PswdChange
	"\xB1\x9D\x93\xE2\x93\x95",                                                     // N_Higher
	"\xB1\x9E\xEB\xF4\xF3\x93\x99",                                                 // N_Lower
	"\xF5\xE3\xAD\x91\xAF\x20\x9C\x93\xE7\xF4\xC8\xEB\x9D",                         // N_LocalOpts
	"\xAE\xB6\x93\x99",                                                             // N_Response
	"\xE9\xAF\xB1\xDA\xB2\x93\x95",                                                 // N_Open
	"\xEC\x9E\xB7\x95",                                                             // N_Close
	"\x98\x93\xA8",                                                                 // N_Print
	"\xE9\xAF\xEA\x91\xB2\xCF\xE2",                                                 // N_Scroll
	"",                                                                             // N_EnterFunction
	"\x48\x4F\x53\x54\x20\xB2\xDA\xB1\xE6\x20\xB4\xB9\xB1\x91\xB2\xDE",             // N_PrintHost
	"\x93\xEF\x20\xEE\xAF\x91\xAF\x20\xDC\xEA\x93\x95\x20\xB4\xB9\xB1\x91\xB2\xDE", // N_DBRpt
	"\x9F\xD2\x93\xF3\xB1\xAF\x2D\xE4\x93\xD3\x9E\xEA\x91\x20\xB4\xB9\xB1\x91\xB2\xDE",   // N_TxRxRpt
	"\xE1\xED\xAF\xA5\x20\xB4\xBC\xF3\x93\xE7\xEA",                                 // N_ViewTable
	"\xB1\xDF\xBB\xF3\x93\xE7\xEA\x20\x9F\xB7\x9D",                                 // N_DisplayTest
	"FUNCTION NOT SUPPORTED",														// N_FuncNotSupported //Note:  Must be re-translated shorter.
	"\xF1\xAD\xB7\xEA",                                                             // N_Version
	"\xB1\xF4\xAD\x91\x20\xAE\xB6\x93\x99",                                         // N_LastRsp
	"\xB1\xF4\xAD\x91\x20\xAE\xB6\x93\x99",                                         // N_Last
	"\xAF\xB1\x91\xAF\xEA\x20\xAF\xED\xA5\xED\x20\xEE\xAF\x91\xAF",                 // N_NoData
	"",                                                                             // N_BatchInfo
	"\x3A\x54\x53\x4F\x48\xB2\xDA\xB1\xE6\x20\xF1\xB6\x93\xEB\xBA\x20\xAF\xDA",     // N_HostId
	"\x3A\x54\x53\x4F\x48\xB2\xDA\xB1\xE6\x20\xE5\x93\xEA",                         // N_HostName
	"\x3A\xF1\x9E\xB6\xAF\x20\xEE\xB1\x93\xE7\xBA",                                 // N_BatchNumber
	"\x93\xEF\xB2\xDA\xB1\xE6\x20\xF1\xE7\xEF\x20\xF2\x91\xB1\x95\x20\x30",         // N_ZeroForAllHost
	"\xB4\xB9\xB1\x91\xB2\xDE",                                                     // N_Report
	"\xB1\x93\xDB\xEF\xAF\x95",                                                     // N_Debit
	"\xB1\x93\xDB\xEA\x93\x9E\xB7\x95",                                             // N_Credit
	"\xAF\xF4\xF4\xF8\x9D\x20\xAF\xDA\x20\xAF\xED\xB1\xED",                         // N_EnterAuth
	"",                                                                             // N_EnterGenNum
	"",                                                                             // JournalTesting
	"ENTER NUM OF ENTRIES",															// NumEntries //Note:  Must be re-translated shorter.
	"\xB0\xCE\x93\xDA\x20\xF5\xA5\xED\xB1\xAD",                                     // PaperOut
    "\x93\xE2\x93\x95\x20\xB1\xDF\x99\x93\xA8\x20\xF2\x93\xE6\xAF",					// PrinterOverheat
	"\x94\x93\xB7\xAA\x20\xEE\xB1\x93\xE7\xBA\x20\xB1\xC8\x9E\xEB\xE6",				// WaitingForAcct
	"\xAF\xF4\xF4\xF8\x9D\x20\xB1\xC8\x9E\xEB\xE6",									// WaitingForApproval
	"\x50\x49\x4E\x20\xEE\xB1\x93\xE7\xBA\x20\xB1\xC8\x9E\xEB\xE6",                 // WaitingForPin
	"SWIPE CARD ON PINPAD",															// SwipeCardPin //Note:  Must be re-translated shorter.
	"CHANGE TO PIN READ?",															// ChangeToPinRead //Note:  Must be re-translated shorter.
	"CHANGE TO TERM READ?",															// ChangeToTermRead //Note:  Must be re-translated shorter.
	"\xB1\x96\x9E\xCB\xE6\x93\xEA\x20\x94\x93\xB7\xAA",                             // InvalidAccount
	"\xEE\xAF\xEA\x93\xE6",                                                         // AvailBal
	"     TODAY",																	// Today
	"CARD # DOESN'T MATCH",															// CardNoMatch //Note:  Must be re-translated shorter.
	"\x50\x49\x4E\x50\x41\x44\x20\x9F\xB7\x9D",                                     // PinPadTest
	"PINPAD AUTO DETECT",															// PinPadAutoDetect //Note:  Must be re-translated shorter.
	"\x94\x93\xB7\xAA\x20\x94\x93\xAD\x9E\xEA\x91",                                 // SelectAccount
	"\x94\x93\xB7\xAA",                                                             // N_Account
	"\x30",                                                                         // N_0
	"\x31",                                                                         // N_1
	"\x32",                                                                         // N_2
	"\x33",                                                                         // N_3
	"\x34",                                                                         // N_4
	"\x50\x49\x4E\x20\xEE\xB1\x93\xE7\xBA\x20\xAF\xED\xB1\xED",                     // EnterPin
	"\x9F\xB7\xF4\xEA\x20\xAF\xED\xA5\xED\xE6\x20\x50\x49\x4E\x50\x41\x44",			// ExtPinNot
	"\xF2\xAF\xEB\x95\xB2\xE6\xB1\x20\xF2\x93\xC6\xAD",                             // EncryptError
	"\xAF\xED\xB1\xED",                                                             // N_Logon
	"\xF2\xB1\x91\xAF\xF0\xDF\xEA",                                                 // MaintFunction
	"\xF5\xE3\xBE\x91\x20\xAE\xF3\xB1\x93\x9D\x20\xAF\xED\xB1\xED",					// N_OrigDate
	"\xF5\xF3\x93\xB6\x93\xEB\xBA",                                                 // N_Identification
	"\xB1\x91\xB2\xD2\x91\x20\xE5\xB1\xEA",                                         // N_Software
	"",                                                                             // N_DLL
	"\xB2\x91\xAF\xEA\x91\x20\xEE\x91\xB1",                                         // N_EPROM
	"\xC9\xED\xA5\xB1",                                                             // N_Referral
	"\x48\x4F\x53\x54\x20\xB2\xDA\x91\xB1\xE6\x20\xE5\x93\xE7\x9D",                 // N_AllHosts
	"",                                                                             // N_IcePac
	"ENTER FOR ALL",																// PressEnterForAll //Note:  Must be re-translated shorter.
	"\xB1\xAD\x92\x20\xB1\xED\x9E\xDA\x93\xD2\x20\xF2\x91\xB1\x95",                 // ForLastInv
	"\xF5\xEA\x93\xE2\xED\xC5\x20\xE9\x93\xE6\xB2",                                 // N_Timeout
	"\xAF\xED\xE7\xEA\x20\xED\xCF\xE2\x20\xB1\x95\xB1\x93\xDA",                     // N_UserCancelled
	"\xB4\xB5\xED\xDB\xCB\xE6",                                                     // N_Reversals
	"\xAF\xBB\xEA\x20\x9F\xD2\x93\xF3\x20\x50\x49\x4E\x50\x41\x44",                 // PinpadNotFound
	"\xF5\xE3\x95",                                                                 // N_YES
	"\xB1\xF4\xAD",                                                                 // N_NO
	"\xE1\xED\x96\xD6",                                                             // N_OK
	"\xA4\xED\xB1\xAD",                                                             // N_QUIT
	"\xF2\xAF\xCB\x95",                                                             // N_NEXT
	"SWIPE OR INSERT CARD",															// N_SwipeOrInsertCard
	"X",																			// N_X
	"\xF1\xE6\x91\xAF\x91",                                                         // N_Continue
	"\xAF\xF4\xE3\xDA",                                                             // N_KEY
	"\xD8\xD2\xED\xE6",                                                             // N_Passed
	"\xD8\xD2\xED\xE6\x93\xEA",                                                     // N_Failed
	"\xB4\xBC\xF3\x93\xE6\xB2\x92",                                                 // N_Test
	"\xEC\x9D",                                                                     // N_Tone
	"\xB4\xB8\xE2\x93\x99",                                                         // N_Pulse
	"\x50\x49\x4E\x50\x41\x44\x20\xE9\xAF\xEA\x91\xED\xAD\x20\xF2\x93\xC6\xAD",		// PinReadErr
	"\x50\x49\x4E\x50\x41\x44\x20\xB4\xB9\xB1\x93\xDF\xEA\x20\xF2\x93\xC6\xAD",		// PinWriteErr
	"\xF9\xA5\xED\xB1\xAD",                                                         // SignOffYN
	"\xF1\xF3\xED\xEA\x91\xB3",                                                     // Month_1
	"\xF1\xF3\xB1\xED\xD2",                                                         // Month_2
	"\xB4\xB5\xB1\x93\xE6",                                                         // Month_3
	"\xE4\xF3\xB1\xED\x92",                                                         // Month_4
	"\xF5\xE6",                                                                     // Month_5
	"\xF1\xF4\xFB\xED\xB3",                                                         // Month_6
	"\xF2\x93\xE2\xED\xB3",                                                         // Month_7
	"\x9F\xB6\xED\xDF\x92",                                                         // Month_8
	"\xB1\x96\xE6\x93\x9E\x9A\xB6",                                                 // Month_9
	"\xB1\x96\x9E\xDA\x91",                                                         // Month_10
	"\xB1\x96\xE6\x91\xED\xEA",                                                     // Month_11
	"\xB1\x96\xE6\x93\xB6\xAF",                                                     // Month_12
	"REENTER PIN",																	// Msg_Reenter_Pin //Note:  Must be re-translated shorter.
	"\xF9\x20\x50\x49\x4E\x20\xAF\xDA",                                             // Msg_Pin
	"\x3A\xF5\xA5\xB1\x93\xAD",                                                     // Msg_External
	"\x3A\xF5\xE3\xAD\x91\xAF",                                                     // Msg_Internal
	"\x43\x48\x45\x43\x4B\x53\x55\x4D",                                             // Msg_Chksum
	"\xF9\xAA\xF4\xAA\xBE",                                                         // Msg_Correct
	"",                                                                             // Msg_SVR
	"\x3A\xAF\xD7\xEA",                                                             // Msg_CSH
	"\x3A\xB1\xED\x9E\xDA\x93\xD2",                                                 // Msg_INV
	"\xB4\xB5\xED\xDB\xCB\xE6",                                                     // Msg_reversal
	"",                                                                             // Msg_voided
	"\xF1\xF4\xBE\xED\x9D",                                                         // Msg_advice
	"\x3A\x97\xF4\x9D\xB1\x9D\x20\xEE\xB1\x93\xE7\xBA",                             // Msg_SequenceNo
	"\xAF\xB1\x91\xED\xE6",                                                         // Msg_Items
	"\xB4\xB9\xED\xB1\xD2",                                                         // Msg_Sales
	"\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x2A\x20\x93\xC6\xAD",                     // Msg_Error
	"\xAF\xED\xA5\xED\xE6\x20\xB2\xDA\x91\xB1\xE6",                                 // Msg_AvailHosts
	"\x3A\xAF\xD7\xEA",                                                             // Msg_Cash
	"",                                                                             // Msg_Tender
	"\xF2\xB1\x93\xF4\x9E\xAD\x91\x20\xE9\x91\xB2\xF4\xE6",                         // AmountOpt
	"",                                                                             // N_CVV2Indicator
	"\x30\x3D\x9B\xB1\xBA",                                                         // N_CVV2IndEntry1
	"\x31\x3D\xAB\x93\xC2\xB1",                                                     // N_CVV2IndEntry2
	"",                                                                             // N_CVV2IndEntry3
	"\x39\x3D\xD0\x93\xF4\x95",                                                     // N_CVV2IndEntry4
	"",                                                                             // N_CVV2Entry
	"\xF9\x9E\xB6\x91\x20\xAF\xED\xA8\xED\xE6\x20\x9C\xB1\x93\xDA",                 // Msg_CardPresent
	"\x34\x20\xE9\x91\xED\xEB\xCA",                                                 // Msg_HeaderLine
	"\xF5\xD6\xB1\xED\x93\x99",                                                     // Msg_FooterLine
	"\xAF\xF4\xBB\xDB\x95\x20\x91\xB1\x20\x9C\xB1\x93\xDA",                         // Msg_SwipeYourCard
	"\xE5\x93\xCB\xEA\x91\x20\xF1\xEB\xF3\xB2\xDE\x20\x94\x93\xAD\x9E\xEA\x91",     // Msg_PreTip
	"\xF1\xF4\xD7\x95",                                                             // Msg_Other
	"\x3A\xF1\xE6\x93\xEA\xB1\x95\x20\xF2\x93\xC6\xAD",                             // Msg_ProgramError
	"\x93\xF0\xE6\x93\xCB\xEA\x91",                                                 // Msg_Tips
	"TIP    %",																		// Msg_TipPct
	"SUGGESTED TIP PERCENT",														// Msg_Sug_Tip //Note:  Must be re-translated shorter.
	"",                                                                             // Msg_LocalOpt
	"TX/RX ERRORS",																	// Msg_TxRxErrors //Note:  Must be re-translated shorter.
	"\x3A\xE4\x93\xD7\x9E\xEA\x91\x20\xF2\x93\xEF\x93\xC6\xAD",                     // Msg_TranErrors
	"\x3A\x9F\xD2\x93\xF3\xB1\xAF\x20\xF2\x93\xEF\x93\xC6\xAD",                     // Msg_RecvErrors
	"\xE4\xF3\x93\xD2\x20\x9F\xD2\x93\xF3\xB1\xAF",                                 // N_FileDownLoad
	"\xF1\xF3\xED\xB7\x9D",                                                         // N_Settle
	"\xE4\xBF\x9E\xE6\x20\xB1\xF4\xCE",                                             // N_Offline
	"\xB4\xBC\xEB\xDA\x91\xB1\x9D",                                                 // N_Trans
	"\x9F\xCA\x93\xB6\x2D\xAE\xF3\xB1\x93\x9D\x20\xE8\xF4\xC8\xEB\x9D",             // N_DateTimeSetup
	"\x9F\xB6\x91\xB1\x9E\xEB\xDA\x20\xE9\x91\xB2\xF4\xE6",                         // N_Contrast
	"\xB0\xCE\x93\xDA\x20\xF1\xF3\xB0\xCF\x9D",                                     // N_PaperFd
	"\xF1\xF3\xED\xB7\x9D\x20\x9F\xF4\xCB\xC2\xED",                                 // N_SettleStat
	"\xF1\xBE\x93\xE3\xAD\x20\xF1\xEB\xF3\xB2\xDE\x20\x94\x93\xAD\x9E\xEA\x91",		// Msg_ChooseSummary
	"\xDC\xF4\x9D\x93\xE6\xED\x9D\x91",                                             // Msg_Auto
	"\xEE\xED\xB1\xDE",                                                             // Msg_Group
	"\xF5\x9E\xB6\xAF",                                                             // Msg_Manual
	"\xB2\xDE\xB1\xEF",                                                             // Msg_Never
	"\xB4\xB9\xED\xB1\xD2\x20\xAF\xB1\x91\xED\xE6",                                 // Msg_SaleItems
	"\x9F\xAD\x91\xAF\xB1\x99\xB2\x93\x95\x20\xAF\xB1\x91\xED\xE6",                 // Msg_RefundItems
	"\xEE\xB1\x93\xE7\xBA\x20\xF1\xCB\xD2\xAF",                                     // Msg_Attempt
	"\xAE\xB6\x93\x99",                                                             // Msg_Resp
	"\xF1\xE7\xEF",                                                                 // Msg_Evrythg
	"\xF1\xE7\xEF",                                                                 // Msg_Everything
	"\xF5\xE2\x93\xAD\x20\xB1\xED\xB1\xB6\x20\xE1\xED\xAF\xA5",						// Msg_SrvTabEmpty
	"\xF5\xE2\x93\xAD\x20\xB1\x91\xAF\xD6\xED\xAF\xEB\xBE\x20\xE1\xED\xAF\xA5",		// Msg_CshTabEmpty
	"\x9F\xF4\xCB\xC2\xED",                                                         // Msg_Status
	"\xE4\xF3\x93\xD2\x20\xE9\xAF\xB1\xDA\xB2\x93\x95\x20\xF2\x93\xC6\xAD",         // Msg_fopenerror
	"\x3A\xE4\xF3\x93\xD2\x20\xE1\x93\xD7\x9E\xEA\x91",								// Msg_DownloadingFile
	"\xE9\x93\x95\xB2\x20\x94\x93\xAD\x9E\xEA\x91",                                 // Msg_SelectLanguage
	"\xE9\x93\x95\xB2",                                                             // Msg_Language
	"\xF5\xB7\xF4\xE3\xDF\xEA\x91",                                                 // Lng_English
	"GERMAN",																		// Lng_German
	"SWEDISH",																		// Lng_Swedish
	"ESPANOL",																		// Lng_Spanish
	"PORTUGUESE",																	// Lng_Portugues
	"\xF5\xE3\xCB\xD2\x20\xE9\x93\x95\xB2\x20\xE1\xED\x96\xD6",						// Msg_CurrentLang
	"\xE9\x93\x95\xB2",                                                             // Msg_Lang
	"\xB1\x96\x9E\xCB\xE6\x93\xEA\x20\x93\xEF\xAF\xF4\xE3\xDA\x20\xE1\xED\xAF\xA5", // Msg_KeyTableInvalid
	"DATABASE REPORT\n",															// Prt_DatabaseReport
	"---------------\n",															// Prt_Dashes
	"TERMINAL EPROM CONFIG\n",														// Prt_TermEpromCfg
	"\xF1\xEA\x93\xF3\x93\x99\x20\xE8\xF4\xC8\xEB\x9D",								// Prt_TermConfig
	"\x3A\xF1\xAF\xEA\xB1\xF3\xB0\x99\x20\xF1\xB6\x93\xEB\xBA\x20\xAF\xDA",				// Prt_AcquirerID
	"\x3A\xF1\xAF\xEB\xEB\xDA\x20\xB1\xAF\x93\xBE\x20\xF1\xB6\x93\xEB\xBA\x20\xAF\xDA", // Prt_IssuerID
	"\x3A\x9F\xB1\x93\xDA\x20\xEE\xAF\xED\xAF\xAA\xE6\x20\xAF\xDA",                 // Prt_CardRangeID
	"\x3A\xB1\xDE\x20\xD4\xF4\xBE\xED\x9D\x20\xAF\xDA",                             // Prt_DescriptorID
	"\x3A\xF1\xAF\xBA\x20\x9F\xD2\x93\xF3\xB1\xAF\x20\xF2\x93\xEF\x20\xEE\xAF\x91\xAF",// Prt_ReceivedData
	"\x3A\xF1\xAF\xBA\x20\xE1\x93\xB6\xB1\x91\x20\xF2\x93\xEF\x20\xEE\xAF\x91\xAF",    // Prt_SendData
	"RAW DATA (HEX BYTES):\n",														// Prt_RawDataHexBytes //Note:  Must be re-translated shorter.
	"\xF1\xDB\x96\xBA\x20\xE4\x95\x93\xDA\x20\xB1\xC8\x9E\xEB\xE6",					// WaitingForLanCable
	"\xF5\xEB\xD3\xE3\x9D\x20\x9F\xE2\x93\xAA\x20\xED\xCF\xE2",						// LineModeCanceled
	"Restart terminal",																// RestartTerminal //Note:  Must be re-translated shorter.
	"ONLY 0-9 A-F VALID",															// N_Only09AFvalid //Note:  Must be re-translated shorter.
	"",																				// N_IdleButton
	"\xF1\xE7\xDE\xAF\x20\x94\x93\xAD\x9E\xEA\x91",                                 // SelectButton
	"\xD5\x93\xD3\x9D\x91\x20\xC9\xED\xEA\x20\x94\x93\xAD\x9E\xEA\x91",             // EventType
	"\xF1\xE7\xDE\xAF",                                                             // N_Button
	"\xB2\x93\xA5\xE6\x20\xB4\xBC\xEB\xDA\x91\xB1\x9D",								// ConfirmTrans
	"",                                                                             // ConfirmFunc
	"",                                                                             // N_InitButTab
	"",                                                                             // Init_Dis
	"",                                                                             // Init_Ena
	"",                                                                             // N_IdlBtn
	"",                                                                             // N_InitIdlBtn
	"\xB1\xF4\xF4\xCF\x9D\x20\xF1\xE7\xDE\xAF",                                     // N_ChgIdlBtn
	"",																				// Chang_Btn1
	"",																				// Chang_Btn2
	"",																				// Chang_Btn3
	"",																				// Chang_Btn4
	"\x31\x20\xF1\xE7\xDE\xAF",                                                     // B_1
	"\x32\x20\xF1\xE7\xDE\xAF",                                                     // B_2
	"\x33\x20\xF1\xE7\xDE\xAF",                                                     // B_3
	"\x34\x20\xF1\xE7\xDE\xAF",                                                     // B_4
	"\xEE\xAF\xEA\xB1\xF3\xB0\x99\x20\xF5\x9A\xDA\x20\xEC\x9E\xBA\x91\xAF\xB1\x95",	// RemoveMerchantCopy
	"NO FIELD TABLE TO PRINT",														// PclTabError //Note:  Must be re-translated shorter.
	"\xB1\x93\xDA\x20\x9F\xF4\xD3\xF4\xDA",                                         // Prt_PerformBtn
	"\xE8\xF4\xC8\xEB\x9D",                                                         // Prt_ConfBtn   
	"\x9C\x93\xE7\xF4\xC8\xEB\x9D\x20\xB4\xB9\xB1\x91\xB2\xDE",						// Config_RPT
	"\xF1\xEA\x93\xF3\x93\x99",                                                     // Terminal_Tab 
	"\xEE\xAF\xEB\xEB\xDA\xB1\xAF\x93\xBE",                                         // Issuer_Tab 
	"\xEE\xAF\xEA\xB1\xF3\xB0\x99",                                                 // Acq_Tab 
	"\x9C\xB1\x93\xDA",                                                             // Card_Tab 
	"\xF1\xE7\xEF",                                                                 // All_Tab
	"\xF1\xEA\x93\xAD\x95\x93\x9E\xDA\x20\xF1\xAD\xB7\xEA",                         // N_LibsVersion
	"SDK",																			// N_SDKVersion //Note:  Must be re-translated shorter.
	"\xE4\xE6\x93\xCA\x20\xE8\x9E\xB7\xF4\xB6\x20\xF1\xAD\xB7\xEA",                 // N_OSVersion
	"\xE4\xE6\x93\xCA\x20\xE8\x9E\xB7\xF4\xB6\x20\xF1\xAD\xB7\xEA",                 // N_OSRevision
    "HFC",																			// N_HFCVersion
	"\x9C\x93\xF4\xE3\xE7\xCA\x20\xF1\xA8\xAD\xF3\xB1\x93\x9D\x20\xB4\xB9\xB1\x91\xB2\xDE",// History_RPT
	"",                                                                             // Msg_freaderror
	"Entry # Exceeds Max",															// N_EntryNumExceedsMax //Note:  Must be re-translated shorter.
	"\x3A\xE4\xED\xAF\xA5\x20\xAF\xDA",                                             // N_TableId
	"",                                                                             // N_HFS
	"No enougth space in HFS",														// N_NoEnougthSpaceInHFS //Note:  Must be re-translated shorter.
	"\xB1\xB2\xF3\x91\xED\xB1\x99\xED\xB6\x20\xB2\xE6\xB1",							// SVPassword
	"",																				// CallAuthCentre
	"\xF9\x20\xAF\xF4\xF4\xF8\x9D\x20\xAF\xDA",										// AuthCode
    "CASH NOT ALLOWED",																// N_CashNotAllowed //Note:  Must be re-translated shorter.
	"\x9F\xB7\xF4\xEA\x20\xB2\x93\xA5\xE6\x20\x9C\xB1\x93\xDA\x20\xE9\xAF\xF4\xBB\xDA",// SwipeNotAllowed
	"\x9F\xB7\xF4\xEA\x20\xB2\x93\xA5\xE6\x20\xB4\xBC\xEB\xDA\x91\xB1\x9D",			// TranNotAllowed
    "",																				// InitMaxTries
	"\x93\xE2\x93\x95",                                                             // N_Up
	"\xEC\xF4\xF3\x93\x99",                                                         // N_Down
	"\x94\x93\xAD\x9E\xEA\x91",                                                     // N_Select
	"\xB4\xB9\xB1\x91\xB2\xDE",                                                     // N_ReprintShort
	"\xE9\x91\xED\xEB\xCA",                                                         // Msg_HeaderLine
	"\xF2\xAF\xED\xE7\xCA",                                                         // N_Veritcal
	"\xF5\xD7\xD2\x91"	,                                                           // N_Horizontal
	"\xC6\xAD",																		// N_Modem
	"\x43\x4F\x4D\x31",																// N_RS232RS0
	"\x43\x4F\x4D\x32",																// N_RS232RS1
	"\x43\x55\x20\xB4\xB5\xB1\xAF\x92\x20\x3A\xC6\x93\x96\x9D\xB1\x91",				// N_RS232   
	"\x9C\x93\xC5\x93\x96\x9D\xB1\x91\x20\xE8\xF4\xC8\xEB\x9D",						// N_RS232Config
	"\xC6\xB6\x91\xED\x20\x3A\xC6\x93\x96\x9D\xB1\x91",								// N_EnableComm 
	"\xE4\xDB\x9D\xED\xB1\x99\x20\x3A\xC6\x93\x96\x9D\xB1\x91",						// N_RS232Prot	
	"COMMS: BIT RATE, bps",															// N_RS232Baud   //Note:  Must be re-translated shorter.
	"\x31\x32\x30\x30",																// N_Baud1200   
	"\x31\x34\x30\x30",																// N_Baud2400   
	"\x34\x38\x30\x30",																// N_Baud4800   
	"\x39\x36\x30\x30",																// N_Baud9600   
	"\x31\x39\x32\x30\x30",															// N_Baud19200  
	"\x33\x38\x34\x30\x30",															// N_Baud38400  
	"\x35\x37\x36\x30\x30",															// N_Baud57600  
	"\x31\x31\x35\x32\x30\x30",														// N_Baud115200 
	"\x42\x49\x54\x73\x20\x53\x54\x4F\x50\x20\x43\x4F\x4D\x4D\x53\x3A",				// N_RS232Stop  
	"\x62\x69\x74\x20\x20\x31",														// N_RS232Stop1 
	"\x62\x69\x74\x73\x20\x32",														// N_RS232Stop2 
	"\x42\x49\x54\x73\x20\x44\x41\x54\x41\x20\x43\x4F\x4D\x4D\x53\x3A",				// N_RS232Data  
	"\x62\x69\x74\x73\x20\x37",														// N_RS232Data7 
	"\x62\x69\x74\x73\x20\x38",														// N_RS232Data8 
	"\x50\x41\x52\x49\x54\x59\x20\x43\x4F\x4D\x4D\x53\x3A",							// N_RS232Parity
	"\xA7\xF4\xEF",																	// N_RS232None  
	"\xAF\xB1\xD2",																	// N_RS232Odd   
	"\xA4\xED\xB2",																	// N_RS232Even  
	"\x93\xEF\xB1\x9E\xDA\x91\xB1\x93\xDA\x20\xE1\xED\xAF\xA5",						// N_CharMap	
    "\xF2\xB1\xF4\xDE\x20\xEE\xB1\x93\xE7\xBA\x20\xF2\x93\xF0\xE6\x93\xCF\xF4\x99",	// N_DialMsgs
	"\x43\x4F\x4E\x54\x52\x4F\x4C\x20\x46\x4C\x4F\x57\x20\x43\x4F\x4D\x4D\x53\x3A",	// N_RS232FlowC
    "\x9F\xD2\x93\xF3\xB1\xAF",														// N_RX
    "\xE1\x93\xB6\xB1\x91\x2F\x9F\xD2\x93\xF3\xB1\xAF",								// N_RXTX
    "\xE5\xAF\xED\xE6\x20\x9F\xE2\x93\xAA\x20\x3A\xC6\x93\x96\x9D\xB1\x91",			// N_CommsModemMode
    "",																				// N_CommsDBModemMode
    "COMMS CHANGED",																// N_CommsChanged //Note:  Must be re-translated shorter.
    "RESTART TERMINAL",																// N_RestartTerminal //Note:  Must be re-translated shorter.
    "PRESS CANCEL TO EXIT",															// MsgPressCancelToExit //Note:  Must be re-translated shorter.
    "\xF1\xE6\x93\xEA\xB1\x95\x20\xB1\xF4\xF4\xCF\x9D",								// N_ChangeApp
    "\xF1\xE6\x93\xEA\xB1\x95\x20\x94\x93\xAD\x9E\xEA\x91",							// N_SelectApplication
    "\xCC\xF3\x93\xD6\xED\x20\x9F\xB7\xF4\xE2",										// N_TraceLog
    "\xAF\xF3\xAF\xA5\x20\xC9\xED\xB1\xBA",											// N_StartNew
    "\x43\x4F\x4D\x31\x20\xD8\xF3\xB1\xC5\x20\xB2\x91\x20\xE1\x93\xB6\xB1\x91",		// N_Send1
    "\x43\x4F\x4D\x32\x20\xD8\xF3\xB1\xC5\x20\xB2\x91\x20\xE1\x93\xB6\xB1\x91",		// N_Send
    "\xD4\xD6\xED\x9D",																// N_Stop
    "",																				// N_TraceIsEmpty,
    "*** TRACING ***",																// Tracing //Note:  Must be re-translated shorter.
    "",																				// TraceOverflow
	"\x47\x53\x4D",																	// N_GSM
    "\x45\x54\x48\x45\x52\x4E\x45\x54",												// N_Ethernet
    "\x4C\x41\x4E",																	// N_LAN
    "\x42\x4C\x55\x45\x54\x4F\x4F\x54\x48",											// N_Bluetooth
    "\x49\x53\x44\x4E",																// N_ISDN
    "DIAL BACKUP",																	// N_DialBackUp     //Note:  Must be re-translated shorter.
	"",																				// EnterDBInitTel
	"",																				// EnterDBNMSTel
	"",																				// InitPrefService
	"NMS PREFERRED SERVICE",      													// NMSPrefService //Note:  Must be re-translated shorter.
	"\xD4\xF4\xCB\xC2\x20\xE1\x93\xEB\xDF\xF4\xB6",                                 // LowSignal
	"",                                                                             // N_DBInitPref
	"NMS PREF",																		// N_DBNMSPref //Note:  Must be re-translated shorter.
	"",                                                                             // N_DBInitPhone
	"DB NMS",																		// N_DBNMSPhone //Note:  Must be re-translated shorter.
	"\x93\xEF\x20\xEE\xAF\x91\xAF\x20\xDC\xEA\x93\x95\x20\x93\x95\x20\x47\x53\x4D", // N_GSMwDial
	"\x47\x53\x4D\x20\x93\x95\x20\x93\xEF\x20\xEE\xAF\x91\xAF\x20\xDC\xEA\x93\x95", // N_DialwGSM
	"\x47\x53\x4D\x20\xC6\xD7\xD2",                                                 // N_GSMonly
	"",                                                                             // N_Dialonly
	"",																				// StartGSMModem
	"",																				//ProcWasCanel
	"",																				// NoSIM	
	"",																				// ForbSIMStatus
	"",																				// Msg_InsertYourCard
    "\xB1\xF4\xAD\x91\x20\x49\x50\x20\xB4\xB5\xB1\xAF\x92",							// N_LastIPaddr
    "\xD4\xF4\xCB\xC2\x20\xF2\xB1\xC5\x93\x95",										// BatteryLow
    "",																				// UnlockingError

#ifdef	MAKE_EMV
	"EMV INFO ENABLED",         // EmvInfoEna 
	"EMV INFO DISABLED",        // EmvInfoDis 
	" DISABLE EMV INFO ?",      // DisEmvInfo 
	"  ENABLE EMV INFO ?",      // EnaEmvInfo
	"PROCESSING CARD",			// ProcCard
	"SMART CARD",				// SmartCard
	"PLEASE REMOVE CARD",		// PlsRemoveCared
	"CARD DECLINED",			// CardDeclined
	"PIN FAILED",				// PINFailed
	"SWIPE/INSERT CARD",		// SwipeInsert
	"CARD FAIL",				// CardFail
	"NO MORE APPS",				// NoMoreApps
	"CARD BLOCKED",				// CardBlocked
	"PIN BLOCKED",				// PinBlocked
	"APPLICATION BLOCKED",		// AppBlocked
	"EMV",						// N_EMV
	"APP SELECTION",			// AppSelection
	"AUTHENTICATION",			// Authentication
	"EMV DEBUG",				// EMVdebug
	"CHIP NOT SUPPORTED",		// ChipNotSupported
	"ON PINPAD",				// OnPinpad
	"\x50\x49\x4E\x20\xEE\xB1\x93\xE7\xBA\x20\xB1\xC8\x9E\xEB\xE6",                  // WaitingForPin
        "BYPASS",                   // N_Bypass 
        "PINPAD IS ENABLED",		// PinpadEP 
        "PINPAD IS DISABLED",		// PinpadDP 
	"ACCEPTED",                 // Accepted
        
        "EASY ENTRY",           // "EASY ENTRY"
        "LAST TRY LEFT",        // "LAST TRY LEFT"
        "TRIES LEFT: ",         // "TRIES LEFT: "
        "CARD FAIL",            // "CARD FAIL"
        "PIN TRIES EXCEEDED",   // "PIN TRIES EXCEEDED"
        "PIN BLOCKED",          // "PIN BLOCKED"
        "LAST PIN TRY LEFT",    // "LAST PIN TRY LEFT"
        "PRESS OK TO CONTINUE", // "PRESS OK TO CONTINUE"
        "SELECTING",            // "SELECTING"
        "BLOCKED",              // "BLOCKED"
        "CHIP CARD",            // "CHIP CARD"
        "PLEASE INSERT CARD",   // "PLEASE INSERT CARD"
        "CONTINUE?",            // "CONTINUE?"
        
        "APP BLOCKED",          // "APP BLOCKED"
        "LIST EMPTY",           // "LIST EMPTY"
        "APPLICATION SELECT",   // "APPLICATION SELECT"
        "SELECT",               // "SELECT"
        "CARD DECLINE",         // "CARD DECLINE"
        "OVERRIDE?",            // "OVERRIDE?"

#endif // MAKE_EMV

#ifdef	MAKE_SMARTINIT                
        "SMART INIT ENABLED",         // SmartInitEna  //Note:  Must be re-translated shorter.
	"SMART INIT DISABLED",        // SmartInitDis  //Note:  Must be re-translated shorter.
	" DISABLE SMART INIT?",      // DisSmartInit 
	" ENABLE SMART INIT ?",      // EnaSmartInit
#endif // MAKE_SMARTINIT

#ifdef MAKE_FARSI
    "PERSIAN", // Lng_Farsi						
#endif
    "N_IpCon",
    "N_NMSIp",
    "N_NMSPort",
    "N_InitIp",
    "N_InitPort",
    "N_LocalIp",
    "N_LocalSubnetMask",
    "N_GatewayIp",
    "N_LastSettle",
    "N_DBFormattedReport",
    "N_AcqTableReport",
    "N_TermTableReport",
    "N_IssTableReport",
    "N_CardTableReport",
    "N_PreAuth_Long",
    "N_CancelPreAuth",
    "N_ClosedPreAuth",
    "N_CancelClosedPreAuth",
    "N_SaleRefund",
    "N_InstalRefund",
    "N_PreAuth",

    "TERMINAL NOT ALLOWED",    // N_TerminalNotAllowed
        "TRANSACTION UNSUPPORT",   // TranUnsupported
        "INSTALMENT",              // Installment
        "YYYY/MM/DD",              // gg_aa_yy
        "ENTER TRANS DATE",        // TarihGir
        "INS.REFUND",              // N_InstalRefund
        "PREAUTH",                 // N_PreAuth
        "PREAUTH",                // N_PreAuth_Long
        "CLOSED PREAUTH",          // N_ClosedPreAuth
        "CANCEL PREAUTH",          // N_CancelPreAuth
        "CANCEL CLOSED P.",        // N_CancelClosedPreAuth
        "PRESS ANY KEY",           // N_ClickKey
        "FOR SECOND RECEIPT",      // N_SecondReceipt
        "USE MAGNETIC STRIPE",     // UseMagneticStripeMsg
        "  NO MANUEL ENTRY  ",     // Msg_NoManEntry,         // @ydeniz --  BC --   
        "OVER INSTALLMENT",        // Msg_OverInsNum
        "LOWER INSTALLMENT",       // Msg_LowerInsNUm
        "INS. NUM.",               // InstNum  "TAKSÝT ADETÝ:",			// 462	// 
		"FORMATTED REP",                // N_DBFormattedReport @erd 06.03.2007 !!!
		"ACQ. TABLE RPT.",          // N_AcqTableReport  @erd 06.03.2007
		"TERM. TABLE RPT.",          // N_TermTableReport @erd 06.03.2007
		"ISS. TABLE RPT.",            // N_IssTableReport  @erd 06.03.2007
		"CARD TABLE RPT.",              // N_CardTableReport @erd 06.03.2007
		"SELECT = '.'",				// N_Selectkey
		"WRONG CARD",				// Msg_WrongCard
        "PREAUTH NOT ALLOWED", // Msg_PreAuthNotAllowed
        "CLOSED PREAUTH NOT ALLOWED",
        "INSTALLMENT NOT ALLOWED",
        "MAIL ORDER NOT ALLOWED",
        "SALE REFUND",                  // N_SaleRefund
        "LAST SETTLE",                  // N_LastSettle,
        "BAL INQ." ,                           // N_BalanceInq

};

//====================================================
//!===  PINPAD MESSAGE TABLE                       ===
//====================================================
const char *const FaPinPadMessages[] = {
	"                ",			//   Msg_ClrScrn
	"    APPROVED    ",			//   Msg_Approved
	"\x20\x2A\x20\x50\x49\x4E\x50\x41\x44\x20\x9F\xB7\x9D\x20\x2A\x20",                     //   Msg_PinTestM
	"    DECLINED    ",			//   Msg_Declined
	"\x20\x20\xAB\xF4\xAA\xBE\x93\xEA\x20\x50\x49\x4E\x20\xAF\xDA\x20",                         //   Msg_IncrtPin
	"\x2A\x2A\x20\x9C\xB1\x93\xDA\x20\xF2\x93\xC6\xAD\x20\x2A\x2A\x2A",         //   Msg_PinCardErr
	"\xC6\xEB\xE6\x20\xB1\xF4\xCE\x20\x9C\xB1\x93\xDA\x20\xC9\xED\xEA",     //   Msg_UnspCard
	"  INVALID CARD  ",			//   Msg_InvldCard
	"   TRY AGAIN    ",			//   Msg_TryAgain
	"READY           ",         //	 Msg_PinReady
	"\x20\x20\x9C\xB1\x93\xDA\x20\xB4\xB9\xB2\x91\xAF\xB1\x99\x20\x20",                       //	 Msg_ProcCard
	"ENTER CARD      ",         //   Msg_EnterCard
	"TABLE IS EMPTY  "			//	 Msg_InternalTablesAreEmpty
};


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add HDT and pinpad Farsi messages tables for DML support.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void HdtTextFa_Init( void )
{
	short pgnum;

	// Verify the number of messages match
	TableItemCount = MSG_SPECIAL;
	if ( TableItemCount != ( sizeof( FarsiMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "Farsi Messages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Verify the number of messages match
	TableItemCount = LAST_PINM_ID;
	if ( TableItemCount != ( sizeof( FaPinPadMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "FaPinPad Messages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table EnglishMessages into database
	TextIndexAddTable( hdttext_table_id, LNG_FA, FarsiMessages, pgnum );

	// Put text table EnPinPadMessages into database
	TextIndexAddTable( pintext_table_id, LNG_FA, FaPinPadMessages, pgnum );
}


//=============================================================================
// Private function definitions
//=============================================================================

#endif // MAKE_FARSI
