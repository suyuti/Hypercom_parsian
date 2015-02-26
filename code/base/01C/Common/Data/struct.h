#ifndef _STRUCT_H_
#define _STRUCT_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             struct.h
//      Application structure definitions
//
//      This include file is used to declare all structures used
//      by the application.
//
//      Files that include this file will not need to specify
//      external references for the equates in this file
//
//=============================================================================

#include "stddef.h"

#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"

#include "transdef.h"
#include "defines.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================

typedef void (*XCALL_T)(void);
//
//-----------------------------------------------------------------------
// EQUATES ARE USED FOR VARIOUS FIELDS IN THE 'TRINP' STRUCTURE.
//-----------------------------------------------------------------------
//
#define MAXTRK3 107				// Max data elements in TRK3
#define MAXTRK2 37				// MAX DATA ELEMENTS IN TRK2
#define MAXTRK1 76				// MAX DATA ELEMENTS IN TRK1
#define MAXPANL 19				// MAX PAN LENGTH
#define MAXNAML 26				// MAX LENGTH OF NAME FROM TRK1
#define MINNAML 3				// MINIMUM NAME LENGTH FROM TRK1
#define TRK1SEP '^'				// TRACK 1 SEPARATOR
#define TRK2SEP '='				// TRACK 2 SEPARATOR
#define TRK3SEP '='				// TRACK 3 SEPARATOR
#define PINTXT  '2'				// text entered from PINPAD
//
//
//       TRANSACTION DATA INPUT DEFINITIONS
//
//       OPTION DEFINITIONS
//
//       OPTION 1
//
#define T1_NOTIP      0x01		// NO TIP AMOUNTS
#define T1_NOACCT     0x02		// NO ACCOUNT REQUIRED IN PROC CODE
#define T1_NORECEIPT  0x04		// NO RECEIPT REQUIRED
#define T1_NOCAPTURE  0x08		// NO CAPTURE REQUIRED
#define T1_NOTIME     0x10		// NO SYSTEM TIME UPDATE
#define T1_CREDIT     0x20		// CREDIT-TYPE TRANSACTION
#define T1_PREADV     0x40		// SEND ALL ADVICES PRIOR
#define T1_POSTADV    0x80		// SEND A SINGLE ADVICE AFTER

#define T2_SETTLE     0x01		// SETTLE TYPE TRANSACTION
#define T2_NOPREREV   0x02		// NO REVERSAL PRIOR
#define T2_OFFLINE    0x04		// TRANSACTION IS OFFLINE ENTERED
#define T2_FLOOR      0x08		// FLOOR LIMTS ALLOWED
#define T2_D8_SUPPORT 0x10		// D8 DATA INPUT PAD SUPPORTED
#define T2_COMPLETE   0x20		// SHOW TRANSACTION COMPLETE AFTER TRANS
#define T2_REFER      0x40		// REFERRAL ALLOWED
#define T2_REPRINT    0x80		// REPRINT ALLOWED (IF RECEIPT DISALLOWED)

#define T3_BATPEND    0x01		// SET BATCH PENDING FOR THIS TRAN
#define T3_NOISNAME   0x02		// NO ISSUER NAME IN TRANBUF
#define T3_CHK_IS_AQ  0x04		// USE CHK SERVICE ACQUIRER/ISSUER
#define T3_CHK_PEND   0x08		// CHECK BATPEND
#define T3_ACCEPTED   0x10		// SHOW TRANSACTION ACCEPTED AFTER TRANS
#define T3_NO_INIT    0x20		// IGNORE INIT REQUIRED FLAG
#define T3_REV        0x40		// REVERSAL REQUIRED FOR THIS TRANS
#define T3_TERMPARAM  0x80		// USE TERMINAL PARAMETERS FOR DIALING

#define T4_PINRES     0x01		// CHECK TO RESET PINPAD
#define T4_SPARE1     0x02		// NOT USED AT PRESENT
#define T4_CHKEXPD    0x04		// CHECK EXPIRY DATE
#define T4_NOCLRBAT   0x08		// DO NOT CLEAR HIDDEN BATCH
#define T4_NOUPLOAD   0x10		// DO NOT UPLOAD (320) AT SETTLE
#define T4_DEBCARD    0x40		// DEBIT CARD TRANSACTION
#define T4_NOPINDSP   0x80		// NO RESPONSE DISPLAY ON PINPAD
//
//       TERMINAL CONFIGURATION
//
#define TCMAX 1					// MAXIMUM NO OF TERMINAL CONFIGURATIONS
//
#define TC1_DUALAMT    0x01		// AMOUNT DUAL ENTRY
#define TC1_SHOWMSR    0x02		// DISPLAY TRACK 2
#define TC1_REST       0x04		// RESTAURANT PROCESSING
#define TC1_CASHIER    0x08		// CASHIER ENTRY ON
#define TC1_TILL       0x10		// TILL PROCESSING  0-NO 1-YES
#define TC1_PR_TIME    0x20		// PRINT TIME ON RECEIPT
#define TC1_PINDETECT  0x40		// DETECT PINPAD AT START UP
#define TC1_RECON      0x80		// 0-ENTER, 1-CONFIRM
//
#define TC2_GRAPHMODE	0x01	// GRAPHIC MODE PRINTING
#define TC2_PRINT       0x02	// PRINTER CONFIGURED
#define TC2_PRTTOT      0x04	// 0-PRINT "TOTAL" 1-DONT PRINT "TOTAL"
#define TC2_FONTUPD		0x08	// UPDATE PRINTER FONT
#define TC2_REF_OFF     0x10	// REFUNDS OFFLINE? 0-NO 1-YES
#define TC2_VOID_OFF    0x20	// VOIDS OFFLINE?   0-NO 1-YES
#define TC2_REMAP		0x40	// PRINTER REMAPING ALLOW
#define TC2_CASHONLY	0x80	// CASH ONLY TERMINAL

#define TC3_PRTVER      0x08	// 1- PRINT CARD VERIFY
#define TC3_IMPRINT     0x10	// 1- TAKE IMPRINT ON MANUAL ENTRY
#define TC3_CHKREAD     0x20	// 1- CHECK READER INSTALLED

#define TC4_SRV_ADJ     0x01	// ADJUST BY CASHIER ONLY
#define TC4_SRV_PRM     0x02	// PROMPT FOR SERVER NOT CASHIER
#define TC4_SRV_BAT     0x04	// BATCH REVIEW BY CASHIER ONLY
#define TC4_TENDER      0x08	// REQUEST AMT TENDER INSTEAD OF CASH AMT
#define TC4_DUPL        0x20	// ENABLE CHECKING FOR DUPLICATE TRANSCTN
#define TC4_TRAINING    0x40	// ENABLED IF TRAINING MODE
#define TC4_INTRRFID	0x80	// USE INTERNAL RFID

#define TC5_HSMENU      0x01	// DO HOST SELECT USING MENU WHERE APPLIC
#define TC5_NOHSPRM     0x02	// NO HOST SELECT PROMPT DURING SETTLEMNT
#define TC5_CLOGOFF     0x04	// LOG OFF ALL CASHIERS AT SETTLEMENT
#define TC5_CALCTIP     0x08	// CALCULATE TIP FROM TOTAL
#define TC5_SHIFT       0x10	// SHIFT PROCESSING ENABLED
#define TC5_NOTIP       0x20	// DISABLE TIP AT INITIAL SALE

#ifdef	MAKE_EMV
#define TC6_PINBYPASSTERM	0x01// ALLOW PIN BYPASS (from terminal)
#define TC6_PINBYPASSPIN	0x02// ALLOW PIN BYPASS (from pinpad)
#define	TC6_ICCEARLYAMT		0x10// EARLY AMOUNT ENTRY FOR ICC TXNS
#endif	// MAKE_EMV

#define TC6_TRUNCPAN    0x20	// TRUNCATE PAN ON CREDIT
#define TC6_NOSECOND    0x40	// SUPRESS SECOND RECEIPT
#define TC6_LONGTO		0x80	// LONG KEY PRESS TIMEOUT (255 vs 30 secs)

#define TC7_CONFIG		0x10	// CONFIGURATION REPORT
#define TC7_PERFORMANCE	0x40	// PERFORMANCE REPORT
#define TC7_PERMN_PPP	0x80	// permanent PPP link enabled

//                                                                              
//  THE FOLLOWING OPTIONS APPLY TO TCPINPAD                                     
//  THEY DESCRIBE THE CAPABILITY OF THE PINPAD.                                 
//                                                                              
#define TCP_MSR	    	0x01	// CARD READER ATTACHED
#define TCP_DUKPT   	0x02	// DUKPT ENCRYPTION (ELSE MASTER SESSION)
#define TCP_RESV    	0x04	// RESERVED TO AVOID CONFLICTS WITH OLD CONFIG VALUES
#define TCP_SMCARD  	0x10	// SMART CARD ENABLED
//                                                                              
//  THE FOLLOWING OPTIONS ARE FOR USE WITH THE TCP_DSPMASK RESULTS              
//                                                                              
#define TCPDSP_2X20 	0x00	// 2 x 20 DISPLAY
#define TCPDSP_4X20 	0x20	// 4 x 20 GRAPHICAL DISPLAY
#define TCPDSP_ICE  	0x40	// 160 x 80 ICE GRAPHICAL DISPLAY
//
//       THE FOLLOWING OPTIONS APPLY TO TCDOPT
//
#define TCD_ENH         0x04	// ENHANCED DISPLAY MODE
//
//       THE FOLLOWING OPTIONS APPLY TO TCLOCOP
//
#define TCL_LOCKED      0x01	// KEYBOARD LOCKED
#define TCL_NO_VPWD     0x02	// NO VOID PASSWORD
#define TCL_NO_RPWD     0x04	// NO REFUND PASSWORD
#define TCL_NO_APWD     0x08	// NO ROC SEARCH/ADJUST PASSWORD
#define TCL_NO_RPPW     0x10	// NO REPORT PASSWORD
#define TCL_SPARE5      0x20    // BIT-5 SPARE
#define TCL_SPARE6      0x40	// BIT-6 SPARE
#define TCL_SPARE7      0x80	// BIT-7 SPARE
//
//                                                          
#define S_TCDATE            3
#define S_TCTIME            3
#define S_TCPWD             2
#define S_TCHTEL            12
#define S_TCNAM1            40
#define S_TCNAM2            40
#define S_TCNAM3            40

#define S_TCLEADC           3
#define S_TCCSHBKLIMIT      2
#define S_TCCHECKF          3
#define S_TCMERDBS          2
#define S_TCACHCHECKF       3
#define S_TCACHSVC          2
#define S_TCPHCARDPR        2
#define S_TCMPWD            6
#define S_TCRESV            10

#define S_TCCURRCODE        2
#define S_TCCTRYCODE        2
#define S_TCEMVTYPE         2
#define S_TCTCAP            3
#define S_TCADDTCAP         5
#define S_TCADMINCRD	(MAXTRK2+1)/2
#define S_TCAPPCAP          3

