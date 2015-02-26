//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2006
//=============================================================================
//
//  Module overview:     crttab.c
//  Functions used to manage X509 Certificate download and store 
//  (Currently used for SSL Client Auth. facilities)
//

#ifdef MAKE_CERTIFICATE
//=============================================================================
//=============================================================================
#include <eftos.h>
#include <string.h>
#include <stdlib.h>

#include "basictyp.h"
#include "defines.h"
#include "struct.h"
#include "appdata.h"
#include "utllib.h"
#include "crttab.h"
#include "CryptoLib.h"
#include "CryptoService.h"

//=============================================================================
// External variables / declarations
//=============================================================================
// Public functions defined in this module


//=============================================================================
// Private defines and typedefs
//=============================================================================
#define SECTION_START		"-----BEGIN "
#define MaxSectionDataSize	(KEY_DATA_MAX_LENGTH / 3) * 4
#define NonPackedSize		offsetof( struct certificate_rec, CERT )

typedef enum
{
	KF_PKCS1_PLAIN,		// Don't change the value (same as in Crypto Library)
	KF_PKCS8_PLAIN,		// Don't change the value (same as in Crypto Library)
	KF_PKCS8_ENCRYPTED,	// Don't change the value (same as in Crypto Library)
	KF_PKCS8_ENCFIXEDKEY,// Don't change the value (same as in Crypto Library)	
	CF_X509,
	FORMAT_UNKNOWN,
}
PEM_STYPE;

typedef struct {
	PEM_STYPE	type;	// Section data type ID
	char*		begstr;	// Section begin string
	char*		endstr;	// Section end string
}
pem_section;


#endif // MAKE_CERTIFICATE


//=============================================================================
// Private function declarations
//=============================================================================
static void UnpackPemData( const char* fname );
static UWORD GetNextSection( FILE *f, UBYTE* pDest, PEM_STYPE* pType );
static UWORD StoreRSAKey(  UBYTE* pSrc, WORD Len, PEM_STYPE Type );
static UWORD StoreCertificate( UBYTE* pSrc, WORD Len );
static UWORD Base64Decode( UBYTE* pDest, UBYTE* pSrc, UWORD Len);
static void SaveCertificate( const char* fname );
static UBYTE GetTag( FILE *f );
static Bool GetTagData( UBYTE cCurrnetTag, FILE *f );
static void PrintCorruptCert( void );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static FILE		*fp;		// File handler for certificate
static char		tmpstr[128];	// Temporary file string storage
static UBYTE		DirectLen;	// Global counter storage

// Base64 "dictionary"
static const UBYTE abBase64[] = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// pem format file sections descriptions
static const pem_section SectionConsts[] = {
	{ KF_PKCS1_PLAIN,
	  "-----BEGIN RSA PRIVATE KEY-----",
	  "-----END RSA PRIVATE KEY-----",	},
	{ KF_PKCS8_PLAIN,
	  "-----BEGIN PRIVATE KEY-----",
	  "-----END PRIVATE KEY-----" 		},
	{ KF_PKCS8_ENCRYPTED,
	  "-----BEGIN ENCRYPTED PRIVATE KEY-----",
	  "-----END ENCRYPTED PRIVATE KEY-----" },
	{ CF_X509,
	  "-----BEGIN CERTIFICATE-----",
	  "-----END CERTIFICATE-----" 		},
	{ FORMAT_UNKNOWN,
	  NULL,
	  NULL 					}
};

// TAGS DEFINE
#define TAG_9M_PRIVATE_KEY_MODE		0x01
#define TAG_9K_CLIENT_CERTIFICATE	0x02
#define TAG_9T_CLIENT_KEY			0x04
#define TAG_9W_KEY_MODULE_LENGHT	0x08
#define TAG_UNKNOWN					0x40
#define TAG_NO_MORE_TAG_OR_ERROR	0x80

#define ALL_TAGS_WAS_READ           0x0F
#define ALL_TAGS_MASK			   0x0F


//=============================================================================
// Public function definitions
//=============================================================================


