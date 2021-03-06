
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             pinpad.c
//      Pinpad functions
//
//=============================================================================
#include <eftos.h>

#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdktime.h"
#include "utllib.h"
#include "osclib.h"
#include "fixdata.h"
#include "CryptoService.h"

#include "defines.h"
#include "transdef.h"
#include "struct.h"
#include "util.h"
#include "msg.h"
#include "pinpad.h"
#include "appdata.h"
//#include "comdata.h"
#include "tables.h"
#include "scard.h"
#include "rs232tab.h"
#include "pinattck.h"
#include "cvtamt.h"
#include "cstmamnt.h"


#ifdef	MAKE_EMV
#include "emv_l2.h"
#endif	// MAKE_EMV

//=============================================================================
// External variables / declarations
//=============================================================================

// Public functions defined in this module
static Bool Write_Pin( UWORD, OS_TIMES );
static Bool Read_Pin( OS_TIMES timeout, Bool bCheckCancel );
static Bool Close_Pin( void );
static Bool VerifyTag( UBYTE RequestedIndex, UBYTE RequestedTag );
static void FormatPin( void );
//extern void FormatPin( void );
extern UBYTE * pinBlockFormat(UBYTE *pin , UWORD len);
#ifdef	MAKE_EMV
#ifdef EMV_FN
#undef EMV_FN
#endif

#define LOCAL_FN(FN) FN
#define EMV_FN(FN) EMV_##FN

extern OSRETURNCODES EMV_FN(ScrdInitIO)( void );
extern OSRETURNCODES EMV_FN(ScrdStartSession)( SCRDDEVICE ScrdDevice );
extern OSRETURNCODES EMV_FN(ScrdEndSession)( SCRDDEVICE ScrdDevice );
extern OSRETURNCODES EMV_FN(ScrdInserted)( SCRDDEVICE ScrdDevice, UBYTE *pCardStatus );
extern OSRETURNCODES EMV_FN(ScrdEject)( SCRDDEVICE ScrdDevice );
extern OSRETURNCODES EMV_FN(ScrdGetATR)( SCRDDEVICE ScrdDevice, Bool EMVFlag,
		UBYTE  *pATRBytes, UWORD *pATRLen, UBYTE *pHistBytesOffset, UBYTE  *pHistLen, UBYTE *pCardStatus );
extern OSRETURNCODES EMV_FN(ScrdSendCmd)( SCRDDEVICE ScrdDevice, Bool EMVFlag,
		UBYTE *pCmdBuf, UWORD  CmdLen, UBYTE *pRspBuf, UWORD *pRspLen, UBYTE *pSWBytes, UBYTE *pCmdStatus, UBYTE *pNAD );
extern OSRETURNCODES EMV_FN(ScrdGetProtocol)( SCRDDEVICE ScrdDevice, UBYTE *pProtocol );
extern void EMV_FN(ScrdVersion)( char *returnBuffer, UBYTE *pZ8VersionID );

static void PinOffline ( UBYTE );
#endif	// MAKE_EMV


void C_DesTripleEncryptCBC(const BYTE* pKey, const BYTE* pIV, const BYTE* pData, DWORD nLen, BYTE* pResult);
extern void GetPinFromPinblock( void );


//=============================================================================
// Private defines and typedefs
//=============================================================================
typedef struct
{
	UBYTE		device;	// Device ID
	PIN_CPWR 	power;	// Power mode
	PINDATA*		cfgptr;	// Pointer to configuration parameters
} CFGDATA;

//static const PINPRC	CFG_S8_2400		= { PIN_CAT33,   PIN_TERMINAL, SIO_7EVEN1,	SIO_B2400  };
//static const PINPRC	CFG_S8_9600		= { PIN_CAT33,   PIN_TERMINAL, SIO_7EVEN1,	SIO_B9600  };
//static const PINPRC	CFG_S8_19200		= { PIN_CAT33,   PIN_TERMINAL, SIO_7EVEN1,	SIO_B19200 };

//static const PINPRC	CFG_HFT105_485_2400	= { PIN_POSMINI, PIN_TERMINAL, SIO_8NONE1,	SIO_B2400  };
//static const PINPRC	CFG_HFT105_485_9600	= { PIN_POSMINI, PIN_TERMINAL, SIO_8NONE1,	SIO_B9600  };
//static const PINPRC	CFG_HFT105_485_19200	= { PIN_POSMINI, PIN_TERMINAL, SIO_8NONE1,	SIO_B19200 };

#ifdef	MAKE_HFT_P1100
//static const PINPRC	CFG_P1100_422_19200	= { PIN_HFT, PIN_TERMINAL, /*SIO_8NONE1*/ SIO_8EVEN1, SIO_B19200 };
#endif	//MAKE_HFT_P1100

//static const prtprc	CFG_HFT105_232_2400	= { PRTP_POSMINI,              SIO_8NONE1,	SIO_B2400  };
//static const prtprc	CFG_HFT105_232_9600	= { PRTP_POSMINI,              SIO_8NONE1,	SIO_B9600  };
//static const prtprc	CFG_HFT105_232_19200	= { PRTP_POSMINI,              SIO_8NONE1,	SIO_B19200 };

static const PINDATA CFG_CAT33_2400 = { pp_CAT33, pm_Terminal, pc_7E1, pb_2400 };
static const PINDATA CFG_CAT33_9600 = { pp_CAT33, pm_Terminal, pc_7E1, pb_9600 };
static const PINDATA CFG_CAT33_19200 = { pp_CAT33, pm_Terminal, pc_7E1, pb_19200 };

static const PINDATA CFG_POSMINI_2400 = { pp_POSMINI, pm_Terminal, pc_8N1, pb_2400 };
static const PINDATA CFG_POSMINI_9600 = { pp_POSMINI, pm_Terminal, pc_8N1, pb_9600 };
static const PINDATA CFG_POSMINI_19200 = { pp_POSMINI, pm_Terminal, pc_8N1, pb_19200 };

#ifdef	MAKE_HFT_P1100
static const PINDATA CFG_HFT_2400 = { pp_HFT, pm_Terminal, pc_8E1, pb_2400 };
static const PINDATA CFG_HFT_9600 = { pp_HFT, pm_Terminal, pc_8E1, pb_9600 };
static const PINDATA CFG_HFT_19200 = { pp_HFT, pm_Terminal, pc_8E1, pb_19200 };
#endif	//MAKE_HFT_P1100

static const CFGDATA ProtList[]	=
{
#ifdef	MAKE_HFT_P1100
	{ PIN, PIN_CPWR_ON, (void*)&CFG_HFT_19200  },
#endif	//MAKE_HFT_P1100
	{ PIN, PIN_CPWR_OFF, (void*)&CFG_POSMINI_19200 },
//	{ PIN, PIN_CPWR_OFF, (void*)&CFG_HFT105_485_9600  },
//	{ PIN, PIN_CPWR_OFF, (void*)&CFG_HFT105_485_2400  },
//	{ PIN, PIN_CPWR_OFF, (void*)&CFG_S8_19200 },
//	{ PIN, PIN_CPWR_OFF, (void*)&CFG_S8_9600  },
	{ PIN, PIN_CPWR_OFF, (void*)&CFG_CAT33_2400  },
	{   0, PIN_CPWR_OFF, NULL                  }
};

//=============================================================================
// Private function declarations
//=============================================================================
extern Bool Open_Config( void );
static char  GetPINDevice( void );
//static void  GetCommand( char* );
//static UBYTE GetMode( void );
static void  GetFS( void );						
static void  GetEncKey( UBYTE*, short, Bool );	
static UBYTE GetEncKeyId( void );
static void  GetPinPrompt( void );
static void GetPinMsg( enum pinm_id, char* );	
static void  PreparePinCalc( void );
static void  GetPINData( char * pData, int lenght ); // @AAMELIN
static void  GetTotam( void ); // @AAMELIN
static void  GetPAN(UBYTE MaxLength,Bool bRemLast, Bool bSaveBCD); // @AAMELIN
static void  GetPINByte( char cBYTE ); //@AAMELIN
static void  SavePINBlock( void ); //@AAMELIN
static void  SaveASCIPINBlock( void ); //@AAMELIN
static void  SaveSerialNumber( void ); //@AAMELIN
static void ClearPinPad( void );//@AAMELIN
static UBYTE SaveTrack( UBYTE startPos, UBYTE * destBuf, UBYTE maxLen);

#ifdef	MAKE_HFT_P1100
// HFT protocol additions
//static void HFT_BuildDARReq ( void );
//static Bool HFT_ParseDARRsp( void );
static Bool HFT_LoadPINKey( void );
static Bool HFT_IncEncCount( void );
static void SetLenghtP1100();
#endif	//MAKE_HFT_P1100
static Bool Write_Read_Pin( UWORD, unsigned short, Bool );
#ifdef	MAKE_EMV
static Bool  PinCancel        ( void ) ;

static Bool PinScrCmd( UBYTE*, UWORD, UBYTE, UBYTE, UBYTE*, UWORD* );
static Bool PinScrATR( UBYTE*, UWORD*, char*, char*, UBYTE );
static Bool PinCardStatus( UBYTE, UBYTE* );
static Bool PinEject( UBYTE );
#endif	// MAKE_EMV
static UBYTE TryPinPad(PINDATA* tekProt,UBYTE PinPadType, Bool bQuickSearch);
extern Bool GetPinFromInternalPP( void );
extern void SaveEncPin( void );
//=============================================================================
// Public data definitions
//=============================================================================
UBYTE PINBUF[S_PINBUF];
UBYTE OUTPIN[128];
UBYTE *pOUTPIN;
UWORD RecLen;
UWORD pinlen;
OSRETURNCODES Status;
UBYTE length;
struct KEKREC *pKEK_Key_Address;
UBYTE WorkData[8];
UBYTE pinblock[13];
UBYTE PowerOnFlag  ;

//=============================================================================
// Private data definitions
//=============================================================================

static const ENTRY_STR PinEntry = {
	N_Pinpad,					// Title
	CustomMsg,					// Prompt
	EnterPin,					// Default value
};

static const ENTRY_STR ContEntry = {
	N_NullStr,
	CustomMsg,
	CustomMsg2,
};

//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//  PinInit         Initialize the pin pad.
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        None
//  Notes:          
//                  
//-----------------------------------------------------------------------------
extern void PinInit( void )
{
	Bool DetectFlag;

	/* Get PIN Pad Detect Flag */
	DetectFlag = ( 0xFF == TERM.TERMInit ) && ( TCONF.TCOPT1 & TC1_PINDETECT );
        
	// Force pin type to invalid.
	PIN_TYPE = PIN_TYPE_INVALID;

	// Clear all bits in PINSTAT
	PINSTAT = 0;

	// Reset the Pin Pad.
	PinReset(  );

	/* Check detect flag */
	if ( DetectFlag )
	{
		// Clear PIN Pad display
		PinMessage( Msg_ClrScrn );
	}

	Rewind_Pin( );

}


//-----------------------------------------------------------------------------
//  PinReset        Reset the pin pad. 
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        None
//  Notes:
//
//-----------------------------------------------------------------------------
extern void PinReset( void )
{
	Rewind_Pin( );

	// Reset Bits of PINSTAT
	PINSTAT &= ~PinCardReadReqPend;
	PINSTAT &= ~PinWaitUserAction;
}

