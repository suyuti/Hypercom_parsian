
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2004
//=============================================================================
//
// Module overview:             SerialTrace.c
//      serial trace interface
//
//=============================================================================

#include "eftos.h"
#include "string.h"

#include "basictyp.h"
#include "sdk.h"		   
#include "sdkos.h"
#include "sdkmem.h"
#include "sdktime.h"

#include "struct.h"
#include "rs232tab.h"
#include "comdata.h"
#include "comm.h"
#include "SerialTrace.h"
#include "hw.h"
#include "sertrce.h"
#include "util.h"

#define Max_Trace_Len 256*1024


// Build trace for:
// RS_0 - COM1 (RS422)
// RS_1 - comm module
// RS_2 - COM2 (RS232)
#define PortNumber TRACE_LINE

const char BeginRS0[]="***** Begin Com1 trace *****\n\r";
const char BeginRS1[]="***** Begin Line trace *****\n\r";
const char BeginRS2[]="***** Begin Com2 trace *****\n\r";

const char EndRS0[]="***** End Com1 trace *****\n\r";
const char EndRS1[]="***** End Line trace *****\n\r";
const char EndRS2[]="***** End Com2 trace *****\n\r";

//-----------------------------------------------------------------------------
// Public from other modules
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public from this modules
//-----------------------------------------------------------------------------
void CurrentTracePrint(unsigned int portNum);

//char * pSerialTrace;

//-----------------------------------------------------------------------------
//  Trace_Start    Start serial driver trace 
//
//  Parameters:         None
//  Global Inputs:
//  Returns:            
//  Notes:
//-----------------------------------------------------------------------------
extern void Trace_Start( void )
{
	Trace_Stop();
	RSTAB.TraceMask |= PortNumber;
	RSLRC = CalcLrc( (UBYTE *)&RSTAB, RSSZE );;
	Trace_Start_Mask();
	
}
extern void Trace_Start_Mask( void )
{
	if (RSTAB.TraceMask & TRACE_COM1)   
	{
          SerialTraceStop(RS_0);
          SerialTraceStart(RS_0, Max_Trace_Len);
    }

    if (RSTAB.TraceMask & TRACE_COM2)
    {
          SerialTraceStop(RS_2);
          SerialTraceStart(RS_2, Max_Trace_Len);
    }

    if (RSTAB.TraceMask & TRACE_LINE)          
    {
          SerialTraceStop(RS_1);
          SerialTraceStart(RS_1, Max_Trace_Len);
    }
}


//-----------------------------------------------------------------------------
//  Trace_Stop    Stop serial driver trace 
//
//  Parameters:         None
//  Global Inputs:
//  Returns:            
//  Notes:
//-----------------------------------------------------------------------------
extern void Trace_Stop( void )
{
	SerialTraceStop(RS_0);
	RSTAB.TraceMask &= ~TRACE_COM1;

	SerialTraceStop(RS_2);
	RSTAB.TraceMask &= ~TRACE_COM2;

	SerialTraceStop(RS_1);
	RSTAB.TraceMask &= ~TRACE_LINE;

	RSLRC = CalcLrc( (UBYTE *)&RSTAB, RSSZE );;
}


//-----------------------------------------------------------------------------
//  Trace_Print    Print serial driver trace 
//
//  Parameters:         None
//  Global Inputs:
//  Returns:            
//  Notes:
//-----------------------------------------------------------------------------
extern void Trace_Print( void )
{
	PRTSTATUS printStat;

    // Open the printer
    SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ) ;

    // Check for paper
    if ( orvOK == SDK_PrinterStatus( &printStat ))
    {
       // Get printer status 
       switch ( printStat )
       {
          // OK - continue
          case prt_PAPER_OK:
              break;

          // Paper out - exit
          case prt_PAPER_OUT:
              SDK_PrinterClose();
              ShowErrMsg( PaperOut );
              return;

          // Printer overheat - exit
          case prt_OVERHEAT:
              SDK_PrinterClose();
              ShowErrMsg( PrinterOverheat );
              return;

          // Other printer errors - exit
          default:
              SDK_PrinterClose();
              ShowErrMsg( PrintError );
              return;
       }
    }
    else
    {
        // Unable to get printer status - exit
        SDK_PrinterClose();
		ShowErrMsg( PrintError );
        return;
    }

	if (RSTAB.TraceMask & TRACE_COM1)   
        CurrentTracePrint(RS_0);
	if (RSTAB.TraceMask & TRACE_COM2)   
        CurrentTracePrint(RS_2);
	if (RSTAB.TraceMask & TRACE_LINE)   
        CurrentTracePrint(RS_1);

    // Close printer
    SDK_PrinterClose();

}


//-----------------------------------------------------------------------------
//  Trace_Send_COM1    Send serial driver trace via COM1
//
//  Parameters:         None
//  Global Inputs:
//  Returns:            
//  Notes:
//-----------------------------------------------------------------------------
extern void Trace_Send_COM1( void )
{
     // Show "Transmitting, Please wait"
    ShowInfoMsg(Transmitting, PleaseWait);

	if (RSTAB.TraceMask & TRACE_COM1)   
        SendTrace(RS_0, RS_0);
	if (RSTAB.TraceMask & TRACE_COM2)   
        SendTrace(RS_0, RS_2);
	if (RSTAB.TraceMask & TRACE_LINE)   
        SendTrace(RS_0, RS_1);

}


