
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001, 2004, 2005
//=============================================================================
//
// Module overview:             sprite.c
//      moving screen image processing
//
//=============================================================================

#include "string.h"
#include "stdlib.h"
#include "eftos.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "Bci.h"
#include "fixdata.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "comm.h"
#include "defines.h"
#include "transdef.h"
#include "msg.h"
#include "pinpad.h"
#include "util.h"
#include "utllib.h"
#include "icepak.h"
#include "screen.h"
#include "sdl.h"
#include "sprite.h"
#include "img.h"
#include "rs232tab.h"
#include "datetime.h"
#include "Threshold.h" //@AAMELIN

//=============================================================================
// External variables / declarations
//=============================================================================
extern Bool Trace_Check( void );
extern Bool Trace_ChkOverflow( void );
Bool Battery_available;
extern Bool IsStateSocketIdle( void );
extern Bool COM_GSMLowSignalLevel;

//=============================================================================
// Private variables
//=============================================================================
Bool NoCommsFlag;

//=============================================================================
// Private defines and typedefs
//=============================================================================
#define SPRITE_VERTICAL 	0x01
#define SPRITE_HORIZONTAL 	0x02
#define SPRITE_ONEWAY		0x04
#define SPRITE_TWOWAY	    0x08


typedef struct
{
	UBYTE flags;				// determines sprite type
	UWORD startx;				// starting x pixel
	UWORD starty;				// starting y pixel
	int step;					// step size in pixels (can be negative for left or up moving sprites )
	UBYTE stepcount;			// number of steps to end
	UBYTE *image;				// pointer to image
	RECT rect;					// set by SetupSprite(), image area
	UBYTE current;				// maintained by SpriteStep(), current x or y pixel where image is
	UBYTE count;				// maintained by SpriteStep(), step counter
}
SPRITEDEF;

// sprite table entry defintion
typedef struct
{
	SPRITE_ID spriteID;
	void ( *pInitFunc ) ( void );	// sprite initialization function
	void ( *pStepFunc ) ( void );	// sprite step function
	OS_TIMES time;				// time period to call stprite step function
}
SPRITE_ENTRY;

typedef struct
{
	SPRITE_ENTRY *pEntry;
	UBYTE timerId;
	UBYTE timerFlag;
}
ACTIVE_SPRITE;


//=============================================================================
// Private function declarations
//=============================================================================
static void SetupSprite( SPRITEDEF * sprite );
static void SpriteStep( SPRITEDEF * sprite );

static void init_swipe_card( void );
static void step_swipe_card( void );

static void init_alternating_menu( void );
static void step_alternating_menu( void );
static void draw_altmenu_text( void );

static void init_GSMmeter( void );
static void step_GSMmeter( void );

static void init_Battery( void );
static void step_Battery( void );

static void init_Flash( void );
static void step_Flash( void );

static SPRITE_ENTRY *GetSpriteEntry( SPRITE_ID spriteID );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static const SPRITE_ENTRY SpriteTable[NUM_OF_SPRITES] = {

	{SPRITE_SWIPE_CARD, init_swipe_card, 
    step_swipe_card, 20 * TENMS},

	{SPRITE_ALTERNATING_MENU, init_alternating_menu,
	 step_alternating_menu, 150 * TENMS},

	{SPRITE_GSMMETER, init_GSMmeter,
	 step_GSMmeter, 100 * TENMS},

	{SPRITE_BATTARY, init_Battery,
	 step_Battery, 100 * TENMS},

	{SPRITE_FLASH, init_Flash,
	 step_Flash, 30 * TENMS},

};

static const SPRITEDEF CardDefault =
	{ SPRITE_VERTICAL | SPRITE_TWOWAY, 117, 12, 3, 13, ( UBYTE * ) IMG_CARD };

static SPRITEDEF Card;

static ACTIVE_SPRITE ActiveSprite[NUM_OF_SPRITES];	// sprite queue
static UBYTE QueueCount;		// Number of items in Queue
static UBYTE SpriteSem;

static UBYTE altmenuToggle;
static UBYTE GSMmeterToggle;
static DWORD BatteryToggle;
static DWORD VarPart;
static UBYTE CardDirection;  // 1=Card to down; 2=Card to up
static UBYTE CardSteps;       // Card sprite steps

