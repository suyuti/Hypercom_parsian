#ifdef	MAKE_EMV
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     iccdebug.c
//  EMV application processing
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "osclib.h"
#include "sdk.h"
#include "sdktime.h"
#include "ddtm.h"
#include "fixdata.h"
#include "sysstruc.inc"
#include "tlib.h"
#include "utllib.h"
#include "emv_l2.h"
#include "emv2_pk.h"
#include "emvdata.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "emvtabs2.h"
#include "emvutils.h"
#include "tables.h"
#include "chkprinter.h"
#include "iccdebug.h"
#include "hyputils.h" //@AAMELIN


void TranslateCert(UBYTE *in_buffer, char *out_buffer);
void Hex2String( UBYTE * ICCtag, UBYTE length, char *buffer );
void Bin2Bcd( UBYTE* pDest, UBYTE* pSrc, UBYTE lDest, UBYTE lSrc );
void AddTagToBuffer(UWORD TagID, UBYTE TagLen, UBYTE *TagData, UBYTE *Buffer );


//void PrintPinpadStatus( void );
static void PrintLine( char* pString );
static void PrintBuf( void );
static void PrintLF( void );
static Bool IsBitSet ( UBYTE * pTestByte, UBYTE TestBit );

//! ICC record buffer
static struct iccrec_rec IccEntry;
//! Internal buffer
static char PrtBuffer[40];
//! Print Hex string with spaces if True, w/out if False
Bool DelimFlag;

//! Handle of ICC Debug file
FILE	*IccDebFilePtr;
//! ICC Debug filename
const char IccDebFileName[] = "ICCDEB.RPT";

static const char StrAbsent[] = "absent";
static const char StrNone[] = "none";
static const char RequestedNotPerformed[] = "requested but not performed\n";
static const char PerformedNotRequested[] = "performed when not requested\n";
static const char InfoAdviceRequested1[] = "Info Advice Requested 1st GenAC\n";
static const char InfoAdviceRequested2[] = "Info Advice Requested 2nd GenAC\n";


