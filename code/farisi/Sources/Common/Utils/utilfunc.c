
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002, 2004
//=============================================================================
//
// Module overview:             utilfunc.c
//      Generic User Interface functions.
//
//=============================================================================

// System includes
#include "string.h"

// HSDK includes
#include "basictyp.h"
#include "utllib.h"

// Application includes
#include "struct.h"
#include "appdata.h"
#include "util.h"
#include "disproc.h"
#include "cvtamt.h"

// This module external declaration
#include "utilfunc.h"
#include "pinwrap.h"
//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static UBYTE InputBox( char InputMode, char DspMode, char Maxlen,
					   char Minlen );
static UBYTE TwoBtnEntry( UWORD screen, struct ESParms_str *pESParms );
static void FormAmt( void );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

static char InitialValStr[MAX_MSG_LEN + 1];
static char tmp[MAX_MSG_LEN+1];
static UBYTE cnt;


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief     Accepts input from user & saves in Dspbuf.
//!
//!  \param
//!     ESParms     pointer to ESParms_str structure
//!                     display mode:   1 - echo
//!                                     2 - password input, echo '*'
//!                                     '0' - '9' - Account number entry,
//!                                              1st digit of acct. #.
//!                                     3 - echo, cancel disabled
//!                                     4 - password input, echo '*', cancel disabled
//!                     Max length      Maximum data length allowed.
//!                     Min length      Minimun data length expected.
//!                     TitleMid        Put in title bar
//!                                     TitleMid is usually used for
//!                                     the transaction name.
//!                     Line1Mid        Put on first line
//!                                     Line1Mid is usually used for
//!                                     the name of the value being entered.
//!                     Line2Mid        Put on second line
//!                                     Line2Mid is usually used for
//!                                     the default value.
//!                     Alpha           If non-zero, allows toggle to alpha screen
//!                                     for input of alphanumeric
//!
//!  \return
//!     ESParms.Alpha
//!                 0 - CANCEL_KY key was pressed
//!                    (or 2 CLEAR's in succession),
//!                 1 - data was entered successfully,
//!                 2 - Zap key was pressed.
//!
//!  \note
//!          Must clear GlobalDsp flag before return.
//!
//!                  If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
//!                  buffer is used. If it is equal to CustomMsg2, then
//!                  CSTMSG2 is used.
//!
//!                  If TitleMid of ENTRY_STR is NOT equal to CustomMsg, 
//!                  then the text identified by the TitleMid identifier is used.
//!
//!                  The above applies also to Line1Mid and Line2Mid
//!
//!                  If TitleMid is equal to N_NullStr then the value
//!                  in Titlebuf or DefaultTitle is used.
//!
//!                  This function uses Titlebuf, Dspbuf and CSTMSG3.
//!
//-----------------------------------------------------------------------------
extern void EntryScreen( struct ESParms_str *pESParms )
{
	UBYTE retval;

	InitialValStr[0] = 0;
	// toggle between alphanum entry and num entry until user exits 
	do
	{
		// setup title text 
		if ( pESParms->TitleMid == N_NullStr )
			User_Msgs[7] = MSG_DEFAULT_TITLE;
		else
			User_Msgs[7] = pESParms->TitleMid;

		// setup prompt text 
		User_Msgs[1] = pESParms->Line1Mid;
		User_Msgs[2] = pESParms->Line2Mid;

		// initialize screen input box 
		SetInputBoxMaxChars( pESParms->MaxLen );
		SetInputBox( "" );

		// Display the base input screen (see disproc.c) 

		// assume cancel key used 
		Enable_Flags[0] = 0;	// disable long title 
		Enable_Flags[1] = 1;	// enable short title with cancel 


		if ( 3 == pESParms->DspMode )
		{
			// disable cancel key 
			Enable_Flags[0] = 1;	// enable long title 
			Enable_Flags[1] = 0;	// disable short title with cancel 
			pESParms->DspMode = 1;	// set to echo 
		}

		if ( 4 == pESParms->DspMode )
		{
			// disable cancel key 
			Enable_Flags[0] = 1;	// enable long title 
			Enable_Flags[1] = 0;	// disable short title with cancel 
			pESParms->DspMode = 2;	// set to echo '*' 
		}


#if	defined(MAKE_OPTIMUM)
		// If we started to enter something, remove the original value,
		// otherwise it flashes not nicely on T4100
		if ( pESParms->Alpha && 0 != InitialValStr[0] ) User_Msgs[1] = N_NullStr;
#endif	// defined(MAKE_OPTIMUM)

		if ( pESParms->Alpha )
			DrawScreen( ALPHANUM1_SCR );	// contains ABC key 
		else
			DrawScreen( NUMENTRY_SCR );	// numeric entry only 

		// if keypad toggle key hit 
		if ( ( retval =
			   InputBox( pESParms->Alpha, pESParms->DspMode, pESParms->MaxLen,
						 pESParms->MinLen ) ) == 3 )
		{

			strcpy( InitialValStr, Dspbuf );
#if	defined(MAKE_OPTIMUM)
			// If we started to enter something, remove the original value,
			// otherwise it flashes not nicely on T4100
			if ( pESParms->Alpha && 0 != InitialValStr[0] ) User_Msgs[1] = N_NullStr;
#endif	// defined(MAKE_OPTIMUM)

			DrawScreen( ALPHANUM2_SCR );

			// if keypad toggle key is hit 
			if ( ( retval =
				   InputBox( pESParms->Alpha, pESParms->DspMode, pESParms->MaxLen,
							 pESParms->MinLen ) ) == 3 )
			{

				strcpy( InitialValStr, Dspbuf );

#if	defined(MAKE_OPTIMUM)
				// If we started to enter something, remove the original value,
				// otherwise it flashes not nicely on T4100
				if ( pESParms->Alpha && 0 != InitialValStr[0] ) User_Msgs[1] = N_NullStr;
#endif	// defined(MAKE_OPTIMUM)

				DrawScreen( ALPHANUM3_SCR );

				// if keypad toggle key 
				if ( ( retval =
					   InputBox( pESParms->Alpha, pESParms->DspMode,
								 pESParms->MaxLen, pESParms->MinLen ) ) == 3 )
				{
					strcpy( InitialValStr, Dspbuf );

#if	defined(MAKE_OPTIMUM)
					// If we started to enter something, remove the original value,
					// otherwise it flashes not nicely on T4100
					if ( pESParms->Alpha && 0 != InitialValStr[0] ) User_Msgs[1] = N_NullStr;
#endif	// defined(MAKE_OPTIMUM)

                    // Draw Symbol entry screen
                    DrawScreen( ALPHANUM4_SCR );

                    // if keypad toggle key 
                    if ( ( retval =
                           InputBox( pESParms->Alpha, pESParms->DspMode,
								 pESParms->MaxLen, pESParms->MinLen ) ) == 3 )
                    {
                        // back to numeric entry screen 
    					strcpy( InitialValStr, Dspbuf );
    					continue;	
                    }
				}
				else
					break;

			}
			else
				break;			// exit loop 
		}
		else
			break;				// exit loop 

	}
	while ( 1 );

	pESParms->Alpha = retval;

}