static void  GetFS( void )
{
	*( pOUTPIN ) = '\x1C';
	pOUTPIN++;
}


//-----------------------------------------------------------------------------
//  GetEncKey       Get Encryption working Key 
//
//  Parameters:     UBYTE* - pointer to encrypted key
//                  short  - key length
//                  Bool   - ASCII data flag
//  Global Inputs:
//  Returns:        None
//  Notes:          Put the Key into output buffer
//
//-----------------------------------------------------------------------------
void GetEncKey( UBYTE* keyptr, short len, Bool bASCII )
{
	UBYTE tmpaqbuffer[S_AQWORKKEY + S_AQWORKKEY];

	/* Copy key from aquirer table */
	MoveItAq( tmpaqbuffer, keyptr, len );



	if ( bASCII )
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

//-----------------------------------------------------------------------------
//  GetEncKeyId     Get Encryption working Key ID
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        UBYTE - key ID
//  Notes:          
//
//-----------------------------------------------------------------------------
static UBYTE GetEncKeyId( void )
{
	UBYTE AqMKID;

	AqMKID = TRINP.TRAQPTR->AQMKID;

	if ( ((AqMKID < '0') || (AqMKID > '9')) && (AqMKID != 'A') && (AqMKID != 'B') )
	{
		return ('0');
	}

	return (AqMKID);
}

//-----------------------------------------------------------------------------
//  GetPinPrompt    Get Pin Prompt
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        None
//  Notes:          
//
//-----------------------------------------------------------------------------
static void  GetPinPrompt( void )
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

	memcpy( pOUTPIN, CSTMSG2, 16);
	pOUTPIN += 16;
}

//-----------------------------------------------------------------------------
//  GetPinMsg       Retrieve a message from PIN table, and
//                  store it in a user buffer
//
//  Parameters:     enum msg_id Mid     message index in the PIN table.
//
//                  char *pBuf          pBuf buffer to return the message
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:          Use MLSemaphore.
//
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
//  PreparePinCalc  Clear PINBUF, OUTPIN and initialize pointer into OUTPIN
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        None
//  Notes:          
//
//-----------------------------------------------------------------------------
static void  PreparePinCalc( void )
{
	// Clear PINBUF, OUTPIN and initialize pointer into OUTPIN.
	memset( ( UBYTE * ) PINBUF, 0, S_PINBUF );
	memset( ( UBYTE * ) OUTPIN, 0, sizeof( OUTPIN ) );
	pOUTPIN = OUTPIN;
}



//-----------------------------------------------------------------------------
//  PinGetPin       Get the PIN from the pin pad.
//
//  Parameters:     useGenerateKey - Use temporary generated key
//                  pPin - pointer to output buffer (place for decrypted pin) 
//                         Used only for case with temporary key;
//  Global Inputs:
//  Returns:        None
//  Notes:          True if OK, False otherwise
//                  Notes: The S7 will retransmit both packets upon
//                  CLEAR Key, S8 does not retransmit.      
//
//-----------------------------------------------------------------------------
extern Bool PinGetPin( Bool useGenerateKey )
{
	UBYTE status, mkid;
	Bool loopflag, DUKPTflag, GotClear;
	UBYTE *WorkingKeyAddr;
	short WorkingKeyLen;							
	UBYTE tmpaqbuffer[S_AQWORKKEY + S_AQWORKKEY];	
	Bool bRetVal;
	UBYTE EncGenKey[CS_DOUBLE_KEY_SIZE];

	// May need to add a Get PIN Semaphore in the future

	bRetVal = False;

	// Set DUKPT flag
	DUKPTflag = (TRINP.TRAQPTR->AQOPT4) & AQ4_DUKPT;

	memcpy (EncGenKey, TRINP.TRAQPTR->AQWORKKEY, CS_DOUBLE_KEY_SIZE);
	// if we need use generated key
	if (useGenerateKey)
	{

		DUKPTflag = False;
		if( 1 != CS_GenerateKey ( (GetEncKeyId() - '0'), EncGenKey ) ) // OK means 1 for CS_
		{
			return False;
		}
	} 



	// Open and configure Pin Pad Port
	if ( True == Open_Config(  ) )
	{
		if ( PIN_TYPE_INTERNAL != PIN_TYPE )
		{
                      // Display PINPAD in title bar.
			GetMsg( N_Pinpad, Titlebuf );
			User_Msgs[1] = WaitForPin;

			// Do not draw Numeric Keypad on ICE 4000
			Enable_Flags[0] = 0;
			DrawScreen( STATUS_SCR );

			// Remove PINPAD from title buffer.
			Titlebuf[0] = 0;

			// Loop for CLEAR Key Pressed.
			do
			{
				// Clear PINBUF, OUTPIN and initialize pointer into OUTPIN.
				PreparePinCalc();

				switch(PIN_TYPE)
				{
				case PIN_TYPE_S9:
					if (DUKPTflag)
					{
						// Prompt for PIN
						// Send message to pinpad to display
						GetPINData("3C.",3); // Load the Message Type (Write Text on PINPAD)
						GetPinPrompt();  // Get pin prompt. Either 'PIN?   $9.99' or 'REENTER PIN'
						if (True != Write_Pin( ( UWORD ) ( pOUTPIN - OUTPIN ),ONESECOND*10  )) //Send Text
						{
							Close_Pin();
							return False;
						}

						// Clear PINBUF, OUTPIN and initialize pointer into OUTPIN.
						PreparePinCalc();

						GetPINData("70",2); // Pin request (DUKPT)

						GetPAN(19,False,False); // 19 digits pan

						GetFS(); // Set FS in buffer
		
						GetPINByte('D'); // Load 'D' for Debit
				
						GetTotam(); // Set Amount to buffer
						break;
					}
					else
					{
						GetPINData("2032.",5);  // Load the Message Type - PIN Entry Request (Double key)
		
						// Copy master key id from acquirer table.
						GetPINByte(GetEncKeyId());

						GetPAN(18,True,False); // 18 digits pan
					
						GetFS(); // Set FS in buffer
						
						if (((TRINP.TRAQPTR->AQOPT4) & AQ4_DBLPINKEY) || useGenerateKey )
						{
							GetPINByte('2'); // Send Encrypting KEY Lenght (Double)
							// Move encrypted session key to output buffer.
							// Double key
							if (useGenerateKey)
								GetEncKey( (UBYTE*)	EncGenKey , CS_DOUBLE_KEY_SIZE , True );
							else
								GetEncKey( TRINP.TRAQPTR->AQWORKKEY, S_AQWORKKEY + S_AQWORKKEY, True );
						}
						else
						{
							// DOes not work  with 1 (S9 PINPAD error) use twice same key for single DES
							GetPINByte('1'); // Send Encrypting KEY Lenght (Single)
							// Move encrypted session key to output buffer.
							// Single key
							GetEncKey( TRINP.TRAQPTR->AQWORKKEY, S_AQWORKKEY, True );
						}


						// Get pin prompt. Either 'PIN?   $9.99' or 'REENTER PIN'
						GetPinPrompt();

						GetPINData("12",2); // Send Max. PIN Digits (12 digits)

						GetPINByte('0'); // Send Enter Key required flag (Enter key is requered)

						GetFS(); // Set FS in buffer

						GetPINData("                ",16); //  Send display data, line 2

						GetFS(); // Set FS in buffer

                                                GetPINData("00",2);// TimeOut
						break;
					}
	
				case PIN_TYPE_HFT106:
					if (DUKPTflag)
					{
						// Load the Message Type - DUKPT PIN Entry Request 
						GetPINData( "A0.",3 );
			
						GetPAN(19,False,False); // 19 digits pan

						GetFS(); // Set FS in buffer

 						GetPINByte('D'); // Load 'D' for Debit

						GetTotam(); // Set Amount to buffer
						
						GetFS(); // Set FS in buffer

						GetPinPrompt();  // PIN USDXXXX
						break;
					}
					else
					{
						// Load the Message Type - PIN Entry Request 
						GetPINData( "32.",3 );

						// Copy master key id from acquirer table.
						GetPINByte(GetEncKeyId());

						GetPAN(18,True,False); // 18 digits pan

						GetFS(); // Set FS in buffer

						// Move encrypted session key to output buffer.
						// Single or Double key
						if (useGenerateKey)
							GetEncKey( (UBYTE*)	EncGenKey , CS_DOUBLE_KEY_SIZE , True );
						else
						{
							if ((TRINP.TRAQPTR->AQOPT4) & AQ4_DBLPINKEY)
								GetEncKey( TRINP.TRAQPTR->AQWORKKEY, S_AQWORKKEY + S_AQWORKKEY, True);
							else
								GetEncKey( TRINP.TRAQPTR->AQWORKKEY, S_AQWORKKEY, True );
						}

						// Get pin prompt. Either 'PIN?   $9.99' or 'REENTER PIN'
						GetPinPrompt();

						GetPINData("12",2); // Send Max. PIN Digits (12 digits)		
				
						GetPINByte('0'); // Send Enter Key required flag (Enter key is requered)

						GetFS(); // Set FS in buffer

						GetPINData("                ",16); //  Send display data, line 2

						GetFS(); // Set FS in buffer
						break;
					}
#ifdef	MAKE_HFT_P1100
				case PIN_TYPE_P1100:
					if (DUKPTflag)
					{
						// Set next Dukpt key
						if (HFT_IncEncCount() == False)
						{
							Close_Pin(  );
							return False;
						}
						// Load the Message Type - Display Text Request
						PreparePinCalc();
						GetPINByte(0x41); 	// Request PIN entry (Dukpt)
					}
					else
					{
						// Load encrypted work keys
						if (HFT_LoadPINKey() == False)
						{
							Close_Pin(  );
							return False;
						}
						// Load the Message Type - Display Text Request
						PreparePinCalc();
						GetPINByte(0x51); 	// Request PIN entry 
					}
					GetPINByte(0); 		// Size of data more 256 byte
					GetPINByte(103); // Size of data always send 64 - blank non-text lines
					GetPINByte('2'); 	// Row 2
					GetPINData("16",2); 	// Position 16 
					GetPINByte(0x32); 	// Blink Cursor
					GetPINByte(0x31); 	//  echo_direction - 30 from left 31 from right
					GetPINByte(0x30); 	//  echo_host 0 no echo - 31 echo to host
					GetPINData("04",2); 	// Min PIN lenght (4 digits)
					GetPINData("12",2); 	// Max PIN lenght (12 digits)
					GetPINByte(0x31); 	// ENT-key is used to terminate the PIN entry 
					GetPAN(12,True,True);	// Last 12 digits in BCD format

					GetPINByte(0x1F); 	// Text Separator (Start Text Area)
					GetPINByte('1'); 	// ROW number for text (1 row)

					GetPinPrompt();
     					// blank the non-message lines - always send max
					memset (pOUTPIN, ' ', 64-16);
					pOUTPIN = pOUTPIN + 64-16;
					GetPINByte(0x1F); 	// Text Separator (End Text Area)
					GetPINByte('1'); 	// ROW number for text (1 row)
					memset (pOUTPIN, ' ', 16);
					pOUTPIN = pOUTPIN + 16;
					GetPINByte(0x1F); 	// Text Separator (End Text Area)
					// Copy master key id from acquirer table.
					GetPINByte(GetEncKeyId() - '0');

					break; 
#endif	//MAKE_HFT_P1100
				  
				}

				loopflag = False;

				// Loop so user can clear and terminal will refresh screen.
				do
				{
					// Clear GotClear each time through the loop else will have
					// problem if clear on pinpad and then CANCEL_KY or CLEAR
					// is pressed on the terminal.
					GotClear = False;

					// Write message and read responce to Pin Pad
					if ( True != Write_Read_Pin( ( UWORD ) ( pOUTPIN - OUTPIN ), ONEMINUTE*5, True ) )
					{                                                       
						Close_Pin(  );                                                
                                                PIN_TYPE = PIN_TYPE_INTERNAL;
                                                bRetVal = GetPinFromInternalPP();
						return False;
					}
					bRetVal = True;

					switch(PIN_TYPE)
					{
					case PIN_TYPE_S9:

						// Check DUKPT flag
						if ( DUKPTflag )
						{
							// CHECK for responce code is correct
							if ( memcmp( PINBUF, ( UBYTE * ) "71", 2 ) != 0)
							{
								Close_Pin(  );
								return False;
							}

							// CHECK for clear key pressed
							if ( memcmp( PINBUF, ( UBYTE * ) "716", 3 ) == 0)
							{
								GotClear = True;
								break;
							}
						     	// ADD here Function key processing

							// CHECK for pin was entered code
							if (memcmp( PINBUF, ( UBYTE * ) "710", 3 ) == 0)
							{
								// GET Key serial number
								SaveSerialNumber();
								// Make sure that the KSN was set, else send again.
								if ( NullComp( ( char * ) TRINP.TRKSN, S_TRKSN ) )
								{
									// Nothing in TRKSN.
									loopflag = True;
									bRetVal = False;
								}
								else
								{
									// Get encrypted PIN Block
									SaveASCIPINBlock();
								}
							}	
							else
							{
								Close_Pin(  );
								return False;
							}
							break;
						}
						else
						{
							// CHECK for responce code is correct
							if ( memcmp( PINBUF, ( UBYTE * ) "33", 2 ) != 0)
							{
								Close_Pin(  );
								return False;
							}
							// CHECK for clear key was pressed
							if ( PINBUF[4] == '6')
							{
								GotClear = True;
								break;
							}
						    // ADD here Function key processing
							

							// Check for PIN was entered and 
							if ((PINBUF[4] == '0') && (memcmp( &PINBUF[5], ( UBYTE * ) "00", 2 ) != 0))
							{
								SavePINBlock();
							}
							else
							{
								Close_Pin(  );
								return False;
							}
							break;
						}
					case PIN_TYPE_HFT106:

						// Check DUKPT flag
						if ( DUKPTflag )
						{
							// CHECK for responce code is correct
							if ( memcmp( PINBUF, ( UBYTE * ) "A1", 2 ) != 0)
							{
								Close_Pin(  );
								return False;
							}

						     	// ADD here Function key processing

							// CHECK for pin was entered code
							if (memcmp( PINBUF, ( UBYTE * ) "A10", 3 ) == 0)
							{
								// GET Key serial number
								SaveSerialNumber();
								// Make sure that the KSN was set, else send again.
								if ( NullComp( ( char * ) TRINP.TRKSN, S_TRKSN ) )
								{
									// Nothing in TRKSN.
									loopflag = True;
									bRetVal = False;
								}
								else
								{
									// Get encrypted PIN Block
									SaveASCIPINBlock();
								}
							}	
							else
							{
								Close_Pin(  );
								return False;
							}
							break;
						}
						else
						{
							// CHECK for responce code is correct
							if ( memcmp( PINBUF, ( UBYTE * ) "33", 2 ) != 0)
							{
								Close_Pin(  );
                                                                PinMessage( Msg_ClrScrn );
								return False;
							}
						     	// ADD here Function key processing

							// Check for PIN was entered and 
							if ((PINBUF[4] == '0') && (memcmp( &PINBUF[5], ( UBYTE * ) "00", 2 ) != 0))
							{
								SavePINBlock();
							}
							else
							{
								Close_Pin(  );
                                                                PinMessage( Msg_ClrScrn );
								return False;
							}
							break;
						}

#ifdef	MAKE_HFT_P1100
					case PIN_TYPE_P1100:
						
						if (DUKPTflag)
						{
							// Check for responce code
							if (0x41 != PINBUF[0])
							{
								Close_Pin(  );
								return False;
							}
							// Check for Clear key was pressed
							if (0x70 == PINBUF[2] )
							{
								GotClear = True;
								break;
							}
							// Place for add additional error code processing

							// PIN was entered
							if ((0x00 == PINBUF[2]) && (0x00 == PINBUF[1])  )
							{
								// Check for size of data
								if (0x12 != PINBUF[4])
								{
									Close_Pin(  );
									return False;
								}
								// Save PIN Block
								memcpy(TRINP.TRPINBL, ( char * ) &PINBUF[5], S_TRPINBL);
								// Save Key Serial Number
								memcpy(TRINP.TRKSN, ( char * ) &PINBUF[13], S_TRKSN);
							}

							else
							{
								Close_Pin(  );
								return False;
							}
						}
						else
						{
							// Check for responce code
							if (0x51 != PINBUF[0])
							{
								Close_Pin(  );
								return False;
							}
							// Check for Clear key was pressed
							if (0x70 == PINBUF[2] )
							{
								GotClear = True;
								break;
							}
							// Place for add additional error code processing

							// PIN was entered
							if ((0x00 == PINBUF[2]) && (0x00 == PINBUF[1])  )
							{
								// Check for size of data
								if (0x8 != PINBUF[4])
								{
									Close_Pin(  );
									return False;
								}
								// Save PIN Block
								memcpy(TRINP.TRPINBL, ( char * ) &PINBUF[5], S_TRPINBL);
							}
							else
							{
								Close_Pin(  );
								return False;
							}
						}
						break;
#endif	//MAKE_HFT_P1100			
					}
//#ifdef MAKE_PINATTACKDELAY
					// Setup the pin attack delay
//					PinAttackDelay( TRINP.TRPAN, TRINP.TRPINBL,  0 );
//#endif // MAKE_PINATTACKDELAY
				}
				while ( loopflag );

			}
			while ( GotClear );


			// Close Pin Pad Port
			if ( True != Close_Pin(  ) )
			{
				bRetVal = False;
			}
		}
		else
		{
			bRetVal = GetPinFromInternalPP();
		}
	}

	// May need to add a Release PIN Semaphore
	if ( bRetVal  && useGenerateKey )
	{       
          
		if( 1 != CS_DecryptPIN( (GetEncKeyId() - '0'), EncGenKey, (BYTE *) WorkData, TRINP.TRPINBL) ) // OK means 1 for CS_
			return False;
		GetPinFromPinblock();
		BfAscii( Dspbuf, &WorkData[1], 7);		
		char *p=(char *)memchr(Dspbuf, 'F', 12); // Set End of Pin code
		if (p!=NULL)
		{
			*p= 0;
		}
		memset( WorkData, 0, sizeof(WorkData) );
		memset( TRINP.TRPINBL, 0, S_TRPINBL );
	}

	return bRetVal;
}


