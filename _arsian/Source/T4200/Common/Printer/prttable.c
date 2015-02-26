//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================
//
// Module overview:             prttable.c
//      Printer Table.
//
//=============================================================================
#include "basictyp.h"
#include "sdk.h"
#include "sdkos.h"
#include "sdkio.h"

#include "struct.h"
#include "comdata.h"
#include "tconf.h"

#include "prttable.h"

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

// *INDENT-OFF*

const struct PrintTableStruct PrinterTable[] = {
    //! P7 EPSON 212
	{ False,	23, 31, 23, PR1_LF										},
    //! P7 EPSON 215
	{ False,	23, 31, 23, PR1_LF										},
    //! P7 EPSON 211
	{ False,	33, 44, 40, PR1_ROLL | PR1_LF							},
    //! VERIFONE
	{ False,	40,  0, 40, PR1_ROLL									},
    //! T7P EPSON 192
	{ False,	40,  0, 40, PR1_ROLL | PR1_LF							},
    //! T7P EPSON 190
	{ False,	23,  0, 23, PR1_ROLL | PR1_LF							},
    //! P8 STAR 212
	{ False,	35, 46, 40, PR1_ROLL | PR1_LF							},
    //! P8 STAR 221
	{ False,	23, 31, 23, PR1_LF										},
    //! Not supported by SPOS - P8 Star 212 Graphics
	{ False,	 0,  0,  0, 0											},
    //! Not supported by SPOS - P8 Star 221 Graphics
	{ False,	 0,  0,  0, 0											},
    //! T7P EPSON MT101
	{ False,	42,  0, 40, PR1_ROLL | PR1_THERM | PR1_GRPH				},
    //! T77 EPSON MT301
	{ False,	44,  0, 40, PR1_ROLL | PR1_THERM | PR1_CUT | PR1_GRPH	},
    //! Not supported by SPOS - P8 Star 212 Graphics
	{ False,	 0,  0,  0, 0											},
    //! Not supported by SPOS - P8 Star 212 Graphics
	{ False,	 0,  0,  0, 0											},
    //! FTP628 for ICE5500plus/T7Plus
	{ True,		42,  0, 40, PR1_ROLL | PR1_THERM | PR1_GRPH				},
    //! Not supported by SPOS - ADP500 for T77Plus
	{ False,	 0,  0,  0, 0                                           }
};

// *INDENT-ON*

//=============================================================================
// Private data definitions
//=============================================================================


//=============================================================================
// Public function definitions
//=============================================================================

//-----------------------------------------------------------------------------
//! \brief
//!     Validates the Printer Type
//!
//! Validates the Printer Type. If invalid, it will replace with default printer.
//!
//! \return
//!     None
//-----------------------------------------------------------------------------
extern void CheckPrinterType( void )
{
    // Force Optimum printer type and enable it
	TCONF.TCPRINTER = prt_FTP628; // 
    TCONF.TCOPT2 |= TC2_PRINT;

	// Set the Checksum (LRC) for the Terminal Configuration Table
	SetTCTabLRC(  );
}

//=============================================================================
// Private function definitions
//=============================================================================

