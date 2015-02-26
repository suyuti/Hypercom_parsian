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


//=============================================================================
// Public defines and typedefs
//=============================================================================

//
//      Report Enumeration
//

enum REPORTTYPE
{
    //! RECEIPT
	RP_RECEIPT,
    //! PRINTER TEST
	RP_TEST,
    //! AUDIT REPORT
	RP_AUDIT,
    //! ISSUER REPORT
	RP_ISSUER,
    //! SETTLEMENT RECEIPT
	RP_SETTLE,
    //! CASHIER REPORT
	RP_CASHIER,
    //! CASHIER LOGON REPORT
	RP_LOGON,
    //! DATABASE REPORT
	RP_DATABASE,
    //! DATASCOPE REPORT
	RP_DATASCOPE,
    //! OPEN TABS REPORT
	RP_OPEN_TABS,
    //! MERCHANT NAME & ADDRESS & TERMINAL ID
	RP_HEAD_MRCH,
    //! SALES & REFUND TOTALS
	RP_TOTALS,
    //! MERCHANT ID & ACQUIRER NAME
	RP_HEAD_MRCH2,
    //! SETTLEMENT RECEIPT HEADER
	RP_HEAD_SETTLE,
    //! MERCHANT NAME & ADDRESS
	RP_HEAD_COMMON,
    //! MERCHANT NAME & ADDRESS & TERMINAL SERIAL #
	RP_HEAD_SERIAL,
    //! SETTLEMENT SUMMARY REPORT
	RP_SETTLE_SUM,
    //! PERFORMANCE REPORT  
    RP_PERFORMANCE,
    //! ISSUER CONFIGURATION REPORT
	RP_ISSUER_CONFIG,
    //! ACQUIRER CONFIGURATION REPORT
	RP_ACQUIRER_CONFIG,
    //! CARD CONFIGURATION REPORT
	RP_CARD_CONFIG,
	RP_MARKER = 255				// End of the report table
};

//! Table of built-in reports in PCL format
struct report_table_struct
{
	enum REPORTTYPE num;
	UBYTE *start;
	UBYTE *end;
};


//! DOUBLE WIDE
#define DOUB     prt_FONT_DOUBLE
//! NORMAL ( UNDOUBLE )
#define NORM     prt_FONT_NORMAL

//
//!      Report processing flags
//

//! DOUBLE WIDTH PRINTING
#define PF_DBLE     0x01
//! PRINTER IS PRINTING DOUBLE
#define PF_DPRT     0x02
//! NEXT FIELD IS TO BE CENTER
#define PF_CNTR     0x04
//! NEXT FIELD IS TO BE RIGHT JUSTIFIED
#define PF_RGHT     0x08
//! *** AVAILABLE *** -- not in use
#define PF_AVAL     0x10
//! PREPRINT ON
#define PF_PREP     0x20
//! PRINTING IS OFF (USED FOR PREPRINT)
#define PF_OFFP     0x40
//! PREVIOUS PRINT CALLS (NOT FIRST TIME)
#define PF_PREV     0x80

//
//!		Parametrs for FONT command
//

//! Small font
#define	SMALL_FONT		0x00
//! Low font
#define LOW_FONT		0x01
//! Default font
#define NORM_FONT		0x02
//! Double font
#define DOUB_FONT		0x03
//! Wide font
#define WIDE_FONT		0x04
//! High font
#define HIGH_FONT		0x05
//! Large font
#define LARGE_FONT		0x06
//! XLarge font
#define XLARGE_FONT		0x07
//! Reverse off
#define REVERSE_OFF		ESC,prt_INVERSE
//! Reverse on
#define REVERSE_ON		REVERSE_OFF
//! Underline off
#define UNDERLINE_OFF	ESC,prt_UNDERLINE
//! Underline on
#define UNDERLINE_ON	UNDERLINE_OFF
//! Bold off
#define BOLD_OFF		0x12
//! Bold on
#define BOLD_ON			0x13
//! Italic off
#define ITALIC_OFF		0x14
//! Italic on
#define ITALIC_ON		0x15
//! Barcode on
#define BARCODE_ON		prt_BARCODE
//! Start barcode sequence
#define BARCODE_START	'*'
//! End barcode sequence
#define BARCODE_END		'*'