//-----------------------------------------------------------------------------
// Name:           CrtTabPre
// Description:    Setup destination address
//
// Parameters:     None
// Return Value:   None.
// Notes:          Tested with single record table only!
//-----------------------------------------------------------------------------
extern void CrtTabPre( void )
{
#ifdef MAKE_SMARTINIT
    // Clear HASH
    memset ( CERT_HASH, 0x00, SM_HASH_LEN );
#endif // MAKE_SMARTINIT

	// Clear temporary CRTTAB table
    memset ( &CRTTAB, 0x00, sizeof(CRTTAB) );

	B60DBytesLeft = MAXCERTSIZE; // Max Size of sertificate

	// Destination address for the Non Packed data
   	pB60DAddress  = (UBYTE *)&CRTTAB ;

	// How much Non Packed data has been stored
	DirectLen = 0;

	// Open a new file for certificate
	fp = fopen( TEMPCERT_FILE, "wb" );

}

//-----------------------------------------------------------------------------
//  Name:           CrtTab_LDIR
//  Description:    Move Data to a temporary Certificate file 
//  Parameters:     None.
//  Return Value:   None.
//  Notes:          None.
//-----------------------------------------------------------------------------
extern void CrtTab_LDIR( void )
{
	int err;

	// Check if any data to move
	if ( MoveSize != 0 )
	{
		// Did we store all table data not included in file
		if ( DirectLen < NonPackedSize )
		{
			// No, look how much we need to store directly into table
			if ( MoveSize <= NonPackedSize )
			{
				// Store all received data
				memcpy( pB60DAddress, pSFData,  MoveSize );

				// Update lenth of stored data and exit
				DirectLen += MoveSize;
				return;
			}
			else
			{
				// Store the data not included into file
				memcpy( pB60DAddress, pSFData, NonPackedSize );

				// Update lenth of stored data
				DirectLen = NonPackedSize;

				// Get file's data 
                pSFData += NonPackedSize;
				MoveSize -= NonPackedSize;
			}
		}

		// Store data into certificate file
		if ( MoveSize != fwrite( pSFData, 1, MoveSize, fp ) )
		{
			err = ferror( fp );
		}
	}
}

//-----------------------------------------------------------------------------
// Name:           CrtTab_Post
// Description:    Parce X509 Certificate data
// Parameters:     None
// Return Value:   None
// Notes:
//-----------------------------------------------------------------------------
extern void CrtTab_Post( void )
{
	// Close certificate file
	fclose( fp );

	// Parse received file
	UnpackPemData( TEMPCERT_FILE );
	// Delete file
	remove( TEMPCERT_FILE );

	if ( (CRTTAB.CERTLEN == 0) ||
       	      (CRTTAB.KEYMODULE == 0) )
	{
		// Wrong data received, 
		PrintCorruptCert();
	}
	else
	{
		// Save Certificate permanent file
		SaveCertificate(CERTIFICATE_FILE);
	}
		
}

//-----------------------------------------------------------------------------
// Name:           SaveCertificate
// Description:    Save parced certificate to certificate file
// Parameters:     
// Return Value:   
//-----------------------------------------------------------------------------
static void SaveCertificate( const char* fname )
{
	UWORD size;
	// Open file
	fp = fopen( fname, "wb" );
	// Move Tag (Private Key mode)
	fwrite( "9M", 1, 2, fp );
	// Move Size
	size = sizeof(CRTTAB.KEYFORMAT) + sizeof(CRTTAB.PKMKID) + sizeof(CRTTAB.CRTOPT1);
	fwrite( &size , 1, 2, fp );
	// Move data
	fwrite( &CRTTAB.KEYFORMAT, 1, sizeof(CRTTAB.KEYFORMAT), fp );
	fwrite( &CRTTAB.PKMKID, 1, sizeof(CRTTAB.PKMKID), fp );
	fwrite( &CRTTAB.CRTOPT1, 1, sizeof(CRTTAB.CRTOPT1), fp );

	// Move Tag (Client certificate)
	fwrite( "9K", 1, 2, fp );
	// Move Size
	size = CRTTAB.CERTLEN;
	fwrite( &size, 1, 2, fp );
	// Move tag data
	fwrite( &CRTTAB.CERT, 1, size, fp );

	// Move Tag (Client Key)
	fwrite( "9T", 1, 2, fp );
	// Move Size
	size = CRTTAB.KEYLEN;
	fwrite( &size, 1, 2, fp );
	// Move tag data
	fwrite( &CRTTAB.KEY, 1, size, fp );

	// Move Tag (Client Key Module)
	fwrite( "9W", 1, 2, fp );
	// Move Size
	size = 2;
	fwrite( &size, 1, 2, fp );
	// Move tag data
	fwrite( &CRTTAB.KEYMODULE, 1, size, fp );

	fclose(fp);
}

