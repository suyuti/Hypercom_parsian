
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
//!  \brief     Get One Key from NUMERIC Layout or AlphaNumeric layout
//!
//!  \param
//!     map         Device Map.
//!     tmout       Timeout.
//!
//!  \return
//!       UBYTE           0 - input time-out, otherwise - Key value entered.
//!
//!  \note
//!          None.
//!
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
//!  \brief     Accept numeric input from user & save in Dspbuf.
//!
//!  \param
//!     DspMode     1 - echo
//!                 2 - password input, echo '*' 
//!                 '0' - '9' - Account number entry, 
//!                                 1st digit of acct. #.
//!                 3 - echo, cancel disabled
//!                 4 - password input, echo '*',
//!                                 cancel disabled.
//!  \param
//!     Maxlen      Maximum data length allowed.
//!  \param
//!     Minlen      Minimun data length expected.
//!  \param
//!     pEntry      Pointer to structure ENTRY_STR with screen information
//!                     TitleMid - put in title bar 
//!                     Line1Mid - put on first line    
//!                     Line2Mid - put on second line   
//!
//!                     TitleMid is usually used for
//!                                  the transaction name. 
//!
//!                     Line1Mid is usually used for
//!                                  the name of the value being entered.  
//!
//!                     Line2Mid is usually used for
//!                                  the default value.    
//!
//!  \return
//!     UBYTE       0 - CANCEL_KY key was pressed
//!                              (or 2 CLEAR's in succession)
//!                 1 - data was entered successfully
//!                 2 - Zap key was pressed
//!
//!  \note
//!     Must clear GlobalDsp flag before return.
//!          
//!     If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
//!     buffer is used. If it is equal to CustomMsg2, then
//!     CSTMSG2 is ued.
//!
//!     If TitleMid of ENTRY_STR is NOT equal to CustomMsg, 
//!     then the text identified by the TitleMid identifier is used.
//!
//!     The above applies also to Line1Mid and Line2Mid 
//!
//!     If TitleMid is equal to N_NullStr then the value
//!     in Titlebuf or DefaultTitle is used.
//!
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



extern void VPP_PinEntry( char DspMode, char Maxlen, char Minlen, const ENTRY_STR * pEntry )
{
	struct ESParms_str ESParms;

	ESParms.DspMode = DspMode;
	ESParms.MaxLen = Maxlen;
	ESParms.MinLen = Minlen;
	ESParms.Alpha = 0;
	ESParms.TitleMid = pEntry->TitleMid;
	ESParms.Line1Mid = pEntry->Line1Mid;
	ESParms.Line2Mid = pEntry->Line2Mid;

    // Set up screen msgs for prompts and title.
    if ( ESParms.TitleMid == N_NullStr )
        User_Msgs[7] = MSG_DEFAULT_TITLE;
    else
        User_Msgs[7] = ESParms.TitleMid;

    // setup prompt text 
    User_Msgs[1] = ESParms.Line1Mid;
    User_Msgs[2] = ESParms.Line2Mid;

    // Initialize screen input box 
    SetInputBoxMaxChars( ESParms.MaxLen );
    SetInputBox( "" );

#if	defined(MAKE_OPTIMUM)
        // If we started to enter something, remove the original value,
        // otherwise it flashes not nicely on T4100
//        if ( ESParms.Alpha && 0 != InitialValStr[0] ) User_Msgs[1] = N_NullStr;
#endif	//defined(MAKE_OPTIMUM)

    // Display the input screen. 
    DrawScreen( NUMENTRY_SCR );	// common entry screen ( numeric entry only )
}