//-----------------------------------------------------------------------------
//! \brief
//!     Converts Hex to ascii and inserts spaces if necessary, null terminates
//!     the line
//!
//! \param
//!		[in]ICCtag			Pointer to tag list
//!		[in]length			Length of tag list
//!		[out]buffer			Pointer to the destination buffer
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void Hex2String( UBYTE * ICCtag, UBYTE length, char *buffer )
{
	static UBYTE *pSrc;
	static char *pDest;
	char nibble;

	for ( pSrc = ICCtag, pDest = buffer; length--; pSrc++ )
	{
		nibble = *pSrc;
		nibble >>= 4;
		nibble += 0x30;
		if ( nibble >= 0x3A )
		{
			nibble += 0x07;
		}
		*pDest = nibble;

		nibble = *pSrc;
		nibble &= 0x0F;
		nibble += 0x30;
		if ( nibble >= 0x3A )
		{
			nibble += 0x07;
		}
		*(pDest + 1) = nibble;

		if (DelimFlag)
		{
			*(pDest + 2) = ' ';
			pDest++;
		}

		pDest += 2;
	}

	*pDest = 0;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Converts binary value to BCD (lSrc <= 4)
//!
//! \param
//!		[out]pDest			Pointer to destination buffer
//!		[out]pSrc			Pointer to source buffer
//!		[in]lDest			Length of destination buffer
//!		[in]lSrc			Length of source buffer
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void Bin2Bcd( UBYTE* pDest, UBYTE* pSrc, UBYTE lDest, UBYTE lSrc )
{
	int i;
	unsigned long src = 0;

	for ( i = 0; i < lSrc; i++ )
	{
		src = (src << 8) | pSrc[i];
	}

	memset( pDest, 0, lDest );

	for ( i = 1; i <= lDest; i++ )
	{
		pDest[lDest - i] = (UBYTE)(src % 10);
		src = src / 10;
		pDest[lDest - i] |= (UBYTE)((src % 10) << 4);
		src = src / 10;
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Adds TLV data to buffer and increments overall length
//!
//! \param
//!		[in]TagID			Tag
//!		[in]TagLen			Length of tag
//!		[in]TagData			Pointer to data associated with the tag
//!		[in/out]Buffer		Pointer to destination buffer
//!
//! \return
//!     None
//!
//! \note
//!     First byte of "Buffer" contains current data length
//-----------------------------------------------------------------------------
void AddTagToBuffer(UWORD TagID, UBYTE TagLen, UBYTE *TagData, UBYTE *Buffer )
{
	// Note First byte of buffer contains current data length
	UBYTE *WritePtr = Buffer + *Buffer + 1;

	// Write the tag ID
	if (TagID > 0xFF)  // Incoming TagID may be big or little endian
	{
		*(WritePtr++) = (UBYTE) (TagID >> 8); 	// First write the most significant byte
		(*Buffer)++;
	}
    *(WritePtr++) = (UBYTE) (TagID & 0x00FF);  //  Now write the least significant byte
	
	// Write the tag length
    *(WritePtr++) = TagLen;
	(*Buffer) += 2;

	// Finally write the tag data
	memcpy(WritePtr, TagData, TagLen);
	*Buffer += TagLen;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints ICC debug data
//!
//! \param
//!		[in]emv_mode		EMV library mode (EMVL2_v311 or EMVL2_v4)
//!		[in]PrintCand		Pointer to candidate list
//!		[in]PrevAmount		Pointer to 'Previous Amount'
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void PrintICCDebug( UBYTE emv_mode, CandTextType * PrintCand, UBYTE *PrevAmount )
{
	UBYTE ICCtag[32];
	UBYTE *debug;
	UBYTE TagReturnLength;
	UBYTE script;
	UBYTE aip;
	UBYTE tsi;
	UWORD index;
	UBYTE tagcount, value;
	UBYTE TxnAmount[6];
   	UBYTE FloorAmount[6];
	UBYTE Threshold[6];
	UBYTE TargetPerCent, MaxPerCent, RandNum;
    UBYTE SC_SW12[2];

	DelimFlag = True;

	// See if printer may be opened
	//if ( PrinterOpen( TCONF.TCPRINTER ) == orvOK )

	if ((IccDebFilePtr = fopen( IccDebFileName, "wb" )) == NULL) return;
	{
		// Initialise the auto_test data buffer
		SC_CmdBytes[Cla] = 0x80;     // set class
        SC_CmdBytes[Ins] = 0x52;     // set ins
        SC_CmdBytes[P1]  = 0x11;     // set p1
        SC_CmdBytes[P2]  = 0x11;     // set P2
		SC_CmdBytes[Lc]  = 0x00;     // initialise to zero, will be incremented as data is added to the buffer

		// Heading
		memcpy( PrtBuffer, "ICC Debug (lib = v", 18 );
		// Read the library version string (5 bytes)
		EMVL2_GetVersionString( (UBYTE *) ICCtag );
		memcpy( &PrtBuffer[18], ICCtag, 5 );
		PrtBuffer[23] = ')';
		PrtBuffer[24] = 0;
		PrintBuf( );
		PrintLine( "=====================" );

		// Print the terminal capabilities
		memcpy( PrtBuffer, "Term Cap:     ", 14 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_9F33 ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[14] );
		}
		else
		{
			memcpy( &PrtBuffer[14], StrNone, 5 );
		}
		PrintBuf( );

		// Print the additional terminal capabilities
		memcpy( PrtBuffer, "Add Term Cap: ", 14 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_9F40 ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[14] );
		}
		else
		{
			memcpy( &PrtBuffer[14], StrNone, 5 );
		}
		PrintBuf( );

		// Print the application capabilities
		memcpy( PrtBuffer, "App Cap:      ", 14 ); 
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_DF56 ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[14] );
		}
		else
		{
			memcpy( &PrtBuffer[14], StrNone, 5 );
		}
        PrintBuf( );

		// Print the config checksum
		memcpy (PrtBuffer, "Checksum:     ", 14 );
		if ( EMVL2_GetConfig( ICCtag, 4 ) )
		{
			Hex2String( ICCtag, 4, &PrtBuffer[14] );
		}
		else
		{
			memcpy( &PrtBuffer[14], StrNone, 5 );
		}
        PrintBuf( );
		PrintLF( );

		// Print the application descriptive text
		memcpy( PrtBuffer, "Txt : ", 6 );
		if ( PrintCand != 0 )	// check application selected
		{
			memcpy( &PrtBuffer[6], &PrintCand->text, (value = strlen((char *)&PrintCand->text)) );
			PrtBuffer[6 + value] = 0;
		    AddTagToBuffer(0xDF60, (UBYTE) strlen((char *) &PrintCand->text), (UBYTE *) &PrintCand->text, &SC_CmdBytes[Lc]);
		}
		else
		{
			memcpy( &PrtBuffer[6], "not set", 8 );
		}
		PrintBuf( );

		// Print the issuer code table index
		memcpy( PrtBuffer, "Code Table : ", 13 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_9F11 ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[13] );
		}
		else
		{
			memcpy( &PrtBuffer[13], StrNone, 5 );
		}
		PrintBuf( );

		// retrieve AID & print
		memcpy( PrtBuffer, "AID : ", 6 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_84 ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[6] );
		}
		else
		{
			memcpy( &PrtBuffer[6], "not selected", 13 );
		}
		PrintBuf( );

		// Print the application priority indicator
		memcpy( PrtBuffer, "API : ", 6 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_87 ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[6] );
		}
		else
		{
			memcpy( &PrtBuffer[6], StrAbsent, 7 );
		}
		PrintBuf( );

		// Print the CA public key index
		memcpy( PrtBuffer, "Cert Auth Public Key Index : ", 29 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_8F ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[29] );
		}
		else
		{
			memcpy( &PrtBuffer[29], StrAbsent, 7 );
		}
		PrintBuf( );
		PrintLF( );

		// retrieve Issuer Action codes & print
		PrintLine( "Issuer Action codes: (* = default)" );

		memcpy( PrtBuffer, "DEN : ", 6 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_9F0E ) )
		{

			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[6] );
		}
		else
		{
			memcpy( &PrtBuffer[6], "00 00 00 00 00 *", 17 );
		}
		PrintBuf( );

		memcpy( PrtBuffer, "ONL : ", 6 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_9F0F ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[6] );
		}
		else
		{
			switch ( emv_mode )
			{
				case EMVL2_v311:
					memcpy( &PrtBuffer[6], "F8 F8 FC F8 F0 *", 17 ); // EMV 3.1.1 bits
					break;

				case EMVL2_v4:
					memcpy( &PrtBuffer[6], "FC F8 FC F8 F0 *", 17 ); // EMV4 bits
					break;
			}
		}
		PrintBuf( );

        // Retrieve AUC & print
		memcpy( PrtBuffer, "   AUC : ", 9 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_9F07 ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[9] );
		}
		else
		{
			memcpy( &PrtBuffer[9], StrAbsent, 7 );
		}
		PrintBuf( );


		memcpy( PrtBuffer, "DEF : ", 6 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_9F0D ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[6] );
			memset( &PrtBuffer[20], ' ', 6 );
		}
		else
		{
			switch ( emv_mode )
			{
				case EMVL2_v311:
					memcpy( &PrtBuffer[6], "F8 F8 FC F8 F0 *", 16 );
					break;

				case EMVL2_v4:
					memcpy( &PrtBuffer[6], "FC F8 FC F8 F0 *", 16 );
					break;
			}
		}

		// retrieve AIP & print
		memcpy( &PrtBuffer[25], "AIP : ", 6 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_82 ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[25+6] );
		}
		else
		{
			memcpy( &PrtBuffer[25+6], StrAbsent, 7 );
		}
		PrintBuf( );
		PrintLF( );

		aip = ICCtag[0];		// Store for later

		// retrieve TVR & print
		memcpy( PrtBuffer, "TVR : ", 6 );
		EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_95 );
		Hex2String( ICCtag, TagReturnLength, &PrtBuffer[6] );
		// write data to auto_test buffer
		AddTagToBuffer(TAG_95, TagReturnLength, ICCtag, &SC_CmdBytes[Lc]);

		// retrieve TSI & print
		memcpy( &PrtBuffer[20], "     TSI : ", 11 );
		EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_9B );
		Hex2String( ICCtag, TagReturnLength, &PrtBuffer[20+11] );
		PrintBuf( );
		tsi = ICCtag[0];		// Store for later
		// write data to auto_test buffer
		AddTagToBuffer(TAG_9B, TagReturnLength, ICCtag, &SC_CmdBytes[Lc]);

		// retrieve CVMR & print
		memcpy( PrtBuffer, "CVMR: ", 6 );
		EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_9F34 );
		Hex2String( ICCtag, TagReturnLength, &PrtBuffer[6] );
		PrintBuf( );
		PrintLF( );
		// write data to auto_test buffer
		AddTagToBuffer(TAG_9F34, TagReturnLength, ICCtag, &SC_CmdBytes[Lc]);

		// If present, print the script processing results
		EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_DF52 );
		if ( TagReturnLength > 0 )
		{
			// write data to auto_test buffer
		    AddTagToBuffer(TAG_DF52, TagReturnLength, ICCtag, &SC_CmdBytes[Lc]);
			PrintLine( "Script Processing Results:" );
			for ( script = 0; script < TagReturnLength; script += 5 )
			{
				Hex2String( ICCtag + script, 5, PrtBuffer );
				PrintBuf( );
			}
			PrintLF( );
		}


		// retrieve Issuer App Data & print
		PrintLine( "Issuer App Data:" );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_9F10 ) )
		{
			Hex2String( ICCtag, TagReturnLength, PrtBuffer );
			PrintBuf( );
		}
		else
		{
			PrintLine( (char*)StrAbsent );
		}
		PrintLF( );


		// retrieve authorisation response code & print
		memcpy( PrtBuffer, "Auth Response Code: ", 20 );
		if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_8A ) )
		{
			Hex2String( ICCtag, TagReturnLength, &PrtBuffer[20] );
			// write data to auto_test buffer
		    AddTagToBuffer(TAG_8A, TagReturnLength, ICCtag, &SC_CmdBytes[Lc]);
			ICCtag[2] = 0;
			memcpy( &PrtBuffer[25], " \"", 2 );
			memcpy( &PrtBuffer[27], ICCtag, 2 );
			memcpy( &PrtBuffer[29], "\"", 2 );
		}
		else
		{
			memcpy( &PrtBuffer[20], StrAbsent, 7 );
		}
		PrintBuf( );
		PrintLF( );

		// Print scheme specific tags
		tagcount = 0;
		PrintLine( "Scheme specific tags:" );
		for (index = 0x9F50; index < 0x9F7F; index++)
		{
			if ( EMVL2_ReadTagData( ICCtag, &TagReturnLength, index ) )
			{
				tagcount++;
				value = (UBYTE)(index >> 8); 
				Hex2String( &value, 1, PrtBuffer);
				value = (UBYTE) (index & 0x00FF); 
				Hex2String( &value, 1, &PrtBuffer[2]);
				PrtBuffer[4] = ' ';
				Hex2String( ICCtag, TagReturnLength, &PrtBuffer[5]);
				PrintBuf( );
			}
		}

		if ( 0 == tagcount )
		{
			PrintLine( (char*)StrNone );
		}
		PrintLF( );


		// Get the 12 bytes of debug data
		EMVL2_GetDebugData( &debug );
		// write data to auto_test buffer
		AddTagToBuffer(0xDF61, 14, debug, &SC_CmdBytes[Lc]);
  
		
		//Print GenAC table
		PrintLine( "GenAC: Request    Response" );

		//1st GenAC
		memcpy(PrtBuffer, "1st    ", 7);
		Hex2String(debug + 8, 1, PrtBuffer + 7);  //request
		TranslateCert(debug + 8, PrtBuffer + 9);
		Hex2String(debug + 9, 1, PrtBuffer + 18); //response
		TranslateCert(debug + 9, PrtBuffer + 20);
		PrtBuffer[28] = 0x00;                     // add null terminator
		PrintBuf( );
 
		//2nd GenAC
		memcpy(PrtBuffer, "2nd ", 4);
		Hex2String(debug + 10, 1, PrtBuffer + 7);  //request
		TranslateCert(debug + 10, PrtBuffer + 9);
		Hex2String(debug + 11, 1, PrtBuffer + 18); //response
		TranslateCert(debug + 11, PrtBuffer + 20);
		PrtBuffer[28] = 0x00;                      // add null terminator
		PrintBuf( );
		PrintLF( );

		// Print Debug Data
		memcpy( PrtBuffer, "L2 Debug Data: ", 15 );
		Hex2String( debug + 4, 4, &PrtBuffer[15] );
		PrintBuf( );
		PrintLF( );

		// Print Terminating SW1 SW2 values
		PrintLine( "Final SW1 SW2");
		memcpy(PrtBuffer, "ICC    ", 7);
		Hex2String(debug, 2, PrtBuffer + 7);
		PrintBuf( );

		memcpy(PrtBuffer, "HCP    ", 7);
		Hex2String(debug + 2, 2, PrtBuffer + 7);
		PrintBuf( );
		PrintLF( );


		// Compare the aip and tsi and report missing or extra processes. (bit 7 to bit 0)
		// TSI bit 7 - Offline data authentication was performed
		if ( ( aip & 0x61 ) && !( tsi & 0x80 ) )
		{
			// the line above effectively writes into the string region, the result is: Page 44 corrupted
			memcpy( PrtBuffer, "DA ", 3 );
			memcpy( &PrtBuffer[3], RequestedNotPerformed, sizeof(RequestedNotPerformed) );
			PrintBuf( );
		}
		else if ( !( aip & 0x61 ) && ( tsi & 0x80 ) )
		{
			memcpy( PrtBuffer, strcat( "DA ", PerformedNotRequested ), 3+sizeof(PerformedNotRequested) );
			PrintBuf( );
		}

		// TSI bit 6 - Cardholder verification was performed
		if ( ( aip & 0x10 ) && !( tsi & 0x40 ) )
		{
			memcpy( PrtBuffer, strcat( "CV ", RequestedNotPerformed ), 3+sizeof(RequestedNotPerformed) );
			PrintBuf( );
		}
		else if ( !( aip & 0x10 ) && ( tsi & 0x40 ) )
		{
			memcpy( PrtBuffer, strcat( "CV ", PerformedNotRequested ), 3+sizeof(PerformedNotRequested) );
			PrintBuf( );
		}

		// TSI bit 5 - card risk management was performed
		if ( !( tsi & 0x20 ) )
		{
			PrintLine( "Mandatory CRM not performed\n" );
		}

		// TSI bit 4 - Issuer authentication was performed
		if ( debug[8] & 0x80 )	// Report this only if we tried to go online
		{
			if ( ( aip & 0x04 ) && !( tsi & 0x10 ) )
			{
				PrintLine( "IA supported but not performed\n" );
			}
			else if ( !( aip & 0x04 ) && ( tsi & 0x10 ) )
			{
				PrintLine( "IA performed when not supported\n" );
			}
		}

		// TSI bit 3 - terminal risk management was performed
		if ( ( aip & 0x08 ) && !( tsi & 0x08 ) )
		{
			memcpy( PrtBuffer, strcat( "TRM ", RequestedNotPerformed ), 4+sizeof(RequestedNotPerformed) );
			PrintBuf( );
		}
		else if ( !( aip & 0x08 ) && ( tsi & 0x08 ) )
		{
			memcpy( PrtBuffer, strcat( "TRM ", PerformedNotRequested ), 4+sizeof(PerformedNotRequested) );
			PrintBuf( );
		}

		// TSI bit 2 - Script processing was performed
		if ( tsi & 0x04 )
		{
			PrintLine( "Script processing was performed\n" );
		}


		// Note if advice requested on 1st GenAC
		if ((debug[9] != 0xFF) && ( debug[9] & 0x08 )) //0xFF = initial value
		{
			PrintLine( (char*)InfoAdviceRequested1 );
		}

		// Note if advice requested on 2st GenAC
		if ((debug[11] != 0xFF) && ( debug[11] & 0x08 )) // 0xFF = initial value
		{
			PrintLine( (char*)InfoAdviceRequested2 );
		}

