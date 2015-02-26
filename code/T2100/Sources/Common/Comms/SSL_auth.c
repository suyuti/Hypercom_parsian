//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             SSL_auth.c
//      SSL server authentication

//=============================================================================
//=============================================================================
#include "Eftos.h"
#include "CryptoLib.h"
#include "basictyp.h"
#include "clib.h"

#include "struct.h"
#include "appdata.h"
#include "fixdata.h"
#include "sdkio.h"
#include "x509parser.h"



//=============================================================================
// External variables / declarations
//=============================================================================
extern int SSL_authentication ( void* ptr );


//=============================================================================
// Private defines and typedefs
//=============================================================================
#define S_MODMAX	256	// Maximum length of Public Key Modulus
#define S_DIGEST        20	// Length of digist
#define S_RSARES	S_MODMAX

struct CA_Const
{
	UBYTE	PublicKeyModulos[S_MODMAX];	// Public Key Modulus
	DWORD	PublicKeyModLen;		// Public Key Modulus length
	DWORD	PublicKeyExp;			// Public Key Exponent
	char*	CommonName;			// Issuer Common Name
};

struct SubjectConst
{
	char*	AqName;				// Acquirer name
	char*	CommonName;			// Subject common name
};

struct Sertificat_rec				
{
	unsigned char Srt_FragType;		// Fragment type
	unsigned char Srt_FragLen[3];		// Fragment length
	unsigned char Srt_ChainLen[3];		// Length of sertificats chain
	unsigned char Srt_SertLen[3];		// Length of sertificat
	unsigned char Srt_Sert[];		// Certificate data
};


//=============================================================================
// Private function declarations
//=============================================================================
Bool SignVerif(struct CA_Const * pCurrentKey );


//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
// Subject identifiers trusted for the selected acquirer name
// (currently Common Name only)
static const struct SubjectConst SbjTab1[] =
{
	{"EUROCONEX",
	 "Peter Novikov", },
	{NULL, NULL},
};

// Table of Certification Authority Public keys supported by terminal
static const struct CA_Const KeyTab[] =
{
	{
	 {
		0xc6,0xe1,0x68,0xef,0xda,0x15,0xac,0x0a,0x27,0x7f,0x28,0xcf,0x36,0x9d,
      0x2f,0xf3,0x16,0x51,0x16,0xa0,0xe9,0x6d,0xd0,0xfd,0xd9,0xe7,0x44,0x7f,0xfa,
      0x74,0x14,0x1f,0xcb,0x4e,0x87,0xc7,0xb6,0x50,0xf5,0x91,0x80,0x76,0x77,0x3b,
      0x0f,0x99,0xc1,0xbd,0xaf,0x26,0x0f,0xb3,0x3a,0xfd,0x37,0x00,0x7d,0xe0,0xad,
      0x23,0xf6,0xbb,0x13,0x4c,0x75,0xc1,0xc5,0x66,0x7f,0x13,0xeb,0x1f,0x69,0x9f,
      0x17,0x67,0x92,0xb0,0x85,0x79,0x91,0x0a,0xe1,0x58,0x38,0x06,0xff,0x8a,0x7a,
      0x44,0x82,0x74,0x4f,0x4e,0x63,0x36,0x19,0x3d,0x8a,0xcc,0x7c,0x34,0xd8,0x65,
      0xd7,0xf3,0xa3,0x7d,0x43,0x0b,0x3a,0x48,0xe0,0x6a,0x75,0xb9,0x73,0x79,0x2a,
      0x7f,0x54,0x60,0x17,0x79,0x4d,0x91,0x25,0xf2,0x4d,0x78,0x09,0xca,0x5e,0x8b,
      0xec,0x19,0xb1,0x76,0x69,0x38,0x1a,0x64,0xbc,0x4e,0xf1,0x21,0x2b,0x69,0x9f,
      0x91,0x4e,0x54,0x8c,0x55,0x0e,0xb6,0x68,0x06,0x40,0xba,0xb0,0x93,0x4b,0xbe,
      0x06,0x94,0x84,0x10,0xd1,0xa5,0x76,0xfb,0xb5,0x99,0x69,0x64,0x90,0x0a,0x46,
      0x87,0x54,0xb5,0xef,0xc0,0x0d,0x7c,0x42,0x6d,0x3c,0xde,0x00,0x2a,0x6e,0x91,
      0x7a,0xab,0x52,0x8d,0x26,0xf3,0x20,0xc9,0xc7,0x02,0x41,0x7b,0xad,0x3d,0xa4,
      0x0a,0x6f,0x7c,0xdb,0x29,0x75,0x4c,0x5e,0x4f,0x35,0x20,0x4d,0x34,0xfe,0x06,
      0x45,0x46,0x0f,0x2a,0xb5,0x58,0x8b,0x6d,0x0f,0x86,0xc4,0x34,0x87,0x99,0x51,
      0xb9,0xca,0xcd,0xd0,0x09,0xf6,0x32,0x5e,0x04,0x5c,0xda,0xdd,0x0d,0xd6,0xfa,
      0x83,0xcd 

	 },
	 0x100,
	 0x010001,
	 "X2100 Core Controller CA",
	},
};
#define KEYMAX	(sizeof(KeyTab) / sizeof(struct CA_Const))

