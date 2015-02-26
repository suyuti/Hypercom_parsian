//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      hdtentext.c
//
// File Description:
//      English message table
//
//=============================================================================

#ifdef MAKE_ENGLISH

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
const char *const EnglishMessages[] = {
	"CM",						// CustomMsg
	"CM2",						// CustomMsg2
	"CM3",						// CustomMsg3
	"CM4",						// CustomMsg4
	"CM5",						// CustomMsg5
	"CM6",						// CustomMsg6
	"CM7",						// CustomMsg7
	"CM8",						// CustomMsg8
	"SWIPE CUSTOMER CARD",		// Swipecard
	"ENTER CARD NUMBER",		// EnterCard
	"** CARD ERROR **",			// CardErr
	"UNSUPPORTED CARD",			// UnsuppCard
	"INVALID CARD",				// InvalidCard
	"EXPIRED CARD",				// ExpiredCard
    "PREVALID CARD",            // PrevalidCard @@@@
	"MAN ENT. NOT ALLOWED",		// NoManEntry
    "EXPIRATION DATE MMYY",     // ExpireDate
    "START DATE MMYY",         	// StartDate
    "ISSUE NUMBER",			    // IssueNum
	"TAKE IMPRINT OF CARD",		// TakeImprint
	"ENTER LAST 4 NO. OF",		// EntPan4
	"CUSTOMER CARD",			// CustomerCard
	"ENTER CASHIER NUMBER",		// CashierNum
	"INVALID ENTRY",			// InvalidEntry
    "INVALID AMOUNT",           // InvalidAmount
	"AMOUNT",					// AmountEnt
	"AMOUNT AGAIN",				// AmountAgain
	"BASE AMOUNT",				// AmountBase
	"BAR AMOUNT",				// AmountBar
	"TAX AMOUNT",				// AmountTax
	"TIP AMOUNT",				// AmountTip
	"SALE AMOUNT",				// AmountSale
	"CASH AMOUNT",				// AmountCash
	"TENDER AMOUNT",			// AmountTender
	"TOTAL",					// AmountTotal
	"CORRECT  ?",				// CorrectYN
	"ENTER CREDIT PLAN",		// CreditPlan
	"ENTER DESCRIPTOR NO",		// EntDescrip
	"INVOICE NUMBER",			// InvoiceNum
	"WAITING FOR APPROVAL",		// WaitApprov @@@@
	"LINE IDLE",				// LineIdle
	"CHECKING LINE",			// CheckingLine
	"* LINE BUSY,  WAIT *",		// LineBusy
	"WAIT FOR DIAL TONE",		// WaitDialTone
	"DIALING NOW",				// DialNow
	"WAITING FOR ANSWER",		// WaitForAnswer
	"WAITING FOR LINE",			// WaitForLine
	"TRAINING MODEM",			// TrainModem
	"PROCESSING NOW",			// ProcNow
	"TRANSMITTING",				// Transmitting
	"NOW DIALING",				// NowDialing
	"INITIALIZING MODEM",		// InitModem
    "NETWORK LOGON",            // NetworkLogon
    "PAGING",                   // Paging
    "GATE CONNECTING",          // GateConnecting
    "LINKING...",               // Linking
    "CONNECTING...",            // Connecting
	"MEMORY PAGE STATUS:",		// MemPgStatus
	"CONFIGURE TERMINAL",		// ConfigTerm
	" OR  CALL HELP - CO",		// CallHelp
	"PLEASE INITIALIZE",		// PleaseInit
	" ",						// BlankMsg
	"PRINTER ERROR",			// PrintError
	"TRANSACTION MEMORY",		// TransMemErr
	"WAIT  SELF-TEST",			// SelfTest
	"NOT ENOUGH MEMORY",		// NotEnoughMem
	"TO RUN APPLICATION",		// ToRunApplic
	"INITIALIZE",				// InitTran
	"MERCHANT FUNCTION",		// MerchantFunc
	"SETTLE",					// SettleTran
	"BALANCE",					// BalanceTran
	"REFUND",					// RefundTran
	"REPRINT INVOICE",			// ReprintTran
	"LAST",						// ReprintLast
	"INVOICE",					// ReprintInvoice
	"** OFF-LINE MODE **",		// OfflineTran
	"ENTER INVOICE NUMBER",		// EnterInvNum
	"ENTER SOC NUMBER",			// EnterSOCNum
	"P.O. / I.D. NUMBER",		// EnterPONum
	"MERCHANT FUNCTION",		// MercFunction
	"ENTER MAINT PASSWORD",		// MaintPswd
	"PASSWORD DOES NOT MATCH",	// PswdNotMatch @@@@
	"CORRECT ?",				// CorrectYesNo
	"MERCH PROG",				// MerchProg
	"ENTER PASSWORD",			// EnterPswd
	"TERMINAL ID. NUMBER",		// EnterTSer
	"INIT TELEPHONE NO.",		// EnterInitTel
	"NMS TELEPHONE NO.",		// EnterNMSTel
	"DIAL MODE",				// EnterDialMode
    "SIGNATURE",             	// Signature
	"Please sign below",		// SignaturePlease
	"and press ENTER",			// PressEnter
	"Retry signature?",			// RetrySigCap
	"PABX ACCESS CODE",			// EnterPABX
	"EXIT, PRESS 'CANCEL'",		// ExitPressCncl
	"DUPLICATE TRANS",			// DuplicateTrans
	"ADD?",						// AddYesNo
	"NO INV. TO REPRINT",		// NoInvoice
	"PLEASE WAIT",				// PleaseWait
	"TRANSACTION COMPLETE",		// TransComplete
	"TRANSACTION ACCEPTED",		// TransAccepted
	"PROG. LOAD",				// ProgamLoad
	"FUNCTION 99 PASSWORD",		// Fun99Password
	"PROGRAM FUNCTION",			// ProgFunction
	"LOCAL OPTIONS SELECT",		// LocalOptSel
	"COMMS SETUP",              // LocalConf
	"EXCEEDED MAXIMUM COUNT",	// ExceedMax
	"ENHANCED",                 // EnhDial
	"0-LL 1-DIAL 2,3-LAN",		// LDLPmt
	"CU ADDR",					// CUAddrP
	"CURRENTLY",				// Currently
	"SIGNON/OFF",				// SignOnOff
	"ENTER TILL NUMBER",		// TillNum
	"SIGN-ON?",					// SignOnYN
	"VOID PASSWORD USED",		// VoidPassM
	"REFUND PASSWORD USED",		// RefPassM
	"ADJUST PASSWORD USED",		// AdjPassM
	"REPORT PASSWORD USED",		// RepPassM
	"HANDSET DIAL ALLOWED",		// KbdDialM
	"TOLL DIAL ALLOWED",		// TollDialM
	"MEMORY (FAST) DIAL",		// MemDialM
	"REFERRAL DIAL USED",		// RefDialM
	"TERMINAL  L O C K ?",		// KeyLock
	"ENTER ACCOUNT NUMBER",		// EnterAcct
	"ENTER OLD PASSWORD",		// OldPassword
	"ENTER NEW PASSWORD",		// NewPassword
	"NEW PASSWORD AGAIN",		// NewPassAgn
	"NO PRINTER",				// NoPrinter
	"REPRINT COMPLETE",			// ReprintComplete
	"INV. NUMBER MISSING",		// InvNumberMissing
	"PRINTING",					// PrintingWait
	"SERVERS SIGNED ON",		// ServerOn
	"CASHIERS SIGNED ON",		// CashierOn
	"KEYBOARD TEST",			// KeyBrdTest
	"CANCEL TWICE TO END",		// Cancel2End
	"KEY = ",					// DispKeyP
	"SETTLEMENT REQUIRED",		// SettlementRequired
	"READ CARD NOW",			// CardRdT
	"PRINTER IS ENABLED",		// PrinterEP
	"PRINTER IS DISABLED",		// PrinterDP
	"ENABLE  ?",				// EnablePP
	"DISABLE ?",				// DisablePP
	"SCANNING BATCH",			// ScanningBatch
	"NO BATCH TOTALS",			// NoBatchTotals
	"NO MORE TOTALS",			// NoMoreTotals
	"PRINT TEST",				// TestPrnC
	"C L O S E D",				// Closed
	"ENTER DATE MMDDYY",		// DateMDY
	"ENTER TIME HHMM",			// TimeHM
	"HOST   CURRENT BATCH",		// HostBatch
	"NO ACQUIRER",				// NoAcq
	"START OF BATCH:",			// StartOfBatch
	"END OF BATCH:",			// EndOfBatch
	"SELECT SHIFT NUMBER",		// SelectShiftNum
	"0 FOR ALL SHIFT",			// ZeroForAllShift
	"TILL",						// ReportMenuTill
	"CASHIER",					// ReportMenuCashier
	"SERVER",					// ReportMenuServer
	"AUDIT",					// ReportMenuAudit
	"SUMMARY",					// ReportMenuSummary
	"O TABS",					// ReportMenuOpenTabs
	"1=DETAIL 3=UNADJUSTD",		// CashServMenuLine1
	"2=SUMMARY",				// CashServMenuLine2
	"VOID NOT ALLOWED",			// VoidNotAllowed
	"ENTER HOST NUMBER",		// HostNum
	"TYPE ?",					// Type
	"ADJUST NOT ALLOWED",		// AdjustNotAllowed
	"RE-TRY BATCH TRANS",		// RetryBatchTrans
	"TOTAL",					// AmountTotals
	"NEW AMOUNT ?",				// AmountNew
	"ENTER RECEIPT TEXT",		// RecieptTXT
	"ENTER APPROVAL CODE",		// EnterApp
	"***TRAINING  MODE***",		// TrainingMode
	"SWITCH TO TRAIN MODE",		// GoToTrainingMode
	"LEAVE TRAINING MODE",		// LeaveTrainingMode
	"REFUND NOT ALLOWED",		// RefundNotAllowed
	"SALES TOTAL",				// SalesTotal
	"SALES TOTALS WRONG",		// SalesTotalsWrong
	"REFUND TOTAL",				// RefundTotal
	"REFUND TOTALS WRONG",		// RefundTotalsWrong
	"TOTALS",					// BatchTotals
	"RELEASE",					// BatchRelease
	"TEST TRANS",				// TestTransactions
	"NO TRANSACTIONS",			// NoTransactions
	"INCRMENT",					// IncrementShift
	"RESET",					// ResetShift
	"NO COMMS",					// NoComms
    "NO PHONE LINE",            // NoPhoneline
	"ENTER TIP",				// EnterTip
	"ENTER TOTAL",				// EnterTotal
	"1-OPEN TAB 3-TAB RPT",		// TabProcMsg1
	"2-CLOSE TAB",				// TabProcMsg2
	"ENTER SERVER NUMBER",		// ServerNum
	"CLOSE ALL OPEN TABS",		// CloseOpenTabs
	"INVOICE NOT OPEN TAB",		// NotOpenTab
	"CONFIGURE",				// N_Configure 
	"INITIALIZE",				// N_Initialize
	"MAIN MENU",                // N_Main
	"TRANS",					// N_NewTrans
	"BATCH",					// N_Batch
	"REPORTS",					// N_Reports
	"TERMINAL",					// N_Terminal
	"MORE",						// N_More
	"MENU",						// N_Menu
	"DATE/TIME",				// N_Datime
	"PRINTER",					// N_Printer
	"COMMS",					// N_Comms
	"DIAL OP",					// N_Dial
	"PASSWORD",					// N_Passwords
	"SPEEDIAL",					// N_Speedial
	"CARD READ",				// N_CardRdr
	"CONTRAST",					// N_Contras
	"SALE",						// N_Sale
	"SALE+CASH",				// N_SalCash
	"MAIL ORDER",				// N_MailOrder
	"OFFLINE",                  // N_Force
	"AUTHORIZE",				// N_Auth
	"CARD VERIF",				// N_CardVer
	"REVIEW",					// N_Review
	"FIND",						// N_Find
	"ADJUST",					// N_Adjust
	"VOID",						// N_Void
	"CASH",						// N_Cash
	"PROG LOAD",				// N_ProgLoad
	"SHIFT",					// N_Shift
	"VIEW",						// N_View
	"TRAINING",					// N_TrainMode
	"SETUP",					// N_Setup
	"FUNCTION",					// N_Function
	"TESTS",					// N_Tests
	"TAB",						// N_Tab
	"REPRINT",					// N_Reprint
    "EXIT",                     // N_Exit
	"ERROR",					// N_Error
	"CLEAR",					// N_Clear
	"CLEAR CASHIER TABLE",		// N_ClearChsTbl
	"CLEAR SERVER TABLE",		// N_ClearSerTbl
	"ENTER",					// N_Enter
	"CANCEL",					// N_Cancel
	"RESET",					// N_Reset
	"QUICK",					// N_Quick
	"ADD/REMOVE",				// N_AddRmv
	"DETAIL",					// N_Detail
	"SUMMARY",					// N_Summary
	"UNADJUSTED",				// N_Unadjusted
	"HOST",						// N_Host
	"DISPLAY",					// N_Display
	"KEYBOARD",					// N_Keyboard
	"PINPAD",					// N_Pinpad
	"INIT PHONE",				// N_InitPhone
	"NMS PHONE",				// N_NMSPhone
	"DIAL MODE",				// N_DialType
	"PABX",						// N_PABX
	"TERM ID",					// N_TermId
	"ALL",						// N_All
	"ON/OFF",					// N_OnOff
	"RECEIPT",					// N_Receipt
	"TEXT",						// N_Text
	"PAPER FEED",				// N_PaperFeed
	"QUIT",						// N_Quit
	"CHANGE",					// N_Change
	"",							// N_NullStr
	"COMMUNICATIONS",			// N_Communications
	"PASSWORD CHANGE",			// N_PswdChange
	"HIGHER",					// N_Higher
	"LOWER",					// N_Lower
	"LOCAL OPTIONS",			// N_LocalOpts
	"RESPONSE",					// N_Response
	"OPEN",						// N_Open
	"CLOSE",					// N_Close
	"PRINT",					// N_Print
	"SCROLL",					// N_Scroll
	"ENTER FUNCTION",			// N_EnterFunction
	"HOST RPT",					// N_PrintHost
	"DB RPT",					// N_DBRpt
	"TX/RX RPT",				// N_TxRxRpt
	"VIEW TABLE",				// N_ViewTable
	"DISPLAY TEST",				// N_DisplayTest
	"FUNCTION NOT SUPPORTED",	// N_FuncNotSupported
	"VERSION",					// N_Version
	"LAST RESPONSE",			// N_LastRsp
	"LAST RESP",				// N_Last
	"NO DATA",					// N_NoData
	"BATCH INFO",				// N_BatchInfo
	"HOST ID   : ",				// N_HostId
	"HOST NAME : ",				// N_HostName
	"BATCH #   : ",				// N_BatchNumber
	"0 FOR ALL HOST",			// N_ZeroForAllHost
	"REPORT",					// N_Report
	"DEBIT",					// N_Debit
	"CREDIT",					// N_Credit
	"ENTER AUTH CODE",			// N_EnterAuth
	"GENERATION NUMBER",		// N_EnterGenNum
	"JOURNAL TESTING",			// JournalTesting
	"ENTER NUM OF ENTRIES",		// NumEntries
	"PAPER OUT",				// PaperOut
    "PRINTER OVERHEAT",         // PrinterOverheat
	"WAITING FOR ACCT. NO",		// WaitingForAcct
	"WAITING FOR APPROVAL",		// WaitingForApproval
	"WAITING FOR PIN",			// WaitingForPin
	"SWIPE CARD ON PINPAD",		// SwipeCardPin
	"CHANGE TO PIN READ?",		// ChangeToPinRead
	"CHANGE TO TERM READ?",		// ChangeToTermRead
	"INVALID ACCOUNT",			// InvalidAccount
	"AVAIL BAL.",				// AvailBal
	"     TODAY",				// Today
	"CARD # DOESN'T MATCH",		// CardNoMatch
	"PINPAD TEST",				// PinPadTest
	"PINPAD AUTO DETECT",		// PinPadAutoDetect
	"SELECT ACCOUNT",			// SelectAccount
	"ACCOUNT",					// N_Account
	"0",						// N_0
	"1",						// N_1
	"2",						// N_2
	"3",						// N_3
	"4",						// N_4
	"ENTER PIN",				// EnterPin
	"NO EXTERNAL PINPAD",		// ExtPinNot
	"ENCRYPTION ERROR",			// EncryptError
	"LOGON",					// N_Logon
	"MAINTGNCE",				// MaintFunction
	"ENTER ORIGINAL DATE",		// N_OrigDate
	"IDENTIFICATION",			// N_Identification
	"SOFTWARE",					// N_Software
	"DLL",						// N_DLL
	"BOOTER",					// N_EPROM
	"REFER",					// N_Referral
	"ALL HOSTS",				// N_AllHosts
	"ICEPAC",					// N_IcePac
	"ENTER FOR ALL",			// PressEnterForAll
	"FOR LAST INVOICE",			// ForLastInv
	"TIMEOUT",					// N_Timeout
	"USER CANCELLED",			// N_UserCancelled
	"REVERSL",					// N_Reversals
	"PINPAD NOT FOUND",			// PinpadNotFound
	"YES",						// N_YES
	"NO",						// N_NO
	"OK",						// N_OK
	"QUIT",						// N_QUIT
	"NEXT",						// N_NEXT
	"SWIPE OR INSERT CARD",     // N_SwipeOrInsertCard
	"X",						// N_X
	"CONTINUE",					// N_Continue
	"KEY",						// N_KEY
	"PASSED",					// N_Passed
	"FAILED",					// N_Failed
	"TEST",						// N_Test
	"TONE",						// N_Tone
	"PULSE",					// N_Pulse
	"PIN PAD READ ERROR",		// PinReadErr
	"PIN PAD WRITE ERROR",		// PinWriteErr
	"SIGN-OFF?",				// SignOffYN
	"JAN",						// Month_1
	"FEB",						// Month_2
	"MAR",						// Month_3
	"APR",						// Month_4
	"MAY",						// Month_5
	"JUN",						// Month_6
	"JUL",						// Month_7
	"AUG",						// Month_8
	"SEP",						// Month_9
	"OCT",						// Month_10
	"NOV",						// Month_11
	"DEC",						// Month_12
	"REENTER PIN",				// Msg_Reenter_Pin
	"PIN?",						// Msg_Pin
	"EXTERNAL: ",				// Msg_External
	"INTERNAL: ",				// Msg_Internal
	"CKSUM: ",					// Msg_Chksum
	"CORRECT ?",				// Msg_Correct
	"SVR:",						// Msg_SVR
	"CSH:",						// Msg_CSH
	"INV:",						// Msg_INV
	"reversal",					// Msg_reversal
	"voided",					// Msg_voided
	"advice",					// Msg_advice
	"SEQUENCE NO.: ",			// Msg_SequenceNo
	"ITEMS",					// Msg_Items
	"SALES",					// Msg_Sales
	"ERROR **********",			// Msg_Error
	"AVAILABLE HOSTS",			// Msg_AvailHosts
	"Cash   : ",				// Msg_Cash
	"Tender : ",				// Msg_Tender
	"OPTIONAL AMT",				// AmountOpt
	"CVV2 INDICATOR",			// N_CVV2Indicator
	"0=SKIP",					// N_CVV2IndEntry1
	"1=PRESENT",				// N_CVV2IndEntry2
	"2=ILLEGBL",				// N_CVV2IndEntry3
	"9=ABSENT",					// N_CVV2IndEntry4
	"CVV2 FROM CARD",			// N_CVV2Entry
	"CARD PRESENT ?",			// Msg_CardPresent
	"HEADER 4",					// Msg_HeaderLine
	"FOOTER ",					// Msg_FooterLine
	"SWIPE YOUR CARD ",			// Msg_SwipeYourCard
	"CHOOSE TIP OPTION",		// Msg_PreTip
	"OTHER",					// Msg_Other
	"Program Error:",			// Msg_ProgramError
	"TIPS",						// Msg_Tips
	"TIP    %",					// Msg_TipPct
	"SUGGESTED TIP PERCENT",	// Msg_Sug_Tip
	"LOCAL OPT",				// Msg_LocalOpt
	"TX/RX ERRORS",				// Msg_TxRxErrors
	"TRANSMIT ERRORS:",			// Msg_TranErrors
	"RECEIVE ERRORS :",			// Msg_RecvErrors
	"FILE DOWNLOAD",			// N_FileDownLoad
	"SETTLE",					// N_Settle
	"OFFLINE",					// N_Offline
	"TRANS",					// N_Trans
	"DATE/TIME SETUP",			// N_DateTimeSetup
	"CONTRAST",					// N_Contrast
	"PAPER FD",					// N_PaperFd
	"SETTLEMENT STATUS",		// N_SettleStat
	"Choose Summary Option",	// Msg_ChooseSummary
	"AUTO   ",					// Msg_Auto
	"GROUP   ",					// Msg_Group
	"MANUAL   ",				// Msg_Manual
	"NEVER",					// Msg_Never
	"SALE ITEMS",				// Msg_SaleItems
	"REFUND ITEMS",				// Msg_RefundItems
	"ATTEMPT # ",				// Msg_Attempt
	"RESP ",					// Msg_Resp
	"EVRYTHG",					// Msg_Evrythg
	"EVERYTHING",				// Msg_Everything
	"SERVER TABLE EMPTY",		// Msg_SrvTabEmpty
	"CASHIER TABLE EMPTY",		// Msg_CshTabEmpty
	"STATUS",					// Msg_Status
	"fopen error: ",			// Msg_fopenerror
	"Down loading file: ",		// Msg_DownloadingFile
	"SELECT LANGUAGE",			// Msg_SelectLanguage
	"LANGUAGE",					// Msg_Language
	"ENGLISH",					// Lng_English
	"GERMAN",					// Lng_German
	"SWEDISH",					// Lng_Swedish
	"ESPANOL",					// Lng_Spanish
	"PORTUGUESE",				// Lng_Portugues
	"ACCEPT CURRENT LANGUAGE",	// Msg_CurrentLang
	"LANG",						// Msg_Lang
	"KEY TABLE INVALID",		// Msg_KeyTableInvalid
	"DATABASE REPORT\n",		// Prt_DatabaseReport
	"---------------\n",		// Prt_Dashes
	"TERMINAL EPROM CONFIG\n",	// Prt_TermEpromCfg
	"TERMINAL CONFIGURATION\n",	// Prt_TermConfig
	"ACQUIRER ID:",				// Prt_AcquirerID
	"ISSUER ID:",				// Prt_IssuerID
	"CARD RANGE ID:",			// Prt_CardRangeID
	"DESCRIPTOR ID:",			// Prt_DescriptorID
	"RECEIVED DATA:\n",			// Prt_ReceivedData
	"SEND DATA:\n",				// Prt_SendData
	"RAW DATA (HEX BYTES):\n",	// Prt_RawDataHexBytes
	"Waiting for LAN cable",	// WaitingForLanCable
	"Line mode canceled",		// LineModeCanceled
	"Restart terminal",			// RestartTerminal
	"ONLY 0-9 A-F VALID",		// N_Only09AFvalid
	"IDLE BUTTON FUNCTION",		// N_IdleButton
	"SELECT BUTTON",			// SelectButton
	"SELECT EVENT TYPE",		// EventType
	"BUTTON",					// N_Button
	"TRANSACTION ALLOWED",		// ConfirmTrans
	"FUNCTION ALLOWED",			// ConfirmFunc
	"INIT BUTTON TABLE",		// N_InitButTab
	"INIT. IS DISABLED",		// Init_Dis
	"INIT. IS ENABLED",			// Init_Ena
	"IDL BTN",					// N_IdlBtn
	"INIT. BTN",				// N_InitIdlBtn
	"CHG BTN",					// N_ChgIdlBtn
	"CHG EVENT BUTTON 1",		// Chang_Btn1
	"CHG EVENT BUTTON 2",		// Chang_Btn2
	"CHG EVENT BUTTON 3",		// Chang_Btn3
	"CHG EVENT BUTTON 4",		// Chang_Btn4
	"BUTTON 1",					// B_1
	"BUTTON 2",					// B_2
	"BUTTON 3",					// B_3
	"BUTTON 4",					// B_4
	"REMOVE MERCHANT COPY",     // RemoveMerchantCopy
	"NO FIELD TABLE TO PRINT",	// PclTabError
	"PERFORMANC",				// Prt_PerformBtn
	"CONFIG",					// Prt_ConfBtn   
	"CONFIGURATION REPORT",		// Config_RPT
	"TERMINAL",					// Terminal_Tab 
	"ISSUER",					// Issuer_Tab 
	"ACQUIRER",					// Acq_Tab 
	"CARD",						// Card_Tab 
	"All",						// All_Tab
	"LIBS VERSION",				// N_LibsVersion
	"SDK",						// N_SDKVersion
	"O/S VER",					// N_OSVersion
	"O/S REV",					// N_OSRevision
    "HFC",                      // N_HFCVersion
	"HISTORY REPORT",			// History_RPT
	"fread error: ",			// Msg_freaderror
	"Entry # Exceeds Max",		// N_EntryNumExceedsMax
	"Table ID: ",				// N_TableId
	"HFS",						// N_HFS
	"No enougth space in HFS",	// N_NoEnougthSpaceInHFS
	"SUPERVISOR PASSWORD",      // SVPassword
	"CALL AUTH CENTRE",         // CallAuthCentre
	"AUTH CODE ?",              // AuthCode
    "CASH NOT ALLOWED",         // N_CashNotAllowed
	"SWIPE NOT ALLOWED",        // SwipeNotAllowed
	"TRANS. NOT ALLOWED",       // TranNotAllowed
    "INIT MAX TRIES",			// InitMaxTries
	"Up",						// N_Up
	"Down",						// N_Down
	"Sel",						// N_Select
	"Repr",						// N_ReprintShort
	"HEAD",						// Msg_HeaderLine
	"Vertic",					// N_Veritcal
	"Horiz"	,					// N_Horizontal
	"LINE",                     // N_Modem
	"COM1",                     // N_RS232RS0
	"COM2",                     // N_RS232RS1
	"COMMS: CU ADDRESS",        // N_RS232   
	"COMMS CONFIG",             // N_RS232Config
	"COMMS: INTERFACE",         // N_EnableComm 
	"COMMS: PROTOCOL",          // N_RS232Prot	
	"COMMS: BIT RATE, bps",     // N_RS232Baud  
	"1200",                     // N_Baud1200   
	"2400",                     // N_Baud2400   
	"4800",                     // N_Baud4800   
	"9600",                     // N_Baud9600   
	"19200",                    // N_Baud19200  
	"38400",                    // N_Baud38400  
	"57600",                    // N_Baud57600  
	"115200",                   // N_Baud115200 
	"COMMS: STOP BITs",         // N_RS232Stop  
	"1  bit",                   // N_RS232Stop1 
	"2 bits",                   // N_RS232Stop2 
	"COMMS: DATA BITs",         // N_RS232Data  
	"7 bits",                   // N_RS232Data7 
	"8 bits",                   // N_RS232Data8 
	"COMMS: PARITY",            // N_RS232Parity
	"None",                     // N_RS232None  
	"Odd",                      // N_RS232Odd   
	"Even",                     // N_RS232Even  
	"CHAR MAP",                 // N_CharMap	
    "DIALING MESSAGES",         // N_DialMsgs
	"COMMS: FLOW CONTROL",      // N_RS232FlowC
    "RX",                       // N_RX
    "RX / TX",                  // N_RXTX
    "COMMS: MODEM MODE",        // N_CommsModemMode
    "COMMS: DB MODEM MODE",     // N_CommsDBModemMode
    "COMMS CHANGED",            // N_CommsChanged
    "RESTART TERMINAL",         // N_RestartTerminal
    "PRESS CANCEL TO EXIT",     // MsgPressCancelToExit
    "CHANGE APP",               // N_ChangeApp
    "SELECT APP",               // N_SelectApplication
    "TRACE LOG",                // N_TraceLog
    "START NEW",                // N_StartNew
    "SEND VIA COM1",            // N_Send1
    "SEND VIA COM2",            // N_Send
    "STOP",                     // N_Stop
    "TRACE IS EMPTY",           // N_TraceIsEmpty,
    "*** TRACING ***",          // Tracing
    "** TRACE OVERFLOW **",     // TraceOverflow
	"GSM",                      // N_GSM
    "ETHERNET",                 // N_Ethernet
    "LAN",                      // N_LAN
    "BLUETOOTH",                // N_Bluetooth
    "ISDN",                     // N_ISDN
    "DIAL BACKUP",              // N_DialBackUp    
	"DB INIT TELEPHONE NO.",		// EnterDBInitTel
	"DB NMS TELEPHONE NO.",		// EnterDBNMSTel
	"INIT. PREF. SERVICE",		// InitPrefService
	"NMS PREFERRED SERVICE",      	// NMSPrefService
	"LOW SIGNAL",			// LowSignal
	"INIT PREF",     			// N_DBInitPref
	"NMS PREF",				// N_DBNMSPref
	"DB INIT",     				// N_DBInitPhone
	"DB NMS",				// N_DBNMSPhone
	"GSM with DB",    			// N_GSMwDial
	"DB with GSM",			// N_DialwGSM
	"GSM only",     			// N_GSMonly
	"DB only",			// N_Dialonly
	"",					// StartGSMModem
	"",					//ProcWasCanel
	"",					// NoSIM	
	"",					// ForbSIMStatus
	"",					// Msg_InsertYourCard
    "LAST IP ADDRESS",          // N_LastIPaddr
    "BATTERY LOW",              // BatteryLow
    "UNLOCKING ERROR",          // UnlockingError

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
	"WAITING FOR PIN",			// WaitingForPin
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
        "SMART INIT ENABLED",         // SmartInitEna 
	"SMART INIT DISABLED",        // SmartInitDis 
	" DISABLE SMART INIT?",      // DisSmartInit 
	" ENABLE SMART INIT ?",      // EnaSmartInit
#endif // MAKE_SMARTINIT

    "SET COM PARAM",     // N_SetComParam,
    "HOST INFO TYPE",    // N_HostInfoType,
    "SET PARAM.",        // N_SetParam,
    "COM TYPE",          // N_ComType,
    "HOST INFO",         // N_HostInfo,
    "TRANSACTION HOST",  // N_TransactionHost,
    "SETTLEMENT HOST",   // N_SettlementHost,
    "PRIMARY",           // N_Primary,
    "SECONDARY",         // N_Secondary,
    "PHONE NUMBER",      // N_PhoneNumber,
    "CONNECT TIME",      // N_ConnectTime,
    "DIAL ATTEMPT",      // N_DialAttemp,
    "MODEM MODE",        // N_ModemMode,
    "IP ADDRESS",         // N_IpAddress,
    "PPP ENABLE",        // N_PPPEnable,
    "PPP Mode",          // N_PPPMode
    "ENTER PHONE NUMBER",// EnterPhoneNumber
    "ENTER CONNECT TIME",// EnterConnectTime
    "ENTER DIAL ATTEMP ", // EnterDialAttemp
    "ENTER PPP MODE",      // EnterPPPMode
    "ENTER MODEM MODE",    // EnterModemMode, 
    "ENTER IPADDRESS",     // EnterIPAddress, 
    "PPP ENABLED?",         // PPPEnabled,
    "USE * IN ORDER TO .",          // UsePoint, 
    "CURRENT IP:",         // CurrentIP,
    "NII",                       //     N_NII,
    "PORT NUMBER", //N_PortNumber,
    "HEX LEN",      //N_HexLenEnable,
    "POS TPDU",    // N_PosTPDUEnable,
    "ENTER NII",    // EnterNII
    "PPP is ENABLED", // PPP_Ena
    "PPP is DISABLED",// PPP_Dis
    "ENTER IP PORT",              // EnterIPPort
    "HEX LEN IN HEADER is ENABLED", //HexLen_Ena,
    "HEX LEN IN HEADER is DISABLED",//HexLen_Dis,
    "POS TPDU IN HEADER is ENABLED",//PosTPDU_Ena,
    "POS TPDU IN HEADER is DISABLE",//PosTPDU_Dis,
    "IP",   //N_IP,
    "GPRS", //N_GPRS,
    "HLAN", //N_HLAN,
    "DIAL BACKUP", //N_DIALBACKUP,
    "DIAL UP",  //N_ONLYDIALUP,
    "IP is ENABLED", //IP_Ena,
    "IP is DISABLED",//IP_Dis,
    "GPRS is ENABLED", //GPRS_Ena,
    "GPRS is DISABLED",//GPRS_Dis,
    "HLAN is ENABLED", //HLAN_Ena,
    "HLAN is DISABLED", //HLAN_Dis,
    "DIAL BACKUP is ENABLED", //DBackup_Ena,
    "DIAL BACKUP is DISABLED", //DBackup_Dis,
    "ONLY DIAL UP is ENABLED", //Dial_Ena,
    "ONLY DIAL UP is DISABLED", //Dial_Dis,
    "COM PARAM.", //N_ComParameter
};

