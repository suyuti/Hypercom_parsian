//=============================================================================
// Company:
//      Hypercom Inc
//
// \file
//		sprite.c
//	\brief
//		moving screen image processing
//
//=============================================================================

#include "string.h"
#include "stdlib.h"
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"
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

//=============================================================================
// External variables / declarations
//=============================================================================

//=============================================================================
// Private variables
//=============================================================================
//! "no comms" status
Bool NoCommsFlag;

//=============================================================================
// Private defines and typedefs
//=============================================================================
//! vertical sprite size
#define SPRITE_VERTICAL 	0x01
//! horizontal sprite size
#define SPRITE_HORIZONTAL 	0x02
#define SPRITE_ONEWAY		0x04
#define SPRITE_TWOWAY	    0x08


typedef struct
{
	//! determines sprite type
	UBYTE flags;
	//! starting x pixel
	UWORD startx;	
	//! starting y pixel
	UWORD starty;		
	//! step size in pixels (can be negative for left or up moving sprites )
	int step;	
	//! number of steps to end
	UBYTE stepcount;			
	//! pointer to image
	UBYTE *image; 
	//! set by SetupSprite(), image area
	RECT rect;				 
	//! maintained by SpriteStep(), current x or y pixel where image is
	UBYTE current;	   
	//! maintained by SpriteStep(), step counter
	UBYTE count;				
}
SPRITEDEF;

//! sprite table entry defintion
typedef struct
{
	//! sprite ID
	SPRITE_ID spriteID;
	//! sprite initialization function
	void (*pInitFunc)( void );	
	//! sprite step function
	void (*pStepFunc)( void ); 
	//! time period to call stprite step function
	OS_TIMES time;				
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
//! Initialize a sprite defined with SPRITEDEF
static void SetupSprite( SPRITEDEF * sprite );
//! Perform a single step for a sprite defined by SPRITEDEF
static void SpriteStep( SPRITEDEF * sprite );
//! Initialization function for moving card image
static void init_swipe_card( void );
//! Step function for moving card image
static void step_swipe_card( void );
//! Initialization function for alternating menu
static void init_alternating_menu( void );
//! step function function for alternating menu
static void step_alternating_menu( void );
//! draw text of alternating menu to screen buffer
static void draw_altmenu_text( void );
//! Clear Flash area
static void init_Flash( void );
//! Invert Flash area
static void step_Flash( void );
//! Returns pointer in SPRITE_ENTRY table for given id
static SPRITE_ENTRY *GetSpriteEntry( SPRITE_ID spriteID );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
//! Sprite table
static const SPRITE_ENTRY SpriteTable[NUM_OF_SPRITES] = {

	{SPRITE_SWIPE_CARD, init_swipe_card, 
    step_swipe_card, 20 * TENMS},

	{SPRITE_ALTERNATING_MENU, init_alternating_menu,
	 step_alternating_menu, 150 * TENMS},

	{SPRITE_FLASH, init_Flash,
	 step_Flash, 30 * TENMS},
};

#define IMG_CARD_W  10
#define IMG_CARD_H  14
#define IMG_CARD_X  (FW_W-IMG_CARD_W-1)
#define IMG_CARD_Y  (TITLE_H+3)
#define IMG_CARD_S  3
#define IMG_CARD_N  ((FW_H-IMG_CARD_Y-IMG_CARD_H)/IMG_CARD_S)

static const SPRITEDEF CardDefault =
	{ SPRITE_VERTICAL | SPRITE_TWOWAY,
      IMG_CARD_X, IMG_CARD_Y, IMG_CARD_S, IMG_CARD_N, (UBYTE *)IMG_CARD };

static SPRITEDEF Card;
//! sprite queue
static ACTIVE_SPRITE ActiveSprite[NUM_OF_SPRITES];	
//! Number of items in Queue
static UBYTE QueueCount;
static UBYTE SpriteSem;

static UBYTE altmenuToggle;
static DWORD VarPart;
//! 1=Card to down; 2=Card to up
static UBYTE CardDirection;
//! Card sprite steps
static UBYTE CardSteps;       

static const RECT menuRect = { 0, 0, SCR_PIXEL_WIDTH, TITLE_H  };
static const RECT FlashRect = { SCR_PIXEL_WIDTH-10, TITLE_H+INPUTA_H+1, 7, 1 };

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//!
//!      Draw enabled sprites
//!
//! \param
//!				
//! \return
//!     None
//!
//! \note
//!
extern void DoSprites( void )
{
	UBYTE i;

    // Get semaphore
    GetSpriteSem();

    // Loop by sprites registered on the current screen
    // Note: this code does nothing if QueueCount=0 (no sprites on the current screen)
    for ( i = 0; i < QueueCount; i++ )
	{
        // Check if the time for the step occurs
        // and the step function is available
        if ( ( ActiveSprite[i].timerFlag ) &&
             ( ActiveSprite[i].pEntry->pStepFunc ) )
		{
            // Make sure that sprites not disabled by anoter caller (DrawScreen)
            // Note, QueueCount can be cleared by GetSpriteSem
            if ( 0 != QueueCount )
            {
                // Call step function
                ActiveSprite[i].pEntry->pStepFunc();

                // Restart timer, save timer number
                ActiveSprite[i].timerId = SDK_WaitAndFlag(
                    ActiveSprite[i].pEntry->time, &ActiveSprite[i].timerFlag );
            }
		}
    }

    // Relase semaphore
    ReleaseSpriteSem();
}

//-----------------------------------------------------------------------------
//!
//!      X-Callable functions to get sprite semaphore
//!
//! \param
//!				
//! \return
//!     None
//!
//! \note
//!		Called to halt sprites on a screen. Useful when
//!     displaying an error box on a screen with sprites
//!
extern void GetSpriteSem( void )
{
	GetSem( &SpriteSem );
}

//-----------------------------------------------------------------------------
//!
//!      X-Callable functions to release sprite semaphore
//!
//! \param
//!				
//! \return
//!     None
//!
//! \note
//!
extern void ReleaseSpriteSem( void )
{
	RelSem( &SpriteSem );
}

//-----------------------------------------------------------------------------
//!
//!      Perform any initialization required for sprites
//!
//! \param
//!				
//! \return
//!     None
//!
//! \note
//!
extern void InitSprites( void )
{
	QueueCount = 0;				// start with empty que
	ReleaseSpriteSem();		// make sure sprite semaphore is released
}

//-----------------------------------------------------------------------------
//!
//!      Set active the sprite flag for later adding 
//!
//! \param
//!		spriteID			Id of added sprite
//!				
//! \return
//!     None
//!
//! \note
//!
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
		pNewSprite->pInitFunc();	// call initialization function

}

