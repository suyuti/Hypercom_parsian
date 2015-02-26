
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             cashier.c
//      Cashier processing functions
//
//=============================================================================

#include "string.h"
#include "stdlib.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkgui.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "sdkmem.h"
#include "utllib.h"
#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "util.h"
#include "prtdef.h"
#include "chkprinter.h"
#include "hstprint2.h"
#include "inputpwd.h"

#include "cashier.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static void Cashier_View( void );
static BYTE IsCashierOn( void );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static ENTRY_STR YNEntry;

static const ENTRY_STR CashierNumberEntry = {
	SignOnOff,					// Prompt
	CashierNum,					// Prompt
	N_NullStr,					// Not used
};

static const ENTRY_STR ServerNumberEntry = {
	SignOnOff,					// Prompt
	ServerNum,					// Prompt
	N_NullStr,					// Not used
};


static const ENTRY_STR TillNumberEntry = {
	SignOnOff,					// Prompt
	TillNum,					// Prompt
	N_NullStr,					// Not used
};




//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!  \brief
//!     Set the Checksum (LRC) for the Cashier Table.
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//!  \note
//!     Updates CASHLRC.
//!
//-----------------------------------------------------------------------------
extern void SetCashTabLRC( void )
{
	CASHLRC = CalcLrc( ( UBYTE * ) & CASHTAB, CASHIER_SZE * CASHIER_MAX );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&CASHTAB );
#endif
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Cashier Sign-On/Off
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_77( void )
{
	UBYTE status;
	UWORD movcnt;
	BYTE tmp;
	const ENTRY_STR *ptr;

	do
	{
		if ( TCONF.TCOPT1 & TC1_CASHIER )
		{
			// Select server or cashier message.
			// Select server or cashier message.
			if ( TCONF.TCOPT4 & TC4_SRV_PRM )
				ptr = &ServerNumberEntry;
			else
				ptr = &CashierNumberEntry;
			status = NumEntry( 1, 4, 1, ptr );
			do
			{
				if ( status )
				{
					memset( ( char * ) TRINP.TRCASHIER, ' ',4 );
					movcnt =
						MoveTillZero( ( char * ) TRINP.TRCASHIER, Dspbuf, 4 );
					break;
				}
			}
			while ( status );

			if ( !status )
				return;
		}
		else
		{
			ShowErrMsg( N_FuncNotSupported );
			return;
		}

		tmp = IsCashierOn(  );
		if ( -1 != tmp )
		{
			// Cashier is already on, see if log-off is desired.
			status = CashierStatus(  );
		}
		else
		{
			if ( TCONF.TCOPT1 & TC1_TILL )
			{
				do
				{
					// Prompt for till number
					status = NumEntry( 1, 4, 1, &TillNumberEntry );

					if ( status )
					{
						memset( ( char * ) TRINP.TRTILLNO, ' ',4 );
						movcnt =
							MoveTillZero( ( char * ) TRINP.TRTILLNO, Dspbuf,
										  4 );
						break;
					}
				}
				while ( status );

			}
			if ( status )
				status = CashierStatus(  );
		}

		if ( ( 1 == status ) || ( 3 == status ) )	// ON or OFF
		{
			ShowInfoMsg( TransAccepted, N_Response );

			SDK_BeepIt( 2 );
			SDK_Wait( ONESECOND * 2 );
		}

	}
	while ( status );

	return;
}