struct termconf_rec
{
	UBYTE TCDLL;				// DOWNLINE LOAD REVISION
	UBYTE TCINIT;				// INITIALIZATION CONTROL
	UBYTE TCPRBAUD;				// PRINTER BAUD RATE
	UBYTE TCECRBAUD;			// ECR PORT BAUD RATE
	UBYTE TCDATE[S_TCDATE];		// DATE YYMMDD
	UBYTE TCTIME[S_TCTIME];		// TIME HHMMSS
	UBYTE TCDOPT;				// TELEPHONE DIAL OPTIONS
	UBYTE TCPWD[S_TCPWD];		// MERCHANT PASSWORD
	UBYTE TCHTEL[S_TCHTEL];		// HELP DESK TELEPHONE NUMBER
	UBYTE TCOPT1;				// OPTIONS BYTE 1
	UBYTE TCOPT2;				//         BYTE 2
	UBYTE TCOPT3;				//         BYTE 3
	UBYTE TCOPT4;				//         BYTE 4
	UBYTE TCNAM1[S_TCNAM1];		// MERCHANT NAME 1
	UBYTE TCNAM2[S_TCNAM2];		// MERCHANT NAME 2
	UBYTE TCNAM3[S_TCNAM3];		// MERCHANT NAME 3
	UBYTE TCLEADC[S_TCLEADC];	// CURRENCY LEADING SYMBOL
	UBYTE TCDLEN;				// CURRENCY ENTRY DIGITS - TRANSACTNS
	UBYTE TCDPP;				// CURRENCY DECIMAL POINT POSITION
	UBYTE TCSDLEN;				// CURRENCY ENTRY DIGITS - SETTLEMENT
	UBYTE TCCHECK;				// CHECK SERVICE
	UBYTE TCCHKISID;			// CHECK TRANS ISSUER ID
	UBYTE TCCHKAQID;			// CHECK TRANS ACQUIRER ID
	UBYTE TCLOCOP;				// LOCAL TERMINAL OPTIONS
	UBYTE TCPINPAD;				// PINPAD TYPE
	UBYTE TCPRINTER;			// PRINTER TYPE
	UBYTE TCCSHBKLIMIT[S_TCCSHBKLIMIT];	// CASH BACK LIMIT
	UBYTE TCCHECKF[S_TCCHECKF];	// CHECK SERVICE PROMPTS BIT FLAGS
	UBYTE TCMERDBS[S_TCMERDBS];	// MERCHANT DEBIT SURCHARGE AMT (BCD)
	UBYTE TCOPT5;				// OPTIONS BYTE 5
	UBYTE TCOPT6;				// OPTIONS BYTE 6
	UBYTE TCACHCHECKF[S_TCACHCHECKF];	// ACH CHECK SERVICE PROMPTS BIT FLAGS
	UBYTE TCACHSVC[S_TCACHSVC];	// RETURNED CHECK SERVICE CHARGE (ACH)
	UBYTE TCMPWD[S_TCMPWD];		// MAINTENANCE PASSWORD
	UBYTE TCTIPPCT_1;			// SUGGESTED TIP PERCENT #1
	UBYTE TCTIPPCT_2;			// SUGGESTED TIP PERCENT #2
	UBYTE TCTIPPCT_3;			// SUGGESTED TIP PERCENT #3
	UBYTE TCOPT7;				// OPTIONs BYTE 7
	UBYTE TCADMCRDRESV[S_TCADMINCRD];	/* Reserved */
    UBYTE TCCURRCODE[S_TCCURRCODE];     /* LOCAL CURRENCY CODE (BCD)           */
    UBYTE TCCTRYCODE[S_TCCTRYCODE];     /* LOCAL COUNTRY CODE (BCD)            */

#ifdef MAKE_EMV
    UBYTE TCEMVTYPE[S_TCEMVTYPE];       /* EMV TERMINAL TYPE (Ascii)           */
	UBYTE TCTCAP[S_TCTCAP];				/* TERMINAL CAPABILITIES			   */
	UBYTE TCADDTCAP[S_TCADDTCAP];		/* ADDITIONAL TERMINAL CAPABILITIES	   */
	UBYTE TCAPPCAP[S_TCAPPCAP];			// application capabilities
#else
	UBYTE TCEMVRESV[S_TCEMVTYPE+S_TCTCAP+S_TCADDTCAP+S_TCAPPCAP];	/* RESERVED	   */
#endif
	UBYTE TCTHRESLEVEL; 				/* THRESHOLD LEVEL*/
	UBYTE TCTHRESTIME; 					/* THRESHOLD TIME in second*/

	UBYTE TCRESV[107];						// RESERVED
} __attribute__((packed));

#define TCSZE sizeof(struct termconf_rec)	// DEFINE STRUCTURE SIZE

//
//       CARD RANGE TABLE ENTRY
//
#define CRMAX           254

#define	CR1_2DIGISSN	0x08		/* 2 DIGITS ISSUE NUMBER    */
#define CR1_SPARE4		0x10		/* BIT 4-SPARE				*/
#define CR1_SPARE5		0x20		/* BIT 5-SPARE				*/
#define CR1_SPARE6		0x40		/* BIT 6-SPARE				*/
#define CR1_SPARE7		0x80	// BIT 7-SPARE

#define CR2_SPARE2		0x04	// BIT 2-SPARE
#define CR2_SPARE3		0x08	// BIT 3-SPARE
#define CR2_SPARE4		0x10	// BIT 4-SPARE
#define CR2_SPARE5		0x20	// BIT 5-SPARE
#define CR2_SPARE6		0x40	// BIT 6-SPARE
#define CR2_SPARE7		0x80	// BIT 7-SPARE

// FIELD SIZES
#define S_CRPANL        5
#define S_CRPANH        5

struct card_rec
{
	UBYTE CRNUM;				// CARD RANGE NUMBER (BIN, not BCD!!!)
	UBYTE CRPANL[S_CRPANL];		// PAN RANGE LOW
	UBYTE CRPANH[S_CRPANH];		// PAN RANGE HIGH
	UBYTE CRISID;				// ISSUER ID
	UBYTE CRAQID;				// ACQUIRER ID
	UBYTE CRPLEN;				// PAN LENGTH
	UBYTE CROPT1;				// OPTIONS BYTE 1
	UBYTE CRISID2;				// ISSUER ID 2
	UBYTE CRAQID2;				// ACQUIRER ID 2
	UBYTE CRINPOS; 					/* ISSUE NUMBER POSITION (FOR SWITCH CARDS) */
									/* 0 = NO ISSUE NUMBER ON TRACK 2           */
	UBYTE CRVPPOS; 					/* VALIDITY NUMBER START POSTION            */
} __attribute__((packed));

#define CRSZE sizeof(struct card_rec)	// DEFINE STRUCTURE SIZE

//
//       ISSUER TABLE ENTRY
//
#define ISMAX   16

#define IS1_PIN         0x02	// IS PIN SELECT REQUIRED
#define IS1_MANPAN      0x04	// IS MANUAL PAN ALLOWED
#define IS1_EXPD        0x08	// IS EXPIRY DATE ENTRY REQUIRED
#define IS1_OFFL        0x10	// IS OFFLINE ENTRY ALLOWED
#define IS1_REFER       0x20	// ARE VOICE REFERRALS ALLOWED
#define IS1_DESC        0x40	// ARE DESCRIPTORS REQUIRED
#define IS1_ADJ         0x80	// IS ADJUST ALLOWED

#define IS2_MOD10       0x01	// IS MOD-10 CHECK REQUIRED
#define IS2_ROC         0x02	// IS ROC NUMBER REQUIRED
#define IS2_RECEIPT     0x04	// IS RECEIPT REQUIRED
#define IS2_CAPTURE     0x08	// CAPTURE CARD RANGE
#define IS2_CH_EXPD     0x10	// CHECK EXPIRY DATE ?
#define IS2_NOREF       0x40	// REFUNDS ARE BLOCKED
#define IS2_NOCARDVER   0x80	// CARDVERS ARE BLOCKED

#define IS3_NOVOID      0x01	// VOIDS ARE BLOCKED
#define IS3_CASHAMT     0x02	// PROMPT FOR CASH AMOUNT
#define IS3_NOSIGNAT    0x04	// PRINT 'SIGNATURE NOT REQUIRED' ON RECEIPT
#define IS3_NOREFPRT    0x08	// PRINT '***NO REFUNDS ALLOWED' ON RECEIPT
#define IS3_REF_OFF     0x10	// REFUNDS OFFLINE? 0-NO 1-YES
#define IS3_VOID_OFF    0x20	// VOIDS OFFLINE?   0-NO 1-YES
#define IS3_CASH_TRN    0x40	// ALLOW CASH ONLY TRANS?
#define IS3_AUTH_FL6    0x80	// GENERATE 6 DIGIT AUTH FOR FLOOR LIMIT

#ifdef	MAKE_EMV
#define IS4_ICCPOSTCOMMS	0x01	/* POST-COMMS PROCESSING FOR ICC TXNS	*/
#endif	// MAKE_EMV
#define IS4_PANLAST4    0x02	// ENTER LAST 4 PIN DIGITS ON CARD SWIPE
#define IS4_TRK1ERR     0x10	// IGNORE TRACK 1 ERROR
#define IS4_TAX         0x20	// PROMPT FOR TAX AMOUNT
#ifdef   MAKE_EMV
#define IS4_APPTEXT     0x40   // PRINT APPLICATION TEXT FOR ICC TXNS
#endif   // MAKE_EMV
#ifdef MAKE_SIGCAPTURE
#define IS4_SIGCAP      0x80	// M4100. SIGNATURE REPRODUCTION/CAPTURE ENABLED
#endif // MAKE_SIGCAPTURE

#define IS5_PANTRUNC	0x01        /* PAN TRUNCATION ON CUSTOMER COPY	*/
#define IS5_SPARE1     	0x02        /* 1 - SPARE BIT                        */
#define IS5_REFAMTPR    0x08	// PROMPT FOR EACH AMOUNT ON REFUND
#define IS5_BAR         0x20	// PROMPT FOR BAR AMOUNT
#define IS5_ACHCHECK    0x40	// ACH CHECKS ISSUER
#define	IS5_CVV2	    0x80	// ENABLE CVV2 PROCESSING

#define IS6_ALLOWAVS     0x01	// ALLOW ADDRESS VERIFICATION
#define IS6_MAILADDR_REQ 0x02	// CARDHOLDER MAIL ADDRESS REQUIRED
#define IS6_ENASERV		 0x04	// ENABLED THE SEVICE CODE CHECKING
#define IS6_STARTD		 0x08  	/* REQUIRED ENTRY START DATE */
#define IS6_CH_STARTD	 0x10  	/* START DATE CHECKING */
#define IS6_AVSZIPCD_REQ 0x20	// CARDHOLDER ZIPCODE REQUIRED
#define IS6_ERC			 0x80	// ELECTRONICALLY CAPTURE RECEIPT

//
// THE FOLLOWING OPTIONS APPLY TO ISPINTRAN:
//
#define ISPIN_SALE      0x01	// SALE DISABLES PIN ENTRY
#define ISPIN_RETURN    0x02	// RETURN
#define ISPIN_VOID      0x04	// VOID
#define ISPIN_ADJUST    0x08	// ADJUST
#define ISPIN_CASHBK    0x10	// CASHBACK
#define ISPIN_BALINQ    0x20	// BALANCE INQUIRY