extern void VPP_EntryScreen( struct ESParms_str *pESParms, UBYTE* dest )
{
	// Set up screen msgs for prompts and title.
	if ( pESParms->TitleMid == N_NullStr )
		User_Msgs[7] = MSG_DEFAULT_TITLE;
	else
		User_Msgs[7] = pESParms->TitleMid;

	// setup prompt text 
	User_Msgs[1] = pESParms->Line1Mid;
	User_Msgs[2] = pESParms->Line2Mid;

	// Initialize screen input box 
	SetInputBoxMaxChars( pESParms->MaxLen );
	SetInputBox( "" );

#if	defined(MAKE_OPTIMUM)
		// If we started to enter something, remove the original value,
		// otherwise it flashes not nicely on T4100
		if ( pESParms->Alpha && 0 != InitialValStr[0] ) User_Msgs[1] = N_NullStr;
#endif	//defined(MAKE_OPTIMUM)

	// Display the input screen. 
	DrawScreen( NUMENTRY_SCR );	// common entry screen ( numeric entry only )

	pESParms->Alpha = VPP_GetPin( pESParms->MaxLen, Dspbuf2, dest, UpdateInputBox );
}


//-----------------------------------------------------------------------------
//!  \brief     Accept YES or NO from user.
//!
//!  \param
//!     ESParms     pointer to ESParms_str structure
//!                     TitleMid        Put in title bar
//!                     Line1Mid        Put on first line
//!                     Line2Mid        Put on second line
//!
//!  \return
//!     ESParms.Alpha
//!                 0 - timeout,
//!                 CANCEL_KY - CANCEL_KY key was pressed,
//!                 ENTER - YES was entered,
//!                 CELAR - NO  was entered.
//!
//!  \note
//!     If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
//!     buffer is used. If it is equal to CustomMsg2, then
//!     CSTMSG2 is ued.
//!     If TitleMid of ENTRY_STR is NOT equal to CustomMsg, 
//!     then the text identified by the TitleMid identifier is used.
//!     The above applies also to Line1Mid and Line2Mid
//!     If TitleMid is equal to N_NullStr then the value
//!     in Titlebuf or DefaultTitle is used.
//!
//-----------------------------------------------------------------------------