extern UBYTE VPP_PinEntry_( char DspMode, char Maxlen, char Minlen,
						   const ENTRY_STR * pEntry, UBYTE* dest )
{
	struct ESParms_str ESParms;

	ESParms.DspMode = DspMode;
	ESParms.MaxLen = Maxlen;
	ESParms.MinLen = Minlen;
	ESParms.Alpha = 0;
	ESParms.TitleMid = pEntry->TitleMid;
	ESParms.Line1Mid = pEntry->Line1Mid;
	ESParms.Line2Mid = pEntry->Line2Mid;

	VPP_EntryScreen( &ESParms, dest );

	return ESParms.Alpha;		// Alpha contains return value 
}
//-----------------------------------------------------------------------------
//!  \brief     Accept alpha and numeric input from user & save in Dspbuf.
//!
//!  \param
//!     DspMode     1 - echo
//!                 2 - password input, echo '*' 
//!                 '0' - '9' - Account number entry, 
//!                             1st digit of acct. #.
//!  \param
//!     Maxlen      Maximum data length allowed.
//!  \param
//!     Minlen      Minimun data length expected.
//!  \param
//!     pEntry      pointer to ENTRY_STR structure with screen information
//!                     TitleMid - put in title bar 
//!                     Line1Mid - put on first line    
//!                     Line2Mid - put on second line   
//!
//!                     TitleMid is usually used for the transaction name. 
//!
//!                     Line1Mid is usually used for
//!                     the name of the value being entered.  
//!
//!                     Line2Mid is usually used for
//!                     the default value.    
//!
//!  \return
//!     UBYTE       0 - CANCEL_KY key was pressed
//!                     (or 2 CLEAR's in succession)
//!                 1 - data was entered successfully
//!                 2 - Zap key was pressed
//!
//!  \note
//!     Must clear GlobalDsp flag before return.
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


//-----------------------------------------------------------------------------
//!  \brief      Accept YES or NO from user.
//!
//!  \param
//!     pEntry          pointer to ENTRY_STR structure with screen information
//!                         TitleMid - put in title bar 
//!                         Line1Mid - put on first line    
//!                         Line2Mid - put on second line   
//!
//!                         TitleMid is usually used for
//!                         the transaction name. 
//!
//!                         Line1Mid is usually used for
//!                         the name of the value being entered.  
//!
//!                         Line2Mid is usually used for
//!                         the default value.    
//!
//!  \return
//!     UBYTE       0 - Timeout,
//!                 CANCEL_KY - CANCEL_KY key was pressed,
//!                 ENTER - YES was selected,
//!                 CLEAR - NO  was selected.
//!
//!  \note
//!     If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
//!     buffer is used. If it is equal to CustomMsg2, then
//!     CSTMSG2 is used.
//!     If TitleMid of ENTRY_STR is NOT equal to CustomMsg, then
//!     the text identified by the TitleMid identifier is used.  
//!     The above applies also to Line1Mid and Line2Mid.
//!     If TitleMid is equal to N_NullStr then the value
//!     in Titlebuf or DefaultTitle is used.
//!
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
//!  \brief     Accept YES or Next from user.
//!
//!  \param
//!     pEntry      pointer to ENTRY_STR structure with screen information
//!                     TitleMid - put in title bar 
//!                     Line1Mid - put on first line    
//!                     Line2Mid - put on second line   
//!
//!                     TitleMid is usually used for
//!                     the transaction name. 
//!
//!                     Line1Mid is usually used for
//!                     the name of the value being entered.  
//!
//!                     Line2Mid is usually used for
//!                     the default value.    
//!
//!  \return
//!     UBYTE       0 - Timeout,
//!                 CANCEL_KY - CANCEL_KY key was pressed,
//!                 ENTER - YES was selected,
//!                 CLEAR - NEXT was selected.
//!
//!  \note
//!     If TitleMid of ENTRY_STR is equal to CustomMsg, the CSTMSG
//!     buffer is used. If it is equal to CustomMsg2, then
//!     CSTMSG2 is used.
//!     If TitleMid of ENTRY_STR is NOT equal to CustomMsg, then
//!     the text identified by the TitleMid identifier is used.  
//!     The above applies also to Line1Mid and Line2Mid.
//!     If TitleMid is equal to N_NullStr then the value
//!     in Titlebuf or DefaultTitle is used.
//!
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
//!  \brief     Display Issuer name and transaction type on top line
//!
//!  \param
//!     None.
//!
//!  \return
//!     Nothing.
//!
//!  \note
//!     This function uses SDL script TITLEBAR_SCR.
//!
//-----------------------------------------------------------------------------
extern void ShowIssuer_Tran( void )
{
	// Display the Issuer name and the transaction type on top line 
	DrawScreen( TITLEBAR_SCR );
}


