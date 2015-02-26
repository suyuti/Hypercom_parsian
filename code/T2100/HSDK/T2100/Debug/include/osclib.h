/********************************************************************
 *                            Hypercom Inc                          *
 *                        (c) Copyright 2006                        *
 ********************************************************************/

#ifndef _osclib_H_
#define _osclib_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "osopts.inc"
#include <string.h>
#include <Types.h>
#include <time.h>
#include <keys.h>
#if !defined(MAKE_D2100) && !defined(MAKE_H2200)
#include <osgui.h>
#endif
#include "execdef.inc"

/**
	@addtogroup GroupOSC OS C Library Public Interface
	@{
*/

//=============================================================================
// Public defines and typedefs
//=============================================================================

// Define flat memory model if not defined yet
#if !defined(MAKE_FLAT) && (defined(MAKE_VIKING) || defined(MAKE_TURBO500))
#define MAKE_FLAT
#endif

//! \brief Possible input types returned by OS_GetUserInput() and OS_CheckUserInput().
typedef enum
{
    USERINP_NONE,       // No input available
    USERINP_TIMEOUT,    // Timed out while waiting for input
    USERINP_KEYPAD,     // Input was from numeric keypad (a.k.a. keyboard)
    USERINP_BUTTON,     // Input was from touch-pad button
    USERINP_EXTKEY,     // Input was from external keyboard
    USERINP_TRACK1,     // Input was from track 1 of the magnetic stripe reader
    USERINP_TRACK2,     // Input was from track 2 of the magnetic stripe reader
    USERINP_TRACK3,     // Input was from track 3 of the magnetic stripe reader
    USERINP_SIGCAP      // Input was from the signature capture area
} USERINP_TYPE;

//! \brief	System date and time definition structure.
typedef struct _SysDT
{                                                      
	char year[2];	///< Year ('00'..'99')               
	char month[2];	///< Month ('01'..'12')               
	char day[2];	///< Day ('01'..'31')               
	char hour[2];	///< Hour ('00'..'23')               
	char min[2];	///< Minute ('00'..'59')               
	char sec[2];	///< Second ('00'..'59')               
	char mode;	///< 'A' (am), 'P' (pm) or ' '(24-hour) 
	char dow;	///< Day of the Week ('0'..'6')         
} SysDT;                                                     

//
// Printer support ------------- start
// Printer ID values (see structure member "id")
//	The values from 5 on map the same as the TMP_* values in sysstruc.inc.
//                                               COLUMNS  ROLL  BIDIRECTIONAL
#define	PR_HC212	0	/* HYPERCOM E212    P7      23    No    No				*/
#define	PR_HC215	1	/* HYPERCOM E215    P7     23/31  No    Yes				*/
#define	PR_HC211	2	/* HYPERCOM E211    P7     33/44  Yes   No				*/
#define	PR_VF250	3	/* VERIFONE 200             40    Yes   ?				*/
#define	PR_HC192	4	/* HYPERCOM E192    T7P     40    Yes   No				*/
#define	PR_HC190	5	/* HYPERCOM E190    T7P     24    Yes   No				*/
#define	PR_ST212	6	/* HYPERCOM S212    P8     35/46  Yes   Yes				*/
#define	PR_ST221	7	/* HYPERCOM S221    P8     23/31  No    Yes				*/
// #define			8	/* HYPERCOM S212G   P8     35/46  Yes   Yes   Graphics	*/
// #define			9	/* HYPERCOM S221G   P8     23/32  No    Yes   Graphics	*/
#define	PR_MT101	10	/* HYPERCOM MT101   T7P     42    Yes   No    Thermal	*/
#define	PR_MT301	11	/* HYPERCOM MT301   T77     44    Yes   No    Thermal	*/
#define	PR_EPL1920	12	/* HYPERCOM EPL1920 ICE     44    Yes   N/A   Thermal	*/
#define	PR_EPL205	13	/* HYPERCOM EPL205 SmartICE 42    Yes   N/A   Thermal	*/
#define PR_FTP628   14  /* HYPERCOM FTP628 5500+/T7+ 42   Yes   N/A   Thermal   */
#define PR_FTP638   16  /* HYPERCOM FTP638  5500+/T7+ 42  No    Yes   Thermal   */
#define PR_U110		17	// HYPERCOM U110    T77ET/T6600
#define PR_LTPJ245	18	// M2

// Flag bits (see structure member "optionByte")
#define PR1_PRT		0x01	/* Serial printer									*/
#define	PR1_ROLL	0x02	/* Friction feed printer							*/
#define PR1_THERM	0x04	/* Thermal printer									*/
#define	PR1_CUT		0x08	/* Printer has a cutter (thermal only)				*/
#define	PR1_LF		0x10	/* Printer automatically does LF when line is full	*/

//! \brief	Printer capability record.
typedef struct
{
	unsigned char	id;		// Printer ID number
	unsigned char	mode;		// Printer mode
	unsigned char	widthNormal;	// Width of normal font (chars per line)
	unsigned char	widthCondensed;	// Width of condensed font (chars per line)
	unsigned char	widthDefault;	// Default width (chars per line)
	unsigned char	optionByte1;	// Option bit flags (see PR1_* defines)
} PRINTERDATA;