//
//!      Command Token Tolken
//!   Any new command should be added after "LINE" and before
//!   FLD_END command.  Always Place FLD_END at the end.
//
//
//      Report
//

//! Dump the current line to the print buffer and does a LF (if needed).
#define LINE     128+0
//! Dump the current line to the print buffer and does a FF.
#define FORM     128+1
//! Sets printing to single wide charact.
#define NORMAL   128+2
//! Sets printing to double wide charact.
#define DOUBLE   128+3
//! Changes font (1 operand: font type)
#define FONT     128+4
//! Left justifies text at a specific column (1 operand: column #)
#define POSLFT   128+5
//! Right justifies text at a specific column (1 operand: column #)
#define POSRGHT  128+6
//! Centers text at a specific column (1 operand: column #)
#define POSCNTR  128+7
//! Left justifies text on a line
#define LFT      128+8
//! Centers text on a line
#define CNTR     128+9
//! Right justifies text on a line
#define RGHT     128+10
//! Prints multiple spaces (1 operand: number of spaces)
#define MLTSPC   128+11
//! Prints a character multiple time (2 operands: # of char., the char)
#define MLTCHR   128+12
//! Prints a field (1 operand, field #)
#define FLD      128+13
//! Prints the right portion of a field (2 operands, field #, starting pos)
#define FLD_S    128+14
//! Prints the left portion of a field (2 operands, field #, length to prt)
#define FLD_L    128+15
//! Prints the middle portion of a field (3 operands, field #, start, length)
#define FLD_SL   128+16
//! Prints a field with special format (2 operands, field #, format code)
#define FLD_F    128+17
//! Prints the right portion of formatted field
//! (3 operands, field #, format code, starting position)
#define FLD_FS   128+18
//! Prints the left portion of a formatted field
//! (3 operands, field #, format code, length)
#define FLD_FL   128+19
//! Prints the middle portion of a formatted field
//! (4 operands: field #, format code, start, len)
#define FLD_FSL  128+20
//! Nested Report (1 operand: report #)
#define REPORT   128+21
//! Set the Printer Width
#define WIDTH    128+22
//! Used as part of an conditional
#define ELSEIF   128+25
//! Start of a conditional processing (variable # of operands)
#define IF       128+26
//! Used as part of an IF statement
#define OR       128+27
//! Used as part of an IF statement
#define AND      128+28
//! Finishes the conditional (every IF statement must have a matching ENDIF)
#define ENDIF    128+29
//! Used as part of an conditional
#define ELSE     128+30
//! Indicates where PREPRINTING ends
#define PPSTOP   128+31
//! Print a signature
#define SIG      128+33
//! Print the graphic image specified by id
#define GRPH     128+34
//! Print the graphic image specified by address
#define GRPHA    128+35
//! Checks to see if the field is equal to a value
#define EQUAL    128+36
//! Sets the line to red ink (VF250 printer only)
#define RED      128+37
//! Prints text string
#define TXT		 128+38

//! processing extended FLDTAB
#define EXTFLD   128+39
//! processing extended text table
#define EXTTXT   128+40
//! Printer prints its firware version
#define FIRMWARE 128+41
//! Marker for the end of table  Always put at end. Used in PRTDRV.C file.
#define FLD_END  128+42

//! Non-command marker within the command table
#define NO_CMD   255

//
//!      TRANSACTION 'IF' TYPES
//!      NOTE: There are 2 types of testing assoicated with an IF
//!            statement, fields and conditional.  NOT uses 0.
//!            Values 1 through 159 are reserved for fields.
//

