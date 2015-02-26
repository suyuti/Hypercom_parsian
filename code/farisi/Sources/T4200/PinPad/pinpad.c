//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             pinpad.c
//      Pinpad functions
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkgui.h"	 
#include "utllib.h"
#include "tlib.h"
#include "osclib.h"
#include "fixdata.h"

#include "defines.h"
#include "transdef.h"
#include "struct.h"
#include "util.h"
#include "msg.h"
#include "pinpad.h"
#include "appdata.h"
#include "comdata.h"
#include "tables.h"
#include "ddtm.h"
#include "pindev.inc"
#include "prtdev.inc"
#include "siodev.inc"
#include "scard.h"
#include "rs232tab.h"
#include "pinattck.h"
#include "cvtamt.h"
#include "cstmamnt.h"
#include "scard.h"
#include "virtualped.h"

#ifdef	MAKE_EMV
#include "emv_l2.h"
#endif	// MAKE_EMV

//=============================================================================
// External variables / declarations
//=============================================================================

// Public functions defined in this module

#ifdef MAKE_EMV
	#ifdef EMV_FN
		#undef EMV_FN
	#endif
	#define LOCAL_FN(FN) FN
#endif	// MAKE_EMV


//=============================================================================
// Private defines and typedefs
//=============================================================================
//! Pin Pad configure data
typedef struct
{
	//! Device ID
	UBYTE		device;
	//! Power mode
	PIN_CPWR 	power;
	//! Pointer to configuration parameters
	void*		cfgptr;	
} CFGDATA;

static const PINPRC	CFG_S9_CAT33_2400	= { PIN_CAT33,   PIN_TERMINAL, SIO_7EVEN1,	SIO_B2400  };
static const PINPRC	CFG_S9_CAT33_9600   = { PIN_CAT33,   PIN_TERMINAL, SIO_7EVEN1,	SIO_B9600  };
static const PINPRC	CFG_S9_CAT33_19200	= { PIN_CAT33,   PIN_TERMINAL, SIO_7EVEN1,	SIO_B19200 };

static const PINPRC	CFG_S9_POSMINI_2400	 = { PIN_POSMINI, PIN_TERMINAL, SIO_8NONE1,	SIO_B2400  };
static const PINPRC	CFG_S9_POSMINI_9600	 = { PIN_POSMINI, PIN_TERMINAL, SIO_8NONE1,	SIO_B9600  };
static const PINPRC	CFG_S9_POSMINI_19200 = { PIN_POSMINI, PIN_TERMINAL, SIO_8NONE1,	SIO_B19200 };

//! List Pin Pad types
static const CFGDATA ProtList[]	=
{
	{ PIN, PIN_CPWR_OFF, (void*)&CFG_S9_POSMINI_19200 },
	{ PIN, PIN_CPWR_OFF, (void*)&CFG_S9_CAT33_2400  },
	{   0, PIN_CPWR_OFF, NULL                  }
};

// Current PINPad port configuration
static CFGDATA CurrentConfig;

//! response length
static int PINRXLEN;
//! Pin Pad status
static int Status;

//=============================================================================
// Private function declarations
//=============================================================================
//! Opens and configure Pin Pad Port
//static Bool  Open_Config( void );
//! Gets device configuration mode
//static char  GetPINDevice( void );
//! Gets message separator
static void  GetCommand( char* );
//! Gets device configuration mode
static UBYTE GetMode( void );
//! Gets field separator
static void  GetFS( void );
//! Gets encryption working key 
static void  GetEncKey( UBYTE*, short, Bool );
//! Gets Pin prompt
static UBYTE *GetPinPrompt( UBYTE *pDest );
//! Retrieves a message from PIN table, and store it in a user buffer
static void  GetPinMsg( enum pinm_id, char* );
//! Clears PINBUF, OUTPIN and initialize pointer into OUTPIN
static void  PreparePinCalc( void );
//! Starts a timer to detect a response timeout. Flags bit 0 of 'flag' upon expiration.
static void  StartRspTimer( PTCS_BLK*, UBYTE*, unsigned short );
//! Adds data to end of OUTPIN
static void  GetPINData( char * pData, int lenght );
//! Adds char to end of OUTPIN
static void  GetPINByte( char cBYTE );
//! Gets PIN Block from PINBUF and SAVE  to TRINP.TRPINBL as ASCII
static void  SaveASCIPINBlock( void );
//! Gets Serial number from PINBUF and SAVE  to TRINP.TRKSN
static void  GetKSN_PINBLOCK( UBYTE *buffer, int len, UBYTE *ksn, UBYTE* pinblock );
//! Clears external PINPAD
static void  ClearPinPad( void );
//! Writes content of OUTPIN to Pin Pad
static OSRETURNCODES Write_Pin( UWORD );
//! Reads message from Pin Pad
static OSRETURNCODES Read_Pin( OS_TIMES timeout, Bool bCheckCancel );
//! Closes Pin Pad
static Bool Close_Pin( void );
//! 
static UBYTE *MovePANForPIN( UBYTE *pBcdPan, UBYTE *pDest, UBYTE MaxLength, Bool bRemLast, Bool bSaveBCD );

//! Enter and encrypt PIN for online verification using the external PED
static Bool PinVerifyOnline( PINONLTYPE pintype, UBYTE* trpan, int panLength, unsigned char* ksn,
                             int ksn_size, unsigned char key_id, int doubleKey, unsigned char* workingKey,
                             UBYTE *dest, UBYTE *retStatus );

// Enter and encrypt PIN for online verification using the internal PED
static Bool VPP_Online( VPP_ActionType action, UBYTE* trpan, int panLength, unsigned char* ksn,
			  int ksn_size, unsigned char key_id, int doubleKey, unsigned char* workingKey, UBYTE *status );

#ifdef MAKE_EMV
//! Sends a Cancel command to pinpad
static Bool  PinCancel( void );
//! Wrires or reads Pin Pad
static UBYTE Write_Read_Pin( UWORD, unsigned short, Bool );
//! Performs an APDU exchange with the Smart Card Device
static OSRETURNCODES PinScrCmd( UBYTE*, UWORD, UBYTE, UBYTE, UBYTE*, UWORD* );
//! Gets an ATR from the selected device
static OSRETURNCODES PinScrATR( UBYTE*, UWORD*, char*, char*, UBYTE );
//! Gets the card insertion status
static OSRETURNCODES PinCardStatus( UBYTE, UBYTE* );
//! Gets the card insertion status
static OSRETURNCODES PinCardStatusIdle( UBYTE, UBYTE* );
//! Power off the card and display message to remove the card then loop until card is removed.
static OSRETURNCODES PinEject( UBYTE );
//! Load the RSA public key used for offline PIN encryption
static OSRETURNCODES PinLoadPublicKey( UBYTE *key, UWORD keylen );
//! Scan for a crypto processor in the pinpad.
static OSRETURNCODES  PinInitCrypto( UBYTE *slot, char *version );
//! Perform EMV Offline PIN verification using the external pinpad
static Bool PinVerifyOffLine( PINOFFLTYPE type, UBYTE* publicKey, UBYTE publicKeyLength, UBYTE tries, UBYTE* retStatus, UBYTE *sw1sw2 ); 
//! EMV PIN verification
static Bool VPP_OffLine( VPP_EMV_ActionType type, UBYTE* publicKey, UBYTE publicKeyLength, UBYTE tries, UBYTE* retStatus, UBYTE *sw1sw2 );
#endif // MAKE_EMV

//! Display up to 4 lines of text in the PED
extern UBYTE PinDisplayText( char *line1, char *line2, char *line3, char *line4 );

//=============================================================================
// Public data definitions
//=============================================================================

PEDVERREC PEDInfo;

//! Input Pin Pad buffer
UBYTE PINBUF[S_PINBUF];

//! Pointer to output buffer
UBYTE *pOUTPIN;
//! Counter input data
UWORD pinlen;
//! Pin Pad status
IOSTATUS PinStatus;
UBYTE length;
//! Work data
UBYTE WorkData[8];
//! PAN buffer
UBYTE FormatPanBuf[10];
//! PIN block
UBYTE pinblock[13];

//! Power on flag
UBYTE PowerOnFlag;
//=============================================================================
// Private data definitions
//=============================================================================

//! structure for YesNoEntry()
static const ENTRY_STR YNEntry =
{
	N_Pinpad,
	CustomMsg,
	CorrectYesNo,
};

//! Pin entry from keybord
static const ENTRY_STR PinEntry = {
	//! Title
	N_Pinpad,					
	//! Prompt
	CustomMsg,	
	//! Default value
	EnterPin,					
};

//=============================================================================

