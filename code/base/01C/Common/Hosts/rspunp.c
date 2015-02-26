//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             rspunp.c
//      Unpack Response.
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "utllib.h"

#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "tables.h"
#include "is8583.h"
#include "rspunp.h"
#include "bcdbinw.h"

#ifdef MAKE_EMV
#include "emv_l2.h"
#endif

//=============================================================================
// External variables / declarations
//=============================================================================

// Public from other modules

extern UBYTE *pRecvData;

//=============================================================================
// Private defines and typedefs
//=============================================================================

struct save_rec
{
    //! Bit number to save for
	UBYTE savbitno;
    //! Where to save data
	UBYTE *savdest;
    //! Save routine
	void ( *sav_it ) ( void );
};


//=============================================================================
// Private function declarations
//=============================================================================

static Bool SavData( UBYTE BitNum );
static struct save_rec *FindSaveEntry( UBYTE bitno );
static void GenSav( void );
static void SavPan( void );
static void SavTIP( void );
static void SavDate( void );
static void SavRSPA( void );
static void SavKey( void );
static void SavB60( void );
static void SavB61( void );
static void SavB62( void );
static void SavB63( void );
#ifdef MAKE_ISOMAC
static void SavB64( void );
#endif // MAKE_ISOMAC
static Bool BumpPtr( void );
#ifdef MAKE_EMV
static void SavICC( void );
static void Field55Parse( UBYTE * fldptr );
#endif	// MAKE_EMV


//=============================================================================
// Public data definitions
//=============================================================================

const struct is8583_rec *prs8583;

UBYTE *p60Data;
UBYTE *p61Data;
UBYTE *p62Data;
UBYTE *p63Data;
#ifdef MAKE_ISOMAC
UBYTE *p64Data;
#endif // MAKE_ISOMAC

struct save_rec *psave_rec;


//=============================================================================
// Private data definitions
//=============================================================================

// *INDENT-OFF*

//! Table of Save Entries.
//! Each entry respresents a single save item.
//! All items are saved in TRRSP, and moved to TRREQ if
//! it is determined that the response is for us
static const struct save_rec Save_Tab[] = {
    //! Primary Account Number
	{  2,	 TRRSP.TRPAN,		SavPan	},
    //! Processing Code
	{  3,	 TRRSP.TRPROC,		GenSav	},
    //! Amount, Transaction
	{  4,	 TRRSP.TRTOTAM,		GenSav	},
    //! Systems Trace Audit Number
	{ 11,	 TRRSP.TRSTAN,		GenSav	},
    //! Time, Transaction
	{ 12,	 TRRSP.TRTIME,		GenSav	},
    //! Date, Transaction
	{ 13,	 TRRSP.TRDATE,		SavDate	},
    //! Date, Expiry
	{ 14,	 TRRSP.TREXPD,		GenSav	},
    //! POS Entry Mode
	{ 22,	 TRRSP.TRPOSE,		GenSav	},
    //! Network International Identifier
	{ 24,	 TRRSP.TRNII,		GenSav	},
    //! POS Condition Code
	{ 25,	&TRRSP.TRPOCC,		GenSav	},
    //! Retrieval Reference Number
	{ 37,	 TRRSP.TRRRN,		GenSav	},
    //! Auth Id Response code
	{ 38,	 TRRSP.TRAUTH,		GenSav	},
    //! Response Code
	{ 39,	 TRRSP.TRRSPC,		GenSav	},
    //! Terminal Id
	{ 41,	 TRRSP.TRTID,		GenSav	},
    //! Acceptor Id
	{ 42,	 TRRSP.TRACCID,		GenSav	},
    //! Additional Response Data
	{ 44,	 TRRSP.TRRSPA,		SavRSPA	},
    //! PIN and MAC Keys
	{ 48,	 0,					SavKey	},
    //! Tip Amount
	{ 54,	 TRRSP.TRTIP,		SavTIP	},
#ifdef MAKE_EMV
    //! ICC Data
	{ 55,	 0,					SavICC  },
#endif
    //! Private Field 60
	{ 60,	( UBYTE * ) &TRRSP,	SavB60	},
    //! Private Field 61
	{ 61,	( UBYTE * ) &TRRSP,	SavB61	},
    //! Private Field 62
	{ 62,	( UBYTE * ) &TRRSP,	SavB62	},
    //! Private Field 63
	{ 63,	( UBYTE * ) &TRRSP,	SavB63	},
#ifdef MAKE_ISOMAC
    //! Private Field 64, MAC
	{ 64,	( UBYTE * ) &TRRSP,	SavB64	},
#endif // MAKE_ISOMAC
    //! Table End
	{  0,	( UBYTE * ) &TRRSP,	GenSav	}
};

