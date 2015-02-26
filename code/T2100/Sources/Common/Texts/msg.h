
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002, 2004
//=============================================================================
#ifndef _MSG_H_
#define _MSG_H_
//=============================================================================
//
// Module overview:             msg.h
//      Message defintions
//
//=============================================================================



//=============================================================================
// Public defines and typedefs
//=============================================================================
// Message Text Identifiers
// Messages are defined in hdtentext.c, hdtestext.c, hdtpttext.c, hdtswtext.c

enum msg_id
{
	CustomMsg,
	CustomMsg2,
	CustomMsg3,
	CustomMsg4,
	CustomMsg5,
	CustomMsg6,
	CustomMsg7,
	CustomMsg8,
	Swipecard,
	EnterCard,
        InsertCard, 
	CardErr,
	UnsuppCard,
	InvalidCard,
	ExpiredCard,
        PrevalidCard,
	NoManEntry,
	ExpireDate,
        StartDate,
    IssueNum,
	TakeImprint,
	EntPan4,
	CustomerCard,
	CashierNum,
	InvalidEntry,
    InvalidAmount,
	AmountEnt,
	AmountAgain,
	AmountBase,
	AmountBar,
	AmountTax,
	AmountTip,
	AmountSale,
	AmountCash,
	AmountTender,
	AmountTotal,
	CorrectYN,
	CreditPlan,
	EntDescrip,
	InvoiceNum,
	WaitApprov,
	LineIdle,
	CheckingLine,
	LineBusy,
	WaitDialTone,
	DialNow,
	WaitForAnswer,
	WaitForLine,
	TrainModem,
	ProcNow,
	Transmitting,
	NowDialing,
	InitModem,
    NetworkLogon,
    Paging,
    GateConnecting,
    Linking,
    Connecting,
	MemPgStatus,
	ConfigTerm,
	CallHelp,
	PleaseInit,
	BlankMsg,
	PrintError,
	TransMemErr,
	SelfTest,
	NotEnoughMem,
	ToRunApplic,
	InitTran,
	MerchantFunc,
	SettleTran,
	BalanceTran,
	RefundTran,
	ReprintTran,
	ReprintLast,
	ReprintInvoice,
	OfflineTran,
	EnterInvNum,
	EnterSOCNum,
	EnterPONum,
	MercFunction,
	MaintPswd,
	PswdNotMatch,
	CorrectYesNo,
	MerchProg,
	EnterPswd,
	EnterTSer,
	EnterInitTel,
	EnterNMSTel,
	EnterDialMode,

	EnterIpConfig,
	EnterNMSIp,
	EnterNMSPort,
	EnterInitIp,
	EnterInitPort,
	EnterLocalIp,
	EnterLocalSubnet,
	EnterLocalGateway,