//-----------------------------------------------------------------------------
//!  \brief
//!     Cashier report
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_78( void )
{
	enum msg_id MsgId;
	UBYTE loopflag;

	if ( !( TCONF.TCOPT1 & TC1_CASHIER ) )
	{
		ShowErrMsg( N_FuncNotSupported );
		return;
	}

	// Prompt for Server ?
	if ( TCONF.TCOPT4 & TC4_SRV_PRM )
		MsgId = ReportMenuServer;
	else
		MsgId = ReportMenuCashier;

	if ( InputPswd( MsgId ) )
	{
		DrawMenu( MENU_CASHIER );

		do
		{
			loopflag = False;

			switch ( GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout ) )
			{
				case 'A':
					// If Printer is available, print Cashier Signon report
                    if ( ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
						Cashier_SignonRpt ();
					break;

				case 'B':
					Cashier_View(  );
					break;

				case 'C':
					if ( TCONF.TCOPT4 & TC4_SRV_PRM )
						YNEntry.TitleMid = N_ClearSerTbl;
					else
						YNEntry.TitleMid = N_ClearChsTbl;


					if ( TCONF.TCOPT4 & TC4_SRV_PRM )
						YNEntry.Line1Mid = N_ClearSerTbl;
					else
						YNEntry.Line1Mid = N_ClearChsTbl;

					YNEntry.Line2Mid = N_NullStr;

					if ( ENTER_KY == YesNoEntry( &YNEntry ) )
					{
						// Set Format Flag to 0x55 -- Formatted
                        InitTable( (UBYTE *)&CASHFMT,
                                   (UBYTE *)&CASHTAB,CASHIER_SZE * CASHIER_MAX,
                                   (UBYTE *)&CASHLRC );
                        SetCashTabLRC();

//FIXME?						if ( TCONF.TCOPT4 & TC4_SRV_PRM )
//							Parm2 = ReportMenuServer;
//						else
//							Parm2 = ReportMenuCashier;

						ShowInfoMsg( TransComplete, N_ClearChsTbl );
						SDK_BeepIt( 2 );

						// Delay for two seconds
						SDK_Wait( ONESECOND * 2 );

					}

					SDK_GuiClearScr(  );
					SDK_GuiDispAll(  );
					break;

				case 'D':		// Add/Remove
					Fun_77(  );
					break;

				case B_PREVIOUS:
					DrawMenu( MENU_PREVIOUS );
					loopflag = True;
					break;

				case CANCEL_KY:
				case CLEAR_KY:
				case 0:
					break;

				case B_NEXT:
					DrawMenu( MENU_NEXT );
					loopflag = True;
					break;

				case B_PREVIOUS_NEW_MENU:
					ButtonId = B_PREVIOUS_NEW_MENU;
					break;

				default:
					// bad key, error beep
					SDK_Beeper( TENMS * 40 );
					loopflag = True;
					break;
			}
		}
		while ( loopflag );
	}
	return;
}