// *INDENT-ON*

//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Unpacks the Message Response into TRRSP
//!
//! \return
//!     True on success else False
//-----------------------------------------------------------------------------
extern Bool RspUnPackMsg( void )
{
	UBYTE *pBitMap;
	UBYTE BitNum;
	UBYTE BitMask;
	UBYTE Status;

	// Initialize Status to okay
	Status = True;

	// Save pointer to bitmap
	pBitMap = pRecvData;

	// Save the bitmap
	memcpy( TRRSP.TRBMAP, pRecvData, S_TRBMAP );

	// Save pointer to start of data
	pRecvData += S_TRBMAP;

	// Clear pointers to Private Data (Bits 60-63)
	p60Data = NULL;
	p61Data = NULL;
	p62Data = NULL;
	p63Data = NULL;
#ifdef MAKE_ISOMAC
    p64Data = NULL;
#endif // MAKE_ISOMAC

	// Initialize bit mask
	BitMask = 0x80;

	// For bits 1-64...
	for ( BitNum = 1; BitNum <= 64; BitNum++ )
	{
		// Check if its in the bitmap
		if ( *pBitMap & BitMask )
		{
			// Yes; save the data
			Status = SavData( BitNum );

			// Check if save okay
			if ( Status )
			{
				// Yes; update Response Pointer
				Status = BumpPtr(  );

				// Check if update okay
				if ( !Status )
				{
					// No; quit the loop
					break;
				}
			}
			else
			{
				// No; quit the loop
				break;
			}
		}

		// Update bit mask
		BitMask = BitMask >> 1;

		// Check if zero
		if ( BitMask == 0x00 )
		{
			// Yes; re-Initialize the bit mask
			BitMask = 0x80;

			// Update the bitmap pointer
			pBitMap++;
		}
	}

	// Return status
	return ( Status );
}


