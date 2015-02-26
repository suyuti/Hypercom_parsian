//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     bit8583.c
//  Function used to implement the Hypercom Request Building Routines
//
//=============================================================================
#include "string.h"

#include "fixdata.h"

#include "i8583utl.h"
#include "bit8583.h"
#include "struct.h"
#include "tables.h"
#include "is8583.h"
#include "tables.h"
#include "comdata.h"
#ifdef	MAKE_EMV
#include "emv_l2.h"
#endif	// MAKE_EMV

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================
#define TCAP_PLAINPINOFFL	0x80
#define TCAP_ENCHPINOFFL	0x40
#define TCAP_ENCHPINONL		0x10
#define TC6_ONLINE			0x04	// TERMINAL ONLINE CAPABLE
#define TC6_PINONLINE		0x10	// ONLINE PIN VERIFICATION SUPPORTED
#define TC6_PINOFFLINE		0x20	// OFFLINE PIN VERIFICATION SUPPORTED



//=============================================================================
// Private function declarations
//=============================================================================
static struct move_rec *FindMoveEntry( UBYTE bitno );
static BYTE MoveData( UBYTE BitNum );



//=============================================================================
// Public data definitions
//=============================================================================
//! Pointer to moving table
const struct move_rec *Move_Table;
//! Container for found table entry
static const struct move_rec *pmove_rec;
//! Pointer into send buffer
UBYTE *pSendData;
//! Pointer to Bit 63 length bytes  
UBYTE *pB63Len;
//! Bit 63 total length  
int B63_Len;



//=============================================================================
// Private data definitions
//=============================================================================



//=============================================================================
// Public function definitions
//=============================================================================




//-----------------------------------------------------------------------------
//! \brief
//!     Packs the Message Request into the Send Buffer
//!
//! \param[in] pTBitMap         Pointer to request bitmap
//!
//! \return
//!     True on success else False
//-----------------------------------------------------------------------------
extern Bool ReqPackMsg( UBYTE * pTBitMap )
{
	UBYTE Status;
	UBYTE *pBitMap;
	UBYTE BitNum;
	UBYTE BitMask;
	BYTE MoveStatus;

	// Initialize Status to okay
	Status = True;

	// Set up bitmap pointer
	pBitMap = SENDBUF + TPDU_SZE + 2;

	// Initialize bit mask
	BitMask = 0x80;

	// For bits 1-64... 
	for ( BitNum = 1; BitNum <= 64; BitNum++ )
	{
		// Check if its in the Transaction bitmap 
		if ( *pTBitMap & BitMask )
		{
			// Yes; move the data
			MoveStatus = MoveData( BitNum );

			// Check if data moved
			if ( MoveStatus == 1 )
			{
				// Yes; update the Request bitmap 
				*pBitMap |= BitMask;
			}
			else
			{
				// No; check if error 
				if ( MoveStatus == -1 )
				{
					// Yes; set status to False 
					Status = False;

					// Quit the loop 
					break;
				}
			}
		}

		// Update bit mask 
		BitMask = BitMask >> 1;

		// Check if zero 
		if ( BitMask == 0x00 )
		{
			// Yes; re-initialize the bit mask 
			BitMask = 0x80;

			// Update the bitmap pointers 
			pTBitMap++;
			pBitMap++;
		}
	}

	// Return status 
	return ( Status );
}



//-----------------------------------------------------------------------------
//! \brief
//!     Tests if field is present
//!
//! \param[in] pField           Pointer to the field to be checked
//! \param[in] Length           Length of the field to be checked
//!
//! \return
//!     True if field contains data else False
//-----------------------------------------------------------------------------
extern Bool IfThere( UBYTE * pField, UWORD Length )
{
	Bool FormatOkay;
	Bool DataExists;

	// Initialize the format okay flag to True 
	FormatOkay = True;

	// Reset the data exists flag 
	DataExists = False;

	// Switch on the message format 
	switch ( prs8583->IS8583_FMT )
	{
		case FFIX + ATTN:		// Fixed Numeric 
		case FLLVAR + ATTN:	// LL Variable Numeric 
			// Make length even and divide by 2 
			if ( Length & 0x01 )
				Length++;
			Length /= 2;
			break;

		case FFIX + ATTBIN:	// Fixed Binary 
			// Divide by 8 
			Length /= 8;
			break;

		case FFIX + ATTAN:		// Fixed Alpha Numeric 
		case FFIX + ATTANS:	// Fixed Alpha Numeric Special 
		case FLLVAR + ATTANS:	// LL Variable Alpha Numeric Special
		case FLLLVAR + ATTAN:	// LLL Variable Alpha Numeric 
		case FLLLVAR + ATTANS:	// LLL Variable Alpha Numeric Special
			break;

		default:				// Unknown format 
			FormatOkay = False;
			break;
	}

	// Check if field format is valid 
	if ( FormatOkay )
	{
		// Yes; perform safety validation on length 
		if ( Length != 0 )
		{
			// Okay; check if field contains data 
			if ( !NullComp( ( char * ) pField, Length ) )
			{
				// Yes; data in field exists 
				DataExists = True;
			}
		}
	}

	// Return the data exists flag 
	return ( DataExists );
}



