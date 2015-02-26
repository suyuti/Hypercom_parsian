
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             perfutil.c
//      Performance Report
//
//=============================================================================

#include "string.h"

#include "basictyp.h"
#include "fixdata.h"

#include "utllib.h"
#include "struct.h"
#include "comdata.h"
#include "appdata.h"
#include "prtdef.h"
#include "util.h"
#include "perfutil.h"
#include "tables.h"
#include "chkprinter.h"
#include "menu.h"
#include "hstprint.h"
#include "prtdrv.h"
#include "tabutil.h"
#include "PFlrc.h"


//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================
#define VALPERCENT 100			 // Constant to calculate percent 
#define BCD_BOUNDARY 9999   	 // bcd Boundary 


//=============================================================================
// Private function declarations
//=============================================================================
static void SetRptPeriod ( void );
static void SetPftabFld ( UWORD* PfTabFld );
static UWORD CalcPercent ( UWORD x, UWORD y );
static UBYTE ValidateRptTable ( UBYTE * pTabFMT, UBYTE * pTabData, UWORD len,
						   UBYTE * pTabLRC);
static Bool FinantialTransCount ( void );

static void PrintIssAvail ( void );
static void PrintIssTable ( void );
static void PrintAcqAvail ( void );
static void PrintAcqTable ( void );
static void PrintCardAvail ( void );
static void PrintCardTable ( void );
static void AllTableRpt ( void );

static void CardTableRpt ( void );
static void AcqTableRpt ( void );
static void IssuerTableRpt ( void );
static void CalcPerfRpt_Percent ( void );



//=============================================================================
// Public data definitions
//=============================================================================


//=============================================================================
// Private data definitions
//=============================================================================
UWORD RedialResult;
UWORD RetransResult;
UWORD NMS_FirstRedial;  
UWORD NMS_SecondRedial; 
UWORD NMS_FirstRetrans; 
UWORD NMS_SecondRetrans;
static struct acquirer_rec * pAcqPtr;	// Pointer to AQTAB record
static struct issuer_rec * pIssPtr;		// Pointer to ISTAB record
static struct card_rec * pCRPtr;		// Pointer to CRTAB record



//=============================================================================
// Public function definitions
//=============================================================================
//-----------------------------------------------------------------------------
//  INCREMENT_CNT	    Function to increment the data counter for Performance Report 
//
//  Parameters:	 		referred the specific data counter that should be incremented  
//	   Get_kind
//
//  Global Inputs:		
//
//  Returns:	 		Nothing
//  Notes:
//
//-----------------------------------------------------------------------------
extern void IncrementCnt(void)
{
    
    switch (Data_kind)             	// Data type to be incremented 
	{

     case APPROVALS:		        // TOTAL TRANSACTION APPROVED  

           
			 // To increment Counter for this transaction status 
			SetPftabFld( (UWORD*) &PFTAB.TOTAPPROVAL );

			break;

     case DENIALS:	            	// Total Transaction denials 
	    
			// To increment counter for this transaction status 
			SetPftabFld( (UWORD*) &PFTAB.TOTDENIALS );

   		    break;

	 case MANUAL:	             	// Total Transaction in Manual Entry mode 

	        // To increment counter for this transaction entry mode 
			SetPftabFld( (UWORD*) &PFTAB.TOTMANUAL );

			break;

	case SWIPECARD:		            // Total Transaction in Swipe Card mode  

			// To increment counter for this transaction entry mode 
			SetPftabFld( (UWORD*) &PFTAB.TOTSWIPE );

 		  	break;

	case TOTALTRANS:	        	// comm. Total transaction sent to the host 

			// To increment counter for this transaction entry mode 
			SetPftabFld( (UWORD*) &PFTAB.TOTCOUNT );

			break;


	case TTIMEOUT:		            // Comm. Total timeout 

			// To increment counter for TIME OUT 
			SetPftabFld( (UWORD*) &PFTAB.TOTTIMEOUT );

			break;


    default:			        	// Unknown, notify user 

			// Invalid value
		    ShowErrMsg(InvalidEntry);

		    break;


	}
	
	return; 
}

//-----------------------------------------------------------------------------
//  Valrptab     		validate Report Table 
//
//  Parameters:			None
//
//  Global Inputs:		None
//
//  Returns:	    	Nothing
//
//  Notes:
//
//-----------------------------------------------------------------------------
extern void Valrptab (void)
{	

	// ICE Idle Screen Button Table 
	ValidateRptTable(&PFFMT, (UBYTE *) & PFTAB, PFSZE, &PFLRC);

}