//
// THE FOLLOWING OPTIONS APPLY TO ISAP :
//
#define ISAP_1          0x01	// ADDITIONAL PROMPT 1
#define ISAP_2          0x02	// ADDITIONAL PROMPT 2
#define ISAP_3          0x04	// ADDITIONAL PROMPT 3
#define ISAP_4          0x08	// ADDITIONAL PROMPT 4
#define ISAP_5          0x10	// ADDITIONAL PROMPT 5
#define ISAP_6          0x20	// ADDITIONAL PROMPT 6
#define ISAP_7          0x40	// ADDITIONAL PROMPT 7
#define ISAP_8          0x80	// ADDITIONAL PROMPT 8


// FIELD SIZES
#define S_ISNAME        10
#define S_ISRTEL        12
#define S_ISFLOOR       2
#define S_ISCASHFLOOR   2

struct issuer_rec
{
	UBYTE ISNUM;				// ISSUER TABLE ENTRY NUMBER
	UBYTE ISID;					// ISSUER ID
	UBYTE ISNAME[S_ISNAME];		// ISSUER NAME
	UBYTE ISRTEL[S_ISRTEL];		// ISSUER REFERRAL TEL NO
	UBYTE ISOPT1;				// OPTION BYTE 1
	UBYTE ISOPT2;				// OPTION BYTE 2
	UBYTE ISOPT3;				// OPTION BYTE 3
	UBYTE ISOPT4;				// OPTION BYTE 4
	UBYTE ISDEFAC;				// DEFAULT ACCOUNT
	UBYTE ISFLOOR[S_ISFLOOR];	// FLOOR LIMIT
    UBYTE ISCSHFLRRESV[S_ISCASHFLOOR]; /* RESERVED */
	UBYTE ISREAUTH;				// REAUTHORIZE MARGIN
	UBYTE ISPINTRAN;			// TRANSACTIONS REQUIRING PIN ENTRY
	UBYTE ISAP;					// ADDITIONAL PROMPTS FOR THIS ISSUER
	UBYTE ISOPT5;				// OPTION BYTE 5
	UBYTE ISOPT6;				// OPTION BYTE 6
	UBYTE ISRESV[25];           // RFU

} __attribute__((packed));

#define ISSZE sizeof(struct issuer_rec)	// DEFINE STRUCTURE SIZE

//
//       ACQUIRER TABLE ENTRY
//
#define AQMAX           6

#define	AQ1_USEIP       0x02	// Use IP communications (instead of dial)
#define AQ1_NOTIME      0x08	// IGNORE HOST TIME

#ifdef	MAKE_EMV
#define AQ1_PREVAMT		0x80        /* USE PREVIOUS AMOUNT FOR EMV TERM. RISK MANAGEMENT */
#endif	// MAKE_EMV

#define AQ2_TPDU	    0x01        /* SEND TPDU IN MESSAGE  */
#define AQ2_STXETX		0x02        /* USE STX/ETX FRAMING ON APPLICATION LEVEL */
#define AQ2_SENDBATNO   0x10	// SEND BATCH NO IN 01X0 & 02X0 MESSAGES
#define AQ2_PINKEYEXCM  0x20	// PIN KEY EXCHANGE METHOD: 0=TNMS 1=HOST
#define AQ2_MACKEYEXCM  0x40	// MAC KEY EXCHANGE METHOD: 0=TNMS 1=HOST

#ifdef	MAKE_EMV
#define AQ2_ICCACQ      0x80	// CHIP CARD ACQUIRER
#endif	// MAKE_EMV

#define AQ3_HOLDADV     0x01	// HOLD ADVICES UNTIL SETTLEMENT
#define AQ3_HSEXCL      0x04	// EXCLUDE FROM HOST SELECT WHERE APPLIC
#define AQ3_IMITDIAL	0x08	// IMITATE DIALING FOR OFFLINE TRANSACTION

#define	AQ4_MAC			0x02		// ENABLE MAC CALCULATION      
#define AQ4_DUKPT		0x04		// ENABLE DUKPT PIN CALCULATION
#define AQ4_DUKPTMAC	0x08		// ENABLE DUKPT MAC CALCULATION
#define AQ4_PREPRINT	0x10		// ENABLE PREPRINT             
#define AQ4_PREDIAL		0x20		// ENABLE PREDIAL               
#define AQ4_DBLPINKEY	0x40		// ENABLE DOUBLE WORKING PIN KEY
#define AQ4_DBLMACKEY	0x80		// ENABLE DOUBLE WORKING MAC KEY


#define	AQ5_TRANS_PPP			0x01		// Use PPP protocol for transaction
#define AQ5_SETTLE_PPP			0x02		// Use PPP protocol for settlement
#define AQ5_DBPHONE1			0x04		// Use Dial backup
#define AQ5_UNUSED1 			0x08		// Unused 
#define AQ5_UNUSED2     		0x10		// Unused
#define AQ5_UNUSED3		        0x20		// Unused
#define AQ5_USEHLAN				0x40        // Use HLAN communications (instead of IP & dial)
#define AQ5_USEGPRS				0x80		// Use GPRS communications

// FIELD SIZES
#define S_AQPNAME       10
#define S_AQNAME        10
#define S_AQPTTEL       12
#define S_AQSTTEL       12
#define S_AQPSTEL       12
#define S_AQSSTEL       12
#define S_AQNII         2
#define S_AQTID         8
#define S_AQACCID       15
#define S_AQBATCUR      3
#define S_AQBATNXT      3
#define S_AQSETLTIME    4
#define S_AQSETLDAY     2
#define S_AQWORKKEY     8
#define S_AQMACKEY      8
#define S_AQVISATID     23
#define S_AQSPNAME      10
#define S_AQSCUADDRESS  3



#define	MAP_2BYTE	0			// 2-byte length delimited messages
#define	MAP_HTTP	1			// HTTP delimited messages

#define	S_IPADDRESS	15			// IP address ASCII length.
#define S_TCPPORT	5			// TCP Port number


typedef struct
{
	UBYTE phoneNum[S_AQPSTEL];	// Telephone number (BCD)
	UBYTE timeout;				// Timeout (BCD)
	UBYTE trys;					// Attempts (BCD)
} __attribute__((packed))
PHONE_STRUCT;

typedef struct
{
	PHONE_STRUCT primary;		// Primary Transaction data
	PHONE_STRUCT secondary;		// Secondary Transaction data
} __attribute__((packed))
PS_DIAL_STRUCT;

typedef struct
{
	PS_DIAL_STRUCT trans;		// Primary Settlement data
	PS_DIAL_STRUCT settle;		// Secondary Settlement data
	UBYTE aqModem;				// Modem mode
} __attribute__((packed))
DIAL_DATA_STRUCT;

typedef struct
{
	char url[48];				// IP address or Domain Name (Null terminated)
	char port[6];				// Port number (Null terminated)
	UBYTE timeout;				// Timeout (BCD)
	UBYTE trys;					// Attempts (BCD)
	UBYTE messageProtocol;		// Message Application Protocol
} __attribute__((packed))
IP_DATA_STRUCT;

struct acquirer_rec
{
	UBYTE AQNUM;				// ACQUIRER TABLE ENTRY NUMBER
	UBYTE AQID;					// ACQUIRER ID
	UBYTE AQPNAME[S_AQPNAME];	// PROGRAM NAME FOR HOST COMMS
	UBYTE AQNAME[S_AQNAME];		// NAME FOR PRINTING DISPLAY
	union
	{
		DIAL_DATA_STRUCT dial;	// DIAL CONNECTION DATA
		IP_DATA_STRUCT ipTrans;	// IP CONNECTION DATA
	} __attribute__((packed))
	conn;
	UBYTE AQOPT1;				// OPTION BYTE 1
	UBYTE AQOPT2;				// OPTION BYTE 2
	UBYTE AQOPT3;				// OPTION BYTE 3
	UBYTE AQOPT4;				// OPTION BYTE 4
	UBYTE AQOPT5;				// OPTION BYTE 5
	UBYTE AQNII[S_AQNII];		// NII
	UBYTE AQTID[S_AQTID];		// TERMINAL ID
	UBYTE AQACCID[S_AQACCID];	// ACCEPTOR ID
	UBYTE AQTOUT;				// HOST RESPONSE TIME OUT (SECS)
	UBYTE AQBATCUR[S_AQBATCUR];	// NEW CURRENT BATCH NUMBER
	UBYTE AQBATNXT[S_AQBATNXT];	// NEW NEXT BATCH NUMBER
	UBYTE AQSETLTIME[S_AQSETLTIME];	// AUTO-SETTLEMENT TIME
	UBYTE AQSETLDAY[S_AQSETLDAY];	// AUTO-SETTLEMENT DAY
	UBYTE AQWORKKEY[S_AQWORKKEY + S_AQWORKKEY];	// PIN WORKING KEY
	UBYTE AQMKID;				// PIN MASTERKEY ID
	UBYTE AQMACKEY[S_AQMACKEY + S_AQMACKEY];	// MAC KEY
	UBYTE AQMACID;				// MAC KEY ID
	UBYTE AQVISATID[S_AQVISATID];	// VISA 1 TERMINAL ID
	UBYTE AQSMODEM;				// MODEM MODE FOR SETTLEMENT
	UBYTE AQSMAXTRIES;			// MAX SETTLE TRIES (0=FOREVER)
	UBYTE AQDMODE;				// ISDN PROTOCOL (V.110/V.120 etc.)
	UBYTE AQ_RFU1[2];           // Reserved for future use
	UBYTE AQ_RFU2[10];          // Reserved for future use
	UBYTE AQRESV[80];           // Reserved for future development
	UBYTE AQ_TRANS_P_IPTAB;		// Primary Transaction IP table index
	UBYTE AQ_TRANS_P_CON;		// Primary Transaction IP connection time
	UBYTE AQ_TRANS_S_IPTAB;		// Secondary Transaction IP table index
	UBYTE AQ_TRANS_S_CON;		// Secondary Transaction IP connection time
	UBYTE AQ_SETTLE_P_IPTAB;	// Primary Settlement IP table index
	UBYTE AQ_SETTLE_P_CON;		// Primary Settlement IP connection time
	UBYTE AQ_SETTLE_S_IPTAB;	// Secondary Settlement IP table index
	UBYTE AQ_SETTLE_S_CON;		// Secondary Settlement IP connection time
	UBYTE AQMODEM2;				// MODEM MODE FOR SECOND phone number
	UBYTE AQPPPMODE1;			// PPP mode for 1 phone number
	UBYTE AQPPPMODE2;			// PPP mode for 2 phone number
	UBYTE AQSMODEM2;			// MODEM MODE FOR SECOND phone number in settlement
	UBYTE AQSPPPMODE1;			// PPP mode for 1 phone number in settlement
	UBYTE AQSPPPMODE2;			// PPP mode for 2 phone number in settlement
    UBYTE AQSCUADDRESS[S_AQSCUADDRESS]; // CU address for HLAN connection

} __attribute__((packed));

#define AQSZE sizeof(struct acquirer_rec)	// DEFINE STRUCTURE SIZE