//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Saves Data from Response Data
//!
//! \param
//!		[in]BitNum			Bit number to be saved
//!
//! \return
//!     True on success else False
//-----------------------------------------------------------------------------
static Bool SavData( UBYTE BitNum )
{
	// Get attributes for bit
	prs8583 = FindBit( BitNum );

	// Check if found
	if ( prs8583 != NULL )
	{
		// Yes; find Save Entry
		psave_rec = FindSaveEntry( BitNum );

		// Check if entry found
		if ( psave_rec != NULL )
		{
			// Yes; call the save routine
			psave_rec->sav_it(  );
		}

		// Return completion status
		return ( True );
	}
	else
	{
		// No; return incomplete status
		return ( False );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Finds Save Table Entry
//!
//! \param
//!		[in]BitNum			Bit number to be saved
//!
//! \return
//!     Pointer to entry for bit number
//-----------------------------------------------------------------------------
static struct save_rec *FindSaveEntry( UBYTE bitno )
{
	struct save_rec *pSaveRec;

	// Start at first record
	pSaveRec = ( struct save_rec * ) Save_Tab;

	// Do till record found or end of table
	while ( ( pSaveRec->savbitno != 0 ) && ( pSaveRec->savbitno != bitno ) )
	{
		// Look at next record
		pSaveRec++;
	}

	// If not found return NULL, else return ptr
	if ( pSaveRec->savbitno == 0 )
		return ( NULL );
	else
		return ( pSaveRec );
}


//-----------------------------------------------------------------------------
//! \brief
//!     General Purpose Save Routine
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void GenSav( void )
{
	UWORD genlen;
	UBYTE *pData;

	// Get the length
	genlen = prs8583->IS8583_LEN;

	// Get Response Pointer
	pData = pRecvData;

	// Switch on the message format
	switch ( prs8583->IS8583_FMT )
	{
		case FFIX + ATTN:		// Fixed Numeric

			// Make length even and divide by 2
			if ( genlen & 0x01 )
				genlen++;

			genlen /= 2;
			break;

		case FFIX + ATTBIN:	// Fixed Binary

			// Divide by 8
			genlen /= 8;
			break;

		case FFIX + ATTAN:		// Fixed Alpha Numeric
		case FFIX + ATTANS:	// Fixed Alpha Numeric Special
			break;

		case FLLVAR + ATTANS:	// LL Variable Alpha Numeric Special

			// Bump pointer past length (1 byte)
			pData++;
			break;

		case FLLLVAR + ATTAN:	// LLL Variable Alpha Numeric
		case FLLLVAR + ATTANS:	// LLL Variable Alpha Numeric Special

			// Bump pointer past length (2 bytes)
			pData += 2;
			break;

		case FLLVAR + ATTN:	// LL Variable Numeric

			// Bump pointer past length (1 byte)
			pData++;

			// Make length even and divide by 2
			if ( genlen & 0x01 )
				genlen++;

			genlen /= 2;
			break;
	}

	// Safety validation on length
	if ( genlen != 0 )
	{
		// Move the Data
		memcpy( psave_rec->savdest, pData, genlen );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Saves Primary Account Number Field
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SavPan( void )
{
	UBYTE *pData;
	UBYTE nlen;
	UBYTE len;

	// Initialize the field
	memset( ( UBYTE * ) TRRSP.TRPAN, 0xFF, sizeof( TRRSP.TRPAN ) );

	// Get Response Pointer
	pData = pRecvData;

	// Get length of field in binary (nibbles)
	nlen = CvtBin( *pData );

	// Step forward the pointer over length
	pData++;

	// Divide the length by 2 to get the number of bytes
	len = nlen / 2;

	// Move the data
	memcpy( TRRSP.TRPAN, pData, len );

	// Step forward the pointer over data
	pData += len;

	// Check if length is odd
	if ( nlen & 0x01 )
	{
		// Yes; move the last nibble
		TRRSP.TRPAN[len] = ( *pData ) | 0x0f;
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Saves the Tip from the Response Message
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SavTIP( void )
{
	UBYTE *pData;
	UWORD len;

	// Get Response Pointer
	pData = pRecvData;

	// Get length of field in binary
	len = BcdBinW( pData );

	// Step forward the pointer over length
	pData += 2;

	// Convert the Ascii Amount to Hex
	AscHex( TRRSP.TRTIP, ( char * ) pData, 6 );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Saves Date from Response
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SavDate( void )
{
	UBYTE CurrentTime[RTCLEN];
	OS_RETURN_VALS retval;

	// Use Generic Save
	GenSav(  );

	// Read the Real Time Clock
	retval = SDK_RtcRead( CurrentTime );

	// Check the status
	if ( retval == orvOK )
	{
		// Okay; convert the Ascii Year to Hex
		AscHex( TRREQ.TRYEAR, ( char * ) CurrentTime, 1 );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Saves Additional Response Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SavRSPA( void )
{
	UBYTE *pData;
	UWORD len;

	// Get Response Pointer
	pData = pRecvData;

	// Get length of field in binary
	len = ( UWORD ) CvtBin( *pData );

	// Step forward the pointer over length
	pData++;

	// Move the Data
	memcpy( psave_rec->savdest, pData, S_TRRSPA );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Saves PIN and MAC Keys
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SavKey( void )
{
	UBYTE *pData;
	UWORD len;

	// Get Response Pointer
	pData = pRecvData;

	// Get length of field in binary
	len = BcdBinW( pData );

	// Does Host Control PIN Key?
	if ( (TRREQ.TRAQPTR->AQOPT2) & AQ2_PINKEYEXCM )
	{
		// Yes; move Working Key
		MoveIt2Aq( TRREQ.TRAQPTR->AQWORKKEY, pData + 10, S_AQWORKKEY );
	}

	// Does Host Control MAC Key?
	if ( (TRREQ.TRAQPTR->AQOPT2) & AQ2_MACKEYEXCM )
	{
		// Yes; move MAC Key
		MoveIt2Aq( TRREQ.TRAQPTR->AQMACKEY, pData + 2, S_AQMACKEY );
	}

	// Update Acquirer Table LRC
	SetAQTabLRC(  );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Saves Field 60 Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SavB60( void )
{
	// Save Response Pointer for later
	p60Data = pRecvData;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Saves Field 61 Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SavB61( void )
{
	// Save Response Pointer for later
	p61Data = pRecvData;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Saves Field 62 Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SavB62( void )
{
	// Save Response Pointer for later
	p62Data = pRecvData;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Saves Field 63 Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SavB63( void )
{
	// Save Response Pointer for later
	p63Data = pRecvData;
}

#ifdef MAKE_ISOMAC
//-----------------------------------------------------------------------------
//! \brief
//!     Saves Field 64 Data
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SavB64( void )
{
	// Save Response Pointer for later
	p64Data = pRecvData;
}
#endif // MAKE_ISOMAC

//-----------------------------------------------------------------------------
//! \brief
//!     Bumps Response Pointer along, according to IS8583
//!
//! \return
//!     False on unknown format else True
//-----------------------------------------------------------------------------
static Bool BumpPtr( void )
{
	UWORD Length;

	// Get default length in case required for bump
	Length = prs8583->IS8583_LEN;

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
			// Get length from message
			Length = ( UWORD ) CvtBin( *pRecvData );

			// Go past length
			pRecvData++;

			break;

		case FLLLVAR + ATTAN:	// LLL Variable Alpha Numeric
		case FLLLVAR + ATTANS:	// LLL Variable Alpha Numeric Special
			// Get length from message
			Length = BcdBinW( pRecvData );

			// Go past length
			pRecvData += 2;
			break;

		case FLLVAR + ATTN:	// LL Variable Numeric
			// Get length from message
			Length = ( UWORD ) CvtBin( *pRecvData );

			// Make length even and divide by 2
			if ( Length & 0x01 )
				Length++;

			Length /= 2;

			// Go past length
			pRecvData++;

			break;

		default:				// Unknown format
			return ( False );
	}

	// Bump Response Pointer by length
	pRecvData += Length;

	// Known format
	return ( True );
}

#ifdef MAKE_EMV
//-----------------------------------------------------------------------------
//! \brief
//!     Saves ICC data from response
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SavICC( void )
{
	Field55Parse( pRecvData );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Parses Field 55 (ICC System Information Data)
//!
//! \param
//!		[in]fldptr			Pointer to start of Field 55
//!
//! \return
//!     None
//!
//! \note
//!     If there's an error in Field 55 structure - the whole field is ignored.
//!     Uses globals IssScriptBuff & ScriptLen.
//-----------------------------------------------------------------------------
static void Field55Parse( UBYTE * fldptr )
{
	static UWORD FldLen55;
	static UBYTE FldLen55Save;
	static UBYTE FldLen55Chk;
	static UBYTE DataLen;
	static UBYTE *pTmp;

	// Setup the pointer to Field 55
	pTmp = fldptr;

	// Check we have Field 55 -  ignore for void, advice, adjust & reversal
	if ( pTmp != NULL &&
		 !( TRREQ.TRSTATUS[0] & ( TS1_REV + TS1_ADV + TS1_ADJ + TS1_VOID ) ) )
	{
		//  Save total length as binary 
		FldLen55 = BcdBin( pTmp );

		if ( FldLen55 > 255 )
		{
			return;				// Total length is beyond the limit
		}

		// Initialise the Script variables since we are starting afresh
		memset( IssScriptBuf, 0,sizeof( IssScriptBuf ) );
		ScriptLen = 0;			// Len of script (global)

		FldLen55Save = ( UBYTE ) FldLen55;
		// Initialize var used to check (total field len = cumulative subfield len)
		FldLen55Chk = 0;

		// Set data pointer 
		pTmp += 2;
		// Set the pointer to destination (see notes to REQROUTINE_ONLINE)

		// Loop until all parsed or error is detected
		// Fetch tags 0x71, 0x72, 0x91 and skip the rest
		while ( ( FldLen55 != 0 ) && ( FldLen55Chk <= FldLen55Save ) )
		{
			if ( ( *pTmp == 0x71 ) || ( *pTmp == 0x72 ) )	// Issuer Script
			{
				DataLen = *( pTmp + 1 );	// set length for this script
				if ( ( DataLen + ScriptLen ) <= 128 )	// Field length is within the limit 
				{
					// Move script into buffer, update pTmp, update ScriptLen
					memcpy( IssScriptBuf + ScriptLen, pTmp, DataLen + 2 );
					ScriptLen += ( DataLen + 2 );
				}
			}
			else if ( *pTmp == 0x91 )	// Issuer Authentication Data
			{
				DataLen = *( pTmp + 1 );
				if ( DataLen <= 16 )	// Field length OK
				{
					// Move to destination & skip over
					EMVL2_WriteTagData( ( pTmp + 2 ), DataLen, TAG_TE,
										TAG_91 );
				}
			}
			else				// Skip other tags 
			{
				while ( ( *pTmp & 0x1F ) == 0x1F )
				{				// Keep skipping the multibyte tag and decrementing total field length
					pTmp++;
					FldLen55--;
					FldLen55Chk++;
				}
				// Get subfield length (pTmp points to last byte of the tag)
				DataLen = *( pTmp + 1 );
			}

			// Decrement total field length by subfield length 
			FldLen55 -= DataLen + 2;
			// Set pointer after subfield
			pTmp += DataLen + 2;
			// Update cumulative length
			FldLen55Chk += DataLen + 2;

		}						// while ( FldLen55 != 0 )
		if ( FldLen55Save != FldLen55Chk )
		{						// Integrity error, ignore the whole field

		}

	}							// endif 55 exists & not void, reversal, adjust
}
#endif

