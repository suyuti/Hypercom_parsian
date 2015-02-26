#ifndef RP_SETTLE_DATA_H
#define RP_SETTLE_DATA_H

#include "pcltext.h"
#include "prtdef.h"
#include "prtdef_defines.h"
#include "rp_common_data.h"

//
//--------------------------------------------------------------------
//       SETTLEMENT RECEIPT
//--------------------------------------------------------------------
//
const UBYTE RP_HEAD_SETTLE_DATA[] = {
	REPORT, RP_HEAD_MERCHANT,
	LINE,

	REPORT, RP_DATE_TIME_TRANS, 
	LINE,
    LINE
};

const UBYTE RP_SETTLE_DATA[] = {	    
	EXTTXT, TX_SERVUS,
	EXTFLD, FX_SERVUS,

	REPORT, RP_HEAD_SETTLE, 

	TXT, TranType,
	POSLFT, 20, TXT, Count,
	RGHT, TXT, TotAmount,
	LINE,
        
        MLTCHR, 50, '-', LINE,
        
        TXT, Sale, ':',
	POSLFT, 20, FLD_F, FLD_TCRCNT, FN_TRIM,
        RGHT, FLD, FLD_TCRAMT, LINE,

	TXT, Refund, ':',
        POSLFT, 20, FLD_F, FLD_REFCNT, FN_TRIM,
        RGHT, FLD, FLD_REFAMT,

	LINE,
        FORM
        /*
	'+', MLTCHR, 49, '-', LINE,

	TXT, Total, ':',
	POSLFT, 20, FLD_F, FLD_TOTALCNT, FN_TRIM,
	RGHT, FLD, FLD_TOTALAMT,
	LINE,
      
	MLTCHR, 50, '-', LINE,

	CNTR, TXT, SucSettle,
	LINE,

	EXTTXT, TX_DEFAULT,
	EXTFLD, FX_DEFAULT,
        FORM
	*/
	
};
#endif
