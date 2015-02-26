
#ifndef _PRTTABLE_H_
#define _PRTTABLE_H_
//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             prttable.h
//      Printer table header file
//
//=============================================================================


//=============================================================================
// Public defines and typedefs
//=============================================================================
// Option flags for PrinterTable.Flags
#define PR1_ROLL  0x02			// Printer is a roll printer
#define PR1_THERM 0x04			// Printer is a thermal printer
#define PR1_CUT	  0x08			// Printer has cutter
#define PR1_LF	  0x10			// Printer automatically does LF when line full
#define PR1_GRPH  0x20			// Printer is capable of graphics

struct PrintTableStruct
{
	Bool Supported;				// Is it supported on this platform flag?
	UBYTE Font0width;			// Line width with normal font
	UBYTE Font1width;			// Line width with condensed font (if any)
	UBYTE DefaultWidth;			// Default line width
	UBYTE Flags;				// Printer options
};


//=============================================================================
// Public data declarations
//=============================================================================
extern const struct PrintTableStruct PrinterTable[];

//=============================================================================
// Public function declarations
//=============================================================================
extern void CheckPrinterType( void );


#endif // _PRTTABLE_H_
