
///////////////////////////////////////////////////////////////////////////
//
// Module   : CRYPTO
// Part of  : SSW, EFTOS
// Created  : Allan Ekman
// Abstract : Header file of module CRYPTO
//
//         Copyright (C) 2003 Hypercom Financial Terminals AB
//
///////////////////////////////////////////////////////////////////////////
//
// Revision history
// 
// Date    Sign    Change
// ----    ----    ------
//
// 030707  AE      Original version
// 050127  RZ      Added 3DES CFB and OFB functions
// 051201  RZ      Added MD5 & SHA1 functions with dynamic context
// 051212  RZ      Added Security Memory management functions
// 051216  RZ      Added RSA Private Key encryption function
//
///////////////////////////////////////////////////////////////////////////

#ifndef CRYPTOLIB_H
#define CRYPTOLIB_H

// CRC functions
DWORD C_Crc(DWORD nPolynomial, DWORD nCrc, BOOL bMsbFirst, const BYTE* pData, DWORD nLen);
WORD C_Crc16(const BYTE* pData, DWORD nLen);
WORD C_Crc16Next(WORD nCrc, const BYTE* pData, DWORD nLen);
DWORD C_Crc32(const BYTE* pData, DWORD nLen);
DWORD C_Crc32Next(DWORD nCrc, const BYTE* pData, DWORD nLen);
DWORD C_Crc32Final(DWORD nCrc);

// DES core functions
void C_DesKeySchedule(const BYTE* pKey, BYTE* pResult);
void C_DesTripleKeySchedule(const BYTE* pKey, BYTE* pResult);
void C_DesTrueTripleKeySchedule(const BYTE* pKey, BYTE* pResult);
void C_DesDecrypt(const BYTE* pKey, const BYTE* pData, BYTE* pResult);
void C_DesEncrypt(const BYTE* pKey, const BYTE* pData, BYTE* pResult);
void C_DesTripleDecrypt(const BYTE* pKeySchedule, const BYTE* pData, BYTE* pResult);
void C_DesTripleEncrypt(const BYTE* pKeySchedule, const BYTE* pData, BYTE* pResult);
void C_DesTrueTripleDecrypt(const BYTE* pKeySchedule, const BYTE* pData, BYTE* pResult);
void C_DesTrueTripleEncrypt(const BYTE* pKeySchedule, const BYTE* pData, BYTE* pResult);

// DES Single 
void C_DesSingleDecryptECB(const BYTE* pKey, const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesSingleEncryptECB(const BYTE* pKey, const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesSingleDecryptCBC(const BYTE* pKey, const BYTE* pIV, 
                           const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesSingleEncryptCBC(const BYTE* pKey, const BYTE* pIV, 
                           const BYTE* pData, DWORD nLen, BYTE* pResult);

// DES Triple 
void C_DesTripleDecryptECB(const BYTE* pKey, const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTripleEncryptECB(const BYTE* pKey, const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTripleDecryptCBC(const BYTE* pKey, const BYTE* pIV, 
                           const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTripleEncryptCBC(const BYTE* pKey, const BYTE* pIV, 
                           const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTripleDecryptCFB(const BYTE* pKey, const BYTE* pIV, 
                           const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTripleEncryptCFB(const BYTE* pKey, const BYTE* pIV, 
                           const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTripleOFB       (const BYTE* pKey, const BYTE* pIV, 
                           const BYTE* pData, DWORD nLen, BYTE* pResult);

// DES True Triple 
void C_DesTrueTripleDecryptECB(const BYTE* pKey, const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTrueTripleEncryptECB(const BYTE* pKey, const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTrueTripleDecryptCBC(const BYTE* pKey, const BYTE* pIV, 
                               const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTrueTripleEncryptCBC(const BYTE* pKey, const BYTE* pIV, 
                               const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTrueTripleDecryptCFB(const BYTE* pKey, const BYTE* pIV, 
                               const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTrueTripleEncryptCFB(const BYTE* pKey, const BYTE* pIV, 
                               const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_DesTrueTripleOFB       (const BYTE* pKey, const BYTE* pIV, 
                               const BYTE* pData, DWORD nLen, BYTE* pResult);

// DES MAC
void C_MacSingleCBC(const BYTE* pKey, const BYTE* pIV, const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_MacTripleCBC(const BYTE* pKey, const BYTE* pIV, const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_MacLastTripleCBC(const BYTE* pKey, const BYTE* pIV, const BYTE* pData, DWORD nLen, BYTE* pResult);

// MD5 functions
typedef struct
{
	DWORD	nHash[4];
	DWORD	nBits[2];
	BYTE	pTransitBuf[64];
} MD5_CONTEXT;

void C_Md5( const BYTE* pData, DWORD nLen, BYTE* pResult );
void C_Md5Start( MD5_CONTEXT *ctx );
void C_Md5Calc( MD5_CONTEXT *ctx, const BYTE* pData, DWORD nLen );
void C_Md5Last( MD5_CONTEXT *ctx, BYTE* pResult );
// Old function prototypes to keep library compatibility
void C_Md5Begin(void);
void C_Md5Update(const BYTE* pData, DWORD nLen);
void C_Md5Final(BYTE* pResult);


// RSA function
void C_RsaModExp(const BYTE* pData, DWORD nDataLen, DWORD nExponent, 
                 const BYTE* pModulos, DWORD nModulosLen, BYTE* pResult);
DWORD C_RsaEncrypt(const BYTE* pData, DWORD nDataLen, 
                   const BYTE* pRSAPrivateKey, BYTE* pResult, DWORD *pResultLen );
DWORD C_RsaEncrypt_Ext(const BYTE *pData, DWORD nDataLen, 
                       const BYTE *pPrivateKey, DWORD nPrivateKeyFormat, 
                       char* pPassword, BYTE* pResult, DWORD *pResultLen);

// RSA operation error codes:
#define RSA_OK					0
#define RSA_DER_FORMAT_ERROR 	0x10000
#define RSA_DER_PUBEXP_TOOLONG	0x20000
#define RSA_EVEN_MODULUS		0x30000
#define RSA_DATA_WRONG			0x40000
#define RSA_MEM_OVERFLOW		0x50000
#define RSA_INVALID_PARAMS		0x60000
#define RSA_DECRYPT_FAILED		0x70000

// SHA-1 functions
typedef struct
{
	DWORD	nHash[5];
	DWORD	nByteCount;
	DWORD	nTransCount;
	BYTE	pTransBuf[64];
} SHA_CONTEXT;

void C_Sha1(const BYTE* pData, DWORD nLen, BYTE* pResult);
void C_Sha1Start(SHA_CONTEXT *ctx);
void C_Sha1Calc(SHA_CONTEXT *ctx, const BYTE* pData, DWORD nLen);
void C_Sha1Last(SHA_CONTEXT *ctx, BYTE* pResult);
// Old function prototypes to keep library compatibility
void C_Sha1Begin(void);
void C_Sha1Update(const BYTE* pData, DWORD nLen);
void C_Sha1Final(BYTE* pResult);

#endif
