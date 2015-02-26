
#ifdef MAKE_ENGLISH
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             rspentext.c
//      English Response Message Table.
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkmem.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "rsptextinit.h"
#include "msg.h"
#include "syserr.h"
#include "msgfunc.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


static const char *const RspEnText[] = {
        "APPROVED SUCCESSFULY",		// "00"     
	"REFER TO CARD ISSUER",		// "01"     
	"REFER TO CARD ISSUER",		// "02"   
	"      PICK-UP       ",		// "04"   	
	"   DO NOT HONOUR    ",         // "05"     	
	"HONOUR WITH IDEN.   ",         // "08"
	"     APPROVED       ",		// "11"     
	"INVALID TRANSACTION ",		// "12"     
	"  INVALID AMOUNT    ",		// "13"     
	"INVALID CARD NUMBER ",		// "14"
	"   NO SUCH ISSUER   ",		// "15"	
        "APPROVED SUCCESSFULY",         // "25"  Parsian Reversal
	"   FORMAT ERROR     ",		// "30"
	"EXPIRED CARD PICK-UP",		// "33"
	"COUNTERFEIT PICK-UP ",		// "34"
        "APPROVED SUCCESSFULY",         // "35"  Parsian Reversal
	"A.PYN EXCEED PICK-UP",         // "38"
	"LOST CARD PICK-UP   ",		// "41"
	"STOLEN CARD PICK-UP ",		// "43"
	"INSUFFICIENT FUNDS  ",		// "51"
	"    EXPIRED CARD    ",		// "54"
	"   INCORRECT PIN    ",		// "55"
	"TRX NOT PERMITTED CH",		// "57"
	"TRX NOT PERMITTED TR",		// "58"
	"   RESTRICTED CARD  ",		// "62"
	"ACTIVITY C.LIMIT EX.",         //"65"
	"PIN TRIES EXCEEDED  ",		// "75"
	"REFER TO CARD ISSUER",		// "85"
        "ISSUER OR SWITCH IN.",         // "91"
        "FINANCIAL INS.UNKNOW",         // "92"
	"BATCH TRANSFER,WAIT ",		// "95"
	"SYSTEM MALFUNCTION  ",		// "96"



	"CARD CAP TYPE INVLD ",		// "AA"     
	"CARD CAP TYPE INVLD ",		// "aa"

	// Locally Generated Error Messages

	"INV BAL/SETL        ",		// "NB"     
	"CALL BANK A.HOST ERR",		// "BH"     
	"BUSI. DAY BALANCING ",		// "BD"     
	"BATCH SCANNING...   ",		// "BB"     
	"FAIL- WRONG TRANS   ",		// "UN"     
	"TRY AGAIN        -LC",		// "LC"     
	"TRY AGAIN        -CE",		// "CE"     
	"NMS CALL COMPLETED  ",		// "NC"     
	"TRY AGAIN        -TO",		// "TO"     
	"TRY AGAIN        -ND",		// "ND"     	
	"LINE BUSY           ",		// "LB"     
	"PLEASE TRY AGAIN    ",		// "NA"     
        "USER BY CANCELLED   ",     // "UC"
        "NO PHONE LINE       ",     // "NL"


        "FAIL-INVLD     39 rr",		// "**"   
};


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//  RspTextEn_Init  Add English Response table for DML support.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

extern void RspTextEn_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	ResponseTabSize ();

	if ( TableItemCount != ( sizeof( RspEnText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "RspEnText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table RspEnText into database
	TextIndexAddTable( rsptext_table_id, LNG_EN, RspEnText, pgnum );
}

#endif // MAKE_ENGLISH



//=============================================================================
// Private function definitions
//=============================================================================