// Flag bits used in the system variables LPTFLG and PRTFLG
#define PRTR_OPEN 0x80		/* The printer device is open */

// Printer support ------------- end


// Magnetic Stripe Reader support ------- start

// These constants represent the maximum number of characters that
// will be returned in response to a card swipe.  Note that the OS
// strips off the STX, ETX, and LRC characters so these numbers are
// three less than the actual number of bytes stored on the card.
#define MAX_TRACK1_LEN       76
#define MAX_TRACK2_LEN       37
#define MAX_TRACK3_LEN      104

// Magnetic Stripe Reader support ------- end


//! \brief  Signature scaling structure.
typedef struct
{
    unsigned short  x_res;      // Scaled x-axis resolution (1..1024)
    unsigned short  y_res;      // Scaled y-axis resolution (1..1024)
    unsigned char x_limit;      // X-axis Extended Line Limit.
    unsigned char y_limit;      // Y-axis Extended Line Limit.
} SIGCAP_SCALING;


//=============================================================================
// Public data declarations
//=============================================================================


//=============================================================================
// Public function declarations
//=============================================================================


//!	\brief Sound the beeper.
//!
//!	This function sounds the beeper for the specified number of
//!	system clock ticks (10mS per tick).  Specifying a duration
//!	of 255 (0xFF) will turn the beeper on and leave it on.
//!	Specifying a duration of zero will turn the beeper off.
//!
//!	The calling process is blocked for the specified duration,
//!	unless the duration is 0xFF.  In that case, the beeper is
//!	turned on and the function returns immediately.
//!
//!	\param duration Number of system ticks to sound the beeper (10mS per
//!	                tick), or 0xFF to turn on the beeper, or zero to
//!	                turn off the beeper.
extern void OS_Beeper( unsigned char duration );


//!
//!	\brief Beep n times.
//!
//!	Each beep is 120mS long with 120mS of silence in between.
//!
//!	The calling process is blocked until all of the beeps have occurred.
//!
//!	\param n Number of times to beep.
//!
extern void OS_Beepit( unsigned char n );


//!
//!	\brief Cancel a timer.
//!
//!	\param tmrblk Pointer to a timer control block.
//!
extern void OS_CancelTimer( PTCS_BLK *tmrblk );


//!
//!	\brief Copy a data buffer from the caller's page.
//!
//!	\param pDest Pointer to destination address.
//!	\param pSource Pointer to source address.
//!	\param len Number of bytes to copy.
//!
#if defined(MAKE_FLAT)
#define CopyFromCallersPage(pDest,pSource,len) memcpy(pDest,pSource,len)
#else
extern void CopyFromCallersPage(void *pDest, const void *pSource, int len);
#endif


//!
//!	\brief Copy a data buffer to the caller's page.
//!
//!	\param pDest Pointer to destination address.
//!	\param pSource Pointer to source address.
//!	\param len Number of bytes to copy.
//!
#if defined(MAKE_FLAT)
#define CopyToCallersPage(pDest,pSource,len) do { memcpy(pDest,pSource,len); } while (0)
#else
extern void CopyToCallersPage(void *pDest, const void *pSource, int len);
#endif


//!
//!	\brief Call "Function".
//!
//!	\param cmd "Function" to execute.
//!	\param pBlk Pointer to control data block to use with this command.
//!
extern void OS_Function( unsigned char cmd, void *pBlk );


//!
//!	\brief Returns the physical page mask of the latest cross-page caller.
//!
//!	\retval Physical page mask of the latest cross-page caller.
//!	
//!	\note On Z80 global input XCALL_SP (cross-call stack pointer) is used
//!	      to determine the caller's page.
//!
extern unsigned char GetCallersPage( void );


//!
//!	\brief Returns the physical page mask for a given address.
//!
//!	\param pData Address of data buffer.
//!
//!	\retval If pData points to lower memory, this function returns the
//!	        physical page mask of the latest cross-page caller.  If pData
//!	        points to upper memory, zero is returned.
//!
extern unsigned char GetDataPage( void *pData );


//!
//!	\brief Send a close command to a device.
//!
//!	\param dev Device number (symbolic name is preferred).
//!
//!	\retval OSRETURNCODES.
//!
//!	\note The WAIT subcommand is used which blocks the calling process
//!	      until the close command completes.
//!
OSRETURNCODES OS_DeviceClose( UBYTE dev );


//!
//!	\brief Send a configuration command to a device.
//!
//!	\param dev Device number (symbolic name is preferred).
//!	\param mode The desired mode of operation for the device. This is a
//!	            device-dependent value.
//!	\param ddda Device-dependent data. For example, a timeout value or a
//!	            pointer to a data structure.
//!
//!	\retval OSRETURNCODES.
//!
//!	\note The WAIT subcommand is used which blocks the calling process
//!	      until the config command completes.
//!
OSRETURNCODES OS_DeviceConfig( UBYTE dev, UBYTE mode, IOCS_DDDA_DATA *ddda );


