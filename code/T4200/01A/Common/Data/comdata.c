
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             comdata.c
//      Data defined in comdata region
//
//=============================================================================

#include "basictyp.h"

#include "struct.h"
#include "perfutil.h"
#include "rs232tab.h"
#include "collect.h"
#include "comdata.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================


//=============================================================================
// Public data definitions
//=============================================================================

// Terminal Configuration table
__attribute__((section("comdata")))
UBYTE TCFMT = 0;                 // FORMAT INDICATOR
__attribute__((section("comdata")))
struct termconf_rec TCONF = {0}; // TABLE CONTENT
__attribute__((section("comdata")))
UBYTE TCLRC = 0;                 // LRC OF TABLE

// Terminal Capability Table 
__attribute__((section("comdata")))
UBYTE TCAPFMT = 0;                 // FORMAT INDICATOR
__attribute__((section("comdata")))
struct termcapability_rec TCAP = {0}; // TABLE CONTENT
__attribute__((section("comdata")))
UBYTE TCAPLRC = 0;  


// Card Acquirer table
__attribute__((section("aqtab")))
UBYTE AQFMT = 0;                      
__attribute__((section("aqtab")))
struct acquirer_rec AQTAB[AQMAX] ={{0}}; 
__attribute__((section("aqtab")))
UBYTE AQLRC = 0;                     

// Issuer table
__attribute__((section("istab")))
UBYTE ISFMT = 0;
__attribute__((section("istab")))
struct issuer_rec ISTAB[ISMAX] = {{0}};  
__attribute__((section("istab")))
UBYTE ISLRC = 0;

// Card Range table
__attribute__((section("cardrange")))
UBYTE CRFMT = 0;
__attribute__((section("cardrange")))
struct card_rec CRTAB[CRMAX] ={{0}};  
__attribute__((section("cardrange")))
UBYTE CRLRC =0;

// Descriptor table
__attribute__((section("comdata")))
UBYTE DSFMT = 0;
__attribute__((section("comdata")))
struct desc_rec DSTAB[DSMAX] = {{0}}; 
__attribute__((section("comdata")))
UBYTE DSLRC = 0;

// Idle Screen Buttons Table
__attribute__((section("comdata")))
UBYTE IBFMT = 0;
__attribute__((section("comdata")))
struct idlescr_rec IBTAB = {{0}}; 
__attribute__((section("comdata")))
UBYTE IBLRC = 0;


// Additional prompts Table
__attribute__((section("comdata")))
UBYTE APFMT = 0;
__attribute__((section("comdata")))
struct addprompt_rec APTAB[APMAXENT] = {{0}}; 
__attribute__((section("comdata")))
UBYTE APLRC = 0;

// Batch Table
__attribute__((section("comdata")))
UBYTE BATFMT = 0;
__attribute__((section("comdata")))
struct batch_rec BATTAB[AQMAX] = {{0}}; 
__attribute__((section("comdata")))
UBYTE BATLRC = 0;

// Terminal Flag Indicator table
__attribute__((section("comdata")))
UBYTE TFFMT = 0;
__attribute__((section("comdata")))
struct termflg_rec TFTAB = {0}; 
__attribute__((section("comdata")))
UBYTE TFLRC = 0;

// User Data table (4th line on receipt/report header)
__attribute__((section("comdata")))
UBYTE USRDFMT = 0;
__attribute__((section("comdata")))
UBYTE USRDTAB[USRDSZE] = {0};  
__attribute__((section("comdata")))
UBYTE USRDLRC = 0;

// Visa2 Table
__attribute__((section("comdata")))
UBYTE V2FMT = 0;
__attribute__((section("comdata")))
struct v2_rec V2TAB[V2MAX] = {{0}}; 
__attribute__((section("comdata")))
UBYTE V2LRC = 0;

// ICEPAK confiiguration table
__attribute__((section("comdata")))
UBYTE IPFMT = 0;
__attribute__((section("comdata")))
struct stipac IPTAB = {{0}};     
__attribute__((section("comdata")))
UBYTE IPLRC = 0;

// Footer Table configuration table
__attribute__((section("comdata")))
UBYTE FTFMT = 0;
__attribute__((section("comdata")))
struct ft_table FTTAB = {{0}};   
__attribute__((section("comdata")))
UBYTE FTLRC = 0;
__attribute__((section("comdata")))
UBYTE FTHASH[16] = {0};

// Scheduler configuration table
__attribute__((section("comdata")))
UBYTE SCHFMT = 0;
__attribute__((section("comdata")))
struct schedule_rec SCHTAB[SCHMAX] = {{0}}; 
__attribute__((section("comdata")))
UBYTE SCHLRC = 0;

