#ifndef RP_COMMON_DATA_H
#define RP_COMMON_DATA_H

#include "pcltext.h"
#include "prtdef.h"
#include "prtdef_defines.h"


const UBYTE RP_HEADER_COMMON_DATA[] =
{
	GRPH, 0x50, 0x00,

	REPORT, RP_HEAD_MERCHANT,
	LINE,
	
	REPORT, RP_DATE_TIME_TRANS,		
	REPORT, RP_ID_MERCHANT, 
	REPORT, RP_ID_TERMINAL, 		
        REPORT, RP_ID_AID,             
        REPORT, RP_LABEL_APP,

	TXT, CardNumber ,
	RGHT,REPORT, RP_DATE_EXP,
        LINE,
	
	IF , TRN_VOID,
            CNTR, BOLD_ON, TXT, Void,
            LINE,
	ENDIF,

	CNTR, REPORT, RP_NO_PAN,
	CNTR, REPORT, RP_NAME_CARDHOLDER,	
        
        IF, TRN_VOID, 
            REPORT, RP_NAME_TRANS, 
        ELSE,
            REPORT, RP_CNAME_TRANS,	
        ENDIF,

        IF, NOT, TRN_BALI,
            REPORT, RP_AMOUNT_TOTAL,
        ENDIF,
};


const UBYTE RP_HEAD_MERCHANT_DATA[] = 
{	
// ********************** LINE 1 **
	CNTR, FLD_F, FLD_TCNAM1, FC_TRIM,
    LINE,
// ********************** LINE 2 **
    CNTR, FLD_F, FLD_TCNAM2, FC_TRIM,
    LINE,
// ********************** LINE 3 **
    CNTR, FLD_F, FLD_TCNAM3, FC_TRIM,
    LINE,
};

const UBYTE RP_ID_MERCHANT_DATA[] = 
{
    IF, CND_ROLL,
    TXT, MerchantID,
    ENDIF,        
    FLD_F, FLD_AQACCID, FC_TRIM,
    LINE,
};

const UBYTE RP_ID_TERMINAL_DATA[] = 
{
	IF, CND_ROLL,
        TXT, TerminalID,
        ENDIF,
	FLD_F, FLD_AQTID, FC_TRIM,
	LINE,
};

//*****************************************************************************
// Transaction Date/Time Report Data
//*****************************************************************************
const UBYTE RP_DATE_TIME_TRANS_DATA[] = 
{
	EXTFLD, FX_DEFAULT,

	IF, FLD_SYSDT,
        FLD_F, FLD_SYSDT, FD_DMYS,
	ENDIF,
    
	IF, FLD_SYSTM,                
		IF, CND_PRINTTIME,                        
                      RGHT, FLD_FL, FLD_SYSTM, FT_MIL, 5,
		ENDIF,
	ENDIF,	      

	LINE,

	EXTFLD, FX_SERVUS,
};

const UBYTE RP_NO_STAN_DATA[] = 
{
	IF, FLD_TRSTAN,
		IF, CND_ROLL,
			TXT, Stan,
		ENDIF, 
		FLD, FLD_TRSTAN,                
	ENDIF
};

const UBYTE RP_NO_BATCH_DATA[] = 
{
	IF, FLD_TRBATNO,
		IF, CND_ROLL,
			TXT, Batch,
		ENDIF, 
		FLD, FLD_TRBATNO,
	ENDIF
};

const UBYTE RP_NO_INVOICE_DATA[] =
{	
	IF, FLD_TRINV,
		IF, CND_ROLL,
                    TXT, Invoice,
		ENDIF,
		FLD, FLD_TRINV,
                LINE,
	ENDIF
};

const UBYTE RP_NAME_VERSION_DATA[] = 
{
	IF, CND_ROLL,
        TXT, Version,
        ENDIF, 

	PCL_VRELEASE,
	PCL_VREV,
	PCL_VMOD
};

const UBYTE RP_NAME_SOURCE_DATA[] =
{
	IF, FLD_TRSRC,
		IF, CND_ROLL,
			TXT, Source,
		ENDIF, 
		
		FLD_F, FLD_TRSRC, FC_TRIM,
	ENDIF
};

const UBYTE RP_TYPE_CARD_DATA[] = 
{
	IF, FLD_TRCRDTYPE,
		IF, CND_ROLL,
			TXT, CardType,
		ENDIF,

		FLD_F, FLD_TRCRDTYPE, FC_TRIM,
		LINE,
	ENDIF
};

const UBYTE RP_PAN_DATA[]=
{
    FLD_SL, FLD_TRPAN, (UBYTE)1, (UBYTE)4,
    ' ',
    FLD_SL, FLD_TRPAN, (UBYTE)5, (UBYTE)4,
    ' ',
    FLD_SL, FLD_TRPAN, (UBYTE)9, (UBYTE)4,
    ' ',
    FLD_S, FLD_TRPAN, (UBYTE)-4,	
    LINE,
};