//!
//!	\brief Send an open command to a device.
//!
//!	\param dev Device number (symbolic name is preferred).
//!
//!	\retval OSRETURNCODES.
//!
//!	\note The WAIT subcommand is used which blocks the calling process
//!	      until the open command completes.
//!
OSRETURNCODES OS_DeviceOpen( UBYTE dev );


//!
//!	\brief Rewind device.
//!	
//!	Reset a device by cancelling all outstanding I/O and resetting the
//!	hardware (this is driver dependent).
//!
//!	\param dev Device number (symbolic name is preferred).
//!
//!	\retval OSRETURNCODES.
//!
//!	\note The WAIT subcommand is used which blocks the calling process
//!	      until the open command completes.
//!
OSRETURNCODES OS_DeviceRewind( UBYTE dev );



//!
//!	\brief Read from a device.
//!
//!	\param dev Device number (symbolic name is preferred).
//!	\param start Address of the first byte of a user buffer. The byte at
//!	             this address will receive the first byte read from the
//!	             device.
//!	\param len Pointer to the length in bytes of the user buffer on input,
//!	           actual size of data read on exit.
//!
//!	\retval OSRETURNCODES.
//!
//!	\note The WAIT subcommand is used which blocks the calling process
//!	      until the open command completes.
//!
OSRETURNCODES OS_DeviceRead( UBYTE dev, void *start, UWORD *len );


//!
//!	\brief Write to a device.
//!
//!	\param dev Device number (symbolic name is preferred).
//!	\param start Address of the first byte of a user buffer. The byte at
//!	             this address will be the first byte written to the device.
//!	\param len Length of user buffer.
//!
//!	\retval OSRETURNCODES.
//!
//!	\note The WAIT subcommand is used which blocks the calling process
//!	      until the open command completes.
//!
OSRETURNCODES OS_DeviceWrite( UBYTE dev, void *start, UWORD len );


//!
//!	\brief Memory copy from page (logical).
//!
//!	\param dst Pointer to destination buffer (on current page or upper memory).
//!	\param srcp Source page (logical page number).
//!	\param src Pointer to source buffer (on source page).
//!	\param len Number of bytes to copy.
//!
#if defined(MAKE_FLAT)
#define memcpyfr(dst,srcp,src,len) do { (void)srcp; memcpy(dst,src,len); } while(0)
#else
extern void *memcpyfr( void *dst, int srcp, const void *src, size_t len );
#endif


//!
//!	\brief Memory copy from page (direct).
//!
//!	\param dst Pointer to destination buffer (on current page or upper memory)
//!	\param srcp Source page (physical page mask).
//!	\param src Pointer to source buffer (on source page).
//!	\param len Number of bytes to copy.
//!
#if defined(MAKE_FLAT)
#define memcpyfrd(dst,srcp,src,len)  do { (void)srcp; memcpy(dst,src,len); } while(0)
#else
extern void *memcpyfrd( void *dst, int srcp, const void *src, size_t len );
#endif


//!
//!	\brief Memory copy to page (logical).
//!
//!	\param dstp Destination page (logical page number).
//!	\param dst Pointer to destination buffer (on destination page).
//!	\param src Pointer to source buffer (on current page or upper memory).
//!	\param len Number of bytes to copy.
//!
#if defined(MAKE_FLAT)
#define memcpyto(dstp,dst,src,len)  do { (void)dstp; memcpy(dst,src,len); } while(0)
#else
extern void *memcpyto( int dstp, void *dst, const void *src, size_t len );
#endif


//!
//!	\brief Memory copy to page (direct).
//!
//!	\param dstp Destination page (physical page mask).
//!	\param dst Pointer to destination buffer (on destination page).
//!	\param src Pointer to source buffer (on current page or upper memory).
//!	\param len Number of bytes to copy.
//!
#if defined(MAKE_FLAT)
#define memcpytod(dstp,dst,src,len)  do { (void)dstp; memcpy(dst,src,len); } while(0)
#else
extern void *memcpytod( int dstp, void *dst, const void *src, size_t len );
#endif


//!
//!	\brief Sends given data buffer to printer.
//!
//!	\param pBuffer Pointer to data to print (may contain NULLs).
//!	\param buflen Length of buffer to print.
//!
//!	\retval OSRETURNCODES.
//!
//!	\note Returns after all of the data has been passed to the OS. The OS
//!	      buffers the data, so it may still be printing. There is no need
//!	      to open the printer first, because this code will automatically
//!	      open it if necessary.
//!
extern OSRETURNCODES PrintBuffer( char* pBuffer, UWORD buflen );


//!
//!	\brief Close the printer device.
//!
//!	\retval OSRETURNCODES.
//!
//!	\note This function takes two global inputs:
//!	      @arg LPTFLG - Current status of the LPT device.
//!	      @arg PRTFLG - Current status of the PRT device.
//!
extern OSRETURNCODES PrinterClose( void );


//!
//!	\brief Get the current printer's parameters.
//!
//!	\retval Pointer to a structure of parameters for the current printer.<br>
//!	        NULL if no match found.
//!
//!	\note This function takes global input:
//!	      @arg TERM - Terminal configuration table set up in boot program.
//!
extern PRINTERDATA *PrinterParameters( void );


