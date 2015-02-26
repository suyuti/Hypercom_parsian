/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef _DSPDEV_INC
#define _DSPDEV_INC

/*
; The DSP Driver supports the following LCD Displays with the indicated
; display parameters.
;
;    Color 1/4 VGA Graphics Mode Only LCD Display
;
;                     Text Mode        Graphics Mode
;                  (5x7 Characters)   (Columns, Rows)
;                  ----------------   ---------------
;    Color 1/4 VGA       53x30            320x240
;
;
; The DSP Driver supports the following Display Commands (via a Write Request)
; for updating the LCD Displays.
;
;	DSP_CLR_DISPLAY - Clear All of the LCD Display
;			     (blank the display)
;			     (for Color Displays, updates All of the Display
;			      with the BackGround Color)
;	DSP_UPDATE_ALL	- Update All of the LCD Display
;	DSP_UPDATE_PART	- Update Part of the LCD Display
;	DSP_PCX_8BIT	- Update LCD Display with the PCX Format
;			  8-Bits / Pixel Color Image
;
; ------------------------------
; DSP_UPDATE_ALL & DSP_UPDATE_PART Display Commands
;
; For the Update LCD Display Commands (DSP_UPDATE_ALL, DSP_UPDATE_PART) the
; DSP Driver uses the contents of the Display Screen Buffer (DSP_SCREEN) to
; update the LCD Display.
;
; For Color LCD Displays, the Pixels in the Display Screen Buffer are
; translated as follows when they are displayed on the Color Display.
;
;     A "1" Pixel (bit set)   is set to the ForeGround Color
;     A "0" Pixel (bit clear) is set to the BackGround Color
;
;     If the Transparent Background Option is Enabled, then a "0" Pixel
;     (bit clear) causes the Display Pixel to remain unchanged.
;
; The Application is responsible for changing the Display Screen Buffer
; (DSP_SCREEN) to contain the Pixel Data to be displayed on the LCD Display
; by the DSP Driver.
; The Application should use the GUI Library Utilitites to change the contents
; of the Display Screen Buffer (DSP_SCREEN).
; 
; The DSP Driver expects the Pixel Data in the Display Screen Buffer
; (DSP_SCREEN) to have the following data format.
;
;	Each Row of Column Pixel Data starts on a Byte Boundary.  If the
;	Number of Columns / Row is not a multiple of 8, then the last byte
;	in that Row contains filler bits to make the next Row start on a
;	Byte boundary (the filler bits should be zero).
;
;	Each Row of Column Pixel Data is Left justified within the Data
;	Bytes of each Row.  Bit 7 of each Row's 1st Data Byte is the
;	Left Most Column's Data and the Right Most Column's Data is in
;	each Row's Last Data Byte.  The Pixel data is also Left justfifed
;	within each Data Byte, bit 7 is the Left Most Column's Data and
;	bit 0 is the Right Most Column's Data.
;
;	For example if the LCD Display was 20 Columns by 5 Rows, the Display
;	Screen Buffer would look as follows:
;
;		Columns 0-7	Columns 8-15	Columns 16-19
;
;	Row 0	DSP_SCREEN+0	DSP_SCREEN+1	DSP_SCREEN+2
;		8 bits (7-0)	8 bits (7-0)	4 bits (7-4)
;
;	Row 1	DSP_SCREEN+3	DSP_SCREEN+4	DSP_SCREEN+5
;		8 bits (7-0)	8 bits (7-0)	4 bits (7-4)
;
;	Row 2	DSP_SCREEN+6	DSP_SCREEN+7	DSP_SCREEN+8
;		8 bits (7-0)	8 bits (7-0)	4 bits (7-4)
;
;	Row 3	DSP_SCREEN+9	DSP_SCREEN+10	DSP_SCREEN+11
;		8 bits (7-0)	8 bits (7-0)	4 bits (7-4)
;
;	Row 4	DSP_SCREEN+12	DSP_SCREEN+13	DSP_SCREEN+14
;		8 bits (7-0)	8 bits (7-0)	4 bits (7-4)
;
; ------------------------------
; DSP_PCX_8BIT Display Command
;
; For the Update LCD Display with the PCX Format 8-Bits / Pixel Color Image
; (DSP_PCX_8BIT) the DSP Driver uses the PCX Image Data pointed to by this
; Display Command to update the LCD Display.
;
; The Update LCD Display with PCX Format Image Data Display Command doesn't
; use the Image Data contained in the Display Screen Buffer (DSP_SCREEN) but
; instead uses the PCX Format Image Data pointed to by this Display Command.
;
; ---------------------------------------------------------------------
;
; Format of DSP_CLR_DISPLAY Command
; ----------------------------------
; 1 byte  - Clear All of the LCD Display Command (DSP_CLR_DISPLAY)
;
;
; Format of DSP_UPDATE_ALL Command
; ---------------------------------
; 1 byte  - Update All of the LCD Display Command (DSP_UPDATE_ALL)
;
;
; Format of DSP_UPDATE_PART Command
; ----------------------------------
; 1 byte  - Update Part of the LCD Display Command (DSP_UPDATE_PART)
;
; 2 bytes - Starting Row Number
;
; 2 bytes - Starting Column Number
;
; 2 bytes - Number of Columns / Row
;
; 2 bytes - Number of Rows to Update
;
;
; Format of DSP_PCX_8BIT Command
; -------------------------------
; 1 byte  - Update LCD Display with the PCX Format 8-Bits / Pixel Color
;	    Image Command (DSP_PCX_8BIT)
;
; 2 bytes - Starting Row Number
;
; 2 bytes - Starting Column Number
;
; 1 byte  - Page Mask of Page where PCX Image exists
;
; 2 bytes - Start Address of the PCX Image Data
;
;
; ===========================================================================
;
; Display Command Equates (used in Write Requests to the DSP Driver)
;
*/
#define DSP_CLR_DISPLAY		0x00	//; Clear All of the LCD Display
 					//; (blank the display)
