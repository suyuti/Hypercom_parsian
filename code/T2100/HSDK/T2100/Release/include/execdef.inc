/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef	EXECDEF_H
#define EXECDEF_H

#include <hyctypes.h>
#include <osplatform.inc> //; Exclusive OS platform definitions

#define EMULATE TRUE		//; used to create Loadable software 

//; Commom function call numbers

#define RQIOCS 	1 		//; IOCS request
#define RQPTCS 	2 		//; PTCS request
#define TOPAGE 	3 		//; move memory to page
#define FRPAGE 	4 		//; move memory from page
#define BD_PDT 	5 		//; Build PDT
#define SET_BIT	6 		//; set bit in page
#define RES_BIT	7 		//; reset bit in page
#define LCALL 	8 		//; call subroutine in a page
#define CKPAGE 	9 		//; checksum page
#define RSTTERM 10		//; reset terminal
#define EXCALL 	11		//; extended call
#define GETPID 	12		//; return process id
#define BD_BPDT 13		//; build PDT for BOOT mode
#define TOPAGED 14		//; move memory to direct page
#define FRPAGED 15		//; move memory to direct page
#define	BD_DCB	16		//; Build DCBs for Devices on Application Pages
#define	BD_BDCB	17		//; Build DCBs for Devices on Boot Pages
#define	MV_PAGE	18		//; Move data from any page to any page
#define REBOOTTERM	19	//; reboot terminal

#define MAXPCB 12       	 //; number of Processes
//; ----------------------------
//; Page Status Table Definitions

//; Page Status Table Format Code (P_STATFMT) Equates:
#define FMTNORM		0x55	//; 12 Page Table
#define FMTEMUL		0x66	//; 12 Page Table w/emulator connected
#define FMTNORM_XM	0xAA	//; Extended Memory Table
#define FMTEMUL_XM	0xBB	//; Extended Memory Table w/emulator connected
#define FMTNORM_T6K	0xCC	//; T6000 Table
#define FMTEMUL_T6K	0xDD	//; T6000 Table w/emulator connected

//; PAGE_STATUS
//; Enumeration defining status of a Page.
typedef enum
{
    psNO_FIT,                   //; not fitted
    psFREE,                     //; avail, but not in use
    psACTIVE,                   //; currently activated
    psINACTIVE,                 //; deactivated
    psLOADING,                  //; page being loaded
    psCORRUPT,                  //; checksum error
    psBOOTACTIVE,               //; currently activated for Boot Mode
    psAPPMANACTIVE,             //; currectly activated for Application Manager
    psDATAPAGE                  //; Application Data Page
} PAGESTATUS;

#define OUT_OF_RANGE 0xFF       //; requested page not in table

//; Old Format Page Status Table Description
//;	(1st Boot Page, 12 Physical RAM Pages)
#define NUMPAGE 13		//; Maximum Page Number
				//; (1st Boot Page, 12 Physical RAM Pages)

#define PAGENAMELEN 10         	//; Length of Software name
#define PAGEVERLEN  6          	//; Length of Software version

typedef struct page_stat_blk {
        UBYTE sname[PAGENAMELEN];   // SOFTWARE NAME
        UBYTE sversion[PAGEVERLEN]; // SOFTWARE VERSION
        UBYTE l_date[16];           // LOAD DATE
        UBYTE pstatus;              // PAGE STATUS
        UBYTE *pend;                // PAGE END ADDRESS
        UWORD pscheck;              // PAGE CHECKSUM
} PAGESTATBLK;
#define P_STATL sizeof(PAGESTATBLK)   

//; Extended Memory Page Status Table description
//;	(1st Boot Page, 47 Physical RAM Pages)

#define NUMPAGE_XM 	48	//; Maximum page number
    				//; (1st Boot Page, 47 Physical RAM Pages)
#define NUMPAGELOG_XM 	60	//; Number of logical page entries