//!
//!	\brief Opens and configures printer.
//!
//!	\retval OSRETURNCODES.
//!
//!	\note This function takes two global inputs:
//!	      @arg LPTFLG - Current status of the LPT device.
//!	      @arg PRTFLG - Current status of the PRT device.
//!
extern OSRETURNCODES PrinterOpen( void );


//!
//!	\brief Sends given null terminated string to printer.
//!
//!	\param pString Pointer to the null terminated string to print.
//!
//!	\retval OSRETURNCODES.
//!
//!	\note Returns after all of the data has been passed to the OS. The
//!	      OS buffers the data, so it may still be printing. There is no
//!	      need to open the printer first, because this code will
//!	      automatically open it if necessary.
//!
extern UBYTE PrintString( char* pString );


//!
//!	\brief Read the hardware real-time clock (RTC).
//!
//!	\param pDT Pointer to RTC structure.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES OS_ReadRTC( SysDT *pDT );


//!
//!	\brief Write the hardware real-time clock (RTC).
//!
//!   This function writes data from the specified buffer to the
//!   hardware real-time clock (RTC).
//!
//!	\param pDT Pointer to RTC structure.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES OS_WriteRTC( SysDT *pDT );


//!
//!	\brief Convert system date/time to calendar time in seconds.
//!
//!	\param pDT Pointer to RTC structure.
//!
//!	\retval time_t corresponding to RTC structure.
//!
extern time_t RTCToTime( SysDT *pDT );


//!
//!	\brief String copy between pages.
//!
//!	This function is functionally similar to the standard C library
//!	function strcmp().  The difference is that this function works
//!	with strings which may reside on memory pages other than the
//!	current page.
//!
//!	\param pg1 Memory page on which s1 resides.
//!	\param s1 Pointer to first null-terminated string.
//!	\param pg2 Memory page on which s2 resides.
//!	\param s2 Pointer to second null-terminated string.
//!
#if defined(MAKE_FLAT)
#define strcpyfr(pg1,s1,pg2,s2)  do { (void)pg1; (void)pg2; strcpy(s1,s2); } while(0)
#else
extern int strcmpfr( unsigned char pg1, const char * s1, unsigned char pg2, const char * s2 );
#endif


//!
//!	\brief Copy counted string from page (logical).
//!
//!	\param dst Pointer to destination buffer (on current page or upper memory).
//!	\param srcp Source page (logical page number).
//!	\param src Pointer to source string (on source page).
//!	\param n Number of bytes to copy.
//!
//!	\note This function copies not more than 'n' characters from the 'src'
//!	      string on page 'srcp' to the buffer 'dst'.  Characters beyond the NUL
//!	      terminator of 'src' may be read but they will not be copied to 'dst'.
//!	      If the length of 'src' is less than 'n' bytes, 'dst' is padded with NULs.
//!
#if defined(MAKE_FLAT)
#define strncpyfr(dst,srcp,src,n) do { (void)srcp; strncpy(dst,src,n); } while(0)
#else
extern char *strncpyfr( char *dst, unsigned char srcp, const char *src, size_t n );
#endif


//!
//!	\brief Copy counted string from page (direct).
//!
//!	\param dst Pointer to destination buffer (on current page or upper memory).
//!	\param srcp Source page (physical page mask).
//!	\param src Pointer to source string (on source page).
//!	\param n Number of bytes to copy.
//!
//!	\note This function copies not more than 'n' characters from the 'src'
//!	      string on page 'srcp' to the buffer 'dst'.  Characters beyond the NUL
//!	      terminator of 'src' may be read but they will not be copied to 'dst'.
//!	      If the length of 'src' is less than 'n' bytes, 'dst' is padded with NULs.
//!
#if defined(MAKE_FLAT)
#define strncpyfrd(dst,srcp,src,n)  do { (void)srcp; strncpy(dst,src,n); } while(0)
#else
extern char *strncpyfrd( char *dst, unsigned char srcp, const char *src, size_t n );
#endif


//!
//!	\brief Convert calendar time in seconds to system date/time.
//!
//!	\param pDT Pointer to RTC structure.
//!	\param t Calendar time in time_t format.
//!
extern void TimeToRTC( SysDT *pDT, time_t t );


//!
//!	\brief Relinquish CPU time slice.
//!
extern void OS_RelinqCPU( void );


//!
//!	\brief This function returns the ID of the current process.
//!
//!	\retval The current process ID.
//!
extern unsigned char OS_GetPID( void );


//!
//!	\brief Set timer.
//!
//!	\param tmrblk Pointer to timer control block.
//!	\param tm Time to wait before interrupt.
//!	\param func Pointer to ISR function to call when timer expires.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES OS_SetTimerISR( PTCS_BLK *tmrblk, unsigned short tm, void (*func)(void) );


//!
//!	\brief Suspend the task to the given time.
//!
//!	This function suspends the calling process for the specified amount of
//!	time, specified in system clock ticks.
//!
//!	\param ticks Number of system ticks to wait (each tick is 10mS).
//!
extern void OS_Wait( unsigned short ticks );


