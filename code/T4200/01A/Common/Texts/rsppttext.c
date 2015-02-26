
#ifdef MAKE_PORTUGUESE
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             rspentext.c
//      Portuguese Response Message Table.
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
static const char *const RspPtText[] = {
	"APROVADO      authno",		// "00"
	"  LIGUE EMISSOR     ",		// "01"
	"REFERENCIA    authno",		// "02"
	"ERRO-LIGUE AJUDA-SN ",		// "03"
	"RECOLHER CARTAO     ",		// "04"
	"     NEGADO         ",		// "05"
	"APROVADO      authno",		// "08"
	"TRANSACAO INVALIDA",		// "12"
	"ERRO-LIGUE AJUDA-AM ",		// "13"
	"CARTAO INVALIDO     ",		// "14"
	"REINICIE TRANSACAO  ",		// "19"
	"ERRO-LIGUE AJUDA-NT ",		// "25"
	"ERRO-LIGUE AJUDA-FE ",		// "30"
	" LIGUE AJUDA - NS   ",		// "31"
	"POR FAVOR LIGUE - LC",		// "41"
	"POR FAVOR LIGUE - CC",		// "43"
	"     NEGADO         ",		// "51"
	"  CARTAO VENCIDO    ",		// "54"
	"   PIN INCORRETO    ",		// "55"
	"TRANSACAO INVALIDA  ",		// "58"
    " PLEASE CALL        ",     // "60"
	"ERRO-LIGUE AJUDA-DC ",		// "76"
	"ERRO RECONCILIACAO  ",		// "77"
	"TRANS NAO ENCONTRADA",		// "78"
	"  CVV2 - DECLINADO  ",		// "79"
	"NUM DE LOTE INVALIDO",		// "80"
	"TRANS INEXISTENTE   ",		// "82"
	"SEM TRANS. SUSPEN.  ",		// "83"
	" RECIBO NAO ENCONT  ",		// "85"
	"NUM TERM INVALIDO   ",		// "89"
	"ERRO-LIGUE AJUDA-NA ",		// "91"
	"ERRO-LIGUE AJUDA-SQ ",		// "94"
	"AGUARDE,TRANSMITINDO",		// "95"
	"ERRO-LIGUE AJUDA-SE ",		// "96"
	"TIPO CARTAO INVAL.  ",		// "AA"
	"TIPO CARTAO INVAL.  ",		// "aa"

	// Locally Generated Error Messages

	"BAL/RECONC INVALIDO ",		// "NB"
	"LIGUE AJUDA-HOST MAL",		// "BH"
	"SALDO DO DIA        ",		// "BD"
	"SALDO POR LOTE      ",		// "BB"
	"TRANSACAO ERRADA    ",		// "UN"
	"TENTE DE NOVO - LC  ",		// "LC"
	"TENTE DE NOVO - CE  ",		// "CE"
	"CHAMADA NMS COMPLETA",		// "NC"
	"TENTE DE NOVO - TO  ",		// "TO"
	"TENTE DE NOVO - ND  ",		// "ND"
    "INVALID MAC         ",     // "IM"
	"  TRANSACTION VOID  ",     /* "XC"		*/
	"TENTE DE NOVO - LB  ",		// "LB"     
	"TENTE DE NOVO - NA  ",		// "NA"     
    " CANCELLED BY USER  ",     /* "UC"     */
    "   NO PHONE LINE    ",     /* "NL"     */

	"   CARD DECLINED    ",     /* "DC"		*/
    "AUTH NO:      authno",     /* "Y1"		*/
    "AUTH NO:      authno",     /* "Y3"		*/
	"   CARD DECLINED    ",     /* "Z1"		*/
	"   CARD DECLINED    ",     /* "Z3"		*/

	"ERRO-TRANS INVALD 39",		// "**"
};

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Add Portuguese Response table for DML support
//!
//! \param
//!							  		
//! \return
//!     None
//!
//! \note
//!
extern void RspTextPt_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	ResponseTabSize ();

	if ( TableItemCount != ( sizeof( RspPtText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "RspPtText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Puts text table RspPtText into database
	TextIndexAddTable( rsptext_table_id, LNG_PT, RspPtText, pgnum );
}

#endif // MAKE_PORTUGUESE



//=============================================================================
// Private function definitions
//=============================================================================