//-----------------------------------------------------------------------
//
//       DESCRIPTOR TABLE ENTRY
//
#define DSMAX           10		// MAXIMUM DESCRIPTORS

// FIELD SIZES
#define S_DSCODE        2
#define S_DSTXT         20

struct desc_rec
{
	UBYTE DSNUM;				// ENTRY NUMBER
	UBYTE DSKEY;				// SELECTION KEY
	UBYTE DSCODE[S_DSCODE];		// CODE TO SEND TO HOST
	UBYTE DSTXT[S_DSTXT];		// TXT FOR DISPLAY/PRINTING
} __attribute__((packed));

#define DSSZE sizeof(struct desc_rec)	// DEFINE STRUCTURE SIZE

// The following was added to support T5000/ICE.

//
//       IDLE SCREEN BUTTONS TABLE
//
#define S_BTNTRNLEN		2
#define S_BTNLBLLEN     10

struct idlescr_rec
{
	UBYTE IBTRANBTN1[S_BTNTRNLEN];	// TRANSACTION BUTTON ONE
	UBYTE IBBTN1LABEL1[S_BTNLBLLEN];	// LABEL FOR BUTTON 1, LINE 1
	UBYTE IBBTN1LABEL2[S_BTNLBLLEN];	// LABEL FOR BUTTON 1, LINE 2
	UBYTE IBTRANBTN2[S_BTNTRNLEN];	// TRANSACTION BUTTON TWO
	UBYTE IBBTN2LABEL1[S_BTNLBLLEN];	// LABEL FOR BUTTON 2, LINE 1
	UBYTE IBBTN2LABEL2[S_BTNLBLLEN];	// LABEL FOR BUTTON 2, LINE 2
	UBYTE IBTRANBTN3[S_BTNTRNLEN];	// TRANSACTION BUTTON THREE
	UBYTE IBBTN3LABEL1[S_BTNLBLLEN];	// LABEL FOR BUTTON 3, LINE 1
	UBYTE IBBTN3LABEL2[S_BTNLBLLEN];	// LABEL FOR BUTTON 3, LINE 2
	UBYTE IBTRANBTN4[S_BTNTRNLEN];	// TRANSACTION BUTTON FOUR
	UBYTE IBBTN4LABEL1[S_BTNLBLLEN];	// LABEL FOR BUTTON 4, LINE 1
	UBYTE IBBTN4LABEL2[S_BTNLBLLEN];	// LABEL FOR BUTTON 4, LINE 2
} __attribute__((packed));

#define IBSZE sizeof(struct idlescr_rec)	// DEFINE STRUCTURE SIZE


struct localvalues_rec
{
	Bool IBTAB_INIT;			// IBTAB Table Init Flag- Idle Button Screen Function
	UBYTE LANGID;		// Language Identification
} __attribute__((packed));

#define LOCSZE sizeof(struct localvalues_rec)	// DEFINE STRUCTURE SIZE


//
//       TRANSACTION DATA
//
//
//-----------------------------------------------------------------------
// EQUATES ARE USED IN THE FIELD 'TRSTATUS' IN THE 'TRINP' STRUCTURE.
//-----------------------------------------------------------------------
//
#define TS1_SETL        0x01	// HOST SETTLED
#define TS1_TIPENT      0x02	// TIP ENTERED
#define TS1_ADV         0x04	// NEED TO SEND ADVICE TO HOST
#define TS1_REV         0x08	// THIS IS A REVERSAL
#define TS1_VOID        0x10	// TRANSACTION VOIDED
#define TS1_ADJ         0x40	// TRANSACTION ADJUSTED
#define TS1_TRAIN       0x80	// TRAINING MODE TRANSACTION
//
#define TS2_AVSVOID		0x01	// AVS FAILED AND TRANSACTION NOT CAPTURED
#define TS2_PINBLOCK	0x02	/* Encrypted PIN Block present */
#define TS2_NOSALCASH	0x04		/* SALE + CASH DISABLED			*/
#define TS2_AMEXANSI	0x08		/* AMEX ANSI CARD DETECTED (NO SERVICE CODE) */

#define TS2_FALLBACK    0x20        /* Performing ICC Fallback */
#define TS2_REJECT	    0x40        /* Transaction should be rejected */
#define TS2_ERCSENT	    0x80	// ELECTRONIC RECEIPT SENT TO SIG CAP HOST

//
//-----------------------------------------------------------------------
// EQUATES ARE USED IN THE FIELD 'TRMICRfmt' IN THE 'TRINP' STRUCTURE.
//-----------------------------------------------------------------------
//
#define TM_RDR  0
#define TM_MICR	1
#define TM_ACCT	2
//
//-----------------------------------------------------------------------
// STRUCTURE TO HOLD TRANSACTION DATA DURING INPUT, REQUEST BUILDING AND
// RESPONSE PARSING.
// THE VARIABLES DEFINED ARE 'TRINP', 'TRREQ' AND 'TRRSP'.
//-----------------------------------------------------------------------
//
#define S_TRTRANBUF         20
#define S_TRRSPTXT          20
#define S_TRPAN             10	// = (MAXPANL+1)/2
#define S_TREXPD            2
#define S_TRSTARTD          2
#define S_TRPWD             6
#define S_TRBASE            6
#define S_TRTIP             6
#define S_TRTOTAM           6
#define S_TRDESC            4
#define S_TRINV             6
#define S_TRMICR            22
#define S_TRTRANSIT         10
#define S_TRTCNO            6
#define S_TRZIP                 9
#define S_TRSTOT            6
#define S_TRCTOT            6
#define S_TRAUTH            6
#define S_TRBATNO           6
#define S_TRPROC            3
#define S_TRSTAN            3
#define S_TRYEAR            1
#define S_TRDATE            2
#define S_TRTIME            3
#define S_TRPOSE            2
#define S_TRNII             2
#define S_TRTRK2            (MAXTRK2+1)
#define S_TRRRN             12
#define S_TRRSPC            2
#define S_TRTID             8
#define S_TRACCID           15
#define S_TRRSPA            2
#define S_TRTRK1            (MAXTRK1+1)
#define S_TRMTYPE           2
#define S_TRBMAP            8
#define S_TRTPDU            5
#define S_TRNAME            26
#define S_TRSTATUS          2
#define S_TRHOSTTEXT        40
#define S_TRCASHIER         4
#define S_TRTILLNO          4
#define S_TRPINBL           8
#define S_TRDEBIT_SETLDATE      4
#define S_TRSOC             6
#define S_TRBUSDT           3
#define S_TRTRANID          15
#define S_TRVALID           4
#define S_TRHSTMSGID        8
#define S_TRV2RESP              2
#define S_TRAUTHAMT         6
#define S_TRREFDATA         54
#define S_TRAP              35
#define S_TRBSEQ            2
#define S_TRKSN             10
#define S_TRVOUCHER			15
#define S_TRPONUM               20
#define S_TRAVSADDR         24
#define S_TRSHIFT           1
#define S_TRCVV2			4
#define S_TROPTAMT			6
#define HOSTTEXTC_LINES     3
#define HOSTTEXTC_WIDTH     20
#define S_SUGTIP			6
#define S_TRLCCURRPTS		6
#define S_TRLCPREVPTS		6
#define S_TRLCPTSAMT		6
#define S_TRLCPTSOLD		6
#define S_TRGCAVBAL			6
#define S_TRGCPVBAL			6
#define	S_TRAVSRSP			6

#ifdef	MAKE_EMV
#define S_TRAPPEXP			3
#define S_TRATC				2
#define S_TRAC				8
#define S_TRISAPDAT			(1+32)
#define S_TRAIP				2
#define S_TRAUC				2
#define S_TRRAND			4
#define S_TRAPPVER			2
#define S_TRTAIDVER			2
#define S_TRTVR				5
#define S_TRISAC			(5+5+5)
#define S_TRAID				16
#define S_TREMVRSPC			2
#define S_TRISSSCRIPTRES	48
#define S_TRTSI				2
#define	S_TRCVMRESULTS		3
#define S_TRAPPTEXT         17
#endif /* MAKE_EMV */

#define S_TRSERVCD			2
#ifdef MAKE_SIGCAPTURE
#define S_TRSIGDATA		 2048	// Signature in vector format
#endif // MAKE_SIGCAPTURE

struct trandata_rec
{
	UBYTE TRKEY;				// TRANSACTION KEY      (ENUM)
	UBYTE TRCRNUM;				// CARD RANGE TABLE NUMBER (BIN)
	UBYTE TRISID;				// ISSUER ID            (BIN)
	UBYTE TRAQID;				// ACQUIRER ID          (BIN)
	UBYTE TRINV[S_TRINV];		// INVOICE/ROC NUMBER   (ASC)
	UBYTE TRTOTAM[S_TRTOTAM];	// TOTAL AMOUNT         (BCDR)
	UBYTE TRRSPC[S_TRRSPC];		// RESPONSE CODE        (ASC)
	UBYTE TRSTATUS[S_TRSTATUS];	// STATUS FLAG FOR JOURNAL (BITMASK)
	UBYTE TRCASHIER[S_TRCASHIER];	// CASHIER              (ASC)
	UBYTE TRSHIFT;				// SHIFT NUMBER

	UBYTE TRFIXEDEND;			// IF CHANGED FROM UBYTE MUST MODIFY UNDERLYING TLIB ASSEMBLY PRGS.
	// Marker for end of fixed area of trandata_rec.  Using a byte
	// so developer/customer programmer never has to change code if
	// moving variables into or out of the fixed area.
	// END OF TRANSACTION HEADER AREA. THIS AREA IS STORED IN JOURNAL
	// AS FIXED LENGTH HEADER WITHOUT ANY CHANGE AND/OR COMPRESSION.
	// ONLY FIELDS RELEVANT FOR SELECT MATCH ROUTINE IN JOURNAL MODULE
	// SHOULD BE PLACED HERE. CHANGING THIS HEADER WILL MAKE THE OLD
	// JOURNAL STRUCTURE AND THE DATA STORED INCOMPATIBLE, CAUSING
	// UNPREDICTABLE RESULTS. ALWAYS CLEAR THE JOURNAL WHEN LOADING
	// MODIFICATIONS TO THIS STRUCTURE.

