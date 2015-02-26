#ifndef RP_OTHER_DATA_H
#define RP_OTHER_DATA_H

#include "pcltext.h"
#include "prtdef.h"
#include "prtdef_defines.h"
#include "rp_common_data.h"

//
//--------------------------------------------------------------------
//       OTHER REPORT
//--------------------------------------------------------------------
//
//
//--------------------------------------------------------------------
//       DATABASE DUMP
//--------------------------------------------------------------------
//
const UBYTE RP_DATABASE_DATA[] = {
    IF, CND_FIRST,
    TXT, DatabaseReport,
    LINE,
    MLTCHR, 15, '-', LINE,
    LINE,
    ENDIF,
    IF, OPT1,
    TXT, TermEpromConfig,
    LINE,
    ENDIF,
    IF, OPT2,
    TXT, TermConfig,
    ENDIF,
    IF, OPT3,
    TXT, AcqId,
    ENDIF,
    IF, OPT4,
    TXT, IssuerID,
    ENDIF,
    IF, OPT5,
    TXT, CardRangeId,
    ENDIF,
    IF, OPT6,
    TXT, Id_Descriptor,
    ENDIF,
    IF, OPT7,
    TXT, Visa2Id,
    ENDIF,
    IF, OPT8,
    TXT, AddPromptId,
    ENDIF,
    IF, OPT3, OR, OPT4, OR, OPT5, OR, OPT6, OR, OPT7, OR, OPT8,
    FLD_L, FLD_CUSTOM1, 2, LINE,
    FLD_S, FLD_CUSTOM1, 3, LINE,
    ENDIF,
    IF, OPT1, OR, OPT2, OR, OPT3, OR, OPT4, OR, OPT5, OR, OPT7, OR, OPT8,
    TXT, RawData,
    LINE, LINE,
    ENDIF,
    FLD, FLD_CUSTOM2, LINE
};
//
//--------------------------------------------------------------------
//       DATASCOPE
//--------------------------------------------------------------------
//
const UBYTE RP_DATASCOPE_DATA[] = {
    IF, OPT1,
    TXT, ReceivedData,
    LINE, LINE,
    ENDIF,
    IF, OPT2,
    TXT, SendData,
    LINE, LINE,
    ENDIF,
    FLD, FLD_CUSTOM2, LINE
};

//
//--------------------------------------------------------------------
//       HEADERS
//--------------------------------------------------------------------
//
const UBYTE RP_HEAD_COMMON_DATA[] = {
// ********************** LINE 1 **
    CNTR,
    IF, FLG_TCONF, TCOPT4, TC4_TRAINING,
    TXT, PclTrainingMode,
    ELSE,
    FLD_F, FLD_TCNAM1, FC_TRIM,
    ENDIF,
    LINE,
// ********************** LINE 2 **
    CNTR, FLD_F, FLD_TCNAM2, FC_TRIM,
    LINE,
// ********************** LINE 3 **
    CNTR, FLD_F, FLD_TCNAM3, FC_TRIM,
    LINE
};

