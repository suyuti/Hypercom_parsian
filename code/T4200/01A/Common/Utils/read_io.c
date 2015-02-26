//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             read_io.c
//      read data from input devices
//
//=============================================================================
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkmem.h"
#include "sdkio.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "fixdata.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"

#include "defines.h"
#include "menu.h"
#include "msg.h"
#include "util.h"
#include "pinpad.h"
#include "getkey.h"
#include "osclib.h"
#include "ddtm.h"
#include "getkey.h"

#include "read_io.h"
#include "DbgTrace.h"

//=============================================================================
// External variables / declarations
//=============================================================================
#define EVT_BUTTON_UP       UP_KY
#define EVT_BUTTON_DOWN     DOWN_KY
#define EVT_BUTTON_SELECT   SELECT_KY

//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static UWORD open_io( UWORD );
static void close_io( UWORD );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static UBYTE InputType;
static UWORD PadBtnId;


//#ifdef MAKE_VIKING
RECT menuRect;

//  from menu.c
#define NEXT_ENABLE_FLAG			0
#define PREV_ENABLE_FLAG			11
//#endif

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Performs input from various devices
//!
//!	\param
//!		[in]DevMap      contains bit flags telling which device(s) to read.
//!	\param
//!		[in]tmout       timeout (in seconds).
//!	\param
//!		[out]pbPinStatus pointer to the PinPad reader boolean status.
//!	\param
//!		[in,out]pDevFlag pointer of location that is set to the devices left open.
//!
//! \return
//!     The device which was read.
//!
//! \note
//!     
//-----------------------------------------------------------------------------
extern UBYTE read_io( UWORD DevMap, OS_TIMES tmout, Bool *pbPinStatus, UWORD *pDevFlag )
{
    UBYTE timerID;
    UBYTE timed_out;
	UWORD MenuX;
	UWORD MenuY;
	UBYTE MenuItemCounterOld;
    UBYTE Max;
    Bool  bHandledScr;
	Bool circle = True;

    UWORD   BtnId;
    PADDATA PadDat;
	PADDATA PadDatLast;

#ifdef MAKE_EMV
	UBYTE Card_Status;
#endif

    //  mkulakov
    // It is done to border the scope  of the function.
    //! The function redraw pushed button and exits from waiting loop
    void ButtonUpIsHappened()
    {
        if( BtnId )
        {
            ButtonId= BtnId;
            SDK_RedrawButton( (UWORD)PadDat.X_Pos,(UWORD)PadDat.Y_Pos,BUTTON_DOWN );
            // "pen up" and "pen down" happened in the same button
            // This is a legal button press
        }
        SDK_Beeper( 12 * TENMS ); // Only beep if button gets hit
        SDK_RedrawButton( (UWORD)PadDatLast.X_Pos,(UWORD)PadDatLast.Y_Pos,BUTTON_UP );
        InputType = DEV_PAD;
        /* button has been pressed and released, so exit */
        circle = False; 
    }

	*pbPinStatus = 0;
	InputType = 0;

	if ( !DevMap )			// Request close all
	{
        close_io( 0 );
		*pDevFlag = 0;			// Reset device open status flags
		return InputType;
	}

	// If the requested devices are the same as the last time, then
	// there is no need to close and reopen them.
	if ( *pDevFlag != DevMap )
	{
		if ( *pDevFlag )			// First, close all devices except keyboard
		{
            close_io( *pDevFlag );
		}

        // Open requested devices
        open_io( DevMap );

		// Set DevFlag to the devices currently open
		*pDevFlag = DevMap;
	}
	InputType = KeyVal = 0;
	ButtonId = BtnId = 0;       // Reset button id

	// Set timer
	if ( tmout != 0 )
	{
		// timed_out will be set to zero by the wait and flag call.
		timerID = SDK_WaitAndFlag( (OS_TIMES)(ONESECOND * tmout),(UBYTE *)&timed_out );
	}
	else
	{
		timed_out = 1;
	}
    bHandledScr= ( SDLScreenId == MENU_5BTNTEXT_SCR ) ||
			 ( SDLScreenId == MENU_10BTNTEXT_SCR ) ||
			 ( SDLScreenId == IDLE_SCR ) ||
             ( SDLScreenId == INIT_SCR ) ? True : False;

	// Input loop
	while ( circle )
	{
#ifdef MAKE_M2
        // CHECK FOR PAD EVENT
        if ( DevMap & DEV_PAD )
        {
          if ( PadStatus.CompleteFlag )
          {
              DBGTRACE (LVL_WARNING, TYPE_SPOS32, "<<< Pad Event >>>\n" );
              // If there was an error, break out of the input loop
              if ( orvDEVICEBUSY == PadStatus.Error )
              {
                  *pDevFlag = DEV_ABORT;  // Flag timed out
                  break;
              }

              // Process the pad data
              SDK_GetPad( &PadDat );
              // Set ButtonId to the button pressed first.  The pen must be
              // on the same button when lifted for the action to be done.
              BtnId = SDK_GetButtonEvent( (UWORD)PadDat.X_Pos,(UWORD)PadDat.Y_Pos );
              SDK_SetPad( 0, &PadStatus );  // Read request
              DBGTRACE (LVL_WARNING, TYPE_SPOS32, "<- Pad ReOpen ->\n" );
              if ( PAD_PEN_DOWN == PadDat.PenState )  // Pen is down?
              {
                  DBGTRACE (LVL_WARNING, TYPE_SPOS32, "Pad DOWN: Last= 0x%.4X New= 0x%.4X\n", PadBtnId,BtnId );
                  if( DEV_TOUCH == (DevMap & DEV_TOUCH) )
                  {
                      // an application looking just for touch pad pressing
                      InputType = DEV_PAD;
                      KeyVal = 0;
                      ButtonId = PadBtnId = BtnId = 0;
                      break;
                  }
                  // If we have recorded a "pen down" event and moving to other item
                  if ( PadBtnId && PadBtnId != BtnId )
                  {
                      DBGTRACE (LVL_WARNING, TYPE_SPOS32, "Pad MOVE: Last= 0x%.4X New= 0x%.4X\n", PadBtnId,BtnId );
                      SDK_RedrawButton( (UWORD)PadDatLast.X_Pos,(UWORD)PadDatLast.Y_Pos,BUTTON_UP );
                      PadBtnId = 0;
                  }

                  // If we have not yet recorded a "pen down" event
                  // (pen was not down before)
                  if ( BtnId && 0 == PadBtnId )
                  {
                      PadBtnId = BtnId;
                      // Was this a "pen down" in a button area?
                      if ( PadBtnId )
                      {
                          // Save button's coordinates
                          MoveIt( (UBYTE*)&PadDatLast,(UBYTE*)&PadDat,sizeof(PadDat) );
                      }
                  }
                  continue;
              }
              else if ( PAD_PEN_UP == PadDat.PenState ) // Pen is up
              {
                  DBGTRACE (LVL_WARNING, TYPE_SPOS32, "Pad UP: Last= 0x%.4X New= 0x%.4X\n", PadBtnId,BtnId );
                  if ( PadBtnId && ( PadBtnId == BtnId ) )
                  {
                      ButtonUpIsHappened();
					  continue;
                  }
                  else
                  {
                      // Not a legal button press
                      // If button was drawn down, draw it up
//                      if ( PadBtnId )
                      {
                          SDK_RedrawButton( (UWORD)PadDatLast.X_Pos,(UWORD)PadDatLast.Y_Pos,BUTTON_UP );
                      }

                      // Reset button and temp button ids
                      ButtonId = PadBtnId = BtnId = 0;
                  }
              }
          }
		}
#endif // MAKE_M2

        // CHECK FOR KEYPRESS
		if ( KeyStatus.CompleteFlag )
		{
			KeyVal = KeyBuffer[0];
            // reset the read and exit
			SDK_ReadKey( KeyBuffer, &KeyStatus );

            // switch to ON display backlight
			SDK_DisplayBackLight( 1 );
			backlight_timer = BACKLIGHT_DELAY;
		}
		else
        {
            KeyVal = 0;
        }

// Menu Processing
		if ( bHandledScr == True )
		{//-----Keep the choosen button down-------

			// Screen Coords
            if ( SDLScreenId == MENU_10BTNTEXT_SCR )
            {
                if ( MenuItemCounter < 5 )
                {
                    MenuX = B10_M_LEFT;
                    MenuY = Bv_B1_Y + 2 + MenuItemCounter*Bv_H;
                }
                else
                {
                    MenuX = B10_M_RIGHT;
                    MenuY = Bv_B1_Y + 2 + (MenuItemCounter - 5)*Bv_H;
                }
            }
            else
            {
                MenuX = B10_M_LEFT;
                MenuY = Bv_B1_Y + 2 + MenuItemCounter*Bv_H;
            }

			// button area?
			if ( SDK_GetButtonEvent( (UWORD) MenuX, (UWORD) MenuY ) )
			{
#ifndef MAKE_M2
				SDK_RedrawButton( (UWORD) MenuX, (UWORD) MenuY, BUTTON_DOWN );
#endif
			}

            // Do process numeric key pressed in menu
            // Do not process it on IDLE screen - numerics are used for PAN entry 
            if ( ( KeyVal >= '0' ) && ( KeyVal <= '9' ) && ( SDLScreenId != IDLE_SCR ) )
            {
                // convert to 1..10
                if ( KeyVal == '0' )
                    KeyVal = 10;
                else
                    KeyVal -= '0';
              
                // Validate it
                if ( KeyVal <= ButtonCnt )
                {
                    // Valid, apply it
                    MenuItemCounter = KeyVal - 1;
                    KeyVal = ENTER_KY;
                }
                else
                {
                    // Invalid, discard it
                    KeyVal = 0;
                }
            }
        }

//--------------------------------------------
        if ( (KeyVal == EVT_BUTTON_UP) || ( KeyVal == RIGHT_UP_KY ) ||
             (KeyVal == EVT_BUTTON_DOWN) || ( KeyVal == RIGHT_DOWN_KY ) ||
             (KeyVal == EVT_BUTTON_SELECT) || ( KeyVal == LEFT_MIDDLE_KY ) ||
             (KeyVal == ENTER_KY) )
  		{
//------------ Main Menu Processing ---------
			if ( bHandledScr == True )

			{//Simulate up,down,select- MENU system

				if ( timed_out )
				{
					// Need to wait for pen up - timed_out will be set
					// to zero by the wait and flag call.
					timerID = SDK_WaitAndFlag( ONESECOND, &timed_out );
				}

                MenuItemCounterOld = MenuItemCounter;
				if ((KeyVal == EVT_BUTTON_DOWN) || ( KeyVal == RIGHT_DOWN_KY ) ) //Down) //Down
				{
                    Max= SDLScreenId == MENU_10BTNTEXT_SCR ? 9 : 4;
                    if ( MenuItemCounter < Max )
                    {
                        MenuItemCounter++;
                    }
                    else
                    {//we should go to the next screen if it exists
                        if (Enable_Flags[NEXT_ENABLE_FLAG] == 1)
                        {
                            ButtonId = B_NEXT;
                            InputType = DEV_PAD;
                            break;
                        }
                    }
				}
                else if ((KeyVal == EVT_BUTTON_UP) || ( KeyVal == RIGHT_UP_KY )) 
				{
					if ( MenuItemCounter > 0 )
					{
						MenuItemCounter --;
					}
					else
					{//we should go to the prev screen if it exists
						if (Enable_Flags[PREV_ENABLE_FLAG] == 1)
						{
							//simulate pressing "back" key
							ButtonId = B_PREVIOUS_NEW_MENU;
							InputType = DEV_PAD;
							break;
						}
					}
				}

				if ( MenuItemCounterOld != MenuItemCounter )
				{//Pointer is moved, so redraw the button back

                    if ( MenuItemCounterOld < 5 )
                    {
                        MenuX = B10_M_LEFT;
                        MenuY = Bv_B1_Y + 2 + MenuItemCounterOld*Bv_H;
                    }
                    else
                    {
                        MenuX = B10_M_RIGHT;
                        MenuY = Bv_B1_Y + 2 + (MenuItemCounterOld - 5)*Bv_H;
                    }

					SDK_RedrawButton( (UWORD) MenuX, (UWORD) MenuY, BUTTON_UP );
				}

				//Keep choosen button down
                if ( MenuItemCounter < 5 )
                {
                    MenuX = B10_M_LEFT;
                    MenuY = Bv_B1_Y + 2 + MenuItemCounter*Bv_H;
                }
                else
                {
                    MenuX = B10_M_RIGHT;
                    MenuY = Bv_B1_Y + 2 + (MenuItemCounter - 5)*Bv_H;
                }

				ButtonId = SDK_GetButtonEvent( (UWORD) MenuX, (UWORD) MenuY );
				// button area?
				if ( ButtonId )
				{
					SDK_RedrawButton( (UWORD) MenuX, (UWORD) MenuY, BUTTON_DOWN );
				}
				else
				{
                    MenuItemCounter = MenuItemCounterOld;
				}


				if ( (KeyVal == EVT_BUTTON_SELECT) || ( KeyVal == ENTER_KY ) || (KeyVal == RIGHT_MIDDLE_KY ))
				{
                    if ( MenuItemCounter < 5 )
                    {
                        MenuX = B10_M_LEFT;
                        MenuY = Bv_B1_Y + 2 + MenuItemCounter*Bv_H;
                    }
                    else
                    {
                        MenuX = B10_M_RIGHT;
                        MenuY = Bv_B1_Y + 2 + (MenuItemCounter - 5)*Bv_H;
                    }

					ButtonId = SDK_GetButtonEvent( (UWORD) MenuX, (UWORD) MenuY );
					// button area?
					if ( ButtonId )
					{
						SDK_RedrawButton( MenuX, MenuY, BUTTON_DOWN );
					}

					InputType = DEV_PAD;
					SDK_Beeper( 12 * TENMS );	// Only beep if button gets hit
					break;
				}

				//Don't send the key out
				KeyVal = 0;
			}
			else
			{//DIALOG system - process control buttons: emulate pressing the drawn buttons
				if (KeyVal == LEFT_DOWN_KY) //Down	
                                            // 
                {
					MenuX = CTL_BTN1_X;
					MenuY = CTL_BTN_Y;
				}

				if (KeyVal == EVT_BUTTON_SELECT) //Select
				{
					MenuX = CTL_BTN2_X;
					MenuY = CTL_BTN_Y;
				}

				if (KeyVal == RIGHT_DOWN_KY ) //Up
				{
					MenuX = CTL_BTN3_X;
					MenuY = CTL_BTN_Y;
				}

				if ( ( KeyVal == LEFT_DOWN_KY) ||
                     ( KeyVal == EVT_BUTTON_SELECT ) ||
                     ( KeyVal == RIGHT_DOWN_KY) )		
                    		{
					ButtonId = SDK_GetButtonEvent( (UWORD) MenuX, (UWORD) MenuY );
					// button area?
					if ( ButtonId )
					{
						SDK_RedrawButton( MenuX, MenuY, BUTTON_DOWN );
						SDK_Beeper( 12 * TENMS );	// Only beep if button gets hit
						InputType = DEV_PAD;
						break;
					}
				}
			}
//------------------------------------
		}

		if ( '\x7f' == KeyVal )  //DISABLED KEY: IGNORED
		{
			KeyVal = 0;
		}

		// Just in case KeyBuffer[0] was 0x00.
		if ( KeyVal != 0 )
		{
			if ( orvOK == KeyStatus.Error )
			{
				// Valid keyboard input
				InputType = DEV_KBD;	// Flag Key Press
			}
			else
			{
				// Keyboard error, close all devices
				*pDevFlag = DEV_ABORT;
			}
			break;
		}

		// CHECK FOR CARD READER INPUT
		if ( DevMap & DEV_CARDRDR )
		{
			// Test for track 2 complete.  Must always have track 2
			if ( Trk2Status.CompleteFlag )
			{
				// Track 2 is complete. Check completion of track 1
				if ( 0 == Trk1Status.CompleteFlag )
				{
					// Delay for 100ms to allow track 1 read to complete.
					// Track 1 error allowed will be tested in acctproc.
					SDK_Wait( TENMS * 10 );
				}

				// Must have a good read on TRK2.
				if ( orvOK == Trk2Status.Error )
				{
					// Valid card read
					SDK_BeepIt( 1 );
				}
				else
				{
					// Device error, close all devices
					*pDevFlag = DEV_ABORT;
				}
				InputType = DEV_CARDRDR;
				break;
			}
			else
			{
				// See if track 1 is complete.
				// If track 1 only card is read do not accecpt it and clean up.
				if ( Trk1Status.CompleteFlag )
				{
					// Delay for 200ms to allow track 2 read to complete.
					SDK_Wait( TENMS * 20 );

					// If track 2 is complete then OK, else abort to clean up.
					// If track 2 ok then next time thru loop should process trk2.
					if ( 0 == Trk2Status.CompleteFlag )
					{
						memset( TRINP.TRTRK1, 0,S_TRTRK1 );

						// Force complete with read errors so caller
						// knows the card swipe was not accepted.
						Trk1Status.CompleteFlag = 1;
						Trk1Status.Error = orvINVDATA;
						Trk2Status.CompleteFlag = 1;
						Trk2Status.Error = orvINVDATA;
						InputType = DEV_CARDRDR;
						*pDevFlag = DEV_ABORT;
						break;
					}
				}
			}

		}

		// CHECK FOR PINPAD CARD READER INPUT
		if ( ( DevMap & DEV_PINCARD ) &&
			 ( PINSTAT & PinCard_Read ) && ( PINSTAT & PinCardReadReqPend ) )
		{
			// Test for pinpad input if card was read on the pinpad.
			if ( PinCardReadStatus.CompleteFlag )
			{
				// Card read complete on the pin pad.
				if ( orvOK == PinCardReadStatus.Error )
				{
					// Yes; get card read response from Pinpad
					*pbPinStatus = PinCardReadResp ();
					// Must have a good read on TRK2
					if ( *pbPinStatus )
						InputType = DEV_PINCARD;
					else
						*pDevFlag = DEV_ABORT;	// Flag to close devices
				}
				else
				{
					*pDevFlag = DEV_ABORT;	// Flag to close devices
				}
				break;
			}
		}

#ifdef MAKE_EMV					// CHECK FOR SMART CARD READER (SCR) INPUT
		if ( DevMap & DEV_SCR )
		{
			Card_Status = SCRD_INS_NO;
			ScrdInserted ( scrd_SCR, &Card_Status );
			if ( SCRD_INS_YES == Card_Status )
			{
				// Smart card inserted
				SDK_DisplayBackLight( 1 );
				backlight_timer = BACKLIGHT_DELAY;
				SDK_BeepIt( 1 );	// Sound ack
				InputType = DEV_SCR;
				// Clear corresponding DevFlag bit to re-open Trk1&2 next time we get here
				*pDevFlag &= ~DEV_CARDRDR;
				break;
			}
		}						// End of SCR Input check
#endif // MAKE_EMV

		// Exit loop if timed_out.
		if ( 0 != timed_out )
		{
            // Timed out - draw button back up before exit.
			if ( ButtonId )
				SDK_RedrawButton( (UWORD)PadDatLast.X_Pos, (UWORD)PadDatLast.Y_Pos, BUTTON_UP );
			break;
		}

		// Relinquish CPU while waiting on timed input.
		SDK_RelinqCPU(  );
	}							// End of while( True )....

	SDK_RelinqCPU(  );

	// Cancel timer, if not already timed out
	if ( timed_out == 0 )
		SDK_CancelTimer( timerID );

	// If time out or error occurred, close all devices
	if ( DEV_ABORT == *pDevFlag )
	{
        close_io( 0 );
		*pDevFlag = 0;			// Reset device flag
		return InputType;
	}

	// If no input, and we want to stay open waiting for input,
	// then just return now.
	if ( !InputType )
	{
		// If the timer was set to stay in Read_IO for the duration
		// then clear DevMap so all devices will be reset.
		if ( tmout != 0 )
		{
			DevMap = 0;
		}
		else
		{
			if ( DevMap & ( STAY_OPEN0 | STAY_OPEN1 ) )
            {
                return InputType;
            }
		}
	}

	// Was the input a button press on the pad?
	if ( ButtonId )
	{
		// If the button was a screen control key (prev, next, cancel)
		// then force a close on all devices
		if ( ButtonId >= CANCEL_KY )
			DevMap = 0;
		else
			switch ( ButtonId )
			{
				case ENTER_KY:
				case 0x0A:		// Qwerty ENTER button
				case 0x1B:		// ESC = CLEAR
				case CLEAR_KY:	// User pressed Cancel
					DevMap = 0;	// Force Close devices
					break;
			}
	}

	// Was the "CLEAR" or "ENTER" key pressed?
	if ( ( CLEAR_KY == KeyVal ) || ( ENTER_KY == KeyVal ) )
		DevMap = 0;			// Force Close devices

	// Close all devices, if DevMap says so
	if ( !DevMap || ( DevMap & STAY_OPEN0 ) )
	{
        close_io( DevMap );
		*pDevFlag = 0;			// Reset device flag
		return InputType;
	}

	return InputType;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Returns last read device.
//!
//! \return
//!     The device which was read last.
//!
//-----------------------------------------------------------------------------
extern UBYTE GetLastInputType( void )
{
	return InputType;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Called from printer handler to read the RTC.
//!
//! \return
//!     void
//!
//! \note
//!     PRT_RTCbuff contains the output of the real time clock
//-----------------------------------------------------------------------------
extern void PRT_Read_RTC( void )
{
	SDK_RtcRead( PRT_RTCbuff );	// Read the RTC
}

//=============================================================================
// Private function definitions
//=============================================================================
//-----------------------------------------------------------------------------
//! \brief
//!     Opens specified devices.
//!
//! \param
//!		[in]DevMap      contains bit flags telling which device(s) to close.
//!
//! \return
//!     0 on succes else bit flag of device(s) has caused during opening.
//-----------------------------------------------------------------------------
static UWORD open_io( UWORD DevMap )
{
    UWORD nRes= 0;

    // Open the keyboard, if requested
    if ( DevMap & DEV_KBD )
    {
        OpenKeyPad ();
    }
    // Open the card reader, if requested
    if ( DevMap & DEV_CARDRDR )
    {
        Trk1ReadLength = MAXTRK1;
        if( orvOK != SDK_GetTrk1( TRINP.TRTRK1, &Trk1ReadLength, 0, &Trk1Status ) )
        {
            nRes|= DEV_CARDRDR;
        }
        else
        {
            Trk1Status.CompleteFlag= 0;
        }
        Trk2ReadLength = MAXTRK2;
        if( orvOK != SDK_GetTrk2( TRINP.TRTRK2, &Trk2ReadLength, 0, &Trk2Status ) )
        {
            nRes|= DEV_CARDRDR;
        }
        else
        {
            Trk2Status.CompleteFlag= 0;
        }
    }

    // Open the touch pad reader, if requested
    if( DevMap & DEV_PAD )
    {
        SDK_SetPad( 0, &PadStatus );
        PadBtnId = 0;
        DBGTRACE (LVL_WARNING, TYPE_SPOS32, "<- Pad Open ->\n" );
    }
    // Don't start read card from the pin pad if read is already up.
    // Only start read if at idle.
    if ( ( DevMap & DEV_PINCARD ) &&
         ( PINSTAT & PinCard_Read ) && !( PINSTAT & PinCardReadReqPend ) )
    {
        PinCardRead ();
    }
    return nRes;
}
//-----------------------------------------------------------------------------
//! \brief
//!     Closes specified devices.
//!
//! \param
//!		[in]DevMap      contains bit flags telling which device(s) to close.
//!
//! \return
//!     void
//-----------------------------------------------------------------------------
static void close_io( UWORD DevMap )
{
    // Close the card reader, if requested
    if ( !DevMap || (DevMap & DEV_CARDRDR) )
    {
        SDK_Trk1Close();
        SDK_Trk2Close();
    }

    // Close the Keyboard, if requested
    if ( !DevMap || (DevMap & DEV_KBD) )
    {
        SDK_KbdClose();
        KeyStatus.CompleteFlag = 0;
    }

    // Close the card reader, if requested
    if ( !DevMap || ((DevMap & DEV_PINCARD)
         && (PINSTAT & PinCard_Read) && (PINSTAT & PinCardReadReqPend)) )
    {
        PinReset();
    }

    // Close the card reader, if requested
    if( DevMap & DEV_PAD )
    {
        SDK_PadClose();
        PadStatus.CompleteFlag = 0;
        KeyStatus.CompleteFlag = 0;
        PadBtnId = 0;
        DBGTRACE (LVL_WARNING, TYPE_SPOS32, "<- Pad Close ->\n" );
    }
}


