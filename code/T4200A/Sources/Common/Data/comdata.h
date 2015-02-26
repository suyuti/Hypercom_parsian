
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
#ifndef _COMDATA_H_
#define _COMDATA_H_
//=============================================================================
//
// Module overview:             comdata.h
//      Data declarations for comdata region
//
//=============================================================================

#include "perfutil.h"

//=============================================================================
// Public defines and typedefs
//=============================================================================
#define SM_HASH_LEN 16 // SmartInit HASH length


//=============================================================================
// Public data declarations
//=============================================================================

extern UBYTE TCFMT;                 // TERMINAL CONTROL FORMAT INDICATOR
extern struct termconf_rec TCONF;	// TABLE
extern UBYTE TCLRC;                 // LRC OF TABLE

extern UBYTE TCAPFMT;                 // TERMINAL CONTROL FORMAT INDICATOR
extern struct termcapability_rec TCAP;	// TABLE
extern UBYTE TCAPLRC;                 // LRC OF TABLE

extern UBYTE AQFMT;
extern struct acquirer_rec AQTAB[AQMAX];
extern UBYTE AQLRC;

extern UBYTE ISFMT;
extern struct issuer_rec ISTAB[ISMAX];  // Issuer table
extern UBYTE ISLRC;

extern UBYTE CRFMT;                          
extern struct card_rec CRTAB[CRMAX];    // Card range table
extern UBYTE CRLRC;							 

extern UBYTE DSFMT;
extern struct desc_rec DSTAB[DSMAX];	// Descriptor table
extern UBYTE DSLRC;

extern UBYTE IBFMT;
extern struct idlescr_rec IBTAB;	// Idle Screen Buttons Table
extern UBYTE IBLRC;

extern UBYTE CASHFMT;
extern struct cashier_rec CASHTAB[CASHIER_MAX];	// Cashier Table
extern UBYTE CASHLRC;

extern UBYTE APFMT;
extern struct addprompt_rec APTAB[APMAXENT];	// additional prompts Table
extern UBYTE APLRC;

extern UBYTE BATFMT;
extern struct batch_rec BATTAB[AQMAX];	// Batch Table
extern UBYTE BATLRC;

extern UBYTE TFFMT;
extern struct termflg_rec TFTAB;	// Terminal Flag Indicator table
extern UBYTE TFLRC;

extern UBYTE USRDFMT;			// 4th line of receipt/report header
extern UBYTE USRDTAB[USRDSZE];
extern UBYTE USRDLRC;

extern UBYTE V2FMT;
extern struct v2_rec V2TAB[V2MAX];	// Visa2 Table
extern UBYTE V2LRC;

extern UBYTE IPFMT;
extern struct stipac IPTAB;		// ICEPAK confiiguration table
extern UBYTE IPLRC;

extern UBYTE FTFMT;
extern struct ft_table FTTAB;	// Footer Table configuration table
extern UBYTE FTLRC;

extern UBYTE SCHFMT;
extern struct schedule_rec SCHTAB[SCHMAX];	// Scheduler Table config table
extern UBYTE SCHLRC;

extern UBYTE LOCFMT;
extern struct localvalues_rec LOCTAB;	// Local parameter table
extern UBYTE LOCLRC;

extern UBYTE PFFMT ;					// Performance Report Format
extern struct pf_table PFTAB;			        // Performance report Table
extern UBYTE PFLRC;

extern UBYTE IPCFMT;
extern struct ipcontab IPCTAB[IPCMAX]; 		// IP Connect Table
extern UBYTE IPCLRC;

extern UBYTE RSFMT;
extern struct rs232_rec RSTAB;				/* RS232 configuration table */
extern UBYTE RSLRC;

extern UBYTE PDNFMT;                        /* FORMAT INDICATOR */
extern PDNSTRUCT PDNTAB[PDNMAX];			/* PDN table */
extern UBYTE PDNLRC;						/* LRC OF TABLE */

#ifdef MAKE_SMARTINIT
extern Bool SmartInitEnabled;         // Flag to enable SmartInit functionality

//------------------------------------------------------------
// SmartInit HASH data
// Main HASH of all TM downloaded tables
extern UBYTE MAIN_HASH[SM_HASH_LEN];  

// HASHes of separate TM downloaded tables and files

extern UBYTE TCONF_HASH[SM_HASH_LEN]; // Terminal configuration
extern UBYTE ACQ_HASH[SM_HASH_LEN];   // Acquirer
extern UBYTE ISS_HASH[SM_HASH_LEN];   // Issuer
extern UBYTE CARD_HASH[SM_HASH_LEN];  // Card Range
extern UBYTE DESC_HASH[SM_HASH_LEN];  // Descriptor
extern UBYTE IDLB_HASH[SM_HASH_LEN];  // Idle screen buttons
extern UBYTE ADPR_HASH[SM_HASH_LEN];  // Additional prompts
extern UBYTE RPT_HASH[SM_HASH_LEN];   // Report downloaded
extern UBYTE FOOT_HASH[SM_HASH_LEN];  // Receipt footers
extern UBYTE V2_HASH[SM_HASH_LEN];    // Visa 2
extern UBYTE IPAK_HASH[SM_HASH_LEN];  // ICE-Pak
extern UBYTE IPC_HASH[SM_HASH_LEN];   // IP connection
extern UBYTE PDN_HASH[SM_HASH_LEN];   // PDN
extern UBYTE PFNT_HASH[SM_HASH_LEN];  // Printer font
extern UBYTE GRPH_HASH[SM_HASH_LEN];  // Graphic file downloaded
#ifdef	MAKE_EMV
extern UBYTE ICC_HASH[SM_HASH_LEN];   // ICC
extern UBYTE TAID_HASH[SM_HASH_LEN];  // TAID
extern UBYTE CAPK_HASH[SM_HASH_LEN];  // CA Public keys
extern UBYTE REVC_HASH[SM_HASH_LEN];  // Revocated keys
extern UBYTE ADTG_HASH[SM_HASH_LEN];  // Additional tags
extern UBYTE KEXP_HASH[SM_HASH_LEN];  // Key expiry
#endif //MAKE_EMV
#endif // MAKE_SMARTINIT

//=============================================================================
// Public function declarations
//=============================================================================


#endif // _COMDATA_H_