//-----------------------------------------------------------------------------
//!  \brief     Get last 4 digits of PAN and memcmpe with that from card swipe.
//!
//!  \param
//!     None.
//!
//!  \return
//!     UBYTE       0 - If the last 4 digits entered does 
//!                     NOT match the card swipe,<br>
//!                 1 - If the last 4 digits entered MATCH the card,<br>
//!                 CANCEL_KY - If cancel key was pressed.
//!
//!  \note
//!     None.
//!
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
//!  \brief     Test for journal full or nearly full.  Also, if Signature
//!             Capture test for file system full or nearly full.
//!
//!  \param
//!     None.
//!
//!  \return
//!     Bool        True  - Journal or file system is full,
//!                 False - otherwise.
//!
//!  \note
//!     None.
//!
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
//! \brief     ContinueEntry   Accept Continue or Cancel from user.
//!
//!  \param
//!     pEntry      pointer to ENTRY_STR structure with screen information
//!                     TitleMid - put in title bar 
//!                     Line1Mid - put on first line    
//!                     Line2Mid - put on second line   
//!
//!                     TitleMid is usually used for
//!                     the transaction name. 
//!
//!                     Line1Mid is usually used for
//!                     the name of the value being entered.  
//!
//!                     Line2Mid is usually used for
//!                     the default value.    
//!
//!  \return
//!     UBYTE       0 CANCEL_KY key was pressed or timed-out
//!                 1 CONTINUE was entered 
//!
//!  \note
//!     If TitleMid of ENTRY_STR is equal to CustomMsg, the buffer
//!     pointed to by pTitle of ENTRY_STR is used.  
//!     If TitleMid of ENTRY_STR is NOT equal to CustomMsg, then
//!     the text identified by the TitleMid identifier is used.  
//!     The above applies also to Line1Mid and pLine1 and Line2Mid 
//!     and pLine2.
//!
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


//-----------------------------------------------------------------------
//! \brief     Accept Continue or Cancel from user
//!
//! \param
//!     *pEntry     pointer to ENTRY_STR structure
//!                     TitleMid or pTitle - put in title bar    
//!                     Line1Mid or pLine1 - put in first line   
//!                     Line2Mid or PLine2 - put on second line  
//!
//! \return
//!      UBYTE      0 CANCEL_KY key was pressed
//!                 1 CONTINUE was entered
//!				    2 Timeout occurred	
//!
//! \note
//!     If TitleMid of ENTRY_STR is equal to CustomMsg, the buffer
//!     pointed to by pTitle of ENTRY_STR is used.  
//!     If TitleMid of ENTRY_STR is NOT equal to CustomMsg, then the text 
//!     identified by the TitleMid identifier is used.  
//!     The above applies also to Line1Mid and pLine1 and Line2Mid 
//!     and pLine2
//!
//-----------------------------------------------------------------------------
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
//!  \brief     Clears the Custom messages, CSTMSG and CSTMSG2
//!
//!  \param
//!     None.
//!
//!  \return
//!     None.
//!
//-----------------------------------------------------------------------------
extern void ClrCstMsgs( void )
{
	memset( ( UBYTE * ) CSTMSG, 0, sizeof( CSTMSG ) );
	memset( ( UBYTE * ) CSTMSG2, 0, sizeof( CSTMSG2 ) );
}


//=============================================================================
// Private function definitions
//=============================================================================
