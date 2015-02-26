
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _PCLTEXT_H_
#define _PCLTEXT_H_
//=============================================================================
//
// Module overview:             pcltext.h
//      PCL defintions, data, and functions
//
//=============================================================================

//=============================================================================
// Public defines and typedefs
//=============================================================================
// Message Text Identifiers
// Messages are defined in pclengtext.c and pclestext.c

enum pcltextDef_id
{
	LangId,
	Merchant,
	Srv,
	Csh,
	Till,
	Void_s,
	Food,
	Cash,
	Benefit,
	Expire,
	Date,
	Time,
	AuthN,
	Fcs,
	Avs,
	AvailB,
	PclToday,
	Base,
	Tax,
	Bar,
	Surcharge,
	Tip,
	TipGuide,
	SaleAmt,
	CashBack,
	Fee,
	AmountDue,
	NoRefundsAllow,
	OfflineEntered,
	FoodBal,
	CashBal,
	Duplicate,
	SignatureNotReq,
	AvsCanceled,
	PclTrainingMode,
	IAuthorizeElect,
	FundFromMyAccount,
	OrPaperDraft,
	ForTheAmount,
	InTheEvent,
	Unpaid,
	Point,
	Return,
	ItemFee,
	TransferFunds,
	ForTAmount,
	IAgree,
	According,
	MerchantAgreement,
	NormalFont,
	S_ABCDEFGHIJ,
	S_KLMNOPQRST,
	S_UVWXYZ,
	S_abcdefghi,
	S_klmnopqrst,
	S_uvwxyz0123,
	S_SetSymbol1,
	S_SetSimbols2,
	Normal,
	Double,
	FontChange,
	TheEnd,
	AllAcquirers,
	InvTrans,
	Server,
	Cashr,
	Auth,	
	Cashbk,
	TipTotal,
	CashTotal,
	TransTotal,
	Shift,
	Star,
	OpenTabs,
	PclInvTrans,
	TransTotals,
	ByIssuer,
	Cashier,
	PclServer,
	PclTill,
	GranTotals,
	NothingToPrint,
	Unadj,
	Audit,
	Terminal,
	DatabaseReport,
	TermEpromConfig,
	TermConfig,
	AcqId,
	IssuerID,
	CardRangeId,
	Id_Descriptor,
	Visa2Id,
	AddPromptId,
	RawData,
	ReceivedData,
	SendData,
	HostTotals,
	Debit,
	AcqTable,
	MerchantN,
	Sales,
	Refunds,
	CashBk,
	Amount,
	Log,
	On,
	Report,
	SettlStatus,
	NotOfRec,
	NumOfRec,
	RecUpload,
	PleasSettl,
	UploadFail,
	HelpDesk,
	lblPerfRpt,
	lblTotAprorv,
	lblTotDenials,
	lblTotManual,
	lblTotSwipe, 
	TransCount,
	lblTransCount,
	lblTotRedials,
	lblTottimeout,
	lblTotRetrans,
	Period,
	To,
	Comm,
	lblConfigRpt,
	Adjust,
	Off, 		

	LAST_PCLTEXTDEF_ID		// This MUST be the last definition
};

