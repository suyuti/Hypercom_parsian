#ifndef RP_AUDIT_DATA_H
#define RP_AUDIT_DATA_H

#include "pcltext.h"
#include "prtdef.h"
#include "prtdef_defines.h"
#include "rp_common_data.h"

//
//--------------------------------------------------------------------
//       AUDIT REPORT
//--------------------------------------------------------------------
//

const UBYTE RP_HEAD_AUDIT_DATA[] = {
	REPORT, RP_HEAD_MERCHANT,	

	REPORT, RP_DATE_TIME_TRANS,	

	REPORT, RP_ID_TERMINAL,	
	
	CNTR, TXT, AuditReport,
	LINE
};

const UBYTE RP_AUDIT_DATA[] = {
	EXTTXT, TX_SERVUS,
	EXTFLD, FX_SERVUS,

	IF, CND_FIRST,
		REPORT, RP_HEAD_AUDIT,

		MLTCHR, 50, '-', LINE,
	ENDIF,
	
	CNTR, REPORT, RP_NAME_TRANS,
	CNTR, REPORT, RP_NO_PAN,
	REPORT, RP_ID_MERCHANT,
	REPORT, RP_NO_INVOICE,
	REPORT, RP_TYPE_CARD,
	REPORT, RP_CODE_AUTH,
	REPORT, RP_AMOUNT_TOTAL,

	MLTCHR, 50, '-', LINE,

	EXTTXT, TX_DEFAULT,
	EXTFLD, FX_DEFAULT,
	FORM
};


#endif
