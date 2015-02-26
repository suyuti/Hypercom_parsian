//=============================================================================
//             Hypercom POS Foundation Classes HPOSFC
//                Copyright (c) 2006 Hypercom Inc
//=============================================================================

#ifndef __x509parser_h
#define __x509parser_h

#ifdef __cplusplus
extern "C" {
#endif

/**
	@addtogroup GroupSSL
	@{
*/

/**
	@brief Algorithm identifiers.
*/
typedef enum
{
	md5WithRSAEncryption = 4,       ///< MD5 with RSA encryption.
	sha1WithRSAEncryption = 5       ///< SHA1 with RSA encryption.
} asn_algident;

/**
	@brief Name identifiers.
*/
typedef enum
{
	commonName = 3,                 ///< Common name.
	countryName = 6,                ///< Country name.
	localityName = 7,               ///< Locality name.
	stateOrProvinceName = 8,        ///< State or province name.
	organizationName = 10,          ///< Name of organization.
	organizationUnitName = 11       ///< Organization unit name.
} asn_nameident;

/**
	@brief ASN1 fields.
*/
typedef struct
{
	int Len;                        ///< Length of field.
	UBYTE *pData;                   ///< Pointer to the data in the field.
} asn_field;

typedef asn_field asn_integer;          ///< Integer stored in the ASN1 field.
typedef asn_field asn_string;           ///< String stored in the ASN1 field.
typedef asn_field asn_bitstring;        ///< Bitstream stored in the ASN1 field.
typedef asn_field asn_UTCTime;          ///< Time stored in the ASN1 field.

/**
	@brief X.509 certificate name fields.
*/
typedef struct
{
	asn_string commonName;          ///< Common name.
	asn_string countryName;         ///< Country name.
	asn_string localityName;        ///< Locality name.
	asn_string stateOrProvinceName; ///< State or province name.
	asn_string organizationName;    ///< Name of organization.
	asn_string organizationUnitName; ///< Organization unit name.
} asn_name;

/**
	@brief X.509 certificate validity dates.
*/
typedef struct
{
	asn_UTCTime NotBefore;          ///< Start date.
	asn_UTCTime NotAfter;           ///< End date.
} asn_validity;

/**
	@brief X.509 certificate public key.
*/
typedef struct
{
	int AlgorithmIdent;             ///< Algorithm identifier.
	asn_bitstring PublicKey;        ///< Public key.
} asn_keyinfo;

/**
	@brief X.509 certificate signature structure.
*/
typedef struct
{
	int AlgorithmIdent;             ///< Algorithm identifier.
	asn_bitstring Signature;        ///< Signature.
} asn_signature;

/**
	@brief X.509 certificate fields.
*/
typedef struct
{
	int Version;                    ///< Version.
	asn_integer SerialNumber;       ///< Serial number.
	asn_name Issuer;                ///< Issuer.
	asn_validity Validity;          ///< Validity dates.
	asn_name Subject;               ///< Subject name.
	asn_keyinfo PublicKeyInfo;      ///< Public key.
	asn_bitstring CertTBS;          ///< Certificate to sign.
	int CertSigType;                ///< Signature type.
	asn_signature CertSign;         ///< Signature.
} X509_Certificate_v1;

/**
	@brief X.509 certificate parser.

	@param pCert Pointer to the certificate data to be parsed.
	@param nCertLen Length of certificate data.
	@param pCertStruct Pointer to the structure to fill up with parsed data.

	@return Zero on success, non-zero otherwise.
*/
int X509Parse( UBYTE *pCert, int nCertLen, X509_Certificate_v1 *pCertStruct );

/**
	@example x509parser.c
	This is an example of how to use the X509Parse() function.
*/

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif // __x509parser_h