//-----------------------------------------------------------------------------
//  PinMessage      Build message and send it to the Pin Pad.
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        None
//  Notes:          True if OK, False otherwise
//                  Message to display in PinMsgNumber.
//-----------------------------------------------------------------------------
extern Bool PinMessage( enum msg_id PinMsgNumber )
{
	Bool bRetVal;
	// May need to add a Get PIN Semaphore in the future
	UBYTE MaxDigits;
	bRetVal = False;

	// Open and configure Pin Pad Port
	if ( True != Open_Config(  ) )
		return False;
	//	If internal pinpad return True
	if ( PIN_TYPE_INTERNAL == PIN_TYPE )
		return True;

	// Load the Message Type - Display Text Request
	PreparePinCalc();
			

#ifdef	MAKE_HFT_P1100
	if (PIN_TYPE_P1100 == PIN_TYPE)	 // @AAMELIN
	{
		MaxDigits = 64;		// 4 Row by 16 Digits
		GetPINByte(0x32); 	// Send text Command
		GetPINByte(0); 		// Size of data more 256 byte
		GetPINByte(MaxDigits+4); // Size of data always send 64 - blank non-text lines
		GetPINByte('1'); 	// Row 1
		GetPINData("01",2); 	// Position 1
		GetPINByte('0'); 	// No Flash
	}
	else
	{
#endif	//MAKE_HFT_P1100	
		MaxDigits = 16;
		GetPINData( "3C.",3 );
#ifdef	MAKE_HFT_P1100
	}
#endif	//MAKE_HFT_P1100	

	// blank the non-message lines - always send max
	memset (pOUTPIN, ' ', MaxDigits);
	// Load text message into buffer after message type ( 16 Bytes)
	GetPinMsg( PinMsgNumber, CSTMSG );
	length = StrLn( CSTMSG, 16 );
	memcpy( pOUTPIN, CSTMSG, ( UWORD ) length );
	pOUTPIN += MaxDigits;

	// Write message to Pin Pad
	bRetVal = Write_Pin( ( UWORD ) (pOUTPIN - OUTPIN ), ONESECOND );

	// Close Pin Pad Port
	if ( True != Close_Pin(  ) )
	{
		bRetVal = False;
	}

	return bRetVal;
}



