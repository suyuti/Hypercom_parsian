#ifndef RP_LOYALTY_DATA_H
#define RP_LOYALTY_DATA_H

#include "pcltext.h"
#include "prtdef.h"
#include "prtdef_defines.h"

// smart puan tutarý ( iþlemde kullanýlan puan tutarýdýr ) RP2 
const UBYTE RP_POINTPART1_DATA[] =
{  
    TXT, SmartTotal,
    RGHT, FLD_F, FLD_TRUSEDLOYALTYAMOUNT, FA_TRIM,
    LINE,    
};

// tutar - puan = satýþ
// toplam RP3
const UBYTE RP_POINTPART2_DATA[] =
{
    TXT, Sale,
    RGHT, FLD_F, FLD_TRTOTAMAFTPOINT, FA_TRIM,
    LINE,    

    TXT, LastTotal,
    RGHT, FLD_F, FLD_TRTOTAMAFTDISCOUNT, FA_TRIM,
    LINE, 

};

// RP4
const UBYTE RP_DISCOUNT_DATA[] =
{
  TXT, SmartDiscount, LINE,
  TXT, Advantage, 
  FLD_L, FLD_TRDISCOUNTRATIO, 2, '.',
  FLD_S, FLD_TRDISCOUNTRATIO, 3,
 
  RGHT, FLD_F, FLD_TRDISCOUNTTOTAM, FA_TRIM, LINE,
  // toplam 
  IF, CND_POINT, 
    TXT, SubTotal, 
  ELSE, 
    TXT, LastTotal, 
  ENDIF,

  RGHT,FLD_F, FLD_TRTOTAMAFTDISCOUNT, FA_TRIM,
  LINE,
};


const UBYTE RP_AVAILABLE_LOYALTY_TOTAL_DATA[] =
{	
    IF, CND_ONUSCARD, 
        CNTR, TXT, AvailablePoint,
        LINE,
	CNTR, FLD_F, FLD_AVAIL_LOYALTY_AMT, FA_TRIM,
        LINE,   
        CNTR, TXT, Note2, 
        LINE,
        CNTR, TXT, Note3, 
        LINE, LINE, 
    ENDIF, 
};



const UBYTE RP_LOYALTYAMOUNT_DATA[] = 
{
  IF, CND_DISCOUNT,
    REPORT, RP_AMOUNT_TOTAL, 
    REPORT, RP_DISCOUNT,
    IF, CND_POINT,
      IF, CND_ALLPOINT,             // indirimli iþlemse ve tüm puan harcanmýþsa
        REPORT, RP_POINTPART1, 
        REPORT, RP_POINTPART2,  
      ELSE, 
        REPORT, RP_POINTPART1, 
      ENDIF,
    ENDIF,
  ELSE, // indirim yoksa 
    IF, CND_POINT , // puanlý iþlemse 
        IF, CND_ALLPOINT, // tüm puan harcanmýþsa 
            REPORT, RP_AMOUNT_TOTAL, 
            REPORT, RP_POINTPART1, 
            REPORT, RP_POINTPART2,
        ELSE, 
            REPORT, RP_POINTPART1,
        ENDIF,
    ELSE, // puanlý deðilse 
        RP_AMOUNT_TOTAL,
    ENDIF, 
  ENDIF,
  
};

const UBYTE RP_POINTINSTFAIL_DATA[] = 
{
    EXTTXT, TX_SERVUS,
    EXTFLD, FX_SERVUS,

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

    CNTR, REPORT, RP_NO_PAN,
    CNTR, REPORT, RP_CNAME_TRANS,
    
    LINE,
    TXT, FailMsg1, LINE,
    TXT, FailMsg2, LINE,
    LINE,

    TXT, FailMsg3,LINE,
    TXT, FailMsg4, LINE,
    LINE,

    TXT, FailMsg5,LINE,
    TXT, FailMsg6, LINE,
    LINE,

    REPORT, RP_AVAILABLE_LOYALTY_TOTAL,

    CNTR, BOLD_ON, TXT, Thanks,
    LINE,
    GRPH, 0x60, 0x00,
    FORM,

    EXTTXT, TX_DEFAULT,
    EXTFLD, FX_DEFAULT,

};

#endif