	UBYTE TRPAN[S_TRPAN];		// MANUALLY ENTERED PAN (BCDL)
	UBYTE TREXPD[S_TREXPD];		// EXPIRATION DATE      (BCD)
	UBYTE TRSTARTD[S_TRDATE];	// START DATE (BCD)
	UBYTE TRISSUENUM;			// ISSUE NUMBER FOR SWITCH CARDS
	UBYTE TRYEAR[S_TRYEAR];		// TRANSACTION YEAR
	UBYTE TRDATE[S_TRDATE];		// DATE, TRANSACTION (BCD) ** ORDER
	UBYTE TRTIME[S_TRTIME];		// TIME, TRANSACTION (BCD) ** ASSUMED
	UBYTE TRBSEQ[S_TRBSEQ];		// BATCH SEQUENCE NUMBER
	UBYTE TRPOSE[S_TRPOSE];		// POS ENTRY MODE       (BCDR)
	UBYTE TRBUSDT[S_TRBUSDT];	// BUSINESS DATE (MDY, BCD)
	UBYTE TRAUTH[S_TRAUTH];		// AUTH NUMBER          (ASCL)
	UBYTE TRRRN[S_TRRRN];		// RETRIEVAL REFERENCE NO (ASC)
	UBYTE TRSTAN[S_TRSTAN];		// SYSTEMS TRACE AUDIT NO (BCD)
	UBYTE TRCASHBK[S_TRBASE];	// CASHBACK AMOUNT          (BCDR)
	UBYTE TRBAR[S_TRBASE];		// BAR AMOUNT          (BCDR)
	UBYTE TRTIP[S_TRTIP];		// TIP AMOUNT           (BCDR)
	UBYTE TRTAX[S_TRBASE];		// TAX AMOUNT          (BCDR)
	UBYTE TRORIGAMT[S_TRBASE];	// ORIGINAL AMOUNT OF A 
	UBYTE TRDESC[S_TRDESC];		// DESCRIPTOR CODES     (ASCL)
	UBYTE TRRSPA[S_TRRSPA];		// ADDITIONAL RESPONSE DATA (ASC)
	UBYTE TRPOCC;				// POS CONDITION CODE   (BCD)
	UBYTE TRTILLNO[S_TRTILLNO];	// TILL NUMBER          (ASC)

    UBYTE                       TRAUTHSOURCE;		// VISA II AUTH SOURCE CODE (ASC)

        UBYTE TRPSI;				// PAYMENT SERVICE INDICATOR  (ASCII)
	UBYTE TRTRANID[S_TRTRANID];	// TRANSACTION ID      (ASCII)
	UBYTE TRVALID[S_TRVALID];	// VALIDATION CODE     (ASCII)

    UBYTE                       TRV2RESP[S_TRV2RESP];	// VISA II RESPONSE CODE    (ASCII)

        UBYTE TRAUTHAMT[S_TRAUTHAMT];// AUTHORIZED AMOUNT        (BCD)
	UBYTE TRMSURC[S_TRBASE];	// MERCHANT SURCHARGE       (BCDR)
	UBYTE TRSURC[S_TRBASE];		// NETWORK SURCHARGE        (BCDR)
	UBYTE TRREFDATA[S_TRREFDATA];// HOST DATA FOR UPLOAD AND ADJUST ADVICES
	UBYTE TRAP[S_TRAP];			// ADDITIONAL PROMPT DATA

    UBYTE                       TRPONUM[S_TRPONUM];	// P.O. NUMBER / I.D. (ASC)
    UBYTE                       TRDEBIT_NETWORKID;	// NETWORK ID (VISA DEBIT)
    UBYTE                       TRDEBIT_SETLDATE[S_TRDEBIT_SETLDATE];	// SETTLEMENT DATE (VISA DEBIT)
    UBYTE                       TRDEBIT_STATUS;		// ACK NOT RECEIVED (VISA DEBIT)

        union
        {
          // Check reader (or manual entry) of MICR may enter account only,
          // or transit+account.  If TRMICR spills over into TRTRANSIT, then
          // it is assumed that TRMICR includes the transit info.
          UBYTE                   TRMICR[S_TRMICR + S_TRTRANSIT];
          struct
          {
            UBYTE             TRChkAcct[S_TRMICR];	// Checking Account number  (ASCL)
            UBYTE             TRTRANSIT[S_TRTRANSIT];	// Bank Routing Number      (ASCL)
          } __attribute__((packed))
          f2;
        } __attribute__((packed))
        micr;
        UBYTE TRTCNO[S_TRTCNO];		// TRAVELER'S/CHECKING CHECK NO  (ASCL)

    UBYTE                       TRZIP[S_TRZIP];		// ZIP CODE         (ASCL)
    UBYTE                       TRCARDHOLDERID;		// CARDHOLDER IDENTIFICATION CODE

        UBYTE TRSUGTIP1[S_SUGTIP];	// Suggested TIP #1 field
	UBYTE TRSUGTIP2[S_SUGTIP];	// Suggested TIP #2 field
	UBYTE TRSUGTIP3[S_SUGTIP];	// Suggested TIP #3 field
	UBYTE TRAVSRSP[S_TRAVSRSP];	// ADDRESS VERIFICATION RESPONSE CODE
	UBYTE TRTRK2[S_TRTRK2];		// TRACK 2 DATA  (ASCL)	

#ifdef MAKE_EMV
    UBYTE	TRPANSEQ;					/* PAN SEQUENCE NUMBER (0x5F34) */
    UBYTE	TRAPPEXP[S_TRAPPEXP];		/* APPLICATION EXPIRY DATE (0x5F24) */
#endif /* MAKE_EMV */

    UBYTE	TRSERVCD[S_TRSERVCD];		/* SERVICE CODE (0x5F30) */

#ifdef MAKE_EMV
/* ### THE FOLLOWING ORDER ASSUMED, DO NOT CHANGE!!! #### */
    UBYTE	TRCRYPTINFO;				/* CRYPTOGRAM INFORMATION DATA (0x9F27) */
    UBYTE	TRATC[S_TRATC];				/* APPL TRAN COUNTER (0x9F36) */
    UBYTE	TRAC[S_TRAC];				/* TRANSACTION CRYPTOGRAM (0x9F26) */
    UBYTE	TRISAPDAT[S_TRISAPDAT]; 	/* ISSUER APPLICATION DATA (0x9F10) */
/*	###################################################### */
    UBYTE	TRAIP[S_TRAIP];  	    	/* APPLICATION INTERCHANGE PROFILE (0x8200) */
    UBYTE	TRAUC[S_TRAUC]; 	     	/* APPLICATION USAGE CONTROL (0x9F07) */
    UBYTE	TRRAND[S_TRRAND];       	/* UNPREDICTABLE NUMBER (0x9F37) */
    UBYTE	TRAPPVER[S_TRAPPVER];	  	/* APPLICATION VERSION (0x9F08) */
    UBYTE	TRTAIDVER[S_TRTAIDVER];  	/* TERMINAL APPLICATION VERSION (0x9F09) */
    UBYTE	TRTVR[S_TRTVR];      		/* TERMINAL VERIFICATION RESULTS (0x9500) */
	UBYTE	TRISAC[S_TRISAC];			/* ISSUER ACTION CODES (0x9F0E,0x9F0D,0x9F0F) */
	UBYTE	TRAID[S_TRAID]; 			/* APPLICATION IDENTIFIER (0x9F06) */
	UBYTE	TREMVRSPC[S_TREMVRSPC];		/* OFFLINE GENERATED EMV RESPONSE CODE (0x8A00) */
	UBYTE	TRISSSCRIPTRES[S_TRISSSCRIPTRES];   /* ISSUER SCRIPT RESULTS (0xDF52) */
 	UBYTE	TRCVMRESULTS[S_TRCVMRESULTS];	/* CVM RESULTS (0x9F34) */
	UBYTE   TRAPPTEXT[S_TRAPPTEXT];      // NULL TERMINATED TEXT SUPPLIED BY APPL. (TAG 9F12 OR 50 OR 84) (ASCII)
#endif /* MAKE_EMV */

#ifdef MAKE_SIGCAPTURE
    // These 2 fields are here to be used for REPRINT
    UWORD   TRSIGLEN;                   // LENGTH OF SIGNATURE DATA
	UBYTE   TRSIGDATA[S_TRSIGDATA];     // Signature in vector format
#endif // MAKE_SIGCAPTURE

	UBYTE TRJRNEND;				// END OF TRANSACTION JOURNAL AREA
	// IF CHANGED FROM UBYTE MUST MODIFY UNDERLYING TLIB ASSEMBLY PRGS.
	// Marker for end of journal transaction area of trandata_rec.  Using a
	// byte so developer/customer programmer never has to change code if
	// moving variables into or out of the this area.
	// END OF TRANSACTION JOURNAL AREA. THIS AREA IS STORED IN JOURNAL
	// AS VARIABLE LENGTH COMPRESSED DATA IMMEDIATELY AFTER THE HEADER.
	// ONLY FIELDS TO BE KEPT IN THE JOURNAL (OTHER THAN HEADER FIELDS)
	// SHOULD BE PLACED HERE. IF FIELD(S) ARE ADDED TO THE END OF THIS
	// AREA THEN THE OLD JOURNAL STRUCTURE AND THE DATA STORED ARE FULLY
	// COMPATIBLE. ANY OTHER CHANGE OF THIS AREA WILL MAKE THEM INCOMPATIBLE,
	// CAUSING UNPREDICTABLE RESULTS.
	//
	// THE AREA BELOW IS NOT SAVED IN JOURNAL AND IN GENERAL CAN BE CHANGED
	// IN ANY NECESSARY WAY. IT CONTAINS WORKING FIELDS WHICH ARE BEING
	// USED GENERALLY DURING TRANSACTION ENTRY AND PROCESSING ONLY.

	// NOTE TO APPLICATION PROGRAMMER:  JRBUF is a structure consisting of the size of the link
	// list controls and the size of trandata_rec. If trandata_rec is modified drastically so 
	// there is no additional data after the part of trandata_rec that is written/read
	// to/from the journal there could be data overruns.  
	// If a developer modifies trandata_rec so there is not at least 34 additional bytes
	// following "TRJRNEND" then additional area must be added to the journal_rec structure.  
	// Since the amount of expansion area is controlled by the application, one possible fix is
	// to add the following array at the end of the journal_rec structure with an adjustment to
	// 34 if some area already existed:    UBYTE EXPDATA[34];
	//
	// The 34 bytes were not just added to JRBUF because in almost all cases there is sufficient
	// data after TRJRNEND.  It is only if drastic changes are made to trandata_rec that the
	// additional space would be necessary.  


	struct card_rec *TRCRPTR;	// CARD RANGE POINTER   (PTR)
	struct issuer_rec *TRISPTR;	// ISSUER POINTER       (PTR)
	struct acquirer_rec *TRAQPTR;// ACQUIRER POINTER     (PTR)
    struct batch_rec *TRBATPTR;	// BATTAB POINTER     (PTR)
	UBYTE TRORIGKEY;			// ORIGINAL KEY OF A VOIDED TXN. (ENUM)
	UBYTE TRTRANBUF[S_TRTRANBUF];// TRANSACTION BUFFER   (ASCL)
	UBYTE TRRSPOPT;				// RESPONSE CODE OPTIONS (BITMASK)
	UBYTE TRRSPOPT2;			// RESPONSE CODE OPTIONS (BYTE 2)
	UBYTE TRRSPTXT[S_TRRSPTXT];	// RESPONSE TEXT        (ASC)
	UBYTE TRPWD[S_TRPWD];		// PASSWORD             (ASCL)
	UBYTE TRBASE[S_TRBASE];		// BASE AMOUNT          (BCDR)
	UBYTE TRTOTF[S_TRBASE];		// TOTAL W/ NETWORK SURCHARGE    (BCDR)
	UBYTE TRTOTAMI[S_TRBASE];	// TOTAL AMOUNT + ISREAUTH % (BCDR)

    UBYTE                       TRHSTMSGID[S_TRHSTMSGID];	// HOST MESSAGE IDENTIFIER   (ASCII)