static unsigned char		Digest[S_DIGEST];
static UBYTE			RsaResBuff[S_RSARES];
static X509_Certificate_v1	CertificateData;
static char			RtcTime[RTCLEN];


//=============================================================================
// Public function definitions
//=============================================================================
//-----------------------------------------------------------------------------
//  SSL_authentication      Check for certificate
//
//  Parameters:     Pointer to certificate record
//
//  Global Inputs:  None
//
//  Returns:        GOOD STATUS:
//                  0 - authentication passesd successfully or does not required
//
//                  WRONG STATUSES:
//                  1 - wrong RSA algorithm
//                  2 - authentication declined
//                  3 - authentication not performed
//
//  Notes:          SSL authentication for the TNMS transactions currently IS
//                  NOT SUPPORTED, since SSL is not realized on TNMS side and
//                  HSSL is not realizaed in the HSDK.
//-----------------------------------------------------------------------------
extern int SSL_authentication ( void* ptr )
{
	struct Sertificat_rec* pData = (struct Sertificat_rec*)ptr;
	int	TmpLen;
	char * pTmpPtr;
	UBYTE	CAID;

	// Exit if TNMS transaction proceeding
//	if ( TRINP.TRAQPTR == 0 )
//		return( 3 );

	// Calculate certificate data length
	TmpLen = ((pData->Srt_SertLen[0] << 16) + 
	          (pData->Srt_SertLen[1] << 8) + 
	          (pData->Srt_SertLen[2]) );

	// Decode X509 certificate fields (and check certificate data format
	// while decoding)
	if ( 0 == X509Parse( pData->Srt_Sert, TmpLen, &CertificateData ) )
	{
		// Check Validity Date
		SDK_RtcRead( RtcTime );

		// Check date format
		if ( CertificateData.Validity.NotBefore.Len != 13 )
			return( 2 );

		if ( CertificateData.Validity.NotAfter.Len != 13 )
			return( 2 );

		// Check validity except time zone
		if ( 0 > memcmp(RtcTime, CertificateData.Validity.NotBefore.pData, 12) )
			return( 2 );

		if ( 0 < memcmp(RtcTime, CertificateData.Validity.NotAfter.pData, 12) )
			return( 2 );

		// Check if Certificateion Authority supported
		for( CAID = 0; CAID < KEYMAX; CAID++ )
		{
			// Exit from loop if CA is in list
			if( (0 == memcmp( KeyTab[CAID].CommonName,
			                 CertificateData.Issuer.commonName.pData,
			                 CertificateData.Issuer.commonName.Len )) && 
				( CertificateData.Issuer.commonName.Len == strlen(KeyTab[CAID].CommonName)  )	)
			{
				// Validate certificate signature
				if ( True == SignVerif( &KeyTab[CAID] ))
					break;
			}
		}

		// Exit if CA is not supported by application
		if( CAID >= KEYMAX )
			return( 2 );



		// Signature verification passed, certificate data is valid
		// Validate Common Name of certificate 
		switch (TRINP.TRKEY)
		{
		case INITIALIZE:
			if( 0 == memcmp( CertificateData.Subject.organizationUnitName.pData, "TermMaster",
			                 CertificateData.Subject.organizationUnitName.Len ) )
				return (0);
			else 
				return (2);
		case CLBATCH:
		case CLBATCHA:
		case CLBATCHE:
		case RUPLOAD:
			if ( TRINP.TRAQPTR == 0 )
				return( 2 );
			if (0 == (TRINP.TRAQPTR->AQOPT1 & AQ1_COMMNAME_SETTLE) )
				return ( 0 );

			// check lenght for common name
			pTmpPtr = memchr(TRINP.TRAQPTR->AQNAME, ' ', S_AQNAME);
			if ( pTmpPtr == 0)
				TmpLen = S_AQNAME;
			else
				TmpLen = pTmpPtr - (char *)TRINP.TRAQPTR->AQNAME;

			if ( CertificateData.Subject.commonName.Len != TmpLen) 
				// BAD lenght
				return ( 2 );

			if( 0 == memcmp( TRINP.TRAQPTR->AQNAME,
			                 CertificateData.Subject.commonName.pData,
			                 CertificateData.Subject.commonName.Len )  )
				// Common Name is trusted
				// Verification succesefful
				return( 0 );
			else 
				return ( 2 );

		default:

			if ( TRINP.TRAQPTR == 0 )
				return( 2 );
			if (0 == (TRINP.TRAQPTR->AQOPT1 & AQ1_COMMNAME_TRAN ) )
				return ( 0 );

			// check lenght for common name
			pTmpPtr = memchr(TRINP.TRAQPTR->AQNAME, ' ', S_AQNAME);
			if ( pTmpPtr == 0)
				TmpLen = S_AQNAME;
			else
				TmpLen = pTmpPtr - (char *)(TRINP.TRAQPTR->AQNAME);

			if ( CertificateData.Subject.commonName.Len != TmpLen) 
				// BAD lenght
				return ( 2 );

			if( 0 == memcmp( TRINP.TRAQPTR->AQNAME,
			                 CertificateData.Subject.commonName.pData,
			                 CertificateData.Subject.commonName.Len )  )
				// Common Name is trusted
				// Verification succesefful
				return( 0 );
			else 
				return ( 2 );

		}

		// Get Trusted table pointer for selected CA
/*		pTrusted = KeyTab[CAID].pSbjTab;

		// Check if received Common Name is trusted for selected acquirer
		while( pTrusted->AqName != NULL )
		{
			// Check acquirer name
			if( 0 == memcmp( TRINP.TRAQPTR->AQNAME, pTrusted->AqName,
			                 strlen(pTrusted->AqName) ) )
			{
				// Validate common name for selected acquirer
				if( 0 == memcmp( pTrusted->CommonName,
				                 CertificateData.Subject.commonName.pData,
				                 CertificateData.Subject.commonName.Len )  )
					// Common Name is trusted
					// Verification succesefful
					return( 0 );
			}

			// Point next record
			pTrusted++;
		}
*/
	}

	return( 2 );

}

	DWORD	PublicKeyModLen;		// Public Key Modulus length
	DWORD	PublicKeyExp;			// Public Key Exponent