//-----------------------------------------------------------------------------
//! \brief
//!     Generic Move function
//!
//! \param[in] pData            Pointer to the data to be moved
//! \param[in] Length           Length of the data to be checked
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void MovGen( UBYTE * pData, UWORD Length )
{
	UWORD LLLvar;
	UBYTE tmpacq[6];

	// Switch on the message format 
	switch ( prs8583->IS8583_FMT )
	{
		case FFIX + ATTN:		// Fixed Numeric 
			// Make length even and divide by 2 
			if ( Length & 0x01 )
				Length++;
			Length /= 2;
			break;

		case FFIX + ATTBIN:	// Fixed Binary 
			// Divide by 8 
			Length /= 8;
			break;

		case FFIX + ATTAN:		// Fixed Alpha Numeric 
		case FFIX + ATTANS:	// Fixed Alpha Numeric Special 
			break;

		case FLLVAR + ATTANS:	// LL Variable Alpha Numeric Special 

			// AMEX host requires that track 1 data be sent in as is.
			// No length byte is required. 2-15-02.

			MoveItAq( tmpacq, TRINP.TRAQPTR->AQPNAME, sizeof(tmpacq) );
			if ( !memcmp( tmpacq, ( UBYTE * ) "AMEX  ", 6 ) )
			{

				// Move BCD length (1 byte) 
				*pSendData = BinBcdB( ( UBYTE ) Length );
				// Bump pointer past length (1 byte) 
				pSendData++;
			}
			break;

		case FLLLVAR + ATTAN:	// LLL Variable Alpha Numeric 
		case FLLLVAR + ATTANS:	// LLL Variable Alpha Numeric Special
			// Convert length to BCD (2 bytes)
			LLLvar = BinBcdW( Length );
			// Move BCD length (2 bytes) 
			memcpy( pSendData, &LLLvar, 2 );

			// Bump pointer past length (2 bytes)
			pSendData += 2;

			break;

		case FLLVAR + ATTN:	// LL Variable Numeric 

			// Move BCD length (1 byte) 
			*pSendData = BinBcdB( ( UBYTE ) Length );
			// Bump pointer past length (1 byte) 
			pSendData++;
			// Make length even and divide by 2 
			if ( Length & 0x01 )
				Length++;
			Length /= 2;
			break;

		default:				// Unknown format; no data to move 
			Length = 0;
			break;
	}

	// Safety validation on length 
	if ( Length != 0 )
	{
		// Move the Data to the Send Buffer 
		memcpy( pSendData, pData, Length );

		// Increment the pointer 
		pSendData += Length;
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Copies PAN into SENDBUF if TRACK 2 data does not exist
//!
//! \return
//!     True if data has been copied otherwise False
//!
//! \note
//!     pSendData points to the destination
//-----------------------------------------------------------------------------
extern Bool MovPan( void )
{
	static Bool retval;
	static UWORD cnt, bytecount;

	// Assuming we have Track 2 data, therefore, no need to send
	// the PAN 

	retval = False;

	// If track 2 element in TRREQ is not empty, we don't need to
	// send the PAN if it is not advice or adjus.

    /*
    if ((NullComp((char *)TRREQ.TRTRK2, S_TRTRK2))  || 
        (TRREQ.TRKEY == RUPLOAD)                    || 
        (TRREQ.TRSTATUS[0] & (TS1_ADV + TS1_ADJ)))
    */
    if ((NullComp((char *)TRREQ.TRTRK2, S_TRTRK2))  || 
        (TRREQ.TRKEY == RUPLOAD)                    || 
        (TRREQ.TRSTATUS[0] & (TS1_ADV + TS1_ADJ))   ||
        g_cancelTransactionType == CLOSEDPAUTHCANCEL)
    {
		// Track data is empty, we need to move PAN into SENDBUF 

		bytecount =
			MoveTillChar( ( char * ) ( pSendData + 1 ),
						  ( char * ) TRREQ.TRPAN, 10, ( char ) 0xff );

		if ( ( TRREQ.TRPAN[bytecount - 1] & 0x0f ) == 0x0f )
			cnt = bytecount * 2 - 1;
		else
			cnt = bytecount * 2;

		// Quick and Dirty!!. Account number should be greater than 10
		// digits and less than 20 digits. 

		*pSendData = ( 0x10 | ( cnt % 10 ) );

		// Increment pSendData to after end of PAN field. 
		pSendData += bytecount + 1;

		retval = True;
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Transaction Amount
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovTotAm( void )
{
	// Check the type of Transaction 
	switch ( TRREQ.TRKEY )
	{
		case AUTH:				// AUTHORIZATION 
		case CRDVER:			// CARD VERIFICATION 
			// Move Total Amount+Reauth Margain into 
			// Authorized Amount 
			memcpy( TRREQ.TRAUTHAMT, TRREQ.TRTOTAMI, S_TRAUTHAMT );
			// Call the Generic Move routine 
			MovGen( TRREQ.TRTOTAMI, prs8583->IS8583_LEN );
			break;

		case VOIDTRAN:			// VOID; call the Generic Move routine 
			MovGen( TRREQ.TRORIGAMT, prs8583->IS8583_LEN );
			break;

		case ADJSALE:			// ADJUST SALE   
		case ADJREFUND:		// ADJUST REFUND 
		case RUPLOAD:			// Upload the batch; see if Voided 
			if ( TRREQ.TRSTATUS[0] & TS1_VOID )
			{
				// Yes; clear the Amount 
				memset( TRREQ.TRTOTAM, 0, S_TRTOTAM );

				// And clear the Tip 
				memset( TRREQ.TRTIP, 0, S_TRTIP );
			}
			// Call the Generic Move routine 
			MovGen( TRREQ.TRTOTAM, prs8583->IS8583_LEN );
			break;

		default:				// Default; move Total Amount into Authorized
			// Amount 
			memcpy( TRREQ.TRAUTHAMT, TRREQ.TRTOTAM, S_TRAUTHAMT );
			// Call the Generic Move routine 
			MovGen( TRREQ.TRTOTAM, prs8583->IS8583_LEN );
			break;
	}

	// Data moved; return True 
	return ( True );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Systems Trace Audit Number
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovStan( void )
{
	if ( TRREQ.TRKEY == TESTTRAN )
	{
		// Note:  This will need to chek if in AutoTest mode when TESTTRAN 
		return ( False );
	}
	else
	{
		// Call the Generic Move routine 
		MovGen( TRREQ.TRSTAN, prs8583->IS8583_LEN );
		// Data moved; return True 
		return ( True );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Transaction Time
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovTime( void )
{
	// Check if field contains data 
	if ( IfThere( TRREQ.TRTIME, prs8583->IS8583_LEN ) )
	{
		// Yes; call the Generic Move routine 
		MovGen( TRREQ.TRTIME, prs8583->IS8583_LEN );
		// Data moved; return True 
		return ( True );
	}
	else
	{
		// No; data not moved, return False 
		return ( False );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Transaction Date
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovDate( void )
{
/*    if (LOCTAB.LANGID == LNG_FA) {
        // Check if field contains data 
        if ( IfThere( TRREQ.TRJALALIDATE, prs8583->IS8583_LEN ) )
        {
            // Yes; call the Generic Move routine 
            MovGen( TRREQ.TRJALALIDATE, prs8583->IS8583_LEN );

            // Data moved; return True
            return ( True );
        }
        else
        {
            // No; data not moved, return False
            return ( False );
        }
    }
*/
	// Check if field contains data 
	if ( IfThere( TRREQ.TRDATE, prs8583->IS8583_LEN ) )
	{
		// Yes; call the Generic Move routine 
		MovGen( TRREQ.TRDATE, prs8583->IS8583_LEN );

		// Data moved; return True
		return ( True );
	}
	else
	{
		// No; data not moved, return False
		return ( False );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Expiration Date
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Manually entered expiration date will be YYMM
//-----------------------------------------------------------------------------
extern Bool MovExpdYYMM( void )
{
    //if ( ( NullComp( ( char * ) TRREQ.TRTRK2, S_TRTRK2 ) ) || ( TRREQ.TRSTATUS[0] & ( TS1_ADV + TS1_ADJ )))
    if ((NullComp((char *)TRREQ.TRTRK2, S_TRTRK2)) || (TRREQ.TRSTATUS[0] & (TS1_ADV + TS1_ADJ)) || g_cancelTransactionType == CLOSEDPAUTHCANCEL)
	{
		// No; check if field contains data 
		if ( IfThere( TRREQ.TREXPD, prs8583->IS8583_LEN ) )
		{
			// Yes; call the Generic Move routine 
			MovGen( TRREQ.TREXPD, prs8583->IS8583_LEN );

			// Data moved 
			return ( True );
		}
	}

	// Return the data moved flag
	return ( False );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Expiration Date
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Manually entered expiration date will be MMYY
//-----------------------------------------------------------------------------
extern Bool MovExpdMMYY( void )
{
	Bool DataMoved;

	// Check if Track 2 is available
	if ( !NullComp( ( char * ) TRREQ.TRTRK2, S_TRTRK2 ) )
	{
		// Yes; no data to move
		DataMoved = False;
	}
	else
	{
		// No; check if field contains data
		if ( IfThere( TRREQ.TREXPD, prs8583->IS8583_LEN ) )
		{
			// Yes; move the Expiration Date Month to the send data
			pSendData[0] = TRREQ.TREXPD[1];

			// Move the Expiration Date Year to the send data
			pSendData[1] = TRREQ.TREXPD[0];

			// Increment pSendData to after end of Expiration Date
			pSendData += 2;

			// Data moved 
			DataMoved = True;
		}
		else
		{
			// No; data not moved
			DataMoved = False;
		}
	}

	// Return the data moved flag
	return ( DataMoved );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Point of Service Entry Mode
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovPose( void )
{
	// Call the Generic Move routine 
	MovGen( TRREQ.TRPOSE, prs8583->IS8583_LEN );

	// Data moved; return True 
	return ( True );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Network International Identifier
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovNII( void )
{
	// Update the TPDU in the Header 
	MoveItAq( SENDBUF + 1, TRREQ.TRAQPTR->AQNII, 2 );

	// Call the Generic Move routine 
	MovGen( SENDBUF + 1, prs8583->IS8583_LEN );

	// Data moved; return True
	return ( True );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the POS Condition Code
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovPocc( void )
{
	// Call the Generic Move routine 
	MovGen( &TRREQ.TRPOCC, prs8583->IS8583_LEN );

	// Data moved; return True 
	return ( True );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Track 2 Data
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovTrk2( void )
{
	UBYTE Length;
	UBYTE Index;
	UBYTE Value;

    // PreAuthClosedCancel da gonderilmez.
    if (g_cancelTransactionType == CLOSEDPAUTHCANCEL) {
        return False;
    }
	// Check if field contains data 
	if ( IfThere( TRREQ.TRTRK2, prs8583->IS8583_LEN ) )
	{
		// Initialize Track 2 data length 
		Length = 0;

		// Initialize index into temporary buffer 
		Index = 0;

		// Calculate Track 2 Data length 
		while ( True )
		{
			// Get byte value 
			Value = TRREQ.TRTRK2[Length];

			// See if Terminator found 
			if ( Value == 0xff )
			{
				// Yes; break out of the loop 
				break;
			}

			// Save right nibble, padded on the right 
			TmpBuf[Index] = ( Value << 4 ) | 0x0f;

			// Bump counter 
			Length++;

			// Check if done 
			if ( Length == S_TRTRK2 )
			{
				// Yes; break out of the loop 
				break;
			}

			// Get byte value 
			Value = TRREQ.TRTRK2[Length];

			// See if Terminator found 
			if ( Value == 0xff )
			{
				// Yes; break out of the loop 
				break;
			}

			// Save right nibble, masked with other nibble 
			TmpBuf[Index] &= Value | 0xf0;

			// Increment index into temporary buffer 
			Index++;

			// Increment counter 
			Length++;

			// Check if done 
			if ( Length == S_TRTRK2 )
			{
				// Yes; break out of the loop 
				break;
			}
		}

		// Call the Generic Move routine 
		MovGen( TmpBuf, Length );

		// Data moved; return True 
		return ( True );
	}
	else
	{
		// No; data not moved, return False 
		return ( False );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Retrieval Reference Number
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovRRN( void )
{
	// Check if field contains data
	if ( IfThere( TRREQ.TRRRN, prs8583->IS8583_LEN ) )
	{
		// Yes; call the Generic Move routine 
		MovGen( TRREQ.TRRRN, prs8583->IS8583_LEN );

		// Data moved; return True 
		return ( True );
	}
	else
	{
		// No; data not moved, return False 
		return ( False );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Auth ID Response (Approval code)
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovAuth( void )
{
    if (g_cancelTransactionType == VOIDTRAN) {
        return False;
    }
	// Check if field contains data
	if ( IfThere( TRREQ.TRAUTH, prs8583->IS8583_LEN ) )
	{
		// Yes; call the Generic Move routine 
		MovGen( TRREQ.TRAUTH, prs8583->IS8583_LEN );

		// Data moved; return True 
		return ( True );
	}
	else
	{
		// No; data not moved, return False 
		return ( False );
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Response code
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovResp( void )
{
	// Check if field contains data
	if ( IfThere( TRRSP.TRRSPC, S_TRRSPC ) )
	{
		// Yes; call the Generic Move routine 
		MovGen( TRRSP.TRRSPC, S_TRRSPC );

		// Data moved; return True 
		return ( True );
	}
	else
	{
		// No; data not moved, return False 
		return ( False );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Terminal ID or Terminal Serial Number based on Transaction type
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovTID( void )
{
	// When STATISTICS function is supported, we need to send terminal
	// serial number  
	// move Terminal ID to request record 
	MoveItAq( TRREQ.TRTID, TRREQ.TRAQPTR->AQTID, S_TRTID );

	// Call the Generic Move routine 
	MovGen( TRREQ.TRTID, prs8583->IS8583_LEN );

	// Data moved; return True 
	return ( True );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Acceptor ID
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovAccID( void )
{
	UBYTE tmpacq[S_AQACCID];

	// Move data from AQTAB
	MoveItAq( tmpacq, TRREQ.TRAQPTR->AQACCID, S_AQACCID );

	// Call the Generic Move routine
	MovGen( tmpacq, prs8583->IS8583_LEN );

	// Data moved; return True
	return ( True );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Additional Response Data
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovRspa( void )
{
	// Check if Additional Response has data 
	if ( IfThere( TRREQ.TRRSPA, S_TRRSPA ) )
	{
		// Call the Generic Move routine 
		MovGen( TRREQ.TRRSPA, S_TRRSPA );

		// Data moved; return True 
		return ( True );
	}
	else
	{
		// No; data not moved, return False 
		return ( False );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the PIN Number
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovPin( void )
{
	// Do not move the pin block if it is nulls. 
	if ( IfThere( TRREQ.TRPINBL, prs8583->IS8583_LEN ) )
	{
		// Move the pin block data to output buffer. 
		MovGen( TRREQ.TRPINBL, prs8583->IS8583_LEN );
		return ( True );
	}
	else
	{
		// No; data not moved, return False 
		return ( False );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the Tip Amount
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovTip( void )
{
	Bool DataMoved;

	// Check if voided 
	if ( TRREQ.TRSTATUS[0] & TS1_VOID )
	{
		// Yes; no data to move 
		DataMoved = False;
	}
	else
	{
		// No; check if field contains data 
		if ( IfThere( TRREQ.TRTIP, S_TRTIP ) )
		{
			// Yes; prepare the Tip Amount for moving 
			BfAscii( ( char * ) TmpBuf, TRREQ.TRTIP, S_TRTIP );

			// Call the Generic Move routine 
			MovGen( TmpBuf, S_TRTIP * 2 );

			// Data moved 
			DataMoved = True;
		}
		else
		{
			// No; data not moved 
			DataMoved = False;
		}
	}

	// Return the data moved flag 
	return ( DataMoved );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the description
//!
//! \return
//!     True if data has been moved otherwise False
//-----------------------------------------------------------------------------
extern Bool MovDesc( void )
{
	UBYTE i, j;
	struct desc_rec *pDscRec;

	if ( IfThere( TRREQ.TRDESC, S_TRDESC ) )
	{
		// There is descriptor data - Build in TmpBuf  
		memset( ( char * ) TmpBuf, ' ',( S_TRDESC * 2 ) );

		// Search descriptor table using keys from TRREQ.TRDESC  
		for ( i = 0, j = 0; i < S_TRDESC; i++ )
		{
			// If descriptor exists copy it to TmpBuf.  
			if ( ( pDscRec = FindDesc( TRREQ.TRDESC[i] ) ) != 0x0000 )
			{
				// Copy DSCODE for found entry to TmpBuf.  
				memcpy( &TmpBuf[j], pDscRec->DSCODE, S_DSCODE );
				j += 2;
			}
		}
		if ( j != 0 )
		{
			// Call the Generic Move routine 
			MovGen( TmpBuf, ( S_TRDESC * 2 ) );

			// Data moved;  return True  
			return ( True );
		}
		else
		{
			// Data was not moved;  return False 
			return ( False );
		}
	}
	else
	{
		// Data does not exist; return False 
		return ( False );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Formats Cashier/Till information of bit63
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_Cashier( void )
{
	Bool retval;
	int i;

	retval = False;

	// The cashier field of bit63 includes cashier number & Till number.
	// If the Till option is not enabled, it will be space-filled.  

	if ( TCONF.TCOPT1 & TC1_CASHIER )
	{
		// T7 MULTI-HOST application has a fixed length for this field, 
		// 00, 10(BCD). 

		*pSendData = 0;
		*( pSendData + 1 ) = 0x10;
		//       TYPE "10" Cashier/Till data  
		*( pSendData + 2 ) = '1';
		*( pSendData + 3 ) = '0';

		for ( i = 0; i < 4; i++ )
		{
			if ( TRREQ.TRCASHIER[i] )
				*( pSendData + 4 + i ) = TRREQ.TRCASHIER[i];
			else
				*( pSendData + 4 + i ) = ' ';
		}

		for ( i = 0; i < 4; i++ )
		{
			if ( TRREQ.TRTILLNO[i] )
				*( pSendData + 8 + i ) = TRREQ.TRTILLNO[i];
			else
				*( pSendData + 8 + i ) = ' ';
		}
		pSendData += 12;
		retval = True;
		B63_Len += 12;
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Formats CVV2 field of bit63
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_CVV2( void )
{
	Bool retval;
	UWORD len;

	// Initialize move status to false 
	retval = False;

	// See if Adjust and Advice 
	if ( !( TRREQ.TRSTATUS[0] & ( TS1_ADV + TS1_ADJ ) ) )
	{
		// No, See if Upload 
		if ( RUPLOAD != TRREQ.TRKEY )
		{
			// No, See if CVV2 present 
			if ( TRREQ.TRCVV2IND > 0x20 )
			{
				*pSendData = 0;
				len = ( S_TRCVV2 + 2 );

				// Move length in BCD form 
				*( pSendData + 1 ) = BinBcdB( ( UBYTE ) ( len + 2 ) );

				*( pSendData + 2 ) = '1';
				*( pSendData + 3 ) = '6';

				// Move CVV2IND data into SENDBUF 
				*( pSendData + 4 ) = TRREQ.TRCVV2IND;
				*( pSendData + 5 ) = '0';

				// Move CVV2 data into SENDBUF 
				memcpy( pSendData + 6, TRREQ.TRCVV2, S_TRCVV2 );

				// Update the data pointer 
				pSendData += ( len + 4 );

				B63_Len += ( len + 4 );

				retval = True;
			}
		}
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Formats PS 2000 field of bit63
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_PS2000( void )
{
	Bool retval;
	UWORD len;

	// Initialize move status to false 
	retval = False;

	// See if UPLOAD transaction 
	if ( RUPLOAD == TRREQ.TRKEY )
	{
		// Yes, See if PS2000 exists 
		if ( TRREQ.TRPSI > 0x20 )
		{
			*pSendData = 0;
			len =
				sizeof( TRREQ.TRPSI ) + S_TRTRANID + S_TRVALID;

			// Move length in BCD form 
			*( pSendData + 1 ) = BinBcdB( ( UBYTE ) ( len + 2 ) );

			*( pSendData + 2 ) = '2';
			*( pSendData + 3 ) = '0';

			// Move PS2000 data into SENDBUF 
			memcpy( pSendData + 4, &TRREQ.TRPSI, len );

			// Update the data pointer 
			pSendData += ( len + 4 );

			B63_Len += ( len + 4 );

			*pSendData = 0;
			len = S_TRAUTHAMT * 2;
			*( pSendData + 1 ) = BinBcdB( ( UBYTE ) ( len + 2 ) );	// length in BCD form 

			*( pSendData + 2 ) = '2';
			*( pSendData + 3 ) = '1';

			BfAscii( ( char * ) pSendData + 4, TRREQ.TRAUTHAMT, S_TRAUTHAMT );
			pSendData += ( 4 + ( S_TRAUTHAMT * 2 ) );

			B63_Len += ( 4 + ( S_TRBASE * 2 ) );

			retval = True;
		}
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Formats Host Reference field of bit63
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_Hostref( void )
{
	Bool retval;
	UWORD len;

	// Initialize move status to false 
	retval = False;

	// See if Host Reference field needs to be moved (Adjust/Advice or
	// an upload transaction detail request) 
	if ( ( TRREQ.TRSTATUS[0] & ( TS1_ADV + TS1_ADJ ) ) ||
		 ( RUPLOAD == TRREQ.TRKEY ) )
	{
		// Yes; get the length of the data to be moved 
		len = *( UWORD * ) TRREQ.TRREFDATA;

		// See if any data to be moved 
		if ( len )
		{
			// Yes; move in the length field (length of the data to be
			// moved + length of type field) 
			*( UWORD * ) pSendData = BinBcdW( ( UWORD ) ( len + 2 ) );

			// Update the data pointer 
			pSendData += 2;

			// Move in the Type field 
			*pSendData = '3';
			*( pSendData + 1 ) = '1';

			// Update the data pointer 
			pSendData += 2;

			// Move the Host Reference data into SENDBUF 
			memcpy( pSendData, &TRREQ.TRREFDATA[2], len );

			// Update the data pointer 
			pSendData += len;

			// Update the length of the Bit 63 field 
			B63_Len += ( 4 + len );

			// Set the move status to true 
			retval = True;
		}
	}

	// Return move status to caller 
	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Formats the Key Serial Number field of bit63
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_DUKPT( void )
{
	Bool retval;
	UBYTE len;

	retval = False;

	// If TRKSN in TRREQ is not empty, we need to send KSN 

	if ( !NullComp( ( char * ) TRREQ.TRKSN, S_TRKSN ) )
	{
		// KSN is not empty, we need to move KSN into SENDBUF 

		// 2-byte length field. Put a NULL at first byte then add the length+2
		// (2-byte field type - "33" for KSN amount) 

		*pSendData = 0;
		len = S_TRKSN * 2;

		*( pSendData + 1 ) = BinBcdB( ( UBYTE ) ( len + 2 ) );	// length in BCD form 

		// Move  "33" ); 
		*( pSendData + 2 ) = '3';
		*( pSendData + 3 ) = '3';
		BfAscii( ( char * ) pSendData + 4, TRREQ.TRKSN, S_TRKSN );
		pSendData = pSendData + 4 + len;
		B63_Len += ( 4 + len );
		retval = True;
	}
	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Formats the Additional Prompt field of bit 63
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_Addprmpt( void )
{
	Bool retval;
	int idx;
	int len;
	int totallen;

	retval = False;

	idx = 1;
	totallen = len = 0;

	if ( !NullComp( ( char * ) TRREQ.TRAP, 2 ) )
	{
		// quick & dirty - only 70 bytes were allocated for additional prompt.
		// Assuming the prompt cannot be longer than 255 bytes,
		// we can safely add 2 (length of data type to the 
		// lower byte for the length. 

		while ( TRREQ.TRAP[idx] )
		{
			len = CvtBin( TRREQ.TRAP[idx] );
			totallen += ( len + 2 );
			idx += ( len + 2 );	// skipping data & length byte 
		}
		*pSendData = TRREQ.TRAP[0];

		*( pSendData + 1 ) = BinBcdB( ( UBYTE ) ( totallen + 2 ) );
		//       TYPE "35" ADDITIONAL PROMPT DATA  
		*( pSendData + 2 ) = '3';
		*( pSendData + 3 ) = '5';
		memcpy( pSendData + 4, TRREQ.TRAP, ( UWORD ) totallen );
		pSendData = pSendData + 4 + totallen;
		B63_Len += ( 4 + totallen );
		retval = True;
	}
	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Formats Current Batch Number
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_BatNum( void )
{
	struct batch_rec *pBatchRec;
	Bool retval;
	UBYTE len;

	// Initialize move status to false 
	retval = False;

	// See if bit AQ2_SENDBATNO is set 
	if ( TRREQ.TRAQPTR->AQOPT2 & AQ2_SENDBATNO )
	{
		// Yes; move in the Length field 
		pSendData[0] = 0x00;
		len = S_BATCUR * 2;
		pSendData[1] = len + 2;

		// Update the data pointer 
		pSendData += 2;

		// Move in the Type field 
		*( pSendData + 2 ) = '3';
		*( pSendData + 3 ) = '7';

		// Update the data pointer 
		pSendData += 2;

		// Get the batch table record for this acquirer 
		pBatchRec = FindBatTab( TRREQ.TRAQID );

		// Moves the current batch number into SENDBUF 
		BfAscii( ( char * ) pSendData, pBatchRec->BATCUR, S_BATCUR );

		// Update the data pointer 
		pSendData += len;

		// Update the length of the Bit 63 field 
		B63_Len += ( 4 + len );

		// Set the move status to true 
		retval = True;
	}

	// Return move status to caller 
	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves the tax amount into bit63 area
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_TaxAmt( void )
{
	Bool retval;
	UBYTE len;

	retval = False;

	// If TRTAX in TRREQ is not empty, we need to send TAX amount 

	if ( !NullComp( ( char * ) TRREQ.TRTAX, S_TRBASE ) )
	{
		// tax is not empty, we need to move tax amount into SENDBUF 

		// 2-byte length field. Put a NULL at first byte then add the length+2
		// (2-byte field type - "39" for TAX amount) 

		*pSendData = 0;
		len = S_TRBASE * 2;
		*( pSendData + 1 ) = BinBcdB( ( UBYTE ) ( len + 2 ) );	// length in BCD form 

		*( pSendData + 2 ) = '3';
		*( pSendData + 3 ) = '9';
		BfAscii( ( char * ) pSendData + 4, TRREQ.TRTAX, S_TRBASE );
		pSendData = pSendData + 4 + len;
		B63_Len += ( 4 + len );

		retval = True;
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves CASHBK amount into Bit63 buffer
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_CashBK( void )
{
	Bool retval;
	UBYTE len;

	retval = False;

	// If TRCASHBK in TRREQ is not empty, we need to send CASHBK amount 

	if ( !NullComp( ( char * ) TRREQ.TRCASHBK, S_TRBASE ) )
	{
		// CASHBK is not empty, we need to move CASHBK amount into SENDBUF 

		// 2-byte length field. Put a NULL at first byte then add the length+2
		// (2-byte field type - "41" for CASHBK amount) 

		*pSendData = 0;
		len = S_TRBASE * 2;
		*( pSendData + 1 ) = BinBcdB( ( UBYTE ) ( len + 2 ) );	// length in BCD form 

		*( pSendData + 2 ) = '4';
		*( pSendData + 3 ) = '1';
		BfAscii( ( char * ) pSendData + 4, TRREQ.TRCASHBK, S_TRBASE );
		pSendData = pSendData + 4 + ( S_TRBASE * 2 );
		B63_Len += ( 4 + ( S_TRBASE * 2 ) );

		retval = True;
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves BAR amount into SENDBUF
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_BarAmt( void )
{
	Bool retval;
	UBYTE len;

	retval = False;

	// If TRBAR in TRREQ is not empty, we need to send BAR amount 

	if ( !NullComp( ( char * ) TRREQ.TRBAR, S_TRBASE ) )
	{
		// bar is not empty, we need to move bar amount into SENDBUF 

		// 2-byte length field. Put a NULL at first byte then add the length+2
		// (2-byte field type - "53" for BAR amount) 

		*pSendData = 0;
		len = S_TRBASE * 2;
		*( pSendData + 1 ) = BinBcdB( ( UBYTE ) ( len + 2 ) );	// length in BCD form 

		*( pSendData + 2 ) = '5';
		*( pSendData + 3 ) = '3';
		BfAscii( ( char * ) pSendData + 4, TRREQ.TRBAR, S_TRBASE );
		pSendData = pSendData + 4 + ( S_TRBASE * 2 );
		B63_Len += ( 4 + ( S_TRBASE * 2 ) );

		retval = True;
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves standard totals for CLBATCH, CLBATCHA and CLBATCHE
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_MoveStdTotals( void )
{
	UBYTE len, index;
	char tmp[4];

	// Build the totals in TmpBuf then copy to the send buffer. 
	memset( ( char * ) &TmpBuf, '0',90 );
	index = 0;

	// Capture sales count and capture sales amount. 
	BfAscii( tmp, STDTOTALS.TOTCPSALCNT, S_TOTCPSALCNT );
	memcpy( &TmpBuf[index], &tmp[1], 3 );
	index += 3;
	BfAscii( ( char * ) &TmpBuf[index], STDTOTALS.TOTCPSALAMT,
			 S_TOTCPSALAMT );
	index += 12;

	// Capture refund count and capture refund amount. 
	BfAscii( tmp, STDTOTALS.TOTCPREFCNT, S_TOTCPREFCNT );
	memcpy( &TmpBuf[index], &tmp[1], 3 );
	index += 3;
	BfAscii( ( char * ) &TmpBuf[index], STDTOTALS.TOTCPREFAMT,
			 S_TOTCPREFAMT );
	index += 12;

	// Debit sales count and debit sales amount.  
	BfAscii( tmp, STDTOTALS.TOTDBSALCNT, S_TOTDBSALCNT );
	memcpy( &TmpBuf[index], &tmp[1], 3 );
	index += 3;
	BfAscii( ( char * ) &TmpBuf[index], STDTOTALS.TOTDBSALAMT,
			 S_TOTDBSALAMT );
	index += 12;

	// Debit refund count and debit refund amount.  
	BfAscii( tmp, STDTOTALS.TOTDBREFCNT, S_TOTDBREFCNT );
	memcpy( &TmpBuf[index], &tmp[1], 3 );
	index += 3;
	BfAscii( ( char * ) &TmpBuf[index], STDTOTALS.TOTDBREFAMT,
			 S_TOTDBREFAMT );
	index += 12;

	// Move length in BCD form to the send buffer.  
	*pSendData = 0;
	len = 90;
	*( pSendData + 1 ) = BinBcdB( len );

	// Move totals from the temp buffer to the send buffer. 
	memcpy( pSendData + 2, TmpBuf, 90 );
	// Increment the pointer to the data. 
	pSendData += ( 90 + 2 );

	return ( True );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves totals for CLBATCH, CLBATCHA and CLBATCHE
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_MoveTotals( void )
{
	struct batch_rec *pBatchRec;
	UBYTE len, index;
	char tmp[4];

	// Build the totals in TmpBuf then copy to the send buffer. 
	memset( ( char * ) &TmpBuf, ' ',36 );
	index = 0;

	// Capture sales count and capture sales amount. 
	BfAscii( tmp, TERMTOTALS.TOTBASIC.TOTDBCNT, S_TOTDBCNT );
	memcpy( &TmpBuf[index], &tmp[1], 3 );
	index += 3;
	BfAscii( ( char * ) &TmpBuf[index], TERMTOTALS.TOTBASIC.TOTDBAMT,
			 S_TOTDBAMT );
	index += 12;

	// Capture refund count and capture refund amount. 
	BfAscii( tmp, TERMTOTALS.TOTBASIC.TOTCRCNT, S_TOTCRCNT );
	memcpy( &TmpBuf[index], &tmp[1], 3 );
	index += 3;
	BfAscii( ( char * ) &TmpBuf[index], TERMTOTALS.TOTBASIC.TOTCRAMT,
			 S_TOTCRAMT );
	index += 12;

	// Get the batch table record for this acquirer 
	pBatchRec = FindBatTab( TRREQ.TRAQID );

	// Moves the current batch reversal count to the temp buffer 
	BfAscii( tmp, pBatchRec->BATREV, S_BATREV );
	memcpy( &TmpBuf[index], &tmp[1], 3 );
	index += 3;

	// Moves the current batch adjustment count to the temp buffer 
	BfAscii( tmp, pBatchRec->BATADJ, S_BATADJ );
	memcpy( &TmpBuf[index], &tmp[1], 3 );
	index += 3;

	// Move length in BCD form to the send buffer.  
	*pSendData = 0;
	len = 36;
	*( pSendData + 1 ) = BinBcdB( len );

	// Move totals from the temp buffer to the send buffer. 
	memcpy( pSendData + 2, TmpBuf, 36 );
	// Increment the pointer to the data. 
	pSendData += ( 36 + 2 );

	return ( True );
}


//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     Finds Move Table Entry
//!
//! \param[in] bitno            bit number
//!
//! \return
//!     Pointer to entry for bit number
//-----------------------------------------------------------------------------
static struct move_rec *FindMoveEntry( UBYTE bitno )
{
	struct move_rec *pMoveRec;

	// Start at first record
	pMoveRec = ( struct move_rec * ) Move_Table;

	// Do till record found or end of table
	while ( ( pMoveRec->movbitno != 00 ) && ( pMoveRec->movbitno != bitno ) )
	{
		// Look at next record
		pMoveRec++;
	}

	// If not found return NULL, else return ptr
	if ( pMoveRec->movbitno == 00 )
		return ( NULL );
	else
		return ( pMoveRec );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Moves Data to the Send Buffer
//!
//! \param[in] bitno            bit number
//!
//! \return
//!     -1 - bad,<br>
//!     0 - data not moved,<br>
//!     1 - data moved
//-----------------------------------------------------------------------------
static BYTE MoveData( UBYTE BitNum )
{
	// Get attributes for bit
	prs8583 = FindBit( BitNum );

	// Check if found 
	if ( prs8583 != NULL )
	{
		// Yes; find Move Entry 
		pmove_rec = FindMoveEntry( BitNum );

		// Check if entry found
		if ( pmove_rec != NULL )
		{
			// Yes; call the move routine 
			if ( pmove_rec->mov_it(  ) )
			{
				// Data moved; return data moved status 
				return ( 1 );
			}
			else
			{
				// Data not moved; return data not moved status 
				return ( 0 );
			}
		}
	}
	// No; return bad status 
	return ( -1 );
}

#ifdef MAKE_EMV
//-----------------------------------------------------------------------------
//! \brief
//!     Moves the ICC data (bit 55) into SENDBUF
//!
//! \return
//!     Always returns True
//-----------------------------------------------------------------------------
extern Bool MovICC( void )
{

	static UBYTE Bit55Data[280];	// construction area
    UBYTE Offset;

	// initialise bit55data
	memset( Bit55Data, 0x00, 280 );

	// Bit 55 fields
// 0x8400 (DF Name) is not available to application in the current version of Level 2 library
// (because EMV_INIT_ICC_DATA is called in the beginning of INITIATEAPPL(see emvlib.lib sources))

    // note that hardcoding the first offsets saves 15 bytes per increment !

	// 5F2A - txn currency code
	Bit55Data[0] = 0x5F;
	Bit55Data[1] = 0x2A;
	Bit55Data[2] = S_TCCURRCODE;
	memcpy( &Bit55Data[3], TCONF.TCCURRCODE, S_TCCURRCODE );

	// 5F34 - PAN Sequence Number
	Bit55Data[5] = 0x5F;
	Bit55Data[6] = 0x34;
	Bit55Data[7] = 0x01;
	Bit55Data[8] = TRINP.TRPANSEQ;

	// 82 - AIP
	Bit55Data[9] = 0x82;
	Bit55Data[10] = S_TRAIP;
	memcpy( &Bit55Data[11], TRREQ.TRAIP, S_TRAIP );

//    //{  0x8400, TRREQ.TRMCAID, S_TRMCAID, Mov55_8400 },    /* Application Identifier */
	// 84 - AID - have to know how long this is
	Bit55Data[13] = 0x84;
	Bit55Data[14] = TRREQ.TRAID[0];
	memcpy( &Bit55Data[15], &TRREQ.TRAID[1], TRREQ.TRAID[0] );
	Offset = TRREQ.TRAID[0] + 15; // first variable length field - must use offset

	// 95 - TVR
	//{  0x9500, TRREQ.TRTVR, S_TRTVR, NULL },              /* Terminal Verification Results */
	Bit55Data[Offset++] = 0x95;
	Bit55Data[Offset++] = S_TRTVR;
	memcpy( &Bit55Data[Offset], TRREQ.TRTVR, S_TRTVR );
	Offset += S_TRTVR;


	// 9A - txn date
	//{  0x9A00, TRREQ.TRYEAR, S_TRYEAR+S_TRDATE, NULL },   /* Transaction Date */
	Bit55Data[Offset++] = 0x9A;
	Bit55Data[Offset++] = ( S_TRYEAR + S_TRDATE );
  //  if (LOCTAB.LANGID == LNG_FA) {
  //      memcpy( &Bit55Data[Offset], TRREQ.TRJALALIYEAR, S_TRYEAR + S_TRDATE );
  //  }
  //  else {
        memcpy( &Bit55Data[Offset], TRREQ.TRYEAR, S_TRYEAR + S_TRDATE );
  //  }
	Offset += ( S_TRYEAR + S_TRDATE );

	// 9C - txn type
	Bit55Data[Offset++] = 0x9C;
	Bit55Data[Offset++] = 1;
	Bit55Data[Offset++] = TRREQ.TRPROC[0];

	// 9F02 - amount auth'd
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x02;
	Bit55Data[Offset++] = S_TRBASE;
	memcpy( &Bit55Data[Offset], TRREQ.TRTOTAM, S_TRBASE );
	Offset += S_TRBASE;

	// 9F03 - amount other - only move if present
	//if ( !NullComp( (char *) TRREQ.TRCASHBK, S_TRBASE ) )
	//{
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x03;
	Bit55Data[Offset++] = S_TRBASE;
	memcpy( &Bit55Data[Offset], TRREQ.TRCASHBK, S_TRBASE );
	Offset += S_TRBASE;
	//}

	// 9F07 - App Usage Ctrl - ?
	//Bit55Data[Offset++] = 0x9F;
	//Bit55Data[Offset++] = 0x07;
	//if ( EMVL2_ReadTagData (&Bit55Data[Offset+1], &Bit55Data[Offset], TAG_9F07))
	//    Offset += Bit55Data[Offset] + 1;
	//else
	//    Offset -= 2;

	// 9F09 - App version number
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x09;
	Bit55Data[Offset++] = sizeof( TRREQ.TRTAIDVER );
	memcpy( &Bit55Data[Offset], TRREQ.TRTAIDVER, sizeof( TRREQ.TRTAIDVER ) );
	Offset += sizeof( TRREQ.TRTAIDVER );

	// 9F10 - Issuer App Data - variable size
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x10;
	memcpy( &Bit55Data[Offset + 1], &TRREQ.TRISAPDAT[1], TRREQ.TRISAPDAT[0] );
	Bit55Data[Offset] = TRREQ.TRISAPDAT[0];
	Offset += TRREQ.TRISAPDAT[0] + 1;

	//{  0x9F1A, TCONF.TCCTRYCODE, S_TCCTRYCODE, NULL },    /* Terminal Country Code */
	// 9F1A - Term Country Code
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x1A;
	Bit55Data[Offset++] = S_TCCTRYCODE;
	memcpy( &Bit55Data[Offset], TCONF.TCCTRYCODE, S_TCCTRYCODE );
	Offset += S_TCCTRYCODE;

	//{  0x9F1E, TERM.TERMSer, sizeof(TERM.TERMSer), NULL },/* Interface Device Serial Number (IFD) */
	// 9F1E - TID (IFD Serial Number)
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x1E;
	Bit55Data[Offset++] = sizeof( TERM.TERMSer );
	memcpy( &Bit55Data[Offset], (const void*)TERM.TERMSer, sizeof( TERM.TERMSer ) );
	Offset += sizeof( TERM.TERMSer );

	// 9F26 - Cryptogram
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x26;
	Bit55Data[Offset++] = S_TRAC;
	memcpy( &Bit55Data[Offset], TRREQ.TRAC, S_TRAC );
	Offset += S_TRAC;

	//{  0x9F27, &TRREQ.TRMCCRYPTINFO, 1, NULL },             /* Cryptogram Information Data */
	// 9F27 - CID
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x27;
	Bit55Data[Offset++] = 1;
	Bit55Data[Offset++] = TRREQ.TRCRYPTINFO;

	//{  0x9F33, NULL, 3, Mov55_9F33 },                       /* Terminal Capabilities */
	// 9F33 - Term Capabilities
	// cf. static const UBYTE CAPABILDEF in hdt6 system - needs sorted !!!
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x33;
	Bit55Data[Offset++] = 0x03;
    memcpy( &Bit55Data[Offset], TCONF.TCTCAP , S_TCTCAP);
	Offset += S_TCTCAP;

	// 9F34 - CVM
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x34;
	Bit55Data[Offset++] = 0x03;
	memcpy( &Bit55Data[Offset], TRREQ.TRCVMRESULTS,	3 );
	Offset += 3;

	// 9F35 - Terminal Type
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x35;
	Bit55Data[Offset++] = 1;
	//  we have a offline terminal with online capability - Issue #643 - so fix as 0x22
	Bit55Data[Offset++] = 0x22 ; // ( TCONF.TCOPT6 & TC6_ONLINE ) ? 0x22 : 0x23;

	// 9F36 - ATC
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x36;
	Bit55Data[Offset++] = S_TRATC;
	memcpy( &Bit55Data[Offset], TRREQ.TRATC, S_TRATC );
	Offset += S_TRATC;

	//{  0x9F37, TRREQ.TRRAND, S_TRRAND, NULL },              /* Unpredictable Number */
	// 9F37 - Unpred Number
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x37;
	Bit55Data[Offset++] = S_TRRAND;
	memcpy( &Bit55Data[Offset], TRREQ.TRRAND, S_TRRAND );
	Offset += S_TRRAND;

	//{  0x9F41, NULL, 4, Mov55_9F41 }, /* Transaction Sequence Counter (Invoice Number) */
	// 9F41 - Txn Seq Counter
	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x41;
	Bit55Data[Offset++] = (S_TRINV/2); // length of value
    AscHex( &Bit55Data[Offset], (char *) TRREQ.TRINV, (S_TRINV/2) );
	Offset += (S_TRINV/2);

	//{  0x9F53, NULL, 1, Mov55_9F53 },                       /* Transaction Category Code */
	// 9F53 - Txn Cat Code
	//Bit55Data[Offset++] = 0x9F;
	//Bit55Data[Offset++] = 0x53;
	//if ( !EMVL2_ReadTagData (&Bit55Data[Offset+1], &Bit55Data[Offset], TAG_9F53))
	//    Offset += Bit55Data[Offset] + 1;
	//else
	//    Offset -= 2;

	/* the setup for MovGen is done earlier from the Move Table */
	MovGen( Bit55Data, Offset );

	/* Data moved */
	return ( True );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Formats 'Issuer Script Results' field of bit63
//!
//! \return
//!     True if data has been moved otherwise False
//!
//! \note
//!     Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
extern Bool B63_IssScriptRes( void )
{
	Bool retval = False;	// Initialise return status
	UBYTE safety_cnt;
	UWORD len;
	UBYTE *pLen;			// pointer to BCD length destination
	UBYTE *pSrc;

	/* Only send the subfield if it's not empty, valid data
	   in TRREQ.TRISSSCRIPTRES must start with 0xFF */
	if( 0xFF == TRREQ.TRISSSCRIPTRES[0] )
	{
		/* Save pointer to BCD length */
		pLen = pSendData;

		/* First byte of length will always be zero */
		*pSendData = 0;
		/* Skip the 2nd length byte, we'll set it later */
		/* Move subfield identifier 'SR' */
		*(pSendData+2) = 'S';
		*(pSendData+3) = 'R';

		/* Update pointer to destination */
		pSendData += 4;

		/* Initialise pointer to source */
		pSrc = TRREQ.TRISSSCRIPTRES;

		/* Initialise safety count with max number of results (currently 8) */
		safety_cnt = S_TRISSSCRIPTRES / 6;

		/* Move the data into send buffer until all results are moved
		   or safety counter becomes zero */
		while ( (0xFF == *pSrc) && (safety_cnt--) )
		{
			/* Move 5 bytes of data (skipping the presence indicator in the source) */
			memcpy( pSendData, (pSrc+1), 5 );
			pSendData += 5;
			pSrc += 6;
		}

		/* All data moved, calculate the actual length of the subfield */
		len = (pSendData - pLen);

		/* Set the 2nd byte of BCD length */
		*(pLen+1) = BinBcdB( (UBYTE) (len - 2) );

		/* Update the total field 63 length */
		B63_Len += len;

		/* Set return status to True */
		retval = True;
	}

	return(retval);
}
#endif	// MAKE_EMV

