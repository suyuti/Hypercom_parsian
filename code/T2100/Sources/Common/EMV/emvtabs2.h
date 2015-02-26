#ifndef EMVTABS2_H_
#define EMVTABS2_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:     emvtabs2.h
//  EMV application processing
//
//=============================================================================

extern void InitICCTab ( void );
extern Bool ValICCTab ( void );
extern void ICCTabPre ( void );
extern void ICCTabPost ( void );

extern void InitTAIDTab ( void );
extern Bool ValTAIDTab ( void );
extern void TAIDTabPre ( void );
extern void TAIDTabPost ( void );

extern void P60_LDIR_EMVTabs2 ( void );
extern struct iccrec_rec * FindIcc(UBYTE iss_id);

/*------------------------------------------------------------------         */
/*                                                                           */
/*       This include file is used to declare all structures used            */
/*       by emvtabs2.c                                                       */
/*                                                                           */
/*                                                                           */
/*       ICC TABLE ENTRY                                                     */
/*                                                                           */

#define TAIDMAX		10
#define ICCMAX		ISMAX

#define ICC1_SIGNATURE	0x01   // ALWAYS PRINT AND CHECK SIGNATURE
#define ICC1_TRK2CMP    0x02   // COMPARE TRACK2 EQUIV. DATA & PAN/EXPIRY

#define S_ICCTACDEN	5
#define S_ICCTACONL	5
#define S_ICCTACDEF	5
#define S_ICCTHRESH	2

struct iccrec_rec
{
	UBYTE    ICCNUM;        /* POSITION IN TABLE */
	UBYTE    ICCISID;       /* ISSUER ID */
	UBYTE    ICCOPT1;       /* ICC OPTION 1 */
	UBYTE    ICCOPT2;       /* ICC OPTION 2 */
	UBYTE    ICCOPT3;       /* ICC OPTION 3 */
	UBYTE    ICCTARGPCT;    /* TARGET PERCENTAGE FOR RANDOM SELECTION (bcd) !ORDER ASSUMED! */
	UBYTE    ICCMAXTPCT;    /* MAX TARGET PCT FOR BIASED RAND SELECT  (bcd) */
	UBYTE    ICCTHRESH[S_ICCTHRESH];  /* THRESHOLD VALUE FOR BIASED RAND SELECT (bcd) */
	UBYTE    ICCTACDEN[S_ICCTACDEN];  /* TERMINAL ACTION CODE DENIAL (hex)	       !ORDER ASSUMED! */
	UBYTE    ICCTACONL[S_ICCTACONL];  /* TERMINAL ACTION CODE ONLINE (hex) */
	UBYTE    ICCTACDEF[S_ICCTACDEF];  /* TERMINAL ACTION CODE DEFAULT (hex) */
	UBYTE    ICCTCC;	    /* TRANSACTION CATEGORY CODE (NON EMV!) */
} __attribute__((packed));

#define ICCSZE sizeof(struct iccrec_rec)   /* DEFINE STRUCTURE SIZE */

//
// THE FOLLOWING OPTIONS APPLY TO TERMINAL AID OPTION BYTE 1
//
#define	 TAID1_EXACT  	0x01	// SELECT THIS AID IF EXACT MATCH ONLY
#define	 TAID1_ISSFLOOR	0x02	// USE ISSUER FLOOR LIMIT
#define	 TAID1_SPC_AAC	0x04	// SPECIAL MASTERCARD PROCESSING ARQC -> AAC
#define	 TAID1_AAR_ONL	0x08	// GO ONLINE IF	AAR.
#define	 TAID1_EE 	0x10	// VISA	EASY ENTRY ALLOWED
#define	 TAID1_ADDTAGS 	0x20	// USE ADDITIONAL TAGS TABLE
#define  TAID1_SKIPEMV	0x40	// SKIP EMV PROCESSING FOR THIS AID
//

//--------------------------------------------------------------------
// TERMINAL SUPPORTED AID ENTRY	STRUCTURE
//
#define  APP_DOL_MAX	99		// Maximum permitted size of DOL buffer supported by the application

struct taidrec_rec
{
        UBYTE    TAIDNUM;       	/* POSITION IN TABLE */
	UBYTE    TAIDADTID;	       	/* LINKED ADT ID */
	UBYTE    TAIDNAMELEN;		/* TERMINAL APPLICATION ID LENGTH */
	UBYTE    TAIDNAME[16];		/* TERMINAL APPLICATION ID */
	UBYTE    TAIDOPT1;		    /* OPTION BYTE 1 */
	UBYTE    TAIDFLOOR[2];		/* AID'S FLOOR LIMIT (NOT USED YET) */
	UBYTE    TAIDVER1[2];		/* SUPPORTED AID'S VERSION NUMBER 1 */
	UBYTE    TAIDVER2[2];		/* SUPPORTED AID'S VERSION NUMBER 2 */
	UBYTE    TAIDVER3[2];		/* SUPPORTED AID'S VERSION NUMBER 3 */
	UBYTE    TAIDTDOL[APP_DOL_MAX];		/* TERMINAL DEFAULT TDOL */
	UBYTE    TAIDDDOL[APP_DOL_MAX];		/* TERMINAL DEFAULT DDOL */
	UBYTE    TAIDOFL_APPR[2];	/* OFFLINE APPROVED RESPONSE CODE */
	UBYTE    TAIDOFL_DECL[2];	/* OFFLINE DECLINED RESPONSE CODE */
	UBYTE    TAIDUNAB_APPR[2];	/* UNABLE TO GO ONLINE (OFFLINE APPROVED) RESPONSE CODE */
	UBYTE    TAIDUNAB_DECL[2];	/* UNABLE TO GO ONLINE (OFFLINE DECLINED) RESPONSE CODE */
} __attribute__((packed));

#define TAIDSZE sizeof(struct taidrec_rec)   /* DEFINE STRUCTURE SIZE */

//====================================================================
// ADDITIONAL TAGS TABLE
//

#define	 ADTMAX  10		// MAXIMUM TABLE ENTRIES

struct adtrec_rec
{
	UBYTE	ADTNUM;	/* POSITION IN TABLE */
	UBYTE	ADTID;   	/* ID */
	UBYTE	ADTTAGS[128];	/* ADDITIONAL TAGS */
} __attribute__((packed));

#define ADTSZE sizeof(struct adtrec_rec)   /* DEFINE STRUCTURE SIZE */


/* External Variables */

extern UBYTE ICCFMT;                          /* FORMAT INDICATOR*/
extern struct iccrec_rec ICCTAB[ICCMAX];      /* ICC TABLE */
extern UBYTE ICCLRC;                          /* LRC OF TABLE*/
									  
extern UBYTE TAIDFMT;
extern struct taidrec_rec TAIDTAB[TAIDMAX];   /* TAID TABLE */
extern UBYTE TAIDLRC;

#ifdef SUPP_ADDICCTAGS
extern UBYTE ADTFMT;
extern	ADTRecType ADTData[EMVL2_ADT_MAX];
extern struct adtrec_rec ADTTAB[ADTMAX];   /* ADDITIONAL TAGS TABLE */
extern UBYTE ADTLRC;
#endif

extern struct PublicKeyLookupEntry KeyLookupTable[KEYMAX];


#endif    // end of "ifndef EMVTABS2_H_" 