    Signature,
	SignaturePlease,
	PressEnter,
	RetrySigCap,
	EnterPABX,
	ExitPressCncl,
	DuplicateTrans,
	AddYesNo,
	NoInvoice,
	PleaseWait,
	TransComplete,
	TransAccepted,
	ProgramLoad,
	Fun99Password,
	ProgFunction,
	LocalOptSel,
	LocalConf,
	ExceedMax,
	EnhDial,
	LDLPmt,
	CUAddrP,
	Currently,
	SignOnOff,
	TillNum,
	SignOnYN,
	VoidPassM,
	RefPassM,
	AdjPassM,
	RepPassM,
	KbdDialM,
	TollDialM,
	MemDialM,
	RefDialM,
	KeyLock,
	EnterAcct,
	OldPassword,
	NewPassword,
	NewPassAgn,
	NoPrinter,
	ReprintComplete,
	InvNumberMissing,
	PrintingWait,
	ServerOn,
	CashierOn,
	KeyBrdTest,
	Cancel2End,
	DispKeyP,
	SettlementRequired,
	CardRdT,
	PrinterEP,
	PrinterDP,
	EnablePP,
	DisablePP,
	ScanningBatch,
	NoBatchTotals,
	NoMoreTotals,
	TestPrnC,
	Closed,
	DateMDY,
	TimeHM,
	HostBatch,
	NoAcq,
	StartOfBatch,
	EndOfBatch,
	SelectShiftNum,
	ZeroForAllShift,
	ReportMenuTill,
	ReportMenuCashier,
	ReportMenuServer,
	ReportMenuAudit,
	ReportMenuSummary,
	ReportMenuOpenTabs,
	CashServMenuLine1,
	CashServMenuLine2,
	VoidNotAllowed,
	HostNum,
	Type,
	AdjustNotAllowed,
	RetryBatchTrans,
	AmountTotals,
	AmountNew,
	RecieptTXT,
	EnterApp,
	TrainingMode,
	GoToTrainingMode,
	LeaveTrainingMode,
	RefundNotAllowed,
	SalesTotal,
	SalesTotalsWrong,
	RefundTotal,
	RefundTotalsWrong,
	BatchTotals,
	BatchRelease,
	TestTransaction,
	NoTransactions,
	IncrementShift,
	ResetShift,
	NoComms,
    NoPhoneLine,
	EnterTip,
	EnterTotal,
	TabProcMsg1,
	TabProcMsg2,
	ServerNum,
	CloseOpenTabs,
	NotOpenTab,
	N_Configure,
	N_Initialize,
	N_Main,
	N_NewTrans,
        N_Refund,
	N_Batch,
	N_Reports,
	N_Terminal,
	N_More,
	N_Menu,
	N_Datime,
	N_Printer,
	N_Comms,
	N_Dial,
	N_Passwords,
	N_Speedial,
	N_CardRdr,
	N_Contras,
	N_Sale,
	N_SalCash,
	N_MailOrder,
	N_Force,
	N_Auth,
	N_CardVer,
	N_Review,
	N_Find,
	N_Adjust,
	N_Void,
	N_Cash,
	N_ProgLoad,
	N_Shift,
	N_View,
	N_TrainMode,
	N_Setup,
	N_Function,
	N_Tests,
	N_Tab,
	N_Reprint,
	N_Error,
	N_Clear,
	N_ClearChsTbl,
	N_ClearSerTbl,
	N_Enter,
	N_Cancel,
	N_Reset,
        N_ResetMenu,
	N_Quick,
	N_AddRmv,
	N_Detail,
	N_Summary,
	N_Unadjusted,
	N_Host,
	N_Display,
	N_Keyboard,
	N_Pinpad,
	N_InitPhone,
	N_NMSPhone,
	N_DialType,
	N_PABX,

	N_IpCon,
	N_NMSIp,
	N_NMSPort,
	N_InitIp,
	N_InitPort,
	N_LocalIp,
	N_LocalSubnetMask,
	N_GatewayIp,

	N_IpAuto,
	N_IpManual,

