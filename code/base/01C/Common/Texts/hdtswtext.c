//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      hdtswtext.c
//
// File Description:
//      Swedish message table
//
//=============================================================================

#ifdef MAKE_SWEDISH

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
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
//!===  SWEDISH MESSAGE TABLE                       ==
//====================================================
//! Refer to msg.h for enum msg_id definition
const char *const SwedishMessages[] = {
	"CM",						// CustomMsg  
	"CM2",						// CustomMsg2 
	"CM3",						// CustomMsg3
	"CM4",						// CustomMsg4
	"CM5",						// CustomMsg5
	"CM6",						// CustomMsg6
	"CM7",						// CustomMsg7
	"CM8",						// CustomMsg8
	"DRAG KUND KORT",			// Swipecard
	"ANGE KORT NUMMER",			// EnterCard
	"** KORT FEL **",			// CardErr
	"DETTA KORT STÖDS EJ",		// UnsuppCard
	"OGILTIGT KORT",			// InvalidCard
	"KORTET UTGÅNGET",			// ExpiredCard
    "KORTET 'PREVALID'",        // PrevalidCard
	"MAN INM. EJ TILLÅTEN",		// NoManEntry
    "GILTIGT TILL MMYY",        // ExpireDate
    "START DATUM MMYY",         // StartDate
    "UTGÅVA NUMMER",		    // IssueNum
	"AVTRYCK FRÅN KORT",		// TakeImprint
	"ANGE SISTA 4 NR.AV",		// EntPan4
	"KUND KORT",				// CustomerCard
	"ANGE EXPEDIT NUMMER",		// CashierNum
	"OGILTIG POST",				// InvalidEntry
    "OGILTIG SUMMA",            // InvalidAmount
	"BELOPP",					// AmountEnt
	"UPPREPA BELOPP",			// AmountAgain
	"BAS BELOPP",				// AmountBase
	"BAR BELOPP",				// AmountBar
	"SKATT BELOPP",				// AmountTax
	"DRICKS BELOPP",			// AmountTip
	"FÖRSÄLJNINGSPRIS",			// AmountSale
	"KONTANT BELOPP",			// AmountCash
	"ANBUDS BELOPP",			// AmountTender
	"TOTAL",					// AmountTotal
	"KORREKT  ?",				// CorrectYN
	"ANGE KREDIT PLAN",			// CreditPlan
	"ANGE DESCRIPT NO",			// EntDescrip
	"FAKTURA NUMMER",			// InvoiceNum
	"VÄNTAR PÅ GODKÄNNANDE",	// WaitApprov
	"LINJE LEDIG",				// LineIdle
	"KONTROLLERAR LINJE",		// CheckingLine
	"LINJE UPPTAGEN, VÄNTA",	// LineBusy
	"VÄNTA PÅ RINGTON",			// WaitDialTone
	"RINGER",					// DialNow
	"VÄNTAR PÅ SVAR",			// WaitForAnswer
	"VÄNTAR PÅ LEDIG LINJE",	// WaitForLine
	"TESTAR MODEM",				// TrainModem
	"UTFÖR",					// ProcNow
	"SÄNDER",					// Transmitting
	"RINGER",					// NowDialing
	"INITIERAR MODEM",			// InitModem
    "NETWORK LOGON",            // NetworkLogon
    "PAGING",                   // Paging
    "GATE CONNECTING",          // GateConnecting
    "LÄNKAR...",		        // Linking
    "ANSLUTER...",	            // Connecting
	"STATUS MINNESSIDA:",		// MemPgStatus
	"KONFIGURERAR TERMINAL",	// ConfigTerm
	"ELLER RING HJÄLP - CO",	// CallHelp
	"INITIERA",					// PleaseInit
	" ",						// BlankMsg
	"UTSKRIFTS FEL",			// PrintError
	"MINNES TRANSAKTION",		// TransMemErr
	"VÄNTA  SJÄLV-TEST",		// SelfTest
	"MINNE EJ TILLRÄCKL",		// NotEnoughMem
	"KÖR APPLIKATION",			// ToRunApplic
	"INITIERA",					// InitTran
	"HANDLAR FUNKTION",			// MerchantFunc
	"BETALA",					// SettleTran
	"BALANSERA",				// BalanceTran
	"ÅTERBETALA",				// RefundTran
	"SKRIV UT FAKTURA IGEN",	// ReprintTran
	"SIST",						// ReprintLast
	"FAKTURA",					// ReprintInvoice
	"** OFF-LINE MODE **",		// OfflineTran
	"ANGE FAKTURA NUMMER",		// EnterInvNum
	"ANGE SOC NUMMER",			// EnterSOCNum
	"P.K. / I.D. NUMMER",		// EnterPONum
	"HANDLAR FUNKTION",			// MercFunction
	"ANGE UNDERH LÖSENORD",		// MaintPswd
	"FELAKTIGT LÖSENORD",		// PswdNotMatch
	"KORREKT ?",				// CorrectYesNo
	"HANDLAR PROG",				// MerchProg
	"ANGE LÖSENORD",			// EnterPswd
	"TERMINAL ID. NUMMER",		// EnterTSer
	"INIT. TELEFON NR.",		// EnterInitTel
	"NMS TELEPHONE NO.",		// EnterNMSTel
	"UPPRIGNINGS MODE",			// EnterDialMode
    "SIGNATUR",            		// Signature
	"Signera här under",		// SignaturePlease
	"och tryck ENTER",			// PressEnter
	"Skriv in signatur igen?",	// RetrySigCap
	"PABX ÅTKOMST KOD",			// EnterPABX
	"AVSLUTA, TRYCK 'CANCEL'",	// ExitPressCncl
	"DUBBLA TRANSAKTIONER",		// DuplicateTrans
	"LÄGG TILL?",				// AddYesNo
	"INGEN FAKTURA ATT SKRIVA OM",// NoInvoice
	"VÄNTA",					// PleaseWait
	"TRANSAKTION AVSLUTAD",		// TransComplete
	"TRANSAKTION ACCEPTERAD",	// TransAccepted
	"PROGAM LADDAS",			// ProgamLoad
	"FUNKTION 99 LÖSENORD",		// Fun99Password
	"PROGRAM FUNKTION",			// ProgFunction
	"LOKALA VAL",				// LocalOptSel
	"KOMM INIT",				// LocalConf
	"MAX BELOPP ÖVERSKR",		// ExceedMax
	"ÖKAD",						// EnhDial
	"0-LL 1-DIAL 2,3-LAN",		// LDLPmt
	"CU ADDR",					// CUAddrP
	"NUVARANDE",				// Currently
	"SIGN ON/OFF",				// SignOnOff
	"ENTER TILLS NUMMER",		// TillNum
	"SIGN-PÅ?",					// SignOnYN
	"OGILTIGT LÖSENORD",		// VoidPassM
	"GAMMALT LÖSENORD",			// RefPassM
	"ADJUST PASSWORD USED",		// AdjPassM
	"REPORT PASSWORD USED",		// RepPassM
	"UPPRINGNING MED LUR",		// KbdDialM
	"TOLL DIAL ALLOWED",		// TollDialM
	"MINNES (SNABB) UPPR",		// MemDialM
	"REFERRAL DIAL USED",		// RefDialM
	"TERMINAL  LÅS ?",			// KeyLock
	"ANGE KONTO NUMMER",		// EnterAcct
	"ANGE GAMMALT LÖSENORD",	// OldPassword
	"ANGE NYTT LÖSENORD",		// NewPassword
	"NYTT LÖSENORD IGEN",		// NewPassAgn
	"INGEN SKRIVARE",			// NoPrinter
	"OMUTSKRIFT KLAR",			// ReprintComplete
	"INV. NUMMER SAKNAS",		// InvNumberMissing
	"SKRIVER UT",				// PrintingWait
	"SERVRAR INLOGGADE",		// ServerOn
	"KASSÖR INLOGGAD",			// CashierOn
	"TANGENTBORDS TEST",		// KeyBrdTest
	"AVBRYT 2 GGR FÖR AVSL",	// Cancel2End
	"TANGENT = ",				// DispKeyP
	"UPPGÖRELSE KRÄVS",			// SettlementRequired
	"LÄS KORT NU",				// CardRdT
	"SKRIVARE ÄR REDO",			// PrinterEP
	"SKRIVARE ÄR EJ REDO",		// PrinterDP
	"REDO  ?",					// EnablePP
	"EJ REDO ?",				// DisablePP
	"GENOMSÖKER OMGÅNG",		// ScanningBatch
	"NO BATCH TOTALS",			// NoBatchTotals
	"NO MORE TOTALS",			// NoMoreTotals
	"SKRIV UT TEST",			// TestPrnC
	"S T Ä N G D",				// Closed
	"ANGE DATUM MMDDÅÅ",		// DateMDY
	"ANGE TID TTMM",			// TimeHM
	"HOST CURRENT BATCH",		// HostBatch
	"INGEN INLÖSARE",			// NoAcq
	"START PÅ OMGÅNG:",			// StartOfBatch
	"SLUT PÅ OMGÅNG:",			// EndOfBatch
	"VÄLJ VÄXLINGS NUMMER",		// SelectShiftNum
	"0 FÖR ALLA VÄXLINGAR",		// ZeroForAllShift
	"TILLS",					// ReportMenuTill
	"EXPEDIT",					// ReportMenuCashier
	"SERVER",					// ReportMenuServer
	"GRANSKNING",				// ReportMenuAudit
	"SUMMERING",				// ReportMenuSummary
	"O NOTA",					// ReportMenuOpenTabs
	"1=DETALJ 3=OINSTÄLLD",		// CashServMenuLine1
	"2=SUMMERING",				// CashServMenuLine2
	"VOID NOT ALLOWED",			// VoidNotAllowed
	"ANGE SERVER NUMMER",		// HostNum
	"TYP?",						// Type
	"JUSTERING EJ TILLÅTEN",	// AdjustNotAllowed
	"FÖRSÖK TRANS IGEN",		// RetryBatchTrans
	"TOTAL",					// AmountTotals
	"NYTT BELOPP?",				// AmountNew
	"ANGE KVITTO TEXT",			// RecieptTXT
	"ANGE GODKÄNNANDE KOD",		// EnterApp
	"***TRÄNINGS  MODE***",		// TrainingMode
	"VÄXLA TILL TRÄN MODE",		// GoToTrainingMode
	"LÄMNA TRÄNINGS MODE",		// LeaveTrainingMode
	"ÅTERBET EJ MÖJLIG",		// RefundNotAllowed
	"TOTAL FÖRSÄLJNING",		// SalesTotal
	"TOTAL FÖRSÄLJNING FEL",	// SalesTotalsWrong
	"TOTAL ÅTERBETALNING",		// RefundTotal
	"TOTAL ÅTERBET. FEL",		// RefundTotalsWrong
	"TOTAL",					// BatchTotals
	"UTGÅVA",					// BatchRelease
	"TEST TRANS",				// TestTransactions
	"INGA TRANSAKTIONER",		// NoTransactions
	"ÖKA",						// IncrementShift
	"RESET",					// ResetShift
	"INGEN KOMM",				// NoComms
	"INGEN TFN LINJE",			// NoPhoneLine
	"ANGE DRICKS",				// EnterTip
	"ANGE TOTALT",				// EnterTotal
	"1-ÖPP NOTA 3-NOTA RPT",	// TabProcMsg1
	"2-STÄNG NOTA",				// TabProcMsg2
	"ANGE SERVER NUMMER",		// ServerNum
	"STÄNG ALLA ÖPP NOTOR",		// CloseOpenTabs
	"FAKTURA, EJ ÖPP NOTA",		// NotOpenTab
	"KONFIGURERA",				// N_Configure 
	"INIT",						// N_Initialize
	"HUVUD MENY",               // N_Main
	"TRANS",					// N_NewTrans
	"OMGÅNG",					// N_Batch
	"RAPPORTER",				// N_Reports
	"TERM",						// N_Terminal
	"MER",						// N_More
	"MENY",						// N_Menu
	"DAT/TID",					// N_Datime
	"SKRIVARE",					// N_Printer
	"KOMM",						// N_Comms
	"RING UPP OPTIONER",		// N_Dial
	"LÖSENORD",					// N_Passwords
	"SNABBUPPR",				// N_Speedial
	"KORT LÄSARE",				// N_CardRdr
	"KONTRAST",					// N_Contras
	"FÖRSÄLJN",					// N_Sale
	"SALE+CASH",				// N_SalCash
	"M ORDER",					// N_MailOrder
	"EJ ANSL",					// N_Force
	"GODK",						// N_Auth
	"KORT VER",					// N_CardVer
	"GRANSKNING",				// N_Review
	"HITTA",					// N_Find
	"JUSTERA",					// N_Adjust
	"OGILTIG",					// N_Void
	"KONTANTER",				// N_Cash
	"PROG LD",					// N_ProgLoad
	"VÄXLA",					// N_Shift
	"BETRAKTA",					// N_View
	"TRÄNA",					// N_TrainMode
	"SETUP",					// N_Setup
	"FUNKTION",					// N_Function
	"TESTER",					// N_Tests
	"NOTA",						// N_Tab
	"SKRIV UT IGEN",			// N_Reprint
    "EXIT",                     // N_Exit
	"FEL",						// N_Error
	"RENSA",					// N_Clear
	"RENSA EXPEDIT TABELL",		// N_ClearChsTbl
	"RENSA SERVER TABELL",		// N_ClearSerTbl
	"ENTER",					// N_Enter
	"AVBRYT",					// N_Cancel
	"RESET",					// N_Reset
	"QUICK",					// N_Quick
	"LÄGG TILL/TA BORT",		// N_AddRmv
	"DETALJ",					// N_Detail
	"SUMMERING",				// N_Summary
	"OINTSÄLLD",				// N_Unadjusted
	"SERVER",					// N_Host
	"DISPLAY",					// N_Display
	"TANGENTBORD",				// N_Keyboard
	"PINPAD",					// N_Pinpad
	"INIT TFN",					// N_InitPhone
	"NMS TFNNUMMER",			// N_NMSPhone
	"UPPRINGNINGSTYP",			// N_DialType
	"PABX",						// N_PABX
	"TERM ID",					// N_TermId
	"ALLA",						// N_All
	"PÅ/AV",					// N_OnOff
	"KVITTO",					// N_Receipt
	"TEXT",						// N_Text
	"PAPPERS UTMATNING",		// N_PaperFeed
	"AVSLUTA",					// N_Quit
	"ÄNDRA",					// N_Change
	"",							// N_NullStr
	"KOMMUNIKATION",			// N_Communications
	"ÄNDRA LÖSENORD",			// N_PswdChange
	"HÖGRE",					// N_Higher
	"LÄGRE",					// N_Lower
	"LOKALA VAL",				// N_LocalOpts
	"SVAR",						// N_Response
	"ÖPPNA",					// N_Open
	"STÄNG",					// N_Close
	"SKRIV UT",					// N_Print
	"BLÄDDRA",					// N_Scroll
	"ANGE FUNKTION",			// N_EnterFunction
	"SERVER RPT",				// N_PrintHost
	"DATABAS RPT",				// N_DBRpt
	"TXRX RPT",					// N_TxRxRpt
	"TABELLÖVERSIKT",			// N_ViewTable
	"DISPLAY TEST",				// N_DisplayTest
	"STÖDJER EJ FUNKTIONEN",	// N_FuncNotSupported
	"VERSION",					// N_Version
	"SISTA SVARET",				// N_LastRsp
	"SISTA",					// N_Last
	"INGA DATA",				// N_NoData
	"OMGÅNGS INFO",				// N_BatchInfo
	"SERVER ID   : ",			// N_HostId
	"SERVER NAMN : ",			// N_HostName
	"OMGÅNG #   : ",			// N_BatchNumber
	"0 FÖR ALLA SERVRAR",		// N_ZeroForAllHost
	"RAPPORT",					// N_Report
	"DEBIT",					// N_Debit
	"KREDIT",					// N_Credit
	"ANGE VERIF. KOD",			// N_EnterAuth
	"GENERATIONS NUMMER",		// N_EnterGenNum
	"JOURNAL TESTNING",			// JournalTesting
	"ANGE ANTAL INGÅNGAR",		// NumEntries
	"PAPPER SLUT",				// PaperOut
    "PRINTER OVERHEAT",         // PrinterOverheat - NEED TO BE TRANSLATED !!
    "WAITING FOR ACCT. NO",		// WaitingForAcct
	"VÄNTAR PÅ GODKÄNNANDE",	// WaitingForApproval
	"VÄNTAR PÅ PIN",			// WaitingForPin
	"DRAG KORT PÅ PINPAD",		// SwipeCardPin
	"ÄNDRA TILL PIN LÄSN?",		// ChangeToPinRead
	"ÄNDRA TILL TERM LÄSN?",	// Change the place of card read from PinPad to Terminal ChangeToTermRead						?
	"OGILTIGT KONTO",			// InvalidAccount
	"TILLGÄNGLIGT SALDO",		// AvailBal
	"     IDAG",				// Today
	"KORT # PASSAR EJ",			// CardNoMatch
	"PINPAD TEST",				// PinPadTest
	"PINPAD AUTO DETEKT",		// PinPadAutoDetect
	"VÄLJ KONTO",				// SelectAccount
	"KONTO",					// N_Account
	"0",						// N_0
	"1",						// N_1
	"2",						// N_2
	"3",						// N_3
	"4",						// N_4
	"ANGE PIN",					// EnterPin
	"INGEN EXTERN PINPAD",		// ExtPinNot
	"KRYPTERINGS FEL",			// EncryptError
	"LOGGA IN",					// N_Logon
	"UNDERHÅLL",				// MaintFunction
	"ANGE ORIG. DATUM",			// N_OrigDate
	"IDENTIFIKATION",			// N_Identification
	"MJUKVARA",					// N_Software
	"DLL",						// N_DLL
	"BOOTER",					// N_EPROM
	"REFER",					// N_Referral	 
	"ALLA SERVRAR",				// N_AllHosts
	"ICEPAC",					// N_IcePac		 
	"ANGE FÖR ALLA",			// PressEnterForAll
	"FÖR SISTA FAKTURAN",		// ForLastInv
	"TIMEOUT",					// N_Timeout	 
	"ANVÄNDARE AVBRÖT",			// N_UserCancelled
	"REVERSL",					// N_Reversals	
	"PINPAD FINNS EJ",			// PinpadNotFound
	"JA",						// N_YES
	"NEJ",						// N_NO
	"OK",						// N_OK
	"AVSLUTA",					// N_QUIT
	"NÄSTA",					// N_NEXT
	"DRAG/SÄTT I KORT",			// N_SwipeOrInsertCard
	"X",						// N_X
	"FORTSÄTT",					// N_Continue
	"TANGENT",				   	// N_KEY
	"GODKÄND",					// N_Passed
	"EJ GODKÄND",			   	// N_Failed
	"TEST",						// N_Test
	"TON",						// N_Tone
	"PULS",						// N_Pulse
	"PIN PAD LÄSNINGS FEL",		// PinReadErr
	"PIN PAD SKRIVN FEL",		// PinWriteErr
	"LOGGA UT?",				// SignOffYN
	"JAN",						// Month_1
	"FEB",						// Month_2
	"MAR",						// Month_3
	"APR",						// Month_4
	"MAJ",						// Month_5
	"JUN",						// Month_6
	"JUL",						// Month_7
	"AUG",						// Month_8
	"SEP",						// Month_9
	"OKT",						// Month_10
	"NOV",						// Month_11
	"DEC",						// Month_12
	"SKRIV IN PIN IGEN",		// Msg_Reenter_Pin
	"PIN?",						// Msg_Pin
	"EXTERN: ",					// Msg_External
	"INTERN: ",					// Msg_Internal
	"CKSUM: ",					// Msg_Chksum
	"KORREKT ?",				// Msg_Correct
	"SVR:",						// Msg_SVR
	"EXP:",						// Msg_CSH
	"FAK:",						// Msg_INV
	"omslag",					// Msg_reversal
	"misslyckades",				// Msg_voided
	"advice",					// Msg_advice
	"SEKVENS NR.: ",			// Msg_SequenceNo
	"POSTER",					// Msg_Items
	"FÖRSÄLJNING",				// Msg_Sales
	"FEL **********",			// Msg_Error
	"TILLGÄNGLIGA SERVRAR",		// Msg_AvailHosts
	"Kontanter   : ",			// Msg_Cash
	"Anbud : ",					// Msg_Tender
	"VALBART BELOPP",			// AmountOpt
	"CVV2 INDIKATOR",			// N_CVV2Indicator
	"0=HOPPA ÖVER",				// N_CVV2IndEntry1
	"1=NUVARANDE",				// N_CVV2IndEntry2
	"2=ILLEGAL",				// N_CVV2IndEntry3
	"9=FRÅNVARANDE",			// N_CVV2IndEntry4
	"CVV2 FRÅN KORT",			// N_CVV2Entry
	"NUVARANDE KORT ?",			// Msg_CardPresent
	"SIDHUVUD 4",				// Msg_HeaderLine					
	"SIDFOT ",					// Msg_FooterLine
	"DRAG DITT KORT ",			// Msg_SwipeYourCard
	"VÄLJ DRICKS VAL",			// Msg_PreTip
	"ANDRA",					// Msg_Other
	"Program Fel:",				// Msg_ProgramError
	"DRICKS",					// Msg_Tips
	"DRICKS    %",				// Msg_TipPct
	"FÖRESLAGEN DRICKS %",		// Msg_Sug_Tip
	"LOKALA VAL",				// Msg_LocalOpt
	"TX/RX FEL",				// Msg_TxRxErrors
	"SÄNDNINGS FEL:",			// Msg_TranErrors
	"MOTTAGNINGS FEL :",		// Msg_RecvErrors
	"NERLADDNING AV FIL",		// N_FileDownLoad
	"BETALNING",				// N_Settle
	"EJ UPPKOPPLAD",			// N_Offline
	"TRANS",					// N_Trans
	"DATUM/TID INSTÄLLNING",	// N_DateTimeSetup
	"KONTRAST",					// N_Contrast
	"PAPPERS UTMATNING",		// N_PaperFd
	"FASTSTÄLLANDE STATUS",		// N_SettleStat
	"Välj Sammanst. Alt.",		// Msg_ChooseSummary
	"AUTOMATISK   ",			// Msg_Auto
	"GRUPP   ",					// Msg_Group
	"MANUELL   ",				// Msg_Manual
	"ALDRIG",					// Msg_Never
	"FÖRSÄLJNINGS POSTER",		// Msg_SaleItems
	"ÅTERBETALDA POSTER",		// Msg_RefundItems
	"FÖRSÖK # ",				// Msg_Attempt
	"SVAR ",					// Msg_Resp
	"ALLTING",					// Msg_Evrythg
	"ALLTING",					// Msg_Everything
	"SERVER TABELL TOM",		// Msg_SrvTabEmpty
	"EXPEDIT TABELL TOM",		// Msg_CshTabEmpty
	"STATUS",					// Msg_Status
	"filöppn. fel: ",			// Msg_fopenerror
	"Laddar ner fil: ",			// Msg_DownloadingFile
	"VÄLJ SPRÅK",				// Msg_SelectLanguage
	"SPRÅK",					// Msg_Language
	"ENGELSKA",					// Lng_English
	"GERMAN",					// Lng_German
	"SVENSKA",					// Lng_Swedish
	"SPANSKA",					// Lng_Spanish
	"PORTUGISISKA",				// Lng_Portugues
	"VÄLJ NUVARANDE SPRÅK?",	// Msg_CurrentLang
	"SPRÅK",					// Msg_Lang
	"TANGENTTABELL OGILTIG",	// Msg_KeyTableInvalid
	"DATABAS RAPPORT\n",		// Prt_DatabaseReport
	"---------------\n",		// Prt_Dashes
	"TERMINAL EPROM KONFIG\n",	// Prt_TermEpromCfg
	"TERMINAL KONFIGURATION\n",	// Prt_TermConfig
	"INLÖSAR ID:",				// Prt_AcquirerID
	"UTFÄRDAR ID:",				// Prt_IssuerID
	"KORTNR. SERIE ID:",		// Prt_CardRangeID
	"DESKRIPTOR ID:",			// Prt_DescriptorID
	"MOTTAGET DATA:\n",			// Prt_ReceivedData
	"SEND DATA:\n",				// Prt_SendData
	"RÅ DATA (HEX BYTES):\n",	// Prt_RawDataHexBytes
	"Väntar på LAN kabel",		// WaitingForLanCable
	"Linje mode avbruten",		// LineModeCanceled
	"Starta om terminalen",		// RestartTerminal
	"ENDAST 0-9 A-F GILT",		// N_Only09AFvalid
	"FUNKTION INAKT. KNAPP",	// N_IdleButton
	"VÄLJ KNAPP",				// SelectButton
	"VÄLJ HÄNDELSE TYP",		// EventType
	"KNAPP",					// N_Button
	"TRANSAKTION TILLÅTEN",		// ConfirmTrans
	"FUNKTION TILLÅTEN",		// ConfirmFunc
	"INIT KNAPP TABELL",		// N_InitButTab
	"INIT. ÄR EJ AKTIVERAD",	// Init_Dis
	"INIT. ÄR AKTIVERAD",		// Init_Ena
	"INAKT KNP",				// N_IdlBtn
	"INIT. KNP",				// N_InitIdlBtn
	"ÄNDRA KNP",				// N_ChgIdlBtn
	"ÄNDRA HÄNDELSE KNAPP1",	// Chang_Btn1
	"ÄNDRA HÄNDELSE KNAPP2",	// Chang_Btn2
	"ÄNDRA HÄNDELSE KNAPP3",	// Chang_Btn3
	"ÄNDRA HÄNDELSE KNAPP4",	// Chang_Btn4
	"KNAPP 1",					// B_1
	"KNAPP 2",					// B_2
	"KNAPP 3",					// B_3
	"KNAPP 4",					// B_4
	"TA BORT HANDLARENS KOP",	// RemoveMerchantCopy
	"INGEN FÄLTTAB ATT SKR",	// PclTabError
	"UTFÖR",					// Prt_PerformBtn
	"KONFIGURERA",				// Prt_ConfBtn   
	"KONFIGURATIONS RAPPORT",	// Config_RPT
	"TERMINAL",					// Terminal_Tab 
	"UTFÖRDARE",				// Issuer_Tab 
	"INLÖSARE",					// Acq_Tab 
	"KORT",						// Card_Tab 
	"AllA",						// All_Tab
	"BIBLIOTEK VERSION",		// N_LibsVersion
	"SDK",						// N_SDKVersion
	"O/S VER",					// N_OSVersion
	"O/S REV",					// N_OSRevision
    "HFC",                      // N_HFCVersion
	"HISTORIA RAPPORT",			// History_RPT
	"filläsn. fel: ",			// Msg_freaderror
	"Entry # Överskr Max",		// N_EntryNumExceedsMax
	"Tabell ID: ",				// N_TableId
	"HFS",						// N_HFS
	"Ej tillr. utr. i HFS",		// N_NoEnougthSpaceInHFS
	"LÖSENORD ÖVERVAKARE",		// SVPassword
	"RING VERIF. CENTER",       // CallAuthCentre
	"VERIF KOD ?",              // AuthCode
    "KONTANTER EJ TILLÅTET"		,   // N_CashNotAllowed
	"DRA KORT EJ TILLÅTET"		,	// SwipeNotAllowed
	"TRANS. EJ TILLÅTET"		,	// TranNotAllowed
    "INIT MAX FÖRSÖK",			// InitMaxTries
	"Upp",						// N_Up
	"Ner",						// N_Down
	"Val",						// N_Select
	"Skrv om",					// N_ReprintShort
	"HUVUD",					// Msg_HeaderLine
	"Vertik",					// N_Veritcal
	"Horis"	,					// N_Horizontal
	"LINJE",					// N_Modem		 
	"COM1",						// N_RS232RS0	 
	"COM2",						// N_RS232RS1	 
	"COMMS: CU ADDRESS",		// N_RS232       
	"COMMS CONFIG",				// N_RS232Config 
	"COMMS: INTERFACE",			// N_EnableComm  
	"COMMS: PROTOCOL",			// N_RS232Prot	 
	"COMMS: BIT RATE, bps",		// N_RS232Baud   
	"1200",						// N_Baud1200    
	"2400",                    	// N_Baud2400    
	"4800",                     // N_Baud4800    
	"9600",                     // N_Baud9600    
	"19200",                    // N_Baud19200   
	"38400",                    // N_Baud38400   
	"57600",                    // N_Baud57600   
	"115200",					// N_Baud115200  
	"COMMS: STOP BITs",         // N_RS232Stop   
	"1 bit",					// N_RS232Stop1  
	"2 bitar",					// N_RS232Stop2  
	"COMMS: DATA BITs",			// N_RS232Data   
	"7 bitar",					// N_RS232Data7  
	"8 bitar",					// N_RS232Data8  
	"COMMS: PARITY",			// N_RS232Parity 
	"Ingen",					// N_RS232None   
	"Udda",						// N_RS232Odd    
	"Jämn",						// N_RS232Even   
	"TECKEN KARTA",				// N_CharMap	 
    "UPPRINGNINGS MEDDELANDEN", // N_DialMsgs
	"COMMS: FLOW CONTROL",		// N_RS232FlowC	 
    "RX",                       // N_RX
    "RX / TX",                  // N_RXTX
    "COMMS: MODEM MODE",        // N_CommsModemMode
    "COMMS: DB MODEM MODE",     // N_CommsDBModemMode
    "COMMS CHANGED",            // N_CommsChanged  
    "STARTA OM TERMINAL",       // N_RestartTerminal
    "PRESS CANCEL TO EXIT",     // MsgPressCancelToExit - NEED TO BE TRANSLATED!!!
    "CHANGE APP",               // N_ChangeApp          - NEED TO BE TRANSLATED!!!  
    "SELECT APP",               // N_SelectApplication  - NEED TO BE TRANSLATED!!!
    "TRACE LOG",                // N_TraceLog           - NEED TO BE TRANSLATED!!!
    "START NEW",                // N_StartNew           - NEED TO BE TRANSLATED!!!
    "SEND VIA COM1",            // N_Send1
    "SEND VIA COM2",            // N_Send               - NEED TO BE TRANSLATED!!!
    "STOP",                     // N_Stop               - NEED TO BE TRANSLATED!!!
    "TRACE IS EMPTY",           // N_TraceIsEmpty       - NEED TO BE TRANSLATED!!! 
    "*** TRACING ***",          // Tracing              - NEED TO BE TRANSLATED!!!
    "** TRACE OVERFLOW **",     // TraceOverflow        - NEED TO BE TRANSLATED!!!
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
    "LAST IP ADDRESS",          // N_LastIPaddr        - NEED TO BE TRANSLATED!!!
    "BATTERY LOW",              // BatteryLow          - NEED TO BE TRANSLATED!!!
    "UNLOCKING ERROR",          // UnlockingError - NEED TO BE TRANSLATED

#ifdef	MAKE_EMV
	"EMV INFO AKTIVERAD",		// EmvInfoEna
	"EMV INFO DEAKTIVERAD",		// EmvInfoDis
	"DEAKTIVERA EMV INFO ?",	// DisEmvInfo
	"AKTIVERA EMV INFO ?",		// EnaEmvInfo
	"BEARBETA KORT",			// ProcCard
	"SMART KORT",				// SmartCard
	"TA BORT KORT",				// PlsRemoveCared
	"KORT NEKAT",				// CardDeclined
	"PIN MISSLYCKADES",			// PINFailed
	"DRAG/SÄTT I KORT",			// SwipeInsert
	"KORT MISSLYCKADES",		// CardFail
	"NO MORE APPS",				// NoMoreApps
	"KORT BLOCKERAT",			// CardBlocked
	"PIN BLOCKERAD",			// PinBlocked
	"APPLIKATION BLOCKERAD",	// AppBlocked
	"EMV",						// N_EMV
	"APP VAL",					// AppSelection
	"VERIFIERING",				// Authentication
	"EMV DEBUG",				// EMVdebug
	"EJ STÖD FÖR CHIPET",		// ChipNotSupported
	"PÅ PINPAD",				// OnPinpad
	"VÄNTAR PÅ PIN",			// WaitingForPin
    "FÖRBIKOPPLING",            // N_Bypass
    "PINPAD ÄR AKTIVERAD",		// PinpadEP
    "PINPAD ÄR DEAKTIVERAD",	// PinpadDP
	"ACCEPTERAD",               // Accepted

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
#endif	// MAKE_EMV

#ifdef	MAKE_SMARTINIT                
        "SMART INIT ENABLED",         // SmartInitEna 
	"SMART INIT DISABLED",        // SmartInitDis 
	" DISABLE SMART INIT?",      // DisSmartInit 
	" ENABLE SMART INIT ?",      // EnaSmartInit
#endif // MAKE_SMARTINIT
    "SET COM PARAM",        // N_SetComParam
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
    "IPADDRESS",         // N_IpAddress,
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
    "COM PARAMETER", //N_ComParameter

};