//====================================================
//!===  PINPAD MESSAGE TABLE                       ===
//====================================================
const char *const EnPinPadMessages[] = {
	"                ",			//   Msg_ClrScrn
	"    APPROVED    ",			//   Msg_Approved
	"* PINPAD TEST * ",			//   Msg_PinTestM
	"    DECLINED    ",			//   Msg_Declined
	"  INCORRECT PIN ",			//   Msg_IncrtPin
	"** CARD ERROR **",			//   Msg_PinCardErr
	"UNSUPPORTED CARD",			//   Msg_UnspCard
	"  INVALID CARD  ",			//   Msg_InvldCard
	"   TRY AGAIN    ",			//   Msg_TryAgain
	"READY           ",         //	 Msg_PinReady
	"PROCESSING CARD ",         //	 Msg_ProcCard
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
//!      Add HDT and pinpad English messages tables for DML support.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void HdtTextEn_Init( void )
{
	short pgnum;

	// Verify the number of messages match
	TableItemCount = MSG_SPECIAL;
	if ( TableItemCount != ( sizeof( EnglishMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "English Messages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Verify the number of messages match
	TableItemCount = LAST_PINM_ID;
	if ( TableItemCount != ( sizeof( EnPinPadMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "EnPinPad Messages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table EnglishMessages into database
	TextIndexAddTable( hdttext_table_id, LNG_EN, EnglishMessages, pgnum );

	// Put text table EnPinPadMessages into database
	TextIndexAddTable( pintext_table_id, LNG_EN, EnPinPadMessages, pgnum );
}


//=============================================================================
// Private function definitions
//=============================================================================

#endif // MAKE_ENGLISH