//=============================================================================
// Public function declaration
//=============================================================================

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Gets field separator.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//-----------------------------------------------------------------------------
static void  GetFS( void )
{
	*( pOUTPIN ) = '\x1C';
	pOUTPIN++;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets Pin Prompt.
//!
//! \param
//!		pDest  pointer to the destination message
//!
//! \return
//!     pointer to the first byte in the destination buffer after the prompt.
//!
//-----------------------------------------------------------------------------
static UBYTE * GetPinPrompt( UBYTE *pDest )
{
	UBYTE length;
	
	// Clear string
	memset( CSTMSG2, ' ', sizeof( CSTMSG2 ) );

	if ( TRINP.TRPINRETRY )
	{
		// Get the "REENTER PIN" prompt, force field to 16 chars )
		GetMsg( Msg_Reenter_Pin, CSTMSG3 );
		length = StrLn( CSTMSG3, 16 );
		memcpy( CSTMSG2, CSTMSG3, length );
	}
	else
	{
		GetMsg( Msg_Pin, CSTMSG3 );
		length = StrLn( CSTMSG3, 4 );
		memcpy( CSTMSG2, CSTMSG3, 4 );

		// If balance inquiry do not display an amount on pin request.
		if ( BALINQ != CurrentEvent.TRNKEY )
		{
			CvtAmt( ( char * ) &CSTMSG2[3], TRINP.TRTOTAM );
		}
	}

	memcpy( pDest, CSTMSG2, 16);
	pDest += 16;

    return pDest;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Retrieves a message from PIN table, and store it in a user buffer.
//!
//! \param
//!		Mid		message index in the PIN table.
//!		pBuf	pBuf buffer to return the message.
//!
//! \return
//!     None
//!
//! \note   Uses MLSemaphore.
//-----------------------------------------------------------------------------
static void GetPinMsg( enum pinm_id Mid, char *pBuf )
{

	struct MLGetMsg_str MLGetMessage;

	// Get MultiLingual Semaphore
	GetSem( &MLSemaphore );

	// Set Table and Index of message to retrieve.
	MLGetMessage.tblid = pintext_table_id;
	MLGetMessage.msgid = Mid;

	// Get the message from the tables.
	TEXTGETSTRING ( &MLGetMessage );

	// Save message in application buffer.
	memcpy( pBuf, MLBuffer,
			( UWORD ) ( StrLn( MLBuffer, sizeof( MLBuffer ) ) + 1 ) );

	// Release MultiLingual Semaphore
	RelSem( &MLSemaphore );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Clears PINBUF, OUTPIN and initialize pointer into OUTPIN.
//!
//! \param
//!		None
//!
//! \return
//!     None
//!
//-----------------------------------------------------------------------------
static void  PreparePinCalc( void )
{
	memset( ( UBYTE * ) PINBUF, 0, S_PINBUF );
	pOUTPIN = PINBUF;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Add data to end of OUTPIN.
//!
//! \param
//!		pData		pointer to data
//!		lenght		lenght of data
//!				
//! \return
//!     None
//!
//! \note   
//!
//-----------------------------------------------------------------------------
static void  GetPINData( char * pData, int lenght )
{
	memcpy( pOUTPIN, pData, lenght );
	pOUTPIN += lenght;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Add char to end of OUTPIN.
//!
//! \param
//!		cBYTE		pointer to data
//!				
//! \return
//!     None
//!
//! \note   
//!
//-----------------------------------------------------------------------------
static void  GetPINByte( char cBYTE )
{
        *pOUTPIN = cBYTE;
	pOUTPIN++;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Format the PAN in accordance to the external PED requirements
//!
//! \param
//!     pDest           pointer to the destination buffer
//!		MaxLength		max lenght of PAN (GET LAST digits (LSB) )
//!		bRemLast		remove last digit (TRUE - remove, false do not remove)
//!		bSaveBCD		save in BCD format (TRUE - save in BCD , False - save in ASCI)
//!				
//! \return
//!     pointer to the first location after the PAN in the destination bufer
//!
//! \note   
//!
//-----------------------------------------------------------------------------
static UBYTE *MovePANForPIN( UBYTE *pBcdPan, UBYTE *pDest, UBYTE MaxLength, Bool bRemLast, Bool bSaveBCD )
{
    char ascii_pan[20];
    int panlen;

    // Clear the ascii pan buffer
    memset( ascii_pan, 0, sizeof( ascii_pan ) );

	// Move PAN to output and convert to ASCII. (Max 18 bytes)
	MovPanAsc( &ascii_pan[1], pBcdPan, 19 );
    panlen = StrLn( &ascii_pan[1], sizeof( ascii_pan ) + 1 );
    // Check the PAN format to be used
    if( bSaveBCD )
    {
        if( bRemLast ) panlen--;
        // Check if the pan length is odd
        if( panlen % 2 )
        {
            ascii_pan[0] = 0x30;
            AscHex( pDest, ascii_pan, ( panlen/2 ) + 1 );
            pDest += ( panlen/2 ) + 1;
        }
        else
        {
            AscHex( pDest, &ascii_pan[1], panlen/2 );
            pDest += panlen/2;
        }
    }
    else // Saving in ASCII
    {
        if (bRemLast) panlen--;	
        memcpy( pDest, &ascii_pan[1], panlen );
        pDest += panlen;
    }
    return pDest;
}

//-----------------------------------------------------------------------------
//  SaveSerialNumber      Get Serial number from PINBUF and SAVE  to TRINP.TRKSN // @AAMELIN
//
//  Parameters:     None
//
//  Returns:        None
//-----------------------------------------------------------------------------
static void  GetKSN_PINBLOCK( UBYTE *buffer, int len, UBYTE *ksn, UBYTE* pinblock )
{
	int ksn_len;
    char tmpbuf[20];

    // First, fill the buffer with 'F's
    memset( tmpbuf, 'F', sizeof( tmpbuf ) );
    // The returned KSN length is the length of the data portion minus the pinblock size
    ksn_len = len - 16;
    // padd the KSN with 'F's from the left
    memcpy( &tmpbuf[20 - ksn_len], buffer, ksn_len );
    // Now we have a nice ascii buffer, convert it to hex
    AscHex( ksn, tmpbuf, 10 );
    // Let's now get the pinblock
    AscHex( pinblock, (char *)( buffer + ksn_len ), 8 );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Clear external PINPAD.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note   
//!
//-----------------------------------------------------------------------------
static void ClearPinPad( void )
{
	// Clear PINBUF, OUTPIN and initialize pointer into OUTPIN.
	PreparePinCalc();
	GetCommand("3C");
	if ( orvOK == Write_Pin( ( UWORD ) ( pOUTPIN - PINBUF ) ) )
	{
		Read_Pin(ONESECOND, False);
	}
	else
	{
//		Close_Pin(  );
	}	
}

//-----------------------------------------------------------------------------
//! \brief
//!     Writes contents of OUTPIN to Pin Pad.
//!
//! \param
//!		length		length of data that must be in OUTPIN
//!				
//! \return
//!     True if successful, False if error
//!
//! \note
//!
//-----------------------------------------------------------------------------
static OSRETURNCODES Write_Pin( UWORD length )
{
        OSRETURNCODES Retval = orvDEVICEFAULT;

 	if ( PIN_TYPE_INTERNAL == PIN_TYPE )
	{
		/* Internal pinpad */
		Retval = orvOK;
	}
	else
 	{
		if ( orvOK != (Retval = OS_DeviceWrite( PIN_DEVICE, PINBUF, length )) )
		{ 
			Rewind_Pin( );
		}
	}
	return(Retval);
}

//-----------------------------------------------------------------------------
//! \brief
//!     Reads message from Pin Pad.
//!
//! \param
//!		timeout 		timeout value
//!		bCheckCancel	check Cancel
//!				
//! \return
//!     True if successful, False if error
//!
//! \note
//!		Data is returned in global buffer OUTPIN
//!
//-----------------------------------------------------------------------------
static OSRETURNCODES Read_Pin( OS_TIMES timeout, Bool bCheckCancel )
{
	OSRETURNCODES Retval = orvDEVICEFAULT;
	UBYTE readflag;
	UBYTE InputType;
	Bool bPinCardStatus;
	Bool bKeyWasOpen;

    bKeyWasOpen = DevFlag | DEV_KBD;
	pinlen = 0;

	if ( PIN_TYPE_INTERNAL == PIN_TYPE )
	{
		// Internal T5000 pinpad
	}
	else
	{
		/* Post a read to the pin device */
		if ( orvOK != DeviceRead (PIN_DEVICE, &PINBLKR, PROCEED+FLAG, PINBUF, sizeof(PINBUF), &readflag, 7 ) )
		{
		}
		else
		{
			/* Start response timer */
			StartRspTimer( &TIM_01, &readflag, timeout );

			/* Wait for timeout (bit 0) or read complete (bit 7) */
			while( ! ( readflag & 0x01 ) )
			{
				if( bCheckCancel )
				{
					InputType = read_io( DEV_PAD + DEV_KBD , 0, &bPinCardStatus, &DevFlag );	// Wait for input
					switch ( InputType )
					{
						case DEV_PAD:
							// Check for CANCEL Button press
							if ( CANCEL_KY == ButtonId ) KeyVal = CANCEL_KY;	// Yes; exit the loop
							break;

						case DEV_KBD:
							// Check for CANCEL_KY Key
							if ( CLEAR_KY == KeyVal ) KeyVal = CANCEL_KY;	// Yes; exit the loop
							break;
					}

					if ( KeyVal == CANCEL_KY ) 
					{
   	                	ClearPinPad();
						break;
					}
				}
				if ( ( readflag & 0x80 ) && ( OK == PINBLKR.iocs_stati ) )
				{
					pinlen = PINBLKR.iocs_var.iocs_xfer.iocs_tlenx;
					Retval = orvOK ;
					break;
				}

				// Give up some cycles while waiting...
				SDK_RelinqCPU(  );
			}	// while()

			if (bCheckCancel)
				// Force a close of still open pad and kbd devices.
				read_io( 0, 0, &bPinCardStatus, &DevFlag );

			/* Cancel response timeout timer */
			OS_CancelTimer ( &TIM_01 );
		}

		if ( orvOK != Retval )
		{
			Rewind_Pin();
		}
	}

	return ( Retval );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Closes Pin Pad.
//!
//! \param
//!		None
//!				
//! \return
//!     True if successful, False if error
//!
//! \note
//!
//-----------------------------------------------------------------------------
static Bool Close_Pin( void )
{
	Bool retval;

	if (!PINFLG.deviceOpen)
	{
		/* Device already closed */
		return( True );
	}

	retval = False ;

	if ( PIN_TYPE_INTERNAL != PIN_TYPE )
	{
		if ( orvOK == OS_DeviceClose( PIN_DEVICE ) )
		{
			PINFLG.deviceOpen = 0;
			retval = True;
		}
	}
	else
	{
		// Internal T5000 pinpad.
		retval =  True ;
	}

	return ( retval );
}

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Initialize the pin pad.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note   Calls PinReset() which will set PinRetval to True if OK, False otherwise
//-----------------------------------------------------------------------------
void PinInit( void )
{
	Bool DetectFlag;
    UBYTE slot;

	/* Get PIN Pad Detect Flag */
	DetectFlag = (( TCONF.TCOPT1 & TC1_PINDETECT ) || ( TCONF.TCPINPAD & TCP_SMCARD ));

	// Force pin type to invalid.
	PIN_TYPE = PIN_TYPE_INVALID;

	// Clear all bits in PINSTAT
	PINSTAT = 0;
	// Reset PIN device
	PIN_DEVICE = 0;

	// Reset the Pin Pad.

    // Check detect flag
	if ( DetectFlag )
	{
        if( PinAutoDetect() )
        {
            // If we got an S9S or SC, try to detect the HCP
            if( PIN_TYPE == PIN_TYPE_S9SC )
            {
                PinInitCrypto( &slot, NULL );
            }

            if (TCONF.TCPINPAD & TCP_SMCARD)
            {
                // Send a "Ready" message
                PinMessage( Msg_PinReady );
            }
            else
            {
                // Clear PIN Pad display
                PinMessage( Msg_ClrScrn );
            }


            if ( (TCONF.TCPINPAD & TCP_SMCARD) && 
                 (PIN_CAT33 != *((UBYTE*)PIN_CONFIG)) && 
                 (PIN_TYPE != PIN_TYPE_INTERNAL) )
            {
                // Set the flag Smart Card Read from Pinpad
                PINSTAT |= PinSmartCard_Read;
            }
        }
	}
//	Rewind_Pin( );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Resets the pin pad.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//-----------------------------------------------------------------------------
void PinReset( void )
{
//	Rewind_Pin( );

	// Reset Bits of PINSTAT
	PINSTAT &= ~PinCardReadReqPend;
	PINSTAT &= ~PinWaitUserAction;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets Encryption working Key.
//!
//! \param
//!		keyptr		pointer to encrypted key
//!		len			key length
//!		dblflag		double length key flag
//!				
//! \return
//!     None
//!
//! \note   Puts the Key into output buffer
//-----------------------------------------------------------------------------
void GetEncKey( UBYTE* keyptr, short len, Bool dblflag )
{
	Bool  empty;
	UBYTE tmpaqbuffer[S_AQWORKKEY + S_AQWORKKEY];

	/* Copy key from aquirer table */
	MoveItAq( tmpaqbuffer, keyptr, len );

    // Init. empty flag
	empty = True;

    // Check conditions
	if ( !NullComp( ( char * ) tmpaqbuffer, len ) )
	{
		if ( dblflag )
		{
			if ((S_AQWORKKEY + S_AQWORKKEY) == len)	empty = False;
		}
		else
		{
			if (S_AQWORKKEY == len) empty = False;
		}
	}

	// If key is all zeros then send all spaces.
	if ( empty ) 
	{
		if ( PIN_CAT33 == *((UBYTE*)PIN_CONFIG) )
		{
			memset( pOUTPIN, ' ', ( len << 1 ) );
			pOUTPIN += ( len << 1 );
		}
		else
		{
			memset( pOUTPIN, 0, len );
			pOUTPIN += len;
		}
	}
	else
	{
		if ( PIN_CAT33 == *((UBYTE*)PIN_CONFIG) )
		{
			BfAscii( ( char * ) pOUTPIN, tmpaqbuffer, len );
			pOUTPIN += ( len << 1 );
		}
		else
		{
			memcpy( pOUTPIN, tmpaqbuffer, len );
			pOUTPIN += len;
		}
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Starts a timer to detect a response timeout. Flags bit 0 of 'flag' upon expiration.
//!
//! \param
//!		blk			pointer to PTCS block
//!		flag		pointer to flag to be used
//!		timeout		timeout
//!				
//! \return
//!     None
//!
//-----------------------------------------------------------------------------
void StartRspTimer( PTCS_BLK *blk, UBYTE *flag, unsigned short timeout )
{
    blk->ptcs_cmnd = TIMER;		        // Request a PTCS timer service
    blk->ptcs_scmd = PROCEED + FLAG;	// Select Proceed+Flag option
    blk->ptcs_time = timeout;			// Set timer timeout value
    blk->ptcs_ifad.pubyte = flag;		// Pointer to the flag to be used
    blk->ptcs_fbit = 0;		            // Flag to bit 0

  OS_Function ( RQPTCS, blk );		    // Call the OS service handler	
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets the PIN from the pin pad.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note   True if OK, False otherwise. The S7 will retransmit both packets upon
//!         CLEAR Key, S8 does not retransmit.      
//-----------------------------------------------------------------------------
Bool PinGetPin( void )
{
	UBYTE retStatus;
	Bool DUKPTflag, bRetVal;
    VPP_ActionType action;

	// May need to add a Get PIN Semaphore in the future

    bRetVal = False;

	// Set DUKPT flag
	DUKPTflag = (TRINP.TRAQPTR->AQOPT4) & AQ4_DUKPT;

	    if ( PIN_TYPE_INTERNAL != PIN_TYPE )
        {
            // Display PINPAD in title bar.
            GetMsg( N_Pinpad, Titlebuf );
            User_Msgs[1] = WaitForPin;

            // ???
            Enable_Flags[0] = 0;
            DrawScreen( STATUS_SCR );

            // Remove PINPAD from title buffer.
            Titlebuf[0] = 0;

            bRetVal = PinVerifyOnline( ( DUKPTflag )?PINTYPE_DUKPT:PINTYPE_MASTER, TRINP.TRPAN, MAXPANL, TRINP.TRKSN,
                              S_TRKSN, TRINP.TRAQPTR->AQMKID, (TRINP.TRAQPTR->AQOPT4) & AQ4_DBLPINKEY ? 16 : 8,
                              TRINP.TRAQPTR->AQWORKKEY, TRINP.TRPINBL, &retStatus );

		}
		else // Virtual Pinpad Detected
		{
			// Display TOTAL and amount on line 1.
			if ( TRINP.TRKEY != BALINQ )
				CustomAmount( AmountTotals, TRINP.TRTOTAM );
			else
				CSTMSG[0] = '\0';

            action = DUKPTflag?DUKPT:MASTER_SESSION;

            // Request an Online PIN entry
            bRetVal = VPP_Online( action, TRINP.TRPAN, MAXPANL, TRINP.TRKSN, S_TRKSN, TRINP.TRAQPTR->AQMKID,
								(TRINP.TRAQPTR->AQOPT4) & AQ4_DBLPINKEY ? S_AQWORKKEY*2 : S_AQWORKKEY,
								TRINP.TRAQPTR->AQWORKKEY, &retStatus );

		}
        if( retStatus == EMVL2_PIN_ENTERED )
        {
            // Set PIN Block ready flag 
            TRINP.TRSTATUS[1] |= TS2_PINBLOCK;
            bRetVal = True;
        }
        else
        {
            bRetVal = False;
        }
        // May need to add a Release PIN Semaphore
        return bRetVal;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Builds message and send it to the Pin Pad.
//!
//! \param
//!		None
//!				
//! \return
//!     True if OK, False otherwise.
//!
//! \note   Message to display in PinMsgNumber.  
//!
//-----------------------------------------------------------------------------
Bool PinMessage( enum msg_id PinMsgNumber )
{
	Bool bRetVal;
	// May need to add a Get PIN Semaphore in the future
	UBYTE MaxDigits;
	bRetVal = False;

		if ( PIN_TYPE_INTERNAL != PIN_TYPE )
		{
			// Load the Message Type - Display Text Request
			PreparePinCalc();
            MaxDigits = 16;
            GetCommand( "3C" );

     		// blank the non-message lines - always send max
			memset (pOUTPIN, ' ', MaxDigits);
			// Load text message into buffer after message type ( 16 Bytes)
			GetPinMsg( PinMsgNumber, CSTMSG );
			length = StrLn( CSTMSG, 16 );
			memcpy( pOUTPIN, CSTMSG, ( UWORD ) length );
                        pOUTPIN += MaxDigits;

			// Write message to Pin Pad
			if ( orvOK == Write_Pin( ( UWORD ) (pOUTPIN - PINBUF ) ) )
			{
				bRetVal = True;
			}
		}
		else
		{
			// Internal pinpad.
			// Spoof it for now just like SPOS.
			bRetVal = True;
		}
//	}

	// May need to add a Release PIN Semaphore
	return bRetVal;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Displayes version information received from Pin PAd.
//!
//! \param
//!		None
//!				
//! \return
//!     True if OK, False otherwise.
//!
//-----------------------------------------------------------------------------
Bool PinDisplayDiag( void )
{
	UBYTE retval;
	char LoopFlag = True;
	Bool bRetVal;
    char version[16];

	// May need to add a Get PIN Semaphore in the future

	bRetVal = False;

		// Set version screen Title
		GetMsg( N_Version, CSTMSG );
		// Display text line 1
		Enable_Flags[3] = 1;

		if ( PIN_TYPE_INTERNAL != PIN_TYPE )
		{
            // Load the Revision text into the buffer "EXTERNAL: "
			GetMsg( Msg_External, CSTMSG2 );
            length = StrLn( CSTMSG2, 19 );

            if( PinGetVersion( &PEDInfo ) )
            {
                // Get revision and load it into display buffer
                strcpy( (char *)&CSTMSG2[length], (const char *)PEDInfo.PEDTYPE );
				length = length + 10;
				CSTMSG2[length] = 0;

                // Load the Version text Text "VERSION"
				GetMsg( N_Version, CSTMSG3 );
				length = StrLn( CSTMSG3, 19 );
                // Add an space
                CSTMSG3[length] = ' ';
				// Get checksum and load it into display buffer
				memcpy( &CSTMSG3[length + 1], PEDInfo.SECVER, 10 );
                length = StrLn( CSTMSG3, 19 );
				CSTMSG3[length] = 0;

                bRetVal = True;
            }
            else
            {
                ShowErrMsg( PinpadNotFound );
            }
		}
		else
		{
            // Test the internal PIN PAD
			// Request version from the Internal Pinpad
			length = VPPGetVersion( version, sizeof( version ) );
			// If get something - display it
			if ( length > 0 )
			{
				bRetVal = True;

				// Load the Revision text into the buffer "INTERNAL: "
				GetMsg( Msg_Internal, CSTMSG2 );
				MoveIt( CSTMSG3, version, length );
				CSTMSG3[length] = 0;
			}
			else
			{
				ShowErrMsg( PinpadNotFound );
			}
		}
        // May need to add a Release PIN Semaphore

        if ( True == bRetVal )
        {
            // Enable QUIT button
            Enable_Flags[4] = 1;

            // Display Screen
            DrawScreen( TEST_SCR );

            // Wait until EXIT button pressed
            while ( LoopFlag )
            {
                // Get Keyboard Entry
                retval = GetOneKey( DEV_KBD | DEV_PAD, UserTimeout );
                // Exit if timeout, 'x' or Exit pressed
                if ( ( CLEAR_KY == retval ) || 
                     ( SELECT_KY == retval ) ||
                     ( 'C' == retval ) ||
                     ( 0 == retval ) || 
                     ( CANCEL_KY == retval ) ||
                     ( ENTER_KY == retval ) )
                    LoopFlag = False;
            }
        }

        return bRetVal;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets Card Read from the external Pin Pad.
//!
//! \param
//!		None
//!				
//! \return
//!     True if OK, False otherwise.
//!
//-----------------------------------------------------------------------------
Bool PinCardRead( void )
{
	Bool bRetVal;

	// May need to add a Get PIN Semaphore in the future

	bRetVal = False;
	pinlen = S_PINBUF;

	// Open and configure Pin Pad Port
//	if ( Open_Config(  ) )
//	{
		// Pinpad card read is only valid with external pinpad.
		if ( PIN_TYPE_INTERNAL != PIN_TYPE )
		{
			// Clear PINBUF, OUTPIN and initialize pointer into OUTPIN.
			PreparePinCalc();

			// Load the Message Type - Read Card Request
			GetCommand( "3A" );

			// Set read track one in buffer
			*( pOUTPIN ) = '1';
			pOUTPIN++;

			// Set read track two in buffer
			*( pOUTPIN ) = '1';
			pOUTPIN++;

            // Set read ICC in buffer
            if ( TCONF.TCPINPAD & TCP_SMCARD )
            {
                *( pOUTPIN ) = '1';
            }
            else
            {
                *( pOUTPIN ) = '0';
            }
			pOUTPIN++;

			// Skip over two reserved postions.
			pOUTPIN += 2;

			// Load prompt to be displayed.
			memset( ( char * ) pOUTPIN, ' ', 16 );
			pOUTPIN += 16;

			// Set file separator character in buffer
			GetFS();

			// Load prompt to be displayed.
			GetMsg( Msg_SwipeYourCard, CSTMSG );
			length = StrLn( CSTMSG, 16 );
			memcpy( pOUTPIN, CSTMSG, ( UWORD ) length );
			if ( 16 == length )
			{
				pOUTPIN += 16;
			}
			else
			{
				// Pad the field
				pOUTPIN += length;
				memset( ( char * ) pOUTPIN, ' ', ( UWORD ) ( 16 - length ) );
				pOUTPIN += ( 16 - length );
			}

			// Write message to Pin Pad
			if ( orvOK == Write_Pin( ( UWORD ) ( pOUTPIN - PINBUF ) ) )
			{
				// Write message was successful, now start read.
				// Note that read has no timeout value.
				if ( orvOK == Read_Pin( ONESECOND * 45, True ) )
				{
					// Set status showing read is outstanding.
					PINSTAT |= PinCardReadReqPend;
					bRetVal = True;
				}
				else
				{
					// Error messages while testing.
					ShowErrMsg( PinReadErr );
				}
			}

			// Close Pin Pad Port if an error occurred.
//			if ( !bRetVal )
//			{
//				Close_Pin(  );
//			}
		}
//	}

	// May need to add a Release PIN Semaphore

	return bRetVal;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Process card data read from the pin pad.
//!
//! \param
//!		None
//!				
//! \return
//!     True if OK, False otherwise.
//!
//-----------------------------------------------------------------------------
extern Bool PinCardReadResp( void )
{
	UBYTE i, j;
	Bool bRetVal;

	bRetVal = False;

	// Don't process unless card was read on external pinpad.
	if ( PINSTAT & PinCardReadReqPend )
	{
		// Clear the outstanding read flag.
		PINSTAT &= ~PinCardReadReqPend;

		// Remove parity bits from pin pad data.
		Parity( PINBUF, ( UWORD ) strlen( ( char * ) PINBUF ) );
		if ( !memcmp( PINBUF, ( UBYTE * ) "3B", 2 ) )
		{
			// Received correct response.
			// If track one received OK copy to TRINP.TRTRK1.
			if ( '0' == PINBUF[3] )
			{
				// Move data to TRINP.TRTRK1.
				// Start just past first sentinel.
				// At exit of for loop i will point to end sentinel.
				for ( i = 9, j = 0; j < MAXTRK1; i++ )
				{
					if ( ( 0x1C == PINBUF[i] ) || ( '?' == PINBUF[i] ) )
					{
						// End of track 1 data has been found.
						break;
					}
					else
					{
						if ( '%' != PINBUF[i] )
						{
							TRINP.TRTRK1[j] = PINBUF[i];
							j++;
						}
					}
				}

				Trk1ReadLength = j;

				// FF remainder of TRINP.TRTRK1
				for ( j = j; j < S_TRTRK1; j++ )
				{
					TRINP.TRTRK1[j] = 0xFF;
				}
			}
			else
			{
				// Be sure TRK1 data is cleared of any trash data.
				// This field will be tested later if issurer option
				// indicates TRK1 is required.
				memset( TRINP.TRTRK1, 0, S_TRTRK1 );
				Trk1ReadLength = 0;

				// Locate starting position for track 2.
				for ( i = 9, j = 0; j < MAXTRK1; i++ )
				{
					if ( 0x1C == PINBUF[i] )
					{
						// Get out of loop pointing to sentinel.
						break;
					}
					else
					{
						j++;
					}
				}

				// Verify that file separator was found.
				if ( 0x1C != PINBUF[i] )
					return bRetVal;
			}

			// If track two received OK copy to TRINP.TRTRK2.
			if ( '0' == PINBUF[4] )
			{
				// Increment past end marker of TRK1.
				i++;

				// Move data to TRINP.TRTRK2.
				// Start just past first sentinel.
				// At exit of for loop i will point to end sentinel.
				for ( j = 0; j < MAXTRK2; i++ )
				{
					if ( ( 0x1C == PINBUF[i] ) || ( '?' == PINBUF[i] ) )
					{
						// End of track 2 data has been found.
						break;
					}
					else
					{
						if ( ';' != PINBUF[i] )
						{
							TRINP.TRTRK2[j] = PINBUF[i];
							j++;
						}
					}
				}

				Trk2ReadLength = j;

				// FF remainder of TRINP.TRTRK2
				for ( j = j; j < S_TRTRK2; j++ )
				{
					TRINP.TRTRK2[j] = 0xFF;
				}

				// Only return true if track 2 is OK.
				bRetVal = True;
			}
		}
//		Close_Pin(  );
	}

	return bRetVal;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Set the pinpad communication mode
//!
//! \param
//!		Desired mode
//!				
//! \return
//!     True - success, False - error
//!
//! \note
//!
//-----------------------------------------------------------------------------
extern Bool PinSetMode( PIN_MODE mode )
{
    unsigned char Retval = False ;
    void *port_mode;
    IOCS_DDDA_DATA ddda;
    UBYTE *pbuf;
    UBYTE status; 

    // May need to add a Get PIN Semaphore in the future

     // Open and configure Pin Pad Port 
//     if( Open_Config () )
//     {
         // Load Message Type "92." - Set Mode
         memcpy( (char*)PINBUF, "92.", 3 );
         pbuf += 3 ;

         // Selecty the desired mode string
         switch( mode )
         {
                case PIN_CAT33_2400:
                {
                    memcpy( &PINBUF[3], "00", 2 );
                    port_mode = (void *)&CFG_S9_CAT33_2400;
                    break;
                }

                case PIN_CAT33_9600:
                {
                    memcpy( &PINBUF[3], "05", 2 );
                    port_mode = (void *)&CFG_S9_CAT33_9600;
                    break;
                }
                case PIN_CAT33_19200:
                {
                    memcpy( &PINBUF[3], "06", 2 );
                    port_mode = (void *)&CFG_S9_CAT33_19200;
                    break;
                }
                case PIN_POSMINI_2400:
                {
                    memcpy( &PINBUF[3], "20", 2 );
                    port_mode = (void *)&CFG_S9_POSMINI_2400;
                    break;
                }

                case PIN_POSMINI_9600:
                {
                    memcpy( &PINBUF[3], "21", 2 );
                    port_mode = (void *)&CFG_S9_POSMINI_9600;
                    break;
                }
                case PIN_POSMINI_19200:
                {
                    memcpy( &PINBUF[3], "12", 2 );
                    port_mode = (void *)&CFG_S9_POSMINI_19200;
                    break;
                }
                default:
                {
                    memcpy( &PINBUF[3], "12", 2 );
                    port_mode = (void *)&CFG_S9_POSMINI_19200;
                    break;
                }
         }
         // Write message to Pin Pad
         if( Write_Read_Pin( 5, 4*MS1000, 0 ) == OK )
         {
                // Check if we got a valid response
                if ( PINBUF[0] == '9' && PINBUF[1] == '3' ) 
                { 
                    // We successfully switched the mode, let's switch the PIN port
                    // Close the device
                    OS_DeviceClose( PIN_DEVICE );
                    PINFLG.deviceOpen = 0;

                    // Try to open device
                    status = OS_DeviceOpen( PIN_DEVICE );
                    if ( OK == status ) PINFLG.deviceOpen = 1;
                    // Device opened successfully, set config parameters pointer
                    PIN_CONFIG = port_mode;
                    // Configure device 
                    ddda.pubyte = PIN_CONFIG;
                    status = OS_DeviceConfig ( PIN_DEVICE, GetMode(), &ddda );
                    Retval = True ;
                }
         }
//         // Close Pin Pad Port
//         if( !Close_Pin() )
//         {
//             Retval = False ;
//         }
//     }
   return ( Retval );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Returns device configuration mode.
//!
//! \param
//!		None
//!				
//! \return
//!     True - success, False - error
//!
//! \note   Set PIN_DEVICE & PIN_CONFIG variables.
//!
//-----------------------------------------------------------------------------
extern Bool PinAutoDetect( void )
{
	IOCS_DDDA_DATA ddda;
	OSRETURNCODES status;
	UBYTE i;
    char version[16];
    int length;

        // Prepare progress buffer
	    CSTMSG2[0] = '<';
        memset( &CSTMSG2[1], ' ', sizeof(ProtList)/sizeof(CFGDATA) - 1 );
        CSTMSG2[sizeof(ProtList)/sizeof(CFGDATA)] = '>';
        CSTMSG2[sizeof(ProtList)/sizeof(CFGDATA) + 1] = 0;

	 	// Display PIN AUTO DETECTING message.
		GetMsg( PinPadAutoDetect, CSTMSG );
		ShowInfoMsg( CustomMsg2, N_Pinpad );
 
		for ( i = 0; ProtList[i].device != 0; i++ )
		{
			// Set device 
			PIN_DEVICE = ProtList[i].device;

			// Check communication
			if ( !(RSTAB.RSOPT & RSOPT_RSACTIVE) || !(PIN_DEVICE == PRT) )
			{
				// Close the device
				if ( OK == OS_DeviceClose( PIN_DEVICE )) PINFLG.deviceOpen = 0;

				// Try to open device
				status = OS_DeviceOpen( PIN_DEVICE );
				if ( OK == status ) PINFLG.deviceOpen = 1;
                // Device opened successfully, set config parameters pointer
				PIN_CONFIG = ProtList[i].cfgptr;

				// Configure device 
				ddda.pubyte = PIN_CONFIG;
				status = OS_DeviceConfig ( PIN_DEVICE, GetMode(), &ddda );

				// PINPAD just powered up
				if ( ProtList[i].power == PIN_CPWR_ON ) OS_Wait( ONESECOND*3 );

				// Check config status
				if ( OK == status )
				{
					// Try to talk to the External pinpad
					if ( PinGetVersion( &PEDInfo ) )
					{
                        if( 0 == memcmp( PEDInfo.PEDTYPE, ( UBYTE * ) "S9SC", 4 ) )
                        {
                            // Set S9SC PINPAD TYPE
							PIN_TYPE = PIN_TYPE_S9SC;							
						}
						else if( 0 == memcmp( PEDInfo.PEDTYPE, ( UBYTE * ) "S9", 2 ) )
						{
                            // Set S9SC PINPAD TYPE
							PIN_TYPE = PIN_TYPE_S9;
						}
						else
						{
                            // Set external PINPAD type
							PIN_TYPE = PIN_TYPE_INVALID;							
						}
/*						// Set power mode
						if ( PowerOnFlag != 0xAA )
						{
                            OS_Wait( MS100*2 );
							ddda.data = ProtList[i].power;
							OS_DeviceConfig ( PIN_DEVICE, PIN_CONST_PWR, &ddda );
							OS_Wait( MS100*2 );
							PowerOnFlag = 0xAA ;
                        }
*/
						// Close the device
//						if ( OK == OS_DeviceClose( PIN_DEVICE )) PINFLG.deviceOpen = 0;
						// Change detect progress
						memset( &CSTMSG2[1], '-', sizeof(ProtList)/sizeof(CFGDATA) - 1 );
						CSTMSG2[i + 1] = '*';
						ShowInfoMsg( CustomMsg2, N_Pinpad );
						OS_Wait( ONESECOND*1 );
						// Receive correct response code, exit
                        // Now, if we detected the device at PIN_CAT33_*, switch is to PIN_POSMINI_19200
                        return  PinSetMode( PIN_POSMINI_19200 );
//						return True;
					}
				}
				// Close the device 
				if ( OK == OS_DeviceClose( PIN_DEVICE )) PINFLG.deviceOpen = 0;
			}

			// Clear PIN device
			PIN_DEVICE = 0;

			/* Change detect progress */
			CSTMSG2[i + 1] = '-';
			ShowInfoMsg( CustomMsg2, N_Pinpad );
		}

		// Failed to detect an external pinpad, test the internal PIN PAD
		length = VPPGetVersion( version, sizeof( version ) );
        // If get something, means the internal pinpad is alive
		if ( length > 0 )
		{
 			// Set PIN device
			PIN_DEVICE = PIN;

 			// Internal pinpad exists.
 			PIN_TYPE = PIN_TYPE_INTERNAL;
 		}
 		else
 		{
		   	// Force pin type to invalid.
			PIN_TYPE = PIN_TYPE_INVALID;

 			// No external and no internal pinopad detected
 			ShowErrMsg( PinpadNotFound );
			
			// Exit 
			return False;
 		}
        return True;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets message separator.
//!
//! \param
//!		cmd		command string
//!				
//! \return
//!     None
//!
//! \note   Set proper message separator in the buffer
//!		   '.' - for CAT33 protocol
//!		   '/' - for POSMINI protocol
//!
//-----------------------------------------------------------------------------
void GetCommand( char* cmd )
{
	memcpy( pOUTPIN, cmd, 2 );
	pOUTPIN += 2;
	
	if ( PIN_CAT33 == *((UBYTE*)PIN_CONFIG) || ( PINBUF[0] == 'A' ) ) 
		*pOUTPIN = '.';
	else 
		*pOUTPIN = '/';

	pOUTPIN++;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets device configuration mode.
//!
//! \param
//!		None
//!				
//! \return
//!     PIN_CFG_PROT for PIN device, PRT_CFG_PROT for PRT device
//!
//! \note
//!
//-----------------------------------------------------------------------------
UBYTE GetMode( void )
{
	/* Set device mode */
	if ( PIN == PIN_DEVICE ) return PIN_CFG_PROT;
	else					 return PRT_CFG_PROT;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Rewinds PIN device.
//!
//! \param
//!		None
//!				
//! \return
//!     None
//!
//! \note
//!
//-----------------------------------------------------------------------------
void Rewind_Pin( void )
{
	OS_DeviceRewind( PIN_DEVICE );
//	OS_DeviceClose( PIN_DEVICE );

	/* Clear 'Device Open' flag */
//	PINFLG.deviceOpen = 0;
}

#if defined(MAKE_ISOMAC) 
//-----------------------------------------------------------------------------
//! \brief
//!     MAC the message.
//!
//! \param
//!		PinMacId		master key index to be used - ASCII
//!		pMacKeyPtr		pointer to MAC key
//!		pMacDataPtr		pointer to data to be MAC'ed
//!		MacDataLength	length of data to be MAC'ed
//!		PinData			page where data resides that is to be MAC'ed
//! 
//!				
//! \return
//!     True - success, False - error. PinData -  Result of MAC.
//!
//! \note   This function is cross callable.
//!         This function only for master session encryption.
//!			If internal ICE pinpad is being used
//!         this function will use format that works on ver 1.17.
//!         For internal the master keys are stored encrypted in
//!         the KEK table external to the Z8.
//-----------------------------------------------------------------------------
extern Bool PinGenMac( UBYTE PinMacId, UBYTE *pMacKeyPtr, UBYTE *pMacDataPtr, UWORD MacDataLength, UBYTE *PinData )
{
	UBYTE mkid;
	UWORD mac_data_size;
	Bool loop_flag, first_block;
	UBYTE *pFirst;
	UWORD data_remaining;
	UBYTE mac_binary[8];
	Bool bRetVal;

	// May need to add a Get PIN Semaphore in the future

	bRetVal = False;
	first_block = True;

	// Don't do anything unless data and a length are present.
	if ( ( 0 == MacDataLength ) || ( NULL == pMacDataPtr ) )
		return bRetVal;

	// Open and configure Pin Pad Port
//	if ( Open_Config(  ) )
//	{
        if ( PIN_TYPE_INTERNAL != PIN_TYPE )
		{
//-----------------------------------------------------------------------------
//                  EXTERNAL PIN PAD OPERATION        
//-----------------------------------------------------------------------------
			// External pinpad MAC.
			// MAC the message pointed to by pMacDataPtr.
			// Length to be mac'ed is in MacDataLength.

			// Clear PINBUF, OUTPIN and initialize pointer into OUTPIN.
			PreparePinCalc();

			// Initialize local copy of data length
			data_remaining = MacDataLength;

			// Load the Message Type - MAC Generation Request
			GetCommand( "86" );

			// Mac id from acquirer table is passed in.  Default to one.
			if ( ((PinMacId < '0') || (PinMacId > '9')) && (PinMacId != 'A') && (PinMacId != 'B') )
				// If null, blank or ascii 0 then default to 1
				*pOUTPIN = '0';
			else
				// Master key id is ASCII in the acquirer table.
				*pOUTPIN = PinMacId;
			pOUTPIN++;

			// Move encrypted session mac key to output buffer.
			GetEncKey( pMacKeyPtr, S_AQMACKEY, (TRINP.TRAQPTR->AQOPT4) & AQ4_DBLMACKEY );

			// Data input part of message.
			pFirst = pOUTPIN;

			// Set loop control to stay in loop till last response rec'd.
			loop_flag = True;

			/* Clear start data */
			memset( PinData, 0, 8 );

			do
			{
				if ( data_remaining > 8 )
				{
					mac_data_size = 8;
					data_remaining -= 8;
				}
				else
				{
					memset( mac_binary, 0, sizeof( mac_binary ) );
					mac_data_size = data_remaining;
					data_remaining = 0;
				}

				// Move the data, first, intermediate or last.
				// Must move from across page.
				SDK_ReadPage( mac_binary, mac_data_size, 11, pMacDataPtr );
				
				// First block flag
				*pOUTPIN = first_block ? '1' : '0';
				pOUTPIN++;
				first_block = False;

				// Last block flag
				*pOUTPIN = data_remaining ? '0' : '1';
				pOUTPIN++;
				
				/* Check message format */
				if ( PIN_CAT33 == *((UBYTE*)PIN_CONFIG) )
				{
					BfAscii( ( char * ) pOUTPIN, mac_binary, 8 );
					pOUTPIN += ( 8 * 2 );
				}
				else
				{
					memcpy( pOUTPIN, mac_binary, 8 );
					pOUTPIN += 8;
				}

				// Check for double length flag
				if ((TRINP.TRAQPTR->AQOPT4) & AQ4_DBLMACKEY)
				{
					// Set FS
					GetFS();

					// Move double length key
					GetEncKey( pMacKeyPtr, S_AQMACKEY + S_AQMACKEY, (TRINP.TRAQPTR->AQOPT4) & AQ4_DBLMACKEY );
				}
				
				// Write message to Pin Pad
				if ( orvOK != Write_Pin( ( UWORD ) ( pOUTPIN - PINBUF ) ) )
					loop_flag = False;

                if ( True == loop_flag )
				{
					// Data input part of message.
					pOUTPIN = pFirst;

					// Increment pointer to next group of 8 bytes.
					pMacDataPtr += 8;
				}

			}
			while ( ( 0 != data_remaining ) && ( True == loop_flag ) );


			bRetVal = False;

            // Receive final response
            if ( True == loop_flag )
            {
                // Read PinPad response
                if ( orvOK == Read_Pin( ONESECOND * 5, False ) )
                {   
                    // Check response type (87 expected)  
                    if ( !memcmp( PINBUF, ( UBYTE * ) "87", 2 ) )
					{
						// Check message format
						if ( PIN_CAT33 == *((UBYTE*)PIN_CONFIG) )
						{
							// Extract data from the message.
							AscHex( PinData, ( char * ) &PINBUF[4], 4 );
                            bRetVal = True;
						}
						else
						{
							memcpy( PinData, &PINBUF[4], 8 );
                            bRetVal = True;
						}
					}
				}
            }

			// Close Pin Pad Port
//			if ( !Close_Pin(  ) )
//				bRetVal = False;
		}
//-----------------------------------------------------------------------------
//                  INTERNAL PIN PAD OPERATION        
//-----------------------------------------------------------------------------
		else
		{
			// Set working key - binary value
			if ( ((PinMacId < '0') || (PinMacId > '9')) && (PinMacId != 'A') && (PinMacId != 'B') )
				mkid = 0x00;
			else 
				mkid = Binary( ( char ) PinMacId );

			// Check for DUKPT MAC calculation
			if( ( TRINP.TRAQPTR->AQOPT4 ) & AQ4_DUKPTMAC )
			{
//-----------------------------------------------------------------------------
//                  INTERNAL DUKPT SESSION        
//-----------------------------------------------------------------------------
					// Open CryptoService
					CS_Open( False );

					// Check DUKPT Key ID
					if (mkid > 2) mkid = 0;

					// Calculate DUKPT MAC
					switch( CS_CreateMAC_DUKPT( mkid, PINBUF, pMacDataPtr, MacDataLength, 0 ) )
					{
						case    1: 
							// Mac calculation successful
							bRetVal = True;
							break;	// OK
						case 1002:			// DUKPT key not generated
						default:
							bRetVal = False;
							ShowErrMsg( EncryptError );
					}

					if (bRetVal)
					{
						// Copy MAC
						memcpy( PinData, PINBUF, 8 );
						
						// Copy KSN
						memcpy( TRINP.TRKSN, &PINBUF[8], S_TRKSN );
					}

			}
//-----------------------------------------------------------------------------
//                  INTERNAL MASTER SESSION        
//-----------------------------------------------------------------------------
			else
			{
				// Prepare key buffer
				if ( pMacKeyPtr != NULL )
				{
					// Check for double length key
					if ((TRINP.TRAQPTR->AQOPT4) & AQ4_DBLMACKEY)
					{
						MoveItAq( PINBUF, pMacKeyPtr, S_AQWORKKEY + S_AQWORKKEY );
					
					}
					else
					{
						MoveItAq( PINBUF, pMacKeyPtr, S_AQWORKKEY );
						MoveItAq( &PINBUF[S_AQWORKKEY], pMacKeyPtr, S_AQWORKKEY );
					}
					
					// Open CryptoService
					CS_Open( False );

					// Calculate MAC
					if ( 1 == CS_CreateMAC( mkid, PINBUF, S_AQWORKKEY<<1, PinData, pMacDataPtr, MacDataLength ) )
					{
						// Mac calculation successful
						bRetVal = True;
					}
				}
			}
		}
//	}

	return bRetVal;
}
#endif // MAKE_ISOMAC


//-----------------------------------------------------------------------------
//! \brief
//!     Prompt for PIN and get the encrypted PIN block
//!
//! \param
//!		type		Type of PIN encryption, 1=DUKPT, 0=Master Session
//!		trpan		pointer to PAN
//!		panLength	PAN length
//!		ksn			pointer to Key Serial Number (for DUKPT only)
//!		ksn_size	KSN size (for DUKPT only) 
//!		key_id		Master Key ID in the key collection
//!		doubleKey	working key length (8,16,24)
//!		workingKey	pointer to working session key
//!     status      pointer to the status to be returned
//!				
//! \return
//!		True if success, False otherwise
//!
//! \note
//!
//-----------------------------------------------------------------------------
static Bool PinVerifyOnline( PINONLTYPE pintype, UBYTE* trpan, int panLength, unsigned char* ksn,
                             int ksn_size, unsigned char key_id, int doubleKey, unsigned char* workingKey,
                             UBYTE *dest, UBYTE *retStatus )
{
    UBYTE *pPINBUF;
    Bool bRetVal = False;
    OSRETURNCODES status;
    char promptline[17] = {0};

        // Set the pointer to the communication buffer
        pPINBUF = PINBUF;

        if( pintype == PINTYPE_MASTER )
        {
            // Build a Master/Session PIN Entry request message
            memcpy( pPINBUF, "2032/", 5 ); // Load the Extended message type 32
            pPINBUF += 5;

            // Copy master key id from acquirer table.
            *pPINBUF++ = key_id;
            // Move PAN formatted accordingly
            pPINBUF = MovePANForPIN( trpan, pPINBUF, 18, True, True );
            *pPINBUF++ = 0x1C;

            // Move the key size indicator
            *pPINBUF++ = ( doubleKey == 16 )?'2':'1';

            // Move encrypted session key to output buffer.
            memcpy( pPINBUF, workingKey, doubleKey );
            pPINBUF += doubleKey;

            // Get pin prompt. Either 'PIN?   $9.99' or 'REENTER PIN'
            pPINBUF = GetPinPrompt( pPINBUF );

            memcpy( pPINBUF, "12", 2 ); // Send Max. PIN Digits (12 digits)
            pPINBUF += 2;
            *pPINBUF++ = '0'; // Send Enter Key required flag (Enter key is requered)
            *pPINBUF++ = 0x1C; // Move Field Setarator

            memcpy( pPINBUF, "                ", 16 ); //  Send display data, line 2
            pPINBUF += 16;
            *pPINBUF++ = 0x1C; // Move Field Setarator

            memcpy( pPINBUF, "00", 2 );// TimeOut
            pPINBUF += 2;
        }
        else
        {
            // First, let's display the PIN prompt
            GetPinPrompt( (UBYTE *)promptline );
            PinDisplayText( promptline, NULL, NULL, NULL );
            
            // Build a DUKPT PIN Entry request message
            memcpy( pPINBUF, "70", 2 ); // Move command code "70" Get DUKP PIN
            pPINBUF += 2;

            // Move PAN formatted accordingly
            pPINBUF = MovePANForPIN( trpan, pPINBUF, 19, False, False );
            *pPINBUF++ = 0x1C;

            // Move Debit/Credit indicator
            *pPINBUF++ = 'D';

            // Move the transaction amount
            BfAscii( ( char * ) pPINBUF, TRINP.TRTOTAM, S_TRTOTAM );
            pPINBUF += 12;
        }
        // The message is built, let's try to send it to the PED
        // set default as pin pad failure (no response)
        *retStatus = EMVL2_PINPAD_FAILURE ;
        // The terminal waits for pin entry for 60 seconds (must be <= timeout on pinpad)
        status =  Write_Read_Pin( (UWORD)( pPINBUF - PINBUF ), 60*MS1000, True );
        if ( orvOK == status )
        {
            // Check if a Master/Session response
            if( PINBUF[0] == '3' && PINBUF[1] == '3' )
            {
                switch( PINBUF[4] )
                {
                case '0':
                    {
                        // PIN has been entered, retirve the encrypted PIN block
                        memcpy( dest, &PINBUF[7], 8 );
                        *retStatus = EMVL2_PIN_ENTERED;
                        bRetVal = True;
                        break;
                    }
                case '5':
                default:
                    {
                        // Cancelled by user
                        *retStatus = EMVL2_ONLINE_CANCELLED;
                        bRetVal = True;
                        break;
                    }
                }
            }
            // Check if a Master/Session response
            if( PINBUF[0] == '7' && PINBUF[1] == '1' )
            {
                switch( PINBUF[4] )
                {
                case '0':
                    {
                        // PIN has been entered, retrieve KSN and the encrypted PIN block
                        GetKSN_PINBLOCK( &PINBUF[3], PINRXLEN - 3, ksn, dest );
                        *retStatus = EMVL2_PIN_ENTERED;
                        bRetVal = True;
                        break;
                    }
                case '5':
                default:
                    {
                        // Cancelled by user
                        *retStatus = EMVL2_ONLINE_CANCELLED;
                        bRetVal = True;
                        break;
                    }
                }
            }
            else
            {
                // Do something if other response
                // Pinpad failure will do for now
                *retStatus = EMVL2_PINPAD_FAILURE;
                bRetVal = True;
            }
        }
        // Write_Read_Pin returned !=orvOK
        else if ( 2 == status )
        {
            // Transaction cancelled (the name of the flag is a bit misleading...)
            *retStatus = EMVL2_ONLINE_CANCELLED;
            bRetVal = True;
        }
        else if ( 3 == status )
        {
            // PIN entry bypassed
            *retStatus = EMVL2_PIN_BYPASSED;
            bRetVal = True;
        }
        else
        {
            // Pinpad failure
            *retStatus = EMVL2_PINPAD_FAILURE;
        }

        return bRetVal;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Build an offline Pine request and send to Pin Pad.
//!
//! \param
//!		PinType		Pin type (1 plain, 2 encrypted (Sim1), 3 encrypted (Sim2) )
//!				
//! \return
//!     None
//!
//! \note	PinRetval set to True if OK, False otherwise.
//!         Message to display in PinMsgNumber.
//!
//-----------------------------------------------------------------------------
static Bool PinVerifyOffline( PINOFFLTYPE type, UBYTE* publicKey, UBYTE publicKeyLength, UBYTE tries, UBYTE* retStatus, UBYTE *sw1sw2 )
{
    UBYTE Bypass;
    UBYTE status;
    UBYTE count;
    UBYTE *pbuf;

	    // Setup PIN Bypass capability (check TNMS flag)
	    // It is 'Bypass key on Pinpad' (not on the terminal)
	    Bypass   = (TCONF.TCOPT6 & TC6_PINBYPASSPIN) ? '2' : '1';
        // May need to add a Get PIN Semaphore in the future

        // Load the Message Type - Display Text Request

		memcpy( PINBUF, "62/", 3 );
		PINBUF [3] = type ;
		PINBUF [4] = '1';		// Language (English)
		PINBUF [5] = Bypass ;

		pbuf = &PINBUF[6];

        // Additional fields for S9S & P1100 pinpads
		// (Display Data rows 3 & 4)
		memset( pbuf, ' ', 32 );
		// 'Pin Entry'
		*(pbuf+32) = '1';
		// Update the pointer
		pbuf += 33;

		// Get Amount and insert along with currency character
		memset( CSTMSG, ' ', 16 );
		CvtAmt( ( char * ) &CSTMSG[3], TRINP.TRTOTAM );
		// now need to convert the currency symbol to 0x7F for the PINPAD
		// as the 0x7F char has been set up on PinInit for the external pinpad
		// and I assume that if we start searching from the end of string
		// forward then the currency symbol is the one after the last space
		// (I could look for currency symbol, but though it was easier to look
		// for a space char)
		// "TOTAL" to display buffer but do not
		// overlay anything but spaces.
		GetMsg( AmountTotal, CSTMSG2 );
		for ( count = 0; count < 5; count++ )
		{
			if ( ' ' == CSTMSG[count] )
			{
				CSTMSG[count] = CSTMSG2[count];
			}
		}

		memcpy ( pbuf, CSTMSG, 16);
        pbuf += 16;

		*pbuf++ = '3';				// display timeout
		*pbuf++ = 'F';				// PIN entry timeout
		*pbuf++ = 'F';				// PIN entry timeout
	
		// set default as pin pad failure (no response)
		*retStatus = EMVL2_PINPAD_FAILURE ;
		// The terminal waits for pin entry for 60 seconds (must be <= timeout on pinpad)
		status =  Write_Read_Pin( (UWORD)( pbuf - PINBUF ), 60*MS1000, True );
		if ( orvOK == status )
		{
                // Get the last ICC status words.
                memcpy ( sw1sw2, &PINBUF[5], 2 );
                /*
                  00 - Good
                  01 - User Reset
                  02 - General error
                  03 - Bypass
                  04 - Cancel key
                  05 - ICC has returned SW1 SW2 != 90 00
                  06 - SIM has returned SW1 SW2 != 90 00
                  10 - Customer card not present
                  11 - HTC not present
                */
          		  if (PINBUF[3] == '0')  // byte0 = 0
          		  {
                      switch (PINBUF[4]){
          				case '0':
          					*retStatus  = EMVL2_PIN_OK ;
          					break ;
          				case '1':
          				case '2':
          					*retStatus  = EMVL2_PINICC_FAIL ;
          					break ;
          				case '3':
          					*retStatus  = EMVL2_PIN_BYPASSED ;
          					break ;
          				case '4':
							*retStatus = EMVL2_ONLINE_CANCELLED;
          					break ;
          				case '5':
							// need to determine if a 6983 or 6984 was returned as we have to return EMVL2_PIN_BLocked
							// otherwise return EMVL2_PINICC_FAIL
       						*retStatus  = EMVL2_PINICC_FAIL ;
							if( PINBUF[5] == 0x63 )
							{
								if( PINBUF[6] == 0xC0 )
	          						*retStatus = EMVL2_PIN_BLOCKED ; // overwrite status
							}
							else if( PINBUF[5] == 0x69 )
							  { // this should not happen 
								if ( ( PINBUF[6] == 0x83 ) || ( PINBUF[6] == 0x84 ) )
	          						*retStatus = EMVL2_PIN_BLOCKED ; // overwrite status
							  }

          					break ;
          				case '6':
          					*retStatus = EMVL2_PINSIM_FAIL ;
          					break ;
                      } // end switch
          		   }
          		   else
				   {
						if (PINBUF[4] == '0') // customer card error or not present
          					*retStatus = EMVL2_PINICC_FAIL ;
						else
						if (PINBUF[4] == '1') // HTC error or not present
          					*retStatus = EMVL2_PINSIM_FAIL ;
				   } // end byte0 = 1
		}
		else if ( 2 == status )
		{
			// Transaction cancelled (the name of the flag is a bit misleading...)
            *retStatus = EMVL2_ONLINE_CANCELLED;
		}
		else if ( 3 == status )
		{
			// PIN entry bypassed
			*retStatus = EMVL2_PIN_BYPASSED;
		}
		else
		{
			// Pinpad failure or timeout waiting for pin
			if ( OK == PinCancel( ) )
			{
				// Pinpad is alive, timed out waiting for pin. Acquirers requested to handle this as 'txn cancelled'
				*retStatus = EMVL2_ONLINE_CANCELLED;
			}
			else
			{
                // Pinpad failure
                *retStatus = EMVL2_PINPAD_FAILURE;
            }
		}
	// May need to add a Release PIN Semaphore
}

#ifdef	MAKE_EMV
//-----------------------------------------------------------------------------
//! \brief
//!     Load the RSA public key usde for offline PIN encryption
//!
//! \param
//!		publicKey			ICC public key
//!		publicKeyLength		ICC public key length
//!				
//! \return
//!		OSRETURNCODES, orvOK if key successfully loaded
//!
//! \note
//!		The public key format is 'eekkk..' where e is the exponent
//!		and 'kk..' the public key. Public key length is the total
//!		length of key + exponent.
//!		Possible exponent values are: 
//!		02 = 2
//!		03 = 3 
//!		01 = 65537 (2^16 +1) 
//!
//-----------------------------------------------------------------------------
static OSRETURNCODES PinLoadPublicKey( UBYTE *key, UWORD keylen )
{
    OSRETURNCODES status = orvDEVICEFAULT;

		// Command '88' 'Load RSA Key'
		memcpy( PINBUF, "88/", 3 );
        // Put now the key and the exponent
        memcpy( &PINBUF[3], key, keylen );
        // The key is available, load it
		if ( orvOK == ( status = Write_Read_Pin( keylen + 3, 10*MS1000, False ) ) )
		{
                // Check the response (Msg. type '89' and status '0' for success)
				if ( PINBUF[0] != '8' || PINBUF[1] != '9' || PINBUF[3] != '0' )
				{
					status = orvDEVICEFAULT;
				}

		}
        return status;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Check point of VPP Online PIN entry
//!
//! \param
//!		type				VPP action type as it defined into pinwrap.h
//!		DUKPTflag	1 in case if DUKPT mode, 0 - Master Session mode
//!		trpan		pointer to PAN
//!		panLength	PAN length
//!		ksn			pointer to Key Serial Number (for DUKPT only)
//!		ksn_size	KSN size (for DUKPT only) 
//!		key_id		Master Key ID in the key collection
//!		doubleKey	working key length (8,16,24)
//!		workingKey	pointer to working session key
//!				
//! \return
//!		True if success, False otherwise
//!
//! \note
//!
//-----------------------------------------------------------------------------

Bool VPP_Online( VPP_ActionType action, UBYTE* trpan, int panLength, unsigned char* ksn,
			  int ksn_size, unsigned char key_id, int doubleKey, unsigned char* workingKey,
              UBYTE *retStatus )
{
    VPP_EVENTS ev;
    Bool bRetVal = False;
    UBYTE sw1sw2[2];

    while(1)
    {
        // Initialise the virtual pinpad for Online PIN 
        VPPInit( action, trpan, panLength, ksn, ksn_size, key_id, doubleKey, workingKey );

        // Display PIN entry screen
        VPP_PinEntry( 2, 12, 4, &PinEntry );

        // Get the PIN from te virtual pinpad
        ev = VPPGetPin( 12, Dspbuf2, NULL, UpdateInputBox, sw1sw2 );
        // Check if pin number has been entered OK
        if( VPP_EnterKey == ev )
        {
#ifdef MAKE_PINATTACKDELAY
            // Setup the pin attack delay
            PinAttackDelay( TRINP.TRPAN, TRINP.TRPINBL,  SDK_ThisPage() );
#endif //MAKE_PINATTACKDELAY
            *retStatus = EMVL2_PIN_ENTERED;
            bRetVal = True;
            break;
        }
        else if( VPP_CancelKey == ev )
        {
            ShowErrMsg( N_UserCancelled );
            *retStatus = EMVL2_ONLINE_CANCELLED;
            break;
        }
        else if( VPP_EncryptError == ev )
        {
            ShowErrMsg( EncryptError );
            *retStatus = EMVL2_PINPAD_FAILURE;
            break;
        }
        else
        {
            ShowErrMsg( PINFailed );
            *retStatus = EMVL2_PINPAD_FAILURE;
            break;
        }
    }// end while
    return bRetVal;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Check point of VPP EMV PIN entry
//!
//! \param
//!		type				VPP action type as it defined into pinwrap.h
//!		publicKey			ICC public key
//!		publicKeyLength		ICC public key length
//!		tries				tries number
//!		retStatus			PIN status
//!				
//! \return
//!		True if success, False otherwise
//!
//! \note
//!		The public key format is 'eekkk..' where e is the exponent
//!		and 'kk..' the public key. Public key length is the total
//!		length of key + exponent.
//!		Possible exponent values are: 
//!		02 = 2
//!		03 = 3 
//!		01 = 65537 (2^16 +1) 
//!
//-----------------------------------------------------------------------------

Bool VPP_OffLine( VPP_EMV_ActionType type, UBYTE* publicKey, UBYTE publicKeyLength, UBYTE tries, UBYTE* retStatus, UBYTE *sw1sw2 )
{
	*retStatus = EMVL2_PINPAD_FAILURE;
    int bContinue = 1;

    do
    {
        // Init the VPP for Offline PIN verification
        VPPEMVInit( type, publicKey, publicKeyLength );

        // Display PIN entry screen 
		VPP_PinEntry( 2, 12, 4, &PinEntry );

        // Get the PIN from the virtual PINPad
        switch( VPPGetPin( 12, Dspbuf2, NULL, UpdateInputBox, sw1sw2 ))
        {
			// Check if pin number has been entered OK
			case VPP_EnterKey:
				*retStatus = EMVL2_PIN_OK;
                bContinue = 0;
				break;

			case VPP_WrongPINLastTry: 
            case VPP_WrongPIN:
				if ( TCONF.TCOPT6 & TC6_PINBYPASSTERM )
				{
					// Exit the function
					*retStatus = EMVL2_PIN_BYPASSED;
				}
				else
				{
					if( 1 == --tries )
					{
						GetMsg( LAST_TRY_LEFT, CSTMSG );
						ShowErrMsg( CustomMsg );
						continue;
					}
					else if( 0 != tries )
					{
						GetMsg( TRIES_LEFT, CSTMSG );
						// Get the lenght of the Cashier/Server prompt
						UBYTE length = StrLn( CSTMSG, sizeof( CSTMSG ) );
						// convert pin try count to decimal
						CSTMSG[length] = ( tries > 10 ) ? 0x31 : 0x20;
						CSTMSG[length+1] = 0x30 + tries % 10;
						CSTMSG[length+2] = 0x00;
						ShowErrMsg( CustomMsg );
						continue;
					}
                    bContinue = 0;
					break;
				}

			case VPP_CustomerCardNotPresent:
				*retStatus = EMVL2_PINICC_STOP;
                bContinue = 0;
				break;

			case VPP_GeneralError:
            case VPP_NotActive:
				*retStatus = EMVL2_PINPAD_FAILURE;
                bContinue = 0;
				break;

			case VPP_ICCError:
				*retStatus = EMVL2_PIN_BLOCKED;
                bContinue = 0;
				break;

        case VPP_TimedOut:
        case VPP_CancelKey:
				*retStatus = EMVL2_ONLINE_CANCELLED;
                bContinue = 0;
				break;

            case VPP_HTCCardError:
			default:
				*retStatus = EMVL2_PINICC_FAIL;
                bContinue = 0;
                break;
		}
    }while( bContinue );

	return True;
}
/*
static VPP_EVENTS VPP_PinEntry( char DspMode, char Maxlen, char Minlen,
						   const ENTRY_STR * pEntry, UBYTE* dest, UBYTE *sw1sw2 )
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

    return  VPPGetPin( ESParms.MaxLen, Dspbuf2, (char*)dest, UpdateInputBox, sw1sw2 );
//    return SDK_VPPGetPin(Maxlen, Dspbuf2, ( char * )dest, UpdateInputBox );
}
*/

//-----------------------------------------------------------------------------
//! \brief
//!     Checks PIN offline
//!
//! \param
//!		CVMstatus		CVM processing flags
//!		tries			tries number of PIN entry 
//!		PINdata			external PIN PAD data
//!		retStatus		PIN status
//!				
//! \return
//!		True if success
//!
//! \note	
//!
//-----------------------------------------------------------------------------
Bool EMV_PINVerifyOffline( UBYTE CVMstatus, UBYTE tries, UBYTE* PINdata, UBYTE* retStatus )
{
    UBYTE pLength = 0xff;
    UBYTE key[pLength];

    if( PIN_TYPE_INTERNAL != PIN_TYPE )
	{
		// External Pinpad
		Enable_Flags[4] = (TCONF.TCOPT6 & TC6_PINBYPASSTERM) ? True : False;

		/* setup prompt text */
		DrawScreen( PINWAIT_SCR );

		switch (CVMstatus)
		{
			case EMVL2_CVM_OFP:  // Plain text
                PinVerifyOffline( PINTYPE_CLEAR, key, pLength, tries, retStatus, PINdata );
				break ;

			case EMVL2_CVM_OFE:  // Encrypted
                // Get the PIN encryption key & exponent
                EMVL2_ReadPINKeyData( key, &pLength );
                PinLoadPublicKey( key, pLength );
                PinVerifyOffline( PINTYPE_ENCR, key, pLength, tries, retStatus, PINdata );
				break ;
		}
	}
	else
	{
		EMVL2_ReadPINKeyData( key, &pLength );
		return VPP_OffLine( ( EMVL2_CVM_OFE == CVMstatus )?EMV_OFFLINE_ENCPIN:EMV_OFFLINE_CLEARPIN,
							  key, pLength, tries, retStatus, PINdata );
	}
	return True;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Checks PIN Online
//!
//! \param
//!		retStatus		PIN status
//!				
//! \return
//!		True if success
//!
//! \note	
//!
//-----------------------------------------------------------------------------
Bool EMV_PINVerifyOnline( UBYTE* retStatus )
{
    UBYTE status, mkid;
    Bool loopflag, DUKPTflag;
    Bool bRetVal = False;
    VPP_ActionType action;

        // May need to add a Get PIN Semaphore in the future

        // Set DUKPT flag
        DUKPTflag = (TRINP.TRAQPTR->AQOPT4) & AQ4_DUKPT;

        if ( PIN_TYPE_INTERNAL != PIN_TYPE )
        {
            // Display PINPAD in title bar.
            GetMsg( N_Pinpad, Titlebuf );
            User_Msgs[1] = WaitForPin;

            // ???
            Enable_Flags[0] = 0;
            DrawScreen( STATUS_SCR );

            // Remove PINPAD from title buffer.
            Titlebuf[0] = 0;

            bRetVal = PinVerifyOnline( ( DUKPTflag )?PINTYPE_DUKPT:PINTYPE_MASTER, TRINP.TRPAN, MAXPANL, TRINP.TRKSN,
                              S_TRKSN, TRINP.TRAQPTR->AQMKID, (TRINP.TRAQPTR->AQOPT4) & AQ4_DBLPINKEY ? 16 : 8,
                              TRINP.TRAQPTR->AQWORKKEY, TRINP.TRPINBL, retStatus );

        }
        else // Virtual Pinpad Detected
        {
            // Display TOTAL and amount on line 1.
            if ( TRINP.TRKEY != BALINQ )
                CustomAmount( AmountTotals, TRINP.TRTOTAM );
            else
                CSTMSG[0] = '\0';

            action = DUKPTflag?DUKPT:MASTER_SESSION;

            // Request an Online PIN entry
            bRetVal = VPP_Online( action, TRINP.TRPAN, MAXPANL, TRINP.TRKSN, S_TRKSN, TRINP.TRAQPTR->AQMKID,
                                (TRINP.TRAQPTR->AQOPT4) & AQ4_DBLPINKEY ? S_AQWORKKEY*2 : S_AQWORKKEY,
                                TRINP.TRAQPTR->AQWORKKEY, retStatus );
            if( bRetVal )
            {
                // Set PIN Block ready flag 
                TRINP.TRSTATUS[1] |= TS2_PINBLOCK;
            }
        }
        // May need to add a Release PIN Semaphore
        return bRetVal;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Get the software version from the external pinpad.
//!
//! \param
//!		buf			destination buffer for the software version
//!		buflen		length of the buffer
//!
//! \return
//!		True if OK. False otherwise
//!
//! \note	
//!
//-----------------------------------------------------------------------------
Bool PinGetVersion( PEDVERREC *version )
{
    unsigned char Retval = False;
    UBYTE *pbuf ;

        // May need to add a Get PIN Semaphore in the future

        // Clear the version structure
        memset( version, 0, sizeof( PEDVERREC ) );

        // Open and configure Pin Pad Port 
//     if( Open_Config () )
//     {
         // Load Message Type "90." - Get version
         memcpy( (char*)PINBUF, "90.", 3 );
         pbuf += 3 ;
         // Write message to Pin Pad
         if( Write_Read_Pin( 5, 4*MS1000, 0 ) == OK )
         {
                // Check if we got a valid response
                if ( PINBUF[0] == '9' && PINBUF[1] == '1' ) 
                { 
                    // Move received version number to the user buffer
                    if( 0 == memcmp( &PINBUF[3], ( UBYTE * ) "S9SC", 4 ) )
                    {
                        memcpy( version->PEDTYPE, &PINBUF[3], 4 );
                    }
                    else if( 0 == memcmp( &PINBUF[3], ( UBYTE * ) "S9", 2 ) )
                    {
                        memcpy( version->PEDTYPE, &PINBUF[3], 2 );
                    }
                    // Move the PED software name & version
                    memcpy( version->SECVER, &PINBUF[3], 10 );
                    // Move the EMVL1 software version
                    memcpy( version->EMVL1VER, &PINBUF[21], 16 );
                    // Move the EMVL2 software version
                    memcpy( version->EMVL2VER, &PINBUF[37], 12 );
                    Retval = True ;
                }
         }
//         // Close Pin Pad Port
//         if( !Close_Pin() )
//         {
//             Retval = False ;
//         }
//     }
   return ( Retval );
}

//-----------------------------------------------------------------------------
//  CAT33Message      Build message and send it to the Pin Pad.
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        None
//  Notes:          CAT33Retval set to True if OK, False otherwise
//                  Message to display in PinMsgNumber.
//-----------------------------------------------------------------------------
extern UBYTE PinDisplayText( char *line1, char *line2, char *line3, char *line4 )
{
    int length;
    UBYTE *bufptr = &PINBUF[3];
  
    // Put CAT33 command 3C Display message 
    PINBUF[0] = '3';
    PINBUF[1] = 'C';
    PINBUF[2] = '.';
  
    // Now move line1 if present
    if( line1 )
    {
       length = MoveTillZero( (char *)bufptr, (const char *)line1, 16 );
       bufptr += length;
    }
    else
    {
//        memset( bufptr, ' ', 16 );
//        bufptr += 16;
    }

    // Now move line2 if present
    if( line2 )
    {
       // Add a field separator
       *bufptr = 0x1C; bufptr++;

       length = MoveTillZero( (char *)bufptr, (const char *)line2, 16 );
       bufptr += length;
    }
    else
    {
//        memset( bufptr, ' ', 16 );
//        bufptr += 16;
    }

    // Now move line3 if present
    if( line3 )
    {
       // Add a field separator
       *bufptr = 0x1C; bufptr++;
       length = MoveTillZero( (char *)bufptr, (const char *)line3, 16 );
       bufptr += length;
    }
    else
    {
//        memset( bufptr, ' ', 16 );
//        bufptr += 16;
    }

    // Now move line4 if present
    if( line4 )
    {
       // Add a field separator
       *bufptr = 0x1C; bufptr++;
       length = MoveTillZero( (char *)bufptr, (const char *)line4, 16 );
       bufptr += length;
    }
    else
    {
//        memset( bufptr, ' ', 16 );
//        bufptr += 16;
//       // Add a field separator
//       *bufptr = 0x1C; bufptr++;
    }

  return Write_Pin( (UBYTE)( bufptr - PINBUF ) );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Writes/reads Pin
//!
//! \param
//!		length
//!		timeout
//!		chkkbd
//!				
//! \return
//!
//! \note	
//!
//-----------------------------------------------------------------------------
UBYTE Write_Read_Pin( UWORD length, unsigned short timeout, Bool chkkbd )
{
	UBYTE eventflag;
	Bool Retval = True;	// error/timeout
	UBYTE InputType;
	Bool bPinCardStatus;

	PINRXLEN = 0;

	if ( chkkbd != 0 )
	{
		read_io( 0, 0, &bPinCardStatus, &DevFlag );
	}

	/* Now Post a write to the pin device (flag will be ignored) */
	if ( OK == DeviceWrite (PIN_DEVICE, &PINBLKW, PROCEED+FLAG, PINBUF, length, &eventflag, 6 ) )
	{
		/* First Post a concurrent read to the pin device */
		if ( OK == DeviceRead (PIN_DEVICE, &PINBLKR, PROCEED+FLAG, PINBUF, sizeof(PINBUF), &eventflag, 7 ) )
		{
			/* Start response timer (timeout will set Bit 0) */
			StartRspTimer( &TIM_01, &eventflag, timeout );

			/* Wait for timeout (bit 0) or read complete (bit 7) */
			while( ! ( eventflag & 0x01 ) )
			{
				if ( chkkbd != 0)
				{
					//@TUM The key press processing is a bit messy:
					// CLEAR_KY - 'Clear' key on the keyboard or 'CANCEL' button on the pad. Always means 'Cancel the trxn';
					// CANCEL_KY - 'X' button in the title. Means 'Bypass' if bypass is allowed, 'Cancel' otherwise;
					// ENTER_KY - 'Enter' on the keyboard or 'BYPASS' button on the pad. Always means 'Bypass', not shown if bypass is not allowed.
					InputType = read_io( DEV_KBD + DEV_PAD + STAY_OPEN0, 1, &bPinCardStatus, &DevFlag );
					if ( InputType )
					{
						if ( DEV_PAD == InputType )
						{
							KeyVal = ButtonId;
						}

						if ( KeyVal == CLEAR_KY )
						{	// means we wish to Cancel the transaction
							Retval = 2;
							break;
						}
						else if ( KeyVal == CANCEL_KY )
						{
							if ( TCONF.TCOPT6 & TC6_PINBYPASSTERM )
							{
								// bypass
								Retval = 3;
							}
							else
							{
								// cancel
								Retval = 2;
							}
							break;
						}
						else if ( KeyVal == ENTER_KY && (TCONF.TCOPT6 & TC6_PINBYPASSTERM) )
						{
							// bypass
							Retval = 3;
							break;
						}
					}
				} // if ( chkkbd != 0)

				if ( ( eventflag & 0x80 ) && ( OK == PINBLKR.iocs_stati ) )
				{
					// Successful Request/Response, clear the TFPINFAILFLG if set
					if ( TFTAB.TFPINFAILFLG )
					{
						TFTAB.TFPINFAILFLG = 0;
						TFLRC = CalcLrc( (UBYTE *)&TFTAB, TFSZE );
					}

					// Get response length
					PINRXLEN = PINBLKR.iocs_var.iocs_xfer.iocs_tlenx;
					Retval = OK ;
					break;
				}
			OS_RelinqCPU(); //@aist
			}	// while( ! ( readflag & 0x01 ) )
		}	// if ( OK == DeviceWrite (PIN_DEVICE, &PINBLKW, PROCEED+FLAG, PINBUF, length, &eventflag, 6 ) )

		/* Cancel response timeout timer */
		OS_CancelTimer ( &TIM_01 );
	}	// if ( OK == DeviceRead (PIN_DEVICE, &PINBLKR, PROCEED+FLAG, PINBUF, sizeof(PINBUF), &readflag, 7 ) )
	
//	if ( Retval != OK )
//	{
//		/* Rewind PIN device if error */
//		Rewind_Pin();
//	}

	return(Retval);	   
}

//-----------------------------------------------------------------------------
//! \brief
//!     Performs an APDU exchange with the Smart Card Device.
//!
//! \param
//!		cmd			array of bytes containing the command
//!		cmdlen		length of the comand
//!		NADByte		T1 NAD char Value (0x00 in EMV)
//!		device		device to be used (0-user card;1,2-SIM slots)
//!		RspBuf		response buffer
//!		RspLen		response buffer length
//!				
//! \return
//!		True if OK. False otherwise
//!
//! \note	
//!
//-----------------------------------------------------------------------------
OSRETURNCODES PinScrCmd( UBYTE *cmd, UWORD cmdlen, UBYTE NADByte, UBYTE device, UBYTE *RspBuf, UWORD *RspLen )
{
   unsigned char Retval = orvDEVICEFAULT ;
   UBYTE *pbuf ;
 
   /* May need to add a Get PIN Semaphore in the future */

   pbuf = PINBUF;

	/* Open and configure Pin Pad Port */
//	if( Open_Config() )
//	{
 		/* Check configuration */
 		if ( (PIN_CAT33 == *((UBYTE*)PIN_CONFIG)) || (PIN_TYPE == PIN_TYPE_INTERNAL) )
			return Retval;
 		
 		/* Load the Message Type - Display Text Request */
		memset(PINBUF, 0, S_PINBUF );

		strcpy( (char*)PINBUF, "E2/" );/* Move request code and separator*/
		pbuf += 3;

		*pbuf++ = device;  /* Move device number */
		*pbuf++ = NADByte;  /* Move NAD byte */
		memcpy ( pbuf, cmd, cmdlen ); /* Move in the card command */
		pbuf +=	cmdlen; /* Point to the end of the buffer */

		/* Write message to Pin Pad */
		if( Write_Read_Pin( (UWORD)( pbuf-PINBUF ), 30*MS1000,0 ) == orvOK  )
		{
			if ( PINBUF[3] == '0' && PINBUF[4] == '0' ) 
			{ 
				/* Calculate ICC Response length  */
				*RspLen = (UWORD)(PINRXLEN - 6);

				 /* Move received response to the user buffer */
				 memcpy( RspBuf, &PINBUF[6], *RspLen );
				 Retval = orvOK ;
			}
		}
		else
			OS_DeviceRewind(PIN_DEVICE);

//		/* Close Pin Pad Port */
//		if( !Close_Pin() )
//		{
//			Retval = False ;
//		}
//	}
    return (Retval);
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets an ATR from the selected device.
//!
//! \param
//!		ATRBuffer		where to put the received ATR. First byte is the total ATR length
//!		ATRLen 			length of the ATR
//!		msg1			message 1 to be displayed
//!		msg2 			message 2 to be displayed
//!		device 			device to be used (0-user card;1,2-SIM slots
//!				
//! \return
//!		True if OK. False otherwise
//!
//! \note	
//!
//-----------------------------------------------------------------------------
OSRETURNCODES PinScrATR( UBYTE *ATRBuffer, UWORD *ATRLen, char *msg1, char *msg2, UBYTE device )
{
   unsigned char Retval = orvDEVICEFAULT ;
   UBYTE *pbuf ;
   UBYTE ATRloop = 2;

   /* May need to add a Get PIN Semaphore in the future */

	/* Open and configure Pin Pad Port */
//	if( Open_Config () )
//	{
 		/* Check configuration */
 		if ( (PIN_CAT33 == *((UBYTE*)PIN_CONFIG)) || (PIN_TYPE == PIN_TYPE_INTERNAL) )
			return Retval;
 		
		// due to possible problems with ATR of HFT105 
		// get a second ATR if the first one fails

		do 
		{
		    pbuf = (UBYTE*) PINBUF ;
		/* Load the Message Type - Display Text Request */
			memset( PINBUF, 0, S_PINBUF );
   			strcpy( (char*)PINBUF, "E0/" ); /* Move request code and separator*/
			pbuf += 3 ;
   			*pbuf++ = device;  /* Move device number */
            length = 4;
            // If upper message present
            if ( msg1 )
            {
                strcat( (char *)pbuf, msg1 ); /* Move display message */
                length += strlen( msg1 );
            }
			strcat( (char *)pbuf, "\x1c" );
            length++;
            // If lower message present
			if ( msg2 )
			{
				strcat( (char *)pbuf, msg2 );
                length += strlen( msg2 );
     		}

			/* Write message to Pin Pad */
			if( Write_Read_Pin( length, 5*MS1000, 0 ) == orvOK )
			{
				if ( PINBUF[3] == '0' && PINBUF[4] == '0' ) 
			  	{ 
					/* Calculate ART length  */
				 	*ATRLen = (UWORD)(PINRXLEN - 6);

				 	/* Move received ATR to user buffer */
				 	memcpy( ATRBuffer, &PINBUF[6], *ATRLen );

				 	Retval = orvOK ;
				 	ATRloop = 0 ;
			   	}
			  	else
					ATRloop--;
			}
		    else
				ATRloop--;
		 } while (ATRloop != 0);


		// Close Pin Pad Port
//		if( !Close_Pin() )
//		{
//			Retval = False ;
//		}
//	}

  return (Retval);
}

//-----------------------------------------------------------------------------
//! \brief
//!     Sends a Cancel command to pinpad.
//!
//! \param
//!		None
//!				
//! \return
//!		True if Card Inserted, otherwise False.
//!
//! \note	
//!
//-----------------------------------------------------------------------------
Bool PinCancel ( void )
{
    unsigned char Retval = False ;

   // May need to add a Get PIN Semaphore in the future

   // Open and configure Pin Pad Port
//	if( Open_Config() )
//	{
 		// Check configuration
 		if ( PIN_TYPE == PIN_TYPE_INTERNAL )
        {
            return Retval;
        }
		// Load the Message Type - Display Text Request
		memcpy( PINBUF, "72", 2 ); /* Move request code and separator*/

		/* Write message to Pin Pad */
		if( Write_Read_Pin ( (UWORD) 2, MS1000, 0 ) == OK )
		{
                Retval= True ;
		}

		// Close Pin Pad Port
//		if( !Close_Pin() )
//		{
//			Retval = False ;
//		}
//	}

  return (Retval);
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets the card insertion status.
//!
//! \param
//!		device			device to be used (0-user card;1,2-SIM slots)
//!		CardStatus		card status
//!				
//! \return
//!		True if Card Inserted, otherwise False.
//!
//! \note	
//!
//-----------------------------------------------------------------------------
OSRETURNCODES PinCardStatus( UBYTE device, UBYTE * CardStatus )
{
   UBYTE *pbuf = PINBUF ;
   unsigned char Retval = orvDEVICEFAULT ;

   /* May need to add a Get PIN Semaphore in the future */

	*CardStatus = SCRD_INS_NO ;

	/* Open and configure Pin Pad Port */
//	if( Open_Config() )
//	{
 		// Check configuration
 		if ( (PIN_CAT33 == *((UBYTE*)PIN_CONFIG)) || (PIN_TYPE == PIN_TYPE_INTERNAL) )
			return Retval;
 		
		// Load the Message Type - Display Text Request

		strcpy( (char*)pbuf, "E6/" ); /* Move request code and separator*/
		pbuf += 3;
		*pbuf++ = 0 ; // device;

		/* Write message to Pin Pad */
		if( Write_Read_Pin ( (UWORD) 4, MS1000, 0 ) == OK )
		{
			if ( (*(pbuf-1) == '0') && (*pbuf == '0') ) 
			{
				 *CardStatus = SCRD_INS_YES ;
				 Retval= orvOK ;
			}
		}
		else
		{
			OS_DeviceRewind(PIN_DEVICE);
		}

		/* Close Pin Pad Port */
//		if( !Close_Pin() )
//		{
//			Retval = False ;
//		}
//	}
  return (Retval);
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets the card insertion status.
//!
//! \param
//!		device			device to be used (0-user card;1,2-SIM slots)
//!		CardStatus		card status
//!				
//! \return
//!		True if Card Inserted, otherwise False.
//!
//! \note	
//!
//-----------------------------------------------------------------------------
OSRETURNCODES PinCardStatusIdle( UBYTE device, UBYTE * CardStatus )
{
    unsigned char Retval = orvDEVICEFAULT ;

	*CardStatus = SCRD_INS_NO;

	if ( !(ScrChkStat & ScrRecvState ) )
	{
		if ( !(ScrChkStat & ScrTimeOutState) )
		{
			if ( !(ScrChkStat & ScrSendState) )
			{
				/* Open and configure Pin Pad Port */
//				if( Open_Config() )
//				{
 					/* Check configuration */
 					if ( (PIN_CAT33 == *((UBYTE*)PIN_CONFIG)) || (PIN_TYPE == PIN_TYPE_INTERNAL) )
						return Retval;
 		
					strcpy( (char*)PINBUF, "E6/" ); /* Move request code and separator*/
					PINBUF[3] = 0;// device;

					/* Send request to the pinpad */
					if ( OK == DeviceWrite (PIN_DEVICE, &PINBLKW, PROCEED+FLAG, PINBUF, 4, &ScrChkStat, 6 ) )
					{
						ScrChkStat |= ScrSendState;
						Retval = orvOK;
					}
					else
					{
						OS_DeviceRewind(PIN_DEVICE);
					}
//				}
			}
			else
			{
				if ( !(ScrChkStat & ScrTimerState) )
				{
					/* Start response timer */
					StartRspTimer( &TIM_01, &ScrChkStat, 3*MS1000 );

					/* Read response from the pinpad */
					if ( orvOK == DeviceRead (PIN_DEVICE, &PINBLKR, PROCEED+FLAG, PINBUF, sizeof(PINBUF), &ScrChkStat, 5 ) )
					{
						ScrChkStat |= ScrTimerState;
						Retval = orvOK;
					}
					else
					{
						OS_DeviceRewind(PIN_DEVICE);
						ScrChkStat &= ~ScrSendState;
					}
				}
			}
		}
		else
		{
			/* Cancel response timeout timer */
			OS_CancelTimer ( &TIM_01 );
			OS_DeviceRewind(PIN_DEVICE);
			ScrChkStat &= ScrIdleState;
		}
	}
	else
	{
		if ( orvOK == PINBLKR.iocs_stati )
		{
			if ( (PINBUF[3] == '0') && (PINBUF[4] == '0') ) 
			{
				*CardStatus = SCRD_INS_YES ;
			}

			Retval = orvOK ;
		}

//		Close_Pin();
		ScrChkStat = 0;
	}

	return (Retval);
}

//-----------------------------------------------------------------------------
//! \brief
//!     Power off the card and display message to remove the card then loop until card is removed.
//!
//! \param
//!		device			device to be used (0-user card;1,2-SIM slots)
//!		CardStatus		card status
//!				
//! \return
//!		True if Card Inserted, otherwise False.
//!
//! \note	
//!
//-----------------------------------------------------------------------------
OSRETURNCODES PinEject ( UBYTE device )  // new function added 29 Oct 02 - BD
{
   unsigned char Retval = orvDEVICEFAULT ;
   UBYTE *pbuf = PINBUF ;
 
   /* May need to add a Get PIN Semaphore in the future */

	/* Open and configure Pin Pad Port */
//	if( Open_Config () )
//	{
 		/* Check configuration */
 		if ( (PIN_CAT33 == *((UBYTE*)PIN_CONFIG)) || (PIN_TYPE == PIN_TYPE_INTERNAL) )
			return Retval;
 		
		/* Load the Message Type - Display Text Request */
		memset(PINBUF, 0, 20 );

		strcpy( (char*)PINBUF, "E4/" ); /* Move request code and separator*/
		pbuf += 3;
		*pbuf++ = device;

		/* Write message to Pin Pad */
		if( Write_Read_Pin( 4, 3*MS1000,0 ) == OK )
		{
			if ( *(pbuf-1) == '0' && *pbuf == '0' ) 
			{ 
				Retval = orvOK ;
			}
		}
		else
			OS_DeviceRewind(PIN_DEVICE);

		/* Close Pin Pad Port */
//		if( !Close_Pin() )
//		{
//			Retval = False ;
//		}
//	}
  return (Retval);
}

//-----------------------------------------------------------------------------
//! \brief
//!     Initialises the Smart Card Interface(s).
//!
//! \param
//!		None
//!				
//! \return
//!		Status
//!
//! \note	
//!		should only do this if Termaster Configuration indicates that
//! 	there should be a pinpad.  if not the function sets PinSmartCard_Read
//! 	to zero to indicate no pinpad.
//-----------------------------------------------------------------------------
OSRETURNCODES LOCAL_FN(ScrdInitIO)( void ) 
{
// should only do this if Termaster Configuration indicates that
// there should be a pinpad.  if not the function sets PinSmartCard_Read
// to zero to indicate no pinpad.
//@TUM DEBUG Should be called separately	PinInit( ); 

	if ( !(PINSTAT & PinSmartCard_Read) )
	{ // Internal card reader
		Status = SDK_SC_ScrdInitIO( scrd_SCR );
	}
	else
	{ // External Pinpad Card reader
		Status = orvOK;
	}
	return Status;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Checks to see if the card is inserted
//!
//! \param
//!		[in]ScrdDevice  SDK device number
//!		[out]pCmdStatus operation result buffer (SCRD_INS_YES or SCRD_INS_NO)
//!				
//! \return
//!     orvOK on success else error code
//-----------------------------------------------------------------------------
OSRETURNCODES LOCAL_FN(ScrdInserted)( SCRDDEVICE ScrdDevice,UBYTE *pCardStatus )
{
    UBYTE CardStatus = SCRD_INS_NO;

    if ( !(PINSTAT & PinSmartCard_Read) )
    { // internal card reader
		Status = SDK_SC_CheckInserted( ScrdDevice );
        if( orvOK == Status )
		{
			CardStatus = SCRD_INS_YES;
		}
		Status = orvOK;
    }
    else
    { // external card reader
        if (ScrChkStat & ScrIdleState)
        {
            PinCardStatusIdle( ScrdDevice, &CardStatus );
            Status = orvOK;
        }
        else
        {
            if ( ScrChkStat )
            {
                /* Cancel response timeout timer */
                OS_CancelTimer ( &TIM_01 );
                /* Close Pin Pad Port */
//                Close_Pin();
                /* Reset all flags */
                ScrChkStat = 0;
            }
            PinCardStatus( ScrdDevice, &CardStatus );
            Status = orvOK;
        }
    }
    *pCardStatus = CardStatus;
    /* Return okay status back to caller */
    return( Status );
} // ScrdInserted

//-----------------------------------------------------------------------------
//! \brief
//!     Ejects the smart card device
//!
//! \param
//!		[in]ScrdDevice  SDK device number
//!				
//! \return
//!     orvOK on success else error code
//-----------------------------------------------------------------------------
OSRETURNCODES LOCAL_FN(ScrdEject)( SCRDDEVICE ScrdDevice )
{
    SCRDDEVICE      PinDevice;

    if (!(PINSTAT & PinSmartCard_Read))
    {	// Internal smart card
		Status = SDK_SC_Eject( ScrdDevice );
    }
    else
    {
        if (ScrdDevice == scrd_SCR)
        {
            PinDevice = 0;
        }
        else
        {
            PinDevice = ScrdDevice;
        }
        Status = PinEject( PinDevice );
    }
    /* Return the status to the caller */
    return( Status );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Power on Card and get ATR
//!
//! \param
//!		[in]ScrdDevice  SDK device number
//!		[in]EMVFlag     (not used)
//!		[out]pATRBytes  Answer-To-Reset buffer
//!		[out]pATRLen    Answer-To-Reset response length buffer
//!		[out]pHistBytesOffset
//!                     historical bytes offset buffer
//!		[in/out]pHistLen
//!                     historical bytes length buffer
//!		[out]pCmdStatus operation result buffer
//!				
//! \return
//!     orvOK on success else error code
//-----------------------------------------------------------------------------
OSRETURNCODES LOCAL_FN(ScrdGetATR)( SCRDDEVICE ScrdDevice,
							   Bool    EMVFlag,
 							   UBYTE  *pATRBytes, 
 							   UWORD  *pATRLen, 
 							   UBYTE  *pHistBytesOffset, 
 							   UBYTE  *pHistLen, 
 							   UBYTE  *pCardStatus )
{
    UBYTE           ATRBuf[33];	// Answer-To-Reset scratch buffer
    UWORD           ATRLen = 0; 	// Answer-To-Reset length   
    UBYTE           HistLen = 0;	// Historical Bytes length 
    UBYTE           HistLenOffset = 0;	// Historical Bytes length offset
    UBYTE           CardStatus;
    SCRDDEVICE      PinDevice;

    *pCardStatus = ATR_NONE;
	memset( ATRBuf, 0, sizeof(ATRBuf) );
    if (!(PINSTAT & PinSmartCard_Read))
	{
		Status = SDK_SC_GetATR( ScrdDevice, ATRBuf, &ATRLen, &HistLenOffset );
        if( Status == orvOK )
        {
            CardStatus = ATR_GOOD;
        }
	}
	else
	{
		if (ScrdDevice == scrd_SCR)
		{
			PinDevice = 0;
		}
		else
		{
			PinDevice = ScrdDevice;
		} 
		
		GetPinMsg( Msg_ProcCard, CSTMSG3 );
		GetPinMsg( Msg_ClrScrn  , CSTMSG4 );
		
		Status = PinScrATR ( ATRBuf, &ATRLen, CSTMSG3, CSTMSG4, PinDevice );
		// now need to calculate Historical bytes
		HistLen       = ATRBuf[1] & 0x0F;  // get the length
		HistLenOffset = ATRLen - HistLen; 
		// now need to convert the error code to smart card codes
		if (orvOK == Status )
		{
			CardStatus = ATR_GOOD;
			if (ScrdDevice == scrd_SCR)
			{
			    PinMessage( Msg_ProcCard );
			}
		}
		else
		{
			CardStatus = ATR_BAD;
		}
	}	// if (!(PINSTAT & PinSmartCard_Read))

	if (Status == orvOK)
	{
		if (ATRLen > 0)
		{
			memcpy ( pATRBytes, ATRBuf, ATRLen );
		}
		*pATRLen = ATRLen;
		*pHistLen = HistLen;
		*pHistBytesOffset = HistLenOffset;
		*pCardStatus = CardStatus;
	}
	else
	{
		LOCAL_FN(ScrdEject)( ScrdDevice ); // just ensure that the card is powered down.
	}

	/* Return status back to caller */
	return( Status );
} // end ScrdGetATR

//-----------------------------------------------------------------------------
//! \brief
//!     Returns the protocol detected in ATR
//!
//! \param
//!		[in]ScrdDevice  SDK device number
//!		[out]pProtocol  protocol buffer pointer
//!				
//! \return
//!     orvOK on success
//-----------------------------------------------------------------------------
OSRETURNCODES LOCAL_FN(ScrdGetProtocol)( SCRDDEVICE ScrdDevice, UBYTE *pProtocol )
{
    UBYTE Prot;

    if (!(PINSTAT & PinSmartCard_Read))
    {
        Status = orvOK;
    }
    else
    {
        // need to do some checking for external !!
        // but its not required
        Prot = 0; // this is not right !
    }
    *pProtocol = Prot;
    return Status;
}// end ScrdGetProtocol

//-----------------------------------------------------------------------------
//! \brief
//!     Sends command to smart card
//!
//! \param
//!		[in]ScrdDevice  SDK device number
//!		[in]EMVFlag     (not used)
//!		[in]pCmdBuf     command buffer pointer
//!		[in]CmdLen      length of data in command buffer
//!		[out]pRspBuf    response buffer pointer
//!		[in/out]pRspLen length of response buffer
//!		[out]pSWBytes   "switches" data buffer
//!		[out]pCmdStatus operation result receive pointer
//!		[out]pNAD       
//!				
//! \return
//!     orvOK on success else error code
//-----------------------------------------------------------------------------
OSRETURNCODES LOCAL_FN(ScrdSendCmd)( SCRDDEVICE ScrdDevice,
								Bool   EMVFlag,
 								UBYTE *pCmdBuf, 
 								UWORD  CmdLen,
 								UBYTE *pRspBuf,
 								UWORD *pRspLen,
 								UBYTE *pSWBytes, 
 								UBYTE *pCmdStatus,
 								UBYTE *pNAD )
{
    SCRDDEVICE      PinDevice;

    if (!(PINSTAT & PinSmartCard_Read))
    { 
		Status = SDK_SC_SendCmd( ScrdDevice, pCmdBuf, CmdLen, pRspBuf, pRspLen, pNAD );
        if (Status == orvOK )
        {
            *pCmdStatus = SCRD_INS_YES;
            *pSWBytes = *( pRspBuf + *pRspLen - 2 );
            *(pSWBytes+1) = *( pRspBuf + *pRspLen - 1 );
        }
    }
    else
    {
        if (ScrdDevice == scrd_SCR)
        {
            PinDevice = 0;
        }
        else
        {
            PinDevice = ScrdDevice;
        }
        // copy the cmd to buffer Rsp1, due to having to share buffer space.
        Status = PinScrCmd( pCmdBuf, CmdLen, *pNAD, PinDevice, pRspBuf, pRspLen );
        memcpy ( pSWBytes, &pRspBuf[*pRspLen -2], 2 );
        // set the CardStatus value as documentation - Update BD 17th June 2002
        /*
			ICCCMD_ERR  : command error
			ICCCMD_BAD  : comamnd okay with bad response code (non-0x9000)
			ICCCMD_OK 	: command OK with good response code (0x9000)
        */
        *pCmdStatus = ICCCMD_BAD;  // set to BAD first and change if necessary
        if( orvOK == Status )
        {
            *pCmdStatus = ICCCMD_ERR;
        }
        else
        {
            if (pSWBytes[0] == 0x90)
                *pCmdStatus = ICCCMD_OK;
        }
    }
    return (Status);
} // end ScrdSendCmd

//-----------------------------------------------------------------------------
//! \brief
//!     Starts a session with smart card
//!
//! \param
//!		[in]ScrdDevice  SDK device number
//!				
//! \return
//!     orvOK
//-----------------------------------------------------------------------------
OSRETURNCODES LOCAL_FN(ScrdStartSession)( SCRDDEVICE ScrdDevice )
{
    if (!(PINSTAT & PinSmartCard_Read)) // internal card reader
    {
        Status = orvOK;
    }
    else
    {
        // do nothing
        Status = orvOK;
    }
    return Status;
} // end ScrdStartSession

//-----------------------------------------------------------------------------
//! \brief
//!     Ends a session with smart card
//!
//! \param
//!		[in]ScrdDevice  SDK device number
//!				
//! \return
//!     orvOK
//-----------------------------------------------------------------------------
OSRETURNCODES LOCAL_FN(ScrdEndSession)( SCRDDEVICE ScrdDevice )
{
    if (!(PINSTAT & PinSmartCard_Read)) // internal card reader
    {
        Status = orvOK;
    }
    else
    {
        Status = orvOK;
    }
    /* Return status to the caller */
    return( Status );
} // end ScrdEndSession

//-----------------------------------------------------------------------------
//! \brief
//!     Returns the version string
//!
//! \param
//!		[out]returnBuffer version buffer
//!		[out]pZ8VersionID crypto processor (Z8) version buffer
//!				
//! \return
//!     None
//-----------------------------------------------------------------------------
static const char Version[] = "Scrd Ext 23Dec02";
UBYTE Z8Version;

void LOCAL_FN(ScrdVersion)( char *returnBuffer, UBYTE *pZ8VersionID )
{
    if (!(PINSTAT & PinSmartCard_Read)) // internal card reader
    {
        SDK_SC_Version( returnBuffer );
        Z8Version= 0;
    }
    else
    {
        Z8Version = 'x';
        memcpy ( returnBuffer, Version, strlen( Version ) +1  );
    }
    *pZ8VersionID = Z8Version;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Scans the PinPad SAM slots for an HCP chip
//!
//! \param
//!		[out]Sot number where the crypto was found
//!				
//! \return 
//!     status, orvOK if sucessful
//-----------------------------------------------------------------------------
static OSRETURNCODES  PinInitCrypto( UBYTE *slot, char *version )
{
    UBYTE   SCBytes[50];   // 50 bytes is enough for an ATR !!
    UWORD   SCLength;
    UBYTE   SCStatus;
    UBYTE   HistBytesOffset;
    UBYTE   HistLen;
    Bool    Found = False;
    UBYTE   cryptslot;

    // Start with the first SAM slot
    cryptslot = scrd_SC1;
     
    do
    {
        if( orvOK == PinScrATR ( SCBytes, &SCLength, NULL, NULL, cryptslot ) )
        {
            // now need to calculate Historical bytes
            HistLen       = SCBytes[1] & 0x0F;  // get the length
            HistBytesOffset = SCLength - HistLen; 

            if (memcmp(&SCBytes[HistBytesOffset], (UBYTE*) "HYPEMV", 6) == 0)
            {
                Found = True;
                // save the ATR information
                if(NULL != version )
                {
                    memcpy ( version, &SCBytes[HistBytesOffset], 11 );
                    *(version + 11 ) = 0 ; // null terminate
                }
            }
            else
            {
                cryptslot++;
            } 
        }
        else
        {
            cryptslot++;
        } 

    } while (!Found && cryptslot <= scrd_SC4);

    if( Found )
    {
         *slot = cryptslot;
         SCStatus = orvOK;
    }
    else
    {
        SCStatus = orvDEVICETIMEOUT;
    }
    return SCStatus;
}
#endif	// MAKE_EMV

//! gets PIN
//! The function is being used only for T4100 platform. The actually implementation this function
//! is located into security.lib library of HSDK
VPP_EVENTS VPP_GetPin( char Maxlen, char* screen_buf, char* dest, Call_Back UpdateInput )
{
	return VPP_NotActive;
}