//-----------------------------------------------------------------------------
//!
//!      Clear sprite queue
//!
//! \param
//!				
//! \return
//!     None
//!
//! \note
//!
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
//!
//!      Returns pointer in SPRITE_ENTRY table for given id
//!
//! \param
//!		spriteID			Identifier number of sprite
//!				
//! \return
//!     SPRINT_ENTRY*		pointer to sprite table entry
//!
//! \note
//!
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
//!
//!      Initialization function for moving card image
//!
//! \param
//!				
//! \return
//!     None
//!
//! \note
//!
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
//!
//!      Step function for moving card image
//!
//! \param
//!				
//! \return
//!     None
//!
//! \note
//!
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
//!
//!      Initialization function for alternating menu
//!
//! \param
//!				
//! \return
//!     None
//!
//! \note
//!
static void init_alternating_menu( void )
{
	altmenuToggle = 1;
	draw_altmenu_text();
}

//-----------------------------------------------------------------------------
//!
//!      step function function for alternating menu
//!
//! \param
//!				
//! \return
//!     None
//!
//! \note
//!
static void step_alternating_menu( void )
{
	draw_altmenu_text();

    // check if no 4-th state processed
    if ( 0xFF == altmenuToggle )
    {
        // yes, call again with state 1
		altmenuToggle = 1;
        draw_altmenu_text();
    }

	if ( 4 == altmenuToggle )
		altmenuToggle = 1;
	else
		altmenuToggle++;
	
    SDK_GuiDispPart( (RECT *)&menuRect );
}

//-----------------------------------------------------------------------------
//!
//!      Initialize a sprite defined with SPRITEDEF
//!
//! \param
//!		sprite			pointer to sprite
//!				
//! \return
//!     None
//!
//! \note
//!
static void SetupSprite( SPRITEDEF * sprite )
{
	IMAGE_PARMS Iparms;

    if ( sprite->flags & SPRITE_VERTICAL )
	{
		sprite->current = (UBYTE)sprite->starty;
	}
	else
	{
		sprite->current = (UBYTE)sprite->startx;
	}

	sprite->count = 0;
	SDK_GetImageParms( sprite->image, &Iparms );
	sprite->rect.x = sprite->startx;
	sprite->rect.y = sprite->starty;
	sprite->rect.height = Iparms.Height;
	sprite->rect.width = Iparms.Width;
}

