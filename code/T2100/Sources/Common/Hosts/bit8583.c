
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
extern char cSerNum[256];


//=============================================================================
// Public data definitions
//=============================================================================
const struct move_rec *Move_Table;
static const struct move_rec *pmove_rec;
UBYTE *pSendData;				// Pointer into send buffer
UBYTE *pB63Len;					// Pointer to Bit 63 length bytes  
int B63_Len;					// Bit 63 total length  



//=============================================================================
// Private data definitions
//=============================================================================



//=============================================================================
// Public function definitions
//=============================================================================





//-----------------------------------------------------------------------------
//  ReqPackMsg      Pack the Message Request into the Send Buffe 
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool            True - Complete
//                                  False - Not Complete
//  Notes:
//
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
//  IfThere     See if field is present
//
//  Parameters: UBYTE * pField          pointer to the field to be checked
//
//              UWORD Length            length of the field to be checked
//
//  Global Inputs:
//
//  Returns:    Bool        True  - field contains data
//                          False - field does not contain data.
//  Notes:
//
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
//  MovGen          Generic Move
//
//  Parameters:     UBYTE *pData    pointer to the data to be moved
//
//                  UWORD Length    length of the data to be moved
//      
//  Global Inputs:
//
//  Returns:        None
//
//  Notes:
//
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
//  MovPan          Copy PAN into SENDBUF if TRACK 2 data does not exist 
//
//  Parameters:     None

