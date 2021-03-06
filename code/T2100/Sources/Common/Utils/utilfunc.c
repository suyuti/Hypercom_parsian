
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
//static void FormAmt( void );
extern void FormAmt( int cnt, char *pDest, const char *pBuf, char *CrrCode);
static void otele( char * const pDest, int len);


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
//  EntryScreen     Accepts input from user & saves in Dspbuf.
//
//  Parameters:     None.
//
//  Global Inputs:  ESParms.
//                  display mode:   1 - echo
//                                  2 - password input, echo '*'
//                                  '0' - '9' - Account number entry,
//                                              1st digit of acct. #.
//                                  3 - echo, cancel disabled
//                                  4 - password input, echo '*', cancel disabled
//
//                  Max length      Maximum data length allowed.
//
//                  Min length      Minimun data length expected.
//
//                  TitleMid        Put in title bar
//
//                                  TitleMid is usually used for
//                                  the transaction name.
//
//                  Line1Mid        Put on first line
//
//                                  Line1Mid is usually used for
//                                  the name of the value being entered.
//
//                  Line2Mid        Put on second line
//
//                                  Line2Mid is usually used for
//                                  the default value.
//
//                  Alpha           If non-zero, allows toggle to alpha screen
//                                  for input of alphanumeric
//
//  Returns:        UBYTE Placed in ESParms.Alpha
//                                  0   CANCEL_KY key was pressed
//                                      (or 2 CLEAR's in succession),
//                                  1   data was entered successfully,
//                                  2   Zap key was pressed.
//
//  Notes:          Must clear GlobalDsp flag before return.
//
//                  If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
//                  buffer is used. If it is equal to CustomMsg2, then
//                  CSTMSG2 is used.
//
//                  If TitleMid of ENTRY_STR is NOT equal to CustomMsg, 
//                  then the text identified by the TitleMid identifier is used.
//
//                  The above applies also to Line1Mid and Line2Mid
//
//                  If TitleMid is equal to N_NullStr then the value
//                  in Titlebuf or DefaultTitle is used.
//
//                  This function uses Titlebuf, Dspbuf and CSTMSG3.
//
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


		if ( pESParms->Alpha == 1 )
			DrawScreen( ALPHANUM1_SCR );	// contains ABC key
		else if(pESParms->Alpha == 2)
			DrawScreen( IPNUM_SCR );	// contains Select key 
		else
			DrawScreen( NUMENTRY_SCR );	// numeric entry only 

		// if keypad toggle key hit 
		if ( ( retval =
			   InputBox( pESParms->Alpha, pESParms->DspMode, pESParms->MaxLen,
						 pESParms->MinLen ) ) == 3 )
		{

			strcpy( InitialValStr, Dspbuf );
			DrawScreen( ALPHANUM2_SCR );

			// if keypad toggle key is hit 
			if ( ( retval =
				   InputBox( pESParms->Alpha, pESParms->DspMode, pESParms->MaxLen,
							 pESParms->MinLen ) ) == 3 )
			{

				strcpy( InitialValStr, Dspbuf );
				DrawScreen( ALPHANUM3_SCR );

				// if keypad toggle key 
				if ( ( retval =
					   InputBox( pESParms->Alpha, pESParms->DspMode,
								 pESParms->MaxLen, pESParms->MinLen ) ) == 3 )
				{
					strcpy( InitialValStr, Dspbuf );

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


//-----------------------------------------------------------------------------
//  XYesNoEntry     Accept YES or NO from user.
//
//  Parameters:     None.
//
//  Global Inputs:  ESParms.
//
//                  TitleMid        Put in title bar
//
//                  Line1Mid        Put on first line
//
//                  Line2Mid        Put on second line
//
//  Returns:        UBYTE Placed in ESParms.Alpha
//                      0           timeout,
//                      CANCEL_KY   CANCEL_KY key was pressed,
//                      ENTER       YES was entered,
//                      CELAR       NO  was entered.
//
//  Notes:          If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
//                  buffer is used. If it is equal to CustomMsg2, then
//                  CSTMSG2 is ued.
//
//                  If TitleMid of ENTRY_STR is NOT equal to CustomMsg, 
//                  then the text identified by the TitleMid identifier is used.
//
//                  The above applies also to Line1Mid and Line2Mid
//
//                  If TitleMid is equal to N_NullStr then the value
//                  in Titlebuf or DefaultTitle is used.
//
//-----------------------------------------------------------------------------

extern void XYesNoEntry( struct ESParms_str *pESParms )
{
	pESParms->Alpha = TwoBtnEntry( YESNO_SCR, pESParms );
}


//-----------------------------------------------------------------------------
//  XYesNoEntry     Accept YES or NEXT from user.
//
//  Parameters:     None.
//
//  Global Inputs:  ESParms.
//
//                  TitleMid        Put in title bar
//
//                  Line1Mid        Put on first line
//
//                  Line2Mid        Put on second line
//
//  Returns:        UBYTE Placed in ESParms.Alpha
//                      0           timeout,
//                      CANCEL_KY   CANCEL_KY key was pressed,
//                      ENTER       YES was entered,
//                      CLEAR       NEXT was entered.
//
//  Notes:          If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
//                  buffer is used. If it is equal to CustomMsg2, then
//                  CSTMSG2 is ued.
//
//                  If TitleMid of ENTRY_STR is NOT equal to CustomMsg, 
//                  then the text identified by the TitleMid identifier is used.
//
//                  The above applies also to Line1Mid and Line2Mid
//
//                  If TitleMid is equal to N_NullStr then the value
//                  in Titlebuf or DefaultTitle is used.
//
//-----------------------------------------------------------------------------

void XYesNextEntry(struct ESParms_str *pESParms)
{
	pESParms->Alpha = TwoBtnEntry(YES_NEXT_SCR, pESParms);
}


//-----------------------------------------------------------------------------
//  XContinueEntry  Cross-callable implementation of ContinueEntry.
//                  Accept Continue or Cancel from user
//
//  Parameters:     None.
//
//  Global Inputs:  ESParms.
//
//                  TitleMid            Put in title bar
//
//                  Line1Mid            Put on first line
//
//                  Line2Mid            Put on second line
//
//  Returns:        UBYTE Placed in ESParms.Alpha
//                          0   CANCEL key was pressed or timed out,
//                          1   CONTINUE was entered.
//
//  Notes:          If TitleMid of ENTRY_STR is equal to CustomMsg, the buffer
//                  pointed to by pTitle of ENTRY_STR is used.
//
//                  If TitleMid of ENTRY_STR is NOT equal to CustomMsg, 
//                  then the text identified by the TitleMid identifier is used.
//
//                  The above applies also to Line1Mid and pLine1 and Line2Mid
//                  and pLine2
//
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


/*-----------------------------------------------------------------------
 * Name:           XContinueEntryTimeout
 *                 Cross-callable implementation of ContinueEntryTimeout
 * Description:    Accept Continue or Cancel from user
 * Include files:
 * Parameters:    Globally passed from ESParms
 *                                 TitleMid or pTitle - put in title bar
 *                                 Line1Mid or pLine1 - put in first line
 *                                 Line2Mid or PLine2 - put on second line
 *
 * Return Value:   Placed in ESParms.Alpha
 * 				   0 CANCEL_KY key was pressed
 *                 1 CONTINUE was entered
 *				   2 Timeout occurred	
 *
 * Notes:         If TitleMid of ENTRY_STR is equal to CustomMsg, the buffer
 *                pointed to by pTitle of ENTRY_STR is used.
 *
 *                If TitleMid of ENTRY_STR is NOT equal to CustomMsg, then the text
 *                identified by the TitleMid identifier is used.
 *
 *                The above applies also to Line1Mid and pLine1 and Line2Mid
 *                and pLine2
 *-----------------------------------------------------------------------*/
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
//  XAmountEntry    Accept user input for amount.
//
//  Parameters:     None.
//
//  Global Inputs:  ESParms.
//
//                  char DspMode    1       zero amount is allowed,
//                                  other   zero amount is not allowed.
//
//                  char Maxlen     1       use settle sized amount,
//                                  other   standard sized amount.
//
//                  const ENTRY_STR * pEntry
//                                  Structure with screen information
//                                  TitleMid - put in title bar 
//                                  Line1Mid - put on first line    
//                                  Line2Mid - put on second line   
//
//                                  TitleMid is usually used for
//                                  the transaction name. 
//
//                                  Line1Mid is usually used for
//                                  the name of the value being entered.  
//
//                                  Line2Mid is usually used for
//                                  the default value.    
//
//  Returns:        Placed in ESParms.Alpha
//
//                                  0       CANCEL key was pressed,
//                                  1       amount was entered successfully,
//                                  3       CLEAR key was pressed.
//
//  Notes:          CSTMSG contains prompt to put on amount line.
//
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

        cnt = 0;
        tmp[0] = '0';
        FormAmt(cnt, defaultbuf, tmp, TCONF.TCLEADC );

	// Set max size of amount to be entered.    
	if ( IO_Settle_Amt )
		maxcount = CvtBin( TCONF.TCSDLEN );
	else
		maxcount = CvtBin( TCONF.TCDLEN );

	// initialize screen input box (include currency and '.')
        // ((maxcount-1) / 3) ka� tane '.' oldu�u
	SetInputBoxMaxChars( ( UBYTE ) ( maxcount + ((maxcount-1) / 3) + S_TCLEADC) );
	
        strcpy( Dspbuf, defaultbuf );

        SetInputBox( Dspbuf );
        

	DrawScreen( AMTENTRY_SCR );

	


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
				cnt = 0;
                                FormAmt(cnt, Dspbuf, tmp,   TCONF.TCLEADC );
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
					//strcpy( Dspbuf, defaultbuf );	/* Restore default value */
					//FormAmt( );
                                        FormAmt(cnt, Dspbuf, tmp, TCONF.TCLEADC );
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
					//strcpy( Dspbuf, defaultbuf );	/* Restore default value */
					//FormAmt( );
                                        FormAmt(cnt, Dspbuf, tmp, TCONF.TCLEADC );
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
//  InputBox        Accept user input keypress, or pad touch.
//
//  Parameters:     char InputMode  0       numeric input
//                                  non-0   alpha-numeric input
//
//                  char DspMode    1 - echo
//                                  2 - password input, echo '*' 
//                                  '0' - '9' - Account number entry, 
//                                              1st digit of acct. #.
//
//                  char Maxlen     Maximum data length allowed.
//
//                  char Minlen     Minimun data length expected.
//
//
//  Global Inputs:  None.
//
//  Returns:        UBYTE   0   CANCEL key was pressed or timed out,
//                          1   A key press or pad touch entered successfully,
//                          3   ABC_KEY was pressed,
//                          4   Numeric entry key ('#') was pressed.
//
//  Notes:          Contents of InitialValStr is initial value.
//
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

			case SELECT_KY:
				if(InputMode == 2)
				{
					if ( cnt >= Maxlen )	// Exceed Maxlen length 
					{
						SDK_Beeper( TENMS * 40 );
						break;
					}

					Dspbuf[cnt] = '.';
					cnt++;
				}
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
                    if ( (0 == InputMode) || (InputMode == 2) )
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
//  TwoBtnEntry     Common code for two button entry type functions,
//                  YesNoEntry() for example
//
//  Parameters:     UWORD screen        Screen ID to prompt.
//
//  Global Inputs:  ESParms.
//
//                  TitleMid            Put in title bar
//
//                  Line1Mid            Put on first line
//
//                  Line2Mid            Put on second line
//
//  Returns:        UBYTE   0           timeout,
//                          CANCEL_KY   CANCEL_KY key was pressed,
//                          ENTER       YES was entered,
//                          CELAR       NO  was entered.
//
//  Notes:          None.
//
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
		retval =
			GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

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


/*-----------------------------------------------------------------------
 * Name:            FormAmt
 * Description:     format user input amount.
 * Include files:   None.
 * Parameters:      None.
 * Return Value:    None.
 * Notes:           @TUM New variant which uses TCONF.TCLEADC,
 * 					it also caters for multicharacter Currency Symbols (like GBP)
 *-----------------------------------------------------------------------*/
static void FormAmtXXX( void )
{
/*
	UBYTE c, i ;
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

	// Move digits until '.' (pounds) 
        memcpy( ptr, tmp, c );

        *(ptr += c) = '.';

        memcpy( ptr+1, tmp+c, (TCONF.TCDPP - 1) );
        
        for ( i = 0; i < 3; i++)
            *(ptr + TCONF.TCDPP + i) = TCONF.TCLEADC[i];
        *(ptr + TCONF.TCDPP + S_TCLEADC) = '\0'; 
    }

*/
    #define SRC_LEN 20
    int iDest; // dest indeksi
    int iSrc;  // src  indeksi
    int len;   // kopyalanacak veri boyutu 
    char TempStr[3];  // Para birimi icin
    int j;
    char *pDest;
    int digitNumber;

    digitNumber = cnt;
    pDest = Dspbuf;

    // Clear Source buffer
    memset( pDest, 0x00, SRC_LEN);
    pDest[SRC_LEN] = 0;

    // bo� ise "0" yazmas� i�in gelen dataya '0' ekle
    if ( digitNumber == 0 ) 
    {
        digitNumber = 1;
        tmp[0] = '0';
    }

    memset( TempStr, 0, 3 );

    // Curreny Code u al
    len =  0;
    for (j = 0; j < 3; ++j) {
        if ((TCONF.TCLEADC[j] > 0x40) && (TCONF.TCLEADC[j] < 0x5B)) {	// A-Z
            TempStr[len] = TCONF.TCLEADC[j];            
            ++len;
        }
    }
   
    // copy Currency Code    
    iDest = digitNumber;
    memcpy( &pDest[iDest], TempStr, len );
    
    // Source indeksini ayarla
    iSrc = digitNumber ;

    // Karakterleri ��er ��er ekrana bas
    while(digitNumber > 0) 
    {
        // 2 virgul arasinda kac basamak var 
        len = (digitNumber > 3 ) ? 3 : digitNumber;
        
        // Indeksleri ayarla
        digitNumber   -= len;
        iDest -= len;
        iSrc  -= len;

        // 2 virgul arasini kopyala
        memcpy( &pDest[iDest], &tmp[iSrc], len ); 
        
        // Yazilacak basamak var '.' koy
        if ( digitNumber > 0 ) 
        {
            otele( pDest, SRC_LEN - len );
            pDest[iDest] = '.';
        }
            
    }

}

// -------------------------------------------
//
// Karakter dizisini belirli uzunluk kadar sa�a kayd�r�r
// Parametre : 
//    char * const pDest : otelenecek karakter dizisi
//    int len            : Saga oteleme miktari
// -------------------------------------------
static void otele( char * const pDest, int len)
{
    int i;

    for ( i = len ; i >= 0 ; --i )
    {
      *(pDest + i + 1) =  *(pDest + i);
    }
}


extern void FormAmt( int cnt, char *pDest, const char *pBuf, char *CrrCode)
{
    int iDest; // dest indeksi
    int iSrc;  // src  indeksi
    int len;   // kopyalanacak veri boyutu 
    char TempStr[3];  // Para birimi icin
    int totalLen;
    int j;

    memset( TempStr, 0, 3 );

    // Curreny Code u al
    len =  0;
    for (j = 0; j < 3; ++j) {
        if ((CrrCode[j] > 0x40) && (CrrCode[j] < 0x5B)) {	// A-Z
            TempStr[len] = CrrCode[j];            
            ++len;
        }
    }

    // Formatlanm�� tutar�n uzunlu�unu bul
    totalLen = getFormattedLen( cnt, TCONF.TCDPP );
    // Para birimi kodunun uzunlu�unu ekle
    totalLen += len;


    // Clear Source buffer
    memset( pDest, ' ', totalLen);
    pDest[totalLen] = 0;
   
    // copy Currency Code
    iDest = totalLen -  len;
    memcpy( &pDest[iDest], TempStr, len );

    iSrc = cnt;
    /*
    // ONDALIK KISIM var m�?
    if ( TCONF.TCDPP > 0 )
    {
        // �rn :0,000 i�in en soldaki 0 i�in +1
        len = TCONF.TCDPP + 1 ;
        //'0' ile doldur
        memset( &pDest[iDest-len], '0', len );
        // En soldaki '0' dan sonraki eleman ',' olmal�
        pDest[iDest-len+1] = ',';  

        // Ondalik ka� basamak girilmi�
        len = (cnt > TCONF.TCDPP-1) ? TCONF.TCDPP-1 : cnt;
        
        // Kalan eleman say�s�
        cnt   -= len;
        // Indeksleri ayarla
        iDest -= len;
        iSrc  -= len;

        // Ondal�k k�sm� kopyala
        memcpy( &pDest[iDest], &pBuf[iSrc], len);
        
        // Virg�l var bir basamak daha geri gel
        --iDest ; 
    }else 
    {   
        // '0' kopyala
        pDest[iDest-1] = '0';
    }
    */
    // �lk de�er olarak "0" ver 
    if ( cnt == 0)
    {
        pDest[iDest-1] = '0';
    }
    

    while(cnt > 0) 
    {
        // 2 virgul arasinda kac basamak var 
        len = (cnt > 3 ) ? 3 : cnt;
        
        // Kalan eleman say�s�
        cnt   -= len;
        // Indeksleri ayarla
        iDest -= len;
        iSrc  -= len;

        // 2 virgul arasini kopyala
        memcpy( &pDest[iDest], &pBuf[iSrc], len ); 
        
        // Karakterlerin tamam� yaz�lmad� '.' koy
        if ( cnt > 0 ) 
        {
            --iDest;
            pDest[iDest] = '.';
        }       
    }
}

/// <summary>
/// Formats�z uzunlugu verilen tutar�n formatl� olarak uzunlu�unu bulur
/// </summary>
/// <param name="len"> Tutar Uzunlu�u</param>
/// <param name="decimalLen"></param>
/// <returns>int Formatl� Uzunluk</returns>
extern int getFormattedLen( int len, int decimalLen )
{
    // Formatl� halde ka� basamakl�k alan gerekir hesapla
    if ( decimalLen > 0 )
    {
        // Ondal�k k�sma 3 l� basamaklar i�in '.' konulmayaca�� i�in 
        // TCONF.TCDPP k�sm�n� d��
        len += ((len - decimalLen - 1) / 3 ); 
        // Ondalik kisim ile tam k�sm� ay�ran ',' i�in
        ++len; 

        // Decimal alan (virg�lle beraber) + virg�l�n solundaki 1 tane '0' olmal�
        if ( len < decimalLen + 1)
            len = decimalLen + 1;
        
    }else
    {
        // Her 4. , 7. , 10. alanlar i�in yeni bir '.' nokta konulur
        len += ((len - 1) / 3 );  

        // '0' ilk de�eri i�in
        if ( 0 == len)
            len = 1;
    }
    

    return len;
}