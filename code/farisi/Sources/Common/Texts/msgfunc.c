
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             msgfunc.c
//      Various message functions
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "util.h"
#include "icepak.h"
#include "tables.h"
#include "runtime.h"
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


//====================================================
//!===  RESPONSE MESSAGE TABLES                     ==
//====================================================
//! For the definition of resp_rec, please refer to struct.h 
const struct resp_rec ResponseTab[] = {
//             |RSPOPT                    |RSPOPT2
	{"00", "", R_APPROVED,                0 },
	{"01", "", R_REFERRAL,                0 },
	{"02", "", R_REFERRAL,                0 },
	{"03", "", 0,                         0 },
	{"04", "", 0,                         0 },
	{"05", "", 0,                         0 },
	{"08", "", R_APPROVED,                R_NOCAPT },
	{"12", "", 0,                         0 },
	{"13", "", 0,                         0 },
	{"14", "", 0,                         0 },
	{"19", "", 0,                         0 },
	{"25", "", 0,                         0 },
	{"30", "", 0,                         0 },
	{"31", "", 0,                         0 },
	{"41", "", 0,                         0 },
	{"43", "", 0,                         0 },
	{"51", "", 0,                         0 },
	{"54", "", 0,                         0 },
	{"55", "", R_BADPIN,                  0 },
	{"58", "", 0,                         0 },
    {"60", "", R_REFERRAL,                0 },
	{"76", "", 0,                         0 },
	{"77", "", 0,                         0 },
	{"78", "", 0,                         0 },
	{"79", "", 0,                         0 },
	{"80", "", 0,                         0 },
	{"82", "", 0,                         0 },
	{"83", "", 0,                         0 },
	{"85", "", 0,                         0 },
	{"89", "", 0,                         0 },
	{"91", "", R_NOCLRREV + R_NOCLRPEND,  0 },
	{"94", "", 0,                         0 },
	{"95", "", R_UPLOAD + R_NOCLRPEND,    0 },
	{"96", "", 0,                         0 },
	{"AA", "", 0,                         0 },
	{"aa", "", 0,                         0 },

	// Locally Generated Error Messages

	{ "NB", "", 0,                        0 },
	{ "BH", "", 0,                        0 },
    { "BD", "", R_APPROVED,               0 },
    { "BB", "", R_APPROVED,               0 },
    { "UN", "", 0,                        0 },
    { "LC", "", R_NOCLRREV+R_NOCLRPEND,   0 },
    { "CE", "", R_NOCLRREV+R_NOCLRPEND,   0 },
    { "NC", "", R_APPROVED,               0 },
    { "TO", "", R_NOCLRREV+R_NOCLRPEND,   0 },
    { "ND", "", R_NOCLRREV+R_NOCLRPEND,   0 },
    { "IM", "", R_NOCLRREV+R_NOCLRPEND,   0 },
    { "XC", "", R_NOCLRREV+R_NOCLRPEND,   0 },
    { "LB", "", R_NOCLRREV+R_NOCLRPEND,   0 },
    { "NA", "", R_NOCLRREV+R_NOCLRPEND,   0 },
    { "UC", "", 0,                        0 },
    { "NL", "", 0,                        0 },

    { "DC", "", R_NOCLRREV+R_NOCLRPEND , 0        },
    { "Y1", "", R_NOCLRREV+R_NOCLRPEND , 0        },
    { "Y3", "", R_NOCLRREV+R_NOCLRPEND , 0        },
    { "Z1", "", R_NOCLRREV+R_NOCLRPEND , 0        },
    { "Z3", "", R_NOCLRREV+R_NOCLRPEND , 0        },

	{ "**", "", 0,                        0 }
};



//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Returns the number of entries defined in ResponseTab
//!
//! \param
//!		
//!	\Global Inputs
//!		ResponseTab
//!
//! \return
//!     None
//!
//! \note
//!
extern void ResponseTabSize( void )
{
	TableItemCount =
		( UWORD ) ( sizeof( ResponseTab ) / sizeof( struct resp_rec ) );
}

