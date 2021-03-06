/*  *****************************************************************        */
/*  *                                                               *        */
/*  *                      Hypercom Inc                             *        */
/*  *                     (c) Copyright 1998                        *        */
/*  *                                                               *        */
/*  *  PRODUCT   :  Hypercom Base Application in C                  *        */
/*  *                                                               *        */
/*  *  COMPONENT :  EMV public keys header file                     *        */
/*  *                                                               *        */
/*  *  FILENAME  :  emv2_pk.h                                       *        */
/*  *                                                               *        */
/*  *  HISTORY   :                                                  *        */
/*  *                                                               *        */
/*  *    DATE         AUTHOR         COMMENT                        *        */
/*  *  --------     ----------     -----------                      *        */
/*  *  11/01/01     A Shuttleworth  1st Draft                       *        */
/*  *  21 may 02    A Shuttleworth  Removed vestigial prototypes    *        */
/*  *                                                               *        */
/*  *****************************************************************        */

#ifndef _EMV2_PH_H_
#define _EMV2_PH_H_


#include "basictyp.h"

/* definitions */
#define KEYMAX 60


/* data type for Atmel access */
enum AtmelRetVal
{
    ATMEL_OK = 0,
    ATMEL_FAIL,
    ATMEL_WRITE_KEY
};

/* Host Table Entry Structure */
struct CAPublicKeyEntry
{
    UBYTE KeyNum;           /* Table index (NOT key index!!)           */
    UBYTE Index;            /* Index                                   */
	UBYTE KeyLen;			/* length of key in bytes                  */
    UBYTE Key[248];         /* Actual Key data                         */
    UBYTE RID[5];           /* Registered Identifier                   */
    UBYTE Exponent;         /* Exponent - 01 used for 65537            */
	UBYTE Hash[20];			/* Hash value of rid, index, key, exponent */
} ;
#define KEYSZE   sizeof(struct CAPublicKeyEntry) 

// structure for holding key expiry dates in a separate table for backwards 
// compatibility with previous termmaster profile
struct KeyExpiryEntry 
{
    UBYTE ExpIndx;
    UBYTE RID[5];           /* Registered Identifier                   */
    UBYTE Index;            /* Index                                   */
    UBYTE ExpiryDate[3];    /* Expiry Date for this key                */
};
#define KEYEXPSZE   sizeof(struct KeyExpiryEntry) 


// structure for moving key through upper memory as an overlay of SC buffers
// used for casting only
struct CAPublicKeyTransport
{
	UBYTE KeyLen;			/* length of key in bytes                  */
    UBYTE Exponent;         /* Exponent - 01 used for 65537            */
    UBYTE Key[248];         /* Actual Key data                         */
} ;

/* Host Table Entry Structure */
struct PublicKeyLookupEntry
{
    UBYTE RID[5];           /* Registered Identifier                   */
    UBYTE Index;            /* Index                                   */
	UBYTE KeyTableIndex;	/* Index into key table                    */
	UBYTE GoodHash;			/* Whether hash valid or not               */
} ;
#define KEYLOOKUPSZE   sizeof(struct PublicKeyLookupEntry) 

struct RevocationEntry
{
    UBYTE cert1_RID[5];			/* Registered Identifier                */
    UBYTE cert1_Index;			/* Index                                */
	UBYTE cert1_SerialNo[3];	/* Serial number of Issuer public key   */
    UBYTE cert2_RID[5];			/* Registered Identifier                */
    UBYTE cert2_Index;			/* Index                                */
	UBYTE cert2_SerialNo[3];	/* Serial number of Issuer public key   */
    UBYTE cert3_RID[5];			/* Registered Identifier                */
    UBYTE cert3_Index;			/* Index                                */
	UBYTE cert3_SerialNo[3];	/* Serial number of Issuer public key   */
    UBYTE cert4_RID[5];			/* Registered Identifier                */
    UBYTE cert4_Index;			/* Index                                */
	UBYTE cert4_SerialNo[3];	/* Serial number of Issuer public key   */
    UBYTE cert5_RID[5];			/* Registered Identifier                */
    UBYTE cert5_Index;			/* Index                                */
	UBYTE cert5_SerialNo[3];	/* Serial number of Issuer public key   */
    UBYTE cert6_RID[5];			/* Registered Identifier                */
    UBYTE cert6_Index;			/* Index                                */
	UBYTE cert6_SerialNo[3];	/* Serial number of Issuer public key   */
    UBYTE cert7_RID[5];			/* Registered Identifier                */
    UBYTE cert7_Index;			/* Index                                */
	UBYTE cert7_SerialNo[3];	/* Serial number of Issuer public key   */
    UBYTE cert8_RID[5];			/* Registered Identifier                */
    UBYTE cert8_Index;			/* Index                                */
	UBYTE cert8_SerialNo[3];	/* Serial number of Issuer public key   */
    UBYTE cert9_RID[5];			/* Registered Identifier                */
    UBYTE cert9_Index;			/* Index                                */
	UBYTE cert9_SerialNo[3];	/* Serial number of Issuer public key   */
    UBYTE cert10_RID[5];		/* Registered Identifier                */
    UBYTE cert10_Index;			/* Index                                */
	UBYTE cert10_SerialNo[3];	/* Serial number of Issuer public key   */
} ;
#define REVSZE   sizeof(struct RevocationEntry) 

/* external declarations */
extern UBYTE PKFMT;
extern struct CAPublicKeyEntry PublicKeyStore[KEYMAX];
extern UBYTE PKLRC;

extern UBYTE RKFMT;
extern struct RevocationEntry RevocationTable;
extern UBYTE RKLRC;

extern UBYTE KEFMT;
extern struct KeyExpiryEntry KeyExpiryTable[KEYMAX];
extern UBYTE KELRC;

/* function calls */
extern Bool EMVPK_IsRevoked( UBYTE *RevKey );
extern UBYTE EMVPK_GetKey( UBYTE *, struct CAPublicKeyTransport *);

#endif // _EMV2_PH_H_
