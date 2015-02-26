//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             respdsp.c
//      Display Response Routines.
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "pinpad.h"
#include "util.h"
#include "disproc.h"
#include "respdsp.h"
#include "cstmamnt.h"
#include "tables.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================

static void RespDisplayHostText2( void );
static void RespDisplayHostText1( void );
static void RespDisplayStatus( void );
static void RespDisplayText( void );
static void RespBeeps( void );



//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Displays Response
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void RespDisplay( void )
{
	// See if any host text 
	if ( TRINP.TRHOSTTEXT[0] )
	{
		// Yes; see if second line 
		if ( TRINP.TRHOSTTEXT[20] )
		{
			// Yes; display 2 lines of custom data 
			RespDisplayHostText2(  );
		}
		else
		{
			// No; display transaction on top, host on bottom 
			RespDisplayHostText1(  );
		}
	}
	else
	{
		// No; see if approved 
		if ( TRINP.TRRSPOPT & R_APPROVED )
		{
			// Yes; see if refund 
			if ( ( TRINP.TRKEY == REFDEBIT ) || ( TRINP.TRKEY == REFUND ) )
			{
				// Yes; see if offline 
				if ( ( TCONF.TCOPT2 & TC2_REF_OFF ) ||
					 ( (TRINP.TRISPTR->ISOPT3) & IS3_REF_OFF ) )
				{
					// Yes; display transaction accepted line 
					ShowEOTMsg( TransAccepted, N_Response );
				}
				else
				{
					// No; display transaction status 
					RespDisplayStatus(  );
				}
			}
			else
			{
				// No; display transaction status 
				RespDisplayStatus(  );
			}
		}
		else
		{
			// No; display transaction text 
			RespDisplayText(  );
		}
	}

	// Beep for a Response 
	RespBeeps(  );

	// Set flag to leave response on display.  
	DelayIdleDisplay = True;

	// Display response on PIN PAD 
	if ( TRINP.TRPINBL[0] != 0 )	// Was PIN entered? 
	{
		if ( TRINP.TRRSPOPT & R_APPROVED )
		{
			PinMessage ( Msg_Approved );
		}
		else if ( TRINP.TRRSPOPT & R_BADPIN )
		{
			// INCORRECT PIN 
			PinMessage ( Msg_IncrtPin );
		}
		else if ( TRINP.TRRSPC[0] >= '0' && TRINP.TRRSPC[0] <= '9' )	// Numeric response? 
		{
			PinMessage ( Msg_Declined );
		}
		else
		{
			PinMessage ( Msg_TryAgain );
		}
	}
	else if ( PINSTAT & PinCard_Read )	// Card swipe on PIN PAD? 
	{
		// Should we display response on PIN PAD for this transaction? 
		if ( !( CurrentEvent.TRNOPT4 & T4_NOPINDSP ) )
		{
			if ( TRINP.TRRSPOPT & R_APPROVED )
			{
				PinMessage ( Msg_Approved );
			}
			else if ( TRINP.TRRSPC[0] >= '0' && TRINP.TRRSPC[0] <= '9' )	//  Numeric response? 
			{
				PinMessage ( Msg_Declined );
			}
			else
			{
				PinMessage ( Msg_TryAgain );
			}
		}
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Displays Response to Balance Inquiry
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void BalanceRespDisplay( void )
{
	// See if any host text 
	if ( TRINP.TRHOSTTEXT[0] )
	{
		// Yes; see if second line 
		if ( TRINP.TRHOSTTEXT[20] )
		{
			// Yes; display 2 lines of custom data 
			RespDisplayHostText2(  );
		}
		else
		{
			// No; display transaction on top, host on bottom 
			RespDisplayHostText1(  );
		}
	}
	else
	{							// No host text 
		// see if approved 
		if ( TRINP.TRRSPOPT & R_APPROVED )
		{
			if ( TRINP.TRTIP[0] != 0xFF )
			{
				// Move the today amount into CSTMSG 
				CustomAmount( Today, TRINP.TRTIP );
			}
			else
			{
				// Don't display Today without an amount. 
				GetMsg( BlankMsg, CSTMSG );
			}

			memcpy( CSTMSG2, CSTMSG, sizeof( CSTMSG ) );

			// Move the Base Amount into the global custom message 
			CustomAmount( AvailBal, TRINP.TRTOTAM );

			// Display "AVAIL BAL. $TRTOTAM" on line 1 
			// Display "     TODAY   $TRTIP" on line 2 
			ShowEOTMsg( CustomMsg2, N_Response );
		}
		else
		{
			// No; display transaction text 
			RespDisplayText(  );
		}
	}							// No host text 

	// Beep for a Response 
	RespBeeps(  );

	// Set flag to leave response on display.  
	DelayIdleDisplay = True;
}


//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     Displays Host Text - 2 lines
//!
//! \return
//!     None
//!
//! \note
//!     As global inputs TRINP.TRHOSTTEXT.
//!     Display first 20 characters in first line, second group of 20
//!     characters in second line.
//-----------------------------------------------------------------------------
static void RespDisplayHostText2( void )
{
	// Transfer first line of host response text to custom message 
	memcpy( CSTMSG, TRINP.TRHOSTTEXT, 20 );

	// Null terminate the custom message 
	CSTMSG[20] = '\0';

	// Transfer second line of host response text to custom message 
	memcpy( CSTMSG2, &TRINP.TRHOSTTEXT[20], 20 );

	// Null terminate the custom message line 2 
	CSTMSG2[20] = '\0';

	// Display host response text 
	ShowEOTMsg( CustomMsg2, N_Response );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Displays Host Text - 1 line
//!
//! \return
//!     None
//!
//! \note
//!     As global inputs TRINP.TRHOSTTEXT.
//-----------------------------------------------------------------------------
static void RespDisplayHostText1( void )
{
	// Calculate maximum number of characters to display on a line,
	// in screen_limit 
	MaxCharDisp ();

	// Transfer host response text to custom message 
	memcpy( CSTMSG, TRINP.TRHOSTTEXT, ( UWORD ) screen_limit );

	// Null terminate the custom message 
	CSTMSG[screen_limit] = '\0';

	// Display host response text 
	ShowEOTMsg( CustomMsg, N_Response );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Displays Transaction Status
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void RespDisplayStatus( void )
{
	// See if transaction complete only 
	if ( CurrentEvent.TRNOPT2 & T2_COMPLETE )
	{
		// Yes; display transaction complete 
		ShowEOTMsg( TransComplete, N_Response );
	}
	else
	{
		// No; see if accepted message required 
		if ( CurrentEvent.TRNOPT3 & T3_ACCEPTED )
		{
			// Yes; display transaction accepted 
			ShowEOTMsg( TransAccepted, N_Response );
		}
		else
		{
			// No; display text 
			RespDisplayText(  );
		}
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Displays Host Response Text
//!
//! \return
//!     None
//!
//! \note
//!     As global inputs TRINP.TRRSPTXT.
//-----------------------------------------------------------------------------
static void RespDisplayText( void )
{
	// Transfer host response text to custom message 
	memcpy( CSTMSG, TRINP.TRRSPTXT, S_TRRSPTXT );

	// Null terminate the custom message 
	CSTMSG[S_TRRSPTXT] = '\0';

	// Display host response text 
	ShowEOTMsg( CustomMsg, N_Response );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Beeps for a Response
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void RespBeeps( void )
{
	// Added this reset   Without it the 
	// beep it was causing a previous message to be displayed. 
	SDK_BeeperReset(  );

	// See if approved 
	if ( TRINP.TRRSPOPT & R_APPROVED )
	{
		// Yes; beep twice 
		SDK_BeepIt( 2 );
	}
	else
	{
		// No; do error beep 
		SDK_Beeper( TENMS * 40 );
	}
}