//-----------------------------------------------------------------------------
//  PinDisplayDiag  Display version information received from Pin PAd.
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        None
//  Notes:          True if OK, False otherwise
//-----------------------------------------------------------------------------
extern Bool PinDisplayDiag( void )
{
	UBYTE retval;
	char LoopFlag = True;
	Bool bRetVal;

	// May need to add a Get PIN Semaphore in the future

	bRetVal = False;

	// Open and configure Pin Pad Port
	if ( True == Open_Config(  ) )
	{
		// Set version screen Title
		GetMsg( N_Version, CSTMSG );
		// Display text line 1
		Enable_Flags[3] = 1;

		if ( PIN_TYPE_INTERNAL != PIN_TYPE )
		{
			// Load the Message Type - Software Revision Inquiry Request
			// clear the input pin buffer, PINBUF.
			PreparePinCalc();
			
			GetPINData( "90.",3 );

			// Write message to Pin Pad
			// Read message from Pin Pad
			if ( True == Write_Read_Pin( (UWORD)(pOUTPIN - OUTPIN), ONESECOND * 5, False) )
			{
				bRetVal = True;
				if ( memcmp( PINBUF, ( UBYTE * ) "91", 2 ) )
				{
					// Did not receive correct response code.
					bRetVal = False;
				}
				else
				{
					// Load the Revision text into the buffer "EXTERNAL: "
					GetMsg( Msg_External, CSTMSG2 );
					length = StrLn( CSTMSG2, 16 );

					// Get revision and load it into display buffer
					memcpy( &CSTMSG2[length], &PINBUF[3], 10 );
					length = length + 10;
					CSTMSG2[length] = 0;

					// Load the Checksum Text "CKSUM: "
					GetMsg( Msg_Chksum, CSTMSG3 );
					length = StrLn( CSTMSG3, 16 );

					// Get checksum and load it into display buffer
					memcpy( &CSTMSG3[length], &PINBUF[13], 4 );
					length = length + 4;
					CSTMSG3[length] = 0;
				}
			}
		}
		else
		{
			// Open CryptoService
			CS_Open( False );

			// Test the internal PIN PAD
			if ( CS_StatusSSM() )
			{
				bRetVal = True;

				// Load the Revision text into the buffer "INTERNAL: "
				GetMsg( Msg_Internal, CSTMSG2 );
				length = StrLn( CSTMSG2, 16 );

#if defined _TERM_325
				memcpy( &CSTMSG2[length], "SC&K 325  ", 10 );
#elif defined _TERM_328
				memcpy( &CSTMSG2[length], "SC&K 328  ", 10 );
#else
				memcpy( &CSTMSG2[length], "SC&K xxx  ", 10 );
#endif
				length = length + 10;
				CSTMSG2[length] = 0;

				// Load the Checksum Text "CKSUM: "
				GetMsg( Msg_Chksum, CSTMSG3 );
				length = StrLn( CSTMSG3, 16 );

				memcpy( &CSTMSG3[length], "----", 4 );
				length = length + 4;
				CSTMSG3[length] = 0;
			}
			else
			{
				ShowErrMsg( PinpadNotFound );
			}
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

	if ( PIN_TYPE_INTERNAL != PIN_TYPE )
	{
		if ( True != Close_Pin(  ) )
		{
			bRetVal = False;
		}
	}

	return bRetVal;
}



//-----------------------------------------------------------------------------
//  PinCardRead     Get Card Read from the external Pin Pad.
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        None
//  Notes:          True if OK, False otherwise
//-----------------------------------------------------------------------------
extern Bool PinCardRead( void )
{
	// May need to add a Get PIN Semaphore in the future
	// Reset Smart Card read flag
	PINSTAT &= ~PinSmartCard_Read;
	// Set Max Lenght for receive buffer
	pinlen = S_PINBUF;

	// Open and configure Pin Pad Port
	if ( False == Open_Config(  )  )
	{
		PinReset();
		return False;
	}

	// Clear PINBUF, OUTPIN and initialize pointer into OUTPIN.
	PreparePinCalc();
	memset(CSTMSG,' ',16); // Fill Upper string by spaces
	memset(CSTMSG2,' ',16); // Fill Upper string by spaces
	// Make Pin data
	switch ( PIN_TYPE )
	{
	case PIN_TYPE_S9:
	case PIN_TYPE_HFT106:

		GetPINData("3A.",3); // Message Type PinCard request
		if ( TCONF.TCOPT7 & TC7_PINMSR )
		{
			GetPINByte('1'); // Read Track 1
			GetPINByte('1'); // Read Track 2
			GetMsg( Msg_SwipeYourCard, CSTMSG );
		}
		else
		{
			GetPINByte('0'); // Do not Read Track 1
			GetPINByte('0'); // Do not Read Track 2
		}
		if ( (TCONF.TCOPT7 & TC7_PINSCR) && ( PIN_CONFIG->Protocol != pp_CAT33) )
		{
			GetPINByte('1'); // Read SCR
			GetMsg( Msg_InsertYourCard, CSTMSG2 );
		}
		else
		{
			GetPINByte('0'); // Do not Read SCR
		}
		// Skip over two reserved postions.
		pOUTPIN += 2;
		GetPINData( CSTMSG, 16); // OutPut Upper string
		// Set file separator character in buffer
		GetFS();
		GetPINData( CSTMSG2, 16); // OutPut Lower string
		// Set file separator character in buffer
		GetFS();
		break;
#ifdef	MAKE_HFT_P1100
	case PIN_TYPE_P1100:
		if ( TCONF.TCOPT7 & TC7_PINSCR )
		{
			GetPINByte(0x70); // Read ICC card command
			GetPINByte(0x00); // Set lenght 00 (it will be set at the end)
			GetPINByte(0x00); // Set lenght 00 (it will be set at the end)
			GetMsg( Msg_InsertYourCard, CSTMSG ); // Get Message insert your card
			if ( TCONF.TCOPT7 & TC7_PINMSR )
			{
				GetMsg( Msg_SwipeYourCard, CSTMSG2 ); // Get Message swipe your card
				GetPINByte(0x34); // Read ICC and track 1 and track 2 
			}
			else
			{
				GetPINByte(0x30); // Read ICC only 
			}
			GetPINByte(0x31); // EMV mode
		}
		else
		{
			GetPINByte(0x34); // Read magnetic stripe command
			GetPINByte(0x00); // Set lenght 00 (it will be set at the end)
			GetPINByte(0x00); // Set lenght 00 (it will be set at the end)
			GetPINByte(0x34); // Read track 1 and track 2 
			GetMsg( Msg_SwipeYourCard, CSTMSG ); // Get Message swipe your card
		}
		GetPINByte(0x1F); 	// Text Separator (Start Text Area)
		GetPINByte('1'); 	// ROW number for text (1 row)
		GetPINData( CSTMSG, 16); // OutPut Upper string
		GetPINData( CSTMSG2, 16); // OutPut Lower string
		GetPINByte(0x1F); 	// Text Separator (Start Text Area 2)
		memset(CSTMSG,' ',32); // Fill string by spaces
		GetPINByte('1'); 	// ROW number for text (1 row)
		GetPINData( CSTMSG, 32); // Clear screen after card inserted
		GetPINByte(0x1F); 	// Text Separator (End Text Area)
		SetLenghtP1100();		

		break;
#endif //	MAKE_HFT_P1100

	case PIN_TYPE_INTERNAL:
	case PIN_TYPE_INVALID:
	default:
		PINSTAT &= ~PinCard_Read;
		Close_Pin();
		return False;
	}

	
	// Write message to Pin Pad
	if ( True == Write_Pin( ( UWORD ) ( pOUTPIN - OUTPIN ) ,ONESECOND ) )
	{
		// Write message was successful, now start read.
		// Note that read has no timeout value.
		if ( orvOK ==  SDK_PinRead( PINBUF, &pinlen , ONEMINUTE * 5, &PinCardReadStatus) )
		{
			// Set status showing read is outstanding.
			PINSTAT |= PinCardReadReqPend;
			return True;
		}
	}

	// If somthing wrong when data send
	PinReset();
	PINSTAT &= ~PinCard_Read;
	// Error messages while testing.
	ShowErrMsg( PinReadErr );
	return False;

}


//-----------------------------------------------------------------------------
//  PinCardReadResp     Process card data read from the pin pad.
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            None
//
//  Notes:              True if OK, False otherwise
//
//-----------------------------------------------------------------------------

extern Bool PinCardReadResp( void )
{
	UBYTE i;
	BYTE * pPointer;

	// Don't process unless card was read on external pinpad.
	if ( 0 == ( PINSTAT & PinCardReadReqPend ))
		return False;
	// Close Pin port anyway
	Close_Pin(  );
	// Clear the outstanding read flag.
	PINSTAT &= ~PinCardReadReqPend;
	
	switch ( PIN_TYPE )
	{
	case PIN_TYPE_S9:
	case PIN_TYPE_HFT106:
		if ( 0 != memcmp( PINBUF, ( UBYTE * ) "3B.", 2 ) )
			return False;
		// Received correct response.
		// If track two received OK copy both track to TRINP
		if ( '0' == PINBUF[4] )
		{
			// Save Track1 only if Track 2 Present
			// If track one received OK copy to TRINP.TRTRK1.
			if ( '0' == PINBUF[3] )
			{
				memset( &TRINP.TRTRK1, 0xff, S_TRTRK1);
				i = SaveTrack( 9, TRINP.TRTRK1, pinlen ); //9 - Position of start TRACK1
				Trk1ReadLength = i - 9;
			}
			// copy TRACK2
			memset( &TRINP.TRTRK2, 0xff, S_TRTRK2);
			Trk2ReadLength = SaveTrack( i+1, TRINP.TRTRK2, pinlen );
			Trk2ReadLength = Trk2ReadLength - i - 1;
			return True;
		}
		// If SC with correct ATR Start EMV transaction
		if ( '0' == PINBUF[5] )
		{
			PINSTAT |= PinSmartCard_Read;
			return True;
		}
		break;
#ifdef	MAKE_HFT_P1100

	case PIN_TYPE_P1100:
		if ( TCONF.TCOPT7 & TC7_PINSCR )
		{
			if ( (PINBUF[0] != 0x70) || (PINBUF[1] != 0x00) || (PINBUF[2] != 0x00) )
				return False;
			if ( PINBUF[5] == 0x30 )
			{
				PINSTAT |= PinSmartCard_Read;
				return True;
			}
		}
		else
		{
			if ( (PINBUF[0] != 0x34) || (PINBUF[1] != 0x00) || (PINBUF[2] != 0x00) )
				return False;
		}
		// Save track data
		// Check for track 2 present and OK
		pPointer = memchr( &PINBUF[7], 0x1c, (pinlen - 7) );
		if ( (PINBUF[5] == 0x32) && (PINBUF[6] == 0x00) )
		{
			Trk2ReadLength = SaveTrack( 7, TRINP.TRTRK2, pinlen );
			return True;
		}
		if ( (pPointer != 0) && (*(pPointer+1) == 0x32) && ( *(pPointer+2) == 0x00))
		{
			i = SaveTrack( 5, TRINP.TRTRK1, pinlen );
			Trk2ReadLength = SaveTrack( i+3, TRINP.TRTRK2, pinlen );
			return True;
		}
		break;
#endif //	MAKE_HFT_P1100
	}
	return False;
}

//-----------------------------------------------------------------------------
//  SaveTrack	    Get TRACK data from PINBUF and save to dest buffer
//
//  Parameters:     startPos - start position in PINBUF
//					destBuf - pointer to destination buffer
//					maxLen - maximum lenght of recieved data
//  Returns:        position of end of parse
//  Notes:			
//-----------------------------------------------------------------------------
static UBYTE SaveTrack( UBYTE startPos, UBYTE * destBuf, UBYTE maxLen)
{
	UBYTE i;
	for ( i = startPos; i < maxLen ;i++ )
	{
		// CHECK for separator
		if ( 0x1C == PINBUF[i] )
			return (i);
		*(destBuf + i - startPos) = PINBUF[i];
	}
	return (i);
}

//-----------------------------------------------------------------------------
//  GetPINDevice	    Return device configuration mode
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        True - success, False - error
//  Notes:			Set PIN_DEVICE & PIN_CONFIG variables 
//-----------------------------------------------------------------------------
char GetPINDevice( void )
{
	UBYTE i;
	
	/* Check for terminal restart or initialization */
	if ( PIN_TYPE_INVALID !=  PIN_TYPE )
		return True;
        
        // pin pad ba�l� m� ? 
	if ( TCONF.TCOPT1 & TC1_PINDETECT )
	{
		/* Prepare progress buffer */ // CHANGE TO STRING FROM TABLES @AAMELIN
		strcpy( &CSTMSG2[0], " " );

	 	// Display PIN AUTO DETECTING message.
		GetMsg( PinPadAutoDetect, CSTMSG );
		ShowInfoMsg( CustomMsg2, N_Pinpad );
	
		// Quick Search
		for ( i = 0; ProtList[i].device != 0; i++ )
		{
			PIN_TYPE = TryPinPad( ProtList[i].cfgptr, TCONF.TCPINPAD, True);
			if ( PIN_TYPE != PIN_TYPE_INVALID)
			{
				PIN_CONFIG = ProtList[i].cfgptr;
				if ( (TCONF.TCOPT7 & TC7_PINSCR) || 
					(TCONF.TCOPT7 & TC7_PINMSR) )
				{
					// Set the flag Smart Card Read from Pinpad
					PINSTAT |= PinCard_Read;
				}
				return True;
			}
		}

		/* Prepare progress buffer */
		CSTMSG2[0] = '<';
		memset( &CSTMSG2[1], ' ', sizeof(ProtList)/sizeof(CFGDATA) - 1 );
		CSTMSG2[sizeof(ProtList)/sizeof(CFGDATA)] = '>';
		CSTMSG2[sizeof(ProtList)/sizeof(CFGDATA) + 1] = 0;
		ShowInfoMsg( CustomMsg2, N_Pinpad );

		for ( i = 0; ProtList[i].device != 0; i++ )
		{
			PIN_TYPE = TryPinPad(ProtList[i].cfgptr,TCONF.TCPINPAD, False);
			if ( PIN_TYPE != PIN_TYPE_INVALID)
			{
				PIN_CONFIG = ProtList[i].cfgptr;
				/* Change detect progress */
				CSTMSG2[i + 1] = '*';
				ShowInfoMsg( CustomMsg2, N_Pinpad );
				if ( (TCONF.TCOPT7 & TC7_PINSCR) || 
					(TCONF.TCOPT7 & TC7_PINMSR) )
				{
					// Set the flag Smart Card Read from Pinpad
					PINSTAT |= PinCard_Read;
				}
				return True;
			}
			else
			{
				/* Change detect progress */
				CSTMSG2[i + 1] = '-';
			}
			ShowInfoMsg( CustomMsg2, N_Pinpad );
		}

	}
        
        // internal pinpad 
	if ( 1 )
	{		
          if ( CS_StatusSSM() )
          {
		// Internal pinpad exists.
                PIN_TYPE = PIN_TYPE_INTERNAL;
          }
	}

	if ( PIN_TYPE == PIN_TYPE_INVALID )
	{
		// No external and no internal Z8 detected.
		ShowErrMsg( PinpadNotFound );
		
		/* Exit */
		return False;
	}
	else
		return True;
}

UBYTE TryPinPad( PINDATA* tekProt,UBYTE PinPadType, Bool bQuickSearch)
{
	UBYTE lenght;
	OS_TIMES DelayTime;

	// Build version request
	switch (PinPadType)
	{
	case PIN_TYPE_S9:
	case PIN_TYPE_HFT106:
		if ((pp_CAT33 != tekProt->Protocol ) &&  (pp_POSMINI != tekProt->Protocol ))
			return ( PIN_TYPE_INVALID );
		lenght = 3;
		memcpy ( OUTPIN, "90.", lenght );
		break;
#ifdef	MAKE_HFT_P1100
	case PIN_TYPE_P1100:
		if ( pp_HFT != tekProt->Protocol ) 
			return ( PIN_TYPE_INVALID ); 
		lenght = 3;
		AscHex ( OUTPIN, "200000", lenght );
		break;
#endif //	MAKE_HFT_P1100
/*	case PIN_TYPE_AUTODETECT:
		if ((PIN_CAT33 != *((UBYTE*)ProtList[i].cfgptr) ) &&  (PIN_POSMINI != *((UBYTE*)ProtList[i].cfgptr) ))
		{
			lenght = 3;
			memcpy ( OUTPIN[0], "90.", lenght );
		}
		if (PIN_HFT != *((UBYTE*)ProtList[i].cfgptr) ) )
		{
		}
		break;
*/	}

	SDK_PinClose();
	if ( orvOK != SDK_PinOpen( tekProt, PIN_NO_EOT  ) )
		return( PIN_TYPE_INVALID );
	// Check for wuick search
	if (bQuickSearch)
		DelayTime = ONESECOND;
	else
		DelayTime = ONESECOND * 60;
	// Send version request
	if ( True != Write_Read_Pin(lenght , DelayTime, False))
		return( PIN_TYPE_INVALID );
	// Recive version responce
//	if ( True != Read_Pin(DelayTime , False) )
//		return( PIN_TYPE_INVALID );
	// Recive version responce
	if ( orvOK != SDK_PinClose() )
		return( PIN_TYPE_INVALID );
	// Check version respone
	switch (PinPadType)
	{
	case PIN_TYPE_S9:
	case PIN_TYPE_HFT106:
		if ( 0 == memcmp (PINBUF, "91." , 3) )
			return( PinPadType );
		else
			return( PIN_TYPE_INVALID );
#ifdef	MAKE_HFT_P1100
	case PIN_TYPE_P1100:
		if ( (PINBUF[0] == 0x20) && (PINBUF[1] == 0x00)  )
			return( PinPadType );
		else
			return( PIN_TYPE_INVALID );
#endif //	MAKE_HFT_P1100
	default:
			return( PIN_TYPE_INVALID );

/*	case PIN_TYPE_AUTODETECT:
		if ((PIN_CAT33 != pCurProtc-> ) &&  (PIN_POSMINI != pCurProtc-> ))
		{
			lenght = 3;
			memcpy ( OUTBUF[0], "90.", lenght );
		}
		if (PIN_HFT != pCurProtc-> ) )
		{
		}
		break;
*/	}
}

//-----------------------------------------------------------------------------
//  GetCommand      Get message separator
//
//  Parameters:     char* - Command string
//  Global Inputs:
//  Returns:        None
//  Notes: Set proper message separator in the buffer
//		   '.' - for CAT33 protocol
//		   '/' - for POSMINI protocol
//-----------------------------------------------------------------------------
/*void GetCommand( char* cmd )
{
	memcpy( pOUTPIN, cmd, 2 );
	pOUTPIN += 2;
	
	if (( PIN_CONFIG->Protocol == pp_CAT33) || ( OUTPIN[0] == 'A' ) ) 
		*pOUTPIN = '.';
	else 
		*pOUTPIN = '/';

	pOUTPIN++;
}*/

//-----------------------------------------------------------------------------
//  GetPINData      Add data to end of OUTPIN	  // @AAMELIN
//
//  Parameters:     cData - pointer to Data 
//		    lenght - of Data
//  Returns:        None
//-----------------------------------------------------------------------------
static void  GetPINData( char * pData, int lenght )
{
	memcpy( pOUTPIN, pData, lenght );
	pOUTPIN += lenght;
}

//-----------------------------------------------------------------------------
//  GetPINByte      Add BYTE to end of OUTPIN	  // @AAMELIN
//
//  Parameters:     cBYTE - pointer to Data 
//  Returns:        None
//-----------------------------------------------------------------------------
static void  GetPINByte( char cBYTE )
{
        *pOUTPIN = cBYTE;
	pOUTPIN++;
}

//-----------------------------------------------------------------------------
//  GetTotam      Add Total Amount to end of OUTPIN	  // @AAMELIN
//
//  Parameters:     None
//
//  Returns:        None
//-----------------------------------------------------------------------------
static void  GetTotam( void )
{
	// Convert BCD to ASCII and load it into buffer.
	BfAscii( ( char * ) pOUTPIN, TRINP.TRTOTAM, S_TRTOTAM );
	pOUTPIN += 12;
}

//-----------------------------------------------------------------------------
//  GetPAN      Add PAN to end of OUTPIN	  // @AAMELIN
//
//  Parameters:     MaxLength - MAX Lenght of PAN (GET LAST digits (LSB) )
//		    bRemLast - remove last digit (TRUE - remove, false do not remove)
//		    bSaveBCD - Save in BCD format (TRUE - save in BCD , False - save in ASCI)
//
//  Returns:        None
//-----------------------------------------------------------------------------
static void  GetPAN(UBYTE MaxLength,Bool bRemLast, Bool bSaveBCD)
{
	UBYTE *pSAVEPTR;
	UBYTE RemBytes;

	// Save starting location of PAN in output buffer.
	pSAVEPTR = pOUTPIN;

	// Move PAN to output and convert to ASCII. (Max 18 bytes)
	RemBytes = MovPanAsc( ( char * ) pOUTPIN, ( UBYTE * ) TRINP.TRPAN, MAXPANL );

	// if we need to remove last digit, increment counter of Remove bytes
	if (bRemLast)
		RemBytes++;	

		
	// All PAN digits including the check digit are sent to the external pinpad
	pOUTPIN += ( MAXPANL - RemBytes );

	// if moved Bytes more then max then
	if ((MAXPANL - RemBytes) > MaxLength)
	{
		// Copy necessary count of digits
		memcpy( pSAVEPTR, pOUTPIN - MaxLength, MaxLength );
		pOUTPIN = pSAVEPTR + MaxLength;
	}
	if (bSaveBCD)
	{
		AscHex(pSAVEPTR,pSAVEPTR,(pOUTPIN - pSAVEPTR)/2 );
		pOUTPIN = pSAVEPTR + (pOUTPIN - pSAVEPTR)/2;
	}
}

//-----------------------------------------------------------------------------
//  SavePINBlock      Get PIN block from PINBUF and SAVE  to TRINP.TRPINBL // @AAMELIN
//
//  Parameters:     None
//
//  Returns:        None
//-----------------------------------------------------------------------------
static void  SavePINBlock( void )
{
	// Extract pin block from the message.
	AscHex( TRINP.TRPINBL, ( char * ) &PINBUF[7], S_TRPINBL );
}


//-----------------------------------------------------------------------------
//  SaveSerialNumber      Get Serial number from PINBUF and SAVE  to TRINP.TRKSN // @AAMELIN
//
//  Parameters:     None
//
//  Returns:        None
//-----------------------------------------------------------------------------
static void  SaveSerialNumber( void )
{
	int i;
	UBYTE KSN_Size, j, TempByte, TempByte1;

	// Get Length of packet received from PIN PAD
	KSN_Size = strlen( ( char * ) PINBUF );

	// Get Length of Key Serial Number.
	// Subtract 3 byte header and 16 byte pin block.
	KSN_Size = KSN_Size - 19;

	// External pinpad does not send the leading F's of a KSN
	// Preset KSN to all F's
	memset ( TRINP.TRKSN, 0xFF, S_TRKSN );

	// Pack the data right justified for sending to a host.
	// Adjust values for use as indexes
	for ( i = KSN_Size - 1, j = S_TRKSN - 1; i >= 0; i -= 2, j-- )
	{
		// Process backwards from end of KSN to end of TRKSN field
		TempByte = A2hex ( PINBUF[ i + 3 ] );
											
		// Adjust to previous byte if any bytes left
		if ( ( i - 1 ) >= 0 )
		{
			TempByte1 = A2hex ( PINBUF[ i -1 + 3 ] );
			TempByte1 <<= 4;
			TempByte |= TempByte1;
		}
		else
		{
			TempByte |= 0xF0;
		}

		TRINP.TRKSN[ j ] = TempByte;
	}

}

//-----------------------------------------------------------------------------
//  SaveSerialNumber      Get PIN Block from PINBUF and SAVE  to TRINP.TRPINBL // @AAMELIN
//
//  Parameters:     None
//
//  Returns:        None
//-----------------------------------------------------------------------------
static void  SaveASCIPINBlock( void )
{
	UBYTE KSN_Size;

	// Get Length of packet received from PIN PAD
	KSN_Size = strlen( ( char * ) PINBUF );

	KSN_Size = KSN_Size - 16;

	// Extract pin block from the message.
	AscHex( TRINP.TRPINBL, ( char * )&PINBUF[( KSN_Size )], S_TRPINBL );

	/* Set PIN Block ready flag */
	TRINP.TRSTATUS[1] |= TS2_PINBLOCK;
}
//-----------------------------------------------------------------------------
//  Name:           ClearPinPad    //@AAMELIN
//  Function:       Clear external PINPAD
//  Parameters:     None
//  Globals:        
//  Returns:        
//  Notes:          
//-----------------------------------------------------------------------------
static void ClearPinPad( void )
{
	// Clear PINBUF, OUTPIN and initialize pointer into OUTPIN.
	PreparePinCalc();
	GetPINData("3C.",3);
	if ( True == Write_Pin( ( UWORD ) ( pOUTPIN - OUTPIN ), ONESECOND ) )
	{
		Read_Pin(ONESECOND, False);
	}
	else
	{
		Close_Pin(  );
	}	
}

#ifdef	MAKE_HFT_P1100

//-----------------------------------------------------------------------------
//  Name:           HFT_ParseDARRsp    
//  Function:       Save lenght of HFT pocket
//  Parameters:     None
//  Globals:        OUTPIN 
//  Notes:          
//-----------------------------------------------------------------------------
static void SetLenghtP1100()
{
	UWORD lenght;
	lenght = (UWORD) (pOUTPIN - OUTPIN);
	lenght = lenght - 3; // substract comman and lenght
	OUTPIN[1] = lenght >>8;
	OUTPIN[2] = lenght & 0xff;
}

//-----------------------------------------------------------------------------
//  Name:           HFT_ParseDARRsp    
//  Function:       Parse response to HFT DAR message
//  Parameters:     None
//  Globals:        PINBUF, gets sent to PED
//  Returns:        True or False based on whether successful
//  Notes:          
//-----------------------------------------------------------------------------
/*static Bool HFT_ParseDARRsp( void )
{
	if ((0x20 == PINBUF[0]) && (0x0 == PINBUF[2]))
	{
		// ADD HERE responce information about PINPAD if you need it
		return True;
	}
	else
	{
		return False;
	}
}

static void HFT_BuildDARReq ( void )
{
	OUTPIN[0] = 0x20;
	OUTPIN[1] = 0x00;
	OUTPIN[2] = 0x00;
	pOUTPIN = OUTPIN + 3;
}*/

//-----------------------------------------------------------------------------
//  LoadPINKey      Function load encrypted PIN key to PINPAD (P1100)
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        None
//  Notes:          None
//-----------------------------------------------------------------------------
static Bool HFT_LoadPINKey( void )
{
	GetPINByte(0x66); 		// Send Command (Load PIN key third level)
	GetPINByte(0x00); 		// Size of data more 256 byte
	GetPINByte(0x1A); 		// Size of data (DO not send KCV)
	GetPINByte('1'); 		// Key index 1 - PIN according to ISO 9564 format 0

	// Check for Key lenght
	if ((TRINP.TRAQPTR->AQOPT4) & AQ4_DBLPINKEY )
	{
		// Move encrypted session key to output buffer.
		// Double key
		GetEncKey( TRINP.TRAQPTR->AQWORKKEY, S_AQWORKKEY + S_AQWORKKEY, False);
	}
	else
	{
		// Move encrypted session key to output buffer.
		// Single key twice
		GetEncKey( TRINP.TRAQPTR->AQWORKKEY, S_AQWORKKEY, False);
		GetEncKey( TRINP.TRAQPTR->AQWORKKEY, S_AQWORKKEY, False);
	}

	GetPINData("PINKEY00",8); 		// Name of Key
	GetPINByte(GetEncKeyId() - '0'); 	// Key use number

	// Write message to Pin Pad
	if ( False == Write_Read_Pin( ( UWORD ) (pOUTPIN - OUTPIN ), ONESECOND*10, False ) )
	{
		return False;

	}
	// CHECK for responce from PINPAD
	if ((00 == PINBUF[2]) && (0x66 == PINBUF[0]))
		return True;
	else
		return False;
}

//-----------------------------------------------------------------------------
//  IncEncCount      Increment encryption counter
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        None
//  Notes:          None
//-----------------------------------------------------------------------------
static Bool HFT_IncEncCount( void )
{
	GetPINByte(0x40); 		// Send Command (Load PIN key third level)
	GetPINByte(0x00); 		// Size of data more 256 byte
	GetPINByte(0x01); 		// Size of data 

	GetPINByte(GetEncKeyId() - '0'); 	// Key use number

	// Write message to Pin Pad
	if ( False == Write_Read_Pin( ( UWORD ) (pOUTPIN - OUTPIN ), ONESECOND*10, False ) )
	{
		return False;
	}
	// CHECK for responce from PINPAD
	if ((00 == PINBUF[2]) && (0x40 == PINBUF[0]))
		return True;
	else
		return False;
}

#endif	//MAKE_HFT_P1100


//-----------------------------------------------------------------------------
//  GetMode		    Return device configuration mode
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        UBYTE: PIN_CFG_PROT for PIN device
//						   PRT_CFG_PROT for PRT device
//  Notes:
//-----------------------------------------------------------------------------

//UBYTE GetMode( void )
//{
	/* Set device mode */
//	if ( PIN == PIN_DEVICE ) return PIN_CFG_PROT;
//	else					 return PRT_CFG_PROT;
//} @AAMELIN

//-----------------------------------------------------------------------------
//  Open_Config     Opens and configures the Pin Pad port.
//
//  Parameters:     None
//  Global Inputs:
//  Returns:        Bool    True if successful, False if error
//  Notes:
//-----------------------------------------------------------------------------
extern Bool Open_Config( void )
{
	OSRETURNCODES status;

	/* Already open, return */
	if ( PINFLG.deviceOpen ) return( True );

	/* Check PIN device type */
	if ( PIN_TYPE == PIN_TYPE_INTERNAL )	return ( True );
	
	/* Check PIN device setting */
	if ( !GetPINDevice() ) return ( False );

	/* Check PIN device type again */
	if ( PIN_TYPE == PIN_TYPE_INTERNAL )	return ( True );
	
	/* Try to open the PIN device   */
	status = SDK_PinOpen( PIN_CONFIG ,PIN_NO_EOT); 

	if (status == ERRBSY)  // BUSY
	{
		Rewind_Pin( ); // close, rewind and reopen
		status = SDK_PinOpen( PIN_CONFIG ,PIN_NO_EOT);
	}

	if ( orvOK == status )	
	{
		PINFLG.deviceOpen = 1;	/* Device opened successfully */
		return(True);
	}
	else
	{
		Rewind_Pin( );			/* Error opening & configuring device */
		return(False);
	}
	
}


//-----------------------------------------------------------------------------
//  Write_Pin       Writes contents of OUTPIN to Pin Pad.
//
//  Parameters:     UWORD Length of data that must be in OUTPIN
//  Global Inputs:
//  Returns:        Bool    True if successful, False if error
//  Notes:
//-----------------------------------------------------------------------------
extern Bool Write_Pin( UWORD length, OS_TIMES Timeout )
{
	IOSTATUS Status;
 	if ( PIN_TYPE_INTERNAL == PIN_TYPE )
	{
		/* Internal pinpad */
		return True;
	}
	else
 	{
		if ( orvOK == SDK_PinWrite( OUTPIN, length, Timeout, &Status ) )
		{ 
			while ( Status.CompleteFlag == 0 )
				SDK_RelinqCPU( );
			if ( Status.Error == orvOK )
				return True;

		}
		Rewind_Pin( );
		return False;
	}

}


//-----------------------------------------------------------------------------
//  Read_Pin        Reads message from Pin Pad.
//
//  Parameters:     OS_TIMES timout     Time out value
//  Global Inputs:
//  Returns:        Bool    True if successful, False if error
//  Notes:          Data is returned in global buffer OUTPIN
//-----------------------------------------------------------------------------
extern Bool Read_Pin( OS_TIMES timeout, Bool bCheckCancel )
{
	Bool Retval;
	IOSTATUS Status;
	UBYTE InputType;
	Bool bPinCardStatus;
	Bool bKeyWasOpen;

	RecLen = S_PINBUF;
    bKeyWasOpen = DevFlag | DEV_KBD;
	Retval = False ;

	if ( PIN_TYPE_INTERNAL == PIN_TYPE )
	{
		// Internal T5000 pinpad
	}
	else
	{
		/* Post a read to the pin device */
		if ( orvOK == SDK_PinRead ( PINBUF, &RecLen, timeout, &Status )  )
		{

			/* Wait for timeout (bit 0) or read complete (bit 7) */
			while( Status.CompleteFlag == 0 )
			{
				if (bCheckCancel)
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

				// Give up some cycles while waiting...
				SDK_RelinqCPU(  );
			}	// while()

			if ( Status.Error != orvOK )
			{
				Retval = False;
				SDK_PinReset( );

			}
			else
			{
				Retval = True;			
			}
			if (bCheckCancel)
				// Force a close of still open pad and kbd devices.
				read_io( 0, 0, &bPinCardStatus, &DevFlag );
		}

		if ( True != Retval )
		{
			Rewind_Pin();
		}
	}

	return ( Retval );
}


//-----------------------------------------------------------------------------
//  Close_Pin       Closes Pin Pad.
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True if successful, False if error
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool Close_Pin( void )
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
		if ( orvOK == SDK_PinClose(  ) )
		{
			PINFLG.deviceOpen = 0;
			retval = True;
		}
	}
	else
	{
		// Internal pinpad.
		retval =  True ;
	}

	return ( retval );
}