//-----------------------------------------------------------------------------
//!
//!      Retrieve a message and store it in MLBuffer
//!
//! \param
//!		MSGID			id of message
//!		
//! \return
//!     None
//!
//! \note
//!		On entry:          
//!			MSGID = id of message
//!		On exit:
//!			MLBuffer contains message text
//!
extern void GETMSG( enum msg_id MSGID )
{
	enum msg_id tempmsg;
	UBYTE *ptr = 0;
	UBYTE i, MaxLen;
	struct MLGetMsg_str MLGetMessage;

	memset( ( UBYTE * ) MLBuffer, 0,sizeof( MLBuffer ) );

	tempmsg = MSGID;

	// process user messages here
	if ( MSG_USER0 <= MSGID && MSGID < MSG_USER0 + NUM_OF_USER_MESSAGES )
	{
		tempmsg = User_Msgs[MSGID - MSG_USER0];

		// Note - user messages (MSG_USER0 -> MSG_USERn ) can be redirected
		// to another special message - but not to another user message

		if ( MSG_USER0 <= tempmsg &&
			 tempmsg < MSG_USER0 + NUM_OF_USER_MESSAGES )
		{
			// illegal user message - can't be set to another user message
			RunTimeError ( 0x0004 );
		}

	}

	// process special messages other than user messages here
	if ( tempmsg >= MSG_SPECIAL || tempmsg <= CustomMsg8 )
	{
		switch ( tempmsg )
		{

			case MSG_ICEBUTTON1_L1:
				ptr = IBTAB.IBBTN1LABEL1;
				break;
			case MSG_ICEBUTTON1_L2:
				ptr = IBTAB.IBBTN1LABEL2;
				break;
			case MSG_ICEBUTTON2_L1:
				ptr = IBTAB.IBBTN2LABEL1;
				break;
			case MSG_ICEBUTTON2_L2:
				ptr = IBTAB.IBBTN2LABEL2;
				break;
			case MSG_ICEBUTTON3_L1:
				ptr = IBTAB.IBBTN3LABEL1;
				break;
			case MSG_ICEBUTTON3_L2:
				ptr = IBTAB.IBBTN3LABEL2;
				break;
			case MSG_ICEBUTTON4_L1:
				ptr = IBTAB.IBBTN4LABEL1;
				break;
			case MSG_ICEBUTTON4_L2:
				ptr = IBTAB.IBBTN4LABEL2;
				break;
			case MSG_DEFAULT_TITLE:
				if ( Titlebuf[0] )
					memcpy( MLBuffer, Titlebuf, sizeof( Titlebuf ) );
				else if ( DefaultTitle[0] )
					memcpy( MLBuffer, DefaultTitle, sizeof( DefaultTitle ) );
				else
					memcpy( MLBuffer, CurrentEvent.TRNTEXT,
							sizeof( CurrentEvent.TRNTEXT ) );
				break;
			case CustomMsg:
				memcpy( MLBuffer, CSTMSG, sizeof( CSTMSG ) );
				break;
			case CustomMsg2:
				memcpy( MLBuffer, CSTMSG2, sizeof( CSTMSG2 ) );
				break;
			case CustomMsg3:
				memcpy( MLBuffer, CSTMSG3, sizeof( CSTMSG3 ) );
				break;
			case CustomMsg4:
				memcpy( MLBuffer, CSTMSG4, sizeof( CSTMSG4 ) );
				break;
			case CustomMsg5:
				memcpy( MLBuffer, CSTMSG5, sizeof( CSTMSG5 ) );
				break;
			case CustomMsg6:
				memcpy( MLBuffer, CSTMSG6, sizeof( CSTMSG6 ) );
				break;
			case CustomMsg7:
				memcpy( MLBuffer, CSTMSG7, sizeof( CSTMSG7 ) );
				break;
			case CustomMsg8:
				memcpy( MLBuffer, CSTMSG8, sizeof( CSTMSG8 ) );
				break;
			case N_Ctxt1:
			case N_Ctxt2:
			case N_Ctxt3:
			case N_Ctxt4:
			case N_Ctxt5:
			case N_Ctxt6:
			case N_Ctxt7:
			case N_Ctxt8:
				// these pointers to custom messages are only used
				// to point to acquirer names (AQTAB[n].NAME), 
				// and therefore the data must be moved into another 
				// buffer and formatted. The data really
				// should be formatted by the user of N_Ctxt1..8, but
				// this would require allocatting buffers in global
				// memory for each one.

				MoveItAq( MLBuffer, CtxtPtr[tempmsg - N_Ctxt1], ( UWORD ) 10 );
				MLBuffer[10] = 0;	// Limit to 10 characters
				// remove trailing and leading spaces
				for ( i = 0; i <= 9; i++ )
				{
					if ( MLBuffer[9 - i] == ' ' )
						MLBuffer[9 - i] = 0;
					else
						break;
				}
				// skip leading spaces
				for ( i = 0; i < 9; i++ )
				{
					if ( MLBuffer[i] != ' ' )
						break;
				}
				// shift buffer to cover leading spaces
				if ( i )
					memcpy( MLBuffer, &MLBuffer[i],
							( UWORD ) ( 10 - i + 1 ) );
				break;
			default:
				break;

		}

		// format ICE button data
		if ( ptr )
		{
            // Check if this is image definition in 1-st line
            // (the string starts by "0")
            if ( '0' == *ptr)
            {
                // Yes, image definition - skip it
                MaxLen = S_BTNLBLLEN;
                ptr += S_BTNLBLLEN;
            }
            else
            {
                // No, assume actual text
                MaxLen = 15; // 15 chars per ICE button are possible!
            }

            memcpy( MLBuffer, ptr, MaxLen );
			ptr = ( UBYTE * ) MLBuffer;
			for ( i = ( MaxLen - 1 ); i; i-- )
			{
				if ( ptr[i] != ' ' )
					break;
			}
			ptr[i + 1] = 0;		// NULL terminate string
		}

		// make sure MLBuffer is '\0' terminated
		MLBuffer[sizeof( MLBuffer ) - 1] = '\0';
		return;					// done at this point
	}							// end of special message processing

	MLGetMessage.tblid = hdttext_table_id;
	MLGetMessage.msgid = tempmsg;

	TEXTGETSTRING ( &MLGetMessage );
}