// Local parameters table
__attribute__((section("comdata")))
UBYTE LOCFMT = 0;
__attribute__((section("comdata")))
struct localvalues_rec LOCTAB = {0}; 
__attribute__((section("comdata")))
UBYTE LOCLRC = 0;

// Performance report Table
__attribute__((section("comdata")))
UBYTE PFFMT = 0;
__attribute__((section("comdata")))
struct pf_table PFTAB = {{0}};          
__attribute__((section("comdata")))
UBYTE PFLRC = 0;


// IP Connect Table
__attribute__((section("comdata")))
UBYTE IPCFMT = 0;
__attribute__((section("comdata")))
struct ipcontab IPCTAB[IPCMAX] = {{0}}; 
__attribute__((section("comdata")))
UBYTE IPCLRC = 0;

// COM1/COM2 configuration table
__attribute__((section("RS232data")))
UBYTE RSFMT = 0;
__attribute__((section("RS232data")))
struct rs232_rec RSTAB = {0};  
__attribute__((section("RS232data")))
UBYTE RSLRC = 0;

// Public Data Network (PDN) table
__attribute__((section("pdntab")))
UBYTE PDNFMT = 0;
__attribute__((section("pdntab")))
PDNSTRUCT PDNTAB[PDNMAX] = {{0}};
__attribute__((section("pdntab")))
UBYTE PDNLRC = 0;

// Super Variables
__attribute__((section("superVariables")))
UBYTE SUPERFMT = 0;
__attribute__((section("superVariables")))
struct super_variables SUPER_VARIABLES_TAB = {{0}};
__attribute__((section("superVariables")))
UBYTE SUPERLRC = 0;

#ifdef MAKE_SMARTINIT
Bool SmartInitEnabled = False;		// Flag to enable SmartInit functionality

//------------------------------------------------------------
// SmartInit HASH data
// Main HASH of all TM downloaded tables
__attribute__((section("comdata")))
UBYTE MAIN_HASH[SM_HASH_LEN] = {0};

// HASHes of separate TM downloaded tables
__attribute__((section("comdata")))
UBYTE TCONF_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE ACQ_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE ISS_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE CARD_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE DESC_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE IDLB_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE ADPR_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE RPT_HASH[SM_HASH_LEN] = {0};			

__attribute__((section("comdata")))
UBYTE V2_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE FOOT_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE IPAK_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE IPC_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE PDN_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE PFNT_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE GRPH_HASH[SM_HASH_LEN] = {0};

#ifdef	MAKE_EMV
__attribute__((section("comdata")))
UBYTE ICC_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE TAID_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE CAPK_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE REVC_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE ADTG_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE KEXP_HASH[SM_HASH_LEN] = {0};

__attribute__((section("comdata")))
UBYTE CERT_HASH[SM_HASH_LEN] = {0};
#endif //MAKE_EMV

// Super Variables
//__attribute__((section("superVariables")))
__attribute__((section("comdata")))
UBYTE SUPERFMT = 0;
__attribute__((section("comdata")))
struct super_variables SUPER_VARIABLES_TAB = {{0}};
__attribute__((section("comdata")))
UBYTE SUPERLRC = 0;

#endif // MAKE_SMARTINIT

// MIB counters table
__attribute__((section("comdata")))
UBYTE MIBTRANSFMT = 0;
__attribute__((section("comdata")))
struct mib_trans_table MIBTRANSTAB = {{0}};   
__attribute__((section("comdata")))
UBYTE MIBTRANSLRC = 0;

#ifdef MAKE_SNMP
// MIB writeble objects table
__attribute__((section("comdata")))
UBYTE MIBWRITEOBJFMT = 0;
__attribute__((section("comdata")))
struct mib_writable_obj MIBWRITEOBJ = {{0}};   
__attribute__((section("comdata")))
UBYTE MIBWRITEOBJLRC = 0;
#endif // MAKE_SNMP

// son g�nsonu raporu bas�m� i�in 
__attribute__((section("comdata")))
struct settletotals_rec TEMPSETTLE = {0}; 

__attribute__((section("comdata")))
struct settletotals_rec LASTSETTLE = {0}; 


// field63 tag 42 
__attribute__((section("comdata")))
UBYTE flagTag42 = {0};

__attribute__((section("comdata")))
UBYTE TempInitNo[S_AQPSTEL] = {0};

// son parametre y�kleme tarih/saat
__attribute__((section("comdata")))
UBYTE LastInitDate[RTCLEN] = {0};

// reversal deneme say�s� flagi 
__attribute__((section("comdata")))
 UBYTE revTryCount = {0};;

// flag 
__attribute__((section("comdata")))
extern UBYTE flagTheFirst[2]= {0};

//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================


//=============================================================================
// Private function definitions
//=============================================================================

