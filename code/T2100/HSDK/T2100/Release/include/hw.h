//=============================================================================
//                       Hypercom Inc
//                      (c) Copyright 2002
//=============================================================================

#ifndef _HW_H_
#define _HW_H_

#include "eftos.h"
#include "drivdef.inc"
#include "transports.h"
#include "HypEErrors.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
	@defgroup GroupHW Hardware Library Public Interface
	@{
*/

/**
	Maximum number of processes supported by Hardware Library
*/
#define MAX_PROCESSES 64

/**
	Maximum number of timers supported by Hardware Library
*/
#define MAX_TIMERS 20

/**
	System date and time definition structure.
*/
typedef struct _HWSysDT
{                                                      
	char year[2];	///< Year ('00'..'99')               
	char month[2];	///< Month ('01'..'12')               
	char day[2];	///< Day ('01'..'31')               
	char hour[2];	///< Hour ('00'..'23')               
	char min[2];	///< Minute ('00'..'59')               
	char sec[2];	///< Second ('00'..'59')               
	char mode;	///< 'A' (am), 'P' (pm) or ' '(24-hour) 
	char dow;	///< Day of the Week ('0'..'6')         
} HWSysDT;                                                     

/**
	Device type definitions.
*/
typedef enum
{
	HW_PIN,		///< PIN device
	HW_PRT		///< PRT device
} HWRSDEV;

/**
	PPP mode definitions.

	@note Should be in sync with sdkio.h.
*/
typedef enum 
{
	HW_PPP_DISABLE,		///< Disabled.
	HW_PPP_CHAP_PAP,	///< CHAP or PAP, authentication optional.
	HW_PPP_PAP,		///< PAP, authentication optional.
	HW_PPP_CHAP,		///< CHAP, authentication optional.
	HW_PPP_NONE,		///< Authentication disabled.
	HW_PPP_AUTH_CHAP_PAP,	///< CHAP or PAP, authentication mandatory.
	HW_PPP_AUTH_PAP,	///< PAP, authentication mandatory.
	HW_PPP_AUTH_CHAP	///< CHAP, authentication mandatory.
} HWPPPMODE;

/**
	PPP configuration data.
*/
typedef struct
{
	HWPPPMODE mode;			///< PPP mode
	UBYTE login[16];		///< Login name
	UBYTE password[16];		///< Password
	unsigned int connectionTimeout;	///< Connection timeout
	unsigned int LCPTimeout;	///< LCP timeout
} HWPPPCONF;

/**
	BlueTooth configuration data.
*/
typedef struct
{
	const char *bt_addr;		///< BlueTooth address
	const char *bt_uuid;		///< BlueTooth UUID
	UBYTE bt_encr;			///< BlueTooth encryption
	UBYTE bt_auth;			///< BlueTooth authentication
} HWBTCONF;

/**
	TCP/IP destination data.
*/
typedef struct
{
	UDWORD address;			///< IP address
	UWORD port;			///< Destination port
	unsigned int timeout;		///< Connection timeout
} HWTCPIPDEST;

/**
	POS LAN connection mode.
*/
typedef enum
{
	HW_POS_PERMANENT=0,		///< Permanent connection
	HW_POS_ONDEMAND			///< On-demand connection
} HWCOMPOSMODE;

/**
	POS LAN destination data.
*/
typedef struct
{
	UBYTE TPDU[5];			///< POS TPDU
	HWCOMPOSMODE Mode;		///< POS LAN mode
	unsigned int Timeout;		///< Connection timeout
} HWPOSLANDEST;

/**
	Baud rates available for POS LAN.
*/
typedef enum
{
	HW_POS_4800=0,			///< Low speed LAN (4800)
	HW_POS_19200			///< High speed LAN (19200)
} HWPOSBAUDRATE;

/**
	@brief Initialize hardware library.

	Initialize timers, serial ports and random number generator.
*/
void HW_Start( void );

/**
	@brief Initialize communication module type.

	@param hwtype The hardware module type to use.

	@see HW_ResetCommModule
*/
void HW_InitCommModuleType( enum HypEModuleType hwtype );

/**
	@brief Reset communication module.

	Initialize communication module type and reset specified module.

	@param hwtype The hardware module type to use.

	@note On D2100 setting hwtype to HW_BLUETOOTH_EXT will not cause
	      module type change but rather BlueTooth module attached to serial
	      port will be reset.

	@note Dial backup module will be started in GSM mode.

	@see HW_InitCommModuleType, HW_StopCommModule
*/
void HW_ResetCommModule( enum HypEModuleType hwtype );

/**
	@brief Stop communication module.

	Stop specified module. After the module stopped it should be reset
	to start normal operation again.

	@param hwtype The hardware module type to stop.

	@note GSM units will be entered in Emergency Off mode, for the modems
	DTR signal will be deasserted.

	@note For the dial backup unit the currently active board is affected,
	      the other one is left untouched.

	@see HW_ResetCommModule
*/
void HW_StopCommModule( enum HypEModuleType hwtype );

/**
	@brief Switch dial backup module to GSM.
*/
void HW_DBSwitchToGSM( void );

/**
	@brief Switch dial backup module to analog modem.

	@return 0 if dial backup is in communication disabled state.<br>
	        1 if dial backup was switched to analog modem successfully.
*/
int HW_DBSwitchToAnalog( void );

/**
	@brief Convert TermMaster phone number.

	Convert TermMaster phone number to Hayes dial string.

	0-9	Dial digits as specified
	A	Select dial mode as DTMF
	B	Dial tone pause
	C	Select pulse
	D	Skip PABX
	E	Extended DTMF
		The next digit is:
		0 - +
		A - *
		B - #
		C - A
		D - B
		E - C
		F - D
	F	Terminator

	@param phoneStr Sting to be converted (conversion is done in-place).

	@return 0 if no prefix must be inserted before number.<br>
	        1 if PABX prefix must be inserted before number.
*/
int HW_ConvertNum( char *phoneStr );

/**
	@brief Convert PABX prefix number.

	Convert PABX prefix number to Hayes dial string.

	@param phoneStr Sting to be converted (conversion is done in-place).

	@return 0 if prefix must be prepended before phone number as is.<br>
	        1 if 'W' must be inserted between prefix and phone number.

	@see HW_ConvertPrefix
*/
int HW_ConvertPrefix( char *phoneStr );

/**
	@brief Reset terminal.

	Restart terminal.

	@note Not all hardware is reset during restarting.

	@note On T2100 and M2100 quickly load Application Manager or Open
	      Loader.

	@note Not recommended to use by application.
*/
void HW_ResetTerminal( void );

/**
	@brief Reboot terminal.

	Restart terminal and enter to Booter.

	@note Not recommended to use by application.
*/
void HW_RebootTerminal( void );

/**
	@brief Convert errors from HPOSFC to IOSC library.

	@param error HPOSFC error code.

	@return IOCS library error (see execdef.inc).
*/
unsigned char HW_ConvertError( enum HypETransportError error );

/**
	@brief Get process ID.

	@return Unique process ID.
*/
unsigned int HW_GetPid( void );

/**
	@brief Create new process.

	This function is only initializes data for the new process. The
	process starts living in the suspended state.

	@param pFunc Pointer to the process function.
	@param stackSize Stack size to allocate to the given process.

	@return Process ID if successfully created or (DWORD)-1 on error.

	@note If task function return from it's main loop the process will be
	      suspended.

	@see HW_ResumeTask, HW_TerminateTask
*/
DWORD HW_CreateTask( PROCPTR pFunc, DWORD stackSize );

/**
	@brief Run suspended process.

	Function starts suspended process and pass parameter to it.

	@param taskNo ID of the process to start.
	@param parameter Parameter to pass to the process function.

	@see HW_CreateTask, HW_TerminateTask
*/
void HW_ResumeTask( DWORD taskNo, DWORD *parameter );

/**
	@brief Terminate process.

	@param taskNo ID of the process to start.

	@note The process should be terminated from the same process that
	      started it.

	@see HW_CreateTask, HW_ResumeTask
*/
void HW_TerminateTask( DWORD taskNo );

/**
	@brief Relinquish CPU cycles to other processes.

	@note Due to EFTOS is non-preemptive OS it is mandatory to relinquish
	      CPU cycles with good latency in order to provide good
	      performance.
*/
void HW_Relinquish( void );

/**
	@brief Keyboard task.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_KeyboardTask( void );

/**
	@brief Generate sound tone for the given time.

	@param ms Time to generate sound tone.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_Beeper( long ms );

/**
	@brief Generate sound tone for the 200 ms.
*/
void HW_Beep( void );

/**
	@brief Get the active device associated with RS0 port.

	@return Device associated with EFTOS RS0 port.
*/
HWRSDEV HW_RS0GetDevice( void );

/**
	@brief Associate given device with RS0 port.

	This function is intended to specify whether RS0 will be acting as PIN
	or PRT device for the IOCS library. When the PIN is associated with RS0
	the PRT will be associated with RS2 and vice versa.

	@param dev Device to associate with RS0 port.

	@return	FALSE: One of the devices is busy.<br>
	        TRUE: Devices was swapped successfully.
*/
BOOL HW_RS0SetDevice( HWRSDEV dev );

/**
	@brief Open serial port.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_OpenSerialPort( void );

/**
	@brief Check if serial port open.

	@return	FALSE: Serial port is closed.<br>
	        TRUE: Serial port is opened.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
BOOL HW_IsSerialOpen( void );

/**
	@brief Close serial port.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CloseSerialPort( void );

/**
	@brief Reset serial port.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_ResetSerialPort( void );

/**
	@brief Write to serial port.

	@param data Pointer to the data to write.
	@param length Data length.

	@return true: Request successfully passed.<br>
	        false: Error during execution of request.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_WriteSerialPortReq( unsigned char * data, unsigned short length );

/**
	@brief Configure serial port.

	@param baud Baud rate for the serial port.
	@param formatSIO Combination of data bits, parity and stop bits to use.
	@param protocol Protocol that should be used on the serial port.

	@return true: Request successfully passed.<br>
	        false: Error during execution of request.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_ConfigureSerialPort( unsigned short baud, unsigned char formatSIO,
                            unsigned char protocol );

/**
	@brief Configure protocol flags on the serial port.

	@param flags Bit-mask for the protocol flags.

	@return true: Request successfully passed.<br>
	        false: Error during execution of request.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_ConfigureSerialPortProtocol( int flags );

/**
	@brief Open PIN port.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_OpenPINPort( void );

/**
	@brief Check if PIN port open.

	@return	FALSE: PIN port is closed.<br>
	        TRUE: PIN port is opened.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
BOOL HW_IsPINOpen( void );

/**
	@brief Close PIN port.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_ClosePINPort( void );

/**
	@brief Reset PIN port.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_ResetPINPort( void );

/**
	@brief Write to PIN port.

	@param data Pointer to the data to write.
	@param length Data length.

	@return true: Request successfully passed.<br>
	        false: Error during execution of request.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_WritePINPortReq( unsigned char * data , unsigned short length );

/**
	@brief Configure PIN port.

	@param baud Baud rate for the PIN port.
	@param formatSIO Combination of data bits, parity and stop bits to use.
	@param protocol Protocol that should be used on the PIN port.
	@param mode PIN mode (terminal or PIN PAD).

	@return true: Request successfully passed.<br>
	        false: Error during execution of request.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_ConfigurePINPort( unsigned short baud, unsigned char formatSIO,
                         unsigned char protocol,
                         unsigned char mode );

/**
	@brief Configure protocol flags on the PIN port.

	@param flags Bit-mask for the protocol flags.

	@return true: Request successfully passed.<br>
	        false: Error during execution of request.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_ConfigurePINPortOptions( int flags );

/**
	@brief Display bitmap image.

	@param x X coordinate of the top left corner.
	@param y Y coordinate of the top left corner.
	@param width Width of the image.
	@param height Height of the image.
	@param pBuf Pointer to the image buffer.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_PutBitmapBlock( unsigned short x, unsigned short y,
                        unsigned short width, unsigned short height,
                        void * pBuf );

/**
	@brief Delete all controls from the window area.

	Not applicable for Optimum A. Do nothing.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_DeleteAllControls( void );

/**
	@brief Set display contrast.

	@param level Contrast level.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_SetContrast( int level );

/**
	@brief Turn backlight on.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_BacklightOn( void );

/**
	@brief Turn backlight off.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_BacklightOff( void );

/**
	@brief Start screen capture server.

	This feature was primarly designed for the documentation writers to
	be able to capture terminal scripts and use them in customer
	documentation. STIM script ScreenCapture.vbs utilizing client side of
	the screen capture is shipped with Base Application.

	The logic is described in details in the "T2100 MA screen capture.doc". 

	@param port EFTOS port to start server on.

	@note Available in DEBUG build only for the terms of security.

	@see HW_StopCaptureServer
*/
int HW_StartCaptureServer( int port );

/**
	@brief Stop screen capture server.

	@note Available in DEBUG build only for the terms of security.

	@see HW_StartCaptureServer
*/
int HW_StopCaptureServer( void );

/**
	@brief Initialize generic semaphore API.

	Should be called at startup before using semaphores.

	@note The maximum number of semaphores is defined at compile-time. Once
	      generic semaphore is initialized with HW_Init_Semaphore() or
	      HW_ReleaseSemaphore() it cannot be deallocated from the list, so
	      generic semaphores should not be used in dynamic environment.

	@see HW_Init_Semaphore, HW_ReleaseSemaphore, HW_GetSemaphore.
*/
void HW_SemInitialize( void );

/**
	@brief Initialize semaphore.

	@param pSemaphore pointer to the semaphore location.

	@note If no more semaphores available the data under pointer is
	      filled with 0x55.

	@note If semaphore successfully released the data under pointer is
	      filled with 0x00.

	@see HW_SemInitialize, HW_ReleaseSemaphore, HW_GetSemaphore.
*/
void HW_Init_Semaphore( unsigned char *pSemaphore );

/**
	@brief Release semaphore.

	@param pSemaphore pointer to the semaphore location.

	@note If semaphore was not initialized it will be initialized.

	@note If no more semaphores available the data under pointer is
	      filled with 0x55.

	@note If semaphore successfully released the data under pointer is
	      filled with 0x01.

	@see HW_SemInitialize, HW_Init_Semaphore, HW_GetSemaphore.
*/
void HW_ReleaseSemaphore( unsigned char *pSemaphore );

/**
	@brief Release semaphore.

	Get the specified semaphore. If the semaphore is already in use, wait.
	This is not a real semaphore, as it cannot be used for process
	synchronization, just mutual exclusion (it only has two states:
	available and unavailable).

	@param pSemaphore pointer to the semaphore location.

	@note If semaphore was not initialized it will be initialized.

	@note If no more semaphores available the data under pointer is
	      filled with 0xaa.

	@note If semaphore successfully released the data under pointer is
	      filled with 0x00.

	@see HW_SemInitialize, HW_Init_Semaphore, HW_ReleaseSemaphore
*/
void HW_GetSemaphore( unsigned char *pSemaphore );

/**
	@brief Enable MSR track.

	@param track Track number to enable.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CardReaderEnableTrack( int track );

/**
	@brief Disable MSR track.

	@param track Track number to disable.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CardReaderDisableTrack( int track );

/**
	@brief Handle MSR track read event.

	@param track Track number data received from.
	@param pData Pointer to the received data or NULL if track read failed.
	@param dataLength Length of the received data.

	@note Used internally by hardware library. Should not be used by the
	      applications.
*/
void HWI_CardInputReceived( int track, unsigned char* pData,
                            unsigned int dataLength );

/**
	@brief Open communication port.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_OpenCommPort( void );

/**
	@brief Connect communication port.

	@param mode One of the COM_IOCSMODE enumerated values.
	@param pDialData Pointer to DIALDATA structure.
	@param noWait Not used.

	@return IOCS error code (see execdef.inc).

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
unsigned char HW_ConnectCommPort( int mode, DIALDATA *pDialData,
                                  unsigned char noWait );

/**
	@brief Disconnect communication port.

	@return IOCS error code (see execdef.inc).

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
unsigned char HW_DisconnectCommPort( void );

/**
	@brief Close communication port.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CloseCommPort( void );

/**
	@brief Reset communication port.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_ResetCommPort( void );

/**
	@brief Write to communication port.

	@param data Pointer to the data to write.
	@param length Length of the data.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_WriteCommPort( unsigned char *data , unsigned short length );

/**
	@brief Get communication port status.

	@return Current status as defined by COMSTATUS enumeration (see
	        drivdef.h).

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_GetCommStatus( void );

/**
	@brief Output trace collected on RS1 to RS2 port.

	@deprecated Not recommended for new design.
*/
void HW_OutputCommDebug( void ) __attribute__((deprecated));

/**
	@brief Output trace collected on RS1 to the given port.

	@param commNo Port to send trace to.

	@deprecated Not recommended for new design.
*/
void HW_OutputCommDebugN( int commNo ) __attribute__((deprecated));

/**
	@brief Parse Send & Match Strings.

	This function parses Send & Match Strings to extract NUA and saves NUA
	for use at next call. All other data is ignored.

	@param str Send & Match Strings in ICE format.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_SendMatchStr( const char *str );

/**
	@brief Set additional AT command for next call.

	@param ptr Addidional AT command to use on next call.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CommSetATCmd( const char *ptr );

/**
	@brief Set PAD logon for next call.

	@param ptr PAD logon to use on next call (chat script format).

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CommSetPADLogon( const char *ptr );

/**
	@brief Set serial mode for next call.

	@param mode Packet assembly mode.
	@param timeout Intercharacter timeout.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CommSetSerialMode( unsigned char mode, unsigned int timeout );

/**
	@brief Set PPP configuration for next call.

	@param pPPPConf PPP configuration to use on next call.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CommSetPPPConf( const HWPPPCONF *pPPPConf );

/**
	@brief Set BlueTooth configuration for next call.

	@param pBTConf BlueTooth configuration to use on next call.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CommSetBTConf( const HWBTCONF *pBTConf );

/**
	@brief Set 1st TCP/IP destination address for next call.

	@param pDest 1st TCP/IP destination address to use on next call.

	@note Used internally by IOCS library. Should not be used by the
	      applications.

	@see HW_CommSetTCPIPDest2
*/
void HW_CommSetTCPIPDest1( const HWTCPIPDEST *pDest );

/**
	@brief Set 2nd TCP/IP destination address for next call.

	@param pDest 2nd TCP/IP destination address to use on next call.

	@note Used internally by IOCS library. Should not be used by the
	      applications.

	@see HW_CommSetTCPIPDest1
*/
void HW_CommSetTCPIPDest2( const HWTCPIPDEST *pDest );

/**
	@brief Set local IP configuration.

	@param Address Local IP address.
	@param Mask Netmask.
	@param Gateway Default gateway.
	@param Parameters Bit-masked value. Less significant byte if set to 1
	       specify Ethernet transparent firmware mode (recommended),
	       otherwise reliable mode will be used (not recommended for new
	       design). Second byte if set to 0 indicates DHCP is on, otherwise
	       off.
*/
void HW_CommSetLocalIPConf( UDWORD Address, UDWORD Mask, UDWORD Gateway, UDWORD Parameters );

/**
	@brief Set hex length flag for next call.

	@param IsLengthRequired If non-zero hex length will be verified during
	                        packet receiption on next call (not all modem
	                        modes support this).

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CommSetHexLength( UBYTE IsLengthRequired );

/**
	@brief Set POS LAN destination address for next call.

	@param pDest POS LAN destination address to use on next call.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CommSetPOSLANDest( const HWPOSLANDEST *pDest );

/**
	@brief Set POS LAN configuration.

	@param Address CU address.
	@param BaudRate POS LAN baud rate.
	@param Timeout SNRM timeout.
*/
void HW_CommSetPOSLANConf( UBYTE Address, HWPOSBAUDRATE BaudRate, unsigned int Timeout );

/**
	@brief Set SSL configuration for next call.

	@param bEnable SSL enable flag.
	@param pCheckCertFunc Pointer to SSL certificate verification function.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CommSetSSLConf( UBYTE bEnable, int (*pCheckCertFunc)(void*) );

/**
	@brief Set country code for next call.

	@param code Country code (see Conexant modem AT command reference).

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_CommSetCountryCode( int code );

/**
	@brief Get communication line status.

	@return For T2100 Ethernet return HEMS tag 0x53 value.<br>
	        For P2100 Ethernet rerurn EFTOS Ethernet status.<br>
	        For V22 and V90 modems return non-zero if voltage on the line
	        is equal or above 19V.<br>
	        For GSM module return value returned by AT+CREG command.<br>
	        Not defined for other types of communication.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
unsigned char HW_GetLineStatus( void );

/**
	@brief Get communication line signal level.

	@return For V22 and V90 modems return integer part of the value
	        returned by AT-TRV command.<br>
	        For GSM module return value returned by AT+CSQ command.<br>
	        Not defined for other types of communication.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
unsigned char HW_GetSignalLevel( void );

/**
	@brief Get ring indicator state for the serial port.

	@param port The serial port to check RI state.

	@return 0: RI is inactive.<br>
	        1: RI is active.

	@note The real pin state is only returned for RS_1 port of T/M/D2100.
*/
int HW_GetRI( int port );

/**
	@brief Initialize printer variables.

	@note Should be called before use of the printer. Timers should be
	      already initialized before this call. Normally called from
	      InitDevices() in IOCS library.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_InitPrinter( void );

/**
	@brief Open printer.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_OpenPrinter( void );

/**
	@brief Set graphics mode.

	Activate graphical rendering engine.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_SetGraphMode( void );

/**
	@brief Check if printer is out of paper.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_IsPaperOut( void );

/**
	@brief Check if printer is overheated.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_IsPrinterOverheat( void );

/**
	@brief Close printer.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_ClosePrinter( void );

/**
	@brief Reset printer.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_ResetPrinter( void );

/**
	@brief Print data without processing.

	@param data Pointer to the data to print.
	@param length Length of data.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_PrintStart( const char *data, int length );

/**
	@brief Print data with preprocessing.

	@param data Pointer to the data to print.
	@param length Length of data.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_WritePrinter( const unsigned char *data, unsigned short length );

/**
	@brief Get printer firmware revision string.

	@return Printer firmware revision string.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
const char *HW_GetPrinterRevision( void );

/**
	@brief Print graphical data.

	@param data Pointer to the graphical data.
	@param length Length of data.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
int HW_PrintDotRow( const unsigned char *data, unsigned int length );

/**
	@brief Setup printer autoreset feature.

	@param flag If set to non-zero printer will be automatically reset on
	            error.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_SetPrinterAutoReset( int flag );

/**
	@brief Get printer firmware revision byte.

	@return Printer firmware revision as byte.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
BYTE HW_GetPrinterFirmwareRevision( void );

/**
	@brief Read real time clock.

	@param pSD Pointer to structure to place date and time information.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_ReadRTC( HWSysDT *pSD );

/**
	@brief Write real time clock.

	@param pSD Pointer to structure to get date and time information from.

	@note Used internally by IOCS library. Should not be used by the
	      applications.
*/
void HW_WriteRTC( HWSysDT *pSD );

/**
	@brief Initialize timer subsystem.

	Should be called at startup before using timers.
*/
void HW_InitTimers( void );

/**
	@brief Start timer.

	@param time Time in tens milliseconds to start timer.
	@param repeatTime Time to restart timer after it expired first time or
	                  zero to disable timer after expiration.
	@param callback Callback function to call after timer expired. This
	                function should do very simple action to not break
	                system. Normally, it should setup flag and that's it.
	@param arg Argument to pass to callback function on timer expiration.

	@return Timer ID or MAX_TIMERS if all timers are in use.

	@see HW_StopTimer
*/
unsigned int HW_StartTimer( unsigned int time, unsigned int repeatTime,
                            void ( *callback )( unsigned int ),
                            unsigned int arg );

/**
	@brief Stop timer.

	@param handle Timer ID returned by HW_StartTimer.

	@see HW_StartTimer
*/
void HW_StopTimer( unsigned int handle );

/**
	@brief Relinquish CPU for the given time.

	@param time Time to relinquish CPU on.
*/
void HW_Sleep( unsigned int time );

/**
	@brief Convert time from milliseconds.

	Convert time from milliseconds to time acceptable by HW_StartTimer.

	@param time Time in milliseconds.

	@return Time in form acceptable by HW_StartTimer.
*/
unsigned int HW_MsToTics( unsigned int time );

/**
	@brief Get OS name string.

	@return Pointer to OS name.
*/
const char* HW_OS_Name(void);

/**
	@brief Get OS version string.

	@return Pointer to OS version.
*/
const char* HW_OS_Version(void);

/**
	@brief Get platform name string.

	@return Pointer to platform name.
*/
const char* HW_HSDK_Platform(void);

/**
	@brief Get platform variant string.

	@return Pointer to platform variant.
*/
const char* HW_HSDK_Variant(void);

/**
	@brief Get HSDK version string.

	@return Pointer to HSDK version.
*/
const char* HW_HSDK_Version(void);

/**
	@brief Get HSDK build time.

	@return Pointer to HSDK build time.
*/
const char* HW_HSDK_BuildTime(void);

/**
	@brief Get Booter version.

	@return Pointer to Booter version.
*/
const char* HW_Booter_Version(void);

/**
	@}
*/

#ifdef __cplusplus
}
#endif

#endif  // _HW_H_