// Added IssScriptBuf & ScriptLen content printing
		if ( 0 != ScriptLen )
		{
			PrintLF( );
			memcpy( PrtBuffer, "ScriptLen= 0x", 13 );
			BufAsc( &PrtBuffer[13], &ScriptLen );
			PrtBuffer[15] = '\0';	// Null terminator
			PrintBuf( );
			PrintLine( "IssScriptBuf= " );

			/* Print IssScriptBuf content, 14 bytes per line */
			for ( TagReturnLength = 0; TagReturnLength < ScriptLen; TagReturnLength+=14 )
			{
				value = ScriptLen - TagReturnLength;
				Hex2String( &IssScriptBuf[TagReturnLength], (value>14) ? 14 : value, PrtBuffer );
				PrintBuf( );
			}	// for ()...
			/* Clear the ScriptLen once we've printed the data */
			ScriptLen = 0;
		}	// if ( 0 != ScriptLen )

		/* Terminal Risk Management Debug Info */
		// TSI bit 3 - terminal risk management was performed
		if ( ( aip & 0x08 ) && ( tsi & 0x08 ) )
		{
			/* Print Hex Strings without spaces */
			DelimFlag = False;

			memcpy( PrtBuffer, "Risk Management Info:", 22 );
			PrintBuf( );

		    /* Read the Transaction Amount from tag store - sale + cash amt */
		    EMVL2_ReadTagData( TxnAmount, &TagReturnLength, TAG_9F02 );
			memset( PrtBuffer, ' ', sizeof(PrtBuffer) );
			memcpy( PrtBuffer, " Amount=", 8 );
			Hex2String( TxnAmount, 6, &PrtBuffer[8] );
			PrintBuf( );

			memcpy( PrtBuffer, "PrevAmt=", 8 );
			Hex2String( PrevAmount, 6, &PrtBuffer[8] );
			PrintBuf( );

			if ( 0 != AddAmt( TxnAmount, TxnAmount, PrevAmount ) )
			{
				memset( TxnAmount, 0x99, 6 );
			}
			memcpy( PrtBuffer, " TotAmt=", 8 );
			Hex2String( TxnAmount, 6, &PrtBuffer[8] );
			PrintBuf( );

		    /* Read the Terminal Floor Limit Tag */
			memset( FloorAmount, 0, 6 );
		    EMVL2_ReadTagData( FloorAmount, &TagReturnLength, TAG_9F1B );

			memcpy( PrtBuffer, "  Floor=", 8 );
			Bin2Bcd( FloorAmount, FloorAmount, 6, 4 );
			Hex2String( FloorAmount, 6, &PrtBuffer[8] );
			PrintBuf( );

			memcpy( PrtBuffer, "TotAmt >= Floor", 16 );
		    if ( memcmp( TxnAmount, FloorAmount, 6 ) < 0 )
			{
				PrtBuffer[7] = '<';
				PrtBuffer[8] = ' ';
				PrintBuf( );
				PrintLF( );

				memcpy( PrtBuffer, "Random Online Selection:", 25 );
				PrintBuf( );

				IccEntry = *FindIcc(TRINP.TRISID);
				memset( Threshold, 0, sizeof(Threshold) );
				memcpy( &Threshold[3], &IccEntry.ICCTHRESH, 2 );
				Bcd2Bin( &TargetPerCent, &IccEntry.ICCTARGPCT, 1 );
				Bcd2Bin( &MaxPerCent, &IccEntry.ICCMAXTPCT, 1 );

			   	/* Read the Terminal Type */
			   	EMVL2_ReadTagData( (UBYTE *)&ICCtag, &TagReturnLength, TAG_9F35 );
				memcpy( PrtBuffer, "  Term Type=", 12 );
				Hex2String( ICCtag, 1, &PrtBuffer[12] );
				PrintBuf( );
			   
			   	/* Check if random selection was not to be performed */
			   	switch ( ICCtag[0] )
			   	{
			   	    case 13 :
			   	    case 16 :
			   	    case 23 :
			   	    case 26 : 
			       	    break;

					default :
					{
						/* Check if Target Percentage is greater than the maximum range value
							(0 -> 99) */
						if ( TargetPerCent > 99 )
						{
							/* Yes; set Target Percentage to the maximum range value (0 -> 99) */
							TargetPerCent = 99;
						}
							/* Check if Maximum Target Percentage is less than the minimum range value
							(Target Percentage -> 99) */
						if ( MaxPerCent < TargetPerCent )
						{
							/* Yes; set Maximum Target Percentage to the minimum range value
							(Target Percentage -> 99) */
							MaxPerCent = TargetPerCent;
						}
						else
						{
							/* No; check if Maximum Target Percentage is greater than the maximum
								range value (Target Percentage -> 99).
							*/
							if ( MaxPerCent > 99 )
							{
								/* Yes; set Maximum Target Percentage to the maximum range value
								(Target Percentage -> 99) */
								MaxPerCent = 99;
							}
						}

					  	/* Read the Unpredictable number */
					   	EMVL2_ReadTagData( ICCtag, &TagReturnLength, TAG_9F37 );
					    RandNum = (UBYTE) CvtBcd( ((( ICCtag[0] & 0x7f) * 98) / 127) + 1 );

						if ( memcmp( Threshold, FloorAmount, 6 ) >= 0 )
						{
							/* Threshold >= Floor, so put Threshold = (Floor - 1) */
							SubAmt( Threshold, FloorAmount, (UBYTE *)"\x00\x00\x00\x00\x00\x01" );
						}

						memcpy( PrtBuffer, "  Threshold=", 12 );
						Hex2String( Threshold, 6, &PrtBuffer[12] );
						PrintBuf( );
						memcpy( PrtBuffer, "MaxTargPerc=", 12 );
                        PrtBuffer[14]= (UBYTE)CvtBcd( MaxPerCent );
                        BufAsc( &PrtBuffer[12],(UBYTE*)&PrtBuffer[14] );
						PrintBuf( );
						memcpy( PrtBuffer, "   TargPerc=", 12 );
                        PrtBuffer[14]= (UBYTE)CvtBcd( TargetPerCent );
                        BufAsc( &PrtBuffer[12],(UBYTE*)&PrtBuffer[14] );
						PrintBuf( );
						memcpy( PrtBuffer, "  RandomNum=", 12 );
						Hex2String( &RandNum, 1, &PrtBuffer[12] );
						PrintBuf( );

						PrintLF( );

	   		            /* Check if the random number is less than or equal to the Target
			               Percentage */
						memcpy( PrtBuffer, "RandomNum <= TargPerc", 22 );
						if ( RandNum > TargetPerCent )
						{
							PrtBuffer[10] = '>';
							PrtBuffer[11] = ' ';
							PrintBuf( );

				   	        /* Check if the Transaction Amount is less than the Threshold value */
							memcpy( PrtBuffer, "TotAmt <  Threshold", 20 );
							if ( memcmp( TxnAmount, Threshold, 6 ) >= 0 )
							{
								PrtBuffer[7] = '>';
								PrtBuffer[8] = '=';
								PrintBuf( );
								PrintLF( );

								memcpy( PrtBuffer, "Transaction Target Percentage=", 31 );
								PrintBuf( );

								memset( PrtBuffer, ' ', 40 );
								Hex2String( TxnAmount, 6, &PrtBuffer[0] );
								Hex2String( Threshold, 6, &PrtBuffer[13] );
								PrtBuffer[12] = '-';
								PrintBuf( );

								memset( PrtBuffer, ' ', 40 );
								memset( &PrtBuffer[0], '-', 25 );
								PrtBuffer[25] = ' ';
								PrtBuffer[26] = 'X';
								PrtBuffer[28] = '(';
                                PrtBuffer[40]= (UBYTE)CvtBcd( MaxPerCent );
                                BufAsc( &PrtBuffer[29],(UBYTE*)&PrtBuffer[40] );
								PrtBuffer[31] = '-';
                                PrtBuffer[40]= (UBYTE)CvtBcd( TargetPerCent );
                                BufAsc( &PrtBuffer[32],(UBYTE*)&PrtBuffer[40] );
								PrtBuffer[34] = ')';
                                PrtBuffer[40]= (UBYTE)CvtBcd( TargetPerCent );
                                BufAsc( &PrtBuffer[38],(UBYTE*)&PrtBuffer[40] );
								PrtBuffer[36] = '+';
								PrintBuf( );

								memset( PrtBuffer, ' ', 40 );
								Hex2String( FloorAmount, 6, &PrtBuffer[0] );
								Hex2String( Threshold,   6, &PrtBuffer[13] );
								PrtBuffer[12] = '-';
								PrintBuf( );
								PrintLF( );
								
			                    /* Zero out buffer */
			                    memset( ICCtag, 0x00, sizeof(ICCtag) );

			                    /* Set the last byte of Temp Amount 1 to Temp Percentage in BCD */
			                    ICCtag[5] = (UBYTE) CvtBcd( (CvtBin(MaxPerCent) - CvtBin(TargetPerCent)) );

			                    /* Calculate Amount Authorised - Threshold Value */
			                    SubAmt( &ICCtag[6], TxnAmount, Threshold );
			
			                    /* Calculate (Maximum Target Percentage - Target Percentage) *
			                      (Amount Authorised - Threshold Value) */
			                    MltAmt( &ICCtag[12], &ICCtag[0], &ICCtag[6] );
			   
			                    /* Calculate Floor Limit - Threshold Value */
			                    SubAmt( &ICCtag[6], FloorAmount, Threshold );

			                    /* Calculate [(Maximum Target Percentage - Target Percentage) *
			                    (Amount Authorised - Threshold Value)] /
			                    (Floor Limit - Threshold Value) */
			                    DivAmt( &ICCtag[0], &ICCtag[18], &ICCtag[12], &ICCtag[6] );
			
			                    /* Set the last byte of Temp Amount 2 to Target Percentage in BCD */
			                    ICCtag[29] = TargetPerCent;
			
			                    /* Calculate Ttp */
			                    AddAmt( &ICCtag[12], &ICCtag[0], &ICCtag[24] );
			
								memset( PrtBuffer, ' ', 40 );
								memcpy( &PrtBuffer[0], " = ", 3 );
								Hex2String( &ICCtag[12], 6, &PrtBuffer[3] );
								PrintBuf( );
								PrintLF( );

			                    /* Set the last byte of Temp Amount 1 to random number in BCD */
			                    ICCtag[29] = RandNum;

			                    /* Check if random number is less than or equal to the Transaction
			                    Target Percentage (Ttp) */
								memcpy( PrtBuffer, "RandomNum >  Ttp", 17 );
			                    if ( memcmp( &ICCtag[24], &ICCtag[12], 6 ) <= 0 )
			                    {
									PrtBuffer[10] = '<';
									PrtBuffer[11] = '=';
							    }

								PrintBuf( );
							}	// if ( memcmp( TxnAmount, Threshold, 6 ) >= 0 )
							else
							{
								/* TxnAmount < Threshold, not selected */
								PrintBuf( );
								PrintLF( );
							}
						}	// if ( RandNum > TargetPerCent )
						else
						{
							/* RandNum <= TargPerCent, selected */
							PrintBuf( );
							PrintLF( );
						}
					}	// default {}
				}	// switch
			}	// if ( memcmp( TxnAmount, FloorAmount, 6 ) < 0 )
			else
			{
				/* No Random Online Selection */
				PrintBuf( );
				PrintLF( );
			}	// if ( memcmp( TxnAmount, FloorAmount, 6 ) < 0 )
		}

		/* Perform a form feed which cuts the paper */
		PrintLine( "\f" );

		fclose( IccDebFilePtr );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Translates certificate buffer first byte into text description