	N_TermId,
	N_All,
	N_OnOff,
	N_Receipt,
	N_Text,
	N_PaperFeed,
	N_Quit,
	N_Change,
	N_NullStr,
	N_Communications,
	N_PswdChange,
	N_Higher,
	N_Lower,
	N_LocalOpts,
	N_Response,
	N_Open,
	N_Close,
	N_Print,
	N_Scroll,
	N_EnterFunction,
	N_PrintHost,
	N_DBRpt,
	N_TxRxRpt,
	N_ViewTable,
	N_DisplayTest,
	N_FuncNotSupported,
	N_Version,
	N_LastRsp,
	N_Last,
	N_NoData,
	N_BatchInfo,
	N_HostId,
	N_HostName,
	N_BatchNumber,
	N_ZeroForAllHost,
	N_Report,
	N_Debit,
	N_Credit,
	N_EnterAuth,
	N_EnterGenNum,
	JournalTesting,
	NumEntries,
	PaperOut,
    PrinterOverheat,
	WaitForAcct,
	WaitForApproval,
	WaitForPin,
	SwipeCardPin,
	ChangeToPinRead,
	ChangeToTermRead,
	InvalidAccount,
	AvailBal,
	Today,
	CardNoMatch,
	PinPadTest,
	PinPadAutoDetect,
	SelectAccount,
	N_Account,
	N_0,
    N_1,
	N_2,
	N_3,
	N_4,
	EnterPin,
	ExtPinNot,
	EncryptError,
	N_Logon,
	MaintFunction,
	N_OrigDate,
	N_Identification,
	N_Software,
	N_DLL,
	N_EPROM,
	N_Referral,
	N_AllHosts,
	N_IcePac,
	PressEnterForAll,
	ForLastInv,
	N_Timeout,
	N_UserCancelled,
	N_Reversals,
	PinpadNotFound,
	N_YES,
	N_NO,
	N_OK,
	N_QUIT,
	N_NEXT,
	N_SwipeOrInsertCard,
	N_X,
	N_Continue,
	N_KEY,
	N_Passed,
	N_Failed,
	N_Test,
	N_Tone,
	N_Pulse,
	PinReadErr,
	PinWriteErr,
	SignOffYN,
	Month_1,
	Month_2,
	Month_3,
	Month_4,
	Month_5,
	Month_6,
	Month_7,
	Month_8,
	Month_9,
	Month_10,
	Month_11,
	Month_12,
	Msg_Reenter_Pin,
	Msg_Pin,
	Msg_External,
	Msg_Internal,
	Msg_Chksum,
	Msg_Correct,
	Msg_SVR,
	Msg_CSH,
	Msg_INV,
	Msg_reversal,
	Msg_voided,
	Msg_advice,
	Msg_SequenceNo,
	Msg_Items,
	Msg_Sales,
	Msg_Error,
	Msg_AvailHosts,
	Msg_Cash,
	Msg_Tender,
	AmountOpt,
	N_CVV2Indicator,
	N_CVV2IndEntry1,
	N_CVV2IndEntry2,
	N_CVV2IndEntry3,
	N_CVV2IndEntry4,
	N_CVV2Entry,
	Msg_CardPresent,
	Msg_HeaderLine,
	Msg_FooterLine,
	Msg_SwipeYourCard,
	Msg_PreTip,
	Msg_Other,
	Msg_ProgramError,
	Msg_Tips,
	Msg_TipPct,
	Msg_Sug_Tip,
	Msg_LocalOpt,
	Msg_TxRxErrors,
	Msg_TranErrors,
	Msg_RecvErrors,
	N_FileDownLoad,
	N_Settle,
	N_Offline,
	N_Trans,
	N_DateTimeSetup,
	N_Contrast,
	N_PaperFd,
	N_SettleStat,
	Msg_ChooseSummary,
	Msg_Auto,
	Msg_Group,
	Msg_Manual,
	Msg_Never,
	Msg_SaleItems,
	Msg_RefundItems,
	Msg_Attempt,
	Msg_Resp,
	Msg_Evrythg,
	Msg_Everything,
	Msg_SrvTabEmpty,
	Msg_CshTabEmpty,
	Msg_Status,
	Msg_fopenerror,
	Msg_DownloadingFile,
	Msg_SelectLanguage,
	Msg_language,
	Lng_English,
        Lng_Turkish,
	Lng_German,
    Lng_Swedish,
	Lng_Spanish,
	Lng_Portugues,
	Msg_CurrentLang,
	Msg_Lang,
	Msg_KeyTableInvalid,
	Prt_DatabaseReport,
	Prt_Dashes,
	Prt_TermEpromCfg,
	Prt_TermConfig,
	Prt_AcquirerID,
	Prt_IssuerID,
	Prt_CardRangeID,
	Prt_DescriptorID,
	Prt_ReceivedData,
	Prt_SendData,
	Prt_RawDataHexBytes,
	WaitingForLanCable,
	LineModeCanceled,
	RestartTerminal,
	N_Only09AFvalid,
	N_IdleButton,
	SelectButton,
	EventType,
	N_Button,
	ConfirmTrans,
	ConfirmFunc,
	N_InitButTab,
	Init_Dis,
	Init_Ena,
	N_IdlBtn,
	N_InitIdlBtn,
	N_ChgIdlBtn,
	Chang_Btn1,
	Chang_Btn2,
	Chang_Btn3,
	Chang_Btn4,
	B_1,
	B_2,
	B_3,
	B_4,
	RemoveMerchantCopy,
	PclTabError,
	Prt_PerformBtn,
	Prt_ConfigBtn,
	Config_RPT,
	Terminal_Tab,
	Issuer_Tab,
	Acq_Tab,
	Card_Tab,
	All_Tab,
	N_LibsVersion,
	N_SDKVersion,
	N_OSVersion,
	N_OSRevision,
	History_RPT,
	Msg_freaderror,
	N_EntryNumExceedsMax,
	N_TableId,
	N_HFS,
	N_NoEnougthSpaceInHFS,
	SVPassword,
	CallAuthCentre,
	AuthCode,
	N_CashNotAllowed,
	SwipeNotAllowed,
	TranNotAllowed,
    InitMaxTries,
	N_Up,
	N_Down,
	N_Select,
	N_ReprintShort,
	Msg_HeaderLineShort,
    N_Veritcal,
	N_Horizontal,
	N_Modem,
	N_RS232RS0,
	N_RS232RS1,
	N_RS232,
	N_RS232Config ,
	N_EnableComm,
	N_RS232Prot,
	N_RS232Baud,
	N_Baud1200,
	N_Baud2400,
	N_Baud4800,
	N_Baud9600,
	N_Baud19200,
	N_Baud38400,
	N_Baud57600,
	N_Baud115200,
	N_RS232Stop,
	N_RS232Stop1,
	N_RS232Stop2,
	N_RS232Data,
	N_RS232Data7,
	N_RS232Data8,
	N_RS232Parity,
	N_RS232None,
	N_RS232Odd,
	N_RS232Even,
	N_CharMap,
    N_DialMsgs,
    N_RS232FlowC,
    N_RX,
    N_RXTX,
    N_CommsModemMode,
    N_CommsDBModemMode,
    N_CommsChanged,
    N_RestartTerminal,
    MsgPressCancelToExit,
    N_ChangeApp,
    N_SelectApplication,
    N_TraceLog,
    N_StartNew,
    N_Send1,
    N_Send,
    N_Stop,
    N_TraceIsEmpty,
    Tracing,
    TraceOverflow,
	N_GSM,
    N_Ethernet,
    N_LAN,
    N_Bluetooth,
    N_ISDN,
    N_DialBackUp, //@AAMELIN
	EnterDBInitTel,	//@AAMELIN
	EnterDBNMSTel, //@AAMELIN
	InitPrefService, //@AAMELIN
	NMSPrefService, //@AAMELIN
	LowSignal, //@AAMELIN
	N_DBInitPref, //@AAMELIN
	N_DBNMSPref, //@AAMELIN
	N_DBInitPhone, //@AAMELIN
	N_DBNMSPhone,//@AAMELIN
	N_GSMwDial, //@AAMELIN
	N_DialwGSM, //@AAMELIN
	N_GSMonly, //@AAMELIN
	N_Dialonly, //@AAMELIN
	StartGSMModem, //@AAMELIN
	ProcWasCanel,	// @AAMELIN
	NoSIM,	// @AAMELIN
	ForbSIMStatus,	// @AAMELIN
	Msg_InsertYourCard,// @AAMELIN
    N_LastIPaddr,
    BatteryLow,
    UnlockingError,

#ifdef	MAKE_EMV
	EmvInfoEna,
	EmvInfoDis,
	DisEmvInfo,
	EnaEmvInfo,
	ProcCard,
	SmartCard,
	PlsRemoveCard,
	CardDeclined,
	PINFailed,
	SwipeInsert,
	CardFail,
	NoMoreApps,
	CardBlocked,
	PinBlocked,
	AppBlocked,
	N_EMV,
	AppSelection,	
	Authentication,
	EMVdebug,	
	ChipNotSupported,
	OnPinpad,
	WaitingForPin,
	N_Bypass,
        PinpadEP,
        PinpadDP,
        Accepted,
       