/*------------------------------------------------------------------
 *  Name:           Rewind_Pin
 *  Description:    Rewinds PIN device
 *				   
 *  Parameters:		None
 *  Return Value:	None
 *  Notes: 			
 *-----------------------------------------------------------------*/
void Rewind_Pin( void )
{
	SDK_PinReset(  );
	SDK_PinClose(  );

	/* Clear 'Device Open' flag */
	PINFLG.deviceOpen = 0;
}


//-----------------------------------------------------------------------------
//  VerifyTag       Verifies tag is legal for requested operation.
//
//  Parameters:     UBYTE RequestedIndex    Index of tag to be tested
//                  UBYTE RequestedTagTag   value to be tested,
//  Global Inputs:
//  Returns:        Bool    True if successful, False if error
//  Notes:          Only one tag type can be tested at a time, i.e.,
//                  test for pin or test for mac, not both at once.
//-----------------------------------------------------------------------------
extern Bool VerifyTag( UBYTE RequestedIndex, UBYTE RequestedTag )
{
	UBYTE Tag;
	CYPHERREGION *pCypherRegion;

	// Get the address of the CYPHER region.
	pCypherRegion = ( CYPHERREGION * ) TERM.TERMCypher.CypAddr;

	// Get the KEK tag for the specified index.
	Tag = pCypherRegion->KEKTable[RequestedIndex].KEKTAG;

	// Test to see if tag from cypher region for specified index is on.
	if ( RequestedTag != ( Tag & RequestedTag ) )
		return ( False );

	// Set the address of the KEK key in pointer for use by caller.
	pKEK_Key_Address =
		( struct KEKREC * ) &pCypherRegion->KEKTable[RequestedIndex].KEKKEY;

	return ( True );
}

