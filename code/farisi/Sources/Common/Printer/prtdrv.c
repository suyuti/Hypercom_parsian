//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             prtdrv.c
//      Print driver routines
//
//=============================================================================
#include "string.h"

#include "basictyp.h"
#include "sdk.h"		   
#include "sdkos.h"
#include "sdkmem.h"
#include "sdktime.h"
#include "fixdata.h"
#include "utllib.h"
#include "tlib.h"

#include "struct.h"
#include "appdata.h"
#include "comdata.h"
#include "prttable.h"
#include "prtdef.h"
#include "prtdef2.h"
#include "tables.h"
#include "findtran.h"
#include "datetime.h"
#include "pinutils.h"
#include "read_io.h"
#include "pcltextinit.h"
#include "cvtamt.h"
#include "negsign.h"
#include "emv_l2.h"
#include "emvdata.h"

#include "prtdrv.h"
#include "DbgTrace.h"

#ifdef MAKE_FARSI
#include "Farsi_print.h"
#endif
//=============================================================================
// External variables / declarations
//=============================================================================


//=============================================================================
// Private defines and typedefs
//=============================================================================

#define getoffset(OFFSET) ((int)(OFFSET))

//! MAXIMUM EDIT FIELD SIZE (NEEDS TO BE BIG ENOUGH FOR WIDEST PRINTER)
#define BUFSZE  128

//! Size of Name in Track 1 Data
#define S_TRK1NAME 26

//! RE_Flag bit: Outer nested IF codition is true
#define RF_OTRUE    0x01
//! RE_Flag bit: Inner IF condition is true
#define RF_ITRUE    0x02
//! RE_Flag bit: Else token is allowed
#define RF_ELSE     0x04
//! RE_Flag bit: True case has already occurred
#define RF_ATRUE    0x08

#define Encode(out, input) \
	out[0] = (UBYTE)( (input >> 8) & 0xff); \
	out[1] = (UBYTE)( input & 0xff );

struct CommandTableStruct
{
    //! Command code
	UBYTE Code;
    //! Number of operands
	UBYTE Operands;
    //! Routine for this command
	Bool( *Func ) ( void );
    //! Flags for this command
	UBYTE Flags;
};


//! Buffer for acquirer and issuer data the size is the size of the longest
//! field in AQTAB and ISTAB
UBYTE LOCBUF[S_AQVISATID];

//! Flags for CommandTableStruct

//! Process routine even when False
#define CMD_TF      0x01
//! Multiple Char, read the value
#define CMD_VAL     0x02
//! Field - adjust format
#define CMD_FOR     0x02
//! Field - adjust the start
#define CMD_STR     0x04
//! Field - adjust length
#define CMD_LEN     0x08
//! Reset center & right justify flags
#define CMD_RESJ    0x10
//! Dump character buffer
#define CMD_DMPB    0x20
//! Used to process when True but print off
#define CMD_POFF    0x40


//=============================================================================
// Private function declarations
//=============================================================================
static Bool PrintDriverCommon( enum REPORTTYPE Report );
static Bool FindReport( enum REPORTTYPE Report );
static void PrintPCL( void );

static UBYTE PeekToken( void );
static Bool PrintItem( char *Item, UBYTE Length );

extern Bool PrintMLTCHR( void );
extern Bool PrintLINE( void );
extern Bool PrintFORM( void );

extern Bool PrintIt( char *SrcPtr, UWORD DataLen, Bool ForceIt );
extern Bool PrintErrDetect( void );
extern Bool PrintFLD( void );
extern Bool PrintFLD_F( void );
extern Bool PrintFLD_L( void );
extern Bool PrintFLD_S( void );
extern Bool PrintFLD_FL( void );
extern Bool PrintNORMAL( void );
extern Bool PrintDOUBLE( void );
extern Bool PrintFONT( void );
extern Bool PrintWIDTH( void );
extern Bool PrintLEFT( void );
extern Bool PrintCNTR( void );
extern Bool PrintRGHT( void );
extern Bool PrintPOSLFT( void );
extern Bool PrintPOSRGHT( void );
extern Bool PrintPOSCNTR( void );
extern Bool PrintREPORT( void );
extern Bool PrintIF( void );
extern Bool PrintENDIF( void );
extern Bool PrintELSE( void );
extern Bool PrintELSEIF( void );
extern Bool PrintAND( void );
extern Bool PrintOR( void );
extern Bool PrintSignature( void );
extern Bool PrintGraphics( void );
extern Bool PrintRed( void );
extern Bool PrintText( void );
extern Bool ComplexTest( void );
extern Bool SimpleTest( void );
extern Bool PrintPPSTOP( void );
extern Bool TestTRN_SALE( void );
extern Bool TestTRN_REFD( void );
extern Bool TestTRN_OFFL( void );
extern Bool TestTRN_VOID( void );
extern Bool TestTRN_OPENT( void );
extern Bool TestTRN_CRCD( void );
extern Bool TestTRN_DBCD( void );
extern Bool TestTRN_BALI( void );
extern Bool TestTRN_NOSIG( void );


extern Bool TestCND_REJ( void );
extern Bool TestCND_TIP( void );
extern Bool TestCND_REPR( void );
extern Bool TestCND_PREP( void );
extern Bool TestCND_SREQ( void );
extern Bool TestCND_MAN( void );
extern Bool TestTRN_AVSVOID( void );

extern void SelectPrinterFont( UBYTE fontID );
extern void SelectPrinterFont_Width( UBYTE lineWidth );

extern Bool PrintFirmware( void );

static UBYTE GetToken( void );
static Bool PrintLineBuff( Bool ForceIt );
static Bool PrintSubLine( void );
static Bool BLD_FLD( UBYTE Field );
static Bool FINDFLD( UBYTE Field );
static Bool ADR_LEN( void );
static Bool EDITFLD( void );
static Bool FMT_FLD( UBYTE Format );
static Bool FMT_CHR( UBYTE Format );
static Bool FMT_AMT( UBYTE Format );
static Bool FMT_NUM( UBYTE Format );
static Bool FMT_DATE( UBYTE Format );
static Bool FMT_TIME( UBYTE Format );
static char *FMT_STRLEN( UBYTE * pLength );
static void GetName( void );
static void SetTip( void );
static Bool TST_FLD( void );
static Bool SetDefaultTab( void );
static Bool SetTableType( void );
static Bool SetDefaultTxtTab( void );
static Bool SetTxtTableType( void );
static void PrintRemap( void * pDest, const void * pSource, UWORD  Bytes );


//=============================================================================
// Public data definitions
//=============================================================================
Bool( *PrintRtn ) ( char *SrcPtr, UWORD DataLen, Bool ForceIt );
//! Print Line Number
UBYTE PRLINE;

//! Custom Function vector Pointers
struct FLDDEF *(*PRVECT[8])( void );

//! Transaction Buffer Location
struct trandata_rec *TRLOC;

//! Current pointer to PCL report
UBYTE *CMDLOC;
//! Pointer to end of PCL report
UBYTE *CMDEND;

//! Length of report
UWORD CMDMORE;
//! Start of report
UBYTE *CMDSTART;
//! page tbat contain report token
PAGE_NUMBERS CMDPAGE;

//! used to set PRFLAG for PCL operations
UBYTE PresetPRFLAG;

//! Variables for reading a PCL script from a file
FILE *ReportFileHandle;
UBYTE ReportBuffer[128];

char TextGetBuf[50];
//! Default Print Transaction Data
struct trandata_rec TRPRT;


//=============================================================================
// Private data definitions
//=============================================================================
//! Pointer to Field Definition
struct FLDDEF *pFieldDef;
//! Address of the Field
static UBYTE *pFieldData;
//! Length of the Field
static UBYTE FieldLen;

//! Edited Field Related Items

//! Starting Address of the Edited Field
static char *FLD_ADR;
//! The Length of the Edited Field
static UBYTE FLD_LEN;

//! Current position within print sub-line
static UBYTE PrintPos;
//! Length of the sub-line to print
static UBYTE PRBUF3LN;

//! The quontity of ESC characters per line
static UWORD ESC_number;

//! Print Line Text Justification
static UBYTE PRJUST;

static char BUF1[BUFSZE];
static char BUF2[BUFSZE];
static char BUF3[BUFSZE];

//! Current location within print line
static char LineBuff[BUFSZE + 1];
static UBYTE LineLoc;
//! Current length of print line
static UBYTE LineSize;

static UBYTE HOLDAMT[6];
static UBYTE HOLDSGN;

//! PCL interpreter state variables

//! Error in the PCL report
static UBYTE PCL_Error;
//! Re-entrancy level
static BYTE Level;
//! Copy of command flags from CommandTable
static UBYTE CmdFlags;
//! Re-entrant interpreter flags
static UBYTE RE_Flag;

static const UBYTE DR_String[] = {' ','D','R'};
static const UBYTE CR_String[] = {' ','C','R'};

static const struct CommandTableStruct CommandTable[] = {
	{LINE, 0, PrintLINE, CMD_DMPB | CMD_RESJ},
	{FORM, 0, PrintFORM, CMD_DMPB | CMD_RESJ},
	{NORMAL, 0, PrintNORMAL, CMD_DMPB},
	{DOUBLE, 0, PrintDOUBLE, CMD_DMPB},
	{FONT, 1, PrintFONT, 0},
	{POSLFT, 1, PrintPOSLFT, CMD_DMPB | CMD_RESJ},
	{POSRGHT, 1, PrintPOSRGHT, CMD_DMPB | CMD_RESJ},
	{POSCNTR, 1, PrintPOSCNTR, CMD_DMPB | CMD_RESJ},
	{LFT, 0, PrintLEFT, CMD_DMPB | CMD_RESJ},
	{CNTR, 0, PrintCNTR, CMD_DMPB | CMD_RESJ},
	{RGHT, 0, PrintRGHT, CMD_DMPB | CMD_RESJ},
	{MLTSPC, 1, PrintMLTCHR, 0},
	{MLTCHR, 2, PrintMLTCHR, CMD_VAL},
	{FLD, 1, PrintFLD, 0},
	{FLD_S, 2, PrintFLD, CMD_STR},
	{FLD_L, 2, PrintFLD, CMD_LEN},
	{FLD_SL, 3, PrintFLD, CMD_STR | CMD_LEN},
	{FLD_F, 2, PrintFLD, CMD_FOR},
	{FLD_FS, 3, PrintFLD, CMD_FOR | CMD_STR},
	{FLD_FL, 3, PrintFLD, CMD_FOR | CMD_LEN},
	{FLD_FSL, 4, PrintFLD, CMD_FOR | CMD_STR | CMD_LEN},
	{REPORT, 1, PrintREPORT, CMD_POFF},
	{WIDTH, 1, PrintWIDTH, CMD_DMPB},
	{NO_CMD, 0, 0, 0},
	{NO_CMD, 0, 0, 0},
	{ELSEIF, 0, PrintELSEIF, CMD_TF},
	{IF, 0, PrintIF, CMD_TF},
	{OR, 0, PrintOR, CMD_TF},
	{AND, 0, PrintAND, CMD_TF},
	{ENDIF, 0, PrintENDIF, CMD_TF},
	{ELSE, 0, PrintELSE, CMD_TF},
	{PPSTOP, 0, PrintPPSTOP, CMD_POFF | CMD_DMPB | CMD_RESJ},
	{NO_CMD, 0, 0, 0},
	{SIG, 0, PrintSignature, 0},
	{GRPH, 0, PrintGraphics, 0},
	{NO_CMD, 0, 0, 0},
	{NO_CMD, 0, 0, 0},
	{RED, 0, PrintRed, 0},
	{TXT, 1, PrintText, 0},
	{EXTFLD, 1, SetTableType, CMD_TF},
	{EXTTXT, 1, SetTxtTableType, 0},
	{FIRMWARE, 0, PrintFirmware, 0 },
	{FLD_END, 0, 0, 0},
	{NO_CMD, 0, 0, 0}
};

//=============================================================================
// Public function definitions
//=============================================================================



