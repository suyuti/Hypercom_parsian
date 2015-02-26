
#ifdef MAKE_GERMAN
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2005
//=============================================================================
//

// Module overview:             rspgrtext.c
//      German Response Message Table.
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
static const char *const RspGrText[] = {
	"GENEHMIGT:NR. authno",		// "00"     
	"    BITTE ANRUFEN   ",		// "01"     
	"REFERRAL      authno",		// "02"     
	"FEHLER-BITTE ANRUFEN",		// "03"     
	"    PICKUP CARD     ",		// "04"     
	"NICHT AKZEPTIEREN   ",		// "05"     
	"GEPRUEFT  Nr. authno",		// "08"     
	"FEHL.BITTE ANRUFEN-1",		// "12"     
	"FEHL.BITTE ANRUFEN-2",		// "13"     
	"FEHL.BITTE ANRUFEN-3",		// "14"     
	"BITTE NEUEN VERSUCH ",		// "19"     
	"FEHL.BITTE ANRUFEN-4",		// "25"     
	"FEHL.BITTE ANRUFEN-5",		// "30"     
	"BITTE ANRUFEN - NS  ",		// "31"     
	"BITTE ANRUFEN   - LC",		// "41"     
	"BITTE ANRUFEN   - CC",		// "43"     
	"    ABGELEHNT       ",		// "51"     
	"  KARTE VERFALLEN   ",		// "54"     
	"  PIN FALSCH        ",		// "55"     
	"TRANSAKT. UNGUELTIG ",		// "58"     
    " BITTE ANRUFEN      ",     // "60"
	"BITTE ANRUFEN   - DC",		// "76"     
	" RECONCILE FEHLER   ",		// "77"     
	"TRANS. NICHT GEFUND.",		// "78"     
	"   ABGELEHNT - CVV2 ",		// "79"     
	"FALSCHE ABSCHLUSS-NR",		// "80"     
	"NO CLOSED SOC SLOTS ",		// "82"     
	"NO SUSP. SOC SLOTS  ",		// "83"     
	"BELEG NICHt GEFUNDEN",		// "85"     
	"FALSCHE TERMINAL-ID ",		// "89"     
	"BITTE ANRUFEN   - NA",		// "91"     
	"BITTE ANRUFEN   - SQ",		// "94"     
	"BATCH TRANS.- WARTEN",		// "95"     
	"BITTE ANRUFEN   - SE",		// "96"     
	"CARD CAP TYPE INVLD ",		// "AA"     
	"CARD CAP TYPE INVLD ",		// "aa"

	// Locally Generated Error Messages

	"INV BAL/SETL        ",		// "NB"     
	"FALSCH. HOST-ANRUFEN",		// "BH" ------------------------------------    
    "SUMMEN ABWEICHUNG   ",		// "BD"
	"BATCH ABSCHLUSS     ",		// "BB"     
	"FEHLER-TRANS.FALSCH ",		// "UN"     
	"BITTE WIEDERHOLEN-LC",		// "LC"     
	"BITTE WIEDERHOLEN-CE",		// "CE"     
	" NMS RUF BEENDET    ",		// "NC"     
	"BITTE WIEDERHOLEN-TO",		// "TO"     
	"BITTE WIEDERHOLEN-ND",		// "ND"     
    "INVALID MAC         ",     // "IM"
	" STORNO TRANSAKTION ",     // "XC"
	"BELEGT-WIEDERHOLEN..",		// "LB"     
	"BITTE WIEDERHOLEN-NA",		// "NA"     
    " BENUTZER ABBRUCH   ",     // "UC"
    "KEINE LEITUNG       ",     // "NL"

	"   CARD DECLINED    ",     /* "DC"		*/
    "AUTH NO:      authno",     /* "Y1"		*/
    "AUTH NO:      authno",     /* "Y3"		*/
	"   CARD DECLINED    ",     /* "Z1"		*/
	"   CARD DECLINED    ",     /* "Z3"		*/

	"FEHLER-unguelt.39 rr",		// "**"     
};

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add German Response table for DML support
//!
//! \param
//!							  		
//! \return
//!     None
//!
//! \note
//!
extern void RspTextGr_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	ResponseTabSize( );

	if ( TableItemCount != ( sizeof( RspGrText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "RspGrText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table RspEnText into database
	TextIndexAddTable( rsptext_table_id, LNG_DE, RspGrText, pgnum );
}

#endif // MAKE_GERMAN



//=============================================================================
// Private function definitions
//=============================================================================
