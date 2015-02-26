
///////////////////////////////////////////////////////////////////////////
//
// Module   : CryptoService.h
// Part of  : SSW, EFTOS
// Created  : Allan Ekman
// Abstract : Header file of module CryptoService.
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
// 030922  AE      Original version
// 040713  RZ      Change Cryptographic Service Region
// 050112  RZ      Added old library compatibility
// 051216  RZ      Implemented Dynamic Secure Memory support
//                 Added RSA Private Key encryption
// 060613  RZ      Added several new definitions and functions
//
///////////////////////////////////////////////////////////////////////////

#ifndef CRYPTOSERVICE_H
#define CRYPTOSERVICE_H

#if !defined( __GNUC__ ) && !defined( __attribute__ )
#define __attribute__(A)
#endif

// Definitions for argument "nType" used in function CS_CreateMAC
#define SINGLE_MAC              0
#define LAST_TRIPLE_MAC         1

// Definitions for argunent "nDES"
#define TYPE_CBC                0
#define TYPE_ECB                1
#define TYPE_CFB                2
#define TYPE_OFB                3

// Key types
#define KEY_TYPE_NONE			0x00
#define KEY_TYPE_PIN            0x01
#define KEY_TYPE_DISPLAY        0x02
#define KEY_TYPE_DATA           0x04
#define KEY_TYPE_MAC            0x08
#define KEY_TYPE_KEK			0x10
#define KEY_TYPE_Reserved_0		0x20
#define KEY_TYPE_Reserved_1		0x40
#define KEY_TYPE_TRIPLE			0x80
#define KEY_TYPE_ALL			(KEY_TYPE_PIN + KEY_TYPE_DISPLAY + KEY_TYPE_DATA + KEY_TYPE_MAC + KEY_TYPE_KEK)

// DUKPT mode
#define DOUBLE_KEY_LENGTH       0
#define SINGLE_KEY_LENGTH       1
// DUKPT Keys mask
#define DUKPT_PIN_KEY			0x01
#define DUKPT_ENC_KEY			0x02
#define DUKPT_MAC_OUT_KEY		0x04
#define DUKPT_MAC_IN_KEY		0x08
// DUKPT Key generation type
#define DUKPT_STANDARD			0
#define DUKPT_NORWEGIAN			1

// Memory allocation size (in bytes)
#define CS_PK_SIZE				16		// PK size
#define CS_PK_CRC32_SIZE		4		// PK CRC32 value size
#define CS_SCK_COMM_SIZE		32		// SC&K controller communication buffer size

#define CS_SINGLE_KEY_SIZE		8		// Single length key size
#define CS_DOUBLE_KEY_SIZE		16		// Double length key size
#define CS_TRIPLE_KEY_SIZE		24		// Triple length key size

// Error codes (see also SswDef.h)
#define SCR_BUSY                0x00
#define SPB_LRC_ERROR           0x04
#define SCR_FORMAT_ERROR        0x07
#define COMMAND_ERROR           0x09
#define ADDRESS_ERROR			0x0A
#define SN_INVALID              0x0B
#define BK_PARITY_ERROR         0x0C
#define MK_PARITY_ERROR         0x0D
#define SCR_BUFFER_OVERFLOW     0x0E
#define OTHER_ERROR             0x0F
#define BK_LOADED				0x12
#define KEY_PARITY_ERROR        0x14
#define AUTH_WRONG				0x15
#define DATA_LRC_ERROR			0x18
#define CRC_ERROR               0x1E
#define HW_ERROR				0x3C
#define CHECKSUM_ERROR			0x3F
#define INT_SYSTEM_ERROR		0x41
#define DUKPT_NO_MORE_KEYS      200
#define DUKPT_KEY_ERROR         201
#define DUKPT_KEY_LOADED		202
#define DUKPT_UNKNOWN_TYPE      203
#define WRONG_KEY_TYPE          1001
#define DUKPT_KEY_NOT_GENERATED 1002
#define KVC_ERROR				1003
#define IKSN_ERROR				1004
#define SECURE_MEM_ERROR		1005
#define MK_PASSWORD_ERROR		1006

