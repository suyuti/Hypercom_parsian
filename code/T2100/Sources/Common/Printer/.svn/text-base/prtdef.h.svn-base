
#ifndef _PRTDEF_H_
#define _PRTDEF_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             prtdef.h
//      Printer definitions header file
//
//=============================================================================

#include "prtfields.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================

//
//      Report Enumeration
//

enum REPORTTYPE
{
	RP_RECEIPT,					// RECEIPT
	
	RP_INIT,					// INIALIZE REPORT
	RP_TRANS_SALE,				// SALE TRANSACTION	
	RP_TRANS_REFUND,			// REFUND TRANSACTION
	RP_TRANS_VOID,				// VOID TRANSACTION

        RP_TRANS_BALINQ,                       // LOYALTY TRANSACTION 


	RP_HEAD_SETTLE,				// SETTLEMENT RECEIPT HEADER
	RP_SETTLE,					// SETTLEMENT RECEIPT

	RP_HEAD_AUDIT,				// AUDIT REPORT HEADER
	RP_AUDIT,					// AUDIT REPORT

	RP_HEAD_SUMMARY_BYISS,		// SUMMARY REPORT HEADER
	RP_SUMMARY_BYISS,			// SUMMARY REPORT BY ISSUER

	RP_HEAD_MERCHANT,			// HEAD MERCHANT NAME & ADDRESS 
	RP_ID_MERCHANT,				// MERCHANT ID
	RP_ID_TERMINAL,				// TERMINAL ID
	RP_DATE_TIME_TRANS,			// TRANSACTION DATE/TIME
	RP_NO_STAN,					// STAN
	RP_NO_BATCH,				// BATCH NUMBER
	RP_NO_INVOICE,				// INVOICE NUMBER
	RP_NAME_VERSION,			// VERSION NAME
	RP_NAME_SOURCE,				// TRANSACTION SOURCE NAME
	RP_TYPE_CARD,				// CARD
	RP_NO_PAN,					// PAN
	RP_NAME_CARDHOLDER,			// CARD HOLDER NAME
	RP_NAME_TRANS,				// TRANSACTION NAME
        RP_CNAME_TRANS,
	RP_HEADER_COMMON,			// COMMON HEADER REPORT
	RP_AMOUNT_TOTAL,			// TOTAL AMOUNT REPORT
	RP_CODE_AUTH,				// Authorisation code REPORT
	RP_ID_AID,				    // Application Id
        RP_LABEL_APP,               // Application Label
	RP_NO_RRN,				    // Retrival Reference Number
	RP_DATE_EXP,			    // Expired Date 
	RP_TXT_FOOTERS,				// Custom receipt footer
	RP_TXT_HOST,				// Additional Host Print Data Report
	RP_SIGN,                                // Signagure report
	RP_TEST,				// PRINTER TEST
	RP_DATABASE,				// DATABASE REPORT
	RP_DATASCOPE,				// DATASCOPE REPORT
	RP_HEAD_MRCH,               // MERCHANT NAME & ADDRESS & TERMINAL ID
	RP_TOTALS,					// SALES & REFUND TOTALS
	RP_HEAD_MRCH2,				// MERCHANT ID & ACQUIRER NAME	
	RP_HEAD_COMMON,				// MERCHANT NAME & ADDRESS
	RP_HEAD_SERIAL,				// MERCHANT NAME & ADDRESS & TERMINAL SERIAL #
	RP_SETTLE_SUM,				// SETTLEMENT SUMMARY REPORT
        RP_PERFORMANCE,             // PERFORMANCE REPORT  
	RP_ISSUER_CONFIG,			// ISSUER CONFIGURATION REPORT
	RP_ACQUIRER_CONFIG,			// ACQUIRER CONFIGURATION REPORT
	RP_CARD_CONFIG,		   		// CARD CONFIGURATION REPORT
        RP_PAN,                                 // PAN WITHOUT MASKING
#ifdef MAKE_KIB
        RP_AVAILABLE_LOYALTY_TOTAL,
#endif
        RP_DISCOUNT,                            // indirimli 
        RP_POINTPART1,                          // puanlý iþlem 1. kýsým raporu  
        RP_POINTPART2,                          // puanlý iþlem 2. kýsým raporu 
        RP_LOYALTYAMOUNT,
        RP_POINTINSTFAIL,
	RP_MARKER = 255				// End of the report table
};

