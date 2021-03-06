
#ifdef MAKE_TURKISH
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


static const char *const RspTrText[] = {
	"  ��LEM ONAYLANDI   ",		// "00"     
	"BANKANIZI ARAYIN    ",		// "01"     
	"BANKANIZI ARAYIN    ",		// "02"   
	"RED-KARTA EL KOY    ",		// "04"   	
	"RED-ONAYLANMADI     ",		// "05"     	
	"K�ML�K KONTROL�/ONAY",     // "08"
	"i�LEM ONAYLANDI     ",		// "11"     
	"RED-GE�ERS�Z i�LEM  ",		// "12"     
	"RED-GE�ERS�Z TUTAR  ",		// "13"     
	"RED-HATALI KART     ",		// "14"
	"RED-GE�ERS�Z KART   ",		// "15"	
        "  ��LEM ONAYLANDI   ",         // "25"  Parsian Reversal
	"BANKANIZI ARAYIN    ",		// "30"
	"RED-KARTA  EL KOY   ",		// "33"
	"RED-KARTA  EL KOY   ",		// "34"
        "APPROVED SUCCESSFULY",         // "35"  Parsian Reversal
	"RED-KARTA  EL KOY   ",		// "38"
	"RED-KARTA  EL KOY   ",		// "41"
	"RED-KARTA  EL KOY   ",		// "43"
	"RED-LiMiT YETERSiZ  ",		// "51"
	"RED-ONAYLANMADI     ",		// "54"
	"RED-��FRE TEKRAR    ",		// "55"
	"RED-ONAYLANMADI     ",		// "57"
	"RED-ONAYLANMADI     ",		// "58"
	"RED-ONAYLANMADI     ",		// "62"
	"BANKANIZI ARAYIN    ",		// "65"
	"RED-ONAYLANMADI     ",		// "75"
	"BANKANIZI ARAYIN    ",		// "85"
        "BANKANIZI ARAYIN    ",		// "91"
        "RED-GE�ERS�Z KART   ",		// "92"
	"BATCH TRANSFER,WAIT ",		// "95"
	"BANKANIZI ARAYIN    ",		// "96"



	"CARD CAP TYPE INVLD ",		// "AA"     
	"CARD CAP TYPE INVLD ",		// "aa"

	// Locally Generated Error Messages

	"INV BAL/SETL        ",		// "NB"     
	"BANKANIZI A.HOST ERR",		// "BH"     
	"BUSI. DAY BALANCING ",		// "BD"     
	"BATCH TARANIYOR     ",		// "BB"     
	"HATA- YANLI� i�LEM  ",		// "UN"     
	"TEKRAR DENEYiN   -LC",		// "LC"     
	"TEKRAR DENEYiN   -CE",		// "CE"     
	"NMS ARAMA TAMAMLANDI",		// "NC"     
	"TEKRAR DENEYiN   -TO",		// "TO"     
	"TEKRAR DENEYiN   -ND",		// "ND"     	
	"HAT ME�GUL          ",		// "LB"     
	"L�TFEN TEKRAR DENE  ",		// "NA"     
        "KULLANICI �PTAL ETT�",     // "UC"
        "TELEFON HATTI YOK   ",     // "NL"


        "HATA- GE�ERS�Z 39 rr",		// "**"   
};

//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//  RspTextTr_Init  Add Turkish Response table for DML support.
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

extern void RspTextTr_Init( void )
{
	short pgnum;

	// Verify the number of messages match the number of events
	ResponseTabSize ();

	if ( TableItemCount != ( sizeof( RspTrText ) / sizeof( char * ) ) )
	{
		strcpy( Dspbuf, "RspTrText" );
		strcat( Dspbuf, MsgSizeMismatch );

		// Yes; notify the user of this error condition
		SystemErrMsg(  );
	}

	// Get our page number
	pgnum = SDK_ThisPage(  );

	// Put text table RspEnText into database
	TextIndexAddTable( rsptext_table_id, LNG_TR, RspTrText, pgnum );
}

#endif 


//=============================================================================
// Private function definitions
//=============================================================================