//-----------------------------------------------------------------------------
// Name:           GetCertificate
// Description:    Return pointer to selected X509 certificate record if valid.
// Parameters:     Id - Number of Certificate record (offset)
// Return Value:   Pointer to certificate record (NULL if wrong Id or
//                 certificate data not present)
// Notes:          None.
//-----------------------------------------------------------------------------
extern struct certificate_rec * GetCertificate( UBYTE Id )
{
	UBYTE cTagsWasRead = 0;
	UBYTE cCurrnetTag;
	// Check for certificate already readed


	// And certificate is present
	if ( (CRTTAB.CERTNUM != 0) &&
		 (CRTTAB.CERTLEN != 0) &&
			(CRTTAB.KEYMODULE != 0) )
	{
		// Return pointer to certificate record
		return( &CRTTAB );
	}
	else
	{

		fp = fopen( CERTIFICATE_FILE, "rb");
		// Check for file is presnt
		if ( NULL == fp)
			return (NULL);
		while ( ALL_TAGS_WAS_READ != ( cTagsWasRead & ALL_TAGS_MASK ) )
		{
			cCurrnetTag = GetTag(fp);
			if ( TAG_NO_MORE_TAG_OR_ERROR == cCurrnetTag )
			{
				fclose (fp);
				return (NULL);
			}
			if ( True == GetTagData( cCurrnetTag, fp) )
				cTagsWasRead |= cCurrnetTag;
			else
			{
				fclose (fp);
				return (NULL);
			}
		}
		fclose (fp);
		return (&CRTTAB);
		
	}
	
	// No valid data
	return( NULL );
}

static UBYTE GetTag( FILE *f )
{
	UBYTE cTAG[2];
	if ( 2 != fread(&cTAG, 1, 2, f) )
		return (TAG_NO_MORE_TAG_OR_ERROR);
	if ( 0 == memcmp( &cTAG,"9M" , 2))
		return (TAG_9M_PRIVATE_KEY_MODE);
	if ( 0 == memcmp( &cTAG,"9K" , 2))
		return (TAG_9K_CLIENT_CERTIFICATE);
	if ( 0 == memcmp( &cTAG,"9T" , 2))
		return (TAG_9T_CLIENT_KEY);
	if ( 0 == memcmp( &cTAG,"9W" , 2))
		return (TAG_9W_KEY_MODULE_LENGHT);
	return (TAG_UNKNOWN);
}
static Bool GetTagData( UBYTE cCurrnetTag, FILE *f )
{
	UWORD lenght;
	UBYTE * pTagData;
	if ( 2 != fread(&lenght, 1, 2, f) )
		return False;
	pTagData = malloc (lenght);
	if ( lenght != fread( pTagData, 1, lenght, f) )
	{
		free( pTagData);
		return False;
	}
	switch (cCurrnetTag)
	{
	case TAG_9M_PRIVATE_KEY_MODE:
		CRTTAB.KEYFORMAT = *pTagData;
		CRTTAB.PKMKID = *(pTagData+1);
		CRTTAB.CRTOPT1 = *(pTagData+2);
		break;
	case TAG_9K_CLIENT_CERTIFICATE:
		CRTTAB.CERTLEN = lenght;
		memcpy(&CRTTAB.CERT, pTagData, lenght);
		break;
	case TAG_9T_CLIENT_KEY:
		CRTTAB.KEYLEN = lenght;
		memcpy(&CRTTAB.KEY, pTagData, lenght);
		break;
	case TAG_9W_KEY_MODULE_LENGHT:
		memcpy( &CRTTAB.KEYMODULE, pTagData, 2);
	case TAG_UNKNOWN:
		// Just skip unknow tag
		break;
	}
	free (pTagData);
	return True;
	
}

