//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _BIT8583_H_
#define _BIT8583_H_
//=============================================================================
//
// Module overview:             bit8583.h
//      ISO8583 request building routines
//

//=============================================================================
//=============================================================================
#include "transdef.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================
//
//		 The following is a list of #defines corresponding to the
//		 ISO 8583 bitmap.
//

//
//       Bitmap Byte 1
//
#define B11_EMPTY   0x80		// Bit 1  - Not Used
#define B12_PAN     0x40		// Bit 2  - Primary Account Number
#define B13_PROC	0x20		// Bit 3  - Processing Code
#define B14_TOTAM   0x10		// Bit 4  - Amount, Transaction
#define B15_EMPTY   0x08		// Bit 5  - Not Used
#define B16_EMPTY   0x04		// Bit 6  - Not Used
#define B17_EMPTY	0x02		// Bit 7  - Not Used
#define B18_EMPTY	0x01		// Bit 8  - Not Used

//
//       Bitmap Byte 2
//
#define B21_EMPTY   0x80		// Bit 9  - Not Used
#define B22_EMPTY   0x40		// Bit 10 - Not Used
#define B23_STAN    0x20		// Bit 11 - Systems Trace Audit Number
#define B24_TIME    0x10		// Bit 12 - Time, Transaction
#define B25_DATE    0x08		// Bit 13 - Date, Transaction
#define B26_EXPD	0x04		// Bit 14 - Date, Expiry
#define B27_EMPTY	0x02		// Bit 15 - Not Used
#define B28_EMPTY   0x01		// Bit 16 - Not Used

//
//       Bitmap Byte 3
//



#define B31_EMPTY	0x80		// Bit 17 - Not Used
#define B32_MERCCODE	0x40		// Bit 18 - EMV @@A
#define B33_EMPTY	0x20		// Bit 19 - Not Used
#define B34_CNTRYCODE   0x10		// Bit 20 - EMV @@A
#define B35_EMPTY	0x08		// Bit 21 - Not Used
#define B36_POSE	0x04		// Bit 22 - POS Entry Mode
#define B37_EMPTY	0x02		// Bit 23 - Not Used
#define B38_NII		0x01		// Bit 24 - Network International Identifier

//
//       Bitmap Byte 4
//
#define B41_POCC	0x80		// Bit 25 - POS Condition Code
#define B42_EMPTY	0x40		// Bit 26 - Not Used
#define B43_EMPTY   0x20		// Bit 27 - Not Used
#define B44_EMPTY	0x10		// Bit 28 - Not Used
#define B45_EMPTY	0x08		// Bit 29 - Not Used
#define B46_EMPTY	0x04		// Bit 30 - Not Used
#define B47_EMPTY	0x02		// Bit 31 - Not Used
#define B48_EMPTY	0x01		// Bit 32 - Not Used

//
//       Bitmap Byte 5
//
#define B51_EMPTY	0x80		// Bit 33 - Not Used
#define B52_EMPTY	0x40		// Bit 34 - Not Used
#define B53_TRK2    0x20		// Bit 35 - Track 2 Data
#define B54_EMPTY	0x10		// Bit 36 - Not Used
#define B55_RRN 	0x08		// Bit 37 - Retrieval Reference Number
#define B56_AUTH	0x04		// Bit 38 - Auth Id Response code
#define B57_RESP	0x02		// Bit 39 - Respoce code
#define B58_EMPTY	0x01		// Bit 40 - Not Used

//
//       Bitmap Byte 6
//
#define B61_TID		0x80		// Bit 41 - Terminal Id
#define B62_ACCID	0x40		// Bit 42 - Acceptor Id
#define B63_ACCNAME     0x20		// Bit 43 - Not Used
#define B64_RSPA	0x10		// Bit 44 - Not Used
#define B65_TRK1	0x08		// Bit 45 - AMEX Trk1
#define B66_EMPTY	0x04		// Bit 46 - Not Used
#define B67_EMPTY	0x02		// Bit 47 - Not Used
#define B68_CVV2	0x01		// Bit 48 - Not Used