//-----------------------------------------------------------------------------
//  Init. Performance Report Table		Init. Performance Report Table
//
//  Parameters:
//		None
//  Global Inputs:
//
//  Returns:
//		Nothing
//  Notes:
//
//-----------------------------------------------------------------------------
extern void InitRptTable(void)
{

	// Table is bad, so fix it 
	InitTable(&PFFMT, (UBYTE *) & PFTAB, PFSZE, &PFLRC);
	      
   	SetRptPeriod();    // After initialize the report table set the new report period 
}

//-----------------------------------------------------------------------------
//  Performance report 	   Print Performance report 
//
//  Parameters:
//		None
//  Global Inputs:
//
//  Returns:
//		Nothing
//  Notes:
//
//-----------------------------------------------------------------------------
extern void PrintPerformRpt(void) 
{
	// See if Printer is available
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller
		return;
	}

    //Calculate Percent for Performance Report 
    CalcPerfRpt_Percent ();

    
	// Initialize printer PCL data and flags 
     PrintInit ();

 	
	// Print "PERFORMANCE REPORT"	
	 PrintDriver ( RP_PERFORMANCE, NULL );

		// Close the Printer 
	SDK_PrinterClose();

	// If printer error was reported then display error here. 
	if (PrinterOK != PrinterErrorFlags)
		DisplayPrintError ();

}

//-----------------------------------------------------------------------------
//  Doconfigrpt		Do configuration Report
//
//  Parameters:
//		None
//  Global Inputs:
//
//  Returns:
//		Nothing
//  Notes:
//
//-----------------------------------------------------------------------------
extern void DoConfigRpt( void )
{
	UBYTE Option = 0;
	UBYTE RetVal = 1;

	ClearMenus ();

	CurrentEvent.NEXTEVENT = 0;

	DrawMenu(MENU_CONFIG_RPT);
		
	// Call the selected Option Processor 
	do
	{


		Option = GetOneKey( DEV_KBD | DEV_PAD | STAY_OPEN0, UserTimeout );

		switch ( Option )
		{
			case 'B':			// Acquirer Table

				// Notify user -- "PRINTING, WAIT..." 
				ShowInfoMsg( PrintingWait, PleaseWait );
				IssuerTableRpt();
				DrawMenu( MENU_REDRAW );  	// redraw menu 

			    RetVal = 1; 
				break;

			case 'C':			// Issuer Table 

				// Notify user -- "PRINTING, WAIT..." 
				ShowInfoMsg( PrintingWait, PleaseWait );
				AcqTableRpt();
				DrawMenu( MENU_REDRAW );  	// redraw menu 

			    RetVal = 1;
				break;

			case 'D':			// Card Table 

				// Notify user -- "PRINTING, WAIT..." 
				ShowInfoMsg( PrintingWait, PleaseWait );
				CardTableRpt();
				DrawMenu( MENU_REDRAW );  	// redraw menu 

	    		RetVal = 1;
				break;

			case 'E':			// ALL Table 

				// Notify user -- "PRINTING, WAIT..." 
				ShowInfoMsg( PrintingWait, PleaseWait );
				AllTableRpt();
				DrawMenu( MENU_REDRAW );  	// redraw menu 

	    		RetVal = 1;
				break;

			case B_NEXT:	// Go to next menu page 
				DrawMenu( MENU_NEXT );
				RetVal = 1;
				break;

			case B_PREVIOUS:
			case B_PREVIOUS_NEW_MENU:
				DrawMenu( MENU_PREVIOUS );
				RetVal = 1;
				break;

			case CANCEL_KY:	// CANCEL key pressed
			case 0:		// Timed out				

				RetVal = 0;
				break;
			   
     		default:
    	    	// Invalid entry; notify user
			    SDK_Beeper( TENMS * 40 );
				DrawMenu( MENU_REDRAW );  	// redraw menu 

			    RetVal = 1;
				break;
		}

	}while (RetVal);

	return;
}
//=============================================================================
// Private function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//  CalcPerf_Percent		CalcPerfRpt_Percent
//
//  Parameters:
//	
//  Global Inputs:
//
//  Returns:	            _Percent Associated  
//	                        
//  Notes:
//
//-----------------------------------------------------------------------------
static void CalcPerfRpt_Percent (void)
{       
	// Percent of Transaction Approved 
	PFTAB.APPROV_PERC = CalcPercent(PFTAB.TOTAPPROVAL, PFTAB.TOTCOUNT);

	// Percent of Transaction Denials 
	PFTAB.DENIALS_PERC = CalcPercent(PFTAB.TOTDENIALS, PFTAB.TOTCOUNT);

	// Percent of Transaction Processed in Manual entry mode 
	PFTAB.MANUAL_PERC = CalcPercent(PFTAB.TOTMANUAL, PFTAB.TOTCOUNT);

	// Percent of the Transaction Processed in Swipe entry mode
	PFTAB.SWIPE_PERC = CalcPercent(PFTAB.TOTSWIPE, PFTAB.TOTCOUNT);

	SetPFTabLRC();
}