// Table of built-in reports in PCL format
struct report_table_struct
{
	enum REPORTTYPE num;
	UBYTE *start;
	UBYTE *end;
};


#define DOUB     prt_FONT_DOUBLE	// DOUBLE WIDE
#define NORM     prt_FONT_NORMAL	// NORMAL ( UNDOUBLE )

//
//      Report processing flags
//

#define PF_DBLE     0x01		// DOUBLE WIDTH PRINTING
#define PF_DPRT     0x02		// PRINTER IS PRINTING DOUBLE
#define PF_CNTR     0x04		// NEXT FIELD IS TO BE CENTER
#define PF_RGHT     0x08		// NEXT FIELD IS TO BE RIGHT JUSTIFIED
#define PF_AVAL     0x10		// *** AVAILABLE *** -- not in use
#define PF_PREP     0x20		// PREPRINT ON
#define PF_OFFP     0x40		// PRINTING IS OFF (USED FOR PREPRINT)
#define PF_PREV     0x80		// PREVIOUS PRINT CALLS (NOT FIRST TIME)

//
//		Parametrs for FONT command
//

#define	SMALL_FONT		0x00	// Small font
#define LOW_FONT		0x01	// Low font
#define NORM_FONT		0x02	// Default font
#define DOUB_FONT		0x03	// Double font
#define WIDE_FONT		0x04	// Wide font
#define HIGH_FONT		0x05	// High font
#define LARGE_FONT		0x06	// Large font
#define XLARGE_FONT		0x07	// XLarge font
#define REVERSE_OFF		0x0E	// Reverse off
#define REVERSE_ON		0x0F	// Reverse on
#define UNDERLINE_OFF	0x10	// Underline off
#define UNDERLINE_ON	0x11	// Underline on
#define BOLD_OFF		0x12	// Bold off
#define BOLD_ON			0x13	// Bold on
#define ITALIC_OFF		0x14	// Italic off
#define ITALIC_ON		0x15	// Italic on
#define BARCODE_ON		0x0B	// Barcode on
#define BARCODE_START	'*'		// Start barcode sequence
#define BARCODE_END		'*'		// End barcode sequence	

//
//      Command Token Tolken
//   Any new command should be added after "LINE" and before
//   FLD_END command.  Always Place FLD_END at the end.
//
//
//      Report
//

#define LINE     128+0			// Dump the current line to the print buffer and
						 //  does a LF (if needed).
#define FORM     128+1			// Dump the current line to the print buffer and
						 //  does a FF.
#define NORMAL   128+2			// Sets printing to single wide charact.
#define DOUBLE   128+3			// Sets printing to double wide charact.
#define FONT     128+4			// Changes font (1 operand: font type)
#define POSLFT   128+5			// Left justifies text at a specific column
						 //  (1 operand: column #)
#define POSRGHT  128+6			// Right justifies text at a specific column
						 //  (1 operand: column #)
#define POSCNTR  128+7			// Centers text at a specific column
						 //  (1 operand: column #)
#define LFT      128+8			// Left justifies text on a line
#define CNTR     128+9			// Centers text on a line
#define RGHT     128+10			// Right justifies text on a line
#define MLTSPC   128+11			// Prints multiple spaces
						 //  (1 operand: number of spaces)
#define MLTCHR   128+12			// Prints a character multiple time
						 //  (2 operands: # of char., the char)
#define FLD      128+13			// Prints a field (1 operand, field #)
#define FLD_S    128+14			// Prints the right portion of a field
						 //  (2 operands, field #, starting pos)
#define FLD_L    128+15			// Prints the left portion of a field
						 //  (2 operands, field #, length to prt)
#define FLD_SL   128+16			// Prints the middle portion of a field
						 //  (3 operands, field #, start, length)
#define FLD_F    128+17			// Prints a field with special format
						 //  (2 operands, field #, format code)
#define FLD_FS   128+18			// Prints the right portion of formatted field
						 //  (3 operands, field #, format code, starting position)
#define FLD_FL   128+19			// Prints the left portion of a formatted field
						 //  (3 operands, field #, format code, length)
#define FLD_FSL  128+20			// Prints the middle portion of a formatted field
						 //  (4 operands: field #, format code, start, len)