//! IF NOT
#define NOT          0

//! Sale type transaction
#define TRN_SALE     160
//! Refund type transaction
#define TRN_REFD     161
//! Authorization transaction
#define TRN_AUTH     162
// Free
//#define            163
//! Balance inquiry
#define TRN_BALI     164
//! Credit card transaction
#define TRN_CRCD     165
//! Debit card transaction
#define TRN_DBCD     166
//! Offline transaction
#define TRN_OFFL     167
//! Voided transaction
#define TRN_VOID     168
//! Open tab transaction
#define TRN_OPENT    169
//! Settlement transaction
#define TRN_SETTLE   170
// Free
//#define            172
//! AVS Voided transaction
#define TRN_AVSVOID  173
//! Suppress signature line
#define TRN_NOSIG    174
//! Line # equal to
#define CND_LNEQ     176
//! Line # greater or equal to
#define CND_LNGE     177
//! First time through, (for headings)
#define CND_FIRST    178
//! Transaction rejected?
#define CND_REJ      179
//! Tip processing?
#define CND_TIP      180
//! Reprint?
#define CND_REPR     181
//! Pre-print?
#define CND_PREP     182
//! Signature required?
#define CND_SREQ     183
//! Roll printer in use?
#define CND_ROLL     184
//! Manually entered?
#define CND_MAN      185
//! Accepted, signature required?
#define CND_ACCSIG   186

//! Option 1
#define OPT1         192
//! Option 2
#define OPT2         193
//! Option 3
#define OPT3         194
//! Option 4
#define OPT4         195
//! Option 5
#define OPT5         196
//! Option 6
#define OPT6         197
//! Option 7
#define OPT7         198
//! Option 8
#define OPT8         199

#ifdef MAKE_FARSI
#define LNG_FARSI 200
#endif

//! Bitmasks corresponding to OPT1 through OPT8
#define OPT1MASK 0x01
#define OPT2MASK 0x02
#define OPT3MASK 0x04
#define OPT4MASK 0x08
#define OPT5MASK 0x10
#define OPT6MASK 0x20
#define OPT7MASK 0x40
#define OPT8MASK 0x80

//
//!      Print Line Justification Values
//

//! Left justifies text on a line
#define LJ_LEFT     1
//! Centers text on a line
#define LJ_CENTER   2
//! Right justifies text on a line
#define LJ_RIGHT    3


//
//!      The following #defines are used for field definitions
//

//
//!      ADDRESSING TYPES ENUMERATION
//

enum ADDRTYPE
{
    //! NOT AN ADDRESS
	A_NONE,
    //! ACTUAL ADDRESS
	A_ACT = 1,
    //! OFFSET INTO THE ACQUIRER TABLE
	A_ACQ,
    //! OFFSET INTO THE CARD TABLE
	A_CRD,
    //! OFFSET INTO THE ISSUER TABLE
	A_ISS,
    //! OFFSET INTO THE TRANSDEF TABLE
	A_TRN,
    //! OFFSET INTO THE BATCH TABLE
	A_BAT,
    //! DESCRIPTION TABLE ENTRY
	A_DSC,
    //! OFFSET INTO RTCBUF AFTER RD_RTC CALL
	A_RTC,
    //! OFFSET INTO TRANS. BUFFER (TRINP, ...)
	A_TR,
    //! CUSTOM ROUTINE (FOR DIFFER. REPORTS)
	A_CUS,
    //! OFFSET INTO THE VISA 2 TABLE
	A_V2
};

//
//!      EDIT TYPE (HOW DATA IS STORED) ENUMERATION
//