// SC&K command codes
#define STORE_MASTER_KEY        0x9F
#define RETREIVE_KEY            0xC3
#define DECRYPT_KEY             0x69
#define ENCRYPT_KEY             0xC0

// Number of keys
#define MAX_MK_KEYS				12
#define MAX_DUKPT_KEYS			3

// Max derivate DUPKT keys
#define MAX_DUKPT_KEY           0x1FF800

// Key Verification Code Mask
#define KVC_MASK				0xFFFFFF00



// Type definition
typedef void (FAR_FUNC *ECB_3DESPTR)(const BYTE*, const BYTE*, DWORD, BYTE*);
typedef void (FAR_FUNC *ALL_3DESPTR)(const BYTE*, const BYTE*, const BYTE*, DWORD, BYTE*);

// Master Key description
// The size of this structure must divide to 8
typedef struct
{
	BYTE  pMK[CS_DOUBLE_KEY_SIZE];	// Master key, [MK]PK
	DWORD nType;					// Master key type
} MK_KEY;

// DUKPT key structure
typedef struct
{
    BYTE key[CS_DOUBLE_KEY_SIZE];	// Key buffer
} FR_RECORD;

// DUKPT future key registers structure 
typedef struct
{
    FR_RECORD fr[21];   // Future Registers
    DWORD ec;           // Encryption Counter
} FR_TYPE;

// DUKPT key description
// The size of this structure must divide to 8
typedef struct
{
	BYTE	IKSN[8];	// [in] IK Serial Number
	BYTE	KSN[10];	// [out] Key Serial Number
	FR_TYPE fkr;		// Future key Registers [FKR]PK
	DWORD	nMode;		// DUPKT mode, double or single
} DUKPT_KEY;

// CryptoService Region
// The size of this structure must divide to 8
typedef struct
{
	BOOL		bKeyStoreInit;      		// Key store initiated or not
	BOOL		bPKGenerated;       		// PK generated or not
	BYTE		pBKLK[CS_DOUBLE_KEY_SIZE];	// [BKLK]BK, Init MK
	BYTE 		pPK[CS_DOUBLE_KEY_SIZE];	// [PK]BKLK
	MK_KEY		mk[MAX_MK_KEYS];			// Master key structure
	DUKPT_KEY	dk[MAX_DUKPT_KEYS];			// DUKPT key sructure
    DWORD 		nChksum;					// CRC32 checksum   
} CS_REGION;

// Size of Cryptographic Region for checking by CRC32
#define CS_REGION_SIZE	(sizeof(CS_REGION)-sizeof(DWORD))

// DUKPT Control Structure
// IK encrypted by PK
typedef struct
{
	BYTE	pEIK[MAX_DUKPT_KEYS][CS_DOUBLE_KEY_SIZE];	// DUKPT IK encrypted by PK
	DWORD	nCRC32;
} CS_DUKPT_CTRL;
// Size of DUKPT Control Structure for checking by CRC32
#define CS_DUKPT_CTRL_SIZE	(sizeof(CS_DUKPT_CTRL)-sizeof(DWORD))

// Master Key Extension Structure
// Contain third part of [MK]PK
typedef struct
{
	BYTE	pExtMK[MAX_MK_KEYS][CS_SINGLE_KEY_SIZE];	// Third part of [MK]PK
	DWORD	nCRC32;
} CS_MK_EXT;
// Size of Master Key Extension Structure for checking by CRC32
#define CS_MK_EXT_SIZE	(sizeof(CS_MK_EXT)-sizeof(DWORD))
// Master Key operate strucrute
typedef struct
{
	BYTE  pMK[CS_TRIPLE_KEY_SIZE];	// Master key, [MK]PK
	DWORD nType;					// Master key type
} MK_TRIPLE_KEY;


