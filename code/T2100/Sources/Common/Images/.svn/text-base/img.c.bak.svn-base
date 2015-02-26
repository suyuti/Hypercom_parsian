
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2001
//=============================================================================
//
// Module overview:             img.c
//      Image data
//
//=============================================================================

#include "basictyp.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "utllib.h"
#include "AcqTableVarName.h"
#include "TermTableVarName.h"
#include "IssTableVarName.h"
#include "CardTableVarName.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================

// image table entry defintion
typedef struct
{
	IMAGE_ID imageID;			// image identifier
	const UBYTE *pImage;		// pointer to image
}
IMAGE_ENTRY;


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================

UBYTE const IMG_CARD[] = {
	0x23, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x0D, 0x00, 0x00, 0x1A, 0x00,
	0x7F, 0x00, 0xB0, 0x80, 0xB0, 0x80, 0xB0, 0x80, 0xB0, 0x80,
	0xB0, 0x80, 0xB0, 0x80, 0xB0, 0x80, 0xB0, 0x80, 0xB0, 0x80,
	0xB0, 0x80, 0xB0, 0x80, 0x7F, 0x00
};

UBYTE const IMG_ICCCARD[] = {
	0x1C, 0x00,	// Overall length 28    
	0x00, 0x00,	// 16-bit screen id
	0x0A, 0x00,	// Image width 10
	0x0E, 0x00,	// Image height 14
	0x01,		// Compression type
	0x13, 0x00,	// Data length 19    
	0xBF,		//    7 Mixed pixels
	0xC8,		//    7 Mixed pixels
	0x85,		//    7 Mixed pixels
	0x9C,		//    7 Mixed pixels
	0xD3,		//    7 Mixed pixels
	0xCA,		//    7 Mixed pixels
	0xB9,		//    7 Mixed pixels
	0xA0,		//    7 Mixed pixels
	0x94,		//    7 Mixed pixels
	0x82,		//    7 Mixed pixels
	0xC0,		//    7 Mixed pixels
	0xA8,		//    7 Mixed pixels
	0x85,		//    7 Mixed pixels
	0x07,		//    7 White pixels
	0xD0,		//    7 Mixed pixels
	0x8A,		//    7 Mixed pixels
	0x81,		//    7 Mixed pixels
	0x9F,		//    7 Mixed pixels
	0xE0,		//    7 Mixed pixels
};


//    8
//   18
//  33FFFFFFFFFFFFEEE
// 777FFFFFFFFFFFFEEE
//FFFFFFFFFFFFFFFFEEE
// 777FFFFFFFFFFFFEEE
//  33FFFFFFFFFFFFEEE
//   18
//    8

UBYTE const IMG_BACKARROW[] = {
	0x26, 0x00, 0x00, 0x00, 0x13, 0x00, 0x09, 0x00, 0x00, 0x1B, 0x00,
	0x08, 0x00, 0x00, 0x18, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x7f, 0xff, 0xe0,
	0xff, 0xff, 0xe0, 0x7f, 0xff, 0xe0, 0x3f, 0xff, 0xe0, 0x18, 0x00, 0x00,
	0x08, 0x00, 0x00
};


//..............2.........
//..............33........
//FFFFFFFFFFFFFFFF8.......
//FFFFFFFFFFFFFFFFCC......
//FFFFFFFFFFFFFFFFEEE.....
//FFFFFFFFFFFFFFFFCC.....
//FFFFFFFFFFFFFFFF8.......
//..............33........
//..............2.........

UBYTE const IMG_FORWARDARROW[] = {
	0x26, 0x00, 0x00, 0x00, 0x13, 0x00, 0x09, 0x00, 0x00, 0x1B, 0x00,
	0x00, 0x02, 0x00, 0x00, 0x03, 0x00, 0xff, 0xff, 0x80, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0xe0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0x80, 0x00, 0x03, 0x00,
	0x00, 0x02, 0x00
};