#define DSP_UPDATE_ALL		0x01	//; Update All of the LCD Display
#define DSP_UPDATE_PART		0x02	//; Update Part of the LCD Display
#define DSP_PCX_8BIT		0x03	//; Update LCD Display with the PCX Format
	 				//; 8-Bits / Pixel Color Image
#define DSP_PUTBLOCK        	0x04    //;
#define DSP_GETBLOCK        	0x05    //;
#define DSP_MOVBLOCK        	0x06    //;
#define DSP_GETBITMAP       	0x07    //;
#define DSP_PUTBITMAP       	0x08    //;


//;
//; ===========================================================================
//;
//; CONFIG Mode Equates (IOCSMODE)
//;
#define DSP_REQ_DSPTYPE    	0x00	//; Request Display Type
#define DSP_SETCONTRAST    	0x01	//; Set Display Contrast value
#define DSP_BACKLIGHT_ON   	0x02	//; Turn Backlight On
#define DSP_BACKLIGHT_OFF  	0x03	//; Turn Backlight Off
//;		   EQU	0x04		//; spare
#define DSP_SET_COLORS     	0x05	//; Set the Active ForeGround & BackGround
					//; Colors and Color Option Bits
#define DSP_GET_COLORS     	0x06	//; Get the Active ForeGround & BackGround
					//; Colors and Color Option Bits
#define DSP_SET_COLOR_LUT  	0x07	//; Set Color Look Up Table Palette
#define DSP_GET_COLOR_LUT  	0x08	//; Get Color Look Up Table Palette
#define DSP_SET_UPD_VPAGE  	0x09	//; Set Update Video Page
#define DSP_GET_UPD_VPAGE  	0x0A	//; Get Update Video Page
#define DSP_SET_VIS_VPAGE  	0x0B	//; Set Visible Video Page
#define DSP_GET_VIS_VPAGE  	0x0C	//; Get Visible Video Page
//;
//; ===========================================================================
//;
//; CONFIG Command Equates :
//;
//; -------------------------------------------------------------
//; Enumerated types for DSP_SET_COLORS & DSP_GET_COLORS Config Commands:
//;
//; Structure for Active Display Colors Record :
//;
#ifdef C_DEV
typedef struct
{
	UBYTE	DC_ForeGround;	// Active ForeGround Color
	UBYTE	DC_BackGround;	// Active BackGround Color
	UBYTE	DC_Option;	// Active Color Option flags
} DCOLOR;
		// Size of Active Display Colors Record
#define DCOLORSZE sizeof( DCOLOR )
#else
DCOLOR STRUCT
	RESV	DC_FOREGROUND,1	; Active ForeGround Color
	RESV	DC_BACKGROUND,1	; Active BackGround Color
	RESV	DC_OPTION,1	; Active Color Option flags
	ENDSTRUCT