const UBYTE RP_HEAD_SERIAL_DATA[] = {
    REPORT, RP_HEAD_COMMON,
    IF, CND_ROLL, RGHT, ENDIF,
    FLD_F, FLD_SERNO, FC_TRIM, LINE,
    IF, CND_ROLL, LINE, ENDIF
};
//
const UBYTE RP_HEAD_MRCH_DATA[] = {
    REPORT, RP_HEAD_COMMON,
    RGHT, FLD_F, FLD_AQTID, FC_TRIM, LINE,
    IF, CND_ROLL, LINE, ENDIF
};
//
const UBYTE RP_HEAD_MRCH2_DATA[] = {
    IF, CND_ROLL,
    TXT, MerchantN,
    RGHT,
    ELSE,
    RGHT,
    ENDIF,
    FLD_F, FLD_AQACCID, FC_TRIM, LINE,  // MERCHANT NUMBER
    FLD, FLD_AQNAME, LINE,
    LINE
};
//
//--------------------------------------------------------------------
//       SALES & REFUNDS TOTALS
//--------------------------------------------------------------------
//
const UBYTE RP_TOTALS_DATA[] = {
    TXT, Sales,
    FLD_F, FLD_SALECNT, FN_LDZ,
    IF, FLD_BASEAMT,
        LINE, ' ', ' ',
        TXT, Base,
        FLD, FLD_BASEAMT,
    ENDIF,
    IF, FLD_TAXAMT,
        LINE, ' ', ' ',
        TXT, Tax,
        FLD, FLD_TAXAMT,
    ENDIF,
    IF, FLD_BARAMT,
        LINE, ' ', ' ',
        TXT, Bar,
        FLD, FLD_BARAMT,
    ENDIF,

    IF, FLD_CASHAMT,
        LINE, ' ', ' ',
        TXT, Cash,
        FLD, FLD_CASHAMT,
    ENDIF,

    RGHT, FLD_F, FLD_SALEAMT, FA_TRIM, LINE,
    TXT, Refunds,
    FLD_F, FLD_REFCNT, FN_LDZ,
    RGHT, FLD_F, FLD_REFAMT, FA_TNEG, LINE,
    IF, FLD_CASHCNT,
        TXT, CashBk,
        FLD_F, FLD_CASHCNT, FN_LDZ,
        RGHT, FLD_F, FLD_CASHAMT, FA_TRIM, LINE,
    ENDIF,

/*
	TXT, Sales,
	CNTR,FLD_F,FLD_TOTSALCNT,FN_LDZ,
	RGHT,FLD,FLD_TOTSALAMT,
	
	LINE,
	
	TXT, Refund,
	CNTR,FLD_F,FLD_TOTREFCNT,FN_LDZ,
	RGHT,FLD,FLD_TOTREFAMT,
	
	LINE,
	
	TXT, SatisIptal,
	CNTR,FLD_F,FLD_TOTSALVCNT,FN_LDZ,
	RGHT,FLD,FLD_TOTSALVAMT,
	
	LINE,
	
	TXT, IadeIptal,
	CNTR,FLD_F,FLD_TOTREFVCNT,FN_LDZ,
	RGHT,FLD,FLD_TOTREFVAMT,
	
	LINE,
	
	TXT, odeme,
	CNTR,FLD_F,FLD_TOTPAYCNT,FN_LDZ,
	RGHT,FLD,FLD_TOTPAYAMT,
	
	LINE,
	
	TXT, OdemeIptal,
	CNTR,FLD_F,FLD_TOTPAYVCNT,FN_LDZ,
	RGHT,FLD,FLD_TOTPAYVAMT,
	
	LINE,
	
	TXT, Taksitli,
	CNTR,FLD_F,FLD_TOTTAKCNT,FN_LDZ,
	RGHT,FLD,FLD_TOTTAKAMT,
	
	LINE,
	
	TXT,TaksIptal,
	CNTR,FLD_F,FLD_TOTTAKVCNT,FN_LDZ,
	RGHT,FLD,FLD_TOTTAKVAMT,
	
	LINE,
	
	'-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
	'-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
	LINE,
	
	TXT,Total,
	CNTR,FLD_F,FLD_TOTTOPLAMCNT,FN_LDZ,' ',
	
*/
	
	
	//@AGT 30/11/04
	
/*
	IF,CND_NEGATIVE,
		RGHT,'-',
		FLD_F,FLD_TOTTOPLAMAMT,FA_TRIM,
	ELSE,
		RGHT,
		FLD,FLD_TOTTOPLAMAMT,
	ENDIF,
*/
	LINE,LINE,
};


