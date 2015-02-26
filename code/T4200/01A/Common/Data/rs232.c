//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2003, 2004
//=============================================================================
//	Module overview: RS232 Communication
//=============================================================================
#include <osclib.h>
#include "basictyp.h"
#include "sdk.h"
#include "sdktime.h"
#include "sdkgui.h"
#include "prtdev.inc"
#include "struct.h"
#include "fixdata.h"
#include "appdata.h"
#include "comdata.h"
#include "comm.h"
#include "utllib.h"
#include "util.h"
#include "error.h"

#include "prtdev.inc"
#include "siodev.inc"
#include "sprite.h"
#include "rs232tab.h"
#include "menu.h"
#include "tabutil.h"
#ifdef MAKE_OPTIMUM
#include "persistence.h"
#endif


//=============================================================================
// Public from other modules
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

// Default RS2322 table definition
static const struct rs232_rec RsDefs=
{
	PRTP_TRANS,                                         // COM PORT PROTOCOL
    SIO_8NONE1,                                         // DATA BITS, PARITY, STOP BITS
	SIO_B57600,                                         // PORT BAUD RATE
	RSOPT_RSACTIVE,                                     // OPTION BYTE 1
	PRTP_CAT33,                                         // PIN PORT PROTOCOL
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },             // Bluetooth Hub Addr
	{ 0x11, 0x01 },                                     // Bluetooth UUID
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }   // RESERVED
};


//=============================================================================
// Private defines and typedefs
//=============================================================================