static const RECT menuRect = { 0, 0, SCR_PIXEL_WIDTH, TITLE_H  };
static const RECT GSMmeterRect = { 2, 12, 13, 24 };
static const RECT BatteryRect = { 2, 49, 8, 13 };
static const RECT FlashRect = { 118, 17, 7, 1 };

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  DoSprites           Draw enabled sprites
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

extern void DoSprites( void )
{
	UBYTE i;

    // Loop by sprites registered on the current screen
    // Note: this code does nothing if QueueCount=0 (no sprites on the current screen)
    for ( i = 0; i < QueueCount; i++ )
	{
        // Check if the time for the step occurs
        // and the step function is available
        if ( ( ActiveSprite[i].timerFlag ) &&
             ( ActiveSprite[i].pEntry->pStepFunc ) )
		{
            // Get semaphore
            GetSpriteSem( );

            // Make sure that sprites not disabled by anoter caller (DrawScreen)
            // Note, QueueCount can be cleared by GetSpriteSem
            if ( 0 != QueueCount )
            {
                // Call step function
                ActiveSprite[i].pEntry->pStepFunc(  );

                // Restart timer, save timer number
                ActiveSprite[i].timerId =
                    SDK_WaitAndFlag( ActiveSprite[i].pEntry->time,
                                    &ActiveSprite[i].timerFlag );
            }

            // Relase semaphore
            ReleaseSpriteSem( );
		}
    }
}


//-----------------------------------------------------------------------------
//  GetSpriteSem    X-Callable functions to get sprite semaphore
//
//  Parameters:     None
//
//  Global Inputs:  None
//
//  Returns:        Nothing
//
//  Notes:          Called to halt sprites on a screen. Useful when
//                  displaying an error box on a screen with sprites
//-----------------------------------------------------------------------------

extern void GetSpriteSem( void )
{
	GetSem( &SpriteSem );
}



//-----------------------------------------------------------------------------
//  ReleaseSpriteSem    X-Callable functions to release sprite semaphore
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

extern void ReleaseSpriteSem( void )
{
	RelSem( &SpriteSem );
}



//-----------------------------------------------------------------------------
//  InitSprites         Perform any initialization required for sprites
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

extern void InitSprites( void )
{
	QueueCount = 0;				// start with empty que
	RelSem( &SpriteSem );		// make sure sprite semaphore is released
}



//-----------------------------------------------------------------------------
//  AddSprite           Set active the sprite flag for later adding 
//                      
//
//  Parameters:         SPRITE_ID spriteID  Id of added sprite
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

extern void AddSprite( SPRITE_ID spriteID )
{
	SPRITE_ENTRY *pNewSprite;

	pNewSprite = GetSpriteEntry( spriteID );

	if ( 0 == pNewSprite )
		return;					// no table entry

	QueueCount++;				// number of items in queue
	if ( QueueCount > NUM_OF_SPRITES )
	{
		QueueCount--;
		return;					// no room
	}
	ActiveSprite[QueueCount - 1].pEntry = pNewSprite;	// add sprite
	ActiveSprite[QueueCount - 1].timerFlag = 1;	// kick start

	if ( pNewSprite->pInitFunc )
		pNewSprite->pInitFunc(  );	// call initialization function

}

//-----------------------------------------------------------------------------
//  DeleteAllSprites    Clear sprite queue
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

extern void DeleteAllSprites( void )
{
	UBYTE i;

	// cancel any pending timers
	for ( i = 0; i < QueueCount; i++ )
	{
		if ( 0 == ActiveSprite[i].timerFlag )
		{
			SDK_CancelTimer( ActiveSprite[i].timerId );
		}
	}
	QueueCount = 0;
}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  GetSpriteEntry      Returns pointer in SPRITE_ENTRY table for given id
//
//  Parameters:         SPRITE_ID spriteID  Identifier number of sprite
//
//  Global Inputs:      None
//
//  Returns:            SPRINT_ENTRY *
//                      pointer to sprite table entry
//  Notes:
//-----------------------------------------------------------------------------

static SPRITE_ENTRY *GetSpriteEntry( SPRITE_ID spriteID )
{
	UBYTE i;
	SPRITE_ENTRY *pReturnVal = 0;

	// find table entry for this sprite
	for ( i = 0; i < sizeof( SpriteTable ) / sizeof( SPRITE_ENTRY ); i++ )
	{
		if ( SpriteTable[i].spriteID == spriteID )
			pReturnVal = ( SPRITE_ENTRY * ) & SpriteTable[i];

	}
	return pReturnVal;
}


