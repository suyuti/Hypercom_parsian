//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             receipt.c
//      Receipt Support Functions.
//
//=============================================================================
#include "string.h"
#include "stdlib.h"
#include "ctype.h"

#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "sdkterm.h"
#include "utllib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "defines.h"
#include "msg.h"
#include "prtdef.h"
#include "util.h"
#include "perfutil.h"
#include "tables.h"
#include "prttable.h"
#include "prtdrv.h"
#include "void.h"
#include "respdsp.h"

#ifdef MAKE_EMV
#include "emv_l2.h"
#include "emv2_pk.h"
#include "emvtabs2.h"
#include "emvdata.h"
#include "emvutils.h"
#endif

#include "receipt.h"

//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================


//=============================================================================
// Private function declarations
//=============================================================================
static void PaperCutPrompt( void );
static Bool CheckSignature( void );

//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
static ENTRY_STR  YNEntry;


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Prints the receipt header
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void PreprintReceipt( void )
{
	// If pre-printing has already been called once, just return.        
	// This test allows the preprint call to be inside the bad pin loop. 
	if ( PRFLAG & PF_PREP )
		return;


	// If pre-printing is not enabled, just return 
	if ( !(AQ4_PREPRINT & (TRINP.TRAQPTR->AQOPT4) ) )
		return;

	// Only print the receipt if issuer allows receipts. 
	if ( !( (TRINP.TRISPTR->ISOPT2) & IS2_RECEIPT ) )
		return;


	// Make sure Total Amount with Network Surcharge is set properly 
	AddAmt( TRINP.TRTOTF, TRINP.TRTOTAM, TRINP.TRSURC );

	if ( !( TCONF.TCOPT2 & TC2_PRINT ) )
		return;					// no printer, so don't even try to print 

	// Generate and Send the Receipt header to a file 

	PresetPRFLAG = PF_PREP;		// inform the PCL print driver that this is a Pre-print 

	// Initialize printer PCL data and flags 
	PrintInit(  );

	// Don't use print server for ICE5500 PLUS
	PrintDriver( RP_RECEIPT, &TRINP );

	return;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints the receipt
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void Receipt( void )
{
	Bool PrtStatus;
	enum REPORTTYPE Report;

	// Processing Statistic - Performance Report
	if ( TCONF.TCOPT7 & TC7_PERFORMANCE ) 	
	{
        UpdateStatistic ();
	}
  
	// Only print the receipt if issuer allows receipts. 
	if ( !( (TRINP.TRISPTR->ISOPT2) & IS2_RECEIPT ) )
		return;

#ifdef MAKE_SIGCAPTURE
    // skip Signature capturing if either receipt reprinting or communication failure
    if( !TRINP.TRREPRINT && (TRINP.TRRSPOPT & R_APPROVED) && (TRINP.TRISPTR->ISOPT4 & IS4_SIGCAP) )
    {
        GetSignature( &TRINP );
    }
#endif // MAKE_SIGCAPTURE

	Report = RP_RECEIPT;

	// Did we preprint the receipt header?   
	// (PRFLAG is cleared by PrintPrepare()) 
	if ( PRFLAG & PF_PREP )
	{
		// Yes, Disable printing.  PPSTOP will re-enable printing 
		// So the remainder of the receipt will be printed.       
		PresetPRFLAG = PF_OFFP;
	}

	// Make sure Total Amount with Network Surcharge is set properly 
	AddAmt( TRINP.TRTOTF, TRINP.TRTOTAM, TRINP.TRSURC );

	if ( !( TCONF.TCOPT2 & TC2_PRINT ) )
		return;					// no printer, so don't even try to print 

	// Initialize printer PCL data and flags 
	PrintInit(  );

	// Print the Receipt header 
	PrtStatus = PrintDriver( Report, &TRINP );

	if ( PrtStatus && CheckSignature() && (TRINP.TRRSPOPT & R_APPROVED) )
	{
		// Should we print a second copy? 
		switch ( TCONF.TCPRINTER )
		{
			case prt_EPL205:		// ICE4000 thermal with cutter 
			case prt_EPL1920ICE:	// ICE thermal with cutter 
			case prt_FTP628:		// FTP628 for ICE5500 plus 
			case prt_T77THERMAL:	// Thermal with cutter 
				// Print a second Receipt? 
				if ( !TRINP.TRREPRINT )
				{
					// Initialize printer PCL data and flags 
					PrintInit(  );

					// Set print option to show printing second receipt.    
					PROPT |= OPT5MASK;

					// We want to generate a 2nd receipt  
					PrintDriver( Report, &TRINP );
				}
				break;

			default:			// Non thermal printers 
				break;
		}						// switch 
	}

	return;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Signature Check prompt
//!
//! \return
//!     False if printing is allowed else True
//-----------------------------------------------------------------------------
static Bool CheckSignature( void )
{
	UBYTE key;
	Bool  retval = True;

//	if (APPROVED)
	if( !(TRINP.TRRSPOPT & R_APPROVED) )
		return retval;

//	if (not AUTH)
	if ( AUTH == TRINP.TRKEY )
		return retval;

//	if (REPRINT)
	if (TRINP.TRREPRINT)
		return retval;

	if ( !( TCONF.TCOPT6 & TC6_NOSECOND ) )
	{
		// Generate prompt for paper cutting
		PaperCutPrompt();
	}	// if ( !( TCONF.TCOPT6 & TC6_NOSECOND ) )
	else
	{
		// No 2nd Receipt for CNP Txn
		retval = False;
        return retval;
	}

//	if (CardHolder present)
	if ( 0x30 == (TRINP.TRPOSE[1] & 0xF0) )
	{
		if ( !( TCONF.TCOPT6 & TC6_NOSECOND ) )
		{
			// Generate prompt for paper cutting
			PaperCutPrompt();
		}	// if ( !( TCONF.TCOPT6 & TC6_NOSECOND ) )
		else
		{
			// No 2nd Receipt for CNP Txn
			retval = False;
		}
		return retval;
	}	// if ( 0x30 == (TRINP.TRPOSE[1] & 0xF0) )

	if ( (!( (TRINP.TRISPTR->ISOPT3) & IS3_NOSIGNAT)) 
#ifdef	MAKE_EMV
		 && (!SuppressSigLine)
#endif	// MAKE_EMV
	   )
	{

#ifdef	MAKE_EMV
		/* If an ICC transaction, first ask to remove card,
		   then ask if signature is correct */
		if ( 0x50 == (TRINP.TRPOSE[1] & 0xF0) )
		{
			PerformCardClose ();
		}
#endif	// MAKE_EMV

		YNEntry.TitleMid = Signature;
		YNEntry.Line1Mid = Signature;
		YNEntry.Line2Mid = CorrectYesNo;

		do // Awaiting for Yes or No entry only
		{
			key = YesNoEntry(&YNEntry);
		
		} while (!((key == ENTER_KY) || (key == CLEAR_KY)));
		

		if ( ENTER_KY != key )
		{
			// Check, isn't it already voided
			if ( !(TRINP.TRSTATUS[0] & TS1_VOID) )
			{										
				/* Void the transaction */
				VoidIdent ( 0x0000 );
			}										
			retval = True;
		}

		/* Display Response */
		RespDisplay ();

		return retval;
	}
	else
	{
		// Generate prompt for paper cutting
		PaperCutPrompt();

		retval = True;
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Displays paper cutting prompt
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PaperCutPrompt( void )
{
	if ( !(PrinterTable[TCONF.TCPRINTER].Flags & PR1_CUT) )
	{
		YNEntry.TitleMid = N_Receipt;
		YNEntry.Line1Mid = RemoveMerchantCopy;
		YNEntry.Line2Mid = N_NullStr;

		/* Wait for 'Continue' to be pressed */
		while( 1 !=	ContinueEntryTimeout( &YNEntry ) );

		// Display Response 
		RespDisplay ();
	}
}

//=============================================================================
// Private function definitions
//=============================================================================