//-----------------------------------------------------------------------------
//!  \brief
//!     Sign on/off a cashier according to its status
//!
//!  \param
//!     None
//!
//!  \return
//!     UBYTE       0 - Cancel key pressed or timed-out
//!                 1 - Everything OK (YES key pressed)
//!                 2 - User pressed NO key
//!                 3 - User signed off the cashier
//!
//-----------------------------------------------------------------------------
extern UBYTE CashierStatus( void )
{
	int idx;
	int first_free;
	UBYTE retval;
	UBYTE keyval;
	Bool loopflag;

	retval = 0;
	first_free = CASHIER_MAX + 1;

	for ( idx = 0; idx < CASHIER_MAX; idx++ )
	{
		// save the first available slot
		if ( ( CASHIER_MAX + 1 ) == first_free )
		{
			if ( !( CASHIER_USED & CASHTAB[idx].CASHIER_FLAG ) )
				first_free = idx;
		}

		if ( !memcmp( CASHTAB[idx].CASHIER, TRINP.TRCASHIER, S_TRCASHIER ) )
		{
			// Cashier is already in the table, get out of the loop
			retval = 1;
			break;
		}
	}

	if ( !retval && ( first_free >= CASHIER_MAX ) )	// No more slot
	{
		ShowErrMsg( ExceedMax );
		return ( 0 );
	}

	// Place the Sign On/Off message in the Custom buffer and get the lenght
	GetMsg( SignOnOff, CSTMSG );
	memset( ( UBYTE * ) CSTMSG2, 0, sizeof( CSTMSG2 ) );

	// Change the prompt if cashier is already on
	if ( ( CASHIER_USED & CASHTAB[idx].CASHIER_FLAG ) && retval )
	{
		// Get the Sign OFF message
		GetMsg( SignOffYN, CSTMSG3 );
	}
	else
	{
		// Get the Sign ON message
		GetMsg( SignOnYN, CSTMSG3 );
	}

	// Draw the two button Yes/No Screen
	DrawScreen( TWO_BTN_SCR );

	// Wait for a key press or timeout
	do
	{
		loopflag = False;

		keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

		switch ( keyval )
		{
			case ENTER_KY:
			case CANCEL_KY:
			case CLEAR_KY:
			case 0:
				break;

			default:
				// Bad key, error beep
				SDK_Beeper( TENMS * 40 );
				loopflag = True;
				break;
		}
	}
	while ( loopflag );
	// Only CANCEL_KY, ENTER_KY, CLEAR_KY, and timeout are allowed

	// check for cancel or time-out
// FIXME?
//@ASSA at 04.07.2005 	if ( ( CANCEL_KY == keyval ) || ( 0 == keyval ) )
//@ASSA at 04.07.2005 		chRetval = 0;

	if ( keyval != ENTER_KY )
	{
		// Cancel key pressed or timed-out
		retval = keyval;
	}
	else
	{
		if ( !retval )			// Not signed on yet
		{
			idx = first_free;
			memset( ( UBYTE * ) & CASHTAB[idx], 0, CASHIER_SZE );
			// add the cashier and sign on
			memcpy( CASHTAB[idx].CASHIER, TRINP.TRCASHIER, S_TRCASHIER );
			memcpy( CASHTAB[idx].TILLNO, TRINP.TRTILLNO, S_TRTILLNO );
		}

		if ( CASHIER_USED & CASHTAB[idx].CASHIER_FLAG )
		{
			// sign-off
			memset( ( UBYTE * ) & CASHTAB[idx], 0, CASHIER_SZE );

            // make shure that there is not empty entry in the middle of table
            if ( idx < CASHIER_MAX - 1 )
            {
                // shift the table to the left after new empty entry
                memcpy ( ( UBYTE * ) & CASHTAB[idx], ( UBYTE * ) & CASHTAB[idx+1], (CASHIER_MAX - 1 - idx )*CASHIER_SZE );

                // clear out last table entry
                memset( ( UBYTE * ) & CASHTAB[CASHIER_MAX-1], 0, CASHIER_SZE );
            }

			retval = 3;
		}
		else
		{
			// sign-on
			CASHTAB[idx].CASHIER_FLAG |= CASHIER_USED;
			retval = 1;
		}
		// Update the LRC of the table
        SetCashTabLRC();
	}
	return retval;
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Sign off all cashiers in the CASHTAB
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Signoff_All_Cashiers( void )
{
	memset( ( UBYTE * ) & CASHTAB, 0, CASHIER_SZE * CASHIER_MAX );

	// Update the LRC of the table
    SetCashTabLRC();
	return;
}

//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//!  \brief
//!     Review cashier signed-on list
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
static void Cashier_View( void )
{
	int idx;
	UBYTE keyval;
	UBYTE c_len, t_len;
	Bool signed_on;

	// Reset server/cashier signed-on flag
	signed_on = False;

	// Loop through Server/Cashier Table
	for ( idx = 0; idx < CASHIER_MAX; idx++ )
	{
		// Check if server/cashier is signed-on
		if ( CASHIER_USED & CASHTAB[idx].CASHIER_FLAG )
		{
			// Yes; set server/cashier signed-on flag
			signed_on = True;

			// Exit the loop
			break;
		}
	}

	// Check if any servers/cashiers are signed-on
	if ( !signed_on )
	{
		// No; check if Server option
		if ( TCONF.TCOPT4 & TC4_SRV_PRM )
		{
			// Yes; notify the user
			ShowErrMsg( Msg_SrvTabEmpty );
		}
		else
		{
			// No; notify the user
			ShowErrMsg( Msg_CshTabEmpty );
		}

		// Return to caller
		return;
	}

	// Get the prompt for Server Or Cashier
	if ( TCONF.TCOPT4 & TC4_SRV_PRM )
	{
		GetMsg( ServerOn, CSTMSG );
		GetMsg( ReportMenuServer, CSTMSG2 );
	}
	else
	{
		GetMsg( CashierOn, CSTMSG );
		GetMsg( ReportMenuCashier, CSTMSG2 );
	}

	// Clear the third Custom Message Buffer
	memset( ( UBYTE * ) CSTMSG3, 0, sizeof( CSTMSG3 ) );

	// Get the lenght of the Cashier/Server prompt
	c_len = StrLn( CSTMSG2, sizeof( CSTMSG2 ) );

	// Insert ':  '
	memcpy( &CSTMSG2[c_len], ":  ", 3 );
	c_len = c_len + 3;

	idx = 0;

	while ( True )
	{
		// Default to Next Arrow OFF and Previous Arrow OFF
		Enable_Flags[0] = 0;
		Enable_Flags[7] = 0;

		if ( CASHIER_USED & CASHTAB[idx].CASHIER_FLAG )
		{
			// Append the Cashier Number and NULL terminate it
			memcpy( ( UBYTE * ) & CSTMSG2[c_len], CASHTAB[idx].CASHIER,
					S_TRCASHIER );
			CSTMSG2[c_len + S_TRCASHIER] = 0;

			// If TILL is supported, display it
			if ( TCONF.TCOPT1 & TC1_TILL )
			{
				if ( !NullComp( ( char * ) CASHTAB[idx].TILLNO, S_TRTILLNO ) )
				{
					// Get the TILL prompt , and append ':  ' then the TILL Number
					GetMsg( ReportMenuTill, CSTMSG3 );
					t_len = StrLn( CSTMSG3, sizeof( CSTMSG3 ) );
					memcpy( &CSTMSG3[t_len], ":  ", 3 );
					t_len = t_len + 3;
					memcpy( &CSTMSG3[t_len], CASHTAB[idx].TILLNO,
							S_TRTILLNO );
					CSTMSG3[t_len + S_TRTILLNO] = 0;
				}
			}

			// Check if another Cashier/Server is available in forward 
			// direction
			
			if ( CASHIER_USED & CASHTAB[idx + 1].CASHIER_FLAG )
			{
				// Yes; turn Next Arrow ON
				Enable_Flags[0] = 1;
			}

			// Check if another Cashier/Server is available in backward 
			// direction

			if ( idx > 0 )
			{
				// Yes; turn Previous Arrow ON
				Enable_Flags[7] = 1;
			}
		}

		DrawScreen( SERVER_SCR );

		keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

		switch ( keyval )
		{
			case CANCEL_KY:	// User is done, or timed out
			case 0:
				return;

			case 'P':			// Previous Button
				// Decrement count if not zero
				if ( idx )
				{
					// Decrement count
					idx = idx - 1;
				}

				break;

			case 'N':			// Next or ENTER
			case ENTER_KY:
				// Increment count Unless at the end
				if ( idx < ( CASHIER_MAX - 1 ) )
				{
					idx++;
					if ( !( CASHIER_USED & CASHTAB[idx].CASHIER_FLAG ) )
					{
						// Gone too far, return count to end
						idx = idx - 1;
					}
				}

				break;

			default:
				SDK_Beeper( TENMS * 40 );

				break;
		}
	}
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Find the cashier id contains in DspBuf from CASHTAB table
//!
//!  \param
//!     None
//!
//!  \return
//!     BYTE        -1=Not found, or Index into CASHTAB
//!
//!  \note
//!     The Cashier Id to look for is in DspBuf
//!
//-----------------------------------------------------------------------------
BYTE IsCashierOn( void )
{
	BYTE retval;
	int idx;

	retval = -1;
	// Need to pad the cashier number with trailing spaces if the
	// length is less than S_TRCASHIER
	
	for ( idx = 0; idx < S_TRCASHIER; idx++ )
	{
		// See if character is a null
		if ( !Dspbuf[idx] )
		{
			// Yes; replace with a space
			Dspbuf[idx] = ' ';
		}
	}

	for ( idx = 0; idx < CASHIER_MAX; idx++ )
	{
		if ( !memcmp
			 ( CASHTAB[idx].CASHIER, ( UBYTE * ) Dspbuf, S_TRCASHIER ) )
		{
			// Cashier is already in the table, get out of the loop,
			// and save the index into the table for return
			retval = idx;
			break;
		}
	}

	return retval;
}
//-----------------------------------------------------------------------------
//!  \brief
//!     Get user input of cashier number
//!
//!  \param
//!     AllowNull   True  - allow NULL entry
//!                 False - do not allow NULL entry.
//!
//!  \return
//!     BYTE        0 - Valid cashier number was entered and index
//!                     into CASHTAB is being returned
//!                 -1 - NULL entry allowed and was entered
//!                 -2 - CANCEL key was pressed
//!                 -3 - Invalid cashier number was entered, retry.
//-----------------------------------------------------------------------------
BYTE GetCashier( Bool AllowNull )
{
	BYTE Index;
	static ENTRY_STR NEntry;
	UBYTE retval;
	
	enum msg_id msgID;

	NEntry.TitleMid = N_NullStr,	// Title
	NEntry.Line2Mid = N_NullStr;

	// Select server or cashier message.
	if ( TCONF.TCOPT4 & TC4_SRV_PRM )
	{
		msgID = ServerNum;
		NEntry.Line1Mid = msgID;
	}
	else
	{
		msgID = CashierNum;
		NEntry.Line1Mid = msgID;
	}

	CSTMSG[0] = 0;				// No 2nd line prompt or default value
	// See if NULL entry is allowed
	if ( AllowNull )
	{
		retval = NumEntry( 1, 4, 0, &NEntry );

		// Yes; get the Server/Cashier Number with zero minimum length
		if ( !retval )
		{
			// User cancel; notify the caller
			return ( -2 );
		}

		// See if NULL entry
		if ( !Dspbuf[0] )
		{
			// Notify the caller
			return ( -1 );
		}
	}
	else
	{
		retval = NumEntry( 1, 4, 1, &NEntry );

		// Get the Server/Cashier Number
		if ( !retval )
		{
			// User cancel; notify the caller
			return ( -2 );
		}
	}


	// See if Server/Cashier is logged on
	Index = IsCashierOn(  );

	if ( -1 == Index )
	{
		// No; invalid entry, notify the user
		ShowErrMsg( InvalidEntry );

		// Return value for re-entering cashier number
		return ( -3 );
	}
	// Return index into Cashier Table
	return ( Index );
}

//=============================================================================
// Private function definitions
//=============================================================================
