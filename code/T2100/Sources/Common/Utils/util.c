
//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             util.c
//      General Util functions.
//
//=============================================================================

#include "string.h"
#include "basictyp.h"

#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "comdata.h"
#include "transdef.h"
#include "journal.h"
#include "pinpad.h"
#include "utilfunc.h"
#include "read_io.h"
#include "inputcsh.h"

#include "util.h"


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

const ENTRY_STR YNEntryTrk2 = {
	N_NullStr,
	CustomMsg,
	CustomMsg2,
};

//=============================================================================
// Private data definitions
//=============================================================================

static ENTRY_STR Pan4Entry;

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  GetOneKey       Get One Key from NUMERIC Layout or AlphaNumeric layout
//
//  Parameters:     UWORD    map    Device Map.
//                  OS_TIMES tmout  Timeout.
//
//  Global Inputs:  None.
//
//  Returns:        UBYTE           0 - input time-out,
//                          otherwise - Key value entered.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

extern UBYTE GetOneKey( UWORD map, OS_TIMES tmout )
{
	UBYTE InputType;
	Bool bPinCardStatus;

	InputType = read_io( map, tmout, &bPinCardStatus, &DevFlag );

	switch ( InputType )
	{
		case DEV_KBD:
			break;


		case DEV_PAD:
// We should not be making intelligent decisions at this level
// but this is a quick fix for some event Ids which
// can't be easily changed.
			switch ( ButtonId )
			{
				case 0x1B:		// ESC = CLEAR
					KeyVal = CLEAR_KY;
					break;

				default:
					KeyVal = ( UBYTE ) ButtonId;
					break;
			}
			break;

		case 0:				// Timed out
			if ( !( TCONF.TCLOCOP & TCL_LOCKED ) )
            {
                if  (3 < tmout)
                {
                    // Four beeps by long timeout
                    SDK_BeepIt( 4 );
                }
                else
                {
                    // Single beep only by short timeout
					SDK_Beeper( 12 * TENMS );
                }
            }

			KeyVal = 0;
			break;
	}
	return ( KeyVal );
}


//-----------------------------------------------------------------------------
//  NumEntry        Accept numeric input from user & save in Dspbuf.
//
//  Parameters:     char DspMode    1 - echo
//                                  2 - password input, echo '*' 
//                                  '0' - '9' - Account number entry, 
//                                              1st digit of acct. #.
//                                  3 - echo, cancel disabled
//                                  4 - password input, echo '*',
//                                      cancel disabled.
//
//                  char Maxlen     Maximum data length allowed.
//
//                  char Minlen     Minimun data length expected.
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
//  Global Inputs:  None.
//
//  Returns:        UBYTE   0   CANCEL_KY key was pressed
//                              (or 2 CLEAR's in succession)
//                          1   data was entered successfully
//                          2   Zap key was pressed
//
//  Notes:          Must clear GlobalDsp flag before return.
//          
//                  If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
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

extern UBYTE NumEntry( char DspMode, char Maxlen, char Minlen,
					   const ENTRY_STR * pEntry )
{
	struct ESParms_str ESParms;

	ESParms.DspMode = DspMode;
	ESParms.MaxLen = Maxlen;
	ESParms.MinLen = Minlen;
	ESParms.Alpha = 0;
	ESParms.TitleMid = pEntry->TitleMid;
	ESParms.Line1Mid = pEntry->Line1Mid;
	ESParms.Line2Mid = pEntry->Line2Mid;

	EntryScreen ( &ESParms );

	return ESParms.Alpha;		// Alpha contains return value 
}


//-----------------------------------------------------------------------------
//  AlphaNumEntry   Accept alpha and numeric input from user & save in Dspbuf.
//
//  Parameters:     char DspMode    1 - echo
//                                  2 - password input, echo '*' 
//                                  '0' - '9' - Account number entry, 
//                                              1st digit of acct. #.
//                  char Maxlen     Maximum data length allowed.
//                  char Minlen     Minimun data length expected.
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
//  Global Inputs:  None.
//
//  Returns:        UBYTE   0   CANCEL_KY key was pressed
//                              (or 2 CLEAR's in succession)
//                          1   data was entered successfully
//                          2   Zap key was pressed
//
//  Notes:          Must clear GlobalDsp flag before return.
//
//                  If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
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

