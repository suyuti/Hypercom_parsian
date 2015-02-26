
#ifdef MAKE_FARSI
//=============================================================================
//                       Altius It
//                      (c) Copyright 2007
//=============================================================================
//
// Module overview:             rspfatext.c
//      Farsi Response Message Table.
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

//! response text
static const char *const RspFaText[] = {
	"APPROVAL      authno",		// "00"     
	"PLEASE CALL         ",		// "01"     
	"REFERRAL      authno",		// "02"     
	"ERR-CALL HELP-SN    ",		// "03"     
	"PICKUP CARD         ",		// "04"     
	"DO NOT HONOUR       ",		// "05"     
	"APPROVAL      authno",		// "08"     
	"ERR-CALL HELP-TR    ",		// "12"     
	"ERR-CALL HELP-AM    ",		// "13"     
	"ERR-CALL HELP-RE    ",		// "14"     
	"RE-ENTER TRANSACTION",		// "19"     
	"ERR-CALL HELP-NT    ",		// "25"     
	"ERR-CALL HELP-FE    ",		// "30"     
    "CALL HELP - NS      ",		// "31"     
    "REVERSAL APPROVED   ",		// "34"     
	"PLEASE CALL     - LC",		// "41"     
	"PLEASE CALL     - CC",		// "43"     
	"DECLINED            ",		// "51"     
	"EXPIRED CARD        ",		// "54"     
	"INCORRECT PIN       ",		// "55"     
	"\x20\x20\x20\x20\x20\xB1\x96\x9E\xCB\xE6\x93\xEA\x20\xB4\xBC\xEB\xDA\x91\xB1\x9D",		// "58"     
    "PLEASE CALL         ",     // "60"
	"ERR-CALL HELP-DC    ",		// "76"     
	"RECONCILE ERROR     ",		// "77"     
	"TRANS. NOT FOUND    ",		// "78"     
	"DECLINED - CVV2     ",		// "79"     
	"BAD BATCH NUMBER    ",		// "80"     
	"NO CLOSED SOC SLOTS ",		// "82"     
	"NO SUSP. SOC SLOTS  ",		// "83"     
	"RECEIPT NOT FOUND   ",		// "85"     
	"BAD TERMINAL ID.    ",		// "89"     
	"ERR-CALL HELP-NA    ",		// "91"     
	"ERR-CALL HELP-SQ    ",		// "94"     
	"BATCH TRANSFER, WAIT",		// "95"     
	"ERR-CALL HELP-SE    ",		// "96"     
	"CARD CAP TYPE INVLD ",		// "AA"     
	"CARD CAP TYPE INVLD ",		// "aa"

	// Locally Generated Error Messages

	"INV BAL/SETL        ",		// "NB"     
	"CALL HELP-BAD HOST  ",		// "BH"     
    "TOTALS DISAGREE     ",		// "BD"
	"BATCH BALANCING     ",		// "BB"     
	"ERROR - WRONG TRAN  ",		// "UN"     
	"PLEASE TRY AGAIN -LC",		// "LC"     
	"PLEASE TRY AGAIN -CE",		// "CE"     
	"NMS CALL COMPLETE   ",		// "NC"     
	"PLEASE TRY AGAIN -TO",		// "TO"     
	"PLEASE TRY AGAIN -ND",		// "ND"     
    "INVALID MAC         ",     // "IM"
	"TRANSACTION VOID    ",     // "XC"
	"LINE BUSY, TRY AGAIN",		// "LB"     
	"PLEASE TRY AGAIN -NA",		// "NA"     
    "CANCELLED BY USER   ",     // "UC"
    "NO PHONE LINE       ",     // "NL"

	"CARD DECLINED       ",     /* "DC"		*/
    "AUTH NO:      authno",     /* "Y1"		*/
    "AUTH NO:      authno",     /* "Y3"		*/
	"CARD DECLINED       ",     /* "Z1"		*/
	"CARD DECLINED       ",     /* "Z3"		*/

	"ERROR - INVLD 39  rr",		// "**"     
};

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add Farsi Response table for DML support
//!
//! \param
//!							  		
//! \return
//!     None
//!
//! \note
//!
extern void RspTextFa_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	ResponseTabSize ();

	if ( TableItemCount != ( sizeof( RspFaText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "RspFaText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table RspEnText into database
	TextIndexAddTable( rsptext_table_id, LNG_FA, RspFaText, pgnum );
}

#endif // MAKE_FARSI



//=============================================================================
// Private function definitions
//=============================================================================