extern void XYesNoEntry( struct ESParms_str *pESParms )
{
	pESParms->Alpha = TwoBtnEntry( YESNO_SCR, pESParms );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Accept YES or NEXT from user.
//!
//!
//!  \param
//!     ESParms     pointer to ESParms_str structure
//!                     TitleMid        Put in title bar
//!                     Line1Mid        Put on first line
//!                     Line2Mid        Put on second line
//!
//!  \return
//!     ESParms.Alpha
//!                 0 - timeout,
//!                 CANCEL_KY - CANCEL_KY key was pressed,
//!                 ENTER - YES was entered,
//!                 CLEAR - NEXT was entered.
//!
//!  \note
//!          If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
//!                  buffer is used. If it is equal to CustomMsg2, then
//!                  CSTMSG2 is ued.
//!
//!                  If TitleMid of ENTRY_STR is NOT equal to CustomMsg, 
//!                  then the text identified by the TitleMid identifier is used.
//!
//!                  The above applies also to Line1Mid and Line2Mid
//!
//!                  If TitleMid is equal to N_NullStr then the value
//!                  in Titlebuf or DefaultTitle is used.
//!
//-----------------------------------------------------------------------------
void XYesNextEntry(struct ESParms_str *pESParms)
{
	pESParms->Alpha = TwoBtnEntry(YES_NEXT_SCR, pESParms);
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Cross-callable implementation of ContinueEntry.
//!     Accept Continue or Cancel from user
//!
//!
//!  \param
//!     ESParms     pointer to ESParms_str structure
//!                     TitleMid            Put in title bar
//!                     Line1Mid            Put on first line
//!                     Line2Mid            Put on second line
//!
//!  \return
//!     ESParms.Alpha
//!                 0 - CANCEL key was pressed or timed out,
//!                 1 - CONTINUE was entered.
//!
//!  \note
//!     If TitleMid of ENTRY_STR is equal to CustomMsg, the buffer
//!     pointed to by pTitle of ENTRY_STR is used.
//!     If TitleMid of ENTRY_STR is NOT equal to CustomMsg, 
//!     then the text identified by the TitleMid identifier is used.
//!     The above applies also to Line1Mid and pLine1 and Line2Mid
//!     and pLine2
//!
//-----------------------------------------------------------------------------
extern void XContinueEntry( struct ESParms_str *pESParms )
{
	UBYTE retval;

	// setup title text 
	if ( pESParms->TitleMid == N_NullStr )
		User_Msgs[7] = MSG_DEFAULT_TITLE;
	else
		User_Msgs[7] = pESParms->TitleMid;

	// setup prompt text 
	User_Msgs[1] = pESParms->Line1Mid;
	User_Msgs[2] = pESParms->Line2Mid;

	DrawScreen( CONTINUE_SCR );

	do
	{
		retval = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

		// continue/ENTER button pressed 
		if ( ( 'C' == retval ) || ( ENTER_KY == retval ) )
		{
			retval = 1;
		}
		else if ( ( retval == 0 )
				  || ( CANCEL_KY == retval ) || ( CLEAR_KY == retval ) )
		{
			retval = 0;
		}
		else
		{
			// Invalid key, notify user 
			SDK_Beeper( TENMS * 40 );
			retval = 5;
		}
	}
	while ( retval == 5 );

	pESParms->Alpha = retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Cross-callable implementation of ContinueEntryTimeout
//!     Accept Continue or Cancel from user
//! 
//! \param
//!     ESParms     pointer to ESParms_str structure
//!                     TitleMid or pTitle - put in title bar
//!                     Line1Mid or pLine1 - put in first line
//!                     Line2Mid or PLine2 - put on second line
//!
//! \return
//!     ESParms.Alpha
//! 	            0 - CANCEL_KY key was pressed
//!                 1 - CONTINUE was entered
//!				    2 - Timeout occurred	
//!
//! \note
//!     If TitleMid of ENTRY_STR is equal to CustomMsg, the buffer
//!     pointed to by pTitle of ENTRY_STR is used.
//!     If TitleMid of ENTRY_STR is NOT equal to CustomMsg, then the text
//!     identified by the TitleMid identifier is used.
//!     The above applies also to Line1Mid and pLine1 and Line2Mid
//!     and pLine2
//-----------------------------------------------------------------------------
void XContinueEntryTimeout(struct ESParms_str *pESParms)
{
    UBYTE retval;

	/* setup title text */
 	if(pESParms->TitleMid != N_NullStr)     
		GetMsg( pESParms->TitleMid, DefaultTitle );

	/* setup prompt text */
	User_Msgs[1] = pESParms->Line1Mid;
	User_Msgs[2] = pESParms->Line2Mid;

	DrawScreen(CONTINUE_SCR);

    do
    {
        retval = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );
        if( ('C' == retval) || (ENTER_KY == retval) )    /* continue/ENTER button pressed */
            retval = 1;
        else if( (CANCEL_KY == retval) || (CLEAR_KY == retval ) )
            retval = 0;			// cancelled
		else if ( 0 == retval )
            retval = 2;			// Timeout
        else
        {
            /* Invalid key, notify user */
            SDK_Beeper( TENMS * 40 );
            retval = 5;
        }
    } while (retval == 5);

    pESParms->Alpha =  retval;
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Accept user input for amount.
//!
//!  \param
//!      ESParms    pointer to ESParms_str structure
//!                     char DspMode    1       zero amount is allowed,
//!                                  other   zero amount is not allowed.
//!                     char Maxlen     1       use settle sized amount,
//!                                  other   standard sized amount.
//!                     const ENTRY_STR * pEntry
//!                                  Structure with screen information
//!                                  TitleMid - put in title bar 
//!                                  Line1Mid - put on first line    
//!                                  Line2Mid - put on second line   
//!
//!                                  TitleMid is usually used for
//!                                  the transaction name. 
//!
//!                                  Line1Mid is usually used for
//!                                  the name of the value being entered.  
//!
//!                                  Line2Mid is usually used for
//!                                  the default value.    
//!
//!  \return
//!     ESParms.Alpha   
//!                 0 - CANCEL key was pressed,
//!                 1 - amount was entered successfully,
//!                 3 - CLEAR key was pressed.
//!
//!  \note
//!     CSTMSG contains prompt to put on amount line.
//!
//-----------------------------------------------------------------------------
extern void XAmountEntry( struct ESParms_str *pESParms )
{
	UBYTE maxcount;
	UBYTE retval;
	static char defaultbuf[MAX_MSG_LEN + 1];
	static UBYTE ZeroAmount[S_TRBASE];
	UBYTE Zero_OK;
	UBYTE IO_Settle_Amt;
	UBYTE keep_looping = 0;

	Zero_OK = ( UBYTE ) pESParms->DspMode;	// passed via global memory 
	IO_Settle_Amt = ( UBYTE ) pESParms->MaxLen;	// passed via globbal memory 

	// setup title text 
	if ( pESParms->TitleMid == N_NullStr )
		User_Msgs[7] = MSG_DEFAULT_TITLE;
	else
		User_Msgs[7] = pESParms->TitleMid;

	// setup prompt text 
	User_Msgs[1] = pESParms->Line1Mid;
	User_Msgs[2] = pESParms->Line2Mid;

	// set defaultbuf to default value 
	memset( ZeroAmount, 0, S_TRBASE );
	CvtAmt( defaultbuf, ZeroAmount );
	strcpy( defaultbuf, strchr( defaultbuf, TCONF.TCLEADC[0] ) );

	// Set max size of amount to be entered.    
	if ( IO_Settle_Amt )
		maxcount = CvtBin( TCONF.TCSDLEN );
	else
		maxcount = CvtBin( TCONF.TCDLEN );

	// initialize screen input box (include currency and '.')
	SetInputBoxMaxChars( ( UBYTE ) ( maxcount + 1 + S_TCLEADC) );
	SetInputBox( defaultbuf );

	DrawScreen( AMTENTRY_SCR );

	strcpy( Dspbuf, defaultbuf );

	cnt = 0;

	do
	{
		// Get a key or pad 
		retval =
			GetOneKey( DEV_PAD | DEV_KBD | STAY_OPEN0, UserTimeout );

		switch ( retval )
		{
			case 0:			// Time-out 
				break;

			case CLEAR_KY:		// Clear input buffer and try again 
            	memset( (UBYTE *)tmp, 0x00, sizeof(tmp) );
				strcpy( Dspbuf, defaultbuf );
				cnt = 0;
				break;

			case ENTER_KY:		// Done input amount 
				if ( cnt )
				{
					memset( ( UBYTE * ) Dspbuf, 0, sizeof( Dspbuf ) );
					memcpy( Dspbuf, tmp, ( UWORD ) cnt );
				}
				else if ( !Zero_OK )	// zero amount not allowed 
				{
					keep_looping = 1;	// Go back for more input 
					SDK_Beeper( TENMS * 40 );	// Error beep 
				}
				else
					memset( ( UBYTE * ) Dspbuf, 0, sizeof( Dspbuf ) );	// To pass CalTrTotal
				break;

			case BSP_KY:		// Back space 
				if ( cnt )
				{
					tmp[cnt--] = 0;
					strcpy( Dspbuf, defaultbuf );	/* Restore default value */
					FormAmt( );
				}
				break;

			case CANCEL_KY:	// User pressed Cancel 
				break;

			default:
				if ( ( retval < '0' ) || ( retval > '9' ) )	// Check for numeric input 
				{
					SDK_Beeper( TENMS * 40 );	// Error beep 
					continue;
				}

				if ( !cnt && ( retval == '0' ) )	// Don't take leading zeroes 
				{
					SDK_Beeper( TENMS * 40 );	// Error beep 
					continue;
				}

				if ( cnt < maxcount )	// Stop accepting at maxcount input 
				{
					tmp[cnt++] = retval;
					strcpy( Dspbuf, defaultbuf );	/* Restore default value */
					FormAmt( );
				}
				else			// Exceeded max number of digits allowed 
					SDK_Beeper( TENMS * 40 );	// Error beep 
				break;
		}

		if ( retval && ( retval != ENTER_KY ) && ( retval != CANCEL_KY ) )
		{
			UpdateInputBox( Dspbuf );	// show current amount 
		}
		else
		{
			if ( 0 == keep_looping )	// don't break if keep_looping set 
				break;
			else
				keep_looping = 0;
		}

	}
	while ( 1 );

	switch ( retval )			// Convert to the correct response 
	{
		case CANCEL_KY:
			retval = 0;
			break;

		case ENTER_KY:
			retval = 1;
			break;

		case CLEAR_KY:
			retval = 3;
			break;
	}

	pESParms->Alpha = retval;		// use pESParms->Alpha to pass return value 
}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief        Accept user input keypress, or pad touch.
//!
//!  \param[in]
//!     InputMode   0 - numeric input
//!                 non-0   alpha-numeric input
//!  \param[in]
//!     DspMode     1 - echo
//!                 2 - password input, echo '*' 
//!                 '0' - '9' - Account number entry, 
//!                         1st digit of acct. #.
//!  \param[in]
//!     Maxlen      Maximum data length allowed.
//!  \param[in]
//!     Minlen      Minimun data length expected.
//!
//!
//!  \return
//!     UBYTE       0 - CANCEL key was pressed or timed out,
//!                 1 - A key press or pad touch entered successfully,
//!                 3 - ABC_KEY was pressed,
//!                 4 - Numeric entry key ('#') was pressed.
//!
//!  \note
//!     Contents of InitialValStr is initial value.
//!
//-----------------------------------------------------------------------------
static UBYTE InputBox( char InputMode, char DspMode, char Maxlen,
					   char Minlen )
{

// The tables to parse Numeric entry to the values defined on the
// images IMAGE_KEYNUM, IMAGE_KEYASC, IMAGE_KEYASCSM, IMAGE_KEYSYM 
static const char *ParseKeyTable1[] = 
{
	 	"0",  // Numeric entry by ALPHANUM1_SCR with IMAGE_KEYNUM
  		"1",
    	"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",		
};

static const char *ParseKeyTable2[] =								
{	
	  	" +?",  // Numeric entry by ALPHANUM2_SCR with IMAGE_KEYASC
		"ABC",
  		"DEF",
	  	"GHI",
	  	"JKL",
	  	"MNO",
	  	"PQR",
	  	"STU",
	  	"VWX",
	  	"YZ\\",
};

static const char *ParseKeyTable3[] =
{	
	  	" .,",  // Numeric entry by ALPHANUM3_SCR with IMAGE_KEYASCSM
		"abc",
  		"def",
	  	"ghi",
	  	"jkl",
	  	"mno",
	  	"pqr",
	  	"stu",
	  	"vwx",
	  	"yz\"",
};

static const char *ParseKeyTable4[] =
{       "'`\"",  // Numeric entry by ALPHANUM4_SCR with IMAGE_KEYSYM
  		"*!-",
	  	"()/",
	  	"[]\\",
	  	"{}|",
	  	"#$%",
	  	"&@^",
	  	"<=>",
	  	".,:",
	  	"_~;"
};

    // Flag to indicate that the current position entry is not complete 
    UBYTE InterDigit_Flag = 0;
    UBYTE TabIndex = 0;
    UBYTE PrevKey = 0;    // Previous Key pressed
	UBYTE clrcnt, limit;
	UBYTE retval, cnt, done, keyval;
	UBYTE MaxInputDispChar;

	// Get Maximum Input Box Characters, will be stored in MaxInputDispChar 
	MaxInputDispChar = GetInputBoxMaxDsp ();
	if ( MaxInputDispChar )
	{
		// Set maximum input to that calculated in SDL 
		screen_limit = MaxInputDispChar;
	}
	else
	{
		// For some reason value was not calculated, set to full screen 
		// Calculate maximum number of characters to display on a line, in screen_limit 
		MaxCharDisp ();
	}
	clrcnt = cnt = 0;
	memset( ( UBYTE * ) Dspbuf, 0,sizeof( Dspbuf ) );	// Init input buffer 


	// Check for manual account entry 
	if ( ( DspMode >= '0' ) && ( DspMode <= '9' ) )
	{
		cnt = 1;
		Dspbuf[0] = DspMode;	// Get first character 
	}

	if ( InitialValStr[0] != 0 )
	{
		strcpy( Dspbuf, InitialValStr );
		cnt = StrLn( ( char * ) Dspbuf, sizeof( Dspbuf ) );
	}

	done = 0;					// loop done flag 

	do							// Get a key, pad trigger, or timeout 
	{
		retval = StrLn( ( char * ) Dspbuf, sizeof( Dspbuf ) );

        // Clear string for display
		memset( Dspbuf2, ' ',Maxlen );

		if ( Maxlen <= screen_limit )
			limit = Maxlen;
		else
		{
			// Adjust to what the screen can display 
			limit = screen_limit;
			Dspbuf2[limit] = '\0';

		}

		if ( retval <= limit )
		{
			if ( DspMode == 2 )	// Password input ? 
				memset( ( UBYTE * ) Dspbuf2 + limit - retval, '*',retval  );
			else				// (DspMode == 2)  Echo 
				memcpy( Dspbuf2 + limit - retval, Dspbuf, retval );
		}
		else
        {					
            // Display the later part of the text 
			memcpy( Dspbuf2, Dspbuf + retval - limit, limit );
        }

        // NULL terminate string 
		Dspbuf2[(UBYTE)Maxlen] = 0;	

        // If NOT inter-digit situation by alphanum mode
        if ( ( 0 == InterDigit_Flag ) && ( 0 != InputMode ) )
        {
            // Move out last entered char from the edit (with flashing) pozitionn
            // (add space at end of Dspbuf2 string)
            Dspbuf2[(UBYTE)Maxlen] = ' ';
            Dspbuf2[(UBYTE)Maxlen+1] = '\0';
        }

		if ( Dspbuf[0] || clrcnt )	// Anything to display ? 
		{
			UpdateInputBox( Dspbuf2 );
		}

         if ( 0 != InterDigit_Flag )
         {
              // Use one second timeout for inter-digit
              keyval = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN1, 1 );
              
              // If timed out:
              if (0 == keyval)
              {
                  // Clear inter-digit flag
                  InterDigit_Flag = 0;
                  TabIndex = 0;

                  // Apply the new value
                  cnt++;

                  // Redraw input box 
                  Dspbuf2[(UBYTE)Maxlen] = ' ';	
                  Dspbuf2[(UBYTE)Maxlen+1] = 0;	

                  // clear the input box 
                  UpdateInputBox( "" );	

                  // Anything to display ? 
                  if ( Dspbuf[0] || clrcnt )	
                  {
                      UpdateInputBox( Dspbuf2 );
                  }

                  // Start entry with user (long) timeout
                  keyval = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN1, UserTimeout );
              }
         }
         else
         {
              keyval = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN1, UserTimeout );
         } 

		switch ( keyval )
		{
			case 0:			// time out 
				done = 1;
				break;

			case CLEAR_KY:		// Clear Input window 
				if ( !cnt )		// Clear default ? 
				{
					clrcnt++;	// Increment Clearkey count 
					if ( clrcnt >= 2 )
					{
						done = 1;
						keyval = CANCEL_KY;
					}
				}
				else
				{
					cnt = 0;	// Reset character count                
					// Set clear count to one so if another 
					clrcnt = 1;	// clear will return a cancel.          
				}
				memset( ( UBYTE * ) Dspbuf, 0,( UWORD ) ( Maxlen + 1 ) );	// Clear display buffer
                InterDigit_Flag = 0;
                break;

			case BSP_KY:
			case 0x08:
				if ( !cnt )
				{
					if ( GlobalDsp & ZAPKEYON )
					{
						clrcnt = 99;
						memset( ( UBYTE * ) Dspbuf, 0,( UWORD ) ( Maxlen + 1 ) );	// Clear display buffer 
					}
					else
						SDK_Beeper( TENMS * 40 );	// Nothing to backspace 
				}
				else
				{
					cnt--;
					Dspbuf[cnt] = 0;	// Clear last character 

                    if ( !cnt )
                    {
                      // Redraw input box 
                      Dspbuf2[(UBYTE)Maxlen] = ' ';	
                      Dspbuf2[(UBYTE)Maxlen+1] = 0;	

                      // clear the input box 
                      UpdateInputBox( "" );	
                    }
                }
				break;


			case ENTER_KY:
                // Must enter at least Minlen number of char 
				if ( cnt < Minlen )	
				{
                    // Long beep when no required length attain and no finish
					SDK_Beeper( TENMS * 40 );
				}
				else
                {            
                    // Normal beep and finish
					SDK_Beeper( TENMS * 12 );
					done = 1;
                }
				break;

			case ABC_KY:		// screen toggle key 
				done = 1;
				break;

			case CANCEL_KY:
				done = 1;
				break;

			default:
				if ( cnt >= Maxlen )	// Exceed Maxlen length 
				{
					SDK_Beeper( TENMS * 40 );
					break;
				}

                // Parse NUMERIC entry
				if ( ( '0' <= keyval && '9' >= keyval ) )
				{
                    // Nimeric simple entry
                    if ( 0 == InputMode )
                    {
                        Dspbuf[cnt] = keyval;	// Collect the character                     }
                        cnt++;
                        clrcnt = 0;	// Reset flag of clear default 
                    }
                    // Alphanumeric entry
                    else
                    {
                        // If another key pressed
                        if ( PrevKey != keyval )
                        {
                            // Start at 1-st table entry
                            TabIndex = 0;

                            // Apply previous entry if it actual
                            if ( 0 != Dspbuf[cnt] )
                                cnt++;
    
                            // Update "previous key"  
                            PrevKey = keyval;
                        }

                        // What the Mode at the moment
                        if ( cnt >= Maxlen )
                        {
                            SDK_Beeper( TENMS * 40 );
                        }
                        else switch ( SDLScreenId )
                        {
                            case ALPHANUM1_SCR: // Numeric keypad
                                Dspbuf[cnt] = *ParseKeyTable1[keyval - '0'];
                                cnt++;
                                break;

                            case ALPHANUM2_SCR: // Capital alphabetic keypad
                                Dspbuf[cnt] = ParseKeyTable2[keyval - '0'][TabIndex];

                                // Indicate that current entry can be changed
                                InterDigit_Flag = 1;
                                break;

                            case ALPHANUM3_SCR: // Small alphabetic keypad 
                                Dspbuf[cnt] = ParseKeyTable3[keyval - '0'][TabIndex];

                                // Indicate that current entry can be changed
                                InterDigit_Flag = 1;
                                break;

                            case ALPHANUM4_SCR: // Symbol keypad
                                Dspbuf[cnt] = ParseKeyTable4[keyval - '0'][TabIndex];

                                // Indicate that current entry can be changed
                                InterDigit_Flag = 1;
                                break;

                            default:
                                break;
                        }

                        // To next table index
                        if (2 == TabIndex)
                            TabIndex = 0;
                        else
                            TabIndex++;

                        clrcnt = 0;	// Reset flag of clear default 
					}
				}
				break;
		}

	} while ( !done && ( clrcnt != 99 ) );

	if ( ( CANCEL_KY == keyval ) || ( 0 == keyval ) )
		retval = 0;
	else if ( ( ABC_KY == keyval ) )
		retval = 3;
	else
	{
		if ( clrcnt == 99 )		// This was a clear default 
			retval = 2;			// Indicating clear default 
		else if ( keyval > 0 )
			retval = 1;			// Input successful 
	}
	GlobalDsp = 0;				// Safety house keeping 
	return ( retval );
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Common code for two button entry type functions, YesNoEntry() for example
//!
//!  \param[in]
//!     screen      Screen ID to prompt.
//!
//!  \param[in]
//!     ESParms     pointer to ESParms_str structure
//!                  TitleMid            Put in title bar
//!                  Line1Mid            Put on first line
//!                  Line2Mid            Put on second line
//!
//!  \return
//!     UBYTE       0 - timeout,
//!                 CANCEL_KY - CANCEL_KY key was pressed,
//!                 ENTER - YES was entered,
//!                 CELAR - NO  was entered.
//!
//!  \note
//!     None.
//!
//-----------------------------------------------------------------------------
static UBYTE TwoBtnEntry( UWORD screen, struct ESParms_str *pESParms )
{
	UBYTE retval;

	// setup title text 
	if ( pESParms->TitleMid == N_NullStr )
		User_Msgs[7] = MSG_DEFAULT_TITLE;
	else
		User_Msgs[7] = pESParms->TitleMid;

	// setup prompt text 
	User_Msgs[1] = pESParms->Line1Mid;
	User_Msgs[2] = pESParms->Line2Mid;

	DrawScreen( screen );

	do
	{
		retval = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

		switch ( retval )
		{
			case ENTER_KY:
			case CLEAR_KY:
			case CANCEL_KY:
			case 0:
				break;
			default:
				// Invalid key, notify user 
				SDK_Beeper( TENMS * 40 );
				retval = 1;
				break;
		}
	}
	while ( retval == 1 );
	Titlebuf[0] = 0;

	return retval;
}


//-----------------------------------------------------------------------
//! \brief     format user input amount.
//! 
//! \param
//!     None.
//! 
//! \return
//!     None.
//!
//! \note
//!     @TUM New variant which uses TCONF.TCLEADC,
//! 	it also caters for multicharacter Currency Symbols (like GBP)
//-----------------------------------------------------------------------
static void FormAmt( void )
{
	UBYTE c;
	static UBYTE * ptr;

	ptr = (UBYTE *)Dspbuf;

	while ( (*ptr) && (*ptr != ' ') && ((*ptr < '0') || (*ptr > '9')) ) ptr++;

    if ( 0 == TCONF.TCDPP )
    {
        memcpy( ptr, tmp, cnt );

        *(ptr + cnt) = '\0'; 
    }
    else if ( cnt < TCONF.TCDPP )
    {
        memcpy( ptr + (TCONF.TCDPP + 1) - cnt, tmp, cnt );
    }
    else
    {
		c = cnt - (TCONF.TCDPP - 1);

		/* Move digits until '.' (pounds) */
        memcpy( ptr, tmp, c );

        *(ptr += c) = '.';

        memcpy( ptr+1, tmp+c, (TCONF.TCDPP - 1) );

        *(ptr + TCONF.TCDPP) = '\0'; 
    }
}