//!
//! \param
//!		[in]in_buffer		Pointer to certificate buffer
//!		[out]out_buffer		Pointer to destination (string) buffer
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void TranslateCert(UBYTE *in_buffer, char *out_buffer)
{
   if (*in_buffer == 0xFF) // buffer in virgin state
   {
       memcpy(out_buffer, " (none)  ", 9); // data was never populated
   }
   else
   {
       switch (*in_buffer & 0xC0)  // read only top two bits
       {
           case 0x00:
              memcpy(out_buffer, " (AAC)   ", 9);
              break;

           case 0x40:
              memcpy(out_buffer, " (TC)    ", 9);
              break;

           case 0x80:
              memcpy(out_buffer, " (ARQC)  ", 9);
              break;

           case 0xC0:
              memcpy(out_buffer, " (AAR)   ", 9);
              break;
       }
   } 

}
   
#if	0
//-----------------------------------------------------------------------------
//! \brief
//!     Prints External Pinpad info
//!
//! \return
//!     None
//!
//! \note
//!     Called by Function 61 (FUN_61)
//-----------------------------------------------------------------------------
void PrintPinpadStatus( void )
{
    PrintLine ("================");
    memcpy( PrtBuffer, "External Pinpad: ", 17 );
	if ( TCONF.TCOPT2 & TC2_PINSCR )
	{
	    memcpy( &PrtBuffer[17], "ENABLED", 8 );
        PrintBuf( );

	    // Report on Atmel
        memcpy( PrtBuffer, "HCP : ", 6 );
        if (CryptSlot != 0)
        {
            BfAscii( &PrtBuffer[6], (UBYTE *) &CryptSlot, 1);
			// print out the Atmel Version
            memcpy( &PrtBuffer[8], ". ver: ", 7 );
            memcpy( &PrtBuffer[15], Atmel_ATR,  sizeof(Atmel_ATR) );
			PrtBuffer[15+sizeof(Atmel_ATR)] = '\0';
        }
        else
        {
            memcpy( &PrtBuffer[6], StrAbsent, 7 );
        }

        PrintBuf( );
	}
	else
	{
	    memcpy( &PrtBuffer[17], "DISABLED", 9 );
        PrintBuf( );
	}

    // Close Printer
    /* Perform a form feed which cuts the paper */
   	PrintLine( "\f" );
}
#endif	// False

