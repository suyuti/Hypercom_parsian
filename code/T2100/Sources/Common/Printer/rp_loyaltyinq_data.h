#ifdef MAKE_KIB 

#ifndef RP_LOYALTYINQ_DATA_H
#define RP_LOYALTYINQ_DATA_H

#include "pcltext.h"
#include "prtdef.h"
#include "prtdef_defines.h"
#include "rp_common_data.h"

//
//--------------------------------------------------------------------
//       LOYALTY INQUIRY TRANSACTION REPORT
//--------------------------------------------------------------------
//

const UBYTE RP_TRANS_BALINQ_DATA[] =
{
	GRPH, 0x50, 0x00,

        EXTTXT, TX_SERVUS,
	EXTFLD, FX_SERVUS,
        
	REPORT, RP_HEAD_MERCHANT,
	LINE,
        // tarih  saat 
        REPORT, RP_DATE_TIME_TRANS,
        LINE,
        // kart numarasý , exp date
        TXT, CardNumber ,
	RGHT,REPORT, RP_DATE_EXP,
        LINE,
        // pan 
        CNTR, REPORT, RP_PAN,
        LINE, 

        PPSTOP,

        // iþlem baþarýlý ise 
        IF, NOT, CND_ROLL, OR, NOT, CND_REJ,

            EXTTXT, TX_EXTENDED,
                TXT, BalInq, RGHT, LINE,
                RGHT, FLD_F, FLD_TRTOTAM, FA_TRIM, 
                LINE,
            EXTTXT, TX_DEFAULT,

            // note 
            //CNTR, TXT, Note1, LINE,
            // 'A', LINE,
                     
            //REPORT, RP_ID_TERMINAL,
            //'B', LINE,
                           
            //REPORT, RP_ID_MERCHANT,
            //'C', LINE,
            //LINE,                     

        ELSE,  // iþlem baþarýsýz ise 
            LINE,
            CNTR, TXT, TransactFailed,
            LINE,
        ENDIF,        

/*
        IF, FLD_HOSTTEXT0,
            CNTR, FLD_F, FLD_HOSTTEXT0, FC_TRIM,
			LINE,
            CNTR, FLD_F, FLD_HOSTTEXT20, FC_TRIM,
        ELSE,
            CNTR, FLD_F, FLD_RSPTXT, FC_TRIM,
        ENDIF,
        LINE,
*/
        REPORT, RP_TXT_FOOTERS,	
	REPORT, RP_TXT_HOST,

        GRPH, 0x60, 0x00,
        LINE, LINE,
        FORM, 

        EXTTXT, TX_DEFAULT,
	EXTFLD, FX_DEFAULT,
};

#endif

#endif