enum EDITTYPE
{
    //! NOT AN EDIT TYPE
	E_NONE,
    //! ASCII FIELD -- JUST MOVE THE FIELD
	E_CHR = 1,
    //! PAN EDIT
	E_PAN,
    //! BCD FIELD
	E_BCD,
    //! HEX Field 
    E_HEX,
    //! AMOUNT FIELD--RECEIPTS
	E_AMT1,
    //! AMOUNT FIELD--NORMAL AMOUNTS
	E_AMT2,
    //! AMOUNT FIELD--POSSIBLY NEGATIVE AMOUNT
	E_AMT3,
    //! ASC DATE FIELD IN YMD FORMAT
	E_ASCYMD,
    //! BCD DATE FIELD IN YMD FORMAT
	E_BCDYMD,
    //! EDIT ROUTINE FORMAT TRACK 1 NAME
	E_NAME,
    //! ASCII FIELD -- TRUNCATED TO PRT WIDTH
	E_CHRTRNC,
    //! EDIT ROUTINE FOR TIP
	E_TIPPCT,
#ifdef	MAKE_EMV
    //! VAR LEN HEX FIELD WITH LEN IN FIRST BYTE
	E_VARHEX,
#endif	// MAKE_EMV
	E_PREVENT_WARNING_ON_ARM
};

//
//!      FIELD FORMATS (AFTER EDITING) ENUMERATION
//

enum FLDFRMAT
{
    //! NOT A FORMAT
	F_NONE,
    //! CHARACTER FORMAT
	F_CHR = 1,
    //! AMOUNT FORMAT
	F_AMT,
    //! NUMERIC FORMAT
	F_NUM,
    //! DATE FORMAT
	F_DATE,
    //! TIME FORMAT
	F_TIME
};

//
//!      HOW TO TEST THE FIELD (FOR IF STATEMENTS)
//

//! NULLS
#define T_NULL      0
//! SPACE
#define T_SPC       ' '
//! NULLS OR SPACES
#define T_NOS       'N'
//! ALWAYS
#define T_ALWAYS    'A'
//! FLAG
#define T_FLAG      'F'
//! FF'S
#define T_FF        0xFF
//! FLAG Extended
#define T_FLAGX     'X'

//
//!      The following is a list of FORMATS that can be used
//!      to reformat a field.  Formats grouped and available by
//!      a fields original format.
//

//! CHR - LEFT JUSTIFY
#define FC_LEFT     0
//! CHR - RIGHT JUSTIFY
#define FC_RIGHT    1
//! CHR - CENTER
#define FC_CENTER   2
//! CHR - TRIM
#define FC_TRIM     3
//! CHR - CLEAR FF
#define FC_CLRFF    4

//! DATE - MMDDYY or MMYY
#define FD_MDY      10
//! DATE - DDMMYY or MMYY
#define FD_DMY      11
//! DATE - YYMMDD or YYMM
#define FD_YMD      12
//! DATE - MM/DD/YY or MM/YY
#define FD_MDYS     13
//! DATE - DD/MM/YY or MM/YY
#define FD_DMYS     14
//! DATE - YY/MM/DD or YY/MM
#define FD_YMDS     15
//! DATE - MON, DD YY or MON YY
#define FD_TEXT     16
//! TIME - MILITARY TIME HH:MM:SS
#define FT_MIL      21
//! TIME - STANDARD TIME HH:MM AM
#define FT_STDS     22
//! TIME - STANDARD TIME HH:MM:SS AM
#define FT_STDL     23

//! AMOUNT - TRIM
#define FA_TRIM     30
//! AMOUNT - FORCE POSITIVE
#define FA_POS      31
//! AMOUNT - FORCE NEGATIVE
#define FA_NEG      32
//! AMOUNT - SWITCH THE SIGN
#define FA_CHG      33
//! AMOUNT - FORCE POSITIVE $ TRIM
#define FA_TPOS     34
//! AMOUNT - FORCE NEGATIVE $ TRIM
#define FA_TNEG     35
//! AMOUNT - SWITCH THE SIGN $ TRIM
#define FA_TCHG     36
//! AMOUNT - SET ' DR' AT THE END
#define FA_TDR		37
//! AMOUNT - SET ' CR' AT THE END
#define FA_TCR		38
//! AMOUNT - SET ' CR' OR ' DR' INSTEAD OF SIGN
#define FA_TCRDR	39