//-----------------------------------------------------------------------------
//  init_swipe_card     Initialization function for moving card image
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

static void init_swipe_card( void )
{
	// Do not do card sprite if external Pin Pad with Card reader
	if ( !( PINSTAT & PinCard_Read ) )
	{
		memcpy( &Card, &CardDefault, sizeof( SPRITEDEF ) );
		SetupSprite( &Card );
	}

    // Indicate that Card sprite moves to down
    CardDirection = 1;
    CardSteps = 0;
}



//-----------------------------------------------------------------------------
//  step_swipe_card     Step function for moving card image
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

static void step_swipe_card( void )
{
	// Do not do card sprite if external Pin Pad with Card reader
	if ( !( PINSTAT & PinCard_Read ) )
	{

#ifdef MAKE_EMV
        // Increase the number of steps processed
        CardSteps++;

        // Check if end of direction
        if ( CardSteps == CardDefault.stepcount )
        {
            // Start step counter again
            CardSteps = 0;

            // What is a direction?
            if ( 1 == CardDirection )
            {
                // Down - change image to ICC Card
                CardDirection = 2;
                Card.image = ( UBYTE * )IMG_ICCCARD;
            }
            else
            {
                // Up - change image to Magnetic Card
                CardDirection = 1;
                Card.image = ( UBYTE * )IMG_CARD;
            }

        }
#endif // MAKE_EMV

		SpriteStep( &Card );
	}
}



//-----------------------------------------------------------------------------
//  init_alternating_menu   Initialization function for alternating menu
//
//  Parameters:             None
//
//  Global Inputs:          None
//
//  Returns:                Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

static void init_alternating_menu( void )
{
	altmenuToggle = 1;
	draw_altmenu_text( );
}



//-----------------------------------------------------------------------------
//  step_alternating_menu   step function function for alternating menu
//
//  Parameters:             None
//
//  Global Inputs:          None
//
//  Returns:                Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

static void step_alternating_menu( void )
{
	draw_altmenu_text( );

    // check if no 4-th state processed
    if ( 0xFF == altmenuToggle )
    {
        // yes, call again with state 1
		altmenuToggle = 1;
        draw_altmenu_text( );
    }

	if ( 4 == altmenuToggle )
		altmenuToggle = 1;
	else
		altmenuToggle++;
	
    SDK_GuiDispPart( ( RECT * ) & menuRect );
}


//-----------------------------------------------------------------------------
//  SetUpSprite         Initialize a sprite defined with SPRITEDEF
//
//  Parameters:         SPRITEDEF * sprite
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

static void SetupSprite( SPRITEDEF * sprite )
{
	IMAGE_PARMS Iparms;
	if ( sprite->flags & SPRITE_VERTICAL )
	{
		sprite->current = ( UBYTE ) sprite->starty;
	}
	else
	{
		sprite->current = ( UBYTE ) sprite->startx;
	}

	sprite->count = 0;
	SDK_GetImageParms( sprite->image, &Iparms );
	sprite->rect.x = sprite->startx;
	sprite->rect.y = sprite->starty;
	sprite->rect.height = Iparms.Height;
	sprite->rect.width = Iparms.Width;
}



//-----------------------------------------------------------------------------
//  SpriteStep          Perform a single step for a sprite defined by SPRITEDEF
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

static void SpriteStep( SPRITEDEF * sprite )
{
	RECT savevir;
	UWORD current, next;
	UBYTE flags;

	SDK_GuiGetVirWin( &savevir );
	SDK_GuiDefWindow(  );

	current = sprite->current;	// get current x or y position
	sprite->count++;
	flags = sprite->flags;

	if ( sprite->count == sprite->stepcount )	// if at end of line
	{
		sprite->count = 0;

		if ( flags & SPRITE_TWOWAY )
		{
			sprite->step = -sprite->step;	// change sign of step
			next = current;
		}
		else
		{
			if ( flags & SPRITE_VERTICAL )
			{
				next = sprite->starty;
			}
			else
			{
				next = sprite->startx;
			}
		}
	}
	else
		next = current + sprite->step;


	if ( flags & SPRITE_VERTICAL )
	{
		sprite->rect.y = current;
		SDK_GuiClrArea( &sprite->rect );	// clear previous image area
		SDK_GuiDispPart( &sprite->rect );
		sprite->rect.y = next;	// move to next location
	}
	else
	{
		sprite->rect.x = current;
		SDK_GuiClrArea( &sprite->rect );	// clear previous image area
		SDK_GuiDispPart( &sprite->rect );
		sprite->rect.x = next;	// move to next location
	}

	SDK_GuiImage( sprite->rect.x, sprite->rect.y, sprite->image );
	SDK_GuiDispPart( &sprite->rect );
	sprite->current = ( UBYTE ) next;

	SDK_GuiSetWindow( &savevir );
}