//=============================================================================
// Private function definitions
//=============================================================================
//-----------------------------------------------------------------------------
// Name:           UnpackPemData
// Description:    Read "pem" file by sections and call sections data store
//                 routines
// Parameters:     fname  - file name to proceed
//                 
// Return Value:   None
// Notes:          Don't check if one or several sections received.
//-----------------------------------------------------------------------------
static void UnpackPemData( const char* fname )
{
	UBYTE* 		TempBuf;
	UWORD		Len;
	PEM_STYPE	sType;

	// Allocate temporary data buffer
	TempBuf = malloc( MaxSectionDataSize );

	// Open pem file
	fp = fopen( fname, "rb" );

	if ( fp != NULL )
	{
		// Proceed received file by sections
		while( 0 != (Len = GetNextSection( fp, TempBuf, &sType )) )
		{
			// Check if section too big
			if ( Len == 1 )
				// Read next section
				continue;

			// If retrieved successefully
			if ( Len > 3 )
			{
				// If found
				switch ( sType )
				{
					case KF_PKCS1_PLAIN:
					case KF_PKCS8_PLAIN:
					case KF_PKCS8_ENCRYPTED:
						// Proseed store routine
						StoreRSAKey(  TempBuf, Len, sType );

						// Exit from loop
						break;

					case CF_X509:
						// Proseed store routine
						StoreCertificate(  TempBuf, Len );

						// Exit from loop
						break;

					default:	// Unknown format
						// Search next section
						break;
				}
			}
		}
	}

	// Release memory
	free( TempBuf );

	// Close pem file
	fclose( fp );
}

//-----------------------------------------------------------------------------
// Name:           GetNextSection
// Description:    Find out any section in the file and store section type
//                 and data
// Parameters:     f     - pointer to opened file where to search
//                 pDest - destination buffer pointer to store section data
//                 pType - where to store type of section
// Return Value:   0    - EOF or Unrecoverable error 
//                 1    - Length of section exceed terminal limit
//                 2,3  - RFU
//                 4... - Length of section data
// Notes:          Section data length must be divisible by 4 since it is
//                 encoded in Base64
//-----------------------------------------------------------------------------
static UWORD GetNextSection( FILE *f, UBYTE* pDest, PEM_STYPE* pType )
{
	const pem_section* ptr;
	UBYTE	len;
	UWORD	retval = 0;

	while( True )
	{
		// Read next string from file
		if ( NULL == fgets( tmpstr, sizeof( tmpstr ), f ) )
		{
			// EOF or read error
			return( 0 );
		}

		// Check if start section string was found
		if ( 0 == memcmp( tmpstr, SECTION_START, sizeof(SECTION_START) - 1 ) )
		{
			// Check the section type found
			for( ptr = &SectionConsts[0]; ptr->begstr; ptr++ )
			{
				if ( 0 == memcmp( ptr->begstr,
							tmpstr,
							strlen(ptr->begstr) ) )
					break;
			}

			// Check if known
			if ( NULL == ptr->begstr )
				// Unknown section
				return( 0 );

			// Store section type
			*pType = ptr->type;

			// Jump to analize section data
			break;
		}
	}

	// Read section data into temporary buffer and calculate the length
	while (True)
	{
		// Read next string from file
		if ( NULL == fgets( tmpstr, sizeof( tmpstr ), f ) )
		{
			// EOF or read error
			return( 0 );
		}

		// Check if it was end of section string
		if ( 0 == memcmp( tmpstr, ptr->endstr, strlen(ptr->endstr) ) )
		{
			// Normal return
			return( retval );
		}

		// Get readed string length
		len = StrLn( tmpstr, sizeof(tmpstr) );

		// File string may be finished with 10 or 13,10 sequence.
		if ( tmpstr[len-1] == 10 )
		{
			// Delete last character
			len--;
			
			// Check for 13,10 sequence
			if ( tmpstr[len-1] == 13 ) len--;
		}

		// Check for destination bufer overflow
		if ( MaxSectionDataSize > (retval + len) )
		{
			// Store data
			memcpy( pDest, tmpstr, len );

			// update pointer and counter
			pDest += len;
			retval += len;
		}
		else
			// Recoverable parce error
			return( 1 );
	}
}