        UBYTE TRBATTYP;				// BATCH TYPE           (BCD)
	UBYTE TRSTOT[S_TRSTOT];		// SALES TOTAL          (BCDR)
	UBYTE TRCTOT[S_TRCTOT];		// CREDITS TOTAL        (BCDR)
	UBYTE TRBATNO[S_TRBATNO];	// BATCH NUMBER         (ASCL)
	UBYTE TRPROC[S_TRPROC];		// PROCESSING CODE      (BCD)
	UBYTE TROSTAN[S_TRSTAN];	// ORIGINAL SYSTEMS TRACE NO (BCD)
	UBYTE TRNII[S_TRNII];		// NII                  (BCDR)
	UBYTE TRTID[S_TRTID];		// TERMINAL ID          (ASC)
	UBYTE TRACCID[S_TRACCID];	// CARD ACCEPTOR ID     (ASC)
	UBYTE TRTRK1[S_TRTRK1];		// TRACK 1 DATA         (ASCL)
	UBYTE TRMTYPE[S_TRMTYPE];	// MESSAGE TYPE         (BCD)
	UBYTE TRBMAP[S_TRBMAP];		// BITMAP               (BIN)
	UBYTE TRTPDU[S_TRTPDU];		// TPDU                 (BCD)
	UBYTE TRAGAIN;				// IS THIS AN AGAIN REQUEST (BOOL)
	UBYTE TRNAME[S_TRNAME];		// CARD HOLDER NAME     (ASC)
	UBYTE TRREPRINT;			// REPRINT FLAG         (BOOL)
	UBYTE TRHOSTTEXT[S_TRHOSTTEXT];	// HOST RESPONSE TEXT (ASCL)(DO NOT CHANGE)
	UBYTE TRPINBL[S_TRPINBL];	// ENCRYPTED PIN BLOCK
	UBYTE TRPINRETRY;			// NUMBER OF RETRIES FOR BAD PAN
	UBYTE TRSOC[S_TRSOC];		// SOC NUMBER
	UBYTE TRKSN[S_TRKSN];		// DUKPT KEY SERIAL NUMBER
    UBYTE                       TRAVSADDR[S_TRAVSADDR];	// ADDRESS VERIFICATION MAIL ADDRESS
	UBYTE TRCVV2IND;			// CVV2 presence indicator (asc)
	UBYTE TRCVV2[S_TRCVV2];		// CVV2 VALUE (ASC)
    UBYTE                       TROPTAMTID;		// LEVEL II CMRC./PURCH. CARD OPT. AMOUNT ID (ASC)
	UBYTE TROPTAMT[S_TROPTAMT];	// LEVEL II CMRC./PURCH. CARD OPT. AMOUNT (BCDR)
	UBYTE TRLVL2ID;				// LEVEL II CMRC./PURCH. CARD INDICATOR (ASC)
	UWORD TRJRNIDENT;			// Store Journal Pointer, for record updates

#ifdef MAKE_EMV						/* EMV RELATED DATA: */
    UBYTE TRTSI[S_TRTSI];			/* TRANSACTION STATUS INFORMATION (0x9B00)*/
   	UBYTE TREMVSIGREQ; 				/* SIGNATURE REQUIRED FLAG */
#endif	// MAKE_EMV

} __attribute__((packed));

#define TRSZE sizeof(struct trandata_rec)	// DEFINE STRUCTURE SIZE

// TRFIXEDSZE defines the size of the journal uncompressed header.
// The uncompressed area is from the start of the trandata_rec structure down to TRFIXEDEND.
#define TRFIXEDSZE offsetof(struct trandata_rec,TRFIXEDEND)

// TRCOMPSZE defines the size of compressed area.
// The compressed area is from TRFIXEDEND in the trandata_rec structure 
// down to TRJRNEND in the same trandata_rec structure.
#define TRCOMPSZE (offsetof(struct trandata_rec,TRJRNEND) - \
                   offsetof(struct trandata_rec,TRFIXEDEND))

//
//
//
//
//       IS8583 TABLE DEFINITION
//

struct is8583_rec
{
	UBYTE IS8583_FMT;			// FORMAT
	UWORD IS8583_LEN;			// LENGTH
} __attribute__((packed));

#define IS8583_SZE sizeof(struct is8583_rec)	// DEFINE STRUCTURE SIZE

//
//       RESPONSE TABLE STRUCTURE
//
#define R_APPROVED    0x01		// TRANSACTION APPROVED
#define R_UPLOAD      0x02		// UPLOAD BATCH
#define R_NOCLRREV    0x04		// DO NOT CLEAR REVERSAL
#define R_REFERRAL    0x08		// REFERRAL RESPONSE CODE
#define R_NOCLRPEND   0x20		// DO NOT CLEAR BATCH PENDING FLAG
#define R_BADPIN      0x80		// BAD PIN RESPONSE RECEIVED

#define R_NOCAPT      0x01		// APPROVED NO CAPTURE (ISO NDC)

#define S_RSPCDE      2
#define S_RSPTXT      20

struct resp_rec
{
	UBYTE RSPCDE[S_RSPCDE + 1];	// RESPONSE CODE
	UBYTE RSPTXT[S_RSPTXT + 1];	// TRANSLATED TEXT
	UBYTE RSPOPT;				// CONTROL BITS (BYTE 1)
	UBYTE RSPOPT2;				// CONTROL BITS (BYTE 2)
} __attribute__((packed));

#define RSPSZE sizeof(struct resp_rec)	// DEFINE STRUCTURE SIZE


//
//
//       CASHIER / TILL TABLES
//
#define CASHIER_USED      0x01	// ENTRY IS USED

#define CASHIER_MAX       20	// MAXIMUM NUMBER OF CASHIERS

struct cashier_rec
{
	UBYTE CASHIER_FLAG;			// STATUS
	UBYTE CASHIER[S_TRCASHIER];	// Cashier ID
	UBYTE TILLNO[S_TRTILLNO];	// TILL NUMBER
} __attribute__((packed));

#define CASHIER_SZE sizeof(struct cashier_rec)	// DEFINE STRUCTURE SIZE

//
//       BASIC TOTALS STRUCTURE
//

#define S_TOTDBCNT      2
#define S_TOTDBAMT      6
#define S_TOTDBVCNT     2
#define S_TOTDBVAMT     6
#define S_TOTCRCNT      2
#define S_TOTCRAMT      6
#define S_TOTCRVCNT     2
#define S_TOTCRVAMT     6

struct basictotals_rec
{
	UBYTE TOTDBCNT[S_TOTDBCNT];	// DEBITS COUNT
	UBYTE TOTDBAMT[S_TOTDBAMT];	// DEBITS AMOUNT
	UBYTE TOTDBVCNT[S_TOTDBVCNT];	// VOIDED DEBITS COUNT
	UBYTE TOTDBVAMT[S_TOTDBVAMT];	// VOIDED DEBITS AMOUNT
	UBYTE TOTCRCNT[S_TOTCRCNT];	// CREDITS COUNT
	UBYTE TOTCRAMT[S_TOTCRAMT];	// CREDITS AMOUNT
	UBYTE TOTCRVCNT[S_TOTCRVCNT];	// VOIDED CREDITS COUNT
	UBYTE TOTCRVAMT[S_TOTCRVAMT];	// VOIDED CREDITS AMOUNT
} __attribute__((packed));

#define TOTBASESZE sizeof(struct basictotals_rec)	// DEFINE STRUCTURE SIZE

//
//       TOTALS STRUCTURE
//

#define S_TOTBASEAMT    6
#define S_TOTTAXAMT     6
#define S_TOTBARAMT     6
#define S_TOTTIPAMT     6
#define S_TOTCBAMT      6
#define S_TOTCBCNT      2
#define S_TOTTIPPCT     6


struct totals_rec
{
	struct basictotals_rec TOTBASIC;	// BASIC PORTION OF TOTALS
	UBYTE TOTBASEAMT[S_TOTBASEAMT];	// DEBITS BASE AMOUNT
	UBYTE TOTTAXAMT[S_TOTTAXAMT];	// DEBITS TAX AMOUNT
	UBYTE TOTBARAMT[S_TOTBARAMT];	// DEBITS BAR AMOUNT
	UBYTE TOTTIPAMT[S_TOTTIPAMT];	// DEBITS TIP AMOUNT
	UBYTE TOTCBAMT[S_TOTCBAMT];	// CASHBACK AMOUNT
	UBYTE TOTCBCNT[S_TOTCBCNT];	// CASHBACK COUNT
	UBYTE TOTTIPPCT[S_TOTTIPPCT];	// TIP PERCENTAGE OF (TOTAL-TIP)
} __attribute__((packed));

#define TOTSZE sizeof(struct totals_rec)	// DEFINE STRUCTURE SIZE

//
//       "STANDARD" TOTALS TABLE
//

#define S_TOTCPSALCNT   2
#define S_TOTCPSALAMT   6
#define S_TOTCPREFCNT   2
#define S_TOTCPREFAMT   6
#define S_TOTDBSALCNT   2
#define S_TOTDBSALAMT   6
#define S_TOTDBREFCNT   2
#define S_TOTDBREFAMT   6
#define S_TOTAUSALCNT   2
#define S_TOTAUSALAMT   6
#define S_TOTAUREFCNT   2
#define S_TOTAUREFAMT   6

struct stdtot_rec
{
	UBYTE TOTCPSALCNT[S_TOTCPSALCNT];	// CAPTURE SALES COUNT
	UBYTE TOTCPSALAMT[S_TOTCPSALAMT];	// CAPTURE SALES AMOUNT
	UBYTE TOTCPREFCNT[S_TOTCPREFCNT];	// CAPTURE REFUND COUNT
	UBYTE TOTCPREFAMT[S_TOTCPREFAMT];	// CAPTURE REFUND AMOUNT
	UBYTE TOTDBSALCNT[S_TOTDBSALCNT];	// DEBIT SALES COUNT
	UBYTE TOTDBSALAMT[S_TOTDBSALAMT];	// DEBIT SALES AMOUNT
	UBYTE TOTDBREFCNT[S_TOTDBREFCNT];	// DEBIT REFUND COUNT
	UBYTE TOTDBREFAMT[S_TOTDBREFAMT];	// DEBIT REFUND AMOUNT
	UBYTE TOTAUSALCNT[S_TOTAUSALCNT];	// AUTHORIZE SALES COUNT   ** NOT USED
	UBYTE TOTAUSALAMT[S_TOTAUSALAMT];	// AUTHORIZE SALES AMOUNT  ** NOT USED
	UBYTE TOTAUREFCNT[S_TOTAUREFCNT];	// AUTHORIZE REFUND COUNT  ** NOT USED
	UBYTE TOTAUREFAMT[S_TOTAUREFAMT];	// AUTHORIZE REFUND AMOUNT ** NOT USED
} __attribute__((packed));

#define STDTOTSZE sizeof(struct stdtot_rec)	// DEFINE STRUCTURE SIZE

//
//       BATCH TABLE STRUCTURE
//
#define BT_USED       0x01		// ENTRY IS USED
#define BT_PEND       0x02		// BATCH TRANS IS PENDING
#define BT_SETL       0x08		// BATCH HAS BEEN HOST SETTLED
#define BT_SPARE4	  0x10		// BIT 4-SPARE
#define BT_SIGPEND	  0x40		// BATCH HAS STARTED SIGNATURE UPLOAD
#define BT_SIGSETL	  0x80		// BATCH HAS BEEN SIGNATURE HOST SETTLED