//-----------------------------------------------------------------------------
//  draw_altmenu_text   draw text of alternating menu to screen buffer
//                      This function toggles display between date/time
//                      and CutomMsg (AltMenuStr)
//
//  Parameters:         None
//
//  Global Inputs:      altmenuToggle   1..4 draw state
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------

static void draw_altmenu_text( void )
{
	UWORD LeftX;
	CHARSIZE charsz;
    UBYTE LineStatus;
	PRTSTATUS printStat;

//-----------------------------------------------------------------------------
//
// State 1 - "Swipe or insert card" or "Please initialise" or Custom idle prompt
//
//-----------------------------------------------------------------------------
    if ( 1 == altmenuToggle )
	{
		//Clear Title
		memset( Dspbuf, ' ', 21);
		Dspbuf[19] = 0;

        // Check if init required
		if ( 0x00 == TERM.TERMInit )
        {
            // Show "Please Initialize"
            GetMsg( PleaseInit, Dspbuf );
        }
        else
        {
            // Check if there is custom idle prompt
            if ( ( 0 == FTTAB.FTIDLEPROMPT[0] ) || 
                 ( !memcmp(FTTAB.FTIDLEPROMPT,"                    ",S_CSTM_IDLE)) )
            {
                // No, show "Swipe or insert card"
                GetMsg( N_SwipeOrInsertCard, Dspbuf );
            }
            else
            {
                // Yes, show custom tdle prompt
                memcpy (Dspbuf, FTTAB.FTIDLEPROMPT, S_CSTM_IDLE );
                Dspbuf[S_CSTM_IDLE] = 0; 
            }
        }
	}


//-----------------------------------------------------------------------------
//
// State 2 - Date/Time/Shift
//
//-----------------------------------------------------------------------------
	if ( 2 == altmenuToggle )
    {
		//Clear Title
		memset( Dspbuf, ' ',  21);
		Dspbuf[19] = 0;
                FormatDateTime ();
		MoveTillZero( Dspbuf, CSTMSG5, sizeof( CSTMSG5 ) );
    }

//-----------------------------------------------------------------------------
//
// State 3 - "Training mode", "*** Tracing ***", "** Trace overflow"
//                            or 
//            Comm module status if it wrong
//
//-----------------------------------------------------------------------------
	if (  3 == altmenuToggle )
	{
		//Clear Title
		memset( Dspbuf, ' ', 21);
		Dspbuf[19] = 0;

		if ( TCONF.TCOPT4 & TC4_TRAINING )
		{
			// Yes; display Training Mode
			GetMsg( TrainingMode, Dspbuf );
		}
		else
		{
            // Check if tracing is ON
            if ( Trace_Check() )
            {
                // Yes, tracing is ON - do not check for Line status
                // notify user about tracing mode
                NoCommsFlag = True;

                // Check if trace overflow
                if ( Trace_ChkOverflow () )
                    GetMsg( TraceOverflow, Dspbuf );
                else
                    GetMsg( Tracing, Dspbuf );
            }
            
            // Perform line status checking depended on COMM module type
            // and COMMS port used
                    // Do not check COMM module status if COM1/COM2 used
            else if  ( ( RSTAB.RSOPT & RSOPT_RSACTIVE ) ||
                    // Do not check COMM module status if Bluetooth used
                       ( TM3_BLUETOOTH == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
                    // Do not check COMM module status if ISDN used
                       ( TM3_ISDN == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) )
                    )
            {
                // The status does not required
                NoCommsFlag = False;
            }
            else
            {
                // Set preliminary "no comms" status
                NoCommsFlag = True;
                GetMsg( NoComms, Dspbuf );

                // Get the communication line status
//                LineStatus = SDK_ComInfo ( ci_LINESTATUS );
// FIXME: don't do anything but data access in sprites and display!
		 if ( IsStateSocketIdle( ) )
		 {
		 	LineStatus = -1;
                 	NoCommsFlag = False;
		 }
		 else
		 {
	                LineStatus = 1;// GetLineStatus( ); // TEMP

	         	// -- GSM module state interpreter
	         	if( ( TM3_GSM == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
	                    ( TM3_GSM_OLD == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
	                    ( TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) )
	                {
	                    switch (LineStatus)
	                    {
	                        case 1: // "registred, home network"
	                        case 5: // "registred, roaming"
				    if ( COM_GSMLowSignalLevel && ( TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK)))
				    {
		                        GetMsg( LowSignal, Dspbuf );
		                        NoCommsFlag = True;
				    }
				    else
	                            	NoCommsFlag = False;
	                            break;

	                        case 0: // "not registered, ME is currently NOT searching for new operator"
	                        case 2: // "not registered, ME is currently searching for new operator"
	                        case 3: // "registration denied"
	                        case 4: // "unknown"
	                        default:
	                            GetMsg( NoComms, Dspbuf );
	                            NoCommsFlag = True;
	                            break;
	                    }
	                }
		}
            
                // -- Ethernet module state interpreter
                if( TM3_ETHERNET == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) )
                {
                    switch (LineStatus)
                    {
                        case 1: // "ready"
                            NoCommsFlag = False;
                            break;

                        case 0: // "unknown status"
                        case 2: // "no Ethernet link"
                        case 3: // "DHCP in progress"
                        default:
                            GetMsg( NoComms, Dspbuf );
                            NoCommsFlag = True;
                            break;
                    }
                }

                // -- Analog telephone phone modem V22 or V90 state interpreter
                if( ( TM3_V22 == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
                    ( TM3_V90 == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) )
                {
                    switch (LineStatus)
                    {
                        case 1: // "Ok"
                            NoCommsFlag = False;
                            break;

                        case 0: // "No phone line"
                        default:
                            GetMsg( NoPhoneLine, Dspbuf );
                            NoCommsFlag = True;
                            break;
                    }
                }

                // -- Hypercom LAN state interpreter
                if( TM3_LAN == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) 
                {
                    switch (LineStatus)
                    {
                        case 1: // "connection present"
                            NoCommsFlag = False;
                            break;

                        case 0: // "unknown status"
                        case 2: // "connection absent"
                        case 3: // "closed by a Disc. Request"
                        case 4: // "closed by remote host"
                        case 5: // "closed by timeout"
                        default:
                            GetMsg( NoComms, Dspbuf );
                            NoCommsFlag = True;
                            break;
                    }
                }

            } // !( RSTAB.RSOPT & RSOPT_RSACTIVE )

            // Skip this state if not training and not comm errors
			if ( ! NoCommsFlag )
                altmenuToggle = 4;
        }
	}


//-----------------------------------------------------------------------------
//
// State 4 - "Paper Out" or "Printer Error"
//
//-----------------------------------------------------------------------------
	if ( ( 4 == altmenuToggle ) && ( 0x00 == TERM.TERMInit ) )
    {
        // Do not check for printer if init required
        altmenuToggle = 0xFF;
        return;
    }

    if ( ( 4 == altmenuToggle ) && ( TCONF.TCOPT2 & TC2_PRINT ) )
    {

        //Clear Title
		memset( Dspbuf, ' ', 21);
		Dspbuf[19] = 0;

        // Open the printer
        if ( orvOK == SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ) )
        {
            // Ask printer about status
            if ( orvOK == SDK_PrinterStatus( &printStat ) )
            {
                if ( prt_PAPER_OUT == printStat )
                {
                    // Paper out
                    GetMsg( PaperOut, Dspbuf );
                }
                else if ( prt_OVERHEAT == printStat )
                {
                    // Printer overheat
                    GetMsg( PrinterOverheat, Dspbuf );
                }
                else if ( prt_PAPER_OK != printStat )
                {
                    // Printer error
                    GetMsg( PrintError, Dspbuf );
                }
                else
                {
                    // Close printer
                    SDK_PrinterClose(  );

                    // paper OK - skip this state
                    // and direct caller to perform 1-st state
                    altmenuToggle = 0xFF;
                    return;
                }
            }
            // status not returned
            else
            {
                // Printer error
                GetMsg( PrintError, Dspbuf );
            }
        }
        else
        {
            // Printer error
            GetMsg( PrintError, Dspbuf );
        }

        // Close printer
        SDK_PrinterClose(  );
    }

	// Get Font size
	SDK_GuiGetCharSize( &charsz );

	// Clear the Menu area
	SDK_GuiClrArea( ( RECT * ) & menuRect );

	// Calculate the offset to first text character
	LeftX =	( SCR_PIXEL_WIDTH -
		  StrLn( Dspbuf, sizeof( Dspbuf ) ) * charsz.Width ) / 2;

	// Display the text
	SDK_GuiTextP( ( UWORD ) LeftX,
				  ( UWORD ) ( 1 ), Dspbuf );

}