//-------------------------------------
// User input functions
//-------------------------------------

//!
//!	\brief Check for input from any one of the standard input devices.
//!
//!	Check for input from any one of the standard input devices: the numeric
//!	keypad (a.k.a. keyboard) on the front of the ICE terminal, a button on
//!	the touch-pad screen, or the external PC-type keyboard (if present).
//!
//!	Note that this function will block the calling process if the user
//!	presses and holds down the pen. In other words, after a pen down event
//!	occurs, this function will not return until the subsequent pen up event
//!	occurs. This is needed to ensure the button table remains intact while
//!	the user is interacting with the objects on the screen.
//!
//!	\param pType Where to store the type (or source) of user input. This
//!	             pointer may be NULL, in which case the type of input is
//!	             not returned.
//!
//!	\retval The key or button ID, or zero if no input is available.
//!
//!	\note See keys.h header file for the list of key values.
//!
extern char OS_CheckStandardInput( USERINP_TYPE * pType );

//!
//!	\brief Checks for input from any one of user input devices.
//!	
//!   This function checks for input from any one of the many user input
//!   devices on the terminal.<dl>
//!	<dt>Standard Input Devices</dt>
//!	    <dd>The standard input devices include the numeric keypad (a.k.a.
//!	        keyboard) on the front of the ICE terminal, a button on the
//!	        touch-pad screen, or the external PC-type keyboard (if present).
//!
//!	        The OS_CheckStandardInput() function is called to check the
//!	        status of these devices.  Note that this function will block
//!	        the calling process if the user presses and holds down the pen.
//!	        In other words, after a pen down event occurs, this function
//!	        will not return until the subsequent pen up event occurs.  This
//!	        is needed to ensure the button table remains intact while the
//!	        user is interacting with the objects on the screen.</dd>
//!	<dt>Magnetic Stripe Reader</dt>
//!	    <dd>Data can be obtained from any of the three tracks of the
//!	        magnetic stripe reader.  Initially, all three tracks are
//!	        disabled and card swipes are ignored by this function.
//!	        When an application needs card data, it must enable the
//!	        desired track(s) individually by calling the appropriate
//!	        function (OS_EnableTrack1() for example).  After that, a
//!	        card swipe will generate one event for each of the enabled
//!	        tracks (USERINP_TRACK1 for example).  In response to the
//!	        event, the application must call the corresponding data
//!	        conversion function (OS_GetTrack1Data() for example) to
//!	        actually obtain the data.  When card data is no longer needed,
//!	        the application should disable the track(s) by calling the
//!	        appropriate function (OS_DisableTrack1() for example) in order
//!	        to minimize power consumption on battery-operated terminals.</dd>
//!	<dt>Signature Capture</dt>
//!	    <dd>Initially, signature capture mode is disabled.  When an
//!	        application needs to capture a signature, it must call
//!	        OS_EnableSigCap() with the appropriate parameters.  It is
//!	        recommended that while signature capture mode is enabled,
//!	        the application make available an "Enter" and a "Clear"
//!	        button.  If the user presses the "Clear" button, the
//!	        application should call OS_DisableSigCap() and then
//!	        OS_EnableSigCap() to initiate a new signature capture.
//!	        If the user presses the "Enter" key, the application
//!	        should call OS_DisableSigCap() to disable signature
//!	        capture.
//!
//!	        While signature capture mode is enabled, OS_CheckUserInput()
//!	        will monitor the status of the signature input and report
//!	        any error conditions.  The USERINP_SIGCAP input type will
//!	        only be returned by this function if an error occurred with
//!	        signature capture; under normal circumstances a USERINP_SIGCAP
//!	        event will not be returned.</dd>
//!	</dl>
//!
//!	\param pType Where to store the type (or source) of user input.  This
//!	             pointer may be NULL, in which case the type of input is
//!	             not returned.
//!
//!	\retval The value returned depends on the type of user input as shown
//!	        in the following table: @include OS_CheckUserInput.txt
//!
//!	\note See keys.h header file for the list of key values.
//!
extern char OS_CheckUserInput( USERINP_TYPE * pType );

//!
//!	\brief Close the user input devices.
//!
//!	\see OS_InitUserInput.
//!
extern void OS_CloseUserInput( void );

//!
//!	\brief Wait for input.
//!
//!	This function waits for input from any one of the many user input
//!	devices on the terminal, or until a specified amount of time elapses.
//!
//!	This function simply calls OS_CheckUserInput() repeatedly until input
//!	becomes available or until the specified timeout expires. The time
//!	slice for the calling process is relinquished after each call to
//!	OS_CheckUserInput() if no input is available.
//!
//!	\param pType Where to store the type (or source) of user input. This
//!	             pointer may be NULL, in which case the type of input is
//!	             not returned.
//!	\param secs Number of seconds to wait before timing out (zero to wait
//!	            forever).
//!
//!	\retval See OS_CheckUserInput() for a description of the return value.
//!	        If a timeout occurs, zero is returned.
//!
//!	\see OS_CheckUserInput.
//!
extern char OS_GetUserInput( USERINP_TYPE * pType, unsigned char secs );

