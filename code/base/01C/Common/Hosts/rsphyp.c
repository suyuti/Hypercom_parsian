//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             rsphyp.c
//      Hypercom Response Parsing Routines.
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "fixdata.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "msg.h"
#include "tables.h"
#include "pinpad.h"
#include "comm.h"
#include "msgfunc.h"
#include "schtab.h"
#include "hstmsg.h"
#include "rsphyp.h"
#include "hstresp.h"
#include "rspunp.h"
#include "updttime.h"

//=============================================================================
// External variables / declarations
//=============================================================================

// Public functions defined in this module

// Public from other modules

extern UBYTE *pRecvData;
extern UBYTE *p62Data;
extern UBYTE *p63Data;

#ifdef MAKE_ISOMAC
extern UBYTE *p64Data;
#endif	// MAKE_ISOMAC


//=============================================================================
// Private defines and typedefs
//=============================================================================

struct MovFldRec
{
    //! Destination 
	UBYTE *pDest;
    //! Source      
	UBYTE *pSrc;
    //! Data length 
	UBYTE dlen;
};

#define N_MOVTABINQ ( sizeof( MOVTABINQ ) / sizeof( struct MovFldRec ) )


//=============================================================================
// Private function declarations
//=============================================================================

static void ResponseBad( void );
static void RespInit( void );
static void PrivateFields( void );
static void ProcB61( void );
static void ProcB62( void );
static void ProcB63( void );
static void ProcB63HstPrData( void );
static void ProcB63NewText( void );
static void ProcB63OldText( void );
static void ProcB63PS2000( void );
static void ProcB63RefData( void );
static void ProcB63Schedule( void );
static void ProcB63Surc( void );
static void ProcB63AVS( void );
static void ProcB63New( void );
static void ProcB63Old( void );
static void ProcB63Len( void );
static void ProcB63HostText( void );
static void ProcB63SvPtr( UBYTE * pData );
static void RspAct( void );
static void MoveFields( void );
static Bool ValRsp( void );
static void ProcB63LVL2ID( void );
static void ProcB63CVV2( void );
#ifdef MAKE_ISOMAC
static void ProcB64( void );
#endif	// MAKE_ISOMAC


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================

//! Bit 62 field length 
static UWORD FldLen62;
//! Bit 63 field length 
static UWORD FldLen63;
//! Field length 
static UWORD FldLen;
//! Temporary Data pointer 
static UBYTE *pTempData;