typedef struct page_stat_blk_xm {
        UBYTE pstatus_xm;               // PAGE STATUS
        UBYTE pnumphy_xm;               // PAGE NUMBER (PHYSICAL)
        UBYTE *pstart_xm;               // PAGE START ADDRESS
        UBYTE *pend_xm;                 // PAGE END ADDRESS
        UWORD pscheck_xm;               // PAGE CHECKSUM
} pagestatblk_xm;
#define P_STATL_XM sizeof(pagestatblk_xm)   // DEFINE STRUCTURE SIZE

typedef struct page_stat_blk_t6k {
        UBYTE pstatus_t6k;	// PAGE STATUS
        UBYTE *pstart_t6k;	// PAGE START ADDRESS
        UBYTE *pend_t6k;	// PAGE END ADDRESS
        UWORD pscheck_t6k;	// PAGE CHECKSUM
} PAGESTATBLK_T6K;

#define P_STATL_T6K sizeof(PAGESTATBLK_T6K)   //; DEFINE STRUCTURE SIZE

//; ----------------------------

//; Load Updated Flag (LOAD_UPDATED) Equates:

#define LUP_CLEAR     0         //; Initial state prior to loading
#define LUP_NMS       1         //; Loaded via NMS
#define LUP_TERMINAL  2         //; Loaded from a Terminal (FastLoad/Clone)

//;      LOGICAL PAGE NUMBERS
//;	(1 32k Page of the RAM is for Upper Memory 0x8000--0xffff)
enum LOGICAL_PAGE_NUMBERS {
	MPB3 = -3,	//; Select Boot Page 3
	MPB2,		//; Select Boot Page 2
	MPB1,		//; Select Boot Page 1

	MP0,		//; Select Boot 0 & Page  0 - 512k
	MP1,		//; Select Page  1 - 512k
	MP2,		//; Select Page  2 - 512k
	MP3,		//; Select Page  3 - 512k
	MP4,		//; Select Page  4 - 512k
	MP5,		//; Select Page  5 - 512k
	MP6,		//; Select Page  6 - 512k
	MP7,		//; Select Page  7 - 512k
	MP8,		//; Select Page  8 - 512k
	MP9,		//; Select Page  9 - 512k
	MP10,		//; Select Page 10 - 512k
	MP11,		//; Select Page 11 - 512k
	MP12,		//; Select Page 12 - 512k
	MP13,		//; Select Page 13 - 512k
	MP14,		//; Select Page 14 - 512k

	MP15,		//; Select Page 15 - 1024k (512k if Flash)
	MP16,		//; Select Page 16 - 1024k 
	MP17,		//; Select Page 17 - 1024k
	MP18,		//; Select Page 18 - 1024k
	MP19,		//; Select Page 19 - 1024k
	MP20,		//; Select Page 20 - 1024k
	MP21,		//; Select Page 21 - 1024k
	MP22,		//; Select Page 22 - 1024k
	MP23,		//; Select Page 23 - 1024k
	MP24,		//; Select Page 24 - 1024k
	MP25,		//; Select Page 25 - 1024k
	MP26,		//; Select Page 26 - 1024k
	MP27,		//; Select Page 27 - 1024k
	MP28,		//; Select Page 28 - 1024k
	MP29,		//; Select Page 29 - 1024k
	MP30,		//; Select Page 30 - 1024k

	MP31,		//; Select Page 31 - 1536k (1024k if Flash)
	MP32,		//; Select Page 32 - 1536k
	MP33,		//; Select Page 33 - 1536k
	MP34,		//; Select Page 34 - 1536k
	MP35,		//; Select Page 35 - 1536k
	MP36,		//; Select Page 36 - 1536k
	MP37,		//; Select Page 37 - 1536k
	MP38,		//; Select Page 38 - 1536k
	MP39,		//; Select Page 39 - 1536k
	MP40,		//; Select Page 40 - 1536k
	MP41,		//; Select Page 41 - 1536k
	MP42,		//; Select Page 42 - 1536k
	MP43,		//; Select Page 43 - 1536k
	MP44,		//; Select Page 44 - 1536k
	MP45,		//; Select Page 45 - 1536k
	MP46,		//; Select Page 46 - 1536k