//-----------------------------------------------------------------------------
//  Trace_Send_COM2    Send serial driver trace via COM2
//
//  Parameters:         None
//  Global Inputs:
//  Returns:            
//  Notes:
//-----------------------------------------------------------------------------
extern void Trace_Send_COM2( void )
{
     // Show "Transmitting, Please wait"
    ShowInfoMsg(Transmitting, PleaseWait);

	if (RSTAB.TraceMask & TRACE_COM1)   
        SendTrace(RS_2, RS_0);
	if (RSTAB.TraceMask & TRACE_COM2)   
        SendTrace(RS_2, RS_2);
	if (RSTAB.TraceMask & TRACE_LINE)   
        SendTrace(RS_2, RS_1);
}

//-----------------------------------------------------------------------------
//  Trace_Check         Check if tracing is ON 
//
//  Parameters:         None
//  Global Inputs:
//  Returns:            True - tracing is ON
//                      False - tracing is OFF
//  Notes:
//-----------------------------------------------------------------------------
extern Bool Trace_Check( void )
{
	if (RSTAB.TraceMask & TRACE_COM1)   
        return True;
	if (RSTAB.TraceMask & TRACE_COM2)   
        return True;
	if (RSTAB.TraceMask & TRACE_LINE)   
        return True;
	if (RSTAB.TraceMask & TRACE_STATION_RS0)   
        return True;
	if (RSTAB.TraceMask & TRACE_STATION_RS1)   
        return True;
	if (RSTAB.TraceMask & TRACE_STATION_RS2)   
        return True;

    return False;

}

void CurrentTracePrint(unsigned int portNum)
{
	int BufLen;
	char * pBuf;
	char * pTmp;		
	char * pEnd;
	int    LineStr = 0;
    static ENTRY_STR YNEntry;

    // Get trace length
    BufLen = SerialTraceGetBufLen(portNum);

    // Check trace length
  	if ( BufLen	> 0 )
	{
		// Print Title
		if (RS_0 == portNum) // COM1
			SDK_PrintString((char*)BeginRS0);
		if (RS_1 == portNum) // LINE
			SDK_PrintString((char*)BeginRS1);
		if (RS_2 == portNum) // COM2
			SDK_PrintString((char*)BeginRS2);
		SDK_PrintString(" \n");


        // Get trace pointer
        pBuf = SerialTraceGetBuf(portNum);
		pTmp = pBuf;
		pEnd = pBuf + BufLen;

        // Notify user -- "PRINTING, WAIT..."
        ShowInfoMsg( PrintingWait, PleaseWait );

        // Loop trace buffer
		for(; pTmp < pEnd; pTmp++)
		{
            // Check if <CR>
			if (*pTmp == 0x0D)
			{
                // Increment line counter
                LineStr++;

                // Print current line
				SDK_PrintBuffer(pBuf, pTmp - pBuf + 1);				
                SDK_PrintString(" \n");

                // Skip <CR>,<LF>
				pTmp++;
				pBuf = pTmp + 1;
			}

		    // Prompt for continue every 50 lines
			if (LineStr >= 50)
			{
                // Clear line counter
				LineStr = 0;

                // Prompt "Printing/Continue" 
                YNEntry.TitleMid = PrintingWait;
                YNEntry.Line1Mid = N_Continue;
                YNEntry.Line2Mid = N_NullStr;

                // "Continue Yes/No"
                if ( ENTER_KY != YesNoEntry( &YNEntry ) )	       
                    break;

                // Notify user -- "PRINTING, WAIT..."
                ShowInfoMsg( PrintingWait, PleaseWait );
			}

		}

		// Print End
		SDK_PrintString(" \n");
		if (RS_0 == portNum) // COM1
			SDK_PrintString((char*)EndRS0);
		if (RS_1 == portNum) // LINE
			SDK_PrintString((char*)EndRS1);
		if (RS_2 == portNum) // COM2
			SDK_PrintString((char*)EndRS2);

        // Paper feed
        SDK_PrintString(" \n");
        SDK_PrintString(" \n");
        SDK_PrintString(" \n");

	}

}

//-----------------------------------------------------------------------------
//  Trace_ChkOverflow   Check if trace buffer overflow 
//
//  Parameters:         None
//  Global Inputs:
//  Returns:            True - trace buffer overflow
//                      False - not overflow
//  Notes:
//-----------------------------------------------------------------------------
extern Bool Trace_ChkOverflow( void )
{
	if (RSTAB.TraceMask & TRACE_COM1)  
	{
		if ( SerialTraceGetBufLen(RS_0)	> ( Max_Trace_Len - 257 ) )
			return True;
	}
	if (RSTAB.TraceMask & TRACE_COM2)   
	{
		if ( SerialTraceGetBufLen(RS_2)	> ( Max_Trace_Len - 257 ) )
			return True;
	}
	if (RSTAB.TraceMask & TRACE_LINE)   
	{
		if ( SerialTraceGetBufLen(RS_1)	> ( Max_Trace_Len - 257 ) )
			return True;
	}

    return False;
}