//-----------------------------------------------------------------------------
// Name:           StoreRSAKey
// Description:    Unpack Base64 encoded RSA privat key data, get key modulus
//                 length and store all data into certificate table
// Parameters:     Idx  - Number of Certificate table record where to store data
//                 pSrc - Pointer to the source buffer.
//                 Len  - Source data length.
//                 Type - RSA key format supported by OS.
// Return Value:   Non Zero - Unpacked key modulus length.
//                 Zero     - Unpack error. Input data is not Base 64 encoded
//                            or key format error or RSA encryption error.
// Notes:          Note that Type should be the same as used in Crypto
//                 library and service. Master Key should be loaded for the
//                 KF_PKCS8_ENCRYPTED key format with known key index.
//-----------------------------------------------------------------------------
static UWORD StoreRSAKey(  UBYTE* pSrc, WORD Len, PEM_STYPE Type )
{
	UBYTE*	pTempData;
	UWORD	retval;
	DWORD	reslen = 0;

	// Decode and store privat key data
	CRTTAB.KEYLEN = Base64Decode( CRTTAB.KEY, pSrc, Len );

	if ( CRTTAB.KEYLEN != 0 )
	{
		// Store key format
		CRTTAB.KEYFORMAT = Type;
		CRTTAB.PKMKID -= '0';
		if ( ( KF_PKCS8_ENCRYPTED == CRTTAB.KEYFORMAT ) && ( CRTTAB.CRTOPT1 & CRT_FIXEDKEY ) )
			CRTTAB.KEYFORMAT = KF_PKCS8_ENCFIXEDKEY;


		// Allocate temporary buffer (max 2048 bit)
		pTempData = malloc( 2048 / 8);

		// Encrypt some data and get the result len which is equal to key modulus
		switch ( CRTTAB.KEYFORMAT )
		{
			case KF_PKCS1_PLAIN:
			case KF_PKCS8_PLAIN:
				// RSA encryption using clear key
				retval = C_RsaEncrypt_Ext( "0", 1, CRTTAB.KEY, CRTTAB.KEYFORMAT,
										NULL, pTempData, &reslen );
				break;
				
			case KF_PKCS8_ENCFIXEDKEY:
					// Encrypted fixed key
					retval = C_RsaEncrypt_Ext("0", 1, CRTTAB.KEY, 
								KF_PKCS8_ENCRYPTED, "TestPassword0001", pTempData, &reslen);
					break;

			case KF_PKCS8_ENCRYPTED:
					// RSA encryption using encypted key
					retval = CS_EncryptByPrivateKey( CRTTAB.PKMKID , CRTTAB.KEY,
										"0", 1, pTempData, &reslen );
				break;

			default:
				// We shouldn't be here
				break;
		}
	
		// Release memory
		free( pTempData );
	
		// Check encryption result
		if ( (retval != RSA_OK) || (reslen == 0) )
		{
			// Error! erase the length!
			CRTTAB.KEYMODULE = 0;
			
			switch( retval )
			{
				case MK_PASSWORD_ERROR:		// master key error
				case RSA_DER_FORMAT_ERROR:	// bad certificate
				case RSA_DATA_WRONG:		// bad certificate
				case RSA_DECRYPT_FAILED:	// decryption error
				default:
					reslen = 0;
					break;
			}
		}
		else
		{
			// Encryption successeful. Save key modulus
			CRTTAB.KEYMODULE = reslen * 8;
		}
	}

	return( reslen );
}