extern UBYTE AlphaNumEntry( char DspMode, char Maxlen, char Minlen,
							const ENTRY_STR * pEntry )
{
	struct ESParms_str ESParms;

	ESParms.DspMode = DspMode;
	ESParms.MaxLen = Maxlen;
	ESParms.MinLen = Minlen;
	ESParms.Alpha = 1;
	ESParms.TitleMid = pEntry->TitleMid;
	ESParms.Line1Mid = pEntry->Line1Mid;
	ESParms.Line2Mid = pEntry->Line2Mid;

	EntryScreen ( &ESParms );

	return ESParms.Alpha;		// Alpha contains return value 
}


extern UBYTE IpNumEntry( char DspMode, char Maxlen, char Minlen,
							const ENTRY_STR * pEntry )
{
	struct ESParms_str ESParms;

	ESParms.DspMode = DspMode;
	ESParms.MaxLen = Maxlen;
	ESParms.MinLen = Minlen;
	ESParms.Alpha = 2;
	ESParms.TitleMid = pEntry->TitleMid;
	ESParms.Line1Mid = pEntry->Line1Mid;
	ESParms.Line2Mid = pEntry->Line2Mid;

	EntryScreen ( &ESParms );

	return ESParms.Alpha;		// Alpha contains return value 
}


//-----------------------------------------------------------------------------
//  YesNoEntry      Accept YES or NO from user.
//
//  Parameters:     const ENTRY_STR * pEntry
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
//  Global Inputs:  None.
//
//  Returns:        UBYTE   0           Timeout,
//                          CANCEL_KY   CANCEL_KY key was pressed,
//                          ENTER       YES was selected,
//                          CLEAR       NO  was selected.
//
//  Notes:          If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
//                  buffer is used. If it is equal to CustomMsg2, then
//                  CSTMSG2 is used.
//
//                  If TitleMid of ENTRY_STR is NOT equal to CustomMsg, then
//                  the text identified by the TitleMid identifier is used.  
//
//                  The above applies also to Line1Mid and Line2Mid.
//
//                  If TitleMid is equal to N_NullStr then the value
//                  in Titlebuf or DefaultTitle is used.
//
//-----------------------------------------------------------------------------

extern UBYTE YesNoEntry( const ENTRY_STR * pEntry )
{
	struct ESParms_str ESParms;

	ESParms.TitleMid = pEntry->TitleMid;
	ESParms.Line1Mid = pEntry->Line1Mid;
	ESParms.Line2Mid = pEntry->Line2Mid;

	XYesNoEntry ( &ESParms );

	return ESParms.Alpha;
}


//-----------------------------------------------------------------------------
//  YesNextEntry    Accept YES or Next from user.
//
//  Parameters:     const ENTRY_STR * pEntry
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
//  Global Inputs:  None.
//
//  Returns:        UBYTE   0           Timeout,
//                          CANCEL_KY   CANCEL_KY key was pressed,
//                          ENTER       YES was selected,
//                          CLEAR       NEXT was selected.
//
//  Notes:          If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
//                  buffer is used. If it is equal to CustomMsg2, then
//                  CSTMSG2 is used.
//
//                  If TitleMid of ENTRY_STR is NOT equal to CustomMsg, then
//                  the text identified by the TitleMid identifier is used.  
//
//                  The above applies also to Line1Mid and Line2Mid.
//
//                  If TitleMid is equal to N_NullStr then the value
//                  in Titlebuf or DefaultTitle is used.
//
//-----------------------------------------------------------------------------

extern UBYTE YesNextEntry( const ENTRY_STR * pEntry )
{
	struct ESParms_str ESParms;

	ESParms.TitleMid = pEntry->TitleMid;
	ESParms.Line1Mid = pEntry->Line1Mid;
	ESParms.Line2Mid = pEntry->Line2Mid;

	XYesNextEntry ( &ESParms );

	return ESParms.Alpha;

}


//-----------------------------------------------------------------------------
//  ShowIssuer_Tran Display Issuer name and transaction type on top line
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Nothing.
//
//  Notes:          This function uses SDL script TITLEBAR_SCR.
//
//-----------------------------------------------------------------------------

extern void ShowIssuer_Tran( void )
{
	// Display the Issuer name and the transaction type on top line 
	DrawScreen( TITLEBAR_SCR );
}


//-----------------------------------------------------------------------------
//  GetPAN4         Get last 4 digits of PAN and memcmpe with that from card
//                  swipe.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        UBYTE   0           If the last 4 digits entered does 
//                                      NOT match the card swipe,
//                          1           If the last 4 digits entered MATCH
//                                      the card,
//                          CANCEL_KY   If cancel key was pressed.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