	MP47,		//; Select Page 47 - 2048k (1536k if Flash)
	MP48,		//; Select Page 48 - 2048k
	MP49,		//; Select Page 49 - 2048k
	MP50,		//; Select Page 50 - 2048k
	MP51,		//; Select Page 51 - 2048k
	MP52,		//; Select Page 52 - 2048k
	MP53,		//; Select Page 53 - 2048k
	MP54,		//; Select Page 54 - 2048k
	MP55,		//; Select Page 55 - 2048k
	MP56,		//; Select Page 56 - 2048k
	MP57,		//; Select Page 57 - 2048k
	MP58,		//; Select Page 58 - 2048k
	MP59,		//; Select Page 59 - 2048k
	MP60,		//; Select Page 60 - 2048k
	MP61,		//; Select Page 61 - 2048k
	MP62,		//; Select Page 62 - 2048k
	MP63		//; Select Page 63 - 2048k if flash
};

//; Display Line Equates
enum DISPLAY_LINES {
	UPPER = 0,	//; Upper Line
	LOWER		//; Lower Line
};

//; Buzzer definitions

#define BORIDE	0              // buzzer override
#define BUZON	1              // buzzer on

//; General definitions :

#define FORMSIZE 33		//; lines per form

#if defined( LF )
#undef LF
#endif
#if defined( FF )
#undef FF
#endif
#if defined( CR )
#undef CR
#endif
#if defined( SO )
#undef SO
#endif
#if defined( BS )
#undef BS
#endif
#if defined( DC2 )
#undef DC2
#endif
#if defined( DC4 )
#undef DC4
#endif
#if defined( CAN )
#undef CAN
#endif
#if defined( ESC )
#undef ESC
#endif

#define CR	0x0D		//; Carriage Return
#define LF	0x0A		//; Line Feed
#define FF	0x0C		//; Form Feed
#define SI	0x0F		//; Shift In
#define SO	0x0E		//; Shift Out
#define BS 	0x08		//; Backspace
#define	HT 	0x09		//; Horizontal TAB 
#define	VT 	0x0B		//; Vertical TAB
#define	DC2	0x12		//; Cancel Condensed Mode
#define	DC4	0x14		//; Cancel Double-wide Mode (one line)
#define	CAN	0x18		//; Cancel current line
#define	ESC	0x1B		//; Escape

//; Process State bits :
#if 0
#define WAITIO   0            	//; process waiting for I/O
#define WAITTIME 1            	//; process waiting for Timeout
#define WAITWAKE 2            	//; process waiting for WAKE
#define USERINT  7            	//; process executing a User Interrupt
#endif

//; Timeout Types :

#define TIMENULL 0x00		//; dummy timeout
#define TIMEWAIT 0x01		//; process wait timeout
#define TIMEFLAG 0x02		//; set a flag bit
#define TIMEPINT 0x03		//; process interrupt
#define TIMEDINT 0x04		//; driver interrupt
#define TIMEREPT 0x80		//; repeatable timeout

//; Structure of a PCB :

typedef struct pcb 
{
        struct pcb *pcb_next;	// pointer to next PCB
        UBYTE pcb_type;      	// process type
        UBYTE pcb_pnum;      	// process number
        UBYTE pcb_stat;      	// process state bits
        UBYTE pcb_cont;      	// process context
        UBYTE *pcb_cpsp;     	// process stack pointer
        UBYTE *pcb_xsp;      	// XCall Stack Pointer
        UBYTE pcb_stak[1];   	// bottom of process stack
} PCB;
#define SIZEPCB sizeof(PCB)+SIZESTAK+13

//; Structure of a DQB (Device Queue Block) :