//
//       Bitmap Byte 7
//
#define B71_CURCODE	0x80		// Bit 49 - Not Used
#define B72_EMPTY	0x40		// Bit 50 - Not Used
#define B73_EMPTY   0x20		// Bit 51 - Not Used
#define B74_PIN 	0x10		// Bit 52 - PIN number
#define B75_EMPTY	0x08		// Bit 53 - Not Used
#define B76_TIP 	0x04		// Bit 54 - Tip Amoun
#define B77_ICC 	0x02		// Bit 55 - Not Used
#ifdef MAKE_EMV
#define B78_MOVISS	0x01		// Bit 56 - MovIsScriptRes // @@AA  
#else
#define B78_EMPTY	0x01		// Bit 56 - Not Used
#endif


//
//       Bitmap Byte 8
//
#ifdef MAKE_EMV
#define B81_MOVAPPDT	0x80		// Bit 57 - Not Used
#else
#define B81_EMPTY	0x80		// Bit 57 - Not Used
#endif



#define B82_EMPTY	0x40		// Bit 58 - Not Used
#define B83_EMPTY   0x20		// Bit 59 - Not Used
#define B84_BIT60 	0x10		// Bit 60 - Private Field 60
#define B85_BIT61 	0x08		// Bit 61 - Private Field 61
#define B86_BIT62   0x04		// Bit 62 - Private Field 62
#define B87_BIT63 	0x02		// Bit 63 - Private Field 63
#define B88_MAC		0x01		// Bit 64 - MAC

struct bitmap_rec
{
	enum transactions trans;	// Transaction
	UBYTE TRNMSG[2];			// MESSAGE TYPE
	UBYTE TRNPROC;				// PROCESSING CODE
	UBYTE bitmap[8];			// Bit map bytes
} __attribute__((packed));

struct move_rec
{
	UBYTE movbitno;				// Bit number to move for
	Bool( *mov_it ) ( void );	// Move routine
} __attribute__((packed));

//=============================================================================
// Public data declarations
//=============================================================================
extern const struct is8583_rec *prs8583;
extern UBYTE TmpBuf[256];

extern const struct move_rec *Move_Table;
extern UBYTE *pSendData;		// Pointer into send buffer
extern UBYTE *pB63Len;			// Pointer to Bit 63 length bytes
extern int B63_Len;				// Bit 63 total length


//=============================================================================
// Public function declarations
//=============================================================================
extern UBYTE BinBcdB( UBYTE ubBIN );
extern UWORD BinBcdW( UWORD uwBIN );
extern Bool ReqPackMsg( UBYTE * pTBitMap );
extern Bool IfThere( UBYTE * pField, UWORD Length );
extern void MovGen( UBYTE * pData, UWORD Length );
extern Bool MovPan( void );
extern Bool MovTotAm( void );
extern Bool MovStan( void );
extern Bool MovTime( void );
extern Bool MovDate( void );
extern Bool MovExpdYYMM( void );
extern Bool MovExpdMMYY( void );
extern Bool MovPose( void );
extern Bool MovNII( void );
extern Bool MovPocc( void );
extern Bool MovTrk2( void );
extern Bool MovRRN( void );
extern Bool MovAuth( void );
extern Bool MovResp( void ); //@AAMELIN
extern Bool MovTID( void );
extern Bool MovAccID( void );
extern Bool MovRspa( void );
extern Bool MovPin( void );
extern Bool MovTip( void );
extern Bool MovDesc( void );
extern Bool B63_CVV2( void );
extern Bool B63_PS2000( void );
extern Bool B63_Hostref( void );
extern Bool B63_DUKPT( void );
extern Bool B63_Addprmpt( void );
extern Bool B63_BatNum( void );
extern Bool B63_TaxAmt( void );
extern Bool B63_CashBK( void );
extern Bool B63_BarAmt( void );
extern Bool B63_MoveStdTotals( void );
extern Bool B63_MoveTotals( void );
extern Bool B63_InstalRefund( void );

#ifdef MAKE_REWARD
extern Bool MovReward( void );		
#endif

#ifdef MAKE_EMV
extern Bool MovICC( void );
extern Bool MovMerchCCode( void );        
extern Bool MovCountryCode( void );
extern Bool MovIsScriptRes( void );
extern Bool MovIssuerAppData( void );
#endif


extern Bool MovAcqName(void);
extern Bool MovTrk1(void);
extern Bool MovCVV2(void);
extern Bool B63_BatchUpload( void );
extern Bool B63_InstalRefund( void );
extern Bool B63_MoveTableId90 ( void );
extern Bool B63_MoveTableId95 ( void );

#endif // _BIT8583_H_