//  Global Inputs:  pSendData   points to the destination
//
//  Returns:        Bool    True if data has been copied
//                          False Otherwise
//  Notes:
//
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

	if ( ( NullComp( ( char * ) TRREQ.TRTRK2, S_TRTRK2 ) ) || (TRREQ.TRKEY == RUPLOAD) )
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
//  MovTotAm        Move the Amount, Transaction
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True - Data moved
//                              False - Data not moved 
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool MovTotAm( void )
{
	// Check the type of Transaction 
	switch ( TRREQ.TRKEY )
	{		
		case VOIDTRAN:			
			MovGen( TRREQ.TRORIGAMT, prs8583->IS8583_LEN );
			break;

		case RUPLOAD:			
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
//  MovStan         Move the Systems Trace Audit Number
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True - Data moved
//                              False - Data not moved
//
//  Notes:  To be completed
//
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
//  MovTime         Move the Time, Transaction
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//  Notes:
//
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
//  MovDate         Move the Date, Transaction
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:    Bool        True  - Data moved
//                          False - Data not moved
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool MovDate( void )
{
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
//  MovExpdYYMM     Move the Date, Expiration
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//          
//  Notes:          Manually entered expiration date will be YYMM
//
//-----------------------------------------------------------------------------

extern Bool MovExpdYYMM( void )
{

    if ( ( NullComp( ( char * ) TRREQ.TRTRK2, S_TRTRK2 ) ))
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
//  MovExpdMMYY     Move the Date, Expiration
//
//  Parameters:     None

//  Global Inputs:
//
//  Returns:        Bool    True  - Data moved
//                          False - Data not moved
//
//  Notes:          Manually entered expiration date will be MMYY
//
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
//  MovPose         Move the Point of Service Entry Mode
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True - Data moved
//                              False - Data not moved
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool MovPose( void )
{
	// Call the Generic Move routine 
	MovGen( TRREQ.TRPOSE, prs8583->IS8583_LEN );

	// Data moved; return True 
	return ( True );
}



//-----------------------------------------------------------------------------
//  MovNII          Move the Network International Identifier
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//
//  Notes:
//
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
//  MovPocc         Move the POS Condition Code
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool MovPocc( void )
{
        
	// Call the Generic Move routine 
	MovGen( &TRREQ.TRPOCC, prs8583->IS8583_LEN );

	// Data moved; return True 
	return ( True );
}



//-----------------------------------------------------------------------------
//  MovTrk2         Move the Track 2 Data
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool MovTrk2( void )
{
	UBYTE Length;
	UBYTE Index;
	UBYTE Value;

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
//  MovRRN          Move the Retrieval Reference Number
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//  Notes:
//
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
//  MovAuth         Move the Auth ID Response (Approval code)
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool            True  - Data moved
//                                  False - Data not moved
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool MovAuth( void )
{
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
//  MovResp         Move the Response code
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool            True  - Data moved
//                                  False - Data not moved
//  Notes:
//
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
//  MovTID          Move the Terminal ID or Terminal Serial Number
//                  based on Transaction type
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//
//  Notes:
//
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
//  MovAccID        Move the Acceptor ID
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//  Notes:
//
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
//  MovRspa         Move the Additional Response Data
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//  Notes:
//
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
//  MovPin          Move the PIN Number
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool MovPin( void )
{

      //const UBYTE pin[] =  { 0xCA, 0x26, 0x6F, 0xCD, 0xC2, 0x82, 0xBD, 0xA4 };

      // Reversal ise field 52 gitmez 
      if ( TRREQ.TRSTATUS[0] & TS1_REV ) 
        return ( False );

      //memcpy ( TRREQ.TRPINBL, pin, sizeof(pin) );

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
//  MovTip          Move the Tip Amount
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//  Notes:
//
//-----------------------------------------------------------------------------

extern Bool MovTip( void )
{
  return False;
}



//-----------------------------------------------------------------------------
//  MovDesc         Move the description
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//  Notes:
//
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
//  B63_CVV2        Format CVV2 field of bit63
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//
//  Notes:          Destination buffer pointed by pSendData
//
//-----------------------------------------------------------------------------
// mail order iþleminde ve elle giriþli satýþta CVV2 gönderilir . 
extern Bool B63_CVV2( void )
{
	Bool retval;
	UWORD len;

	// Initialize move status to false 
	retval = False;
        
        if ( (TRREQ.TRKEY == SALE  || 
                  TRREQ.TRKEY  == INSTALLMENT ) && !INJECTKEY)
              return False;
        
	// See if Adjust and Advice 
	if ( !( TRREQ.TRSTATUS[0] & ( TS1_ADV ) ) )
	{
		// No, See if Upload 
		if ( RUPLOAD != TRREQ.TRKEY )
		{
                        if((TRINP.TRPOSE[1] == 0x12 ) &&(TRINP.TRISPTR->ISOPT5 & IS5_CVV2)) 
			{
				*pSendData = 0;
				len = ( S_TRCVV2 + 2 );

				// Move length in BCD form 
				*( pSendData + 1 ) = BinBcdB( ( UBYTE ) ( len + 3 ) );

				*( pSendData + 2 ) = '1';
				*( pSendData + 3 ) = '6';

				// Move CVV2IND data into SENDBUF 
				*( pSendData + 4 ) = '1';
				*( pSendData + 5 ) = '0';
                                *( pSendData + 6 ) = ' ';		
				// Move CVV2 data into SENDBUF 				
                                memcpy( pSendData + 7, TRREQ.TRCVV2, S_TRCVV2 );
				// Update the data pointer 
				pSendData += ( len + 5 );

				B63_Len += ( len + 5 );

				retval = True;
			}
		}
	}

	return retval;
}



//-----------------------------------------------------------------------------
//  B63_PS2000      Format PS 2000 field of bit63
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool    True  - Data moved
//                          False - Data not moved
//
//  Notes:          Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------

extern Bool B63_PS2000( void )
{
return False;
}


// Batch Upload 
extern Bool B63_BatchUpload( void )
{
	Bool retval;
	UWORD len;

	// Initialize move status to false 
	retval = False;

	// See if UPLOAD transaction 
	if ( RUPLOAD == TRREQ.TRKEY )
	{
		{
			*pSendData = 0;
			len = 26;				
			// Move length in BCD form 
			*( pSendData + 1 ) = BinBcdB( ( UBYTE ) ( len ) );

			*( pSendData + 2 ) = '2';
			*( pSendData + 3 ) = '0';

			memset( pSendData + 4, '0', 24);

			// Update the data pointer 
			pSendData += ( 24 + 4 );

			B63_Len += ( 24 + 4 );

			// *pSendData = 0;
			// len = 12;
			// *( pSendData + 1 ) = BinBcdB( ( UBYTE ) ( len + 2 ) );	// length in BCD form 

			*( pSendData + 0 ) = '2';
			*( pSendData + 1 ) = '1';

			BfAscii( ( char * ) pSendData + 2, TRREQ.TRTOTAM, S_TRTOTAM );
			pSendData += ( 2 + ( S_TRTOTAM * 2 ) );

			B63_Len += ( 2 + ( S_TRTOTAM * 2 ) );

			retval = True;
		}
	}

	return retval;
}

//-----------------------------------------------------------------------------
//  B63_Hostref     Format Host Reference field of bit63
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//
//  Notes:          Destination buffer pointed by pSendData
//
//-----------------------------------------------------------------------------

extern Bool B63_Hostref( void )
{
	Bool retval;
	UWORD len;

	// Initialize move status to false 
	retval = False;

        if ( TRREQ.TRKEY == SALE )
            return retval;

	// See if Host Reference field needs to be moved (Adjust/Advice or
	// an upload transaction detail request) 
	if ( ( TRREQ.TRSTATUS[0] & ( TS1_ADV ) ) ||
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
//  B63_B63_DUKPTHostref        Format the Key Serial Number field of bit63
//
//  Parameters:                 None
//
//  Global Inputs:
//
//  Returns:                    Bool        True  - Data moved
//                                          False - Data not moved
//
//  Notes:                      Destination buffer pointed by pSendData
//
//-----------------------------------------------------------------------------

extern Bool B63_DUKPT( void )
{
return False;
}



//-----------------------------------------------------------------------------
//  B63_Addprmpt        Format additional prompt field of bit 63
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool        True  - Data moved
//                                  False - Data not moved
//
//  Notes:              Destination buffer pointed by pSendData
//
//-----------------------------------------------------------------------------

extern Bool B63_Addprmpt( void )
{
return False;
}



//-----------------------------------------------------------------------------
//  B63_BatNum      Move Current Batch Number
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//
//  Notes:          Destination buffer pointed by pSendData
//
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
//  B63_TaxAmt      Move the tax amount into bit63 area
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool    True  - Data moved
//                          False - Data not moved
//
//  Notes:          Destination buffer pointed by pSendData
//
//-----------------------------------------------------------------------------

extern Bool B63_TaxAmt( void )
{
return False;
}



//-----------------------------------------------------------------------------
//  B63_CashBK      Move CASHBK amount into Bit63 buffer
//
//  Parameters:     None
//
//  Global Inputs:
//
//. Returns:        Bool        True  - Data moved
//                              False - Data not moved
//
//  Notes:          Destination buffer pointed by pSendData
//
//-----------------------------------------------------------------------------

extern Bool B63_CashBK( void )
{
return False;
}





//-----------------------------------------------------------------------------
//  B63_BarAmt      Move BAR amount into SENDBUF
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//
//  Notes:          Destination buffer pointed by pSendData
//
//-----------------------------------------------------------------------------

extern Bool B63_BarAmt( void )
{
return False;
}



//-----------------------------------------------------------------------------
//  B63_MoveStdTotals       Move standard totals for CLBATCH, CLBATCHA and CLBATCHE
//
//  Parameters:             None
//
//  Global Inputs:
//
//  Returns:                Bool        True  - Data moved
//                                      False - Data not moved
//
//  Notes:                  Destination buffer pointed by pSendData
//
//-----------------------------------------------------------------------------

extern Bool B63_MoveStdTotals( void )
{
	UWORD index;
	char tmp[4];

	// Build the totals in TmpBuf then copy to the send buffer. 
	memset( ( char * ) &TmpBuf, '0',105 );
	index = 0;

	// Capture sales count and capture sales amount. 
	BfAscii( tmp, TERMTOTALS.TOTBASIC.TOTCRCNT, S_TOTCRCNT );
	memcpy( &TmpBuf[index], &tmp[1], 3 );
	index += 3;
	BfAscii( ( char * ) &TmpBuf[index], TERMTOTALS.TOTBASIC.TOTCRAMT,
			 S_TOTCRAMT );
	index += 12;

	// Capture refund count and capture refund amount. 
	BfAscii( tmp,  TERMTOTALS.TOTBASIC.TOTRFCNT, S_TOTRFCNT );
	memcpy( &TmpBuf[index], &tmp[1], 3 );
	index += 3;
	BfAscii( ( char * ) &TmpBuf[index], TERMTOTALS.TOTBASIC.TOTRFAMT,
			 S_TOTRFAMT );
	index += 12;

	// Move length in BCD form to the send buffer.  
	*pSendData = 0x00;
        *(pSendData + 1) = 0x30;
		
	// Move totals from the temp buffer to the send buffer. 
	//memcpy( pSendData + 2, TmpBuf, 105 );
        memcpy( pSendData + 2, TmpBuf, 30);
	// Increment the pointer to the data.        
        pSendData += ( 30 + 2);        

	return ( True );
}



//-----------------------------------------------------------------------------
//  B63_MoveTotals      Move totals for CLBATCH, CLBATCHA and CLBATCHE
//
//  Parameters:         None
//
//  Global Inputs:
//
//  Returns:            Bool            True  - Data moved
//                                      False - Data not moved
//
//  Notes:              Destination buffer pointed by pSendData
//
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
//  FindMoveEntry   Find Move Table Entry
//
//  Parameters:     UBYTE bitno         bit number
//
//  Global Inputs:
//
//  Returns:        struct move_rec *   Pointer to entry for bit number
//
//  Notes:
//
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
//  MoveData        Move Data to the Send Buffer
//
//  Parameters:     UBYTE BitNum        bit number
//  
//  Global Inputs:
//
//  Returns:        BYTE        -1 - bad
//                               0 - data not moved
//                               1 - data moved
//  Notes:
//
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
//  MovICC          Move the ICC data (bit 55) into SENDBUF
//
//  Parameters:     None
//  
//  Global Inputs:
//
//  Returns:        Bool    Always returns True
//
//  Notes:          None
//
//-----------------------------------------------------------------------------

extern Bool MovICC( void )
{

    static UBYTE Bit55Data[280];	// construction area
    UBYTE Offset;
    UBYTE index;

    index = 0;

    // reversal ise field 55 gitmez 
    if ( TRREQ.TRSTATUS[0] & TS1_REV ) 
      return ( False );
    
    // TC Advice mesajýnda field 55 gitmez 
    if ( TRREQ.TROPTEMV & TROPTEMV_INFADV ) 
      return False;

    
    memset( Bit55Data, 0x00, 280 );

        // 5F2A - txn currency code
	Bit55Data[index++] = 0x5F;
	Bit55Data[index++] = 0x2A;
	Bit55Data[index++] = S_TCCURRCODE;
	memcpy( &Bit55Data[index], TCONF.TCCURRCODE, S_TCCURRCODE );
        index += S_TCCURRCODE;
                
        Bit55Data[index++] = 0x5F;
        Bit55Data[index++] = 0x34;
        Bit55Data[index++] = 0x01;
        Bit55Data[index++] = TRINP.TRPANSEQ;
      

	// 82 - AIP
	Bit55Data[index++] = 0x82;
	Bit55Data[index++] = S_TRAIP;
	memcpy( &Bit55Data[index], TRREQ.TRAIP, S_TRAIP );
        index += S_TRAIP;  
        Offset = index;

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
	memcpy( &Bit55Data[Offset], TRREQ.TRYEAR, S_TRYEAR + S_TRDATE );
	Offset += ( S_TRYEAR + S_TRDATE );

	// 9C - txn type	
	Bit55Data[Offset++] = 0x9C;
	Bit55Data[Offset++] = 1;        
	Bit55Data[Offset++] = 0x00;

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
	Bit55Data[Offset++] = S_TRSTAN;
        memcpy( &Bit55Data[Offset],TRREQ.TRSTAN, S_TRSTAN );
	Offset += (S_TRINV/2);

	Bit55Data[Offset++] = 0x9F;
	Bit55Data[Offset++] = 0x53;
        Bit55Data[Offset++] = 0x01;
        Bit55Data[Offset++] = 0x52;

#ifdef MAKE_ICCDEBUG
    if ( !( TRREQ.TROPTEMV & ( TROPTEMV_INFADV )) )
    {
      MoveIt(Field55_1, Bit55Data, 34);
      MoveIt(Field55_2, &Bit55Data[34], 34);
      MoveIt(Field55_3, &Bit55Data[68], 34);
      MoveIt(Field55_4, &Bit55Data[102], 34);
    }
#endif 

	MovGen( Bit55Data, Offset );

	// Data moved 
	return ( True );
}


//-----------------------------------------------------------------------------
//  B63_IssScriptRes Format 'Issuer Script Results' field of bit63
//
//  Parameters:     None
//
//  Global Inputs:
//
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//
//  Notes:          Destination buffer pointed by pSendData
//
//-----------------------------------------------------------------------------

extern Bool MovIsScriptRes( void )
{
    UBYTE *pSrc;
    UBYTE safety_cnt;
    UWORD offset;
    UWORD poffSet;


    if (!(TRREQ.TROPTEMV & TROPTEMV_INFADV) ) 
      return False;

    if ( VOIDTRAN == TRREQ.TRKEY ) 
      if ( EMV_SALE != TRREQ.TRORIGKEY && EMV_INSTALLMENT != TRREQ.TRORIGKEY)
          return False;

    offset = 0;
    // //ClrIt( TmpBuf, 21 );
    memset( ( UBYTE * ) & TmpBuf, 0, 21 );    

    safety_cnt = 4;
    pSrc = TRREQ.TRISSSCRIPTRES;
    
    memcpy ( TRREQ.TRISSSCRIPTRES, TRINP.TRISSSCRIPTRES, S_TRISSSCRIPTRES);
    // Only send the subfield if it's not empty, valid data
	//   in TRREQ.TRISSSCRIPTRES must start with 0xFF 
    if( 0xFF == TRREQ.TRISSSCRIPTRES[0] )
    {   
        poffSet = 0;
        while ( (0xFF == TRREQ.TRISSSCRIPTRES[poffSet]) && (safety_cnt--) )
        {
            // Move 5 bytes of data (skipping the presence indicator in the source) 
            memcpy( &TmpBuf[offset], &TRREQ.TRISSSCRIPTRES[poffSet + 1], 5 );
            offset += 5;
			poffSet += 6;
        }
    }

   MovGen ( TmpBuf, 21 ) ;
    
   return ( True ) ;
}

extern Bool MovIssuerAppData( void )
{
    UBYTE offset;
    
     if ( (( TRINP.TRKEY == EMV_SALE || TRINP.TRKEY == EMV_INSTALLMENT )) && !(TRREQ.TROPTEMV & TROPTEMV_INFADV) ) 
      return False;
   
    //ClrIt( TmpBuf, 32 );
    memset( ( UBYTE * ) & TmpBuf, 0, 32 );
    offset = 0;  
    memcpy( &TmpBuf[offset],TRREQ.TRAC, S_TRAC );
    offset += S_TRAC;
    memcpy( &TmpBuf[offset], &TRREQ.TRISAPDAT[1], TRREQ.TRISAPDAT[0] );
    offset += TRREQ.TRISAPDAT[0];
    
    MovGen ( TmpBuf, offset ) ;
    
    return True;
}

// field 18 
extern Bool MovMerchCCode( void )
{
   UBYTE tmp[8];

   if ( VOIDTRAN == TRREQ.TRKEY ) 
     if ( EMV_SALE != TRREQ.TRORIGKEY && EMV_INSTALLMENT != TRREQ.TRORIGKEY)
          return False;

    tmp[0] = 0x00;
    tmp[1] = 0x00;
    memcpy(TRREQ.TRMERCHCATCODE, tmp, 2);

    // Call the Generic Move routine 
    MovGen( TRREQ.TRMERCHCATCODE, prs8583->IS8583_LEN );
	
    return ( True );
}

// field 20 
extern Bool MovCountryCode( void )
{
        if ( VOIDTRAN == TRREQ.TRKEY ) 
          if ( EMV_SALE != TRREQ.TRORIGKEY  && EMV_INSTALLMENT != TRREQ.TRORIGKEY)
            return False;

	memcpy(TRREQ.TRCOUNTRYCODE, TCONF.TCCTRYCODE, 2);

	// Call the Generic Move routine 
	MovGen( TRREQ.TRCOUNTRYCODE, prs8583->IS8583_LEN );

	// Data moved; return True
	return ( True );
}

#endif	// MAKE_EMV
//-----------------------------------------------------------------------------
//  MovAcqName      Move the Acceptor Name
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//-----------------------------------------------------------------------------
extern Bool MovAcqName(void)
{          
        UBYTE index = 8;
        struct batch_rec *pBatchRec;

	// Clear TmpBuf
        // spcit();
	memset((char *)TmpBuf, 0x20, 40);
	
        // POS serial number 
	memcpy((char *)TmpBuf, (UBYTE *)&cSerNum[8], 8);				// 0-7
        
        // Number of installment 
        if ( (TRREQ.TRSTATUS[0] & TS1_REV) && (TRREQ.TRKEY == INSTALLMENT ) ) 
        {
          BfAscii((char *)&TmpBuf[index], (UBYTE *)&TRREQ.TRINSTNUM, 1);	// 8-9
          index += 2;	
        }
        else 
        {
          BfAscii((char *)&TmpBuf[index], (UBYTE *)&TRINP.TRINSTNUM, 1);	// 8-9
          index += 2;	
        }

        // filler space 
	index++;
        
        // POS Company name version number 
	memcpy(&TmpBuf[index], "HC.", 3);
	index += 3;
	memcpy(&TmpBuf[index], &VERSION[0], 5);								// 11-17
	index += 5;	
	TmpBuf[index++] = '.';											// 18
	memcpy(&TmpBuf[index], &VERSION[7], 3);							// 19-21
	index += 3;													// 22
        
        // batch number 
	index++;
        pBatchRec = FindBatTab( TRREQ.TRAQID );	
	BfAscii((char *)&TmpBuf[index], pBatchRec->BATCUR, 3);				// 24-29       
	index += 6;
        
        // filler space 
        index++;	
                
        // Source code --Elle giriþ yapýldýysa M diðerleri ise D gider 
	if ( TRREQ.TRPOSE[1] == 0x12 )								// 31-33
              TRREQ.TRSRC[0] = 'M';		// Manual 
	else           
              TRREQ.TRSRC[0] = 'D';		// Magnetic stripe
        
        // Authorization Source Code 	
        if ( TRREQ.TRPOSE[1] & BIT_0_SET )								// 31-33
            TRREQ.TRSRC[1] = 'X';          // pin based
        else 
            TRREQ.TRSRC[1] = 'Þ';          // signature based

        // Identification Code 
        TRREQ.TRSRC[2] = '1';		// Online transaction
        // TmpBuf[index] = '3';		// Offline transaction
      
        memcpy(&TmpBuf[index], TRREQ.TRSRC, 3);
        index += 3;
        
        index +=3;

	// Call the Generic Move routine
	MovGen((UBYTE *)TmpBuf, 40);
	
	// Data moved 
	return True;
      
}

//-----------------------------------------------------------------------------
//  MovTrk1         Move the Track 1 Data
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//-----------------------------------------------------------------------------

extern Bool MovTrk1(void)
{
	UBYTE Length;
	UBYTE Index;
	UBYTE Value;

	// Check if field contains data 
	if (IfThere( TRREQ.TRTRK1, prs8583->IS8583_LEN)) {
		// Initialize Track 1 data length 
		Length = 0;

		// Initialize index into temporary buffer 
		Index = 0;

		// Calculate Track 1 Data length 
		while (True) {
			// Get byte value 
			Value = TRREQ.TRTRK1[Length];

			// See if Terminator found 
			if (Value == 0xff)
				// Yes; break out of the loop 
				break;

			// Save right nibble, padded on the right 
			TmpBuf[Index] = (Value << 4) | 0x0f;

			// Bump counter 
			Length++;

			// Check if done 
			if (Length == S_TRTRK1)
				// Yes; break out of the loop 
				break;

			// Get byte value 
			Value = TRREQ.TRTRK1[Length];

			// See if Terminator found 
			if (Value == 0xff)
				// Yes; break out of the loop 
				break;

			// Save right nibble, masked with other nibble 
			TmpBuf[Index] &= Value | 0xf0;

			// Increment index into temporary buffer 
			Index++;

			// Increment counter 
			Length++;

			// Check if done 
			if (Length == S_TRTRK1)
				// Yes; break out of the loop 
				break;
		}

		// Call the Generic Move routine 
		MovGen(TmpBuf, Length);

		// Data moved; return True 
		return True;
	}
	else
		// No; data not moved, return False 
		return False;
}

//-----------------------------------------------------------------------------
//  MovCVV2         Move the CVV2 Data
//  Returns:        Bool        True  - Data moved
//                              False - Data not moved
//-----------------------------------------------------------------------------
#define S_CVV2_CONST sizeof(CVV2_CONST)

extern Bool MovCVV2( void )
{
	UBYTE CVV2_CONST[] = {'0', '0', '0', '0', '0', '5', '9', '2'};

	if  (TRREQ.TRPOSE[1] & BIT_4_SET) { //&& (TCONF.TCOPT4 & TC4_GETCVV2)) //@GOKER 
		// Move constant
		memcpy((char *) TmpBuf, CVV2_CONST, S_CVV2_CONST);

		// Move entered CVV2
		MoveOrSpace((char *)&TmpBuf[S_CVV2_CONST], (char *)TRREQ.TRCVV2, S_TRCVV2);

		// Call the Generic Move routine
		MovGen(TmpBuf, S_CVV2_CONST + S_TRCVV2);
		return True;
	}
	else
		return False;
}

extern Bool B63_InstalRefund( void )
{        
	UWORD len;
        
        if ( TRREQ.TRKEY != INSTAL_REFUND )
            return False;
	
        len = 0x18; 
        *pSendData = 0x00;
        *( pSendData + 1 ) = 0x16;
        *( pSendData + 2 ) = '9';
        *( pSendData + 3 ) = '4';      
              
        memcpy( pSendData + 4, RefundDate, 8 );
        memcpy( pSendData + 12, TRREQ.TRAUTH, S_TRAUTH );
          
        pSendData += 18;
        B63_Len += 18;              
   
	return True;
}

//-----------------------------------------------------------------------------
//  B63_MoveTableId90       Move Loyalty Amount
//  Returns:               UBYTE       Kac bayt kopyalandigini dondurur
//
//
//  Notes:                  Destination buffer pointed by pSendData
//-----------------------------------------------------------------------------
//{@erd 28.03.07
extern Bool B63_MoveTableId90 ( void )
{
	UBYTE index; // Table 90 indeksi (ve uzunluðu)
        
        if ( !(TRREQ.TRLOYALTYSTATUS & TS_POINT) || (TRREQ.TRSTATUS[0] & TS1_REV) )   
            return False;

        index = 0;
	memset(( char * ) TmpBuf, 0x20, 16);
	

	TmpBuf[index++] = 0x00;		// Table Length 14
	TmpBuf[index++] = 0x14;

	TmpBuf[index++] = '9';		// table id 90 harcanan puan 
	TmpBuf[index++] = '0';

	BfAscii( ( char * ) &TmpBuf[index] , TRREQ.TRUSEDLOYALTYAMOUNT, S_TRTOTAM );
	index += 12;

        memcpy(pSendData, TmpBuf, index);
        pSendData += index;
        B63_Len += index ;

	return True;
}

//-----------------------------------------------------------------------------
//  B63_MoveTableId95       Move Discount Data
//  Returns:                UBYTE       Kac bayt kopyalandigini dondurur
//                                      
//
//  Notes:                  Destination buffer pointed by pSendData
//  @erd 06.04
//-----------------------------------------------------------------------------
extern Bool B63_MoveTableId95 ( void )
{
        UBYTE index; // Table 95 indeksi (ve uzunluðu)        

        if ( !(TRREQ.TRLOYALTYSTATUS & TS_DISCOUNT) || (TRREQ.TRSTATUS[0] & TS1_REV) )   
            return False;
        
        index = 0;
        memset(( char * ) TmpBuf, 0x30, 26);              
        TmpBuf[index++] = 0x00;		// Table Length 26
        TmpBuf[index++] = 0x26;
      
        TmpBuf[index++] = '9';          // Table Id 95
        TmpBuf[index++] = '5';


        // Discount Amount kopyala
        BfAscii( ( char * ) &TmpBuf[index] , TRREQ.TRDISCOUNTTOTAM , S_TRTOTAM );
        index += 12;
        
        // Discount Rate kopyala
        // Not : Specte Discount Rate 12 haneli ,
        // biz sadece 4 bayt kullanýyoruz Ilk 8 baytý 0 la doldur             
        BfAscii( ( char * ) &TmpBuf[index + 8] , TRREQ.TRDISCOUNTRATIO , S_TRDISCRATIO );
        index += 12;
               
        memcpy(pSendData, TmpBuf, index);
        pSendData += index;
        B63_Len += index ;

        return True;
}