#define REPORT   128+21			// Nested Report (1 operand: report #)
#define WIDTH    128+22			// Set the Printer Width
#define ELSEIF   128+25			// Used as part of an conditional
#define IF       128+26			// Start of a conditional processing
						 //  (variable # of operands)
#define OR       128+27			// Used as part of an IF statement
#define AND      128+28			// Used as part of an IF statement
#define ENDIF    128+29			// Finishes the conditional (every IF statement must
						 //  have a matching ENDIF)
#define ELSE     128+30			// Used as part of an conditional
#define PPSTOP   128+31			// Indicates where PREPRINTING ends
#define SIG      128+33			// Print a signature
#define GRPH     128+34			// Print the graphic image specified by id
#define GRPHA    128+35			// Print the graphic image specified by address
#define EQUAL    128+36			// Checks to see if the field is equal to a value
#define RED      128+37			// Sets the line to red ink (VF250 printer only)
#define TXT		 128+38			// Prints text string

#define EXTFLD   128+39         // processing extended FLDTAB 
#define EXTTXT   128+40        	// processing extended text table  
#define FIRMWARE 128+41			// Printer prints its firware version
#define FLD_END  128+42			// Marker for the end of table  Always put at end. Used in PRTDRV.C file.

#define NO_CMD   255			// Non-command marker within the command table
//
//      TRANSACTION 'IF' TYPES
//      NOTE: There are 2 types of testing assoicated with an IF
//            statement, fields and conditional.  NOT uses 0.
//            Values 1 through 159 are reserved for fields.
//

#define NOT          0			// IF NOT

#define TRN_SALE     160		// Sale type transaction
#define TRN_REFD     161		// Refund type transaction
#define TRN_AUTH     162		// Authorization transaction
#define TRN_INSREF   163		// Ýnstallment Refund
#define TRN_BALI     164		// Balance inquiry
#define TRN_CRCD     165		// Credit card transaction
#define TRN_DBCD     166		// Debit card transaction
#define CND_USEDPIN  167		// Offline transaction
#define TRN_VOID     168		// Voided transaction
#define TRN_OPENT    169		// Open tab transaction
#define TRN_SETTLE   170        // Settlement transaction */
#define TRN_INSTAL   172		// Free
#define TRN_AVSVOID  173		// AVS Voided transaction
#define TRN_NOSIG    174		// Suppress signature line
#define CND_LNEQ     176		// Line # equal to
#define CND_LNGE     177		// Line # greater or equal to
#define CND_FIRST    178		// First time through, (for headings)
#define CND_REJ      179		// Transaction rejected?
#define CND_TIP      180		// Tip processing?
#define CND_REPR     181		// Reprint?
#define CND_PREP     182		// Pre-print?
#define CND_SREQ     183		// Signature required?
#define CND_ROLL     184		// Roll printer in use?
#define CND_MAN      185		// Manually entered?
#define CND_ACCSIG   186		// Accepted, signature required?
#define CND_DISCOUNT 187                // indirimli iþlem mi 
#define CND_POINT    188                // puanlý iþlem mi 
#define CND_ALLPOINT 189                // tüm puan mý kullanýldý 
#define CND_LOYALTY  190                // iþlem loyalty iþlemi mi 
#define CND_ONUSCARD 191                // hosttapuan bilgisi

#define OPT1         192		// Option 1
#define OPT2         193		// Option 2
#define OPT3         194		// Option 3
#define OPT4         195		// Option 4
#define OPT5         196		// Option 5
#define OPT6         197		// Option 6
#define OPT7         198		// Option 7
#define OPT8         199		// Option 8

#define CND_SECONDCOPY 200
#define CND_PANTRUNC   201
#define CND_PRINTTIME  202


// Bitmasks corresponding to OPT1 through OPT8
#define OPT1MASK 0x01
#define OPT2MASK 0x02
#define OPT3MASK 0x04
#define OPT4MASK 0x08
#define OPT5MASK 0x10
#define OPT6MASK 0x20
#define OPT7MASK 0x40
#define OPT8MASK 0x80

//
//      Print Line Justification Values
//

#define LJ_LEFT     1			// Left justifies text on a line
#define LJ_CENTER   2			// Centers text on a line
#define LJ_RIGHT    3			// Right justifies text on a line



//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================


#endif	// _PRTDEF_H_

