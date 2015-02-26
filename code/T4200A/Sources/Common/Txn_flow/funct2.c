
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             funct2.c
//      Function module 2
//
//=============================================================================

#include "string.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "sdkmem.h"
#include "fixdata.h"
#include "siodev.inc"

#include "utllib.h"
#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "util.h"
#include "tables.h"
#include "rs232tab.h"
#include "function.h"
#include "flaghost.h"
#include "respdsp.h"
#include "inputpwd.h"
#include "inpmpswd.h"
#include "inputspw.h"
#include "getdate.h"
#include "gettime.h"

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
static const ENTRY_STR DateEntry = {
	N_DateTimeSetup,			// Title
	DateMDY,					// Prompt
	CustomMsg,					// Default value
};

static const ENTRY_STR TimeEntry = {
	N_DateTimeSetup,			// Title
	TimeHM,						// Prompt
	CustomMsg,					// Default value
};

static ENTRY_STR YNEntry;


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//!  \brief
//!     Set DATE/TIME
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//-----------------------------------------------------------------------------
extern void Fun_10( void )
{
	char LoopFlag = True;
	static UBYTE NewValue[RTCLEN];

	// Get Supervisor Password with no Local Option check
	if ( !InputSPswd( N_DateTimeSetup, 0x00 ) )
		return;					// Exit if CANCEL_KY or TIMEOUT

	// Keep trying until date is saved, or cancel or timeout
	do
	{
		// assume no problem
		LoopFlag = False;

		// Get the date from the user, and check the value
		switch ( GetDate( &DateEntry, False ) )
		{
			case 0:			// User cancelled or timed out
				return;

			case 1:			// Data valid, Save the Date entered
				// Format the data
		        SDK_RtcRead( (UBYTE *)NewValue );
				memcpy( &NewValue[2], &Dspbuf[0], 4 );	// 2 digit month + 2 digit day
				NewValue[0] = Dspbuf[4];
				NewValue[1] = Dspbuf[5];

				// Write it to the real time clock chip
				if ( SDK_RtcWrite( NewValue, 6 ) != orvOK )
				{
					ShowErrMsg( InvalidEntry );
					LoopFlag = True;	// Try again
				}
				break;

			case 2:			// Just ENTER pressed, go get the time
				break;
		}

	}
	while ( LoopFlag );

	// Now get the time
	// Keep trying until time is saved, or cancel or timeout
	do
	{
		// assume no problem
		LoopFlag = False;

		// Prompt for time, and check the value
		switch ( GetTime( &TimeEntry ) )
		{
			case 0:			// User cancelled or timed out
				return;

			case 1:			// Data valid, Save the time entered
				// Format the data
				memcpy( &NewValue[0], &Dspbuf[5], 6 );	// Current date
				memcpy( &NewValue[6], &Dspbuf[0], 4 );	// HHMM

                // Set seconds to zero
				NewValue[10] = '0';
				NewValue[11] = '0';

				// Write it to real time clock chip
				if ( SDK_RtcWrite( NewValue, 12 ) != orvOK )
				{
					ShowErrMsg( InvalidEntry );
					LoopFlag = True;	// Try again
				}
				break;

			case 2:			// Just ENTER pressed, nothing to do
				break;
		}

	}
	while ( LoopFlag );

}