//-----------------------------------------------------------------------------
//! \brief Configurates RS232 communication
//! 
//! \return
//!     1 - Cancel or Clear pressed else 0.
//-----------------------------------------------------------------------------
UBYTE RS232CFG( void )
{
	Bool loopflag;
	UBYTE	keyval, stop, data, parity;
	UBYTE chRetval;


//===================================================================
//
//      Step 1 - select LINE/COM1/COM2
//

	ClearMenus ();

	/* Draw RS232 protocol select screen */
	DrawMenu( RS232_CHOOSE_MODEM_RS );

	// Determine which button should be displayed inverted 
	if ( RSTAB.RSOPT & RSOPT_RSACTIVE ) 
    {
        if ( RSTAB.RSOPT & RSOPT_COMNUM )
            MenuItemCounter = 2;  // COMMS=COM2, PIN=COM1
        else
            MenuItemCounter = 1;  // COMMS=COM1, PIN=COM2
    }
    else
        MenuItemCounter = 0;  // COMMS=LINE, PIN=COM1

	// Wait for a key press or timeout
	do
	{
		loopflag = False;

		switch ( GetOneKey( DEV_KBD | DEV_PAD, UserTimeout ) )
		{
		case CANCEL_KY :
		case CLEAR_KY  :
            // Return "Cancel or Clear was pressed"
            chRetval = 1;
			return chRetval;

		case ENTER_KY  :
			break;

		case '0': // COMMS=LINE, PIN=COM1
			RSTAB.RSOPT &= ~RSOPT_RSACTIVE;
			RSTAB.RSOPT |= RSOPT_COMNUM;
			SetRSTabLRC();
			break;

		case '1':  // COMMS=COM1, PIN=COM2
			RSTAB.RSOPT |= RSOPT_RSACTIVE;
			RSTAB.RSOPT &= ~RSOPT_COMNUM;
			SetRSTabLRC();
			break;

		case '2': //  COMMS=COM2, PIN=COM1
            RSTAB.RSOPT |= RSOPT_RSACTIVE;
            RSTAB.RSOPT |= RSOPT_COMNUM;
            SetRSTabLRC();
			break;

		default:
			// Bad key, error beep
			SDK_Beeper( TENMS * 40 );
			loopflag = True;
			break;
		}
	}
	while ( loopflag );

/*
//===================================================================
//
//      Step 2 - Select COMM protocol: PosMiniNAK/Transparent/CAT33
//

    // Do it for COM1/COM2 only
	if ( RSTAB.RSOPT & RSOPT_RSACTIVE ) 
    {

        ClearMenus ();

        // Draw RS232 protocol select screen
        DrawMenu( RS232_PROT_MENU );

        // Determine which button should be displayed inverted 
        switch ( RSTAB.RSCOMPROTOCOL )
        {
            case PRTP_POSMINI: 
                MenuItemCounter = 0; 
                break;

            case PRTP_TRANS: 
                MenuItemCounter = 1; 
                break;

            case PRTP_CAT33: 
                MenuItemCounter = 2;	
                break;

            default:  // Wrong entry -> to default TRANSPARENT
                RSTAB.RSCOMPROTOCOL = PRTP_TRANS;
                SetRSTabLRC();
                MenuItemCounter = 1;
                break;
        }

        // Wait for a key press or timeout
        do
        {
            loopflag = False;

            keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );
            switch ( keyval )
            {
                case CANCEL_KY :
                case CLEAR_KY  :
                    // Return "Cancel or Clear was pressed"
                    chRetval = 1;
                    return chRetval;

                case ENTER_KY  :
                    break;

                case   PRTP_TRANS:
                case PRTP_POSMINI:
                case   PRTP_CAT33:
                    // Set Protocol
                    RSTAB.RSCOMPROTOCOL = keyval;
                    // Recalculate RS232 Table LRC
                    SetRSTabLRC();
                    break;

                default:
                    // Bad key, error beep
                    SDK_Beeper( TENMS * 40 );
                    loopflag = True;
                    break;
            }
        }
        while ( loopflag );

    }
*/

//===================================================================
//
//      Step 3 - Select Bit Rate
//
    // Check if POS LAN or BLUETOOTH or COM1/COM2 used
    if ( ( RSTAB.RSOPT & RSOPT_RSACTIVE ) )
    {
	
    ClearMenus ();

	// Draw RS232 baud rate select screen
	DrawMenu( RS232_BAUD_MENU );

    // Determine which button should be displayed inverted 
    if ( (RSTAB.RSOPT & RSOPT_RSACTIVE ) )
    {
        switch ( RSTAB.RSSPEED )
        {
            case SIO_B1200: 
                MenuItemCounter = 0; 
                break;

            case SIO_B2400: 
                MenuItemCounter = 1; 
                break;

            case SIO_B4800: 
                MenuItemCounter = 2; 
                break;

            case SIO_B9600: 
                MenuItemCounter = 3; 
                break;

            case SIO_B19200: 
                MenuItemCounter = 4; 
                break;

            case SIO_B38400: 
                MenuItemCounter = 5; 
                break;

            case SIO_B57600: 
                MenuItemCounter = 6; 
                break;

            case SIO_B115200: 
                MenuItemCounter = 7; 
                break;

            default: // Unknown - force to 115200
                RSTAB.RSSPEED = SIO_B115200; 
                SetRSTabLRC();
                MenuItemCounter = 7;
                break;
        }
    }

	// Wait for a key press or timeout
	do
	{
		loopflag = False;

		keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );

		switch ( keyval )
		{
		case CANCEL_KY :
		case CLEAR_KY  :
            // Return "Cancel or Clear was pressed"
            chRetval = 1;
			return chRetval;

		case ENTER_KY  :
			break;

		case B_NEXT:
			DrawMenu( MENU_NEXT );
			loopflag = True;
			break;

		case B_PREVIOUS:
			DrawMenu( MENU_PREVIOUS );
			loopflag = True;
			break;

		case SIO_B300:
		case SIO_B1200:
		case SIO_B2400:
		case SIO_B9600:
		case SIO_B19200:
			/* Set Baud Rate */
			RSTAB.RSSPEED = keyval;
            SetRSTabLRC();
			break;

        case 'D':
			RSTAB.RSSPEED = SIO_B4800;
            SetRSTabLRC();
			break;

		case 'A': 
			RSTAB.RSSPEED = SIO_B38400;
            SetRSTabLRC();
			break;

		case 'B':
			RSTAB.RSSPEED = SIO_B57600;
            SetRSTabLRC();
			break;

		case 'C':
			RSTAB.RSSPEED = SIO_B115200;
            SetRSTabLRC();
			break;

		default:
			// Bad key, error beep
			SDK_Beeper( TENMS * 40 );
			loopflag = True;
			break;
		}
	}
	while ( loopflag );

    }

    // Check if  COM1/COM2 used
    if ( RSTAB.RSOPT & RSOPT_RSACTIVE )
    {

//===================================================================
//
//      Step 4 - Select Data bits 7/8
//

	// Set current parameters value
	data   = RSTAB.RSDPS & SIO_BIT_MASK;
	parity = RSTAB.RSDPS & (SIO_PMASK);
	stop   = RSTAB.RSDPS & SIO_STOP_MASK;


	ClearMenus ();

	/* Draw RS232 data bits select screen */
	DrawMenu( RS232_DATA_MENU );

	// Determine which button should be displayed inverted 
	switch ( data )
	{
		case SIO_8BIT: 
            MenuItemCounter = 0; 
            break;
		case SIO_7BIT: 
            MenuItemCounter = 1; 
            break;
		default:	   
            MenuItemCounter = 0;
	}

	// Wait for a key press or timeout
	do
	{
		loopflag = False;

		keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );
		switch ( keyval )
		{
		case CANCEL_KY :
		case CLEAR_KY  :
            // Return "Cancel or Clear was pressed"
            chRetval = 1;
			return chRetval;

		case ENTER_KY  :
			break;

		case SIO_8BIT:
		case SIO_7BIT:
			/* Set data bits */
			data = keyval;
			RSTAB.RSDPS = data + stop + parity;
			SetRSTabLRC();
			break;
		default:
			// Bad key, error beep
			SDK_Beeper( TENMS * 40 );
			loopflag = True;
			break;
		}
	}
	while ( loopflag );

