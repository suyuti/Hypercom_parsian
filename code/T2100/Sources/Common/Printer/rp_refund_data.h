#ifndef RP_REFUND_DATA_H
#define RP_REFUND_DATA_H

#include "pcltext.h"
#include "prtdef.h"
#include "prtdef_defines.h"
#include "rp_common_data.h"

//
//--------------------------------------------------------------------
//       REFUND TRANSACTION REPORT
//--------------------------------------------------------------------
//

const UBYTE RP_TRANS_REFUND_DATA[] =
{
	REPORT, RP_HEADER_COMMON, 
	LINE,
	PPSTOP,

//*****************************************************************************
// "No refund allowed" text
//*****************************************************************************
/*
        IF, CND_ROLL, AND, FLG_ISS, ISOPT3, IS3_NOREFPRT,
            TXT, NoRefundsAllow,
            LINE,
        ENDIF,
*/
};


#endif