typedef struct dqb {
        UBYTE *dqb_riob;	// pointer to Read IOCS blocks
        UBYTE *dqb_riol;	// last Read IOCS block
        UBYTE *dqb_wiob;	// pointer to Write IOCS blocks
        UBYTE *dqb_wiol;	// last Write IOCS block
} DQB;
#define DQBSZE sizeof(DQB) 	//; DEFINE STRUCTURE SIZE 
              
//; Structure of a TCB :

typedef struct tcb {
        UWORD tcb_next; 	// link to next TCB
        UWORD tcb_time; 	// cumulative time count
        UWORD tcb_rtim; 	// repeat time count
        UBYTE tcb_tnum; 	// timeout number
        UBYTE tcb_type; 	// timeout type
        UBYTE *tcb_cpcb;	// Process PCB or Device DCB
        UBYTE tcb_cont; 	// timeout context
        UBYTE *tcb_ifad;	// interrupt/flag address
        UBYTE tcb_fbit; 	// flag bit number
} TCB;

#define SIZETCB sizeof(TCB)	//; DEFINE STRUCTURE SIZE              
              
//; Process/Device Types :

#define	SYSTEM 	1		//; a system process/device
#define	USER 	2 		//; a user process/device
              
//; Structure of PDT table :

typedef struct pdt 
{
        UBYTE pdt_type;   	// offset of process type
        UBYTE *pdt_addr;  	// offset of start address
        UBYTE pdt_cont;   	// process context
        UBYTE pdt_name[4];	// process name
        UBYTE pdt_numb;   	// process number
} PDT_STRUCT;
#define SIZEPDT sizeof(PDT_STRUCT)  //; DEFINE STRUCTURE SIZE

/*
;
;======================================================================
;
;                   Definitions and Equates
;                           for the
;            Input/Output Control Subsystem (IOCS)
;                            and
;            Process Timer Control Subsystem (PTCS)
*/

//; Command Equates (IOCS) :

#define OPEN	0x01		//; Open Command
#define CLOSE	0x02		//; Close Command
#define REWIND	0x03		//; Rewind Command
#define CONFIG	0x04		//; Configure Command
#define READ	0x05		//; Read Command
#define WRITE	0x06		//; Write Command

//; Command Equates (PTCS) :

#define TIMER	0x10		//; TIMER Command
#define CANCEL	0x11		//; CANCEL Command
#define SLEEP	0x12		//; SLEEP Command
#define WAKE	0x13		//; WAKE Command
#define RELINQ	0x14		//; RELINQ Command

//;	Subcommand/Option Equates (IOCS & PTCS) :

#define PROCEED		0x01	//; proceed or wait for completion
#define WAIT		0x00	//; wait for completion

#ifndef BYPASS_INT_DEF
// The following INT definition conflicts with a typedef 
// for INT in Nucleus.h and therefore must be bypassed
// when when Nucleus.h is required.
//
// Note that the INT defintion here is only used in 
// for OSCLIB and SDKLIB that are not dependent on Nucleus.h.
//
// Define BYPASS_INT_DEF to bypass the 
// following definition
//
#define INT		0x02	//; jump to interrupt routine
#endif

#define FLAG		0x00	//; set a flag bit

//; Subcommand/Options Equates (PTCS) :

#define REPEAT	0x10		//; repetitive timeout

//;	Former IMAGE and IMGBIT defines have been removed.

//; Equates used in decoding Subcommand/Options field :

#define PRCBIT	0		//; bit number of REPEAT bit
#define INTBIT	1		//; bit number of INT bit
#define RPTBIT	4		//; bit number of REPEAT bit

//; Return Code Equates (IOCS & PTCS) :