//-----------------------------------------------------------------------------
//! \brief
//!     Print Initialization
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void PrintInit( void )
{
	UBYTE i;

	// Initialize the Printer Buffer
	memset( PRTBUF, 0, sizeof( PRTBUF ) );
	Pbufptr = PRTBUF;

	PROPT = 0x00;				// Initialize Print Options
	PRFLAG = 0x00;				// Reset Print Flags
	PRLINE = 0;					// Reset Print Line Number
	PRJUST = LJ_LEFT;			// Reset Print Line Text Justification
	PrintPos = 0;				// Reset the current position within print sub-line
	PRBUF3LN = 0;				// Reset the current print sub-line length
	LineLoc = 0;				// Reset the current location within print line
	LineSize = 0;				// Reset the current print line size
	ESC_number = 0;				// Reset number of ESC commands within print line

	for ( i = 0; i < 8; ++i )
		PRVECT[i] = NULL;		// Reset Pointer to Custom Routine array

	memset( BUF3, ' ', BUFSZE );		// Clear the printer sub-line buffer
	memset( LineBuff, ' ', BUFSZE + 1 );	// Clear the printer line buffer

	// Printer specific initialization
	MaxCols = PrinterTable[TCONF.TCPRINTER].Font0width;

#ifdef MAKE_FARSI
    // Redefine MaxCols if Farsi laguage selected
    if( LOCTAB.LANGID == LNG_FA )
    {
        MaxCols = Farsi_max_char_per_line;
    }
#endif

	RollPrinter = ( ( PrinterTable[TCONF.TCPRINTER].Flags & PR1_ROLL ) != 0 );
	GraphicsPrinter =
		( ( PrinterTable[TCONF.TCPRINTER].Flags & PR1_GRPH ) != 0 );
	ForceLF = ( ( PrinterTable[TCONF.TCPRINTER].Flags & PR1_LF ) == 0 );
	SelectPrinterFont_Width( PrinterTable[TCONF.TCPRINTER].DefaultWidth );

	// Initialize error flags
	PrinterErrorFlags = PrinterOK;	// Initialize printer error flag.
	PCL_Error = 0;				// Set PCL error code to no error

	PRFLAG = PresetPRFLAG;		// initialize PRFLAG to any preset value
	PresetPRFLAG = 0;

	if ( !SetDefaultTab(  ) )	// Set FX_Default print Table by default 
	{
		// Error messages while testing.
		ShowErrMsg( PclTabError );

		return;
	}

	if ( !SetDefaultTxtTab(  ) )	/* Set FX_Default print Table by default */
	{
		// Error messages while testing.
		ShowErrMsg( PclTabError );

		return;
	}

	if ( SDK_PrinterOpen( TCONF.TCPRINTER | ((TCONF.TCOPT2 & TC2_GRAPHMODE) ? 0x80 : 0x00) ) != orvOK )
	{
		SDK_PrinterClose(  );
	}

	return;						// Return okay
}