//-----------------------------------------------------------------------------
//  ValideRptTable		Check validity of Tables
//
//  Parameters:	     	 pTabFMT
//                       pTabData
//                       len
//                       LRC  
//
//  Global Inputs:
//
//  Returns:			 True if ok, False if bad
//
//  Notes:
//
//-----------------------------------------------------------------------------

static UBYTE ValidateRptTable(UBYTE * pTabFMT, UBYTE * pTabData, UWORD len,
						   UBYTE * pTabLRC)
{
	UBYTE oldlrc;
	UBYTE newlrc;

	// Check if Table has been formatted 
	if (*pTabFMT == 0x55)
	{
		// Yes; get present LRC 
		oldlrc = *pTabLRC;

		// Calculate LRC 
		newlrc = CalcLrc(pTabData, len);

		// Check if LRCs are the same
		if (newlrc == oldlrc)
		{
			// Yes; return okay 
			return (True);
		}
	}

	InitRptTable();			 // Init. Performance Report Table 


    // Clear Terminal Initialization Done Flag 
	Clr_TermInit();

	// Table has not been initialized 
	return (False);
}


//-----------------------------------------------------------------------------
//  CalcPercent		Percent of Approved, Denial, Swipe and Manually 
//                  Transaction.
//  Parameters:		x and y 
//		    
//  Global Inputs:
//
//  Returns:	   Percent value
//		
//  Notes:
//
//-----------------------------------------------------------------------------
static UWORD CalcPercent(UWORD x, UWORD y)
{
	if ( (0 == x) || (0 == y))
	{
		return 0;
	}
	else
	{
		return (UWORD)((100UL * x) / y);
	}
}

//-----------------------------------------------------------------------------
//  SetPftabFld		  	  Increment the counter of the Performance report
//                         database numeric fields. Set the numeric range to process until 9999
//
//  Parameters:
//		None
//  Global Inputs:
//
//  Returns:
//		Nothing
//  Notes:
//
//-----------------------------------------------------------------------------
static void SetPftabFld( UWORD* PfTabFld )
{
	if ( *PfTabFld < BCD_BOUNDARY )
	{
	    switch ( Data_kind )             	// Field Data in processing 
		{

			case REDIALS:					// REDIALS  
				// To increment counter for Retransmits
    			PFTAB.TOTREDIALS =  PFTAB.TOTREDIALS + RedialResult ;

				break;
			case RETRANSMITS:               // RETRANSMIT
				// To increment counter for Retransmits
    			PFTAB.TOTRETRANS = PFTAB.TOTRETRANS + RetransResult ;

				break;
			default:						// Unknown, notify user 
				(*PfTabFld)++;
				break;
		}
	}
	else
	{
		//	Overflow the BCD limit
		*PfTabFld = 0;
		(*PfTabFld)++;
	}

	SetPFTabLRC();
}

//-----------------------------------------------------------------------------
//  SetRptPeriod		Set the Performance report evaluation period
//
//  Parameters:
//		None
//  Global Inputs:
//
//  Returns:
//		Nothing
//  Notes:
//
//-----------------------------------------------------------------------------
static void SetRptPeriod(void)
{
  	UBYTE RtcBuffer[RTCLEN];	// RTC Read Buffer 

	// Put today's date in the PERIODDATE field in MMDDYY format
	SDK_RtcRead(RtcBuffer);	// Read the real time clock.
		  
	memcpy(&PFTAB.PERIODDATE[0], &RtcBuffer[0], S_PERIOD);

    // Calculate PFTAB LRC because the PERIODDATE field was updated 
	SetPFTabLRC();					// Calculate LRC 
}