//====================================================
//!===  PINPAD MESSAGE TABLE                       ===
//====================================================
const char *const SwPinPadMessages[] = {
	"                ",			//   Msg_ClrScrn
	"    GODKÄNND    ",			//   Msg_Approved
	"* PINPAD TEST * ",			//   Msg_PinTestM
	"     NEKAD      ",			//   Msg_Declined
	"  FEL PIN		 ",			//   Msg_IncrtPin
	"** KORT FEL **	 ",			//   Msg_PinCardErr
	"KORTET STÖDS EJ ",			//   Msg_UnspCard
	"  OGILTIGT KORT ",			//   Msg_InvldCard
	"   FÖRSÖK IGEN  ",			//   Msg_TryAgain
	"REDO            ",			//	 Msg_PinReady
	"BEARBETAR KORT  ",			//	 Msg_ProcCard
	"   ANGE KORT    ",			//   Msg_EnterCard
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
extern void HdtTextSw_Init( void )
{
	short pgnum;

	// Verify the number of messages match
	TableItemCount = MSG_SPECIAL;
	if ( TableItemCount != ( sizeof( SwedishMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "Swedish Messages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Verify the number of messages match
	TableItemCount = LAST_PINM_ID;
	if ( TableItemCount != ( sizeof( SwPinPadMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "SwPinPad Messages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table EnglishMessages into database
	TextIndexAddTable( hdttext_table_id, LNG_SV, SwedishMessages, pgnum );

	// Put text table EnPinPadMessages into database
	TextIndexAddTable( pintext_table_id, LNG_SV, SwPinPadMessages, pgnum );
}


//=============================================================================
// Private function definitions
//=============================================================================

#endif // MAKE_SWEDISH