//-----------------------------------------------------------------------------
//!
//!      Perform a single step for a sprite defined by SPRITEDEF
//!
//! \param
//!		sprite			pointer to sprite
//!				
//! \return
//!     None
//!
//! \note
//!
static void SpriteStep( SPRITEDEF * sprite )
{
	RECT savevir;
	UWORD current, next;
	UBYTE flags;
    RECT update;
    UWORD delta;

	SDK_GuiGetVirWin( &savevir );
	SDK_GuiDefWindow();

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

    delta = sprite->step < 0 ? -sprite->step : sprite->step;
	if ( flags & SPRITE_VERTICAL )
	{
		sprite->rect.y = current;
		SDK_GuiClrArea( &sprite->rect );	// clear previous image area

        // updating area calculation
        memcpy( (char*)&update,(char*)&sprite->rect,sizeof(update) );
        if( sprite->step < 0 )
            update.y = next;
        update.height += delta;

        // move to next location
        sprite->rect.y = next;
	}
	else
	{
		sprite->rect.x = current;
		SDK_GuiClrArea( &sprite->rect );	// clear previous image area

        // updating area calculation
        memcpy( (char*)&update,(char*)&sprite->rect,sizeof(update) );
        if( sprite->step < 0 )
            update.x = next;
        update.width += delta;

        // move to next location
		sprite->rect.x = next;
	}

	SDK_GuiImage( sprite->rect.x, sprite->rect.y, sprite->image );
	SDK_GuiDispPart( &update );
	sprite->current = ( UBYTE ) next;

	SDK_GuiSetWindow( &savevir );
}

//-----------------------------------------------------------------------------
//!
//!      draw text of alternating menu to screen buffer
//!
//! \param
//!		Global Inputs:      altmenuToggle   1..4 draw state
//!				
//! \return
//!     None
//!
//! \note
//!		This function toggles display between date/time
//!     and CutomMsg (AltMenuStr)
//!
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
            if ( (0 == FTTAB.FTIDLEPROMPT[0]) || 
                 (!memcmp(FTTAB.FTIDLEPROMPT,"                    ",S_CSTM_IDLE)) )
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

		// If shift processing is enabled then display the
		// shift number on the display.
		if ( TCONF.TCOPT5 & TC5_SHIFT )
		{
			Dspbuf[11] = 'S';
			BfAscii( (char *)&Dspbuf[12],
					 (UBYTE *)&TFTAB.TFSHIFT, S_TRSHIFT );

			// Do not display leading zero of shift number.
			if ( '0' == Dspbuf[12] )
			{
				Dspbuf[12] = Dspbuf[13];
				Dspbuf[13] = ' ';
			}
		}
    }

//-----------------------------------------------------------------------------
//
// State 3 - "Training mode", "*** Tracing ***", "** Trace overflow"
//                            or 
//            Comm module status if it wrong
//
//-----------------------------------------------------------------------------
	if ( 3 == altmenuToggle )
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
            // Perform line status checking depended on COMM module type
            // and COMMS port used
            // Do not check COMM module status if COM1/COM2 used
            if ( (RSTAB.RSOPT & RSOPT_RSACTIVE) )
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
                // LineStatus = SDK_ComInfo ( ci_LINESTATUS );
// FIXME: don't do anything but data access in sprites and display!
                if ( IsStateSocketIdle() )
                {
                    LineStatus = -1;
                 	NoCommsFlag = False;
                }
                else
                {
	                LineStatus = GetLineStatus();
                }
            
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
	if ( ( 4 == altmenuToggle )/* && ( 0x00 == TERM.TERMInit )*/ )
    {
        // Do not check for printer if init required
        altmenuToggle = 0xFF;
        return;
    }
/*
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
*/
	// Get Font size
	SDK_GuiGetCharSize( &charsz );

	// Clear the Menu area
	SDK_GuiClrArea( (RECT *)&menuRect );

	// Calculate the offset to first text character
	LeftX =	(SCR_PIXEL_WIDTH -
		  StrLn( Dspbuf, sizeof( Dspbuf ) ) * charsz.Width) / 2;

	// Display the text
	SDK_GuiTextP( (UWORD)LeftX,(UWORD)1,Dspbuf );
}

//-----------------------------------------------------------------------------
//!
//!      Clear Flash area
//!
//! \param
//!		   
//! \return
//!     None
//!
//! \note
//!
static void init_Flash( void )
{
	// Invert the area
    SDK_GuiClrArea( (RECT *)&FlashRect );

    // Display it
	SDK_GuiDispPart( (RECT *)&FlashRect );
}

//-----------------------------------------------------------------------------
//!
//!      Invert Flash area
//!
//! \param
//!		   
//! \return
//!     None
//!
//! \note
//!
static void step_Flash( void )
{
	// Invert the area
	SDK_GuiInvArea( (RECT *)&FlashRect );

    // Display it
	SDK_GuiDispPart( (RECT *)&FlashRect );
}