//-----------------------------------------------------------------------------
//  FinantialTransCount	   Finantial Transaction Counter
//
//  Parameters:
//		
//  Global Inputs:
//
//  Returns:                0 = false, 1 = true
//		
//  Notes:
//
//-----------------------------------------------------------------------------
Bool FinantialTransCount( void )
{
	UBYTE chRetval = 0;

    switch (CurrentEvent.TRNKEY)
    {
      case SALE:
      case REFUND:
	  case VOIDTRAN:
	  case CRDVER:
	  case AUTH:
	  case OPENTAB:
	  case SALCASH:
	  case DBCASH:
	  case MAIL_ORDER: 
	  case DEBITCONF:
#ifdef MAKE_EMV
	  case EMV_SALE:
	  case EMV_INSTALLMENT:
	  case EMV_FALLBACK:
#endif // MAKE_EMV

	     Data_kind = TOTALTRANS;	 // Just  Finantial  Transaction  		 
	     
         // Increase counter for Finantial Transaction - Performance Report  
         IncrementCnt( );
		 
		 // Is a finantial transaction
		  chRetval = 1;

          break;
      
       default:
	      break;


    }

    return ( chRetval );
}

//-----------------------------------------------------------------------------
//  PrintIssuAvail     Print Issuer name available
//
//  Parameters: 		None
//
//  Global Inputs:
//
//  Returns:            Nothing
// 
//  Notes:				Print Issuer names and ID - available in the terminal
//
//-----------------------------------------------------------------------------
static void PrintIssAvail ( void )
{
    // Initialize printer PCL data and flags 
     PrintInit ();

	     // Yes; set Option 1 for Issuer Available 
	   	 PROPT |= OPT1MASK;


	// Loop through the Issuers 
	for ( pIssPtr = &ISTAB[0]; pIssPtr < &ISTAB[ISMAX]; pIssPtr++ )
	{

		// See if Table Populated 
		if ( pIssPtr->ISID )
		{
		   	// Yes; get Issuer Table pointer
		    TRINP.TRISPTR = pIssPtr;
																						
			 // Print "Configuration REPORT"	
            PrintDriver ( RP_ISSUER_CONFIG, &TRINP );
        }
		else
		{
		 return;
		}

	}

}


//-----------------------------------------------------------------------------
//  PrintIssTable		Print Issuer Table
//
//  Parameters: 		None
//
//  Global Inputs:
//
//  Returns:            Nothing
// 
//  Notes:			   Print Issuer Table setup
//
//-----------------------------------------------------------------------------
static void PrintIssTable ( void )
{
	// Initialize printer PCL data and flags 
     PrintInit ();

   	     // Yes; set Option 2 for Issuer table 
    	 PROPT |= OPT2MASK;

	// Loop through the Issuers 
	for ( pIssPtr = &ISTAB[0]; pIssPtr < &ISTAB[ISMAX]; pIssPtr++ )
	{
		// See if Table Populated 
		if ( pIssPtr->ISID )
		{
		   	// Yes; get Issuer Table pointer
		    TRINP.TRISPTR = pIssPtr;
																						
			 // Print "Configuration REPORT"	
			PrintDriver ( RP_ISSUER_CONFIG, &TRINP );			 
        }
		else
		{
		 return;
		}

	}

}


//-----------------------------------------------------------------------------
//  PrintAcqAvail    Print Acquirer Available
//
//  Parameters: 		None
//
//  Global Inputs:
//
//  Returns:            Nothing
// 
//  Notes:				Print Acquirer names and ID - available in the terminal
//
//-----------------------------------------------------------------------------
static void PrintAcqAvail ( void )
{
    // Initialize printer PCL data and flags 
     PrintInit ();

	     // Yes; set Option 1 for Issuer Available 
	   	 PROPT |= OPT1MASK;


	// Loop through the Acquirers 
	for ( pAcqPtr = &AQTAB[0]; pAcqPtr < &AQTAB[AQMAX]; pAcqPtr++ )
	{

		// See if Table Populated 
		if ( pAcqPtr->AQID )
		{
		   	 // Yes; get Issuer Table pointer
			TRINP.TRAQPTR = pAcqPtr;
																						
			 // Print "Configuration REPORT"	
			PrintDriver ( RP_ACQUIRER_CONFIG, &TRINP );
        }
		else
		{
		 return;
		}

	}

}