const UBYTE IMG_KEY[] = {
	0x22, 0x00,					// Overall length 34    
	0x00, 0x00,					// 16-bit screen id
	0x10, 0x00,					// Image width 16
	0x0F, 0x00,					// Image height 15
	0x01,						// Compression type
	0x19, 0x00,					// Data length 25    
	0x40, 0x21,					//   33 White pixels
	0xC8,						//    7 Mixed pixels
	0x09,						//    9 White pixels
	0xD0,						//    7 Mixed pixels
	0x09,						//    9 White pixels
	0xE0,						//    7 Mixed pixels
	0x09,						//    9 White pixels
	0xD3,						//    7 Mixed pixels
	0xC0,						//    7 Mixed pixels
	0x92,						//    7 Mixed pixels
	0xC0,						//    7 Mixed pixels
	0x09,						//    9 White pixels
	0xE0,						//    7 Mixed pixels
	0x09,						//    9 White pixels
	0xC4,						//    7 Mixed pixels
	0xA0,						//    7 Mixed pixels
	0x9C,						//    7 Mixed pixels
	0xD0,						//    7 Mixed pixels
	0x0A,						//   10 White pixels
	0xC0,						//    7 Mixed pixels
	0x09,						//    9 White pixels
	0xC0,						//    7 Mixed pixels
	0x09,						//    9 White pixels
	0xC0,						//    7 Mixed pixels
// Trailing white pixels are suppressed
//  0x1D,       //   29 White pixels
};								// 240 Total Pixels defined

const UBYTE IMG_ENTER[] = {
	0x2E, 0x00,					// Overall length 46    
	0x00, 0x00,					// 16-bit screen id
	0x1F, 0x00,					// Image width 31
	0x0F, 0x00,					// Image height 15
	0x01,						// Compression type
	0x25, 0x00,					// Data length 37    
	0x40, 0x5F,					//   95 White pixels
	0xFB,						//    7 Mixed pixels
	0x97,						//    7 Mixed pixels
	0xEF,						//    7 Mixed pixels
	0xBC,						//    7 Mixed pixels
	0x88,						//    7 Mixed pixels
	0xB2,						//    7 Mixed pixels
	0x91,						//    7 Mixed pixels
	0x84,						//    7 Mixed pixels
	0xA1,						//    7 Mixed pixels
	0x87,						//    7 Mixed pixels
	0xA2,						//    7 Mixed pixels
	0x90,						//    7 Mixed pixels
	0xC4,						//    7 Mixed pixels
	0x9E,						//    7 Mixed pixels
	0xD4,						//    7 Mixed pixels
	0xA3,						//    7 Mixed pixels
	0xE8,						//    7 Mixed pixels
	0xC2,						//    7 Mixed pixels
	0x8A,						//    7 Mixed pixels
	0xC4,						//    7 Mixed pixels
	0xA1,						//    7 Mixed pixels
	0xF0,						//    7 Mixed pixels
	0xA1,						//    7 Mixed pixels
	0xB8,						//    7 Mixed pixels
	0xC4,						//    7 Mixed pixels
	0x91,						//    7 Mixed pixels
	0x84,						//    7 Mixed pixels
	0x93,						//    7 Mixed pixels
	0x88,						//    7 Mixed pixels
	0xC2,						//    7 Mixed pixels
	0x90,						//    7 Mixed pixels
	0xFA,						//    7 Mixed pixels
	0xB1,						//    7 Mixed pixels
	0x8F,						//    7 Mixed pixels
	0xA2,						//    7 Mixed pixels
// Trailing white pixels are suppressed
//  0x40, 0x7D, //  125 White pixels
};								// 465 Total Pixels defined
const UBYTE IMG_UP[] =
{
	0x15, 0x00,	// Overall length
	0x00, 0x00,	// 16-bit screen id
	0x0D, 0x00,	// 13 Wide 
	0x07, 0x00,	// 7 High
	0x01,		// Compression type
	0x0C, 0x00,	// Data length
	0x81,		//    7 Mixed pixels
	0x0B,		//   11 White pixels
	0xF0,		//    7 Mixed pixels
	0x83,		//    7 Mixed pixels
	0xF0,		//    7 Mixed pixels
	0x8F,		//    7 Mixed pixels
	0xF0,		//    7 Mixed pixels
	0xBF,		//    7 Mixed pixels
	0xF1,		//    7 Mixed pixels
	0x2A,		//   10 Black pixels
	0xBF,		//    7 Mixed pixels
	0x27,		//    7 Black pixels
};
const UBYTE IMG_DOWN[] =
{
	0x15, 0x00,	// Overall length
	0x00, 0x00,	// 16-bit screen id
	0x0D, 0x00,	// 13 Wide 
	0x07, 0x00,	// 7 High
	0x01,		// Compression type
	0x0C, 0x00,	// Data length
	0x2D,		//   13 Black pixels
	0xBF,		//    7 Mixed pixels
	0xFC,		//    7 Mixed pixels
	0xBF,		//    7 Mixed pixels
	0xF0,		//    7 Mixed pixels
	0xBF,		//    7 Mixed pixels
	0xC0,		//    7 Mixed pixels
	0xBE,		//    7 Mixed pixels
	0x08,		//    8 White pixels
	0xF0,		//    7 Mixed pixels
	0x07,		//    7 White pixels
	0xC0		//    7 Mixed pixels
};