// BIT DEFINITIONS FOR BATFLAG2
#define BT2_SPARE1	  0x02		// BIT 1-SPARE
#define BT2_SPARE2	  0x04		// BIT 2-SPARE
#define BT2_SPARE3	  0x08		// BIT 3-SPARE
#define BT2_SPARE4	  0x10		// BIT 4-SPARE
#define BT2_SPARE5	  0x20		// BIT 5-SPARE
#define BT2_SPARE6	  0x40		// BIT 6-SPARE
#define BT2_SPARE7	  0x80		// BIT 7-SPARE

#define S_BATCUR      3
#define S_BATNXT      3
#define S_BATREV      2
#define S_BATADJ      2
#define S_BATSETLDAT  3
#define S_BATSETLTIM  3
#define S_BATATTMDAT  3
#define S_BATATTMTIM  3
#define S_BATSETLRSPC 2
#define S_BATSETLTEXT 40
#define S_BATSETLNO   3

struct batch_rec
{
	UBYTE BATFLAG;					// FLAG
	UBYTE BATAQID;					// BATCH ACQUIRER ID
	UBYTE BATSETLDAT[S_BATSETLDAT];	// DATE OF LAST SUCCESSFUL SETTLEMENT
	UBYTE BATSETLTIM[S_BATSETLTIM];	// TIME OF LAST SUCCESSFUL SETTLEMENT
	UBYTE BATCUR[S_BATCUR];			// CURRENT BATCH NUMBER
	UBYTE BATNXT[S_BATNXT];			// NEXT BATCH NUMBER
	UWORD BATXFER;					// IDENT OF OUTSTANDING ADVICE TO HOST
	UBYTE BATREV[S_BATREV];			// BATCH REVERSALS COUNT
	UBYTE BATADJ[S_BATADJ];			// BATCH ADJUSTMENTS
	UWORD BATCKPT;					// IDENT FOR CHECKPOINT RESTART
	UBYTE BATSETLST;				// SETL STATUS
	UBYTE BATSETLINI;				// METHOD OF SETTLE INITIATION
	UBYTE BATATTMDAT[S_BATATTMDAT];	// DATE OF LAST SETTLE ATTEMPT
	UBYTE BATATTMTIM[S_BATATTMTIM];	// TIME OF LAST SETTLE ATTEMPT
	UBYTE BATSETLTRY;				// NUMBER OF SETL ATTEMPTS SINCE SUCCESS
	UBYTE BATSETLIDL;				// IDLE TIMER COUNT
	UBYTE BATSETLRSPC[S_BATSETLRSPC];// LAST RESPONSE CODE TO SETTLEMENT
	UBYTE BATSETLTEXT[S_BATSETLTEXT];// LAST HOST RESPONSE TO SETTLEMENT
	struct basictotals_rec BATSETLTOT;// TOTALS OF LAST SUCCESSFUL SETTLEMENT
	UBYTE BATSETLNO[S_BATSETLNO];	// BATNO OF LAST BATCH SETTLED OK
	UBYTE BATLOGSTATUS;				// LOGON STATUS FLAG
} __attribute__((packed));

#define BATSZE sizeof(struct batch_rec)	// DEFINE STRUCTURE SIZE

//
//
//
//       JOURNAL FIND STRUCTURE
//
// For JSSPECIAL
#define JS_RRNINV        0x01	// Use Last 6 Digits of RRN for INV. No
#define JS_UNUSED        0x02	// Unused -- Special Journal Select Bit1

struct jrnfind_rec
{
	UWORD JSIDENT;				// IDENT                0000 IS ANYST
	UBYTE JSKEY;				// TRANSACTION KEY      00   IS ANYST
	UBYTE JSAQID;				// ACQUIRER ID          00   IS ANY
	UBYTE JSISID;				// ISSUER ID            00   IS ANY
	UBYTE JSINV[S_TRINV];		// INVOICE NUMBER       0..0 IS ANY
	UBYTE JSSTATON;				// STATUS BITS THAT MUST BE ON
	UBYTE JSSTATON2;			// STATUS2 BITS THAT MUST BE ON
	UBYTE JSSTATOFF;			// STATUS BITS THAT MUST BE OFF
	UBYTE JSSTATOFF2;			// STATUS2 BITS THAT MUST BE OFF
	UBYTE JSTYPE;				// TYPE 0-SALE T1_CREDIT-REFUND 0xff-ANY
	UBYTE JSTOTAM[S_TRTOTAM];	// TRANSACTION AMOUNT
	UBYTE JSAQIGNORE;			// ACQUIRER TO IGNORE
	UBYTE JSCASHIER[S_TRCASHIER];	// CASHIER ID      000 IS ANY
	UBYTE JSSHIFT;				// SHIFT NUMBER    00  IS ANY
	UBYTE JSSPECIAL;			// Special Search Bits
} __attribute__((packed));

#define JSSZE sizeof(struct jrnfind_rec)	// DEFINE STRUCTURE SIZE

//
//       TERMINAL FLAG TABLE
//

#define TF1_DOING_INIT	 0x01	// INITIALIZATION IS IN PROGRESS

#define S_TFYEAR   	S_TRYEAR+S_TRDATE+S_TRTIME
#define S_TRKEY    	1

struct termflg_rec
{
	UBYTE TFIND1;				// INDICATOR 1
	UBYTE TFLAQID;				// LAST ACQUIRER - NON-ZERO IF CLOSE BATCH
	//  WAS LAST TRANSACTION
	// FOLLOWING 6 FIELDS ONLY VALID IF TFLAQID
	//  IS NON-ZERO
	UBYTE TFKEY;				// TRANSACTION KEY
	UBYTE TFYEAR[S_TFYEAR];		// TRANSACTION TIME
	UBYTE TFHOSTTEXT[S_TRHOSTTEXT];	// LAST HOST RESPONSE
	UBYTE TFSHIFT;				// SHIFT NUMBER
	UBYTE TFMAXTRY;				/* Maximum try counter for Init Recovery (hex, but displayed as bcd (Fun_54)) */
#ifdef	MAKE_EMV
	UBYTE TFPINFAILFLG;				/* External Pinpad failure flag */
#endif	// MAKE_EMV

	UBYTE TFSPARE[38];			// SPARE FOR FUTURE EXPANSION
} __attribute__((packed));

#define TFSZE sizeof(struct termflg_rec)	// DEFINE STRUCTURE SIZE

//
//
//
//-----------------------------------------------------------------------
// EQUATES ARE USED IN THE 'V2TAB' STRUCTURE.
//-----------------------------------------------------------------------
//
#define V2MAX                   2	// MAXIMUM TABLE ETNRIES

// FIELD SIZES
#define S_V2NUM                 1	//  POSITION IN TABLE
#define S_V2AQID                1	//  ACQUIRER ID
#define S_V2MERCH               12	//  MERCHANT NUMBER
#define S_V2STORE               4	//  STORE NUMBER
#define S_V2TERM                4	//  TERMINAL NUMBER
#define S_V2MERCHNAME           25	//  MERCHANT NAME
#define S_V2CITYST              15	//  MERCHANT CITY AND STATE
#define S_V2BNUM                24	//  ACQUIRER BANK NUMBERS
#define S_V2CATEG               4	//  CATEGORY CODE
#define S_V2LOCAT               4	//  LOCATION CODE
#define S_V2TSER                8	//  TERMINAL IDENTIFICATION/SERIAL NUMBER
#define S_V2CITY                5	//  MERCHANT CITY CODE
#define S_V2COUNTRY             3	//  MERCHANT COUNTRY CODE
#define S_V2SECURITY            5	//  MERCHANT SECURITY CODE
#define S_V2CURRENCY            3	//  MERCHANT CURRENCY CODE
#define S_V2STATE               2	//  MERCHANT STATE CODE
#define S_V2TIMEZONE            3	//  TIME ZONE DIFFERENTIAL
#define S_V21STLOCAT            1	//  FIRST CHAR OF LOCATION CODE
#define S_V2APPID               7	//  MAGCARD APPLICATION ID
#define S_V2APPVER              2	//  MAGCARD APPLICATION VERSION NUMBER
#define S_V2DEBIT_GROUP        14	//  DEBIT SHARING GROUP
#define S_V2DEBIT_ABA          14	//  MERCHANT ABA NUMBER
#define S_V2DEBIT_SETTLEAGENT   4	//  DEBIT SETTLEMENT AGENT NUMBER
#define S_V2REIMBURSE           1	//  REIMBURSEMENT ATTRIBUTE
#define S_V2EXTZIP              4	//  ZIP EXTENSION FOR ZIP+4
#define S_V2RESV                6	//  RESERVED

//
//
//
//-----------------------------------------------------------------------
// STRUCTURE TO HOLD VISANET, BASE II MERCHANT DATA ASSIGNED BY VISA.
// THE VARIABLES DEFINED ARE 'V2TAB'.
//-----------------------------------------------------------------------
//

struct v2_rec
{
    UBYTE                       V2NUM;			// POSITION IN TABLE
    UBYTE                       V2AQID;			// ACQUIRER ID
    UBYTE                       V2MERCH[S_V2MERCH];	// MERCHANT NUMBER
    UBYTE                       V2STORE[S_V2STORE];	// STORE NUMBER
    UBYTE                       V2TERM[S_V2TERM];	// TERMINAL NUMBER
    UBYTE                       V2MERCHNAME[S_V2MERCHNAME];	// MERCHANT NAME
    UBYTE                       V2CITYST[S_V2CITYST];	// MERCHANT CITY AND STATE
    UBYTE                       V2BNUM[S_V2BNUM];	// ACQUIRER BANK NUMBERS
    UBYTE                       V2CATEG[S_V2CATEG];	// CATEGORY CODE
    UBYTE                       V2LOCAT[S_V2LOCAT];	// LOCATION CODE
    UBYTE                       V2TSER[S_V2TSER];	// TERMINAL IDENTIFICATION/SERIAL NUMBER
    UBYTE                       V2CITY[S_V2CITY];	// MERCHANT CITY CODE
    UBYTE                       V2COUNTRY[S_V2COUNTRY];	// MERCHANT COUNTRY CODE
    UBYTE                       V2SECURITY[S_V2SECURITY];	// MERCHANT SECURITY CODE
    UBYTE                       V2CURRENCY[S_V2CURRENCY];	// MERCHANT CURRENCY CODE
    UBYTE                       V2STATE[S_V2STATE];	// MERCHANT STATE CODE
    UBYTE                       V2TIMEZONE[S_V2TIMEZONE];	// TIME ZONE DIFFERENTIAL
    UBYTE                       V21STLOCAT;		// FIRST CHAR OF LOCATION CODE
    UBYTE                       V2APPID[S_V2APPID];	// MAGCARD APPLICATION ID
    UBYTE                       V2APPVER[S_V2APPVER];	// MAGCARD APPLICATION VERSION NUMBER
    UBYTE                       V2DEBIT_GROUP[S_V2DEBIT_GROUP];	// DEBIT SHARING GROUP
    UBYTE                       V2DEBIT_ABA[S_V2DEBIT_ABA];	// MERCHANT ABA NUMBER
    UBYTE                       V2DEBIT_SETTLEAGENT[S_V2DEBIT_SETTLEAGENT];	// DEBIT SETTLEMENT AGENT NUMBER
    UBYTE                       V2REIMBURSE;		// REIMBURSEMENT ATTRIBUTE
    UBYTE                       V2EXTZIP[S_V2EXTZIP];	// ZIP EXTENSION FOR ZIP+4
    UBYTE                       V2RESV[S_V2RESV];	// RESERVED
} __attribute__((packed));

