/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef LPTDEV_INC
#define LPTDEV_INC

//
// CONFIG Mode Equates (IOCSMODE)
//
//                                 Mechanism   # of     Font  Feed Print     Supports
//                                 Type        Columns  Size  Type Direction Graphics
//                                 -------------------------------------------------
#if defined(MAKE_OPTIMUM)

#define LPT_23C_E21X    0x00    // E212/215 P7   23      5x7  Sp   Uni-      No
#define LPT_31C_E215    0x01    // E215     P7   31      7x7  Sp   Bi-       No
#define LPT_33C_E211    0x03    // E211     P7   33      5x7  Fr   Uni-      No
#define LPT_44C_E211    0x04    // E211     P7   44      7x7  Fr   Uni-      No
#define LPT_XXX_XXXX    0x05    // -- backward compatibility E212/E215 5x7 --
#define LPT_E19X        0x06    // E190/192 T7P 24/40    5x7  Fr   Uni-      Yes
#define LPT_S212        0x07    // S212     P8  35/46  5/7x7  Fr   Bi-       No
#define LPT_S221        0x08    // S221     P8  23/31  5/7x7  Sp   Bi-       No
#define LPT_S212G       0x0B    // S212G    P8G 35/46  5/7x7  Fr   Bi-       Yes
#define LPT_S221G       0x0C    // S221G    P8G 23/31  5/7x7  Sp   Bi-       Yes
#define LPT_S212G_U     0x0D    // S212G    P8G 35/46  5/7x7  Fr  *see note  Yes
#define LPT_S221G_U     0x0E    // S221G    P8G 23/31  5/7x7  Sp  *see note  Yes
#define LPT_MT101       0x0F    // EM-T101  P?  42       5x7  Fr   Uni-      Yes
#define LPT_MT301       0x11    // EM-T301  P?  44       5x7  Fr   Uni-      Yes
#define LPT_ADP500      0x12    // ADP-500  T77+ 24      5x7  Sp   Bi-       Yes
#define LPT_FTP638      0x13    // FTP-638  T7+               Fr   Thermal   Yes
#define LPT_U111S       0x14    // M-U111S  T77+ 20      5x7  Sp   Bi-       Yes
#define LPT_FTP628      0x15    // FTP-628  T7+  42      8x7  Fr   Thermal   Yes
#define LPT_LTPJ245     0x16    // M2

#elif defined(MAKE_VIKING)

#define LPT_FTP628	 0x17	// FTP628   p?	42       8x7  Fr   Thermal   Yes

#endif

//
// Feed Type: "Sp"rocket , "Fr"iction
//
//
//  Used with the ESC 'M' command to control the printing mode for the ADP-500 Sprocket Printer
#define BIDIRECTIONAL   0x00
#define UNIDIRECTIONAL  0x01
//
// * Note Regarding Print Direction
// --------------------------------
// The P8G printers can suffer from vertical dot misalignment between
// rows by up to +/- 1 dot.  When printing text, this is acceptable.
// When printing graphics, however, it can result in a less than
// acceptable image.  Because this misalignment value varies from one
// printer sample to the next, the application may prefer printing
// graphics in unidirectional mode (that is, left to right) instead of
// the normal bidirectional mode.  This will result in slower printing
// during graphics, but is necessary to achieve a sharp graphics image.
// The two CONFIG modes, LPT_S212G_U & LPT_S221G_U provide bidirectional
// text and unidirectional graphics printing.
//
// Other General Purpose CONFIG Commands:
//
#if defined(MAKE_OPTIMUM)

#define LPT_REQPS       0x20    // Request Paper Out Status
#define LPT_REQTS       0x21    // Request Temperature Status
#define LPT_REQ_T77PLUS 0x22    // Request LPT Printer Type (valid for T77 Plus only)
#define LPT_REQ_T7P     0x23    // Request Current LPT Type 
#define LPT_ALOAD_ENA   0x24    // Enable Auto Paper Load Feature
#define LPT_ALOAD_DIS   0x25    // Disable Auto Paper Load Feature
#define LPT_RAWMODE_ENA 0x26    // Enable "raw" mode (smart printers only)
#define LPT_RAWMODE_DIS 0x27    // Disable "raw" mode (smart printers only)
#define LPT_REQLAST_ERR 0x28    // Request the last error detected by the LPT DRIVER (T77Plus only)
#define LPT_FWDFEED     0x29    // Forward Feed the Paper for the T77Plus Sprocket Printer
#define LPT_REVFEED     0x2A    // Reverse Feed the Paper for the T77Plus Sprocket Printer

#elif defined(MAKE_VIKING)