extern UBYTE GetPAN4( void )
{
	static UBYTE retval;
	static char buf[21];


	memset( ( UBYTE * ) buf, 0, sizeof( buf ) );
	GetMsg( EntPan4, buf );
	memcpy( CSTMSG2, buf, sizeof( buf ) );

	// NumEntry parameters 
	Pan4Entry.TitleMid = N_NullStr;
	Pan4Entry.Line1Mid = CustomMsg2;
	Pan4Entry.Line2Mid = CustomerCard;

	retval = NumEntry( 1, 4, 4, &Pan4Entry );

	if ( retval )
	{
		memset( ( UBYTE * ) buf, 0, sizeof( buf ) );
		retval = ( char ) MovPanAsc( buf, TRINP.TRPAN, 20 );

		// calculate the offset for the last 4 bytes of ACCT# 

		retval = 20 - retval - 4;
		if ( !memcmp( ( UBYTE * ) ( &buf[retval] ), ( UBYTE * ) Dspbuf, 4 ) )
			retval = 1;
		else
			retval = 0;
	}
	else
		retval = CANCEL_KY;

	return retval;
}


//-----------------------------------------------------------------------------
//  JournalFullTest Test for journal full or nearly full.  Also, if Signature
//                  Capture test for file system full or nearly full.
//
//  Parameters:     None.
//
//  Global Inputs:  None.
//
//  Returns:        Bool    True  - Journal or file system is full,
//                          False - otherwise.
//
//  Notes:          None.
//
//-----------------------------------------------------------------------------

extern Bool JournalFullTest( void )
{
    int RoomCount;

    // Get the number of transaction can be allocated
    RoomCount = JournalRoom ( );

	// If 3 or less journal records available then there is no room for more 
	if ( 3 >= RoomCount )
	{
		ShowErrMsg( SettlementRequired );
		return ( True );
	}

	// If between 4 and 10 journal records available then issue a warning then
	// restore top line 
	if ( 10 >= RoomCount )
	{
		ShowErrMsg( SettlementRequired );
		ShowIssuer_Tran(  );
		return ( False );
	}

	return ( False );
}


//-----------------------------------------------------------------------------
//  ContinueEntry   Accept Continue or Cancel from user.
//
//  Parameters:     const ENTRY_STR * pEntry
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
//  Global Inputs:  None.
//
//  Returns:        UBYTE   0 CANCEL_KY key was pressed or timed-out
//                          1 CONTINUE was entered 
//
//  Notes:          If TitleMid of ENTRY_STR is equal to CustomMsg, the buffer
//                  pointed to by pTitle of ENTRY_STR is used.  
//
//                  If TitleMid of ENTRY_STR is NOT equal to CustomMsg, then
//                  the text identified by the TitleMid identifier is used.  
//
//                  The above applies also to Line1Mid and pLine1 and Line2Mid 
//                  and pLine2.
//
//-----------------------------------------------------------------------------

extern UBYTE ContinueEntry( const ENTRY_STR * pEntry )
{
	struct ESParms_str ESParms;

	ESParms.TitleMid = pEntry->TitleMid;
	ESParms.Line1Mid = pEntry->Line1Mid;
	ESParms.Line2Mid = pEntry->Line2Mid;

	XContinueEntry ( &ESParms );

	return ESParms.Alpha;
}


/*-----------------------------------------------------------------------
 * Name:           ContinueEntryTimeout
 * Description:    Accept Continue or Cancel from user
 * Include files:
 * Parameters:    1. ENTRY_STR     TitleMid or pTitle - put in title bar    
 *                                 Line1Mid or pLine1 - put in first line   
 *                                 Line2Mid or PLine2 - put on second line  
 *
 * Return Value:   0 CANCEL_KY key was pressed
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
UBYTE ContinueEntryTimeout(const ENTRY_STR *pEntry)
{
	struct ESParms_str ESParms;

  	ESParms.TitleMid = pEntry->TitleMid;	
	ESParms.Line1Mid = pEntry->Line1Mid;
	ESParms.Line2Mid = pEntry->Line2Mid;

	XContinueEntryTimeout ( &ESParms );

    return ESParms.Alpha;
}


//-----------------------------------------------------------------------------
//  ClrCstMsgs      Clears the Custom messages, CSTMSG and CSTMSG2
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

extern void ClrCstMsgs( void )
{
	memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );
	memset( ( UBYTE * ) CSTMSG2, 0, sizeof( CSTMSG2 ) );
}


//=============================================================================
// Private function definitions
//=============================================================================