#define OK	0x00		//; normal completion
#define ERRADDR	0x01		//; PTCS/IOCS block address error
#define ERRCMMD	0x02		//; unknown Command
#define ERRSCMD	0x03		//; unknown Subcommand/Option
#define ERRDNUM	0x04		//; unknown Device Number
#define ERRBUFF	0x05		//; Buffer address error
#define ERRIFAD	0x06		//; Interrupt/Flag Address error
#define ERRIOCS 0x0F		//; IOCS Block is Already in Use error
#define ERRCNS	0x10		//; Command not supported
#define ERRSNS	0x11		//; Subcommand/Option not supported
#define ERRMNS	0x12		//; Mode not supported
#define ERRCNV	0x13		//; configuration not valid
#define ERRDTO	0x14		//; device time out
#define ERRBOF	0x15		//; buffer overflow
#define ERRBSY	0x16		//; device busy
#define ERRDNA	0x17		//; device not available
#define ERRFLT	0x18		//; device fault
#define ERRDNO	0x19		//; device not open
#define ERRRWD	0x1A		//; device rewound during I/O
#define ERRCNF	0x1B		//; device configured during I/O
#define ERRDSC	0x1C		//; device disconnected during I/O
#define ERRRST	0x1D		//; device reset during I/
#define ERRCLS	0x1E		//; device closed during I/O
#define ERRNTA	0x31		//; No Timers Available
#define ERRTNF	0x32		//; Timer Not Found
#define ERRTIM	0x33		//; Invalid Time Interval
#define ERRPWS	0x34		//; Process in Wrong State
#define ERRPNF	0x35		//; Process Not Found
#define ERRPTY	0x40		//; Parity error in data
#define ERRLRC	0x41		//; LRC error in data
#define ERROVR	0x42		//; data overrun
#define ERRINV	0x43		//; invalid data
#define ERRNAK	0x44		//; negative acknowledge
#define ERRFRM	0x45		//; framing error
#define ERRNOL	0x46		//; NO DIAL LINE AVAILABLE
#define ERRXFC	0x47		//; XMODEM 1st Block Error CRC
#define ERRXFL	0x48		//; XMODEM 1st Block Error LRC
#define ERRLBSY	0x49		//; BUSY tone detected on the line
#define ERRNOAN	0x4A		//; continuous ringback signal is detected 
				//; on theline (NO ANSWER)
#define ERRPOUT	0x50		//; paper out (lpt)
#define ERROVHT	0x51		//; over heat (lpt)
#define ERRCTL  0x60		//; lwk - Contactless General Error
#define ERRREQ  0x61		//; lwk - Contactless Request Error (0x11)
#define ERRSEL	0x62		//; lwk - Contactless Select Error (0x21)
#define ERRUSP	0x63		//; lwk - Contactless Unsupported Card Error (0x31)
#define ERRATH	0x64		//; lwk - Contactless Authentication Error (0x41)
#define ERRUNV	0x65		//; lwk - Value Underflow (02)
#define ERROVV	0x66		//; lwk - Value Overflow (03)
#define ERRDCD	0xE0		//; DCD lost
#define ERRDMR	0xE1		//; DM received
#define ERRSNE	0xE2		//; Sending error
#define ERRUXS	0xE3		//; Unexpected SNRM received
#define ERRPOL	0xE4		//; Loss of polling
#define ERRXTO	0xE5  		//; XMODEM Read Timeout
#define ERRLBFRM 0xE6 		//; Invalid LAPM Frame received
#define ERRLBNR  0xE7 		//; Invalid LAPM Receive Sequence Number (Nr) received
#define ERRLRTO  0xE8 		//; LAPM Response Time-out Error
#define ERRLDM   0xE9 		//; LAPM Disconnected Mode (DM) Frame received
#define ERRLDISC 0xEA 		//; LAPM Disconnect (DISC) Frame received
#define ERRLFRMR 0xEB 		//; LAPM Frame Reject (FRMR) Frame received

//; Union type handle Interrupt/Flag address
typedef union ifad_type 
{
	UBYTE* pubyte;
	void  (*pfunc)(void);
}IFADTYPE;

//; Structure of a PTCS block :