        EASY_ENTRY,
        LAST_TRY_LEFT,
        TRIES_LEFT,
        CARD_FAIL,
        PIN_TRIES_EXCEEDED,
        PIN_BLOCKED,
        LAST_PIN_TRY_LEFT,
        PRESS_OK_TO_CONTINUE,
        SELECTING,
        BLOCKED_MSG,
        CHIP_CARD,
        PLEASE_INSERT_CARD,
        CONTINUE_MSG,

        APP_BLOCKED, 
        LIST_EMPTY,
        APPLICATION_SELECT,
        SELECT_MSG,
        CARD_DECLINE,
        OVERRIDE_MSG,
        UPDFIELDSPOSTGENAC_MSG,   //@A - BC
        REFERRALDECLINE_MSG,      //@A - BC
        CARDREFERRAL_MSG,         //@A - BC
        AUTHORISED_MSG,           //@A - BC
        POSTGENACFAIL_MSG,        //@A - BC
        DECLINED_MSG,             //@A - BC
        RESULT_MSG,               //@A - BC
        EASYENTRY_MSG,            //@A - BC 
        MNDTTDATAMISSING_MSG,     //@A - BC 
        FORCEONLINE_MSG,          //@A - BC 
        ISFORCEONLINE_MSG,        //@A - BC 
        ISAUTHCODEOBTAINED_MSG,   //@A - BC 
        REFERRAL_MSG,             //@A - BC 
#endif	// MAKE_EMV
        
#ifdef	MAKE_SMARTINIT      
	SmartInitEna,
	SmartInitDis,
	DisSmartInit,
	EnaSmartInit,
#endif // MAKE_SMARTINIT      
      TerminalNotAllowed,     // @ydeniz --  BC -- 
      TranUnsupported,        // @ydeniz --  BC -- 
      N_Installment,          // @ydeniz --  BC -- 
      YYYY_MM_DD,             // @ydeniz --  BC -- 
      TarihGir,               // @ydeniz --  BC -- 
      N_InstalRefund,         // @ydeniz --  BC --  
      N_PreAuth,              // @ydeniz --  BC --  
      N_PreAuth_Long,         // @ydeniz --  BC --  
      N_ClosedPreAuth,        // @ydeniz --  BC --  
      N_CancelPreAuth,        // @ydeniz --  BC --  
      N_CancelClosedPreAuth,  // @ydeniz --  BC --  
      N_ClickKey,             // @ydeniz --  BC --  
      N_SecondReceipt,        // @ydeniz --  BC --  
      Msg_UseMagneticStripe,  // @ydeniz --  BC -- 
      Msg_NoManEntry,         // @ydeniz --  BC --   
      Msg_OverInsNum,         // @ydeniz --  BC -- 
      Msg_LowerInsNUm,        // @ydeniz --  BC -- 
      InstNum,                // @ydeniz --  BC -- 
      N_DBFormattedReport,    // @erd 06.03.2007  
      N_AcqTableReport,       // @erd 06.03.2007
      N_TermTableReport,      // @erd 06.03.2007
      N_IssTableReport,       // @erd 06.03.2007
      N_CardTableReport,      // @erd 06.03.2007
      N_SelectKey,
      Msg_WrongCard,          // @y
      Msg_PreAuthNotAllowed,
      Msg_ClosedPreAuthNotAllowed,
      Msg_InstallmentNotAllowed, 
      Msg_MailOrderNotAllowed,
      N_SaleRefund,
      N_LastSettle,
      N_BalanceInq,
#ifdef MAKE_KIB 
      N_LoyaltyInquiry,
      N_LoyaltyList,
      N_SaleWithLoyalty, 
      LoyaltyAmountEnt,
      Msg_NotPermitLoyIns,
      N_InstallmentWithLoyalty,
      N_Discount, 
      N_DiscountSale, 
      N_DiscountInst, 
      N_RateEntry,
      Msg_OverDiscount,
      N_Point,
      N_Disc,            
#endif
      
      
// *****************************************************************
// below this point are message id's with special operations, 
// see void GETMSG(void)    MSG_SPECIAL,
// *****************************************************************
	MSG_SPECIAL,
	N_Ctxt1 = 2900,
	N_Ctxt2,
	N_Ctxt3,
	N_Ctxt4,
	N_Ctxt5,
	N_Ctxt6,
	N_Ctxt7,
	N_Ctxt8,
	MSG_USER0,
	MSG_USER1,
	MSG_USER2,
	MSG_USER3,
	MSG_USER4,
	MSG_USER5,
	MSG_USER6,
	MSG_USER7,
	MSG_USER8,
	MSG_USER9,
	MSG_USER10,
	MSG_USER11,
	MSG_USER12,
	MSG_USER13,
	MSG_USER14,
	MSG_USER15,
	MSG_ICEBUTTON1_L1,
	MSG_ICEBUTTON1_L2,
	MSG_ICEBUTTON2_L1,
	MSG_ICEBUTTON2_L2,
	MSG_ICEBUTTON3_L1,
	MSG_ICEBUTTON3_L2,
	MSG_ICEBUTTON4_L1,
	MSG_ICEBUTTON4_L2,
	MSG_DEFAULT_TITLE
};