//-----------------------------------------------------------------------------
//  FormatPin       Formats the pin and pan for use in pin encryption.
//
//  Parameters:     None
//  Global Inputs:  On entry:   pinlen = number of digits in entered pin
//                              pinblock = ASCII pin digits null terminated
//                              TRINP.TRPAN = PAN as entered from kbd or card    
//  Returns:        None
//  Notes:          Result of pin/pan format is stored in WorkData.
//-----------------------------------------------------------------------------
static void FormatPin( void )
{
	UBYTE FormatPanBuf[10];
	UBYTE i, j;
	UBYTE *pASCII_PAN;

	// Start by filling the pin work buffer with hex FF's.
	memset( WorkData, 0xFF, sizeof( WorkData ) );

	// Move the length of the pin to start of pin work buffer.
	WorkData[0] = ( UBYTE ) pinlen;

	// Convert pin digits to hex and store in pin work buffer.  The
	// result should be one byte length, pin in hex padded with F's.
	// For example, pin length of 5 and pin of "12345" results in
	// 05 12 34 5F FF FF FF FF
	for ( i = 0, j = 1; i < pinlen; i++, j++ )
	{
		// Move 1st nibble to format buffer, keeping F in 2nd nibble.
		WorkData[j] = ( pinblock[i] & 0x0f );
		WorkData[j] <<= 4;
		WorkData[j] |= 0x0f;

		// Quit here if an odd number of bytes in PIN.
		if ( i == pinlen - 1 )
			break;

		// Move to next ASCII input byte.
		i++;

		// Move second nibble to format buffer.
		WorkData[j] &= 0xf0;
		WorkData[j] |= ( pinblock[i] & 0x0f );
	}

	// Shift the PAN 5 nibbles to the right.
	// Original PAN of '44 27 80 26 41 00 47 97 FF FF' should be
	// '00 00 00 00 78 02 64 10 04 79' after shifts.

	// Clear the working buffer for formatting the PAN.
	memset( FormatPanBuf, 0, sizeof( FormatPanBuf ) );

	// Convert the PAN to an ASCII string.
	BfAscii( ( char * ) Dspbuf, TRINP.TRPAN, sizeof( FormatPanBuf ) );

	// Set pASCII_PAN to the end of the PAN data (look for 'F').
	pASCII_PAN = ( UBYTE * ) Dspbuf;
	while ( *pASCII_PAN != 'F' )
		pASCII_PAN++;

	// Now back up two - skip the 'F' we just found, and the check digit.
	pASCII_PAN -= 2;

	// Now walk backwards through the ASCII PAN data, storing 12 nibbles.
	// Stop when we've got 12 nibbles or when we're out of PAN data.

	i = 7;
	while ( ( i > 1 ) && ( pASCII_PAN >= ( UBYTE * ) Dspbuf ) )
	{
		FormatPanBuf[i] = A2hex( *pASCII_PAN );	// Low nibble.
		pASCII_PAN--;

		if ( pASCII_PAN >= ( UBYTE * ) Dspbuf )
		{
			FormatPanBuf[i] |= ( A2hex( *pASCII_PAN ) << 4 );	// High nibble.
			pASCII_PAN--;
		}

		i--;
	}

	// XOR FormatPanBuf with WorkData.
	for ( i = 0; i < sizeof( WorkData ); i++ )
	{
		WorkData[i] = WorkData[i] ^ FormatPanBuf[i];
	}
}

