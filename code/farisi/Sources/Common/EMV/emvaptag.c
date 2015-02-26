#ifdef MAKE_EMV
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     emvaptag.c
//  EMV application processing
//
//=============================================================================
#include "basictyp.h"
#include "osclib.h"
#include "ddtm.h"
#include "utllib.h"
#include "fixdata.h"
#include "emv_l2.h"
#include "emv2_pk.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "util.h"
#include "emvdata.h"
#include "scard.h"
#include "string.h"
#include "emvtabs2.h"
#include "emvutils.h"
#include "tables.h"
#include "tlib.h"
#include "settime.h"

#include "emvaptag.h"


/*-------- local static function prototypes ---------*/
extern UBYTE DOL_TRANTYPE( UBYTE * destptr, UBYTE entry );
static UBYTE DOL_FLOOR( UBYTE * destptr, UBYTE entry );
static UBYTE DOL_ADDCAPABIL( UBYTE * destptr, UBYTE entry );
static UBYTE DOL_RANDOM( UBYTE * destptr, UBYTE entry );
static UBYTE DOL_DEFDDOL( UBYTE * destptr, UBYTE entry );
static UBYTE DOL_DEFTDOL( UBYTE * destptr, UBYTE entry );



//-----------------------------------------------------------------------------
//! \brief
//!     Provides EMV Library with Application independent data
//! 
//! \return
//!     EMVL2_OK on success else EMVL2_FAIL.
//!
//! \note
//!     Writes terminal data to tag store.
//-----------------------------------------------------------------------------
UBYTE WriteTermData( void )
{

    UBYTE TmpBuf[20], retCode;			// Temporary storage

    // derived from: 
//  static const UBYTE TermData[] = { 
//    TO_9F1A,	/* TERMINAL COUNTRY CODE  */
//	  TO_5F2A,	/* TRANSACTION CURRENCY CODE */
//	  TO_9A,	/* TERMINAL DATE */
//	  TO_9C,	/* TRANSACTION TYPE */
//	  TO_9F21,	/* TERMINAL TIME */
//	  TO_9F33,	/* TERMINAL CAPABILITIES */
//	  TO_9F35,	/* TERMINAL TYPE */
//	  TO_9F37   /* ADDITIONAL TERMINAL CAPABILITIES */
//	  TO_9F40,	/* UNPREDICTABLE NUMBER */
//};								

    // start optimistically
    retCode = True;

	//{TAG_5F2A, NULL, TCONF.TCCURRCODE, 2},  // 0  TRANSACTION CURRENCY CODE
	retCode &= EMVL2_WriteTagData( TCONF.TCCURRCODE, S_TCCURRCODE, TAG_TE, TAG_5F2A );

	//{TAG_5F36, DOL_CURREXP, NULL, 1},       // 1  TRANSACTION CURRENCY EXPONENT
    //DOL_CURREXP (TmpBuf, 0);
	//retCode &= EMVL2_WriteTagData( TmpBuf, 1, TAG_TE, TAG_5F36 );

	//{TAG_9A,   DOL_DATE, NULL, 3},          // 2  TERMINAL DATE
    SetTranTime( TmpBuf );
	retCode &= EMVL2_WriteTagData( TmpBuf, 3, TAG_TE, TAG_9A );

	//{TAG_9C,   DOL_TRANTYPE, NULL, 1},      // 3  TRANSACTION TYPE
    DOL_TRANTYPE (TmpBuf, 0);
	retCode &= EMVL2_WriteTagData( TmpBuf, 1, TAG_TE, TAG_9C );

    //{TAG_9F1A, NULL, TCONF.TCCTRYCODE, 2},  // 5  TERMINAL COUNTRY CODE
	retCode &= EMVL2_WriteTagData( TCONF.TCCTRYCODE, S_TCCTRYCODE, TAG_TE, TAG_9F1A );

	//{TAG_9F21, DOL_TIME, NULL, 3},	        // 7  TERMINAL TIME
	retCode &= EMVL2_WriteTagData( TRINP.TRTIME, S_TRTIME, TAG_TE, TAG_9F21 );

	//{TAG_9F33, DOL_CAPABIL, NULL, 3},       // 8  TERMINAL CAPABILITIES
	retCode &= EMVL2_WriteTagData( TCONF.TCTCAP, S_TCTCAP, TAG_TE, TAG_9F33 );

	//{TAG_9F35, DOL_TERMTYPE, NULL, 1},      // 9  TERMINAL TYPE
	AscHex( TmpBuf, (char*)TCONF.TCEMVTYPE, 1 );
	retCode &= EMVL2_WriteTagData( TmpBuf, 1, TAG_TE, TAG_9F35 );

	//{TAG_9F37, DOL_RANDOM, NULL, 4},        // 10 UNPREDICTABLE NUMBER
    DOL_RANDOM (TmpBuf, 0);
	retCode &= EMVL2_WriteTagData( TmpBuf, 4, TAG_TE, TAG_9F37 );

	//{TAG_9F40, DOL_ADDCAPABIL, NULL, 5},    // 14 ADDITIONAL TERMINAL CAPABILITIES
    DOL_ADDCAPABIL (TmpBuf, 0);
	retCode &= EMVL2_WriteTagData( TmpBuf, 5, TAG_TE, TAG_9F40 );

	// Set the Application capabilities
	retCode &= EMVL2_WriteTagData( LocalAppCap , 3, TAG_TE, TAG_DF56);

	return retCode;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Provides EMV Library with Scheme data
//! 
//! \return
//!     None.
//!
//! \note
//!     Writes acquirer TERMID and MERCHID to tagstore. Must be called after
//!     CheckCardRange().
//-----------------------------------------------------------------------------
void WriteSchemeData( void )
{
	/* Move Terminal ID to transaction record */
	MoveItAq( TRINP.TRTID, TRINP.TRAQPTR->AQTID, S_TRTID ); 

	/* Move the Merchant Id from AQTAB to transaction record */
	MoveItAq( TRINP.TRACCID, TRINP.TRAQPTR->AQACCID, S_TRACCID );
    
	EMVL2_WriteTagData( TRINP.TRACCID, S_TRACCID, TAG_TE, TAG_9F16 );

	EMVL2_WriteTagData( TRINP.TRTID, S_TRTID, TAG_TE, TAG_9F1C );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Provides EMV Library with AID related
//! 
//! \param[in/out] TAIDTABIndex      Index of AID
//! 
//! \return
//!     0 on success otherwise Error code.
//-----------------------------------------------------------------------------
UBYTE WriteAIDData( UBYTE TAIDTABIndex )
{

	UBYTE TmpBuf[20];			// Temporary storage
	UBYTE Len;
	UBYTE retCode;

	retCode = EMVL2_OK;

    // validate parameters
	// retrieve DDOL for AID and write to the tag store
	Len = DOL_DEFDDOL( TmpBuf, TAIDTABIndex );	// 
	retCode &= EMVL2_WriteTagData( TmpBuf, Len, TAG_TE, TAG_DF50 );

	// retrieve TDOL for AID and write to the tag store
	Len = DOL_DEFTDOL( TmpBuf, TAIDTABIndex );
	retCode &= EMVL2_WriteTagData( TmpBuf, Len, TAG_TE, TAG_DF51 );

	return retCode;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Provides EMV Library with Amount details
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void WriteAmountData( void )
{

	UBYTE TmpBuf[6];			// Temporary storage

	// Amount
	EMVL2_WriteTagData( TRINP.TRTOTAM, 6, TAG_TE, TAG_9F02 );

	// Convert amount from BCD to Binary - note field only 4 bytes long
	Bcd2Bin( TmpBuf, TRINP.TRTOTAM, S_TRBASE );
	EMVL2_WriteTagData( &TmpBuf[2], 4, TAG_TE, TAG_81 );

	// #664 - cashback problem - Tag 9F03 must only be populated
 // during cashback transactions. Previously we were writing 
 // zero values to this tag during non cashback transactions
 // which causes the EMV library to behave badly.
	if ( TRINP.TRKEY == EMV_SALCASH)
	{
		// Amount, Other 
		EMVL2_WriteTagData( TRINP.TRCASHBK, 6, TAG_TE, TAG_9F03 );

		// Amount, Other - Binary  - note field only 4 bytes long 
		Bcd2Bin( TmpBuf, TRINP.TRCASHBK, S_TRBASE );
		EMVL2_WriteTagData( &TmpBuf[2], 4, TAG_TE, TAG_9F04 );
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Provides EMV Library with Floor Limit and Invoice Number
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void WriteFloorInvData( void )
{

	UBYTE TmpBuf[6];			// Temporary storage

    // floor limit
	DOL_FLOOR( TmpBuf, 0 );
	EMVL2_WriteTagData( TmpBuf, 4, TAG_TE, TAG_9F1B );

	// Invoice number
	AscHex( TmpBuf, (char *) TRINP.TRINV, (S_TRINV/2) );
	EMVL2_WriteTagData( TmpBuf, (S_TRINV/2), TAG_TE, TAG_9F41 );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Updates TRINP structure with ICC data
//! 
//! \return
//!     EMVL2_OK on success else error code.
//! 
//! \note
//!     Called after Application Selection.
//-----------------------------------------------------------------------------
UBYTE UpdFieldsPostAppSel( void )
{
	UBYTE Trk2Offset;
	UBYTE TempBuffer[S_TRTRK2];	// 38 bytes (19 * 2)
	UBYTE TagReturnLength;
	UBYTE retCode;

	retCode = EMVL2_OK; // set global return value

	// Fill PAN with FF's 
	memset( TRINP.TRPAN, 0xFF, S_TRPAN );
	// Fill AID with FF's 
	memset( TRINP.TRAID, 0xFF, S_TRAID );

	// Need to decide what consequences are of not finding one of the fields
	// Organize terminal first with returns, and optional subsequently ?

	// PAN - mandatory field
	if ( !EMVL2_ReadTagData( TRINP.TRPAN, &TagReturnLength, TAG_5A ) )
		retCode = EMVL2_FAIL;

	// Cardholder name - optional
	EMVL2_ReadTagData( TRINP.TRNAME, &TagReturnLength, TAG_5F20 );

	// Service Code - optional
	EMVL2_ReadTagData( TRINP.TRSERVCD, &TagReturnLength, TAG_5F30 );

	// PAN Seq No
	EMVL2_ReadTagData( &TRINP.TRPANSEQ, &TagReturnLength, TAG_5F34 );

	// App Expiry Date - mandatory
	if ( !EMVL2_ReadTagData( TRINP.TRAPPEXP, &TagReturnLength, TAG_5F24 ) )
		retCode = EMVL2_FAIL;

	// Move it into TREXPD
	memcpy( TRINP.TREXPD, TRINP.TRAPPEXP, S_TREXPD );

	// App Usage Ctrl
	EMVL2_ReadTagData( TRINP.TRAUC, &TagReturnLength, TAG_9F07 );

	// AIP
	EMVL2_ReadTagData( TRINP.TRAIP, &TagReturnLength, TAG_82 );

	// AID (terminal)
	// populate for appropriate length - use temporary buffer
	if ( !EMVL2_ReadTagData( TempBuffer, &TagReturnLength, TAG_84 ) )
		retCode = EMVL2_FAIL;
	else
	{
		memcpy( &TRINP.TRAID[1], TempBuffer, TagReturnLength );
		TRINP.TRAID[0] = TagReturnLength;
	}

	// App Version Number - mandatory
	EMVL2_ReadTagData( TRINP.TRAPPVER, &TagReturnLength, TAG_9F08 );

	// Iss action codes - optional
	EMVL2_ReadTagData( TRINP.TRISAC, &TagReturnLength, TAG_9F0E );
	EMVL2_ReadTagData( TRINP.TRISAC + 5, &TagReturnLength, TAG_9F0D );
	EMVL2_ReadTagData( TRINP.TRISAC + 10, &TagReturnLength, TAG_9F0F );

	// Unpredictable number
	if ( !EMVL2_ReadTagData( TRINP.TRRAND, &TagReturnLength, TAG_9F37 ) )
		retCode = EMVL2_FAIL;

	// now process Track 2 - needs to be converted to ASCII
    // Fill Track2 with 0xFF first (not with 0x00)
	memset( TRINP.TRTRK2, 0xFF, S_TRTRK2 );
	memset( TempBuffer, 0, S_TRTRK2 );

	// Get Track 2 from TAG store - optional
	EMVL2_ReadTagData( TempBuffer, &TagReturnLength, TAG_57 );
	{
		for ( Trk2Offset = 0; Trk2Offset < TagReturnLength; Trk2Offset++ )
		{
			// convert top nibble
			TRINP.TRTRK2[Trk2Offset * 2] = ( TempBuffer[Trk2Offset] >> 4 ) + 0x30;
			// convert bottom nibble if not finished
			if ( ( TempBuffer[Trk2Offset] & 0x0F ) != 0x0F )
			{
				TRINP.TRTRK2[( Trk2Offset * 2 ) + 1] = ( TempBuffer[Trk2Offset] & 0x0F ) + 0x30;
			}
			// else leave it as 0xFF
		}
	}

	// send return status
	return retCode;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Updates TRINP structure with ICC data after GenAC
//! 
//! \return
//!     EMVL2_OK on success else error code.
//! 
//! \note
//!     Called after Application Selection.
//-----------------------------------------------------------------------------
UBYTE UpdFieldsPostGenAC( void )
{

	UBYTE TagReturnLength;
	UBYTE retCode;

    retCode = EMVL2_OK;

	// Need to decide what consequences are of not finding one of the fields
	// Organize terminal first with returns, and optional subsequently ?

	// TSI
	EMVL2_ReadTagData( TRINP.TRTSI, &TagReturnLength, TAG_9B );

	// Cryptogram Information Data
	EMVL2_ReadTagData( &TRINP.TRCRYPTINFO, &TagReturnLength, TAG_9F27 );

	// Application Transaction Counter
	EMVL2_ReadTagData( TRINP.TRATC, &TagReturnLength, TAG_9F36 );

	// Application Cryptogram
	EMVL2_ReadTagData( TRINP.TRAC, &TagReturnLength, TAG_9F26 );

	// Issuer Application Data
	EMVL2_ReadTagData( &TRINP.TRISAPDAT[1], &TRINP.TRISAPDAT[0], TAG_9F10 );

	// Terminal Verification Results
	EMVL2_ReadTagData( TRINP.TRTVR, &TagReturnLength, TAG_95 );

	// AIP
	EMVL2_ReadTagData( TRINP.TRAIP, &TagReturnLength, TAG_82 );

	// CVM Results
	if ( !EMVL2_ReadTagData
		 ( TRINP.TRCVMRESULTS, &TagReturnLength, TAG_9F34 ) )
		retCode = EMVL2_FAIL;

	// Terminal Application Version Number
	if ( !EMVL2_ReadTagData( TRINP.TRTAIDVER, &TagReturnLength, TAG_9F09 ) )
		retCode = EMVL2_FAIL;

	return retCode;

}

//-----------------------------------------------------------------------------
//! \brief
//!     Updates TRINP structure with ICC data after GenAC
//!
//! Updates TRINP structure with ICC data after GenAC for storing in the
//! journal and receipt printing
//! 
//! \return
//!     EMVL2_OK on success else error code.
//! 
//! \note
//!     Called after Application Selection.
//-----------------------------------------------------------------------------
void UpdFinalCryptogram( void )
{
	UBYTE TagReturnLength;

    // Application Cryptogram
	EMVL2_ReadTagData( TRINP.TRAC, &TagReturnLength, TAG_9F26 );

	// Cryptogram Information Data
	EMVL2_ReadTagData( &TRINP.TRCRYPTINFO, &TagReturnLength, TAG_9F27 );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Updates TRINP structure with Response Code from the EMV Tag Store
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void UpdFinalResponseCode( void )
{
	UBYTE TagReturnLength;

	// Response Code
	EMVL2_ReadTagData( TRINP.TRRSPC, &TagReturnLength, TAG_8A );

    return;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets terminal floor limit
//! 
//! \param[out] destptr         Pointer to the floor limit buffer
//! \param[in] entry            Not used
//! 
//! \return
//!     True.
//-----------------------------------------------------------------------------
UBYTE DOL_FLOOR( UBYTE * destptr, UBYTE entry )
{
	UBYTE FloorAmt[S_TRBASE] = { 0 };	/* Temp storage for Floor limit */

	if ( NULL != TRINP.TRISPTR )
	{
		/* Prepare Amount like TRTOTAM (use Issuer floor limit) */
///		memcpy( &FloorAmt[S_TRBASE-S_ISFLOOR-1], TRINP.TRISPTR->ISFLOOR, S_ISFLOOR );
		MoveItIss( &FloorAmt[S_TRBASE-S_ISFLOOR-1], TRINP.TRISPTR->ISFLOOR, S_ISFLOOR );

		/* Convert it from BCD to Binary */
		Bcd2Bin( destptr, &FloorAmt[2], S_TRBASE - 2 );	// We need 4 bytes only
	}
	else
	{
		memset( destptr, 0, 4 );
	}

	return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Gets Additional terminal capabilities
//! 
//! \param[out] destptr         Pointer to the Additional terminal capabilities buffer
//! \param[in] entry            Not used
//! 
//! \return
//!     True.
//-----------------------------------------------------------------------------
UBYTE DOL_ADDCAPABIL( UBYTE * destptr, UBYTE entry )
{
	memcpy( destptr, TCONF.TCADDTCAP, 5 );
	return True;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Generates a random number
//! 
//! \param[out] destptr         Pointer to the random number buffer
//! \param[in] entry            Not used
//! 
//! \return
//!     True.
//-----------------------------------------------------------------------------
UBYTE DOL_RANDOM( UBYTE * destptr, UBYTE entry )
{
	int i;
	/* Get 4 random BCD numbers */
	for ( i = 0; i < 4; i++ )
	{
		/* Get a random number (UBYTE) and convert to BCD */
		*destptr++ = GetRandom(  );
	}
	return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Gets transaction type
//! 
//! \param[out] destptr         Pointer to the transaction type buffer
//! \param[in] entry            Not used
//! 
//! \return
//!     True.
//-----------------------------------------------------------------------------
UBYTE DOL_TRANTYPE( UBYTE * destptr, UBYTE entry )
{
	UBYTE trnproc;

	/* EMV TRANSACTION TYPE IS THE
	 * FIRST BYTE OF THE PROCESSING CODE 
	 * FROM ISO8583 */
	/* @TUM In HDT Processing Code (TRNPROC) is used only
	 * in host modules (see struct bitmap_rec). 
	 * That's why it's easier to repeat it here using switch
	 * operator.
	 */
	switch ( TRINP.TRKEY )
	{
		case EMV_SALE:
			trnproc = EMVL2_GOODS_SERVICES;
			break;

		case EMV_CASH:
			trnproc = EMVL2_CASH;
			break;

		case EMV_REFUND:
			trnproc = EMVL2_REFUND;
			break;

		case EMV_SALCASH:
			trnproc = EMVL2_CASHBACK;
			break;

		default:			// EMV_AUTH
			trnproc = EMVL2_GOODS_SERVICES;
			break;
	}

	destptr[0] = trnproc;
	return True;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets Default DDOL
//! 
//! \param[out] destptr         Pointer to the Default DDOL buffer
//! \param[in] entry            Not used
//! 
//! \return
//!     Number of bytes copied to the destination buffer.
//-----------------------------------------------------------------------------
UBYTE DOL_DEFDDOL( UBYTE * destptr, UBYTE entry )
{
	UBYTE *tmpptr;
	UBYTE count = 0;

	tmpptr = &TAIDTAB[entry].TAIDDDOL[0];

	while ( *tmpptr && count++ < 20 )
	{
		*destptr++ = *tmpptr++;
	}
	return count;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Gets Default TDOL
//! 
//! \param[out] destptr         Pointer to the Default TDOL buffer
//! \param[in] entry            Not used
//! 
//! \return
//!     Number of bytes copied to the destination buffer.
//-----------------------------------------------------------------------------
UBYTE DOL_DEFTDOL( UBYTE * destptr, UBYTE entry )
{
	UBYTE *tmpptr;
	UBYTE count = 0;

	tmpptr = &TAIDTAB[entry].TAIDTDOL[0];

	while ( *tmpptr && count++ < 20 )
	{
		*destptr++ = *tmpptr++;
	}
	return count;
}

#endif // MAKE_EMV