//-----------------------------------------------------------------------------
//!
//!      Get Response Text and Options for this RSPC code
//!
//! \param
//!		
//! \return
//!     None
//!
//! \note
//!		Data Passed via RSPCODE, RSPTEXT, etc.
//!
extern void GetRspTxtOpts( void )
{
	UBYTE i;

	// Clear the Translated Text
	memset( ( UBYTE * ) ResponseText, 0,sizeof( ResponseText ) );

	// Initialize the table index
	i = 0;

	// Search till end of table has been reached
	while ( ResponseTab[i].RSPCDE[0] != '*' )
	{
		// If table string = requested string...
		if ( memcmp( ResponseTab[i].RSPCDE, RSPCODE, ( UWORD ) 2 ) == 0 )
		{
			// Yes; end search
			break;
		}

		// Increment index
		i++;
	}

	// Move Data From Table

	// Control Bits (Byte 1)
	RSPOPT = ResponseTab[i].RSPOPT;

	// Control Bits (Byte 2)
	RSPOPT2 = ResponseTab[i].RSPOPT2;

	// Translated Message
	TextGetString( Dspbuf, rsptext_table_id, i );
	memcpy( ResponseText, Dspbuf, S_RSPTXT );

	// Translated Message
}

//-----------------------------------------------------------------------------
//!
//!      Display an error message and wait for 2 seconds
//!
//! \param
//!		Mid			message ID
//!		
//! \return
//!     None
//!
//! \note
//!
extern void ShowErrMsg( enum msg_id Mid )
{
	UBYTE retval;

	retval = 1;

	// Get the Error data to display
	GetMsg( Mid, CSTMSG );

	// Turn ON OK Button
	Enable_Flags[3] = 1;

	// Remove buttons from button table
	SDK_InvalidateButtons(  );

	// Remove virtual windows
	SDK_GuiDefWindow(  );

	// Draw box screen with Error Info
	DrawScreen( ERROR_SCR );

	SDK_Beeper( TENMS * 40 );

	// Error message has been displayed. Wait for user
	// press ENTER, CLEAR or OK
	do
	{
		// Get Keyboard Entry
		retval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );
	}

    // Check for ENTER, CANCEL or CLEAR pressed
    while ( retval && ( CLEAR_KY != retval ) && ( ENTER_KY != retval ) && ( CANCEL_KY != retval ) );

}