typedef struct ptcs_blk 
{
        UBYTE ptcs_cmnd; 	// Command
        UBYTE ptcs_scmd; 	// Subcommand/Options
        UBYTE ptcs_tidna;	// Timer Identity Number
        UBYTE ptcs_statp;	// Returned Status
	IFADTYPE ptcs_ifad;
        UBYTE ptcs_fbit; 	// Flag bit number
        UWORD ptcs_time; 	// Time Interval
        UBYTE ptcs_cont; 	// PTCS context
} PTCS_BLK;

#define PTCSSIZE sizeof(PTCS_BLK) //; DEFINE STRUCTURE SIZE 

//; "C" structure for an IOCS Block :

typedef union ddda_type
{
	UBYTE *pubyte;
	UWORD data;
} IOCS_DDDA_DATA;

//; Additional structure for an IOCS CONFIG block :

typedef struct iocs_cfg_data {
        UBYTE iocs_mode;                // Mode
        UBYTE iocs_cfgfiller;           // filler byte - not used?
	IOCS_DDDA_DATA iocs_ddda;		// Device Dependent Data
	IOCS_DDDA_DATA iocs_ddda2;		// Device Dependent Data second part
} IOCS_CFG_DATA;

//; Additional structure for an IOCS READ or WRITE block :

typedef struct iocs_xfer_data {
        UBYTE *iocs_bufs;               // Buffer Start Address
        UBYTE *iocs_bufe;               // Buffer End Address
        UWORD iocs_tlenx;               // Transfer length
} IOCS_XFER_DATA;

//; Additional union for request type dependent IOCS blocks :

typedef union iocs_var_data {
        IOCS_CFG_DATA  iocs_cfg;        // CONFIG request data
        IOCS_XFER_DATA iocs_xfer;       // READ/WRITE request data
} IOCS_VAR_DATA;

//; Structure of an IOCS return block:

typedef struct iocs_ret_blk 
{
        UBYTE iocs_stat;                // Returned Status
        UWORD iocs_tlen;                // Transfer Length
} IOCS_RET_BLK;

//; Actions to be taken during IOCS calls

typedef enum iocs_action 
{
	IA_WAIT = 0,				//; WAIT
	IA_PROC_AND_FLAG = 1,		//; PROC + FLAG
	IA_PROC_AND_INT = 3			//; PROC + INT
} IOCS_ACTION;

//; Structure common to all IOCS blocks :

typedef struct iocs_blk 
{
	UBYTE iocs_cmnd;				// Command
	UBYTE iocs_scmd;				// Subcommand/Options
	UBYTE iocs_dnum;				// Device Number
	UBYTE iocs_stati;				// Returned Status
	IFADTYPE iocs_ifad;				// Interrupt/Flag Address
	UBYTE iocs_fbit;				// Flag bit number
	struct iocs_blk *iocs_next;		// Next IOCS (RESERVED)
	PCB  *iocs_cpcb;				// Callers PCB (RESERVED)
	UBYTE iocs_cont;				// I/O Context
	IOCS_VAR_DATA iocs_var;			// Request Type Dependent Data
} IOCS_BLK;
#define IOCSSIZE sizeof(IOCS_BLK)	//; DEFINE STRUCTURE SIZE

//; Structure of a PTCS return block:

typedef struct ptcs_ret_blk
{
        UBYTE ptcs_stat;                // Returned Status
        UWORD ptcs_tidn;                // Timer Identity Number
} PTCS_RET_BLK;

//; Actions to be taken during PTCS calls

typedef enum ptcs_action 
{
        PA_WAIT = 0,                          //; WAIT
        PA_PROC_AND_FLAG = 1,                 //; PROC + FLAG
        PA_PROC_AND_INT = 3,                  //; PROC + INT
        PA_PROC_AND_FLAG_THEN_REPEAT = 0x11,  //; PROC + FLAG + REPEAT
        PA_PROC_AND_INT_THEN_REPEAT = 0x13    //; PROC + INT + REPEAT
} PTCS_ACTION;

typedef struct
{
   UBYTE Type;           	// type of the process
   void  (*pProcess)( ); 	// Address of the process
   char  Name[4];        	// 4 character name
} PROCESS;

#endif // EXECDEF_H
