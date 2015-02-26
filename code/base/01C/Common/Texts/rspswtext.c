
#ifdef MAKE_SWEDISH
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             rspswtext.c
//      Swedish Response Message Table.
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
static const char *const RspSwText[] = {
    "GODKÄNNANDE   authno",         // "00"     
    " VÄNLIGEN RING      ",         // "01"     
    " PÅFYLLNING   authno",         // "02"     
    "FEL-RING HJÄLP-SN   ",         // "03"
    "    TA UT KORTET    ",         // "04"     
    "DO NOT HONOUR       ",         // "05"             ?
    "GODKÄNNANDE   authno",         // "08"     
    "FEL-RING HJÄLP-TR   ",         // "12"                 
    "FEL-RING HJÄLP-AM   ",         // "13"             
    "FEL-RING HJÄLP-RE   ",         // "14"             
    "SKRIV OM TRANS      ",         // "19"     
    "FEL-RING HJÄLP-NT   ",         // "25"             
    "FEL-RING HJÄLP-FE   ",         // "30"             
    "RING HJÄLP - NS     ",         // "31"     
    "VÄNLIGEN RING   - LC",         // "41"     
    "VÄNLIGEN RING   - CC",         // "43"     
    "    AVSLAGEN        ",         // "51"     
    "  KORTET UTGÅNGET   ",         // "54"     
    "  FEL PIN KOD       ",         // "55"     
    "OGILTIG TRANSAKTION ",         // "58"     
    " VÄNLIGEN RING      ",         // "60"
    "FEL-RING HJÄLP-DC   ",         // "76"                
    " RECONCILE ERROR    ",         // "77"             ?     
    "TRANS. INTE FUNNEN  ",         // "78"     
    "   AVSLAGEN - CVV2  ",         // "79"     
    "FEL OMGÅNGS NUMMER  ",         // "80"     
    "NO CLOSED SOC SLOTS ",         // "82"             ?
    "NO SUSP. SOC SLOTS  ",         // "83"             ?     
    " KVITTO FINNS EJ    ",         // "85"     
    "FEL TERMINAL ID.    ",         // "89"     
    "FEL-RING HJÄLP-NA   ",         // "91"
    "FEL-RING HJÄLP-SQ   ",         // "94"
    "SKICKAR OMG, VÄNTA  ",         // "95"     
    "FEL-RING HJÄLP-SE   ",         // "96"
    "CARD CAP TYPE INVLD ",         // "AA"             ?    
    "CARD CAP TYPE INVLD ",         // "aa"             ?

    // Locally Generated Error Messages

    "INV BAL/SETL        ",         // "NB"             ?           
    "RING HJÄLP-FEL SERV ",         // "BH"     
    "TOTALS DISAGREE     ",         // "BD"  ?
    "BALANSERA OMGÅNG    ",         // "BB"     
    "FEL - FEL ÖVERF.    ",         // "UN"     
    "FÖRSÖK IGEN -LC     ",         // "LC"     
    "FÖRSÖK IGEN -CE     ",         // "CE"     
    "NMS UPPR. SLUFÖRD   ",         // "NC"     
    "FÖRSÖK IGEN -TO     ",         // "TO"     
    "FÖRSÖK IGEN -ND     ",         // "ND"     
    "INVALID MAC         ",         // "IM"
    "  TRANSAKTION OGILT ",         // "XC"
    "LINJE UPPT FÖRS IGEN",         // "LB"     
    "FÖRSÖK IGEN -NA     ",         // "NA"     
    "AVBR AV ANVÄNDAREN  ",         // "UC"
    "INGEN TELEONLINJE   ",         // "NL"

	"   CARD DECLINED    ",     /* "DC"		*/
    "AUTH NO:      authno",     /* "Y1"		*/
    "AUTH NO:      authno",     /* "Y3"		*/
	"   CARD DECLINED    ",     /* "Z1"		*/
	"   CARD DECLINED    ",     /* "Z3"		*/

    "FEL - OGILTIG 39  rr",         // "**"     
};


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add Swedish Response table for DML support
//!
//! \param
//!							  		
//! \return
//!     None
//!
//! \note
//!
extern void RspTextSw_Init( void )
{
    short pgnum;

    // Verify the number of messages match the number of events
    ResponseTabSize ();

    if ( TableItemCount != ( sizeof( RspSwText ) / sizeof( char * ) ) )
    {
        strcpy( Dspbuf, "RspSwText" );
        strcat( Dspbuf, MsgSizeMismatch );

        // Yes; notify the user of this error condition
        SystemErrMsg(  );
    }

    // Get our page number
    pgnum = SDK_ThisPage(  );

    // Put text table RspEnText into database
    TextIndexAddTable( rsptext_table_id, LNG_SV, RspSwText, pgnum );
}


//=============================================================================
// Private function definitions
//=============================================================================

#endif // MAKE_SWEDISH