//-----------------------------------------------------------------------------
//  PrintAcqTable		Print Acquirer Table
//
//  Parameters: 		None
//
//  Global Inputs:
//
//  Returns:            Nothing
// 
//  Notes:			   Print Acquirer Table setup
//
//-----------------------------------------------------------------------------
static void PrintAcqTable ( void )
{
	// Initialize printer PCL data and flags 
     PrintInit ();

   	     // Yes; set Option 2 for Issuer table 
    	 PROPT |= OPT2MASK;

	// Loop through the Issuers 
	for ( pAcqPtr = &AQTAB[0]; pAcqPtr < &AQTAB[AQMAX]; pAcqPtr++ )
	{
		// See if Table Populated 
		if ( pAcqPtr->AQID )
		{
		   	// Yes; get Issuer Table pointer
		    TRINP.TRAQPTR = pAcqPtr;
																						
			 // Print "Configuration REPORT"	
			PrintDriver ( RP_ACQUIRER_CONFIG, &TRINP );
        }
		else
		{
		 return;
		}

	}

}


//-----------------------------------------------------------------------------
//  PrintCardAvail    Print Card Available
//
//  Parameters: 		None
//
//  Global Inputs:
//
//  Returns:            Nothing
// 
//  Notes:				Print Card names and ID - available in the terminal
//
//-----------------------------------------------------------------------------
static void PrintCardAvail ( void )
{
    // Initialize printer PCL data and flags 
     PrintInit ();

	     // Yes; set Option 1 for Issuer Available 
	   	 PROPT |= OPT1MASK;


	// Loop through the Issuers 
	for ( pCRPtr = &CRTAB[0]; pCRPtr < &CRTAB[CRMAX]; pCRPtr++ )
	{

		// See if Table Populated 
		if (pCRPtr->CRNUM)
		{
		   	 // Yes; get Issuer Table pointer
		     TRINP.TRCRPTR = pCRPtr;
																						
			 // Print "Configuration REPORT"	
			PrintDriver ( RP_CARD_CONFIG, &TRINP );
        	//PrintDriver(RP_CARD_CONFIG, &TRINP);
        }
		else
		{
		 return;
		}

	}

}


//-----------------------------------------------------------------------------
//  PrintCardTable		Print Card Table 
//
//  Parameters: 		None
//
//  Global Inputs:
//
//  Returns:            Nothing
// 
//  Notes:			   Print Issuer Table setup
//
//-----------------------------------------------------------------------------
static void PrintCardTable ( void )
{
	// Initialize printer PCL data and flags 
     PrintInit ();

   	     // Yes; set Option 2 for Issuer table 
    	 PROPT |= OPT2MASK;

	// Loop through the Issuers 
	for ( pCRPtr = &CRTAB[0]; pCRPtr < &CRTAB[CRMAX]; pCRPtr++ )
	{
		// See if Table Populated 
		if (pCRPtr->CRNUM)
		{
		   	 // Yes; get Issuer Table pointer
		     TRINP.TRCRPTR = pCRPtr;
																						
			 // Print "Configuration REPORT"	
        	 PrintDriver(RP_CARD_CONFIG, &TRINP);
        }
		else
		{
		 return;
		}

	}
}


//-----------------------------------------------------------------------------
//  IssuerTableRpt		IssuerTableRpt
//
//  Parameters: 		None
//
//  Global Inputs:
//						
//  Returns:			Nothing
//		  
//  Notes:
//
//-----------------------------------------------------------------------------
static void IssuerTableRpt(void)
{
	// See if Printer is available 
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller 
		return;
	}

	 // Print a list of names and Id of the issuer stored in the terminal
     PrintIssAvail( );

     // Print a list of each issuer table available in the terminal 
     PrintIssTable( );


	// Flush the print buffer with a form-feed
	if ( PrinterOK == PrinterErrorFlags )
		PrintFormFeed ();

	// Close the Printer 
	SDK_PrinterClose();

	// If printer error was reported then display error here. 
	if (PrinterOK != PrinterErrorFlags)
		DisplayPrintError ();

}