//
//--------------------------------------------------------------------
//       SETTLMENT SUMMARY REPORT
//--------------------------------------------------------------------
//
const UBYTE RP_SETTLE_SUM_DATA[] = {

    IF, OPT3,                   // Report is done, just do a form feed and exit
    FORM,
    ELSE,
    IF, OPT1,                   // Only Display the Header the first time
    CNTR,
    TXT, SettlStatus,
    LINE,
    CNTR, '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
    '_', '_', '_', LINE,
    LINE, LINE,
    ENDIF,
    IF, FLD_CSTMSG,
    FLD, FLD_CSTMSG, LINE,
    ENDIF,
    IF, FLD_CSTMSG2,
    FLD, FLD_CSTMSG2, LINE, LINE,
    ENDIF,
    IF, FLD_CSTMSG3,
    FLD, FLD_CSTMSG3, LINE,
    ENDIF,
    IF, FLD_CSTMSG4,
    FLD, FLD_CSTMSG4, LINE, LINE,
    ENDIF,
    IF, FLD_CSTMSG5,
    FLD, FLD_CSTMSG5, LINE,
    ENDIF,
    IF, FLD_CSTMSG6,
    FLD, FLD_CSTMSG6, LINE, LINE,
    ENDIF,
    IF, OPT2,
    LINE,
    CNTR, '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
    '_', '_', '_', LINE,
    LINE, LINE,
    ENDIF,
    ENDIF,
};

//
//--------------------------------------------------------------------
//       PERFORMANCE REPORT
//--------------------------------------------------------------------
//
const UBYTE RP_PERFORMANCE_DATA[] = {
    REPORT, RP_HEAD_SERIAL,     // TERMINAL INFO

    IF, CND_ROLL,
    TXT, Date,
    ENDIF,
    RGHT, FLD_F, FLD_SYSDT, FD_TEXT,LINE,   // SYSTEM DATE
    IF, CND_ROLL, 
    TXT, Time,
    ENDIF,
    RGHT,' ', FLD_FL, FLD_SYSTM, FT_MIL, 5, LINE, LINE,  // SYSTEM TIME
    IF, CND_ROLL, DOUBLE,ENDIF,
    CNTR, TXT, lblPerfRpt,
    LINE , LINE, TXT, Period,

    EXTFLD,FX_EXTENDED,
        FLD_F, FLD_PERIOD, FD_MDYS,
        TXT, To, 
    EXTFLD,FX_DEFAULT,
        FLD_F, FLD_SYSDT, FD_MDYS, LINE, LINE,
        TXT, lblTotAprorv,
    EXTFLD,FX_EXTENDED,
        FLD_F, FLD_TOTAPPROVAL, FN_TRIM, 
        RGHT, FLD_F, FLD_APPROV_PERC, FN_TRIM, '%',LINE,  // Approval Percent 
        TXT, lblTotDenials,
        FLD_F, FLD_TOTDENIALS, FN_TRIM,  
        RGHT, FLD_F, FLD_DENIAL_PERC, FN_TRIM, '%',LINE,  // Denials Percent
        TXT, lblTotManual,
        FLD_F, FLD_TOTMANUAL, FN_TRIM, 
        RGHT, FLD_F, FLD_MANUAL_PERC, FN_TRIM, '%',LINE,  // MANUAL Percent
        TXT, lblTotSwipe,
        FLD_F, FLD_TOTSWIPE, FN_TRIM, 
        RGHT, FLD_F, FLD_SWIPE_PERC, FN_TRIM, '%',LINE,LINE,  // SWIPE Percent

        CNTR,TXT, Comm, LINE, LINE,
        TXT, TransCount ,LINE,
        TXT, lblTransCount, 
        FLD_F, FLD_TOTCOUNT, FN_TRIM, LINE, LINE,
        TXT, lblTotRedials,
        FLD_F, FLD_TOTREDIALS, FN_TRIM, LINE, LINE,
        TXT, lblTottimeout,
        FLD_F, FLD_TOTTIMEOUT, FN_TRIM, LINE, LINE,
        TXT, lblTotRetrans,
        FLD_F, FLD_TOTRETRANS, FN_TRIM, LINE, LINE,
    EXTFLD,FX_DEFAULT,
    LINE,LINE, FORM
};



//--------------------------------------------------------------------
//       ISSUER CONFIGURATION REPORT
//--------------------------------------------------------------------