const UBYTE IMG_EMPTY[] =
{
	0x3A,0x00,	// Overall length
	0x00,0x00,	// 16-bit screen id
	0x26,0x00,	// 38 Wide 
	0x09,0x00,	// 9 High
	0x01,		// Compression type
	0x31,0x00,	// Data length
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xA5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAB,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xCA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD6,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0x95,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAD,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xDA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD5,		//    7 Mixed pixels
	0xAA,		//    7 Mixed pixels
	0xD4		//    6 Mixed pixels
};
#include "ImgNew16.h"
#include "ImgBatch16.h"
#include "ImgReport16.h"
#include "ImgTerm16.h"
#include "ImgSetup16.h"
#include "ImgFunction16.h"
#include "ImgNoimage16.h"
#include "ImgTest16.h"
#include "ImgPrint16.h"
#include "ImgBrowser16.h"
#include "ImgTab16.h"
#include "ImgCheck16.h"
#include "ImgSale16.h"
#include "ImgServer16.h"
#include "ImgRefund16.h"
#include "ImgOffline16.h"
#include "ImgAuth16.h"
#include "ImgBalance16.h"
#include "ImgCard16.h"
#include "ImgMorder16.h"
#include "ImgBank16.h"
#include "ImgReview16.h"
#include "ImgTotal16.h"
#include "ImgFind16.h"
#include "ImgAdjust16.h"
#include "ImgVoid16.h"
#include "ImgAudit16.h"
#include "ImgSummary16.h"
#include "ImgTill16.h"
#include "ImgInit16.h"
#include "ImgProgload16.h"
#include "ImgShift16.h"
#include "ImgEye16.h"
#include "ImgTrain16.h"
#include "ImgReset16.h"
#include "ImgLogon16.h"
#include "ImgIcepac16.h"
#include "ImgClock16.h"
#include "ImgPhone16.h"
#include "ImgPwd16.h"
#include "ImgContrast16.h"
#include "ImgHost16.h"
#include "ImgDisplay16.h"
#include "ImgKbd500016.h"
#include "ImgCrdr16.h"
#include "ImgPinpad16.h"
#include "ImgInitphone16.h"
#include "ImgNmsphone16.h"
#include "ImgDialtype16.h"
#include "ImgPabx16.h"
#include "ImgTermid16.h"
#include "ImgAll16.h"
#include "ImgReceipt16.h"
#include "ImgOnoff16.h"
#include "ImgQuit16.h"
#include "ImgAddrmv16.h"
#include "ImgTxrx16.h"
#include "ImgDb16.h"
#include "ImgClear16.h"
#include "ImgAmex16.h"
#include "ImgRelease16.h"
#include "ImgClose16.h"
#include "ImgOpen16.h"
#include "ImgAtm16.h"
#include "ImgUp16.h"
#include "ImgChange16.h"
#include "ImgLast16.h"
#include "ImgInvoice16.h"
#include "ImgHandset16.h"
#include "ImgToll16.h"
#include "ImgMemory16.h"
#include "ImgReferral16.h"
#include "ImgDetail16.h"
#include "ImgUnadjusted16.h"
#include "ImgAchcheck16.h"
#include "ImgPriorachcheck16.h"
#include "ImgLanguage16.h"
#include "ImgCash16.h"
#include "ImgSalCash16.h"
#include "ImgChkrdr16.h"
#include "ImgGSMsignal_0.h"
#include "ImgGSMsignal_20.h"
#include "ImgGSMsignal_40.h"
#include "ImgGSMsignal_60.h"
#include "ImgGSMsignal_80.h"
#include "ImgGSMsignal_100.h"
#include "ImgBattary_0.h"
#include "ImgBattary_20.h"
#include "ImgBattary_40.h"
#include "ImgBattary_60.h"
#include "ImgBattary_80.h"
#include "ImgBattary_100.h"
#include "ImgKeyAsc.h"
#include "ImgKeyAscSmall.h"
#include "ImgKeyNum.h"
#include "ImgKeySym.h"
#include "ImgBlueTooth.h"

