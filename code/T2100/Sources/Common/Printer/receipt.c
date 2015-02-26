
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
static UBYTE GetReport();

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
//	PreprintReceipt		Print the receipt header.
//
//	Parameters: 		None.
//
//	Global Inputs:		None.
//
//	Returns:			Nothing.
//
//	Notes:				None.
//
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
//	Receipt			Generate the RECEIPT.and send to a file which is printed 
//					by the Print Server.
//					It handles both the roll and sprocket printer
//
//	Parameters: 	None.
//
//	Global Inputs:	None.
//
//	Returns:		Nothing.
//
//	Notes:			None.
//
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

	Report = GetReport();

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

	if ( TRINP.TRRSPOPT & R_APPROVED )
	{
		// Should we print a second copy? 
		switch ( TCONF.TCPRINTER )
		{
			case prt_EPL205:		// ICE4000 thermal with cutter 
			case prt_EPL1920ICE:	// ICE thermal with cutter 
			case prt_FTP628:		// FTP628 for ICE5500 plus 
			case prt_T77THERMAL:	// Thermal with cutter 
                                
                                // Puan sorgu da tek slip basýlýyor
                                if ( BALINQ == TRINP.TRKEY  ) 
                                  break;

				// Print a second Receipt? 
				if ( !TRINP.TRREPRINT && !(TCONF.TCOPT6 & TC6_DIS2NDRCPT))
				{                                                                                                          
                                  // Initialize printer PCL data and flags 
                                  PrintInit(  );

                                  // Set print option to show printing second receipt.    
                                  PROPT |= OPT5MASK;

                                  ClickKey ();
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

/*------------------------------------------------
 *  Name:           GetReport
 *  Description:    Get Report
 *  Include files:  
 *  Parameters:     None.
 *  Return Value:   UBYTE (Report No)
 *  Notes:          None.
 *-----------------------------------------------*/
static UBYTE GetReport()
{
	switch(TRINP.TRKEY)
	{
            case SALE:
            case INSTALLMENT:	
            case MAIL_ORDER:
            case VOIDTRAN:
            case REFUND:
            case INSTAL_REFUND:
            case PREAUTH:
            case CLOSED_PREAUTH:
                return RP_TRANS_SALE;
            case BALINQ:
                return RP_TRANS_BALINQ;                            
            default :        
                return RP_RECEIPT;		
	}

	return RP_RECEIPT;
}




// @ydeniz -- 2. nüsha için bekleme
extern void ClickKey( void )
{
  ShowInfoMsg( N_ClickKey, N_SecondReceipt );  
  GetOneKey( DEV_KBD |DEV_PAD |STAY_OPEN0, 5);  
}
//=============================================================================
// Private function definitions
//=============================================================================