DCOLORSZE EQU	`SIZEOF(DCOLOR)	; Size of Active Display Colors Record
#endif
//;
//;
//;	Note:	All Spare Color Option flags must be set to 0.
//;		By defaulting the spare flags to 0, existing Application code
//;		will still work correctly when new Option flags are defined
//;		in the future.
//;
//; Color Option Flags Equates :
#define DCOPT_TRANSBACK	 0		//; Bit 0 - Transparent Background flag
					//;	  (Bit = 1, Enables the Transparent
					//;	   Background feature)
#define DCOPT_BIT1	 1		//; Bit 1 - spare
#define DCOPT_BIT2	 2		//; Bit 2 - spare
#define DCOPT_BIT3	 3		//; Bit 3 - spare
#define DCOPT_BIT4	 4		//; Bit 4 - spare
#define DCOPT_BIT5	 5		//; Bit 5 - spare
#define DCOPT_BIT6	 6		//; Bit 6 - spare
#define DCOPT_BIT7	 7		//; Bit 7 - spare
//;
//;
#define DSP_BLACK	 0x00	//; Black, All Colors Off
#define DSP_WHITE	 0xFF	//; White, All Colors On Maximum
//;
#define DSP_RED7	 0xE0	//; Red, Maximum Level
#define DSP_RED1	 0x20	//; Red, Minimum Level
#define DSP_NO_RED	 0x00	//; Red, Turned Off
//;
#define DSP_GREEN7	 0x1C	//; Green, Maximum Level
#define DSP_GREEN1	 0x04	//; Green, Minimum Level
#define DSP_NO_GREEN	 0x00	//; Green, Turned Off
//;
#define DSP_BLUE3	 0x03	//; Blue, Maximum Level
#define DSP_BLUE1	 0x01	//; Blue, Minimum Level
#define DSP_NO_BLUE	 0x00	//; Blue, Turned Off
//;
//; -------------------------------------------------------------
//; Enumerated types for DSP_SET_COLOR_LUT & DSP_GET_COLOR_LUT Config Commands:
//;
//; Structure for Color LUT Command Record :
//;
#ifdef C_DEV
typedef struct
{
	UWORD	LUTC_Address;	// Start Address of Color LUT Buffer
	UWORD	LUTC_Length;	// Length of Color LUT Buffer
	UBYTE	LUTC_PageMask;	// Page Mask of Page where Color LUT Buffer
				// exists
} LUTCMD;
		// Size of Color LUT Command Record
#define LUTCMDSZE sizeof( LUTCMD )

#else
LUTCMD STRUCT
	RESV	LUTC_ADDRESS,2	; Start Address of Color LUT Buffer
	RESV	LUTC_LENGTH,2	; Length of Color LUT Buffer
	RESV	LUTC_PAGEMASK,1	; Page Mask of Page where Color LUT Buffer
				; exists
	ENDSTRUCT
LUTCMDSZE EQU	`SIZEOF(LUTCMD)	; Size of Color LUT Command Record
#endif
//;
//; ===========================================================================
//;
//; CONFIG Return Status Equates (IOCSSTAT) :
//;
//; -------------------------------------------------------------
//; Enumerated types for DSP_REQ_DSPTYPE config mode response :
//;
//; Display Type (IOCSSTAT) :
//;

#ifdef MAKE_OPTIMUM

// Macros definition for MAKE_T7PLUS display
	#define DSP_2LINE	0x00	//; Text Only 2x20 Display
					//;   (Display has Built-In Text Font)
	#define DSP_GRAPHICS	0x01	//; Text & Graphics 128x64 Display (8 Line)
					//;   (Display has Built-In Text Font)
	#define DSP_GPH4LINE	0x02	//; Graphics Only 140x32 Display (4 Line)
					//;   (OS supplies Text Font)
	#define DSP_4LINE	0x03	//; Text Only 4x20 Display
					//;   (Display has Built-In Text Font)
	#define DSP_ST7920	0x04	//; Text & Graphics 140x32 Display
					//;   (Display has Built-In Chinese Text Font)
#endif // MAKE_OPTIMUM

// Macros definition for all ICE terminals

#define DSP_NONE         0x00        //; Display does Not Exist
#define DSP_GPH10LINE    0x03        //; Graphics Only 160x80 Display (10 Line)
#define DSP_GPH20LINE    0x04        //; Graphics Only 160x160 Display (20 Line)
#define DSP_CQVGA	 0x05        //; Color 1/4 VGA Graphics Only 320x240
			     	     //; Display (30 Line)
