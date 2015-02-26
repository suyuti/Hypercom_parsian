
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

//=============================================================================
// External variables / declarations
//=============================================================================


#ifdef MAKE_VIKING
//Control buttons touch coords
#define VIKING_BUTTON1_X	20
#define VIKING_BUTTON2_X	60
#define VIKING_BUTTON3_X	100
#define VIKING_BUTTON_Y		60

//Menu strat X coord
#define V_MENU_START_X		20

#define V_ALPHANUM_W		13
#define V_ALPHANUM_KEY_W	9
#define V_ALPHANUM_KEY_H	12

#define V_ALPHANUM_SHIFT_X	5
#define V_ALPHANUM_SHIFT_Y	15

#ifdef MAKE_VIKING

#define VIKING_BUTTON_UP	UP_KY
#define VIKING_BUTTON_DOWN	DOWN_KY
#define VIKING_BUTTON_SELECT	SELECT_KY

#else

#define VIKING_BUTTON_UP	'1'
#define VIKING_BUTTON_DOWN	'3'
#define VIKING_BUTTON_SELECT	'2'

#endif

#endif

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
static UBYTE InputType;

#ifdef MAKE_VIKING
RECT menuRect;

//  from menu.c
#define NEXT_ENABLE_FLAG			0
#define PREV_ENABLE_FLAG			11
#endif

//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  Read_IO         Called to perform input from various devices
//
//  Parameters:     None
//
//  Global Inputs:  None
//
//  Returns:        Nothing
//
//  Notes:          On entry:
//                      map contains bit flags telling which device(s) to read
//                  On exit:
//                      DevFlag is set to the devices left open
//                      InputType tells the device which was read
//                      Depending on InputType, various status and data structures
//                      contain the data which was read.
//-----------------------------------------------------------------------------