const UBYTE RP_ISSCONFIG_DATA[] = {
    IF,OPT1,
        IF, CND_FIRST,                                 ////*** HEADING LINES *************
            REPORT, RP_HEAD_SERIAL,                    // TERMINAL INFO
            IF, CND_ROLL,
            TXT, Date,
            ENDIF,
            RGHT, FLD_F, FLD_SYSDT, FD_TEXT,LINE,      // SYSTEM DATE
            IF, CND_ROLL, 
                TXT, Time,
            ENDIF,
            RGHT,' ', FLD_FL, FLD_SYSTM, FT_MIL, 5, LINE, LINE,  // SYSTEM TIME
            IF, CND_ROLL, DOUBLE,ENDIF,
                CNTR, TXT, lblConfigRpt,LINE,          // CONFIGURATION REPORT
            EXTTXT,TX_EXTENDED,
                IF, CND_ROLL, DOUBLE,ENDIF,
                CNTR, TXT, lblIssuerTab,LINE, LINE,    // ISSUER TABLE
                CNTR, TXT, lblIssAvail, LINE, LINE,    // ISSUER AVAILABLE
                TXT,lblName, RGHT, TXT, lblID, LINE, LINE, // NAME      ID 

        ENDIF,                                         // (IF CND_FIRST) 

///********* ISSUER AVAILABLE ********************
            FLD, FLD_CNAME,RGHT,
            EXTFLD, FX_EXTENDED,
                FLD, FLD_ISID, LINE, LINE,
            EXTFLD, FX_DEFAULT,

    ENDIF,                                             //  (( IF OPT1 ))
///********* CONFIGURATION TABLE BY ISSUER ********
/*
    IF, OPT2,
            EXTTXT, TX_EXTENDED,
            LINE,LINE,
            MLTCHR, 50, '*',CNTR,' ', TXT,lblConfigByIss,' ', LINE, LINE, // CONFIGURATION BY ISSUER

            TXT,lblName, RGHT, TXT, lblID, LINE,       // NAME      ID 
            FLD, FLD_CNAME,RGHT,
            EXTFLD, FX_EXTENDED,
                FLD, FLD_ISID, LINE, LINE,
            EXTFLD, FX_DEFAULT,

            EXTTXT, TX_EXTENDED,
                TXT, CaptureTrans, RGHT,               // Capture Transaction
            EXTTXT, TX_DEFAULT,
                IF, FLG_ISS, ISOPT2, IS2_CAPTURE,
                TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, PrintReceipt, RGHT,               // Print Receipt
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT2, IS2_RECEIPT,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, CaptureReceipt,RGHT,              // Capture Receipt
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT6, IS6_ERC,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, FloorLimit, RGHT,                 // Floor Limit
            EXTFLD, FX_EXTENDED,
            FLD_F, FLD_FLOOR, FN_TRIM, LINE,  

            TXT, ReauthPercent, RGHT,                  // Reauthorization
                FLD_F, FLD_ISREAUTH, FN_TRIM, LINE,  
            EXTFLD, FX_DEFAULT,

            EXTTXT, TX_EXTENDED,                       // ACH Check Trans.
                TXT, AchCheckTrans, RGHT,
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT5, IS5_ACHCHECK,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, Use6Digit, RGHT,                  // Use 6 Digit
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT3, IS3_AUTH_FL6 ,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, PinEntryReq, RGHT,                // Pin Entry Required
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_PIN,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, AllowCashBack, RGHT,
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT3, IS3_CASHAMT,          // Allow Cash Back
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, AllowCashOnly, RGHT,              // Allow Cash Only
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT3, IS3_CASH_TRN,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE, LINE,

            EXTTXT, TX_EXTENDED,
                CNTR, TXT, lblProcOption, LINE, LINE,  // PROCCESSING OPTIONS

                TXT, InvoiceN, RGHT,                   // Invoice Number
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT2, IS2_ROC,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, ManualPan, RGHT,                  // Manual Pan Entry
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_MANPAN,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, Mod10Check, RGHT,                 // Mod 10 Check on Pan
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT2, IS2_MOD10,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, IgnoreTrack, RGHT,                // Ignore Track #1
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT4, IS4_TRK1ERR,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, ExpiryDate, RGHT,                 // Expire date required
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_EXPD,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, CheckExpiry, RGHT,                // Check Expire Date
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT2, IS2_CH_EXPD,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, OfflineAllow, RGHT,               // Offline Allowed
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_OFFL,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,                        // Adjust Allowed
                TXT, AdjustAllow, RGHT,
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_ADJ,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, VoiceReferral, RGHT,              // Voice Referral
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT1, IS1_REFER,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, PromptForTax, RGHT,               // Prompt For Tax
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT4, IS4_TAX,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                CNTR, TXT, lblAvsOptions, LINE, LINE,  // AVS OPTIONS

            TXT, AvsEnab, RGHT,                       // Avs enabled on Sale
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT6, IS6_ALLOWAVS,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, PromptAddress, RGHT,             // Prompt Address
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT6, IS6_MAILADDR_REQ,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT,TX_EXTENDED,
                TXT, PromptZipCode, RGHT,             // Prompt Zip Code
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT6, IS6_AVSZIPCD_REQ,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE, LINE,

            EXTTXT, TX_EXTENDED,
                CNTR, TXT,lblBlockTrans, LINE, LINE,  // Blocked Transaction
            EXTTXT, TX_DEFAULT,

            TXT, Void_s, RGHT,                        // Void
            IF, FLG_ISS, ISOPT3, IS3_NOVOID,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            TXT, Refunds, RGHT,                       // Refunds    
            IF, FLG_ISS, ISOPT2, IS2_NOREF,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, CardVerif, RGHT,                 // Card Verify
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISOPT2, IS2_NOCARDVER,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE, LINE,


            EXTTXT, TX_EXTENDED,
                CNTR, TXT, lblPinRequired, LINE, LINE, // PIN REQUIRED
            EXTTXT, TX_DEFAULT,

                TXT, Debit, RGHT,                       // Debit
            IF, FLG_ISS, ISPINTRAN, ISPIN_SALE,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

                TXT, Refunds, RGHT,                     // Refund
            IF, FLG_ISS, ISPINTRAN, ISPIN_RETURN,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

                TXT, Void_s, RGHT,                      // Void
            IF, FLG_ISS, ISPINTRAN, ISPIN_VOID,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

                TXT, Adjust, RGHT,                      // Adjust   
            IF, FLG_ISS, ISPINTRAN, ISPIN_ADJUST,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,
                TXT, BalInq, RGHT,                     // Balinq
            EXTTXT, TX_DEFAULT,
            IF, FLG_ISS, ISPINTRAN, ISPIN_BALINQ,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

                TXT, CashBack, RGHT,                   // Cash Back
            IF, FLG_ISS, ISPINTRAN, ISPIN_CASHBK,
            EXTFLD, FX_DEFAULT,
            TXT, Off, ELSE, TXT, On, ENDIF, LINE,

   ENDIF,                                             // ( IF OPT2 )
*/

};