// *INDENT-OFF*
//! Table of Fields to Move 
static const struct MovFldRec MOVTABINQ[] = {
	// Balance Inquiries also move in the Balance Fields 
    //! Total Amount 
	{ TRREQ.TRTOTAM,	TRRSP.TRTOTAM,	S_TRTOTAM	},
    //! Additional ("Today") Amount 
	{ TRREQ.TRTIP,		TRRSP.TRTIP,	S_TRTIP		},

	// Non-Balance Fields 
    //! Transaction Time  
	{ TRREQ.TRTIME,		TRRSP.TRTIME,	S_TRTIME	},
    //! Transaction Date  
	{ TRREQ.TRDATE,		TRRSP.TRDATE,	S_TRDATE	},
    //! Retrieval Ref Num 
	{ TRREQ.TRRRN,		TRRSP.TRRRN,	S_TRRRN		},
    //! Auth ID Resp Code 
	{ TRREQ.TRAUTH,		TRRSP.TRAUTH,	S_TRAUTH	},
    //! Response Code     
	{ TRREQ.TRRSPC,		TRRSP.TRRSPC,	S_TRRSPC	},
    //! Additnl Resp Data 
	{ TRREQ.TRRSPA,		TRRSP.TRRSPA,	S_TRRSPA	},
    //! Surcharge         
	{ TRREQ.TRSURC,		TRRSP.TRSURC,	S_TRBASE	},
    //! Total with Fee    
	{ TRREQ.TRTOTF,		TRRSP.TRTOTF,	S_TRBASE	}
};
// *INDENT-ON*


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Handles Hypercom Response
//!
//! \param
//!		[in]ReqCommStatus		status Req Communication
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void RspHyper( OS_RETURN_VALS ReqCommStatus )
{
	OS_RETURN_VALS CommStatus;

	// Loop till Response Done 
	while ( True )
	{
		// Clear the Buffer 
		memset( (UBYTE*)&TRRSP, 0, TRSZE );

		// Set Initial Responses 
		RespInit( );

		// Check if Request Packet was sent okay 
		if( ReqCommStatus == orvOK )
		{
			// Yes; get Response Message from COM 
			RecvMessage( );
			CommStatus = HostOSRetval;
		}
		else
		{
			// No; set Comm status to reflect the send status 
			CommStatus = ReqCommStatus;
		}

		// Check if Normal Data Was Received 
		if ( HostResp( CommStatus ) )
		{
			// Yes; set Receive Buffer Pointer to TPDU 
			pRecvData = RECVBUF;

			// Store the TPDU 
			memcpy( TRRSP.TRTPDU, pRecvData, S_TRTPDU );

			// Check if an NMS Message 
			if ( TRRSP.TRTPDU[0] == 0x68 )
			{
				// Yes; return Host Not Available Response 
				memcpy( TRRSP.TRRSPC, RSP_NA, S_TRRSPC );

				// Response is Done 
				break;
			}

			// Check if ISO Format Message 
			if ( TRRSP.TRTPDU[0] != 0x60 )
			{
				// No; bad Response 
				ResponseBad(  );

				// Restart the read before waiting some more.   
				RequestMessage ();
				if ( orvOK != HostOSRetval )
					break;

				// Get the next Message 
				continue;
			}

			// Point to the Response Message Type 
			pRecvData += S_TRTPDU;

			// Save Response Message Type 
			memcpy( TRRSP.TRMTYPE, pRecvData, S_TRMTYPE );

			// Make sure Message Type matches and low byte is Req+10 
			if (   ( TRRSP.TRMTYPE[0] != TRREQ.TRMTYPE[0] ) 
				|| ( TRRSP.TRMTYPE[1] != ( TRREQ.TRMTYPE[1] + 0x10 ) ) )
			{
				// No; bad Response 
				ResponseBad(  );

				// Restart the read before waiting some more.   
				RequestMessage ();
				if ( orvOK != HostOSRetval )
					break;

				// Get the next message 
				continue;
			}

			// Point to the Bitmap 
			pRecvData += S_TRMTYPE;

			// Unpack the Message and check if completed okay 
			if ( !RspUnPackMsg(  ) )
			{
				// No; bad Response 
				ResponseBad(  );

				// Restart the read before waiting some more.   
				RequestMessage ();
				if ( orvOK != HostOSRetval )
					break;

				// Get the next Message 
				continue;
			}

			// The Message is unbuilt 

			// Calculate Base again 
			SubAmt( TRRSP.TRBASE, TRRSP.TRTOTAM, TRRSP.TRTIP );

			// See if Response is for us 
			if ( ValRsp(  ) )
			{
				// Yes; handle data of Fields 60-64 
				PrivateFields(  );

				// Get out of the loop 
				break;
			}
			else
			{
				// No; so ignore it 
				ResponseBad(  );

				// Restart the read before waiting some more.   
				RequestMessage ();
				if ( orvOK != HostOSRetval )
					break;
			}
		}
		else
		{
			// No; error.  Get out of the loop 
			break;
		}
	}

	// Move fields from TRRSP to TRREQ 
	MoveFields(  );

	// Look up Response in TRREQ 
	RspLookUp( &TRREQ );

	// Take specific actions from Request 
	RspAct(  );
    ResetCommModule();

}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Displays Please Wait if a txn type is 0830 or 0820
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ResponseBad( void )
{
	// Check for an 0830 or an 0820 
	if (   ( TRRSP.TRMTYPE[0] == 0x08 ) 
		&& ( ( TRRSP.TRMTYPE[1] == 0x20 ) || ( TRRSP.TRMTYPE[1] == 0x30 ) ) )
	{
		// Yes; display Please Wait 
		ShowInfoMsg( PleaseWait, N_Comms );

		// Signal user 
		SDK_BeepIt( 1 );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sets Initial Values for Response
//!
//! \return
//!     None
//!
//! \note
//!		Any values validated in ValRsp must be set up in this routine for
//!     timed out responses, and other errors.
//-----------------------------------------------------------------------------
static void RespInit( void )
{
	// Approved Response 
	memcpy( TRRSP.TRRSPC, RSP_APP, S_TRRSPC );

	// Terminal ID 
	memcpy( TRRSP.TRTID, TRREQ.TRTID, S_TRTID );

	// Trace Number 
	memcpy( TRRSP.TRSTAN, TRREQ.TRSTAN, S_TRSTAN );

	// Transaction Amount 
	memcpy( TRRSP.TRTOTAM, TRREQ.TRTOTAM, S_TRTOTAM );

	// Set a null value for TRTIP for balance inquire -- difference
	// between zero and nothing sent from Host 
	memset( TRRSP.TRTIP, 0xFF, S_TRTIP );

	// Put 0's in all PS2000 fields 
	memset( ( char * ) &TRREQ.TRPSI, '0', sizeof( TRREQ.TRPSI ) + S_TRTRANID
			+ S_TRVALID );

	// Authorization Number 
	memcpy( TRRSP.TRAUTH, TRREQ.TRAUTH, S_TRAUTH );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Data for 60-63 fields
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PrivateFields( void )
{
	// Process Bit 61 
	ProcB61( );

	// Process Bit 62 
	ProcB62( );

	// Process Bit 63 
	ProcB63( );

#ifdef MAKE_ISOMAC
	// Process Bit 64
	ProcB64( );
#endif	// MAKE_ISOMAC
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Bit 61 of Response Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB61( void )
{
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Bit 62 of Response Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB62( void )
{
	// Process bit 62 data if it exists.    
	if ( p62Data != NULL )
	{
		if ( LOGON == TRREQ.TRKEY )
		{
			// Acquirer working key and mac key from host.  
			// Convert the length to binary and save it 
			FldLen62 = BcdBin( p62Data );

			// Point past length bytes  
			p62Data += 2;

			// Move working key to acquirer table.  
			MoveIt2Aq( TRREQ.TRAQPTR->AQWORKKEY, p62Data, S_AQMACKEY );
			p62Data += S_AQWORKKEY;

			// Move MAC key to acquirer table.  
			MoveIt2Aq( TRREQ.TRAQPTR->AQMACKEY, p62Data, S_AQMACKEY );
			p62Data += S_AQMACKEY;

			SetAQTabLRC(  );

			// Move merchant name if the data exists.   
			if ( p62Data[0] )
			{
				memset( TCONF.TCNAM1, 0, S_TCNAM1 );
				memcpy( TCONF.TCNAM1, p62Data, 23 );
				p62Data += 23;

				memset( TCONF.TCNAM2, 0, S_TCNAM2 );
				memcpy( TCONF.TCNAM2, p62Data, 23 );
				p62Data += 23;

				memset( TCONF.TCNAM2, 0, S_TCNAM2 );
				memcpy( TCONF.TCNAM3, p62Data, 23 );
				p62Data += 23;

				SetTCTabLRC(  );
			}

		}
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Parse data in Bit 63 of Response from Hyper, CDC or Alpine acquirers
//!
//! \return
//!     None
//!
//! \note
//!     Layout of alternate Host Response and/or PS2000 Data in Bit 63.<br>
//!     <br>
//!     (Ignore spacing, only to clarify description).<br>
//!     <br>
//!     AA L1 22 ZZZZZZZZZZZZZZZZZZZZ L2 20 B CCCCCCCCCCCCCCC<br>
//!     <br>
//!     DDDD EE FF<br>
//!     AA		Total length data in Bit 63				BCD<br>
//!     L1		First field length						BCD<br>
//!     22		First field type						ASCII<br>
//!             ('22' for alternative Host Response)<br>
//!     Z...Z	20(?) Digit Host Response Text			ASCII<br>
//!     L2		Second field length						BCD<br>
//!     20		Second field type						ASCII<br>
//!             ('20' for PS2000 Data)<br>
//!     B		1 Digit Payment Services Indicator		ASCII<br>
//!     C...C	15 Digit Transaction Identifier			ASCII<br>
//!     DDDD	4 Digit Validation Code					ASCII<br>
//!     EE		2 Character Visa II Response Code		ASCII<br>
//!     FF		2 Character Visa II POS Entry Mode		ASCII<br>
//!     <br>
//!     AA ZZZZZZZZZZZZZZZZZZZZ<br>
//!     <br>
//!     AA		Total length of data in Bit 63			BCD<br>
//!     Z...Z	20(?) Digit Host Response Text			ASCII<br>
//!     <br>
//!     AA L2 20 B CCCCCCCCCCCCCCC DDDD EE FF<br>
//!     <br>
//!     L2		Second field length						BCD<br>
//!     20		Second field type						ASCII<br>
//!             ('20' for PS2000 Data)<br>
//!     B		1 Digit Payment Services Indicator		ASCII<br>
//!     C...C	15 Digit Transaction Identifier			ASCII<br>
//!     DDDD	4 Digit Validation Code					ASCII<br>
//!     EE		2 Character Visa II Response Code		ASCII<br>
//!     FF		2 Character Visa II POS Entry Mode		ASCII
//-----------------------------------------------------------------------------
static void ProcB63( void )
{
	UBYTE DataType;

	// Any Bit 63 Data to process 
	if ( p63Data != NULL )
	{
		// Yes; convert the length to binary and save it 
		FldLen63 = BcdBin( p63Data );

		// Save pointer to the start of data 
		pRecvData = p63Data + 2;

		// Increment to point at the possible data type number and
		// save field type address 
		pTempData = pRecvData + 2;

		// Loop through the Bit 63 Data and process it 
		while ( FldLen63 )
		{
			// Convert the possible data type number to binary 
			AscHex( &DataType, ( char * ) pTempData, ( UBYTE ) 1 );

			// Call proper routine based on data type number 
			switch ( DataType )
			{
				case 0x15:		// Level II ID fields 
					ProcB63LVL2ID(  );
					break;

				case 0x16:		// CVV2 fields 
					ProcB63CVV2(  );
					break;

				case 0x20:		// PS2000 fields 
					ProcB63PS2000(  );
					break;

				case 0x22:		// New Text 
					ProcB63NewText(  );
					break;

				case 0x29:		// Host Print Data 
					ProcB63HstPrData(  );
					break;

				case 0x31:		// Host Reference Data 
					ProcB63RefData(  );
					break;

				case 0x42:		// Schedule Event 
					ProcB63Schedule(  );
					break;

				case 0x44:		// Surcharge 
					ProcB63Surc(  );
					break;

				case 0x55:		// Address Verification Response 
					ProcB63AVS(  );
					break;

				default:		// Old Host Text 
					ProcB63OldText(  );
					break;
			}
		}
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Host Print Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63HstPrData( void )
{
	UWORD Len;

	// Get the length and point to the data 
	ProcB63New(  );

	// Clear Additional Host Print Data Buffer 
	memset( (UBYTE*)&HSTPRDATA, 0, S_HSTPRDATA );

	// Set the move length equal to the smallest of the buffer size
	// and the actual data length 
	if ( FldLen < S_HSTPRDATA )
	{
		// Actual data length is the smaller than the buffer size 
		Len = FldLen;
	}
	else
	{
		// Buffer size is equal to or larger than the actual data length 
		Len = S_HSTPRDATA;
	}

	// Copy the Host Print Data 
	memcpy( HSTPRDATA, pRecvData, Len );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + FldLen );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes New Text Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63NewText( void )
{
	// Get the length and point to the data 
	ProcB63New(  );

	// Process the Host Text 
	ProcB63HostText(  );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Old Text Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63OldText( void )
{
	// Get the length and point to the data 
	ProcB63Old(  );

	// Process the Host Text 
	ProcB63HostText(  );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes PS2000 fields
//!
//! \return
//!     None
//!
//! \note
//!     This is a Sale/Refund/Auth Only that may have PS2000 fields in Bit 63.
//-----------------------------------------------------------------------------
static void ProcB63PS2000( void )
{
	UWORD Length;

	// Get the length and point to the data 
	ProcB63New(  );

	// Calculate length to copy 
	Length = sizeof( TRREQ.TRPSI ) + S_TRTRANID + S_TRVALID;

	// Copy PS2000 data 
	memcpy( &TRREQ.TRPSI, pRecvData, Length );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + Length );

	// If a value already exists in the Authorized Amount field,
	// don't overwrite it 
	if ( NullComp( ( char * ) TRREQ.TRAUTHAMT, S_TRAUTHAMT ) )
	{
		// No; copy Total Amount into PS2000 Authorized Amount 
		memcpy( TRREQ.TRAUTHAMT, TRREQ.TRTOTAM, S_TRAUTHAMT );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Host Reference Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63RefData( void )
{
	// Get the length and point to the data 
	ProcB63New(  );

	// Move in the binary length 
	memcpy( TRREQ.TRREFDATA, &FldLen, 2 );

	// Copy the Host Reference Data 
	memcpy( TRREQ.TRREFDATA + 2, pRecvData, FldLen );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + FldLen );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes a Schedule Event Request
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63Schedule( void )
{
	struct schedule_rec *pSchdPtr;	// Pointer to SCHTAB entry 
	UBYTE TmpMsg[20];
	UBYTE i = 0;

	// Get the length and point to the data 
	ProcB63New(  );

	// Find a free schedule table entry 
	for ( pSchdPtr = SCHTAB; pSchdPtr < &SCHTAB[SCHMAX]; pSchdPtr++ )
	{
		// Look at every entry to see if this entry is not used. 
		if ( !( pSchdPtr->SCHFLAG & SCH_USED ) )
		{
			// Found one 
			break;
		}

		i++;
	}

	// If not found unused entry 
	if ( i >= SCHMAX )
	{
		// Point to first schedule table entry 
		pSchdPtr = SCHTAB;
	}

	// Clear this unused entry first 
	memset( (UBYTE*)pSchdPtr, 0, (UWORD)SCHSZE );

	// Set entry flag to used 
	pSchdPtr->SCHFLAG |= SCH_USED;

	// Looking for the first space
	i = StrLnChr( ( char * ) pRecvData, ' ', 2*sizeof( TmpMsg ) );
	
	// Change spaces by F
	if ( i < 2*sizeof( TmpMsg ) )
		memset( &pRecvData[i], 'F', 2*sizeof( TmpMsg ) - i );

	// Get and convert scheduled information to BCD 
	AscHex( ( UBYTE * ) & TmpMsg[0], ( char * ) pRecvData, sizeof( TmpMsg ) );

	// Save received schedule messages 
	pSchdPtr->SMTYPE = TmpMsg[0];
	memcpy( pSchdPtr->SMWHEN, &TmpMsg[1], S_SMWHEN );
	pSchdPtr->SMTELTYPE = TmpMsg[S_SMWHEN + 1];
	memset( pSchdPtr->SMTELNO, 0xFF, S_SMTELNO );
	memcpy( pSchdPtr->SMTELNO, &TmpMsg[S_SMWHEN + 2], ( UWORD ) S_SMTELNO );

	// Recalculate the LRC 
	SetSCHTabLRC(  );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + FldLen );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Surcharge Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63Surc( void )
{
	// Get the length and point to the data 
	ProcB63New(  );

	// Get Surcharge Amount 
	AscHex( TRRSP.TRSURC, ( char * ) pRecvData, S_TRBASE );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + S_TRBASE );

	// Build Total with Network Fee 
	AddAmt( TRRSP.TRTOTF, TRRSP.TRTOTAM, TRRSP.TRSURC );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Address Verification Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63AVS( void )
{
	// Get the length and point to the data 
	ProcB63New(  );

	// Store Address Verification Response 
	memcpy( TRREQ.TRAVSRSP, pRecvData, FldLen );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + FldLen );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes New Style Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63New( void )
{
	// Get length in binary 
	FldLen = BcdBin( pRecvData );

	// Subtract from overall length 
	ProcB63Len(  );

	// Subtract 2 for data type code 
	FldLen -= 2;

	// Point past data length and table id 
	pRecvData += 4;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Old Style Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63Old( void )
{
	// Length of old style is the length of all of field 63 
	FldLen = FldLen63;

	// Clear the overall field length 
	FldLen63 = 0;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Updates overall length
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63Len( void )
{
	// Subtract field length and length bytes from overall length 
	FldLen63 -= FldLen + 2;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes the Host Text
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63HostText( void )
{
	UWORD Length;

	// Clear the Host Text 
	memset( TRREQ.TRHOSTTEXT, 0, S_TRHOSTTEXT );

	// Check if field length smaller than maximum Host Text length 
	if ( FldLen < S_TRHOSTTEXT )
	{
		// Yes; use this length 
		Length = FldLen;
	}
	else
	{
		// No; user maximum Host Text length 
		Length = S_TRHOSTTEXT;
	}

	// Save the Host Text 
	memcpy( TRREQ.TRHOSTTEXT, pRecvData, Length );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + Length );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Saves the pointers for the next field
//!
//! \param
//!		[in]pData			Pointer to a data field
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63SvPtr( UBYTE * pData )
{
	// Save the pointers for the next field 
	pRecvData = pData;
	pTempData = pData + 2;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Takes specific actions from Request
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void RspAct( void )
{
	// Update System Time/Date from Host Response 
	UpdateTime(  );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves Fields from TRRSP to TRREQ
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void MoveFields( void )
{
	UBYTE index;
	UBYTE i;

	// See if Balance Inquiry Transaction 
	if ( TRREQ.TRKEY == BALINQ )
	{
		// Yes; set index to beginning of table 
		index = 0;
	}
	else
	{
		// No; set index to beginning of non-balance fields 
		index = 2;
	}

	// Move fields from TRRSP to TRREQ 
	for ( i = index; i < N_MOVTABINQ; i++ )
	{
		memcpy( MOVTABINQ[i].pDest, MOVTABINQ[i].pSrc, MOVTABINQ[i].dlen );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Validates Response is for this Terminal
//!
//! \return
//!     True if Response for this terminal otherwise False
//-----------------------------------------------------------------------------
static Bool ValRsp( void )
{
	// Make sure the following fields are the same in the Response as
	// in the Request 

	// Check Terminal ID 
	if ( memcmp( TRREQ.TRTID, TRRSP.TRTID, S_TRTID ) == 0 )
	{
		// The same; check if Trace Number exists 
		if ( !NullComp( ( char * ) TRRSP.TRSTAN, S_TRSTAN ) )
		{
			// Yes; check Trace Number 
			if ( memcmp( TRREQ.TRSTAN, TRRSP.TRSTAN, S_TRSTAN ) == 0 )
			{
				// The same; response for this Terminal 
				return ( True );
			}
		}
		else
		{
			// No; response for this Terminal 
			return ( True );
		}
	}

	// Response not for this Terminal 
	return ( False );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes Level II ID fields
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63LVL2ID( void )
{
	// Get the length and point to the data 
	ProcB63New(  );

	// Store Level II ID Response 
	memcpy( &TRREQ.TRLVL2ID, pRecvData, sizeof( TRREQ.TRLVL2ID ) );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + sizeof( TRREQ.TRLVL2ID ) );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes CVV2 fields
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB63CVV2( void )
{
	// Get the length and point to the data 
	ProcB63New(  );

	// Save the pointers 
	ProcB63SvPtr( pRecvData + FldLen );
}

#ifdef MAKE_ISOMAC
//-----------------------------------------------------------------------------
//! \brief
//!     Processes Bit 64 of Response
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void ProcB64( void )
{
	UBYTE rcvMAC[ 8 ];
    int i;
	static char buf[8];
	char* ptr = buf;

	UBYTE PinMacId;					// Mac index.
	UBYTE *pMacKeyPtr;				// Pointer to MAC key.
	UBYTE *pMacDataPtr;				// Pointer to data to be MAC'ed.
	UWORD MacDataLength;			// Length of data to be MAC'ed.
	UBYTE PinData[8];				// Used for passing data to/from pinpad.

    // Check if MAC calculation enabled
    if ( !( ( TRREQ.TRAQPTR->AQOPT4 ) & AQ4_MAC ) )
        return;

	// Process bit 64 data if it exists.
	if ( p64Data == NULL )
    {
        // MAC expected but not found -- 
        // Set in a Invalid MAC Response Code
        memcpy( TRRSP.TRRSPC, RSP_IM, S_TRRSPC );
        return;
    }
    else
	{
		GetRspTxtOpts();

        // Check MAC if approved
		if( ( RSPOPT & R_APPROVED ) || ( RSPOPT & R_UPLOAD ) )
		{
			// Move received MAC to rcvMAC
			memcpy( rcvMAC, p64Data, 8 );

			// Set data pointer
			pMacDataPtr = RECVBUF + TPDU_SZE;
			
            // Set data length for MAC calulating
			MacDataLength = p64Data - RECVBUF - TPDU_SZE;
			
	
            // Set Master Key ID
            PinMacId = TRREQ.TRAQPTR -> AQMACID;

            // Pointer to encrypted MAC key
            pMacKeyPtr = TRREQ.TRAQPTR -> AQMACKEY;
			
            // Display info message.
			ShowInfoMsg ( ProcNow, N_Pinpad );
			
            // Calculate MAC
			PinGenMac( PinMacId, pMacKeyPtr, pMacDataPtr, MacDataLength, PinData );

            for (i = 0; i < 4; i++) 
            {
                unsigned char n = (PinData[i] >> 4) & 15;
                *ptr++ = n + (n < 10 ? '0' : 'A' - 10);
                n = PinData[i] & 15;
                *ptr++ = n + (n < 10 ? '0' : 'A' - 10);
			 }
			 memcpy(PinData, buf, sizeof(buf));

            // Compare MAC
			if( memcmp( rcvMAC, PinData, 4 ) != 0 ) 
            {
                // Set in a Invalid MAC Response Code
                memcpy( TRRSP.TRRSPC, RSP_IM, S_TRRSPC );
            }
        }
	}
}
#endif	// MAKE_ISOMAC