/*
;
; ===========================================================================
;
; CONFIG Command Usage :
;
; -------------------------------------------------------------
; DSP_REQ_DSPTYPE Config Command :
;
; Command Inputs :
;
; none
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Type of Display detected by the DSP Driver
;			  DSP_NONE      = Display does Not Exist
;			  DSP_CQVGA     = Color 1/4 VGA Graphics Only 320x240
;					  Display (30 Line)
;
; This CONFIG Command is used to get the Type of Display that is connected to
; the T6000 Device.
;
; -------------------------------------------------------------
; DSP_SETCONTRAST Config Command :
;
; Command Inputs :
;
; IOCSDDDA -> 1 Byte  - Desired Display Contrast (0..255)
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Status of Updating the Display Contrast
;                         Always returns OK
;
; This CONFIG Command is used to set the Display Contrast.
;
; -------------------------------------------------------------
; DSP_BACKLIGHT_ON Config Command :
;
; Command Inputs :
;
; none
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Status of turning the Display Backlight On
;                         Always returns OK
;
; This CONFIG Command is used to turn the Display Backlight On.
;
; -------------------------------------------------------------
; DSP_BACKLIGHT_OFF Config Command :
;
; Command Inputs :
;
; none
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Status of turning the Display Backlight Off
;                         Always returns OK
;
; This CONFIG Command is used to turn the Display Backlight Off.
;
; -------------------------------------------------------------
; DSP_SET_COLORS Config Command :
;
; Command Inputs :
;
; IOCSDDDA -> 2 Bytes - Pointer to the Desired Display Colors and Display
;			Color Option flags defined in a 3 byte Active
;			Display Colors Record
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Status of Setting the Active Display Colors
;                         Always returns OK
;
; This CONFIG Command is used to set the Active ForeGround and BackGround
; Colors and the Active Color Option flags for a Color Display.
; This Config Command only works with Color LCD Displays.
;
; The Application passes the desired Active Display Color information to
; the DSP Driver in the Active Display Colors Record pointed to by this
; Config Command.
; The Active Display Colors Record has the following format.
;
;	1 Byte - Desired ForeGround Color (0..255)
;	1 Byte - Desired BackGround Color (0..255)
;	1 Byte - Desired Display Color Option flags (8 bits)
;		  Bit 0 - Transparent Background flag (DCOPT_TRANSBACK)
;		  Bit 1 - spare (must be 0)
;		  Bit 2 - spare (must be 0)
;		  Bit 3 - spare (must be 0)
;		  Bit 4 - spare (must be 0)
;		  Bit 5 - spare (must be 0)
;		  Bit 6 - spare (must be 0)
;		  Bit 7 - spare (must be 0)
;
;
; The Transparent Background flag (DCOPT_TRANSBACK) is used to Enable /
; Disable the Transparent Background feature of the DSP Driver.  When the
; flag is set to zero, the Transparent Background feature is disabled
; (this is the default power up state for this flag).  When the flag is
; set to one, the Transparent Background feature is enabled.
;
; When the Transparent Background feature is disabled, the DSP Driver will
; change the Display Pixels to the BackGround Color when processing the
; BackGround Pixels (a zero pixel) in the Display Screen Buffer (BITMAP).
;
; When the Transparent Background feature is enabled, the DSP Driver will
; not change the Display Pixels to the BackGround Color but will leave the
; Pixels unchanged when processing the BackGround Pixels (a zero pixel) in
; the Display Screen Buffer (DSP_SCREEN).
;
; The Transparent Background feature only affects the Update LCD Display
; Commands (DSP_UPDATE_ALL, DSP_UPDATE_PART) where the DSP Driver uses the
; contents of the Display Screen Buffer (DSP_SCREEN) to update the LCD
; Display.
;
; -------------------------------------------------------------
; DSP_GET_COLORS Config Command :
;
; Command Inputs :
;
; IOCSDDDA -> 2 Bytes - Pointer to the Destination Buffer to get the 3 byte
;			Active Display Colors Record Information
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Status of Getting the Active Display Colors
;                         Always returns OK
;
; This CONFIG Command is used to get the Active ForeGround and BackGround
; Colors and the Active Color Option flags defined for a Color Display.
; This Config Command only works with Color LCD Displays.
;
; The DSP Driver returns the Active Display Color information in the Active
; Display Colors Record pointed to by this Config Command.  The Active
; Display Colors Record has the following format.
;
;	1 Byte - Active ForeGround Color (0..255)
;	1 Byte - Active BackGround Color (0..255)
;	1 Byte - Active Display Color Option flags (8 bits)
;		  Bit 0 - Transparent Background flag (DCOPT_TRANSBACK)
;		  Bit 1 - spare (should be 0)
;		  Bit 2 - spare (should be 0)
;		  Bit 3 - spare (should be 0)
;		  Bit 4 - spare (should be 0)
;		  Bit 5 - spare (should be 0)
;		  Bit 6 - spare (should be 0)
;		  Bit 7 - spare (should be 0)
;
; -------------------------------------------------------------
; DSP_SET_COLOR_LUT Config Command :
;
; Command Inputs :
;
; IOCSDDDA -> 2 Bytes - Pointer to the 5 Byte Desired Color LUT Command Record
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Status of Setting the Color Look Up Table Palette
;                         Always returns OK
;
; This CONFIG Command is used to set the Active Color Look Up Table Palette
; for a Color Display.
; This Config Command only works with Color LCD Displays.
;
; The Application passes the desired Active Color Look Up Table information
; to the DSP Driver in the Color LUT Command Record pointed to by this
; Config Command.
; The Color LUT Command Record has the following format.
;
;	2 Bytes - Start Address of Color LUT Buffer
;	2 Bytes - Length of Color LUT Buffer
;	1 Byte  - Page Mask of Page where Color LUT Buffer exists
;
;
; The Epson 1354 Video Chip uses a 20 Bytes Look Up Table Palette formatted
; as follows:
;	8 Bytes - Red   Color Palette
;	8 Bytes - Green Color Palette
;	4 Bytes - Blue  Color Palette
;
; -------------------------------------------------------------
; DSP_GET_COLOR_LUT Config Command :
;
; Command Inputs :
;
; IOCSDDDA -> 2 Bytes - Pointer to the 5 Byte Color LUT Command Record that
;			points to the Destination Buffer to get the Color
;			Look Up Table Palette
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Status of Getting the Color Look Up Table Palette
;                         Always returns OK
;
; This CONFIG Command is used to get the Active Color Look Up Table Palette
; for a Color Display.
; This Config Command only works with Color LCD Displays.
;
; The DPS Driver returns the Active Color Look Up Table information to the
; Application in the Destination Buffer specified in the Color LUT Command
; Record pointed to by this Config Command.
; The Color LUT Command Record has the following format.
;
;	2 Bytes - Start Address of Color LUT Destination Buffer
;	2 Bytes - Length of Color LUT Destination Buffer
;	1 Byte  - Page Mask of Page where Color LUT Buffer exists
;
;
; The Epson 1354 Video Chip uses a 20 Bytes Look Up Table Palette formatted
; as follows:
;	8 Bytes - Red   Color Palette
;	8 Bytes - Green Color Palette
;	4 Bytes - Blue  Color Palette
;
; -------------------------------------------------------------
; DSP_SET_UPD_VPAGE Config Command :
;
; Command Inputs :
;
; IOCSDDDA -> 1 Byte  - Desired Update Video Page (0..15)
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Status of Setting the Update Video Page
;			  OK     = Update Video Page was successfully set
;			  ERRINV = Invalid Desired Video Page Error
;
; This CONFIG Command is used to select the Update Video Memory Page.  The
; Update Video Memory Page is the Video Memory Page that the DSP Driver
; will modify when it receives one of the Update Display Commands
; (DSP_CLR_DISPLAY, DSP_UPDATE_ALL, DSP_UPDATE_PART, DSP_PCX_8BIT).
;
; -------------------------------------------------------------
; DSP_GET_UPD_VPAGE Config Command :
;
; Command Inputs :
;
; None
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Current Update Video Page (0..15)
;
; This CONFIG Command is used to get the current Update Video Memory Page.
;
; -------------------------------------------------------------
; DSP_SET_VIS_VPAGE Config Command :
;
; Command Inputs :
;
; IOCSDDDA -> 1 Byte  - Desired Visible Video Page (0..15)
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Status of Setting the Visible Video Page
;			  OK     = Update Video Page was successfully set
;			  ERRINV = Invalid Desired Video Page Error
;
; This CONFIG Command is used to select the Visible Video Memory Page.  The
; Visible Video Memory Page is the Video Memory Page that is currently being
; displayed on the LCD Display.
; The Visible Video Memory Page can be the same as the Update Video Memory
; Page, but it can also be a different Video Memory Page.
;
; -------------------------------------------------------------
; DSP_GET_VIS_VPAGE Config Command :
;
; Command Inputs :
;
; None
;
; Command Outputs :
;
; IOCSSTAT -> 1 Byte  - Current Visible Video Page (0..15)
;
; This CONFIG Command is used to get the current Visible Video Memory Page.
;
; ===========================================================================
*/

#endif //; _DSPDEV_INC