Bool SignVerif(struct CA_Const * pCurrentKey )
{

		// Get signature algorithm
		switch( CertificateData.CertSigType )
		{
			case md5WithRSAEncryption:
				// Calculate digest for input data block
				C_Md5( CertificateData.CertTBS.pData, 
				        CertificateData.CertTBS.Len, Digest );

				// RSA encryption using public key
				C_RsaModExp( CertificateData.CertSign.Signature.pData,
				             CertificateData.CertSign.Signature.Len,
				             pCurrentKey->PublicKeyExp, pCurrentKey->PublicKeyModulos,
					     pCurrentKey->PublicKeyModLen, RsaResBuff );

				// Compare decripted hash with calculated one
				if ( 0 != memcmp( Digest, &RsaResBuff[S_RSARES - 16], 16 ) )
					return( False );

				return (True);

			case sha1WithRSAEncryption:
				// Calculate digest for input data block
				C_Sha1( CertificateData.CertTBS.pData, 
				        CertificateData.CertTBS.Len, Digest );

				// RSA encryption using public key
				C_RsaModExp( CertificateData.CertSign.Signature.pData,
				             CertificateData.CertSign.Signature.Len,
				             pCurrentKey->PublicKeyExp, pCurrentKey->PublicKeyModulos,
					     pCurrentKey->PublicKeyModLen, RsaResBuff );

				// Compare decripted hash with calculated one
				// TODO If lenght of RsaResBuff not equal of public key lenght it should be chenged
				if ( 0 != memcmp( Digest, &RsaResBuff[S_RSARES - 20], 20 ) )
					return( False );

				return (True);

			default:
				return( False );
		}

}
//=============================================================================
// Private function definitions
//=============================================================================