//-----------------------------------------------------------------------------
//  FormatPin       Formats the pin and pan for use in pin encryption.
//
//  Parameters:     None
//  Global Inputs:  On entry:   pinlen = number of digits in entered pin
//                              pinblock = ASCII pin digits null terminated
//                              TRINP.TRPAN = PAN as entered from kbd or card    
//  Returns:        None
//  Notes:          Result of pin/pan format is stored in WorkData.
//-----------------------------------------------------------------------------
extern void GetPinFromPinblock( void )
{
	UBYTE FormatPanBuf[10];
	UBYTE i;
	UBYTE *pASCII_PAN;


	// Shift the PAN 5 nibbles to the right.
	// Original PAN of '44 27 80 26 41 00 47 97 FF FF' should be
	// '00 00 00 00 78 02 64 10 04 79' after shifts.

	// Clear the working buffer for formatting the PAN.
	memset( FormatPanBuf, 0, sizeof( FormatPanBuf ) );

	// Convert the PAN to an ASCII string.
	BfAscii( ( char * ) Dspbuf, TRINP.TRPAN, sizeof( FormatPanBuf ) );

	// Set pASCII_PAN to the end of the PAN data (look for 'F').
	pASCII_PAN = ( UBYTE * ) Dspbuf;
	while ( *pASCII_PAN != 'F' )
		pASCII_PAN++;

	// Now back up two - skip the 'F' we just found, and the check digit.
	pASCII_PAN -= 2;

	// Now walk backwards through the ASCII PAN data, storing 12 nibbles.
	// Stop when we've got 12 nibbles or when we're out of PAN data.

	i = 7;
	while ( ( i > 1 ) && ( pASCII_PAN >= ( UBYTE * ) Dspbuf ) )
	{
		FormatPanBuf[i] = A2hex( *pASCII_PAN );	// Low nibble.
		pASCII_PAN--;

		if ( pASCII_PAN >= ( UBYTE * ) Dspbuf )
		{
			FormatPanBuf[i] |= ( A2hex( *pASCII_PAN ) << 4 );	// High nibble.
			pASCII_PAN--;
		}

		i--;
	}

	// XOR FormatPanBuf with WorkData.
	for ( i = 0; i < sizeof( WorkData ); i++ )
	{
		WorkData[i] = WorkData[i] ^ FormatPanBuf[i];
	}
}

#if defined(MAKE_ISOMAC) 
//-----------------------------------------------------------------------------
//  PinGenMac       Mac the message.  If internal ICE pinpad is being used
//                  this function will use format that works on ver 1.17.
//                  For internal the master keys are stored encrypted in
//                  the KEK table external to the Z8.
//
//  Parameters:     
//                  PinMacId      - master key index to be used - ASCII.
//                  pMacKeyPtr    - Pointer to MAC key.
//                  pMacDataPtr   - Pointer to data to be MAC'ed. 
//                  PinByte       - Page where data resides that is to be MAC'ed. 
//                  MacDataLength - Length of data to be MAC'ed.
//
//  Returns:        True if OK, False otherwise.None
//                  PinData -  Results of MAC.
//
//  Notes:          This function is cross callable.
//                  This function only for master session encryption.
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
	if ( True == Open_Config(  ) )
	{
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
			GetPINData( "86.",3 );

			// Mac id from acquirer table is passed in.  Default to one.
			if ( ((PinMacId < '0') || (PinMacId > '9')) && (PinMacId != 'A') && (PinMacId != 'B') )
				// If null, blank or ascii 0 then default to 1
				*pOUTPIN = '0';
			else
				// Master key id is ASCII in the acquirer table.
				*pOUTPIN = PinMacId;
			pOUTPIN++;

			// Move encrypted session mac key to output buffer.
			GetEncKey( pMacKeyPtr, S_AQMACKEY, True );

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
				
				/* Check message format */ // @AAMELIN use only ASCI for S9( and HFT106
//				if ( PIN_CAT33 == *((UBYTE*)PIN_CONFIG) ) 
				{
					BfAscii( ( char * ) pOUTPIN, mac_binary, 8 );
					pOUTPIN += ( 8 * 2 );
/*				}
				else
				{
					memcpy( pOUTPIN, mac_binary, 8 );
					pOUTPIN += 8;
				}
*/
				// Check for double length flag
				if ((TRINP.TRAQPTR->AQOPT4) & AQ4_DBLMACKEY)
				{
					// Set FS
					GetFS();

					// Move double length key
					GetEncKey( pMacKeyPtr, S_AQMACKEY + S_AQMACKEY, True );
				}
				
				// Write message to Pin Pad
				if ( True != Write_Pin( ( UWORD ) ( pOUTPIN - OUTPIN ), ONESECOND*10 ) )
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
                if ( True == Read_Pin( ONESECOND * 5, False ) )
                {   
                    // Check response type (87 expected)  
                    if ( !memcmp( PINBUF, ( UBYTE * ) "87", 2 ) )
					{
						// Check message format // USE ONLY ASCII FOR S9 and HFT106
//						if ( PIN_CAT33 == *((UBYTE*)PIN_CONFIG) )
//						{
							// Extract data from the message.
							AscHex( PinData, ( char * ) &PINBUF[4], 4 );
                            bRetVal = True;
/*						}
						else
						{
							memcpy( PinData, &PINBUF[4], 8 );
                            bRetVal = True;
						}
*/					}
				}
            }

			// Close Pin Pad Port
			if ( True != Close_Pin(  ) )
				bRetVal = False;
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
			if ((TRINP.TRAQPTR->AQOPT4) & AQ4_DUKPTMAC)
			{
//-----------------------------------------------------------------------------
//                  INTERNAL DUKPT SESSION        
//-----------------------------------------------------------------------------
					// Open CryptoService
					CS_Open( False );

					// Check DUKPT Key ID
					if (mkid > 2) mkid = 0;

					// Calculate DUKPT MAC
					switch (CS_CreateMAC_DUKPT( mkid, PINBUF, pMacDataPtr, MacDataLength, 0 ))
					{
						case    1: 
							// Mac calculation successful
							bRetVal = True;
							break;	// OK
						case 1002:			// DUKPT key not generated
							// Generate DUKPT keys
							if ( 1 == CS_GenerateKey_DUKPT( mkid ) )
							{
								// Try to encrypt again
								if ( 1 == CS_CreateMAC_DUKPT( mkid, PINBUF, pMacDataPtr, MacDataLength, 0 ) ) 
								{
									// Mac calculation successful
									bRetVal = True;
									break;
								}
							}
						default:
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
						MoveItAq( OUTPIN, pMacKeyPtr, S_AQWORKKEY + S_AQWORKKEY );
					
					}
					else
					{
						MoveItAq( OUTPIN, pMacKeyPtr, S_AQWORKKEY );
						MoveItAq( &OUTPIN[S_AQWORKKEY], pMacKeyPtr, S_AQWORKKEY );
					}
					
					// Open CryptoService
					CS_Open( False );

					// Calculate MAC
					if (1 == CS_CreateMAC( mkid, OUTPIN, PinData, pMacDataPtr, MacDataLength, 1 ))
					{
						// Mac calculation successful
						bRetVal = True;
					}
				}
			}
		}
	}

	return bRetVal;
}
#endif // MAKE_ISOMAC