//!
//!	\brief Initialize the standard input devices.
//!
//!	Initialize the standard input devices: the numeric keypad (a.k.a.
//!	keyboard) on the front of the ICE terminal, the touch-pad, and
//!	the external PC-type keyboard (if present).
//!
//!	Any other device supported by OS_CheckUserInput() is disabled.
//!
extern void OS_InitUserInput( void );

//!
//!	\brief Enable track 1 of the magnetic stripe reader.
//!
//!	This function enables track 1 of the magnetic stripe reader for
//!	use with the user input functions.  After this function is called,
//!	a USERINP_TRACK3 event will be returned by OS_GetUserInput() or
//!	OS_CheckUserInput() whenever the user swipes a card on the terminal.
//!
//!	In response to a USERINP_TRACK1 event, call OS_GetTrack1Data() to
//!	get the track 1 data.
//!
//!	When track 1 data is no longer needed, the device should be turned
//!	off by calling OS_DisableTrack1() in order to save power on battery-
//!	operated terminals.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES OS_EnableTrack1( void );

//!
//!	\brief Disable track 1 of the magnetic stripe reader.
//!
//!	This function disables track 1 of the magnetic stripe reader.
//!
//!	\retval OSRETURNCODES.
//!
//!	\see OS_EnableTrack1.
//!
extern OSRETURNCODES OS_DisableTrack1( void );

//!
//!	\brief Get track 1 data.
//!
//!	This function copies the track 1 data from the most recent card swipe
//!	into the specified buffer.  The data is converted to ASCII, but no
//!	parsing is performed.
//!
//!	Once the user swipes a card, the track 1 reader is disabled until the
//!	data is retrieved by calling this function. This function then
//!	re-enables the track 1 reader, thereby allowing subsequent card swipes
//!	to generate additional USERINP_TRACK1 events.
//!
//!	This function must only be called after OS_GetUserInput() or
//!	OS_CheckUserInput() returns a USERINP_TRACK1 input type and it must
//!	only be called once per each card swipe event. Calling it at any other
//!	time will result in an ERRINV error code.
//!
//!	\param pBuf Pointer to the buffer which is to receive the data. The size
//!	            of the buffer should be at least MAX_TRACK1_LEN bytes in
//!	            order to ensure that it can receive all of the track 3 data.
//!	\param pBufLen Pointer to the buffer length. This is both an input and
//!	               an output parameter. On input, it must point to a variable
//!	               containing the size of the buffer pointed to by pBuf (i.e.
//!	               the maximum number of characters to be written into the
//!	               buffer). On output, the variable pointed to by pBufLen is
//!	               changed to indicate the actual number of characters
//!	               written into the buffer.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES OS_GetTrack1Data( char *pBuf, unsigned short *pBufLen );

//!
//!	\brief Enable track 2 of the magnetic stripe reader.
//!
//!	This function enables track 2 of the magnetic stripe reader for
//!	use with the user input functions.  After this function is called,
//!	a USERINP_TRACK2 event will be returned by OS_GetUserInput() or
//!	OS_CheckUserInput() whenever the user swipes a card on the terminal.
//!
//!	In response to a USERINP_TRACK2 event, call OS_GetTrack2Data() to
//!	get the track 2 data.
//!
//!	When track 2 data is no longer needed, the device should be turned
//!	off by calling OS_DisableTrack2() in order to save power on battery-
//!	operated terminals.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES OS_EnableTrack2( void );

//!
//!	\brief Disable track 2 of the magnetic stripe reader.
//!
//!	This function disables track 2 of the magnetic stripe reader.
//!
//!	\retval OSRETURNCODES.
//!
//!	\see OS_EnableTrack2.
//!
extern OSRETURNCODES OS_DisableTrack2( void );

//!
//!	\brief Get track 2 data.
//!
//!	This function copies the track 2 data from the most recent card swipe
//!	into the specified buffer.  The data is converted to ASCII, but no
//!	parsing is performed.
//!
//!	Once the user swipes a card, the track 2 reader is disabled until the
//!	data is retrieved by calling this function. This function then
//!	re-enables the track 2 reader, thereby allowing subsequent card swipes
//!	to generate additional USERINP_TRACK2 events.
//!
//!	This function must only be called after OS_GetUserInput() or
//!	OS_CheckUserInput() returns a USERINP_TRACK2 input type and it must
//!	only be called once per each card swipe event. Calling it at any other
//!	time will result in an ERRINV error code.
//!
//!	\param pBuf Pointer to the buffer which is to receive the data. The size
//!	            of the buffer should be at least MAX_TRACK2_LEN bytes in
//!	            order to ensure that it can receive all of the track 3 data.
//!	\param pBufLen Pointer to the buffer length. This is both an input and
//!	               an output parameter. On input, it must point to a variable
//!	               containing the size of the buffer pointed to by pBuf (i.e.
//!	               the maximum number of characters to be written into the
//!	               buffer). On output, the variable pointed to by pBufLen is
//!	               changed to indicate the actual number of characters
//!	               written into the buffer.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES OS_GetTrack2Data( char *pBuf, unsigned short *pBufLen );