//-----------------------------------------------------------------------------
//! \brief
//!     Print Command Driver
//!
//! \param
//!		[in]Report			Type of report
//!		[in]pTranData		Pointer to transaction data structure. If NULL, the
//!                         default transaction data structure TRPRT will be used.
//!
//! \return
//!     False if there was an error else True
//-----------------------------------------------------------------------------
extern Bool PrintDriver( enum REPORTTYPE Report, struct trandata_rec *pTranData )
{
	if( !( PRTSTAT & 0x01 ) )
	{
		// Printer is logically disabled
		return( False );
	}

	// Send output to the printer
	PrintRtn = &PrintIt;

	// Set global pointer to the Transaction Buffer Location
	TRLOC = pTranData;

	// We're NOT doing signature capture, so graphics are O.K.
	GraphicsEnabled = True;

	return PrintDriverCommon( Report );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Print Command Driver commonly called by either PrintDriver or PrintCapture
//!
//! \param
//!		[in]Report			Type of report
//!
//! \return
//!     False if there was an error else True
//-----------------------------------------------------------------------------
static Bool PrintDriverCommon( enum REPORTTYPE Report )
{
	UBYTE length;

	// Check if TRLOC is NULL
	if ( NULL == TRLOC )
	{
		// Yes; clear the TRPRT structure
		memset( ( UBYTE * ) & TRPRT, 0, TRSZE );

		// Set up the default Print Transaction Data Structure

		// Set default Transaction Type
		TRPRT.TRKEY = SALE;

		// Set default Acquirer Pointer
    	TRPRT.TRAQPTR = &AQTAB[0];

		// Set default Acquirer ID
    	TRPRT.TRAQID = TRPRT.TRAQPTR->AQID;

		// Set default Issuer Pointer
     	TRPRT.TRISPTR = &ISTAB[0];

		// Set default Issuer ID
     	TRPRT.TRISID = TRPRT.TRISPTR->ISID;

		// Set TRLOC to point to the default Print Transaction Data Structure
		TRLOC = &TRPRT;
	}

	// Set up for PCL interpreter
	if ( FindReport( Report ) )	// Set CMDLOC and CMDEND
	{
		Level = 0;				// Set nesting level to zero
		RE_Flag = RF_OTRUE | RF_ITRUE;	// True condition

		// Call the interpreter
		PrintPCL(  );

		// If a report file was left open, close it
		if ( ReportFileHandle != 0 )
			fclose( ReportFileHandle );

		// Make sure nothing crazy happened
		if ( ( Level != 0 ) && ( 0 == PCL_Error ) )
		{
			PCL_Error = 1;
		}


		PRFLAG |= PF_PREV;		// Set the Flag indicating previous calls
	}

	// If there was a printer error, don't try to print anything else
	if ( PrinterErrorFlags != 0 )
		return False;

	if ( PCL_Error != 0 )		// Was there an error?
	{
		if ( PrintLINE(  ) )	// Yes; print the current line
		{
			// Okay; put an error message into the print line
			GetMsg( Msg_Error, CSTMSG );
			length = StrLn( CSTMSG, sizeof( CSTMSG ) );

			if ( PrintItem( CSTMSG, length ) )
			{
				// Okay; add the error number to error message
				BufAsc( &BUF3[10], &PCL_Error );
				PrintLINE(  );	// Print the error message
				PrintFORM(  );	// Perform formfeed
			}
		}
	}
	( *PrintRtn ) ( NULL, 0, 1 );	// Force flush of print buffer

	if ( PCL_Error != 0 )
		return False;
	else
		return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Finds a PCL report
//!
//! \param
//!		[in]Report			Type of report
//!
//! \return
//!     True if the report was found else False
//-----------------------------------------------------------------------------
static Bool FindReport( enum REPORTTYPE report )
{
    struct report_table_struct *rp;
	char filename[12];
	UWORD len;
	UBYTE *pAddress;			// For cross page report table reading

	// First check the file system for a downloaded report with this number
	memcpy( filename, "Report  rpt", sizeof( filename ) );
	BufAsc( &filename[6], ( UBYTE * ) & report );
	ReportFileHandle = fopen( filename, "r" );
	if ( ReportFileHandle != 0 )
	{
		// A report with this number has been downloaded
		// Read from the file into a buffer
		len =
			fread( ReportBuffer, 1, sizeof( ReportBuffer ),
				   ReportFileHandle );
		if ( 0 != len )
		{
			CMDLOC = ReportBuffer;
			CMDEND = &ReportBuffer[len];
			return True;
		}
	}

	// Get start address of table to search for a build-in report from another page
	SDK_GetAddress( "REPORT_TAB", &CMDPAGE, &pAddress );

	while ( 1 )
	{

		rp = ( struct report_table_struct * ) pAddress;

		if ( rp->num == RP_MARKER )
		{
			return False;
		}
		else if ( rp->num == report )
		{
			CMDMORE = ( rp->end - rp->start );
			len = sizeof( ReportBuffer );
			if ( CMDMORE < len )
			{
				len = CMDMORE;
				CMDMORE = 0;
			}
			else
			{
				CMDMORE -= len;
			}

			// save the start address for possible reloading
			CMDSTART = rp->start;
			SDK_ReadPage( ReportBuffer, len, CMDPAGE, CMDSTART );
			CMDLOC = ReportBuffer;
			CMDEND = &ReportBuffer[len];
			return True;
		}
		pAddress += sizeof( struct report_table_struct );
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     PCL interpreter
//!
//! \return
//!     None
//!
//! \note
//!     Error code will be put in PCL_Error
//-----------------------------------------------------------------------------
static void PrintPCL( void )
{
	UBYTE token;
	static const struct CommandTableStruct *cmd;
	static UBYTE operands;

	while ( PCL_Error == 0 )
	{
		// If all tokens have been read, just return
		token = PeekToken(  );
		if ( NO_CMD == token )
			break;
		else
			CMDLOC++;			// Advance the pointer

		// Check if token is a command.  Modified to use token
		// as index instead of doing a table search.
		if ( ( token >= LINE ) && ( token <= FLD_END ) )
		{
			// Yes; set pointer to command entry within the command table
			cmd = &CommandTable[token - LINE];
		}
		else
		{
			// No, must be normal text; set pointer to non-command entry
			// within the command table
			cmd = &CommandTable[( sizeof( CommandTable )
								  / sizeof( struct CommandTableStruct ) ) -
								1];
		}

		// Skip the command if:
		//  We don't ALWAYS process this command, AND
		//   We are not in a "True" condition,    OR
		//   We're skipping because of pre-print (unless this command is always done at preprint)
		if ( !( cmd->Flags & CMD_TF ) &&
			 ( !( RE_Flag & RF_ITRUE ) ||
			   ( ( PRFLAG & PF_OFFP ) && ( !( cmd->Flags & CMD_POFF ) ) ) ) )
		{
			// Skip command and operands, if any
			operands = cmd->Operands;
			while ( operands-- )
				GetToken(  );
		}
		else
		{
			// Execute the command

			// Store the command flags in a global
			CmdFlags = cmd->Flags;

			// Dump the character buffer before executing this command?
			if ( CmdFlags & CMD_DMPB )
				PrintSubLine(  );

			// Reset the justify flags before executing this command?
			if ( CmdFlags & CMD_RESJ )
				PRFLAG &= ~( PF_CNTR | PF_RGHT );

			// Is it a command or text?
			if ( cmd->Code != NO_CMD )
			{
				// Call the command routine, if it returns false,
				// break out of the interpreter
				if ( !( *cmd->Func ) (  ) )
					break;
			}
			else
			{
				// Must be text, print it
				PrintItem( ( char * ) &token, 1 );
			}
		}
	}
}



//-----------------------------------------------------------------------------
//! \brief
//!     Returns the next PCL token, but do not advance the pointer
//!
//! \return
//!     Token or NO_CMD if no more tokens
//-----------------------------------------------------------------------------
UBYTE PeekToken( void )
{
	UWORD len;

	if ( CMDLOC == CMDEND )
	{
		// Are we reading the PCL script from a file?
		if ( ReportFileHandle != 0 )
		{
			// Read from the file into a buffer
			len =
				fread( ReportBuffer, 1, sizeof( ReportBuffer ),
					   ReportFileHandle );
			if ( 0 != len )
			{
				CMDLOC = ReportBuffer;
				CMDEND = &ReportBuffer[len];
				return *CMDLOC;
			}

			// End of file, close the file and return NO_CMD
			fclose( ReportFileHandle );
		}

		// Anymore tokens in report?
		if ( CMDMORE )
		{
			// Yes, read again
			len = sizeof( ReportBuffer );
			if ( CMDMORE < len )
			{
				len = CMDMORE;
				CMDMORE = 0;
			}
			else
				CMDMORE -= len;
			// The previous ReportBuffer has been emptyed, read more
			CMDSTART += sizeof( ReportBuffer );
			SDK_ReadPage( ReportBuffer, len, CMDPAGE, CMDSTART );
			CMDLOC = ReportBuffer;
			CMDEND = &ReportBuffer[len];
			return *CMDLOC;
		}

		// Not reading a file, no more tokens
		return NO_CMD;
	}

	// Return the next token
	return *CMDLOC;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Buffers the specified item for printing
//!
//! \param
//!		[in]Item			Pointer to data to be buffered
//!		[in]Length			Length of the data to be buffered
//!
//! \return
//!     False on error else True
//-----------------------------------------------------------------------------
extern Bool PrintItem( char *Item, UBYTE Length )
{
	Bool Status;

	Status = False;

	// See if item will fit in the print line
	if ( ( ( PrintPos + Length ) <= ( MaxCols + ESC_number ) ) &&
		 ( ( PrintPos + Length ) <= BUFSZE ) )
	{
		if( ESC == *Item )
		{
			ESC_number += 2;
		}
		// Yes; move the item to the print line buffer
		memcpy( &BUF3[PrintPos], Item, Length );
		PrintPos += Length;		// Update the current position within print line
		// See if current print sub-line length needs to be updated
		if ( PrintPos > PRBUF3LN )
		{
			// Yes; update the current print sub-line length
			PRBUF3LN = PrintPos;
		}
		Status = True;			// Set the status to okay
	}
//  else
//      PCL_Error = 28;      // No; save the error status
	return ( Status );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints a given character a given number of times
//!
//! \return
//!     False on error else True
//-----------------------------------------------------------------------------
extern Bool PrintMLTCHR( void )
{
	UBYTE number;
	char prtChar;

	// Get the arguments
	number = GetToken(  );

	// Is the command MLTCHR?
	if ( CmdFlags & CMD_VAL )
	{
		prtChar = GetToken(  );
	}
	else
	{
		// The command must be MLTSPC
		prtChar = ' ';
	}

	// If there was a problem getting the argument(s), return error
	if ( PCL_Error != 0 )
		return False;

	// See if the number of characters will fit in the print line
	if ( ( PrintPos + number ) > ( MaxCols + ESC_number ) )
		number = ( MaxCols + ESC_number ) - PrintPos;	// Print as many as will fit

	if ( prtChar != ' ' )
		memset( ( UBYTE * ) ( BUF3 + PrintPos ), prtChar, ( UWORD ) ( number ) );

	PrintPos += number;			// Update the current position within print line

	// See if current print sub-line length needs to be updated
	if ( PrintPos > PRBUF3LN )
		PRBUF3LN = PrintPos;	// Yes; update current print sub-line len

	return True;				// Return error
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints the current line of data
//!
//! \return
//!     False on error else True
//-----------------------------------------------------------------------------
extern Bool PrintLINE( void )
{
	Bool Status = False;

	// Move print sub-line to print line
	if ( PrintSubLine(  ) )
	{
		// Increment the Print Line Number
		PRLINE++;

		// See if need to add a linefeed
		if ( ( ForceLF ) || ( LineSize < ( MaxCols + ESC_number ) ) )
		{
			// Yes; add a linefeed to the print sub-line data
			LineBuff[LineSize] = '\n';
			LineSize++;			// Increment the print line data length
		}

		// Print the current line of data
		if ( PrintLineBuff( 0 ) )
			Status = True;		// Okay; set status to okay

		LineLoc = 0;			// Reset the current location within print line
		ESC_number = 0;			// Reset number of ESC commands within print line
		LineSize = 0;			// Reset the current print line length
		PRJUST = LJ_LEFT;		// Reset Print Line Text Justification
		PRFLAG &= ~PF_DBLE;		// Reset printing to Normal characters
		memset( LineBuff, ' ', BUFSZE + 1 );	// Clear the printer line buffer
	}
	return ( Status );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints the current line of data
//!
//! \param
//!		[in]appendChar		character to ouput or NULL if none
//!
//! \return
//!     False on error else True
//-----------------------------------------------------------------------------
extern Bool FlushBuffers( char appendChar )
{
	Bool Status = False;

	// Move print sub-line to print line
	if ( PrintSubLine(  ) )
	{
		if ( appendChar != 0 )
		{
			// Append the passed character to the buffer before printing
			LineBuff[LineSize] = appendChar;
			LineSize++;
		}

		// Print the current line of data, flushing print buffers
		if ( PrintLineBuff( 1 ) )
			Status = True;		// Okay; set status to okay

		// Reset the current location within print line
		LineLoc = 0;

		// Reset the current print line length
		LineSize = 0;

		// Reset number of ESC commands within print line
		ESC_number = 0;

		// Reset Print Line Text Justification
		PRJUST = LJ_LEFT;

		// Reset printing to Normal characters
		PRFLAG &= ~PF_DBLE;

		// Reset the Print Line Number
		PRLINE = 0;

		// Clear the printer line buffer
		memset( LineBuff, ' ', BUFSZE + 1 );
	}

	// Return the status
	return ( Status );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints the current line of data with formfeed
//!
//! \return
//!     False on error else True
//-----------------------------------------------------------------------------
extern Bool PrintFORM( void )
{
	// Print the current line of data with a formfeed
    Bool status = FlushBuffers( '\f' );
    return status;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints the current line of data with formfeed
//!
//! \return
//!     False on error else True
//-----------------------------------------------------------------------------
extern void PrintFormFeed( void )
{
	// Print the current line of data with a formfeed
	FlushBuffers( '\f' );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sends data to the printer
//!
//! \param
//!		[in]SrcPtr			Pointer to the data to be printed
//!		[in]DataLen			Length of the data to be printed
//!		[in]ForceIt			Flush the output buffer to printer when set to 1
//!
//! \return
//!     False if printing failed else True
//-----------------------------------------------------------------------------
extern Bool PrintIt( char *SrcPtr, UWORD DataLen, Bool ForceIt )
{
	char retval;
	UWORD len;
	OS_RETURN_VALS printerStat;

	// Do not keep trying to print once an error occurs.
	if ( PrinterOK != PrinterErrorFlags )
		return 0;

	retval = 1;

	// If data length is zero, we flush the contents of PRTBUF.
	if ( DataLen )
	{
		PrintRemap( Pbufptr, SrcPtr, DataLen );
//		memcpy( Pbufptr, SrcPtr, DataLen );
		Pbufptr += DataLen;
	}

	// Accumulated enough data then send to the printer. This is a
	// more efficient way to handle printing.

	len = Pbufptr - PRTBUF;
	if ( ( len < 200 ) && !ForceIt )	// Have room for at least 56 more bytes?
		return retval;
	else
	{
		if ( PrintErrDetect(  ) )
			retval = 0;

		// Only continue with printing if error was not detected.
		if ( 1 == retval )
		{
#ifdef MAKE_FARSI
            if( LOCTAB.LANGID == LNG_FA )
            {
                printerStat = Farsi_Print( PRTBUF, len );
            }
            else
            {
                printerStat = SDK_PrintBuffer( PRTBUF, len );
            }
#else
			printerStat = SDK_PrintBuffer( PRTBUF, len );
#endif
            if ( printerStat != orvOK )
			{
				SDK_PrinterClose(  );
				if ( orvPAPEROUT == printerStat )
				{
					PrinterErrorFlags = PaperOutError;
				}
				else
				{
					PrinterErrorFlags = OtherPrintError;
				}


				retval = 0;
			}
			else
			{
				// Must delay to print or close will cause print to stop.
				if ( ForceIt )
				{
					while ( SDK_PrinterBusy( ) && ( 0x0E != TCONF.TCPRINTER ) )
						SDK_RelinqCPU(  );
				}

				// Initialize the Printer Buffer
				memset( PRTBUF, 0, sizeof( PRTBUF ) );
				Pbufptr = PRTBUF;
			}
		}
	}
	return retval;
}



//-----------------------------------------------------------------------------
//! \brief
//!     Checks for printer hardware errors
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintErrDetect( void )
{
	PRTSTATUS printStat;

	// Check printer for errors
	if ( orvOK == SDK_PrinterStatus( &printStat ) )
	{
		// Was there an error?
		if ( prt_PAPER_OK != printStat )
		{
			// Error, close the printer
			SDK_PrinterClose(  );

			// Report whether paper out or other error.
			// Main loop will use this.
			if ( prt_PAPER_OUT == printStat )
			{
				PrinterErrorFlags = PaperOutError;
			}
			else if ( prt_OVERHEAT == printStat )
			{
				PrinterErrorFlags = PrinterOverheatError;
			}
			else
				PrinterErrorFlags = OtherPrintError;

			// Report error
			return True;
		}
	}
	else
	{
		// Some SDK error, abort printing
		SDK_PrinterClose(  );
		PrinterErrorFlags = OtherPrintError;
		return True;
	}

	return False;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints a field
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintFLD( void )
{
	UBYTE format;
	BYTE startPos;
	UBYTE NewLength;

	// Get the field number
	vFieldNum = GetToken(  );
	if ( PCL_Error != 0 )
		return False;

	// Build the field
	if ( !BLD_FLD( vFieldNum ) )
		return False;


	// If this command was FLD_F, FLD_FS, FLD_FL, or FLD_FSL
	if ( CmdFlags & CMD_FOR )
	{
		format = GetToken(  );
		if ( PCL_Error != 0 )
			return False;

		// reformat the field
		if ( !FMT_FLD( format ) )
			return False;
	}

	// If this command was FLD_S, FLD_SL, FLD_FS, or FLD_FSL
	if ( CmdFlags & CMD_STR )
	{
		startPos = ( BYTE ) GetToken(  );
		if ( PCL_Error != 0 )
			return False;

		// Okay; see if Positive or Negative Positioning
		if ( startPos >= 0 )
		{
			// Positive; calculate new field address and length
			FLD_ADR += startPos - 1;	// Count from start
			FLD_LEN -= startPos - 1;
		}
		else
		{
			// Negative; calculate new field address and length
			FLD_ADR += FLD_LEN + startPos;	// Count backwards from end
			FLD_LEN = -startPos;
		}
	}

	// If this command was FLD_L, FLD_SL, FLD_FL, or  FLD_FSL
	if ( CmdFlags & CMD_LEN )
	{
		// Get the length to make the field
		NewLength = GetToken(  );
		if ( PCL_Error != 0 )
			return False;

		// Check if length is larger than the print line length
		if ( NewLength > ( MaxCols + ESC_number ) )
		{
			// Yes; set the length to the print line length
			NewLength = ( MaxCols + ESC_number );
		}

		// Check if length is greater than the current field length
		if ( NewLength > FLD_LEN )
		{
			// Yes; check if current field length is non-zero
			if ( FLD_LEN > 0 )
			{
				// Yes; move field data to BUF1
				memcpy( BUF1, FLD_ADR, FLD_LEN );
			}

			// Space out the rest
			memset( &BUF1[FLD_LEN], ' ', ( UWORD ) ( NewLength - FLD_LEN ) );

			// Update the field data address
			FLD_ADR = BUF1;
		}

		// Override the length
		FLD_LEN = NewLength;
	}

	return PrintItem( FLD_ADR, FLD_LEN );	// Print the field
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sets printing to Normal wide characters
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintNORMAL( void )
{
	Bool Status = False;

	if ( PrintSubLine(  ) )		// Move print sub-line to print line
	{
		PRFLAG &= ~PF_DBLE;		// Reset the Double Wide Characters flag
		Status = True;			// Set the status to okay
	}
	return ( Status );			// Return the status
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sets printing to Double wide characters
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintDOUBLE( void )
{
	Bool Status = False;

	if ( PrintSubLine(  ) )		// Move print sub-line to print line
	{
		PRFLAG |= PF_DBLE;		// Set the Double-Wide Characters flag
		Status = True;			// Set the status to okay
	}
	return ( Status );			// Return the status
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sets printer font
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintFONT( void )
{
	UBYTE fontID;

	// Get the font ID (zero or one)
	fontID = GetToken(  );
	if ( PCL_Error != 0 )
		return False;

	PrintItem( (char*)&fontID, 1 );

	return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Selects the printer font
//!
//! \param
//!		[in]fontID			Font ID (0 or 1)
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void SelectPrinterFont( UBYTE fontID )
{
#ifdef MAKE_FARSI
    if( LOCTAB.LANGID == LNG_FA )
    {
        return;
    }
#endif

    switch ( fontID )
	{
		case 0:
			// Is this a different font than we are using?
			if ( MaxCols != PrinterTable[TCONF.TCPRINTER].Font0width )
			{
				// Set the number of characters per line for the new font
				MaxCols = PrinterTable[TCONF.TCPRINTER].Font0width;

				// Send the change font command to the printer
				PrintItem( "\x1Bk0", 3 );
			}
			break;

		case 1:
			// Is the compressed font defined for this printer?
			if ( PrinterTable[TCONF.TCPRINTER].Font1width != 0 )
			{
				MaxCols = PrinterTable[TCONF.TCPRINTER].Font1width;

				// Send the change font command to the printer
				PrintItem( "\x1Bk1", 3 );
			}
			break;

		default:
			break;
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sets the printer font width
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern Bool PrintWIDTH( void )
{
	UBYTE fontWidth;

	// Get the Width requested
	fontWidth = GetToken(  );
	if ( PCL_Error != 0 )
		return False;

#ifdef MAKE_FARSI
    if( LOCTAB.LANGID == LNG_FA )
    {
        return;
    }
#endif

	SelectPrinterFont_Width( fontWidth );

	return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Selects the largest printer font such that the characters/line
//!     is >= the passed parameter
//!
//! \param
//!		[in]lineWidth		Desired width
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void SelectPrinterFont_Width( UBYTE lineWidth )
{
	// Will the normal font work?
	if ( lineWidth <= PrinterTable[TCONF.TCPRINTER].Font0width )
		SelectPrinterFont( 0 );

	// Will the condensed font work?
	if ( lineWidth <= PrinterTable[TCONF.TCPRINTER].Font1width )
		SelectPrinterFont( 1 );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints the next field(s) Left Justified on Line
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintLEFT( void )
{
	Bool Status = False;

	if ( PrintSubLine(  ) )		// Move print sub-line to print line
	{
		PRJUST = LJ_LEFT;		// Set the Justification flag
		LineLoc = 0;			// Set the print line location to the start of the line
		Status = True;			// Set the status to okay
	}
	return ( Status );			// Return the status
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints the next field(s) Center Justified on Line
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintCNTR( void )
{
	Bool Status = False;

	// Move print sub-line to print line
	if ( PrintSubLine(  ) )
	{
		PRJUST = LJ_CENTER;		// Set the Justification flag
		// Line location is size dependent; do not set here
		Status = True;			// Set the status to okay
	}
	// Return the status
	return ( Status );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints the next field(s) Right Justified on Line
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintRGHT( void )
{
	Bool Status = False;

	if ( PrintSubLine(  ) )		// Move print sub-line to print line
	{
		PRJUST = LJ_RIGHT;		// Set the Justification flag
		// Set the print line location to the end of the line
		LineLoc = ( MaxCols + ESC_number ) - 1;
		Status = True;			// Set the status to okay
	}
	return ( Status );			// Return the status
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints the next field at a starting position within the line
//!    (Left Justified)
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintPOSLFT( void )
{
	UBYTE StartPos;
	UBYTE Status = False;

	StartPos = GetToken(  );
	if ( PCL_Error != 0 )
		return False;

	if ( PrintSubLine(  ) )		// Move print sub-line to print line
	{
		if ( StartPos <= ( MaxCols + ESC_number ) )	// See if new starting position is valid
		{						// Yes; update the current position within print line
			LineLoc = StartPos - 1;
			PRJUST = LJ_LEFT;	// Indicate Left Justification
			Status = True;		// Set the status to okay
		}
	}
	return ( Status );			// Return the status
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints the next field at an ending within the line (Right Justified)
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintPOSRGHT( void )
{
	UBYTE EndPos;
	UBYTE Status = False;

	EndPos = GetToken(  );
	if ( PCL_Error != 0 )
		return False;

	if ( PrintSubLine(  ) )		// Move print sub-line to print line
	{
		// See if new field ending position is valid
		if ( EndPos <= ( MaxCols + ESC_number ) )
		{
			LineLoc = EndPos - 1;	// Save new ending position
			PRJUST = LJ_RIGHT;	// Indicate Right Justification
			Status = True;		// Set the status to okay
		}
	}
	return ( Status );			// Return the status
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints the next field at the line center
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintPOSCNTR( void )
{
	UBYTE centerPos;

	centerPos = GetToken(  );
	if ( PCL_Error != 0 )
		return False;

	if ( PrintSubLine(  ) )		// Move print sub-line to print line
	{
		// See if new field position is valid
		if ( centerPos <= ( MaxCols + ESC_number ) )
		{
			LineLoc = centerPos - 1;	// Save new position
			PRJUST = LJ_CENTER;	// Indicate centering
			return True;
		}
	}

	return False;				// There was some problem
}


//-----------------------------------------------------------------------------
//! \brief
//!     Recursive call to PCL interpreter to print another report
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintREPORT( void )
{
	enum REPORTTYPE report;
	UBYTE *savedCMDLOC;
	UBYTE *savedCMDEND;
	UBYTE *savedCMDSTART;
	UWORD savedCMDMORE;
	FILE *savedReportFileHandle;
	long savedFilePos;


	// Get the ID of the report to print
	report = ( enum REPORTTYPE ) GetToken(  );
	if ( PCL_Error != 0 )
		return False;

	// Save old report pointers on the stack
	savedCMDLOC = CMDLOC;
	savedCMDEND = CMDEND;
	savedCMDSTART = CMDSTART;
	savedCMDMORE = CMDMORE;
	savedReportFileHandle = ReportFileHandle;
	if ( ReportFileHandle != 0 )
	{
		// Find the position in the file of the next token
		savedFilePos =
			ftell( ReportFileHandle ) - ( savedCMDEND - savedCMDLOC );
		ReportFileHandle = 0;
	}

	// Set up for PCL interpreter
	if ( FindReport( report ) )	// Set CMDLOC and CMDEND
	{
		// Call the interpreter recursively to interpret the new report
		PrintPCL(  );
	}

	// If a report file was left open, close it
	if ( ReportFileHandle != 0 )
		fclose( ReportFileHandle );

	// Restore the pointers to the old report
	CMDLOC = savedCMDLOC;
	CMDEND = savedCMDEND;
	CMDSTART = savedCMDSTART;
	CMDMORE = savedCMDMORE;
	ReportFileHandle = savedReportFileHandle;

	if ( savedReportFileHandle != 0 )
	{
		// Force re-read of unprocessed data
		if ( 0 != fseek( ReportFileHandle, savedFilePos, SEEK_SET ) )
			if ( 0 == PCL_Error )
				PCL_Error = 2;	// Read past end of report

		CMDLOC = CMDEND = 0;
	}
	else
	{
		// re-read the whole buffer
		SDK_ReadPage( ReportBuffer, sizeof( ReportBuffer ), CMDPAGE,
					  CMDSTART );
	}
	// Return true if no errors occurred in the PCL interpreter
	return ( ( PCL_Error == 0 ) && ( PrinterErrorFlags == 0 ) );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Logical IF command
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintIF( void )
{
	Bool result;
	UBYTE savedRE_Flag;

	// Perform all testing according to subsequent tokens
	result = ComplexTest(  );

	// Increment the recursion level
	if ( ++Level == 0 )
	{
		// Nesting level error in the PCL script
		PCL_Error = 4;
		return False;
	}

	// Save the old recursion level flags
	savedRE_Flag = RE_Flag;

	// Initialize the recursion level flags
	RE_Flag |= RF_ELSE;
	RE_Flag &= ~( RF_OTRUE | RF_ITRUE | RF_ATRUE );
	if ( savedRE_Flag & RF_ITRUE )	// Currently in True state?
	{
		// Set Outer True flag
		RE_Flag |= RF_OTRUE;

		// Is this test True?
		if ( result )
		{
			// Set "Inner True" flag and "True Already occurred" flags
			RE_Flag |= ( RF_ITRUE | RF_ATRUE );
		}
	}

	// Recursively call the PCL interpreter for the IF clause
	PrintPCL(  );
	// ENDIF command will return us back here

	// Restore the recursion level flags
	RE_Flag = savedRE_Flag;

	// Decrement the recursion level
	if ( --Level < 0 )
	{
		// Nesting level error in the PCL script
		PCL_Error = 7;
		return False;
	}

	return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Logical ENDIF command (End of IF or ELSE clause)
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintENDIF( void )
{
	// IF commands cause a recursive call to the PCL interpreter
	// Return False to force a return from the recursive call
	return False;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Logical ELSE (must follow IF)
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintELSE( void )
{
	// Is ELSE allowed? (was there a matching IF?)
	if ( !( RE_Flag & RF_ELSE ) )
	{
		PCL_Error = 6;
		return False;
	}

	// Only one ELSE allowed per IF
	RE_Flag &= ~RF_ELSE;

	// If the PCL interpreter was recursively called from a False IF clause,
	// Or if a previous IF or ELSEIF clause was true,
	// Don't process any statements until the next ENDIF.
	if ( !( RE_Flag & RF_OTRUE ) || ( RE_Flag & RF_ATRUE ) )
	{
		RE_Flag &= ~RF_ITRUE;
	}
	else
	{
		// Otherwise, execute this ELSE clause, and mark that a True clause
		// was found.
		RE_Flag |= ( RF_ITRUE | RF_ATRUE );
	}

	return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Logical ELSE IF (must follow IF)
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintELSEIF( void )
{
	Bool result;

	// Is ELSE allowed? (was there a matching IF?)
	if ( !( RE_Flag & RF_ELSE ) )
	{
		PCL_Error = 6;
		return False;
	}

	// Perform all testing according to subsequent tokens
	result = ComplexTest(  );

	// If the PCL interpreter was recursively called from a False IF clause,
	// Or if a previous IF or ELSEIF clause was true,
	// Or if the test was False,
	// Don't process any statements until the next ENDIF.
	if ( !( RE_Flag & RF_OTRUE ) || ( RE_Flag & RF_ATRUE ) || !result )
	{
		RE_Flag &= ~RF_ITRUE;
	}
	else
	{
		// Otherwise, execute this ELSE clause, and mark that a True clause
		// was found.
		RE_Flag |= ( RF_ITRUE | RF_ATRUE );
	}

	return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Logical OR token should not occur outside of an IF test
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintOR( void )
{
	PCL_Error = 5;
	return False;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Logical AND token should not occur outside of an IF test
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintAND( void )
{
	PCL_Error = 5;
	return False;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Complex test for True or False according to the arguments of
//!     an IF or ELSEIF command
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool ComplexTest( void )
{
	Bool result;
	Bool doingAND;
	UBYTE token;

	// Initialize internal variables
	result = False;
	doingAND = False;

	while ( True )
	{
		// Do one simple test according to the next token
		// If an AND token has been encountered,
		//   AND the return value into result
		// If this is the first test, or an OR token has been read,
		//   OR the return value into result
		if ( doingAND )
			result = SimpleTest(  ) && result;
		else
			result = SimpleTest(  ) || result;

		// Peek at the next token, to see if it is an AND or an OR
		switch ( PeekToken(  ) )
		{
			case AND:
				doingAND = True;
				break;

			case OR:
				doingAND = False;
				break;

			default:
				// Neither AND nor OR, we're through here
				return result;
		}

		// Get the next token (move the pointers this time)
		token = GetToken(  );

		// If there were script errors in any of the above,
		//  abort the interpreter
		if ( PCL_Error != 0 )
			return False;
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Simple test for True or False according to one argument of
//!     an IF or ELSEIF command
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool SimpleTest( void )
{
	UBYTE token;
	Bool result;
	UBYTE mask;

	// Find out what to test
	token = GetToken(  );

	// If the next token is NOT, then test according to the following
	// token, and invert the result.
	if ( NOT == token )
	{
		return !SimpleTest(  );
	}

	// First, see if the token represents a field listed in the field table
	// If so, point pFieldDef to the correct entry in FLDTAB
	if ( FINDFLD( token ) )
	{
		// Set pFieldData to the address and FieldLen to length of the field
		ADR_LEN(  );

		// Is the next token EQUAL?
		if ( PeekToken(  ) == EQUAL )
		{
			// Get the EQUAL token
			GetToken(  );

			// Assume equal until a mismatch is found
			result = True;

			// Read FieldLen tokens, memcmping them to the field value
			do
			{
				// If any bytes do not match, then the string does not match
				if ( *pFieldData++ != GetToken(  ) )
					result = False;

			}
			while ( --FieldLen != 0 );

			return result;
		}
		else
		{
			// Next token was not EQUAL

			// If the field length is zero, then return False
			if ( 0 == FieldLen )
				return False;

			result = TST_FLD(  );
		}
	}

	// Not a field
	else
	{
		switch ( token )
		{

				// TRANSACTION TESTS
			case TRN_SALE:
				result = TestTRN_SALE(  );	// SALE
				break;

			case TRN_REFD:
				result = TestTRN_REFD(  );	// REFUND
				break;

			case TRN_AUTH:
				result = ( AUTH == TRLOC->TRKEY );	// AUTH ONLY
				break;

			case TRN_BALI:
				result = TestTRN_BALI(  );	// BALANCE INQUIRY
				break;

			case TRN_CRCD:
				result = TestTRN_CRCD(  );	// CREDIT CARD
				break;

			case TRN_DBCD:
				result = TestTRN_DBCD(  );	// DEBIT CARD
				break;

			case TRN_OFFL:
				result = TestTRN_OFFL(  );	// OFFLINE
				break;

			case TRN_VOID:
				result = TestTRN_VOID(  );	// VOID
				break;

			case TRN_AVSVOID:
				result = TestTRN_AVSVOID(  );	// AVS VOID
				break;

			case TRN_OPENT:
				result = TestTRN_OPENT(  );	// OPEN TAB
				break;

	        case TRN_SETTLE:
	        	result = ( (CLBATCH == TRLOC->TRKEY) || (CLBATCHA == TRLOC->TRKEY) || (SETTLE == TRLOC->TRKEY) );     /* SETTLEMENT */
	            break;

			case TRN_NOSIG:
				result = TestTRN_NOSIG(  );	// Suppress Signature line?
				break;

				// CONDITION TESTS
			case CND_LNEQ:
				result = ( PRLINE == GetToken(  ) );
				break;

			case CND_LNGE:
				result = ( PRLINE >= GetToken(  ) );
				break;

			case CND_FIRST:
				result = !( PRFLAG & PF_PREV );
				break;

			case CND_REJ:
				result = TestCND_REJ(  );
				break;

			case CND_TIP:
				result = TestCND_TIP(  );
				break;

			case CND_REPR:
				result = TestCND_REPR(  );
				break;

			case CND_PREP:
				result = TestCND_PREP(  );
				break;

			case CND_SREQ:
				result = TestCND_SREQ(  );
				break;

			case CND_ROLL:
				result = RollPrinter;
				break;

			case CND_MAN:
				result = TestCND_MAN(  );
				break;

			case CND_ACCSIG:
				result = ( ( TRLOC->TRRSPOPT & R_APPROVED ) != 0 );
				break;

				// TESTS FOR PRINT OPTIONS
			case OPT1:
			case OPT2:
			case OPT3:
			case OPT4:
			case OPT5:
			case OPT6:
			case OPT7:
			case OPT8:
				// Calculate a bit mask
				mask = 0x01 << ( token - OPT1 );

				result = ( ( PROPT & mask ) != 0 );
				break;
#ifdef MAKE_FARSI
            case LNG_FARSI:
                if( LOCTAB.LANGID == LNG_FA )
                {
                    result = True;
                }
                else
                {
                    result = False;
                }
                break;
#endif
			default:
				PCL_Error = 11;
				result = False;
				break;
		}
	}

	return result;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Stops pre-printing
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintPPSTOP( void )
{
	// If printing is currently enabled, flush the print buffers
	if ( !( PRFLAG & PF_OFFP ) )
	{
		if ( !FlushBuffers( '\n' ) )
			return False;
	}

	// If we are doing preprinting, turn printing off now
	// If we previously did preprinting, and are now finishing
	// the receipt, we need to turn printing on now.
	// If we never did preprinting, printing is always enabled,
	// so enabling it does not change anything.
	if ( PRFLAG & PF_PREP )
		PRFLAG |= PF_OFFP;
	else
		PRFLAG &= ~PF_OFFP;

	return True;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints a signature
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintSignature( void )
{
#ifdef MAKE_SIGCAPTURE
    UBYTE *pImage;
	Bool retval;

	// Does this printer support graphics?
	if ( !GraphicsPrinter )
		return True;			// No error, just ignore the command

	// If no signature is stored, return O.K.
	if ( 0 == TRLOC->TRSIGLEN )
		return True;

	// Flush the print buffer
	if ( !FlushBuffers( 0 ) )
		return False;			// Printer error, no use continuing

	retval = True;

	// Convert signature to image, and send to the printer
    pImage= BuildPrintableSignature( TERM.TERMPrint,TRLOC->TRSIGDATA,TRLOC->TRSIGLEN );
    if ( pImage )
    {
        switch ( SDK_PrintImage( 0, pImage ) )
        {
            case prti_OK:
                break;

            case prti_PAPEROUT:
                PrinterErrorFlags = PaperOutError;
                SDK_PrinterClose();
                retval = False;
                break;

            default:
                PrinterErrorFlags = OtherPrintError;
                SDK_PrinterClose();
                retval = False;
                break;
        }
    }
    else
    {
        PrinterErrorFlags = OtherPrintError;
        retval = False;
    }

	return retval;
#else // MAKE_SIGCAPTURE
	return True;
#endif // MAKE_SIGCAPTURE
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints bitmapped graphics
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintGraphics( void )
{
	struct
	{
		UBYTE high;
		UBYTE low;
	}
	ID;
	static char fileName[12];
	FILE *fileHandle;
	Bool retval;

	ID.low = GetToken(  );
	ID.high = GetToken(  );

    if ( PCL_Error != 0 )
		return False;

	// Flush the print buffer
	if ( !FlushBuffers( 0 ) )
		return False;			// Printer error, no use continuing

	// Does this printer support graphics?
	if ( !GraphicsPrinter )
		return True;			// No error, just ignore the command

	// For signature capture, we print the receipt to a buffer, sans graphics
	if ( !GraphicsEnabled )
		return True;			// No error, just ignore the command


	// if a file exists with this image, print it
	memcpy( fileName, "ip0000", ( UWORD ) 7 );
	BufAsc( &fileName[2], &ID.high );	// Convert high byte to ASCII
	BufAsc( &fileName[4], &ID.low );	// Convert low byte to ASCII

	retval = True;

	// Check for paper out or other printer hardware error
	if ( PrintErrDetect(  ) )
		return False;

    DBGTRACE( LVL_DEBUG, TYPE_SPOS32, "%s: print \"%s\" image\n", __FUNCTION__, fileName );

	fileHandle = fopen( fileName, "r" );
	if ( fileHandle != 0 )
	{
		switch ( SDK_PrintFileImage( fileHandle ) )
		{
			case prti_OK:
				break;

			case prti_PAPEROUT:
				PrinterErrorFlags = PaperOutError;
				SDK_PrinterClose(  );
				retval = False;
				break;

            case prti_OVERHEAT:
				PrinterErrorFlags = PrinterOverheatError;
				SDK_PrinterClose(  );
				retval = False;
				break;

			default:
				PrinterErrorFlags = OtherPrintError;
				SDK_PrinterClose(  );
				retval = False;
				break;
		}

		fclose( fileHandle );
	}

	return retval;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Switches color ( if Verifone 250 printer )
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintRed( void )
{
	if ( prt_VERIFONE250 == TCONF.TCPRINTER )
		return PrintItem( "\x12", 1 );	// DC2

	return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints Text in table
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
extern Bool PrintText( void )
{
	TextGetString( TextGetBuf, TxtTableId, GetToken(  ) );

	return PrintItem( TextGetBuf, ( UBYTE ) strlen( TextGetBuf ) );
}



//-----------------------------------------------------------------------------
//! \brief
//!     Checks if Sale type transaction
//!
//! \return
//!     True if Sale Transaction else False
//-----------------------------------------------------------------------------
extern Bool TestTRN_SALE( void )
{
	// If it's a refund, it's not a sale
	if ( TestTRN_REFD(  ) )
		return False;

	// TestTRN_REFD() will find the transaction and fill SecondEvent

	// Is it a captured transaction?
	if ( SecondEvent.TRNOPT1 & T1_NOCAPTURE )
		return False;
	else
		return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if Refund type transaction
//!
//! \return
//!     True if Refund Transaction else False
//-----------------------------------------------------------------------------
extern Bool TestTRN_REFD( void )
{
	// Get the Transaction Type
	SecondEvent.TRNKEY = TRLOC->TRKEY;

	// Locate the Transaction - put in SecondEvent
	FindTran ();

	if ( SecondEvent.TRNOPT1 & T1_CREDIT )
		return True;
	else
		return False;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if the transaction is a debit
//!
//! \return
//!     True if Balance Inquiry Transaction else False
//-----------------------------------------------------------------------------
extern Bool TestTRN_BALI( void )
{
	if ( TRLOC->TRKEY == BALINQ )
		return True;
	else
		return False;
}

//-----------------------------------------------------------------------------
//! \brief
//!     Checks if the transaction is a credit card trans
//!
//! \return
//!     True if credit card transaction else False
//-----------------------------------------------------------------------------
extern Bool TestTRN_CRCD( void )
{
	// If it's a Debit card transaction, then it's not a credit card trans
	if ( TestTRN_DBCD(  ) )
		return False;

	// Get the Transaction Type
	SecondEvent.TRNKEY = TRLOC->TRKEY;

	// Locate the Transaction - put in SecondEvent
	FindTran ();

	if ( SecondEvent.TRNOPT1 & T1_NOCAPTURE )
		return False;
	else
		return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if the transaction is a debit
//!
//! \return
//!     True if DEBIT type transaction else False
//-----------------------------------------------------------------------------
extern Bool TestTRN_DBCD( void )
{
	return ( (TRLOC->TRKEY == SALCASH) || (TRLOC->TRKEY == DBCASH) 
#ifdef	MAKE_EMV
	|| (TRLOC->TRKEY == EMV_SALCASH) || (TRLOC->TRKEY == EMV_CASH)
#endif	// MAKE_EMV
	);
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if Offline Transaction
//!
//! \return
//!     True if Offline Transaction else False
//-----------------------------------------------------------------------------
extern Bool TestTRN_OFFL( void )
{
	SecondEvent.TRNKEY = TRLOC->TRKEY;	// Get the Transaction Type

	// Locate the Transaction - will be in SecondEvent
	FindTran ();

	if ( SecondEvent.TRNOPT2 & T2_OFFLINE )	// See if Offline Transaction
		return ( True );		// Yes; return true
	else
		return ( False );		// No; return false
}

//-----------------------------------------------------------------------------
//! \brief
//!     Checks if Voided Transaction
//!
//! \return
//!     True if Voided Transaction else False
//-----------------------------------------------------------------------------
extern Bool TestTRN_VOID( void )
{
	if ( TRLOC->TRSTATUS[0] & TS1_VOID )	// See if Voided Transaction
		return ( True );		// Yes; return true
	else
		return ( False );		// No; return false
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if AVS Voided Transaction
//!
//! \return
//!     True if AVS Voided Transaction else False
//-----------------------------------------------------------------------------
extern Bool TestTRN_AVSVOID( void )
{
	if ( TRLOC->TRSTATUS[1] & TS2_AVSVOID )	// See if Voided Transaction
		return ( True );		// Yes; return true
	else
		return ( False );		// No; return false
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if Open Tab Transaction
//!
//! \return
//!     True if Open Tab Transaction else False
//-----------------------------------------------------------------------------
extern Bool TestTRN_OPENT( void )
{
	if ( TRLOC->TRKEY == OPENTAB )
		return True;
	else
		return False;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if the Transaction was Rejected
//!
//! \return
//!     True if Transaction was Rejected else False
//-----------------------------------------------------------------------------
extern Bool TestCND_REJ( void )
{
	// See if Transaction was Approved
	if ( TRLOC->TRRSPOPT & R_APPROVED )
		return ( False );		// Yes; return false
	else
		return ( True );		// No; return true
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if Tip Processing is on
//!
//! \return
//!     True if Tip Processing is on else False
//-----------------------------------------------------------------------------
extern Bool TestCND_TIP( void )
{
	if ( TCONF.TCOPT1 & TC1_REST )	// See if Restaurant Mode selected
	{
		SecondEvent.TRNKEY = TRLOC->TRKEY;	// Yes; get the Transaction Type

		// Locate the Transaction - will be in SecondEvent
		FindTran ();

		if ( !( SecondEvent.TRNOPT1 & T1_NOTIP ) )	// Is this a Tip type Transaction
			return ( True );	// Yes; return true
	}
	return ( False );			// Return false
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if Transaction is being Reprinted
//!
//! \return
//!     True if Transaction is being Reprinted else False
//-----------------------------------------------------------------------------
extern Bool TestCND_REPR( void )
{
	if ( TRLOC->TRREPRINT )		// See if Transaction is being Reprinted
		return ( True );		// Yes; return true
	else
		return ( False );		// No; return false
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if Preprinting is in progress
//!
//! \return
//!     True if Preprinting is in progress else False
//-----------------------------------------------------------------------------
extern Bool TestCND_PREP( void )
{
	// If pre-printing is not enabled, return false
	if ( !( PRFLAG & PF_PREP ) )
		return False;

	// If printing is off ( we already executed the PPSTOP),
	// then return false
	if ( PRFLAG & PF_OFFP )
		return False;

	return True;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if Signature not required
//!
//! \return
//!     True if Signature is not required else False
//-----------------------------------------------------------------------------
extern Bool TestCND_SREQ( void )
{
	// See if "SIGNATURE NOT REQUIRED" print is on
	if ( (TRLOC->TRISPTR->ISOPT3) & IS3_NOSIGNAT )
	{
		// Yes; see if Track 2 data present
		if ( TRLOC->TRTRK2[0] )
		{
			// Was there PIN PAD entry?
			if ( PinTranEnabled () )
			{
				// There was PIN Entry, return True.
				return ( True );
			}
		}
	}

	// Return false
	return ( False );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if the PAN was manually entered
//!
//! \return
//!     True if PAN was manually entered else False
//-----------------------------------------------------------------------------
extern Bool TestCND_MAN( void )
{
	// See if the PAN was manually entered
	if ( 0x10 == ( TRLOC->TRPOSE[1] & 0xf0 ) )
		return ( True );		// Yes; return true
	else
		return ( False );		// No; return false
}


//-----------------------------------------------------------------------------
//! \brief
//!     Checks if signature is to be suppressed
//!
//! \return
//!     True if signature is suppressed else False
//-----------------------------------------------------------------------------
extern Bool TestTRN_NOSIG( void )
{
#ifdef MAKE_EMV
	if (TRLOC->TRSTATUS[1] & TS2_PINBLOCK) return True;
	else								   return False;
//	return( SuppressSigLine );	// Suppress Signature line?
#else				
	return( False );
#endif			
}



//=============================================================================
// Private function definitions
//=============================================================================


//-----------------------------------------------------------------------------
//! \brief
//!     Get the next PCL token, report error if no more tokens
//!
//! \return
//!     Token or NO_CMD if no more tokens
//!
//! \note
//!     Error code will be put in PCL_Error.
//-----------------------------------------------------------------------------
static UBYTE GetToken( void )
{
	// PeekToken will read from the script file, if necessary
	if ( NO_CMD == PeekToken(  ) )
	{
		PCL_Error = 2;			// Read past end of report
		return NO_CMD;			// Invalid token
	}
	else
		return *CMDLOC++;
}


//-----------------------------------------------------------------------------
//! \brief
//!     Buffers the sub-line for printing
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
static Bool PrintSubLine( void )
{
	Bool Status;
	UBYTE TrueLength;
	UBYTE i, j;

	// See if sub-line length is zero
	if ( !PRBUF3LN )
		return ( True );		// Yes; return okay

	Status = True;				// Set status to okay

	// Calculate true sub-line length based on PF_DBLE flag
	if ( PRFLAG & PF_DBLE )
		TrueLength = PRBUF3LN << 1;	// Double Wide Characters
	else
		TrueLength = PRBUF3LN;	// Normal Wide Characters

	// See what type of Text Justification on sub-line is needed
	switch ( PRJUST )
	{
		case LJ_LEFT:			// Left Justify Text on Line
			// Truncate if necessary
			if ( LineLoc + TrueLength > ( MaxCols + ESC_number ) )
			{
				if ( PRFLAG & PF_DBLE )	// Double wide printing?
				{
					PRBUF3LN = ( ( MaxCols + ESC_number ) - LineLoc ) / 2;
					TrueLength = PRBUF3LN << 1;
				}
				else
				{				// Single wide printing
					PRBUF3LN = ( MaxCols + ESC_number ) - LineLoc;
					TrueLength = PRBUF3LN;
				}
			}
			break;

		case LJ_CENTER:		// Center Justify Text on Line
			// See if sub-line will fit in line
			if ( ( MaxCols + ESC_number ) >= (TrueLength - 1) )
			{
				// Calculate current position within print line
				LineLoc = ( ( MaxCols + ESC_number ) - TrueLength + 1 ) >> 1;
			}
			else
			{
				LineLoc = 0;
				TrueLength = ( MaxCols + ESC_number );
			}
			break;

		case LJ_RIGHT:			// Right Justify Text on Line

			// See if sub-line will fit in line
			if ( LineLoc >= ( TrueLength - 1 ) )
			{
				// Yes; calculate current position within print line
				LineLoc = LineLoc - TrueLength + 1;
			}
			else
			{					// Truncate line to fit
				if ( PRFLAG & PF_DBLE )	// Double wide printing?
				{
					PRBUF3LN = ( LineLoc + 1 ) / 2;
					TrueLength = PRBUF3LN << 1;
				}
				else
				{				// Single wide printing
					PRBUF3LN = LineLoc + 1;
					TrueLength = PRBUF3LN;
				}
			}
			break;

		default:				// Unkown Justification Type
			PCL_Error = 26;
			Status = False;
			break;
	}

	// See if status okay
	if ( Status )
	{
		// Yes; see if Printing Double Wide characters
		if ( PRFLAG & PF_DBLE )
		{
			// Yes; move print sub-line to print line with Double
			// character
			for ( i = 0; i < PRBUF3LN; i++ )
			{
				// Calculate offset
				j = i << 1;
				LineBuff[LineLoc + j] = DOUB;
				LineBuff[LineLoc + j + 1] = BUF3[i];
			}
		}
		else					// No; move print sub-line to print line
		{
			memcpy( &LineBuff[LineLoc], BUF3, TrueLength );
		}

		// Update print line location
		LineLoc += TrueLength;

		// See if print line length needs to be updated
		if ( LineLoc > LineSize )
			LineSize = LineLoc;	// Yes; update it

		PrintPos = 0;			// Reset the current position within print sub-line
		PRBUF3LN = 0;			// Reset the current print sub-line length
		memset( BUF3, ' ', BUFSZE );	// Clear the printer sub-line buffer
	}

	// Return the status
	return ( Status );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Prints the current line of data with the handling of both Normal and
//!     Double Wide characters
//!
//! \param
//!		[in]ForceIt			Flush print buffer flag
//!
//! \return
//!     False if error detected else True
//!
//! \note
//!     Uses BUF1 and BUF2 to build printer data.
//-----------------------------------------------------------------------------
static Bool PrintLineBuff( Bool ForceIt )
{
	UBYTE i = 0, j = 0;
	Bool Dmode = False;

	// Build printer data
	while ( i < LineSize )
	{
		// See if character is DOUBLE indicator
		if ( DOUB == LineBuff[i] )
		{
			i++;				// Increment index into line buffer

			// Yes; are we already in DOUBLE mode ?
			if ( False == Dmode )
			{					// No; add DOUBLE mode character to printer data
				BUF1[j] = DOUB;
				j++;			// Increment index into printer data
				Dmode = True;	// Set DOUBLE mode to true
			}
		}
		else
		{						// No; are we in DOUBLE mode ?
			if ( True == Dmode )
			{					// Yes; add NORMAL mode character to printer data
				BUF1[j] = NORM;
				j++;			// Increment index into printer data
				Dmode = False;	// Set DOUBLE mode to false
			}
		}

		BUF1[j] = LineBuff[i];	// move character to printer data
		i++;					// Increment index into line buffer
		j++;					// Increment index into printer data
	}

	if ( Dmode )				// Are we still in DOUBLE mode ?
	{
		// Yes; add NORMAL mode character to printer data
		BUF1[j] = NORM;
		j++;					// Increment index into printer data
	}

	return ( ( *PrintRtn ) ( BUF1, j, ForceIt ) );	// Send data to the printer
}


//-----------------------------------------------------------------------------
//! \brief
//!     Tests to see if the field data exists
//!
//! \return
//!     False if error detected else True
//-----------------------------------------------------------------------------
static Bool TST_FLD( void )
{
	Bool Status = False;
	UBYTE i;

	// Okay; figure what to test for
	switch ( pFieldDef->F_TEST )
	{
		case T_NULL:			// Nulls
			// Check if Field exists
			if ( !NullComp( ( char * ) pFieldData, FieldLen ) )
				Status = True;	// Yes; set status to okay
			break;

		case T_NOS:			// Nulls or Spaces
			for ( i = 0; i < FieldLen; i++ )	// Check if Field exists
			{
				// See if character exists
				if ( ( ' ' != pFieldData[i] ) && ( 0 != pFieldData[i] ) )
				{
					Status = True;	// Yes; set status to okay
					break;		// Exit the loop
				}
			}
			break;


		case T_FF:				// This field is filled with 0xFF when empty

			// If Field is not empty
			for ( i = 0; i < FieldLen; i++ )
			{
				if ( ( pFieldData[i] != 0xFF ) )
				{
					// return True
					Status = True;
					break;
				}
			}

			// If we got here, the field was empty
			break;

		case T_ALWAYS:			// Always
			Status = True;		// Set status to okay
			break;

		case T_FLAG:			// Flag field?
			// Add in the offset to the flag byte
			pFieldData += GetToken(  );

			// Get the bit mask
			i = GetToken(  );

			// memcmpe flag byte with the mask
			Status = ( ( *pFieldData & i ) != 0 );

			break;

        case T_FLAGX:                /* Extended Flag field? */
            /* Skip the offset parameter since it's alreay processed
               by A_ISS/A_ACQ/... */
            GetToken();

            /* Get the bit mask */
            i = GetToken();

            /* Compare flag byte with the mask */
            Status = ( (*pFieldData & i) != 0 );

            break;

		case T_SPC:	// Spaces
			for ( i = 0; i < FieldLen; i++ )	// Check if Field exists
			{
				// See if character exists
				if ( ' ' != pFieldData[i] )
				{
					Status = True;	// Yes; set status to okay
					break;		// Exit the loop
				}
			}
			break;

		default:				// Unknown Test Type
			PCL_Error = 25;
			break;
	}

	return ( Status );			// Return the status
}



//-----------------------------------------------------------------------------
//! \brief
//!     Builds the field
//!
//! \return
//!     False if error detected else True
//!
//! \note
//!     Error code will be put in PCL_Error.
//-----------------------------------------------------------------------------
static Bool BLD_FLD( UBYTE Field )
{
	if ( FINDFLD( Field ) )		// See if the argument is a valid field
		return ( EDITFLD(  ) );	// Yes; edit the field
	else
	{
		PCL_Error = 12;			// No; save the error status
		return ( False );		// Return false
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Find the Field Definition in one of the field tables
//!
//! Find the Field Definition in one of the field tables. Some of the fields
//! from 160 to 230 are reserved, so that range will be skipped by the tables.
//!
//! \return
//!     True if found else False
//-----------------------------------------------------------------------------
static Bool FINDFLD( UBYTE field )
{
	// Check if Field is a valid entry in FLDTAB or FLDTAB2
	if ( ( ( field < MARKER1 ) && ( 160 > MARKER1 ) ) ||
		 ( ( 230 < field ) && ( field < MARKER2 ) && ( MARKER1 < 255 ) ) )
	{
		// setup field number to locate for field definition
		vFieldNum = field;
		FindField ();
		pFieldDef = ( struct FLDDEF * ) &FieldDef;

		return ( True );		// Return entry found
	}

	return ( False );			// No; return entry not found
}


//-----------------------------------------------------------------------------
//! \brief
//!     Gets the true Field Definition, Address and Length of the Field
//!
//! \return
//!     False if error detected else True
//!
//! \note
//!     Error code will be put in PCL_Error.
//-----------------------------------------------------------------------------
static Bool ADR_LEN( void )
{
	UBYTE Status;
	struct FLDDEF *( *pFunc ) ( void );
	struct desc_rec *pDesc;

	// Do until done
	do
	{
		Status = 0;				// Set status to okay

		// Get the Addressing Type
		switch ( pFieldDef->F_ADRTYP )
		{
			case A_ACT:		// Actual Address
				pFieldData = pFieldDef->F_ADR;	// Set address of the field
				FieldLen = pFieldDef->F_LEN;	// Set the length of the field
				break;
			case A_ACQ:		// Offset into the Acquirer Table
                FieldLen = pFieldDef->F_LEN;  /* Set the length of the field */
				/* Check if it's a T_FLAGX type field, add ofset if so */
				/* Move issuer data into local buffer */
				MoveItAq( LOCBUF, 
						  (UBYTE*)TRLOC->TRAQPTR + (getoffset( pFieldDef->F_ADR ) + ((T_FLAGX == pFieldDef->F_TEST) ? PeekToken() : 0)),
						  FieldLen );
                pFieldData = LOCBUF;
				break;

			case A_CRD:		// Offset into the CARD Table
                FieldLen = pFieldDef->F_LEN;  /* Set the length of the field */

				/* Check if it's a T_FLAGX type field, add ofset if so */
				/* Move issuer data into local buffer */
				MoveItCR( (void*)LOCBUF, 
						   (UBYTE *)TRLOC->TRCRPTR + (getoffset( pFieldDef->F_ADR ) + ((T_FLAGX == pFieldDef->F_TEST) ? PeekToken() : 0)),
						    FieldLen );
                pFieldData = LOCBUF;
				break;


			case A_ISS:		// Offset into the Issuer Table
                FieldLen = pFieldDef->F_LEN;  /* Set the length of the field */

				/* Check if it's a T_FLAGX type field, add ofset if so */
				/* Move issuer data into local buffer */
				MoveItIss( LOCBUF, 
						   (UBYTE *)TRLOC->TRISPTR + (getoffset( pFieldDef->F_ADR ) + ((T_FLAGX == pFieldDef->F_TEST) ? PeekToken() : 0)),
						    FieldLen );
                pFieldData = LOCBUF;
				break;

			case A_TRN:		// Offset into Transaction Definition
				SecondEvent.TRNKEY = TRLOC->TRKEY;	// Set the transaction type

				// Get the Transaction Definition
				FindTran ();

				// Set address of the field
				pFieldData = pFieldDef->F_ADR;

				// Set the length of the field
				FieldLen = StrLn( ( char * ) pFieldData, pFieldDef->F_LEN );
				break;

			case A_BAT:		// Offset into the Batch Table

				// Set address of the field
				pFieldData =
					( UBYTE * ) FindBatTab( TRLOC->TRAQID ) +
					getoffset( pFieldDef->F_ADR );

				// Set the length of the field
				FieldLen = pFieldDef->F_LEN;
				break;

			case A_DSC:		// Descriptor Table Entry
				// Set up pointer to Descriptor
				pFieldData = ( UBYTE * ) TRLOC + getoffset( pFieldDef->F_ADR );

				pDesc = FindDesc( *pFieldData );	// Look up the Descriptor
				FieldLen = pFieldDef->F_LEN;	// Set the length of the field

				if ( pDesc )	// See if found
				{				// Yes
					// Set address of the field to point to Descriptor Text
					pFieldData = pDesc->DSTXT;
				}
				else
				{
					memset( BUF1, ' ', FieldLen );	// No; set a buffer to spaces
					// Set address of the field to point to this buffer
					pFieldData = ( UBYTE * ) BUF1;
				}
				break;

			case A_RTC:		// Offset into RTCBUFF after RTC Read call
				PRT_Read_RTC ();
				pFieldData = pFieldDef->F_ADR;	// Set address of the field
				FieldLen = pFieldDef->F_LEN;	// Set the length of the field
				break;

			case A_TR:			// Offset into the TR Buffer (TRINP, ...)
				// Set address of the field
				pFieldData = ( UBYTE * ) TRLOC + getoffset( pFieldDef->F_ADR );
				FieldLen = pFieldDef->F_LEN;	// Set the length of the field
				break;

			case A_CUS:		// Call Custom Routine to build Field
				// Move Routine Address locally
				memcpy( &pFunc, &PRVECT[pFieldDef->F_LEN], sizeof( pFunc ) );

				// Check for routine index range
				
//                                if ( ( pFieldDef->F_LEN >= 0 ) && ( pFieldDef->F_LEN <= 7 ) )
                                if ( pFieldDef->F_LEN <= 7 )
				{
					// Check Routine Address for validity
					if ( !NullComp( ( char * ) &pFunc, sizeof( pFunc ) ) )
					{
						pFieldDef = ( *pFunc ) (  );	// Okay get new Addressing Type
						Status = 99;	// Repeat step with new Addressing Type
					}
				}
				else
				{
					Status = 15;	// Invalid Address
				}
				break;

                        case A_V2:			// Offset into the Visa 2 Table
				// Set address of the field
				pFieldData =
					( UBYTE * ) FindVisa2( TRLOC->TRAQID ) +
					getoffset(pFieldDef->F_ADR);

				// Set the length of the field
				FieldLen = pFieldDef->F_LEN;

				break;

			default:			// Unknown Addressing Type
				Status = 14;
				break;
		}
	}
	while ( 99 == Status );

	if ( 0 == Status )			// See if status is okay
		return ( True );		// Yes; return okay
	else
	{
		PCL_Error = Status;		// No; save the error status
		return ( False );		// Return error
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Builds a Character representation of the Field
//!
//! \return
//!     False if error detected else True
//!
//! \note
//!     Error code will be put in PCL_Error.
//-----------------------------------------------------------------------------
static Bool EDITFLD( void )
{
	Bool Status = False;
	UBYTE RemBytes;
	UWORD TempVal;				//Before UBYTE
	UWORD *pword;
	UBYTE tempbuf[2];			//Before UBYTE


	// Get the true Field Definition, Address, and Length of the Field
	if ( ADR_LEN(  ) )
	{
		Status = True;			// Okay; set status to okay
		FLD_ADR = BUF1;			// Set Field Address to start of buffer
		switch ( pFieldDef->F_EDIT )	// Figure out how to edit
		{
			case E_CHR:		// ASCII Field -- just move the Field
				memcpy( FLD_ADR, pFieldData, FieldLen );
				FLD_LEN = FieldLen;	// Set the length of the Field
				break;

			case E_CHRTRNC:	// ASCII Field -- truncated to Prt width

				// See if field larger than print line
				if ( S_HSTPRDATA > ( MaxCols + ESC_number ) )
					FLD_LEN = ( MaxCols + ESC_number );	// Yes; set field length to print line width
				else
					FLD_LEN = S_HSTPRDATA;	// No; set field length

				// Check if printing in DOUBLE:
				if ( PRFLAG & PF_DBLE )
				{
					// Divide by two
					FLD_LEN >>= 1;
				}

				// Move the field
				memcpy( FLD_ADR, pFieldData, FLD_LEN );

				break;

			case E_PAN:		// PAN Field
				RemBytes = MovPanAsc( FLD_ADR, pFieldData, MAXPANL );
				FLD_LEN = MAXPANL - RemBytes;	// Set the length of the Field
				break;

			case E_BCD:		// BCD Field
				BfAscii( FLD_ADR, pFieldData, FieldLen );
				FLD_LEN = FieldLen << 1;	// Set the length of the Field
				break;

			case E_HEX:		/* BCD Field */
				pword = ( UWORD * ) pFieldData;
				TempVal = ( UWORD ) CvtBcd( *pword );
				Encode( tempbuf, TempVal )
					BfAscii( FLD_ADR, tempbuf, FieldLen );
				FLD_LEN = FieldLen << 1;	/* Set the length of the Field */
				break;

			case E_AMT1:		// Amount Field -- Receipt Amounts
				// See if Transaction Approved
				if ( TRLOC->TRRSPOPT & R_APPROVED )
				{
					// Yes; see if Transaction Voided
					if ( TRLOC->TRSTATUS[0] & TS1_VOID )
					{
						// Yes; indicate Negative Amount
						HOLDSGN = 0x00;
					}
					else
					{
						// No; set the transaction type
						SecondEvent.TRNKEY = TRLOC->TRKEY;

						// Get the Transaction Definition
						FindTran ();

						// See if a Credit Transaction
						if ( SecondEvent.TRNOPT1 & T1_CREDIT )
							HOLDSGN = 0x00;	// Yes; indicate Negative Amount
						else
							HOLDSGN = 0xff;	// No; indicate Positive Amount
					}
					CvtAmt( FLD_ADR, pFieldData );	// Convert the Amount
					if ( 0x00 == HOLDSGN )	// Should Amount be Negative ?
						NegSign( FLD_ADR, 13 );	// Yes; make Amount negative
				}
				else
				{
					// No; indicate Non-Approved Amount
					HOLDSGN = '*';
					// Set Amount to Astericks
					memset( ( UBYTE * ) FLD_ADR, '*', ( UWORD ) 13 );
				}

				FLD_LEN = 13;	// Set the length of the Field
				break;

			case E_AMT2:		// Amount Field -- Normal Amounts
				HOLDSGN = 0xff;	// Indicate Positive Amount
				CvtAmt( FLD_ADR, pFieldData );	// Convert the Amount
				FLD_LEN = 13;	// Set the length of the Field
				break;

			case E_AMT3:		// Amount Field -- Amount possibly Negative
				if ( pFieldData[0] < 0x50 )	// See if the Amount is positive
				{
					HOLDSGN = 0xff;	// Yes; indicate Positive Amount
					CvtAmt( FLD_ADR, pFieldData );	// Convert the Amount
				}
				else
				{
					HOLDSGN = 0x00;	// No; indicate Negative Amount

					// Clear the Hold Amount
					memset( HOLDAMT, 0, sizeof( HOLDAMT ) );

					// Subtract the Amount from zero
					SubAmt( HOLDAMT, HOLDAMT, pFieldData );
					CvtAmt( FLD_ADR, HOLDAMT );	// Convert the Amount
					NegSign( FLD_ADR, 13 );	// Make Amount negative
				}
				FLD_LEN = 13;	// Set the length of the Field
				break;

			case E_ASCYMD:		// ASCII Date Field in YMD Format
				memcpy( FLD_ADR, pFieldData + 2, ( UWORD ) 4 );	// Move Month/Day
				memcpy( &FLD_ADR[4], pFieldData, 2 );	// Move Year
				FLD_LEN = FieldLen;	// Set the length of the Field
				break;

			case E_BCDYMD:		// BCD Date Field in YMD Format
				// Convert/move Month/Day
				BfAscii( FLD_ADR, pFieldData + 1,
						 ( UBYTE ) ( FieldLen - 1 ) );

				// Convert/move Year
				BufAsc( &FLD_ADR[( FieldLen - 1 ) << 1], pFieldData );

				FLD_LEN = FieldLen << 1;	// Set the length of the Field
				break;

			case E_NAME:		// Track 1 Name
				GetName(  );
				break;

			case E_TIPPCT:		// Tip Percent

				SetTip(  );

				break;

#ifdef	MAKE_EMV
			case E_VARHEX:		// Var len hex field, len in first byte
				BfAscii( FLD_ADR, pFieldData+1, *(pFieldData) );
				FLD_LEN = *(pFieldData) << 1;	// Set the length of the Field
				break;
#endif	//MAKE_EMV

			default:			// Unknown Edit Type
				PCL_Error = 16;
				Status = False;	// Error status
				break;
		}
	}
	return ( Status );			// Return the status
}


//-----------------------------------------------------------------------------
//! \brief
//!     Reformats a field
//!
//! \param
//!		[in]Format			Reformat type
//!
//! \return
//!     False if error detected else True
//!
//! \note
//!     Error code will be put in PCL_Error.
//-----------------------------------------------------------------------------
static Bool FMT_FLD( UBYTE Format )
{
	Bool Status;

	// Figure out how to reformat using the Field Type
	switch ( pFieldDef->F_TYPE )
	{
		case F_CHR:			// Character Format
			Status = FMT_CHR( Format );
			break;

		case F_AMT:			// Amount Format
			Status = FMT_AMT( Format );
			break;

		case F_NUM:			// Numeric Format
			Status = FMT_NUM( Format );
			break;

		case F_DATE:			// Date Format
			Status = FMT_DATE( Format );
			break;

		case F_TIME:			// Time Format
			Status = FMT_TIME( Format );
			break;

		default:				// Unknown Field Type
			PCL_Error = 10;
			Status = False;		// Error status
			break;
	}
	return ( Status );			// Return the status
}



//-----------------------------------------------------------------------------
//! \brief
//!     Character Formats
//!
//! \param
//!		[in]Format			Fortmat type
//!
//! \return
//!     False if error detected else True
//!
//! \note
//!     Error code will be put in PCL_Error.
//-----------------------------------------------------------------------------
static Bool FMT_CHR( UBYTE Format )
{
	Bool Status = True;
	UBYTE ORG_LEN;
	UBYTE i;

	ORG_LEN = FLD_LEN;			// Save the original length
	// Trim leading and trailing spaces
	FLD_ADR = FMT_STRLEN( &FLD_LEN );

	switch ( Format )			// Figure out how to reformat
	{
		case FC_CENTER:		// Character - Center Justified
			memset( BUF2, ' ', BUFSZE );	// Clear Buffer 2
			// Move field - Center Justified
			memcpy( &BUF2[( ORG_LEN - FLD_LEN ) >> 1], FLD_ADR, FLD_LEN );
			FLD_ADR = BUF2;		// Set address of the field
			FLD_LEN = ORG_LEN;	// Set length of the field
			break;

		case FC_TRIM:			// Character - Trim
			break;				// Nothing needs to be done


		case FC_CLRFF:
			memset( BUF2, ' ', BUFSZE );	// Clear Buffer 2
			memcpy( &BUF2, FLD_ADR, FLD_LEN );

			// If Field is not empty
			for ( i = 0; i < FLD_LEN; i++ )
			{
				// Replace 'FF' by space
				if ( BUF2[i] == 'F' )
				{
					// fill with space
					BUF2[i] = ' ';

				}
			}

			// Updating the field
			FLD_ADR = BUF2;

			// Trim leading and trailing spaces
			FLD_ADR = FMT_STRLEN( &FLD_LEN );


			break;

		default:				// Unknown format
			PCL_Error = 29;
			Status = False;		// Error status
			break;
	}
	return ( Status );			// Return the status
}


//-----------------------------------------------------------------------------
//! \brief
//!     Amount Formats
//!
//! \param
//!		[in]Format			Fortmat type
//!
//! \return
//!     False if error detected else True
//!
//! \note
//!     Error code will be put in PCL_Error.
//-----------------------------------------------------------------------------
static Bool FMT_AMT( UBYTE Format )
{
	Bool Status = True;

	switch ( Format )			// Figure out how to reformat
	{
		case FA_TRIM:			// Trim leading and trailing spaces
			FLD_ADR = FMT_STRLEN( &FLD_LEN );
			break;

		case FA_TNEG:			// See if field is not astricks
			if ( '*' != HOLDSGN )
				NegSign( FLD_ADR, FLD_LEN );	// Yes; make Amount negative

			// Trim leading and trailing spaces
			FLD_ADR = FMT_STRLEN( &FLD_LEN );
			break;

        case FA_TDR :  /* Trim leading and trailing spaces, set DR at the end */
            FLD_ADR = FMT_STRLEN( &FLD_LEN );
			memcpy( FLD_ADR+FLD_LEN, DR_String, sizeof(DR_String) );
			/* Adjust actual field length */
			FLD_LEN += sizeof(DR_String);
            break;

        case FA_TCR :  /* Trim leading and trailing spaces, set CR at the end */
            FLD_ADR = FMT_STRLEN( &FLD_LEN );
			memcpy( FLD_ADR+FLD_LEN, CR_String, sizeof(CR_String) );
			/* Adjust actual field length */
			FLD_LEN += sizeof(CR_String);
            break;

        case FA_TCRDR :  /* Trim leading and trailing spaces, set DR or CR at the end */
            FLD_ADR = FMT_STRLEN( &FLD_LEN );
			if ( 0xFF == HOLDSGN )
			{
				/* Positive Sign, set ' DR' */
				memcpy( FLD_ADR+FLD_LEN, DR_String, sizeof(DR_String) );
			}
			else
			{
				if (  '-' == *FLD_ADR )
				{
					/* Skip '-' sign */
					FLD_ADR++;
					/* Decrement length */
					FLD_LEN--;
				}
					/* Negative Sign, set ' CR' */
					memcpy( FLD_ADR+FLD_LEN, CR_String, sizeof(CR_String) );
			}
			/* Adjust actual field length */
			FLD_LEN += sizeof(DR_String);
            break;
        
		default:				// Unknown format
			PCL_Error = 29;
			Status = False;		// Error status
			break;
	}
	return ( Status );			// Return the status
}



//-----------------------------------------------------------------------------
//! \brief
//!     Number Formats
//!
//! \param
//!		[in]Format			Fortmat type
//!
//! \return
//!     False if error detected else True
//!
//! \note
//!     Error code will be put in PCL_Error.
//-----------------------------------------------------------------------------
static Bool FMT_NUM( UBYTE Format )
{
	Bool Status = True;

	switch ( Format )			// Figure out how to reformat
	{
		case FN_LDZ:			// Numeric - change leading zeroes to spaces
			LeadSpace( FLD_ADR, ( UBYTE ) ( FLD_LEN - 1 ) );
			break;

		case FN_TRIM:			// Numeric - trim leading zeroes
			// Change leading zeroes to spaces
			LeadSpace( FLD_ADR, ( UBYTE ) ( FLD_LEN - 1 ) );
			// Trim leading and trailing spaces
			FLD_ADR = FMT_STRLEN( &FLD_LEN );
			break;

		default:				// Unknown format
			PCL_Error = 29;
			Status = False;		// Error status
			break;
	}
	return ( Status );			// Return the status
}



//-----------------------------------------------------------------------------
//! \brief
//!     Date Formats
//!
//! \param
//!		[in]Format			Fortmat type
//!
//! \return
//!     False if error detected else True
//!
//! \note
//!     Error code will be put in PCL_Error.
//-----------------------------------------------------------------------------
static Bool FMT_DATE( UBYTE Format )
{
	Bool Status = True;

	switch ( Format )			// Figure out how to reformat
	{
		case FD_MDYS:			// Date - MM/DD/YY
			BUF2[0] = FLD_ADR[0];
			BUF2[1] = FLD_ADR[1];
			BUF2[2] = '/';
			BUF2[3] = FLD_ADR[2];
			BUF2[4] = FLD_ADR[3];

			// If length of the field is 4 digits then
			// there is no day field
			if ( 4 == FLD_LEN )
				FLD_LEN++;		// No Day field; increment by 1 for slash
			else
			{					// Yes Day field
				BUF2[5] = '/';
				BUF2[6] = FLD_ADR[4];
				BUF2[7] = FLD_ADR[5];
				FLD_LEN += 2;	// Increment length by 2 for slashes
			}
			FLD_ADR = BUF2;		// Change field starting address
			break;

		case FD_TEXT:			// Date - Mon DD, YY or Mon YY
			// Get the Month string
			GetMonth ( ( ( FLD_ADR[0] - 0x30 ) * 10 ) + ( FLD_ADR[1] - 0x30 ) );

			// Save the Month Text (Assumes 3 characters)
			memcpy( BUF2, CSTMSG5, ( UWORD ) 3 );

			BUF2[3] = ' ';		// Add space

			if ( FLD_LEN == 6 )	// See if DD value
			{					// Yes; add the day
				BUF2[4] = FLD_ADR[2];
				BUF2[5] = FLD_ADR[3];
				BUF2[6] = ',';
				BUF2[7] = ' ';
				BUF2[8] = FLD_ADR[4];
				BUF2[9] = FLD_ADR[5];
				FLD_LEN = 10;	// Change field length
			}
			else
			{
				BUF2[4] = FLD_ADR[4];
				BUF2[5] = FLD_ADR[5];
				FLD_LEN = 6;	// Change field length
			}
			FLD_ADR = BUF2;		// Change field starting address
			break;

		default:				// Unknown format
			PCL_Error = 29;
			Status = False;		// Error status
			break;
	}
	return ( Status );			// Return the status
}


//-----------------------------------------------------------------------------
//! \brief
//!     Time Formats
//!
//! \param
//!		[in]Format			Fortmat type
//!
//! \return
//!     False if error detected else True
//!
//! \note
//!     Error code will be put in PCL_Error.
//-----------------------------------------------------------------------------
static Bool FMT_TIME( UBYTE Format )
{
	Bool Status = True;

	switch ( Format )			// Figure out how to reformat
	{
		case FT_MIL:			// Time - Military HH:MM:SS
			BUF2[0] = FLD_ADR[0];
			BUF2[1] = FLD_ADR[1];
			BUF2[2] = ':';
			BUF2[3] = FLD_ADR[2];
			BUF2[4] = FLD_ADR[3];
			BUF2[5] = ':';
			BUF2[6] = FLD_ADR[4];
			BUF2[7] = FLD_ADR[5];
			FLD_LEN = 8;		// Change field length
			FLD_ADR = BUF2;		// Change field starting address
			break;

		default:				// Unknown format
			PCL_Error = 29;
			Status = False;		// Error status
			break;
	}
	return ( Status );			// Return the status
}


//-----------------------------------------------------------------------------
//! \brief
//!     Gets the actual start and length of the field (excluding spaces)
//!
//! \param
//!		[out]pLength		Pointer to actual length of the field
//!                         (excluding spaces)
//!
//! \return
//!     Pointer to the actual start of the field (excluding spaces)
//-----------------------------------------------------------------------------
static char *FMT_STRLEN( UBYTE * pLength )
{
	int i, j;

	// Search for the actual start of the field
	for ( i = 0; i < FLD_LEN; i++ )
	{
		// Is it non-space and non-NUL?
		if ( ( FLD_ADR[i] != ' ' ) && ( FLD_ADR[i] != '\0' ) )
			break;				// Yes; exit the loop
	}

	if ( i >= FLD_LEN )			// See if field was all spaces or NULs
	{
		*pLength = 0;			// Yes; set the actual length of the field to zero
		return ( FLD_ADR );		// Return original starting address
	}
	else
	{
		// No; search for the actual ending of the field
		for ( j = FLD_LEN - 1; j >= 0; j-- )
		{
			// Is it non-space and non-NUL?
			if ( ( FLD_ADR[j] != ' ' ) && ( FLD_ADR[j] != '\0' ) )
				break;			// Yes; exit the loop
		}
		*pLength = j - i + 1;	// Set the actual length of the field

		// Return pointer to the actual start of the field
		return ( &FLD_ADR[i] );
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Formats the card holder's name for printing
//!
//! \return
//!     None
//!
//! \note
//!     TRNAME field is filled in ACCTPROC().
//-----------------------------------------------------------------------------
static void GetName( void )
{
	UBYTE len;
	static char Tbuf[S_TRK1NAME];

	memset( FLD_ADR, ' ', S_TRK1NAME );	// Clear the buffer to spaces
	memset( ( UBYTE * ) Tbuf, 0, S_TRK1NAME );

	if ( !NullComp( ( char * ) TRINP.TRNAME, S_TRK1NAME ) )
	{
		// look for '/' which indicates we need to swap first name
		// and last name

		len = StrLnChr( ( char * ) TRINP.TRNAME, '/', S_TRK1NAME );

		if ( S_TRK1NAME > len )
		{
			// Found '/', we need to do the SWAP and remove trailing spaces
			++len;
			MovTrim( Tbuf, ( char * ) &TRINP.TRNAME[len],
					 ( UWORD ) ( S_TRK1NAME - len ) );
			len = StrLn( Tbuf, S_TRK1NAME );
			Tbuf[len++] = ' ';
			MoveTillChar( &Tbuf[len], ( char * ) TRINP.TRNAME,
						  ( UWORD ) ( S_TRK1NAME - len ), ( char ) '/' );
			memcpy( FLD_ADR, Tbuf, StrLn( Tbuf, S_TRK1NAME ) );
		}
		else
			MovTrim( FLD_ADR, ( char * ) TRINP.TRNAME, S_TRK1NAME );

	}

	// See if field larger than print line
	if ( S_TRK1NAME > ( MaxCols + ESC_number ) )
		FLD_LEN = ( MaxCols + ESC_number );		// Yes; set field length to print line width
	else
		FLD_LEN = S_TRK1NAME;	// No; set field length
}


//-----------------------------------------------------------------------------
//! \brief
//!     Processes tip
//!
//! Processes tip. For non roll, if not 2 digits print **.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void SetTip( void )
{
	UBYTE i;

	BfAscii( FLD_ADR, pFieldData, FieldLen );

	// Set the length of the Field
	FLD_LEN = FieldLen << 1;

	// If this is not a roll printer, i.e. a 23 column
	//  printer, then force tip percentage over 99% to **.
	if ( !RollPrinter )
	{
		for ( i = 0; i < ( FLD_LEN - 2 ); i++ )
		{
			if ( '0' != BUF1[i] )
			{
				// More than 2 characters of tip percentage.
				// Force tip percentage to **.
				memcpy( BUF1, ( char * ) "0000000000**", ( UWORD ) 12 );
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sets default PCL table
//!
//! \return
//!     True on success else False
//-----------------------------------------------------------------------------
Bool SetDefaultTab( void )
{

	// Locate Table Type requested.
	if ( FindFldTableType ( FX_DEFAULT ) )
	{
		return ( True );
	}

	// Error messages while testing.
	ShowErrMsg( PclTabError );

	return ( False );
}


//-----------------------------------------------------------------------------
//! \brief
//!     Sets PCL table type
//!
//! \return
//!     True on success else False
//-----------------------------------------------------------------------------
Bool SetTableType( void )
{
	TABTYPE tableType;

	tableType = GetToken(  );

	if ( PCL_Error != 0 )
	{
		return False;
	}

	// Locate the Extended Table
	if ( FindFldTableType ( tableType ) )
	{
		return ( True );
	}

	return ( False );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Sets PCL text default table
//!
//! \return
//!     True on success else False
//-----------------------------------------------------------------------------
Bool SetDefaultTxtTab( void )
{

	// Locate Table Type requested. 
	if ( FindTxtTableType ( TX_DEFAULT ) )
	{
		return ( True );
	}

	// Error messages while testing.
	ShowErrMsg( PclTabError );

	return ( False );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Sets PCL text table type
//!
//! \return
//!     True on success else False
//-----------------------------------------------------------------------------
Bool SetTxtTableType( void )
{
	ENTXTTYPE tabTxtType;

	tabTxtType = GetToken(  );

	if ( PCL_Error != 0 )
	{
		return False;
	}

	// Locate the Extended Table    
	if ( FindTxtTableType ( tabTxtType ) )
	{
		return ( True );
	}

	return ( False );
}

//-----------------------------------------------------------------------------
//! \brief
//!     Remaps characters higher than 7Fh to internal printer code page
//!
//! \param
//!		[out]pDest			Pointer to Destination Buffer
//!		[in]pSource			Pointer to the data for remaping
//!		[in]Bytes			Length of the data to be remapped
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
static void PrintRemap( void * pDest, const void * pSource, UWORD Bytes )
{
	char* ptr;
    UBYTE * pRemapTable;
	UBYTE	chr, i;
	
    // Get remap table pointer depended on current language
    pRemapTable = (UBYTE * )GetRemapTblPtr (  );

    // Check, if the remaping necessary and possible
	if ( ( TCONF.TCOPT2 & TC2_REMAP ) && ( NULL != pRemapTable ) )
	{

        while ( Bytes )
		{
			/* Get next character */
			chr = *(UBYTE*)pSource;

			/* Check, if the character higher 0x7F */
#ifdef MAKE_FARSI
            // allow to remap all the characters
            if( LOCTAB.LANGID == LNG_FA )
#else
            if ( chr & 0x80 )
#endif
			{
				for ( i = 0; i < 0x80; i+=2 )
				{
                    // Check end of remap table
                    if ( 0x00 == pRemapTable[i] )
                    {
                        break;
                    }

					// Get proper character from remaping table
					if ( chr == pRemapTable[i] )
					{
						chr = pRemapTable[i + 1];
						break;
					}
				}
			}

			/* Copy character to destination */
			*(UBYTE*)pDest = chr;

			/* Update buffer pointers */
			ptr = (char*)pSource;
			pSource = ptr + 1;
			ptr = (char*)pDest;
			pDest = ptr + 1;
	
			/* Decrement data size */
			Bytes--;
		}
	}
	else
	{
		/* Move data without remaping */
		memcpy( pDest, pSource, Bytes );
	}
}

//-----------------------------------------------------------------------------
//! \brief
//!     Prints Firware
//!
//! \return
//!     True on success else False
//-----------------------------------------------------------------------------
extern Bool PrintFirmware( void )
{
	const char FirmwareCommand[] = { 0x1B, 'I' };
	Bool retval;

	retval = PrintItem( (char *)FirmwareCommand, 
						sizeof( FirmwareCommand ) );

	if ( ( retval ) && ( PrinterTable[TCONF.TCPRINTER].Flags & PR1_CUT ) )
		SDK_Wait( 2 * ONESECOND );

	return retval;
}