#define V2SZE                   sizeof(struct v2_rec)	// DEFINE STRUCTURE SIZE

//
//
//
//------------------------------------------------------------------
//      TRANSACTION TABLE ENTRY STRUCTURE DEFINITION
//------------------------------------------------------------------

struct event_entry
{
	UBYTE TRNKEY;				// Transaction Key
	XCALL_T X_func;				// Cross-Call action handler
	char TRNTEXT[11];			// Transaction description
	UBYTE TRNOPT1;				// OPTIONS BYTE 1
	UBYTE TRNOPT2;				//              2
	UBYTE TRNOPT3;				//              3
	UBYTE TRNOPT4;				//              4
	UWORD NEXTEVENT;			// trnkey for next triggered event
};


// Definitions for Report Options

#define RPT_SUMRY   0x01		// Summary Report
#define RPT_UNADJ   0x02		// UNADJ Report

//  This structure is for the additional data prompt table

#define APMAXENT 8				// ADDITIONAL PROMPT MAXIMUM ENTRIES

#define AP1_CASH        0x01	//  PROMPT IN CASH TRANS
#define AP1_CARDVER     0x02	//  PROMPT IN CARD VERIFY
#define AP1_PREAUTH     0x04	//  PROMPT IN PREAUTH
#define AP1_CHECK       0x08	//  PROMPT IN CHECK TRANS
#define AP1_DEBIT       0x10	//  PROMPT IN DEBIT TRANS
#define AP1_REFUND      0x20	//  PROMPT IN REFUND TRANS
#define AP1_SALOFFL     0x40	//  PROMPT IN OFFLINE SALE
#define AP1_SALE        0x80	//  PROMPT IN SALE/AUTH

#define AP2_SETTLE      0x80	//  PROMPT IN SETTLE TRANSACTION

#define S_APTEXT 		20		// Additional Text size

struct addprompt_rec
{
	UBYTE APNUM;				// ADDITIONAL PROMPT NUMBER
	UBYTE APID[2];				// CODE SENT IN TRANSACTION DETAIL
	UBYTE APTEXT[S_APTEXT];		// TEXT FOR DISPLAY
	UBYTE APOPT1;				// TRANSACTION SELECT OPTION 1
	UBYTE APOPT2;				// TRANSACTION SELECT OPTION 2
	UBYTE APOPT3;				// TRANSACTION SELECT OPTION 3
	UBYTE APOPT4;				// TRANSACTION SELECT OPTION 4
	UBYTE APMIN;				// MINIMUM INPUT LENGTH
	UBYTE APMAX;				// MAXIMUM INPUT LENGTH
	UBYTE APTYPE;				// ENTRY TYPE
	UBYTE APRESV[5];			// RESERVED
} __attribute__((packed));

#define APSZE   sizeof(struct addprompt_rec)

#define USRDSZE     40


// icepak configuration table structure

#define S_IPTEL		12
#define S_IPNII		2
#define S_IPSID		11

struct stipac
{
	UBYTE IPTEL[S_IPTEL];		// ICEPAK TELEPHONE NUMBER
	UBYTE IPMM;					// ICEPAK MODEM MODE
	UBYTE IPNII[S_IPNII];		// ICEPAK NETWORK INTERFACE IDENTIFIER
	UBYTE IPSID[S_IPSID];		// ICEPAK SCREEN SET IDENTIFIER
	UBYTE IPDELAY1;				// IDLE DELAY BEFORE 1ST AD SCREEN DISPLAYED (BCD)
	UBYTE IPDELAY2;				// DELAY BETWEEN SCREENS (BCD)
} __attribute__((packed));

#define IPSZE   sizeof(struct stipac)

// Receipt Footer Text Table

#define S_FOOTER	24			// Footer Text Size

#define S_CSTM_IDLE   20		// Custom IDLE Prompt Size
struct ft_table
{
	UBYTE FTFOOTER1[S_FOOTER];	// RECEIPT FOOTER TEXT LINE 1
	UBYTE FTFOOTER2[S_FOOTER];	// RECEIPT FOOTER TEXT LINE 2
	UBYTE FTFOOTER3[S_FOOTER];	// RECEIPT FOOTER TEXT LINE 3
	UBYTE FTFOOTER4[S_FOOTER];	// RECEIPT FOOTER TEXT LINE 4
	UBYTE FTIDLEPROMPT[S_CSTM_IDLE];	// CUSTOM IDLE PROMPT
} __attribute__((packed));

#define FTSZE sizeof(struct ft_table)	// DEFINE STRUCTURE SIZE

//                                                                      
//       THE STRUCTURE OF 'SCHTAB'                                  
//                                                                      

#define SM_DLL   		0x01	// SCHEDULE PROGRAM LOAD
#define SM_INIT  		0x02	// SCHEDULE INITIALIZATION
#define SMTEL_TEMP   	0x01	// USE TELEPHONE NUMBER AS A TEMP NUM.
#define SMTEL_PERM		0x02	// USE TELEPHONE NUMBER AS A PERM NUM.
#define SMTEL_AQ1P  	0x03	// USE ACQUIRER RANGE 1 PRIMARY TEL.NUM.
#define SCH_USED  		0x01	// ENTRY IS USED
#define SCH_REQ   		0x02	// ENTRY ACTIVATED FOR PROGRAM LOAD
#define S_SCHOLDTEL		12
#define S_SCHOLDSWARE  	10
#define S_SCHFILLER		5
#define S_SMWHEN		6
#define S_SMTELNO		12
#define SCHMAX   		2		// TWO SCHEDULE ENTRIES

struct schedule_rec
{
	UBYTE SCHFLAG;				// CONTROL FLAG
	UBYTE SMTYPE;				// TYPE OF SCHDULED INFORMATION
	UBYTE SMWHEN[S_SMWHEN];		// SCHEDULE WHEN YYMMDDHHMMSS, DO NOW IF 999999999999
	UBYTE SMTELTYPE;			// TELEPHONE NUMBER TYPE
	UBYTE SMTELNO[S_SMTELNO];	// TELEPHONE NUMBER, NO Phone NUM IF 00000000000000
	UBYTE SCHOLDTEL[S_SCHOLDTEL];	// SAVE AREA FOR TELEPHONE NUMBER
	UBYTE SCHOLDSWARE[S_SCHOLDSWARE];	// OLD SOFTWARE NAME
	UBYTE SCHFILLER[S_SCHFILLER];	// FILLER FOR LATER
} __attribute__((packed));

#define SCHSZE sizeof( struct schedule_rec )	// DEFINE STRUCTURE SIZE



//	PUBLIC DATA NETWORK TABLE

#define S_PDNLOGON	50              // SIZE OF HALF OF SCRIPT STRING
#define	PDNMAX	AQMAX               // MAXIMUM TABLE ENTRIES

typedef struct {
	UBYTE	PDNNUM;                 // POSITION IN TABLE
	UBYTE	PDNAQID;                // ACQUIRER ID
	UBYTE	PDNPTLOGON[S_PDNLOGON];	// 1-ST PART OF SCRIPT
	UBYTE	PDNSTLOGON[S_PDNLOGON];	// 2-ND PART OF SCRIPT
	UBYTE	PDNNullTerminator;      // Do not use this field!! TNMS provides this null
} __attribute__((packed))
PDNSTRUCT;
#define	PDNSZE	sizeof(PDNSTRUCT)


// IP Connect tables number
#define	IPCMAX	AQMAX*4

// Option byte
#define IPC_LENGTH		0x01	// Add HEX length in the packet header
#define IPC_TPDU		0x02	// Add POS TPDU in the packet header
#ifdef MAKE_SSL
#define IPC_SSL			0x10	// Enable SSL for all IP communications
#define	IPC_SSL_PEER   	0x20	// Enable Server Authentication
#define	IPC_SSL_CLIENT	0x40	// Enable sending client certificate
                                // during Mutual Authentication
#endif  // MAKE_SSL

#define S_CERTFILE      32
#define S_CERTTAIL      (96-(26+2*S_CERTFILE))

// IP Connect Table definition
struct ipcontab
{
	UBYTE id;				// IP Connect Table ID
	UBYTE ipaddr[15];		// IP address
	UBYTE ipaddrterminator;	// IP address zero terminator
	UBYTE port[5];			// TCP port number
	UBYTE portterminator;	// TCP port zero terminator
	UBYTE nii[2];			// NII
	UBYTE opt1;				// Option byte
#ifdef MAKE_SSL
    UBYTE cert_CA[S_CERTFILE];		// CA's certificate filename
    UBYTE certificate[S_CERTFILE];	// Terminal's filename
    UBYTE reserved[S_CERTTAIL];		// Reserved for feature use
#else
	UBYTE reserved[S_CERTTAIL + 2*S_CERTFILE];	// Reserved for feature use
#endif // MAKE_SSL
} __attribute__((packed));
#define IPCSZE	sizeof(struct ipcontab)



// Performance Report Table

#define S_PERIOD 6		// Performance Report- Period of time Length

//!!! do not use __attribute__((packed))
struct pf_table
{
	UBYTE PERIODDATE[S_PERIOD]; // PERIOD (REPORT START DATE)      
	UWORD TOTAPPROVAL;          // TRANSACTIONS APPROVED TOTALS   
	UWORD APPROV_PERC;  	    // PERCENT OF TRANS. APPROVED     
	UWORD TOTDENIALS;           // TRANSACTIONS DENIAL TOTALS     
	UWORD DENIALS_PERC;         // TRANS. DENIAL PERCENT          
	UWORD TOTMANUAL;	        // MANUALLY TRANSACTION TOTALS    
	UWORD MANUAL_PERC;  	    // PERCENT OF TRANS. MANUAL ENTRY 
	UWORD TOTSWIPE;     	    // SWIPE TRANSACTION TOTALS       
	UWORD SWIPE_PERC;           // PERCENT OF TRANS. SWIPE        
    UWORD TOTCOUNT;             // TRANSACTION COUNTER TOTALS     
	UWORD TOTREDIALS;           // REDIAL TOTALS                  
	UWORD TOTTIMEOUT;        	// TIMEOUT TOTALS                 
	UWORD TOTRETRANS;         	// RETRANSMITS TOTALS             
};

#define PFSZE sizeof(struct pf_table)	// DEFINE STRUCTURE SIZE 


struct super_variables
{
    UBYTE dontSendPosTPDU;
}__attribute__((packed));

#define SUPERSZE sizeof(struct super_variables)

//=============================================================================
// Public data declarations
//=============================================================================
extern struct termconf_rec TCONF;

//=============================================================================
// Public function declarations
//=============================================================================


#endif // _STRUCT_H_