//=============================================================================
// Private function definitions
//=============================================================================
Bool Write_Read_Pin( UWORD length, unsigned short timeout, Bool chkkbd )
{
	IOSTATUS StatusRead, StatusWrite ;
	UBYTE InputType;
	Bool bPinCardStatus;
	Bool bKeyWasOpen;


	RecLen = S_PINBUF;
    bKeyWasOpen = DevFlag | DEV_KBD;

	/* Post a read to the pin device */
	if ( orvOK == SDK_PinRead ( PINBUF, &RecLen, timeout, &StatusRead )  )
	{

		/* Now Post a write to the pin device (flag will be ignored) */
		if ( orvOK == SDK_PinWrite ( OUTPIN, length, timeout, &StatusWrite ) )
		{
			while ( StatusWrite.CompleteFlag == 0 )
				SDK_RelinqCPU( );
			if ( orvOK != StatusWrite.Error  )
			{
				Rewind_Pin();
				return False;
			}
		}

		/* Wait for timeout (bit 0) or read complete (bit 7) */
		while( StatusRead.CompleteFlag == 0 )
		{
			if (chkkbd)
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

			// Give up some cycles while waiting...
			SDK_RelinqCPU(  );
		}	// while()

	}
	else 
		return False;

	if ( StatusRead.Error != orvOK )
	{
		SDK_PinReset( );
		return False;
	}
	if (chkkbd)
		// Force a close of still open pad and kbd devices.
		read_io( 0, 0, &bPinCardStatus, &DevFlag );
	
	return(True);	   
}

// internal Pinpad kullan�l�yorsa @y
extern Bool GetPinFromInternalPP( void )
{  
  UBYTE status, mkid;
  Bool bRetVal = False;
  UBYTE *WorkingKeyAddr;
  short WorkingKeyLen;							
  UBYTE tmpaqbuffer[S_AQWORKKEY + S_AQWORKKEY];	
	
  
  CustomAmount( AmountTotals, TRINP.TRTOTAM );

  memset( pinblock, 0, sizeof( pinblock ) );

  // Do until pin entered or cancel
  do
  {
	// Read pin from the keyboard.
	status = NumEntry( 2, 12, 4, &PinEntry );

	// Check if pin number  has been entered
	if ( 1 == status )
	{
		// Yes; save the Pin entered
		MoveTillZero( ( char * ) pinblock, ( char * ) Dspbuf, sizeof( pinblock ) );
		memset( Dspbuf, 0, sizeof(Dspbuf) );
		break;
	}
	else
	{
		// Exit the function
		memset( Dspbuf, 0, sizeof(Dspbuf) );
		return bRetVal;
	}
  }while ( True );

   pinlen = strlen( ( char * ) pinblock );

   // Set key index - binary value
   mkid = Binary( ( char ) GetEncKeyId() );


    // Working key length
    if ((TRINP.TRAQPTR->AQOPT4) & AQ4_DBLPINKEY)
    {  
	  WorkingKeyLen = S_AQWORKKEY + S_AQWORKKEY;
         
          MoveItAq( &(tmpaqbuffer[S_AQWORKKEY]), TRINP.TRAQPTR->AQ_TRIPLE_DESKEY2, S_AQWORKKEY );
    }
    else
    {
	  WorkingKeyLen = S_AQWORKKEY;
    }
    
    MoveItAq( tmpaqbuffer, TRINP.TRAQPTR->AQ_TRIPLE_DESKEY1, S_AQWORKKEY );

   // If working key is all nulls then pass NULL pointer
   // to encryption routine.
    //MoveItAq( tmpaqbuffer, TRINP.TRAQPTR->AQWORKKEY, WorkingKeyLen );
    MoveItAq( tmpaqbuffer, TRINP.TRAQPTR->AQ_TRIPLE_DESKEY1, WorkingKeyLen );
    if ( NullComp( (char*)tmpaqbuffer, WorkingKeyLen ) )
            WorkingKeyAddr = NULL;
    else
            WorkingKeyAddr = tmpaqbuffer;

    // Preclear the output buffer to the Z8.
    memset( ( UBYTE * ) OUTPIN, 0, sizeof( OUTPIN ) );

    // Combine pin length, pin and PAN and save at WorkData.
    FormatPin(  );

    // Prepare working key
    if ( WorkingKeyAddr != NULL )
    {
            // Check the key length
            if ( S_AQWORKKEY == WorkingKeyLen )
            {
                    memcpy( &tmpaqbuffer[S_AQWORKKEY], tmpaqbuffer, S_AQWORKKEY );
            }
                            
            // Open CryptoService
            CS_Open( False );

            // Calculate PIN
            if ( 1 == CS_EncryptPIN( mkid, tmpaqbuffer, pinblock, WorkData ) )
                    status = 1;
            else
                    status = 0;
    }
    else
    {
            status = 0;
    }


    if ( !status )
    {
            ShowErrMsg( EncryptError );
    }
    else
    {
            memcpy( TRINP.TRPINBL, pinblock, S_TRPINBL );	
            TRINP.TRSTATUS[1] |= TS2_PINBLOCK;
#ifdef MAKE_PINATTACKDELAY
            // Setup the pin attack delay
            PinAttackDelay( TRINP.TRPAN, TRINP.TRPINBL,  0 );
#endif 

            bRetVal = True;
    }

    // Clean clear PIN block
    memset( WorkData, 0, sizeof(WorkData) );
    // Clear the entered pin.
    memset( pinblock, 0, sizeof( pinblock ) );
    
    return bRetVal;		
}

extern Bool ReadFromS9( Bool useGenerateKey ) 
{
  UBYTE EncGenKey[CS_DOUBLE_KEY_SIZE];
  UBYTE GotClear, loopflag;
  UBYTE bRetVal = False;
  UBYTE tmpaqbuffer[S_AQWORKKEY + S_AQWORKKEY];
  UBYTE mkid;
  UBYTE tempbuf[64];
    

  GetMsg( N_Pinpad, Titlebuf );
  User_Msgs[1] = WaitForPin;
  
  Enable_Flags[0] = 0;
  DrawScreen( STATUS_SCR );
	
  Titlebuf[0] = 0;

  // Loop for CLEAR Key Pressed.
  do
  {
    // Clear PINBUF, OUTPIN and initialize pointer into OUTPIN.
    PreparePinCalc();

    switch(PIN_TYPE)
    {
	case PIN_TYPE_S9:                               
              GetPINData("2032.",5);  // Load the Message Type - PIN Entry Request (Double key)		              
              // Copy master key id from acquirer table.
              GetPINByte(GetEncKeyId());
              GetPAN(18,True,False); // 18 digits pan					
              GetFS(); // Set FS in buffer		

              GetPINByte('1');
              GetEncKey( TRINP.TRAQPTR->AQWORKKEY, S_AQWORKKEY , True );

              // Get pin prompt. Either 'PIN?   $9.99' or 'REENTER PIN'
              GetPinPrompt();
              GetPINData("12",2); // Send Max. PIN Digits (12 digits)
              GetPINByte('0'); // Send Enter Key required flag (Enter key is requered)
              GetFS(); // Set FS in buffer
              GetPINData("                ",16); //  Send display data, line 2
              GetFS(); // Set FS in buffer
              GetPINData("00",2);// TimeOut
              break;
    }

    loopflag = False;
    do {
        GotClear = False;
        if ( True != Write_Read_Pin( ( UWORD ) ( pOUTPIN - OUTPIN ), ONEMINUTE*5, True ) )
        {                                                       
          Close_Pin(  );                                                
          PIN_TYPE = PIN_TYPE_INTERNAL;
          bRetVal = GetPinFromInternalPP();
          return bRetVal;
        }

        bRetVal = True;

        switch(PIN_TYPE)
        {
          case PIN_TYPE_S9: 
              if ( memcmp( PINBUF, ( UBYTE * ) "33", 2 ) != 0)
              {
		Close_Pin(  );
                PinMessage( Msg_ClrScrn );
		return False;
              }
              // CHECK for clear key was pressed
              if ( PINBUF[4] == '6')
              {
		GotClear = True;
		break;
              }
              if ((PINBUF[4] == '0') && (memcmp( &PINBUF[5], ( UBYTE * ) "00", 2 ) != 0))
              {
		SaveEncPin();
              }
              else
              {
		Close_Pin(  );
                PinMessage( Msg_ClrScrn );
                return False;
              }
              break;
        }

    }while ( loopflag );
  }while ( GotClear );
  

  if ( True != Close_Pin(  ) )
  {
    bRetVal = False;
  }

  return bRetVal;
}



//@Aydeniz- Pinpadten alinan encpin saklanir 
extern void SaveEncPin( void )
{
    AscHex( EncPin, ( char * ) &PINBUF[7], S_TRPINBL );
}

//@Aydeniz- Pinpadten alinan encpin dec edilirek Pindata icersine yazilir 
extern Bool DecPin( char *PinData )
{
#define MAXBUFSIZE            16 
      
      UBYTE mkid;      
      short WorkingKeyLen;	
      UBYTE tmpaqbuffer[S_AQWORKKEY + S_AQWORKKEY];	            
      UBYTE i,j;
      static int PanLen;            

      UBYTE TempBuf[MAXBUFSIZE];
      UBYTE Buffer[MAXBUFSIZE];
      UBYTE DecPin[S_TRPINBL]; 
      UBYTE PlainPin[S_TRPINBL];
      UBYTE Pan12Digit[S_TRPINBL];
      UBYTE TempData[20];
      UBYTE TempData2[20];
	// Convert PAN to ASCII for quick check
	
      WorkingKeyLen = S_AQWORKKEY ;
      mkid = Binary( ( char ) GetEncKeyId() );

      CS_Open( False );
      
      memcpy( tmpaqbuffer, TRINP.TRAQPTR->AQWORKKEY, WorkingKeyLen );
      memcpy( &tmpaqbuffer[S_AQWORKKEY], TRINP.TRAQPTR->AQWORKKEY, WorkingKeyLen );
     

      if ( NullComp( (char*)tmpaqbuffer, WorkingKeyLen ) )
      {
        ShowErrMsg( EncryptError );
        return False;
      }

      memset(DecPin, 0x00, S_TRPINBL);      
      if ( 1 != CS_DecryptPIN( mkid, tmpaqbuffer,DecPin, EncPin))
        return False;
      
      memset(TempBuf, 0x00, MAXBUFSIZE);
      memset(Buffer, 0x00, MAXBUFSIZE);
      PanLen = (S_TRPAN * 2) - MovPanAsc(TempBuf, TRINP.TRPAN, (S_TRPAN * 2));
      for( j= MAXBUFSIZE-1,i = PanLen-2; j >= MAXBUFSIZE-12; i-- ,j--)
      {
          Buffer[j] = TempBuf[i];
      }
            
      memset(Pan12Digit, 0x00, S_TRPINBL);      
      AscHex(Pan12Digit, Buffer, S_TRPINBL );
      
      memset(PlainPin, 0x00, S_TRPINBL);
      for (i = 0; i < S_TRPINBL; ++i)
          PlainPin[i] = Pan12Digit[i] ^ DecPin[i];
      
      PanLen = PlainPin[0];
      memset( TempData, 0x00, 20);
      for( j= 0, i = 1; i < PanLen/2 + 1 + PanLen%2; ++i ,j+=2)
          BfAscii( &TempData[j], &PlainPin[i], 1 );
      
      memcpy( PinData,TempData,PanLen);                
      return True;           
} 

extern UBYTE *pinBlockFormat(UBYTE *pin, UWORD len )
{
   memcpy( pinblock, pin, 13 );

   //memcpy ( pinlen, len, sizeof( UWORD ) );

   pinlen = len;

   FormatPin();

   return WorkData;
}