//-----------------------------------------------------------------------------
//!
//!      Display an informational message
//!
//! \param
//!		Mid			Informational Message id
//!	\param
//!		Tid			Title Message
//!		
//! \return
//!     None
//!
//! \note
//!
extern void ShowInfoMsg( enum msg_id Mid, enum msg_id Tid )
{
	// Get the title and data to display
	GetMsg( Tid, CSTMSG4 );

	// If CSTMSG2 set,  CUSTMSG and CUSTMSG2 data
	if ( CustomMsg2 == Mid )
	{
		// Turn ON flag, so it will display data in CUSTMSG and CUSTMSG2
		Enable_Flags[3] = 0;
		Enable_Flags[4] = 1;
	}
	else
	{

		// Set for normal data display
		Enable_Flags[3] = 1;
		Enable_Flags[4] = 0;
		GetMsg( Mid, CSTMSG5 );
	}

	// Draw box screen with Info
	DrawScreen( INFO_SCR );
}

//-----------------------------------------------------------------------------
//!
//!      Display just an informational message or IcePak image  + informational message
//!
//! \param
//!		Mid	   		 	Informational Message id
//!	\param
//!		Tid				Title Message
//!	\param
//!		image_index		index of Icepak image to display
//!		
//! \return
//!     None
//!
//! \note
//!
extern void ShowIcePak(  enum msg_id Mid, enum msg_id Tid, int image_index )
{

	FILE *handle;
	char *pFName;
	BTN_Entry Button;


	// if ICEPAK EOT screen is available, show it in place of normal response messages
	pFName = IcePak[image_index].pFileName;
	if ( ( handle = fopen( pFName, "r" ) ) != NULL )
	{
		SDK_GuiClearScr(  );	// clear screen buffer
		// set button for size of screen to detect touch
		Button.LeftX = 0;
		Button.TopY = 0;
		Button.BTNState = BUTTON_UP;
		Button.BTNType = BUTTON_NOBORDER;
		Button.BTNWidth = FW_W;
		Button.BTNHeight = FW_H;
		Button.ContentType = CT_CUSTOM;
		Button.EventID = CANCEL_KY;
		Button.FontID = FONT_SA06BN08;
		Button.ExFontID = FONT_EA06BN08;

		SDK_Button( &Button );

		SDK_GuiFileImage( 0, 0, handle );	// show IcePak image for response message
		fclose( handle );

		Button.BTNHeight = B1_H / 2;	// bottom of screen for text
		Button.TopY = FW_H - Button.BTNHeight;
		Button.ContentType = CT_TEXT;	// assume one line of text

		// Show informational text at bottom of screen
		if ( ( CustomMsg == Mid ) || ( CustomMsg2 == Mid ) )
		{
			// Message from CSTMSG
			Button.pLabel1 = ( UBYTE * ) CSTMSG;

			// See if 2 line Informational message
			if ( CustomMsg2 == Mid )
			{
				// Yes; set up 2 line button
				Button.ContentType = CT_TEXT_2LINE;
				Button.pLabel2 = ( UBYTE * ) CSTMSG2;
				Button.BTNHeight = B1_H;	// double height for 2 lines
				Button.TopY = FW_H - Button.BTNHeight;
			}
		}
		else
		{
			// Message from display buffer
			GetMsg( Mid, Dspbuf );
			Button.pLabel1 = ( UBYTE * ) Dspbuf;
		}

		// use button functionality to display text
		SDK_Button( &Button );

		SDK_GuiDispAll(  );
	}
	else
	{
		Enable_Flags[SHOWAD_FLG] = 1;	//Enable advertisement
		ShowInfoMsg( Mid, Tid );
		Enable_Flags[SHOWAD_FLG] = 0;	//Disable advertisement
	}

	Tid = 0;					// may be used by ShowErrMsg, so clear
}

//=============================================================================
// Private function definitions
//=============================================================================
