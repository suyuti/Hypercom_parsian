//=============================================================================
//                      Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             tables.c
//      Validate for Tables
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"

#include "utllib.h"

#include "defines.h"
#include "transdef.h"
#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "fixdata.h"
#include "SIM_Init.h"
#include "rs232tab.h"
#include "perfutil.h"
#include "tabutil.h"
#include "text.h"

#ifdef MAKE_EMV
#include "emv_l2.h"
#include "emv2_pk.h"
#include "emvtabs2.h"
#endif // MAKE_EMV

#include "tables.h"
#include "DbgTrace.h"

//=============================================================================
// External variables / declarations
//=============================================================================

// Public functions defined in this module

// Public from other modules
#ifdef	MAKE_EMV
extern void Val_EMVTabs1( void );
#endif	//MAKE_EMV

//=============================================================================
// Private defines and typedefs
//=============================================================================



//=============================================================================
// Private function declarations
//=============================================================================

static void CheckNMS( void );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief Checks all Data Tables for consistency and format them if necessary
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
void CheckTables( void )
{
//============================================================================
//
// Validate tables and force "init required" if anybody is bad
//
//============================================================================

	// NMS Table 
	CheckNMS();

	// Validate Terminal Table 
	if ( !ValidateTable( (UBYTE *)&TERMFMT, (UBYTE *)&TERM, SZTERM, (UBYTE *)&TERMLRC ) )
	{
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<TERM>>: table missing or corrupt,clearing...\n" );
		// Bad; so initialize the Terminal Table 
		InitTermTab();
	}

//-----------------------------------------------------------------------------
// TNMS downloaded tables
//-----------------------------------------------------------------------------
	// Terminal Configuration table 
	if ( !ValidateTable( &TCFMT, ( UBYTE * ) & TCONF, TCSZE, &TCLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<TCONF>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( TCONF_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

    // terminal capabilities table 
    if ( !ValidateTable( &TCAPFMT, ( UBYTE * ) & TCAP, TCAPSZE, &TCAPLRC)) 
    {
        
    }


    // Card Issuer Tables
	if ( !ValidateTable( &ISFMT, ( UBYTE * ) ISTAB, ISSZE * ISMAX, &ISLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<ISTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( ISS_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

  	// Card Acquirer Tables
	if ( !ValidateTable( &AQFMT, ( UBYTE * ) AQTAB, AQSZE * AQMAX, &AQLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<AQTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( ACQ_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

  	// Visa 2 Tables
	if ( !ValidateTable( &V2FMT, ( UBYTE * ) V2TAB, V2SZE * V2MAX, &V2LRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<V2TAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( V2_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

	// Descriptor Tables 
	if ( !ValidateTable( &DSFMT, ( UBYTE * ) DSTAB, DSSZE * DSMAX, &DSLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<DSTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( DESC_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

    // Card Range Tables
  	if ( !ValidateTable( &CRFMT, ( UBYTE * ) CRTAB, CRSZE * CRMAX, &CRLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<CRTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( CARD_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

	// Idle Screen Button Table
    if ( !Val_IBtab() )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<IBTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( IDLB_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

	// IcePak Configuration Table 
	if ( !ValidateTable( &IPFMT, ( UBYTE * ) & IPTAB, IPSZE, &IPLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<IPTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( IPAK_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
        SetIPTabLRC();
    }

	// Additional prompt Tables  
	if ( !ValidateTable( &APFMT, ( UBYTE * ) APTAB, APSZE * APMAXENT, &APLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<APTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( ADPR_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
        SetAPTabLRC();
    }

#ifdef MAKE_EXTRA_TABLES
    // PDN Table
  	if ( !ValidateTable( &PDNFMT, ( UBYTE * ) PDNTAB, PDNSZE * PDNMAX, &PDNLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<PDNTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( PDN_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }
#endif // MAKE_EXTRA_TABLES

  	// Receipt footers Table
  	if ( !ValidateTable( &FTFMT, ( UBYTE * ) & FTTAB, FTSZE, &FTLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<FTTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( FOOT_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

	// IP Connect Table
	if ( !ValidateTable( &IPCFMT, ( UBYTE *) & IPCTAB, IPCMAX * IPCSZE, &IPCLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<IPCTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( IPC_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }


//-----------------------------------------------------------------------------
// EMV tables
//-----------------------------------------------------------------------------
#ifdef MAKE_EMV

    // Validate EMV key storage
	EMVL2_ValidateKeyStore();
    
    // CA Public Key table
  	if ( !ValidateTable( &PKFMT, (UBYTE *)&PublicKeyStore, KEYSZE*KEYMAX, &PKLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<PublicKeyStore>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( CAPK_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

    // Revocation key table
  	if ( !ValidateTable( &RKFMT, (UBYTE *)&RevocationTable, REVSZE, &RKLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<Revocation>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( REVC_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

    // Key expiry table
  	if ( !ValidateTable( &KEFMT, (UBYTE *)&KeyExpiryTable,  KEYMAX*KEYEXPSZE, &KELRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<KeyExpiry>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( KEXP_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

	// ICC Table 
  	if ( !ValidateTable( &ICCFMT, (UBYTE *)ICCTAB, ICCSZE*ICCMAX, &ICCLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<ICCTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( ICC_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

    // TAID Table
    if ( !ValidateTable( &TAIDFMT, (UBYTE *)TAIDTAB, TAIDSZE*TAIDMAX, &TAIDLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<TAIDTAB>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( TAID_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }

    // Additional Tags Table
#ifdef SUPP_ADDICCTAGS
  	if ( !ValidateTable( &ADTFMT, (UBYTE * ) ADTData, EMVL2_ADT_MAX * sizeof(ADTRecType), &ADTLRC ) )
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<ADTData>>: table missing or corrupt,clearing...\n" );
#ifdef MAKE_SMARTINIT
        memset ( ADTG_HASH, 0, SM_HASH_LEN );
        memset ( MAIN_HASH, 0, SM_HASH_LEN );
#endif // MAKE_SMARTINIT
    }
#endif // SUPP_ADDICCTAGS
    
#endif // MAKE_EMV


//-----------------------------------------------------------------------------
// Internal tables (no TNMS downloaded)
//-----------------------------------------------------------------------------

    // Text on 4th line of  receipt/report header (Fun_26) 
	if (!ValidateTable( &USRDFMT, ( UBYTE * ) USRDTAB, USRDSZE, &USRDLRC ))
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<TERM>>: table missing or corrupt,clearing...\n" );
        SetUsrdTabLRC();
    }

	// Batch Control Tables
	if (!ValidateTable( &BATFMT, ( UBYTE * ) & BATTAB, BATSZE * AQMAX, &BATLRC ))
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<BATTAB>>: table missing or corrupt,clearing...\n" );
    }

	// Terminal Flag Indicator Table 
	if (!ValidateTable( &TFFMT, ( UBYTE * ) & TFTAB, TFSZE, &TFLRC ))
    {
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<TFTAB>>: table missing or corrupt,clearing...\n" );
    }

    // Cashier Table
    /* MSD 
	if (!ValidateTable( &CASHFMT, ( UBYTE * ) CASHTAB, CASHIER_SZE * CASHIER_MAX, &CASHLRC ))
    {
		DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<CASHTAB>>: table missing or corrupt,clearing...\n" );
        SetCashTabLRC();
    }
    */

    // Local Parameters Table
	if (!ValidateTable ( &LOCFMT, ( UBYTE * ) & LOCTAB, LOCSZE, &LOCLRC  ))
	{
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<LOCTAB>>: table missing or corrupt,clearing...\n" );
		// Set Idle Button initialization to Enable 
		LOCTAB.IBTAB_INIT = True;
		// Set default language to LOCTAB.LANGID
        SetLanguageDefault ( ); 
		SetLOCTabLRC();
	}

//============================================================================
//
// Validate local tables using ValidateTable_NC method -
//  DO NOT force "init required" if anything is bad
//
//============================================================================

    // RS232 configuration table
	if ( !ValidateTable_NC( &RSFMT, (UBYTE *)&RSTAB, RSSZE, &RSLRC ) )
	{
        DBGTRACE (LVL_NORMAL, TYPE_SPOS32, "<<RSTAB>>: table missing or corrupt,clearing...\n" );
		TableInitRS232 ();
	}
	
    // History/Performance Report Tables
    Valrptab ();	

}

//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief Checks the NMS Table and format if required
//! 
//! \return
//!     None.
//-----------------------------------------------------------------------------
static void CheckNMS( void )
{
	// Check if the NMS Table needs formatting 
	if ( 0x55 != NMSFMT )
	{
		// Yes; clear the NMS Table 
		memset( ( UBYTE * ) & NMSDAT, 0, NMSZE );

		// Mark as formatted 
		NMSFMT = 0x55;
	}
}