const IMAGE_ENTRY ImageTable[] = {
	{IMAGE_CARD, IMG_CARD},
	{IMAGE_ICCCARD, IMG_ICCCARD},
	{IMAGE_BACKARROW, IMG_BACKARROW},
	{IMAGE_FORWARDARROW, IMG_FORWARDARROW},
	{IMAGE_KEY, IMG_KEY},
	{IMAGE_ENTER, IMG_ENTER},
	{IMAGE_NEW, ImgNew16},
	{IMAGE_BATCH, ImgBatch16},
	{IMAGE_REPORT, ImgReport16},
	{IMAGE_TERMINAL, ImgTerm16},
	{IMAGE_SETUP, ImgSetup16},
    {IMAGE_NOIMAGE, ImgNoimage16},
    {IMAGE_FUNCTION, ImgFunction16},
	{IMAGE_TEST, ImgTest16},
	{IMAGE_PRINT, ImgPrint16},
	{IMAGE_BROWSER, ImgBrowser16},
	{IMAGE_TAB, ImgTab16},
	{IMAGE_CHECK, ImgCheck16},
	{IMAGE_SALE, ImgSale16},
	{IMAGE_SERVER, ImgServer16},
	{IMAGE_REFUND, ImgRefund16},
	{IMAGE_OFFLINE, ImgOffline16},
	{IMAGE_AUTH, ImgAuth16},
	{IMAGE_BALANCE, ImgBalance16},
	{IMAGE_CARDS, ImgCard16},
	{IMAGE_MAIL, ImgMorder16},
	{IMAGE_BANK, ImgBank16},
	{IMAGE_REVIEW, ImgReview16},
	{IMAGE_TOTAL, ImgTotal16},
	{IMAGE_FIND, ImgFind16},
	{IMAGE_ADJUST, ImgAdjust16},
	{IMAGE_VOID, ImgVoid16},
	{IMAGE_AUDIT, ImgAudit16},
	{IMAGE_SUMMARY, ImgSummary16},
	{IMAGE_TILL, ImgTill16},
	{IMAGE_INITIALIZE, ImgInit16},
	{IMAGE_PROGLOAD, ImgProgload16},
	{IMAGE_SHIFT, ImgShift16},
	{IMAGE_EYE, ImgEye16},
	{IMAGE_TRAIN, ImgTrain16},
	{IMAGE_RESET, ImgReset16},
	{IMAGE_LOGON, ImgLogon16},
	{IMAGE_ICEPAC, ImgIcepac16},
	{IMAGE_CLOCK, ImgClock16},
	{IMAGE_PHONE, ImgPhone16},
	{IMAGE_PWD, ImgPwd16},
	{IMAGE_CONTRAST, ImgContrast16},
	{IMAGE_HOST, ImgHost16},
	{IMAGE_DISPLAY, ImgDisplay16},
	{IMAGE_KBD, ImgKbd500016},
	{IMAGE_CARDREADER, ImgCrdr16},
	{IMAGE_PINPAD, ImgPinpad16},
	{IMAGE_INITPHONE, ImgInitphone16},
	{IMAGE_NMSPHONE, ImgNmsphone16},
	{IMAGE_DIALTYPE, ImgDialtype16},
	{IMAGE_PABX, ImgPabx16},
	{IMAGE_TERMID, ImgTermid16},
	{IMAGE_ALL, ImgAll16},
	{IMAGE_RECEIPT, ImgReceipt16},
	{IMAGE_ONOFF, ImgOnoff16},
	{IMAGE_QUIT, ImgQuit16},
	{IMAGE_ADDRMV, ImgAddrmv16},
	{IMAGE_TXRX, ImgTxrx16},
	{IMAGE_DB, ImgDb16},
	{IMAGE_CLEAR, ImgClear16},
	{IMAGE_AMEX, ImgAmex16},
	{IMAGE_RELEASE, ImgRelease16},
	{IMAGE_CLOSE, ImgClose16},
	{IMAGE_OPEN, ImgOpen16},
	{IMAGE_ATM, ImgAtm16},
	{IMAGE_UP, ImgUp16},
	{IMAGE_CHANGE, ImgChange16},
	{IMAGE_LAST, ImgLast16},
	{IMAGE_INVOICE, ImgInvoice16},
	{IMAGE_HANDSET, ImgHandset16},
	{IMAGE_TOLL, ImgToll16},
	{IMAGE_MEMORY, ImgMemory16},
	{IMAGE_REFERRAL, ImgReferral16},
	{IMAGE_DETAIL, ImgDetail16},
	{IMAGE_UNADJUSTED, ImgUnadjusted16},
	{IMAGE_ACHCHECK, ImgAchcheck16},
	{IMAGE_PRIORACHCHECK, ImgPriorachcheck16},
	{IMAGE_TIPS, ImgRefund16},
	{IMAGE_LANGUAGE, ImgLanguage16},
	{IMAGE_CASH, ImgCash16},
	{IMAGE_SALCASH, ImgSalCash16},
	{IMAGE_CHKRDR, ImgChkrdr16},
	{IMAGE_UPARROW, IMG_UP},
	{IMAGE_DOWNARROW, IMG_DOWN},
	{IMAGE_EMPTY, IMG_EMPTY},
    {IMAGE_GSMSIGNAL_0, ImgGSMsignal_0},
	{IMAGE_GSMSIGNAL_20, ImgGSMsignal_20},
	{IMAGE_GSMSIGNAL_40, ImgGSMsignal_40},
	{IMAGE_GSMSIGNAL_60, ImgGSMsignal_60},
	{IMAGE_GSMSIGNAL_80, ImgGSMsignal_80},
	{IMAGE_GSMSIGNAL_100, ImgGSMsignal_100},
	{IMAGE_BATTARY_0, ImgBattary_0},
	{IMAGE_BATTARY_20, ImgBattary_20},
	{IMAGE_BATTARY_40, ImgBattary_40},
    {IMAGE_BATTARY_60, ImgBattary_60},
	{IMAGE_BATTARY_80, ImgBattary_80},
	{IMAGE_BATTARY_100, ImgBattary_100},
    {IMAGE_KEYASC,   ImgKeyAsc},
    {IMAGE_KEYASCSM, ImgKeyAscSmall},
    {IMAGE_KEYNUM,   ImgKeyNum},
    {IMAGE_KEYSYM,   ImgKeySym},
    {IMAGE_BLUETOOTH,   ImgBlueTooth},
	//12.03.2007 DB Formatlý yazdýrmak için kullanýlan header dosyalarý
	{ACQUIRER_TABLE_VAR_NAMES,AcqTableVarName},
	{ACQUIRER_TABLE_VAR_TYPE_AND_LENGTH,AcqTableVarTypeAndLength},
	{TERMINAL_TABLE_VAR_NAMES,TermTableVarName},
	{TERMINAL_TABLE_VAR_TYPE_AND_LENGTH,TermTableVarTypeAndLength},
	{ISSUER_TABLE_VAR_NAMES,IssTableVarName},
	{ISSUER_TABLE_VAR_TYPE_AND_LENGTH,IssTableVarTypeAndLength},
	{CARD_TABLE_VAR_NAMES,CardTableVarName},
	{CARD_TABLE_VAR_TYPE_AND_LENGTH,CardTableVarTypeAndLength},
};