// Session DUKPT Key
typedef struct
{
	BYTE	pCS_PIN   [CS_DOUBLE_KEY_SIZE];	// PIN Key DUKPT
	BYTE	pCS_ENC   [CS_DOUBLE_KEY_SIZE];	// Data Encryption/Decryption Key DUKPT
	BYTE	pCS_MACOUT[CS_DOUBLE_KEY_SIZE];	// MAC OUT Key DUKPT
	BYTE	pCS_MACIN [CS_DOUBLE_KEY_SIZE];	// MAC IN Key DUKPT
	BYTE	pCS_KSN[10];					// KSN for current DUKPT keys
	DWORD	nCS_KCV;						// KVC for current DUKPT keys
	DWORD	nCS_Mask;						// Session keys usage mask
	DWORD	nCS_Mode;						// DUPKT mode, double or single
} SK_DUKPT;

// Local Certificate
typedef struct
{
	DWORD	nExp;			// Local Exponent
	BYTE*	pPK;			// Pointer to Public Key
	DWORD	nPKLen;			// Public Key length
	BYTE*	pPKSign;		// Pointer Public Key signature
	DWORD	nPKSLen;		// Public Key Signature length
} CERT_LOCALE;

// Function prototypes (with Key Verification Procedure)
DWORD CS_KVP_CreateMAC          (DWORD nKeyIndex, BYTE* pMod, BYTE* pSMK, BYTE* pMAC, BYTE* pData, DWORD nLen, DWORD nType);
DWORD CS_KVP_EncryptPIN         (DWORD nKeyIndex, BYTE* pMod, BYTE* pSPEK, BYTE* pCipher, BYTE* pPIN);
DWORD CS_KVP_GenerateSPEK       (DWORD nKeyIndex, BYTE* pMod, BYTE* pKey, BYTE* pIV);
DWORD CS_KVP_DecryptCB          (DWORD nKeyIndex, BYTE* pMod, BYTE* pSPEK, BYTE* pIV, BYTE* pPlainTextPIN, BYTE* pEncryptedPIN);
DWORD CS_KVP_GenerateDUKPT      (DWORD nKeyIndex, BYTE* pMod, DWORD nMode);
DWORD CS_KVP_EncryptPIN_DUKPT   (DWORD nKeyIndex, BYTE* pMod, BYTE* pCipher, BYTE* pPIN);
DWORD CS_KVP_StoreEMK           (DWORD nKeyIndex, BYTE* pMod, BYTE* pKey);
DWORD CS_KVP_Load_EMKn_MKm      (DWORD nKeyIndex, BYTE* pMod, DWORD nNewIndex, BYTE* pEMK);
DWORD CS_KVP_Load_EMKn_MKm_TMOD (DWORD nKeyIndex, BYTE* pMod, BYTE* pTMOD, DWORD nNewIndex, BYTE* pEMK);
DWORD CS_KVP_Load_EIKn_MKm      (DWORD nIndexMK,  BYTE* pMod, DWORD nIndexDUKPT, BYTE* pEIK, BYTE* pIKSN, DWORD nMode);
DWORD CS_KVP_ReEncryptKey       (DWORD nKeyIndex, BYTE* pMod, BYTE* pEMK, BYTE* pEK, DWORD nEKLen, BYTE* pREK);
DWORD CS_KVP_TestMK             (DWORD nKeyIndex, BYTE* pMod, BYTE* pEK, BYTE* pCipher);
DWORD CS_KVP_CheckKeyParity     (DWORD nKeyIndex, BYTE* pMod, BYTE* pEK);
DWORD CS_KVP_GetKeyCheckValue   (DWORD nKeyIndex, BYTE* pMod, BYTE* pEK, BYTE* pData, BYTE* pCipher);
DWORD CS_KVP_EncryptData        (DWORD nKeyIndex, BYTE* pMod, BYTE* pSK, BYTE* pIV, BYTE* pResult, BYTE* pData, DWORD nLen, DWORD nType);
DWORD CS_KVP_DecryptData        (DWORD nKeyIndex, BYTE* pMod, BYTE* pSK, BYTE* pIV, BYTE* pResult, BYTE* pData, DWORD nLen, DWORD nType);
DWORD CS_KVP_EncryptMsg         (DWORD nKeyIndex, BYTE* pMod, BYTE* pSK, BYTE* pIV, BYTE* pResult, BYTE* pData, DWORD nLen, DWORD nType);
DWORD CS_KVP_DecryptMsg         (DWORD nKeyIndex, BYTE* pMod, BYTE* pSK, BYTE* pIV, BYTE* pResult, BYTE* pData, DWORD nLen, DWORD nType);
DWORD CS_KVP_DataDUKPT          (DWORD nKeyIndex, BYTE* pMod, BYTE* pIV, BYTE* pResult, BYTE* pKSN, BYTE* pData, DWORD nLen, BOOL bMode, DWORD nType);
DWORD CS_KVP_EncryptByPrivateKey(DWORD nKeyIndex, BYTE* pMod, const BYTE* pPrivateKey, const BYTE* pData, DWORD nDataLen, BYTE* pResult, DWORD *pResultLen);
DWORD CS_KVP_MAC_DUKPT			(DWORD nKeyIndex, BYTE* pMod, BYTE* pMAC, BYTE* pData, DWORD nLen, DWORD nType, DWORD nMask);
#define CS_KVP_GenerateKey_DUKPT(nKeyIndex, pMod)												CS_KVP_GenerateDUKPT(nKeyIndex, pMod, DUKPT_STANDARD)
#define CS_KVP_CreateMAC_DUKPT(nKeyIndex, pMod, pMAC, pData, nLen, nType)						CS_KVP_MAC_DUKPT(nKeyIndex, pMod, pMAC, pData, nLen, nType, DUKPT_MAC_OUT_KEY)
#define CS_KVP_VerifyMAC_DUKPT(nKeyIndex, pMod, pMAC, pData, nLen, nType)						CS_KVP_MAC_DUKPT(nKeyIndex, pMod, pMAC, pData, nLen, nType, DUKPT_MAC_IN_KEY)
// Function prototypes (ordinary)
#define CS_CreateMAC(nKeyIndex, pSMK, pMAC, pData, nLen, nType)									CS_KVP_CreateMAC(nKeyIndex, NULL, pSMK, pMAC, pData, nLen, nType)
#define CS_EncryptPIN(nKeyIndex, pSPEK, pCipher, pPIN)											CS_KVP_EncryptPIN(nKeyIndex, NULL, pSPEK, pCipher, pPIN)
#define CS_DecryptPIN(nKeyIndex, pSPEK, pPlainTextPIN, pEncryptedPIN)							CS_KVP_DecryptCB(nKeyIndex, NULL, pSPEK, NULL, pPlainTextPIN, pEncryptedPIN)
#define CS_GenerateKey(nKeyIndex, pKey)															CS_KVP_GenerateSPEK(nKeyIndex, NULL, pKey, NULL)
#define CS_EncryptByPrivateKey(nKeyIndex, pPrivateKey, pData, nDataLen, pResult, pResultLen)	CS_KVP_EncryptByPrivateKey(nKeyIndex, NULL, pPrivateKey, pData, nDataLen, pResult, pResultLen)
#define CS_GenerateKey_DUKPT(nKeyIndex)															CS_KVP_GenerateDUKPT(nKeyIndex, NULL, DUKPT_STANDARD)
#define CS_GenerateDUKPT(nKeyIndex, nMode)														CS_KVP_GenerateDUKPT(nKeyIndex, NULL, nMode)
#define CS_CreateMAC_DUKPT(nKeyIndex, pMAC, pData, nLen, nType)									CS_KVP_MAC_DUKPT(nKeyIndex, NULL, pMAC, pData, nLen, nType, DUKPT_MAC_OUT_KEY)
#define CS_VerifyMAC_DUKPT(nKeyIndex, pMAC, pData, nLen, nType)									CS_KVP_MAC_DUKPT(nKeyIndex, NULL, pMAC, pData, nLen, nType, DUKPT_MAC_IN_KEY)
#define CS_EncryptPIN_DUKPT(nKeyIndex, pCipher, pPIN)											CS_KVP_EncryptPIN_DUKPT(nKeyIndex, NULL, pCipher, pPIN)
#define CS_DataDUKPT(nKeyIndex, pIV, pResult, pKSN, pData, nLen, bMode, nType)					CS_KVP_DataDUKPT(nKeyIndex, NULL, pIV, pResult, pKSN, pData, nLen, bMode, nType)
// Store key functions
DWORD CS_InitDUKPT              (DWORD nKeyIndex, BYTE* pIK, BYTE* pIKSN, DWORD nMode);
DWORD CS_StoreMK                (DWORD nKeyIndex, BYTE* pKey);
DWORD CS_Load_EMKn_BKLK         (DWORD nKeyIndex, BYTE* pEMK);
DWORD CS_Load_EIKn_BKLK         (DWORD nKeyIndex, BYTE* pEIK, BYTE* pIKSN, DWORD nMode);
#define CS_StoreEncryptedMK(nKeyIndex, pKey)													CS_KVP_StoreEMK(nKeyIndex, NULL, pKey)
#define CS_ReEncryptKey(nKeyIndex, pEMK, pEK, nEKLen, pREK)										CS_KVP_ReEncryptKey(nKeyIndex, NULL, pEMK, pEK, nEKLen, pREK)
#define CS_Load_EMKn_MKm(nKeyIndex, nNewIndex, pEMK)											CS_KVP_Load_EMKn_MKm(nKeyIndex, NULL, nNewIndex, pEMK)
#define CS_Load_EMKn_MKm_TMOD(nKeyIndex, pTMOD, nNewIndex, pEMK)								CS_KVP_Load_EMKn_MKm_TMOD(nKeyIndex, NULL, pTMOD, nNewIndex, pEMK)
#define CS_Load_EIKn_MKm(nIndexMK, nIndexDUKPT, pEIK, pIKSN, nMode)								CS_KVP_Load_EIKn_MKm(nIndexMK, NULL, nIndexDUKPT, pEIK, pIKSN, nMode)
// Test key functions
#define CS_TestMK(nKeyIndex, pEK, pCipher)														CS_KVP_TestMK(nKeyIndex, NULL, pEK, pCipher)
#define CS_CheckKeyParity(nKeyIndex, pEK)														CS_KVP_CheckKeyParity(nKeyIndex, NULL, pEK)
#define CS_GetKeyCheckValue(nKeyIndex, pEK, pData, pCipher)										CS_KVP_GetKeyCheckValue(nKeyIndex, NULL, pEK, pData, pCipher)
DWORD CS_BKLK_KCV               (BYTE* pData, BYTE* pCipher);
// Common functions
void  CS_Open                (BOOL bVirginStart);
BOOL  CS_ChkTripleKeyMode    (void);
void  CS_EnableTripleKeyMode (void);
void  CS_DisableTripleKeyMode(void);
DWORD CS_GetSN               (BYTE* pSn, DWORD nMaxLen);
BOOL  CS_StatusSSM           (void);
// Signature verification functions
BOOL  CS_Verify     (BYTE* pStart, DWORD nLen, BYTE* pSignature);
BOOL  CS_VerifyLocal(CERT_LOCALE* pCertificate, BYTE* pData, DWORD nDataLen, BYTE* pDataSign);
// SC&K firmware update functions
DWORD CS_StatusSCK (BYTE* pName);
DWORD CS_ReloadSCK (void);
DWORD CS_EraseSCK  (void);
DWORD CS_LoadSCK   (DWORD nAddr, BYTE* pData, DWORD nLen);
DWORD CS_EndLoadSCK(BYTE* pAuth);

#endif