//-----------------------------------------------------------------------------
// Name:           StoreCertificate
// Description:    Unpack Base64 encoded X509 certificate and store into table
// Parameters:     Idx  - Number of Certificate table record where to store data
//                 pSrc - Pointer to the source buffer.
//                 Len  - Source data length.
// Return Value:   Non Zero - Unpacked certificate data length
//                 Zero     - Unpack error. Input data is not Base 64 encoded.
// Notes:          None.
//-----------------------------------------------------------------------------
static UWORD StoreCertificate(  UBYTE* pSrc, WORD Len )
{
	// Decode and store certificate data
	CRTTAB.CERTLEN = Base64Decode( CRTTAB.CERT, pSrc, Len );

	return( CRTTAB.CERTLEN );
}

//-----------------------------------------------------------------------------
// Name:           Base64Decode
// Description:    Decode data stream formatted in Base64 Encoding
// Parameters:     pDest - Destination buffer pointer
//                 pSrc  - Source (Base64 encoded) buffer pointer
//                 Len   - Source buffer length
// Return Value:   Zero     - Source data format error (non Base64 format)
//                 Non Zero - Decoded data length
// Notes:          There is no destination buffer length verification.
//-----------------------------------------------------------------------------
static UWORD Base64Decode( UBYTE* pDest, UBYTE* pSrc, UWORD Len)
{
	unsigned long	bits;
	UBYTE		q,pos;
	UWORD		retval = 0;

	// Encoded data must be formatted in groups
	// by 4 characters represented in the "dictionary"
	while( Len >= 4 )
	{
		// Reset group bit string
		bits = 0;

		// Proceed next 4 characters group
		for( q = 0; q < 4; q++, pSrc++ )
		{
			// Check for padding character
			if ( *pSrc != '=' )
			{
				// Try to find out current symbol in the Base64 "dictionary"
				for( pos = 0; (pos < 64) && (*pSrc != abBase64[pos]); pos++ );

				// Check that symbol was found
				if ( pos < 64 )
				{
					// Yes, format continous bit string
					bits <<= 6;
					bits |= pos;
				}
				else
				{
					// No, return format error
					return( 0 );
				}
			}
			else
			{
				// Padding character can be placed only
				// at the end of stream
				if ( Len == 4 )
				{
					bits <<= 6 * (4 - q);
					break;
				}
				else
					// Format error
					return( 0 );
			}
		}

		// Decode bit string into HEX bytes string
		for( q--; q; q-- )
		{
			*pDest++ = (bits & 0x00ff0000) >> 16;
			bits <<= 8;
			retval++;
		}

		// Get left data length
		Len -= 4;
	}

	return( retval );
}


/*------------------------------------------------------------------
 *  Name:           PrintCorruptCert
 *  Description:    Print a line for  corrupt certificate found
 *  Parameters:     none
 *  Return Value:   none
 *  Notes:		    
 *-----------------------------------------------------------------*/
static void PrintCorruptCert( void )
{
    OS_RETURN_VALS PrinterStatus;

    PrinterStatus = SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ) ;
    if ( PrinterStatus == orvOK )
    {
        // Print a line for corrupt key
        SDK_PrintString ("SSL CERTIFICATE TABLE INVALID, ");

   	    SDK_PrintString( "\n" );
   	    SDK_PrintString( "\n" );
   	    SDK_PrintString( "CALL HELP DESK" );
   	    SDK_PrintString( "\f" );
		    
        /* Wait till printing is done */
		while ( SDK_PrinterBusy( ) && ( 0x0E != TCONF.TCPRINTER ) )	//@ZRS++
        {
	        /* Give up the CPU */
	        SDK_RelinqCPU( );
        }

        /* Close the printer */
        SDK_PrinterClose( );

    } // printer opened ok
    else
    {
        // display printer error
		if ( prt_PAPER_OUT == PrinterStatus )
		{
			PrinterErrorFlags = PaperOutError;
		}
		else if ( prt_OVERHEAT == PrinterStatus )
		{
			PrinterErrorFlags = PrinterOverheatError;
		}
		else
			PrinterErrorFlags = OtherPrintError;
    }

    return;

}

//=============================================================================
// Locally stored CRTTAB														  */
//=============================================================================

struct certificate_rec CRTTAB __init__(={0});	// X509 Certificate table