//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  GetImage            Return a pointer to selected image
//
//  Parameters:         IMAGE_ID imageID    Id of image to retrieve
//
//  Global Inputs:      None
//
//  Returns:            UBYTE *     Pointer to buffer of selected image
//
//  Notes:
//-----------------------------------------------------------------------------
extern UBYTE *GetImage( IMAGE_ID imageID )
{
	UBYTE i;
	UBYTE tmpBuf[2];
	IMAGE_ID tmpID;

	tmpID = imageID;

	if ( tmpID >= IMG_USER0 && tmpID <= IMG_USER15 )
	{
		tmpID = User_Imgs[tmpID - IMG_USER0];
	}

	if ( tmpID == MSG_ICEBUTTON1_L1 )
	{
		AscHex( tmpBuf, ( const char * ) ( IBTAB.IBBTN1LABEL1 ), 2 );
		tmpID = BcdBin( tmpBuf );
	}
	if ( tmpID == MSG_ICEBUTTON2_L1 )
	{
		AscHex( tmpBuf, ( const char * ) ( IBTAB.IBBTN2LABEL1 ), 2 );
		tmpID = BcdBin( tmpBuf );
	}
	if ( tmpID == MSG_ICEBUTTON3_L1 )
	{
		AscHex( tmpBuf, ( const char * ) ( IBTAB.IBBTN3LABEL1 ), 2 );
		tmpID = BcdBin( tmpBuf );
	}
	if ( tmpID == MSG_ICEBUTTON4_L1 )
	{
		AscHex( tmpBuf, ( const char * ) ( IBTAB.IBBTN4LABEL1 ), 2 );
		tmpID = BcdBin( tmpBuf );
	}

	for ( i = 0; i < sizeof( ImageTable ) / sizeof( IMAGE_ENTRY ); i++ )
	{
		if ( tmpID == ImageTable[i].imageID )
			return ( UBYTE * ) ( ImageTable[i].pImage );


	}

	return ( UBYTE * ) ImgNoimage16;
}



//=============================================================================
// Private function definitions
//=============================================================================