//-----------------------------------------------------------------------------
//! \brief
//!     Prints the null-terminated line
//!
//! \param
//!		[in]pString			Pointer to string to be printed
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PrintLine( char* pString )
{
	if ( IccDebFilePtr ) fwrite( pString, 1, strlen(pString), IccDebFilePtr );

    /* No need to print LF for impact printers */
	if ( ForceLF )
	{                                    
   	    PrintLF(); 
	}                                    
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints the content of PrtBuffer
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PrintBuf( void )
{
	if ( IccDebFilePtr ) fwrite( PrtBuffer, 1, strlen(PrtBuffer), IccDebFilePtr );

	/* No need to print LF for impact printers */
	if ( ForceLF )    
	{                                    
   	    PrintLF(); 
	}                                    
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints Line Feed
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PrintLF( void )
{
	if ( IccDebFilePtr ) fwrite( "\n", 1, 1, IccDebFilePtr );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints ICC Debug Report
//!
//! Prints ICC Debug Report from a file and remove this file after successful
//! printing
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
void PrintIccDebugReport( void )
{
	static char bufferprn[40];
	static size_t buffercnt;

	if ((IccDebFilePtr = fopen( IccDebFileName, "rb" )) != NULL) 
	{
		// Notify user -- "PRINTING, WAIT..."
		ShowInfoMsg( PrintingWait, PleaseWait );

		// See if Printer is available
		if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
		{
			// Close the file
			fclose( IccDebFilePtr );
			
			// Return to caller
			return;
		}

		// Open the printer
		if ( orvOK == SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ))
		{
			while (1)
			{
				// Read data from file
				buffercnt = fread( bufferprn, 1, sizeof(bufferprn), IccDebFilePtr );

				// Check the data length
				if ( (buffercnt == 0) || (buffercnt > sizeof(bufferprn))) break;

				if ( orvOK != SDK_PrintBuffer( (UBYTE*)bufferprn, buffercnt ) ) break;

				if (buffercnt != sizeof(bufferprn)) break;
			}
			
			// Perform a form feed which cuts the paper
			SDK_PrintString( "\f" );

			// Wait till printing is done
			while ( SDK_PrinterBusy( ) && ( 0x0E != TCONF.TCPRINTER ) )
			{
				// Give up the CPU
				SDK_RelinqCPU();
			}

			// Close the printer
			SDK_PrinterClose();
		}
		
		// Close the file
		fclose( IccDebFilePtr );
		// Remove this file
		remove( IccDebFileName );
	}
	else
	{
		return;
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints Application Capability settings
//!
//! \return
//!     None
//!
//! \note
//!     Created to take care of the bitmap flags
//-----------------------------------------------------------------------------
extern void PrintAppCap ( void )
{
    UBYTE TempASC[7];
    
	if ( orvOK == SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ))
	{
	    //TCONF.TCAPPCAP
	    SDK_PrintString ("TCONF App Cap: \n");
	    SDK_PrintString ("==============\n" )  ;
		BfAscii ((char *) &TempASC[0], (UBYTE *) &TCONF.TCAPPCAP[0], 1 ) ;
	    BfAscii ((char *) &TempASC[2], (UBYTE *) &TCONF.TCAPPCAP[1], 1 ) ;
	    BfAscii ((char *) &TempASC[4], (UBYTE *) &TCONF.TCAPPCAP[2], 1 ) ;
	    TempASC[6] = '\0';
	    SDK_PrintString ( (char *) TempASC )  ;
	    SDK_PrintString( "\n\n" );

	    SDK_PrintString ("Temp App Cap: \n");
	    SDK_PrintString ("=============\n" )  ;
		BfAscii ((char *) &TempASC[0], (UBYTE *) &LocalAppCap[0], 1 ) ;
	    BfAscii ((char *) &TempASC[2], (UBYTE *) &LocalAppCap[1], 1 ) ;
	    BfAscii ((char *) &TempASC[4], (UBYTE *) &LocalAppCap[2], 1 ) ;
	    TempASC[6] = '\0';
	    SDK_PrintString ( (char *) TempASC )  ;

	    SDK_PrintString( "\n\n" );

	    SDK_PrintString( "CHOLDER CONF        :" );
	    if (IsBitSet (&LocalAppCap[0], APP_CAP1_CARDHOLDER_CONF))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );
	    SDK_PrintString( "MULTI_LANG          :" );
	    if (IsBitSet (&LocalAppCap[0], APP_CAP1_MULTI_LANG))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );
	    SDK_PrintString( "PINPAD PRESENT      :" );
	    if (IsBitSet (&LocalAppCap[0], APP_CAP1_PED_PRESENT))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );
	    SDK_PrintString( "PIN BYPASS          :" );
	    if (IsBitSet (&LocalAppCap[0], APP_CAP1_PIN_BYPASS))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );
	    SDK_PrintString( "AMNT/PIN SAME DEVICE:" );
	    if (IsBitSet (&LocalAppCap[0], APP_CAP1_AMT_PIN_SAME    ))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );

	    SDK_PrintString( "\n" );
	    SDK_PrintString( "TXN LOG             :" );
	    if (IsBitSet (&LocalAppCap[1], APP_CAP2_TXN_LOG ))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );
	    SDK_PrintString( "EXCEPTION FILE      :" );
	    if (IsBitSet (&LocalAppCap[1], APP_CAP2_EXC_FILE_TEST ))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );
	    SDK_PrintString( "MERCH FORCE ONLINE  :" );
	    if (IsBitSet (&LocalAppCap[1], APP_CAP2_MERCH_SUSP ))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );
	    SDK_PrintString( "MERCH FORCE ACCEPT  :" );
	    if (IsBitSet (&LocalAppCap[1], APP_CAP2_EMV_FORCED))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );


	    SDK_PrintString( "\n" );
	    SDK_PrintString( "EMV ADVICES         :" );
	    if (IsBitSet (&LocalAppCap[2], APP_CAP3_EMV_ADVICES ))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );
	    SDK_PrintString( "ISSUER REFS         :" );
	    if (IsBitSet (&LocalAppCap[2], APP_CAP3_ISS_REFER ))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );
	    SDK_PrintString( "CARD REFS           :" );
	    if (IsBitSet (&LocalAppCap[2], APP_CAP3_CARD_REFER ))
	        SDK_PrintString( "YES\n" );
	    else
	        SDK_PrintString( "NO\n" );
	    SDK_PrintString( "BATCH CAPTURE       :NO\n" );
	    SDK_PrintString( "ONLINE CAPTURE      :NO\n" );

	    SDK_PrintString( "\n\n\n\n\f" );

	    //WaitPrinterBusy();
	    while ( SDK_PrinterBusy(  ) )
	    	SDK_RelinqCPU(  );

    	SDK_PrinterClose() ;
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Tests bit value in byte
//!
//! \param[in] pTestByte        Pointer to the byte
//! \param[in] TestBit          Bit mask
//!
//! \return
//!     True if bit is nonzero else False
//-----------------------------------------------------------------------------
static Bool IsBitSet ( UBYTE * pTestByte, UBYTE TestBit )
{
    if (* pTestByte & TestBit)
        return True;
    else
        return False;
}

#endif	// MAKE_EMV