//-----------------------------------------------------------------------------
//  AcqTableRpt			Acquirer Table Report
//
//  Parameters: 		None
//
//  Global Inputs:
//						
//  Returns:			Nothing
//		  
//  Notes:
//
//-----------------------------------------------------------------------------
static void AcqTableRpt(void) 
{
	// See if Printer is available 
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller 
		return;
	}


	 // Print a list of names and Id of the issuer stored in the terminal
     PrintAcqAvail( );

     // Print a list of each issuer table available in the terminal 
     PrintAcqTable( );


	// Flush the print buffer with a form-feed
	if ( PrinterOK == PrinterErrorFlags )
		PrintFormFeed ();

	// Close the Printer 
	SDK_PrinterClose();

	// If printer error was reported then display error here. 
	if (PrinterOK != PrinterErrorFlags)
		DisplayPrintError ();


}

//-----------------------------------------------------------------------------
//  CardTableRpt		Card Table Report
//
//  Parameters: 		None
//
//  Global Inputs:
//						
//  Returns:			Nothing
//		  
//  Notes:
//
//-----------------------------------------------------------------------------
static void CardTableRpt(void) 
{
	// See if Printer is available 
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller 
		return;
	}


	 // Print a list of names and Id of the issuer stored in the terminal
     PrintCardAvail( );

     // Print a list of each issuer table available in the terminal 
    PrintCardTable( );


	// Flush the print buffer with a form-feed
	if ( PrinterOK == PrinterErrorFlags )
		PrintFormFeed ();

	// Close the Printer 
	SDK_PrinterClose();

	// If printer error was reported then display error here. 
	if (PrinterOK != PrinterErrorFlags)
		DisplayPrintError ();
}


//-----------------------------------------------------------------------------
//  AllTableRpt		Print All Table Report
//
//  Parameters: 		None
//
//  Global Inputs:
//						
//  Returns:			Nothing
//		  
//  Notes:
//
//-----------------------------------------------------------------------------
static void AllTableRpt(void) 
{
	// See if Printer is available 
	if ( !ChkPrinter ( ErrNotEnabled | ErrNotReady ) )
	{
		// Return to caller 
		return;
	}


	// Initialize printer PCL data and flags 
     PrintInit ();

	 // Print a list of names and Id of the issuer stored in the terminal
     PrintIssAvail( );

     // Print a list of each issuer table available in the terminal 
     PrintIssTable( );


	// Flush the print buffer with a form-feed
	if ( PrinterOK == PrinterErrorFlags )
		PrintFormFeed ();


	// If printer error was reported then display error here. 
	if (PrinterOK != PrinterErrorFlags)
		DisplayPrintError ();



	 // Print a list of names and Id of the issuer stored in the terminal
     PrintAcqAvail( );

     // Print a list of each issuer table available in the terminal 
     PrintAcqTable( );


	// Flush the print buffer with a form-feed
	if ( PrinterOK == PrinterErrorFlags )
		PrintFormFeed ();


	// If printer error was reported then display error here. 
	if (PrinterOK != PrinterErrorFlags)
		DisplayPrintError ();



	 // Print a list of names and Id of the issuer stored in the terminal
     PrintCardAvail( );

     // Print a list of each issuer table available in the terminal 
     PrintCardTable( );


	// Flush the print buffer with a form-feed
	if ( PrinterOK == PrinterErrorFlags )
		PrintFormFeed ();

	// Close the Printer 
	SDK_PrinterClose();

	// If printer error was reported then display error here. 
	if (PrinterOK != PrinterErrorFlags)
		DisplayPrintError ();
}

//-----------------------------------------------------------------------------
//  UpdateStatistic		Processing Statistic for Performance Report
//
//  Parameters: 		None
//
//  Global Inputs:
//						
//  Returns:			Nothing
//		  
//  Notes:                 
//
//-----------------------------------------------------------------------------

extern void UpdateStatistic( void )
{

	// check to know if the transaction is a finantial transaction
	if ( FinantialTransCount( ) )
	{
  		// Check if Card Swiped
		if ( 0x20 == ( TRINP.TRPOSE[1] & 0xF0 ) )
        {
			Data_kind = SWIPECARD;		// Yes; is Swipe Card
			IncrementCnt( );
		}
		else
        {
			Data_kind = MANUAL;		// No; This is manually entry
            IncrementCnt( );
		}


        // check if Transaction was approved
		if ( !( TRINP.TRRSPOPT & R_APPROVED ) )
		{
       		if ( HostOSRetval == orvOK )
			{	 
				Data_kind = DENIALS; 
				IncrementCnt( );
            }
		}
		else
        {
			Data_kind = APPROVALS; 
			IncrementCnt( );
		}
	}
}

