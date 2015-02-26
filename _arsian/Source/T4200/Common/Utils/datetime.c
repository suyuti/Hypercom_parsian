
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             datetime.c
//      Date and Time functions
//
//=============================================================================

#include "string.h"
#include "stdlib.h"

#include "basictyp.h"
#include "sdkgui.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "eventbl.h"

#include "datetime.h"
#include "comdata.h"
//=============================================================================
// External variables / declarations
//=============================================================================
void GetJelaliDateTime(UBYTE* _normalDateBuffer, UBYTE * _jelaliDateBuffer);
extern const char WeekDay[ 8][4];
extern const char Month12[13][4];

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
static const enum msg_id MONTH[13] =
{ 0, Month_1, Month_2, Month_3, Month_4, Month_5, Month_6,
	Month_7, Month_8, Month_9, Month_10, Month_11, Month_12
};


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//!  \brief     Get the Month string
//!
//!  \param
//!      None
//!
//!  \return
//!     None
//!
//!  \note
//!     Month string is stored in global buffer CSTMSG5.
//!
//-----------------------------------------------------------------------------
void GetMonth( UBYTE ByteMonth )
{
	// This function will get the proper Message ID for that month and then the text
	// string for that month and place it in the global display buffer CSTMSG5

	if ( ( ByteMonth > 0 ) && ( ByteMonth < 13 ) )
	{
		GetMsg( MONTH[ByteMonth], CSTMSG5 );
	}
	else
	{
		memcpy( CSTMSG5, "??? ", 4 );
	}

}

//-----------------------------------------------------------------------------
//!  \breif      Format the Date & Time string and store in user buffer
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Formatted string stored in global buffer CSTMSG5.
//!
//-----------------------------------------------------------------------------
void FormatDateTime( void )
{
	static char tmp_t[21];
	static UBYTE month;
	static char buf[3];
	UBYTE length;
  	UBYTE spaces;

	// Read in the date and time from the RTC chaip
	SDK_RtcRead( ( UBYTE * ) tmp_t );

    if(LOCTAB.LANGID == LNG_FA ) {
        GetJelaliDateTime(tmp_t, tmp_t);
    }

	// Format a string like "MAY 20,98 15:35"
	// 1. Get Month digits and convert it to an intiger value
	memset( ( UBYTE * ) buf, 0, 3 );
	buf[0] = tmp_t[2];
	buf[1] = tmp_t[3];
	month = atoi( ( char * ) buf );

	// Get text string for the selected Month
    if(LOCTAB.LANGID == LNG_FA) {
        strncpy(CSTMSG5, Month12[month], 3);
        length = 3;
        CSTMSG5[length++] = ' ';

    }
    else {
        GetMsg( MONTH[month], CSTMSG5 );
        length = StrLn( CSTMSG5, sizeof( CSTMSG5 ) );
        CSTMSG5[length++] = ' ';
    }

	// 2. Day of the Month and add comma and a space
	CSTMSG5[length++] = tmp_t[4];
	CSTMSG5[length++] = tmp_t[5];
	CSTMSG5[length++] = ',';
	CSTMSG5[length++] = ' ';

	// 3. The two digit Year
	CSTMSG5[length++] = tmp_t[0];
	CSTMSG5[length++] = tmp_t[1];

	// The display is currently uses 20 charater positions, with the
	// last five reserved for the time.  Space fill the difference
	// between the date length and time.
	spaces = 14 - length;
	memset( &CSTMSG5[length], ' ',spaces );
	length = length + spaces;

	//4. Time
	CSTMSG5[length++] = tmp_t[6];
	CSTMSG5[length++] = tmp_t[7];
	CSTMSG5[length++] = ':';
	CSTMSG5[length++] = tmp_t[8];
	CSTMSG5[length++] = tmp_t[9];
	CSTMSG5[length++] = '\0';
}


//-----------------------------------------------------------------------------
//!  \brief     Convert the date and time to ASCII and return in CSTMSG5
//!             01234567890123456789012   
//!             MMM DD, YY        HH:MM    
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Moved from PRINT.C and made cross-callable so that the month
//!     string can be shared.
//!     Global Inputs:  TRINP.TRDATE, TRINP.TRYEAR, TRINP.TRTIME
//-----------------------------------------------------------------------------
extern void RptDateTime( void )
{

	UBYTE monnum;

	// DATE
	memset( ( UBYTE * ) CSTMSG5, ' ', sizeof( CSTMSG5 ) );
    //if (LOCTAB.LANGID == LNG_FA) {
    //    monnum = CvtBin( TRINP.TRJALALIDATE[0] );
    //}
    //else {
        monnum = CvtBin( TRINP.TRDATE[0] );
    //}

	// If month is not 1-12 then display ???.
	if ( ( monnum > 0 ) && ( monnum < 13 ) )
	{
		GetMsg( MONTH[monnum], CSTMSG5 );
		CSTMSG5[3] = ' ';
	}
	else
	{
		memcpy( CSTMSG5, "??? ", 4 );
	}

	BufAsc( ( char * ) &CSTMSG5[4], &TRINP.TRDATE[1] );
	CSTMSG5[6] = ',';
	CSTMSG5[7] = ' ';

	BufAsc( ( char * ) &CSTMSG5[8], TRINP.TRYEAR );
	BufAsc( ( char * ) &CSTMSG5[11], TRINP.TRTIME );
	CSTMSG5[13] = ':';
	BufAsc( ( char * ) &CSTMSG5[14], &TRINP.TRTIME[1] );
}


//=============================================================================
// Private function definitions
//=============================================================================
