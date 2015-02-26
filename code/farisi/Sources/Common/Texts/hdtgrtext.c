//=============================================================================
// Company:
//      Hypercom Inc
//
// Product:
//      Hypercom Base Application
//      (c) Copyright 2006
//
// File Name:
//      hdtgrtext.c
//
// File Description:
//      German message table
//
//=============================================================================

#ifdef MAKE_GERMAN

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
//!===  GERMAN MESSAGE TABLE                       ===
//====================================================
const char *const GermanMessages[] = {
	"CM",						// CustomMsg
	"CM2",						// CustomMsg2
	"CM3",						// CustomMsg3
	"CM4",						// CustomMsg4
	"CM5",						// CustomMsg5
	"CM6",						// CustomMsg6
	"CM7",						// CustomMsg7
	"CM8",						// CustomMsg8
	"KARTE DURCHZIEHEN",		// Swipecard
	"KARTENNUMMER ?",		// EnterCard
	"** KARTEN FEHLER **",			// CardErr
	"KARTE UNBEKANNT",			// UnsuppCard
	"KARTE UNGUELTIG",				// InvalidCard
	"KARTE VERFALLEN",				// ExpiredCard
    "KARTE NOCH NICHT GUEL",            // PrevalidCard @@@@
	"KEINE MAN. EINGABE",		// NoManEntry
    "VERFALLSDATUM MMJJ",     // ExpireDate
    "START DATUM MMJJ",         	// StartDate
    "AUSGABE NUMMER",			    // IssueNum
	"IMPRINT ANFERTIGEN !",		// TakeImprint
	"Letzten 4 Stellen ?",		// EntPan4
	"KUNDEN KARTE",			// CustomerCard
	"KASSIERER NUMMER",		// CashierNum
	"FALSCHE EINGABE",			// InvalidEntry
    "FALSCHER BETRAG",           // InvalidAmount
	"BETRAG",					// AmountEnt
	"BETRAG WIEDERHOLEN",				// AmountAgain
	"BETRAG",				// AmountBase
	"BAR BETRAG",				// AmountBar
	"MWST. BETRAG",				// AmountTax
	"TIP BETRAG",				// AmountTip
	"KAUF BETRAG",				// AmountSale
	"BAR BETRAG",				// AmountCash
	"ANFRAGE BETRAG",			// AmountTender
	"SUMME",					// AmountTotal
	"KORREKT  ?",				// CorrectYN
	"KREDIT PLAN ?",		// CreditPlan
	"ENTER DESCRIPTOR NO",		// EntDescrip
	"RECHNUNGS NUMMER",			// InvoiceNum
	"WARTE AUF GENEHMIGUNG",		// WaitApprov @@@@
	"LEITUNG FREI",				// LineIdle
	"PRUEFE LEITUNG",			// CheckingLine
	"* LEITUNG BELEGT *",		// LineBusy
	"WARTE AUF WAEHLTON",		// WaitDialTone
	"WAEHLEN ..",				// DialNow
	"WARTE AUF ANTWORT",		// WaitForAnswer
	"ERWARTE LEITUNG",			// WaitForLine
	"VERBINDUNGSAUFBAU",			// TrainModem
	"VERARBEITE..",			// ProcNow
	"UEBERTRAGE..",				// Transmitting
	"WAEHLEN..",				// NowDialing
	"INITIALISIERE MODEM",		// InitModem
    "NETWORK LOGON",            // NetworkLogon
    "PAGING",                   // Paging
    "GATE CONNECTING",          // GateConnecting
    "LINKING...",               // Linking
    "VERBINDE...",            // Connecting
	"MEMORY PAGE STATUS:",		// MemPgStatus
	"KONFIGURIERE TERMINAL",		// ConfigTerm
	"ODER HELPDESK ANRUFEN",		// CallHelp
	"BITTE INITIALISIEREN",		// PleaseInit
	" ",						// BlankMsg
	"DRUCKERFEHLER",			// PrintError
	"TRANSAKTION SPEICHER",		// TransMemErr
	"WARTE SELBST-TEST",			// SelfTest
	"NICHT GENUG SPEICHER",		// NotEnoughMem
	"ZUM AUSFUEHREN APPL.",		// ToRunApplic
	"INITIALISIERE",				// InitTran
	"HAENDLER FUNKTION",		// MerchantFunc
	"ABSCHLUSS",					// SettleTran
	"SALDO",					// BalanceTran
	"GUTSCHRIFT",					// RefundTran
	"BELEG NACHDRUCKEN",			// ReprintTran
	"LETZTE",						// ReprintLast
	"RECHNUNG",					// ReprintInvoice
	"** OFF-LINE MODE **",		// OfflineTran
	"BELEG NUMMER ?",		// EnterInvNum
	"EINGABE SOC NUMMER",			// EnterSOCNum
	"P.O. / I.D. NUMMER",		// EnterPONum
	"HAENDLER FUNKTION",		// MercFunction
	"MAIN PASSWORT",		// MaintPswd
	"FALSCHES PASSWORT",	// PswdNotMatch @@@@
	"KORREKT ?",				// CorrectYesNo
	"HAEND.PROG",				// MerchProg
	"PASSWORT EINGABE",			// EnterPswd
	"TERMINAL ID. ",		// EnterTSer
	"INIT. TELEFON NR.",		// EnterInitTel
	"NMS TELEFON NR.",		// EnterNMSTel
	"TON/PULS ?",				// EnterDialMode
    "UNTERSCHRIFT",             	// Signature
	"UNTERSCHRIFT BITTE",		// SignaturePlease
	"und  ENTER druecken",			// PressEnter
	"UNTERSCHRIFT WIEDERHOLEN",			// RetrySigCap
	"NUMMER FUER AMT ? ",			// EnterPABX
	"ENDE- DRUECKE 'CANCEL'",		// ExitPressCncl
	"IDENTISCHE TRANSAKT.",			// DuplicateTrans
	"JA ODER NEIN?",						// AddYesNo
	"BELEG UNBEKANNT",		// NoInvoice
	"BITTE WARTEN",				// PleaseWait
	"TRANSAKTION BEENDET",		// TransComplete
	"TRANSAKTION GENEHMIGT",		// TransAccepted
	"PROGRAMM LADEN",				// ProgamLoad
	"FUNKTION 99 PASSWORT",		// Fun99Password
	"PROGRAMM FUNKTION",			// ProgFunction
	"OPTIONEN WAEHLEN",		// LocalOptSel
	"KOMMUNIKATION SETUP",              // LocalConf
	"MAX.ANZAHL erreicht",	// ExceedMax
	"ENHANCED",                 // EnhDial
	"0-LL 1-DIAL 2,3-LAN",		// LDLPmt
	"CU ADDR",					// CUAddrP
	"AKTUELL",				// Currently
	"ANMELDEN j/N",				// SignOnOff
	"KASSEN-NR. EINGEBEN",		// TillNum
	"ANMELDEN ?",					// SignOnYN
	"STORNO PASSWORT ?",		// VoidPassM
	"GUTSCH.PASSWORT ?",		// RefPassM
	"PASSWORT AENDERN",		// AdjPassM
	"BERICHT PASSWORT ?",		// RepPassM
	"HDSET ANWAHL ERLAUBT",		// KbdDialM
	"TOLL DIAL ALLOWED",		// TollDialM
	"MEMORY (FAST) DIAL",		// MemDialM
	"REFERRAL ANWAHL",		// RefDialM
	"TERMINAL  SPERREN ?",		// KeyLock
	"KONTO-NR. ?",		// EnterAcct
	"ALTES PASSWORT ?",		// OldPassword
	"NEUES PASSWORT ?",		// NewPassword
	"erneut NEUES PASSWORT ?",		// NewPassAgn
	"KEIN DRUCKER",				// NoPrinter
	"KOPIE ANGEFERTIGT",			// ReprintComplete
	"BELEGNR. UNBEKANNT",		// InvNumberMissing
	"DRUCKE...",					// PrintingWait
	"KELLNER ANGEMELDET",		// ServerOn
	"KASSIERER ANGEMELDT",		// CashierOn
	"TASTATUR TEST",			// KeyBrdTest
	"2 * CANCEL FUER ENDE",		// Cancel2End
	"TASTE = ",					// DispKeyP
	"ABSCHLUSS ERFORDERLICH",		// SettlementRequired
	"KARTE DURCHZIEHEN",			// CardRdT
	"DRUCKER EINGESCHALTET",		// PrinterEP
	"DRUCKER ABGESCHALTET",		// PrinterDP
	"EINSCHALTEN ?",				// EnablePP
	"ABSCHALTEN ?",				// DisablePP
	"BATCH DURCHSUCHEN",			// ScanningBatch
	"KEINE BUCHUNGEN",			// NoBatchTotals
	"KEINE WEITEREN SUMMEN",			// NoMoreTotals
	"DRUCKER TEST",				// TestPrnC
	"G E S P E R R T",				// Closed
	"DATUM  MMTTJJ ?",		// DateMDY
	"ZEIT HHMM ?",			// TimeHM
	"HOST AKTUELLER BATCH",		// HostBatch
	"KEIN ACQUIRER",				// NoAcq
	"BATCH ANFANG:",			// StartOfBatch
	"BATCH ENDE:",			// EndOfBatch
	"AUSWAHL SCHICHT NUMMER",		// SelectShiftNum
	"0 FUER ALLE SCHICHTEN",			// ZeroForAllShift
	"KASSE",						// ReportMenuTill
	"KASSIERER",					// ReportMenuCashier
	"KELLNER",					// ReportMenuServer
	"JOURNAL",					// ReportMenuAudit
	"UEBERSICHT",				// ReportMenuSummary
	"O TABS",					// ReportMenuOpenTabs
	"1=DETAIL 3=AKTUELL",		// CashServMenuLine1
	"2=ZUSAMMENFASSUNG",				// CashServMenuLine2
	"STORNO NICHT MOEGLICH",			// VoidNotAllowed
	"HOST NUMMER ?",		// HostNum
	"TYP ?",					// Type
	"AENDERN NICHT ERLAUBT",		// AdjustNotAllowed
	"ABSCHLUSS WIEDERHOLEN",		// RetryBatchTrans
	"TOTAL",					// AmountTotals
	"NEUER BETRAG ?",				// AmountNew
	"BELEG TEXT",		// RecieptTXT
	"BESTAETIGUNGSNUMMER ?",		// EnterApp
	"***TRAINING  MODE***",		// TrainingMode
	"TRAIN MODE aktivieren",		// GoToTrainingMode
	"TRAINING MODE verlassen",		// LeaveTrainingMode
	"GUTSCHR.NICHT ERLAUBT",		// RefundNotAllowed
	"KAUF TOTAL",				// SalesTotal
	"KAUF TOTAL FEHLER",		// SalesTotalsWrong
	"GUTSCHRIFT SUMME",				// RefundTotal
	"GUTSCHRIFT SUM.FEHLER",		// RefundTotalsWrong
	"SUMME",					// BatchTotals
	"FREIGABE",					// BatchRelease
	"TEST TRANS",				// TestTransactions
	"KEINE TRANSAKTIONEN",			// NoTransactions
	"ERHOEHEN",					// IncrementShift
	"RESET",					// ResetShift
	"KOMMUNIKATIONsFEHLER",					// NoComms
    "KEINE LEITUNG",            // NoPhoneline
	"TIP EINGEBEN",				// EnterTip
	"TOTAL EINGEBEN",				// EnterTotal
	"1-OEFFNE TAB 3-DECKEL TAB",		// TabProcMsg1
	"2-SCHLIESSE TAB",				// TabProcMsg2
	"KELLNER NUMMER ?",		// ServerNum
	"SCHLIESSE ALLE DECKEL",		// CloseOpenTabs
	"INVOICE NOT OPEN TAB",		// NotOpenTab
	"KONFIGUR.",				// N_Configure 
	"INITIALIS.",				// N_Initialize
	"HAUPT MENU",                // N_Main
	"BUCHUNG..",					// N_NewTrans
	"BATCH..",					// N_Batch
	"BERICHT..",					// N_Reports
	"TERMINAL..",					// N_Terminal
	"MEHR",						// N_More
	"MENUE",						// N_Menu
	"DATUM/ZEIT",				// N_Datime
	"DRUCKER..",					// N_Printer
	"DFUE..",					// N_Comms
	"waehlen",					// N_Dial
	"PASSWORT",					// N_Passwords
	"SCHNELLWAH",					// N_Speedial
	"KARTE LESE",				// N_CardRdr
	"KONTRAST",					// N_Contras
	"KAUF",						// N_Sale
	"SALE+CASH",				// N_SalCash
	"MAIL ORDER",				// N_MailOrder
	"OFFLINE",                  // N_Force
	"AUTHORIS.",				// N_Auth
	"PRUEFEN",				// N_CardVer
	"ANZEIGEN",					// N_Review
	"SUCHEN",						// N_Find
	"AENDERN",					// N_Adjust
	"STORNO",						// N_Void
	"BAR",						// N_Cash
	"PROG. LOAD",				// N_ProgLoad
	"SCHICHT",					// N_Shift
	"ANZEIGEN..",						// N_View
	"TRAINING",					// N_TrainMode
	"SETUP..",					// N_Setup
	"FUNKTION",					// N_Function
	"TESTS",					// N_Tests
	"TAB",						// N_Tab
	"KOPIE",					// N_Reprint
    "EXIT",                     // N_Exit
	"FEHLER",					// N_Error
	"LOESCH",					// N_Clear
	"ALLE KASSIERER LOESCHEN",		// N_ClearChsTbl
	"ALLE KELLNER LOESCHEN",		// N_ClearSerTbl
	"ENTER",					// N_Enter
	"CANCEL",					// N_Cancel
	"RESET",					// N_Reset
	"QUICK",					// N_Quick
	"+/-",                      // N_AddRmv
	"DETAIL",					// N_Detail
	"ZUSAMMENFASSUNG",			// N_Summary
	"AKTUEL",                   // N_Unadjusted
	"HOST",						// N_Host
	"ANZEIGE",					// N_Display
	"TASTATUR",					// N_Keyboard
	"PINPAD",					// N_Pinpad
	"INIT NUMMER",				// N_InitPhone
	"NMS NUMMER",				// N_NMSPhone
	"WAEHLVERFAHREN",			// N_DialType
	"AMT ?",					// N_PABX
	"TERM ID",					// N_TermId
	"ALLES",					// N_All
	"AN/AUS",					// N_OnOff
	"BELEG",					// N_Receipt
	"TEXT",						// N_Text
	"DRUCKER VORSCHUB",			// N_PaperFeed
	"ENDE",						// N_Quit
	"WECHSEL",					// N_Change
	"",							// N_NullStr
	"KOMMUNIKATION.",			// N_Communications
	"PASSWORT WECHSEL",			// N_PswdChange
	"HOEHER",					// N_Higher
	"TIEFER",					// N_Lower
	"LOKALE OPTION.",			// N_LocalOpts
	"ANTWORT",					// N_Response
	"ANFANG",						// N_Open
	"ENDE",					// N_Close
	"DRUCKEN",					// N_Print
	"SCROLL",					// N_Scroll
	"FUNKTION ?",			// N_EnterFunction
	"HOST REPORT",					// N_PrintHost
	"DB RPT",					// N_DBRpt
	"TX/RX REPORT",				// N_TxRxRpt
	"ANSICHT TABELLE",				// N_ViewTable
	"ANZEIGE TEST",				// N_DisplayTest
	"FUNKTION NICHT vorh.",	// N_FuncNotSupported
	"VERSION",					// N_Version
	"LETZTE ANTWORT",			// N_LastRsp
	"LETZTE ANTWORT",				// N_Last
	"KEINE DATEN",					// N_NoData
	"BATCH INFO",				// N_BatchInfo
	"HOST ID   : ",				// N_HostId
	"HOST NAME : ",				// N_HostName
	"BATCH NR  : ",				// N_BatchNumber
	"0 = ALLE HOST",			// N_ZeroForAllHost
	"BERICHT",					// N_Report
	"DEBIT",					// N_Debit
	"KREDIT",					// N_Credit
	"BESTAETIGUNGS CODE ?",			// N_EnterAuth
	"GENERATION NUMMER",		// N_EnterGenNum
	"JOURNAL TEST",			// JournalTesting
	"ANZAHL EINTRAEGE ?",		// NumEntries
	"PAPIER LEER",				// PaperOut
    "PRINTER OVERHEAT",         // PrinterOverheat - NEED TO BE TRANSLATED !!
	"WARTE AUF KONTO NR.",		// WaitingForAcct
	"WARTE AUF BESTAETIGUNG",		// WaitingForApproval
	"WARTE AUF PIN",			// WaitingForPin
	"KARTE AM PINPAD",		// SwipeCardPin
	"-> LESER AM PINPAD ?",		// ChangeToPinRead
	"-> LESER AM TERMINAL ?",		// ChangeToTermRead
	"UNGUELTIGES KONTO",			// InvalidAccount
	"VERFUEGBARER BETRAG",				// AvailBal
	"     HEUTE",				// Today
	"KARTEN-NR nicht gleich",		// CardNoMatch
	"PINPAD TEST",				// PinPadTest
	"PINPAD AUTO DETEKT",		// PinPadAutoDetect
	"AUSWAHL KONTO ?",			// SelectAccount
	"KONTO",					// N_Account
	"0",						// N_0
	"1",						// N_1
	"2",						// N_2
	"3",						// N_3
	"4",						// N_4
	"PIN EINGEBEN",				// EnterPin
	"KEIN EXTERNES PINPAD",		// ExtPinNot
	"ENCRYPTION FEHLER",			// EncryptError
	"LOGON",					// N_Logon
	"WARTUNG",				// MaintFunction
	"EINGABE ORGINAL DATUM",		// N_OrigDate
	"IDENTIFIKATION",			// N_Identification
	"SOFTWARE",					// N_Software
	"DLL",						// N_DLL
	"BOOTER",					// N_EPROM
	"Tel.BUCHUNG",					// N_Referral
	"ALLE HOSTS",				// N_AllHosts
	"ICEPAC",					// N_IcePac
	"ALLES -> ENTER",			// PressEnterForAll
	"FUER LETZTEN BELEG",			// ForLastInv
	"ZEIT UEBERSCHRITTEN",					// N_Timeout
	"BENUTZER ABBRUCH",			// N_UserCancelled
	"STORNO",					// N_Reversals
	"PINPAD NICHT GEFUNDEN",			// PinpadNotFound
	"JA",						// N_YES
	"NEIN",						// N_NO
	"OK",						// N_OK
	"QUIT",						// N_QUIT
	"NÄCHST",					// N_NEXT
	"KARTE stecken/durchz.",     // N_SwipeOrInsertCard
	"X",						// N_X
	"WEITER",					// N_Continue
	"TASTE",						// N_KEY
	"ANGENOMMEN",					// N_Passed
	"ABGELEHNT",					// N_Failed
	"TEST",						// N_Test
	"TON",						// N_Tone
	"PULS",					// N_Pulse
	"LESEFEHLER PINPAD",		// PinReadErr
	"SCHREIBFEHLER PINPAD",		// PinWriteErr
	"ABMELDEN?",				// SignOffYN
	"JAN",						// Month_1
	"FEB",						// Month_2
	"MAR",						// Month_3
	"APR",						// Month_4
	"MAI",						// Month_5
	"JUN",						// Month_6
	"JUL",						// Month_7
	"AUG",						// Month_8
	"SEP",						// Month_9
	"OKT",						// Month_10
	"NOV",						// Month_11
	"DEZ",						// Month_12
	"PIN WIEDERHOLEN",				// Msg_Reenter_Pin
	"PIN ?",						// Msg_Pin
	"EXTERN ",				// Msg_External
	"INTERN ",				// Msg_Internal
	"CHECKSUMME: ",					// Msg_Chksum
	"KORREKTUR ?",				// Msg_Correct
	"SVR:",						// Msg_SVR
	"CSH:",						// Msg_CSH
	"BELEG:",						// Msg_INV
	"STORNO",					// Msg_reversal
	"STORNIERT",					// Msg_voided
	"advice",					// Msg_advice
	"SEQUENCE NO.: ",			// Msg_SequenceNo
	"POSITION",					// Msg_Items
	"BUCHUNG",					// Msg_Sales
	"FEHLER **********",			// Msg_Error
	"verfuegbare HOSTS",			// Msg_AvailHosts
	"BAR    : ",				// Msg_Cash
	"OFFERTE: ",				// Msg_Tender
	"MOEGLICHER BETRAG",				// AmountOpt
	"CVV2 INDICATOR",			// N_CVV2Indicator
	"0=WEITER",					// N_CVV2IndEntry1
	"1=EINGABE",				// N_CVV2IndEntry2
	"2=UNLESBAR",				// N_CVV2IndEntry3
	"9=FEHLEND",					// N_CVV2IndEntry4
	"CVV2 von KARTE",			// N_CVV2Entry
	"KARTE VERFUEGBAR ?",			// Msg_CardPresent
	"KOPFZEILE 4",					// Msg_HeaderLine
	"FUSSZEILE ",					// Msg_FooterLine
	"KARTE DURCHZIEHEN",			// Msg_SwipeYourCard
	"TIP ?",		// Msg_PreTip
	"ANDERE",					// Msg_Other
	"PROGRAMMFEHLER:",			// Msg_ProgramError
	"TIP",						// Msg_Tips
	"TIP    %",					// Msg_TipPct
	"% - VORGABE",	// Msg_Sug_Tip
	"LOCAL OPT",				// Msg_LocalOpt
	"TX/RX FEHLER",				// Msg_TxRxErrors
	"SENDE FEHLER:",			// Msg_TranErrors
	"EMPFANGS FEHLER:",			// Msg_RecvErrors
	"DATEI LADEN",			// N_FileDownLoad
	"ABSCHLUSS",					// N_Settle
	"OFFLINE",					// N_Offline
	"TRX TYP",					// N_Trans
	"DATUM/ZEIT",			// N_DateTimeSetup
	"KONTRAST",					// N_Contrast
	"PAPIER VORSCHUB",					// N_PaperFd
	"STATUS ABSCHLUSS",		// N_SettleStat
	"REPORT WAEHLEN",	// Msg_ChooseSummary
	"AUTO   ",					// Msg_Auto
	"GRUPPE   ",					// Msg_Group
	"MANUAL   ",				// Msg_Manual
	"KEINE",					// Msg_Never
	"VERKAUFSPOSITION",				// Msg_SaleItems
	"GUTSCHRIFT",				// Msg_RefundItems
	"VERSUCH NR.",				// Msg_Attempt
	"ANTWORT ",					// Msg_Resp
	"ALLES",					// Msg_Evrythg
	"ALLES",				// Msg_Everything
	"KELLNER TABELLE LEER",		// Msg_SrvTabEmpty
	"KASSIE. TABELLE LEER",		// Msg_CshTabEmpty
	"STATUS",					// Msg_Status
	"fopen error: ",			// Msg_fopenerror
	"Down loading file: ",		// Msg_DownloadingFile
	"SPRACHE WAEHLEN",			// Msg_SelectLanguage
	"SPRACHE",					// Msg_Language
	"ENGLISCH",					// Lng_English
	"DEUTSCH",					// Lng_German
	"SWEDISCH",					// Lng_Swedish
	"SPANISCH",					// Lng_Spanish
	"PORTUG.",				// Lng_Portugues
	"AKTUELLE SPRACHE",	// Msg_CurrentLang
	"SPRACHE",						// Msg_Lang
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
	"BITTE BELEG ENTNEHMEN",     // RemoveMerchantCopy
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
	"SUPERVISOR PASSWORT",      // SVPassword
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
    "APP.WECHS.",               // N_ChangeApp
    "APP. ASUWAEHLEN",          // N_SelectApplication
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
    "UNLOCKING ERROR",          // UnlockingError - NEED TO BE TRANSLATED

#ifdef	MAKE_EMV
	"EMV INFO ENABLED",         // EmvInfoEna 
	"EMV INFO DISABLED",        // EmvInfoDis 
	" DISABLE EMV INFO ?",      // DisEmvInfo 
	"  ENABLE EMV INFO ?",      // EnaEmvInfo
	"VERARBEITE KARTE",			// ProcCard
	"SMART KARTE",				// SmartCard
	"BITTE KARTE ENTNEHMEN",		// PlsRemoveCared
	"KARTE ABGELEHNT",			// CardDeclined
	"PIN FALSCH",				// PINFailed
	"KARTE STECKEN/ZIEHEN",		// SwipeInsert
	"CARD FAIL",				// CardFail
	"NO MORE APPS",				// NoMoreApps
	"KARTE GESPERRT",				// CardBlocked
	"PIN GESPERRT",				// PinBlocked
	"APPLI: GESPERRT",		// AppBlocked
	"EMV",						// N_EMV
	"APP SELEKTION",			// AppSelection
	"AUTHENTIKATION",			// Authentication
	"EMV DEBUG",				// EMVdebug
	"CHIP NOT SUPPORTED",		// ChipNotSupported
	"AUF PINPAD",				// OnPinpad
	"WARTE AUF PIN",			// WaitingForPin
    "BYPASS",                   // N_Bypass 
    "PINPAD IS ENABLED",		// PinpadEP 
    "PINPAD IS DISABLED",		// PinpadDP 
	"AKZEPTIERT",                 // Accepted

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

#ifdef MAKE_FARSI
    "PERSIAN", // Lng_Farsi						
#endif
};