extern UBYTE read_io( UWORD map, OS_TIMES tmout, Bool *pbPinStatus, UWORD *pDevFlag )
{
    UBYTE timerID;
    UBYTE timed_out;
	UWORD  MenuX;
	UWORD  MenuY;		
	UBYTE  MenuItemCounterOld;
//	UBYTE ucInputType = 0;


    static UWORD tbtnid;
	static PADDATA opad;

#ifdef MAKE_EMV	
	UBYTE Card_Status;
#endif

	*pbPinStatus = 0;
	InputType = 0;

	if ( !map )			// Request close all
	{
		SDK_KbdClose(  );
		SDK_Trk1Close(  );
		SDK_Trk2Close(  );
		Trk2Status.CompleteFlag = Trk1Status.CompleteFlag = 0;

        if ( ( PINSTAT & PinCard_Read ) && ( PINSTAT & PinCardReadReqPend ) )
		{
			PinReset ();
		}
		*pDevFlag = 0;			// Reset device open status flags
		KeyStatus.CompleteFlag = 0;
		return InputType;
	}

	// If the requested devices are the same as the last time, then
	// there is no need to close and reopen them.
	if ( *pDevFlag != map )
	{
		if ( *pDevFlag )			// First, close all devices except keyboard
		{
			SDK_Trk1Close(  );
			SDK_Trk2Close(  );

			if ( ( PINSTAT & PinCard_Read ) &&
				 ( PINSTAT & PinCardReadReqPend ) )
			{
				PinReset ();
			}

		}
		else
        {
			OpenKeyPad ();	// Open keypad
        }

		// Open the card reader, if requested
		if ( map & DEV_CARDRDR )
		{
			Trk1ReadLength = MAXTRK1;
			SDK_GetTrk1( TRINP.TRTRK1, &Trk1ReadLength, 0, &Trk1Status );
			Trk2ReadLength = MAXTRK2;
			SDK_GetTrk2( TRINP.TRTRK2, &Trk2ReadLength, 0, &Trk2Status );
		}

		// Don't start read card from the pin pad if read is already up.
		// Only start read if at idle.
		if ( ( map & DEV_PINCARD ) &&
			 ( PINSTAT & PinCard_Read ) && !( PINSTAT & PinCardReadReqPend ) )
		{
			PinCardRead ();
		}

		// Set DevFlag to the devices currently open
		*pDevFlag = map;
	}
	InputType = KeyVal = 0;
	tbtnid = ButtonId = 0;		// Reset button id

	// Set timer
	if ( tmout != 0 )
	{
		// timed_out will be set to zero by the wait and flag call.
		timerID =
			SDK_WaitAndFlag( ( OS_TIMES ) ( ONESECOND * tmout ),
							 ( UBYTE * ) & timed_out );
	}
	else
	{
		timed_out = 1;
	}

	// Input loop
	while ( True )
	{

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
			KeyVal = 0;

#ifdef MAKE_VIKING
//Viking Menu Processing


		if ( ( SDLScreenId == MENU_5BTNTEXT_SCR ) ||
			 ( SDLScreenId == MENU_10BTNTEXT_SCR ) ||
			 ( SDLScreenId == IDLE_SCR )||
             ( SDLScreenId == INIT_SCR ) )
		{//-----Keep the choosen button down-------

			// Screen Coords
            if ( SDLScreenId == MENU_10BTNTEXT_SCR )
            {
                if ( MenuItemCounter < 5 )
                {
                    MenuX = V_MENU_START_X;//20;
                    MenuY = Bv_B1_Y + 2 + MenuItemCounter*Bv_H;
               }
                else
                {
                    MenuX = 70;
                    MenuY = Bv_B1_Y + 2 + (MenuItemCounter - 5)*Bv_H;
                }
            }
            else
            {
                MenuX = V_MENU_START_X;//20;
                MenuY = Bv_B1_Y + 2 + MenuItemCounter*Bv_H;
            }

			tbtnid = SDK_GetButtonEvent( (UWORD) MenuX, (UWORD) MenuY );

			// button area?
			if ( tbtnid )
			{
				SDK_RedrawButton( (UWORD) MenuX, (UWORD) MenuY, BUTTON_DOWN );
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

		if ( (KeyVal == VIKING_BUTTON_UP) ||
			 (KeyVal == VIKING_BUTTON_DOWN) ||
			 (KeyVal == VIKING_BUTTON_SELECT)||
			 (KeyVal == ENTER_KY)			 	)
		{

//------------ Main Menu Processing ---------
			if ( ( SDLScreenId == MENU_5BTNTEXT_SCR ) ||
			     ( SDLScreenId == MENU_10BTNTEXT_SCR ) ||
			     ( SDLScreenId == IDLE_SCR ) ||
                 ( SDLScreenId == INIT_SCR ) )

			{//Simulate Viking up,down,select- MENU system

				if ( timed_out != 0 )
				{
					// Need to wait for pen up - timed_out will be set
					// to zero by the wait and flag call.
					timerID =SDK_WaitAndFlag( ONESECOND, &timed_out );
				}

				if (KeyVal == VIKING_BUTTON_DOWN) //Down
				{	
					MenuItemCounterOld = MenuItemCounter;

                    if ( SDLScreenId == MENU_10BTNTEXT_SCR )
                    {
                        if ( MenuItemCounter < 9 ) 	
                        {
                            MenuItemCounter++;
                        }
                        else
                        {//we should go to the next screen if it exists
                            if ( Enable_Flags[NEXT_ENABLE_FLAG] == 1)	 
                            {
                                ButtonId = B_NEXT;
                                InputType = DEV_PAD;				
                                break;
                            }
                        }
                    }
                    else
                    {
                        if ( MenuItemCounter < 4 ) 	
                        {
                            MenuItemCounter++;
                        }
                        else
                        {//we should go to the next screen if it exists
                            if ( Enable_Flags[NEXT_ENABLE_FLAG] == 1)	 
                            {
                                ButtonId = B_NEXT;
                                InputType = DEV_PAD;				
                                break;
                            }
                        }
                    }
				}

				if (KeyVal == VIKING_BUTTON_UP) //Up
				{	
					MenuItemCounterOld = MenuItemCounter;
					if ( MenuItemCounter > 0 ) 
					{
						MenuItemCounter --;
					}

					else
					{//we should go to the prev screen if it exists
						if (Enable_Flags[PREV_ENABLE_FLAG] == 1) 
						{
							//simulate pressing "back" key
//							ButtonId = B_PREVIOUS;
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
                        MenuX = V_MENU_START_X;//20;
                        MenuY = Bv_B1_Y + 2 + MenuItemCounterOld*Bv_H;
                    }
                    else
                    {
                        MenuX = 70;
                        MenuY = Bv_B1_Y + 2 + (MenuItemCounterOld - 5)*Bv_H;
                    }

					SDK_RedrawButton( (UWORD) MenuX, (UWORD) MenuY, BUTTON_UP );
				}

				//Keep choosen button down
                if ( MenuItemCounter < 5 )
                {
                    MenuX = V_MENU_START_X;//20;
                    MenuY = Bv_B1_Y + 2 + MenuItemCounter*Bv_H;
                }
                else
                {
                    MenuX = 70;
                    MenuY = Bv_B1_Y + 2 + (MenuItemCounter - 5)*Bv_H;
                }

				tbtnid = SDK_GetButtonEvent( (UWORD) MenuX, (UWORD) MenuY );

				ButtonId = tbtnid;

				// button area?
				if ( ButtonId )
				{
					SDK_RedrawButton( (UWORD) MenuX, (UWORD) MenuY, BUTTON_DOWN );
				}
				else
				{
                    MenuItemCounter = MenuItemCounterOld;
				}


				if ( (KeyVal == VIKING_BUTTON_SELECT) || ( KeyVal == ENTER_KY ) ) 
				{	

                    if ( MenuItemCounter < 5 )
                    {
                        MenuX = V_MENU_START_X;//20;
                        MenuY = Bv_B1_Y + 2 + MenuItemCounter*Bv_H;
                    }
                    else
                    {
                        MenuX = 70;
                        MenuY = Bv_B1_Y + 2 + (MenuItemCounter - 5)*Bv_H;
                    }

					tbtnid = SDK_GetButtonEvent( MenuX, MenuY );

					ButtonId = tbtnid;

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
				if (KeyVal == VIKING_BUTTON_UP) //Down
				{	
					MenuX = VIKING_BUTTON1_X;//20;
					MenuY = VIKING_BUTTON_Y;//60;
				}

				if (KeyVal == VIKING_BUTTON_SELECT) //Select
				{	
					MenuX = VIKING_BUTTON2_X;// 60;
					MenuY = VIKING_BUTTON_Y;//60;
				}			

				if (KeyVal == VIKING_BUTTON_DOWN) //Up
				{	
					MenuX = VIKING_BUTTON3_X;//100;
					MenuY = VIKING_BUTTON_Y;//60;
				}
			
				if ( ( KeyVal == VIKING_BUTTON_UP) ||
					 ( KeyVal == VIKING_BUTTON_DOWN) ||
					 ( KeyVal == VIKING_BUTTON_SELECT) 	)

				{
					tbtnid = SDK_GetButtonEvent( MenuX, MenuY );

					ButtonId = tbtnid;

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
#endif // MAKE_VIKING

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
		if ( map & DEV_CARDRDR )
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
						Trk1Status.CompleteFlag = 0x01;
						Trk1Status.Error = orvINVDATA;
						Trk2Status.CompleteFlag = 0x01;
						Trk2Status.Error = orvINVDATA;
						InputType = DEV_CARDRDR;
						*pDevFlag = DEV_ABORT;
						break;
					}
				}
			}

		}

		// CHECK FOR PINPAD CARD READER INPUT
		if ( ( map & DEV_PINCARD ) &&
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
		if ( map & DEV_SCR )
		{
			Card_Status = SCRD_INS_NO ;
			ScrdInserted ( scrd_SCR, &Card_Status ) ;
			// Card_Status = OS_DeviceConfig( SCR, SCR_CARDINSERT, 0 ) ;

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
				SDK_RedrawButton( ( UWORD ) opad.X_Pos, ( UWORD ) opad.Y_Pos,
								  BUTTON_UP );
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
		SDK_KbdClose(  );
		SDK_Trk1Close(  );
		SDK_Trk2Close(  );
#ifndef MAKE_VIKING
		SDK_PadClose(  );
		SDK_RS232Reset(  );
		SDK_RS232Close(  );
#endif // not MAKE_VIKING

		if ( ( PINSTAT & PinCard_Read ) && ( PINSTAT & PinCardReadReqPend ) )
		{
			PinReset ();
		}
		*pDevFlag = 0;			// Reset device flag
		KeyStatus.CompleteFlag = 0;
		return InputType;
	}

	// If no input, and we want to stay open waiting for input,
	// then just return now.
	if ( !InputType )
	{
		// If the timer was set to stay in Read_IO for the duration
		// then clear map so all devices will be reset.
		if ( tmout != 0 )
		{
			map = 0;
		}
		else
		{
			if ( map & ( STAY_OPEN0 | STAY_OPEN1 ) )
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
			map = 0;
		else
			switch ( ButtonId )
			{
				case ENTER_KY:
				case 0x0A:		// Qwerty ENTER button
				case 0x1B:		// ESC = CLEAR
				case CLEAR_KY:	// User pressed Cancel
					map = 0;	// Force Close devices
					break;
			}
	}

	// Was the "CLEAR" or "ENTER" key pressed?
	if ( ( CLEAR_KY == KeyVal ) || ( ENTER_KY == KeyVal ) )
		map = 0;			// Force Close devices

	// Close all devices, if map says so
	if ( !map || ( map & STAY_OPEN0 ) )
	{
		SDK_Trk1Close(  );
		SDK_Trk2Close(  );

#ifndef MAKE_VIKING
		SDK_PadClose(  );
#endif // MAKE_VIKING

		if ( ( PINSTAT & PinCard_Read ) && ( PINSTAT & PinCardReadReqPend ) )
		{
			PinReset ();
		}
		*pDevFlag = 0;			// Reset device flag
		return InputType;
	}

	return InputType;
}

extern UBYTE GetLastInputType( void )
{
	return InputType;
}

//-----------------------------------------------------------------------------
//  PRT_Read_RTC        Called from printer handler to read the RTC
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:              PRT_RTCbuff contains the output of the real time clock
//                      Cross call to this page so both pages don't both have
//                      to pull in the RTC code from SDKLIB
//-----------------------------------------------------------------------------

extern void PRT_Read_RTC( void )
{
	SDK_RtcRead( PRT_RTCbuff );	// Read the RTC
}

//=============================================================================
// Private function definitions
//=============================================================================