//! NUMERIC - CHANGE LEADING ZEROS TO SPACES
#define FN_LDZ      40
//! NUMERIC - TRIM LEADING ZEROS
#define FN_TRIM     41

//! (set bounder up to 160 
#define MARKER1 159
//! set bounder up to 254 
#define MARKER2 254

//
//! Structure for Field Definition Table
//

struct FLDDEF
{
    //! ADDRESS OR OFFSET OF THE FIELD
	UBYTE *F_ADR;
    //! LENGTH OF THE FIELD OR INDEX
	UBYTE F_LEN;
    //! ADDRESSING TYPE OF THE FIELD
	enum ADDRTYPE F_ADRTYP;
    //! EDIT TYPE FOR THE FIELD
	enum EDITTYPE F_EDIT;
    //! FIELD TYPE
	enum FLDFRMAT F_TYPE;
    //! TEST TYPE FOR THE FIELD
	UBYTE F_TEST;
};

//! DEFINE STRUCTURE SIZE
#define F_SZE sizeof(struct FLDDEF)

//
//!      Field Table Enumeration
//

//! The order of the following enumeration should not be altered!!!
//! Otherwise, downloaded reports will not print properly.
//! Furthermore FLDTAB[] must also be in the same order.
//! Not that a range from 160 to 230 has been reserved
typedef enum 
{
	not_used,
	FLD_RCPT1,
	FLD_RCPT2,
	FLD_RCPT3,
	FLD_TID,
	FLD_MID,
	FLD_CNAME,
	FLD_SRVID,
	FLD_PAN,
	FLD_TRNNAME,
	FLD_EXPDT,
	FLD_BATNO,
	FLD_INVNO,
	FLD_SYSDT,
	FLD_SYSTM,
	FLD_TRNDT,
	FLD_TRNTM,
	FLD_RRN,
	FLD_AUTH,
	FLD_RSPC,
	FLD_TOTAL,
	FLD_AMT1,
	FLD_TIP,
	FLD_DCD1,
	FLD_DTX1,
	FLD_DCD2,
	FLD_DTX2,
	FLD_DCD3,
	FLD_DTX3,
	FLD_DCD4,
	FLD_DTX4,
	FLD_NAME,
	FLD_VISATID,
	FLD_RSPTXT,
	FLD_AQID,
	FLD_AQNAME,
	FLD_SERNO,
	FLD_TILLNO,
	FLD_SALECNT,
	FLD_SALEAMT,
	FLD_SALEVCNT,
	FLD_SALEVAMT,
	FLD_REFCNT,
	FLD_REFAMT,
	FLD_HOSTTEXT1,
	FLD_HOSTTEXT2,
	FLD_FEE,
	FLD_TOTF,
	FLD_CUSTOM1,
	FLD_CUSTOM2,
	FLD_CUSTOM3,
	FLD_CUSTOM4,
	FLD_AQNUM,
	FLD_PNAME,
	FLD_DMODE,
	FLD_PTTEL,
	FLD_PTTRY,
	FLD_PTCON,
	FLD_STTEL,
	FLD_STTRY,
	FLD_STCON,
	FLD_PSTEL,
	FLD_PSTRY,
	FLD_PSCON,
	FLD_SSTEL,
	FLD_SSTRY,
	FLD_SSCON,
	FLD_MODEM,
	FLD_SMODEM,

	FLD_V2MID,
	FLD_V2NUM,
	FLD_V2AQID,
	FLD_V2NAME,
	FLD_V2CITYST,
	FLD_V2STATE,
	FLD_V2BNUM,
	FLD_V2CAT,
	FLD_V2LOC,
	FLD_V21STLOC,
	FLD_V2SER,
	FLD_V2CITY,
	FLD_V2CTRY,
	FLD_V2SEC,
	FLD_V2CUR,
	FLD_V2ZONE,
	FLD_V2APPID,
	FLD_V2APPVER,
	FLD_V2STORE,
	FLD_V2TERM,

        FLD_TDBAL,
	FLD_TAX,
	FLD_BASEAMT,
	FLD_TAXAMT,
	FLD_TIPAMT,
	FLD_HSTPRDATA,
	FLD_HSTPRDATA1,
	FLD_HSTPRDATA2,
	FLD_AMT3,
	FLD_BARAMT,
	FLD_CASH,
	FLD_MSURC,
	FLD_AVSRSP,
	FLD_CASHCNT,
	FLD_CASHAMT,
	FLD_CUSTOM5,
	FLD_CUSTOM6,
	FLD_CUSTOM7,
	FLD_CUSTOM8,
	FLD_RCPTHDR4,
	FLD_SHIFT,
	FLD_TIPPCT,
	FLD_BUSDATE,
	FLD_ACHSVC,
	FLD_STAN,
	FLG_TCONF,
	FLG_ACQ,
	FLG_ISS,
	FLG_TRN,
	FLG_BAT,
	FLG_TRSTATUS,
	FLD_TRBATNO,
#ifdef MAKE_EMV
    FLD_AID,
    FLD_TC,
#endif

	FLD_MARKER = MARKER1,					// Must be less than 160
// Second range of fields
// The order of the following enumeration should not be altered!!!
	FLD_AMTDUE = 231,
	FLD_RCPTFTR1,
	FLD_RCPTFTR2,
	FLD_RCPTFTR3,
	FLD_RCPTFTR4,
	FLD_TIPPCT1,
	FLD_TIPPCT2,
	FLD_TIPPCT3,
	FLD_SUGTIP1,
	FLD_SUGTIP2,
	FLD_SUGTIP3,
	FLD_CSTMSG,
	FLD_CSTMSG2,
	FLD_CSTMSG3,
	FLD_CSTMSG4,
	FLD_CSTMSG5,
	FLD_CSTMSG6,
	FLD_MARKER2 = MARKER2					// Must be the last entry and less than 255
}ENFLDDEF;