//!
//!	\brief Enable track 3 of the magnetic stripe reader.
//!
//!	This function enables track 3 of the magnetic stripe reader for
//!	use with the user input functions.  After this function is called,
//!	a USERINP_TRACK3 event will be returned by OS_GetUserInput() or
//!	OS_CheckUserInput() whenever the user swipes a card on the terminal.
//!
//!	In response to a USERINP_TRACK3 event, call OS_GetTrack3Data() to
//!	get the track 3 data.
//!
//!	When track 3 data is no longer needed, the device should be turned
//!	off by calling OS_DisableTrack3() in order to save power on battery-
//!	operated terminals.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES OS_EnableTrack3( void );

//!
//!	\brief Disable track 3 of the magnetic stripe reader.
//!
//!	This function disables track 3 of the magnetic stripe reader.
//!
//!	\retval OSRETURNCODES.
//!
//!	\see OS_EnableTrack3.
//!
extern OSRETURNCODES OS_DisableTrack3( void );

//!
//!	\brief Get track 3 data.
//!
//!	This function copies the track 3 data from the most recent card swipe
//!	into the specified buffer.  The data is converted to ASCII, but no
//!	parsing is performed.
//!
//!	Once the user swipes a card, the track 3 reader is disabled until the
//!	data is retrieved by calling this function. This function then
//!	re-enables the track 3 reader, thereby allowing subsequent card swipes
//!	to generate additional USERINP_TRACK3 events.
//!
//!	This function must only be called after OS_GetUserInput() or
//!	OS_CheckUserInput() returns a USERINP_TRACK3 input type and it must
//!	only be called once per each card swipe event. Calling it at any other
//!	time will result in an ERRINV error code.
//!
//!	\param pBuf Pointer to the buffer which is to receive the data. The size
//!	            of the buffer should be at least MAX_TRACK3_LEN bytes in
//!	            order to ensure that it can receive all of the track 3 data.
//!	\param pBufLen Pointer to the buffer length. This is both an input and
//!	               an output parameter. On input, it must point to a variable
//!	               containing the size of the buffer pointed to by pBuf (i.e.
//!	               the maximum number of characters to be written into the
//!	               buffer). On output, the variable pointed to by pBufLen is
//!	               changed to indicate the actual number of characters
//!	               written into the buffer.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES OS_GetTrack3Data( char *pBuf, unsigned short *pBufLen );

#if INCLPAD

//!
//!	\brief Enable signature capture mode.
//!
//!	This function enables signature capture mode if it's not already enabled.
//!	Signature capture mode allows the user to sign his or her name directly
//!	on the screen and the resulting graphics data can be stored on the
//!	terminal and/or uploaded to a host.
//!
//!	Signature capture data format is described in the paddev.inc system header.
//!
//!	\param pRect Pointer to the screen area to be used for signature
//!	             capture (absolute screen coordinates).
//!	\param pScaling Pointer to desired scaling, or NULL to use default
//!	                scaling.  See the description of the SIGCAP_SCALING
//!	                structure for more information.
//!	\param pBuf Pointer to buffer in which to store the signature data
//!	            (A buffer of at least 512 bytes is recommended).
//!	\param bufSize Size (in bytes) of the buffer pointed to by pBuf.
//!
//!	\retval OSRETURNCODES.
//!
//!	\see OS_CheckUserInput.
//!
extern OSRETURNCODES OS_EnableSigCap( const RECT * pRect,
                                      const SIGCAP_SCALING * pScaling,
                                      void * pBuf, unsigned short bufSize
                                    );

//!
//!	\brief Disable signature capture mode.
//!
//!	This function disables signature capture mode if it's presently enabled.
//!	Any signature data that has been captured is left unchanged in the buffer
//!	that was specified by the application in the previous call to
//!	OS_EnableSigCap().
//!
//!	\param pSigSize The variable pointed to by this parameter will be
//!	                set to the size of the signature data, i.e. the
//!	                number of bytes written to the application buffer
//!	                which was specified in the previous call to
//!	                OS_EnableSigCap().
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES OS_DisableSigCap( unsigned short * pSigSize );

#endif // INCLPAD

//-------------------------------------
// Device functions
//-------------------------------------

//!
//!	\brief Issue a read to a device.
//!
//!	\param dev Device number (symbolic name is preferred).
//!	\param pIOCS Address of IOCS block to be used for read NULL may be passed if
//!	             subCmd is WAIT. In that case a local IOCS block will be used.
//!	\param subCmd IOCS subcommand (WAIT, PROCEED+FLAG, PROCEED+INT).
//!	\param start Address of the first byte of a user buffer. The byte at this
//!	             address will be the first byte read from the device.
//!	\param len Byte length of user buffer.
//!	\param isrFlag Address of either Interrupt service routine (PROCEED+INT) or
//!	               flag with bit bitno to be set (PROCEED+FLAG).
//!	\param bitno Bit to be set in PROCEED+FLAG mode.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES DeviceRead( UBYTE dev, IOCS_BLK *pIOCS, UBYTE subCmd, 
                                 void *start, UWORD len, void *isrFlag, UBYTE bitno );