const UBYTE RP_NO_PAN_DATA[] = 
{
	IF, TRN_DBCD, OR, TRN_BALI,
		MLTCHR, 4, '*', ' ',
		MLTCHR, 4, '*', ' ',
		MLTCHR, 4, '*', ' ',
		FLD_S, FLD_TRPAN, (UBYTE)-4,        
        ELSE,
          IF, CND_PANTRUNC,    // PAN truncation enabled?
            IF, CND_SECONDCOPY, OR, OPT5,
                // This is either 2-part paper or the customer copy
				MLTCHR, 4, '*', ' ',
				MLTCHR, 4, '*', ' ',
				MLTCHR, 4, '*', ' ',
				FLD_S, FLD_TRPAN, (UBYTE)-4,
            ELSE,
				FLD_SL, FLD_TRPAN, (UBYTE)1, (UBYTE)4,
				' ',
				FLD_SL, FLD_TRPAN, (UBYTE)5, (UBYTE)4,
				' ',
				FLD_SL, FLD_TRPAN, (UBYTE)9, (UBYTE)4,
				' ',
				FLD_S, FLD_TRPAN, (UBYTE)-4,				
            ENDIF,
        ELSE,
            FLD_SL, FLD_TRPAN, (UBYTE)1, (UBYTE)4,
			' ',
			FLD_SL, FLD_TRPAN, (UBYTE)5, (UBYTE)4,
			' ',
			FLD_SL, FLD_TRPAN, (UBYTE)9, (UBYTE)4,
			' ',
			FLD_S, FLD_TRPAN, (UBYTE)-4,	
        ENDIF,
    ENDIF,
    IF, CND_MAN, ' ', '*', ENDIF,
	LINE,
};

const UBYTE RP_NAME_CARDHOLDER_DATA[] = 
{
	IF, FLD_TRNAME,
		FLD_F, FLD_TRNAME, FC_TRIM,
		LINE,
	ENDIF
};


const UBYTE RP_NAME_TRANS_DATA[] =
{
    CNTR, FLD_F, FLD_TRNTEXT, FC_TRIM,    
    LINE,
};


const UBYTE RP_CNAME_TRANS_DATA[] =
{
    CNTR, FLD_F, FLD_TRNTEXTC, FC_TRIM,    
    LINE,
};

// i�lem tutar� 
const UBYTE RP_AMOUNT_TOTAL_DATA[] =
{
	//IF, FLD_TRTOTAM,
		TXT, Total, LINE,
		DOUBLE, RGHT, FLD_F, FLD_TRTOTAM, FA_TRIM,
		LINE,
       // ENDIF
};



const UBYTE RP_CODE_AUTH_DATA[] =
{
	EXTFLD, FX_DEFAULT,
    IF, FLD_AUTH,
        IF, CND_ROLL,
            TXT, AuthNo, 		
			FLD, FLD_AUTH,
			LINE,
		ENDIF,
    ENDIF,
	EXTFLD, FX_SERVUS
};

const UBYTE RP_ID_AID_DATA[] =
{
	
};

// application label
const UBYTE RP_APP_LABEL_DATA[] =
{
	
};

// RRN (Retrival Reference Number)
const UBYTE RP_NO_RRN_DATA[] =
{
    IF, FLD_RRN,
	IF, CND_ROLL, 
            TXT, Rrn,
        ENDIF,
        FLD_F, FLD_TRRRN, FC_TRIM,
        LINE,
    ENDIF
};

// CUSTOM RECEIPT FOOTER TEXT LINES
const UBYTE RP_TXT_FOOTERS_DATA[] =
{
    IF, FLD_FTFOOTER1, 
        CNTR, FLD_F, FLD_FTFOOTER1, FC_TRIM, 
        LINE, 
    ENDIF,

    IF, FLD_FTFOOTER2, 
        CNTR, FLD_F, FLD_FTFOOTER2, FC_TRIM, 
        LINE, 
    ENDIF,

    IF, FLD_FTFOOTER3, 
        CNTR, FLD_F, FLD_FTFOOTER3, FC_TRIM, 
        LINE, 
    ENDIF,

    IF, FLD_FTFOOTER4, 
        CNTR, FLD_F, FLD_FTFOOTER4, FC_TRIM, 
        LINE, 
    ENDIF
};

const UBYTE RP_TXT_HOST_DATA[] =
{
	IF, FLD_HSTPRDATA,
		CNTR,
		FLD_F, FLD_HSTPRDATA, FC_TRIM,
	ENDIF
};

const UBYTE RP_SIGN_DATA[] =
{       
	IF, NOT, OPT5,
		TXT, Sign,
		LINE, LINE, LINE, 'X', MLTCHR, 49, '-', LINE,
	ENDIF
};

const UBYTE RP_EXP_DATE_DATA[] =
{
	EXTFLD, FX_DEFAULT,

	IF, FLD_EXPDT,
		TXT, ExpDate,
		FLD_F, FLD_EXPDT, FD_DMYS,
	ENDIF,

	EXTFLD, FX_SERVUS,
};



#endif
