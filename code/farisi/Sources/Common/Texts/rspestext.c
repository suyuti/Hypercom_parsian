
#ifdef MAKE_SPANISH
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             rspentext.c
//      Spanish Response Message Table.
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
static const char *const RspEsText[] = {
	"APROBADO      authno",		// "00"
	"  LLAME AL EMISOR   ",		// "01"
	"LLAMAR REF.   authno",		// "02"
	"LLAMAR/ERROR - SN   ",		// "03"
	"RECOGER TARJETA     ",		// "04"
	"     DENEGADA       ",		// "05"
	"APROBADO      authno",		// "08"
	"TRANSACCION INVALIDA",		// "12"
	" IMPORTE  INVALIDO  ",		// "13"
	"TARJETA INVALIDA    ",		// "14"
	"REINICIE TRANSACCION",		// "19"
	"NO EXISTE COMPROBANT",		// "25"
	" FORMATO INVALIDO   ",		// "30"
	" LLAMAR/ERROR - NS  ",		// "31"
	"  RETENGA Y LLAME   ",		// "41"
	"  RETENGA Y LLAME   ",		// "43"
	"     DENEGADA       ",		// "51"
	"  TARJETA VENCIDA   ",		// "54"
	"   PIN INCORRECTO   ",		// "55"
	"TRANSACCION INVALIDA",		// "58"
    " PLEASE CALL        ",     // "60"
	"ERROR/LLAMAR - DC   ",		// "76"
	"ERROR/RECONCILIE    ",		// "77"
	"CARGO NO ENCONTRADO ",		// "78"
	"  CVV2-DECLINADO    ",		// "79"
	"NRO.DE LOTE INVALIDO",		// "80"
	"  NO EXISTE CARGO   ",		// "82"
	"NO CARGOS EN SUSPEN.",		// "83"
	"RECIBO NO ENCONTRADO",		// "85"
	"NRO. TERM. INVALIDO ",		// "89"
	"LLAME 91            ",		// "91"
	"ERROR/LLAMAR - SQ   ",		// "94"
	"ESPERE,TRANSMITIENDO",		// "95"
	"LLAME 96            ",		// "96"
	"TIPO TARJ.CAP INVAL.",		// "AA"
	"TIPO TARJ.CAP INVAL.",		// "aa"

	// Locally Generated Error Messages

	"INVALID BAL/RECONC  ",		// "NB"
	"LLAMAR - HOST MALO  ",		// "BH"
	"BALANCE             ",		// "BD"
	"BALANCE POR LOTE    ",		// "BB"
	"TRANSACCION ERRONEA ",		// "UN"
	"LC-INTENTE DE NUEVO ",		// "LC"
	"CE-INTENTE DE NUEVO ",		// "CE"
	"LLAMADA NMS TERMINO ",		// "NC"
	"REINTENTE        -TO",		// "TO"
	"REINTENTE        -ND",		// "ND"
    "INVALID MAC         ",     // "IM"
	"  TRANSACTION VOID  ",     // "XC"
	"REINTENTE        -LB",		// "LB"     
	"REINTENTE        -NA",		// "NA"     
    " CANCELLED BY USER  ",     // "UC" 
    "   NO PHONE LINE    ",     // "NL" 

	"   CARD DECLINED    ",     /* "DC"		*/
    "AUTH NO:      authno",     /* "Y1"		*/
    "AUTH NO:      authno",     /* "Y3"		*/
	"   CARD DECLINED    ",     /* "Z1"		*/
	"   CARD DECLINED    ",     /* "Z3"		*/

	"ERROR - INVLD 39  rr",		// "**"
};

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add Spanish Response table for DML support
//!
//! \param
//!							  		
//! \return
//!     None
//!
//! \note
//!
extern void RspTextEs_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	ResponseTabSize ();

	if ( TableItemCount != ( sizeof( RspEsText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "RspEsText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table RspEsText into database
	TextIndexAddTable( rsptext_table_id, LNG_ES, RspEsText, pgnum );
}
#endif // MAKE_SPANISH

//=============================================================================
// Private function definitions
//=============================================================================