//!
//!	\brief Send an open command to a device.
//!
//!	\param dev Device number (symbolic name is preferred).
//!	\param pIOCS Address of IOCS block to be used for read NULL may be passed
//!	             if subCmd is WAIT. In that case a local IOCS block will be used.
//!	\param subCmd IOCS subcommand (WAIT, PROCEED+FLAG, PROCEED+INT).
//!	\param isrFlag Address of either Interrupt service routine (PROCEED+INT) or
//!	               flag with bit bitno to be set (PROCEED+FLAG).
//!	\param bitno Bit to be set in PROCEED+FLAG mode.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES DeviceOpen( UBYTE dev, IOCS_BLK *pIOCS, UBYTE subCmd,
                                 void *isrFlag, UBYTE bitno );

//!
//!	\brief Send a close command to a device.
//!
//!	\param dev Device number (symbolic name is preferred).
//!	\param pIOCS Address of IOCS block to be used for read NULL may be passed
//!	             if subCmd is WAIT. In that case a local IOCS block will be used.
//!	\param subCmd IOCS subcommand (WAIT, PROCEED+FLAG, PROCEED+INT).
//!	\param isrFlag Address of either Interrupt service routine (PROCEED+INT) or
//!	               flag with bit bitno to be set (PROCEED+FLAG).
//!	\param bitno Bit to be set in PROCEED+FLAG mode.
//!
//!	\retval OSRETURNCODES.
//!
//!	\note The WAIT subcommand is used which blocks the calling process until the
//!	      close command completes.
//!
extern OSRETURNCODES DeviceClose( UBYTE dev, IOCS_BLK *pIOCS, UBYTE subCmd,
                                  void *isrFlag, UBYTE bitno );

//!
//!	\brief Send a rewind command to a device.
//!
//!	\param dev Device number (symbolic name is preferred).
//!	\param pIOCS Address of IOCS block to be used for read NULL may be passed
//!	             if subCmd is WAIT. In that case a local IOCS block will be used.
//!	\param subCmd IOCS subcommand (WAIT, PROCEED+FLAG, PROCEED+INT).
//!	\param isrFlag Address of either Interrupt service routine (PROCEED+INT) or
//!	               flag with bit bitno to be set (PROCEED+FLAG).
//!	\param bitno Bit to be set in PROCEED+FLAG mode.
//!
//!	\retval OSRETURNCODES.
//!
//!	\note The WAIT subcommand is used which blocks the calling process until the
//!	      close command completes.
//!
extern OSRETURNCODES DeviceRewind( UBYTE dev, IOCS_BLK *pIOCS, UBYTE subCmd,
                                   void *isrFlag, UBYTE bitno );

//!
//!	\brief Issue a write to a device.
//!
//!	\param dev Device number (symbolic name is preferred).
//!	\param pIOCS Address of IOCS block to be used for read NULL may be passed if
//!	             subCmd is WAIT. In that case a local IOCS block will be used.
//!	\param subCmd IOCS subcommand (WAIT, PROCEED+FLAG, PROCEED+INT).
//!	\param start Address of the first byte of a user buffer. The byte at this
//!	             address will be the first byte written to the device.
//!	\param len Byte length of user buffer.
//!	\param isrFlag Address of either Interrupt service routine (PROCEED+INT) or
//!	               flag with bit bitno to be set (PROCEED+FLAG).
//!	\param bitno Bit to be set in PROCEED+FLAG mode.
//!
//!	\retval OSRETURNCODES.
//!
extern OSRETURNCODES DeviceWrite( UBYTE dev, IOCS_BLK *pIOCS, UBYTE subCmd, 
                                  void *start, UWORD len, void *isrFlag, UBYTE bitno );

//!
//!	\brief Send a config command to a device.
//!
//!	\param dev Device number (symbolic name is preferred).
//!	\param pIOCS Address of IOCS block to be used for read NULL may be passed
//!	             if subCmd is WAIT. In that case a local IOCS block will be used.
//!	\param subCmd IOCS subcommand (WAIT, PROCEED+FLAG, PROCEED+INT).
//!	\param mode The desired mode of operation for the device. This is a
//!	            device-dependent value.
//!	\param ddda Device-dependent data. For example, a timeout value or a pointer
//!	            to a data structure.
//!	\param isrFlag Address of either Interrupt service routine (PROCEED+INT) or
//!	               flag with bit bitno to be set (PROCEED+FLAG).
//!	\param bitno Bit to be set in PROCEED+FLAG mode.
//!
//!	\retval OSRETURNCODES.
//!
//!	\note The WAIT subcommand is used which blocks the calling process until the
//!	      close command completes.
//!
extern OSRETURNCODES DeviceConfig( UBYTE dev, IOCS_BLK *pIOCS, UBYTE subCmd, UBYTE mode, 
                                   IOCS_DDDA_DATA *ddda, void *isrFlag, UBYTE bitno );

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif	// _osclib_H_