//-----------------------------------------------------------------------------
//!  \brief
//!     Program Load
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_92( void )
{
	UBYTE Status;

	// Get Maintenance Password

	GetMsg( MercFunction, DefaultTitle );
	Status = EnterMPswd(  );
	if ( Status )
	{
		// Okay; see if Maintenance Password disabled
		if ( 2 == Status )
		{
			// Yes; get Supervisor Password with no Local Option check
			if ( !InputSPswd( ProgramLoad, 0x00 ) )
			{
				// Bad; return to caller
				return;
			}
		}

		// Password okay; confirm the Program Load
		YNEntry.TitleMid = ProgramLoad;
		YNEntry.Line1Mid = ProgramLoad;
		YNEntry.Line2Mid = CorrectYesNo;

		// Check if YES was entered
		if ( YesNoEntry( &YNEntry ) == ENTER_KY )
		{
			// Check the RS232 table
			if ( (RSTAB.RSOPT & RSOPT_RSACTIVE) && (RSFMT == 0x55) && (RSLRC == CalcLrc((UBYTE*)&RSTAB, RSSZE)) )
			{
				NMSIDLL[0] = 'E';
				// Check the RS232 port baud rate
				switch (RSTAB.RSSPEED)
				{
					case   SIO_B9600:NMSIDLL[1] =   NMS_9600;break;
					case  SIO_B19200:NMSIDLL[1] =  NMS_19200;break;
					case  SIO_B38400:NMSIDLL[1] =  NMS_38400;break;
					case  SIO_B57600:NMSIDLL[1] =  NMS_57600;break;
					case SIO_B115200:NMSIDLL[1] = NMS_115200;break;
							
					default:NMSIDLL[1] =   NMS_9600;
				}
			}
			else
			{
				NMSIDLL[0] = 'I';
				NMSIDLL[1] = 'L';
			}

			// Yes; wait for 1/2 second
			SDK_Wait( TENMS * 50 );

			// Restart Terminal and request Program Load
			SDK_TerminalLoad(  );

		}
	}
}

//-----------------------------------------------------------------------------
//!  \brief
//!     Logon to get working key
//!
//!  \param
//!     None
//!
//!  \return
//!     None
//!
//-----------------------------------------------------------------------------
extern void Fun_93( void )
{
	UBYTE status, num_hosts;

	CurrentEvent.NEXTEVENT = 0;

	num_hosts = AqidCount(  );	// count the number of acquirers

	// we don't have any acquirer to settle, just return
	if ( num_hosts == 0 )
		return;

		// Set transaction key
		TRINP.TRKEY = LOGON;

		// if we have only one acquirer in acquirer table, don't need to pop up
		// display the host settle menu. Just set the proper data to TRINP
		if ( num_hosts == 1 )
		{
    		TRINP.TRAQID  = AQTAB[0].AQID;
	    	TRINP.TRAQPTR = &AQTAB[0];
			TRINP.TRBATPTR = (struct batch_rec *) FindBatTab(TRINP.TRAQID);
		}
		else
		{
			DrawMenu( MENU_HOST_LOGON );

			do					// Call the selected option processor
			{
				status = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

				if ( status == B_PREVIOUS )
				{
					DrawMenu( MENU_PREVIOUS );

				}
				// Go forward one menu level
				else if ( status == B_NEXT )
				{
					DrawMenu( MENU_NEXT );

				}
				else if ( ( CANCEL_KY == status ) ||
						  ( CLEAR_KY == status ) || ( 0 == status ) )
				{
					break;		// cancel or Time-Out
				}
				else if ( ( status >= 'A' ) && ( status <= 'A' + 7 ) )
				{
        	    	TRINP.TRAQID  = AQTAB[status - 'A'].AQID;
            		TRINP.TRAQPTR = &AQTAB[status - 'A'];
					TRINP.TRBATPTR = (struct batch_rec *) FindBatTab(TRINP.TRAQID);
					break;
				}
				else if ( status == B_PREVIOUS_NEW_MENU )
				{
					ButtonId = B_PREVIOUS_NEW_MENU;
					status = 0;
					break;
				}

			}
			while ( status && ( status != CANCEL_KY ) );

			if ( ( status == CANCEL_KY ) ||
				 ( CLEAR_KY == status ) || ( status == 0 ) )
			{
				if ( ButtonId != B_PREVIOUS_NEW_MENU )
					ButtonId = CANCEL_KY;
				return;
			}
		}

		// Get Merchant Password
		if ( !InputPswd( N_Logon ) ) return;

		// Set flags to not print the receipt header and not allow user cancel.
		FlagHostCntl = 0;

		// Flag the host process
		FlagHost ();

		// Display Response
		RespDisplay ();

		/* Check approved flag */
		if ( TRINP.TRRSPOPT & R_APPROVED )
		{
			/* Set 'Acquirer Enabled' */
			TRINP.TRBATPTR->BATLOGSTATUS = 0xFF;

			SetBATTabLRC();
		}
}

 //=============================================================================
// Private function definitions
//=============================================================================