//-----------------------------------------------------------------------------
//  init_GSMmeter       Clear GSM meter image area
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------
static void init_GSMmeter( void )
{
    //  Check if GSM modem used
    if( ( TM3_GSM == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
        ( TM3_GSM_OLD == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
        ( TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
        ( TM3_BLUETOOTH == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) )
    {
        GSMmeterToggle = 0;

        // Clear the GSM meter area
        SDK_GuiClrArea( ( RECT * ) & GSMmeterRect );
    }

}

//-----------------------------------------------------------------------------
//  step_GSMmeter       Determine GSM signal value and display corresponding
//                      image OR display BlueTooth logo
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------
static void step_GSMmeter( void )
{
    static UBYTE *ImgPtr;

    //  Check if BlueTooth modem used
    if( TM3_BLUETOOTH == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) )
    {
        // Clear the GSM meter area
        SDK_GuiClrArea( ( RECT * ) & GSMmeterRect );

        // Get BlueTooth image pointer
        ImgPtr = GetImage( IMAGE_BLUETOOTH );

        // Draw image
        SDK_GuiImage (GSMmeterRect.x , GSMmeterRect.y, ImgPtr);

        // Display it
        SDK_GuiDispPart( ( RECT* ) & GSMmeterRect );
    }
   
    //  Check if GSM modem used
    if( ( ( TM3_GSM == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
          ( TM3_GSM_OLD == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ) ||
	   ( TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) ))
                        &&
		( !(RSTAB.RSOPT & RSOPT_RSACTIVE) ) ) 
    {

        // Clear the GSM meter area
        SDK_GuiClrArea( ( RECT * ) & GSMmeterRect );

        // Get GSM signal level
		if ( IsStateSocketIdle( ) )
            ImgPtr = GetImage( IMAGE_GSMSIGNAL_100  ); // simulate 100% signal level
		else
		{

//        GSMmeterToggle = SDK_ComInfo( ci_SIGNALLEVEL);
// FIXME: don't do anything but data access in sprites and display!
	        GSMmeterToggle = GetSignalLevel( );
		// Check Threshpld level if currently use GSM/GPRS dial backup 
		if (TM3_DIALBACKUP == (TERM.TERMOpt3 & TM3_HWTYPE_MASK) )
	            CheckThresLevel(GSMmeterToggle);

	        // Select appropriate image
	        if ( ( GSMmeterToggle <= 5 ) || ( GSMmeterToggle == 99 ) )
	            ImgPtr = GetImage( IMAGE_GSMSIGNAL_0  );
	        else if ( ( 6 <= GSMmeterToggle ) && ( GSMmeterToggle <= 10 ) )
	            ImgPtr = GetImage( IMAGE_GSMSIGNAL_20  );
	        else if ( ( 11 <= GSMmeterToggle ) && ( GSMmeterToggle <= 15 ) )
	            ImgPtr = GetImage( IMAGE_GSMSIGNAL_40  );
	        else if ( ( 16 <= GSMmeterToggle ) && ( GSMmeterToggle <= 20 ) )
	            ImgPtr = GetImage( IMAGE_GSMSIGNAL_60  );
	        else if ( ( 21 <= GSMmeterToggle ) && ( GSMmeterToggle <= 25 ) )
	            ImgPtr = GetImage( IMAGE_GSMSIGNAL_80  );
	        else if ( ( 26 <= GSMmeterToggle ) && ( GSMmeterToggle <= 31 ) )
	            ImgPtr = GetImage( IMAGE_GSMSIGNAL_100  );
	        else 
	            ImgPtr = GetImage( IMAGE_GSMSIGNAL_0  );
		}

        // Draw image
        SDK_GuiImage (GSMmeterRect.x , GSMmeterRect.y, ImgPtr);

        // Display it
        SDK_GuiDispPart( ( RECT* ) & GSMmeterRect );

    }
}


//-----------------------------------------------------------------------------
//  init_Battery        Clear Battery area
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------
static void init_Battery( void )
{
    DWORD status;

    BatteryToggle = 0;
    VarPart = 0;
        
    // Clear the Battery area
    SDK_GuiClrArea( ( RECT * ) & BatteryRect );

    // Try to open EFTOS BCI (Battery Charge Interface)
    status = BCI_Open ( 5, FALSE );

    // Determine BCI available flag
    if ( ( OK == status ) || ( PROGRESS == status ) )
        Battery_available = True;
    else
        Battery_available = False;
}

//-----------------------------------------------------------------------------
//  step_Battery        Determine the Battery charge level and display
//                      corresponding image
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------
static void step_Battery( void )
{
    static UBYTE *ImgPtr;

    //  Check if Battery Charge Interface available
    if ( Battery_available )
    {
        // Get battery charge level
        BatteryToggle = BCI_Capacity ( );

        // Convert it to 0..5 scale (0%, 20%, 40%, 60%, 80%, 100%)
        switch ( BatteryToggle )
        {
            case 0:
            case 1:
                BatteryToggle = 0;
                break;
            case 2:
            case 3:
            case 4:
                BatteryToggle = 1;
                break;
            case 5:
            case 6:
            case 7:
                BatteryToggle = 2;
                break;
            case 8:
            case 9:
            case 10:
                BatteryToggle = 3;
                break;
            case 11:
            case 12:
            case 13:
                BatteryToggle = 4;
                break;
            case 14:
            case 15:
                BatteryToggle = 5;
                break;
            default:
                return; // invalid data
        }
        
        // Check if "charge in progress" state
        if ( TRUE == BCI_BatteryCharge ( ) )
        {
            // Make sure that last portion of charge is visible
            if ( BatteryToggle == 5 ) 
                BatteryToggle = 4;

            // Increment variable part
            VarPart++;

            // Check for overflow
            if ( ( BatteryToggle + VarPart ) > 5 )
                VarPart = 0;
        }
        else
        {
            // Normal measurement, no variable part
            VarPart = 0;
        }

        // Build final toggle
        BatteryToggle += VarPart;

        // Select appropriate image
        switch ( BatteryToggle )
        {
            case 0:
                ImgPtr = GetImage( IMAGE_BATTARY_0 );
                break;
            case 1:
                ImgPtr = GetImage( IMAGE_BATTARY_20 );
                break;
            case 2:
                ImgPtr = GetImage( IMAGE_BATTARY_40 );
                break;
            case 3:
                ImgPtr = GetImage( IMAGE_BATTARY_60 );
                break;
            case 4:
                ImgPtr = GetImage( IMAGE_BATTARY_80 );
                break;
            case 5:
                ImgPtr = GetImage( IMAGE_BATTARY_100 );
                break;
            default:
                return; // Unknown image to display
        }

        // Clear the Battery area
        SDK_GuiClrArea( ( RECT * ) & BatteryRect );

        // Draw image
        SDK_GuiImage (BatteryRect.x , BatteryRect.y, ImgPtr);

        // Display it
        SDK_GuiDispPart( ( RECT* ) & BatteryRect );

    }
}


//-----------------------------------------------------------------------------
//  init_Flash          Clear Flash area
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------
static void init_Flash( void )
{
	// Invert the area
    SDK_GuiClrArea( ( RECT * ) & FlashRect );

    // Display it
	SDK_GuiDispPart( ( RECT * ) & FlashRect );
}

//-----------------------------------------------------------------------------
//  step_Flash          Invert Flash area
//
//  Parameters:         None
//
//  Global Inputs:      None
//
//  Returns:            Nothing
//
//  Notes:
//-----------------------------------------------------------------------------
static void step_Flash( void )
{
	// Invert the area
	SDK_GuiInvArea( ( RECT * ) & FlashRect );

    // Display it
	SDK_GuiDispPart( ( RECT * ) & FlashRect );
}