enum pinm_id
{
	Msg_ClrScrn,
	Msg_Approved,
	Msg_PinTest,
	Msg_Declined,
	Msg_IncrtPin,
	Msg_PinCardErr,
	Msg_UnspCard,
	Msg_InvldCard,
	Msg_TryAgain,
	Msg_PinReady,
	Msg_ProcCard,
	Msg_EnterCard,
	Msg_RemoveCard,

	LAST_PINM_ID				// This MUST be the last definition
};


//** RSPCODE IDS **

#define RSP_APP      "00"
#define RSP_01		 "01"
#define RSP_REFER    "02"
#define RSP_08		 "08"
#define RSP_11		 "11"
#define RSP_PROC     "12"
#define RSP_DECLINED "51"
#define RSP_PIN		 "55"
#define RSP_60		 "60"
#define RSP_RECERR	 "77"
#define RSP_NA       "91"
#define RSP_BH		 "BH"
#define RSP_BUS		 "BD"
#define RSP_BAT      "BB"
#define RSP_UN		 "UN"
#define RSP_LC		 "LC"
#define RSP_CE		 "CE"
#define RSP_TO		 "TO"
#define RSP_ND       "ND"
#define RSP_IM       "IM"   // Invalid MAC
#define RSP_ME       "ME"
#define RSP_MC       "MC"	// Security Error MAC
#define RSP_XC       "XC"	// 'Transaction Void'
#define RSP_DC       "DC"	// 'Card Declined'
#define RSP_UC       "UC"   // Cancelled (by user) 
#define RSP_NL       "NL"   // No phone line 
#define RSP_LB		 "LB"   // Line busy
#define RSP_NANSW	 "NA"   // No ansfer

//** End Marker of RspTab[] **
#define rspEND       "**"

typedef short text_msg_id;
typedef short table_index;


//=============================================================================
#include "text.h"


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================
extern void GetMsg( enum msg_id Mid, char *pBuf );
extern void GetRspTxtOpts( void );
extern void ShowErrMsg( enum msg_id Mid );
extern void ShowInfoMsg( enum msg_id Mid, enum msg_id Tid );
extern void ShowEOTMsg( enum msg_id Mid, enum msg_id Tid );
extern void ShowIntervalMsg( enum msg_id Mid, enum msg_id Tid );
extern void TextGetString( char *pBuf, table_index Tid, text_msg_id Mid );
extern Bool TextIndexAddTable( short dbtbl, UBYTE dblang,
							   const char *const *testtab, short pgnum );
extern short TextIndexNew( void );


#endif // _MSG_H_