#define	LPT_REQPS	0x09 	// Request Paper Out Status
#define	LPT_REQTS	0x0A 	// Request Temperature Status
#define	LPT_REQ_T7P	0x10 	// Request Current LPT Type (valid for T7P only)
#define	LPT_REQ_MODE	0x13 	// Request current LPT type(mode)
#define	LPT_PAPER_FEED 	0x14 	// Set the paper feed(LF) softkey flag
#define	LPT_PAPER_CUT 	0x15 	// Set the paper cut softkey flag
#define LPT_RAWMODE_ENA 0x16	// Enable "raw" mode (smart printers only)
#define LPT_RAWMODE_DIS 0x18	// Disable "raw" mode (smart printers only)
#define LPT_AUTORESET_ENA 0x19    // Autoreset mode enable (default)
#define LPT_AUTORESET_DIS 0x1A    // Autoreset mode disable

#endif

//
// ===========================================================================
//
// CONFIG Mode Additional Data (IOCSDDDA)
//
// All CONFIG Modes have no parameters.
//
// ===========================================================================
//
// CONFIG Mode Returned Data (IOCSSTAT)
//
// LPT_REQPS returns the Paper Out Status in IOCSSTAT:
//
#define LPT_PAPER_OK    0x00    // paper is loaded
#define LPT_PAPER_LOW   0x01    // paper is low (not yet supported by H/W)
#define LPT_PAPER_OUT   0x02    // paper is empty

//
// LPT_REQTS returns the Printhead Temperature Status in IOCSSTAT:
//
#define LPT_TEMP_OK     0x00    // temperature is OK, normal printing allowed
#define LPT_TEMP_HOT    0x01    // temperature is HOT, printer is paused

// LPT_REQ_T7P queries the T7P hardware and returns the Current LPT Type
// in IOCSMODE+1 using the equates found in TERMPRINT (see TMP_xxx in
// sysstruc.inc).  If the config command is issued on hardware other than
// a T7P, a "Device Not Available" error (ERRDNA) will be returned.
//
// IOCSSTAT -> 1 Byte  - Status of LPT_REQ_T7P Config Request
//   DONEOK    = Config Request Successful
//   ERRDNA    = Not a T7P platform or unknown printer type
//   ERRMNS    = Mode Not Supported
//
// IOCSMODE+1 -> 1 Byte  - LPT Type
//   TMP_E190  = E-190
//   TMP_T7P   = E-192
//   TMP_MT101 = MT-101
//
//
// LPT_REQ_T7PLUS queries the T77PLUS hardware and returns the Current LPT Type
// in IOCSMODE+1 using the equates found in TERMPRINT (see TMP_xxx in
// sysstruc.inc).  If the config command is issued on hardware other than
// a T77PLUS, a "Device Not Available" error (ERRDNA) will be returned.
//
// IOCSSTAT -> 1 Byte  - Status of LPT_REQ_T77PLUS Config Request
//   DONEOK    = Config Request Successful
//   ERRDNA    = Not a T77PLUS platform or unknown printer type
//   ERRMNS    = Mode Not Supported
//
// IOCSMODE+1 -> 1 Byte  - LPT Type
//   TMP_ADP500 = SeWoo ADP-500 Sprocket Printer
//
// All other CONFIG Modes do not return any special data.
//
// ===========================================================================



// -------------------------------------------------------------
// LPT_REQLAST_ERR Config Command :
//
// Command Inputs :
//      None
//
// Command Outputs :
//
// IOCSSTAT -> 1 Byte  - Status of 'Request last error configuration command' 
//
#define EC_NOERROR     0x00   // No Error at the Print Controller (OK)
#define EC_PAPEROUT    0x01   // Paper Out Error
#define EC_JAMDETECTED 0x02   // Carriage/Head Jam detected
#define EC_HIGHVOLTAGE 0x03   // Voltage is too high at the Print Controller (wrong power supply)
#define EC_LOWVOLTAGE  0x04   // Voltage is too low at the Print Controller
#define EC_OVERHEAT    0x05   // Printer is overheated
#define EC_TIMEOUT     0xFF   // Time out from the Printer Controller (does not support extended errors)
//
//
// This CONFIG Command is used get the last Printer Controller Error for
//  the T77Plus ADP-500 Sprocket Printer.
//
//
// -------------------------------------------------------------
// LPT_FWDFEED Config Command :
//
// Command Inputs :
//      None
//
// Command Outputs :
//
// IOCSSTAT -> 1 Byte  - OK
//
// This CONFIG Command is used forward feed the paper in the sprocket printer
//  while the paper is being inserted (paperout condition).
//
// -------------------------------------------------------------
// LPT_REVFEED Config Command :
//
// Command Inputs :
//      None
//
// Command Outputs :
//
// IOCSSTAT -> 1 Byte  - OK
//
// This CONFIG Command is used forward feed the paper in the sprocket printer
//  while the paper is being inserted (paperout condition).
//

#endif