//--------------------------------------------------------------------
//       ACQUIRER CONFIGURATION REPORT
//--------------------------------------------------------------------

const UBYTE RP_ACQUIRER_DATA[] = {
    IF,OPT1,
      IF, CND_FIRST,                                   ////*** HEADING LINES *************
        REPORT, RP_HEAD_SERIAL,                        // TERMINAL INFO
        IF, CND_ROLL,
        TXT, Date,
        ENDIF,
        RGHT, FLD_F, FLD_SYSDT, FD_TEXT,LINE,          // SYSTEM DATE
        IF, CND_ROLL, 
            TXT, Time,
        ENDIF,
        RGHT,' ', FLD_FL, FLD_SYSTM, FT_MIL, 5, LINE, LINE,  // SYSTEM TIME
        IF, CND_ROLL, DOUBLE,ENDIF,
        CNTR, TXT, lblConfigRpt, LINE,                 // CONFIGURATION REPORT
            IF, CND_ROLL, DOUBLE,ENDIF,
            CNTR, TXT, AcqTable,LINE, LINE,            // ACQUIRER TABLE
        EXTTXT, TX_EXTENDED,
            CNTR, TXT, lblConfbyAcq, LINE, LINE,           // ISSUER AVAILABLE
            TXT,lblName, RGHT, TXT, lblID, LINE, LINE, // NAME      ID 

     ENDIF,                                            // (IF CND_FIRST) 

///********* ACQUIRER AVAILABLE ********************
            FLD, FLD_AQNAME,RGHT,
            EXTFLD, FX_EXTENDED,
                FLD, FLD_ACQUID, LINE, LINE,
            EXTFLD, FX_DEFAULT,

    ENDIF,                                             //  (( IF OPT1 ))

///********* CONFIGURATION TABLE BY ACQUIRER ********
/*
    IF, OPT2, 
            EXTTXT, TX_EXTENDED,
            LINE,LINE,
            MLTCHR, 50, '*',CNTR,' ', TXT, lblConfbyAcq,' ', LINE, LINE, // CONFIGURATION BY ISSUER

            TXT,lblName, RGHT, TXT, lblID, LINE,       // NAME      ID 
            FLD, FLD_AQNAME,RGHT,
            EXTFLD, FX_EXTENDED,
                FLD, FLD_ACQUID, LINE, LINE,
            EXTFLD, FX_DEFAULT,
   
            TXT, NII, RGHT,                            // NII
            EXTFLD, FX_EXTENDED,
                FLD, FLD_NII, LINE,
            EXTFLD, FX_DEFAULT,

            TXT, CardAcceptTerm, RGHT,                 // Card acceptor terminal
            FLD, FLD_AQTID, LINE,

            TXT, CardAcceptMerc, RGHT,                // Card acceptor Merchant
            FLD_F, FLD_AQACCID, FC_TRIM, LINE,

            TXT, VisaITermId, RGHT,                   // Visa 1 terminal ID
            FLD_F, FLD_VISATID, FC_TRIM, LINE,

            EXTTXT, TX_EXTENDED,                      
                TXT, NotASettlement, RGHT,            // Not a settlement
            EXTTXT, TX_DEFAULT,
            IF, FLG_ACQ, AQOPT3, AQ3_HSEXCL,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,                      
                TXT, IgnoreHostTime, RGHT,            // Ignore Host Time
            EXTTXT, TX_DEFAULT,
            IF, FLG_ACQ, AQOPT1, AQ1_NOTIME,
            TXT, On, ELSE, TXT, Off, ENDIF, LINE,

            EXTTXT, TX_EXTENDED,                      
                TXT, ISDNprotocol, RGHT,              // ISDN protocol
            EXTTXT, TX_DEFAULT,
            FLD, FLD_DMODE, LINE,

            EXTTXT, TX_EXTENDED,                      
                TXT, CurrentBatch, RGHT,              // Current Batch
            EXTFLD, FX_EXTENDED,
            FLD, FLD_CURRBATCH, LINE,


            TXT, NextBatch, RGHT,
            FLD, FLD_NEXTBATCH, LINE,

            TXT, MaxSettleAttemp, RGHT,               // Max Settle attemp
            FLD, FLD_MAX_SETTLE, LINE, LINE,

            CNTR, TXT, lblTransHostInform,LINE, LINE,

            TXT, Primary, LINE,                       // PRIMARY

            TXT, PhoneNum, RGHT,                      // Phone Number
            EXTFLD, FX_DEFAULT,
                FLD_F, FLD_PTTEL, FC_CLRFF, LINE,

            TXT, ConnectTime, RGHT,                   // Connect time
            FLD, FLD_PTCON, LINE,

            TXT, DialAttemps, RGHT,                   // Dial Attemps
            FLD, FLD_PTTRY, LINE,

            TXT, ModemMode, RGHT,                     // Modem Mode
            FLD, FLD_MODEM, LINE, LINE,

            TXT, Secondary, LINE,                     // SECONDARY

            TXT, PhoneNum, RGHT,                      // Phone Number
            FLD_F, FLD_STTEL, FC_CLRFF, LINE,

            TXT, ConnectTime, RGHT,                   // Connect Time       
            FLD, FLD_STCON, LINE,

            TXT, DialAttemps, RGHT,                   // Dial Attemps
            FLD, FLD_STTRY, LINE, LINE,

          
            CNTR, TXT, lblSettleHostInf, LINE, LINE,  // SETTLEMENT HOST INFORMATION

            TXT, Primary, LINE,                       // PRIMARY

            TXT, PhoneNum, RGHT,                      // Phone Number
            FLD_F, FLD_PSTEL, FC_CLRFF,  LINE,

            TXT, ConnectTime, RGHT,                   // Connect time
            FLD, FLD_PSCON, LINE,

            TXT, DialAttemps, RGHT,                   // Dial Attemps
            FLD, FLD_PSTRY, LINE,

            TXT, ModemMode, RGHT,                     // Modem Mode
            FLD, FLD_SMODEM, LINE,

            TXT, Secondary, LINE,                     // SECONDARY

            TXT, PhoneNum, RGHT,                      // Phone Number
            FLD_F, FLD_SSTEL, FC_CLRFF, LINE,

            TXT, ConnectTime, RGHT,                   // Connect Time       
            FLD, FLD_SSCON, LINE,

            TXT, DialAttemps, RGHT,                   // Dial Attemps
            FLD, FLD_SSTRY, LINE,
            EXTFLD, FX_DEFAULT,                       // ALWAYS SET DEFAULT AT THE END

   ENDIF,                                             // ( IF OPT2 )
*/
};


