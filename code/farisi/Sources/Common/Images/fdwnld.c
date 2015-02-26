//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             fdwnld.c
//      image file down load
//
//=============================================================================
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkgui.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "sdkmem.h"
#include "hfs.h"
#include "utllib.h"
#include "fixdata.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "msg.h"
#include "util.h"
#include "comdata.h"
#include "pinpad.h"
#include "fdwnld.h"
#include "icepak.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static UBYTE XModemMultFilDwnLoad( RS232BAUDRATE baudRate );
static OS_RETURN_VALS ByteOut( UBYTE sendbyte );
static UBYTE chksum( UBYTE * str, UWORD len );
static UBYTE Long2Str( char *str, long val );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
//! structure for YesNoEntry()
static const ENTRY_STR YNEntry =
{
	N_FileDownLoad,
	N_NullStr,
	CorrectYesNo,
};

static const ENTRY_STR Entry = {
	N_NullStr,
	CustomMsg,
	CustomMsg2,
};


//=============================================================================
// Public function definitions
//=============================================================================


//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     File download function
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void FileDownLoad( void )
{
	// See if this is the correct Amount
	switch ( YesNoEntry( &YNEntry ) )
	{
		case 0:				// Timed out
		case CANCEL_KY:		// Cancel key is pressed
			// Return cancel/abort the Transaction
		case CLEAR_KY:			// NO - return continue
			return;
		case ENTER_KY:			// Yes - accept tip amount.
			break;
	}

	XModemMultFilDwnLoad( rb_9600 );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Performs multiple Xmodem file download
//!
//! \param
//!		[in]baudRate			Baud rate:<br>
//!                                 rb_19200,<br>
//!                                 rb_9600,<br>
//!                                 rb_4800,<br>
//!                                 rb_2400,<br>
//!                                 rb_1200,<br>
//!                                 rb_300
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static UBYTE XModemMultFilDwnLoad( RS232BAUDRATE baudRate )
{
	IOSTATUS readStatus;
	UBYTE retries;
	UWORD bufLen;
	FILE *fp;
	UBYTE nBlock = 1;
	UBYTE nFileBlock;
	UBYTE failCode = 0;
	RS232DATA xData = { rp_TRANS, rc_8N2, rb_19200 };
	UBYTE n;
	UBYTE InputType;
	Bool bPinCardStatus;
	
	long avail;
	long fsize;

	xData.Baud = baudRate;
	retries = 0;
	nFileBlock = 0;

	// display waiting message
	ShowInfoMsg( WaitForAnswer, N_FileDownLoad );

	// open RS232 port
	SDK_RS232Open( ( RS232MODE ) rm_Configurable, ( RS232DATA * ) & xData );

	// post a read request 
	bufLen = sizeof( RECVBUF );
	SDK_RS232Read( RECVBUF, &bufLen, ONESECOND * 20, &readStatus );

	// write a nak  to start download
	ByteOut( NAK );

	// xmodem download loop
	while ( 1 )
	{
		while ( readStatus.CompleteFlag == 0 )
		{
			InputType = read_io( DEV_PAD + DEV_KBD + STAY_OPEN0, 0, &bPinCardStatus, &DevFlag );	// Wait for input
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

			if ( KeyVal == CANCEL_KY ) break;

			SDK_RelinqCPU(  );
		}

		// Force a close of still open pad and kbd devices.
		read_io( 0, 0, &bPinCardStatus, &DevFlag );

		// if a packet is received
		if ( readStatus.Error == orvOK )
		{
			// validate packet  
			if ( RECVBUF[0] == EOT )
			{
				// exit successful
				ByteOut( ACK );
				break;
			}
			else if ( RECVBUF[0] == SOH &&
					  RECVBUF[1] == nBlock &&
					  RECVBUF[2] == ( nBlock ^ 0xFF ) &&
					  bufLen == 132 &&
					  RECVBUF[131] == chksum( RECVBUF, 131 ) )
			{
				nBlock++;

				if ( 0 == nFileBlock )
				{
					nFileBlock = *( ( UBYTE * ) ( &RECVBUF[3] ) );

					// If no enougth space in HFS, return
					avail = fspaceavail(  );
					fsize = nFileBlock*128;
					if(avail < fsize)
					{
						ShowErrMsg(N_NoEnougthSpaceInHFS);

						return 0;
					}

					fp = fopen( ( char * ) &RECVBUF[5], "w" );
					if ( 0 == fp )
					{
						memset( ( UBYTE * ) CSTMSG, 0,sizeof( CSTMSG ) );
						GetMsg( Msg_fopenerror, CSTMSG );

						n = StrLn( CSTMSG, sizeof( CSTMSG ) );
						MoveTillZero( &CSTMSG[n], ( char * ) &RECVBUF[5],
									  sizeof( CSTMSG ) );

						n = StrLn( CSTMSG, sizeof( CSTMSG ) );
						CSTMSG[n] = ' ';
						n++;
						Long2Str( &CSTMSG[n], ( long ) ferror( fp ) );

						GetMsg( N_FileDownLoad, DefaultTitle );
						ContinueEntry( &Entry );

						return 0;
					}

					Enable_Flags[4] = 1;
					Enable_Flags[3] = 0;

					GetMsg( N_FileDownLoad, CSTMSG4 );

					memset( ( UBYTE * ) CSTMSG, 0,sizeof( CSTMSG ) );
					GetMsg( Msg_DownloadingFile, CSTMSG );

					memset( ( UBYTE * ) CSTMSG2, 0,sizeof( CSTMSG2 ) );
					MoveTillZero( CSTMSG2, ( char * ) &RECVBUF[5],
								  sizeof( CSTMSG2 ) );

					DrawScreen( INFO_SCR );
					SDK_BeepIt( 1 );
				}
				else
				{
					nFileBlock--;
					fwrite( &RECVBUF[3], 1, 128, fp );
				}
				bufLen = sizeof( RECVBUF );
				SDK_RS232Read( RECVBUF, &bufLen, ONESECOND * 20,
							   &readStatus );
				ByteOut( ACK );

				if ( 0 == nFileBlock )
				{
					fclose( fp );
				}
			}
			else
			{
				// invalid packet
				if ( retries != 0 )
				{
					// reset any pending requests
					SDK_RS232Reset(  );
					SDK_RS232Open( ( RS232MODE ) rm_Configurable, ( RS232DATA * ) & xData );
					bufLen = sizeof( RECVBUF );
					SDK_RS232Read( RECVBUF, &bufLen, ONESECOND * 20, &readStatus );
					ByteOut( NAK );
					// decrement retry contry
					retries--;
				}
				else
				{
					// no more retries
					failCode = 1;
					// exit with failure
					break;
				}
			}
		}
		else
		{
			// error or timeout
			if ( retries != 0 )
			{
				// reset any pending requests   
				SDK_RS232Reset(  );
				bufLen = sizeof( RECVBUF );
				SDK_RS232Read( RECVBUF, &bufLen, ONESECOND * 20,
							   &readStatus );
				ByteOut( NAK );
				// decrement retry contry
				retries--;
			}
			else
			{
				// exit with failure
				failCode = 1;
				break;
			}
		}
	}

	SDK_RS232Reset(  );
	SDK_RS232Close(  );


	if ( 0 != failCode )
	{
//      remove(pFileName);
		return failCode;
	}


// activiate icepac
	IcePak_Init ();

	return 0;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sends 1 byte to serial port
//!
//! \param
//!		[in]sendbyte			Byte to be sent
//!
//! \return
//!     OS_RETURN_VALS
//!
//! \note
//!     Assumes RS232 port previously open.
//-----------------------------------------------------------------------------
static OS_RETURN_VALS ByteOut( UBYTE sendbyte )
{
	IOSTATUS WStatus;
	OS_RETURN_VALS errcode;

	if ( ( errcode = SDK_RS232Write( &sendbyte, 1, 0, &WStatus ) ) != orvOK )
		return errcode;

	while ( WStatus.CompleteFlag == 0 )
	{
		SDK_RelinqCPU(  );
	}

	return orvOK;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Finds checksum of given buffer
//!
//! \param
//!		[in]str					Data buffer
//!		[in]len					Length of buffer
//!
//! \return
//!     Ñhecksum of buffer
//-----------------------------------------------------------------------------
static UBYTE chksum( UBYTE * str, UWORD len )
{
	UBYTE chksum = 0;

	while ( len-- )
		chksum += *str++;
	return ( chksum );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Converts a long int to a string
//!
//! \param
//!		[out]buf				Buffer to place converted value
//!		[in]val					Value to convert
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static UBYTE Long2Str( char *buf, long val )
{
	UBYTE ip, i, neg;
	char ch;

	neg = 0;
	if ( val < 0 )
	{
		val *= -1;
		neg = 1;
	}

	for ( ip = 0; 0 != val; ip++ )
	{
		buf[ip] = ( char ) ( val % 10 ) + '0';
		val /= 10;
	}
	if ( 0 == ip )
	{
		buf[ip] = '0';
		ip++;
	}
	if ( 0 != neg )
	{
		buf[ip] = '-';
		ip++;
	}
	buf[ip] = '\0';
	for ( i = 0; i < ip / 2; i++ )
	{
		ch = buf[i];
		buf[i] = buf[ip - i - 1];
		buf[ip - i - 1] = ch;
	}
	return ip;
}