enum pcltextExt_id
{
	Percent1,
	Percent2,
	Percent3,
	lblAcqAvail,		  
	lblConfbyAcq,
	NII,
	CardAcceptTerm,
	CardAcceptMerc,
	VisaITermId,
	NotASettlement,
	IgnoreHostTime,
	ISDNprotocol,
	ErcCompression,
	CurrentBatch,
	NextBatch,
	MaxSettleAttemp,
	lblTransHostInform,
	Primary,
	PhoneNum,
	ConnectTime,
	DialAttemps,
	ModemMode,
	Secondary,
    lblSettleHostInf,
	lblIssuerTab,          //Issuer Table
	lblIssAvail,
	lblConfigByIss,
	CaptureTrans,
	PrintReceipt,
	CaptureReceipt,
	FloorLimit,
	ReauthPercent,
	AchCheckTrans,
	Use6Digit,
	PinEntryReq,
	AllowCashBack,
	AllowCashOnly,
	lblProcOption,
	InvoiceN,
	ManualPan,
	Mod10Check,
	IgnoreTrack,
	ExpiryDate,
	CheckExpiry,
	OfflineAllow,
	AdjustAllow,
	VoiceReferral,
	PromptForTax,
	lblAvsOptions,
	AvsEnab,
	PromptAddress,
	PromptZipCode,
	lblBlockTrans,
	CardVerif,
	BalInq,
	lblPinRequired,
	lblCardRangTab,
	lblCardRangAvai,
	lblConfByCard,
	CardRang,
	PanRangeLow,
	PanRangeHigh,
	PanLength,
	lblID,
	lblName,
	lblCrnum,
	SupportIn,
	IUndestand,
	MayBeConverted,
	DebitMy,
	ItemIs,
	IWillPay,
	MaximumAmount,
	LawWith,
	FromMy,
	FundsTransfer,
	MannerSer,

	
	LAST_PCLTEXTEXT_ID		// This MUST be the last definition
};

enum pclServusDef_id
{
	TranType,		// Type
	Count,			// Count
	TotAmount,		// Tot. Amount

	YiKredi,		// Yurt içi kredi iþlemi
	YdVisaKredi,	// Yurt dýþý Visa kredi iþlemi
	YdEuroKredi,	// Yurt dýþý Euro kredi iþlemi
	YiDebit,		// Yurt içi debit iþlem
	YdDebit,		// Yurt dýþý debit iþlem	
	Sale,			// Satýþ
	Refund,			// Ýade
	Void,			// Ýptal
	Payment,		// Ödeme

	Total,			// Toplam
	NetTotal,		// Net Toplam

	MerchantID,		// MERCH ID
	TerminalID,		// TERM ID
	Stan,			// STAN
	Batch,			// BATCH
	Invoice,		// INVOICE
	Version,		// VERSION
	Source,			// SOURCE
	CardType,		// CARD TYPE
	AppId,			// AID :
	Rrn,			// REF. NO :

	AuditReport,	// AUDIT REPORT
	SummaryReport,	// SUMMARY REPORT
	TransTotByIss,	// TRANS TOTAL BY ISSUER
	GrandTotals,	// GRAND TOTALS

	//Response texts
	AuthNo,			//Auth Code
	TransactFailed,	//Transaction failed

	// Successfull Txt Enumerate
	SucSettle,
	SucSale,
	SucRefund,
	SucVoid,

	// Information texts
	Not,
	DomesticInfo,
	InterInfo,
	SecondSlip,			// Second Slip
	ItsOfCardHolder,	// It s Of Card Holder
	Sign,				// Signature not Required
	ByEnterPin,			// By Enter Pin
	MakeSecureTrans,	// You Make Secure Transaction
	Thanks,				// Thanks

	// Extra Messages
	ExtText1,
	ExtText2,
	ExtText3,
	
	InstallmentInfo,	 // Taksit bilgileri 
	Installs,			 // taksitler
	FirstBillDate,		 // hesap kesim tarihi 
        AppLbl,				 // Application Label 
	ExpDate,			 // Expired Date 
        CardNumber,			 // Card Number 
	Initial,			 // Ýnitialize
	Success,             // Success
#ifdef MAKE_KIB
        AvailablePoint,   // 
        Note1,
        Note2,
        Note3,
        SmartTotal,
        SmartDiscount,
        Advantage,
        LastTotal, 
        SubTotal, 
        FailMsg1,
        FailMsg2,
        FailMsg3,
        FailMsg4,
        FailMsg5,
        FailMsg6,
#endif
        

	// Debug Txt Fields
	NotSupport,

	LAST_SERVUSTEXTDEF_ID
};


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================


#endif // _PCLTEXT_H_
