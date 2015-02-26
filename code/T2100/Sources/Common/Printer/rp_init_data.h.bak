#ifndef RP_INIT_DATA_H
#define RP_INIT_DATA_H

#include "pcltext.h"
#include "prtdef.h"
#include "prtdef_defines.h"

const UBYTE RP_INIT_DATA[] =
{	
	EXTFLD, FX_SERVUS,
	EXTTXT, TX_SERVUS,
        LINE,
	CNTR, TXT, Initial,      	
	LINE, 
	CNTR, TXT, Success,
	LINE,
	//CNTR, REPORT, RP_ID_TERMINAL,	

	EXTFLD, FX_DEFAULT,	
	CNTR,
	TXT, TerminalID,
	FLD, FLD_SERNO, 
	LINE,

	IF, FLD_SYSDT,
        CNTR, FLD_F, FLD_SYSDT, FD_DMYS,
	ENDIF, 

        LINE,
	IF, FLD_SYSTM,
		IF, CND_PRINTTIME,
                      CNTR, FLD_FL, FLD_SYSTM, FT_MIL, 5,
		ENDIF,
	ENDIF,
	
	LINE,

	EXTFLD, FX_SERVUS,

	FORM,
};

#endif