//--------------------------------------------------------------------
//       CARD CONFIGURATION REPORT
//--------------------------------------------------------------------

const UBYTE RP_CARD_DATA[] = {
    IF,OPT1,
      IF, CND_FIRST,                                   ////*** HEADING LINES *************
        REPORT, RP_HEAD_SERIAL,                        // TERMINAL INFO
        IF, CND_ROLL,
        TXT, Date,
        ENDIF,
        RGHT, FLD_F, FLD_SYSDT, FD_TEXT,LINE,          // SYSTEM DATE
        IF, CND_ROLL, 
            TXT, Time,
        ENDIF,
        RGHT,' ', FLD_FL, FLD_SYSTM, FT_MIL, 5, LINE, LINE,  // SYSTEM TIME
        IF, CND_ROLL, DOUBLE,ENDIF,
        CNTR, TXT, lblConfigRpt, LINE,                 // CONFIGURATION REPORT
        EXTTXT,TX_EXTENDED,
            IF, CND_ROLL, DOUBLE,ENDIF,
            CNTR, TXT, lblCardRangTab,LINE, LINE,      // CARD RANGE TABLE
            CNTR, TXT, lblCardRangAvai, LINE, LINE,        // ISSUER AVAILABLE
            TXT, lblCrnum, RGHT, TXT, SupportIn, LINE,


     ENDIF,                                            // (IF CND_FIRST) 

///********* CARD AVAILABLE ********************
           EXTFLD, FX_EXTENDED,
            FLD, FLD_CRNUM, 

           EXTTXT, TX_DEFAULT,
            RGHT, TXT, IssuerID,  
           EXTFLD, FX_EXTENDED, 
            FLD, FLD_CRISID, LINE,
            RGHT, TXT, AcqId, FLD, FLD_CRAQID, LINE, LINE,


    ENDIF,                                             //  (( IF OPT1 ))

///********* CONFIGURATION TABLE BY CARD ********
   IF, OPT2,
        EXTTXT, TX_EXTENDED,
         LINE,LINE,
            MLTCHR, 50, '*',CNTR,' ', TXT,  lblConfByCard ,' ', LINE, LINE, // CONFIGURATION BY ISSUER

                                               
            TXT, CardRang, RGHT, TXT, lblID,
        EXTFLD, FX_EXTENDED,
            FLD, FLD_CRNUM, LINE, LINE,
                                                         // Pan Range Low
            TXT, PanRangeLow, RGHT,
            FLD, FLD_CRPANL, LINE,

            TXT, PanRangeHigh, RGHT,                     // Pan Range Hight
            FLD, FLD_CRPANH, LINE,

            TXT,PanLength, RGHT,                         // Pan Lenght
            FLD, FLD_CRPLEN, LINE,
    
  ENDIF,                                                // (IF, OPT2)0

};


#endif
