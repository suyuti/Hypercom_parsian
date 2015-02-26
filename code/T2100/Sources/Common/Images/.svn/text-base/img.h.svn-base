
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _IMG_H_
#define _IMG_H_
//=============================================================================
//
// Module overview:             img.h
//      Image identifier definitions and functions
//
//=============================================================================

//=============================================================================
// Public defines and typedefs
//=============================================================================
// Image Identifiers
typedef enum
{
	IMAGE_CARD = 1,				// 0001
	IMAGE_BACKARROW,			// 0002 
	IMAGE_FORWARDARROW,			// 0003 
	IMAGE_KEY,					// 0004 
	IMAGE_ENTER,				// 0005 
    IMAGE_NEW,					// 0006 
	IMAGE_BATCH,				// 0007 
	IMAGE_REPORT,				// 0008
	IMAGE_TERMINAL,				// 0009
	IMAGE_SETUP,				// 0010
	IMAGE_FUNCTION,				// 0011
	IMAGE_NOIMAGE,				// 0012
	IMAGE_TEST,					// 0013
	IMAGE_PRINT,				// 0014
	IMAGE_BROWSER,				// 0015
	IMAGE_TAB,					// 0016
	IMAGE_CHECK,				// 0017
	IMAGE_SALE,					// 0018
	IMAGE_REFUND,				// 0019
	IMAGE_OFFLINE,				// 0020
	IMAGE_AUTH,					// 0021
	IMAGE_BALANCE,				// 0022
	IMAGE_CARDS,				// 0023
	IMAGE_MAIL,					// 0024
	IMAGE_TIPS,					// 0025
	IMAGE_SERVER,				// 0026
	IMAGE_BANK,					// 0027
	IMAGE_REVIEW,				// 0028
	IMAGE_TOTAL,				// 0029
	IMAGE_FIND,					// 0030
	IMAGE_ADJUST,				// 0031
	IMAGE_VOID,					// 0032
	IMAGE_AUDIT,				// 0033
	IMAGE_SUMMARY,				// 0034
	IMAGE_TILL,					// 0035
	IMAGE_INITIALIZE,			// 0036
	IMAGE_PROGLOAD,				// 0037
	IMAGE_SHIFT,				// 0038
	IMAGE_EYE,					// 0039
	IMAGE_TRAIN,				// 0040
	IMAGE_RESET,				// 0041
	IMAGE_LOGON,				// 0042
	IMAGE_ICEPAC,				// 0043
	IMAGE_CLOCK,				// 0044
	IMAGE_PHONE,				// 0045
	IMAGE_PWD,					// 0046
	IMAGE_CONTRAST,				// 0047
	IMAGE_HOST,					// 0048
	IMAGE_DISPLAY,				// 0049
	IMAGE_KBD,					// 0050
	IMAGE_CARDREADER,			// 0051
	IMAGE_PINPAD,				// 0052
	IMAGE_INITPHONE,			// 0053
	IMAGE_NMSPHONE,				// 0054
	IMAGE_DIALTYPE,				// 0055
	IMAGE_PABX,					// 0056
	IMAGE_TERMID,				// 0057
	IMAGE_ALL,					// 0058
	IMAGE_RECEIPT,				// 0059
	IMAGE_ONOFF,				// 0060
	IMAGE_QUIT,					// 0061
	IMAGE_ADDRMV,				// 0062
	IMAGE_TXRX,					// 0063
	IMAGE_DB,					// 0064
	IMAGE_CLEAR,				// 0065
	IMAGE_AMEX,					// 0066
	IMAGE_RELEASE,				// 0067
	IMAGE_CLOSE,				// 0068
	IMAGE_OPEN,					// 0069
	IMAGE_ATM,					// 0070
	IMAGE_UP,					// 0071
	IMAGE_CHANGE,				// 0072
	IMAGE_LAST,					// 0073
	IMAGE_INVOICE,				// 0074
	IMAGE_HANDSET,				// 0075
	IMAGE_TOLL,					// 0076
	IMAGE_MEMORY,				// 0077
	IMAGE_REFERRAL,				// 0078
	IMAGE_DETAIL,				// 0079
	IMAGE_UNADJUSTED,			// 0080
	IMAGE_ACHCHECK,				// 0083
	IMAGE_PRIORACHCHECK,		// 0084
	IMAGE_LANGUAGE,				// 0085
	IMAGE_CASH,					// 0086
	IMAGE_SALCASH,				// 0087
	IMAGE_CHKRDR,				// 0088
	IMAGE_UPARROW,				// 0089
	IMAGE_DOWNARROW,			// 0090
    IMAGE_EMPTY,				// 0091
    IMAGE_GSMSIGNAL_0,          // 0092
    IMAGE_GSMSIGNAL_20,         // 0093
    IMAGE_GSMSIGNAL_40,         // 0094
    IMAGE_GSMSIGNAL_60,         // 0095
    IMAGE_GSMSIGNAL_80,         // 0096
    IMAGE_GSMSIGNAL_100,        // 0097
    IMAGE_BATTARY_0,            // 0098
    IMAGE_BATTARY_20,           // 0099
    IMAGE_BATTARY_40,           // 0100
    IMAGE_BATTARY_60,           // 0101 
    IMAGE_BATTARY_80,           // 0102
    IMAGE_BATTARY_100,          // 0103
    IMAGE_KEYASC,               // 0104
    IMAGE_KEYASCSM,             // 0105
    IMAGE_KEYHEX,               // 0106
    IMAGE_KEYNUM,               // 0107
    IMAGE_KEYSYM,               // 0108
	IMAGE_ICCCARD,				// 0109
    IMAGE_BLUETOOTH,            // 0110
		
	// DB tablolarýný formatlý bir þekilde yazdýran rapor için @erd 01.03.2007 
	ACQUIRER_TABLE_VAR_NAMES,			// 0111
	ACQUIRER_TABLE_VAR_TYPE_AND_LENGTH, // 0112
	TERMINAL_TABLE_VAR_NAMES,			// 0113
	TERMINAL_TABLE_VAR_TYPE_AND_LENGTH,	// 0114
	ISSUER_TABLE_VAR_NAMES,				// 0115
	ISSUER_TABLE_VAR_TYPE_AND_LENGTH,	// 0116					
	CARD_TABLE_VAR_NAMES,				// 0117
	CARD_TABLE_VAR_TYPE_AND_LENGTH,		// 0118

	NUM_OF_IMAGES,

	IMG_USER0 = 12000,
	IMG_USER1,
	IMG_USER2,
	IMG_USER3,
	IMG_USER4,
	IMG_USER5,
	IMG_USER6,
	IMG_USER7,
	IMG_USER8,
	IMG_USER9,
	IMG_USER10,
	IMG_USER11,
	IMG_USER12,
	IMG_USER13,
	IMG_USER14,
	IMG_USER15

}
IMAGE_ID;


//=============================================================================
// Public data declarations
//=============================================================================
extern UBYTE const IMG_CARD[];
extern UBYTE const IMG_ICCCARD[];
extern UBYTE const IMG_BACKARROW[];
extern UBYTE const IMG_FORWARDARROW[];
extern const UBYTE IMG_KEY[];
extern const UBYTE IMG_ENTER[];



//=============================================================================
// Public function declarations
//=============================================================================
extern UBYTE *GetImage( IMAGE_ID imageID );


#endif // _IMG_H_
