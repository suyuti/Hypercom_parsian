#ifndef RP_SUMMARY_BYISS_DATA_H
#define RP_SUMMARY_BYISS_DATA_H

#include "pcltext.h"
#include "prtdef.h"
#include "prtdef_defines.h"
#include "rp_common_data.h"

//
//--------------------------------------------------------------------
//       SUMMARY REPORT
//--------------------------------------------------------------------
//

const UBYTE RP_HEAD_SUMMARY_BYISS_DATA[] = {
	REPORT, RP_HEAD_MERCHANT,
	LINE,

	REPORT, RP_DATE_TIME_TRANS,
	LINE,

	REPORT, RP_ID_TERMINAL,
	LINE,
	
	CNTR, TXT, SummaryReport, LINE,
	CNTR, TXT, TransTotByIss,
	LINE
};
/*
const UBYTE RP_SUMMARY_BYISS_DATA[] = {
      'A',LINE,

	EXTTXT, TX_SERVUS,
	EXTFLD, FX_SERVUS,

        IF, CND_FIRST,
		REPORT, RP_HEAD_SUMMARY_BYISS,
		MLTCHR, 50, '-', 
		LINE,
		LINE,
	ENDIF,

        IF, OPT1,
		CNTR, TXT, GrandTotals,
        ELSE,
		CNTR, FLD_F, FLD_ISNAME, FC_TRIM,
        ENDIF, 

	LINE,

	TXT, TranType,
	POSLFT, 20, TXT, Count,
	RGHT, TXT, TotAmount,
	LINE,

	TXT, Sale, ':',
	POSLFT, 20, FLD_F, FLD_SALECNT, FN_TRIM,
	RGHT, FLD, FLD_SALEAMT,
	LINE,

	TXT, Refund, ':',
	POSLFT, 20, FLD_F, FLD_REFUNDCNT, FN_TRIM,
	RGHT, FLD, FLD_REFUNDAMT,
	LINE,

	'+', MLTCHR, 49, '-', LINE,

	TXT, Total, ':',
	POSLFT, 20, FLD_F, FLD_TOTALCNT, FN_TRIM,
	RGHT, FLD, FLD_TOTALAMT,
	LINE,

	MLTCHR, 50, '-', 
	LINE,
	LINE,

	EXTTXT, TX_DEFAULT,
	EXTFLD, FX_DEFAULT
        
};
*/
#endif