//! The order of the following enumeration should not be altered!!! 
//! Otherwise, downloaded reports will not print properly.          
//! Furthermore FLDTAB[] must also be in the same order.            
//! Not that a range from 160 to 230 has been reserved			   


typedef enum 
{
   	not_used_ex = 0,			   
	FLD_TOTAPPROVAL,				   // PERFORMANCE REPORT
	FLD_TOTDENIALS,
	FLD_TOTMANUAL,
	FLD_TOTSWIPE,
	FLD_TOTCOUNT,
	FLD_TOTREDIALS,
	FLD_TOTTIMEOUT,
	FLD_TOTRETRANS,
    FLD_APPROV_PERC,
    FLD_DENIAL_PERC,
    FLD_MANUAL_PERC,
    FLD_SWIPE_PERC,
	FLD_FLOOR,
	FLD_ISREAUTH,
	FLD_ISID,
	FLD_NII,						   // ACQUIRER TABLE
	FLD_CURRBATCH,
	FLD_NEXTBATCH,
	FLD_MAX_SETTLE,
	FLD_ACQUID,
	FLD_CRNUM,
	FLD_CRPANL,
	FLD_CRPANH,
	FLD_CRISID,
	FLD_CRAQID,
	FLD_CRPLEN,
	FLG_CRD,
  

	FLD_MARKER_EX = MARKER1,					// Must be the last entry and less than 255
	FLD_PERIOD = 231,

	FLD_MARKER2_EX = MARKER2					// Must be the last entry and less than 255
} ENFLDEXT;



//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================


#endif	// _PRTDEF_H_