//===================================================================
//
//      Step 5 - Select Parity None/Even/Odd
//

	ClearMenus ();

	/* Draw RS232 parity select screen */
	DrawMenu( RS232_PARITY_MENU );

	// Determine which button should be displayed inverted 
	switch ( parity )
	{
		case SIO_PNONE: 
            MenuItemCounter = 0; 
            break;
		case  SIO_PODD: 
            MenuItemCounter = 1; 
            break;
		case SIO_PEVEN: 
            MenuItemCounter = 2; 
            break;
		default:				 
            MenuItemCounter = 0;
	}

	// Wait for a key press or timeout
	do
	{
		loopflag = False;

		keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );
		switch ( keyval )
		{
		case CANCEL_KY :
		case CLEAR_KY  :
            // Return "Cancel or Clear was pressed"
            chRetval = 1;
			return chRetval;

		case ENTER_KY  :
			break;
		case (SIO_PNONE | 0x80):
		case  SIO_PODD:
		case SIO_PEVEN:
			/* Set parity */
			parity = keyval & 0x7F;
			RSTAB.RSDPS = data + stop + parity;
			SetRSTabLRC();
			break;
		default:
			// Bad key, error beep
			SDK_Beeper( TENMS * 40 );
			loopflag = True;
			break;
		}
	}
	while ( loopflag );

//===================================================================
//
//      Step 6 - Select Stop Bits 1/2
//

	ClearMenus ();

	/* Draw RS232 stop bits select screen */
	DrawMenu( RS232_STOP_MENU );
	
	// Determine which button should be displayed inverted 
	switch ( stop )
	{
		case STOP1: 
            MenuItemCounter = 0; 
            break;
		case STOP2: 
            MenuItemCounter = 1; 
            break;
		default:	
            MenuItemCounter = 0;
	}

	// Wait for a key press or timeout
	do
	{
		loopflag = False;

		keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );
		switch ( keyval )
		{
		case CANCEL_KY :
		case CLEAR_KY  :
            // Return "Cancel or Clear was pressed"
            chRetval = 1;
			return chRetval;

		case ENTER_KY  :
			break;

		case STOP1:
		case STOP2:
			/* Set stop bits */
			stop = keyval;
			RSTAB.RSDPS = data + stop + parity;
			SetRSTabLRC();
			break;

		default:
			// Bad key, error beep
			SDK_Beeper( TENMS * 40 );
			loopflag = True;
			break;
		}
	}
	while ( loopflag );

//===================================================================
//
//      Step 7 - Select Flow Control
//

/*              NOT SUPPORTED AT THE MOMENT
	ClearMenus ();

	// Draw COMMS: Flow Control
	DrawMenu( RS232_FLOWC_MENU );

	// Determine which button should be displayed inverted 
	if ( RSTAB.RSOPT & RSOPT_FC )
	{
        if ( RSTAB.RSOPT & RSOPT_FC_RXTX )
            MenuItemCounter = 2;    // RX/TX
        else
            MenuItemCounter = 1;    // RX
	}
    else
    {
        MenuItemCounter = 0;    // None
    }

	// Wait for a key press or timeout
	do
	{
		loopflag = False;

		keyval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );
		switch ( keyval )
		{
		case CANCEL_KY :
		case CLEAR_KY  :
            // Return "Cancel or Clear was pressed"
            chRetval = 1;
			return chRetval;

		case ENTER_KY  :
			break;

		case '0':   // None
			RSTAB.RSOPT &= ~RSOPT_FC;
			SetRSTabLRC();
			break;

		case '1':   // RX
			RSTAB.RSOPT |= RSOPT_FC;
			RSTAB.RSOPT &= ~RSOPT_FC_RXTX;
			break;

		case '2':   // RX/TX
			RSTAB.RSOPT |= RSOPT_FC + RSOPT_FC_RXTX;
			SetRSTabLRC();
			break;

		default:
			// Bad key, error beep
			SDK_Beeper( TENMS * 40 );
			loopflag = True;
			break;
		}
	}
	while ( loopflag );
*/
    }


    // Return "no Cancel or Clear was pressed"
    chRetval = 0;
	return chRetval;
}


//-----------------------------------------------------------------------------
//! \brief Sets the Checksum (LRC) for the RS232 Table
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void SetRSTabLRC( void )
{
	RSLRC = CalcLrc( (UBYTE *)&RSTAB, RSSZE );
#ifdef MAKE_OPTIMUM
	StoreTable( (UBYTE *)&RSTAB );
#endif
}

//-----------------------------------------------------------------------------
//! \brief Initializes RS232 table
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
extern void TableInitRS232( void )
{
    // Format table
	InitTable( &RSFMT, ( UBYTE* )&RSTAB, RSSZE, &RSLRC );

    // Set defaults. Do not perform this for ICE5500
    memcpy( (UBYTE *)&RSTAB, (UBYTE *)&RsDefs, RSSZE );

    // Recalculate LRC
    SetRSTabLRC( );
}

