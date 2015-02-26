
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
    "GODK�NNANDE   authno",         // "00"     
    " V�NLIGEN RING      ",         // "01"     
    " P�FYLLNING   authno",         // "02"     
    "FEL-RING HJ�LP-SN   ",         // "03"
    "    TA UT KORTET    ",         // "04"     
    "DO NOT HONOUR       ",         // "05"             ?
    "GODK�NNANDE   authno",         // "08"     
    "FEL-RING HJ�LP-TR   ",         // "12"                 
    "FEL-RING HJ�LP-AM   ",         // "13"             
    "FEL-RING HJ�LP-RE   ",         // "14"             
    "SKRIV OM TRANS      ",         // "19"     
    "FEL-RING HJ�LP-NT   ",         // "25"             
    "FEL-RING HJ�LP-FE   ",         // "30"             
    "RING HJ�LP - NS     ",         // "31"     
    "V�NLIGEN RING   - LC",         // "41"     
    "V�NLIGEN RING   - CC",         // "43"     
    "    AVSLAGEN        ",         // "51"     
    "  KORTET UTG�NGET   ",         // "54"     
    "  FEL PIN KOD       ",         // "55"     
    "OGILTIG TRANSAKTION ",         // "58"     
    " V�NLIGEN RING      ",         // "60"
    "FEL-RING HJ�LP-DC   ",         // "76"                
    " RECONCILE ERROR    ",         // "77"             ?     
    "TRANS. INTE FUNNEN  ",         // "78"     
    "   AVSLAGEN - CVV2  ",         // "79"     
    "FEL OMG�NGS NUMMER  ",         // "80"     
    "NO CLOSED SOC SLOTS ",         // "82"             ?
    "NO SUSP. SOC SLOTS  ",         // "83"             ?     
    " KVITTO FINNS EJ    ",         // "85"     
    "FEL TERMINAL ID.    ",         // "89"     
    "FEL-RING HJ�LP-NA   ",         // "91"
    "FEL-RING HJ�LP-SQ   ",         // "94"
    "SKICKAR OMG, V�NTA  ",         // "95"     
    "FEL-RING HJ�LP-SE   ",         // "96"
    "CARD CAP TYPE INVLD ",         // "AA"             ?    
    "CARD CAP TYPE INVLD ",         // "aa"             ?

    // Locally Generated Error Messages

    "INV BAL/SETL        ",         // "NB"             ?           
    "RING HJ�LP-FEL SERV ",         // "BH"     
    "TOTALS DISAGREE     ",         // "BD"  ?
    "BALANSERA OMG�NG    ",         // "BB"     
    "FEL - FEL �VERF.    ",         // "UN"     
    "F�RS�K IGEN -LC     ",         // "LC"     
    "F�RS�K IGEN -CE     ",         // "CE"     
    "NMS UPPR. SLUF�RD   ",         // "NC"     
    "F�RS�K IGEN -TO     ",         // "TO"     
    "F�RS�K IGEN -ND     ",         // "ND"     
    "INVALID MAC         ",         // "IM"
    "  TRANSAKTION OGILT ",         // "XC"
    "LINJE UPPT F�RS IGEN",         // "LB"     
    "F�RS�K IGEN -NA     ",         // "NA"     
    "AVBR AV ANV�NDAREN  ",         // "UC"
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