//====================================================
//!===  PINPAD MESSAGE TABLE                       ===
//====================================================
const char *const GrPinPadMessages[] = {
	"                 ",			//   Msg_ClrScrn
	"   BESTAETIGT    ",			//   Msg_Approved
	"* PINPAD TEST *  ",			//   Msg_PinTestM
	"    ABGELEHNT    ",			//   Msg_Declined
	"   FALSCHER PIN  ",			//   Msg_IncrtPin
	"* KARTEN FEHLER *",			//   Msg_PinCardErr
	"  FALSCHE KARTE  ",			//   Msg_UnspCard
	"UNGUELTIGE KARTE ",			//   Msg_InvldCard
	"Bitte WIEDERHOLEN",			//   Msg_TryAgain
	"FERTIG           ",         //	 Msg_PinReady
	"VERARBEITUNG     ",         //	 Msg_ProcCard
	"KARTE EINGEBEN   ",         //   Msg_EnterCard
	"TABLE IS EMPTY   "			//	 Msg_InternalTablesAreEmpty
};


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add HDT and pinpad Germany messages tables for DML support.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
extern void HdtTextGr_Init( void )
{
	short pgnum;

	// Verify the number of messages match
	TableItemCount = MSG_SPECIAL;
	if ( TableItemCount != ( sizeof( GermanMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "German Messages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Verify the number of messages match
	TableItemCount = LAST_PINM_ID;
	if ( TableItemCount != ( sizeof( GrPinPadMessages ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "GrPinPad Messages" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table EnglishMessages into database
	TextIndexAddTable( hdttext_table_id, LNG_DE, GermanMessages, pgnum );

	// Put text table EnPinPadMessages into database
	TextIndexAddTable( pintext_table_id, LNG_DE, GrPinPadMessages, pgnum );
}


//=============================================================================
// Private function definitions
//=============================================================================

#endif // MAKE_GERMAN
